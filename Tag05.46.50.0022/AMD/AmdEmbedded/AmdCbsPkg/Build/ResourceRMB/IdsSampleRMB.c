/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//This file is auto generated, don't edit it manually

//RedirectForReturnDis
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_GEN_W_A05, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdRedirectForReturnDis)) {
  case IDSOPT_CMN_CPU_GEN_W_A05_AUTO:
    break;
  case IDSOPT_CMN_CPU_GEN_W_A05_1:
    break;
  case IDSOPT_CMN_CPU_GEN_W_A05_0:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Core Performance Boost
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_CPB, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdCpbMode)) {
  case IDSOPT_CMN_CPU_CPB_DISABLED:
    break;
  case IDSOPT_CMN_CPU_CPB_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Global C-state Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_GLOBAL_CSTATE_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdCStateMode)) {
  case IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_DISABLED:
    break;
  case IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_ENABLED:
    break;
  case IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Opcache grayout flag
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_OPCACHE_GRAYOUT_FLAG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_0:
    break;
  case IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_1:
    break;
  case IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_DISPLAY:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Opcache Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_OPCACHE_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdOpcacheCtrl)) {
  case IDSOPT_CMN_CPU_OPCACHE_CTRL_DISABLED:
    break;
  case IDSOPT_CMN_CPU_OPCACHE_CTRL_ENABLED:
    break;
  case IDSOPT_CMN_CPU_OPCACHE_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SEV ASID Count
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_SEV_ASID_COUNT, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_SEV_ASID_COUNT_253ASIDS:
    break;
  case IDSOPT_CMN_CPU_SEV_ASID_COUNT_509ASIDS:
    break;
  case IDSOPT_CMN_CPU_SEV_ASID_COUNT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SEV-ES ASID Space Limit Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_SEV_ASID_SPACE_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_SEV_ASID_SPACE_CTRL_AUTO:
    break;
  case IDSOPT_CMN_CPU_SEV_ASID_SPACE_CTRL_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_SEV_ASID_SPACE_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_CPU_SEV_ASID_SPACE_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_CPU_SEV_ASID_SPACE_LIMIT_MIN);
}

//Streaming Stores Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_STREAMING_STORES_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdStreamingStoresCtrl)) {
  case IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_DISABLED:
    break;
  case IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_ENABLED:
    break;
  case IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Local APIC Mode
IDS_NV_READ_SKIP (IDSNVID_CPU_L_APIC_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdApicMode)) {
  case IDSOPT_CPU_L_APIC_MODE_COMPATIBILITY:
    break;
  case IDSOPT_CPU_L_APIC_MODE_XAPIC:
    break;
  case IDSOPT_CPU_L_APIC_MODE_X2APIC:
    break;
  case IDSOPT_CPU_L_APIC_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ACPI _CST C1 Declaration
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_CST_C1_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdAcpiCstC1)) {
  case IDSOPT_CMN_CPU_CST_C1_CTRL_DISABLED:
    break;
  case IDSOPT_CMN_CPU_CST_C1_CTRL_ENABLED:
    break;
  case IDSOPT_CMN_CPU_CST_C1_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ACPI _PSS
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_PM_CTL_PSS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdAcpiCpuSsdtPss)) {
  case IDSOPT_CMN_CPU_PM_CTL_PSS_DISABLED:
    break;
  case IDSOPT_CMN_CPU_PM_CTL_PSS_ENABLED:
    break;
  case IDSOPT_CMN_CPU_PM_CTL_PSS_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MCA error thresh enable
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_MCA_ERR_THRESH_EN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMcaErrThreshEn)) {
  case IDSOPT_CMN_CPU_MCA_ERR_THRESH_EN_FALSE:
    break;
  case IDSOPT_CMN_CPU_MCA_ERR_THRESH_EN_TRUE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_MCA_ERR_THRESH_COUNT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_CPU_MCA_ERR_THRESH_COUNT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_CPU_MCA_ERR_THRESH_COUNT_MIN);
}

//SMU and PSP Debug Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_SMU_PSP_DEBUG_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_DISABLED:
    break;
  case IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_ENABLED:
    break;
  case IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PPIN Opt-in
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_PPIN_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_PPIN_CTRL_DISABLED:
    break;
  case IDSOPT_CMN_CPU_PPIN_CTRL_ENABLED:
    break;
  case IDSOPT_CMN_CPU_PPIN_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Fast Short REP MOVSB
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_FSRM, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdEnableFSRM)) {
  case IDSOPT_CMN_CPU_FSRM_DISABLED:
    break;
  case IDSOPT_CMN_CPU_FSRM_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Enhanced REP MOVSB/STOSB
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_ERMS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdEnableERMS)) {
  case IDSOPT_CMN_CPU_ERMS_DISABLED:
    break;
  case IDSOPT_CMN_CPU_ERMS_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Action on BIST Failure
IDS_NV_READ_SKIP (IDSNVID_CMN_ACTION_ON_BIST_FAILURE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_ACTION_ON_BIST_FAILURE_DONOTHING:
    break;
  case IDSOPT_CMN_ACTION_ON_BIST_FAILURE_DOWNCORE:
    break;
  case IDSOPT_CMN_ACTION_ON_BIST_FAILURE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IBS hardware workaround
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_IBS_HARDWARE_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdIbsHardwareEn)) {
  case IDSOPT_CMN_CPU_IBS_HARDWARE_CTRL_AUTO:
    break;
  case IDSOPT_CMN_CPU_IBS_HARDWARE_CTRL_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//OC Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_OC_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CPU_OC_MODE_NORMALOPERATION:
    break;
  case IDSOPT_CMN_CPU_OC_MODE_OC1:
    break;
  case IDSOPT_CMN_CPU_OC_MODE_OC2:
    break;
  case IDSOPT_CMN_CPU_OC_MODE_OC3:
    break;
  case IDSOPT_CMN_CPU_OC_MODE_CUSTOMIZED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Custom Pstate0
IDS_NV_READ_SKIP (IDSNVID_CPU_PST_CUSTOM_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdCcxP0Setting)) {
  case IDSOPT_CPU_PST_CUSTOM_P0_CUSTOM:
    break;
  case IDSOPT_CPU_PST_CUSTOM_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CPU_PST0_FREQ, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CPU_PST0_FREQ_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CPU_PST0_FREQ_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CPU_PST0_VID, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CPU_PST0_VID_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CPU_PST0_VID_MIN);
}

//L1 Stream HW Prefetcher
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_L1_STREAM_HW_PREFETCHER, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdL1StreamPrefetcher)) {
  case IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_DISABLE:
    break;
  case IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_ENABLE:
    break;
  case IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//L2 Stream HW Prefetcher
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_L2_STREAM_HW_PREFETCHER, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdL2StreamPrefetcher)) {
  case IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_DISABLE:
    break;
  case IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_ENABLE:
    break;
  case IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Core Watchdog Timer Enable
IDS_NV_READ_SKIP (IDSNVID_DBG_CPU_GEN_CPU_WDT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdCpuWdtEn)) {
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_DISABLED:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_ENABLED:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Core Watchdog Timer Interval
IDS_NV_READ_SKIP (IDSNVID_DBG_CPU_GEN_CPU_WDT_TIMEOUT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet16 (PcdAmdCpuWdtTimeout)) {
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_21461S:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_10730S:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_5364S:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_2681S:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_1340S:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_66941MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_33405MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_16637MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_8253MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_4061MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_20970MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_10484MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_5241MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_2620MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_1309MS:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_65408US:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_3264US:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_16256US:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_8064US:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_3968US:
    break;
  case IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Core Watchdog Timer Severity
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_WDT_SEV, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdCpuWdtSeverity)) {
  case IDSOPT_CMN_CPU_WDT_SEV_NOERROR:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_TRANSPARENT:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_CORRECTED:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_DEFERRED:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_UNCORRECTED:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_FATAL:
    break;
  case IDSOPT_CMN_CPU_WDT_SEV_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Core control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_CORE_CTRL_RMB, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdDownCoreMode)) {
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_AUTO:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_ONE10:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_TWO20:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_THREE30:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_FOUR40:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_FIVE50:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_SIX60:
    break;
  case IDSOPT_CMN_CPU_CORE_CTRL_RMB_SEVEN70:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SMT Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CPU_SMT_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdSmtMode)) {
  case IDSOPT_CMN_CPU_SMT_CTRL_DISABLE:
    break;
  case IDSOPT_CMN_CPU_SMT_CTRL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_PERSISTENT_LOW, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_PERSISTENT_LOW_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_PERSISTENT_LOW_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_PERSISTENT_HIGH, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_PERSISTENT_HIGH_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_PERSISTENT_HIGH_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_PERSISTENT_SIZE_LOW, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_PERSISTENT_SIZE_LOW_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_PERSISTENT_SIZE_LOW_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_PERSISTENT_SIZE_HIGH, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_PERSISTENT_SIZE_HIGH_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_PERSISTENT_SIZE_HIGH_MIN);
}

