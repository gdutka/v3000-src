/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
//#include <Library/DebugLib.h>

#include "FchPlatform.h"
#include "Filecode.h"
#include "GnbDxio.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>

#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTXHCISERVICE_FILECODE

/**
 * FchKTXhciSmuService Request -  Xhci1 SMU Service Request
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTXhciSmuService (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  if (NbioSmuServiceRequestV13 (NbioPciAddress, RequestId, SmuArg,0)) {
    status = TRUE;
  }

  return status;
}

BOOLEAN
FchKTXhciSmuServiceWithPara (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    *Parameter
  )
{
  BOOLEAN         Status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  Status = FALSE;
  if (Parameter == NULL) {
    return Status;
  }

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  LibAmdMemCopy ((VOID *)SmuArg, (VOID *)Parameter, sizeof (UINT32) * 6, NULL);
  if (NbioSmuServiceRequestV13 (NbioPciAddress, RequestId, SmuArg, 0)) {
    Status = TRUE;
  }

  return Status;
}

/**
 * FchKTUsbSmuServiceReqOnly -  Xhci1 SMU Service Request Only
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTUsbSmuServiceReqOnly (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];
  UINT32*         pSmuArg;

  status = FALSE;
  pSmuArg = (UINT32*) (UINTN)FchDataPtr->pUsbInitData;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmuArg[0] = *pSmuArg;
  SmuArg[1] = *(pSmuArg+1);
  SmuArg[2] = *(pSmuArg+2);
  if (NbioSmuServiceRequestOnlyV13 (NbioPciAddress, RequestId, SmuArg,1)) {
    status = TRUE;
  }

  return status;
}

/**
 * FchKTUsbSmuServiceNew -  Xhci1 SMU Service New
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTUsbSmuServiceNew (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];
  UINT32*         pSmuArg;

  status = FALSE;
  pSmuArg = (UINT32*) (UINTN)FchDataPtr->pUsbInitData;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmuArg[0] = *pSmuArg;
  SmuArg[1] = *(pSmuArg+1);
  SmuArg[2] = *(pSmuArg+2);
  if (NbioSmuServiceRequestV13 (NbioPciAddress, RequestId, SmuArg,1)) {
    status = TRUE;
  }

  return status;
}
/**
 * FchKTXhciSmuServiceUsbInit  -  Xhci1 SMU Service USB Init
 * Request
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTXhciSmuServiceUsbInit (
  IN  UINT32    DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  FCH_KT_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;
  PlatformUsbConfigureTable = (FCH_KT_USB_OEM_PLATFORM_TABLE *)(FchDataPtr->OemUsbConfigurationTablePtr);
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmuArg[0] = 0;
  SmuArg[1] = 0;
  FchDataPtr->pUsbInitData->Enable.usb_init_combined = 1;
  if (LocalCfgPtr->FchReset.Xhci0Enable) {
    FchDataPtr->pUsbInitData->Enable.usb_hc_0 = 1;
    if (!FchCheckRmbB0()) {
      FchDataPtr->pUsbInitData->ComboPhyStaticConfig.usb_hc_0 = PlatformUsbConfigureTable->ComboPhyStaticConfig[0] & 0x0F;
      FchDataPtr->pUsbInitData->ComboPhyStaticConfig.usb_hc_1 = PlatformUsbConfigureTable->ComboPhyStaticConfig[1] & 0x0F;
    }
  }
  if (LocalCfgPtr->FchReset.Xhci1Enable) {
    FchDataPtr->pUsbInitData->Enable.usb_hc_1 = 1;
    FchDataPtr->pUsbInitData->ComboPhyStaticConfig.usb_hc_1 = PlatformUsbConfigureTable->ComboPhyStaticConfig[2] & 0x0F;
  }
  if (LocalCfgPtr->Xhci2Enable) {
    if (LocalCfgPtr->AmdEnvironmentFlag == 0) {
      FchDataPtr->pUsbInitData->Enable.usb_hc_2 = 1;
    }
  }

  //if (NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbInit, SmuArg,0)) {
  //  status = TRUE;
  //}

  return status;
}

/**
 * FchKTXhciSmuUsbConfigUpdate  -  Xhci Smu Usb Config Update
 *
 *
 *
 */
BOOLEAN
FchKTXhciSmuUsbConfigUpdate (
  IN  UINT32    DieBusNum,
  IN  UINT32    smn_register,
  IN  UINT32    smn_mask,
  IN  UINT32    smn_data,
  IN  UINT32    smn_group
  )
{
  UINT32          Status = BIOSSMC_Result_Failed;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  IDS_HDT_CONSOLE (FCH_TRACE, "FchKTXhciSmuUsbConfigUpdate Enter\n");

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = smn_register;
  SmuArg[1] = smn_mask;
  SmuArg[2] = smn_data;
  SmuArg[3] = smn_group;
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  Status = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  IDS_HDT_CONSOLE (FCH_TRACE, "FchKTXhciSmuUsbConfigUpdate Exit, Status = 0x%x\n", Status);

  if (Status == BIOSSMC_Result_OK) {
    return TRUE;
  }

  return FALSE;
}


/**
 * FchKTXhciSmuUsbPort0Disable  -  Xhci Smu Usb Port0 Disable
 *
 *
 *
 */
