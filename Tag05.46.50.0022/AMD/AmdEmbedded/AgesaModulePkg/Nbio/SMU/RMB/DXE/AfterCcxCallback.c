/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * Callback after CCX initialization is complete
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioSmuV13Dxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include "SmuV13Dxe.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <GnbDxio.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Guid/HobList.h>
#include <Library/UefiLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/BaseLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Protocol/MpService.h>
#include <Protocol/AmdCcxProtocol.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>
#include <NbioSmu.h>

#define FILECODE        NBIO_SMU_RMB_DXE_AFTERCCXCALLBACK_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
extern VOID EFIAPI SmuFinalInit (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
  );
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *  SetScratchRegisterToRetainMem
 *
 *  Description:
 *     Used to set the memory clear bit for subsequent ABL consumption
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
SetScratchRegisterToRetainMem(
  IN    UINT8   Value  
)
{
  UINT32                SmuArg[6];
  EFI_STATUS            Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  PCIe_PLATFORM_CONFIG  *PcieFromHob;
  GNB_HANDLE            *GnbHandle;
  
  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;

  GnbHandle = NbioGetHandle (Pcie);

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = (UINT32) Value;    // 1- Retain 0- clear 
  IDS_HDT_CONSOLE (MAIN_FLOW, "BIOSSMC_MSG_SetScratchRegister: smuArg[0] = 0%x\n", SmuArg[0]);

  Status = SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetScratchRegister, SmuArg[0], 0, 0, 0, 0, 0);
  IDS_HDT_CONSOLE (MAIN_FLOW, "BIOSSMC_MSG_SetScratchRegister: Status = %r\n", Status);

  return Status;
}

EFI_STATUS
PMemCmdSend (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
)
{
  UINT32                SmuArg[6];
  EFI_STATUS            Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  PCIe_PLATFORM_CONFIG  *PcieFromHob;
  GNB_HANDLE            *GnbHandle;

  gBS->CloseEvent(Event);
  
  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;

  GnbHandle = NbioGetHandle (Pcie);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  
  SmuArg[0] = 0;
  IDS_HDT_CONSOLE (MAIN_FLOW, "BIOSSMC_MSG_EnableNVMSelfRefresh: smuArg[0] = 0%x\n", SmuArg[0]);
  
  Status = SmuServiceRequest (GnbHandle, BIOSSMC_MSG_EnableNVMSelfRefresh, SmuArg[0], 0, 0, 0, 0, 0);
  IDS_HDT_CONSOLE (MAIN_FLOW, "BIOSSMC_MSG_EnableNVMSelfRefresh: Status = %r\n", Status);

  if (!EFI_ERROR (Status) && (PcdGet8(PcdPmemRetention) != 0xFF)) {
    SetScratchRegisterToRetainMem(PcdGet8(PcdPmemRetention)); //1 - Retain, 0 - Clear(Default)
  }
  
  return Status;
}