//Persistent memory Retention
IDS_NV_READ_SKIP (IDSNVID_CMN_PERSISTENT_MEM_RETENTION, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPmemRetention)) {
  case IDSOPT_CMN_PERSISTENT_MEM_RETENTION_DISABLED:
    break;
  case IDSOPT_CMN_PERSISTENT_MEM_RETENTION_ENABLED:
    break;
  case IDSOPT_CMN_PERSISTENT_MEM_RETENTION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//CC6 memory region encryption
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_CC6_MEM_ENCRYPTION, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_DISABLED:
    break;
  case IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_ENABLED:
    break;
  case IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Memory Clear
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_MEM_CLEAR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_MEM_CLEAR_ENABLED:
    break;
  case IDSOPT_DF_CMN_MEM_CLEAR_DISABLED:
    break;
  case IDSOPT_DF_CMN_MEM_CLEAR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Disable DF to external downstream IP SyncFloodPropagation
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_EXT_IP_SYNC_FLOOD_PROP, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_SYNCFLOODDISABLED:
    break;
  case IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_SYNCFLOODENABLED:
    break;
  case IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Disable DF sync flood propagation
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_SYNC_FLOOD_PROP, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_SYNC_FLOOD_PROP_SYNCFLOODDISABLED:
    break;
  case IDSOPT_DF_CMN_SYNC_FLOOD_PROP_SYNCFLOODENABLED:
    break;
  case IDSOPT_DF_CMN_SYNC_FLOOD_PROP_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Freeze DF module queues on error
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_FREEZE_QUEUE_ERROR, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdFabricImmSyncFloodOnFatalErrCtrl)) {
  case IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_DISABLED:
    break;
  case IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_ENABLED:
    break;
  case IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DF Cstates
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_CSTATES, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdFabricCstate)) {
  case IDSOPT_DF_CMN_CSTATES_DISABLED:
    break;
  case IDSOPT_DF_CMN_CSTATES_ENABLED:
    break;
  case IDSOPT_DF_CMN_CSTATES_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MP exception recovery handling
IDS_NV_READ_SKIP (IDSNVID_MP_EXCEP_RECOV_HANDLE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_MP_EXCEP_RECOV_HANDLE_RECOVERYDISABLED:
    break;
  case IDSOPT_MP_EXCEP_RECOV_HANDLE_SYNCFLOOD:
    break;
  case IDSOPT_MP_EXCEP_RECOV_HANDLE_WARMRESET:
    break;
  case IDSOPT_MP_EXCEP_RECOV_HANDLE_COLDRESET:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Memory clear timeout
IDS_NV_READ_SKIP (IDSNVID_MEM_CLR_TIMEOUT, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_MEM_CLR_TIMEOUT_5SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_10SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_15SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_20SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_25SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_30SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_35SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_40SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_45SEC:
    break;
  case IDSOPT_MEM_CLR_TIMEOUT_50SEC:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Memory interleaving size
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_MEM_INTLV_SIZE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_MEM_INTLV_SIZE_256BYTES:
    break;
  case IDSOPT_DF_CMN_MEM_INTLV_SIZE_512BYTES:
    break;
  case IDSOPT_DF_CMN_MEM_INTLV_SIZE_1KB:
    break;
  case IDSOPT_DF_CMN_MEM_INTLV_SIZE_2KB:
    break;
  case IDSOPT_DF_CMN_MEM_INTLV_SIZE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM map inversion
IDS_NV_READ_SKIP (IDSNVID_DF_CMN_DRAM_MAP_INVERSION, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DF_CMN_DRAM_MAP_INVERSION_DISABLED:
    break;
  case IDSOPT_DF_CMN_DRAM_MAP_INVERSION_ENABLED:
    break;
  case IDSOPT_DF_CMN_DRAM_MAP_INVERSION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Active Memory Timing Settings
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_SETTING_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_SETTING_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_SETTING_DDR_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TARGET_SPEED_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TARGET_SPEED_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TARGET_SPEED_DDR_MIN);
}

//Tcl Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TCL_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TCL_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TCL_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TCL_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TCL_DDR_MIN);
}

//Trcd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRCD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRCD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRCD_DDR_MIN);
}

//Trp Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRP_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRP_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRP_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRP_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRP_DDR_MIN);
}

//Tras Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRAS_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRAS_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRAS_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRAS_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRAS_DDR_MIN);
}

//Trc Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRC_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRC_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRC_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRC_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRC_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRC_DDR_MIN);
}

//Twr Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWR_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWR_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWR_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWR_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWR_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWR_DDR_MIN);
}

//Trfc1 Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC1_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRFC1_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRFC1_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC1_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRFC1_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRFC1_DDR_MIN);
}

//Trfc2 Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC2_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRFC2_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRFC2_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC2_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRFC2_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRFC2_DDR_MIN);
}

//TrfcSb Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC_SB_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRFC_SB_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRFC_SB_DDR_MIN);
}

//Trtp Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRTP_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRTP_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRTP_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRTP_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRTP_DDR_MIN);
}

//TrrdL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_L_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_L_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRRD_L_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRRD_L_DDR_MIN);
}

//TrrdS Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_S_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_S_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRRD_S_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRRD_S_DDR_MIN);
}

//Tfaw Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TFAW_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TFAW_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TFAW_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TFAW_DDR_MIN);
}

//TwtrL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_L_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_L_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWTR_L_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWTR_L_DDR_MIN);
}

//TwtrS Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_S_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_S_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWTR_S_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWTR_S_DDR_MIN);
}

//TrdrdScL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_L_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_DDR_MIN);
}

//TrdrdSc Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_DDR_MIN);
}

//TrdrdSd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDRD_SD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDRD_SD_DDR_MIN);
}

//TrdrdDd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_DD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDRD_DD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDRD_DD_DDR_MIN);
}

//TwrwrScL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_L_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_DDR_MIN);
}

//TwrwrSc Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_DDR_MIN);
}

//TwrwrSd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRWR_SD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRWR_SD_DDR_MIN);
}

//TwrwrDd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_DD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRWR_DD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRWR_DD_DDR_MIN);
}

//Twrrd Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRRD_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRRD_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRRD_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRRD_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRRD_DDR_MIN);
}

//Trdwr Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDWR_CTRL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDWR_CTRL_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_CTRL_DDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDWR_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDWR_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDWR_DDR_MIN);
}

//Processor CA drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRV_STREN_CA_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor CS drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRV_STREN_CS_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor CK drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRV_STREN_CK_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor DQ drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRV_STREN_DQ_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_HIGHIMPEDANCE:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_240OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_120OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_80OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_60OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_48OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_40OHM:
    break;
  case IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_343OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ODT_IMPED_PROC_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_HIGHIMPEDANCE:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_480OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_240OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_160OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_120OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_96OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_80OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_686OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_60OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_533OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_48OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_436OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_40OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_369OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_343OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_32OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_30OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_282OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_267OHM:
    break;
  case IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_253OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram DQ drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_DRV_STREN_DQ_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_34OHM:
    break;
  case IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_40OHM:
    break;
  case IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_48OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram ODT impedance RTT_NOM_WR
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RTT_OFF:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ240:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ2120:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ380:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ460:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ548:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ640:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ734:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram ODT impedance RTT_NOM_RD
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RTT_OFF:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ240:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ2120:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ380:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ460:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ548:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ640:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ734:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram ODT impedance RTT_WR
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RTT_OFF:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ240:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ2120:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ380:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ460:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ548:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ640:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ734:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram ODT impedance RTT_PARK
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RTT_OFF:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ240:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ2120:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ380:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ460:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ548:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ640:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ734:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram ODT impedance DQS_RTT_PARK
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RTT_OFF:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ240:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ2120:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ380:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ460:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ548:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ640:
    break;
  case IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ734:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Power Down Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Phy Low Power Disable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_0:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_1:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Disable Memory Error Injection
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_FALSE:
    break;
  case IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_TRUE:
    break;
  case IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Data Poisoning
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DATA_POISONING_DDR4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdFabricEccScrubPoisoned)) {
  case IDSOPT_CMN_MEM_DATA_POISONING_DDR4_DISABLED:
    break;
  case IDSOPT_CMN_MEM_DATA_POISONING_DDR4_ENABLED:
    break;
  case IDSOPT_CMN_MEM_DATA_POISONING_DDR4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ECC
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ECC_EN_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ECC_EN_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_ECC_EN_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_ECC_EN_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM Redirect Scrubber Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdFabricEccScrubRedirection)) {
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM Scrub Redirection Limit
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdFabricEccScrubRedirectionLimit)) {
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_8SCRUBS:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_4SCRUBS:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_2SCRUBS:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_UNLIMITEDSCRUBS:
    break;
  case IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM Scrub Time
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_SCRUB_TIME, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdFabricDramScrubTime)) {
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_DISABLED:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_1HOUR:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_4HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_6HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_8HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_12HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_16HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_24HOURS:
    break;
  case IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_48HOURS:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TSME
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TSME_ENABLE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TSME_ENABLE_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TSME_ENABLE_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_TSME_ENABLE_DDR_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Data Scramble
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Enable SPD CRC verification
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_SPD_CRC_VERIFY, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_SPD_CRC_VERIFY_AUTO:
    break;
  case IDSOPT_CMN_MEM_SPD_CRC_VERIFY_ENABLE:
    break;
  case IDSOPT_CMN_MEM_SPD_CRC_VERIFY_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_SPD_READ_RETRY_COUNT_CRC, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_SPD_READ_RETRY_COUNT_CRC_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_SPD_READ_RETRY_COUNT_CRC_MIN);
}

