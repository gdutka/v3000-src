/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>

#include <Library/AmdSocBaseLib.h>
#include <Filecode.h>

#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>

#define FILECODE FCH_PROM21_LIBRARY_BASE_FCHPROM21BASELIB_FILECODE

#define PCIE_BASE_ADDRESS (PcdGet64(PcdPciExpressBaseAddress))

#define CheckPcieWriteCycle(Offset) { while ((*pbCheck & BIT7) == BIT7) {DEBUG((DEBUG_INFO, "  Wait %d Pcie Write Cycle\n", Offset));} }

/*---------------------------------------------------------------------------------------*/
/**
 * Delay with microseconds
 *
 *
 * @param[in] Delay     Delay with microseconds
 *
 */
VOID
PtDelayUs (
  IN      UINT32    Delay
  )
{
  UINT32  timerAddr;
  UINT32  startTime;
  UINT32  elapsedTime;

  timerAddr = ACPI_TIMER_MMIO_ADDRESS;
  startTime = *(volatile UINT32*) (UINTN) timerAddr;
  for ( ;; ) {
    elapsedTime = *(volatile UINT32*) (UINTN) timerAddr;
    if (elapsedTime < startTime) {
      elapsedTime = elapsedTime + ACPI_MAX_TIMER - startTime;
    } else {
      elapsedTime = elapsedTime - startTime;
    }
    if ((elapsedTime * ACPI_ELAPSED_TIME_UNIT / ACPI_ELAPSED_TIME_DIVIDER) > Delay) {
      break;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read an 8-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT8
PtMmioRead8 (
  IN      UINTN     Address
  )
{
  UINT8   Value;

  Value = *(volatile UINT8*)Address;
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a   Address = 0x%08X Value = 0x%02X\n", __FUNCTION__, Address, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write an 8-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT8
PtMmioWrite8 (
  IN      UINTN     Address,
  IN      UINT8     Value
  )
{
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a  Address = 0x%08X Value = 0x%02X\n", __FUNCTION__, Address, Value));
  return *(volatile UINT8*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 16-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT16
PtMmioRead16 (
  IN      UINTN     Address
  )
{
  UINT16  Value;

  Value = *(volatile UINT16*)Address;
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a  Address = 0x%08X Value = 0x%04X\n", __FUNCTION__, Address, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 16-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register to read
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT16
PtMmioWrite16 (
  IN      UINTN     Address,
  IN      UINT16    Value
  )
{
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a Address = 0x%08X Value = 0x%04X\n", __FUNCTION__, Address, Value));
  return *(volatile UINT16*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT32
PtMmioRead32 (
  IN      UINTN     Address
  )
{
  UINT32  Value;

  Value = *(volatile UINT32*)Address;
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a  Address = 0x%08X Value = 0x%08X\n", __FUNCTION__, Address, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 32-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT32
PtMmioWrite32 (
  IN      UINTN     Address,
  IN      UINT32    Value
  )
{
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a Address = 0x%08X Value = 0x%08X\n", __FUNCTION__, Address, Value));
  return *(volatile UINT32*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 64-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT64
PtMmioRead64 (
  IN      UINTN     Address
  )
{
  UINT64  Value;

  Value = *(volatile UINT64*)Address;
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a  Address = 0x%016X Value = 0x%016X\n", __FUNCTION__, Address, Value));
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 64-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT64
PtMmioWrite64 (
  IN      UINTN     Address,
  IN      UINT64    Value
  )
{
  DEBUG((DEBUG_INFO, "  PT-BASELIB-%a Address = 0x%016X Value = 0x%016X\n", __FUNCTION__, Address, Value));
  return *(volatile UINT64*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read an 8-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT8
PtPciRead8 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  return PtMmioRead8 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 16-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT16
PtPciRead16 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  return PtMmioRead16 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT32
PtPciRead32 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  return PtMmioRead32 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write an 8-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
PtPciWrite8 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset,
  IN      UINT8     Data
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  PtMmioWrite8 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 16-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
PtPciWrite16 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset,
  IN      UINT16    Data
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  PtMmioWrite16 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 32-bit PCI register
 *
 *
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
PtPciWrite32 (
  IN      UINT8     Bus,
  IN      UINT8     Device,
  IN      UINT8     Function,
  IN      UINT16    Offset,
  IN      UINT32    Data
  )
{
  UINT32  Address;
  Address = (UINT32) (PCIE_BASE_ADDRESS + (Bus << 20) + (Device << 15) + (Function << 12) + Offset);
  PtMmioWrite32 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32-bit SMN register
 *
 *
 * @param[in] IohcBus       IOHC (Node) bus number
 * @param[in] SmnAddress    The Address of SMN register
 *
 * @retval    SMN register value
 *
 */
UINT32
PtSmnRead (
  IN      UINT8     IohcBus,
  IN      UINT32    SmnAddress
  )
{
  PtPciWrite32 (IohcBus, 0, 0, IOHC_NB_SMN_INDEX_2_BIOS, SmnAddress);
  return PtPciRead32 (IohcBus, 0, 0, IOHC_NB_SMN_DATA_2_BIOS);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 32-bit data to SMN register
 *
 *
 * @param[in] IohcBus       IOHC (Node) bus number
 * @param[in] SmnAddress    The Address of SMN register
 * @param[in] SmnData       The Data to write SMN register
 *
 */
VOID
PtSmnWrite (
  IN      UINT8     IohcBus,
  IN      UINT32    SmnAddress,
  IN      UINT32    SmnData
  )
{
  PtPciWrite32 (IohcBus, 0, 0, IOHC_NB_SMN_INDEX_2_BIOS, SmnAddress);
  PtPciWrite32 (IohcBus, 0, 0, IOHC_NB_SMN_DATA_2_BIOS, SmnData);
}

UINT8
FchProm21XhciReadByteV2 (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  )
{
  volatile UINT8    *pbCheck;
  UINT8             Data;
  UINT8             Counter;

  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  Counter = 0;
  do {
    Data = *pbCheck;
    if (Data == 0xFF) {
      DEBUG((DEBUG_INFO, "  PT reset itself --0-- !!!\n"));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    } else if (Data & BIT7) {
      DEBUG((DEBUG_INFO, "  Wait 0 Pcie Write Cycle V2 Counter = %d\n", Counter));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    }
  } while (Data & BIT7);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  Counter = 0;
  do {
    Data = *pbCheck;
    if (Data == 0xFF) {
      DEBUG((DEBUG_INFO, "  PT reset itself --1-- !!!\n"));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    } else if (Data & BIT7) {
      DEBUG((DEBUG_INFO, "  Wait 1 Pcie Write Cycle V2 Counter = %d\n", Counter));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    }
  } while (Data & BIT7);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  Counter = 0;
  do {
    Data = *pbCheck;
    if (Data == 0xFF) {
      DEBUG((DEBUG_INFO, "  PT reset itself --2-- !!!\n"));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    } else if (Data & BIT7) {
      DEBUG((DEBUG_INFO, "  Wait 2 Pcie Write Cycle V2 Counter = %d\n", Counter));
      Counter++;
      if (Counter == 100) {
        PtDelayUs (200 * 1000);                                             // Delay 200 ms
        Counter = 0;
      }
    }
  } while (Data & BIT7);

  // Read byte Data
  Data = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X) = 0x%02X\n", __FUNCTION__, XhciMmio, Address, Data));
  return  Data;
}

UINT8
FchProm21XhciReadByte (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  )
{
  volatile UINT8    *pbCheck;
  UINT8             Data;

  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Read byte Data
  Data = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X) = 0x%02X\n", __FUNCTION__, XhciMmio, Address, Data));
  return  Data;
}

UINT16
FchProm21XhciReadWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  )
{
  volatile UINT8    *pbCheck;
  UINT8   DataByte;
  UINT16  DataWord;

  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataWord = (UINT16) DataByte;

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 1) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFF) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 1) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFF) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 1) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataWord |= ((UINT16) DataByte) << 8;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X) = 0x%04X\n", __FUNCTION__, XhciMmio, Address, DataWord));
  return  DataWord;
}

UINT32
FchProm21XhciReadDWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  )
{
  volatile UINT8    *pbCheck;
  UINT8   DataByte;
  UINT32  DataDWord;

  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataDWord = (UINT32) DataByte;

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 1) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFF) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 1) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFF) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 1) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataDWord |= ((UINT32) DataByte) << 8;

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 2) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFE) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 2) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFE) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 2) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataDWord |= ((UINT32) DataByte) << 16;

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 3) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFD) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 3) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFD) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 3) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Read byte Data
  DataByte = *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_READ_DATA_OFFSET);
  DataDWord |= ((UINT32) DataByte) << 24;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X) = 0x%08X\n", __FUNCTION__, XhciMmio, Address, DataDWord));
  return  DataDWord;
}

