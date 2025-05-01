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

CHAR8 *EspiSerialLocalBuffer = "Espi Serial --- ";

UINT8
EspiSerialPortReadRegister (
  UINTN  Base,
  UINTN  Offset
  )
{
    return IoRead8 (Base + Offset);
}

BOOLEAN
AumSerialPortWritable (
  VOID
  )
{
  UINT8 Value;

  if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
    Value = EspiSerialPortReadRegister (0x3F8, R_UART_MSR);
    // Wait for CTS to be set.
    if ((Value & B_UART_MSR_CTS) != B_UART_MSR_CTS) {
      return FALSE;
    }
  }
  return TRUE;
}


UINTN AumEspiUartWrite (
  UINT8     *Buffer,
  UINTN     NumberOfBytes
  )
{
  UINTN    Index;
  UINTN    FifoSize;
  UINTN    Result;
  UINT32   RetryCount;

  if (Buffer == NULL) {
    return 0;
  }

  if (NumberOfBytes == 0) {
    while ((EspiSerialPortReadRegister (0x3F8, R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY));
    return 0;
  }

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
      RetryCount = 2000;
      do {
        RetryCount--;
      } while (((EspiSerialPortReadRegister (0x3F8, R_UART_LSR) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) && (RetryCount > 0));
            
      RetryCount = 2000;
      do {
        RetryCount--;
      } while (!AumSerialPortWritable () && (RetryCount > 0));

      IoWrite8 (0x3F8, *Buffer);
    }
  }
  return Result;
}


UINT32 AumGetESPIBase ()
{
  UINT32 base;
  base = PciRead32 (PCI_LIB_ADDRESS (0, 0x14, 3, 0xA0));
  base &= 0xFFFFFFC0;
  base += 0x00010000;
  // eSPI Master Capability
  if ( MmioRead32 (base + 0x2C) == 0 || MmioRead32 (base + 0x2C) == 0xFFFFFFFF) {
    IoWrite32 (0x80, AUM_ESPI_CAP_DETECT_FAIL);
    AUM_DEADLOOP();
  }
  return base;
}

VOID
AumInitEspiIo80 (
  UINT32      EspiBase,
  BOOLEAN     Enable
  )
{
  if (Enable){
    MmioAndThenOr32 (EspiBase + 0x40, ~ (UINT32)BIT2, BIT2);                            // Enable 80 eSPI IO decode
  } else{
    MmioAndThenOr32 (EspiBase + 0x40, ~ (UINT32)BIT2, 0x00);                            // Disable 80 eSPI IO decode
  }
}

RETURN_STATUS
PlatformHookEsipUartInit (
  VOID
  )
{
  UINT32          EspiBase;

  EspiBase = AumGetESPIBase();
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x46, 0x00, 0x00);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x1E, 0x00, 0x01);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x68, 0x00, 0x01);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x69, 0x00, 0x01);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x6A, 0x00, 0x01);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x6B, 0x00, 0x01);
  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x6C, 0x00, 0x01);

  MmioAndThenOr8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x56, 0x00, 0x02);
  MmioAndThenOr32 (ACPI_MMIO_BASE + GPIO70, 0x00, 0x00E40000);
  MmioAndThenOr32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74, ~ (UINT32) (BIT11 + BIT10), 0x00);
  MmioAndThenOr32 (ACPI_MMIO_BASE + PMIO_BASE + 0x90, 0xFFFFFFFF, (UINT32) BIT16);

  AumInitEspiIo80 (EspiBase, 1);

  MmioAndThenOr32 (EspiBase + 0x44,  0x00000000, 0x060003F8); // Slave0IoBaseReg0);
  MmioAndThenOr32 (EspiBase + 0x48,  0x00000000, 0x00000000); // Slave0IoBaseReg1);
  MmioAndThenOr32 (EspiBase + 0x4C,  0x00000000, 0x0000FF07); // Slave0IoSize);
  MmioAndThenOr32 (EspiBase + 0x40,  0xFFFFF0FF, 0x00000304); // Enable IO range
  MmioAndThenOr32 (EspiBase + 0x68,  0x00000000, 0xC0000008);

  AumEspiUartWrite ((UINT8*) EspiSerialLocalBuffer, 16);

  return RETURN_SUCCESS;
}

BOOLEAN AumEspiUartSupport (VOID)
{
  if (AmdIdsEmulationAutoDetect()) {
    // Return FALSE if the system is emulation.
    return FALSE;
  }
  if (PcdGet32 (PcdAmdUnifyMessage) & PCD_AUM_ESPI_UART) {
    if (EspiSerialPortReadRegister (0x3F8, R_UART_MSR) == 0xFF) {
      return FALSE;
    }    
    return TRUE;
  }
  return FALSE;
}

RETURN_STATUS AumEspiUartInitialize (VOID)
{
  // PlatformHookEsipUartInit ();
  IoWrite32 (0x80, AUM_ESPI_EC_UART_INIT);
  return RETURN_SUCCESS;
}

UINTN AumEspiUartRead (
  UINT8     *Buffer,
  UINTN     NumberOfBytes
  )
{
  return 0;
}

BOOLEAN AumEspiUartPoll (VOID)
{
  return TRUE;
}

RETURN_STATUS AumEspiUartSetControl (UINT32 Control) {
  return RETURN_SUCCESS;
}

RETURN_STATUS AumEspiUartGetControl (UINT32 *Control) {
  return RETURN_SUCCESS;
}

RETURN_STATUS AumEspiUartSetAttributes (
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

AMD_UNIFY_MESSAGE_INSTANCE  AmdUnifyMessageEspiUart =
{
  AumEspiUartSupport,
  AumEspiUartInitialize,
  AumEspiUartWrite,
  AumEspiUartRead,
  AumEspiUartPoll,
  AumEspiUartSetControl,
  AumEspiUartGetControl,
  AumEspiUartSetAttributes
};




