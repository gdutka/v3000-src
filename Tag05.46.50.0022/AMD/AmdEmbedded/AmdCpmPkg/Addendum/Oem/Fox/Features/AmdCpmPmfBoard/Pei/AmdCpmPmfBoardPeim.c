/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <AmdCpmCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>

//
// AMD PMF Table
//
AMD_CPM_PMF_TABLE gCpmPmfTable = {
//  AMD_CPM_TABLE_COMMON_HEADER     Header;                                    ///< Table header
//  UINT32                          SupportedNotificationsMask;                ///< Supported Notifications Mask
//  UINT32                          SupportedFunctionsBitVector;               ///< Supported Functions Bit Vector
//  UINT8                           PmfNotifyCommandCode;                      ///< Indicates that Notify (PMF, n) is used as a general-purpose notification
//  UINT32                          HeartbeatTimeInterval;                     ///< Time interval in seconds for PMF to send heartbeat signal to BIOS.
//  AMD_CPM_APMF_FUNCTION_3_TABLE   ApmfFn3Settings;                           ///< APMF function 3 settings.
//  AMD_CPM_APMF_FUNCTION_5_TABLE   ApmfFn5Settings;                           ///< APMF function 5 settings.
//  AMD_CPM_APMF_FUNCTION_6_TABLE   ApmfFn6Settings;                           ///< APMF function 6 settings.
//  AMD_CPM_APMF_FUNCTION_9_TABLE   ApmfFn9Settings;                           ///< APMF function 9 settings.
//  AMD_CPM_APMF_FUNCTION_11_TABLE  ApmfFn11Settings;                          ///< APMF function 11 settings.
//  AMD_CPM_APMF_FUNCTION_12_TABLE  ApmfFn12Settings;                          ///< APMF function 12 settings.
//  AMD_CPM_APMF_FUNCTION_13_TABLE  ApmfFn13Settings;                          ///< APMF function 13 settings.
//  UINT32                          NotifySbiosHeartbeatAcpiMethodObjName;     ///< The OEM defined ACPI method name under \_SB  for PMF heartbeat
//  UINT32                          SetFanTableIndexAcpiMethodObjName;         ///< The OEM defined ACPI method name under \_SB for changing FAN table to EC.
//  UINT32                          SliderEventNotificationAcpiMethodObjName;  ///< The OEM defined ACPI method name under \_SB for getting Slider Event Notification.
//  UINT32                          NotifyInBagPolicyUpdateAcpiMethodObjName;  ///< The OEM defined ACPI method name under \_SB for in Bag Policy Update.
  {CPM_SIGNATURE_PMF, sizeof (gCpmPmfTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0x1C,                                                                        // 0001_1100'b: DYTC CQL Event, DYTC AMT Event, Update Power Limits Event.
  0x000002C3,                                                                  // 010_1101_0011'b: Default Supported function 1, 2, 7, 8, 10
  0x81,                                                                        // Notify (PMF, 0x81)
  60,                                                                          // HeartbeatTimeInterval, default = 60s
  {sizeof (AMD_CPM_APMF_FUNCTION_3_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_5_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_6_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_9_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_11_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_12_TABLE)},
  {sizeof (AMD_CPM_APMF_FUNCTION_13_TABLE)},
  CPM_SIGNATURE_32 ('A', 'P', 'X', '4'),                                       // The OEM defined ACPI method name under \_SB for PMF heartbeat               // Exapmle: \_SB.APX4 ()
  CPM_SIGNATURE_32 ('A', 'P', 'X', '7'),                                       // The OEM ACPI method name under \_SB for changing FAN table to EC            // Exapmle: \_SB.APX7 (Arg0, Arg1)
  CPM_SIGNATURE_32 ('A', 'P', 'X', '8'),                                       // The OEM ACPI method name under \_SB for getting Slider Event Notification.  // Exapmle: \_SB.APX8 (Arg0)
  CPM_SIGNATURE_32 ('A', 'P', 'X', 'A'),                                       // The OEM ACPI method name under \_SB for in Bag Policy Update.               // Exapmle: \_SB.APXA (Arg0)
  CPM_SIGNATURE_32 ('A', 'P', 'X', 'E'),                                       // The OEM ACPI method name under \_SB for Notify Smart PC Solution Updates.   // Exapmle: \_SB.APXE (Arg0, Arg1, Arg2)
};

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM PMF BOARD PEIM driver
 *
 * This function installs AMD CPM PMF table to AMD CPM table lists.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmPmfBoardPeimEntryPoint (
  IN        CPM_PEI_FILE_HANDLE         FileHandle,
  IN        CPM_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS                          Status;
  AMD_CPM_TABLE_PPI                  *AmdCpmTablePpi;
  AMD_PBS_SETUP_OPTION                AmdPbsConfiguration;

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-Start.\n", __FUNCTION__));

  Status = (*PeiServices)->LocatePpi (
                                      (CPM_PEI_SERVICES**)PeiServices,
                                      &gAmdCpmTablePpiGuid,
                                      0,
                                      NULL,
                                      (VOID**)&AmdCpmTablePpi
                                      );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-Locate gAmdCpmTablePpiGuid failed: %r.\n", __FUNCTION__, Status));
    return  Status;
  }

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-GetAmdPbsConfiguration-Status=%r\n",  __FUNCTION__,Status));
    return Status;
  }

  if (AmdPbsConfiguration.AmdPmfSupport == 0 ) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-PMF is not enabled\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  //Assign the heartbeat interval according to PBS settings.
  gCpmPmfTable.HeartbeatTimeInterval = AmdPbsConfiguration.ApmfFn1_HeartbeatTimeInterval;

  if (AmdPbsConfiguration.AmdPmfFn3_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT2; //Set Function 3 support.
    gCpmPmfTable.ApmfFn3Settings.AC_Best_Performance_Limit = AmdPbsConfiguration.ApmfFn3_AC_Best_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_AC_Best_Performance_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_AC_Best_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.AC_Better_Performance_Limit = AmdPbsConfiguration.ApmfFn3_AC_Better_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_AC_Better_Performance_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_AC_Better_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.AC_Better_Battery_Limit = AmdPbsConfiguration.ApmfFn3_AC_Better_Battery_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_AC_Better_Battery_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_AC_Better_Battery_Limit;
    gCpmPmfTable.ApmfFn3Settings.DC_Best_Performance_Limit = AmdPbsConfiguration.ApmfFn3_DC_Best_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_DC_Best_Performance_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_DC_Best_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.DC_Better_Performance_Limit = AmdPbsConfiguration.ApmfFn3_DC_Better_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_DC_Better_Performance_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_DC_Better_Performance_Limit;
    gCpmPmfTable.ApmfFn3Settings.DC_Better_Battery_Limit = AmdPbsConfiguration.ApmfFn3_DC_Better_Battery_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_DC_Better_Battery_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_DC_Better_Battery_Limit;
    gCpmPmfTable.ApmfFn3Settings.DC_Battery_Saver_Limit = AmdPbsConfiguration.ApmfFn3_DC_Battery_Saver_Limit;
    gCpmPmfTable.ApmfFn3Settings.Fan_ID_DC_Battery_Saver_Limit = AmdPbsConfiguration.ApmfFn3_Fan_ID_DC_Battery_Saver_Limit;
  }

  if (AmdPbsConfiguration.AmdPmfFn4_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT3; //Set Function 4 support.
  }

  if (AmdPbsConfiguration.AmdPmfFn5_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT4; //Set Function 5 support.
    gCpmPmfTable.ApmfFn5Settings.T_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn5_T_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn5Settings.T_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn5_T_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn5Settings.T_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn5_T_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn5Settings.T_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn5_T_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn5Settings.Pfloor_Perf = AmdPbsConfiguration.ApmfFn5_Pfloor_Perf;
    gCpmPmfTable.ApmfFn5Settings.Pfloor_Balanced = AmdPbsConfiguration.ApmfFn5_Pfloor_Balanced;
    gCpmPmfTable.ApmfFn5Settings.Pfloor_Quiet = AmdPbsConfiguration.ApmfFn5_Pfloor_Quiet;
    gCpmPmfTable.ApmfFn5Settings.P_delta_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn5_P_delta_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn5Settings.P_delta_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn5_P_delta_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn5Settings.P_delta_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn5_P_delta_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn5Settings.P_delta_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn5_P_delta_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_APU_Perf_on_Lap = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_APU_Perf_on_Lap;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_HS2_Perf_on_Lap = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_HS2_Perf_on_Lap;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_APU_Perf = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_APU_Perf;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_HS2_Perf = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_HS2_Perf;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_APU_Balanced = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_APU_Balanced;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_HS2_Balanced = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_HS2_Balanced;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_APU_Quiet = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_APU_Quiet;
    gCpmPmfTable.ApmfFn5Settings.STT_SkinTempLimit_HS2_Quiet = AmdPbsConfiguration.ApmfFn5_STT_SkinTempLimit_HS2_Quiet;
    gCpmPmfTable.ApmfFn5Settings.STTMinLimit_Perf_on_lap = AmdPbsConfiguration.ApmfFn5_STTMinLimit_Perf_on_lap;
    gCpmPmfTable.ApmfFn5Settings.STTMinLimit_Perf = AmdPbsConfiguration.ApmfFn5_STTMinLimit_Perf;
    gCpmPmfTable.ApmfFn5Settings.STTMinLimit_Balanced = AmdPbsConfiguration.ApmfFn5_STTMinLimit_Balanced;
    gCpmPmfTable.ApmfFn5Settings.STTMinLimit_Quiet = AmdPbsConfiguration.ApmfFn5_STTMinLimit_Quiet;
    gCpmPmfTable.ApmfFn5Settings.FPPT_Perf_on_lap = AmdPbsConfiguration.ApmfFn5_FPPT_Perf_on_lap;
    gCpmPmfTable.ApmfFn5Settings.SPPT_Perf_on_lap = AmdPbsConfiguration.ApmfFn5_SPPT_Perf_on_lap;
    gCpmPmfTable.ApmfFn5Settings.SPL_Perf_on_lap = AmdPbsConfiguration.ApmfFn5_SPL_Perf_on_lap;
    gCpmPmfTable.ApmfFn5Settings.SPPT_APUOnly_Perf_on_Lap = AmdPbsConfiguration.ApmfFn5_SPPT_APUOnly_Perf_on_Lap;
    gCpmPmfTable.ApmfFn5Settings.FPPT_Perf = AmdPbsConfiguration.ApmfFn5_FPPT_Perf;
    gCpmPmfTable.ApmfFn5Settings.SPPT_Perf = AmdPbsConfiguration.ApmfFn5_SPPT_Perf;
    gCpmPmfTable.ApmfFn5Settings.SPL_Perf = AmdPbsConfiguration.ApmfFn5_SPL_Perf;
    gCpmPmfTable.ApmfFn5Settings.SPPT_APUOnly_Perf = AmdPbsConfiguration.ApmfFn5_SPPT_APUOnly_Perf;
    gCpmPmfTable.ApmfFn5Settings.FPPT_Balanced = AmdPbsConfiguration.ApmfFn5_FPPT_Balanced;
    gCpmPmfTable.ApmfFn5Settings.SPPT_Balanced = AmdPbsConfiguration.ApmfFn5_SPPT_Balanced;
    gCpmPmfTable.ApmfFn5Settings.SPL_Balanced = AmdPbsConfiguration.ApmfFn5_SPL_Balanced;
    gCpmPmfTable.ApmfFn5Settings.SPPT_APUOnly_Balanced = AmdPbsConfiguration.ApmfFn5_SPPT_APUOnly_Balanced;
    gCpmPmfTable.ApmfFn5Settings.FPPT_Quiet = AmdPbsConfiguration.ApmfFn5_FPPT_Quiet;
    gCpmPmfTable.ApmfFn5Settings.SPPT_Quiet = AmdPbsConfiguration.ApmfFn5_SPPT_Quiet;
    gCpmPmfTable.ApmfFn5Settings.SPL_Quiet = AmdPbsConfiguration.ApmfFn5_SPL_Quiet;
    gCpmPmfTable.ApmfFn5Settings.SPPT_APUOnly_Quiet = AmdPbsConfiguration.ApmfFn5_SPPT_APUOnly_Quiet;
    gCpmPmfTable.ApmfFn5Settings.Fan_ID_Perf = AmdPbsConfiguration.ApmfFn5_Fan_ID_Perf;
    gCpmPmfTable.ApmfFn5Settings.Fan_ID_Balanced = AmdPbsConfiguration.ApmfFn5_Fan_ID_Balanced;
    gCpmPmfTable.ApmfFn5Settings.Fan_ID_Quiet = AmdPbsConfiguration.ApmfFn5_Fan_ID_Quiet;
  }

  if (AmdPbsConfiguration.AmdPmfFn6_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT5;  //Set Function 6 support.
    gCpmPmfTable.ApmfFn6Settings.ConditionTemp_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_ConditionTemp_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.ConditionTemp_In_Bag_S4_Policy = AmdPbsConfiguration.ApmfFn6_ConditionTemp_In_Bag_S4_Policy;
    gCpmPmfTable.ApmfFn6Settings.FPPT_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_FPPT_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPPT_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_SPPT_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPPT_ApuOnly_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_SPPT_ApuOnly_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPL_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_SPL_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.STTMinLimit_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_STTMinLimit_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.STT_SkinTempLimit_APU_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_STT_SkinTempLimit_APU_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.STT_SkinTempLimit_HS2_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_STT_SkinTempLimit_HS2_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.Fan_ID_In_Bag_Policy = AmdPbsConfiguration.ApmfFn6_Fan_ID_In_Bag_Policy;
    gCpmPmfTable.ApmfFn6Settings.FPPT_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_FPPT_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPPT_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_SPPT_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPPT_ApuOnly_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_SPPT_ApuOnly_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.SPL_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_SPL_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.STTMinLimit_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_STTMinLimit_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.STT_SkinTempLimit_APU_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_STT_SkinTempLimit_APU_In_Bag_Exit_Policy;
    gCpmPmfTable.ApmfFn6Settings.STT_SkinTempLimit_HS2_In_Bag_Exit_Policy = AmdPbsConfiguration.ApmfFn6_STT_SkinTempLimit_HS2_In_Bag_Exit_Policy;
 }

  if (AmdPbsConfiguration.AmdPmfFn9_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT8;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BestPerfAC = AmdPbsConfiguration.ApmfFn9_fPPT_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BestPerfAC = AmdPbsConfiguration.ApmfFn9_sPPT_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBestPerfAC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BestPerfAC = AmdPbsConfiguration.ApmfFn9_SPL_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BestPerfAC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BestPerfAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BestPerfAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BestPerfAC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BestPerfAC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_fPPT_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_sPPT_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBetterPerfAC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_SPL_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BetterPerfAC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BetterPerfAC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_fPPT_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_sPPT_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBetterBatteryAC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_SPL_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BetterBatteryAC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BetterBatteryAC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BestPerfDC = AmdPbsConfiguration.ApmfFn9_fPPT_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BestPerfDC = AmdPbsConfiguration.ApmfFn9_sPPT_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBestPerfDC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BestPerfDC = AmdPbsConfiguration.ApmfFn9_SPL_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BestPerfDC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BestPerfDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BestPerfDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BestPerfDC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BestPerfDC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_fPPT_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_sPPT_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBetterPerfDC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_SPL_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BetterPerfDC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BetterPerfDC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_fPPT_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_sPPT_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBetterBatteryDC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_SPL_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BetterBatteryDC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BetterBatteryDC;
    gCpmPmfTable.ApmfFn9Settings.fPPT_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_fPPT_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_sPPT_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.sPPT_APUOnlyBetterBatterySaverDC = AmdPbsConfiguration.ApmfFn9_sPPT_APUOnlyBetterBatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.SPL_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_SPL_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.STTMinLimit_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_STTMinLimit_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_APU_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_APU_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.STT_SkinTempLimit_HS2_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_STT_SkinTempLimit_HS2_BatterySaverDC;
    gCpmPmfTable.ApmfFn9Settings.Fan_ID_BatterySaverDC = AmdPbsConfiguration.ApmfFn9_Fan_ID_BatterySaverDC;
  }

  if (AmdPbsConfiguration.AmdPmfFn11_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT10;
    gCpmPmfTable.ApmfFn11Settings.Flags = AmdPbsConfiguration.ApmfFn11_Flags;
    gCpmPmfTable.ApmfFn11Settings.T_Perf_to_Turbo = AmdPbsConfiguration.ApmfFn11_T_Perf_to_Turbo;
    gCpmPmfTable.ApmfFn11Settings.T_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn11_T_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn11Settings.T_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn11_T_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn11Settings.T_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn11_T_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn11Settings.T_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn11_T_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn11Settings.T_Turbo_to_Perf = AmdPbsConfiguration.ApmfFn11_T_Turbo_to_Perf;
    gCpmPmfTable.ApmfFn11Settings.Pfloor_Turbo = AmdPbsConfiguration.ApmfFn11_Pfloor_Turbo;
    gCpmPmfTable.ApmfFn11Settings.FPPT_Turbo = AmdPbsConfiguration.ApmfFn11_FPPT_Turbo;
    gCpmPmfTable.ApmfFn11Settings.SPPT_Turbo = AmdPbsConfiguration.ApmfFn11_SPPT_Turbo;
    gCpmPmfTable.ApmfFn11Settings.SPPT_APUOnly_Turbo = AmdPbsConfiguration.ApmfFn11_SPPT_APUOnly_Turbo;
    gCpmPmfTable.ApmfFn11Settings.SPL_Turbo = AmdPbsConfiguration.ApmfFn11_SPL_Turbo;
    gCpmPmfTable.ApmfFn11Settings.STT_MinLimit_Turbo = AmdPbsConfiguration.ApmfFn11_STT_MinLimit_Turbo;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempAPU_Turbo = AmdPbsConfiguration.ApmfFn11_STT_SkinTempAPU_Turbo;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempHS2_Turbo = AmdPbsConfiguration.ApmfFn11_STT_SkinTempHS2_Turbo;
    gCpmPmfTable.ApmfFn11Settings.Fan_ID_Turbo = AmdPbsConfiguration.ApmfFn11_Fan_ID_Turbo;
    gCpmPmfTable.ApmfFn11Settings.Pfloor_Performance = AmdPbsConfiguration.ApmfFn11_Pfloor_Performance;
    gCpmPmfTable.ApmfFn11Settings.FPPT_Performance = AmdPbsConfiguration.ApmfFn11_FPPT_Performance;
    gCpmPmfTable.ApmfFn11Settings.SPPT_Performance = AmdPbsConfiguration.ApmfFn11_SPPT_Performance;
    gCpmPmfTable.ApmfFn11Settings.SPPT_APUOnly_Performance = AmdPbsConfiguration.ApmfFn11_SPPT_APUOnly_Performance;
    gCpmPmfTable.ApmfFn11Settings.SPL_Performance = AmdPbsConfiguration.ApmfFn11_SPL_Performance;
    gCpmPmfTable.ApmfFn11Settings.STT_MinLimit_Performance = AmdPbsConfiguration.ApmfFn11_STT_MinLimit_Performance;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempAPU_Performance = AmdPbsConfiguration.ApmfFn11_STT_SkinTempAPU_Performance;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempHS2_Performance = AmdPbsConfiguration.ApmfFn11_STT_SkinTempHS2_Performance;
    gCpmPmfTable.ApmfFn11Settings.Fan_ID_Performance = AmdPbsConfiguration.ApmfFn11_Fan_ID_Performance;
    gCpmPmfTable.ApmfFn11Settings.Pfloor_Balanced = AmdPbsConfiguration.ApmfFn11_Pfloor_Balanced;
    gCpmPmfTable.ApmfFn11Settings.FPPT_Balanced = AmdPbsConfiguration.ApmfFn11_FPPT_Balanced;
    gCpmPmfTable.ApmfFn11Settings.SPPT_Balanced = AmdPbsConfiguration.ApmfFn11_SPPT_Balanced;
    gCpmPmfTable.ApmfFn11Settings.SPPT_APUOnly_Balanced = AmdPbsConfiguration.ApmfFn11_SPPT_APUOnly_Balanced;
    gCpmPmfTable.ApmfFn11Settings.SPL_Balanced = AmdPbsConfiguration.ApmfFn11_SPL_Balanced;
    gCpmPmfTable.ApmfFn11Settings.STT_MinLimit_Balanced = AmdPbsConfiguration.ApmfFn11_STT_MinLimit_Balanced;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempAPU_Balanced = AmdPbsConfiguration.ApmfFn11_STT_SkinTempAPU_Balanced;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempHS2_Balanced = AmdPbsConfiguration.ApmfFn11_STT_SkinTempHS2_Balanced;
    gCpmPmfTable.ApmfFn11Settings.Fan_ID_Balanced = AmdPbsConfiguration.ApmfFn11_Fan_ID_Balanced;
    gCpmPmfTable.ApmfFn11Settings.Pfloor_Quiet = AmdPbsConfiguration.ApmfFn11_Pfloor_Quiet;
    gCpmPmfTable.ApmfFn11Settings.FPPT_Quiet = AmdPbsConfiguration.ApmfFn11_FPPT_Quiet;
    gCpmPmfTable.ApmfFn11Settings.SPPT_Quiet = AmdPbsConfiguration.ApmfFn11_SPPT_Quiet;
    gCpmPmfTable.ApmfFn11Settings.SPPT_APUOnly_Quiet = AmdPbsConfiguration.ApmfFn11_SPPT_APUOnly_Quiet;
    gCpmPmfTable.ApmfFn11Settings.SPL_Quiet = AmdPbsConfiguration.ApmfFn11_SPL_Quiet;
    gCpmPmfTable.ApmfFn11Settings.STT_MinLimit_Quiet = AmdPbsConfiguration.ApmfFn11_STT_MinLimit_Quiet;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempAPU_Quiet = AmdPbsConfiguration.ApmfFn11_STT_SkinTempAPU_Quiet;
    gCpmPmfTable.ApmfFn11Settings.STT_SkinTempHS2_Quiet = AmdPbsConfiguration.ApmfFn11_STT_SkinTempHS2_Quiet;
    gCpmPmfTable.ApmfFn11Settings.Fan_ID_Quiet = AmdPbsConfiguration.ApmfFn11_Fan_ID_Quiet;
  }
  if (AmdPbsConfiguration.AmdPmfFn12_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT11;
    gCpmPmfTable.ApmfFn12Settings.Flags = AmdPbsConfiguration.ApmfFn12_Flags;
    gCpmPmfTable.ApmfFn12Settings.T_Perf_to_Turbo = AmdPbsConfiguration.ApmfFn12_T_Perf_to_Turbo;
    gCpmPmfTable.ApmfFn12Settings.T_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn12_T_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn12Settings.T_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn12_T_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn12Settings.T_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn12_T_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn12Settings.T_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn12_T_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn12Settings.T_Turbo_to_Perf = AmdPbsConfiguration.ApmfFn12_T_Turbo_to_Perf;
    gCpmPmfTable.ApmfFn12Settings.Pfloor_Turbo = AmdPbsConfiguration.ApmfFn12_Pfloor_Turbo;
    gCpmPmfTable.ApmfFn12Settings.FPPT_Turbo = AmdPbsConfiguration.ApmfFn12_FPPT_Turbo;
    gCpmPmfTable.ApmfFn12Settings.SPPT_Turbo = AmdPbsConfiguration.ApmfFn12_SPPT_Turbo;
    gCpmPmfTable.ApmfFn12Settings.SPPT_APUOnly_Turbo = AmdPbsConfiguration.ApmfFn12_SPPT_APUOnly_Turbo;
    gCpmPmfTable.ApmfFn12Settings.SPL_Turbo = AmdPbsConfiguration.ApmfFn12_SPL_Turbo;
    gCpmPmfTable.ApmfFn12Settings.STT_MinLimit_Turbo = AmdPbsConfiguration.ApmfFn12_STT_MinLimit_Turbo;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempAPU_Turbo = AmdPbsConfiguration.ApmfFn12_STT_SkinTempAPU_Turbo;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempHS2_Turbo = AmdPbsConfiguration.ApmfFn12_STT_SkinTempHS2_Turbo;
    gCpmPmfTable.ApmfFn12Settings.Fan_ID_Turbo = AmdPbsConfiguration.ApmfFn12_Fan_ID_Turbo;
    gCpmPmfTable.ApmfFn12Settings.Pfloor_Performance = AmdPbsConfiguration.ApmfFn12_Pfloor_Performance;
    gCpmPmfTable.ApmfFn12Settings.FPPT_Performance = AmdPbsConfiguration.ApmfFn12_FPPT_Performance;
    gCpmPmfTable.ApmfFn12Settings.SPPT_Performance = AmdPbsConfiguration.ApmfFn12_SPPT_Performance;
    gCpmPmfTable.ApmfFn12Settings.SPPT_APUOnly_Performance = AmdPbsConfiguration.ApmfFn12_SPPT_APUOnly_Performance;
    gCpmPmfTable.ApmfFn12Settings.SPL_Performance = AmdPbsConfiguration.ApmfFn12_SPL_Performance;
    gCpmPmfTable.ApmfFn12Settings.STT_MinLimit_Performance = AmdPbsConfiguration.ApmfFn12_STT_MinLimit_Performance;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempAPU_Performance = AmdPbsConfiguration.ApmfFn12_STT_SkinTempAPU_Performance;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempHS2_Performance = AmdPbsConfiguration.ApmfFn12_STT_SkinTempHS2_Performance;
    gCpmPmfTable.ApmfFn12Settings.Fan_ID_Performance = AmdPbsConfiguration.ApmfFn12_Fan_ID_Performance;
    gCpmPmfTable.ApmfFn12Settings.Pfloor_Balanced = AmdPbsConfiguration.ApmfFn12_Pfloor_Balanced;
    gCpmPmfTable.ApmfFn12Settings.FPPT_Balanced = AmdPbsConfiguration.ApmfFn12_FPPT_Balanced;
    gCpmPmfTable.ApmfFn12Settings.SPPT_Balanced = AmdPbsConfiguration.ApmfFn12_SPPT_Balanced;
    gCpmPmfTable.ApmfFn12Settings.SPPT_APUOnly_Balanced = AmdPbsConfiguration.ApmfFn12_SPPT_APUOnly_Balanced;
    gCpmPmfTable.ApmfFn12Settings.SPL_Balanced = AmdPbsConfiguration.ApmfFn12_SPL_Balanced;
    gCpmPmfTable.ApmfFn12Settings.STT_MinLimit_Balanced = AmdPbsConfiguration.ApmfFn12_STT_MinLimit_Balanced;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempAPU_Balanced = AmdPbsConfiguration.ApmfFn12_STT_SkinTempAPU_Balanced;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempHS2_Balanced = AmdPbsConfiguration.ApmfFn12_STT_SkinTempHS2_Balanced;
    gCpmPmfTable.ApmfFn12Settings.Fan_ID_Balanced = AmdPbsConfiguration.ApmfFn12_Fan_ID_Balanced;
    gCpmPmfTable.ApmfFn12Settings.Pfloor_Quiet = AmdPbsConfiguration.ApmfFn12_Pfloor_Quiet;
    gCpmPmfTable.ApmfFn12Settings.FPPT_Quiet = AmdPbsConfiguration.ApmfFn12_FPPT_Quiet;
    gCpmPmfTable.ApmfFn12Settings.SPPT_Quiet = AmdPbsConfiguration.ApmfFn12_SPPT_Quiet;
    gCpmPmfTable.ApmfFn12Settings.SPPT_APUOnly_Quiet = AmdPbsConfiguration.ApmfFn12_SPPT_APUOnly_Quiet;
    gCpmPmfTable.ApmfFn12Settings.SPL_Quiet = AmdPbsConfiguration.ApmfFn12_SPL_Quiet;
    gCpmPmfTable.ApmfFn12Settings.STT_MinLimit_Quiet = AmdPbsConfiguration.ApmfFn12_STT_MinLimit_Quiet;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempAPU_Quiet = AmdPbsConfiguration.ApmfFn12_STT_SkinTempAPU_Quiet;
    gCpmPmfTable.ApmfFn12Settings.STT_SkinTempHS2_Quiet = AmdPbsConfiguration.ApmfFn12_STT_SkinTempHS2_Quiet;
    gCpmPmfTable.ApmfFn12Settings.Fan_ID_Quiet = AmdPbsConfiguration.ApmfFn12_Fan_ID_Quiet;
  }

  if (AmdPbsConfiguration.AmdPmfFn13_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT12; //Set Function 13 support.
    gCpmPmfTable.ApmfFn13Settings.T_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn13_T_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn13Settings.T_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn13_T_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn13Settings.T_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn13_T_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn13Settings.T_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn13_T_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn13Settings.Pfloor_Perf = AmdPbsConfiguration.ApmfFn13_Pfloor_Perf;
    gCpmPmfTable.ApmfFn13Settings.Pfloor_Balanced = AmdPbsConfiguration.ApmfFn13_Pfloor_Balanced;
    gCpmPmfTable.ApmfFn13Settings.Pfloor_Quiet = AmdPbsConfiguration.ApmfFn13_Pfloor_Quiet;
    gCpmPmfTable.ApmfFn13Settings.P_delta_Balanced_to_Perf = AmdPbsConfiguration.ApmfFn13_P_delta_Balanced_to_Perf;
    gCpmPmfTable.ApmfFn13Settings.P_delta_Perf_to_Balanced = AmdPbsConfiguration.ApmfFn13_P_delta_Perf_to_Balanced;
    gCpmPmfTable.ApmfFn13Settings.P_delta_Quiet_to_Balanced = AmdPbsConfiguration.ApmfFn13_P_delta_Quiet_to_Balanced;
    gCpmPmfTable.ApmfFn13Settings.P_delta_Balanced_to_Quiet = AmdPbsConfiguration.ApmfFn13_P_delta_Balanced_to_Quiet;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_APU_Perf_on_Lap = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_APU_Perf_on_Lap;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_HS2_Perf_on_Lap = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_HS2_Perf_on_Lap;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_APU_Perf = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_APU_Perf;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_HS2_Perf = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_HS2_Perf;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_APU_Balanced = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_APU_Balanced;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_HS2_Balanced = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_HS2_Balanced;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_APU_Quiet = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_APU_Quiet;
    gCpmPmfTable.ApmfFn13Settings.STT_SkinTempLimit_HS2_Quiet = AmdPbsConfiguration.ApmfFn13_STT_SkinTempLimit_HS2_Quiet;
    gCpmPmfTable.ApmfFn13Settings.STTMinLimit_Perf_on_lap = AmdPbsConfiguration.ApmfFn13_STTMinLimit_Perf_on_lap;
    gCpmPmfTable.ApmfFn13Settings.STTMinLimit_Perf = AmdPbsConfiguration.ApmfFn13_STTMinLimit_Perf;
    gCpmPmfTable.ApmfFn13Settings.STTMinLimit_Balanced = AmdPbsConfiguration.ApmfFn13_STTMinLimit_Balanced;
    gCpmPmfTable.ApmfFn13Settings.STTMinLimit_Quiet = AmdPbsConfiguration.ApmfFn13_STTMinLimit_Quiet;
    gCpmPmfTable.ApmfFn13Settings.FPPT_Perf_on_lap = AmdPbsConfiguration.ApmfFn13_FPPT_Perf_on_lap;
    gCpmPmfTable.ApmfFn13Settings.SPPT_Perf_on_lap = AmdPbsConfiguration.ApmfFn13_SPPT_Perf_on_lap;
    gCpmPmfTable.ApmfFn13Settings.SPL_Perf_on_lap = AmdPbsConfiguration.ApmfFn13_SPL_Perf_on_lap;
    gCpmPmfTable.ApmfFn13Settings.SPPT_APUOnly_Perf_on_Lap = AmdPbsConfiguration.ApmfFn13_SPPT_APUOnly_Perf_on_Lap;
    gCpmPmfTable.ApmfFn13Settings.FPPT_Perf = AmdPbsConfiguration.ApmfFn13_FPPT_Perf;
    gCpmPmfTable.ApmfFn13Settings.SPPT_Perf = AmdPbsConfiguration.ApmfFn13_SPPT_Perf;
    gCpmPmfTable.ApmfFn13Settings.SPL_Perf = AmdPbsConfiguration.ApmfFn13_SPL_Perf;
    gCpmPmfTable.ApmfFn13Settings.SPPT_APUOnly_Perf = AmdPbsConfiguration.ApmfFn13_SPPT_APUOnly_Perf;
    gCpmPmfTable.ApmfFn13Settings.FPPT_Balanced = AmdPbsConfiguration.ApmfFn13_FPPT_Balanced;
    gCpmPmfTable.ApmfFn13Settings.SPPT_Balanced = AmdPbsConfiguration.ApmfFn13_SPPT_Balanced;
    gCpmPmfTable.ApmfFn13Settings.SPL_Balanced = AmdPbsConfiguration.ApmfFn13_SPL_Balanced;
    gCpmPmfTable.ApmfFn13Settings.SPPT_APUOnly_Balanced = AmdPbsConfiguration.ApmfFn13_SPPT_APUOnly_Balanced;
    gCpmPmfTable.ApmfFn13Settings.FPPT_Quiet = AmdPbsConfiguration.ApmfFn13_FPPT_Quiet;
    gCpmPmfTable.ApmfFn13Settings.SPPT_Quiet = AmdPbsConfiguration.ApmfFn13_SPPT_Quiet;
    gCpmPmfTable.ApmfFn13Settings.SPL_Quiet = AmdPbsConfiguration.ApmfFn13_SPL_Quiet;
    gCpmPmfTable.ApmfFn13Settings.SPPT_APUOnly_Quiet = AmdPbsConfiguration.ApmfFn13_SPPT_APUOnly_Quiet;
    gCpmPmfTable.ApmfFn13Settings.Fan_ID_Perf = AmdPbsConfiguration.ApmfFn13_Fan_ID_Perf;
    gCpmPmfTable.ApmfFn13Settings.Fan_ID_Balanced = AmdPbsConfiguration.ApmfFn13_Fan_ID_Balanced;
    gCpmPmfTable.ApmfFn13Settings.Fan_ID_Quiet = AmdPbsConfiguration.ApmfFn13_Fan_ID_Quiet;
  }

  if (AmdPbsConfiguration.AmdPmfFn14_Enable) {
    gCpmPmfTable.SupportedFunctionsBitVector |= BIT13; //Set Function 14 support.
  }
  // Install CPM table.
  AmdCpmTablePpi->CommonFunction.AddTable (AmdCpmTablePpi, &gCpmPmfTable);

  DEBUG((DEBUG_INFO, "OEM-PEI-%a-Exit.\n", __FUNCTION__));
  return EFI_SUCCESS;
}
