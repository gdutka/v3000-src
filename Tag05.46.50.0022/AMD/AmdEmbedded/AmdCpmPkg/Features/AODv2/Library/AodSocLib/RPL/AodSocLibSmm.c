/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <RPL/ApcbV3TokenUid.h>
#include <RPL/ApcbV3Priority.h>
#include <AmdSoc.h>
#include <Library/AmdSocBaseLib.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Features/AODv2/Include/Protocol/AodCmdSmmProtocol.h>
#include <Features/AODv2/Include/Aod.h>
#include <Features/AODv2/Include/Library/AodPlatformLib.h>
#include "AodSocLib.h"
#include "AodSyncApcb.h"

AOD_MOUDLE_NAME ("AodSocLibSmm")
AOD_SOC_NAME ("RPL")
/**
  This function used to get fused information of CCLK_Fmax

  @retval 0               the Smn addres is invalid
  @retval CCLK_Fmax value
**/
UINT16
AodRplGetCpuBoostMaxFreqBase (
  VOID
  )
{
  UINT32 Value32Lower;

  Value32Lower = 0;
  AodSmnRead (0, 0x5D586, &Value32Lower);
  if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
    return (UINT16)(((Value32Lower >> 3) & 0xFF) * 25);
  }
  return 0;
}


EFI_STATUS
EFIAPI
AodCmdToNvsCallbackRpl (
  IN  UINT32          CmdId,
  IN  UINT32          Data,
  IN  VOID            *Context
  )
{

  return EFI_SUCCESS;
}
EFI_STATUS
EFIAPI
AodInitRyzenMasterToNvsCallbackRpl (
  IN VOID         *Context
  )
{
  EFI_STATUS                                   Status;
  AOD_CMD_SMM_DISPATCH_PROTOCOL                *This;
  AOD_CMD_SMM_REGISTER_CONTEXT                 CmdContext;
  EFI_HANDLE                                   Handle;
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (AOD_CMD_SMM_DISPATCH_PROTOCOL *)Context;
  CmdContext.CallbackPhase = AOD_SMM_CMD_TO_NVS;
  CmdContext.CmdId         = 0;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodCmdToNvsCallbackRpl,
                   &Handle
                   );
  return Status;
}

