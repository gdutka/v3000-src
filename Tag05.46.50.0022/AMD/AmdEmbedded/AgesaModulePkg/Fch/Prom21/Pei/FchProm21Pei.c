/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Library/PciLib.h>

#include <Library/IdsLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Ppi/NbioEarlyLinkPpi.h>
#include <Filecode.h>

#include <Ppi/FchProm21InitPpi.h>
#include <Ppi/FchProm21FwLoadingPassPpi.h>
#include <Ppi/FchProm21FwChecksumFailPpi.h>
#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include "FchProm21Pei.h"

#define FILECODE FCH_PROM21_PEI_FCHPROM21PEI_FILECODE

STATIC PEI_PT_INIT__PPI mProm21FwLadingPassPpi = {
  AMD_PT_INIT_REVISION,
};

STATIC PEI_PT_INIT__PPI mProm21FwChecksumFailPpi = {
  AMD_PT_INIT_REVISION,
};

STATIC EFI_PEI_PPI_DESCRIPTOR mProm21FwLadingPassPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFchProm21FwLoadingPassPpiGuid,
  &mProm21FwLadingPassPpi
};

STATIC EFI_PEI_PPI_DESCRIPTOR mProm21FwChecksumFailPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFchProm21FwChecksumFailPpiGuid,
  &mProm21FwChecksumFailPpi
};

