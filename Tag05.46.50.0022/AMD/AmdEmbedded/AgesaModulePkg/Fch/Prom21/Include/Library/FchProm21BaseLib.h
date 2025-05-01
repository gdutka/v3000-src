/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_BASE_LIB_H_
#define _FCH_PROM21_BASE_LIB_H_

/// Structure for SOC GPP, PT USP/DSP/XHCI/SATA Save/Restore Registers
typedef struct {
  UINT32            GppBus;             ///< SOC GPP Bus Number Register
  UINT32            UspBus;             ///< PT USP Bus Number Register
  UINT32            XhciDspBus;         ///< PT XHCI DSP Bus Number Register
  UINT32            SataDspBus;         ///< PT SATA DSP Bus Number Register
  UINT32            GppMmio;            ///< SOC GPP Non-Prefetchable Memory Limit and Base Register
  UINT32            UspMmio;            ///< PT USP Non-Prefetchable Memory Limit and Base Register
  UINT32            XhciDspMmio;        ///< PT XHCI DSP Non-Prefetchable Memory Limit and Base Register
  UINT32            XhciMmio;           ///< PT XHCI Base Address 0 Register
  UINT8             GppCmd;             ///< SOC GPP Command Register
  UINT8             UspCmd;             ///< PT USP Command Register
  UINT8             XhciDspCmd;         ///< PT XHCI DSP Command Register
  UINT8             SataDspCmd;         ///< PT SATA DSP Command Register
  UINT8             XhciCmd;            ///< PT XHCI Command Register
  UINT8             SataCmd;            ///< PT SATA Command Register
  UINT8             GppAspm;            ///< SOC GPP Aspm Register
  UINT8             UspAspm;            ///< PT USP Aspm Register
  UINT8             XhciDspAspm;        ///< PT XHCI DSP Aspm Register
  UINT8             SataDspAspm;        ///< PT SATA DSP Aspm Register
  UINT8             XhciAspm;           ///< PT XHCI Aspm Register
  UINT8             SataAspm;           ///< PT SATA Aspm Register
  UINT8             PromBus;            ///< PT PCIe USP Bus Number
  UINT32            SecGppBus;          ///< Secondary SOC GPP Bus Number Register
  UINT32            SecUspBus;          ///< Secondary PT USP Bus Number Register
  UINT32            SecXhciDspBus;      ///< Secondary PT XHCI DSP Bus Number Register
  UINT32            SecSataDspBus;      ///< Secondary PT SATA DSP Bus Number Register
  UINT32            SecGppMmio;         ///< Secondary SOC GPP Non-Prefetchable Memory Limit and Base Register
  UINT32            SecUspMmio;         ///< Secondary PT USP Non-Prefetchable Memory Limit and Base Register
  UINT32            SecXhciDspMmio;     ///< Secondary PT XHCI DSP Non-Prefetchable Memory Limit and Base Register
  UINT32            SecXhciMmio;        ///< Secondary PT XHCI Base Address 0 Register
  UINT8             SecGppCmd;          ///< Secondary SOC GPP Command Register
  UINT8             SecUspCmd;          ///< Secondary PT USP Command Register
  UINT8             SecXhciDspCmd;      ///< Secondary PT XHCI DSP Command Register
  UINT8             SecSataDspCmd;      ///< Secondary PT SATA DSP Command Register
  UINT8             SecXhciCmd;         ///< Secondary PT XHCI Command Register
  UINT8             SecSataCmd;         ///< Secondary PT SATA Command Register
  UINT8             SecGppAspm;         ///< Secondary SOC GPP Aspm Register
  UINT8             SecUspAspm;         ///< Secondary PT USP Aspm Register
  UINT8             SecXhciDspAspm;     ///< Secondary PT XHCI DSP Aspm Register
  UINT8             SecSataDspAspm;     ///< Secondary PT SATA DSP Aspm Register
  UINT8             SecXhciAspm;        ///< Secondary PT XHCI Aspm Register
  UINT8             SecSataAspm;        ///< Secondary PT SATA Aspm Register
  UINT8             SecPromBus;         ///< Secondary PT PCIe USP Bus Number
} PT_PCI_SAVE_RESTORE_TABLE;

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

UINT8
FchProm21XhciReadByteV2 (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  );

UINT8
FchProm21XhciReadByte (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  );

UINT16
FchProm21XhciReadWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  );

UINT32
FchProm21XhciReadDWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address
  );

VOID
FchProm21XhciWriteByte (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT8     Data
  );

VOID
FchProm21XhciWriteWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT16    Data
  );

VOID
FchProm21XhciWriteDWord (
  IN      UINT32    XhciMmio,
  IN      UINT32    Address,
  IN      UINT32    Data
  );

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
  );

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
  );

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
  );

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
  );

#endif