//Chipselect Interleaving
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Address Hash Bank
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ADDRESS_HASH_BANK_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Address Hash CS
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ADDRESS_HASH_CS_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//BankSwapMode
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_SWAPAPU:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_RESTORE_VALID_DAYS, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_RESTORE_VALID_DAYS_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_RESTORE_VALID_DAYS_MIN);
}

//DFE Read Training
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_ENABLE:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM PDA Enumerate ID Programming Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_TOGGLINGPDAENUMERATIONMODE:
    break;
  case IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_LEGACYPDAENUMERATIONMODE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_EN_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_EN_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_EN_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_EN_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Test Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_TESTMODE_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_INTERFACEMODE:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_DATAEYEMODE:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_BOTH:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Aggressors
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_AGGRESSORS_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Per Bit Slave Die Reporting
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pattern Select
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_PATTERN_SELECT_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_PRBS:
    break;
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_SSO:
    break;
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_BOTH:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_PATTERN_LENGTH_DDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_DDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_DDR_MIN);
}

//Aggressor Channel
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_1AGGRESSORCHANNEL:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_3AGGRESSORCHANNELS:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_7AGGRESSORCHANNELS:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Active Memory Timing Settings
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_OVERCLOCK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_OVERCLOCK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_OVERCLOCK_LPDDR_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Maximum Memory Data Clock Speed
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_SPEED_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_SPEED_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_SPEED_LPDDR_3200MTS:
    break;
  case IDSOPT_CMN_MEM_SPEED_LPDDR_4267MTS:
    break;
  case IDSOPT_CMN_MEM_SPEED_LPDDR_5500MTS:
    break;
  case IDSOPT_CMN_MEM_SPEED_LPDDR_6400MTS:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trcpage Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCPAGE_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRCPAGE_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRCPAGE_LPDDR_MIN);
}

//Tcwl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TCWL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_14HCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Tcl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TCL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_20HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_21HCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trcdrd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCDRD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_1BHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trcdwr
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRCDWR_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0ACLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0BCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0CCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0DCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0ECLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0FCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_1BHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trp
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_1BHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trfc Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRFC_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRFC_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRFC_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRFC_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRFC_LPDDR_MIN);
}

//Tras
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRAS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_20HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_21HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_22HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_23HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_24HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_25HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_26HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_27HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_28HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_29HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_30HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_31HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_32HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_33HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_34HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_35HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_36HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_37HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_38HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_39HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_3AHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trc Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRC_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRC_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRC_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRC_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRC_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRC_LPDDR_MIN);
}

//TrrdS
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_S_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0CHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TrrdL
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRRD_L_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0CHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Tfaw Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TFAW_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TFAW_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TFAW_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TFAW_LPDDR_MIN);
}

//TwtrS
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_S_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0EHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TwtrL
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWTR_L_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0EHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Twr Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWR_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWR_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWR_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWR_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWR_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWR_LPDDR_MIN);
}

//TrdrdScL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR_MIN);
}

//TwrwrScL Ctrl
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR_MIN);
}

//Trtp
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRTP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0EHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Tcke
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TCKE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TrdrdSc
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SC_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TrdrdDd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_DD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TrdrdSd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDRD_SD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TwrwrSc
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SC_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TwrwrSd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_SD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TwrwrDd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRWR_DD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Twrrd
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TWRRD_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Trdwr
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TIMING_TRDWR_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_2CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_3CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_4CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_5CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_6CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_7CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_8CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_9CLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0FHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_10HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_11HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_12HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_13HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_14HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_15HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_16HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_17HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_18HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_19HCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1AHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1BHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1CHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1DHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1EHCLK:
    break;
  case IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1FHCLK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//LPDDR Refresh Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_REF_MODE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_REF_MODE_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_REF_MODE_LPDDR_ALLBANKS:
    break;
  case IDSOPT_CMN_MEM_REF_MODE_LPDDR_PERBANK:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//LPDDR RFM
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_RFM_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_RFM_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_RFM_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_RFM_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//WCK Always On
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//RRW Memory Test Control
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Power Down Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Phy Low Power Disable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_0:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_1:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//CAD Bus Timing User Controls
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ADDR_CMD_SETUP_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_ADDR_CMD_SETUP_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_ADDR_CMD_SETUP_LPDDR_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CS_ODT_SETUP_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_CS_ODT_SETUP_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_CS_ODT_SETUP_LPDDR_MIN);
}

//CA drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//CS drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_4000OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_1000OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_670OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_500OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//CK drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//WCK drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DQ drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DQS drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_300OHM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor CA ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_300OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor CK ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_300OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Processor WCK ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_1200OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_600OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_400OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_300OHM:
    break;
  case IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram CA ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_DISABLE:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ1:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ2:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ3:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ4:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ5:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ6:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram DQ ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_DISABLE:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ1:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ2:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ3:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ4:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ5:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ6:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram WCK ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_DISABLE:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ1:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ2:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ3:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ4:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ5:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ6:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram Non-Target ODT impedance
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_DISABLE:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ1:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ2:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ3:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ4:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ5:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ6:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Dram Pull-Down drive strengths
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DRAM_IO_DRV_STREN_PDDS, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_AUTO:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_DISABLE:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ1:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ2:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ3:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ4:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ5:
    break;
  case IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ6:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM Read Link ECC Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DRAM Write Link ECC Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Disable Memory Error Injection
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_FALSE:
    break;
  case IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_TRUE:
    break;
  case IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TSME
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_TSME_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_TSME_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_TSME_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_TSME_LPDDR_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Data Scramble
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Chip Select Interleaving
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Bank Swap
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//BankGroup Swap
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Address Hash Bank
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ADDRESS_HASH_BANK_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Address Hash CS
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_ADDRESS_HASH_CS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DFE Read Training
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_AUTO:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_ENABLE:
    break;
  case IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_EN_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_EN_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_EN_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_EN_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Test Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_TESTMODE_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_INTERFACEMODE:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_DATAEYEMODE:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_BOTH:
    break;
  case IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MBIST Aggressors
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_AGGRESSORS_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_ENABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pattern Select
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_PRBS:
    break;
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_SSO:
    break;
  case IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_BOTH:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_PATTERN_LENGTH_LPDDR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_LPDDR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_LPDDR_MIN);
}

//Aggressor Channel
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_DISABLED:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_1AGGRESSORCHANNEL:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_3AGGRESSORCHANNELS:
    break;
  case IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_7AGGRESSORCHANNELS:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Read Voltage Sweep Step Size
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_1:
    break;
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_2:
    break;
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_4:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Read Timing Sweep Step Size
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_1:
    break;
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_2:
    break;
  case IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_4:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Write Voltage Sweep Step Size
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_1:
    break;
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_2:
    break;
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_4:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Write Timing Sweep Step Size
IDS_NV_READ_SKIP (IDSNVID_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_1:
    break;
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_2:
    break;
  case IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_4:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOMMU
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOMMU, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgIommuSupport)) {
  case IDSOPT_CMN_GNB_NB_IOMMU_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DMAr Support
IDS_NV_READ_SKIP (IDSNVID_CMN_DMAR_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdIvInfoDmaReMap)) {
  case IDSOPT_CMN_DMAR_SUPPORT_AUTO:
    break;
  case IDSOPT_CMN_DMAR_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_DMAR_SUPPORT_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DMA Protection
IDS_NV_READ_SKIP (IDSNVID_CMN_DMA_PROTECTION, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdDmaProtection)) {
  case IDSOPT_CMN_DMA_PROTECTION_AUTO:
    break;
  case IDSOPT_CMN_DMA_PROTECTION_ENABLED:
    break;
  case IDSOPT_CMN_DMA_PROTECTION_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SCPC attribute control
IDS_NV_READ_SKIP (IDSNVID_CMN_SCPC_ATTR_CNTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SCPC_ATTR_CNTL_0:
    break;
  case IDSOPT_CMN_SCPC_ATTR_CNTL_1:
    break;
  case IDSOPT_CMN_SCPC_ATTR_CNTL_2:
    break;
  case IDSOPT_CMN_SCPC_ATTR_CNTL_3:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe ARI Support
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_PCIE_ARI_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgPcieAriSupport)) {
  case IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_DISABLED:
    break;
  case IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe ARI Enumeration
IDS_NV_READ_SKIP (IDSNVID_GNB_CMN_PCIE_ARI_ENUMERATION, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdPcieAriForwardingEnable)) {
  case IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_DISABLE:
    break;
  case IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_ENABLE:
    break;
  case IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Enable AER Cap
IDS_NV_READ_SKIP (IDSNVID_DBG_GNB_DBG_AERCAP_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgAEREnable)) {
  case IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_ENABLE:
    break;
  case IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_DISABLED:
    break;
  case IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ACS Enable
IDS_NV_READ_SKIP (IDSNVID_DBG_GNB_DBG_ACS_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgACSEnable)) {
  case IDSOPT_DBG_GNB_DBG_ACS_ENABLE_ENABLE:
    break;
  case IDSOPT_DBG_GNB_DBG_ACS_ENABLE_DISABLED:
    break;
  case IDSOPT_DBG_GNB_DBG_ACS_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PSPP Policy
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_PCIE_PSPP_POLICY, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPsppPolicy)) {
  case IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_DISABLED:
    break;
  case IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_BALANCED:
    break;
  case IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe All Ports Settings
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_PORT_ALL_PORTS, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_PORT_ALL_PORTS_AUTO:
    break;
  case IDSOPT_CMN_PCIE_PORT_ALL_PORTS_CUSTOM:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIO RAS Control
IDS_NV_READ_SKIP (IDSNVID_CMN_RAS_CONTROL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdNbioRASControlV2)) {
  case IDSOPT_CMN_RAS_CONTROL_DISABLED:
    break;
  case IDSOPT_CMN_RAS_CONTROL_MCA:
    break;
  case IDSOPT_CMN_RAS_CONTROL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_PCD_EGRESS_POISON_SEVERITY_HI, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_EGRESS_POISON_SEVERITY_HI_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_EGRESS_POISON_SEVERITY_HI_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_PCD_EGRESS_POISON_SEVERITY_LO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_EGRESS_POISON_SEVERITY_LO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_EGRESS_POISON_SEVERITY_LO_MIN);
}

