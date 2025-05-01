/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include <Library/DebugLib.h>

#include "FchPlatform.h"
#include "Filecode.h"
#include "GnbDxio.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>

#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTCIOSERVICE_FILECODE

/**
 * @brief FchKTUsb4PhyLoad  -  USB4 PHY loading
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 */
VOID
FchKTUsb4PhyLoad (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PhyLoad is started!\n");

  // USB4 Router0 PHY
  if (LocalCfgPtr->Usb4Host[0].Usb4InitEnable) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0600003);
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PhyLoad enable USB4 Router0 PHY FW loading!\n");
  }
  // USB4 Router1 PHY
  if (LocalCfgPtr->Usb4Host[1].Usb4InitEnable) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0800003);
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PhyLoad enable USB4 Router1 PHY FW loading!\n");
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PhyLoad is completed!\n");
}

VOID
FchKTUsb4PdInterruptMode (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PdInterruptMode is started!\n");

  // BIT2 of Interrupt mode for USB4 Router 0
  if ((LocalCfgPtr->Usb4Host[0].Usb4InitEnable) && (LocalCfgPtr->PdInterruptModeEn & BIT2)) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xE0600001);
  }

  // BIT3 of Interrupt mode for USB4 Router 1
  if ((LocalCfgPtr->Usb4Host[1].Usb4InitEnable) && (LocalCfgPtr->PdInterruptModeEn & BIT3)) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xE0800001);
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PdInterruptMode is completed!\n");
}

/**
 * @brief FchKTUsb4PcieDisable - Disable USB4-PCIe Controller
 *
 *
 * @param[in] PcieController USB4 PCIe Controller.
 *
 */
VOID
FchKTUsb4PcieDisable (
  IN  UINT32   PcieController
  )
{
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PcieDisable is started!\n");

  // BridgeDis + CfgDis
  FchSmnRW (0, FCH_KT_PCIE2_BRIDGE_CNTL + 0x400 * PcieController, ~(UINT32) (BIT0 + BIT2), BIT0 + BIT2, NULL);

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PcieDisable is completed!\n");
}

/**
 * @brief FchKTUsb4InitMessage  -  SMU Service USB Init
 * Request
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
BOOLEAN
FchKTUsb4InitMessage (
  IN  UINT32    DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  BOOLEAN                         Status;
  PCI_ADDR                        NbioPciAddress;
  UINT32                          SmuArg[6];
  FCH_RESET_DATA_BLOCK            *LocalCfgPtr;
  FCH_KT_USB_OEM_PLATFORM_TABLE   *PlatformUsbConfigureTable;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  PlatformUsbConfigureTable = (FCH_KT_USB_OEM_PLATFORM_TABLE *)(FchDataPtr->OemUsbConfigurationTablePtr);

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitMessage is started!\n");

  Status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);

  //
  // BIOSSMC_MSG_UsbInit
  //
  //Bit 0:0  - USB0 host controller
  //Bit 1:1  - USB1 host controller
  //Bit 2:2  - USB2 host controller
  //Bit 3:3  - USB3 host controller
  //Bit 4:4  - USB4 host controller
  //Bit 16:16 - USB4 router 0
  //Bit 17:17 - USB4 router 1
  //Bit 24:24 - USB4 PHY 0
  //Bit 25:25 - USB4 PHY 1
  SmuArg[0] = 0;
  SmuArg[2] = 0;
  // USB4 Router0
  if (LocalCfgPtr->Usb4Host[0].Usb4InitEnable) {
    if (LocalCfgPtr->Usb4Host[0].Usb4PhyEnable != 0) {
      FchDataPtr->pUsbInitData->Enable.usb4_phy_0 = 1;
    }
    if (LocalCfgPtr->Usb4Host[0].Usb4HostEnable) {
      FchDataPtr->pUsbInitData->Enable.usb4_rt_0 = 1;
    } else {
      FchKTUsb4PcieDisable (0);
    }
    if ((LocalCfgPtr->Usb4Host[0].Usb3HCDisable & BIT0) == 0) {
      FchDataPtr->pUsbInitData->Enable.usb_hc_3 = 1;
    } else {
      FchSmnRW (0, FCH_KT_HC3_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);
    }
    FchDataPtr->pUsbInitData->ComboPhyStaticConfig.usb_hc_3 = PlatformUsbConfigureTable->ComboPhyStaticConfig[0] & 0x0F;
  }

  // USB4 Router1
  if (LocalCfgPtr->Usb4Host[1].Usb4InitEnable) {
    if (LocalCfgPtr->Usb4Host[1].Usb4PhyEnable != 0) {
      FchDataPtr->pUsbInitData->Enable.usb4_phy_1 = 1;
    }
    if (LocalCfgPtr->Usb4Host[1].Usb4HostEnable) {
      FchDataPtr->pUsbInitData->Enable.usb4_rt_1 = 1;
    } else {
      FchKTUsb4PcieDisable (1);
    }
    if ((LocalCfgPtr->Usb4Host[1].Usb3HCDisable & BIT0) == 0) {
      FchDataPtr->pUsbInitData->Enable.usb_hc_4 = 1;
    } else {
      FchSmnRW (0, FCH_KT_HC7_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);
    }
    FchDataPtr->pUsbInitData->ComboPhyStaticConfig.usb_hc_4 = PlatformUsbConfigureTable->ComboPhyStaticConfig[1] & 0x0F;
  }

//  if (NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_Usb4Init, SmuArg,0)) {
//    Status = TRUE;
//  }

  return Status;
}

/**
 * FchKTUsb4DisablePort  -  USB4 xHC Disable Port Control
 *
 *                          RMB B0 USB ports
 *                             HC3: 1 USB3 + 1 USB2
 *                             HC4: 1 USB3 + 1 USB2
 */