BOOLEAN
FchKTXhciSmuUsbPort0Disable (
  IN  UINT32    DieBusNum,
  IN  UINT32    smn_port0_disable
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];
  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = smn_port0_disable;
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  ///@todo no such message in SMU V13
  // if (NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_DisableUsbPort0, SmuArg,0)) {
    status = TRUE;
  // }

  return status;
}


/**
 * FchKTPlatformIndependentRegisterConfigurationPerController  -
 * Each Xhci Platform independant register Configuration.
 *
 *  RN_USB_SW_SPEC 1.16
 *
 */
VOID
FchKTPlatformIndependentRegisterConfigurationPerController (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr,
  IN  UINT32   ControllerNum
  )
{
    // Group1
    //Enable legacy SMI support by setting INTERRUPT_CONTROL_CNTR0.USB_LEGACY_SMI_EN
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USB_INTERRUPT_CONTROL,
      0x00000100,
      0x00000100,
      0x01000001 | ControllerNum << 20
      );
    //TCAREGS::TCA_TCPC Bit[3], tcpc_low_power_en
    //FchKTXhciSmuUsbConfigUpdate (
      // DieBusNum,
      // FCH_KT_USBOFFSET_TCA_TCPC,
      // 0x00000008,
      // 0x00000000,
      // 0x01000001 | ControllerNum << 20
      // );
    if (ControllerNum != 2) {
      //USB31::::USB_31PHYPARACTL0_CNTR0 (INST0/INST1 @0x0016_8000/0x0016_8400) EXT_CTRL_SEL (bit[0]) = 1
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB3_PHYPARACTL0,
        0x00000001,
        0x00000001,
        0x01000001 | ControllerNum << 20
        );
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB3_PHYPARACTL0 + FCH_KT_USB3_PHY_SMN_STEP,
        0x00000001,
        0x00000001,
        0x01000001 | ControllerNum << 20
        );
      //USB31::USB31PHYPARAMCTRLREGCNTR0::USB_31PHYPARACTL16_CNTR0 (INST0/INST1 @0x0016_8040/0x0016_8440)
      //Enable legacy RX CDR (bit[20]) = 1
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB3_PHYPARACTL16,
        0x00100000,
        0x00100000,
        0x01000001 | ControllerNum << 20
        );
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB3_PHYPARACTL16 + FCH_KT_USB3_PHY_SMN_STEP,
        0x00100000,
        0x00100000,
        0x01000001 | ControllerNum << 20
        );
    }
    //PLAT-49960 Remove Group 1 item 5.
    //5. USB31::USB31LANEPARAMCTLREGCNTR0::USB_31LANEPARACTL1_CNTR0 (INST0/INST1 @0x0016_c004/0x0016_c404)
    // Enable tx_eq_ovrd_g2 (bit[16]) and tx_eq_ovrd_g1 (bit[15])
    //FchKTXhciSmuUsbConfigUpdate (
      // DieBusNum,
      // FCH_KT_USB3_LANEPARACTL1_S0,
      // 0x00018000,
      // 0x00018000,
      // 0x01000001 | ControllerNum << 20
      // );
    //FchKTXhciSmuUsbConfigUpdate (
      // DieBusNum,
      // FCH_KT_USB3_LANEPARACTL1_S1,
      // 0x00018000,
      // 0x00018000,
      // 0x01000001 | ControllerNum << 20
      // );
    // Group2
    if (ControllerNum != 2) {
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USB_PORT_CONTROL,
        0x0000F000,
        0x00000000,
        0x01000002 | ControllerNum << 20
        );
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x12478c, 0x000000001, 0x00000001, 0x01000002 | ControllerNum << 20 );
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x124790, 0x000000001, 0x00000001, 0x01000002 | ControllerNum << 20 );
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x124B8c, 0x000000001, 0x00000001, 0x01000002 | ControllerNum << 20 );
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x124B90, 0x000000001, 0x00000001, 0x01000002 | ControllerNum << 20 );

      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x124788, 0x000000008, 0x00000008, 0x01000002 | ControllerNum << 20 );
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x124B88, 0x000000008, 0x00000008, 0x01000002 | ControllerNum << 20 );
    }

    // Group3
    // FCHUSBDWCUSB31CNTR0::GSYSBLKWINCTRL
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USBOFFSET_GSYSBLKWINCTRL,
      0x20000000,
      0x20000000,
      0x01000003 | ControllerNum << 20
      );

    // FCHUSBDWCUSB31CNTR0::GUCTL clear [18] (EN_EXTD_TBC_CAP)
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USBOFFSET_GUCTL,
      0x00040000,
      0x00000000,
      0x01000003 | ControllerNum << 20
      );

    // FCHUSBDWCUSB31CNTR0::GUCTL2 Bit[31:25], PERIODIC_UF_THR
    // FCHUSBDWCUSB31CNTR0::GUCTL2 Bit[24:18], ASYNC_UF_THR
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USBOFFSET_GUCTL2,
      0xFFFC0000,
      0x30140000,
      0x01000003 | ControllerNum << 20
      );

    // FCHUSBDWCUSB31CNTR0::Clear GUCTL2 Bit[21], BLOCK_CONCURRENT_IN_CTRL_XFERS
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USBOFFSET_GUCTL3,
      0x00200000,
      0x00000000,
      0x01000003 | ControllerNum << 20
      );

    // FCHUSBDWCUSB31CNTR0::GSMACCTL Bit[2], host_mask_nump0
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      FCH_KT_USBOFFSET_GSMACCTL,
      0x00000004,
      0x00000004,
      0x01000003 | ControllerNum << 20
      );

    if (ControllerNum == 2) {
      // FCHUSBDWCUSB31CNTR2::GUCTL1 Bit[23:21], IP_GAP_ADD_ON
      FchKTXhciSmuUsbConfigUpdate (
        DieBusNum,
        FCH_KT_USBOFFSET_GUCTL1,
        0x00E00000,
        0x00E00000,
        0x01000003 | ControllerNum << 20
        );
    }
}
/**
 * FchKTXhciCommonRegInit  -  Xhci Platform independant register
 * Configuration.
 *
 *
 *
 */
