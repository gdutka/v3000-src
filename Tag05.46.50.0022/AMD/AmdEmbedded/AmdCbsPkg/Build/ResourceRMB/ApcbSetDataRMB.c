/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//This file is auto generated, don't edit it manually


#include <Uefi.h>
#include <Base.h>
#include <Library/DebugLib.h>
#include <AmdCbsVariable.h>
#include <Porting.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Addendum/Apcb/Inc/RMB/APCB.h>

EFI_STATUS
UpdateCbsApcbTokens (
  VOID *CbsVariable,
  AMD_APCB_SERVICE_PROTOCOL *ApcbProtocol
  )
{
  EFI_STATUS                            Status;
  CBS_CONFIG                            *Setup_Config;
  FP_SET_TOKEN_BOOL                     ApcbSetTokenBool;                 ///< Set an APCB BOOL token
  FP_SET_TOKEN_8                        ApcbSetToken8;                    ///< Set an APCB UINT8 token
  FP_SET_TOKEN_16                       ApcbSetToken16;                   ///< Set an APCB UINT16 token
  FP_SET_TOKEN_32                       ApcbSetToken32;                   ///< Set an APCB UINT32 token

  if (CbsVariable == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ApcbProtocol == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = ApcbProtocol->ApcbAcquireMutex (ApcbProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ApcbSetTokenBool = ApcbProtocol->ApcbSetTokenBool;                 ///< Set an APCB BOOL token
  ApcbSetToken8 = ApcbProtocol->ApcbSetToken8;                    ///< Set an APCB UINT8 token
  ApcbSetToken16 = ApcbProtocol->ApcbSetToken16;                   ///< Set an APCB UINT16 token
  ApcbSetToken32 = ApcbProtocol->ApcbSetToken32;                   ///< Set an APCB UINT32 token
  //Clear all token setting
  ApcbProtocol->ApcbPurgeAllTokens (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG);

  Setup_Config = (CBS_CONFIG *) CbsVariable;

  //APCB_TOKEN_UID_CBS_SYNC_SIGNATURE - token for CBS and APCB in sync
  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_SYNC_SIGNATURE, Setup_Config->Header.ApcbVariableHash);

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuCpb != 1) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_CPB, Setup_Config->CbsCmnCpuCpb);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuSevAsidCount != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_SEV_ASID_COUNT, Setup_Config->CbsCmnCpuSevAsidCount);
  }

  //Check Display Condition CbsCmnCpuSevAsidSpaceCtrl=Manual
  if ((Setup_Config->CbsCmnCpuSevAsidSpaceCtrl == 0x1)) {
    ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_MIN_SEV_ASID, Setup_Config->CbsCmnCpuSevAsidSpaceLimit);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuSmuPspDebugMode != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_PSP_ENABLE_DEBUG_MODE, Setup_Config->CbsCmnCpuSmuPspDebugMode);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuPpinCtrl != 0xFF) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_PPIN_OPT_IN, Setup_Config->CbsCmnCpuPpinCtrl);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnActionOnBistFailure != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_ACTION_ON_BIST_FAILURE, Setup_Config->CbsCmnActionOnBistFailure);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuCoreCtrlRmb != 0) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_CORE_CTRL_DUMMY, Setup_Config->CbsCmnCpuCoreCtrlRmb);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnCpuSmtCtrl != 0x1) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CCX_SMT_CTRL, Setup_Config->CbsCmnCpuSmtCtrl);
  }

  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RESERVE_ADDR_LOW, Setup_Config->CbsCmnPersistentLow);  //User Input

  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RESERVE_ADDR_HIGH, Setup_Config->CbsCmnPersistentHigh);  //User Input

  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RESERVE_SIZE_LOW, Setup_Config->CbsCmnPersistentSizeLow);  //User Input

  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RESERVE_SIZE_HIGH, Setup_Config->CbsCmnPersistentSizeHigh);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsDfCmnCc6MemEncryption != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT, Setup_Config->CbsDfCmnCc6MemEncryption);
  }

  //Check if select Auto
  if (Setup_Config->CbsDfCmnMemClear != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_MEM_CLEAR, Setup_Config->CbsDfCmnMemClear);
  }

  //Check if select Auto
  if (Setup_Config->CbsDfCmnExtIpSyncFloodProp != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_EXT_IP_SYNC_FLOOD_PROP, Setup_Config->CbsDfCmnExtIpSyncFloodProp);
  }

  //Check if select Auto
  if (Setup_Config->CbsDfCmnSyncFloodProp != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP, Setup_Config->CbsDfCmnSyncFloodProp);
  }

  //Check Display Condition CbsComboFlag=0
  if ((Setup_Config->CbsComboFlag == 0x0)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_DF_CMN_MPx_EXCEPTION_RECOVERY_MODE, Setup_Config->CbsMpExcepRecovHandle);  //User Input
  }

  //Check Display Condition CbsComboFlag=0
  if ((Setup_Config->CbsComboFlag == 0x0)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_DF_CMN_MEM_CLEAR_TIMEOUT, Setup_Config->CbsMemClrTimeout);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsDfCmnMemIntlvSize != 7) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE, Setup_Config->CbsDfCmnMemIntlvSize);
  }

  //Check if select Auto
  if (Setup_Config->CbsDfCmnDramMapInversion != 3) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_INVERT_DRAM_MAP, Setup_Config->CbsDfCmnDramMapInversion);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemTimingSettingDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_ACTIVE_MEM_TIMING_SETTING_DDR, Setup_Config->CbsCmnMemTimingSettingDdr);
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TARGET_SPEED_DDR, Setup_Config->CbsCmnMemTargetSpeedDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTclCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTclCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TCL_DDR, Setup_Config->CbsCmnMemTimingTclDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrcdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrcdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRCD_DDR, Setup_Config->CbsCmnMemTimingTrcdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrpCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrpCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRP_DDR, Setup_Config->CbsCmnMemTimingTrpDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrasCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrasCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRAS_DDR, Setup_Config->CbsCmnMemTimingTrasDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrcCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrcCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRC_DDR, Setup_Config->CbsCmnMemTimingTrcDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWR_DDR, Setup_Config->CbsCmnMemTimingTwrDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrfc1CtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrfc1CtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRFC1_DDR, Setup_Config->CbsCmnMemTimingTrfc1Ddr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrfc2CtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrfc2CtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRFC2_DDR, Setup_Config->CbsCmnMemTimingTrfc2Ddr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrfcSbCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrfcSbCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRFCSB_DDR, Setup_Config->CbsCmnMemTimingTrfcSbDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrtpCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrtpCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRTP_DDR, Setup_Config->CbsCmnMemTimingTrtpDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrrdLCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrrdLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRRDL_DDR, Setup_Config->CbsCmnMemTimingTrrdLDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrrdSCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrrdSCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRRDS_DDR, Setup_Config->CbsCmnMemTimingTrrdSDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTfawCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTfawCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TFAW_DDR, Setup_Config->CbsCmnMemTimingTfawDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwtrLCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwtrLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWTRL_DDR, Setup_Config->CbsCmnMemTimingTwtrLDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwtrSCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwtrSCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWTRS_DDR, Setup_Config->CbsCmnMemTimingTwtrSDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrdrdScLCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdrdScLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRDRDSCL_DDR, Setup_Config->CbsCmnMemTimingTrdrdScLDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrdrdScCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdrdScCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRDRDSC_DDR, Setup_Config->CbsCmnMemTimingTrdrdScDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrdrdSdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdrdSdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRDRDSD_DDR, Setup_Config->CbsCmnMemTimingTrdrdSdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrdrdDdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdrdDdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRDRDDD_DDR, Setup_Config->CbsCmnMemTimingTrdrdDdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrwrScLCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrwrScLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWRWRSCL_DDR, Setup_Config->CbsCmnMemTimingTwrwrScLDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrwrScCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrwrScCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWRWRSC_DDR, Setup_Config->CbsCmnMemTimingTwrwrScDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrwrSdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrwrSdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWRWRSD_DDR, Setup_Config->CbsCmnMemTimingTwrwrSdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrwrDdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrwrDdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWRWRDD_DDR, Setup_Config->CbsCmnMemTimingTwrwrDdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTwrrdCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrrdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TWRRD_DDR, Setup_Config->CbsCmnMemTimingTwrrdDdr);  //User Input
  }

  //Check Display Condition CbsCmnMemTimingSettingDdr=Enabled,CbsCmnMemTimingTrdwrCtrlDdr=Manual
  if ((Setup_Config->CbsCmnMemTimingSettingDdr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdwrCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TIMING_TRDWR_DDR, Setup_Config->CbsCmnMemTimingTrdwrDdr);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDrvStrenCaDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CA_DRV_DDR, Setup_Config->CbsCmnMemDrvStrenCaDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDrvStrenCsDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CS_DRV_DDR, Setup_Config->CbsCmnMemDrvStrenCsDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDrvStrenCkDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CLK_DRV_DDR, Setup_Config->CbsCmnMemDrvStrenCkDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDrvStrenDqDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DQ_DRV_DDR, Setup_Config->CbsCmnMemDrvStrenDqDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemOdtImpedProcDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PROC_ODT_DDR, Setup_Config->CbsCmnMemOdtImpedProcDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramDrvStrenDqDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRV_IMP_DDR, Setup_Config->CbsCmnMemDramDrvStrenDqDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramOdtImpedRttNomWrDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RTT_NOM_WR_DDR, Setup_Config->CbsCmnMemDramOdtImpedRttNomWrDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramOdtImpedRttNomRdDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RTT_NOM_RD_DDR, Setup_Config->CbsCmnMemDramOdtImpedRttNomRdDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramOdtImpedRttWrDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RTT_WR_DDR, Setup_Config->CbsCmnMemDramOdtImpedRttWrDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramOdtImpedRttParkDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RTT_PARK_DDR, Setup_Config->CbsCmnMemDramOdtImpedRttParkDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramOdtImpedRttParkDqsDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RTT_PARK_DQS_DDR, Setup_Config->CbsCmnMemDramOdtImpedRttParkDqsDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPowerDownEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_POWER_DOWN_EN_DDR, Setup_Config->CbsCmnMemCtrllerPowerDownEnDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR, Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemEccDisErrInjectionDdr != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_ECC_DIS_ERR_INJECTION_DDR, Setup_Config->CbsCmnMemEccDisErrInjectionDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDataPoisoningDdr4 != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DATA_POISONING_DDR, Setup_Config->CbsCmnMemDataPoisoningDdr4);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemEccEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_ECC_EN_DDR, Setup_Config->CbsCmnMemEccEnDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramRedirectScrubEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_DDR, Setup_Config->CbsCmnMemDramRedirectScrubEnDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramRedirectScrubLimitDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_REDIRECT_SCRUB_LIMIT_DDR, Setup_Config->CbsCmnMemDramRedirectScrubLimitDdr);
  }

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PATROL_SCRUB_DDR, Setup_Config->CbsCmnMemDramScrubTime);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsCmnMemTsmeEnableDdr != 0xFF) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TSME_ENABLE_DDR, Setup_Config->CbsCmnMemTsmeEnableDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDataCtrlDataScrambleEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR, Setup_Config->CbsCmnMemDataCtrlDataScrambleEnDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemSpdCrcVerify != 0xf) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_SPD_VERIFY_CRC, Setup_Config->CbsCmnMemSpdCrcVerify);
  }

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_SPD_READ_RETRY_COUNT_ON_CRC_FAILURE, Setup_Config->CbsCmnMemSpdReadRetryCountCrc);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMappingBankInterleaveDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CS_INTERLEAVE_DDR, Setup_Config->CbsCmnMemMappingBankInterleaveDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemAddressHashBankDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR, Setup_Config->CbsCmnMemAddressHashBankDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemAddressHashCsDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_CS_DDR, Setup_Config->CbsCmnMemAddressHashCsDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerBankSwapModeDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_BANK_SWAP_MODE_DDR, Setup_Config->CbsCmnMemCtrllerBankSwapModeDdr);
  }

  ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RESTORE_VALID_DAYS, Setup_Config->CbsCmnMemRestoreValidDays);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPmuTrainDfeDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_DFE_DDR, Setup_Config->CbsCmnMemCtrllerPmuTrainDfeDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramPdaEnumIdProgModeDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR, Setup_Config->CbsCmnMemDramPdaEnumIdProgModeDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_EN_DDR, Setup_Config->CbsCmnMemMbistEnDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistTestmodeDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_TESTMODE_DDR, Setup_Config->CbsCmnMemMbistTestmodeDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistAggressorsDdr != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_DDR, Setup_Config->CbsCmnMemMbistAggressorsDdr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistPerBitSlaveDieReportDdr != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR, Setup_Config->CbsCmnMemMbistPerBitSlaveDieReportDdr);
  }

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT_DDR, Setup_Config->CbsCmnMemMbistPatternSelectDdr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH_DDR, Setup_Config->CbsCmnMemMbistPatternLengthDdr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL_DDR, Setup_Config->CbsCmnMemMbistAggressorsChnlDdr);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsCmnMemOverclockLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_OVERCLOCK_DDR4, Setup_Config->CbsCmnMemOverclockLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemSpeedLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_SPEED_DDR4, Setup_Config->CbsCmnMemSpeedLpddr);
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrcpageCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTrcpageCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTrcpageCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTrcpageCtrlLpddr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCPAGE_DDR4, Setup_Config->CbsCmnMemTimingTrcpageLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTcwlLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCWL_DDR4, Setup_Config->CbsCmnMemTimingTcwlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTclLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCL_DDR4, Setup_Config->CbsCmnMemTimingTclLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrcdrdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDRD_DDR4, Setup_Config->CbsCmnMemTimingTrcdrdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrcdwrLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRCDWR_DDR4, Setup_Config->CbsCmnMemTimingTrcdwrLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrpLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRP_DDR4, Setup_Config->CbsCmnMemTimingTrpLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrfcCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTrfcCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTrfcCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTrfcCtrlLpddr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRFC_DDR4, Setup_Config->CbsCmnMemTimingTrfcLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrasLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRAS_DDR4, Setup_Config->CbsCmnMemTimingTrasLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrcCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTrcCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTrcCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTrcCtrlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRC_DDR4, Setup_Config->CbsCmnMemTimingTrcLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrrdSLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_S_DDR4, Setup_Config->CbsCmnMemTimingTrrdSLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrrdLLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRRD_L_DDR4, Setup_Config->CbsCmnMemTimingTrrdLLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTfawCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTfawCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTfawCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTfawCtrlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TFAW_DDR4, Setup_Config->CbsCmnMemTimingTfawLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwtrSLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_S_DDR4, Setup_Config->CbsCmnMemTimingTwtrSLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwtrLLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWTR_L_DDR4, Setup_Config->CbsCmnMemTimingTwtrLLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTwrCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTwrCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrCtrlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWR_DDR4, Setup_Config->CbsCmnMemTimingTwrLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrdrdScLCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTrdrdScLCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTrdrdScLCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTrdrdScLCtrlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_L_DDR4, Setup_Config->CbsCmnMemTimingTrdrdScLLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrwrScLCtrlLpddr != 0) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR4, Setup_Config->CbsCmnMemTimingTwrwrScLCtrlLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled,CbsCmnMemTimingTwrwrScLCtrlLpddr=Manual
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1) && (Setup_Config->CbsCmnMemTimingTwrwrScLCtrlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_L_DDR4, Setup_Config->CbsCmnMemTimingTwrwrScLLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrtpLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRTP_DDR4, Setup_Config->CbsCmnMemTimingTrtpLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTckeLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TCKE_DDR4, Setup_Config->CbsCmnMemTimingTckeLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrdrdScLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SC_DDR4, Setup_Config->CbsCmnMemTimingTrdrdScLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrdrdDdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_DD_DDR4, Setup_Config->CbsCmnMemTimingTrdrdDdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrdrdSdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDRD_SD_DDR4, Setup_Config->CbsCmnMemTimingTrdrdSdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrwrScLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SC_DDR4, Setup_Config->CbsCmnMemTimingTwrwrScLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrwrSdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_SD_DDR4, Setup_Config->CbsCmnMemTimingTwrwrSdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrwrDdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRWR_DD_DDR4, Setup_Config->CbsCmnMemTimingTwrwrDdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTwrrdLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TWRRD_DDR4, Setup_Config->CbsCmnMemTimingTwrrdLpddr);
    }
  }

  //Check Display Condition CbsCmnMemOverclockLpddr=Enabled
  if ((Setup_Config->CbsCmnMemOverclockLpddr == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnMemTimingTrdwrLpddr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_TIMING_TRDWR_DDR4, Setup_Config->CbsCmnMemTimingTrdwrLpddr);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemRefModeLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_AUTOREFMODE, Setup_Config->CbsCmnMemRefModeLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemRfmEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_RFM_ENABLE, Setup_Config->CbsCmnMemRfmEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemWckAlwaysOnEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_WCKALWAYSON_EN_DDR, Setup_Config->CbsCmnMemWckAlwaysOnEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemRrwMemtestCtrlLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_RRW_MEMTEST_CTRL_LPDDR, Setup_Config->CbsCmnMemRrwMemtestCtrlLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPwrDnEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_PWR_DN_EN_DDR4, Setup_Config->CbsCmnMemCtrllerPwrDnEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR, Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCadBusTimingCtlLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CAD_BUS_TIMING_CTL_DDR4, Setup_Config->CbsCmnMemCadBusTimingCtlLpddr);
  }

  //Check Display Condition CbsCmnMemCadBusTimingCtlLpddr=Manual
  if ((Setup_Config->CbsCmnMemCadBusTimingCtlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_ADDR_CMD_SETUP_DDR4, Setup_Config->CbsCmnMemAddrCmdSetupLpddr);  //User Input
  }

  //Check Display Condition CbsCmnMemCadBusTimingCtlLpddr=Manual
  if ((Setup_Config->CbsCmnMemCadBusTimingCtlLpddr == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CS_ODT_SETUP_DDR4, Setup_Config->CbsCmnMemCsOdtSetupLpddr);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenCaLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CA_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenCaLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenCsLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CS_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenCsLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenCkLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_CK_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenCkLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenWckLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_WCK_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenWckLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenDqLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQ_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenDqLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoDrvStrenDqsLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_DRV_STREN_DQS_LPDDR, Setup_Config->CbsCmnMemPhyIoDrvStrenDqsLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoOdtStrenProcLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_LPDDR, Setup_Config->CbsCmnMemPhyIoOdtStrenProcLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoOdtStrenProcCaLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR, Setup_Config->CbsCmnMemPhyIoOdtStrenProcCaLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoOdtStrenProcCkLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR, Setup_Config->CbsCmnMemPhyIoOdtStrenProcCkLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemPhyIoOdtStrenProcWckLpddr != 0xFF) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR, Setup_Config->CbsCmnMemPhyIoOdtStrenProcWckLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramIoOdtStrenCaLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_CA_LPDDR, Setup_Config->CbsCmnMemDramIoOdtStrenCaLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramIoOdtStrenDqLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR, Setup_Config->CbsCmnMemDramIoOdtStrenDqLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramIoOdtStrenWckLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR, Setup_Config->CbsCmnMemDramIoOdtStrenWckLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramIoOdtStrenNtdq != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_IO_ODT_STREN_NTDQ_LPDDR, Setup_Config->CbsCmnMemDramIoOdtStrenNtdq);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDramIoDrvStrenPdds != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_DRAM_IO_DRV_STREN_PDDS_LPDDR, Setup_Config->CbsCmnMemDramIoDrvStrenPdds);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerDramRdLinkEccEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_DDR, Setup_Config->CbsCmnMemCtrllerDramRdLinkEccEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerDramWrLinkEccEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_DDR, Setup_Config->CbsCmnMemCtrllerDramWrLinkEccEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemDisMemErrInjLpddr != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_DIS_MEM_ERR_INJ, Setup_Config->CbsCmnMemDisMemErrInjLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemTsmeLpddr != 0xFF) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TSME_ENABLE, Setup_Config->CbsCmnMemTsmeLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerDataScrambleLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_DATA_SCRAMBLE_DDR4, Setup_Config->CbsCmnMemCtrllerDataScrambleLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMappingBankInterleaveLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR4, Setup_Config->CbsCmnMemMappingBankInterleaveLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerBankSwapLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_BANK_SWAP_ENABLE, Setup_Config->CbsCmnMemCtrllerBankSwapLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerBankGroupSwapLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_DDR4, Setup_Config->CbsCmnMemCtrllerBankGroupSwapLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemAddressHashBankLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_BANK_DDR4, Setup_Config->CbsCmnMemAddressHashBankLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemAddressHashCsLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_ADDRESS_HASH_CS_DDR4, Setup_Config->CbsCmnMemAddressHashCsLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemCtrllerPmuTrainDfeLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_CTRLLER_PMU_TRAIN_DFE_DDR4, Setup_Config->CbsCmnMemCtrllerPmuTrainDfeLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistEnLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_EN, Setup_Config->CbsCmnMemMbistEnLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistTestmodeLpddr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_TESTMODE, Setup_Config->CbsCmnMemMbistTestmodeLpddr);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnMemMbistAggressorsLpddr != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_MEM_MBIST_AGGRESSORS, Setup_Config->CbsCmnMemMbistAggressorsLpddr);
  }

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_PATTERN_SELECT, Setup_Config->CbsCmnMemMbistPatternSelectLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_PATTERN_LENGTH, Setup_Config->CbsCmnMemMbistPatternLengthLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_AGGRESSORS_CHNL, Setup_Config->CbsCmnMemMbistAggressorsChnlLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP, Setup_Config->CbsCmnMemMbistReadDataEyeVoltageStepLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_READ_DATA_EYE_TIMING_STEP, Setup_Config->CbsCmnMemMbistReadDataEyeTimingStepLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP, Setup_Config->CbsCmnMemMbistWriteDataEyeVoltageStepLpddr);  //User Input

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP, Setup_Config->CbsCmnMemMbistWriteDataEyeTimingStepLpddr);  //User Input

  //Check if select Auto
  if (Setup_Config->CbsCmnGnbGfxUmaMode != 0xf) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_UMAMODE, Setup_Config->CbsCmnGnbGfxUmaMode);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnGnbGfxUmaVersion != 0xf) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_UMAVERSION, Setup_Config->CbsCmnGnbGfxUmaVersion);
  }

  //Check Display Condition CbsCmnGnbGfxUmaMode=UMA_SPECIFIED
  if ((Setup_Config->CbsCmnGnbGfxUmaMode == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnGnbGfxUmaFrameBufferSize != 0xffffffff) {
      ApcbSetToken32 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_UMASIZE, Setup_Config->CbsCmnGnbGfxUmaFrameBufferSize);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnGpuHostTranslation != 0xF) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_GPU_TRANSLATION_CACHE, Setup_Config->CbsCmnGpuHostTranslation);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnFchEspiCrc != 0xFF) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_ESPI_CRC_ENABLE, Setup_Config->CbsCmnFchEspiCrc);
  }

  //Check Display Condition CbsDbgVddpVoltageCtl=Manual
  if ((Setup_Config->CbsDbgVddpVoltageCtl == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDP_VOLTAGE, Setup_Config->CbsDbgVddpVoltage);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnDfPstateP0Ctl != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_PSTATE_P0_CTL, Setup_Config->CbsCmnDfPstateP0Ctl);
  }

  //Check Display Condition CbsCmnDfPstateP0Ctl=Manual
  if ((Setup_Config->CbsCmnDfPstateP0Ctl == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_PSTATE_FCLK_P0, Setup_Config->CbsCmnDfPstateFclkP0);  //User Input
  }

  //Check Display Condition CbsCmnDfPstateP0Ctl=Manual
  if ((Setup_Config->CbsCmnDfPstateP0Ctl == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_DF_PSTATE_MEMCLK_P0, Setup_Config->CbsCmnDfPstateMemClkP0);  //User Input
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnPspRpmcSwitch != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_PSP_RPMC_ENABLE, Setup_Config->CbsCmnPspRpmcSwitch);
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnSocAblConOut != 2) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_FCH_CONSOLE_OUT_ENABLE, Setup_Config->CbsCmnSocAblConOut);
  }

  //Check Display Condition CbsCmnSocAblConOut=Enable
  if ((Setup_Config->CbsCmnSocAblConOut == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSocAblConOutSerialPort != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT, Setup_Config->CbsCmnSocAblConOutSerialPort);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnSocAblConOutSerialPortIO != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_FCH_CONSOLE_OUT_SERIAL_PORT_IO, Setup_Config->CbsCmnSocAblConOutSerialPortIO);
  }

  //Check Display Condition CbsCmnSocAblConOut=Enable
  if ((Setup_Config->CbsCmnSocAblConOut == 0x1)) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_CONSOLE_OUT_BOOT_MODE, Setup_Config->CbsCmnSocAblConOutBootMode);  //User Input
  }

  //Check Display Condition CbsCmnSocAblConOut=Enable
  if ((Setup_Config->CbsCmnSocAblConOut == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSocAblPmuMsgCtrl != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_MEM_TRAINING_HDTCTRL, Setup_Config->CbsCmnSocAblPmuMsgCtrl);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnSoCMiscHspUart != 0xf) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_HSP_UART_ENABLE , Setup_Config->CbsCmnSoCMiscHspUart);
  }

  //Check Display Condition CbsCmnSoCMiscHspUart=Enabled
  if ((Setup_Config->CbsCmnSoCMiscHspUart == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSoCMiscHspUartPort != 0xf) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_HSP_UART_PORT, Setup_Config->CbsCmnSoCMiscHspUartPort);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnSoCMiscHspFips != 0xf) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_HSP_FIPS_ENABLE, Setup_Config->CbsCmnSoCMiscHspFips);
  }

  //Check Display Condition CbsCmnSoCMiscHspFips=Enabled
  if ((Setup_Config->CbsCmnSoCMiscHspFips == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSoCMiscHspFipsZeroization != 0xf) {
      ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_HSP_FIPS_ZEROIZATION_ENABLE, Setup_Config->CbsCmnSoCMiscHspFipsZeroization);
    }
  }

  //Check if select Auto
  if (Setup_Config->CbsCmnSoCMiscHspAcpiFenceEnable != 0xf) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_HSP_ACPI_FENCE_ENABLE, Setup_Config->CbsCmnSoCMiscHspAcpiFenceEnable);
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSoCMiscWiredKvm != 0xf) {
      ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, Setup_Config->CbsCmnSoCMiscWiredKvm);
    }
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSoCMiscWirelessManageability != 0xf) {
      ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, Setup_Config->CbsCmnSoCMiscWirelessManageability);
    }
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled,CbsCmnSoCMiscWirelessManageability=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1) && (Setup_Config->CbsCmnSoCMiscWirelessManageability == 0x1)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnSoCMiscWirelessKvm != 0xf) {
      ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, Setup_Config->CbsCmnSoCMiscWirelessKvm);
    }
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    //Check if select Auto
    if (Setup_Config->CbsCmnIntrusionDetectionEnable != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_INTRUSION_DETECTION_ENABLE, Setup_Config->CbsCmnIntrusionDetectionEnable);
    }
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_LOG_EVENT, Setup_Config->CbsCmnIntrusionLogEvent);  //User Input
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_CLEAR_TPM, Setup_Config->CbsCmnIntrusionClearTpm);  //User Input
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_DEBUG, APCB_TOKEN_UID_INTRUSION_COUNTERMEASURE_POWER_OFF_SYSTEM, Setup_Config->CbsCmnIntrusionPowerOff);  //User Input
  }


  //Write back updated data to SPI
  ApcbProtocol->ApcbFlushData (ApcbProtocol);
  ApcbProtocol->ApcbReleaseMutex (ApcbProtocol);
  return EFI_SUCCESS;
}



