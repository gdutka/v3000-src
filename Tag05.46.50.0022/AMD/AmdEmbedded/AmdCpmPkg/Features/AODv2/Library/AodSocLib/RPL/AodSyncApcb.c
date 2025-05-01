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
#include "AodSyncApcb.h"

AOD_MOUDLE_NAME ("AodSyncApcb")
AOD_SOC_NAME ("RMB")
/**
  This function used to get DF FCLK Frequency ID refer to ABL

  @retval 0               the Smn addres is invalid
  @retval DF FCLK Frequency ID
**/
UINT8
GetRplFclkFreqId (
  IN UINT16   Freq
  )
{
  UINT8   Index;
  UINT16  FClkFrequencies[56] = {
    667,
    800,
    933,
    1067,
    1200,
    1333,
    1367,
    1400,
    1433,
    1467,
    1500,
    1533,
    1567,
    1600,
    1633,
    1667,
    1700,
    1733,
    1767,
    1800,
    1833,
    1867,
    1900,
    1933,
    1967,
    2000,
    2033,
    2067,
    2100,
    2133,
    2167,
    2200,
    2233,
    2267,
    2300,
    2333,
    2367,
    2400,
    2433,
    2467,
    2500,
    2550,
    2600,
    2650,
    2700,
    2750,
    2800,
    2850,
    2900,
    2950,
    3000,
    333,
    400,
    533,
    1050,
    1066
  };
  for (Index = 0; Index < 56; Index ++) {
    if (FClkFrequencies[Index] == Freq) return Index;
  }
  return 0;
}