/**
 *---------------------------------------------------------------------------------------
 *  CallbackAfterCCxDxeDone
 *
 *  Description:
 *     notification event handler after gAmdCcxDxeInitCompleteProtocolGuid ready
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
CallbackAfterCcxDxeDone (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
  )
{
  EFI_STATUS                           CalledStatus, Status;
  PCIe_PLATFORM_CONFIG                 *PcieFromHob;
  PCIe_PLATFORM_CONFIG                 *Pcie;
  GNB_HANDLE                           *GnbHandle;
  DXE_AMD_CCX_INIT_COMPLETE_PROTOCOL   *CcxDxeInitCompleteProtocol;
  VOID                                 *Registration;
  UINT32                               SmuArg[6];
  PRESIL_CTRL1                         PreSilCtrl1;
  CORE_PSM_OFFSET                      *PsmOffsetArray;
  EFI_EVENT                            ReadyToBootEvent;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a First notify\n", __FUNCTION__);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  //
  // Add more check to locate protocol after got event, because
  // the library will signal this event immediately once it is register
  // just in case it is already installed.
  //
  CalledStatus = gBS->LocateProtocol (
                  &gAmdCcxDxeInitCompleteProtocolGuid,
                  NULL,
                  &CcxDxeInitCompleteProtocol
                  );
  if (EFI_ERROR (CalledStatus)) {
    return;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);

  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;

  GnbHandle = NbioGetHandle (Pcie);

  //
  // BIOS call BIOSSMC_MSG_EnableAllSmuFeatures with a bitmask indicating which SMU features to enable.
  // Masks are allocated smu11.h
  // Each mask bit should be linked to a setup option to enable/disable it. If DC BTC is enabled alter sequence
  //

  IDS_HDT_CONSOLE (MAIN_FLOW, "Set SmuFeatureControls, Extended : 0x%x, Standard : 0x%x\n", PcdGet32 (PcdSmuFeatureControlDefinesExt), PcdGet32 (PcdSmuFeatureControlDefines));

  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 0) {
    if (PcdGet8 (PcdDcBtc) != TRUE) {
      SmuServiceRequest(
        GnbHandle,
        BIOSSMC_MSG_EnableSmuFeatures,
        PcdGet32 (PcdSmuFeatureControlDefines),
        PcdGet32 (PcdSmuFeatureControlDefinesExt),
        PcdGet32 (PcdSmuFeatureControlDefines64),
        0,
        0,
        0
        );

      if (PcdGetBool (PcdFllBtcEnable) == TRUE) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "FllBtc Enable \n");
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_FllBootTimeCalibration, 0, 0, 0, 0, 0, 0);
      }

    } else {
      // Send enable mask with CC6, PSI masked out if DC BTC is enabled. Initialize DC BTC and then send CC6 enable separately
      SmuServiceRequest (
        GnbHandle,
        BIOSSMC_MSG_EnableSmuFeatures,
        PcdGet32 (PcdSmuFeatureControlDefines),
        PcdGet32 (PcdSmuFeatureControlDefinesExt) & (~(UINT32)(FEATURE_CC6_MASK | FEATURE_PSI_MASK)),
        PcdGet32 (PcdSmuFeatureControlDefines64),
        0,
        0,
        0
        );
      IDS_HDT_CONSOLE (MAIN_FLOW, "DcBtc Enable on socket %d\n", GnbHandle->SocketId);
      IDS_HDT_CONSOLE (MAIN_FLOW, "DcBtc VID is %x and margin %x\n", PcdGet8 (PcdDcBtcVid), PcdGet8 (PcdDcBtcErrorOfsetVoltageMargin));
      // VID is LSB 8 bits, Voltage margin the next significant 8 bits. PCDs remain at their default unless overriden
      SmuServiceRequest (
        GnbHandle,
        BIOSSMC_MSG_DcBtc,
        (UINT32)PcdGet8 (PcdDcBtcVid) | (((UINT32)PcdGet8 (PcdDcBtcErrorOfsetVoltageMargin)) << 8),
        0,
        0,
        0,
        0,
        0
        );

      if (PcdGetBool (PcdFllBtcEnable) == TRUE) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "FllBtc Enable \n");
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_FllBootTimeCalibration, 0, 0, 0, 0, 0, 0);
      }

      // Send CC6, PSI feature bit if it still needs to be enabled after DC BTC
      SmuServiceRequest (
        GnbHandle,
        BIOSSMC_MSG_EnableSmuFeatures,
        0,
        PcdGet32 (PcdSmuFeatureControlDefinesExt) & (UINT32)(FEATURE_CC6_MASK | FEATURE_PSI_MASK),
        0,
        0,
        0,
        0
        );
    }
  }

    //
    //  Ulv Vid Offset Control
    //
    PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
    if (PreSilCtrl1.Field.FASTSIM_PWR_MSGS == 0) {
      // IDS_HDT_CONSOLE (MAIN_FLOW, "PcdUlvVidCtrl = 0%x\n", PcdGetBool (PcdUlvVidCtrl));
      // IDS_HDT_CONSOLE (MAIN_FLOW, "PcdUlvVidOffset = 0%x\n", PcdGet32 (PcdUlvVidOffset));
      // if (PcdGetBool (PcdUlvVidCtrl) == TRUE) {
      //   // Send enable Ulv Vid override
      //   SmuServiceRequest (
      //     GnbHandle,
      //     BIOSSMC_MSG_SetUlvVidOffset,
      //     PcdGet32 (PcdUlvVidOffset),
      //     0,
      //     0,
      //     0,
      //     0,
      //     0
      //     );
      // }
      //
      //  Vdd off Vid Offset Control
      //
      IDS_HDT_CONSOLE (MAIN_FLOW, "PcdVddOffVidCtrl = 0%x\n", PcdGetBool (PcdVddOffVidCtrl));
      IDS_HDT_CONSOLE (MAIN_FLOW, "PcdVddOffVid = 0%x\n", PcdGet32 (PcdVddOffVid));
      if (PcdGetBool (PcdVddOffVidCtrl) == TRUE) {
        if (PcdGet32 (PcdVddOffVid) >= 250) {
          SmuArg[0] = (((PcdGet32 (PcdVddOffVid)) - 250) * 200 + 1500) / 1000;
        } else {
          SmuArg[0] = 0;
        }

        SmuServiceRequest (
          GnbHandle,
          BIOSSMC_MSG_SetVddOffVid,
          SmuArg[0],
          0,
          0,
          0,
          0,
          0
          );
      }
    }

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    PMemCmdSend,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );

//    if (PcdGetBool (PcdOcDisable)) {
//      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//      SmuArg[0] = 1;
//      CalledStatus = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_OC_DISABLE, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
//      Status = (CalledStatus > Status) ? CalledStatus : Status;
//    } else {
//      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//      SmuArg[0] = (UINT32) PcdGet16 (PcdOcVoltageMax);
//      if (SmuArg[0] != 0) {
//        CalledStatus = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_OC_VOLTAGEMAX, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
//        Status = (CalledStatus > Status) ? CalledStatus : Status;
//      }
//      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//      SmuArg[0] = (UINT32) PcdGet16 (PcdOcFrequencyMax);
//      if (SmuArg[0] != 0) {
//        CalledStatus = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_OC_FREQMAX, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
//        Status = (CalledStatus > Status) ? CalledStatus : Status;
//      }
//    }
//    GnbHandle = GnbGetNextHandle (GnbHandle);
//  }

  //
  // Apply those changes after all feature enable
  //

  if (PcdGet32 (PcdSlowPPTLimitApuOnly) != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetSlowPPTLimitApuOnly, PcdGet32 (PcdSlowPPTLimitApuOnly), 0, 0, 0, 0, 0);
  }

  if (PcdGet32 (PcdMsgSetSustainedPowerLimit) != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetSustainedPowerLimit, PcdGet32 (PcdMsgSetSustainedPowerLimit), 0, 0, 0, 0, 0);
  }

  if (PcdGet32 (PcdMsgStapmTimeConstant) != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetStapmTimeConstant, PcdGet32 (PcdMsgStapmTimeConstant), 0, 0, 0, 0, 0);
  }

  //
  // OC feature
  //
  // PPT Limit
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocPPTLimit);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetFastPPTLimit, SmuArg[0], 0, 0, 0, 0, 0);
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetSlowPPTLimit, SmuArg[0], 0, 0, 0, 0, 0);
  }

  // TDC Limit
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocTDCLimit);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetVrmVddCurrentLimit, SmuArg[0], 0, 0, 0, 0, 0);
  }
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocSocTDCLimit);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetVrmSocCurrentLimit, SmuArg[0], 0, 0, 0, 0, 0);
  }
  // EDC Limit
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocEDCLimit);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetVrmVddMaximumCurrentLimit, SmuArg[0], 0, 0, 0, 0, 0);
  }
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocSocEDCLimit);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetVrmSocMaximumCurrentLimit, SmuArg[0], 0, 0, 0, 0, 0);
  }
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocPBOLimitScalar);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetFitLimitScalar, SmuArg[0], 0, 0, 0, 0, 0);
  }
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdMocTjMax);
  if (SmuArg[0] != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetTjMax, SmuArg[0], 0, 0, 0, 0, 0);
  }

  //  set fPPT/sPPT
  if (PcdGet32 (PcdMsgSetFastPPTLimit) != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetFastPPTLimit, PcdGet32 (PcdMsgSetFastPPTLimit), 0, 0, 0, 0, 0);
  }
  if (PcdGet32 (PcdMsgSetSlowPPTLimit) != 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetSlowPPTLimit, PcdGet32 (PcdMsgSetSlowPPTLimit), 0, 0, 0, 0, 0);
  }

  //  set CoreDldoPsmMargin
  if (PcdGet64 (PcdCfgCoreDldoPsmArray) != 0x0) {
    PsmOffsetArray = (CORE_PSM_OFFSET *)PcdGet64 (PcdCfgCoreDldoPsmArray);
    while (PsmOffsetArray->CoreIndicator != 0xFFFFFFFE) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "CoreIndicator = 0x%x, PsmOffset = 0x%x\n", PsmOffsetArray->CoreIndicator, PsmOffsetArray->PsmOffset);
      if (PsmOffsetArray->CoreIndicator != 0xFFFFFFFF) {
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetCorePsmMargin, (UINT32)((PsmOffsetArray->CoreIndicator & 0xFFF00000) | PsmOffsetArray->PsmOffset), 0, 0, 0, 0, 0);
      } else {
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetAllCorePsmMargin, (UINT32) PsmOffsetArray->PsmOffset, 0, 0, 0, 0, 0);
      }
      PsmOffsetArray++;
    }
  }

  // Per request always send this message default is 10ms also be able change from CBS
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = PcdGet32 (PcdSetS0i3PmeTurnOffDelay);
  SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetS0i3PmeTurnOffDelay, SmuArg[0], 0, 0, 0, 0, 0);

  if (PcdGetBool (PcdSmuGfxPsmMarginControl)) {
    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
    SmuArg[0] = PcdGet16(PcdSmuGfxPsmMarginValue);
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_ForceGfxVidMargin, SmuArg[0], 0, 0, 0, 0, 0);
  }

//
//  if (PcdGetBool (PcdOcDisable)) {
//    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//    SmuArg[0] = 1;
//    Status = SmuServiceRequest (GnbHandle, BIOSSMC_MSG_OC_Disable, SmuArg, 0, 0, 0, 0, 0);
//  } else {
//    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//    SmuArg[0] = (UINT32) PcdGet16 (PcdOcVoltageMax);
//    if (SmuArg[0] != 0) {
//      Status = SmuServiceRequest (GnbHandle, BIOSSMC_MSG_OC_VoltageMax, SmuArg, 0, 0, 0, 0, 0);
//    }
//    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
//    SmuArg[0] = (UINT32) PcdGet16 (PcdOcFrequencyMax);
//    if (SmuArg[0] != 0) {
//      Status = SmuServiceRequest (GnbHandle, BIOSSMC_MSG_OC_FrequencyMax, SmuArg, 0, 0, 0, 0, 0);
//    }
//  }

  //
  //  Step25. If AC BTC is enabled, runs the AC BTC flow.
  // ALWAYS call the AC BTC callback to keep the flow consistent.
  //

  IDS_HDT_CONSOLE (MAIN_FLOW, "Set up callback for AC BTC \n");
  EfiCreateProtocolNotifyEvent (&gEfiMpServiceProtocolGuid, TPL_CALLBACK, SmuFinalInit, NULL, &Registration);

  if (PcdGet8 (PcdXgbeDisable) == 0) {
    SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetGbeStatus, 1, 0, 0, 0, 0, 0);
  }

  // Send PcdSyncFloodToApml state
  IDS_HDT_CONSOLE (MAIN_FLOW, "Send PcdSyncFloodToApml = %x Status to SMU\n", (PcdGetBool (PcdSyncFloodToApml))? 1: 0);
  SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetSyncFloodToApml, ((PcdGetBool (PcdSyncFloodToApml))? 1: 0), 0, 0, 0, 0, 0);

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);
}