VOID
ScanSecXhciDidVid (
  IN      UINT8     GppBus,
  IN      UINT8     GppDev,
  IN      UINT8     GppFun,
  IN      UINT8     TempBusNum
  )
{
  UINT32            DidVid = 0;
  UINT32            PtDspBusData = 0;
  UINT32            SecPtUspBusData = 0;
  UINT32            SecPtDspBusData = 0;
  UINT32            GppPcieAddress = 0;

  DEBUG ((DEBUG_INFO, "    %a-Scan PT DSP [%02X:%02X:%02X]\n", __FUNCTION__, GppBus, GppDev, GppFun));
  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x00);            // Regx00 Device/Vendor ID Register
  if (DidVid == 0xFFFFFFFF) {
    // GPP bridge is not present
    DEBUG ((DEBUG_INFO, "    PT DSP [%02X:%02X:%02X] is not present\n", GppBus, GppDev, GppFun));
    return;
  }

  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x08);            // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    DEBUG ((DEBUG_INFO, "    PT DSP [%02X:%02X:%02X] is not a PCIE bridge\n", GppBus, GppDev, GppFun));
    return;
  }

  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x98);            // Regx98 PT DSP Slot Control and Status Register
  if ((DidVid & BIT22) == 0) {
    // Endpoint device is not present
    DEBUG ((DEBUG_INFO, "    PT DSP [%02X:%02X:%02X] endpoint device is not present\n", GppBus, GppDev, GppFun));
    return;
  }

  // Save PT DSP Primary, Secondary and Subordinate Bus Number
  PtDspBusData = PtPciRead32 (GppBus, GppDev, GppFun, 0x18);

  // Assign PT DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x18, GppBus);
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x19, TempBusNum);
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x1A, TempBusNum + 3);

  DidVid = PtPciRead32 (TempBusNum, 0, 0, 0x08);                  // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    // Restore PT DSP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, PtDspBusData);
    DEBUG ((DEBUG_INFO, "    SEC PT USP [%02X:%02X:%02X] is not a PCIE bridge\n", TempBusNum, 0, 0));
    return;
  }

  // Save SEC PT USP Primary, Secondary and Subordinate Bus Number
  SecPtUspBusData = PtPciRead32 (TempBusNum, 0, 0, 0x18);

  // Assign SEC PT USP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (TempBusNum, 0, 0, 0x18, TempBusNum);
  PtPciWrite8 (TempBusNum, 0, 0, 0x19, TempBusNum + 1);
  PtPciWrite8 (TempBusNum, 0, 0, 0x1A, TempBusNum + 3);

  DidVid = PtPciRead32 (TempBusNum + 1, SecXhciDspDev, 0, 0x08);  // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    // Restore SEC PT USP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (TempBusNum, 0, 0, 0x18, SecPtUspBusData);
    // Restore PT DSP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, PtDspBusData);
    DEBUG ((DEBUG_INFO, "    SEC XHCI DSP [%02X:%02X:%02X] is not a PCIE bridge\n", TempBusNum + 1, SecXhciDspDev, 0));
    return;
  }

  // Save SEC PT Xhci DSP Primary, Secondary and Subordinate Bus Number
  SecPtDspBusData = PtPciRead32 (TempBusNum + 1, SecXhciDspDev, 0, 0x18);

  // Assign SEC PT Xhci DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (TempBusNum + 1, SecXhciDspDev, 0, 0x18, TempBusNum + 1);
  PtPciWrite8 (TempBusNum + 1, SecXhciDspDev, 0, 0x19, TempBusNum + 2);
  PtPciWrite8 (TempBusNum + 1, SecXhciDspDev, 0, 0x1A, TempBusNum + 2);

  // Check Endpoint Device/Vendor ID
  DidVid = PtPciRead32 (TempBusNum + 2, 0, 0, 0);

  DEBUG ((DEBUG_INFO, "    EndPoint DidVid = 0x%08X\n", DidVid));

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) || (DidVid == PT21_XHCI_ID_L3) || \
      (DidVid == PT21_XHCI_ID_L4) ) {
    GppPcieAddress = (UINT32) PCI_LIB_ADDRESS (GppBus, GppDev, GppFun, 0);
    PcdSet8S (PcdChipsetVisableFlag0, (UINT8) (DidVid >> 16));
    PcdSet32S (PcdSecPT21XhciID, DidVid);
    PcdSet32S (PcdSecPT21GppPcieAddress, GppPcieAddress);
  }

  // Restore SEC PT XHCI DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (TempBusNum + 1, SecXhciDspDev, 0, 0x18, SecPtDspBusData);

  // Restore SEC PT USP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (TempBusNum, 0, 0, 0x18, SecPtUspBusData);

  // Restore PT DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, PtDspBusData);

  if (GppPcieAddress != 0) {
    DEBUG ((DEBUG_INFO, "    Found Second Promontory!\n"));
    DEBUG ((DEBUG_INFO, "    Set PcdChipsetVisableFlag0   = 0x%02X\n", PcdGet8 (PcdChipsetVisableFlag0)));
    DEBUG ((DEBUG_INFO, "    Set PcdSecPT21XhciID         = 0x%08X\n", PcdGet32 (PcdSecPT21XhciID)));
    DEBUG ((DEBUG_INFO, "    Set PcdSecPT21GppPcieAddress = 0x%08X\n", PcdGet32 (PcdSecPT21GppPcieAddress)));
  } else {
    DEBUG ((DEBUG_INFO, "    It's not Second Promontory!\n"));
  }

  return;
}