VOID
FchKTUsb4DisablePort (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      Usb3PortDisable;
  UINT32                      Usb2PortDisable;
  UINT32                      UsbPortDisable;

  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  if (!LocalCfgPtr->DisableXhciPortLate) {
      Usb3PortDisable = LocalCfgPtr->XhciUsb3PortDisable;
      Usb2PortDisable = LocalCfgPtr->XhciUsb2PortDisable;

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4DisablePort is started!\n");

      // Port0-1 : Controller3
      Usb3PortDisable = Usb3PortDisable >> 6;
      Usb2PortDisable = Usb2PortDisable >> 12;
      UsbPortDisable = (Usb2PortDisable & 0x01) + ((Usb3PortDisable & 0x01) << 16);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_DISABLE0, 0x00010001, UsbPortDisable, 0x01300001);

      // Port0-1 : Controller4
      Usb3PortDisable = Usb3PortDisable >> 2;
      Usb2PortDisable = Usb2PortDisable >> 4;
      UsbPortDisable = (Usb2PortDisable & 0x01) + ((Usb3PortDisable & 0x01) << 16);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_DISABLE0, 0x00010001, UsbPortDisable, 0x01400001);

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4DisablePort is completed!\n");
  }
}

/**
 * FchKTUsb4XhciPortForceGen1  -  Port Force Gen1
 *
 *
 *
 */