VOID
FchProm21XhciWriteByte (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT8     Data
  )
{
  volatile UINT8    *pbCheck;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X, 0x%02X)\n", __FUNCTION__, XhciMmio, Address, Data));
  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = Data;
  CheckPcieWriteCycle (4);
}

VOID
FchProm21XhciWriteWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT16    Data
  )
{
  volatile UINT8    *pbCheck;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X, 0x%04X)\n", __FUNCTION__, XhciMmio, Address, Data));
  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) (Data & 0xFF);
  CheckPcieWriteCycle (4);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 1) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFF) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 1) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFF) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 1) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) ((Data >> 8) & 0xFF);
  CheckPcieWriteCycle (4);
}

VOID
FchProm21XhciWriteDWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT32    Data
  )
{
  volatile UINT8    *pbCheck = NULL;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%08X, 0x%05X, 0x%08X)\n", __FUNCTION__, XhciMmio, Address, Data));
  pbCheck = (volatile UINT8 *) (UINTN) (XhciMmio + XHCI_PCIE_WRITE_CYCLE_OFFSET);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) (Address & 0xFF);
  CheckPcieWriteCycle (0);

  // Write Address High byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) ((Address >> 8) & 0xFF);
  CheckPcieWriteCycle (1);

  // Write Segment
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) ((Address >> 16) & 0xFF);
  CheckPcieWriteCycle (2);

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) (Data & 0xFF);
  CheckPcieWriteCycle (4);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 1) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFF) == 0xFF) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 1) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFF) == 0xFFFF) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 1) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) ((Data >> 8) & 0xFF);
  CheckPcieWriteCycle (4);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 2) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFE) == 0xFE) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 2) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFE) == 0xFFFE) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 2) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) ((Data >> 16) & 0xFF);
  CheckPcieWriteCycle (4);

  // Write Address Low byte
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_L_OFFSET) = (UINT8) ((Address + 3) & 0xFF);
  CheckPcieWriteCycle (0);

  if ((Address & 0xFD) == 0xFD) {
    // Write Address High byte
    *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_ADDR_H_OFFSET) = (UINT8) (((Address + 3) >> 8) & 0xFF);
    CheckPcieWriteCycle (1);

    if ((Address & 0xFFFD) == 0xFFFD) {
      // Write Segment
      *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_SEG_OFFSET) = (UINT8) (((Address + 3) >> 16) & 0xFF);
      CheckPcieWriteCycle (2);
    }
  }

  // Write byte Data
  *(volatile UINT8 *) (UINTN) (XhciMmio + XHCI_REG_WRITE_DATA_OFFSET) = (UINT8) ((Data >> 24) & 0xFF);
  CheckPcieWriteCycle (4);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Save and Init Prom Bus/Command/Mmio/Aspm
 *
 *
 * @param[out] PtPciTable   Save Restore Table location
 * @param[in]  CheckBit     Bit0  = 1 - Save and Init UspBus
 *                          Bit1  = 1 - Save and Init XhciBus
 *                          Bit2  = 1 - Save and Init SataBus
 *                          Bit3  = 1 - Save and Init XhciMmio
 *                          Bit4  = 1 - Save and Init UspCmd
 *                          Bit5  = 1 - Save and Init XhciCmd
 *                          Bit6  = 1 - Save and Init SataCmd
 *                          Bit7  = 1 - Save and Disable ASPM
 *                          Bit8  = 1 - Save and Init SecUspBus
 *                          Bit9  = 1 - Save and Init SecXhciBus
 *                          Bit10 = 1 - Save and Init SecSataBus
 *                          Bit11 = 1 - Save and Init SecXhciMmio
 *                          Bit12 = 1 - Save and Init SecUspCmd
 *                          Bit13 = 1 - Save and Init SecXhciCmd
 *                          Bit14 = 1 - Save and Init SecSataCmd
 *                          Bit15 = 1 - Save and Disable Secondary ASPM
 *
 */
