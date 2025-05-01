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

#include "AmdUnifyMessageLib.h"

UINT8
FchSerialPortReadRegister (
  UINTN  Offset
  )
{
  return MmioRead8 ((UINTN)PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + Offset * 4);
}

UINT8
FchSerialPortWriteRegister (
  UINTN  Offset,
  UINT8  Value
  )
{
  return MmioWrite8 ((UINTN)PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + Offset * 4, Value);
}

UINTN AumFchUartWrite (
  UINT8     *Buffer,
  UINTN     NumberOfBytes
  )
{
  UINTN    Result;
  UINTN    Index;
  UINTN    FifoSize;
  UINT32   RetryCount;
//  UINT32   RetryCountCTS;

  if (Buffer == NULL) {
    return 0;
  }

  if (NumberOfBytes == 0) {
    //
    // Wait for both the transmit FIFO and shift register empty.
    //
    RetryCount = 2000;
    do {
      RetryCount--;
    } while (((FchSerialPortReadRegister (R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) && (RetryCount > 0));

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
    for (Index = 0; Index < FifoSize && NumberOfBytes != 0; Index++, NumberOfBytes--, Buffer++) {
    //
    // Wait for the serial port to be ready, to make sure both the transmit FIFO
    // and shift register empty.
    //
    RetryCount = 2000;
    do {
      RetryCount--;
    } while (((FchSerialPortReadRegister (R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) && (RetryCount > 0));


/*      RetryCountCTS = 2000;
      do {
        // Wait for the hardware flow control signal
        RetryCountCTS--;
      } */
      while (!AmdIdsDpSerialWritable ());
      FchSerialPortWriteRegister (R_UART_TXBUF, *Buffer);
    }
  }
  return Result;
}

BOOLEAN AumFchUartSupport (VOID)
{
  if (AmdIdsEmulationAutoDetect()) {
    // Return FALSE if the system is emulation.
    return FALSE;
  }
  if (PcdGet32 (PcdAmdUnifyMessage) & (PCD_AUM_FCH_UART0 | PCD_AUM_FCH_UART1 | PCD_AUM_FCH_UART2 | PCD_AUM_FCH_UART3 | PCD_AUM_FCH_UART4)) {
    return TRUE;
  }
  return FALSE;
}

RETURN_STATUS AumFchUartInitialize (VOID)
{
  if (!AmdIdsCheckUartDivisor()) {
    AmdIdsInitUartIomux ();
    AmdIdsInitUartDivisor();
  }

  IoWrite32 (0x80, AUM_FCH_UART_INIT);
  return RETURN_SUCCESS;
}

UINTN AumFchUartRead (
  UINT8     *Buffer,
  UINTN     NumberOfBytes
  )
{
  return 0;
}

BOOLEAN AumFchUartPoll (VOID)
{
  return TRUE;
}

RETURN_STATUS AumFchUartSetControl (UINT32 Control) {
  return RETURN_SUCCESS;
}

RETURN_STATUS AumFchUartGetControl (UINT32 *Control) {
  return RETURN_SUCCESS;
}

RETURN_STATUS AumFchUartSetAttributes (
  UINT64 *BaudRate,
  UINT32 *ReceiveFifoDepth,
  UINT32 *Timeout,
  EFI_PARITY_TYPE *Parity,
  UINT8 *DataBits,
  EFI_STOP_BITS_TYPE *StopBits
  )
{
    return RETURN_SUCCESS;
}

AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessageFchUart =
{
  AumFchUartSupport,
  AumFchUartInitialize,
  AumFchUartWrite,
  AumFchUartRead,
  AumFchUartPoll,
  AumFchUartSetControl,
  AumFchUartGetControl,
  AumFchUartSetAttributes
};