VOID
FchKTUsb4XhciPortForceGen1 (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT8                       UsbPortForceGen1;
  UINT32                      DW0_Index;
  UINT32                      DW1_Mask;
  UINT32                      DW2_Data;
  UINT32                      DW3_Op_Group;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4XhciPortForceGen1 is started!\n");

  //Controller0
  UsbPortForceGen1  = (LocalCfgPtr->Usb3PortForceGen1) >> 4;
  UsbPortForceGen1 &= 0x1;
  if (UsbPortForceGen1 != 0) {
    //Controller0 Port Control
    DW0_Index    = FCH_KT_USB_PORT_CONTROL;
    DW1_Mask     = 0x1 << 16;
    DW2_Data     = (UINT32) (UsbPortForceGen1 << 16);
    DW3_Op_Group = 0x01300001;
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }

  //Controller1
  UsbPortForceGen1  = (LocalCfgPtr->Usb3PortForceGen1) >> 5;
  UsbPortForceGen1 &= 0x1;
  if (UsbPortForceGen1 != 0) {
    //Controller1 Port Control
    DW0_Index    = FCH_KT_USB_PORT_CONTROL;
    DW1_Mask     = 0x1 << 16;
    DW2_Data     = (UINT32) (UsbPortForceGen1 << 16);
    DW3_Op_Group = 0x01400001;
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4XhciPortForceGen1 is completed!\n");
}

/**
 * FchKTUsb4Gen3Support  -  USB4 Gen3 Support configuration
 *
 *
 *
 */
VOID
FchKTUsb4Gen3Support (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      Gen3Support;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4Gen3Support is started!\n");

  // Controller3
  Gen3Support = LocalCfgPtr->Usb4Host[0].Usb4Gen3Support & BIT0;
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_OVRD_PORT_SB_LL_CONFIGURATION,
    0x00002000,
    Gen3Support << 13,
    0x01000003 | FCH_KT_USB_CONFIG_USB4RT0 << 20);
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_OVRD_ADP_CS,
    0x00040000,
    Gen3Support << 18,
    0x01000003 | FCH_KT_USB_CONFIG_USB4RT0 << 20);

  // Controller4
  Gen3Support = LocalCfgPtr->Usb4Host[1].Usb4Gen3Support & BIT0;
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_OVRD_PORT_SB_LL_CONFIGURATION,
    0x00002000,
    Gen3Support << 13,
    0x01000003 | FCH_KT_USB_CONFIG_USB4RT1 << 20);

  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_OVRD_ADP_CS,
    0x00040000,
    Gen3Support << 18,
    0x01000003 | FCH_KT_USB_CONFIG_USB4RT1 << 20);

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4Gen3Support is completed!\n");
}

/**
 * FchKTUsb4OverCurrent  -  USB4 OC Pin Mapping
 *
 *
 *
 */
VOID
FchKTUsb4OverCurrent (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      OverCurrentMap;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OverCurrent is started!\n");

  // Port0-1 : Controller0
  OverCurrentMap = LocalCfgPtr->Xhci3OCpinSelect;
  OverCurrentMap = (OverCurrentMap ) & 0xFF;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_OCMAPPING0, 0x000000FF, OverCurrentMap, 0x01300001);

  // Port0-1 : Controller1
  OverCurrentMap = LocalCfgPtr->Xhci4OCpinSelect;
  OverCurrentMap = (OverCurrentMap ) & 0xFF;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_OCMAPPING0, 0x000000FF, OverCurrentMap, 0x01400001);
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OverCurrent is completed!\n");
}