//NBIO SyncFlood Generation
IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_SYNC_FLOOD_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdMaskNbioSyncFlood)) {
  case IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_ENABLED:
    break;
  case IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_DISABLED:
    break;
  case IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIO SyncFlood Reporting
IDS_NV_READ_SKIP (IDSNVID_PCD_SYNC_FLOOD_TO_APML, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSyncFloodToApml)) {
  case IDSOPT_PCD_SYNC_FLOOD_TO_APML_ENABLED:
    break;
  case IDSOPT_PCD_SYNC_FLOOD_TO_APML_DISABLED:
    break;
  case IDSOPT_PCD_SYNC_FLOOD_TO_APML_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_PCD_AMD_NBIO_EGRESS_POISON_MASK_HI, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_HI_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_HI_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_PCD_AMD_NBIO_EGRESS_POISON_MASK_LO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_LO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_LO_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_PCD_AMD_NBIO_RAS_UCP_MASK_HI, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_HI_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_HI_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_PCD_AMD_NBIO_RAS_UCP_MASK_LO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_LO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_LO_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_PCD_SYSHUB_WDT_TIMER_INTERVAL, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_PCD_SYSHUB_WDT_TIMER_INTERVAL_MAX);
  ASSERT (IdsNvValue >= IDSOPT_PCD_SYSHUB_WDT_TIMER_INTERVAL_MIN);
}

//NBIO Poison Consumption
IDS_NV_READ_SKIP (IDSNVID_DBG_POISON_CONSUMPTION, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdNbioPoisonConsumption)) {
  case IDSOPT_DBG_POISON_CONSUMPTION_ENABLED:
    break;
  case IDSOPT_DBG_POISON_CONSUMPTION_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//iGPU Configuration
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_UMA_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdCfgIgpuContorl)) {
  case IDSOPT_CMN_GNB_GFX_UMA_MODE_AUTO:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_MODE_IGPUDISABLED:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_SPECIFIED:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_AUTO:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_GAME_OPTIMIZED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//UMA Version
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_UMA_VERSION, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_GNB_GFX_UMA_VERSION_LEGACY:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_VERSION_NONLEGACY:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_VERSION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//UMA Frame buffer Size
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_AUTO:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_64M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_128M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_256M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_384M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_512M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_80M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_96M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_768M:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_1G:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_2G:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_3G:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_4G:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_8G:
    break;
  case IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_16G:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//GPU Host Translation Cache
IDS_NV_READ_SKIP (IDSNVID_CMN_GPU_HOST_TRANSLATION, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_GPU_HOST_TRANSLATION_DISABLED:
    break;
  case IDSOPT_CMN_GPU_HOST_TRANSLATION_ENABLED:
    break;
  case IDSOPT_CMN_GPU_HOST_TRANSLATION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NB Azalia
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_GFX_NB_AZALIA, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgAzaliaEnable)) {
  case IDSOPT_CMN_GNB_GFX_NB_AZALIA_DISABLED:
    break;
  case IDSOPT_CMN_GNB_GFX_NB_AZALIA_ENABLED:
    break;
  case IDSOPT_CMN_GNB_GFX_NB_AZALIA_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Audio IOs
IDS_NV_READ_SKIP (IDSNVID_DBG_GNB_AUDIO_I_OS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdNbioAudioSelect)) {
  case IDSOPT_DBG_GNB_AUDIO_I_OS_AUTO:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM2CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDIPDM6CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDISDW01MDATAPDM2CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATAPDM6CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM2CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_3I2SPORTS1REFCLK1INTR:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM6CHI2S:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM8CH:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDISDW01MDATAPDM6CHI2S:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM6CHI2S:
    break;
  case IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM8CH:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ASPM
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_ALL_PORTS_CAPASPM, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_L1SUPPORT:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//L1 PM SubState
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_AUTO:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_L11ONLY:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_L12ANDL11:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//LTR Support
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_ALL_PORTS_CAPLTR, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgPCIeLTREnable)) {
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_NOTSUPPORTED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_SUPPORTED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ASPM
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_L1:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//L1 PM SubState
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_ALL_PORTS_PM_L1_SS, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_AUTO:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_L11ONLY:
    break;
  case IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_L12ANDL11:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//LCLK Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_CORE_LCLK_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PCIE_CORE_LCLK_GATING_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_CORE_LCLK_GATING_ENABLED:
    break;
  case IDSOPT_CMN_PCIE_CORE_LCLK_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Dynamic PG
IDS_NV_READ_SKIP (IDSNVID_CMN_PCIE_CORE_DYNAMIC_POWER_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdGppPcieDynPortGating)) {
  case IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_DISABLED:
    break;
  case IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_ENABLED:
    break;
  case IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIO Global CG Override
IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_GLOBAL_CG_OVERRIDE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdNbioGlobalCgOverride)) {
  case IDSOPT_CMN_NBIO_GLOBAL_CG_OVERRIDE_AUTO:
    break;
  case IDSOPT_CMN_NBIO_GLOBAL_CG_OVERRIDE_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOHC LCLK Clock Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_PMM_IOHC_LCLK_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdIOHCClkGatingSupport)) {
  case IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_DISABLED:
    break;
  case IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_ENABLED:
    break;
  case IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOHC Power Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOHC_POWER_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdIOHCPgEnable)) {
  case IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOMMU L1 Clock Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdIommuL1ClockGatingEnable)) {
  case IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOMMU L2 Clock Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdIommuL2ClockGatingEnable)) {
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOMMU Dynamic Power Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOMMU_DYNAMIC_PG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgIOMMUDynamicPgEnable)) {
  case IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//IOMMU L2 Memory Power Gating
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_NB_IOMMU_L2_MEMORY_PG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgIOMMUL2MemoryPGEnable)) {
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_DISABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_ENABLED:
    break;
  case IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIF PG
IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_NBIF_PG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdCfgNbifPgClkGating)) {
  case IDSOPT_CMN_NBIO_NBIF_PG_AUTO:
    break;
  case IDSOPT_CMN_NBIO_NBIF_PG_ENABLED:
    break;
  case IDSOPT_CMN_NBIO_NBIF_PG_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SHUB PG
IDS_NV_READ_SKIP (IDSNVID_CMN_NBIO_SHUB_PG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSyshubPg)) {
  case IDSOPT_CMN_NBIO_SHUB_PG_AUTO:
    break;
  case IDSOPT_CMN_NBIO_SHUB_PG_ENABLED:
    break;
  case IDSOPT_CMN_NBIO_SHUB_PG_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MMHUB SRAM Light Sleep
IDS_NV_READ_SKIP (IDSNVID_CMN_PMM_MMHUB_SRAM_LS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdMmhubSramLightSleep)) {
  case IDSOPT_CMN_PMM_MMHUB_SRAM_LS_AUTO:
    break;
  case IDSOPT_CMN_PMM_MMHUB_SRAM_LS_DISABLED:
    break;
  case IDSOPT_CMN_PMM_MMHUB_SRAM_LS_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIF INTR OBFF
IDS_NV_READ_SKIP (IDSNVID_CMN_PMM_NBIF_INTR_OBFF, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdEnableNbifOBFF)) {
  case IDSOPT_CMN_PMM_NBIF_INTR_OBFF_AUTO:
    break;
  case IDSOPT_CMN_PMM_NBIF_INTR_OBFF_DISABLED:
    break;
  case IDSOPT_CMN_PMM_NBIF_INTR_OBFF_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//NBIF DMA OBFF
IDS_NV_READ_SKIP (IDSNVID_CMN_PMM_NBIF_DMA_OBFF, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdEnableNbifDmaOBFF)) {
  case IDSOPT_CMN_PMM_NBIF_DMA_OBFF_AUTO:
    break;
  case IDSOPT_CMN_PMM_NBIF_DMA_OBFF_DISABLED:
    break;
  case IDSOPT_CMN_PMM_NBIF_DMA_OBFF_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C/I2C 0 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C0_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C0_CONFIG_BOTHDISABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C0_CONFIG_I3CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C0_CONFIG_I2CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C0_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C 0 Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C0_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C0_MODE_I3C:
    break;
  case IDSOPT_CMN_FCH_I3_C0_MODE_I2C:
    break;
  case IDSOPT_CMN_FCH_I3_C0_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C/I2C 1 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C1_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C1_CONFIG_BOTHDISABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C1_CONFIG_I3CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C1_CONFIG_I2CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C1_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C 1 Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C1_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C1_MODE_I3C:
    break;
  case IDSOPT_CMN_FCH_I3_C1_MODE_I2C:
    break;
  case IDSOPT_CMN_FCH_I3_C1_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C/I2C 2 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C2_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C2_CONFIG_BOTHDISABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C2_CONFIG_I3CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C2_CONFIG_I2CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C2_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C 2 Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C2_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C2_MODE_I3C:
    break;
  case IDSOPT_CMN_FCH_I3_C2_MODE_I2C:
    break;
  case IDSOPT_CMN_FCH_I3_C2_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C/I2C 3 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C3_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C3_CONFIG_BOTHDISABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C3_CONFIG_I3CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C3_CONFIG_I2CENABLED:
    break;
  case IDSOPT_CMN_FCH_I3_C3_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//I3C 3 Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_I3_C3_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_I3_C3_MODE_I3C:
    break;
  case IDSOPT_CMN_FCH_I3_C3_MODE_I2C:
    break;
  case IDSOPT_CMN_FCH_I3_C3_MODE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Controller
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SATA_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataEnable)) {
  case IDSOPT_CMN_FCH_SATA_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Auto Shutdown
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SATA_AUTO_SHUT_DOWN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataControllerAutoShutdown)) {
  case IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_DISABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_ENABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SATA_CLASS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSataClass)) {
  case IDSOPT_CMN_FCH_SATA_CLASS_AHCI:
    break;
  case IDSOPT_CMN_FCH_SATA_CLASS_AHCIASID0X7904:
    break;
  case IDSOPT_CMN_FCH_SATA_CLASS_AUTO:
    break;
  case IDSOPT_CMN_FCH_SATA_CLASS_RAID:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Sata RAS Support
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SATA_RAS_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataRasSupport)) {
  case IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_DISABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Sata Disabled AHCI Prefetch Function
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataAhciDisPrefetchFunction)) {
  case IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_DISABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_ENABLED:
    break;
  case IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Aggresive SATA Device Sleep Port 0
IDS_NV_READ_SKIP (IDSNVID_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataDevSlpPort0)) {
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_DISABLE:
    break;
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_ENABLE:
    break;
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FCH_SATA_DEV_SLP_PORT0_NUM, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT0_NUM_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT0_NUM_MIN);
}

