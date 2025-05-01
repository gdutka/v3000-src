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
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Guid/SocCoreInfo.h>
#include <NbioSmu.h>
#include "AodSocLib.h"

AOD_MOUDLE_NAME ("AodSocLibPei")
AOD_SOC_NAME ("RPL")

/**
  This function set Ccd Freq AGESA Pcd

  @param[in]  pAodCoreInfo               Points to the core info

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
EFIAPI
AodCcdFreqOptimizerRpl  (
  IN   AOD_CORE_INFO *pAodCoreInfo
  )
{
  AOD_CONFIG                               *pAodConfig;
  UINT32                                    *Buffer;
  UINTN                                     CcdIndex;
  UINTN                                     VarSize;
  EFI_STATUS                                Status;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Started\n"));

  //
  // Alloacte CORE_PSM_OFFSET pointer for logical Core Count + end
  //
  VarSize = sizeof(UINT32) * (pAodCoreInfo->AodPhysicalCcdCount + 1);
  Buffer = (UINT32 *) AllocateRuntimePool (VarSize);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (Buffer, VarSize);
  Status = AodGetConfiguration (&pAodConfig);
  if (EFI_ERROR (Status)) return Status;
  //Check Display Condition:  AodCcdOptimizerFreqCtrl=CCD Frequency Adjust
  if (pAodConfig->AodCcdOptimizerFreqCtrl == 1) {
    PcdSet64S (PcdCfgCcdFrequencyArray, (UINTN)Buffer);
    for (CcdIndex = 0; CcdIndex < pAodCoreInfo->AodPhysicalCcdCount; CcdIndex ++) {
      if (pAodConfig->AodCcdOptimizerFreq[CcdIndex] != 0) {
        AOD_DEBUG_SOC ((EFI_D_INFO, "CCD  = %d Freq = %d\n", pAodConfig->AodCcdOptimizerFreq[CcdIndex]));
        *Buffer = ((CcdIndex << 16) | (pAodConfig->AodCcdOptimizerFreq[CcdIndex] & 0xffff));
        AOD_DEBUG_SOC ((EFI_D_INFO, "CCD  = %d Freq = %d, Data32=%08x\n",CcdIndex, \
          pAodConfig->AodCcdOptimizerFreq[CcdIndex], *Buffer));
        Buffer ++;
      }
    }
    *Buffer = 0xffffffff;
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "Ended\n"));
  return EFI_SUCCESS;
}
/**
  This function set CoreCurveOptimizer AGESA Pcd and build logical cores

  @param[in]  pAodCoreInfo               Points to the core info

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
EFIAPI
AodCoreCurveOptimizerRpl  (
  IN   AOD_CORE_INFO *pAodCoreInfo
  )
{
  AOD_CONFIG                               *pAodConfig;
  CORE_PSM_OFFSET                          *CorePsmOffset;
  UINTN                                     CcdIndex,CoreIndex;
  UINTN                                     Index = 0;
  UINTN                                     VarSize;
  EFI_STATUS                                Status;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Started\n"));

  //
  // Alloacte CORE_PSM_OFFSET pointer for logical Core Count + end
  //
  VarSize = sizeof(CORE_PSM_OFFSET) * (pAodCoreInfo->AodPhysicalCcdCount*pAodCoreInfo->AodPhysicalCoreCountPerCcd + 1);
  CorePsmOffset = (CORE_PSM_OFFSET *) AllocateRuntimePool (VarSize);
  if (CorePsmOffset == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (CorePsmOffset, VarSize);
  Status = AodGetConfiguration (&pAodConfig);
  if (EFI_ERROR (Status)) return Status;
  if (pAodConfig->AodCurveOptimizer == 1) { //All Core
    if (pAodConfig->AodAllCoreCurveOptimizerSign) { //Postive
      CorePsmOffset[0].PsmOffset = (UINT16)(0 - pAodConfig->AodAllCoreCurveOptimizerMagnitude);
    } else { //Nagative
      CorePsmOffset[0].PsmOffset = pAodConfig->AodAllCoreCurveOptimizerMagnitude;
    }
    CorePsmOffset[0].CoreIndicator = AOD_ALL_CORE_CURVE_INDICATER(0,0);
    Index = 1;
  } else if (pAodConfig->AodCurveOptimizer == 2) { //Per Core
    Index = 0;
    for (CcdIndex = 0; CcdIndex < pAodCoreInfo->AodPhysicalCcdCount; CcdIndex ++) {
      if ((pAodCoreInfo->AodCcdBitMap & (1 << CcdIndex)) == 0) continue;
      for (CoreIndex = 0; CoreIndex < pAodCoreInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
        if (pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_FUSED_OFF) continue;
        if (pAodConfig->AodPerCoreCurveOptimizerSign[Index] == 0) { //Postive
          CorePsmOffset[Index].PsmOffset = pAodConfig->AodPerCoreCurveOptimizerMagnitude[Index];
        } else {//Nagative
          CorePsmOffset[Index].PsmOffset = (UINT16)(0 - pAodConfig->AodPerCoreCurveOptimizerMagnitude[Index]);
        }
        CorePsmOffset[Index].CoreIndicator = AOD_PER_CORE_CURVE_INDICATER(CcdIndex, CoreIndex);
        Index ++;
      }
    }
  } else {
    return EFI_UNSUPPORTED;
  }
  CorePsmOffset[Index].CoreIndicator = AOD_TERMINATE_CURVE_INDICATER(0, 0);
  PcdSet64S (PcdCfgCoreDldoPsmArray, (UINT64)(UINTN)CorePsmOffset); //gCorePsmOffset
  AOD_DEBUG_SOC ((EFI_D_INFO, "Ended\n"));
  return EFI_SUCCESS;
}
/**
  This function used to update Aod Core info HOB

  @param[in]  Context               Pointer to the AOD_CORE_INFO

  @retval EFI_SUCCESS            The Results is processed successfully.
**/
EFI_STATUS
EFIAPI
AodUpdateCoreInfoRpl (
  IN  VOID         *Context
  )
{
  VOID                             *GuidHob;
  SOC_CORE_INFO                    *pSocInfo;
  AOD_CORE_INFO                    *pAodCoreInfo;
  UINT8                            FusedOffCcdCount = 0;
  UINT8                            DisableCoreCount = 0;
  UINT8                            FusedOffCoreCount = 0;
  UINT8                            LogicalCoreCount = 0;
  UINT8                            LogicalCoreCountPerCcd = 0;
  UINT8                            ActivceCoreCountPerCcd = 0;
  UINT8                            CcdIndex,CoreIndex;
  CHAR8                            String[100];
  AOD_DEBUG_SOC ((EFI_D_INFO, "  start \n"));
  if (Context == NULL) return EFI_INVALID_PARAMETER;
  pAodCoreInfo = (AOD_CORE_INFO *)Context;
  //
  // Get AGESA Core Info Hob
  //
  GuidHob = GetFirstGuidHob (&gSocCoreInfoGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  pSocInfo = (SOC_CORE_INFO *)GET_GUID_HOB_DATA (GuidHob);
  //
  // Sync Soc Core Info Hob into Aod Core Info Hob
  //
  pAodCoreInfo->AodPhysicalCcdCount = (UINT8) \
    ((pSocInfo->CcdMaxCount > AOD_MAX_CCD_COUNT)?AOD_MAX_CCD_COUNT:pSocInfo->CcdMaxCount);
  pAodCoreInfo->AodPhysicalCoreCountPerCcd = \
    (UINT8)((pSocInfo->CoreMaxCountPerCcd > AOD_MAX_CORE_COUNT)?AOD_MAX_CORE_COUNT:pSocInfo->CoreMaxCountPerCcd);
  pAodCoreInfo->AodCcdBitMap = pSocInfo->CcdEnableMap.Value;
  FusedOffCoreCount = 0;

  //
  // Calcuate the fused core count and disable core count
  //
  for (CcdIndex = 0; CcdIndex < pAodCoreInfo->AodPhysicalCcdCount; CcdIndex ++) {
    if ((pSocInfo->CcdEnableMap.Value & (1 << CcdIndex)) == 0) {
      FusedOffCcdCount ++;
      continue;
    }
    DisableCoreCount = 0;
    FusedOffCoreCount = 0;
    for (CoreIndex = 0; CoreIndex < pAodCoreInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
      if (pSocInfo->CoreStatus[CcdIndex][CoreIndex] == Core_Fused_Off) {
        pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] = AOD_CORE_FUSED_OFF;
        FusedOffCoreCount ++;
      }
      if (pSocInfo->CoreStatus[CcdIndex][CoreIndex] == Core_Soft_Off) {
        pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] = AOD_CORE_SOFT_OFF;
        DisableCoreCount ++;
      }
      if (pSocInfo->CoreStatus[CcdIndex][CoreIndex] == Core_Active) {
        pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] = AOD_CORE_ACTIVE;
      }
    }
    if (FusedOffCoreCount < pAodCoreInfo->AodPhysicalCoreCountPerCcd) {
      LogicalCoreCountPerCcd = pAodCoreInfo->AodPhysicalCoreCountPerCcd - FusedOffCoreCount;
      ActivceCoreCountPerCcd = LogicalCoreCountPerCcd - DisableCoreCount;
    }
  }
  pAodCoreInfo->AodLogicalCcdCount = pAodCoreInfo->AodPhysicalCcdCount - FusedOffCcdCount;
  pAodCoreInfo->AodLogicalCoreCountPerCcd = LogicalCoreCountPerCcd;
  pAodCoreInfo->AodActivceCoreCountPerCcd = ActivceCoreCountPerCcd;

  AOD_DEBUG_SOC ((EFI_D_INFO, "  Dump CORE Bit Map information, 1-Active, 0-Soft Off, x-fused off\n"));
  for (CcdIndex = 0; CcdIndex < pAodCoreInfo->AodPhysicalCcdCount; CcdIndex ++) {
    if ((pAodCoreInfo->AodCcdBitMap & (1 << CcdIndex)) == 0) {
      AOD_DEBUG_SOC ((EFI_D_INFO, "  CCD%d : fused off \n"));
      continue;
    }
    ZeroMem(String, 100);
    AsciiSPrint (String, 100, "CCD%d :", CcdIndex);
    for (CoreIndex = 0; CoreIndex < pAodCoreInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
      if (pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_FUSED_OFF) {
        AsciiStrCatS (String, 100, " x ");
        continue;
      }
      LogicalCoreCount ++;
      if (pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_ACTIVE) {
        AsciiStrCatS (String, 100, " 1 ");
      }
      if (pAodCoreInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_SOFT_OFF) {
        AsciiStrCatS (String, 100, " 0 ");
      }
    }
    AOD_DEBUG_SOC ((EFI_D_INFO, "  %a\n", String));
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "  end \n"));
  pAodCoreInfo->AodLogicalCoreCount = LogicalCoreCount;
  AodCoreCurveOptimizerRpl (pAodCoreInfo);
  AodCcdFreqOptimizerRpl (pAodCoreInfo);
  return EFI_SUCCESS;
}
/**
  This function used to sync L"AodSetup" Variable into Agesa Pcd

  @param[in]  AodVariable               Pointer to the L"AodSetup" Variable

  @retval EFI_SUCCESS            The Results is processed successfully.
**/
EFI_STATUS
EFIAPI
AodSyncVariableToPcdsRpl (
  IN       VOID   *AodVariable
  )
{
  AOD_CONFIG                               *AodConfig;
  UINT32                                   AodFMaxOverride;
  UINT16                                   AodGfxCurveOptimizer;
  AOD_DEBUG_SOC ((EFI_D_INFO, "started \n"));
  if (AodVariable == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  AodConfig = (AOD_CONFIG *) AodVariable;

  if ((AodConfig->AodCpuPst0Freq != 0x0)) {
    PcdSet32S (PcdAmdCcxP0Freq, AodConfig->AodCpuPst0Freq);
    PcdSet8S (PcdAmdCcxP0Setting, 1); // Custom
  }

  //Check Display Condition AodCpuPstCustonP0=Custom
  if ((AodConfig->AodCpuPst0Voltage != 0x0)) {
    PcdSet8S (PcdAmdCcxP0Vid, (UINT8)AodConfig->AodCpuPst0Voltage);
    PcdSet8S (PcdAmdCcxP0Setting, 1); // Custom
  }

  // PBO Enabled Mode, Or PBO Advanced Mode with MOTHERBOARD
  if ((AodConfig->AodNbioPBOCtl == 1) || \
      ((AodConfig->AodNbioPBOCtl == 2) && (AodConfig->AodNbioPBOAdvancedCtrl == 1))) {
    PcdSet32S (PcdMocPPTLimit, PcdGet32 (PcdFastPptLimit));
    PcdSet32S (PcdMocTDCLimit, PcdGet32 (PcdVrmCurrentLimit));
    PcdSet32S (PcdMocSocTDCLimit, PcdGet32 (PcdVrmSocCurrentLimit));
    PcdSet32S (PcdMocEDCLimit, PcdGet32 (PcdVrmMaximumCurrentLimit));
    PcdSet32S (PcdMocSocEDCLimit, PcdGet32 (PcdVrmSocMaximumCurrentLimit));
    PcdSet32S (PcdMsgSetSustainedPowerLimit, PcdGet32 (PcdMocPPTLimit));
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOAdvancedCtrl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOAdvancedCtrl == 0x2)) {
    PcdSet32S (PcdMocPPTLimit, AodConfig->AodNbioSocketPPTLimit);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOAdvancedCtrl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOAdvancedCtrl == 0x2)) {
    PcdSet32S (PcdMocTDCLimit, AodConfig->AodNbioTDCVDDLimit);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOAdvancedCtrl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOAdvancedCtrl == 0x2)) {
    PcdSet32S (PcdMocEDCLimit, AodConfig->AodNbioEDCVDDLimit);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOAdvancedCtrl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOAdvancedCtrl == 0x2)) {
    PcdSet32S (PcdMocSocTDCLimit, AodConfig->AodNbioTDCSOCLimit);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOAdvancedCtrl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOAdvancedCtrl == 0x2)) {
    PcdSet32S (PcdMocSocEDCLimit, AodConfig->AodNbioEDCSOCLimit);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbioPBOScalarCtl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbioPBOScalarCtl == 0x1)) {
    PcdSet32S (PcdMocPBOLimitScalar, AodConfig->AodNbioPBOScalar);
  }
  //Check Display Condition AodNbioPBOCtl=Advanced
  if ((AodConfig->AodNbioPBOCtl == 0x2)) {
    PcdSet8S (PcdCpuBoostClockOverride, AodConfig->AodCpuBoostClock?1:0);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodCpuBoostClock=Enabled
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodCpuBoostClock == 1)) {
    PcdSet32S (PcdFMaxFrequency, AodConfig->AodFMaxFreqIncrease);
  }
  //Check Display Condition AodNbioPBOCtl=Advanced,AodCpuBoostClock=Enabled
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodCpuBoostClock == 2)) {
    AodFMaxOverride = (UINT32)(0-AodConfig->AodFMaxFreqDecrease);
    PcdSet32S (PcdFMaxFrequency, AodFMaxOverride);
  }

  //Check Display Condition AodNbioPBOCtl=Advanced,AodNbiocHTCCtl=Manual
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodNbiocHTCCtl == 0x1)) {
    PcdSet32S (PcdMocTjMax, AodConfig->AodNbiocHTC);
  }

  //Check Display Condition: AodLclkCtrl=Manual
  if (AodConfig->AodLclkCtrl == 0x1) {
    PcdSet32S (PcdCfgLclkFrequencyRange, (UINT32)((AodConfig->AodMinLclkFreq << 16) | AodConfig->AodMaxLclkFreq));
  }

  //Check Display Condition: AodVddMiscCtrl=Manual
  if (AodConfig->AodVddMiscCtrl == 0x1) {
    PcdSet16S (PcdVddMiscVoltage, AodConfig->AodVddMiscVoltage);
  }
  //Check Display Condition: AodNbioPBOCtl=Advanced
  if ((AodConfig->AodNbioPBOCtl == 0x2)) {
    PcdSetBoolS (PcdSmuGfxPsmMarginControl, AodConfig->AodGfxCurveOptimizer);
  }
  // Display Condition: AodNbioPBOCtl=Advanced,AodGfxCurveOptimizer=GFX Curve Optimizer
  if ((AodConfig->AodNbioPBOCtl == 0x2) && (AodConfig->AodGfxCurveOptimizer == 0x1)) {
    AodGfxCurveOptimizer = AodConfig->AodGfxCurveOptimizerSign?\
                            (0-AodConfig->AodGfxCurveOptimizerMagnitude):\
                            AodConfig->AodGfxCurveOptimizerMagnitude;
    PcdSet16S (PcdSmuGfxPsmMarginValue, AodGfxCurveOptimizer);
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End \n"));
  return EFI_SUCCESS;
}
///
/// Aod Rpl Pei init table
///
AOD_INIT_TABLE mRplAodPeiInit [] = {
  { 0, AOD_SYNC_VARIABLE_PCD, AodSyncVariableToPcdsRpl},
  { 0, AOD_UPDATE_CORE_INFO,  AodUpdateCoreInfoRpl},
  { 0, 0, NULL}
};
/**
  The constructor function for RPL SOC Aod Init .

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval  EFI_SUCCESS            Table initialization successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to initialize table.
**/
EFI_STATUS
EFIAPI
PeiAodRplConstructor (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{

  SOC_ID_STRUCT       SocId;

  // Check Hardware Identification RPL AM5
  SocId.SocFamilyID = F19_RPL_RAW_ID;
  SocId.PackageType = ZEN4_PKG_AM5;
  if (SocHardwareIdentificationCheck (&SocId)) {
    return AodAddInitTable (mRplAodPeiInit);
  }

  return EFI_SUCCESS;
}