/**
 * @brief FchKTUsb4TunnelDisable - Disable Protocol Tunnel
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4TunnelDisable (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32               RegValue32;
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4TunnelDisable is started!\n");

  //
  // USB4 RT0
  //
  RegValue32 = 0;
  if ((LocalCfgPtr->Usb4Host[0].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[0].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[0].Usb4Usb3TunnelingDisable & BIT0) {
      RegValue32 |= BIT0;
    }
    if (LocalCfgPtr->Usb4Host[0].Usb4PcieTunnelingDisable & BIT0) {
      RegValue32 |= BIT8;
      FchKTUsb4PcieDisable (0);
    }
    if (LocalCfgPtr->Usb4Host[0].Usb4DPTunnelingDisable) {
      RegValue32 |= BIT16;
    }
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USB4_TUNNEL_DISABLE_CNTR0,
      0x00010101,
      RegValue32,
      0x01000003 | FCH_KT_USB_CONFIG_USB4RT0 << 20
      );
  }
  //
  // USB4 RT1
  //
  RegValue32 = 0;
  if ((LocalCfgPtr->Usb4Host[1].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[1].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[1].Usb4Usb3TunnelingDisable & BIT0) {
      RegValue32 |= BIT0;
    }
    if (LocalCfgPtr->Usb4Host[1].Usb4PcieTunnelingDisable & BIT0) {
      RegValue32 |= BIT8;
      FchKTUsb4PcieDisable (1);
    }
    if (LocalCfgPtr->Usb4Host[1].Usb4DPTunnelingDisable) {
    RegValue32 |= BIT16;
    }
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USB4_TUNNEL_DISABLE_CNTR0,
      0x00010101,
      RegValue32,
      0x01000003 | FCH_KT_USB_CONFIG_USB4RT1 << 20
      );
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4TunnelDisable is completed!\n");
}

/**
 * @brief FchKTUsb4AdapterHide - Hide Adapter
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4AdapterHide (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32               RegValue32;
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4AdapterHide is started!\n");

  //
  // USB4 RT0
  //
  RegValue32 = 0;
  if ((LocalCfgPtr->Usb4Host[0].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[0].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[0].Usb4Usb3AdpHidden & BIT0) {
      RegValue32 |= BIT0;
    }
    if (LocalCfgPtr->Usb4Host[0].Usb4PcieAdpHidden & BIT0) {
      RegValue32 |= BIT8;
      FchKTUsb4PcieDisable (0);
    }

    RegValue32 |= (UINT32) (LocalCfgPtr->Usb4Host[0].Usb4DPAdpHidden & (BIT0 + BIT1)) << 16;

    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USB4_ADP_HIDE_CNTR0,
      0x00030101,
      RegValue32,
      0x01000003 | FCH_KT_USB_CONFIG_USB4RT0 << 20
      );
  }

  //
  // USB4 RT1
  //
  RegValue32 = 0;
  if ((LocalCfgPtr->Usb4Host[1].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[1].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[1].Usb4Usb3AdpHidden & BIT0) {
      RegValue32 |= BIT0;
    }
    if (LocalCfgPtr->Usb4Host[1].Usb4PcieAdpHidden & BIT0) {
      RegValue32 |= BIT8;
      FchKTUsb4PcieDisable (1);
    }

    RegValue32 |= (UINT32) (LocalCfgPtr->Usb4Host[1].Usb4DPAdpHidden & (BIT0 + BIT1)) << 16;

    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USB4_ADP_HIDE_CNTR0,
      0x00030101,
      RegValue32,
      0x01000003 | FCH_KT_USB_CONFIG_USB4RT1 << 20
      );
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4AdapterHide is completed!\n");
}

/**
 * @brief FchKTUsb4Tbt3Disable - Disable USB4 TBT3 compatibility
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4Tbt3Disable (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4Tbt3Disable is started!\n");

  //
  // USB4 RT0
  //
  if ((LocalCfgPtr->Usb4Host[0].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[0].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[0].Usb4Tbt3NotSupport) {
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB4_TBT3_NOT_SUPPORT_CNTR0,
        0x00000001,
        0x00000001,
        0x01000003 | FCH_KT_USB_CONFIG_USB4RT0 << 20
        );
    }
  }

  //
  // USB4 RT1
  //
  if ((LocalCfgPtr->Usb4Host[1].Usb4InitEnable) && (LocalCfgPtr->Usb4Host[1].Usb4HostEnable)) {
    if (LocalCfgPtr->Usb4Host[1].Usb4Tbt3NotSupport) {
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB4_TBT3_NOT_SUPPORT_CNTR0,
        0x00000001,
        0x00000001,
        0x01000003 | FCH_KT_USB_CONFIG_USB4RT1 << 20
        );
    }
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4Tbt3Disable is completed!\n");
}

/**
 * @brief FchKTUsb4TxFFEConfigure - TxFFE configuration
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4TxFFEConfigure (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32               DW0;
  UINT32               DW1;
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4TxFFEConfigure is started!\n");

  //
  // USB4 RT0
  //
  DW0 = 0;
  DW1 = 0;
  if (LocalCfgPtr->Usb4Host[0].Usb4TxFFEMode == 1) {
    // FW mode w/o preset value override
    DW1 = 0x01;
  } else if (LocalCfgPtr->Usb4Host[0].Usb4TxFFEMode == 2) {
    // FW mode with preset value override
    DW1 = 0x02;
    DW0 = (UINT32) LocalCfgPtr->Usb4Host[0].TxFFEPreSetValue;
  }

  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    DW0,
    DW1,
    0x00000000,
    0xD0000000 | (FCH_KT_USB_CONFIG_USB4RT0 << 20)
    );

  //
  // USB4 RT1
  //
  DW0 = 0;
  DW1 = 0;
  if (LocalCfgPtr->Usb4Host[1].Usb4TxFFEMode == 1) {
    // FW mode w/o preset value override
    DW1 = 0x01;
  } else if (LocalCfgPtr->Usb4Host[1].Usb4TxFFEMode == 2) {
    // FW mode with preset value override
    DW1 = 0x02;
    DW0 = (UINT32) LocalCfgPtr->Usb4Host[1].TxFFEPreSetValue;
  }

  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    DW0,
    DW1,
    0x00000000,
    0xD0000000 | (FCH_KT_USB_CONFIG_USB4RT1 << 20)
    );

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4TxFFEConfigure is completed!\n");
}

/**
 * @brief FchKTUsb4PlatformIndependentRegInitRt - Platform-Independent Register Configuration per Router.
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Router     USB4 Router number to program.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4PlatformIndependentRegInitRt (
  IN  UINT32   DieBusNum,
  IN  UINT8    Router,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PlatformIndependentRegInitRt is started!\n");
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Programming Platform Independent Register for Router%x!\n", Router);
  //
  // Group 1: PHYS5_REG_PRE_RESET
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 1: PHYS5_REG_PRE_RESET!\n");
  // DWCUSB4UPCSX4NSPIPETCAREGS::TCA_CLK_RST
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TCA_CLK_RST,
    0x00000001,
    0x00000001,
    0x01000001 | (FCH_KT_USB_CONFIG_USB4PHY0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // DWCUSB4UPCSX4NSPIPETCAREGS::TCA_CTRLSYNCMODE_CFG1
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TCA_CTRLSYNCMODE_CFG1,
    0x000FFFFF,
    0x0000A875,
    0x01000001 | (FCH_KT_USB_CONFIG_USB4PHY0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // FCHUSB4PDSLVI2C::USB_PD_Interrupt_Mask
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_USB_PD_Interrupt_Mask,
    0x00003801,
    0x00003801,
    0x01000001 | (FCH_KT_USB_CONFIG_USB4PHY0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

/* removed USB_PD_Slave_Control // FCHPDSLVI2C::USB_PD_Slave_Control
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_USB_PD_Slave_Control,
    0x00100000,
    0x00100000,
    0x01000001 | (FCH_KT_USB_CONFIG_USB4PHY0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );
*/
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 1 done!\n");

  //
  // Group 2: PHYINNER_REG_POST_FW
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 2: PHYINNER_REG_POST_FW!\n");

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 2 done!\n");

  //
  // Group 3: USB4RTS0_REG_PRE_RESET
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 3: USB4RTS0_REG_PRE_RESET!\n");
/* remove Group 3
  // CIO::SDXCIOTLS0CFGBLK::CIO_P0_ECO_Dummy0_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CIO_P0_ECO_Dummy0_CNTR0,
    0x0000000F,
    0x0000000F,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path1_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path1_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path2_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path2_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path3_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path3_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path4_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path4_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path5_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path5_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_Path6_ConfigurationB_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_Path6_ConfigurationB_CNTR0,
    0x000000FF,
    0x00000001,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0ADPCFGBLK::CFG_CMD_FSM_CNTR_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_CMD_FSM_CNTR_CNTR0,
    0x00000040,
    0x00000040,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLMSGCFG::CFG_LL_PORT_CNTR
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_PORT_CNTR,
    0x00000002,
    0x00000002,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLMSGCFG::CFG_LL_PORT_TIMER_PARAMS
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_PORT_TIMER_PARAMS,
    0x00030000,
    0x00010000,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLCMNHSMSGCFG::LL_HS_TXFFE_OVERRIDE_REG2CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_LL_HS_TXFFE_OVERRIDE_REG2CNTR0,
    0x00000C30,
    0x00000420,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLCMNHSMSGCFG::LL_CMNHS_LPM_CTR_STATUS_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_LL_CMNHS_LPM_CTR_STATUS_CNTR0,
    0x00200000,
    0x00200000,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLCMNHSMSGCFG::LL_CMNHS_LASM_CTR0_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_LL_CMNHS_LASM_CTR0_CNTR0,
    0x00006400,
    0x00006400,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLMSGCFG::CFG_FW_PORT_OP_CTRL
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_FW_PORT_OP_CTRL,
    0x00001A38,
    0x00001A38,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOTLS0CFGBLK::CIO_P0_EG_CONFIG3_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CIO_P0_EG_CONFIG3_CNTR0,
    0x00060000,
    0x00000000,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0HIACFGREG::S0_HIA_MiscControl_1_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_S0_HIA_MiscControl_1_CNTR0,
    0x00000FFF,
    0x00000803,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOTLS0CFGBLK::TL_HIA_ADP_CS_1_OWR_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TL_HIA_ADP_CS_1_OWR_CNTR0,
    0x0007FF00,
    0x00000300,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOTLS0CFGBLK::TL_HIA_ADP_CS_5_OWR_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TL_HIA_ADP_CS_5_OWR_CNTR0,
    0x003FFFFF,
    0x00001002,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0ADPCFGBLK::CFG_CONFIG_SPACE_HIA_MAX_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_CONFIG_SPACE_HIA_MAX_CNTR0,
    0x07FF07FF,
    0x00030002,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOS0ADPCFGBLK::CFG_CONFIG_SPACE_MAX_OUTPUT_HOPID_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_CONFIG_SPACE_MAX_OUTPUT_HOPID_CNTR0,
    0x000007FF,
    0x00000002,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLLNHSMSGCFG::LL_HS_INTERRUPT_EN_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_LL_HS_INTERRUPT_EN_CNTR0,
    0x00000400,
    0x00000400,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLMSGCFG::CFG_LL_INTERRUPT_EN
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_INTERRUPT_EN,
    0x00000400,
    0x00000400,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOTMUS0CFGBLK::TMU_PRIVATE_CS_14_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TMU_PRIVATE_CS_14_CNTR0,
    0x00000010,
    0x00000010,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOTMUS0CFGBLK::TMU_PRIVATE_CS_18_CNTR0
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_TMU_PRIVATE_CS_18_CNTR0,
    0x08000000,
    0x08000000,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  // CIO::SDXCIOLLMSGCFG::CFG_LL_OVRD_ADP_CS
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_CFG_LL_OVRD_ADP_CS,
    0xFC000000,
    0x00000000,
    0x01000003 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );
 remove Group 3*/
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 3 done!\n");
  //
  // Group 4: PHYINNER_REG_PRE_FW
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 4: PHYINNER_REG_PRE_FW!\n");

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 4 done!\n");
  //
  // Group 5: USB4RTS5_REG_PRE_RESET
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 5: USB4RTS5_REG_PRE_RESET!\n");

  // CIO::SDXCIOS5LLCONFIG::MSG_S5_LL_PIPE_TO
  FchKTXhciSmuUsbConfigUpdate (
    DieBusNum,
    FCH_KT_USB4_MSG_S5_LL_PIPE_TO,
    0x00000001,
    0x00000001,
    0x01000005 | (FCH_KT_USB_CONFIG_USB4RT0 + Router * FCH_KT_USB_CONFIG_RT_STEP) << 20
    );

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 5 done!\n");
  //
  // Group 6: USB4RTS0_REG_SECURITY
  //
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 6: USB4RTS0_REG_SECURITY!\n");

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]Group 6 done!\n");

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PlatformIndependentRegInitRt is completed!\n");
}