VOID
SaveInitPromBus (
     OUT  PT_PCI_SAVE_RESTORE_TABLE     *PtPciTable,
  IN      UINT16    CheckBit
  )
{
  UINT8   GppBus;
  UINT8   GppDev;
  UINT8   GppFun;
  UINT32  GppPcieAddress;
  UINT32  XhciMmio;

  UINT8   SecGppBus;
  UINT8   SecGppDev;
  UINT8   SecGppFun;
  UINT32  SecGppPcieAddress;
  UINT32  SecXhciMmio;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-Start-Save-Init-%a", __FUNCTION__, (CheckBit & BIT0) ? "UspBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT1) ? "XhciBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT2) ? "SataBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT3) ? "XhciMmio" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT4) ? "UspCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT5) ? "XhciCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT6) ? "SataCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a\n", (CheckBit & BIT7) ? "ASPM" : " "));

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-Start-Save-Init-%a", __FUNCTION__, (CheckBit & BIT8) ? "SecUspBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT9) ? "SecXhciBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT10) ? "SecSataBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT11) ? "SecXhciMmio" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT12) ? "SecUspCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT13) ? "SecXhciCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT14) ? "SecSataCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a\n", (CheckBit & BIT15) ? "SecASPM" : " "));

  XhciMmio = PcdGet32 (PcdPT21XhciMmio);
  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);
  if (CheckBit & (BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT2 + BIT1 + BIT0)) {
    PtPciTable->PromBus = PcdGet8 (PcdPT21TempBusNum) + GppBus;
  }

  SecXhciMmio = PcdGet32 (PcdPT21SecXhciMmio);
  SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
  SecGppBus = (UINT8) ((SecGppPcieAddress >> 20) & 0xFF);
  SecGppDev = (UINT8) ((SecGppPcieAddress >> 15) & 0x1F);
  SecGppFun = (UINT8) ((SecGppPcieAddress >> 12) & 0x07);
  if (CheckBit & (BIT15 + BIT14 + BIT13 + BIT12 + BIT11 + BIT10 + BIT9 + BIT8)) {
    PtPciTable->SecPromBus = PtPciTable->PromBus + 4;
  }

  if (CheckBit & (BIT7 + BIT2 + BIT1 + BIT0)) {
    // Save/Init SOC GPP and PT USP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->GppBus = PtPciRead32 (GppBus, GppDev, GppFun, 0x18);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x18, GppBus);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x19, PtPciTable->PromBus);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x1A, PtPciTable->PromBus + 7);

    PtPciTable->UspBus = PtPciRead32 (PtPciTable->PromBus, 0, 0, 0x18);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x18, PtPciTable->PromBus);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x19, PtPciTable->PromBus + 1);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x1A, PtPciTable->PromBus + 7);
  }

  if (CheckBit & (BIT7 + BIT1)) {
    // Save/Init PT XHCI DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->XhciDspBus = PtPciRead32 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x18);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x18, PtPciTable->PromBus + 1);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x19, PtPciTable->PromBus + 2);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x1A, PtPciTable->PromBus + 2);
  }

  if (CheckBit & (BIT7 + BIT2)) {
    // Save/Init PT SATA DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->SataDspBus = PtPciRead32 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x18);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x18, PtPciTable->PromBus + 1);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x19, PtPciTable->PromBus + 3);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x1A, PtPciTable->PromBus + 3);
  }

  if ((CheckBit & BIT3)) {
    // Save/Init SOC GPP, PT USP, PT XHCI DSP and PT XHCI Original MMIO
    PtPciTable->GppMmio = PtPciRead32 (GppBus, GppDev, GppFun, 0x20);
    PtPciWrite16 (GppBus, GppDev, GppFun, 0x20, (UINT16) (XhciMmio >> 16));
    PtPciWrite16 (GppBus, GppDev, GppFun, 0x22, (UINT16) (SecXhciMmio >> 16));

    PtPciTable->UspMmio = PtPciRead32 (PtPciTable->PromBus, 0, 0, 0x20);
    PtPciWrite16 (PtPciTable->PromBus, 0, 0, 0x20, (UINT16) (XhciMmio >> 16));
    PtPciWrite16 (PtPciTable->PromBus, 0, 0, 0x22, (UINT16) (SecXhciMmio >> 16));

    PtPciTable->XhciDspMmio = PtPciRead32 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x20);
    PtPciWrite16 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x20, (UINT16) (XhciMmio >> 16));
    PtPciWrite16 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x22, (UINT16) (XhciMmio >> 16));

    PtPciTable->XhciMmio = PtPciRead32 (PtPciTable->PromBus + 2, 0, 0, 0x10) & 0xFFFFFF00;
    PtPciWrite32 (PtPciTable->PromBus + 2, 0, 0, 0x10, XhciMmio);
  }

  if ((CheckBit & (BIT6 + BIT5 + BIT4))) {
    // Save/Init SOC GPP, PT USP Command
    PtPciTable->GppCmd = PtPciRead8 (GppBus, GppDev, GppFun, 0x04);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x04, 6);

    PtPciTable->UspCmd = PtPciRead8 (PtPciTable->PromBus, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT5)) {
    // Save/Init PT XHCI DSP and PT XHCI Command
    PtPciTable->XhciDspCmd = PtPciRead8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x04);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x04, 6);

    PtPciTable->XhciCmd = PtPciRead8 (PtPciTable->PromBus + 2, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->PromBus + 2, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT6)) {
    // Save/Init PT SATA DSP and PT SATA Command
    PtPciTable->SataDspCmd = PtPciRead8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x04);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x04, 6);

    PtPciTable->SataCmd = PtPciRead8 (PtPciTable->PromBus + 3, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->PromBus + 3, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT7) == BIT7) {
    // Save/Disable SOC GPP, PT USP, PT XHCI DSP, PT SATA DSP, PT XHCI and PT SATA ASPM
    PtPciTable->GppAspm = PtPciRead8 (GppBus, GppDev, GppFun, 0x68);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x68, PtPciTable->GppAspm & 0xFC);

    PtPciTable->UspAspm = PtPciRead8 (PtPciTable->PromBus, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x90, PtPciTable->UspAspm & 0xFC);

    PtPciTable->XhciDspAspm = PtPciRead8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x90);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x90, PtPciTable->XhciDspAspm & 0xFC);

    PtPciTable->SataDspAspm = PtPciRead8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x90);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x90, PtPciTable->SataDspAspm & 0xFC);

    PtPciTable->XhciAspm = PtPciRead8 (PtPciTable->PromBus + 2, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->PromBus + 2, 0, 0, 0x90, PtPciTable->XhciAspm & 0xFC);

    PtPciTable->SataAspm = PtPciRead8 (PtPciTable->PromBus + 3, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->PromBus + 3, 0, 0, 0x90, PtPciTable->SataAspm & 0xFC);
  }

  if (CheckBit & (BIT15 + BIT10 + BIT9 + BIT8)) {
    // Save/Init Secondary SOC GPP and PT USP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->SecGppBus = PtPciRead32 (SecGppBus, SecGppDev, SecGppFun, 0x18);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x18, SecGppBus);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x19, PtPciTable->SecPromBus);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x1A, PtPciTable->SecPromBus + 3);

    PtPciTable->SecUspBus = PtPciRead32 (PtPciTable->SecPromBus, 0, 0, 0x18);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x18, PtPciTable->SecPromBus);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x19, PtPciTable->SecPromBus + 1);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x1A, PtPciTable->SecPromBus + 3);
  }

  if (CheckBit & (BIT15 + BIT9)) {
    // Save/Init Secondary PT XHCI DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->SecXhciDspBus = PtPciRead32 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x18);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x18, PtPciTable->SecPromBus + 1);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x19, PtPciTable->SecPromBus + 2);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x1A, PtPciTable->SecPromBus + 2);
  }

  if (CheckBit & (BIT15 + BIT10)) {
    // Save/Init Secondary PT SATA DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciTable->SecSataDspBus = PtPciRead32 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x18);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x18, PtPciTable->SecPromBus + 1);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x19, PtPciTable->SecPromBus + 3);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x1A, PtPciTable->SecPromBus + 3);
  }

  if ((CheckBit & BIT11)) {
    // Save/Init Secondary SOC GPP, PT USP, PT XHCI DSP and PT XHCI Original MMIO
    PtPciTable->SecGppMmio = PtPciRead32 (SecGppBus, SecGppDev, SecGppFun, 0x20);
    PtPciWrite16 (SecGppBus, SecGppDev, SecGppFun, 0x20, (UINT16) (SecXhciMmio >> 16));
    PtPciWrite16 (SecGppBus, SecGppDev, SecGppFun, 0x22, (UINT16) (SecXhciMmio >> 16));

    PtPciTable->SecUspMmio = PtPciRead32 (PtPciTable->SecPromBus, 0, 0, 0x20);
    PtPciWrite16 (PtPciTable->SecPromBus, 0, 0, 0x20, (UINT16) (SecXhciMmio >> 16));
    PtPciWrite16 (PtPciTable->SecPromBus, 0, 0, 0x22, (UINT16) (SecXhciMmio >> 16));

    PtPciTable->SecXhciDspMmio = PtPciRead32 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x20);
    PtPciWrite16 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x20, (UINT16) (SecXhciMmio >> 16));
    PtPciWrite16 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x22, (UINT16) (SecXhciMmio >> 16));

    PtPciTable->SecXhciMmio = PtPciRead32 (PtPciTable->SecPromBus + 2, 0, 0, 0x10) & 0xFFFFFF00;
    PtPciWrite32 (PtPciTable->SecPromBus + 2, 0, 0, 0x10, SecXhciMmio);
  }

  if ((CheckBit & (BIT14 + BIT13 + BIT12))) {
    // Save/Init Secondary SOC GPP, PT USP Command
    PtPciTable->SecGppCmd = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x04);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x04, 6);

    PtPciTable->SecUspCmd = PtPciRead8 (PtPciTable->SecPromBus, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT13)) {
    // Save/Init Secondary PT XHCI DSP and PT XHCI Command
    PtPciTable->SecXhciDspCmd = PtPciRead8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x04);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x04, 6);

    PtPciTable->SecXhciCmd = PtPciRead8 (PtPciTable->SecPromBus + 2, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->SecPromBus + 2, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT14)) {
    // Save/Init Secondary PT SATA DSP and PT SATA Command
    PtPciTable->SecSataDspCmd = PtPciRead8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x04);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x04, 6);

    PtPciTable->SecSataCmd = PtPciRead8 (PtPciTable->SecPromBus + 3, 0, 0, 0x04);
    PtPciWrite8 (PtPciTable->SecPromBus + 3, 0, 0, 0x04, 6);
  }

  if ((CheckBit & BIT15) == BIT15) {
    // Save/Disable Secondary SOC GPP, PT USP, PT XHCI DSP, PT SATA DSP, PT XHCI and PT SATA ASPM
    PtPciTable->SecGppAspm = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, PtPciTable->SecGppAspm & 0xFC);

    PtPciTable->SecUspAspm = PtPciRead8 (PtPciTable->SecPromBus, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x90, PtPciTable->SecUspAspm & 0xFC);

    PtPciTable->SecXhciDspAspm = PtPciRead8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x90);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x90, PtPciTable->SecXhciDspAspm & 0xFC);

    PtPciTable->SecSataDspAspm = PtPciRead8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x90);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x90, PtPciTable->SecSataDspAspm & 0xFC);

    PtPciTable->SecXhciAspm = PtPciRead8 (PtPciTable->SecPromBus + 2, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->SecPromBus + 2, 0, 0, 0x90, PtPciTable->SecXhciAspm & 0xFC);

    PtPciTable->SecSataAspm = PtPciRead8 (PtPciTable->SecPromBus + 3, 0, 0, 0x90);
    PtPciWrite8 (PtPciTable->SecPromBus + 3, 0, 0, 0x90, PtPciTable->SecSataAspm & 0xFC);
  }

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-End\n", __FUNCTION__));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Restore Prom Bus/Command/Mmio/Aspm
 *
 *
 * @param[in] PtPciTable    Save Restore Table location
 * @param[in]  CheckBit     Bit0  = 1 - Restore Original UspBus
 *                          Bit1  = 1 - Restore Original XhciBus
 *                          Bit2  = 1 - Restore Original SataBus
 *                          Bit3  = 1 - Restore Original XhciMmio
 *                          Bit4  = 1 - Restore Original UspCmd
 *                          Bit5  = 1 - Restore Original XhciCmd
 *                          Bit6  = 1 - Restore Original SataCmd
 *                          Bit7  = 1 - Restore Original ASPM
 *                          Bit8  = 1 - Restore Original SecUspBus
 *                          Bit9  = 1 - Restore Original SecXhciBus
 *                          Bit10 = 1 - Restore Original SecSataBus
 *                          Bit11 = 1 - Restore Original SecXhciMmio
 *                          Bit12 = 1 - Restore Original SecUspCmd
 *                          Bit13 = 1 - Restore Original SecXhciCmd
 *                          Bit14 = 1 - Restore Original SecSataCmd
 *                          Bit15 = 1 - Restore Original Secondary ASPM
 *
 */