VOID
FchKTXhciCommonRegInit (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCommonRegInit is started!\n");
  if (LocalCfgPtr->FchReset.Xhci0Enable) {
    FchKTPlatformIndependentRegisterConfigurationPerController (DieBusNum, LocalCfgPtr, 0);
  }

  if (LocalCfgPtr->FchReset.Xhci1Enable) {
    FchKTPlatformIndependentRegisterConfigurationPerController (DieBusNum, LocalCfgPtr, 1);
  }

  if (LocalCfgPtr->Xhci2Enable) {
    FchKTPlatformIndependentRegisterConfigurationPerController (DieBusNum, LocalCfgPtr, 2);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCommonRegInit is Completed!\n");
}

/**
 * FchKTXhciDisablePort  -  Xhci Disable Port Control
 *
 *                          RMB USB ports
 *                             HC0: 2 USB3 + 4 USB2
 *                             HC1: 2 USB3 + 3 USB2
 *                             HC2: 0 USB3 + 1 USB2
 */
VOID
FchKTXhciDisablePort (
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

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDisablePort is started!\n");
      // Port0-5 : Controller0
      UsbPortDisable = (Usb2PortDisable & 0x0f) + ((Usb3PortDisable & 0x3) << 16);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_DISABLE0, 0x0003000f, UsbPortDisable, 0x01000001);

      // Port0-4 : Controller1
      Usb3PortDisable = Usb3PortDisable >> 2;
      Usb2PortDisable = Usb2PortDisable >> 4;
      UsbPortDisable = (Usb2PortDisable & 0x07) + ((Usb3PortDisable & 0x3) << 16);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_DISABLE0, 0x0003000f, UsbPortDisable, 0x01100001);

      // Port0 : Controller2
      Usb2PortDisable = Usb2PortDisable >> 4;
      UsbPortDisable = (Usb2PortDisable & 0x01);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_DISABLE0, 0x00000001, UsbPortDisable, 0x01200001);

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDisablePort is completed!\n");
  }
}

/**
 * FchKTXhciDisablePortLate  -  Xhci Disable Port Control Late
 *
 *
 *
 */
VOID
FchKTXhciDisablePortLate (
  IN  UINT32   DieBusNum,
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT32                      Usb3PortDisable;
  UINT32                      Usb2PortDisable;
  UINT32                      UsbPortDisable;
  UINT32                      SmuUsbPort0Disable;

  SmuUsbPort0Disable = 0;
  if (FchDataPtr->FchResetDataBlock.DisableXhciPortLate) {
      Usb3PortDisable = FchDataPtr->FchResetDataBlock.XhciUsb3PortDisable;
      Usb2PortDisable = FchDataPtr->FchResetDataBlock.XhciUsb2PortDisable;

      // Port0-5 : Controller0
      UsbPortDisable = (Usb2PortDisable & 0x0f) + ((Usb3PortDisable & 0x3) << 16);
      FchSmnRW (0, FCH_KT_USB0_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x0003000f), UsbPortDisable, NULL);

      // Port0-4 : Controller1
      Usb3PortDisable = Usb3PortDisable >> 2;
      Usb2PortDisable = Usb2PortDisable >> 4;
      UsbPortDisable = (Usb2PortDisable & 0x07) + ((Usb3PortDisable & 0x3) << 16);
      FchSmnRW (0, FCH_KT_USB1_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x0003000f), UsbPortDisable, NULL);

      // Port0 : Controller2
      Usb2PortDisable = Usb2PortDisable >> 4;
      UsbPortDisable = (Usb2PortDisable & 0x01);
      FchSmnRW (0, FCH_KT_USB2_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x00000001), UsbPortDisable, NULL);

      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDisablePortLate is completed!\n");
  }
}

VOID
FchKTXhciSparseMode(
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  if (LocalCfgPtr->UsbSparseModeEnable) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, BIT17, 0x01000003 | 0 << 20);
	  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, BIT17, 0x01000003 | 1 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, BIT17, 0x01000003 | 2 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, BIT17, 0x01000003 | 3 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, BIT17, 0x01000003 | 4 << 20);
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciSparseMode to Enabled\n");
  } else {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, 0, 0x01000003 | 0 << 20);
	  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, 0, 0x01000003 | 1 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, 0, 0x01000003 | 2 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, 0, 0x01000003 | 3 << 20);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USBOFFSET_GUCTL, BIT17, 0, 0x01000003 | 4 << 20);
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciSparseMode to Disabled\n");
  }
}