/**
 * @brief FchKTUsb4PlatformIndependentRegInit - Platform-Independent Register Configuration
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4PlatformIndependentRegInit (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PlatformIndependentRegInit is started!\n");

  //
  // USB4 RT0
  //
  if (LocalCfgPtr->Usb4Host[0].Usb4HostEnable) {
    FchKTUsb4PlatformIndependentRegInitRt (DieBusNum, 0, FchDataPtr);
  }

  //
  // USB4 RT1
  //
  if (LocalCfgPtr->Usb4Host[1].Usb4HostEnable) {
    FchKTUsb4PlatformIndependentRegInitRt (DieBusNum, 1, FchDataPtr);
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PlatformIndependentRegInit is completed!\n");
}


/**
 * FchKTUsb4OemUsb20PhyConfigure  -  USB2 PHY tuning
 *
 *
 *
 */
VOID
FchKTUsb4OemUsb20PhyConfigure (
  IN  UINT32                    DieBusNum,
  IN  FCH_RESET_DATA_BLOCK      *FchDataPtr
  )
{
  UINT8    Port;
  UINT32   DW0_Index;
  UINT32   DW1_Mask0;
  UINT32   DW1_Mask1;
  UINT32   DW2_Data;
  UINT32   DW3_Op_Group;
  FCH_KT_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;
  FchUsbOemPlatformTable  = FchDataPtr->OemUsbConfigurationTablePtr;
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb20PhyConfigure is started!\n");

  //Controller3 Port0
  DW0_Index    = FCH_KT_USB20_LANEPARACTL0;
  DW1_Mask0    = 0xF3BF7007;
  DW1_Mask1    = 0x0000000F;
  DW3_Op_Group = 0x01300001;
  for (Port = 8; Port < 9; Port++) {
    //Param0
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].COMPDISTUNE & 0x07) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].SQRXTUNE & 0x07) << 12) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXFSLSTUNE & 0x0F) << 16) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXPREEMPAMPTUNE & 0x03) << 20) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXPREEMPPULSETUNE & 0x01) << 23) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRISETUNE & 0x03) << 24) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXVREFTUNE & 0x0F) << 28));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 8),
      DW1_Mask0,
      DW2_Data,
      DW3_Op_Group
      );

    //Param1
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXHSXVTUNE & 0x03) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRESTUNE & 0x03) << 2));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 8) + 0x04,
      DW1_Mask1,
      DW2_Data,
      DW3_Op_Group
      );
  }

  //Controller4 Port0
  DW0_Index    = FCH_KT_USB20_LANEPARACTL0;
  DW1_Mask0    = 0xF3BF7007;
  DW1_Mask1    = 0x0000000F;
  DW3_Op_Group = 0x01400001;
  for (Port = 9; Port < 10; Port++) {
    //Param0
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].COMPDISTUNE & 0x07) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].SQRXTUNE & 0x07) << 12) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXFSLSTUNE & 0x0F) << 16) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXPREEMPAMPTUNE & 0x03) << 20) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXPREEMPPULSETUNE & 0x01) << 23) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRISETUNE & 0x03) << 24) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXVREFTUNE & 0x0F) << 28));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index+ FCH_KT_USB20_PHY_SMN_STEP * (Port - 9),
      DW1_Mask0,
      DW2_Data,
      DW3_Op_Group
      );

    //Param1
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXHSXVTUNE & 0x03) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRESTUNE & 0x03) << 2));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 9) + 0x04,
      DW1_Mask1,
      DW2_Data,
      DW3_Op_Group
      );
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb20PhyConfigure is completed!\n");
}