VOID
ScanXhciDidVid (
  IN      UINT8     GppBus,
  IN      UINT8     GppDev,
  IN      UINT8     GppFun,
  IN      UINT8     TempBusNum
  )
{
  UINT32            DidVid = 0;
  UINT32            GpioDidVid = 0;
  UINT32            GppBusData = 0;
  UINT32            UspBusData = 0;
  UINT32            DspBusData = 0;
  UINT32            GppPcieAddress = 0;
  UINT8             DspDevNum = 0;

  DEBUG ((DEBUG_INFO, "    %a-Scan GPP [%02X:%02X:%02X]\n", __FUNCTION__, GppBus, GppDev, GppFun));
  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x00);            // Regx00 Device/Vendor ID Register
  if (DidVid == 0xFFFFFFFF) {
    // GPP bridge is not present
    DEBUG ((DEBUG_INFO, "    GPP [%02X:%02X:%02X] is not present\n", GppBus, GppDev, GppFun));
    return;
  }

  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x08);            // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    DEBUG ((DEBUG_INFO, "    GPP [%02X:%02X:%02X] is not a PCIE bridge\n", GppBus, GppDev, GppFun));
    return;
  }

  DidVid = PtPciRead32 (GppBus, GppDev, GppFun, 0x70);            // Regx70 Slot Control and Status Register
  if ((DidVid & BIT22) == 0) {
    // Endpoint device is not present
    DEBUG ((DEBUG_INFO, "    GPP [%02X:%02X:%02X] endpoint device is not present\n", GppBus, GppDev, GppFun));
    return;
  }

  // Save GPP Primary, Secondary and Subordinate Bus Number
  GppBusData = PtPciRead32 (GppBus, GppDev, GppFun, 0x18);

  // Assign GPP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x18, GppBus);
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x19, TempBusNum);
  PtPciWrite8 (GppBus, GppDev, GppFun, 0x1A, TempBusNum + 7);

  DidVid = PtPciRead32 (TempBusNum, 0, 0, 0x08);                  // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    // Restore GPP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, GppBusData);
    DEBUG ((DEBUG_INFO, "    USP [%02X:%02X:%02X] is not a PCIE bridge\n", TempBusNum, 0, 0));
    return;
  }

  // Save PT USP Primary, Secondary and Subordinate Bus Number
  UspBusData = PtPciRead32 (TempBusNum, 0, 0, 0x18);

  // Assign PT USP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (TempBusNum, 0, 0, 0x18, TempBusNum);
  PtPciWrite8 (TempBusNum, 0, 0, 0x19, TempBusNum + 1);
  PtPciWrite8 (TempBusNum, 0, 0, 0x1A, TempBusNum + 7);

  DidVid = PtPciRead32 (TempBusNum + 1, XhciDspDev, 0, 0x08);      // Regx08 Class Code/Revision ID Register
  if ((DidVid >> 16) != 0x0604) {
    // It's not a PCIE bridge
    // Restore PT USP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (TempBusNum, 0, 0, 0x18, UspBusData);
    // Restore GPP Primary, Secondary and Subordinate Bus Number
    PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, GppBusData);
    DEBUG ((DEBUG_INFO, "    XHCI DSP [%02X:%02X:%02X] is not a PCIE bridge\n", TempBusNum + 1, XhciDspDev, 0));
    return;
  }

  // Save PT Xhci DSP Primary, Secondary and Subordinate Bus Number
  DspBusData = PtPciRead32 (TempBusNum + 1, XhciDspDev, 0, 0x18);

  // Assign PT Xhci DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite8 (TempBusNum + 1, XhciDspDev, 0, 0x18, TempBusNum + 1);
  PtPciWrite8 (TempBusNum + 1, XhciDspDev, 0, 0x19, TempBusNum + 2);
  PtPciWrite8 (TempBusNum + 1, XhciDspDev, 0, 0x1A, TempBusNum + 2);

  // Check Endpoint Device/Vendor ID
  DidVid     = PtPciRead32 (TempBusNum + 2, 0, 0, 0);
  GpioDidVid = PtPciRead32 (TempBusNum, 0, 0, 0);
  DEBUG ((DEBUG_INFO, "    EndPoint DidVid = 0x%08X\n", DidVid));

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) || (DidVid == PT21_XHCI_ID_L3) || \
      (DidVid == PT21_XHCI_ID_L4) ) {
    GppPcieAddress = (UINT32) PCI_LIB_ADDRESS (GppBus, GppDev, GppFun, 0);
    PcdSet16S (PcdChipsetIdentifiedId, (UINT16) (DidVid >> 16));
    PcdSet32S (PcdPT21XhciID, DidVid);
    PcdSet32S (PcdPT21GpioID, GpioDidVid);
    PcdSet32S (PcdPT21GppPcieAddress, GppPcieAddress);

    for (DspDevNum = 0; DspDevNum < PROM21_MAX_DSP_NUMBER; DspDevNum++) {
      ScanSecXhciDidVid (TempBusNum + 1, DspDevNum, 0, TempBusNum + 4);
      if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
        break;
      }
    }
  }

  // Restore PT XHCI DSP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (TempBusNum + 1, XhciDspDev, 0, 0x18, DspBusData);

  // Restore PT USP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (TempBusNum, 0, 0, 0x18, UspBusData);

  // Restore GPP Primary, Secondary and Subordinate Bus Number
  PtPciWrite32 (GppBus, GppDev, GppFun, 0x18, GppBusData);

  if (GppPcieAddress != 0) {
    DEBUG ((DEBUG_INFO, "    Found Promontory!\n"));
    DEBUG ((DEBUG_INFO, "    Set PcdChipsetIdentifiedId = 0x%04X\n", PcdGet16 (PcdChipsetIdentifiedId)));
    DEBUG ((DEBUG_INFO, "    Set PcdPT21XhciID        = 0x%08X\n", PcdGet32 (PcdPT21XhciID)));
    DEBUG ((DEBUG_INFO, "    Set PcdPT21GpioID        = 0x%08X\n", PcdGet32 (PcdPT21GpioID)));
    DEBUG ((DEBUG_INFO, "    Set PcdPT21GppPcieAddress  = 0x%08X\n", PcdGet32 (PcdPT21GppPcieAddress)));
  } else {
    DEBUG ((DEBUG_INFO, "    It's not Promontory!\n"));
  }

  return;
}