VOID
RestorePromBus (
  IN      PT_PCI_SAVE_RESTORE_TABLE     *PtPciTable,
  IN      UINT16    CheckBit
  )
{
  UINT8   GppBus;
  UINT8   GppDev;
  UINT8   GppFun;
  UINT32  GppPcieAddress;

  UINT8   SecGppBus;
  UINT8   SecGppDev;
  UINT8   SecGppFun;
  UINT32  SecGppPcieAddress;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-Start-Restore-%a", __FUNCTION__, (CheckBit & BIT0) ? "UspBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT1) ? "XhciBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT2) ? "SataBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT3) ? "XhciMmio" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT4) ? "UspCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT5) ? "XhciCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT6) ? "SataCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a\n", (CheckBit & BIT7) ? "ASPM" : " "));

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-Start-Restore-%a", __FUNCTION__, (CheckBit & BIT8) ? "SecUspBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT9) ? "SecXhciBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT10) ? "SecSataBus" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT11) ? "SecXhciMmio" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT12) ? "SecUspCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT13) ? "SecXhciCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a", (CheckBit & BIT14) ? "SecSataCmd" : " "));
  DEBUG ((DEBUG_INFO, "-%a\n", (CheckBit & BIT15) ? "SecASPM" : " "));

  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);

  SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
  SecGppBus = (UINT8) ((SecGppPcieAddress >> 20) & 0xFF);
  SecGppDev = (UINT8) ((SecGppPcieAddress >> 15) & 0x1F);
  SecGppFun = (UINT8) ((SecGppPcieAddress >> 12) & 0x07);

  if (CheckBit & BIT15) {
    // Restore Original Secondary SOC GPP, PT USP, PT XHCI DSP, PT SATA DSP, PT XHCI and PT SATA ASPM
    PtPciWrite8 (PtPciTable->SecPromBus + 2, 0, 0, 0x90, PtPciTable->SecXhciAspm);
    PtPciWrite8 (PtPciTable->SecPromBus + 3, 0, 0, 0x90, PtPciTable->SecSataAspm);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x90, PtPciTable->SecXhciDspAspm);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x90, PtPciTable->SecSataDspAspm);
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x90, PtPciTable->SecUspAspm);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, PtPciTable->SecGppAspm);
  }

  if (CheckBit & BIT13) {
    // Restore Original Secondary PT XHCI DSP and PT XHCI Command
    PtPciWrite8 (PtPciTable->SecPromBus + 2, 0, 0, 0x04, PtPciTable->SecXhciCmd);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x04, PtPciTable->SecXhciDspCmd);
  }

  if (CheckBit & BIT14) {
    // Restore Original Secondary PT SATA DSP and PT SATA Command
    PtPciWrite8 (PtPciTable->SecPromBus + 3, 0, 0, 0x04, PtPciTable->SecSataCmd);
    PtPciWrite8 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x04, PtPciTable->SecSataDspCmd);
  }

  if (CheckBit & (BIT14 + BIT13 + BIT12)) {
    // Restore Original Secondary SOC GPP and PT USP Command
    PtPciWrite8 (PtPciTable->SecPromBus, 0, 0, 0x04, PtPciTable->SecUspCmd);
    PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x04, PtPciTable->SecGppCmd);
  }

  if (CheckBit & BIT11) {
    // Restore Original Secondary SOC GPP, PT USP, PT XHCI DSP and PT XHCI MMIO
    PtPciWrite32 (PtPciTable->SecPromBus + 2, 0, 0, 0x10, PtPciTable->SecXhciMmio);
    PtPciWrite32 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x20, PtPciTable->SecXhciDspMmio);
    PtPciWrite32 (PtPciTable->SecPromBus, 0, 0, 0x20, PtPciTable->SecUspMmio);
    PtPciWrite32 (SecGppBus, SecGppDev, SecGppFun, 0x20, PtPciTable->SecGppMmio);
  }

  if (CheckBit & (BIT15 + BIT9)) {
    // Restore Original Secondary PT XHCI DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->SecPromBus + 1, SecXhciDspDev, 0, 0x18, PtPciTable->SecXhciDspBus);
  }

  if (CheckBit & (BIT15 + BIT10)) {
    // Restore Original Secondary PT SATA DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->SecPromBus + 1, SecSataDspDev, 0, 0x18, PtPciTable->SecSataDspBus);
  }

  if (CheckBit & (BIT15 + BIT10 + BIT9 + BIT8)) {
    // Restore Original Secondary SOC GPP and PT USP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->SecPromBus, 0, 0, 0x18, PtPciTable->SecUspBus);
    PtPciWrite32 (SecGppBus, SecGppDev, SecGppFun, 0x18, PtPciTable->SecGppBus);
  }

  if (CheckBit & BIT7) {
    // Restore Original SOC GPP, PT USP, PT XHCI DSP, PT SATA DSP, PT XHCI and PT SATA ASPM
    PtPciWrite8 (PtPciTable->PromBus + 2, 0, 0, 0x90, PtPciTable->XhciAspm);
    PtPciWrite8 (PtPciTable->PromBus + 3, 0, 0, 0x90, PtPciTable->SataAspm);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x90, PtPciTable->XhciDspAspm);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x90, PtPciTable->SataDspAspm);
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x90, PtPciTable->UspAspm);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x68, PtPciTable->GppAspm);
  }

  if (CheckBit & BIT5) {
    // Restore Original PT XHCI DSP and PT XHCI Command
    PtPciWrite8 (PtPciTable->PromBus + 2, 0, 0, 0x04, PtPciTable->XhciCmd);
    PtPciWrite8 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x04, PtPciTable->XhciDspCmd);
  }

  if (CheckBit & BIT6) {
    // Restore Original PT SATA DSP and PT SATA Command
    PtPciWrite8 (PtPciTable->PromBus + 3, 0, 0, 0x04, PtPciTable->SataCmd);
    PtPciWrite8 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x04, PtPciTable->SataDspCmd);
  }

  if (CheckBit & (BIT6 + BIT5 + BIT4)) {
    // Restore Original SOC GPP and PT USP Command
    PtPciWrite8 (PtPciTable->PromBus, 0, 0, 0x04, PtPciTable->UspCmd);
    PtPciWrite8 (GppBus, GppDev, GppFun, 0x04, PtPciTable->GppCmd);
  }

  if (CheckBit & BIT3) {
    // Restore Original SOC GPP, PT USP, PT XHCI DSP and PT XHCI MMIO
    PtPciWrite32 (PtPciTable->PromBus + 2, 0, 0, 0x10, PtPciTable->XhciMmio);
    PtPciWrite32 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x20, PtPciTable->XhciDspMmio);
    PtPciWrite32 (PtPciTable->PromBus, 0, 0, 0x20, PtPciTable->UspMmio);
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x20, PtPciTable->GppMmio);
  }

  if (CheckBit & (BIT7 + BIT1)) {
    // Restore Original PT XHCI DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->PromBus + 1, XhciDspDev, 0, 0x18, PtPciTable->XhciDspBus);
  }

  if (CheckBit & (BIT7 + BIT2)) {
    // Restore Original PT SATA DSP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->PromBus + 1, SataDspDev, 0, 0x18, PtPciTable->SataDspBus);
  }

  if (CheckBit & (BIT7 + BIT2 + BIT1 + BIT0)) {
    // Restore Original SOC GPP and PT USP Original Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (PtPciTable->PromBus, 0, 0, 0x18, PtPciTable->UspBus);
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, PtPciTable->GppBus);
  }

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a-End\n", __FUNCTION__));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Find PCIe Root Port Number
 *
 *
 * @param[in] RootBridge          PCIe Root Bridge Number
 * @param[in] RootPortDevice      PCIe Root Port Device Number
 * @param[in] RootPortFunction    PCIe Root Port Function Number
 *
 * @retval    PCIe Root Port Number
 *
 */