UINT32
Murmur_32_scramble(UINT32 h, UINT32 k) {
  UINT32 MURMURHASH_C1 = 0xCC9E2D51;
  UINT32 MURMURHASH_C2 = 0x1B873593;
  UINT32 MURMURHASH_M = 5;
  UINT32 MURMURHASH_N = 0xE6546B64;

  k *= MURMURHASH_C1;
  k = (k << 15) | (k >> 17);
  k *= MURMURHASH_C2;
  
  h ^= k;
  h = (h << 13) | (h >> 19);
  h = h * MURMURHASH_M + MURMURHASH_N;
  
  return h;
}

UINT32
GetApcbHash (
  VOID *CbsVariable,
  AMD_APCB_SERVICE_PROTOCOL *ApcbProtocol
  )
{
  CBS_CONFIG                            *Setup_Config;
  UINT32 MURMURHASH_SEED = 0xACB55EED;
  UINT32 hash = MURMURHASH_SEED;
  UINT32 key;

  if (CbsVariable == NULL) {
    return 0xFFFFFFFF;
  }

  Setup_Config = (CBS_CONFIG *) CbsVariable;
  
  key = Setup_Config->CbsCmnCpuCpb;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuSevAsidCount;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuSevAsidSpaceLimit;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuSmuPspDebugMode;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuPpinCtrl;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnActionOnBistFailure;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuCoreCtrlRmb;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnCpuSmtCtrl;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnPersistentLow;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnPersistentHigh;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnPersistentSizeLow;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnPersistentSizeHigh;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnCc6MemEncryption;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnMemClear;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnExtIpSyncFloodProp;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnSyncFloodProp;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsMpExcepRecovHandle;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsMemClrTimeout;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnMemIntlvSize;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDfCmnDramMapInversion;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingSettingDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTargetSpeedDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTclDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrpDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrasDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrfc1Ddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrfc2Ddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrfcSbDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrtpDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrrdLDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrrdSDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTfawDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwtrLDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwtrSDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdScLDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdScDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdSdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdDdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrScLDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrScDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrSdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrDdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrrdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdwrDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDrvStrenCaDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDrvStrenCsDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDrvStrenCkDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDrvStrenDqDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemOdtImpedProcDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramDrvStrenDqDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramOdtImpedRttNomWrDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramOdtImpedRttNomRdDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramOdtImpedRttWrDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramOdtImpedRttParkDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramOdtImpedRttParkDqsDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPowerDownEnDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemEccDisErrInjectionDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDataPoisoningDdr4;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemEccEnDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramRedirectScrubEnDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramRedirectScrubLimitDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramScrubTime;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTsmeEnableDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDataCtrlDataScrambleEnDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemSpdCrcVerify;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemSpdReadRetryCountCrc;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMappingBankInterleaveDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemAddressHashBankDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemAddressHashCsDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerBankSwapModeDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemRestoreValidDays;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPmuTrainDfeDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramPdaEnumIdProgModeDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistEnDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistTestmodeDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistAggressorsDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistPerBitSlaveDieReportDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistPatternSelectDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistPatternLengthDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistAggressorsChnlDdr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemOverclockLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemSpeedLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcpageCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcpageLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTcwlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTclLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcdrdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcdwrLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrpLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrfcCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrfcLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrasLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrcLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrrdSLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrrdLLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTfawCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTfawLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwtrSLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwtrLLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdScLCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdScLLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrScLCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrScLLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrtpLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTckeLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdScLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdDdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdrdSdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrScLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrSdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrwrDdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTwrrdLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTimingTrdwrLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemRefModeLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemRfmEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemWckAlwaysOnEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemRrwMemtestCtrlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPwrDnEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPdPhyPowerSaveDisLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCadBusTimingCtlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemAddrCmdSetupLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCsOdtSetupLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenCaLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenCsLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenCkLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenWckLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenDqLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoDrvStrenDqsLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoOdtStrenProcLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoOdtStrenProcCaLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoOdtStrenProcCkLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemPhyIoOdtStrenProcWckLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramIoOdtStrenCaLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramIoOdtStrenDqLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramIoOdtStrenWckLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramIoOdtStrenNtdq;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDramIoDrvStrenPdds;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerDramRdLinkEccEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerDramWrLinkEccEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemDisMemErrInjLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemTsmeLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerDataScrambleLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMappingBankInterleaveLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerBankSwapLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerBankGroupSwapLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemAddressHashBankLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemAddressHashCsLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemCtrllerPmuTrainDfeLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistEnLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistTestmodeLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistAggressorsLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistPatternSelectLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistPatternLengthLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistAggressorsChnlLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistReadDataEyeVoltageStepLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistReadDataEyeTimingStepLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistWriteDataEyeVoltageStepLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnMemMbistWriteDataEyeTimingStepLpddr;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnGnbGfxUmaMode;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnGnbGfxUmaVersion;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnGnbGfxUmaFrameBufferSize;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnGpuHostTranslation;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnFchEspiCrc;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsDbgVddpVoltage;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnDfPstateP0Ctl;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnDfPstateFclkP0;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnDfPstateMemClkP0;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnPspRpmcSwitch;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSocAblConOut;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSocAblConOutSerialPort;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSocAblConOutSerialPortIO;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSocAblConOutBootMode;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSocAblPmuMsgCtrl;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscHspUart;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscHspUartPort;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscHspFips;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscHspFipsZeroization;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscHspAcpiFenceEnable;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscWiredKvm;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscWirelessManageability;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnSoCMiscWirelessKvm;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnIntrusionDetectionEnable;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnIntrusionLogEvent;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnIntrusionClearTpm;
  hash = Murmur_32_scramble(hash, key);

  key = Setup_Config->CbsCmnIntrusionPowerOff;
  hash = Murmur_32_scramble(hash, key);


  hash = Murmur_32_scramble(hash, 0xD24CF9BA);

  hash ^= 193;
  hash ^= hash >> 16;
  hash *= 0x85EBCA6B;
  hash ^= hash >> 13;
  hash *= 0xC2B2AE35;
  hash ^= hash >> 16;
  
  if(hash == 0x56434552){
    hash++;
  }
  
  return hash;
}
