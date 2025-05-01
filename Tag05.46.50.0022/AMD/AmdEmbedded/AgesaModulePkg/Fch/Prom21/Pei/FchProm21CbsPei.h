/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_CBS_PEI_H_
#define _FCH_PROM21_CBS_PEI_H_

#define PT_SATA_AHCI_DID             0x43F6
#define PT_SATA_RAID_DID             0x43BD

#define PT_AHCI_MMIO_Index           0xE0
#define PT_AHCI_MMIO_Data            0xE4

#define Xhci_PME_reg                 0x18514
#define XHCI_REG_CLR_PME_EN          0x18515
#define Fch_Spread_Spec_reg          0x1288C
#define Xhci_Reg_Num                 0x04

#define PROM21_MAX_SATA_PORT_NUMBER  3
#define PROM21_MIN_SATA_SPEED        1
#define PROM21_MAX_SATA_SPEED        3

#define PROM21_SATA_P0_GEN_REG       0x2D100L
#define PROM21_SATA_P1_GEN_REG       0x2D300L
#define PROM21_SATA_P2_GEN_REG       0x2D500L
#define PROM21_SATA_P3_GEN_REG       0x2D700L

#define PROM21_SATA_PORT_ENABLE_REG  0x2471CL
#define PROM21_AHCI_PI_REG           0x2E006L
#define PROM21_AHCI_DEVSLP_REG       0x2E00AL
#define PROM21_AHCI_PCMD_CAP_REG     0x2E0C8L

#define PROM21_AHCI_CAP_REG          0x2E000L
  #define PROM21_AHCI_CAP_CCCS_REG     BIT7
  #define PROM21_AHCI_CAP_PSC_REG      BIT13
  #define PROM21_AHCI_CAP_SSC_REG      BIT14
  #define PROM21_AHCI_CAP_SALP_REG     BIT26

#define PROM21_XHCI_SVID_REG         0x2474A
#define PROM21_XHCI_SSID_REG         0x2475A
#define PROM21_AHCI_SVID_REG         0x2474C
#define PROM21_AHCI_SSID_REG         0x2475C
#define PROM21_USP_SVID_REG          0x24740
#define PROM21_USP_SSID_REG          0x24750

#define PT_INTL_ADDR_REG             0xE8             // xHCI PCI offset 0xE8 ~ 0xE9: Internal register access address
#define PT_INTL_READ_DATA_REG        0xE4             // xHCI PCI offset 0E4: Internal read data
#define PT_INTL_WRITE_DATA_REG       0xEA             // xHCI PCI offset 0EA: Internal write data

STATIC UINT32 r_warm_rst_assert[] = {
  0x19220,
  0x1A220,
  0x1B220,
  0x1C220,
  0x1D220,
  0x1E220
};

STATIC UINT32 r_force_superspeed[] = {
  0x19211,
  0x1A211,
  0x1B211,
  0x1C211,
  0x1D211,
  0x1E211
};

STATIC UINT32 hw_lpm_en[] = {
  0x1A58C,
  0x1C58C,
  0x1E58C
};

// Configuration values for SataClass
typedef enum {
  PTSataAhci = 0,                  ///< AHCI mode
  PTSataRaid                       ///< RAID mode
} SATA_CLASS;

VOID
FchProm21SetSataMode (
  IN        UINT32 XhciMmio
  );

VOID
FchSecProm21SetSataMode (
  IN        UINT32 XhciMmio
  );

VOID
FchProm21SetSataClassCode (
  IN        UINT32 XhciMmio,
  IN        UINT8  SubClass,
  IN        UINT16 DeviceId
);

VOID FchProm21XhciGen (
IN       UINT32   XhciMmio,
IN       BOOLEAN  Gen2En,
IN       UINT8    PortNum
);

VOID
FchProm21UsbCbsPei (
  IN      UINT32    TempMmio
  );

VOID
FchSecProm21UsbCbsPei (
  IN      UINT32    TempMmio
  );

VOID
FchProm21SataControllerEnable (
  IN      UINT32    XhciMmio
  );

VOID
FchProm21SataControllerDisable (
  IN      UINT32    XhciMmio
  );

VOID
FchProm21SataPortCtrl (
  IN       UINT32 XhciMmio,
  IN       BOOLEAN PortEn,
  IN       UINT8  PortNum
  );

VOID
FchProm21SataPortEnable (
  IN      UINT32    XhciMmio,
  IN      UINT8     PortNum
  );

VOID
FchProm21SataPortDisable (
  IN      UINT32    XhciMmio,
  IN      UINT8     PortNum
  );

VOID
FchProm21SetAhciDevslp (
  IN       UINT32 XhciMmio,
  IN       BOOLEAN Enable,
  IN       UINT8  PortNum
  );

VOID
FchProm21SetAhciCapReg (
  IN       UINT32 XhciMmio,
  IN       UINT32 Capreg,
  IN       BOOLEAN Enable
  );

VOID
FchProm21AhciPortSataSpeed (
  IN       UINT32 XhciMmio,
  IN       UINT8  PortNum,
  IN       UINT8  Speed
  );

VOID
FchProm21AhciPortHotplug (
  IN       UINT32 XhciMmio,
  IN       UINT8  PortNum,
  IN       BOOLEAN Enable
  );

VOID
FchProm21SataCbsPei (
  IN       UINT32  XhciMmio
  );

VOID
FchSecProm21SataCbsPei (
  IN       UINT32  XhciMmio
  );

VOID
FchPromontorySvidSsid (
  IN      UINT32    XhciMmio
  );

VOID
FchProm21DisableUnusedPciePorts (
  IN    UINT32   GppPcieAddress,
  IN    UINTN    PromNo
  );

EFI_STATUS
EFIAPI
FchProm21CbsPei (
  IN      CONST EFI_PEI_SERVICES        **PeiServices
  );

EFI_STATUS
EFIAPI
FchProm21CbsPeiInit (
  IN      EFI_PEI_FILE_HANDLE           FileHandle,
  IN      CONST EFI_PEI_SERVICES        **PeiServices
  );

#endif