//Aggresive SATA Device Sleep Port 1
IDS_NV_READ_SKIP (IDSNVID_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdSataDevSlpPort1)) {
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_DISABLE:
    break;
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_ENABLE:
    break;
  case IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FCH_SATA_DEV_SLP_PORT1_NUM, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT1_NUM_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT1_NUM_MIN);
}

//USB0 controller enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdXhci0Enable)) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 controller enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdXhci1Enable)) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 controller enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I2_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdXhci2Enable)) {
  case IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 2.0 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 2.0 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 2.0 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT2, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 2.0 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_HS_PORT3, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 2.0 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 2.0 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 2.0 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_HS_PORT2, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 2.0 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I2_HS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 2.0 port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I3_HS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB4 2.0 port0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I4_HS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 3.1 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_SS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB0 3.1 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I0_SS_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 3.1 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_SS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB1 3.1 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I1_SS_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 3.1 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I3_SS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb40Enable)) {
  case IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB4 3.1 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_USB_XHC_I4_SS_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb41Enable)) {
  case IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_DISABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_ENABLED:
    break;
  case IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Ac Loss Control
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPwrFailShadow)) {
  case IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_ALWAYSOFF:
    break;
  case IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_ALWAYSON:
    break;
  case IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_RESERVED:
    break;
  case IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_PREVIOUS:
    break;
  case IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 0 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART0_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_UART0_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART0_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_UART0_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 0 Legacy Options
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART0_LEGACY_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (FchUart0LegacyEnable)) {
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X2E8:
    break;
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X2F8:
    break;
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X3E8:
    break;
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X3F8:
    break;
  case IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 1 Enable (no HW FC)
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART1_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_UART1_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART1_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_UART1_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 1 Legacy Options
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART1_LEGACY_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (FchUart1LegacyEnable)) {
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X2E8:
    break;
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X2F8:
    break;
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X3E8:
    break;
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X3F8:
    break;
  case IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 2 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART2_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_UART2_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART2_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_UART2_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 2 Legacy Options
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART2_LEGACY_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (FchUart2LegacyEnable)) {
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X2E8:
    break;
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X2F8:
    break;
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X3E8:
    break;
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X3F8:
    break;
  case IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 3 Enable (no HW FC)
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART3_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_UART3_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART3_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_UART3_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 3 Legacy Options
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART3_LEGACY_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (FchUart3LegacyEnable)) {
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X2E8:
    break;
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X2F8:
    break;
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X3E8:
    break;
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X3F8:
    break;
  case IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Uart 4 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_UART4_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_UART4_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_UART4_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_UART4_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ESPI Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_ESPI_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdEspiEnable)) {
  case IDSOPT_CMN_FCH_ESPI_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_ESPI_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_ESPI_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ESPI CRC Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_ESPI_CRC, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_ESPI_CRC_DISABLED:
    break;
  case IDSOPT_CMN_FCH_ESPI_CRC_ENABLED:
    break;
  case IDSOPT_CMN_FCH_ESPI_CRC_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//XGBE0 enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE0_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdXgbePort0ConfigEn)) {
  case IDSOPT_CMN_FCH_XGBE0_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_XGBE0_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_XGBE0_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//XGBE1 enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE1_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdXgbePort1ConfigEn)) {
  case IDSOPT_CMN_FCH_XGBE1_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_XGBE1_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_XGBE1_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE_PORT0_LED1_GPIO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_XGBE_PORT0_LED1_GPIO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_XGBE_PORT0_LED1_GPIO_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE_PORT0_LED2_GPIO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_XGBE_PORT0_LED2_GPIO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_XGBE_PORT0_LED2_GPIO_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE_PORT1_LED1_GPIO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_XGBE_PORT1_LED1_GPIO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_XGBE_PORT1_LED1_GPIO_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_XGBE_PORT1_LED2_GPIO, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_XGBE_PORT1_LED2_GPIO_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_XGBE_PORT1_LED2_GPIO_MIN);
}

//USB4 pre-CM Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_CIO_PRE_CM_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdUsb4CmEnable)) {
  case IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//RT0 Router Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_CIO_RT0_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb4Rt0En)) {
  case IDSOPT_CMN_FCH_CIO_RT0_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT0_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT0_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//RT0 PCIe Tunnel
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_CIO_RT0_PCIE_TUNNEL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb4Rt0PcieTnlEn)) {
  case IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_DISABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_ENABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//RT1 Router Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_CIO_RT1_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb4Rt1En)) {
  case IDSOPT_CMN_FCH_CIO_RT1_ENABLE_DISABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT1_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT1_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//RT1 PCIe Tunnel
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_CIO_RT1_PCIE_TUNNEL, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdUsb4Rt1PcieTnlEn)) {
  case IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_DISABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_ENABLED:
    break;
  case IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HFP Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HFP_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HFP_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_HFP_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_HFP_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HID Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_HID_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_HID_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HID Spi Read Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_READ_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID_SPI_READ_MODE_QUADIO144:
    break;
  case IDSOPT_CMN_FCH_HID_SPI_READ_MODE_DUALIO122:
    break;
  case IDSOPT_CMN_FCH_HID_SPI_READ_MODE_FASTREAD111:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HID Spi Speed
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPEED, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID_SPEED_133HMZ:
    break;
  case IDSOPT_CMN_FCH_HID_SPEED_222NMZ:
    break;
  case IDSOPT_CMN_FCH_HID_SPEED_3176MHZ:
    break;
  case IDSOPT_CMN_FCH_HID_SPEED_5800KHZ:
    break;
  case IDSOPT_CMN_FCH_HID_SPEED_SPI_SPD6:
    break;
  case IDSOPT_CMN_FCH_HID_SPEED_SPI_SPD7:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_SPEED6_N, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_HID_SPI_SPEED6_N_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_HID_SPI_SPEED6_N_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID_SPI_SPEED7_N, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_HID_SPI_SPEED7_N_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_HID_SPI_SPEED7_N_MIN);
}

//HID2 Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID2_CONFIG_DISABLED:
    break;
  case IDSOPT_CMN_FCH_HID2_CONFIG_ENABLED:
    break;
  case IDSOPT_CMN_FCH_HID2_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HID2 Spi Read Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_READ_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_QUADIO144:
    break;
  case IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_DUALIO122:
    break;
  case IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_FASTREAD111:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//HID2 Spi Speed
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPEED, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_FCH_HID2_SPEED_133HMZ:
    break;
  case IDSOPT_CMN_FCH_HID2_SPEED_222NMZ:
    break;
  case IDSOPT_CMN_FCH_HID2_SPEED_3176MHZ:
    break;
  case IDSOPT_CMN_FCH_HID2_SPEED_5800KHZ:
    break;
  case IDSOPT_CMN_FCH_HID2_SPEED_SPI_SPD6:
    break;
  case IDSOPT_CMN_FCH_HID2_SPEED_SPI_SPD7:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_SPEED6_N, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_HID2_SPI_SPEED6_N_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_HID2_SPI_SPEED6_N_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_HID2_SPI_SPEED7_N, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_FCH_HID2_SPI_SPEED7_N_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_FCH_HID2_SPI_SPEED7_N_MIN);
}