/**
 * FchKTUsb4OemUsb4PhyConfigure  -  USB4 C20 PHY tuning
 *
 *
 *
 */
VOID
FchKTUsb4OemUsb4PhyConfigure (
  IN  UINT32                    DieBusNum,
  IN  FCH_RESET_DATA_BLOCK      *FchDataPtr
  )
{
  UINT32                      PhyTuningSetPtr;
  UINT32                      PhyTuningSetSize;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  PhyTuningSetPtr = LocalCfgPtr->Usb4PhyTuningSetPtr;
  PhyTuningSetSize = LocalCfgPtr->Usb4PhyTuningSetSize;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb4PhyConfigure is started! PhyTuningSetPtr %x\n", PhyTuningSetPtr);
  IDS_HDT_CONSOLE (FCH_TRACE, "  PhyTuningSetPtr %x, PhyTuningSetSize %x\n", PhyTuningSetPtr, PhyTuningSetSize);

  if (LocalCfgPtr->Usb4PhyTuningEnable){
    if (PhyTuningSetPtr != 0) {
      // USB4 Router0 PHY
      if (LocalCfgPtr->Usb4Host[0].Usb4InitEnable) {
        FchKTXhciSmuUsbConfigUpdate (DieBusNum, PhyTuningSetPtr, PhyTuningSetSize & 0xFF, 0x00000000, 0xF0600001);
        IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb4PhyConfigure enable USB4 Router0 PHY tuning!\n");
      }

      // USB4 Router1 PHY
      if (LocalCfgPtr->Usb4Host[1].Usb4InitEnable) {
        FchKTXhciSmuUsbConfigUpdate (DieBusNum, PhyTuningSetPtr, PhyTuningSetSize & 0xFF, 0x00000000, 0xF0800001);
        IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb4PhyConfigure enable USB4 Router1 PHY tuning!\n");
      }
    }
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4OemUsb4PhyConfigure is completed!\n");
}

/**
 * FchKTUsb4OemConfigure  -  USB4 Platform Configuration
 *
 *
 *
 */
VOID
FchKTUsb4OemConfigure (
  IN  UINT32                   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  FchKTUsb4OemUsb20PhyConfigure (DieBusNum, FchDataPtr);
  FchKTUsb4OemUsb4PhyConfigure (DieBusNum, FchDataPtr);
}

/**
 * FchKTUsb4PassParameter - Config Usb4 controller during
 * Power-On
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4PassParameter (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK          *LocalCfgPtr;
  FCH_KT_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  PlatformUsbConfigureTable = (FCH_KT_USB_OEM_PLATFORM_TABLE *)(LocalCfgPtr->OemUsbConfigurationTablePtr);

  // FchKTUsb4PlatformIndependentRegInit (DieBusNum, FchDataPtr);
  // FchKTUsb4DisablePort (DieBusNum, FchDataPtr);
  FchKTUsb4OverCurrent (DieBusNum, FchDataPtr);
  FchKTUsb4XhciPortForceGen1 (DieBusNum, FchDataPtr);
  FchKTUsb4TunnelDisable (DieBusNum, FchDataPtr);
  FchKTUsb4AdapterHide (DieBusNum, FchDataPtr);
  FchKTUsb4Tbt3Disable (DieBusNum, FchDataPtr);
  FchKTUsb4TxFFEConfigure (DieBusNum, FchDataPtr);
  FchKTUsb4Gen3Support (DieBusNum, FchDataPtr);

  if ((PlatformUsbConfigureTable->Version_Major == 0x0D)  && (PlatformUsbConfigureTable->Version_Minor == 0x09)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4PassParameter find KT oem table!\n");
    FchKTUsb4OemConfigure (DieBusNum, FchDataPtr);
  }
}

/**
 * FchKTUsb4InitBootProgram - Config Usb4 controller during
 * Power-On
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4InitBootProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitBootProgram Start...\n");
  if (LocalCfgPtr->Usb4Host[0].Usb4InitEnable || LocalCfgPtr->Usb4Host[1].Usb4InitEnable) {
    // overall control of USB4 init
    if (FchKTXhciCheckUsbSkipFlag (DieBusNum, FchDataPtr)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitBootProgram SkipAllUSBControllerAccess...\n");
    } else {
      FchKTUsb4PassParameter (DieBusNum, FchDataPtr);
      if (FchKTXhciCheckUsbPhySkip (DieBusNum, FchDataPtr)) {
        IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitBootProgram SkipAllUSBPhyAccess...\n");
      } else {
        FchKTUsb4PhyLoad (DieBusNum, FchDataPtr);
      }
      FchKTUsb4PdInterruptMode(DieBusNum, FchDataPtr);
      FchKTUsb4InitMessage (DieBusNum, FchDataPtr);
    }
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitBootProgram Exit.\n");
}

/**
 * FchKTUsb4InitS3ExitProgram - Config Usb4 controller during
 * S3 resume
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTUsb4InitS3ExitProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitS3ExitProgram Start...\n");
  if (FchKTXhciCheckUsbSkipFlag (DieBusNum, FchDataPtr)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitS3ExitProgram SkipAllUSBControllerAccess...\n");
  } else {
    FchKTUsb4PassParameter (DieBusNum, FchDataPtr);
    FchKTXhciSmuService (DieBusNum, BIOSSMC_MSG_UsbSxExit);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsb4InitS3ExitProgram Exit.\n");
}