UINT8
FindPcieRootPortNumber (
  IN      UINT8     RootBridge,
  IN      UINT8     RootPortDevice,
  IN      UINT8     RootPortFunction
  )
{
  UINT32            Ioapic0CfgAddress;
  UINT32            RootPortAddress;
  UINT8             Counter;

  Ioapic0CfgAddress = IOAPICSHDW_NB_PROG_DEVICE_REMAP_PBr;

  if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID) ||
      SocFamilyIdentificationCheck (F17_SSP_RAW_ID) || SocFamilyIdentificationCheck (F19_GN_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_RS_RAW_ID)  || SocFamilyIdentificationCheck (F19_BA_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_RSDN_RAW_ID) ) {
    Ioapic0CfgAddress += (UINT32) (3 - RootBridge) << 20;
  }
  RootPortAddress = ((UINT32) (RootPortDevice & 0x1F) << 3) + (UINT32) (RootPortFunction & 0x7);

  for (Counter = 0; Counter < 24; Counter++) {
    if (RootPortAddress == PtSmnRead (0, Ioapic0CfgAddress + (UINT32) Counter * 4)) {
      break;
    }
  }

  return Counter;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Find PCIe RcCfg/Port/Core Registers Address
 *
 *
 * @param[in] RootBridge          PCIe Root Bridge Number
 * @param[in] RootPortDevice      PCIe Root Port Device Number
 * @param[in] RootPortFunction    PCIe Root Port Function Number
 * @param[in] RegisterSelection   0x00 = PCIERCCFG: PCIe RcCfg Registers Address
 *                                0x04 = PCIEPORT:  PCIe Port Registers Address
 *                                0x08 = PCIECORE:  PCIe Core Registers Address
 *
 * @retval    PCIe RcCfg/Port/Core Registers Address
 *
 */