/**
 * FchKTXhciOverCurrent  -  Xhci OC Pin Mapping
 *
 *
 *
 */
VOID
FchKTXhciOverCurrent (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32    OverCurrentMap;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciOverCurrent is started!\n");
  // Port0-5 : Controller0
  OverCurrentMap = LocalCfgPtr->XhciOCpinSelect;
  OverCurrentMap = OverCurrentMap & 0xFFFFFF;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_OCMAPPING0, 0x00FFFFFF, OverCurrentMap, 0x01000001);

  // Port0-4 : Controller1
  OverCurrentMap = LocalCfgPtr->Xhci1OCpinSelect;
  OverCurrentMap = (OverCurrentMap ) & 0xFFFFF;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_OCMAPPING0, 0x000FFFFF, OverCurrentMap, 0x01100001);

  // Port0 : Controller2
  OverCurrentMap = LocalCfgPtr->Xhci2OCpinSelect;
  OverCurrentMap = (OverCurrentMap ) & 0xF;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_OCMAPPING0, 0x0000000F, OverCurrentMap, 0x01200001);
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciOverCurrent is completed!\n");
}

/**
 * FchKTXhciOCPolarity  -  Xhci OC Pin Polarity configuration
 *
 *
 *
 */
VOID
FchKTXhciOCPolarity (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  if (LocalCfgPtr->XhciOcPolarityCfgLow) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, BIT8, 0x01000001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, BIT8, 0x01100001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, BIT8, 0x01200001);
    if (FchCheckRmbB0()) {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, BIT8, 0x01300001);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, BIT8, 0x01400001);
    }
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciOCPolarity set Polarity to Low\n");
  } else {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, 0, 0x01000001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, 0, 0x01100001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, 0, 0x01200001);
    if (FchCheckRmbB0()) {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, 0, 0x01300001);
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, BIT8, 0, 0x01400001);
    }
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciOCPolarity set Polarity to High\n");
  }
}

/**
 * FchKTXhciDeviceRemovable  -  Xhci Device Removable Control
 *
 *
 *
 */
VOID
FchKTXhciDeviceRemovable (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      DeviceRemovable;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  UINT32                      Usb3Removable;
  UINT32                      Usb2Removable;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDeviceRemovable is started!\n");
  DeviceRemovable = LocalCfgPtr->Xhci0DevRemovable;
  Usb3Removable = (DeviceRemovable & 0xFFFF0000) >> 16;
  Usb2Removable = DeviceRemovable & 0x0000FFFF;
  // Port0-5 : Controller0 2USB3 + 4USB2
  DeviceRemovable = DeviceRemovable & 0x0003000F;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_DEVICE_REMOVABLE, 0x0003000F, DeviceRemovable, 0x01000001);

  // Port0-4 : Controller1 2USB3 + 3USB2
  Usb3Removable = Usb3Removable >> 2;
  Usb2Removable = Usb2Removable >> 4;
  DeviceRemovable = (Usb2Removable & 0x07) + ((Usb3Removable & 0x3) << 16);
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_DEVICE_REMOVABLE, 0x00030007, DeviceRemovable, 0x01100001);

  // Port0 : Controller2 0USB3 + 1USB2
  Usb2Removable = Usb2Removable >> 3;
  DeviceRemovable = Usb2Removable & 0x00000001;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_DEVICE_REMOVABLE, 0x00000001, DeviceRemovable, 0x01200001);
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDeviceRemovable is completed!\n");
}

/**
 * FchKTXhciRasFeature  -  Xhci RAS Control
 *
 *
 *
 */
VOID
FchKTXhciRasFeature (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciRasFeature is started!\n");
  if (LocalCfgPtr->XhciECCDedErrRptEn) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00001000, 0x01000001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00001000, 0x01100001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00001000, 0x01200001);
  } else {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00000000, 0x01000001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00000000, 0x01100001);
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_INTERRUPT_CONTROL, 0x00001000, 0x00000000, 0x01200001);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciRasFeature is completed!\n");
}

/**
 * FchKTXhciSubSequenceEnable  -  Xhci SubSequence Enable
 *
 *
 *
 */
VOID
FchKTXhciSubSequenceEnable (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciSubSequenceEnable is started!\n");
  if (LocalCfgPtr->FchReset.Xhci0Enable) {
    //XHCI0 only has Type-C PHY
    //Combo-PHY SRAM loading
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0000001);
  }

  if (LocalCfgPtr->FchReset.Xhci1Enable) {
    //Combo-PHY SRAM loading
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0100001);

    //Standalone-PHY SRAM loading
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0100002);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciSubSequenceEnable is completed!\n");
}

/**
 * FchKTXhciDdiModeEnable  -  Xhci DDI mode Enable
 *
 *
 *
 */