EFI_STATUS
EFIAPI
AodCmdToVariableCallbackRpl (
  IN  UINT32          CmdId,
  IN  UINT32          CmdData,
  IN  VOID            *Context
  )
{
  AOD_CONFIG                  *AodConfig;
  DowncoreUnion               DownCore;
  AOD_LCLK_FREQ               LclkFreq;
  AOD_VDDG                    VddG;
  AOD_VDDG_IOD                VddGIod;
  AOD_CCD_FREQ_OPT            CcdFreq;
  AOD_GFX_PSM                 GfxPsm;
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  AodConfig = (AOD_CONFIG *)Context;

  switch (CmdId) {
    //
    // Memory addressing
    //
    case SET_INTERLEAVEMODE:
      //
      // Cmdid:Set Interleave Mode 0x00020031
      //
      AodConfig->AodMemIntlv = (UINT8)CmdData;
      break;
    case SET_INTERLEAVESIZE:
      //
      // Cmdid:Set Interleave Size 0x00020032
      //
      AodConfig->AodMemIntlvSize = (UINT8)CmdData;
      break;
    //
    // CPU OverClocking
    //
    case SET_SOFTWARE_DOWN_CORE:
      //
      // Cmdid:Software Downcore 0x00050001
      //
      DEBUG ((EFI_D_INFO, "Downcore start\n"));
      DownCore.ActiveCoreNum = (UINT16)CmdData;
      if (DownCore.DownCoreConfig.IsBitmap == 0) { // is down core index
        // not supported
        return EFI_UNSUPPORTED;
      } else {
        AOD_DEBUG_SOC ((EFI_D_INFO, "  Ccd : %x Core Bit Map: %x\n", \
          DownCore.DownCoreConfig.DieNum, DownCore.DownCoreConfig.DownCoreBitmap));
        if (DownCore.DownCoreConfig.DieNum > AodConfig->AodMaxCcdCount) return EFI_UNSUPPORTED;
        AodConfig->AodCoreBitMap[(UINT8)DownCore.DownCoreConfig.DieNum] = \
          (UINT8)DownCore.DownCoreConfig.DownCoreBitmap;
      }
      break;
    case SET_SMTEN :
      //
      // Cmdid:Set SMTEn 0x00020026
      //
      // Set Aod Varible:AodCpuSmtCtrl
      AodConfig->AodCpuSmtCtrl = (UINT8) CmdData;
      break;
    //
    // PBO Cmd to Variable
    //
    case SET_PPT_LIMIT :
      //
      // Cmdid:Set PPT Limit 0x00050001
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      //Condition AodNbioPBOAdvancedCtrl=Manual
      AodConfig->AodNbioPBOAdvancedCtrl = 0x2;
      // Set Aod Varible:AodNbioSocketPPTLimit
      AodConfig->AodNbioSocketPPTLimit = (UINT32) CmdData;
      break;
    case SET_TDC_LIMIT :
      //
      // Cmdid:Set TDC Limit 0x00050002
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      //Condition AodNbioPBOAdvancedCtrl=Manual
      AodConfig->AodNbioPBOAdvancedCtrl = 0x2;
      // Set Aod Varible:AodNbioTDCVDDLimit
      AodConfig->AodNbioTDCVDDLimit = (UINT32) CmdData;
      break;
    case SET_EDC_LIMIT :
      //
      // Cmdid:Set EDC Limit 0x00050003
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      //Condition AodNbioPBOAdvancedCtrl=Manual
      AodConfig->AodNbioPBOAdvancedCtrl = 0x2;
      // Set Aod Varible:AodNbioEDCVDDLimit
      AodConfig->AodNbioEDCVDDLimit = (UINT32) CmdData;
      break;
    case SET_SOC_TDC_LIMIT :
      //
      // Cmdid:Set Soc TDC Limit 0x0005000C
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      //Condition AodNbioPBOAdvancedCtrl=Manual
      AodConfig->AodNbioPBOAdvancedCtrl = 0x2;
      // Set Aod Varible:AodNbioTDCSOCLimit
      AodConfig->AodNbioTDCSOCLimit = (UINT32) CmdData;
      break;
    case SET_SOC_EDC_LIMIT :
      //
      // Cmdid:Set Soc EDC Limit 0x0005000D
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      //Condition AodNbioPBOAdvancedCtrl=Manual
      AodConfig->AodNbioPBOAdvancedCtrl = 0x2;
      // Set Aod Varible:AodNbioEDCSOCLimit
      AodConfig->AodNbioEDCSOCLimit = (UINT32) CmdData;
      break;
    case SET_SCALAR :
      //
      // Cmdid:Set Scalar 0x00050004
      //
      if (CmdData == 0) {
        //Condition AodNbioPBOCtl=Auto
        AodConfig->AodNbioPBOCtl = 0xff;
      } else {
        //Condition AodNbioPBOCtl=Advanced
        AodConfig->AodNbioPBOCtl = 0x2;
        //Condition AodNbioPBOScalarCtl=Manual
        AodConfig->AodNbioPBOScalarCtl = 0x1;
        // Set Aod Varible:AodNbioPBOScalar
        AodConfig->AodNbioPBOScalar = (UINT32) CmdData;
      }
      break;
    case SET_CURVE_OPT:
      //
      // Cmdid:Set Curve Optimizer 0x0005000A
      //
      AodConfig->AodCurveOptimizer = (UINT8) ((CmdData & 0xF0000000) >> 28);
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 2;
      if (AodConfig->AodCurveOptimizer == 1) { //All Core
        AodConfig->AodAllCoreCurveOptimizerSign = (UINT8) ((CmdData & 0x0F000000) >> 24);
        AodConfig->AodAllCoreCurveOptimizerMagnitude = (UINT8) (CmdData & 0x0FFFF);
      } else if (AodConfig->AodCurveOptimizer == 2) { //Per Core
        AodConfig->AodPerCoreCurveOptimizerSign[(UINT8) ((CmdData & 0x00FF0000)>> 16)] = \
          (UINT8) ( (CmdData & 0x0F000000)>> 24);
        AodConfig->AodPerCoreCurveOptimizerMagnitude[(UINT8) ((CmdData & 0x00FF0000)>> 16)] = \
          (UINT16) (CmdData & 0x0FFFF);
      } else if (AodConfig->AodCurveOptimizer == 0) { //Disable
      } else {
        return EFI_UNSUPPORTED;
      }
      break;
    case SET_GFX_CURVE_OPT:
      GfxPsm.Raw = (UINT8)CmdData;
      if (GfxPsm.GfxPsmData.Magnitude > GFX_CURVE_OPTIMIZER_MAGNITUDE_MAX) {
        return EFI_UNSUPPORTED;
      }
      switch (GfxPsm.GfxPsmData.State) {
        case GFX_CURVE_OPTIMIZER_DISABLE:
          AodConfig->AodGfxCurveOptimizer = 0; //Disable
          AodConfig->AodGfxCurveOptimizerSign = 0;
          AodConfig->AodGfxCurveOptimizerMagnitude = 0;
          break;
        case GFX_CURVE_OPTIMIZER_POSITIVE:
          AodConfig->AodNbioPBOCtl = 2;                                           //Advanced
          AodConfig->AodGfxCurveOptimizer = 1;                                    //GFX Curve Optimizer
          AodConfig->AodGfxCurveOptimizerSign = 0;                                //Positive
          AodConfig->AodGfxCurveOptimizerMagnitude = GfxPsm.GfxPsmData.Magnitude; //Magnitude
          break;
        case GFX_CURVE_OPTIMIZER_NEGATIVE:
          AodConfig->AodNbioPBOCtl = 2;                                           //Advanced
          AodConfig->AodGfxCurveOptimizer = 1;                                    //GFX Curve Optimizer
          AodConfig->AodGfxCurveOptimizerSign = 1;                                //Positive
          AodConfig->AodGfxCurveOptimizerMagnitude = GfxPsm.GfxPsmData.Magnitude; //Negative
          break;
        default:
          return EFI_UNSUPPORTED;
      }
      break;
    case SET_CCLK_FMAX:
      //
      // Cmdid:Set CCLK FMax 0x00020036
      //
      //Condition AodNbioPBOCtl=Advanced
      AodConfig->AodNbioPBOCtl = 0x2;
      if (CmdData == AodRplGetCpuBoostMaxFreqBase()) {
        //Condition AodCpuBoostClock=Enabled
        AodConfig->AodCpuBoostClock = 0;
        // Set Aod Varible:AodFMaxFreqIncrease
        AodConfig->AodFMaxFreqIncrease = 25;
        // Set Aod Varible:AodFMaxFreqDecrease
        AodConfig->AodFMaxFreqDecrease = 25;
      } else if (CmdData > AodRplGetCpuBoostMaxFreqBase()) {
        //Condition AodCpuBoostClock=Positve
        AodConfig->AodCpuBoostClock = 1;
        // Set Aod Varible:AodFMaxFreqIncrease
        AodConfig->AodFMaxFreqIncrease = (UINT8) (CmdData-AodRplGetCpuBoostMaxFreqBase());
      } else {
        //Condition AodCpuBoostClock=Negative
        AodConfig->AodCpuBoostClock = 2;
        // Set Aod Varible:AodFMaxFreqDecrease
        AodConfig->AodFMaxFreqDecrease = (UINT16) (AodRplGetCpuBoostMaxFreqBase()-CmdData);
      }
      break;
    //
    // Memory overclocking
    //
    case SET_MEM_CLOCK :
      //
      // Cmdid:Set Mem Clock 0x00020001
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      // Set Aod Varible:AodMemTargetSpeedDdr
      AodConfig->AodMemTargetSpeedDdr = (UINT16) (CmdData*2);
      break;
    case SET_TCL :
      //
      // Cmdid:Set Tcl 0x00020002
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTclCtrlDdr=Manual
      AodConfig->AodMemTimingTclCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTclDdr
      AodConfig->AodMemTimingTclDdr = (UINT16) CmdData;
      break;
    case SET_TRCD :
      //
      // Cmdid:Set Trcd 0x00020039
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrcdCtrlDdr=Manual
      AodConfig->AodMemTimingTrcdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrcdDdr
      AodConfig->AodMemTimingTrcdDdr = (UINT16) CmdData;
      break;
    case SET_TRP :
      //
      // Cmdid:Set Trp 0x00020006
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrpCtrlDdr=Manual
      AodConfig->AodMemTimingTrpCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrpDdr
      AodConfig->AodMemTimingTrpDdr = (UINT16) CmdData;
      break;
    case SET_TRAS :
      //
      // Cmdid:Set Tras 0x00020005
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrasCtrlDdr=Manual
      AodConfig->AodMemTimingTrasCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrasDdr
      AodConfig->AodMemTimingTrasDdr = (UINT16) CmdData;
      break;
    case SET_TRC :
      //
      // Cmdid:Set Trc 0x00020009
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrcCtrlDdr=Manual
      AodConfig->AodMemTimingTrcCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrcDdr
      AodConfig->AodMemTimingTrcDdr = (UINT16) CmdData;
      break;
    case SET_TWR :
      //
      // Cmdid:Set Twr 0x00020010
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrCtrlDdr=Manual
      AodConfig->AodMemTimingTwrCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrDdr
      AodConfig->AodMemTimingTwrDdr = (UINT16) CmdData;
      break;
    case SET_TRFC1 :
      //
      // Cmdid:Set Trfc1 0x0002003a
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrfc1CtrlDdr=Manual
      AodConfig->AodMemTimingTrfc1CtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrfc1Ddr
      AodConfig->AodMemTimingTrfc1Ddr = (UINT16) CmdData;
      break;
    case SET_TRFC2 :
      //
      // Cmdid:Set Trfc2 0x0002000B
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrfc2CtrlDdr=Manual
      AodConfig->AodMemTimingTrfc2CtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrfc2Ddr
      AodConfig->AodMemTimingTrfc2Ddr = (UINT16) CmdData;
      break;
    case SET_TRFCSB :
      //
      // Cmdid:Set TrfcSb 0x0002003b
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrfcSbCtrlDdr=Manual
      AodConfig->AodMemTimingTrfcSbCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrfcSbDdr
      AodConfig->AodMemTimingTrfcSbDdr = (UINT16) CmdData;
      break;
    case SET_TRTP :
      //
      // Cmdid:Set Trtp 0x00020015
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrtpCtrlDdr=Manual
      AodConfig->AodMemTimingTrtpCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrtpDdr
      AodConfig->AodMemTimingTrtpDdr = (UINT16) CmdData;
      break;
    case SET_TRRDL :
      //
      // Cmdid:Set TrrdL 0x0002000F
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrrdLCtrlDdr=Manual
      AodConfig->AodMemTimingTrrdLCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrrdLDdr
      AodConfig->AodMemTimingTrrdLDdr = (UINT16) CmdData;
      break;
    case SET_TRRDS :
      //
      // Cmdid:Set TrrdS 0x0002000E
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrrdSCtrlDdr=Manual
      AodConfig->AodMemTimingTrrdSCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrrdSDdr
      AodConfig->AodMemTimingTrrdSDdr = (UINT16) CmdData;
      break;
    case SET_TFAW :
      //
      // Cmdid:Set Tfaw 0x0002000D
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTfawCtrlDdr=Manual
      AodConfig->AodMemTimingTfawCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTfawDdr
      AodConfig->AodMemTimingTfawDdr = (UINT16) CmdData;
      break;
    case SET_TWTRL :
      //
      // Cmdid:Set TwtrL 0x00020012
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwtrLCtrlDdr=Manual
      AodConfig->AodMemTimingTwtrLCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwtrLDdr
      AodConfig->AodMemTimingTwtrLDdr = (UINT16) CmdData;
      break;
    case SET_TWTRS :
      //
      // Cmdid:Set TwtrS 0x00020011
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwtrSCtrlDdr=Manual
      AodConfig->AodMemTimingTwtrSCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwtrSDdr
      AodConfig->AodMemTimingTwtrSDdr = (UINT16) CmdData;
      break;
    case SET_TRDRDSCL :
      //
      // Cmdid:Set TrdrdScL 0x00020017
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrdrdScLCtrlDdr=Manual
      AodConfig->AodMemTimingTrdrdScLCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrdrdScLDdr
      AodConfig->AodMemTimingTrdrdScLDdr = (UINT16) CmdData;
      break;
    case SET_TRDRDSC :
      //
      // Cmdid:Set TrdrdSc 0x00020016
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrdrdScCtrlDdr=Manual
      AodConfig->AodMemTimingTrdrdScCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrdrdScDdr
      AodConfig->AodMemTimingTrdrdScDdr = (UINT16) CmdData;
      break;
    case SET_TRDRDSD :
      //
      // Cmdid:Set TrdrdSd 0x00020018
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrdrdSdCtrlDdr=Manual
      AodConfig->AodMemTimingTrdrdSdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrdrdSdDdr
      AodConfig->AodMemTimingTrdrdSdDdr = (UINT16) CmdData;
      break;
    case SET_TRDRDDD :
      //
      // Cmdid:Set TrdrdDd 0x00020019
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrdrdDdCtrlDdr=Manual
      AodConfig->AodMemTimingTrdrdDdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrdrdDdDdr
      AodConfig->AodMemTimingTrdrdDdDdr = (UINT16) CmdData;
      break;
    case SET_TWRWRSCL :
      //
      // Cmdid:Set TwrwrScL 0x0002001B
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrwrScLCtrlDdr=Manual
      AodConfig->AodMemTimingTwrwrScLCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrwrScLDdr
      AodConfig->AodMemTimingTwrwrScLDdr = (UINT16) CmdData;
      break;
    case SET_TWRWRSC :
      //
      // Cmdid:Set TwrwrSc 0x0002001A
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrwrScCtrlDdr=Manual
      AodConfig->AodMemTimingTwrwrScCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrwrScDdr
      AodConfig->AodMemTimingTwrwrScDdr = (UINT16) CmdData;
      break;
    case SET_TWRWRSD :
      //
      // Cmdid:Set TwrwrSd 0x0002001C
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrwrSdCtrlDdr=Manual
      AodConfig->AodMemTimingTwrwrSdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrwrSdDdr
      AodConfig->AodMemTimingTwrwrSdDdr = (UINT16) CmdData;
      break;
    case SET_TWRWRDD :
      //
      // Cmdid:Set TwrwrDd 0x0002001D
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrwrDdCtrlDdr=Manual
      AodConfig->AodMemTimingTwrwrDdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrwrDdDdr
      AodConfig->AodMemTimingTwrwrDdDdr = (UINT16) CmdData;
      break;
    case SET_TWRRD :
      //
      // Cmdid:Set Twrrd 0x0002001F
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTwrrdCtrlDdr=Manual
      AodConfig->AodMemTimingTwrrdCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTwrrdDdr
      AodConfig->AodMemTimingTwrrdDdr = (UINT16) CmdData;
      break;
    case SET_TRDWR :
      //
      // Cmdid:Set Trdwr 0x0002001E
      //
      //Condition AodMemTimingSettingDdr=Enabled
      AodConfig->AodMemTimingSettingDdr = 0x1;
      //Condition AodMemTimingTrdwrCtrlDdr=Manual
      AodConfig->AodMemTimingTrdwrCtrlDdr = 0x1;
      // Set Aod Varible:AodMemTimingTrdwrDdr
      AodConfig->AodMemTimingTrdwrDdr = (UINT16) CmdData;
      break;
    case SET_RTTNOMWR :
      //
      // Cmdid:Set RttNomWr 0x0002003c
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgRttNomWrDdr
      AodConfig->AodMemDataBusCfgRttNomWrDdr = (UINT8) CmdData;
      break;
    case SET_RTTNOMRD :
      //
      // Cmdid:Set RttNomRd 0x0002003e
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgRttNomRdDdr
      AodConfig->AodMemDataBusCfgRttNomRdDdr = (UINT8) CmdData;
      break;
    case SET_RTTWR :
      //
      // Cmdid:Set RttWr 0x00020023
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgRttWrDdr
      AodConfig->AodMemDataBusCfgRttWrDdr = (UINT8) CmdData;
      break;
    case SET_RTTPARK :
      //
      // Cmdid:Set RttPark 0x00020024
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgRttParkDdr
      AodConfig->AodMemDataBusCfgRttParkDdr = (UINT8) CmdData;
      break;
    case SET_RTTPARKDQS :
      //
      // Cmdid:Set RttParkDqs 0x0002003f
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgRttParkDqsDdr
      AodConfig->AodMemDataBusCfgRttParkDqsDdr = (UINT8) CmdData;
      break;
    case SET_PROCODT :
      //
      // Cmdid:Set ProcODT 0x00020007
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgProcOdtDdr
      AodConfig->AodMemDataBusCfgProcOdtDdr = (UINT8) CmdData;
      break;
    case SET_PROCDATADRIVESTRENGTH :
      //
      // Cmdid:Set ProcDataDriveStrength 0x0002003d
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgProcDataDrvStrenDdr
      AodConfig->AodMemDataBusCfgProcDataDrvStrenDdr = (UINT8) CmdData;
      break;
    case SET_DRAMDATADRIVESTRENGTH:
      //
      // Cmdid:Set DRAMDataDriveStrength 0x0002003e
      //
      //Condition AodMemDataBusCfgCtlDdr=Manual
      AodConfig->AodMemDataBusCfgCtlDdr = 0x1;
      // Set Aod Varible:AodMemDataBusCfgDramDataDrvStrenDdr
      AodConfig->AodMemDataBusCfgDramDataDrvStrenDdr = (UINT8) CmdData;
      break;
    case SET_POWERDWONEN :
      //
      // Cmdid:Set PowerDwonEn 0x00020025
      //
      // Set Aod Varible:AodMemCtrllerPowerDownEnDdr
      AodConfig->AodMemCtrllerPowerDownEnDdr = (UINT8) CmdData;
      break;
    case SET_FCLK_FREQUENCY:
      //
      // Cmdid:Set FCLK Frequency 0x00020035
      //
      // Set Aod Varible:AodFclkFrequency
      AodConfig->AodFclkFrequency = (UINT16) CmdData;
      break;
    case SET_UCLK_DIV1:
      //
      // Cmdid:Set UCLK DIV1 0x00020044
      //
      // Set Aod Varible:AodUclkDiv1Mode
      AodConfig->AodUclkDiv1Mode = (UINT8) CmdData;
      break;
    case SET_VPP:
      //
      // Cmdid:Set VPP 0x00030003
      //
      //Condition AodVppCtrl=Manual
      AodConfig->AodVppCtrl = 1;
      // Set Aod Varible:AodVppVoltage
      AodConfig->AodVppVoltage = (UINT16) CmdData;
      break;
    case SET_VDDIO:
      //
      // Cmdid:Set VDDIO 0x00030001
      //
      //Condition AodVddIoCtrl=Manual|AodVddIoCtrl=Separate
      AodConfig->AodVddIoCtrl = 1;
      //Set Aod Varible:AodFclkFrequency
      AodConfig->AodDimmVddIo = (UINT16) CmdData;
      AodConfig->AodVddIoCtrl = (AodConfig->AodDimmVddIo == AodConfig->AodApuVddIo)?1:2;
      break;
    case SET_APU_VDDIO:
      //
      // Cmdid:Set APU VDDIO 0x00030004
      //
      //Condition AodVddIoCtrl = Separate
      AodConfig->AodVddIoCtrl = 2;
      //Set Aod Varible:AodFclkFrequency
      AodConfig->AodApuVddIo = (UINT16) CmdData;
      break;
    case SET_FCLK_OC_MODE:
      //
      // Cmdid:Set FCLK OC Mode 0x00020033
      //
      //Set Aod Varible:AodGnbFclkOcFlag
      AodConfig->AodGnbFclkOcFlag = (UINT8) CmdData;
      break;
    case SET_SOCVID:
      //
      // Cmdid:Set SOC VID 0x00020034
      //
      //Set Aod Varible:AodSocOverclockingVoltage
      AodConfig->AodSocOverclockingVoltage =  (UINT16)VID3_TO_MV((UINT8)CmdData);
      break;
    case SET_CLDO_VDDP:
      //
      // Cmdid:Set CLDO_VDDP 0x00020030
      //
      //Condition AodVddpVoltageCtrl=Manual
      AodConfig->AodVddpVoltageCtrl = 1;
      //Set Aod Varible:AodVddpVoltage
      AodConfig->AodVddpVoltage = (UINT16)CmdData;
      break;
    case SET_LCLK_FREQ:
      //
      // Cmdid: Set LCLK Freq 0x00020043
      //
      LclkFreq.Raw = CmdData;
      //Condition AodLclkCtrl=Manual
      AodConfig->AodLclkCtrl = (UINT8)LclkFreq.Bits.State;
      if (LclkFreq.Bits.MaxOrMin) {
        //Set Aod Varible:AodMaxLclkFreq
        AodConfig->AodMaxLclkFreq = (UINT16)LclkFreq.Bits.Frequency;
        AodConfig->AodMinLclkFreq = \
          (AodConfig->AodMaxLclkFreq >= AodConfig->AodMinLclkFreq)?AodConfig->AodMinLclkFreq:AodConfig->AodMaxLclkFreq;
      } else {
        //Set Aod Varible:AodMinLclkFreq
        AodConfig->AodMinLclkFreq = (UINT16)LclkFreq.Bits.Frequency;
        AodConfig->AodMaxLclkFreq = \
          (AodConfig->AodMaxLclkFreq >= AodConfig->AodMinLclkFreq)?AodConfig->AodMaxLclkFreq:AodConfig->AodMinLclkFreq;
      }
      break;
    case SET_VDD_MISC:
      //
      // Cmdid: Set VDD_MISC 0x00030006
      //
      //Condition AodVddMiscCtrl=Manual
      AodConfig->AodVddMiscCtrl = 1;
      //Set Aod Varible:AodVddMiscVoltage
      AodConfig->AodVddMiscVoltage = (UINT16)CmdData;
      break;
    case SET_VDDG:
      //
      // Cmdid: Set CLDO VDDG 0x00020038
      //
      VddG.Raw = (UINT16)CmdData;
      if (VddG.Bits.PerCcd == 0) {
        //Condition AodVddgCtrl=Global VDDG Voltage Control
        AodConfig->AodVddgCtrl = 1;  // Manual
        //Set Aod Varible:AodGlobalVddgCcdVoltage
        AodConfig->AodGlobalVddgCcdVoltage = VddG.Bits.Voltage;
      }
      if (VddG.Bits.PerCcd == 1) {
        //Condition AodVddgCtrl=Per-CCD VDDG Voltage Control
        AodConfig->AodVddgCtrl = 2;  // Per Ccd
        //Set Aod Varible:AodPerCcdVddgVoltage
        AodConfig->AodPerCcdVddgVoltage[VddG.Bits.Ccd] = VddG.Bits.Voltage;
      }
      break;
    case SET_VDDG_IOD:
      //
      // Cmdid: Set VDDG IOD 0x0005000B
      //
      VddGIod.Raw =  (UINT16)CmdData;
      if (VddGIod.Bits.PerCcd == 0) {
        //Condition AodVddgCtrl=Global VDDG Voltage Control
        AodConfig->AodVddgCtrl = 1;  // Manual
        //Set Aod Varible:AodGlobalVddgIodVoltage
        AodConfig->AodGlobalVddgIodVoltage = VddGIod.Bits.Voltage;
      }
      if (VddGIod.Bits.PerCcd == 1) {
        //Condition AodVddgCtrl=Per-CCD VDDG Voltage Control
        AodConfig->AodVddgCtrl = 2;  // Per Ccd
        //Set Aod Varible:AodPerCcdVddgIODVoltage
        AodConfig->AodPerCcdVddgIODVoltage[VddGIod.Bits.Ccd] = VddGIod.Bits.Voltage;
      }
      break;
    case SET_CCD_FREQ_OPT:
      //
      // Cmdid: Set CCD Freq Optimizer 0x00050011
      //
      CcdFreq.Raw = CmdData;
      //Condition AodCcdOptimizerFreqCtrl=CCD Frequency Adjust
      AodConfig->AodCcdOptimizerFreqCtrl = (UINT8)CcdFreq.Bits.Enable;
      if (AodConfig->AodCcdOptimizerFreqCtrl) {
        //Set Aod Varible:AodCcdOptimizerFreq
        AodConfig->AodCcdOptimizerFreq[CcdFreq.Bits.Ccd] = (UINT16)CcdFreq.Bits.Freq;
      }
      break;
    default:
      return EFI_SUCCESS;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AodCmdToOnboardVoltageCallbackRpl (
  IN  UINT32          CmdId,
  IN  UINT32          CmdData,
  IN  VOID            *Context
  )
{
  EFI_STATUS     Status = EFI_UNSUPPORTED;
  switch (CmdId) {
    case SET_APU_VDDIO:
      Status = AodSetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, (UINT16) CmdData);
      break;
    default:
      break;
  }
  return Status;
}
EFI_STATUS
EFIAPI
AodOnboardVoltageVariableHookRpl (
  IN  UINT32          CmdId,
  IN  UINT32          CmdData,
  IN  VOID            *Context
  )
{
  AOD_CONFIG      *AodConfig;
  EFI_STATUS      Status = EFI_SUCCESS;
  if (Context == NULL) return EFI_INVALID_PARAMETER;

  AodConfig = (AOD_CONFIG  *)Context;
  if ((AodConfig->AodVddIoCtrl == 1) || (AodConfig->AodVddIoCtrl == 2)) {
    Status = AodSetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, AodConfig->AodApuVddIo);
  }
  return Status;
}

