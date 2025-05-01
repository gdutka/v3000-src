/** @file
  Provide xHCI controller initialize function
;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <AGESA.h>
#include <GnbDxio.h>
#include <Library/NbioSmuV13Lib.h>
#include <SMU_RMB_MsgDef.h>
#include <FchRegistersKT.h>
#include <AmdRasRegistersRmb.h>
#include "XhciEralyInit.h"

/**
 Enable and initialize Starship CPU internal xHCI controller

 @param[in]  DieBusNum           The BUS number of the CPU DIE.

 @retval EFI_SUCCESS             xHCI controller was initializing finished.
 @retval EFI_DEVICE_ERROR        xHCI controller can't be setting to D0 state.
**/
EFI_STATUS
RmbXhciControllerInit (
  IN  UINT32     DieBusNum
  )
{
  PCI_ADDR     NbioPciAddress;
  UINT32       SmuArg[6];
  UINT32       SmuMessageResponse;

  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);

  // Set Over Current
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_OCMAPPING0;
  SmuArg[1] = 0x00FFFFFF;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_OCMAPPING0;
  SmuArg[1] = 0x000FFFFF;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // Set Oc Polarity
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = BIT8;
  SmuArg[2] = BIT8;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = BIT8;
  SmuArg[2] = BIT8;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // Set Device Removable
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_DEVICE_REMOVABLE;
  SmuArg[1] = 0x0003000F;
  SmuArg[2] = 0;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_DEVICE_REMOVABLE;
  SmuArg[1] = 0x00030007;
  SmuArg[2] = 0;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // Set Usb20 PhyConfigure
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 0);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 0);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 1);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 1);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 2);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 2);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 3);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 3);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 0);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 0);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 1);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 1);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL0 + (FCH_KT_USB20_PHY_SMN_STEP * 2);
  SmuArg[1] = 0xF3BF7007;
  SmuArg[2] = 0x32233001;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB20_LANEPARACTL1 + (FCH_KT_USB20_PHY_SMN_STEP * 2);
  SmuArg[1] = 0x0000000F;
  SmuArg[2] = 0xB;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // Set Usb30 PhyConfigure
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_PHYPARACTL10 + (FCH_KT_USB3_PHY_SMN_STEP * 0);
  SmuArg[1] = 0x00000707;
  SmuArg[2] = 0x00000202;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_PHYPARACTL10 + (FCH_KT_USB3_PHY_SMN_STEP * 1);
  SmuArg[1] = 0x00000707;
  SmuArg[2] = 0x00000202;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = 0x0000F000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  SmuArg[1] = 0x000000F0;
  SmuArg[2] = 0x00000050;
  SmuArg[3] = 0x01000002;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = 0x0000F000;
  SmuArg[2] = 0x00001000;
  SmuArg[3] = 0x01000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  SmuArg[1] = 0x000000F0;
  SmuArg[2] = 0x00000050;
  SmuArg[3] = 0x01000002;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_PHYPARACTL10 + (FCH_KT_USB3_PHY_SMN_STEP * 0);
  SmuArg[1] = 0x00000707;
  SmuArg[2] = 0x00000202;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_PHYPARACTL10 + (FCH_KT_USB3_PHY_SMN_STEP * 1);
  SmuArg[1] = 0x00000707;
  SmuArg[2] = 0x00000202;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = 0x0000F000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  SmuArg[1] = 0x000000F0;
  SmuArg[2] = 0x00000050;
  SmuArg[3] = 0x01100002;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB_PORT_CONTROL;
  SmuArg[1] = 0x0000F000;
  SmuArg[2] = 0x00001000;
  SmuArg[3] = 0x01100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = FCH_KT_USB3_SUP_DIG_LVL_OVRD_IN;
  SmuArg[1] = 0x000000F0;
  SmuArg[2] = 0x00000050;
  SmuArg[3] = 0x01100002;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x00000000;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0xC0000001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // SubSequenceEnable
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x00000000;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0xC0100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x00000000;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0xC0100002;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  // DdiModeEnable
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x00000000;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0xD0100000;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x00000000;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0xE0100001;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbConfigUpdate, SmuArg, 0);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = 0x8303001F;
  SmuArg[1] = 0x00000000;
  SmuArg[2] = 0x00000000;
  SmuArg[3] = 0x00000000;
  SmuMessageResponse = NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_UsbInit, SmuArg, 0);

  return EFI_SUCCESS;
}

/**
 Enable and initialize CPU internal xHCI controller

 @param[in]  DieBusNum           The BUS number of the CPU DIE.

 @retval EFI_SUCCESS             xHCI controller was initializing finished.
 @retval EFI_DEVICE_ERROR        xHCI controller can't be setting to D0 state.
 @retval EFI_UNSUPPORTED         xHCI controller not supported.
**/
EFI_STATUS
XhciControllerInit (
  IN  UINT32     DieBusNum
  )
{
  EFI_STATUS   Status;

  Status = RmbXhciControllerInit (DieBusNum);

  return Status;
}