VOID
FchKTXhciDdiModeEnable (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  UINT32                      DdiMode;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDdiModeEnable is started!\n");
  if(!FchCheckRmbB0()) {
    if (LocalCfgPtr->FchReset.Xhci0Enable) {
      //XHCI0 has 2 Combo-PHY
      DdiMode = LocalCfgPtr->XhciUsbDdiModeEnable & 0xFF;
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xD0000000 | DdiMode);
    }
  }

  if (LocalCfgPtr->FchReset.Xhci1Enable) {
    //XHCI1 has 1 Combo-PHY
    DdiMode = (LocalCfgPtr->XhciUsbDdiModeEnable >> 8) & 0xF;
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xD0100000 | DdiMode);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciDdiModeEnable is completed!\n");
}

VOID
FchKTXhciPdInterruptMode (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPdInterruptMode is started!\n");

  // XHCI1
  if ((LocalCfgPtr->FchReset.Xhci1Enable) && (LocalCfgPtr->PdInterruptModeEn & BIT1)) {
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xE0600001);
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPdInterruptMode is completed!\n");
}

/**
 * FchKTXhciCheckPwerSts  -  S3 entry to check Power State
 *
 *
 *
 */
VOID
FchKTXhciCheckPwerSts (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32       Timer;
  UINT32       CurrentPowerState;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCheckPwerSts is started!\n");
  if (LocalCfgPtr->FchReset.Xhci0Enable) {
    Timer = 0;
    FchSmnRead (DieBusNum, FCH_KT_USB_CURRENT_PWR_STS, &CurrentPowerState, NULL);
    while ((Timer < FCH_KT_USB_T_D3ENTRY) && ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9))) {
      FchStall (10, NULL);
      Timer += 10;
      FchSmnRead (DieBusNum, FCH_KT_USB_CURRENT_PWR_STS, &CurrentPowerState, NULL);
    }
    if ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCheckPwerSts USB0 is not in D3...\n");
    }
  }

  if (LocalCfgPtr->FchReset.Xhci1Enable) {
    Timer = 0;
    FchSmnRead (DieBusNum, FCH_KT_USB_CURRENT_PWR_STS + FCH_KT_USB_CONTROLLER_SMN_STEP, &CurrentPowerState, NULL);
    while ((Timer < FCH_KT_USB_T_D3ENTRY) && ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9))) {
      FchStall (10, NULL);
      Timer += 10;
      FchSmnRead (DieBusNum, FCH_KT_USB_CURRENT_PWR_STS + FCH_KT_USB_CONTROLLER_SMN_STEP, &CurrentPowerState, NULL);
    }
    if ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCheckPwerSts USB1 is not in D3...\n");
    }
  }

  if (LocalCfgPtr->Xhci2Enable) {
    Timer = 0;
    FchSmnRead (DieBusNum, FCH_KT_USB2_SMN_BASE + FCH_KT_USB_CURRENT_PWR_STS, &CurrentPowerState, NULL);
    while ((Timer < FCH_KT_USB_T_D3ENTRY) && ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9))) {
      FchStall (10, NULL);
      Timer += 10;
      FchSmnRead (DieBusNum, FCH_KT_USB2_SMN_BASE + FCH_KT_USB_CURRENT_PWR_STS, &CurrentPowerState, NULL);
    }
    if ((CurrentPowerState & (BIT8 + BIT9)) != (BIT8 + BIT9)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCheckPwerSts USB2 is not in D3...\n");
    }
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciCheckPwerSts is completed!\n");
}
/**
 * FchKTXhciPortForceGen1  -  Port Force Gen1
 *
 *
 *
 */
VOID
FchKTXhciPortForceGen1 (
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
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPortForceGen1 is started!\n");

  //Controller0
  UsbPortForceGen1  = LocalCfgPtr->Usb3PortForceGen1;
  UsbPortForceGen1 &= 0x3;
  if (UsbPortForceGen1 != 0) {
    //Controller0 Port Control
    DW0_Index    = FCH_KT_USB_PORT_CONTROL;
    DW1_Mask     = 0x3 << 16;
    DW2_Data     = (UINT32) (UsbPortForceGen1 << 16);
    DW3_Op_Group = 0x01000001;
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }

  //Controller1
  UsbPortForceGen1  = (LocalCfgPtr->Usb3PortForceGen1) >> 2;
  UsbPortForceGen1 &= 0x3;
  if (UsbPortForceGen1 != 0) {
    //Controller1 Port Control
    DW0_Index    = FCH_KT_USB_PORT_CONTROL;
    DW1_Mask     = 0x3 << 16;
    DW2_Data     = (UINT32) (UsbPortForceGen1 << 16);
    DW3_Op_Group = 0x01100001;
    FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPortForceGen1 is completed!\n");
}

/**
 * FchKTUsbOemUsb20PhyConfigure  -  USB2 PHY tuning
 *
 *
 *
 */
VOID
FchKTUsbOemUsb20PhyConfigure (
  IN  UINT32   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
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
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsbOemUsb20PhyConfigure is started!\n");

  //Controller0 Port0 - 3
  DW0_Index    = FCH_KT_USB20_LANEPARACTL0;
  DW1_Mask0    = 0xF3BF7007;
  DW1_Mask1    = 0x0000000F;
  DW3_Op_Group = 0x01000001;
  for (Port = 0; Port < 4; Port++) {
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
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * Port,
      DW1_Mask0,
      DW2_Data,
      DW3_Op_Group
      );

    //Param1
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXHSXVTUNE & 0x03) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRESTUNE & 0x03) << 2));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * Port + 0x04,
      DW1_Mask1,
      DW2_Data,
      DW3_Op_Group
      );
  }

  //Controller1 Port0 - 2
  DW0_Index    = FCH_KT_USB20_LANEPARACTL0;
  DW1_Mask0    = 0xF3BF7007;
  DW1_Mask1    = 0x0000000F;
  DW3_Op_Group = 0x01100001;
  for (Port = 4; Port < 7; Port++) {
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
      DW0_Index+ FCH_KT_USB20_PHY_SMN_STEP * (Port - 4),
      DW1_Mask0,
      DW2_Data,
      DW3_Op_Group
      );

    //Param1
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXHSXVTUNE & 0x03) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRESTUNE & 0x03) << 2));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 4) + 0x04,
      DW1_Mask1,
      DW2_Data,
      DW3_Op_Group
      );
  }

  //Controller2 Port0
  DW0_Index    = FCH_KT_USB20_LANEPARACTL0;
  DW1_Mask0    = 0xF3BF7007;
  DW1_Mask1    = 0x0000000F;
  DW3_Op_Group = 0x01200001;
  for (Port = 7; Port < 8; Port++) {
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
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 7),
      DW1_Mask0,
      DW2_Data,
      DW3_Op_Group
      );

    //Param1
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXHSXVTUNE & 0x03) +
                 ((FchUsbOemPlatformTable->Usb20PhyPort[Port].TXRESTUNE & 0x03) << 2));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB20_PHY_SMN_STEP * (Port - 7) + 0x04,
      DW1_Mask1,
      DW2_Data,
      DW3_Op_Group
      );
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsbOemUsb20PhyConfigure is completed!\n");
}