EFI_STATUS
EFIAPI
AodInitRyzenMasterToVariableCallbackRpl (
  IN VOID         *Context
  )
{
  EFI_STATUS                                   Status;
  AOD_CMD_SMM_DISPATCH_PROTOCOL                *This;
  AOD_CMD_SMM_REGISTER_CONTEXT                 CmdContext;
  EFI_HANDLE                                   Handle;
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (AOD_CMD_SMM_DISPATCH_PROTOCOL *)Context;
  CmdContext.CallbackPhase = AOD_SMM_CMD_TO_VARIABLE;
  CmdContext.CmdId         = 0;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodCmdToVariableCallbackRpl,
                   &Handle
                   );

  CmdContext.CallbackPhase = AOD_SMM_CMD_SPECAIL;
  CmdContext.CmdId         = SET_APU_VDDIO;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodCmdToOnboardVoltageCallbackRpl,
                   &Handle
                   );
  CmdContext.CallbackPhase = AOD_SMM_CMD_VARIABLE_HOOK;
  CmdContext.CmdId         = 0;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodOnboardVoltageVariableHookRpl,
                   &Handle
                   );
  return Status;
}


EFI_STATUS
EFIAPI
AodVariableToApcbCallbackRpl (
  IN  UINT32          CmdId,
  IN  UINT32          CmdData,
  IN  VOID            *Context
  )
{
  EFI_STATUS                            Status;
  AMD_APCB_SERVICE_PROTOCOL             *ApcbProtocol;
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = gSmst->SmmLocateProtocol (
              &gAmdApcbSmmServiceProtocolGuid,
              NULL,
              &ApcbProtocol
              );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  return AodUpdateApcbTokensRpl (Context, ApcbProtocol);
}