/**
  This function used to sync CoreInfo Variable into APCB

  @param[in]  This                the AMD_APCB_SERVICE_PROTOCOL handle

  @retval EFI_INVALID_PARAMETER if input protocol point is NULL
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodBitMapDownCoreSyncApcbRpl (
  VOID *AodVariable,
  VOID *vApcbProtocol
  )
{
  EFI_STATUS                            Status;
  AOD_CONFIG                            *AodConfig;
  FP_SET_TOKEN_8                        ApcbSetToken8;
  UINT32                                CcdIndex;
  AMD_APCB_SERVICE_PROTOCOL             *ApcbProtocol;

  Status = EFI_INVALID_PARAMETER;
  if (AodVariable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (vApcbProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  AodConfig = (AOD_CONFIG *) AodVariable;
  ApcbProtocol = (AMD_APCB_SERVICE_PROTOCOL *)vApcbProtocol;
  ApcbSetToken8 = ApcbProtocol->ApcbSetToken8;                    ///< Set an APCB UINT8 token

  for (CcdIndex = 0; CcdIndex < AodConfig->AodMaxCcdCount; CcdIndex ++) {
    if ((AodConfig->AodCcdBitMap & (1<<CcdIndex)) == 0) continue;
    if (AodConfig->AodCoreBitMap[CcdIndex] == 0) continue;
    switch (CcdIndex) {
      case 0:
        Status = ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_AOD_CCD0_CORE_CTRL, \
          AodConfig->AodCoreBitMap[0]);
        Status = ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_AOD_CCD_CORE_CTRL, 1);
        break;
      default:
        break;
    }
  }
  return Status;
}
/**
  This function used to update APCB token by AodVariable

  @param[in]  AodVariable                 Points to the L"AodSetup" Variable
  @param[in]  vApcbProtocol               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
#define AOD_APCB_NUMNER 54
EFI_STATUS
AodUpdateApcbTokensRpl (
  VOID *AodVariable,
  VOID *vApcbProtocol
  )
{
  EFI_STATUS                            Status;
  AOD_CONFIG                            *AodConfig;
  FP_GET_TOKEN_BOOL                     ApcbGetTokenBool;                 ///< Get an APCB BOOL token
  FP_SET_TOKEN_BOOL                     ApcbSetTokenBool;                 ///< Set an APCB BOOL token
  FP_GET_TOKEN_8                        ApcbGetToken8;                    ///< Get an APCB UINT8 token
  FP_SET_TOKEN_8                        ApcbSetToken8;                    ///< Set an APCB UINT8 token
  FP_GET_TOKEN_16                       ApcbGetToken16;                   ///< Get an APCB UINT16 token
  FP_SET_TOKEN_16                       ApcbSetToken16;                   ///< Set an APCB UINT16 token
  FP_GET_TOKEN_32                       ApcbGetToken32;                   ///< Get an APCB UINT32 token
  FP_SET_TOKEN_32                       ApcbSetToken32;                   ///< Set an APCB UINT32 token
  FP_PURGE_TOKENS                       ApcbPurgeTokens;
  AMD_APCB_SERVICE_PROTOCOL             *ApcbProtocol;
  UINT32                                ApcbTokens[AOD_APCB_NUMNER] = {
    APCB_TOKEN_UID_MEM_TARGET_SPEED_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TCL_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRCD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRP_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRAS_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRC_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWR_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRFC1_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRFC2_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRFCSB_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRTP_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRRDL_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRRDS_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TFAW_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWTRL_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWTRS_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRDRDSCL_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRDRDSC_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRDRDSD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRDRDDD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWRWRSCL_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWRWRSC_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWRWRSD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWRWRDD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TWRRD_DDR,
    APCB_TOKEN_UID_MEM_TIMING_TRDWR_DDR,
    APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR,
    APCB_TOKEN_UID_MEM_RTT_NOM_WR_DDR,
    APCB_TOKEN_UID_MEM_RTT_NOM_RD_DDR,
    APCB_TOKEN_UID_MEM_RTT_WR_DDR,
    APCB_TOKEN_UID_MEM_RTT_PARK_DDR,
    APCB_TOKEN_UID_MEM_RTT_PARK_DQS_DDR,
    APCB_TOKEN_UID_MEM_PROC_ODT_DDR,
    APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_PROC_DATA_DRV_STREN_DDR,
    APCB_TOKEN_UID_MEM_DRV_IMP_DDR,
    APCB_TOKEN_UID_MEM_CTRLLER_POWER_DOWN_EN_DDR,
    APCB_TOKEN_UID_GNB_SMU_VDDCR_SOC_VID_SVI3,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_FCLK_FREQUENCY,
    APCB_TOKEN_UID_MEM_OC_VDDIO,
    APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL,
    APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL_ENFORCE_ON,
    APCB_TOKEN_UID_MEM_OC_VPP,
    APCB_TOKEN_UID_CCX_SMT_CTRL,
    APCB_TOKEN_UID_AOD_CCD0_CORE_CTRL,
    APCB_TOKEN_UID_AOD_CCD1_CORE_CTRL,
    APCB_TOKEN_UID_AOD_CCD_CORE_CTRL,
    APCB_TOKEN_UID_CBS_CMN_CLDO_VDDP_CTL,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDP_VOLTAGE,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE1,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE1,
    APCB_TOKEN_UID_GNB_SMU_LN2_MODE,
    APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_DIV1_MODE,
  };
  if (AodVariable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (vApcbProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ApcbProtocol = (AMD_APCB_SERVICE_PROTOCOL *)vApcbProtocol;
  Status = ApcbProtocol->ApcbAcquireMutex (ApcbProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ApcbGetTokenBool = ApcbProtocol->ApcbGetTokenBool;                 ///< Get an APCB BOOL token
  ApcbSetTokenBool = ApcbProtocol->ApcbSetTokenBool;                 ///< Set an APCB BOOL token
  ApcbGetToken8 = ApcbProtocol->ApcbGetToken8;                       ///< Get an APCB UINT8 token
  ApcbSetToken8 = ApcbProtocol->ApcbSetToken8;                       ///< Set an APCB UINT8 token
  ApcbGetToken16 = ApcbProtocol->ApcbGetToken16;                     ///< Get an APCB UINT16 token
  ApcbSetToken16 = ApcbProtocol->ApcbSetToken16;                     ///< Set an APCB UINT16 token
  ApcbGetToken32 = ApcbProtocol->ApcbGetToken32;                     ///< Get an APCB UINT32 token
  ApcbSetToken32 = ApcbProtocol->ApcbSetToken32;                     ///< Set an APCB UINT32 token
  ApcbPurgeTokens =  ApcbProtocol->ApcbPurgeTokens;
  //Clear all token setting

  ApcbProtocol->ApcbPurgeTokens (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, ApcbTokens, AOD_APCB_NUMNER);

  AodConfig = (AOD_CONFIG *) AodVariable;

  //Check Display Condition AodMemTimingSettingDdr=Enabled
  if ((AodConfig->AodMemTimingSettingDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TARGET_SPEED_DDR, \
      AodConfig->AodMemTargetSpeedDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTclCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTclCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TCL_DDR, \
      AodConfig->AodMemTimingTclDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrcdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrcdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRCD_DDR, \
      AodConfig->AodMemTimingTrcdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrpCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrpCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRP_DDR, \
      AodConfig->AodMemTimingTrpDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrasCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrasCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRAS_DDR, \
      AodConfig->AodMemTimingTrasDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrcCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrcCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRC_DDR, \
      AodConfig->AodMemTimingTrcDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWR_DDR, \
      AodConfig->AodMemTimingTwrDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrfc1CtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrfc1CtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRFC1_DDR, \
      AodConfig->AodMemTimingTrfc1Ddr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrfc2CtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrfc2CtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRFC2_DDR, \
      AodConfig->AodMemTimingTrfc2Ddr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrfcSbCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrfcSbCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRFCSB_DDR, \
      AodConfig->AodMemTimingTrfcSbDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrtpCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrtpCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRTP_DDR, \
      AodConfig->AodMemTimingTrtpDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrrdLCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrrdLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRRDL_DDR, \
      AodConfig->AodMemTimingTrrdLDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrrdSCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrrdSCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRRDS_DDR, \
      AodConfig->AodMemTimingTrrdSDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTfawCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTfawCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TFAW_DDR, \
      AodConfig->AodMemTimingTfawDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwtrLCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwtrLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWTRL_DDR, \
      AodConfig->AodMemTimingTwtrLDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwtrSCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwtrSCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWTRS_DDR, \
      AodConfig->AodMemTimingTwtrSDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrdrdScLCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrdrdScLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRDRDSCL_DDR, \
      AodConfig->AodMemTimingTrdrdScLDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrdrdScCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrdrdScCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRDRDSC_DDR, \
      AodConfig->AodMemTimingTrdrdScDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrdrdSdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrdrdSdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRDRDSD_DDR, \
      AodConfig->AodMemTimingTrdrdSdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrdrdDdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrdrdDdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRDRDDD_DDR, \
      AodConfig->AodMemTimingTrdrdDdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrwrScLCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrwrScLCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWRWRSCL_DDR, \
      AodConfig->AodMemTimingTwrwrScLDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrwrScCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrwrScCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWRWRSC_DDR, \
      AodConfig->AodMemTimingTwrwrScDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrwrSdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrwrSdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWRWRSD_DDR, \
      AodConfig->AodMemTimingTwrwrSdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrwrDdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrwrDdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWRWRDD_DDR, \
      AodConfig->AodMemTimingTwrwrDdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTwrrdCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTwrrdCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TWRRD_DDR, \
      AodConfig->AodMemTimingTwrrdDdr);  //User Input
  }

  //Check Display Condition AodMemTimingSettingDdr=Enabled,AodMemTimingTrdwrCtrlDdr=Manual
  if ((AodConfig->AodMemTimingSettingDdr == 0x1) && (AodConfig->AodMemTimingTrdwrCtrlDdr == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_TIMING_TRDWR_DDR, \
      AodConfig->AodMemTimingTrdwrDdr);  //User Input
  }

  //Check if select Auto
  if (AodConfig->AodMemDataBusCfgCtlDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CONFIG_CTL_DDR, \
      AodConfig->AodMemDataBusCfgCtlDdr);
  }

  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgRttNomWrDdr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_RTT_NOM_WR_DDR, \
      AodConfig->AodMemDataBusCfgRttNomWrDdr);
    }
  }

  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgRttNomRdDdr != 0xff) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_RTT_NOM_RD_DDR, \
        AodConfig->AodMemDataBusCfgRttNomRdDdr);
    }
  }

  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgRttWrDdr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_RTT_WR_DDR, \
        AodConfig->AodMemDataBusCfgRttWrDdr);
    }
  }

  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgRttParkDdr != 0xff) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_RTT_PARK_DDR, \
        AodConfig->AodMemDataBusCfgRttParkDdr);
    }
  }


  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgRttParkDqsDdr != 0xff) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_RTT_PARK_DQS_DDR, \
        AodConfig->AodMemDataBusCfgRttParkDqsDdr);
    }
  }


  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgProcOdtDdr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_PROC_ODT_DDR, \
        AodConfig->AodMemDataBusCfgProcOdtDdr);
    }
  }

  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgProcDataDrvStrenDdr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, \
        APCB_TOKEN_UID_CBS_CMN_MEM_DATA_BUS_CFG_PROC_DATA_DRV_STREN_DDR, \
        AodConfig->AodMemDataBusCfgProcDataDrvStrenDdr);
    }
  }


  //Check Display Condition AodMemDataBusCfgCtlDdr=Manual
  if ((AodConfig->AodMemDataBusCfgCtlDdr == 0x1)) {
    //Check if select Auto
    if (AodConfig->AodMemDataBusCfgDramDataDrvStrenDdr != 0xFF) {
      ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_DRV_IMP_DDR, \
        AodConfig->AodMemDataBusCfgDramDataDrvStrenDdr);
    }
  }


  //Check if select Auto
  if (AodConfig->AodMemCtrllerPowerDownEnDdr != 0xFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_CTRLLER_POWER_DOWN_EN_DDR, \
      AodConfig->AodMemCtrllerPowerDownEnDdr);
  }

  ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_GNB_FCLK_OC_FLAG, AodConfig->AodGnbFclkOcFlag);  //User Input

  if (AodConfig->AodSocOverclockingVoltage != 0) {
    // Convert voltage in mV to SVI3 VID
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_GNB_SMU_VDDCR_SOC_VID_SVI3, \
      (UINT16)mV_TO_VID3 (AodConfig->AodSocOverclockingVoltage));  //User Input
    AOD_DEBUG_SOC ((EFI_D_INFO, "AodSocOverclockingVoltage = %d mV, VID = 0x%x\n", \
      AodConfig->AodSocOverclockingVoltage, mV_TO_VID3 (AodConfig->AodSocOverclockingVoltage)));
  }

  //Check if select Auto
  if (AodConfig->AodFclkFrequency  != 0xFFFF) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_FCLK_FREQUENCY, \
      (UINT8)GetRplFclkFreqId (AodConfig->AodFclkFrequency));
  }

  //Check if select Auto
  if (AodConfig->AodUclkDiv1Mode != 0xff) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_UCLK_DIV1_MODE, \
      AodConfig->AodUclkDiv1Mode);
  }

  //Check Display Condition AodVddIoCtrl=Manual|AodVddIoCtrl=Separate
  if (((AodConfig->AodVddIoCtrl == 0x1)) ||((AodConfig->AodVddIoCtrl == 0x2))) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_OC_VDDIO, AodConfig->AodDimmVddIo);  //User Input
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL, 1);
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_OC_VDDIO_CONTROL_ENFORCE_ON, 1);
  }

  //Check Display Condition AodVppCtrl=Manual
  if ((AodConfig->AodVppCtrl == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MEM_OC_VPP, AodConfig->AodVppVoltage);  //User Input
  }

  if (AodConfig->AodCpuSmtCtrl != 0x1) {
    ApcbSetToken8 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CCX_SMT_CTRL, AodConfig->AodCpuSmtCtrl);
  }
  //Check Display Condition AodVddpVoltageCtrl=Manual
  if ((AodConfig->AodVddpVoltageCtrl != 0xff)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_CLDO_VDDP_CTL, 1);
  }
  //Check Display Condition AodVddpVoltageCtrl=Manual
  if ((AodConfig->AodVddpVoltageCtrl == 0x1)) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_VDDP_VOLTAGE,\
      AodConfig->AodVddpVoltage);  //User Input
  }

  AodBitMapDownCoreSyncApcbRpl (AodConfig, ApcbProtocol);

  if (AodConfig->AodVddgCtrl == 1) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE, \
      (UINT16) AodConfig->AodGlobalVddgCcdVoltage);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE1, \
      (UINT16) AodConfig->AodGlobalVddgCcdVoltage);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE, \
      (UINT16) AodConfig->AodGlobalVddgIodVoltage);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE1, \
      (UINT16) AodConfig->AodGlobalVddgIodVoltage);  //User Input
  }

  if (AodConfig->AodVddgCtrl == 2) {
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE, \
      (UINT16) AodConfig->AodPerCcdVddgVoltage[0]);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE1, \
      (UINT16) AodConfig->AodPerCcdVddgVoltage[1]);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE, \
      (UINT16) AodConfig->AodPerCcdVddgIODVoltage[0]);  //User Input
    ApcbSetToken16 (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE1, \
      (UINT16) AodConfig->AodPerCcdVddgIODVoltage[1]);  //User Input
  }
  if (AodConfig->AodLN2Mode != 0xff) {
    ApcbSetTokenBool (ApcbProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_GNB_SMU_LN2_MODE, \
      (BOOLEAN) AodConfig->AodLN2Mode);
  }
  ApcbProtocol->ApcbFlushData (ApcbProtocol);
  ApcbProtocol->ApcbReleaseMutex (ApcbProtocol);

  return EFI_SUCCESS;
}