/**
 * FchKTUsbOemUsb3PhyConfigure  -  USB3 PHY tuning
 *
 *
 *
 */
VOID
FchKTUsbOemUsb3PhyConfigure (
  IN  UINT32   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8    Port;
  UINT32   DW0_Index;
  UINT32   DW1_Mask;
  UINT32   DW2_Data;
  UINT32   DW3_Op_Group;
  FCH_KT_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;
  FchUsbOemPlatformTable  = FchDataPtr->OemUsbConfigurationTablePtr;
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsbOemUsb3PhyConfigure is started!\n");

  //Controller0 Port0 - 1
  DW0_Index    = FCH_KT_USB3_PHYPARACTL10;
  DW1_Mask     = 0x00000707;
  DW3_Op_Group = 0x01000001;
  for (Port = 0; Port < 2; Port++) {
    //Param0
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb3PhyPort[Port].RX_TERM_CTRL & 0x07) +
                 ((FchUsbOemPlatformTable->Usb3PhyPort[Port].TX_TERM_CTRL & 0x07) << 8));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB3_PHY_SMN_STEP * Port,
      DW1_Mask,
      DW2_Data,
      DW3_Op_Group
      );
  }


  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, 0x0000F000, 0x00000000, 0x01000001);
  //PHY internal register
  DW0_Index    = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  DW1_Mask     = 0xF0;
  DW2_Data     = (UINT32)  (((FchUsbOemPlatformTable->Usb3PhyPort[0].TX_VBOOST_LVL_EN & 0x01) << 7) +
                 ((FchUsbOemPlatformTable->Usb3PhyPort[0].TX_VBOOST_LVL & 0x07) << 4));
  DW3_Op_Group = 0x01000002;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);

  if ((FchDataPtr->XhciUsb3PortDisable & BIT1) == 0) {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, 0x0000F000, 0x00001000, 0x01000001);
      //PHY internal register
      DW0_Index    = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
      DW1_Mask     = 0xF0;
      DW2_Data     = (UINT32)  (((FchUsbOemPlatformTable->Usb3PhyPort[1].TX_VBOOST_LVL_EN & 0x01) << 7) +
                     ((FchUsbOemPlatformTable->Usb3PhyPort[1].TX_VBOOST_LVL & 0x07) << 4));
      DW3_Op_Group = 0x01000002;
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }

  //Controller1 Port0 - 1
  DW0_Index    = FCH_KT_USB3_PHYPARACTL10;
  DW1_Mask     = 0x00000707;
  DW3_Op_Group = 0x01100001;
  for (Port = 2; Port < 4; Port++) {
    //Param0
    DW2_Data = (UINT32) ((FchUsbOemPlatformTable->Usb3PhyPort[Port].RX_TERM_CTRL & 0x07) +
                 ((FchUsbOemPlatformTable->Usb3PhyPort[Port].TX_TERM_CTRL & 0x07) << 8));
    FchKTXhciSmuUsbConfigUpdate (
      DieBusNum,
      DW0_Index + FCH_KT_USB3_PHY_SMN_STEP * (Port - 2),
      DW1_Mask,
      DW2_Data,
      DW3_Op_Group
      );
  }

  //PHY internal register
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, 0x0000F000, 0x00000000, 0x01100001);
  DW0_Index    = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  DW1_Mask     = 0xF0;
  DW2_Data     = (UINT32)  (((FchUsbOemPlatformTable->Usb3PhyPort[2].TX_VBOOST_LVL_EN & 0x01) << 7) +
                 ((FchUsbOemPlatformTable->Usb3PhyPort[2].TX_VBOOST_LVL & 0x07) << 4));
  DW3_Op_Group = 0x01100002;
  FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);

  if ((FchDataPtr->XhciUsb3PortDisable & BIT3) == 0) {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, FCH_KT_USB_PORT_CONTROL, 0x0000F000, 0x00001000, 0x01100001);
      DW0_Index    = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
      DW1_Mask     = 0xF0;
      DW2_Data     = (UINT32)  (((FchUsbOemPlatformTable->Usb3PhyPort[3].TX_VBOOST_LVL_EN & 0x01) << 7) +
                     ((FchUsbOemPlatformTable->Usb3PhyPort[3].TX_VBOOST_LVL & 0x07) << 4));
      DW3_Op_Group = 0x01100002;
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  }

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTUsbOemUsb3PhyConfigure is completed!\n");
}