EFI_STATUS
EFIAPI
AodInitRyzenMasterToApcbCallbackRpl (
  IN VOID         *Context
  )
{
  EFI_STATUS                                   Status;
  AOD_CMD_SMM_DISPATCH_PROTOCOL                *This;
  AOD_CMD_SMM_REGISTER_CONTEXT                 CmdContext;
  EFI_HANDLE                                   Handle;
  if (Context == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (AOD_CMD_SMM_DISPATCH_PROTOCOL *)Context;
  CmdContext.CallbackPhase = AOD_SMM_VARIABLE_TO_APCB;
  CmdContext.CmdId         = 0;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodVariableToApcbCallbackRpl,
                   &Handle
                   );
  CmdContext.CallbackPhase = AOD_SMM_CMD_VARIABLE_HOOK;
  CmdContext.CmdId         = 0;
  Status = This->Register (
                   This,
                   &CmdContext,
                   AodVariableToApcbCallbackRpl,
                   &Handle
                   );
  return Status;
}
///
/// Aod Rpl Smm init table
///
AOD_INIT_TABLE mRplAodSmmInit [] = {
  { 0, AOD_UPDATE_NVS,       AodInitRyzenMasterToNvsCallbackRpl},
  { 0, AOD_UPDATE_VARIABLE,  AodInitRyzenMasterToVariableCallbackRpl},
  { 0, AOD_CONFIG_SYNC_APCB, AodInitRyzenMasterToApcbCallbackRpl},
  { 0, 0, NULL}
};

/**
  The library constructuor.

  The function does the necessary initialization work for this library
  instance.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
                                It will ASSERT on error for debug version.
  @retval     EFI_ERROR         Please reference LocateProtocol for error code details.
**/
EFI_STATUS
EFIAPI
AodSmmRplConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SOC_ID_STRUCT       SocId;

  // Check Hardware Identification RPL AM5
  SocId.SocFamilyID = F19_RPL_RAW_ID;
  SocId.PackageType = ZEN4_PKG_AM5;
  if (SocHardwareIdentificationCheck (&SocId)) {
    return AodAddInitTable (mRplAodSmmInit);
  }

  return EFI_SUCCESS;
}