//Reset after sync flood
IDS_NV_READ_SKIP (IDSNVID_CMN_FCH_SYNCFLOOD_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdResetCpuOnSyncFlood)) {
  case IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_ENABLE:
    break;
  case IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_DISABLE:
    break;
  case IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//System Configuration
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_SMU_SYSTEM_CONFIG, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdCfgSystemConfiguration)) {
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_10W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_15W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_25W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_35W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_45W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_54W:
    break;
  case IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_SUSTAINED_POWER_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_SUSTAINED_POWER_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_SUSTAINED_POWER_LIMIT_MIN);
}

//PPT Control
IDS_NV_READ_SKIP (IDSNVID_DBG_PPT_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_PPT_CTL_MANUAL:
    break;
  case IDSOPT_DBG_PPT_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAST_PPT_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAST_PPT_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAST_PPT_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_SLOW_PPT_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_SLOW_PPT_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_SLOW_PPT_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_SLOW_PPT_TIME_CONSTANT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_SLOW_PPT_TIME_CONSTANT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_SLOW_PPT_TIME_CONSTANT_MIN);
}

//STAPM Control
IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_STAPM_CTL_MANUAL:
    break;
  case IDSOPT_DBG_STAPM_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//System Temperature Tracking
IDS_NV_READ_SKIP (IDSNVID_DBG_SYSTEM_TEMPERATURE_TRACKING, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSttEnable)) {
  case IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_AUTO:
    break;
  case IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_1:
    break;
  case IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_0:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//STAPM Boost Override
IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_BOOST_OVERRIDE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_AUTO:
    break;
  case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_0:
    break;
  case IDSOPT_DBG_STAPM_BOOST_OVERRIDE_1:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//STAPM Boost
IDS_NV_READ_SKIP (IDSNVID_DBG_STAPM_BOOST, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_STAPM_BOOST_AUTO:
    break;
  case IDSOPT_DBG_STAPM_BOOST_1:
    break;
  case IDSOPT_DBG_STAPM_BOOST_0:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TSKIN_TIME_CONSTANT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TSKIN_TIME_CONSTANT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TSKIN_TIME_CONSTANT_MIN);
}

//Thermal Control
IDS_NV_READ_SKIP (IDSNVID_DBG_THM_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_THM_CTL_MANUAL:
    break;
  case IDSOPT_DBG_THM_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TJ_MAX_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TJ_MAX_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TJ_MAX_LIMIT_MIN);
}

//TDC Control
IDS_NV_READ_SKIP (IDSNVID_DBG_TDC_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_TDC_CTL_MANUAL:
    break;
  case IDSOPT_DBG_TDC_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TDCVDD_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TDCVDD_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TDCVDD_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TDCSOC_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TDCSOC_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TDCSOC_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TDCGFX_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TDCGFX_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TDCGFX_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_TDCCVIP_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_TDCCVIP_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_TDCCVIP_LIMIT_MIN);
}

//EDC Control
IDS_NV_READ_SKIP (IDSNVID_DBG_EDC_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_EDC_CTL_MANUAL:
    break;
  case IDSOPT_DBG_EDC_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_EDCVDD_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_EDCVDD_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_EDCVDD_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_EDCSOC_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_EDCSOC_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_EDCSOC_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_EDCGFX_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_EDCGFX_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_EDCGFX_LIMIT_MIN);
}

//PSI3 Control
IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_PS_I3_CTL_MANUAL:
    break;
  case IDSOPT_DBG_PS_I3_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_VDD_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_PS_I3_VDD_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_PS_I3_VDD_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_PS_I3_GFX_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_PS_I3_GFX_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_PS_I3_GFX_LIMIT_MIN);
}

//PROCHOT Control
IDS_NV_READ_SKIP (IDSNVID_DBG_PROCHOT_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_PROCHOT_CTL_MANUAL:
    break;
  case IDSOPT_DBG_PROCHOT_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_PROCHOT_RAMP_TIME, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_PROCHOT_RAMP_TIME_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_PROCHOT_RAMP_TIME_MIN);
}

//STT Control
IDS_NV_READ_SKIP (IDSNVID_DBG_STT_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_STT_CTL_MANUAL:
    break;
  case IDSOPT_DBG_STT_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_PCB_SENSOR_COUNT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_PCB_SENSOR_COUNT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_PCB_SENSOR_COUNT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_MIN_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_MIN_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_MIN_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M1, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M1_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M1_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M2, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M2_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M2_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M3, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M3_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M3_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M4, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M4_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M4_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M5, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M5_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M5_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_M6, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_M6_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_M6_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STTCAPU, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STTCAPU_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STTCAPU_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STTC_HS2, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STTC_HS2_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STTC_HS2_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_ALPHA_APU, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_ALPHA_APU_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_ALPHA_APU_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_ALPHA_H_S2, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_ALPHA_H_S2_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_ALPHA_H_S2_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_SKIN_TEMPERATURE_LIMIT_APU, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_APU_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_APU_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_SKIN_TEMPERATURE_LIMIT_HS2, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_HS2_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_HS2_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_ERROR_COEFF, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_ERROR_COEFF_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_ERROR_COEFF_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_STT_ERROR_RATE_COEFF, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_STT_ERROR_RATE_COEFF_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_STT_ERROR_RATE_COEFF_MIN);
}

//Fan Control
IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_FAN_CTL_MANUAL:
    break;
  case IDSOPT_DBG_FAN_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Force PWM Control
IDS_NV_READ_SKIP (IDSNVID_DBG_FORCE_PWM_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_FORCE_PWM_CTL_FORCE:
    break;
  case IDSOPT_DBG_FORCE_PWM_CTL_UNFORCE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FORCE_PWM, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FORCE_PWM_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FORCE_PWM_MIN);
}

//Fan Table Control
IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_FAN_TABLE_CTL_MANUAL:
    break;
  case IDSOPT_DBG_FAN_TABLE_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_LOW, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_TEMP_LOW_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_TEMP_LOW_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_MED, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_TEMP_MED_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_TEMP_MED_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_HIGH, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_TEMP_HIGH_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_TEMP_HIGH_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_TEMP_CRITICAL, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_TEMP_CRITICAL_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_TEMP_CRITICAL_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_T_PWM_LOW, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_T_PWM_LOW_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_T_PWM_LOW_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_MED, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_PWM_MED_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_PWM_MED_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_HIGH, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_PWM_HIGH_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_PWM_HIGH_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_HYST, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_FAN_TABLE_HYST_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_FAN_TABLE_HYST_MIN);
}

//Pwm Frequency
IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_TABLE_PWM_FREQ, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_FAN_TABLE_PWM_FREQ_AUTO:
    break;
  case IDSOPT_DBG_FAN_TABLE_PWM_FREQ_1:
    break;
  case IDSOPT_DBG_FAN_TABLE_PWM_FREQ_0:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Fan polarity
IDS_NV_READ_SKIP (IDSNVID_DBG_FAN_POLARITY, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_FAN_POLARITY_AUTO:
    break;
  case IDSOPT_DBG_FAN_POLARITY_1:
    break;
  case IDSOPT_DBG_FAN_POLARITY_0:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//VDDP Voltage Control
IDS_NV_READ_SKIP (IDSNVID_DBG_VDDP_VOLTAGE_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_VDDP_VOLTAGE_CTL_MANUAL:
    break;
  case IDSOPT_DBG_VDDP_VOLTAGE_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_VDDP_VOLTAGE, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_VDDP_VOLTAGE_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_VDDP_VOLTAGE_MIN);
}

//VDDM Voltage Control
IDS_NV_READ_SKIP (IDSNVID_DBG_VDDM_VOLTAGE_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_DBG_VDDM_VOLTAGE_CTL_MANUAL:
    break;
  case IDSOPT_DBG_VDDM_VOLTAGE_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_DBG_VDDM_VOLTAGE, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_DBG_VDDM_VOLTAGE_MAX);
  ASSERT (IdsNvValue >= IDSOPT_DBG_VDDM_VOLTAGE_MIN);
}

//Zstates (Z9 and Z10)
IDS_NV_READ_SKIP (IDSNVID_CMN_GNB_SMU_ZSTATES, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdSmuZStateControlEnable)) {
  case IDSOPT_CMN_GNB_SMU_ZSTATES_DISABLED:
    break;
  case IDSOPT_CMN_GNB_SMU_ZSTATES_ENABLED:
    break;
  case IDSOPT_CMN_GNB_SMU_ZSTATES_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//CclkFmaxOverride Control
IDS_NV_READ_SKIP (IDSNVID_CMN_CCLK_FMAX_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_CCLK_FMAX_CTL_MANUAL:
    break;
  case IDSOPT_CMN_CCLK_FMAX_CTL_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_CCLK_FMAX_OVERRIDE, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_CCLK_FMAX_OVERRIDE_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_CCLK_FMAX_OVERRIDE_MIN);
}