/**
 * FchKTXhciBatteryCharging  -  Xhci enable USB Battery Charging
 * V1.2
 *
 *
 *
 */
VOID
FchKTXhciBatteryCharging (
  IN  UINT32   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  FCH_KT_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;
  FchUsbOemPlatformTable  = FchDataPtr->OemUsbConfigurationTablePtr;
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciBatteryCharging is started!\n");

  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciBatteryCharging is completed!\n");
}

/**
 * FchKTXhciPhyP3CpmP4Enable - Enable/disable PHY P3-CPM and P4
 * feature
 *
 *
 *
 */
VOID
FchKTXhciPhyP3CpmP4Enable (
  IN  UINT32   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8    Port;
  UINT8    PhyP3CpmP4Support;
  UINT32   DW0_Address;
  UINT32   DW3_Group;
  FCH_KT_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPhyP3CpmP4Enable is started!\n");

  FchUsbOemPlatformTable  = FchDataPtr->OemUsbConfigurationTablePtr;;
  PhyP3CpmP4Support  = FchUsbOemPlatformTable->PhyP3CpmP4Support;

  for (Port = 0; Port < 4; Port++) {
    // USB3 Port 1-0 - Controller0
    //      Port 3-2 - Controller1
    DW0_Address = FCH_KT_USBOFFSET_S0LLUCTL + 0x80 * (Port & BIT0);
    DW3_Group   = 0x01000003 + 0x00100000 * (Port / 2);
    if ((PhyP3CpmP4Support >> Port) & BIT0) {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Address, BIT28, BIT28, DW3_Group);
    } else {
      FchKTXhciSmuUsbConfigUpdate (DieBusNum, DW0_Address, BIT28, 0x00, DW3_Group);
    }
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciPhyP3CpmP4Enable is completed!\n");
}

/**
 * FchKTXhciOemConfigure  -  Xhci Platform Configurationb
 *
 *
 *
 */
VOID
FchKTXhciOemConfigure (
  IN  UINT32   DieBusNum,
  IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  )
{
  FchKTUsbOemUsb20PhyConfigure (DieBusNum, FchDataPtr);
  FchKTUsbOemUsb3PhyConfigure (DieBusNum, FchDataPtr);
  //FchKTXhciBatteryCharging (DieBusNum, FchDataPtr);
  //FchKTXhciPhyP3CpmP4Enable (DieBusNum, FchDataPtr);
}

/**
 * FchKTXhciPassParameter  -  Xhci Pass Parameters
 *
 *
 *
 */
VOID
FchKTXhciPassParameter (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK          *LocalCfgPtr;
  FCH_KT_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;
  PlatformUsbConfigureTable = (FCH_KT_USB_OEM_PLATFORM_TABLE *)(LocalCfgPtr->OemUsbConfigurationTablePtr);

  //FchKTXhciCommonRegInit (DieBusNum, LocalCfgPtr);
  //PLAT-92529 FchKTXhciDisablePort (DieBusNum, LocalCfgPtr);
  FchKTXhciOverCurrent (DieBusNum, LocalCfgPtr);
  FchKTXhciOCPolarity (DieBusNum, LocalCfgPtr);
  FchKTXhciDeviceRemovable (DieBusNum, LocalCfgPtr);
  FchKTXhciPortForceGen1 (DieBusNum, LocalCfgPtr);
  FchKTXhciSparseMode (DieBusNum, LocalCfgPtr);
  if ((PlatformUsbConfigureTable->Version_Major == 0x0D)  && (PlatformUsbConfigureTable->Version_Minor == 0x09)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH] FchKTXhciPassParameter find USB OEM table!\n");
    //Check OEM table size
    IDS_HDT_CONSOLE (FCH_TRACE,
                     "[FCH] USB OEM table length: %x, size of FCH_KT_USB_OEM_PLATFORM_TABLE %x\n",
                     PlatformUsbConfigureTable->TableLength,
                     sizeof (FCH_KT_USB_OEM_PLATFORM_TABLE));
    if (PlatformUsbConfigureTable->TableLength != sizeof (FCH_KT_USB_OEM_PLATFORM_TABLE)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH] USB OEM table length mismatched!\n");
    }
    FchKTXhciOemConfigure (DieBusNum, FchDataPtr);
  }
}

