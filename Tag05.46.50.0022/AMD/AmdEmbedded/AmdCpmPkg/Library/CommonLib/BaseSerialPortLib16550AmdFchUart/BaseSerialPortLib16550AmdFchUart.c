/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
//This file includes code originally published under the following license.

/** @file
  16550 UART Serial Port library functions

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/SerialPortLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformHookLib.h>
#include <Library/AmdIdsDebugPrintLib.h>

//
// 16550 UART register offsets and bitfields
//
#define R_UART_RXBUF          0
#define R_UART_TXBUF          0
#define R_UART_FCR            2
#define   B_UART_FCR_FIFOE    BIT0
#define   B_UART_FCR_FIFO64   BIT5
#define R_UART_MCR            4
#define   B_UART_MCR_RTS      BIT1
#define R_UART_LSR            5
#define   B_UART_LSR_RXRDY    BIT0
#define   B_UART_LSR_TXRDY    BIT5
#define   B_UART_LSR_TEMT     BIT6
#define R_UART_MSR            6
#define   B_UART_MSR_DSR      BIT5

/**
  Read an 8-bit 16550 register.  The parameter Offset is added to the base address of the
  16550 registers that is specified by PcdAmdIdsDebugPrintSerialPort.
  @param  Offset  The offset of the 16550 register to read.
  @return The value read from the 16550 register.
**/
UINT8
SerialPortReadRegister (
  UINTN  Offset
  )
{
  return MmioRead8 ((UINTN)PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + Offset * 4);
}

/**
  Write an 8-bit 16550 register. The parameter Offset is added to the base address of the
  16550 registers that is specified by PcdAmdIdsDebugPrintSerialPort.
  @param  Offset  The offset of the 16550 register to write.
  @param  Value   The value to write to the 16550 register specified by Offset.
  @return The value written to the 16550 register.
**/
UINT8
SerialPortWriteRegister (
  UINTN  Offset,
  UINT8  Value
  )
{
  return MmioWrite8 ((UINTN)PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + Offset * 4, Value);
}

/**
  Return whether the hardware flow control signal allows writing.

  @retval TRUE  The serial port is writable.
  @retval FALSE The serial port is not writable.
**/
BOOLEAN
SerialPortWritable (
  VOID
  )
{
  return AmdIdsDpSerialWritable();
}

BOOLEAN
CheckCableConnection (
  )
{
  UINT32   RetryCount;

  // Check Cable connection
  RetryCount = 200;
  if ((PcdGet64 (PcdAmdIdsDebugPrintSerialPort) < 0x10000) && PcdGetBool (PcdAmdIdsDebugPrintSerialPortDetectCableConnection)) {
    // Wait for DSR to be set.
    do {
      RetryCount--;
    } while (((SerialPortReadRegister (R_UART_MSR) & B_UART_MSR_DSR) != B_UART_MSR_DSR) && (RetryCount > 0));
  }

  if (RetryCount == 0) {
    // Time expired
    return FALSE;
  }
  return TRUE;
}

/**
  Initialize the serial device hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfully initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.

**/

RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  RETURN_STATUS  Status;

  if (!AmdIdsCheckUartDivisor()) {
    Status = PlatformHookSerialPortInitialize ();
    if (RETURN_ERROR (Status)) {
      return Status;
    }
    AmdIdsInitUartDivisor ();
  }

  if (!CheckCableConnection()) {
    return RETURN_DEVICE_ERROR;
  }

  return RETURN_SUCCESS;
}

/**
  Write data from buffer to serial device.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.

  If Buffer is NULL, then ASSERT().

  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
)
{
  UINTN    Result;
  UINTN    Index;
  UINTN    FifoSize;
  UINT32   RetryCount;
  UINT32   RetryCountCTS;

  if (Buffer == NULL) {
    return 0;
  }

  if (!CheckCableConnection()) {
    return 0;
  }

  if (NumberOfBytes == 0) {
    //
    // Flush the hardware
    //

    //
    // Wait for both the transmit FIFO and shift register empty.
    //
    RetryCount = 2000;
    do {
      RetryCount--;
    } while (((SerialPortReadRegister (R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) && (RetryCount > 0));

    if (RetryCount == 0) {
      AmdIdsInitUartDivisor();
    }

    //
    // Wait for the hardware flow control signal
    //
    while (!SerialPortWritable ());
    return 0;
  }

  //
  // Compute the maximum size of the Tx FIFO
  //
  FifoSize = 1;
  if ((PcdGet8 (PcdAmdIdsUartFifoControl) & B_UART_FCR_FIFOE) != 0) {
    if ((PcdGet8 (PcdAmdIdsUartFifoControl) & B_UART_FCR_FIFO64) == 0) {
      FifoSize = 16;
    } else {
      FifoSize = 64;
    }
  }

  Result = NumberOfBytes;
  while (NumberOfBytes != 0) {
    //
    // Wait for the serial port to be ready, to make sure both the transmit FIFO
    // and shift register empty.
    //
    RetryCount = 2000;
    do {
      RetryCount--;
    } while (((SerialPortReadRegister (R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) && (RetryCount > 0));

    //
    // Fill then entire Tx FIFO
    //
    for (Index = 0; Index < FifoSize && NumberOfBytes != 0; Index++, NumberOfBytes--, Buffer++) {
      //
      // Wait for the hardware flow control signal
      //
      RetryCountCTS = 2000;
      do {
        RetryCountCTS--;
      } while (!SerialPortWritable () && (RetryCountCTS > 0));

      //
      // Write byte to the transmit buffer.
      //
      SerialPortWriteRegister (R_UART_TXBUF, *Buffer);
    }
  }
  return Result;
}

/**
  Reads data from a serial device into a buffer.

  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
  @param  NumberOfBytes    Number of bytes to read from the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
)
{
  UINTN  Result;
  UINT8  Mcr;

  if (NULL == Buffer) {
    return 0;
  }

  Mcr = (UINT8)(SerialPortReadRegister (R_UART_MCR) & ~B_UART_MCR_RTS);

  for (Result = 0; NumberOfBytes-- != 0; Result++, Buffer++) {
    //
    // Wait for the serial port to have some data.
    //
    while ((SerialPortReadRegister (R_UART_LSR) & B_UART_LSR_RXRDY) == 0) {
      if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
        //
        // Set RTS to let the peer send some data
        //
        SerialPortWriteRegister (R_UART_MCR, (UINT8)(Mcr | B_UART_MCR_RTS));
      }
    }

    if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
      //
      // Clear RTS to prevent peer from sending data
      //
      SerialPortWriteRegister (R_UART_MCR, Mcr);
    }

    //
    // Read byte from the receive buffer.
    //
    *Buffer = SerialPortReadRegister (R_UART_RXBUF);
  }

  return Result;
}

/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls aserial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  //
  // Read the serial port status
  //
  if ((SerialPortReadRegister (R_UART_LSR) & B_UART_LSR_RXRDY) != 0) {
    if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
      //
      // Clear RTS to prevent peer from sending data
      //
      SerialPortWriteRegister (R_UART_MCR, (UINT8)(SerialPortReadRegister (R_UART_MCR) & ~B_UART_MCR_RTS));
    }
    return TRUE;
  }

  if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
    //
    // Set RTS to let the peer send some data
    //
    SerialPortWriteRegister (R_UART_MCR, (UINT8)(SerialPortReadRegister (R_UART_MCR) | B_UART_MCR_RTS));
  }

  return FALSE;
}