VOID
GetIohcBridgeControl (
  IN      UINT8     GppBus,
  IN      UINT8     GppDev,
  IN      UINT8     GppFun,
  IN      UINT8     NbioNum
  )
{
  UINT32            IohcBridgeCntl = 0;
  UINT32            Index;

  DEBUG ((DEBUG_INFO, "  %a-GPP [%02X:%02X:%02X] NBIO[%d]\n", __FUNCTION__, GppBus, GppDev, GppFun, NbioNum));

  if (SocFamilyIdentificationCheck (F17_ZP_RAW_ID)) {
    // #define F17_ZP_RAW_ID   0x00800F00ul
    // 0000_[4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND=13B3_0000h
    STATIC UINT8      ZpIohcBrightNumbering[16]  = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21};
    for (Index = 0; Index < 16; Index++) {
      if (ZpIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_ZP IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 16) {
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F17_SSP_RAW_ID) || SocFamilyIdentificationCheck (F19_GN_RAW_ID)) {
    // #define F17_SSP_RAW_ID  0x00830F00ul
    // #define F19_GN_RAW_ID   0x00A00F00ul
    // 0000_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND0=13B3_0000h
    // 0000_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND1=13C3_0000h
    // 0000_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND2=13D3_0000h
    // 0000_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND3=13E3_0000h
    STATIC UINT8      SspIohcBrightNumbering[18] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21,
                                                    0x29, 0x2A};
    for (Index = 0; Index < 18; Index++) {
      if (SspIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_SSP / F19_GN IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 18) {
      Index <<= 10;
      IohcBridgeCntl = (UINT32) NbioNum;
      IohcBridgeCntl <<= 20;
      IohcBridgeCntl += Index;
      IohcBridgeCntl += 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID)) {
    // #define F17_MTS_RAW_ID  0x00870F00ul
    // #define F19_VMR_RAW_ID  0x00A20F00ul
    // 0000_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND3=13E3_0000h
    STATIC UINT8      MtsIohcBrightNumbering[18] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21,
                                                    0x29, 0x2A};
    for (Index = 0; Index < 18; Index++) {
      if (MtsIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_MTS / F19_VMR IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 18) {
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13E31004;
    }
  } else if (SocFamilyIdentificationCheck (F17_RV_RAW_ID)  || SocFamilyIdentificationCheck (F17_RV2_RAW_ID) ||
             SocFamilyIdentificationCheck (F17_PCO_RAW_ID) || SocFamilyIdentificationCheck (F17_VN_RAW_ID) ||
             SocFamilyIdentificationCheck (F17_MR_RAW_ID)) {
    // #define F17_RV_RAW_ID   0x00810F00ul
    // #define F17_RV2_RAW_ID  0x00820F00ul
    // #define F17_PCO_RAW_ID  0x00810F80ul
    // #define F17_VN_RAW_ID   0x00890F00ul
    // #define F17_MR_RAW_ID   0x00890F80ul
    // 0000_[28,24,20,1C,18,14,10]04]; IOHCDEVIND=13B3_0000h
    STATIC UINT8      RvIohcBrightNumbering[7]   = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    for (Index = 0; Index < 7; Index++) {
      if (RvIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_RV / F17_RV2 / F17_PCO / F17_VN / F17_MR IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 7) {
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F17_RN_RAW_ID) || SocFamilyIdentificationCheck (F17_LCN_RAW_ID) ||
             SocFamilyIdentificationCheck (F19_CZN_RAW_ID)) {
    // #define F17_RN_RAW_ID   0x00860F00ul
    // #define F17_LCN_RAW_ID  0x00860F80ul
    // #define F19_CZN_RAW_ID  0x00A50F00ul
    // 003[48,44,40,3C,38,34,30]04,0031[8,4,0]04]; IOHC0NBCFG=13B0_0000h
    STATIC UINT8      RnIohcBrightNumbering[10]  = {0x09, 0x0A, 0x0B,
                                                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    for (Index = 0; Index < 10; Index++) {
      if (RnIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_RN / F17_LCN / F19_CZN IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 10) {
      if (Index >= 3) {
        Index += 5;
      }
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F17_MDN_RAW_ID) || SocFamilyIdentificationCheck (F19_RMB_RAW_ID)) {
    // #define F17_MDN_RAW_ID  0x008A0F00ul
    // #define F19_RMB_RAW_ID  0x00A40F00ul
    // 0003_[44,40,3C,38,34,30]04,0003_[20,1C,18,14,10]04]; IOHC0NBCFG=13B0_0000h
    STATIC UINT8      RmbIohcBrightNumbering[11] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D,
                                                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
    for (Index = 0; Index < 11; Index++) {
      if (RmbIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F17_MDN / F19_RMB IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 11) {
      if (Index >= 5) {
        Index += 3;
      }
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F19_RS_RAW_ID) || SocFamilyIdentificationCheck (F19_RSDN_RAW_ID) ) {
    // #define F19_RS_RAW_ID   0x00A10F00ul
    // 0000_[64,60,5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND0=13B3_0000h
    // 0000_[64,60,5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND1=13C3_0000h
    // 0000_[64,60,5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND2=13D3_0000h
    // 0000_[64,60,5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND3=13E3_0000h
    STATIC UINT8      RsIohcBrightNumbering[22]  = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21, 0x22,
                                                    0x29, 0x2A, 0x2B, 0x2C};
    for (Index = 0; Index < 22; Index++) {
      if (RsIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F19_RS IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 22) {
      Index <<= 10;
      IohcBridgeCntl = (UINT32) NbioNum;
      IohcBridgeCntl <<= 20;
      IohcBridgeCntl += Index;
      IohcBridgeCntl += 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F19_BA_RAW_ID)) {
    // #define F19_BA_RAW_ID   0x00A30F00ul
    // 0000_[5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND0=13B3_0000h
    // 0000_[5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND1=13C3_0000h
    // 0000_[5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND2=13D3_0000h
    // 0000_[5C,58,54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHCDEVIND3=13E3_0000h
    STATIC UINT8      BaIohcBrightNumbering[20]  = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21,
                                                    0x29, 0x2A, 0x2B, 0x2C};
    for (Index = 0; Index < 20; Index++) {
      if (BaIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F19_BA IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 20) {
      Index <<= 10;
      IohcBridgeCntl = (UINT32) NbioNum;
      IohcBridgeCntl <<= 20;
      IohcBridgeCntl += Index;
      IohcBridgeCntl += 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F19_RPL_RAW_ID)) {
    // #define F19_RPL_RAW_ID  0x00A60F00ul
    // 13B3_[54,50,4C,48,44,40,3C,38,34,30,2C,28,24,20,1C,18,14,10]04]; IOHC=0000_0000h
    STATIC UINT8      RplIohcBrightNumbering[18] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x21, 0x22};
    for (Index = 0; Index < 18; Index++) {
      if (RplIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F19_RPL IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 18) {
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  } else if (SocFamilyIdentificationCheck (F19_PHX_RAW_ID)) {
    // #define F19_PHX_RAW_ID  0x00A70F00ul
    // 00037004,0003_5[C,8,4,0]04,0003_[44,40,3C,38,34,30]04,0003_[24,20,1C,18,14,10]04]; IOHC0NBCFG=13B0_0000h
    STATIC UINT8      PhxIohcBrightNumbering[17] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                                    0x19, 0x1A, 0x1B, 0x1C,
                                                    0x21};
    for (Index = 0; Index < 17; Index++) {
      if (PhxIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        DEBUG ((DEBUG_INFO, "    F19_PHX IohcBrightNumbering Index = %d\n", Index));
        break;
      }
    }
    if (Index < 17) {
      if (Index >= 6) {
        Index += 2;
        if (Index >= 14) {
          Index += 2;
          if (Index >= 20) {
            Index += 4;
          }
        }
      }
      Index <<= 10;
      IohcBridgeCntl = Index + 0x13B31004;
    }
  }

  if (IohcBridgeCntl > 0) {
    PcdSet32S (PcdPT21IohcBridgeCntl, IohcBridgeCntl);
    DEBUG ((DEBUG_INFO, "    Set PcdPT21IohcBridgeCntl = 0x%08X\n", IohcBridgeCntl));
  }
}

EFI_STATUS
SearchProm (
  IN EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                  Status;
  EARLY_LINK_STATUS           NbioPtLinkData;
  PEI_AMD_NBIO_EARLY_LINK_PPI *Nbio_Early_Link_Ppi;
  UINT8             GppBus = 0;
  UINT8             GppDev = 0;
  UINT8             GppFun = 0;
  UINT8             NbioNum = 0;
  UINT8             TempBusNum;
  UINT8             RootBridgeNum = 0;
  UINT32            ForceGppPcieAddress;

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));
  TempBusNum = PcdGet8 (PcdPT21TempBusNum);
  ForceGppPcieAddress = PcdGet32 (PcdPT21ForceGppPcieAddress);

  if (ForceGppPcieAddress == 0) {
    // Locate NBIO Ppi to get PT Root port information if it's SSP Platform.
    Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gAmdNbioEarlyLinkPpiGuid,
                            0,
                            NULL,
                            (VOID **)&Nbio_Early_Link_Ppi
                            );
    if (Status == EFI_SUCCESS) {
      Status = Nbio_Early_Link_Ppi->GetEarlyLinkConfig (&NbioPtLinkData);
      if (Status == EFI_SUCCESS) {
        if (NbioPtLinkData.EarlyLinkStatus) {
          NbioNum       = NbioPtLinkData.PhysicalRootBridge;
          RootBridgeNum = NbioPtLinkData.LogicalRootBridge;
          GppBus        = NbioPtLinkData.RootPortBus;
          GppDev        = NbioPtLinkData.RootPortDevice;
          GppFun        = NbioPtLinkData.RootPortFunction;
          TempBusNum    += GppBus;
          PcdSet8S (PcdPT21RootBridgeNum, RootBridgeNum);
          DEBUG ((DEBUG_INFO, "  [%02X:%02X:%02X]", GppBus, GppDev, GppFun));
          DEBUG ((DEBUG_INFO, "  NbioNum = %x, RootBridgeNum = %x, TempBusNum = %x\n", NbioNum, RootBridgeNum, TempBusNum));

          ScanXhciDidVid (GppBus, GppDev, GppFun, TempBusNum);

          GetIohcBridgeControl (GppBus, GppDev, GppFun, NbioNum);
        }
      }
    }
  } else {
    NbioNum       = (UINT8) ((ForceGppPcieAddress >> 20) & 0xFF);
    RootBridgeNum = (UINT8) ((ForceGppPcieAddress >> 20) & 0xFF);
    GppBus        = (UINT8) ((ForceGppPcieAddress >> 20) & 0xFF);
    GppDev        = (UINT8) ((ForceGppPcieAddress >> 15) & 0x1F);
    GppFun        = (UINT8) ((ForceGppPcieAddress >> 12) & 0x07);
    TempBusNum    += GppBus;
    PcdSet8S (PcdPT21RootBridgeNum, RootBridgeNum);
    DEBUG ((DEBUG_INFO, "  Force PT21 GPP to [%02X:%02X:%02X]", GppBus, GppDev, GppFun));
    DEBUG ((DEBUG_INFO, "  Force PT21 GPP to NbioNum = %x, RootBridgeNum = %x, TempBusNum = %x\n", NbioNum, RootBridgeNum, TempBusNum));

    ScanXhciDidVid (GppBus, GppDev, GppFun, TempBusNum);

    GetIohcBridgeControl (GppBus, GppDev, GppFun, NbioNum);
  }

  if (PcdGet32 (PcdPT21XhciID) == 0) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/*********************************************************************************
 * Name: FchProm21PeiInit
 *
 * Description:
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21PeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                Status;
  FCH_PROM21_PEI_PRIVATE    *FchProm21PeiPrivate;
  EFI_PEI_PPI_DESCRIPTOR    *PpiListFchProm21PeiInit;

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  Status = SearchProm ((EFI_PEI_SERVICES **)PeiServices);
  if (Status == EFI_DEVICE_ERROR) {
    DEBUG ((DEBUG_INFO, "PT-PEI-%a(%d)-End-ERROR: Promontory is NOT FOUND!\n", __FUNCTION__, __LINE__));
    return EFI_UNSUPPORTED;
  }

  // Init FCH_PROM21_PEI_PRIVATE
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (FCH_PROM21_PEI_PRIVATE),
                             &FchProm21PeiPrivate
                             );
  ASSERT_EFI_ERROR ( Status);

  FchProm21PeiPrivate->Signature                    = FCH_PROM21_PEI_PRIVATE_DATA_SIGNATURE;
  FchProm21PeiPrivate->FchProm21InitPpi.Revision    = FCH_PROM21_INIT_PPI_REV;

  // Allocate memory for the PPI descriptor
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (EFI_PEI_PPI_DESCRIPTOR),
                             &PpiListFchProm21PeiInit
                             );
  ASSERT_EFI_ERROR ( Status);

  PpiListFchProm21PeiInit->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListFchProm21PeiInit->Guid  = &gAmdFchProm21InitPpiGuid;
  PpiListFchProm21PeiInit->Ppi   = &FchProm21PeiPrivate->FchProm21InitPpi;

  Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             PpiListFchProm21PeiInit
                             );
  ASSERT_EFI_ERROR ( Status);

  Status = AmdPTPlusFwLoadPei ((EFI_PEI_SERVICES **)PeiServices);

  if (Status == EFI_SUCCESS) {
    // Publish PT-Plus FW init done Ppi
    DEBUG ((EFI_D_INFO, "  Publish Prom21 FW Loading Pass Ppi!\n"));
    Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             &mProm21FwLadingPassPpiList
                             );
    ASSERT_EFI_ERROR (Status);
  } else if (Status == EFI_UNSUPPORTED) {
    // Publish PT-Plus FW checksum fail Ppi
    DEBUG ((EFI_D_INFO, "  Publish Prom21 FW Checksum Fail Ppi!\n"));
    Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             &mProm21FwChecksumFailPpiList
                             );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