BOOLEAN
FchKTXhciCheckUsbPhySkip (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32 Presil_Ctrl0;

  ///< C2PMSG_97  (SMN address 0x3810A84)
  ///< SkipAllUSBControllerAccess:1;         ///< When set FW will skip all USB Control init communication from the FW
  ///< SkipAllUSBPhyAccess:1;                ///< When set FW will skip all USB phy communication from the FW
  FchSmnRead (DieBusNum, 0x3810A84, &Presil_Ctrl0, NULL);

  if (Presil_Ctrl0 & BIT10) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
FchKTXhciCheckUsbSkipFlag (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32 Presil_Ctrl0;

  ///< C2PMSG_97  (SMN address 0x3810A84)
  ///< SkipAllUSBControllerAccess:1;          ///< When set FW will skip all USB Control init communication from the FW
  ///< SkipAllUSBPhyAccess:1;                 ///< When set FW will skip all USB phy communication from the FW
  FchSmnRead (DieBusNum, 0x3810A84, &Presil_Ctrl0, NULL);

  if (Presil_Ctrl0 & BIT9) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * FchKTXhciInitBootProgram - Config Xhci controller during
 * Power-On
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTXhciInitBootProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram Start...\n");
  if (FchKTXhciCheckUsbSkipFlag (DieBusNum, FchDataPtr)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram SkipAllUSBControllerAccess...\n");
  } else {
    FchKTXhciPassParameter (DieBusNum, FchDataPtr);
    if (FchKTXhciCheckUsbPhySkip (DieBusNum, FchDataPtr)) {
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram SkipAllUSBPhyAccess...\n");
    } else {
      FchKTXhciSubSequenceEnable (DieBusNum, FchDataPtr);
    }
    FchKTXhciDdiModeEnable(DieBusNum, FchDataPtr);
    if (FchCheckRmbB0()) {
      FchKTXhciPdInterruptMode(DieBusNum, FchDataPtr);
    }
    FchKTXhciSmuServiceUsbInit (DieBusNum, FchDataPtr);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram Exit.\n");
}

/**
 * FchKTXhciInitS3ExitProgram - Config Xhci controller during
 * S3 Exit
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTXhciInitS3ExitProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitS3ExitProgram Start...\n");
  if (FchKTXhciCheckUsbSkipFlag (DieBusNum, FchDataPtr)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram SkipAllUSBControllerAccess...\n");
  } else {
    FchKTXhciPassParameter (DieBusNum, FchDataPtr);
    FchKTXhciDdiModeEnable(DieBusNum, FchDataPtr);
    if (!FchCheckRmbB0()) {
      // UsbSxExit message will be sent by USB4 module on B0
      FchKTXhciSmuService (DieBusNum, BIOSSMC_MSG_UsbSxExit);
    }
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitS3ExitProgram Exit.\n");
}

/**
 * FchKTXhciInitS3EntryProgram - Config Xhci controller before
 * entering S3
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTXhciInitS3EntryProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitS3EntryProgram Start...\n");
  if (FchKTXhciCheckUsbSkipFlag (DieBusNum, FchDataPtr)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitBootProgram SkipAllUSBControllerAccess...\n");
  } else {
    FchKTXhciCheckPwerSts (DieBusNum, FchDataPtr);
    FchKTXhciSmuService (DieBusNum, BIOSSMC_MSG_UsbSxEntry);
  }
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchKTXhciInitS3EntryProgram Exit.\n");
}

/**
 * FchKTXhciInitSsid - Update Xhci SSID
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Ssid       The SSID value to be updated
 *
 */
VOID
FchKTXhciInitSsid (
  IN  UINT32   DieBusNum,
  IN  UINT32   Ssid
  )
{
  FchSmnRW (DieBusNum, FCH_KT_USB0_SMN_PCICFG + 0x4C, 0x00, Ssid, NULL);
  FchSmnRW (DieBusNum, FCH_KT_USB1_SMN_PCICFG + 0x4C, 0x00, Ssid, NULL);
  FchSmnRW (DieBusNum, FCH_KT_USB2_SMN_PCICFG + 0x4C, 0x00, Ssid, NULL);
}

/**
 * FchKTDisableXhci2 - Disable USB2
 *
 *
 *
 */
VOID
FchKTDisableXhci2 (
  )
{
  FchSmnRW (0, FCH_KT_RCC_DEV2_PORT_STRAP0, ~(UINT32) BIT31, BIT31, NULL); //dummy Function 0
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);         //RCC_STRAP:RCC_DEV2_EPF0_STRAP0
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP0, 0xFFFF0000, 0x145A, NULL);
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP13, 0xFF000000, 0x00130000, NULL);    //RCC_STRAP:RCC_DEV2_EPF0_STRAP13
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP4, 0x807FFFFF, 0, NULL);              //RCC_STRAP:RCC_DEV1_EPF0_STRAP4
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP3, ~(UINT32) (BIT18 + BIT20), 0x00, NULL); //RCC_STRAP:RCC_DEV1_EPF0_STRAP3
  FchSmnRW (0, FCH_KT_USB2_NBIF_STRAP2, ~(UINT32) BIT8, 0x00, NULL); //RCC_STRAP:RCC_DEV1_EPF0_STRAP2
  FchSmnRW (0, FCH_KT_NBIF_VWR_VCHG_DIS_CTRL, 0xF80FFFFF, 0x07F00000, NULL); //NBIF_SMN_VWR_VCHG_DIS_CTRL[26:20]
}