//DF Pstate P0 Ctl
IDS_NV_READ_SKIP (IDSNVID_CMN_DF_PSTATE_P0_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_DF_PSTATE_P0_CTL_AUTO:
    break;
  case IDSOPT_CMN_DF_PSTATE_P0_CTL_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DF Pstate Fclk P0
IDS_NV_READ_SKIP (IDSNVID_CMN_DF_PSTATE_FCLK_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_333MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_400MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_500MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_533MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_667MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_733MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_800MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_933MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1067MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1200MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1300MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1375MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1466MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1600MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_1800MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_FCLK_P0_2000MHZ:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//DF Pstate MemClk P0
IDS_NV_READ_SKIP (IDSNVID_CMN_DF_PSTATE_MEM_CLK_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_167MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_200MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_267MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_333MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_367MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_400MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_467MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_533MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_600MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_687MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_733MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_800MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_933MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1000MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1600MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1800MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2000MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2400MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2600MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2800MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_3000MHZ:
    break;
  case IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_3200MHZ:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Fixed FCLK
IDS_NV_READ_SKIP (IDSNVID_CMN_FCLK_VALUE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdFclkValue)) {
  case IDSOPT_CMN_FCLK_VALUE_LEVEL0:
    break;
  case IDSOPT_CMN_FCLK_VALUE_LEVEL1:
    break;
  case IDSOPT_CMN_FCLK_VALUE_LEVEL2:
    break;
  case IDSOPT_CMN_FCLK_VALUE_LEVEL3:
    break;
  case IDSOPT_CMN_FCLK_VALUE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Fixed LCLK
IDS_NV_READ_SKIP (IDSNVID_CMN_LCLK_FREQ, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdLclkValue)) {
  case IDSOPT_CMN_LCLK_FREQ_LEVEL0:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL1:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL2:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL3:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL4:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL5:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL6:
    break;
  case IDSOPT_CMN_LCLK_FREQ_LEVEL7:
    break;
  case IDSOPT_CMN_LCLK_FREQ_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Reset on MP1 SMN Timeout
IDS_NV_READ_SKIP (IDSNVID_CMN_RST_ON_MP1_SMN_TIMEOUT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdResetOnMP1SMNTimeout)) {
  case IDSOPT_CMN_RST_ON_MP1_SMN_TIMEOUT_DISABLED:
    break;
  case IDSOPT_CMN_RST_ON_MP1_SMN_TIMEOUT_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SmartShift Control
IDS_NV_READ_SKIP (IDSNVID_CMN_AA_CTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_AA_CTL_AUTO:
    break;
  case IDSOPT_CMN_AA_CTL_MANUAL:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SmartShift Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_AA_EN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdCfgPeApmEnable)) {
  case IDSOPT_CMN_AA_EN_AUTO:
    break;
  case IDSOPT_CMN_AA_EN_DISABLE:
    break;
  case IDSOPT_CMN_AA_EN_ENABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_APU_ONLY_SPPT_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_APU_ONLY_SPPT_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_APU_ONLY_SPPT_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MSG_SET_SUSTAINED_POWER_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MSG_SET_SUSTAINED_POWER_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MSG_SET_SUSTAINED_POWER_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MSG_SET_FAST_PPT_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MSG_SET_FAST_PPT_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MSG_SET_FAST_PPT_LIMIT_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_MSG_SE_SLOW_PPT_LIMIT, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_MSG_SE_SLOW_PPT_LIMIT_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_MSG_SE_SLOW_PPT_LIMIT_MIN);
}

//PSP RPMC Switch
IDS_NV_READ_SKIP (IDSNVID_CMN_PSP_RPMC_SWITCH, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_PSP_RPMC_SWITCH_AUTO:
    break;
  case IDSOPT_CMN_PSP_RPMC_SWITCH_DISABLED:
    break;
  case IDSOPT_CMN_PSP_RPMC_SWITCH_ENABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Rom Armor support
IDS_NV_READ_SKIP (IDSNVID_CMN_ROM_ARMOR_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdAmdPspRomArmorSelection)) {
  case IDSOPT_CMN_ROM_ARMOR_SUPPORT_AUTO:
    break;
  case IDSOPT_CMN_ROM_ARMOR_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_ROM_ARMOR_SUPPORT_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Rom Armor Spi Config Reg Lock
IDS_NV_READ_SKIP (IDSNVID_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspRomArmorSpiConfigRegLock)) {
  case IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_AUTO:
    break;
  case IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_ENABLED:
    break;
  case IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ABL Console Out Control
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_ABL_CON_OUT, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_ABL_CON_OUT_AUTO:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_ENABLE:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_DISABLE:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ABL Console Out Serial Port
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_ABL_CON_OUT_SERIAL_PORT, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_AUTO:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_LPCUART:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART0:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART1:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART2:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART3:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_ESPIUART:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ABL Console Out Serial Port IO
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X3F8:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X2F8:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X3E8:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X2E8:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ABL Console Out Boot Mode Select
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_ABL_CON_OUT_BOOT_MODE, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_ALL:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_S3S0I3ONLY:
    break;
  case IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_NORMALBOOTONLY:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//ABL PMU message Control
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_ABL_PMU_MSG_CTRL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_AUTO:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_MAXIMALDEBUGMESSAGES:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_DETAILEDDEBUGMESSAGE:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_COARSEDEBUGMESSAGE:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_STAGECOMPLETION:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_ASSERTIONMESSAGE:
    break;
  case IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_FIRMWARECOMPLETIONMESSAGEONLY:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Secured-core Auto enablement
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_SCPC_AUTO_ENABLEMENT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdScpcAutoEnablement)) {
  case IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_AUTO:
    break;
  case IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_ENABLED:
    break;
  case IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Mixed DIMM config extended NUMA domain
IDS_NV_READ_SKIP (IDSNVID_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdFabricMixedDimmConfigNumaDomain)) {
  case IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_DISABLED:
    break;
  case IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_ENABLED:
    break;
  case IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//FAR enforcement state
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_FAR_ENFORCED, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_FAR_ENFORCED_ENABLED:
    break;
  case IDSOPT_CMN_SOC_FAR_ENFORCED_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_SPL_FUSE, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_SOC_SPL_FUSE_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_SOC_SPL_FUSE_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_SPL_VALUE_IN_TBL, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_SOC_SPL_VALUE_IN_TBL_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_SOC_SPL_VALUE_IN_TBL_MIN);
}

IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_INIT_SPL_FOR_FAR, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_SOC_INIT_SPL_FOR_FAR_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_SOC_INIT_SPL_FOR_FAR_MIN);
}