UINT32
FindPcieRegisterAddress (
  IN      UINT8     RootBridge,
  IN      UINT8     RootPortDevice,
  IN      UINT8     RootPortFunction,
  IN      UINT8     RegisterSelection
  )
{
  UINT32            RootPortNumber;
  UINT32            MaxPortNumber;
  UINT32            RcCfgAddress;

  DEBUG ((DEBUG_INFO, "  PT-BASELIB-%a (0x%02X, 0x%02X, 0x%02X, 0x%02X)\n", __FUNCTION__, RootBridge, RootPortDevice, RootPortFunction, RegisterSelection));

  RootPortNumber = (UINT32) FindPcieRootPortNumber (RootBridge, RootPortDevice, RootPortFunction);
  DEBUG ((DEBUG_INFO, "    PCIe Root Port Number = %d\n", RootPortNumber));

  RcCfgAddress = PCIERCCFG_ADDRESS;                                   // 0x11100000ul
  if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID) ||
      SocFamilyIdentificationCheck (F17_SSP_RAW_ID) || SocFamilyIdentificationCheck (F19_GN_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_BA_RAW_ID)) {
    RcCfgAddress      += (UINT32) (3 - RootBridge) << 20;
  }

  if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID) ||
      SocFamilyIdentificationCheck (F17_SSP_RAW_ID) || SocFamilyIdentificationCheck (F19_GN_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_BA_RAW_ID)  || SocFamilyIdentificationCheck (F17_ZP_RAW_ID)) {
    MaxPortNumber = 8;
  } else if (SocFamilyIdentificationCheck (F19_RS_RAW_ID) || SocFamilyIdentificationCheck (F19_RPL_RAW_ID) ||
             SocFamilyIdentificationCheck (F19_RSDN_RAW_ID) ) {
    MaxPortNumber = 9;
  } else if (SocFamilyIdentificationCheck (F17_RV2_RAW_ID)) {
    MaxPortNumber = 5;
  } else if (SocFamilyIdentificationCheck (F17_MDN_RAW_ID)) {
    MaxPortNumber = 6;
  } else {
    MaxPortNumber = 7;
  }

  if (SocFamilyIdentificationCheck (F19_RS_RAW_ID) || SocFamilyIdentificationCheck (F19_RSDN_RAW_ID) ) {
    RcCfgAddress = PCIERCCFG_RS_ADDRESS + (RegisterSelection << 16);  // 0x1A300000ul
    RcCfgAddress += (UINT32) (1 - RootBridge) << 21;
    RcCfgAddress += (RootPortNumber % MaxPortNumber) << 12;
    if ((RootPortNumber / MaxPortNumber) == 1) {
      RcCfgAddress += (4 << 22);
    } else if ((RootPortNumber / MaxPortNumber) == 2) {
      RcCfgAddress += (5 << 22);
    } else if ((RootPortNumber / MaxPortNumber) == 3) {
      RcCfgAddress += (1 << 22);
    } else if ((RootPortNumber / MaxPortNumber) == 4) {
      RcCfgAddress += (8 << 22);
    }
  } else if (SocFamilyIdentificationCheck (F17_RN_RAW_ID) || SocFamilyIdentificationCheck (F17_LCN_RAW_ID) ||
             SocFamilyIdentificationCheck (F19_CZN_RAW_ID)) {
    if (RootPortNumber < 3) {
      RcCfgAddress += (RegisterSelection << 16) + (RootPortNumber << 12);
    } else {
      RcCfgAddress += (1 << 20) + (RegisterSelection << 16) + ((RootPortNumber - 3) << 12);
    }
  } else if (SocFamilyIdentificationCheck (F19_RMB_RAW_ID) || SocFamilyIdentificationCheck (F19_PHX_RAW_ID)) {
    if (RootPortNumber < 5) {
      RcCfgAddress += (RegisterSelection << 16) + (RootPortNumber << 12);
    } else if (RootPortNumber < 11) {
      RcCfgAddress += (1 << 20) + (RegisterSelection << 16) + ((RootPortNumber - 5) << 12);
    } else if (RootPortNumber == 11) {
      RcCfgAddress += (2 << 20) + (RegisterSelection << 16);
    } else if (RootPortNumber == 12) {
      RcCfgAddress += (3 << 20) + (RegisterSelection << 16);
    }
  } else {
    RcCfgAddress += ((RootPortNumber / MaxPortNumber) << 22) + (RegisterSelection << 16) + ((RootPortNumber % MaxPortNumber) << 12);
  }

  if (RegisterSelection == 0x8) {
    RcCfgAddress &= 0xFFFF0FFF;
  }

  DEBUG ((DEBUG_INFO, "    PCIe Register Address = 0x%08X\n", RcCfgAddress));
  return RcCfgAddress;
}