//FAR Switch
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_FAR_SWITCH, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspAntiRollbackLateSplFuse)) {
  case IDSOPT_CMN_SOC_FAR_SWITCH_ENABLED:
    break;
  case IDSOPT_CMN_SOC_FAR_SWITCH_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) X86 Firmware Support
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_X86_FW_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdHspCoreEnable)) {
  case IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) UART
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_HSP_UART, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdHspUartEnable)) {
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) UART Port
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_HSP_UART_PORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdHspUartPort)) {
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART0:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART1:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART2:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART3:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART4:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART5:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART6:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART7:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) FIPS
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_HSP_FIPS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdHspFipsEnable)) {
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) FIPS zeroization
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Pluton (HSP) ACPI Fence Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdHspAcpiFenceEnable)) {
  case IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_FALSE:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_TRUE:
    break;
  case IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//AGESA MPM support
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_MPM_SUPPORT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMpmEnable)) {
  case IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//KVM for Wired Manageability
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_WIRED_KVM, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMpmWiredKvmEnable)) {
  case IDSOPT_CMN_SO_C_MISC_WIRED_KVM_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRED_KVM_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRED_KVM_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Wireless Manageability
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMpmWirelessManageability)) {
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//KVM for Wireless Manageability
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_WIRELESS_KVM, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMpmWirelessKvmEnable)) {
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//MPM attribute control
IDS_NV_READ_SKIP (IDSNVID_CMN_SOC_MISC_MPM_ATTR_CNTL, &IdsNvValue) {
  switch (IdsNvValue) {
  case IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_0:
    break;
  case IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_1:
    break;
  case IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_2:
    break;
  case IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_3:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//TCR for Wireless Manageability
IDS_NV_READ_SKIP (IDSNVID_CMN_SO_C_MISC_WIRELESS_TCR, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdMpmWirelessTcrEnable)) {
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_DISABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_ENABLED:
    break;
  case IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

IDS_NV_READ_SKIP (IDSNVID_CMN_INTRUSION_DETECTION_CTL, &IdsNvValue) {
  ASSERT (IdsNvValue <= IDSOPT_CMN_INTRUSION_DETECTION_CTL_MAX);
  ASSERT (IdsNvValue >= IDSOPT_CMN_INTRUSION_DETECTION_CTL_MIN);
}

//Intrusion Detection Enable
IDS_NV_READ_SKIP (IDSNVID_CMN_INTRUSION_DETECTION_ENABLE, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspIntrusionDetectionEnabled)) {
  case IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_AUTO:
    break;
  case IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_ENABLED:
    break;
  case IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Log Intrusion Event
IDS_NV_READ_SKIP (IDSNVID_CMN_INTRUSION_LOG_EVENT, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspLogEventIfIntrusionDetected)) {
  case IDSOPT_CMN_INTRUSION_LOG_EVENT_ENABLED:
    break;
  case IDSOPT_CMN_INTRUSION_LOG_EVENT_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Clear TPM
IDS_NV_READ_SKIP (IDSNVID_CMN_INTRUSION_CLEAR_TPM, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspClearTpmIfIntrusionDetected)) {
  case IDSOPT_CMN_INTRUSION_CLEAR_TPM_ENABLED:
    break;
  case IDSOPT_CMN_INTRUSION_CLEAR_TPM_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Power Off System
IDS_NV_READ_SKIP (IDSNVID_CMN_INTRUSION_POWER_OFF, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGetBool (PcdAmdPspPowerOffSystemIfIntrusionDetected)) {
  case IDSOPT_CMN_INTRUSION_POWER_OFF_ENABLED:
    break;
  case IDSOPT_CMN_INTRUSION_POWER_OFF_DISABLED:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort0Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P0_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort1Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort2Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort3Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort4Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P4_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P4_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort5Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P5_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P5_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort6Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P6_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P6_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort7Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P7_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P7_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort8Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P8_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P8_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort9Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P9_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P9_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 10
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P10, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort10Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P10_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P10_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P10_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 11
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P11, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort11Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P11_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P11_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P11_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 12
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P12, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort12Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P12_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P12_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P12_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 13
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_PCIE_P13, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21PciePort13Enable)) {
  case IDSOPT_CMN_P_T21_PCIE_P13_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P13_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_PCIE_P13_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_SATA_CLASS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataMode)) {
  case IDSOPT_CMN_P_T21_SATA_CLASS_AHCI:
    break;
  case IDSOPT_CMN_P_T21_SATA_CLASS_RAID:
    break;
  case IDSOPT_CMN_P_T21_SATA_CLASS_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_SATA_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataPort0Enable)) {
  case IDSOPT_CMN_P_T21_SATA_PORT0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT0_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_SATA_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataPort1Enable)) {
  case IDSOPT_CMN_P_T21_SATA_PORT1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_SATA_PORT2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataPort2Enable)) {
  case IDSOPT_CMN_P_T21_SATA_PORT2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_SATA_PORT3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataPort3Enable)) {
  case IDSOPT_CMN_P_T21_SATA_PORT3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_SATA_PORT3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 0
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataAggressiveDevSlpP0)) {
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_DISABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_ENABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 1
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataAggressiveDevSlpP1)) {
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_DISABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_ENABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 2
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataAggressiveDevSlpP2)) {
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_DISABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_ENABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 3
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21SataAggressiveDevSlpP3)) {
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_DISABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_ENABLE:
    break;
  case IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P0_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP0Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P0_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P0_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P0_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P1_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP1Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P1_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P1_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P1_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P2_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP2Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P2_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P2_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P2_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P3_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP3Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P3_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P3_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P3_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 4
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P4_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP4Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P4_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P4_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P4_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 5
IDS_NV_READ_SKIP (IDSNVID_DBG_P_T21_XHCI_P5_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21XhciP5Gen)) {
  case IDSOPT_DBG_P_T21_XHCI_P5_GEN_GEN15G:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P5_GEN_GEN2:
    break;
  case IDSOPT_DBG_P_T21_XHCI_P5_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P0)) {
  case IDSOPT_CMN_P_T21_USB3_P0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P0_GEN2X1:
    break;
  case IDSOPT_CMN_P_T21_USB3_P0_GEN2X2:
    break;
  case IDSOPT_CMN_P_T21_USB3_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P1)) {
  case IDSOPT_CMN_P_T21_USB3_P1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P2)) {
  case IDSOPT_CMN_P_T21_USB3_P2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P3)) {
  case IDSOPT_CMN_P_T21_USB3_P3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P4)) {
  case IDSOPT_CMN_P_T21_USB3_P4_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P4_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB3_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb3P5)) {
  case IDSOPT_CMN_P_T21_USB3_P5_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P5_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB3_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P0)) {
  case IDSOPT_CMN_P_T21_USB2_P0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P0_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P1)) {
  case IDSOPT_CMN_P_T21_USB2_P1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P2)) {
  case IDSOPT_CMN_P_T21_USB2_P2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P3)) {
  case IDSOPT_CMN_P_T21_USB2_P3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P4)) {
  case IDSOPT_CMN_P_T21_USB2_P4_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P4_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P5)) {
  case IDSOPT_CMN_P_T21_USB2_P5_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P5_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P6)) {
  case IDSOPT_CMN_P_T21_USB2_P6_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P6_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P7)) {
  case IDSOPT_CMN_P_T21_USB2_P7_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P7_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P8)) {
  case IDSOPT_CMN_P_T21_USB2_P8_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P8_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P9)) {
  case IDSOPT_CMN_P_T21_USB2_P9_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P9_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 10
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P10, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P10)) {
  case IDSOPT_CMN_P_T21_USB2_P10_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P10_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P10_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 11
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_USB2_P11, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21Usb2P11)) {
  case IDSOPT_CMN_P_T21_USB2_P11_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P11_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_USB2_P11_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB3_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb3P0)) {
  case IDSOPT_CMN_P_T21_L4_USB3_P0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P0_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB3_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb3P1)) {
  case IDSOPT_CMN_P_T21_L4_USB3_P1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB3_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb3P2)) {
  case IDSOPT_CMN_P_T21_L4_USB3_P2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB3_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb3P3)) {
  case IDSOPT_CMN_P_T21_L4_USB3_P3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB3_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P0)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P0_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P0_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P1)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P1_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P1_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P2)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P2_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P2_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P3)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P3_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P3_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P4)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P4_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P4_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P5)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P5_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P5_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P6)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P6_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P6_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P7)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P7_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P7_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P8)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P8_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P8_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_P_T21_L4_USB2_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdPT21L4Usb2P9)) {
  case IDSOPT_CMN_P_T21_L4_USB2_P9_DISABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P9_ENABLE:
    break;
  case IDSOPT_CMN_P_T21_L4_USB2_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort0Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P0_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort1Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort2Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort3Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort4Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P4_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P4_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort5Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P5_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P5_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort6Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P6_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P6_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort7Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P7_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P7_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort8Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P8_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P8_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort9Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P9_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P9_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 10
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P10, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort10Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P10_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P10_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P10_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 11
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P11, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort11Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P11_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P11_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P11_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 12
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P12, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort12Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P12_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P12_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P12_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//PCIe Port 13
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_PCIE_P13, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21PciePort13Enable)) {
  case IDSOPT_CMN_SEC_P_T21_PCIE_P13_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P13_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_PCIE_P13_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Mode
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_SATA_CLASS, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataMode)) {
  case IDSOPT_CMN_SEC_P_T21_SATA_CLASS_AHCI:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_CLASS_RAID:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_CLASS_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_SATA_PORT0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataPort0Enable)) {
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT0_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_SATA_PORT1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataPort1Enable)) {
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_SATA_PORT2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataPort2Enable)) {
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//SATA Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_SATA_PORT3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataPort3Enable)) {
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_SATA_PORT3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 0
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP0)) {
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_DISABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_ENABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 1
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP1)) {
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_DISABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_ENABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 2
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP2)) {
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_DISABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_ENABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//Device Sleep for AHCI Port 3
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP3)) {
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_DISABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_ENABLE:
    break;
  case IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P0_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP0Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P1_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP1Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P2_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP2Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P3_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP3Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 4
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P4_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP4Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 5
IDS_NV_READ_SKIP (IDSNVID_DBG_SEC_P_T21_XHCI_P5_GEN, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21XhciP5Gen)) {
  case IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_GEN15G:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_GEN2:
    break;
  case IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P0)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P0_GEN2X1:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P0_GEN2X2:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P1)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P2)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P3)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P4)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P4_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P4_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB3_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb3P5)) {
  case IDSOPT_CMN_SEC_P_T21_USB3_P5_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P5_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB3_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P0)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P0_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P1)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P2)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P3)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P4)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P4_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P4_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P5)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P5_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P5_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P6)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P6_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P6_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P7)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P7_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P7_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P8)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P8_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P8_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P9)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P9_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P9_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 10
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P10, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P10)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P10_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P10_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P10_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 11
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_USB2_P11, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21Usb2P11)) {
  case IDSOPT_CMN_SEC_P_T21_USB2_P11_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P11_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_USB2_P11_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB3_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb3P0)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB3_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb3P1)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB3_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb3P2)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB3 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB3_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb3P3)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 0
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P0, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P0)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 1
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P1, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P1)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 2
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P2, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P2)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 3
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P3, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P3)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 4
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P4, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P4)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 5
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P5, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P5)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 6
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P6, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P6)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 7
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P7, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P7)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 8
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P8, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P8)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

//USB2 Port 9
IDS_NV_READ_SKIP (IDSNVID_CMN_SEC_P_T21_L4_USB2_P9, &IdsNvValue) {
  switch (IdsNvValue) {
  switch (PcdGet8 (PcdSecPT21L4Usb2P9)) {
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_DISABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_ENABLE:
    break;
  case IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_AUTO:
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}

