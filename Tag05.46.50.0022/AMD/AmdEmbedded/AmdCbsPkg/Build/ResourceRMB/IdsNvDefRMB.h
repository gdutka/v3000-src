/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//This file is auto generated, don't edit it manually

#ifndef _IDS_NV_DEF_RMB_H_
#define _IDS_NV_DEF_RMB_H_

///RedirectForReturnDis
///From a workaround for GCC/C000005 issue for XV Core on CZ A0, setting MSRC001_1029 Decode Configuration (DE_CFG) bit 14 [DecfgNoRdrctForReturns] to 1
typedef enum {
  IDSOPT_CMN_CPU_GEN_W_A05_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_CPU_GEN_W_A05_1 = 1,///<1
  IDSOPT_CMN_CPU_GEN_W_A05_0 = 0,///<0
} IDSOPT_CMN_CPU_GEN_W_A05;

///Core Performance Boost
///Disable CPB
typedef enum {
  IDSOPT_CMN_CPU_CPB_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_CPB_AUTO = 1,///<Auto
} IDSOPT_CMN_CPU_CPB;

///Global C-state Control
///Controls IO based C-state generation and DF C-states.\nThere is another DF Cstate option which will be synchronized with this option if DF Cstate option is auto.
typedef enum {
  IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_ENABLED = 1,///<Enabled
  IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_GLOBAL_CSTATE_CTRL;

///Opcache grayout flag
typedef enum {
  IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_0 = 0,///<0
  IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_1 = 1,///<1
  IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG_DISPLAY = 2,///<Display
} IDSOPT_CMN_CPU_OPCACHE_GRAYOUT_FLAG;

///Opcache Control
///Enables or disables the Opcache
typedef enum {
  IDSOPT_CMN_CPU_OPCACHE_CTRL_DISABLED = 1,///<Disabled
  IDSOPT_CMN_CPU_OPCACHE_CTRL_ENABLED = 0,///<Enabled
  IDSOPT_CMN_CPU_OPCACHE_CTRL_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_CPU_OPCACHE_CTRL;

///SEV ASID Count
///This fields specifies the maximum valid ASID, which affects the maximum system physical address space. 16TB of physical address space is available for systems that support 253 ASIDs, while 8TB of physical address space is available for systems that support 509 ASIDs.
typedef enum {
  IDSOPT_CMN_CPU_SEV_ASID_COUNT_253ASIDS = 0,///<253 ASIDs
  IDSOPT_CMN_CPU_SEV_ASID_COUNT_509ASIDS = 1,///<509 ASIDs
  IDSOPT_CMN_CPU_SEV_ASID_COUNT_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_SEV_ASID_COUNT;

///SEV-ES ASID Space Limit Control
typedef enum {
  IDSOPT_CMN_CPU_SEV_ASID_SPACE_CTRL_AUTO = 0,///<Auto
  IDSOPT_CMN_CPU_SEV_ASID_SPACE_CTRL_MANUAL = 1,///<Manual
} IDSOPT_CMN_CPU_SEV_ASID_SPACE_CTRL;

///SEV-ES ASID Space Limit
///SEV VMs using ASIDs below the SEV-ES ASID Space Limit must enable the SEV-ES feature. ASIDs from SEV-ES ASID Space Limit to (SEV ASID Count + 1) can only be used with SEV VMs. If this field is set to (SEV ASID Count + 1), all ASIDs are forced to be SEV-ES ASIDs. Hence, the valid values for this field is 1 - (SEV ASID Count + 1)
#define IDSOPT_CMN_CPU_SEV_ASID_SPACE_LIMIT_MIN 1 ///< Min of SEV-ES ASID Space Limit
#define IDSOPT_CMN_CPU_SEV_ASID_SPACE_LIMIT_MAX 510 ///< Max of SEV-ES ASID Space Limit

///Streaming Stores Control
///Enables or disables the streaming stores functionality
typedef enum {
  IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_DISABLED = 1,///<Disabled
  IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_ENABLED = 0,///<Enabled
  IDSOPT_CMN_CPU_STREAMING_STORES_CTRL_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_CPU_STREAMING_STORES_CTRL;

///Local APIC Mode
typedef enum {
  IDSOPT_CPU_L_APIC_MODE_COMPATIBILITY = 0,///<Compatibility
  IDSOPT_CPU_L_APIC_MODE_XAPIC = 1,///<xAPIC
  IDSOPT_CPU_L_APIC_MODE_X2APIC = 2,///<x2APIC
  IDSOPT_CPU_L_APIC_MODE_AUTO = 0xFF,///<Auto
} IDSOPT_CPU_L_APIC_MODE;

///ACPI _CST C1 Declaration
///Determines whether or not to declare the C1 state to the OS.
typedef enum {
  IDSOPT_CMN_CPU_CST_C1_CTRL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_CST_C1_CTRL_ENABLED = 1,///<Enabled
  IDSOPT_CMN_CPU_CST_C1_CTRL_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_CST_C1_CTRL;

///ACPI _PSS
///Enable / disable the P-state object declaration to the OS.
typedef enum {
  IDSOPT_CMN_CPU_PM_CTL_PSS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_PM_CTL_PSS_ENABLED = 1,///<Enabled
  IDSOPT_CMN_CPU_PM_CTL_PSS_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_PM_CTL_PSS;

///MCA error thresh enable
///Enable MCA error thresholding.
typedef enum {
  IDSOPT_CMN_CPU_MCA_ERR_THRESH_EN_FALSE = 0,///<False
  IDSOPT_CMN_CPU_MCA_ERR_THRESH_EN_TRUE = 1,///<True
} IDSOPT_CMN_CPU_MCA_ERR_THRESH_EN;

///MCA error thresh count
///Effective error threshold count = 4095(0xFFF) - <this value> (e.g. the default value of 0xFF5 results in a threshold of 10).
#define IDSOPT_CMN_CPU_MCA_ERR_THRESH_COUNT_MIN 1 ///< Min of MCA error thresh count
#define IDSOPT_CMN_CPU_MCA_ERR_THRESH_COUNT_MAX 4095 ///< Max of MCA error thresh count

///SMU and PSP Debug Mode
///When this option is enabled, specific uncorrected errors detected by the PSP FW or SMU FW will hang and not reset the system
typedef enum {
  IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_SMU_PSP_DEBUG_MODE;

///PPIN Opt-in
///Turn on PPIN feature
typedef enum {
  IDSOPT_CMN_CPU_PPIN_CTRL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_PPIN_CTRL_ENABLED = 1,///<Enabled
  IDSOPT_CMN_CPU_PPIN_CTRL_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_CPU_PPIN_CTRL;

///Fast Short REP MOVSB
typedef enum {
  IDSOPT_CMN_CPU_FSRM_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_FSRM_ENABLED = 1,///<Enabled
} IDSOPT_CMN_CPU_FSRM;

///Enhanced REP MOVSB/STOSB
typedef enum {
  IDSOPT_CMN_CPU_ERMS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_CPU_ERMS_ENABLED = 1,///<Enabled
} IDSOPT_CMN_CPU_ERMS;

///Action on BIST Failure
///Action to take when a Core BIST failure is detected.
typedef enum {
  IDSOPT_CMN_ACTION_ON_BIST_FAILURE_DONOTHING = 0,///<Do nothing
  IDSOPT_CMN_ACTION_ON_BIST_FAILURE_DOWNCORE = 1,///<Down-Core
  IDSOPT_CMN_ACTION_ON_BIST_FAILURE_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_ACTION_ON_BIST_FAILURE;

///IBS hardware workaround
///Set if using IBS execution sampling without software workaround for erratum 1,285. May impact performance.
typedef enum {
  IDSOPT_CMN_CPU_IBS_HARDWARE_CTRL_AUTO = 0,///<Auto
  IDSOPT_CMN_CPU_IBS_HARDWARE_CTRL_ENABLED = 1,///<Enabled
} IDSOPT_CMN_CPU_IBS_HARDWARE_CTRL;

///OC Mode
typedef enum {
  IDSOPT_CMN_CPU_OC_MODE_NORMALOPERATION = 0,///<Normal Operation
  IDSOPT_CMN_CPU_OC_MODE_OC1 = 1,///<OC1
  IDSOPT_CMN_CPU_OC_MODE_OC2 = 2,///<OC2
  IDSOPT_CMN_CPU_OC_MODE_OC3 = 3,///<OC3
  IDSOPT_CMN_CPU_OC_MODE_CUSTOMIZED = 5,///<Customized
} IDSOPT_CMN_CPU_OC_MODE;

///Pstates Disclaimer
///Legal Disclaimer
///Pstates Disclaimer 1
///Legal Disclaimer
///Custom Pstate0
///Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE\n\nWARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S WARRANTY.\nOperating your AMD processor outside of specification or in excess of factory settings, including but not limited to overclocking, may damage or shorten the life of your processor or other system components, create system instabilities (e.g., data loss and corrupted images) and in extreme cases may result in total system failure. AMD does not provide support or service for issues or damages related to use of an AMD processor outside of processor specifications or in excess of factory settings.\n
typedef enum {
  IDSOPT_CPU_PST_CUSTOM_P0_CUSTOM = 1,///<Custom
  IDSOPT_CPU_PST_CUSTOM_P0_AUTO = 2,///<Auto
} IDSOPT_CPU_PST_CUSTOM_P0;

///Pstate0 Freq (MHz)
///Specifies core frequency (MHz)
#define IDSOPT_CPU_PST0_FREQ_MIN 0 ///< Min of Pstate0 Freq (MHz)
#define IDSOPT_CPU_PST0_FREQ_MAX 0xffffffff ///< Max of Pstate0 Freq (MHz)

///Pstate0 VID
///Specifies the core voltage.
#define IDSOPT_CPU_PST0_VID_MIN 0 ///< Min of Pstate0 VID
#define IDSOPT_CPU_PST0_VID_MAX 0xff ///< Max of Pstate0 VID

///L1 Stream HW Prefetcher
///Option to Enable | Disable L1 Stream HW Prefetcher
typedef enum {
  IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_DISABLE = 0,///<Disable
  IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_ENABLE = 1,///<Enable
  IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_L1_STREAM_HW_PREFETCHER;

///L2 Stream HW Prefetcher
///Option to Enable | Disable L2 Stream HW Prefetcher
typedef enum {
  IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_DISABLE = 0,///<Disable
  IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_ENABLE = 1,///<Enable
  IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER_AUTO = 3,///<Auto
} IDSOPT_CMN_CPU_L2_STREAM_HW_PREFETCHER;

///Core Watchdog Timer Enable
///Enable or disable CPU Watchdog Timer
typedef enum {
  IDSOPT_DBG_CPU_GEN_CPU_WDT_DISABLED = 0,///<Disabled
  IDSOPT_DBG_CPU_GEN_CPU_WDT_ENABLED = 1,///<Enabled
  IDSOPT_DBG_CPU_GEN_CPU_WDT_AUTO = 3,///<Auto
} IDSOPT_DBG_CPU_GEN_CPU_WDT;

///Core Watchdog Timer Interval
typedef enum {
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_21461S = 0x900,///<21.461s
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_10730S = 0x800,///<10.730s
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_5364S = 0x000,///<5.364s
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_2681S = 0x100,///<2.681s
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_1340S = 0x200,///<1.340s
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_66941MS = 0x300,///<669.41ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_33405MS = 0x400,///<334.05ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_16637MS = 0x500,///<166.37ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_8253MS = 0x600,///<82.53ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_4061MS = 0x700,///<40.61ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_20970MS = 0x901,///<20.970ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_10484MS = 0x801,///<10.484ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_5241MS = 0x001,///<5.241ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_2620MS = 0x101,///<2.620ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_1309MS = 0x201,///<1.309ms
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_65408US = 0x301,///<654.08us
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_3264US = 0x401,///<326.4us
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_16256US = 0x501,///<162.56us
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_8064US = 0x601,///<80.64us
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_3968US = 0x701,///<39.68us
  IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT_AUTO = 0xFFFF,///<Auto
} IDSOPT_DBG_CPU_GEN_CPU_WDT_TIMEOUT;

///Core Watchdog Timer Severity
///Specify the CPU watch dog timer severity (MSRC001_0074[CpuWdTmrCfgSeverity]).
typedef enum {
  IDSOPT_CMN_CPU_WDT_SEV_NOERROR = 0,///<No Error
  IDSOPT_CMN_CPU_WDT_SEV_TRANSPARENT = 1,///<Transparent
  IDSOPT_CMN_CPU_WDT_SEV_CORRECTED = 2,///<Corrected
  IDSOPT_CMN_CPU_WDT_SEV_DEFERRED = 3,///<Deferred
  IDSOPT_CMN_CPU_WDT_SEV_UNCORRECTED = 4,///<Uncorrected
  IDSOPT_CMN_CPU_WDT_SEV_FATAL = 5,///<Fatal
  IDSOPT_CMN_CPU_WDT_SEV_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_CPU_WDT_SEV;

///Core control
///Sets the number of cores to be used. Once this option has been used to remove any cores, a POWER CYCLE is required in order for future selections to take effect.
typedef enum {
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_AUTO = 0,///<Auto
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_ONE10 = 1,///<ONE (1 + 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_TWO20 = 3,///<TWO (2 + 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_THREE30 = 4,///<THREE (3 + 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_FOUR40 = 6,///<FOUR (4 + 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_FIVE50 = 8,///<FIVE (5 + 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_SIX60 = 9,///<SIX (6+ 0)
  IDSOPT_CMN_CPU_CORE_CTRL_RMB_SEVEN70 = 10,///<SEVEN  (7 + 0)
} IDSOPT_CMN_CPU_CORE_CTRL_RMB;

///SMT Control
///Can be used to disable symmetric multithreading. To re-enable SMT, a POWER CYCLE is needed after selecting the "Auto" option.
typedef enum {
  IDSOPT_CMN_CPU_SMT_CTRL_DISABLE = 0,///<Disable
  IDSOPT_CMN_CPU_SMT_CTRL_AUTO = 0x1,///<Auto
} IDSOPT_CMN_CPU_SMT_CTRL;

///Persistent memory Base Low
///The persistent memory Base address lower 32 bits.It must be multiple of 1 MB.
#define IDSOPT_CMN_PERSISTENT_LOW_MIN 0 ///< Min of Persistent memory Base Low
#define IDSOPT_CMN_PERSISTENT_LOW_MAX 0xffffffff ///< Max of Persistent memory Base Low

///Persistent memory Base High
///The persistent memory Base address higher 32 bits.
#define IDSOPT_CMN_PERSISTENT_HIGH_MIN 0 ///< Min of Persistent memory Base High
#define IDSOPT_CMN_PERSISTENT_HIGH_MAX 0xffffffff ///< Max of Persistent memory Base High

///Persistent memory reserved size Low
///To reserve the persistent memory size of lower 32 bits.
#define IDSOPT_CMN_PERSISTENT_SIZE_LOW_MIN 0 ///< Min of Persistent memory reserved size Low
#define IDSOPT_CMN_PERSISTENT_SIZE_LOW_MAX 0xffffffff ///< Max of Persistent memory reserved size Low

///Persistent memory reserved size High
///To reserve the persistent memory size of higher 32bits.
#define IDSOPT_CMN_PERSISTENT_SIZE_HIGH_MIN 0 ///< Min of Persistent memory reserved size High
#define IDSOPT_CMN_PERSISTENT_SIZE_HIGH_MAX 0xffffffff ///< Max of Persistent memory reserved size High

///Persistent memory Retention
///To retain or clear Persistent memory during warmboot.\nAuto - Default(clear/OS call)\nEnabled - Send command to retain\nDisbaled - Send Command to clear      \n
typedef enum {
  IDSOPT_CMN_PERSISTENT_MEM_RETENTION_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PERSISTENT_MEM_RETENTION_ENABLED = 1,///<Enabled
  IDSOPT_CMN_PERSISTENT_MEM_RETENTION_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_PERSISTENT_MEM_RETENTION;

///CC6 memory region encryption
///Control whether or not the CC6 save/restore memory is encrypted
typedef enum {
  IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_DISABLED = 0,///<Disabled
  IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_ENABLED = 1,///<Enabled
  IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION_AUTO = 3,///<Auto
} IDSOPT_DF_CMN_CC6_MEM_ENCRYPTION;

///Memory Clear
///When this feature is disabled, BIOS does not implement MemClear after memory training (only if non-ECC DIMMs are used).
typedef enum {
  IDSOPT_DF_CMN_MEM_CLEAR_ENABLED = 0,///<Enabled
  IDSOPT_DF_CMN_MEM_CLEAR_DISABLED = 1,///<Disabled
  IDSOPT_DF_CMN_MEM_CLEAR_AUTO = 3,///<Auto
} IDSOPT_DF_CMN_MEM_CLEAR;

///Disable DF to external downstream IP SyncFloodPropagation
///Disables Error propagation to UMC or any downstream slaves eg. FCH. Use this to avoid reset in failure scenario
typedef enum {
  IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_SYNCFLOODDISABLED = 1,///<Sync flood disabled
  IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_SYNCFLOODENABLED = 0,///<Sync flood enabled
  IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP_AUTO = 0xFF,///<Auto
} IDSOPT_DF_CMN_EXT_IP_SYNC_FLOOD_PROP;

///Disable DF sync flood propagation
///Control DF::PIEConfig[DisSyncFloodProp]\nDisables propagation from PIE to other DF components and eventually to SDP ports
typedef enum {
  IDSOPT_DF_CMN_SYNC_FLOOD_PROP_SYNCFLOODDISABLED = 1,///<Sync flood disabled
  IDSOPT_DF_CMN_SYNC_FLOOD_PROP_SYNCFLOODENABLED = 0,///<Sync flood enabled
  IDSOPT_DF_CMN_SYNC_FLOOD_PROP_AUTO = 3,///<Auto
} IDSOPT_DF_CMN_SYNC_FLOOD_PROP;

///Freeze DF module queues on error
///Disabling this option sets DF:DfGlobalCtrl[DisImmSyncFloodOnFatalError]\nEnables freezing of all DF queues on error and also forces a sync flood on HWA even if MCAs are disabled
typedef enum {
  IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_DISABLED = 0,///<Disabled
  IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_ENABLED = 1,///<Enabled
  IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR_AUTO = 3,///<Auto
} IDSOPT_DF_CMN_FREEZE_QUEUE_ERROR;

///DF Cstates
///When DF Cstate feature is enabled, FW programs the registers required to enable this feature is the DF HW. (For auto option, it means this option will synchronized with Global C State.)
typedef enum {
  IDSOPT_DF_CMN_CSTATES_DISABLED = 0,///<Disabled
  IDSOPT_DF_CMN_CSTATES_ENABLED = 1,///<Enabled
  IDSOPT_DF_CMN_CSTATES_AUTO = 0xFF,///<Auto
} IDSOPT_DF_CMN_CSTATES;

///MP exception recovery handling
///Recovery handling method incase of MP exception
typedef enum {
  IDSOPT_MP_EXCEP_RECOV_HANDLE_RECOVERYDISABLED = 0,///<Recovery Disabled
  IDSOPT_MP_EXCEP_RECOV_HANDLE_SYNCFLOOD = 1,///<SyncFlood
  IDSOPT_MP_EXCEP_RECOV_HANDLE_WARMRESET = 2,///<Warm reset
  IDSOPT_MP_EXCEP_RECOV_HANDLE_COLDRESET = 3,///<Cold reset
} IDSOPT_MP_EXCEP_RECOV_HANDLE;

///Memory clear timeout
///Configure time out on memory clear in seconds
typedef enum {
  IDSOPT_MEM_CLR_TIMEOUT_5SEC = 5,///<5 sec
  IDSOPT_MEM_CLR_TIMEOUT_10SEC = 10,///<10 sec
  IDSOPT_MEM_CLR_TIMEOUT_15SEC = 15,///<15 sec
  IDSOPT_MEM_CLR_TIMEOUT_20SEC = 20,///<20 sec
  IDSOPT_MEM_CLR_TIMEOUT_25SEC = 25,///<25 sec
  IDSOPT_MEM_CLR_TIMEOUT_30SEC = 30,///<30 sec
  IDSOPT_MEM_CLR_TIMEOUT_35SEC = 35,///<35 sec
  IDSOPT_MEM_CLR_TIMEOUT_40SEC = 40,///<40 sec
  IDSOPT_MEM_CLR_TIMEOUT_45SEC = 45,///<45 sec
  IDSOPT_MEM_CLR_TIMEOUT_50SEC = 50,///<50 sec
} IDSOPT_MEM_CLR_TIMEOUT;

///Memory interleaving size
///Controls the memory interleaving size. The valid values are AUTO, 256 bytes, 512 bytes, 1 Kbytes or 2Kbytes. This determines the starting address of the interleave (bit 8, 9, 10 or 11).
typedef enum {
  IDSOPT_DF_CMN_MEM_INTLV_SIZE_256BYTES = 0,///<256 Bytes
  IDSOPT_DF_CMN_MEM_INTLV_SIZE_512BYTES = 1,///<512 Bytes
  IDSOPT_DF_CMN_MEM_INTLV_SIZE_1KB = 2,///<1 KB
  IDSOPT_DF_CMN_MEM_INTLV_SIZE_2KB = 3,///<2 KB
  IDSOPT_DF_CMN_MEM_INTLV_SIZE_AUTO = 7,///<Auto
} IDSOPT_DF_CMN_MEM_INTLV_SIZE;

///DRAM map inversion
///Inverting the map will cause the highest memory channels to get assigned the lowest addresses in the system.
typedef enum {
  IDSOPT_DF_CMN_DRAM_MAP_INVERSION_DISABLED = 0,///<Disabled
  IDSOPT_DF_CMN_DRAM_MAP_INVERSION_ENABLED = 1,///<Enabled
  IDSOPT_DF_CMN_DRAM_MAP_INVERSION_AUTO = 3,///<Auto
} IDSOPT_DF_CMN_DRAM_MAP_INVERSION;

///DRAM Timing Configuration Legal Disclaimer
///DRAM Timing Configuration Legal Disclaimer 1
///Active Memory Timing Settings
///Active Memory Timing Settings
typedef enum {
  IDSOPT_CMN_MEM_TIMING_SETTING_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_SETTING_DDR_ENABLED = 1,///<Enabled
} IDSOPT_CMN_MEM_TIMING_SETTING_DDR;

///Memory Target Speed
///Specifies the memory target speed in MT/s. The valid input is 2000 MT/s, 2400MT/s, and range of 3200MT/s ~ 12000 MT/s(stepping of 200 MT/s). The Value is in decimal.\nThe user input value will be rounded down to align with the stepping of 200MT/s.\nThe Maximum speed defined in the JEDEC spec is 12000MT/s, any input value that is greater than 12000MT/s will be limited to 12000MT/s.
#define IDSOPT_CMN_MEM_TARGET_SPEED_DDR_MIN 2000 ///< Min of Memory Target Speed
#define IDSOPT_CMN_MEM_TARGET_SPEED_DDR_MAX 12000 ///< Max of Memory Target Speed

///Tcl Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TCL_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TCL_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TCL_CTRL_DDR;

///Tcl
///Specifies the CAS Latency. Valid values: 0x16 ~ 0x40, stepping of 2. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TCL_DDR_MIN 0x16 ///< Min of Tcl
#define IDSOPT_CMN_MEM_TIMING_TCL_DDR_MAX 0x40 ///< Max of Tcl

///Trcd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRCD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRCD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRCD_CTRL_DDR;

///Trcd
///Specifies the RAS# Active to CAS# Read Delay Time. Valid values: 0x8 ~ 0x3E, stepping of 2. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRCD_DDR_MIN 0x8 ///< Min of Trcd
#define IDSOPT_CMN_MEM_TIMING_TRCD_DDR_MAX 0x3E ///< Max of Trcd

///Trp Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRP_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRP_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRP_CTRL_DDR;

///Trp
///Specifies Row Precharge Delay Time. Valid values: 0x8 ~ 0x3E, stepping of 2. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRP_DDR_MIN 0x8 ///< Min of Trp
#define IDSOPT_CMN_MEM_TIMING_TRP_DDR_MAX 0x3E ///< Max of Trp

///Tras Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRAS_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRAS_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRAS_CTRL_DDR;

///Tras
///Specifies the Active to Precharge Delay Time. Valid values: 0x1E ~ 0x7E stepping of 2
#define IDSOPT_CMN_MEM_TIMING_TRAS_DDR_MIN 0x1E ///< Min of Tras
#define IDSOPT_CMN_MEM_TIMING_TRAS_DDR_MAX 0x7E ///< Max of Tras

///Trc Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRC_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRC_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRC_CTRL_DDR;

///Trc
///Specifies Active to Active/Refresh Delay Time. Valid values: 0x20 ~ 0xFF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRC_DDR_MIN 0x20 ///< Min of Trc
#define IDSOPT_CMN_MEM_TIMING_TRC_DDR_MAX 0xFF ///< Max of Trc

///Twr Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWR_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWR_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWR_CTRL_DDR;

///Twr
///Specifies the Minimum Write Recovery Time. Valid values: 0x30 ~ 0x60, stepping of 2. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWR_DDR_MIN 0x30 ///< Min of Twr
#define IDSOPT_CMN_MEM_TIMING_TWR_DDR_MAX 0x60 ///< Max of Twr

///Trfc1 Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRFC1_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRFC1_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRFC1_CTRL_DDR;

///Trfc1
///Specifies the Refresh Recovery Delay Time (tRFC1). Valid values 0x32 ~ 0xFFF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRFC1_DDR_MIN 0x32 ///< Min of Trfc1
#define IDSOPT_CMN_MEM_TIMING_TRFC1_DDR_MAX 0xFFF ///< Max of Trfc1

///Trfc2 Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRFC2_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRFC2_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRFC2_CTRL_DDR;

///Trfc2
///Specifies the Refresh Recovery Delay Time (tRFC2).  Valid values: 0x32 ~ 0xFFF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRFC2_DDR_MIN 0x32 ///< Min of Trfc2
#define IDSOPT_CMN_MEM_TIMING_TRFC2_DDR_MAX 0xFFF ///< Max of Trfc2

///TrfcSb Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRFC_SB_CTRL_DDR;

///TrfcSb
///Specifies the Refresh Recovery Delay Time (tRFCSb). Valid values 0x32 ~ 0x7FF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRFC_SB_DDR_MIN 0x32 ///< Min of TrfcSb
#define IDSOPT_CMN_MEM_TIMING_TRFC_SB_DDR_MAX 0x7FF ///< Max of TrfcSb

///Trtp Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRTP_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRTP_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRTP_CTRL_DDR;

///Trtp
///Specifies the Read CAS# to Precharge command delay time. Valid values: 0x5 ~ 0x1F. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRTP_DDR_MIN 0x5 ///< Min of Trtp
#define IDSOPT_CMN_MEM_TIMING_TRTP_DDR_MAX 0x1F ///< Max of Trtp

///TrrdL Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRRD_L_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRRD_L_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRRD_L_CTRL_DDR;

///TrrdL
///Specifies the Activate to Activate Delay Time, same bank group(tRRD_L). Valid values: 0x4 ~ 0x20. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRRD_L_DDR_MIN 0x4 ///< Min of TrrdL
#define IDSOPT_CMN_MEM_TIMING_TRRD_L_DDR_MAX 0x20 ///< Max of TrrdL

///TrrdS Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRRD_S_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRRD_S_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRRD_S_CTRL_DDR;

///TrrdS
///Specifies the Activate to Activate Delay Time, different bank group(tRRD_S). Valid values: 0x4 ~ 0x14. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRRD_S_DDR_MIN 0x4 ///< Min of TrrdS
#define IDSOPT_CMN_MEM_TIMING_TRRD_S_DDR_MAX 0x14 ///< Max of TrrdS

///Tfaw Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_DDR;

///Tfaw
///Specifies the Four Activate Window Time. Valid values: 0x14 ~ 0x50. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TFAW_DDR_MIN 0x14 ///< Min of Tfaw
#define IDSOPT_CMN_MEM_TIMING_TFAW_DDR_MAX 0x50 ///< Max of Tfaw

///TwtrL Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWTR_L_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWTR_L_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWTR_L_CTRL_DDR;

///TwtrL
///Specifies the Minimum Write to Read Time, the same bank group. Valid values: 0x8 ~ 0x30. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWTR_L_DDR_MIN 0x8 ///< Min of TwtrL
#define IDSOPT_CMN_MEM_TIMING_TWTR_L_DDR_MAX 0x30 ///< Max of TwtrL

///TwtrS Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWTR_S_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWTR_S_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWTR_S_CTRL_DDR;

///TwtrS
///Specifies the Minimum Write to Read Time, different bank group. Valid values: 0x2 ~ 0x10. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWTR_S_DDR_MIN 0x2 ///< Min of TwtrS
#define IDSOPT_CMN_MEM_TIMING_TWTR_S_DDR_MAX 0x10 ///< Max of TwtrS

///TrdrdScL Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_DDR;

///TrdrdScL
///Specifies the CAS to CAS delay time, same bank group. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_DDR_MIN 0x1 ///< Min of TrdrdScL
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_DDR_MAX 0xF ///< Max of TrdrdScL

///TrdrdSc Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDRD_SC_CTRL_DDR;

///TrdrdSc
///Specifies the Read to Read turnaround timing in the same chipselect. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_DDR_MIN 0x1 ///< Min of TrdrdSc
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_DDR_MAX 0xF ///< Max of TrdrdSc

///TrdrdSd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDRD_SD_CTRL_DDR;

///TrdrdSd
///Specifies the Read to Read turnaround timing in the same DIMM. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SD_DDR_MIN 0x1 ///< Min of TrdrdSd
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SD_DDR_MAX 0xF ///< Max of TrdrdSd

///TrdrdDd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDRD_DD_CTRL_DDR;

///TrdrdDd
///Specifies the Read and Read turnaround timing in a different DIMM. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRDRD_DD_DDR_MIN 0x1 ///< Min of TrdrdDd
#define IDSOPT_CMN_MEM_TIMING_TRDRD_DD_DDR_MAX 0xF ///< Max of TrdrdDd

///TwrwrScL Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_DDR;

///TwrwrScL
///Specifies the CAS to CAS Delay Time, same bank group. Valide values: 0x1 ~ 0x3F. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_DDR_MIN 0x1 ///< Min of TwrwrScL
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_DDR_MAX 0x3F ///< Max of TwrwrScL

///TwrwrSc Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRWR_SC_CTRL_DDR;

///TwrwrSc
///Specifies the Write to Write turnaround timing in the same chipselect. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_DDR_MIN 0x1 ///< Min of TwrwrSc
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_DDR_MAX 0xF ///< Max of TwrwrSc

///TwrwrSd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRWR_SD_CTRL_DDR;

///TwrwrSd
///Specifies the Write to Write turnaround timing in the same DIMM. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SD_DDR_MIN 0x1 ///< Min of TwrwrSd
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SD_DDR_MAX 0xF ///< Max of TwrwrSd

///TwrwrDd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRWR_DD_CTRL_DDR;

///TwrwrDd
///Specifies the Write to Write turnaround timing in a different DIMM. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWRWR_DD_DDR_MIN 0x1 ///< Min of TwrwrDd
#define IDSOPT_CMN_MEM_TIMING_TWRWR_DD_DDR_MAX 0xF ///< Max of TwrwrDd

///Twrrd Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRRD_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRRD_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRRD_CTRL_DDR;

///Twrrd
///Specifies the Write to Read turnaround timing. Valid values: 0x1 ~ 0xF. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TWRRD_DDR_MIN 0x1 ///< Min of Twrrd
#define IDSOPT_CMN_MEM_TIMING_TWRRD_DDR_MAX 0xF ///< Max of Twrrd

///Trdwr Ctrl
///Auto: Follow default setting, Manual: Manually specify
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDWR_CTRL_DDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDWR_CTRL_DDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDWR_CTRL_DDR;

///Trdwr
///Specifies the Read to Write Turnaround Timing. Valid value: 0x1 ~ 0x1F. The value is in hex.
#define IDSOPT_CMN_MEM_TIMING_TRDWR_DDR_MIN 0x1 ///< Min of Trdwr
#define IDSOPT_CMN_MEM_TIMING_TRDWR_DDR_MAX 0x1F ///< Max of Trdwr

///Processor CA drive strengths
///Select the drive strength for all CA0-13 IOs
typedef enum {
  IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CA_DDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_DRV_STREN_CA_DDR;

///Processor CS drive strengths
///Select the drive strength for all CS IOs
typedef enum {
  IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CS_DDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_DRV_STREN_CS_DDR;

///Processor CK drive strengths
///Select the drive strength for all CK{T,C} IOs
typedef enum {
  IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_DRV_STREN_CK_DDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_DRV_STREN_CK_DDR;

///Processor DQ drive strengths
///Select the drive strength for all DQ and DMI IOs
typedef enum {
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_HIGHIMPEDANCE = 0x0,///<High Impedance
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_240OHM = 0x2,///<240 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_120OHM = 0x4,///<120 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_80OHM = 0x6,///<80 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_60OHM = 0xC,///<60 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_48OHM = 0xE,///<48 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_40OHM = 0x1C,///<40 ohm
  IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR_343OHM = 0x1E,///<34.3 ohm
} IDSOPT_CMN_MEM_DRV_STREN_DQ_DDR;

///Processor ODT impedance
///Select the ODT impedance for all DBYTE IOs
typedef enum {
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_HIGHIMPEDANCE = 0,///<High Impedance
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_480OHM = 1,///<480 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_240OHM = 0x2,///<240 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_160OHM = 0x3,///<160 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_120OHM = 0x4,///<120 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_96OHM = 0x5,///<96 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_80OHM = 0x6,///<80 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_686OHM = 0x7,///<68.6 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_60OHM = 0xC,///<60 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_533OHM = 0xD,///<53.3 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_48OHM = 0xE,///<48 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_436OHM = 0xF,///<43.6 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_40OHM = 0x1C,///<40 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_369OHM = 0x1D,///<36.9 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_343OHM = 0x1E,///<34.3 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_32OHM = 0x1F,///<32 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_30OHM = 0x3C,///<30 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_282OHM = 0x3D,///<28.2 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_267OHM = 0x3E,///<26.7 ohm
  IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR_253OHM = 0x3F,///<25.3 ohm
} IDSOPT_CMN_MEM_ODT_IMPED_PROC_DDR;

///Dram DQ drive strengths
///Selects the Dram Pull-up and Pull-Down Output Driver Impedance for all DQ and DMI IOs
typedef enum {
  IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_34OHM = 0,///<34 ohm
  IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_40OHM = 1,///<40 ohm
  IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR_48OHM = 2,///<48 ohm
} IDSOPT_CMN_MEM_DRAM_DRV_STREN_DQ_DDR;

///Dram ODT impedance RTT_NOM_WR
///Select the DRAMs On-die Termination impedance for RTT_NOM_WR
typedef enum {
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RTT_OFF = 0x0,///<RTT_OFF
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ240 = 0x1,///<RZQ (240)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ2120 = 0x2,///<RZQ/2 (120)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ380 = 0x3,///<RZQ/3 (80)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ460 = 0x4,///<RZQ/4 (60)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ548 = 0x5,///<RZQ/5 (48)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ640 = 0x6,///<RZQ/6 (40)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR_RZQ734 = 0x7,///<RZQ/7 (34)
} IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_WR_DDR;

///Dram ODT impedance RTT_NOM_RD
///Select the DRAMs On-die Termination impedance for RTT_NOM_RD
typedef enum {
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RTT_OFF = 0x0,///<RTT_OFF
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ240 = 0x1,///<RZQ (240)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ2120 = 0x2,///<RZQ/2 (120)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ380 = 0x3,///<RZQ/3 (80)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ460 = 0x4,///<RZQ/4 (60)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ548 = 0x5,///<RZQ/5 (48)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ640 = 0x6,///<RZQ/6 (40)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR_RZQ734 = 0x7,///<RZQ/7 (34)
} IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_NOM_RD_DDR;

///Dram ODT impedance RTT_WR
///Select the DRAMs On-die Termination impedance for RTT_WR
typedef enum {
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RTT_OFF = 0x0,///<RTT_OFF
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ240 = 0x1,///<RZQ (240)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ2120 = 0x2,///<RZQ/2 (120)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ380 = 0x3,///<RZQ/3 (80)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ460 = 0x4,///<RZQ/4 (60)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ548 = 0x5,///<RZQ/5 (48)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ640 = 0x6,///<RZQ/6 (40)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR_RZQ734 = 0x7,///<RZQ/7 (34)
} IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_WR_DDR;

///Dram ODT impedance RTT_PARK
///Select the DRAMs On-die Termination impedance for RTT_PARK
typedef enum {
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RTT_OFF = 0x0,///<RTT_OFF
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ240 = 0x1,///<RZQ (240)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ2120 = 0x2,///<RZQ/2 (120)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ380 = 0x3,///<RZQ/3 (80)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ460 = 0x4,///<RZQ/4 (60)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ548 = 0x5,///<RZQ/5 (48)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ640 = 0x6,///<RZQ/6 (40)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR_RZQ734 = 0x7,///<RZQ/7 (34)
} IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DDR;

///Dram ODT impedance DQS_RTT_PARK
///Select the DRAMs On-die Termination impedance for DQS_RTT_PARK
typedef enum {
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RTT_OFF = 0x0,///<RTT_OFF
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ240 = 0x1,///<RZQ (240)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ2120 = 0x2,///<RZQ/2 (120)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ380 = 0x3,///<RZQ/3 (80)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ460 = 0x4,///<RZQ/4 (60)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ548 = 0x5,///<RZQ/5 (48)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ640 = 0x6,///<RZQ/6 (40)
  IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR_RZQ734 = 0x7,///<RZQ/7 (34)
} IDSOPT_CMN_MEM_DRAM_ODT_IMPED_RTT_PARK_DQS_DDR;

///Power Down Enable
///Enable or disable DDR power down mode
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_POWER_DOWN_EN_DDR;

///Phy Low Power Disable
///When LpDis = 1 : Disable putting phy in low power mode.\nWhen LpDis = 0 : Enable putting phy in low power mode and stop clocks to DRAM when all ranks are in power down.\nAuto should be 0.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_0 = 0,///<0
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR_1 = 1,///<1
} IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_DDR;

///Disable Memory Error Injection
///True: UMC::CH::MiscCfg[DisErrInj]=1
typedef enum {
  IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_FALSE = 0,///<False
  IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_TRUE = 1,///<True
  IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR_AUTO = 0xff,///<Auto
} IDSOPT_CMN_MEM_ECC_DIS_ERR_INJECTION_DDR;

///Data Poisoning
/// Enable/disable data poisoning: UMC_CH::EccCtrl[UcFatalEn] UMC_CH::EccCtrl[WrEccEn] Should be enabled/disabled together.
typedef enum {
  IDSOPT_CMN_MEM_DATA_POISONING_DDR4_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_DATA_POISONING_DDR4_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_DATA_POISONING_DDR4_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_DATA_POISONING_DDR4;

///ECC
///Use this option to enable / disable ECC. Auto will set ECC to enable.
typedef enum {
  IDSOPT_CMN_MEM_ECC_EN_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_ECC_EN_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_ECC_EN_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_ECC_EN_DDR;

///DRAM Redirect Scrubber Enable
///Enable/Disable Dram Redirect Scrubber
typedef enum {
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_EN_DDR;

///DRAM Scrub Redirection Limit
///Dram ECC Scrub Redirection Limit: 0=8 scrubs, 2=4 scrubs, 1=2 scrubs, 3=Unlimited scrub
typedef enum {
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_8SCRUBS = 0,///<8 Scrubs
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_4SCRUBS = 2,///<4 Scrubs
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_2SCRUBS = 1,///<2 Scrubs
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_UNLIMITEDSCRUBS = 3,///<Unlimited Scrubs
  IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_DRAM_REDIRECT_SCRUB_LIMIT_DDR;

///DRAM Scrub Time
///Provide a value that is the number of hours to scrub memory.
typedef enum {
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_1HOUR = 1,///<1 hour
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_4HOURS = 4,///<4 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_6HOURS = 6,///<6 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_8HOURS = 8,///<8 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_12HOURS = 12,///<12 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_16HOURS = 16,///<16 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_24HOURS = 24,///<24 hours
  IDSOPT_CMN_MEM_DRAM_SCRUB_TIME_48HOURS = 48,///<48 hours
} IDSOPT_CMN_MEM_DRAM_SCRUB_TIME;

///TSME
typedef enum {
  IDSOPT_CMN_MEM_TSME_ENABLE_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TSME_ENABLE_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_TSME_ENABLE_DDR_DISABLED = 0,///<Disabled
} IDSOPT_CMN_MEM_TSME_ENABLE_DDR;

///Data Scramble
///Data scrambling: DataScrambleEn
typedef enum {
  IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_ENABLED = 0x1,///<Enabled
  IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_DISABLED = 0x0,///<Disabled
  IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_DATA_CTRL_DATA_SCRAMBLE_EN_DDR;

///Enable SPD CRC verification
///This feature controls the DDR DIMM SPD CRC verify or not
typedef enum {
  IDSOPT_CMN_MEM_SPD_CRC_VERIFY_AUTO = 0xf,///<Auto
  IDSOPT_CMN_MEM_SPD_CRC_VERIFY_ENABLE = 1,///<Enable
  IDSOPT_CMN_MEM_SPD_CRC_VERIFY_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_SPD_CRC_VERIFY;

///SPD read retry count on CRC failure
///Controls how many SPD read retry while SPD CRC check failure
#define IDSOPT_CMN_MEM_SPD_READ_RETRY_COUNT_CRC_MIN 0 ///< Min of SPD read retry count on CRC failure
#define IDSOPT_CMN_MEM_SPD_READ_RETRY_COUNT_CRC_MAX 8 ///< Max of SPD read retry count on CRC failure

///Chipselect Interleaving
///Interleave memory blocks across the DRAM chip selects for node 0.
typedef enum {
  IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_DDR;

///Address Hash Bank
///Enable or disable bank address hashing
typedef enum {
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_DDR;

///Address Hash CS
///Enable or disable CS address hashing
typedef enum {
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR_DISABLED = 0,///<Disabled
} IDSOPT_CMN_MEM_ADDRESS_HASH_CS_DDR;

///BankSwapMode
///BankSwapMode value: 0=Disabled, 2=SwapAPU
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR_SWAPAPU = 2,///<Swap APU
} IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_MODE_DDR;

///Memory Restore valid days
///This token helps to set the raining  intervals and It is limited to 0~65535
#define IDSOPT_CMN_MEM_RESTORE_VALID_DAYS_MIN 0 ///< Min of Memory Restore valid days
#define IDSOPT_CMN_MEM_RESTORE_VALID_DAYS_MAX 0xffffffff ///< Max of Memory Restore valid days

///DFE Read Training
///Perform 2D Read Training with DFE on.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_ENABLE = 1,///<Enable
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_DDR;

///DRAM PDA Enumerate ID Programming Mode
///Specify PDA enumeration mode\nAuto : default\n0 : Continuous DQS toggling PDA enumeration mode (default)\n1 : Legacy PDA enumeration mode
typedef enum {
  IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_TOGGLINGPDAENUMERATIONMODE = 0,///<Toggling PDA enumeration mode
  IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR_LEGACYPDAENUMERATIONMODE = 1,///<Legacy PDA enumeration mode
} IDSOPT_CMN_MEM_DRAM_PDA_ENUM_ID_PROG_MODE_DDR;

///MBIST Enable
///Enable or disable Memory MBIST
typedef enum {
  IDSOPT_CMN_MEM_MBIST_EN_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_EN_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_MBIST_EN_DDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_MBIST_EN_DDR;

///MBIST Test Mode
///Select MBIST Test Mode -Interface Mode (Tests Single and Multiple CS transactions and Basic Connectivity) or Data Eye Mode (Measures Voltage vs. Timing)
typedef enum {
  IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_INTERFACEMODE = 0,///<Interface Mode
  IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_DATAEYEMODE = 1,///<Data Eye Mode
  IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_BOTH = 2,///<Both
  IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR_AUTO = 0xFF,///<AUTO
} IDSOPT_CMN_MEM_MBIST_TESTMODE_DDR;

///MBIST Aggressors
///Enable or disable MBIST Aggressor test
typedef enum {
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR_AUTO = 0xff,///<Auto
} IDSOPT_CMN_MEM_MBIST_AGGRESSORS_DDR;

///MBIST Per Bit Slave Die Reporting
///Reports 2D Data Eye Results in ABL Log for each DQ, Chipselect, and Channel
typedef enum {
  IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR_AUTO = 0xff,///<Auto
} IDSOPT_CMN_MEM_MBIST_PER_BIT_SLAVE_DIE_REPORT_DDR;

///Pattern Select
typedef enum {
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_PRBS = 0,///<PRBS
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_SSO = 1,///<SSO
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR_BOTH = 2,///<Both
} IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_DDR;

///Pattern Length
///This token helps to determine the pattern length. The possible options are N=3...12
#define IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_DDR_MIN 3 ///< Min of Pattern Length
#define IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_DDR_MAX 12 ///< Max of Pattern Length

///Aggressor Channel
///This helps read the aggressors channels. If it is enabled, you can read from one or more than one aggressor channel. The default is set to disabled.
typedef enum {
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_1AGGRESSORCHANNEL = 1,///<1 Aggressor Channel
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_3AGGRESSORCHANNELS = 3,///<3 Aggressor Channels
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR_7AGGRESSORCHANNELS = 7,///<7 Aggressor Channels
} IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_DDR;

///DRAM Timing Configuration Legal Disclaimer
///DRAM Timing Configuration Legal Disclaimer 1
///Active Memory Timing Settings
///Active Memory Timing Settings
typedef enum {
  IDSOPT_CMN_MEM_OVERCLOCK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_OVERCLOCK_LPDDR_ENABLED = 1,///<Enabled
} IDSOPT_CMN_MEM_OVERCLOCK_LPDDR;

///Maximum Memory Data Clock Speed
///Specifies the maximum memory target speed in MT/s.\nThe system will automatically limit to the highest supported speed if user input exceeds the maximum supported speed.
typedef enum {
  IDSOPT_CMN_MEM_SPEED_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_SPEED_LPDDR_3200MTS = 0x30,///<3200MT/s
  IDSOPT_CMN_MEM_SPEED_LPDDR_4267MTS = 0x40,///<4267MT/s
  IDSOPT_CMN_MEM_SPEED_LPDDR_5500MTS = 0x50,///<5500MT/s
  IDSOPT_CMN_MEM_SPEED_LPDDR_6400MTS = 0x59,///<6400MT/s
} IDSOPT_CMN_MEM_SPEED_LPDDR;

///Trcpage Ctrl
///Specify Trcpage
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRCPAGE_CTRL_LPDDR;

///Trcpage
///SDRAM Optional Features (tMAW, MAC). Valid value 3FFh - 0h
#define IDSOPT_CMN_MEM_TIMING_TRCPAGE_LPDDR_MIN 0 ///< Min of Trcpage
#define IDSOPT_CMN_MEM_TIMING_TRCPAGE_LPDDR_MAX 0x3ff ///< Max of Trcpage

///Tcwl
///Specifies the CAS Write Latency
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR_14HCLK = 0x14,///<14h Clk
} IDSOPT_CMN_MEM_TIMING_TCWL_LPDDR;

///Tcl
///Specifies the CAS latency.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_0FHCLK = 0xF,///<0Fh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1CHCLK = 0x1C,///<1Ch Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1DHCLK = 0x1D,///<1Dh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1EHCLK = 0x1E,///<1Eh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_1FHCLK = 0x1F,///<1Fh Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_20HCLK = 0x20,///<20h Clk
  IDSOPT_CMN_MEM_TIMING_TCL_LPDDR_21HCLK = 0x21,///<21h Clk
} IDSOPT_CMN_MEM_TIMING_TCL_LPDDR;

///Trcdrd
///Specifies the RAS# Active to CAS# Read Delay Time.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
} IDSOPT_CMN_MEM_TIMING_TRCDRD_LPDDR;

///Trcdwr
///Specifies the RAS# Active to CAS# Write Delay Time.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0ACLK = 0xA,///<0A Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0BCLK = 0xB,///<0B Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0CCLK = 0xC,///<0C Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0DCLK = 0xD,///<0D Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0ECLK = 0xE,///<0E Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_0FCLK = 0xF,///<0F Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
} IDSOPT_CMN_MEM_TIMING_TRCDWR_LPDDR;

///Trp
///Specifies Row Precharge Delay Time.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_0FHCLK = 0xF,///<0Fh Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRP_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
} IDSOPT_CMN_MEM_TIMING_TRP_LPDDR;

///Trfc Ctrl
///Specify Trfc
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRFC_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRFC_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRFC_CTRL_LPDDR;

///Trfc
///Specifies the Refresh Recovery Delay Time (tRFC1). Valid values 3DEh-3Ch
#define IDSOPT_CMN_MEM_TIMING_TRFC_LPDDR_MIN 0x3C ///< Min of Trfc
#define IDSOPT_CMN_MEM_TIMING_TRFC_LPDDR_MAX 0x3DE ///< Max of Trfc

///Tras
///Specifies the Active to Precharge Delay Time.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1CHCLK = 0x1C,///<1Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1DHCLK = 0x1D,///<1Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1EHCLK = 0x1E,///<1Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_1FHCLK = 0x1F,///<1Fh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_20HCLK = 0x20,///<20h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_21HCLK = 0x21,///<21h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_22HCLK = 0x22,///<22h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_23HCLK = 0x23,///<23h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_24HCLK = 0x24,///<24h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_25HCLK = 0x25,///<25h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_26HCLK = 0x26,///<26h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_27HCLK = 0x27,///<27h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_28HCLK = 0x28,///<28h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_29HCLK = 0x29,///<29h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2AHCLK = 0x2A,///<2Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2BHCLK = 0x2B,///<2Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2CHCLK = 0x2C,///<2Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2DHCLK = 0x2D,///<2Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2EHCLK = 0x2E,///<2Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_2FHCLK = 0x2F,///<2Fh Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_30HCLK = 0x30,///<30h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_31HCLK = 0x31,///<31h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_32HCLK = 0x32,///<32h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_33HCLK = 0x33,///<33h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_34HCLK = 0x34,///<34h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_35HCLK = 0x35,///<35h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_36HCLK = 0x36,///<36h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_37HCLK = 0x37,///<37h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_38HCLK = 0x38,///<38h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_39HCLK = 0x39,///<39h Clk
  IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR_3AHCLK = 0x3A,///<3Ah Clk
} IDSOPT_CMN_MEM_TIMING_TRAS_LPDDR;

///Trc Ctrl
///Specify Trc
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRC_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRC_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRC_CTRL_LPDDR;

///Trc
///Specifies Active to Active/Refresh Delay Time. Valid values 87h-1Dh.
#define IDSOPT_CMN_MEM_TIMING_TRC_LPDDR_MIN 0x1D ///< Min of Trc
#define IDSOPT_CMN_MEM_TIMING_TRC_LPDDR_MAX 0x87 ///< Max of Trc

///TrrdS
///Specifies the Activate to Activate Delay Time, different bank group (tRRD_S)
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR_0CHCLK = 0xC,///<0Ch Clk
} IDSOPT_CMN_MEM_TIMING_TRRD_S_LPDDR;

///TrrdL
///Specifies the Activate to Activate Delay Time, same bank group (tRRD_L)
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR_0CHCLK = 0xC,///<0Ch Clk
} IDSOPT_CMN_MEM_TIMING_TRRD_L_LPDDR;

///Tfaw Ctrl
///Specify Tfaw
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TFAW_CTRL_LPDDR;

///Tfaw
///Specifies the Four Activate Window Time. Valid values 36h-6h.
#define IDSOPT_CMN_MEM_TIMING_TFAW_LPDDR_MIN 0x6 ///< Min of Tfaw
#define IDSOPT_CMN_MEM_TIMING_TFAW_LPDDR_MAX 0x36 ///< Max of Tfaw

///TwtrS
///Specifies the Minimum Write to Read Time, different bank group
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR_0EHCLK = 0xE,///<0Eh Clk
} IDSOPT_CMN_MEM_TIMING_TWTR_S_LPDDR;

///TwtrL
///Specifies the Minimum Write to Read Time, same bank group
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR_0EHCLK = 0xE,///<0Eh Clk
} IDSOPT_CMN_MEM_TIMING_TWTR_L_LPDDR;

///Twr Ctrl
///Specify Twr
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWR_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWR_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWR_CTRL_LPDDR;

///Twr
///Specifies the Minimum Write Recovery Time. Valid value 51h-Ah
#define IDSOPT_CMN_MEM_TIMING_TWR_LPDDR_MIN 0xA ///< Min of Twr
#define IDSOPT_CMN_MEM_TIMING_TWR_LPDDR_MAX 0x51 ///< Max of Twr

///TrdrdScL Ctrl
///Specify TrdrdScL
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_CTRL_LPDDR;

///TrdrdScL
///Specifies the CAS to CAS Delay Time, same bank group. Valid values Fh-1h
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR_MIN 0x1 ///< Min of TrdrdScL
#define IDSOPT_CMN_MEM_TIMING_TRDRD_SC_L_LPDDR_MAX 0xf ///< Max of TrdrdScL

///TwrwrScL Ctrl
///Specify TwrwrScL
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR_AUTO = 0,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_CTRL_LPDDR;

///TwrwrScL
///Specifies the CAS to CAS Delay Time, same bank group. Valid values 3Fh-1h
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR_MIN 0x1 ///< Min of TwrwrScL
#define IDSOPT_CMN_MEM_TIMING_TWRWR_SC_L_LPDDR_MAX 0x3f ///< Max of TwrwrScL

///Trtp
///Specifies the Read CAS# to Precharge Delay Time.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_5CLK = 0x5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_6CLK = 0x6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_7CLK = 0x7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR_0EHCLK = 0xE,///<0Eh Clk
} IDSOPT_CMN_MEM_TIMING_TRTP_LPDDR;

///Tcke
///Specifies the CKE minimum high and low pulse width in memory clock cycles.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_0FHCLK = 0xF,///<0Fh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1CHCLK = 0x1C,///<1Ch Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1DHCLK = 0x1D,///<1Dh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1EHCLK = 0x1E,///<1Eh Clk
  IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR_1FHCLK = 0x1F,///<1Fh Clk
} IDSOPT_CMN_MEM_TIMING_TCKE_LPDDR;

///TrdrdSc
///Specifies the Read to Read turnaround timing in the same chipselect.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TRDRD_SC_LPDDR;

///TrdrdDd
///Specifies the Read to Read turnaround timing in a different DIMM.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TRDRD_DD_LPDDR;

///TrdrdSd
///Specifies the Read to Read turnaround timing in the same DIMM.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TRDRD_SD_LPDDR;

///TwrwrSc
///Specifies the Write to Write turnaround timing in the same chipselect.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TWRWR_SC_LPDDR;

///TwrwrSd
///Specifies the Write to Write turnaround timing in the same DIMM.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TWRWR_SD_LPDDR;

///TwrwrDd
///Specifies the Write to Write turnaround timing in a different DIMM.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TWRWR_DD_LPDDR;

///Twrrd
///Specifies the Write to Read turnaround timing.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR_0FHCLK = 0xF,///<0Fh Clk
} IDSOPT_CMN_MEM_TIMING_TWRRD_LPDDR;

///Trdwr
///Specifies the Read to Write turnaround timing.
typedef enum {
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1CLK = 1,///<1 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_2CLK = 2,///<2 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_3CLK = 3,///<3 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_4CLK = 4,///<4 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_5CLK = 5,///<5 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_6CLK = 6,///<6 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_7CLK = 7,///<7 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_8CLK = 8,///<8 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_9CLK = 9,///<9 Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0AHCLK = 0xA,///<0Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0BHCLK = 0xB,///<0Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0CHCLK = 0xC,///<0Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0DHCLK = 0xD,///<0Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0EHCLK = 0xE,///<0Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_0FHCLK = 0xF,///<0Fh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_10HCLK = 0x10,///<10h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_11HCLK = 0x11,///<11h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_12HCLK = 0x12,///<12h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_13HCLK = 0x13,///<13h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_14HCLK = 0x14,///<14h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_15HCLK = 0x15,///<15h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_16HCLK = 0x16,///<16h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_17HCLK = 0x17,///<17h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_18HCLK = 0x18,///<18h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_19HCLK = 0x19,///<19h Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1AHCLK = 0x1A,///<1Ah Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1BHCLK = 0x1B,///<1Bh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1CHCLK = 0x1C,///<1Ch Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1DHCLK = 0x1D,///<1Dh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1EHCLK = 0x1E,///<1Eh Clk
  IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR_1FHCLK = 0x1F,///<1Fh Clk
} IDSOPT_CMN_MEM_TIMING_TRDWR_LPDDR;

///LPDDR Refresh Mode
///Auto will result in all banks being enabled. Per bank enables refreshes to be sent on a per bank basis.
typedef enum {
  IDSOPT_CMN_MEM_REF_MODE_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_REF_MODE_LPDDR_ALLBANKS = 1,///<All Banks
  IDSOPT_CMN_MEM_REF_MODE_LPDDR_PERBANK = 2,///<Per Bank
} IDSOPT_CMN_MEM_REF_MODE_LPDDR;

///LPDDR RFM
///Use this option to enable / disable LPDDR refresh management feature.
typedef enum {
  IDSOPT_CMN_MEM_RFM_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_RFM_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_RFM_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_RFM_EN_LPDDR;

///WCK Always On
///Use this option to enable / disable WCK Always On.
typedef enum {
  IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_WCK_ALWAYS_ON_EN_LPDDR;

///RRW Memory Test Control
///Reliable Read Write (RRW)\nAuto is disabled by default.
typedef enum {
  IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR_DISABLED = 0,///<Disabled
} IDSOPT_CMN_MEM_RRW_MEMTEST_CTRL_LPDDR;

///Power Down Enable
///Enable or disable DDR power down mode
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_PWR_DN_EN_LPDDR;

///Phy Low Power Disable
///When LpDis = 1 : Disable putting phy in low power mode.\nWhen LpDis = 0 : Enable putting phy in low power mode and stop clocks to DRAM when all ranks are in power down.\n\nAuto should be 0.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_0 = 0,///<0
  IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR_1 = 1,///<1
} IDSOPT_CMN_MEM_CTRLLER_PD_PHY_POWER_SAVE_DIS_LPDDR;

///CAD Bus Timing User Controls
///Setup time on CAD bus signals to Auto or Manual
typedef enum {
  IDSOPT_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR_MANUAL = 1,///<Manual
} IDSOPT_CMN_MEM_CAD_BUS_TIMING_CTL_LPDDR;

///AddrCmdSetup
///Setup time on CAD bus signals. Valid values 3Fh-0h.
#define IDSOPT_CMN_MEM_ADDR_CMD_SETUP_LPDDR_MIN 0 ///< Min of AddrCmdSetup
#define IDSOPT_CMN_MEM_ADDR_CMD_SETUP_LPDDR_MAX 0x3f ///< Max of AddrCmdSetup

///CsOdtSetup
///Setup time on CAD bus signals. Valid values 3Fh-0h.
#define IDSOPT_CMN_MEM_CS_ODT_SETUP_LPDDR_MIN 0 ///< Min of CsOdtSetup
#define IDSOPT_CMN_MEM_CS_ODT_SETUP_LPDDR_MAX 0x3f ///< Max of CsOdtSetup

///CA drive strengths
///Select the drive strength for all CA0-7 IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CA_LPDDR;

///CS drive strengths
///Select the drive strength for all LP5 CS IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_4000OHM = 400,///<400.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_1000OHM = 100,///<100.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_670OHM = 67,///<67.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR_500OHM = 50,///<50.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CS_LPDDR;

///CK drive strengths
///Select the drive strength for all CK{T,C} IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_CK_LPDDR;

///WCK drive strengths
///Select the drive strength for all WCK{T,C} IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_WCK_LPDDR;

///DQ drive strengths
///Select the drive strength for all DQ and DMI IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQ_LPDDR;

///DQS drive strengths
///Select the drive strength for all DQS{T,C} IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_DRV_STREN_DQS_LPDDR;

///Processor ODT impedance
///Select the ODT impedance for all DBYTE IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR_300OHM = 30,///<30.0 Ohm
} IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_LPDDR;

///Processor CA ODT impedance
///Select the ODT impedance for ACHAN CA IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_300OHM = 30,///<30.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CA_LPDDR;

///Processor CK ODT impedance
///Select the ODT impedance for ACHAN CK IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_300OHM = 30,///<30.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_CK_LPDDR;

///Processor WCK ODT impedance
///Select the ODT impedance for ACHAN WCK IOs
typedef enum {
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_1200OHM = 120,///<120.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_600OHM = 60,///<60.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_400OHM = 40,///<40.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_300OHM = 30,///<30.0 Ohm
  IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_PHY_IO_ODT_STREN_PROC_WCK_LPDDR;

///Dram CA ODT impedance
///Select the ODT impedance for Dram CA Bus Receiver
typedef enum {
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_DISABLE = 0,///<Disable
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ1 = 1,///<RZQ/1
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ2 = 2,///<RZQ/2
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ3 = 3,///<RZQ/3
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ4 = 4,///<RZQ/4
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ5 = 5,///<RZQ/5
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR_RZQ6 = 6,///<RZQ/6
} IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_CA_LPDDR;

///Dram DQ ODT impedance
///Select the ODT impedance for Dram DQ Bus Receiver
typedef enum {
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_DISABLE = 0,///<Disable
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ1 = 1,///<RZQ/1
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ2 = 2,///<RZQ/2
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ3 = 3,///<RZQ/3
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ4 = 4,///<RZQ/4
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ5 = 5,///<RZQ/5
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR_RZQ6 = 6,///<RZQ/6
} IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_DQ_LPDDR;

///Dram WCK ODT impedance
///Select the ODT impedance for Dram WCK Bus Receiver
typedef enum {
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_DISABLE = 0,///<Disable
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ1 = 1,///<RZQ/1
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ2 = 2,///<RZQ/2
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ3 = 3,///<RZQ/3
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ4 = 4,///<RZQ/4
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ5 = 5,///<RZQ/5
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR_RZQ6 = 6,///<RZQ/6
} IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_WCK_LPDDR;

///Dram Non-Target ODT impedance
///Select the ODT impedance for Dram Non-Target Rank
typedef enum {
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_DISABLE = 0,///<Disable
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ1 = 1,///<RZQ/1
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ2 = 2,///<RZQ/2
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ3 = 3,///<RZQ/3
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ4 = 4,///<RZQ/4
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ5 = 5,///<RZQ/5
  IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ_RZQ6 = 6,///<RZQ/6
} IDSOPT_CMN_MEM_DRAM_IO_ODT_STREN_NTDQ;

///Dram Pull-Down drive strengths
///Select the drive strength for Dram Pull-Down resistance
typedef enum {
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_DISABLE = 0,///<Disable
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ1 = 1,///<RZQ/1
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ2 = 2,///<RZQ/2
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ3 = 3,///<RZQ/3
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ4 = 4,///<RZQ/4
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ5 = 5,///<RZQ/5
  IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS_RZQ6 = 6,///<RZQ/6
} IDSOPT_CMN_MEM_DRAM_IO_DRV_STREN_PDDS;

///DRAM Read Link ECC Enable
///Use this option to enable / disable Link ECC on DRAM Read. Auto will set ECC to enable.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_DRAM_RD_LINK_ECC_EN_LPDDR;

///DRAM Write Link ECC Enable
///Use this option to enable / disable Link ECC on DRAM Write. Auto will set ECC to enable.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_DRAM_WR_LINK_ECC_EN_LPDDR;

///Disable Memory Error Injection
///True: UMC::CH::MiscCfg[DisErrInj]=1
typedef enum {
  IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_FALSE = 0,///<False
  IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_TRUE = 1,///<True
  IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR_AUTO = 0xff,///<Auto
} IDSOPT_CMN_MEM_DIS_MEM_ERR_INJ_LPDDR;

///TSME
typedef enum {
  IDSOPT_CMN_MEM_TSME_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_TSME_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_TSME_LPDDR_DISABLED = 0,///<Disabled
} IDSOPT_CMN_MEM_TSME_LPDDR;

///Data Scramble
///Data scrambling: DataScrambleEn
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_ENABLED = 0x1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_DISABLED = 0x0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_DATA_SCRAMBLE_LPDDR;

///Chip Select Interleaving
///Interleave memory blocks across the DRAM chip selects for node 0.
typedef enum {
  IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_MAPPING_BANK_INTERLEAVE_LPDDR;

///Bank Swap
///Swapping BankGroup/Bank bits with ColumnHigh bits.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_BANK_SWAP_LPDDR;

///BankGroup Swap
///Swapping Bank and BankGroup bits
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_ENABLED = 2,///<Enabled
  IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_CTRLLER_BANK_GROUP_SWAP_LPDDR;

///Address Hash Bank
///Enable or disable bank address hashing
typedef enum {
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_ADDRESS_HASH_BANK_LPDDR;

///Address Hash CS
///Enable or disable CS address hashing
typedef enum {
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR_DISABLED = 0,///<Disabled
} IDSOPT_CMN_MEM_ADDRESS_HASH_CS_LPDDR;

///DFE Read Training
///Perform 2D Read Training with DFE on.
typedef enum {
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_ENABLE = 1,///<Enable
  IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR_DISABLE = 0,///<Disable
} IDSOPT_CMN_MEM_CTRLLER_PMU_TRAIN_DFE_LPDDR;

///MBIST Enable
///Enable or disable Memory MBIST
typedef enum {
  IDSOPT_CMN_MEM_MBIST_EN_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_EN_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_MBIST_EN_LPDDR_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MEM_MBIST_EN_LPDDR;

///MBIST Test Mode
///Select MBIST Test Mode -Interface Mode (Tests Single and Multiple CS transactions and Basic Connectivity) or Data Eye Mode (Measures Voltage vs. Timing)
typedef enum {
  IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_INTERFACEMODE = 0,///<Interface Mode
  IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_DATAEYEMODE = 1,///<Data Eye Mode
  IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_BOTH = 2,///<Both
  IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR_AUTO = 0xFF,///<AUTO
} IDSOPT_CMN_MEM_MBIST_TESTMODE_LPDDR;

///MBIST Aggressors
///Enable or disable MBIST Aggressor test
typedef enum {
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR_AUTO = 0xff,///<Auto
} IDSOPT_CMN_MEM_MBIST_AGGRESSORS_LPDDR;

///Pattern Select
typedef enum {
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_PRBS = 0,///<PRBS
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_SSO = 1,///<SSO
  IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR_BOTH = 2,///<Both
} IDSOPT_CMN_MEM_MBIST_PATTERN_SELECT_LPDDR;

///Pattern Length
///This token helps to determine the pattern length. The possible options are N=3...12
#define IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_LPDDR_MIN 3 ///< Min of Pattern Length
#define IDSOPT_CMN_MEM_MBIST_PATTERN_LENGTH_LPDDR_MAX 12 ///< Max of Pattern Length

///Aggressor Channel
///This helps read the aggressors channels. If it is enabled, you can read from one or more than one aggressor channel. The default is set to disabled.
typedef enum {
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_1AGGRESSORCHANNEL = 1,///<1 Aggressor Channel
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_3AGGRESSORCHANNELS = 3,///<3 Aggressor Channels
  IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR_7AGGRESSORCHANNELS = 7,///<7 Aggressor Channels
} IDSOPT_CMN_MEM_MBIST_AGGRESSORS_CHNL_LPDDR;

///Read Voltage Sweep Step Size
///This option determines the step size for Read Data Eye voltage sweep, Supported options are 1,2 and 4
typedef enum {
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_1 = 0,///<1
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_2 = 1,///<2
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR_4 = 4,///<4
} IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_VOLTAGE_STEP_LPDDR;

///Read Timing Sweep Step Size
///This options supports step size for Read Data Eye. Supported options are 1, 2 and 4
typedef enum {
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_1 = 1,///<1
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_2 = 2,///<2
  IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR_4 = 4,///<4
} IDSOPT_CMN_MEM_MBIST_READ_DATA_EYE_TIMING_STEP_LPDDR;

///Write Voltage Sweep Step Size
///This option determines the step size for write Data Eye voltage sweep, Supported options are 1,2 and 4
typedef enum {
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_1 = 0,///<1
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_2 = 1,///<2
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR_4 = 4,///<4
} IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_VOLTAGE_STEP_LPDDR;

///Write Timing Sweep Step Size
///This options supports step size for write Data Eye. Supported options are 1, 2 and 4
typedef enum {
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_1 = 1,///<1
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_2 = 2,///<2
  IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR_4 = 4,///<4
} IDSOPT_CMN_MEM_MBIST_WRITE_DATA_EYE_TIMING_STEP_LPDDR;

///IOMMU
///Enable/Disable IOMMU
typedef enum {
  IDSOPT_CMN_GNB_NB_IOMMU_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOMMU_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOMMU_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOMMU;

///DMAr Support
///Enable DMAr system protection during POST.
typedef enum {
  IDSOPT_CMN_DMAR_SUPPORT_AUTO = 0xF,///<Auto
  IDSOPT_CMN_DMAR_SUPPORT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_DMAR_SUPPORT_DISABLED = 0,///<Disabled
} IDSOPT_CMN_DMAR_SUPPORT;

///DMA Protection
///Enable DMA remap support in IVRS IVinfo Field.
typedef enum {
  IDSOPT_CMN_DMA_PROTECTION_AUTO = 0xF,///<Auto
  IDSOPT_CMN_DMA_PROTECTION_ENABLED = 1,///<Enabled
  IDSOPT_CMN_DMA_PROTECTION_DISABLED = 0,///<Disabled
} IDSOPT_CMN_DMA_PROTECTION;

///SCPC attribute control
///Used to control the active behavior of IOMMU and DMAr support and DMA protection options
typedef enum {
  IDSOPT_CMN_SCPC_ATTR_CNTL_0 = 0,///<0
  IDSOPT_CMN_SCPC_ATTR_CNTL_1 = 1,///<1
  IDSOPT_CMN_SCPC_ATTR_CNTL_2 = 2,///<2
  IDSOPT_CMN_SCPC_ATTR_CNTL_3 = 3,///<3
} IDSOPT_CMN_SCPC_ATTR_CNTL;

///PCIe ARI Support
///Enable/Disable ARI
typedef enum {
  IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_PCIE_ARI_SUPPORT;

///PCIe ARI Enumeration
///ARI Forwarding Enable for each downstream port
typedef enum {
  IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_DISABLE = 0,///<Disable
  IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_ENABLE = 1,///<Enable
  IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION_AUTO = 0xf,///<Auto
} IDSOPT_GNB_CMN_PCIE_ARI_ENUMERATION;

///Enable AER Cap
///Enables Advanced Error Reporting Capability
typedef enum {
  IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_ENABLE = 1,///<Enable
  IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE_AUTO = 0xF,///<Auto
} IDSOPT_DBG_GNB_DBG_AERCAP_ENABLE;

///ACS Enable
///AER must be enabled for ACS enable to work
typedef enum {
  IDSOPT_DBG_GNB_DBG_ACS_ENABLE_ENABLE = 1,///<Enable
  IDSOPT_DBG_GNB_DBG_ACS_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_DBG_GNB_DBG_ACS_ENABLE_AUTO = 0xF,///<Auto
} IDSOPT_DBG_GNB_DBG_ACS_ENABLE;

///PSPP Policy
///PSPP Policy
typedef enum {
  IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_BALANCED = 2,///<Balanced
  IDSOPT_CMN_GNB_PCIE_PSPP_POLICY_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_PCIE_PSPP_POLICY;

///PCIe All Ports Settings
///These settings will be applied to all active PCIe ports unless overriden on a per port basis.
typedef enum {
  IDSOPT_CMN_PCIE_PORT_ALL_PORTS_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PCIE_PORT_ALL_PORTS_CUSTOM = 1,///<Custom
} IDSOPT_CMN_PCIE_PORT_ALL_PORTS;

///NBIO RAS Control
///(0) Disabled, (1) MCA
typedef enum {
  IDSOPT_CMN_RAS_CONTROL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_RAS_CONTROL_MCA = 1,///<MCA
  IDSOPT_CMN_RAS_CONTROL_AUTO = 0xF,///<Auto
} IDSOPT_CMN_RAS_CONTROL;

///Egress Poison Severity High
///Each bit set to 1 enables HIGH severity on the associated IOHC egress port. A bit of 0 indicates LOW severity.
#define IDSOPT_PCD_EGRESS_POISON_SEVERITY_HI_MIN 0 ///< Min of Egress Poison Severity High
#define IDSOPT_PCD_EGRESS_POISON_SEVERITY_HI_MAX 0xffffffff ///< Max of Egress Poison Severity High

///Egress Poison Severity Low
///Each bit set to 1 enables HIGH severity on the associated IOHC egress port. A bit of 0 indicates LOW severity.
#define IDSOPT_PCD_EGRESS_POISON_SEVERITY_LO_MIN 0 ///< Min of Egress Poison Severity Low
#define IDSOPT_PCD_EGRESS_POISON_SEVERITY_LO_MAX 0xffffffff ///< Max of Egress Poison Severity Low

///NBIO SyncFlood Generation
///This value may be used to mask SyncFlood caused by NBIO RAS options.  When set to TRUE SyncFlood from NBIO is masked.  When set to FALSE NBIO is capable of generating SyncFlood.
typedef enum {
  IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_ENABLED = 1,///<Enabled
  IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_DISABLED = 0,///<Disabled
  IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN_AUTO = 0xf,///<Auto
} IDSOPT_CMN_NBIO_SYNC_FLOOD_GEN;

///NBIO SyncFlood Reporting
///This value may be used to enable SyncFlood reporting to APML.  When set to TRUE SyncFlood will be reported to APML.  When set to FALSE that reporting well be disabled
typedef enum {
  IDSOPT_PCD_SYNC_FLOOD_TO_APML_ENABLED = 1,///<Enabled
  IDSOPT_PCD_SYNC_FLOOD_TO_APML_DISABLED = 0,///<Disabled
  IDSOPT_PCD_SYNC_FLOOD_TO_APML_AUTO = 0xF,///<Auto
} IDSOPT_PCD_SYNC_FLOOD_TO_APML;

///Egress Poison Mask High
///These set the enable mask for masking of errors logged in EGRESS_POISON_STATUS. For each bit set to 1, errors are masked.  For each bit set to 0, errors trigger response actions.
#define IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_HI_MIN 0 ///< Min of Egress Poison Mask High
#define IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_HI_MAX 0xffffffff ///< Max of Egress Poison Mask High

///Egress Poison Mask Low
///These set the enable mask for masking of errors logged in EGRESS_POISON_STATUS. For each bit set to 1, errors are masked.  For each bit set to 0, errors trigger response actions.
#define IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_LO_MIN 0 ///< Min of Egress Poison Mask Low
#define IDSOPT_PCD_AMD_NBIO_EGRESS_POISON_MASK_LO_MAX 0xffffffff ///< Max of Egress Poison Mask Low

///Uncorrected Converted to Poison Enable Mask High
///These set the enable mask for masking of uncorrectable parity errors on internal arrays.  For each bit set to 0, a system fatal error event is triggered for UCP errors on arrays associated with that egress port.  For each bit set to 1, errors are masked.
#define IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_HI_MIN 0 ///< Min of Uncorrected Converted to Poison Enable Mask High
#define IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_HI_MAX 0xffffffff ///< Max of Uncorrected Converted to Poison Enable Mask High

///Uncorrected Converted to Poison Enable Mask Low
///These set the enable mask for masking of uncorrectable parity errors on internal arrays.  For each bit set to 0, a system fatal error event is triggered for UCP errors on arrays associated with that egress port.  For each bit set to 1, errors are masked.
#define IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_LO_MIN 0 ///< Min of Uncorrected Converted to Poison Enable Mask Low
#define IDSOPT_PCD_AMD_NBIO_RAS_UCP_MASK_LO_MAX 0xffffffff ///< Max of Uncorrected Converted to Poison Enable Mask Low

///System Hub Watchdog Timer
///This value specifies the timer interval of the SYSHUB Watchdog timer in miliseconds
#define IDSOPT_PCD_SYSHUB_WDT_TIMER_INTERVAL_MIN 0 ///< Min of System Hub Watchdog Timer
#define IDSOPT_PCD_SYSHUB_WDT_TIMER_INTERVAL_MAX 0xffff ///< Max of System Hub Watchdog Timer

///NBIO Poison Consumption
///NBIO Poison Consumption
typedef enum {
  IDSOPT_DBG_POISON_CONSUMPTION_ENABLED = 1,///<Enabled
  IDSOPT_DBG_POISON_CONSUMPTION_DISABLED = 0,///<Disabled
} IDSOPT_DBG_POISON_CONSUMPTION;

///iGPU Configuration
///UMA Mode
typedef enum {
  IDSOPT_CMN_GNB_GFX_UMA_MODE_AUTO = 0xf,///<Auto
  IDSOPT_CMN_GNB_GFX_UMA_MODE_IGPUDISABLED = 0,///<iGPU Disabled
  IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_SPECIFIED = 1,///<UMA_SPECIFIED
  IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_AUTO = 2,///<UMA_AUTO
  IDSOPT_CMN_GNB_GFX_UMA_MODE_UMA_GAME_OPTIMIZED = 3,///<UMA_GAME_OPTIMIZED
} IDSOPT_CMN_GNB_GFX_UMA_MODE;

///UMA Version
///UMA Legacy Version\nUMA Non Legacy Version\nHybrid Secure
typedef enum {
  IDSOPT_CMN_GNB_GFX_UMA_VERSION_LEGACY = 0,///<Legacy
  IDSOPT_CMN_GNB_GFX_UMA_VERSION_NONLEGACY = 1,///<Non-Legacy
  IDSOPT_CMN_GNB_GFX_UMA_VERSION_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_GFX_UMA_VERSION;

///UMA Frame buffer Size
///Set UMA FB size
typedef enum {
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_AUTO = 0xffffffff,///<Auto
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_64M = 64,///<64M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_128M = 128,///<128M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_256M = 256,///<256M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_384M = 384,///<384M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_512M = 512,///<512M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_80M = 80,///<80M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_96M = 96,///<96M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_768M = 768,///<768M
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_1G = 1024,///<1G
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_2G = 2048,///<2G
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_3G = 3072,///<3G
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_4G = 4096,///<4G
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_8G = 8192,///<8G
  IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE_16G = 16384,///<16G
} IDSOPT_CMN_GNB_GFX_UMA_FRAME_BUFFER_SIZE;

///GPU Host Translation Cache
///Option to disable GPU Host Translation Cache
typedef enum {
  IDSOPT_CMN_GPU_HOST_TRANSLATION_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GPU_HOST_TRANSLATION_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GPU_HOST_TRANSLATION_AUTO = 0xF,///<Auto
} IDSOPT_CMN_GPU_HOST_TRANSLATION;

///NB Azalia
///Enable Integrate HD Audio controller
typedef enum {
  IDSOPT_CMN_GNB_GFX_NB_AZALIA_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_GFX_NB_AZALIA_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_GFX_NB_AZALIA_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_GFX_NB_AZALIA;

///Audio IOs
///Audio IOs control
typedef enum {
  IDSOPT_DBG_GNB_AUDIO_I_OS_AUTO = 0xFF,///<Auto
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM2CH = 4,///<HDA(3SDI) +  PDM(2CH)
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDIPDM6CH = 5,///<HDA(1SDI) +  PDM(6CH)
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDISDW01MDATAPDM2CH = 6,///<HDA(1SDI) + SDW0(1MDATA) + PDM(2CH)
  IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATAPDM6CH = 7,///<SDW0(4MDATA) + PDM(6CH)
  IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM2CH = 8,///<SDW0(4MDATA) + SDW1(1MDATA) + PDM(2CH)
  IDSOPT_DBG_GNB_AUDIO_I_OS_3I2SPORTS1REFCLK1INTR = 9,///<3I2S PORTS + 1 REFCLK + 1 INTR
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM6CHI2S = 10,///<HDA(3SDI) +  PDM(6CH) +I2S
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA3SDIPDM8CH = 11,///<HDA(3SDI) +  PDM(8CH) 
  IDSOPT_DBG_GNB_AUDIO_I_OS_HDA1SDISDW01MDATAPDM6CHI2S = 12,///<HDA(1SDI) + SDW0(1MDATA) + PDM(6CH) + I2S
  IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM6CHI2S = 13,///<SDW0(4MDATA)+ SDW1(1MDATA) + PDM(6CH) + I2S 
  IDSOPT_DBG_GNB_AUDIO_I_OS_SDW04MDATASDW11MDATAPDM8CH = 14,///<SDW0(4MDATA) + SDW1(1MDATA)+ PDM(8CH) 
} IDSOPT_DBG_GNB_AUDIO_I_OS;

///ASPM
///Enable port ASPM
typedef enum {
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_L1SUPPORT = 2,///<L1 Support
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM_AUTO = 0xf,///<Auto
} IDSOPT_CMN_PCIE_ALL_PORTS_CAPASPM;

///L1 PM SubState
///Enable for PM L1 SS and ASPM L1 SS
typedef enum {
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_L11ONLY = 1,///<L1.1 Only
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS_L12ANDL11 = 2,///<L1.2 (and L1.1)
} IDSOPT_CMN_PCIE_ALL_PORTS_CAPPM_L1_SS;

///LTR Support
///Enables support for Latency Tolerance Reporting
typedef enum {
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_NOTSUPPORTED = 0,///<Not Supported
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_SUPPORTED = 1,///<Supported
  IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR_AUTO = 0xF,///<Auto
} IDSOPT_CMN_PCIE_ALL_PORTS_CAPLTR;

///ASPM
///Enable port ASPM
typedef enum {
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_L1 = 2,///<L1
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM_AUTO = 0xf,///<Auto
} IDSOPT_CMN_PCIE_ALL_PORTS_PM_LINK_ASPM;

///L1 PM SubState
///Enable for PM L1 SS and ASPM L1 SS
typedef enum {
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_L11ONLY = 1,///<L1.1 Only
  IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS_L12ANDL11 = 2,///<L1.2 (and L1.1)
} IDSOPT_CMN_PCIE_ALL_PORTS_PM_L1_SS;

///LCLK Gating
///Enable LCLK clock gating
typedef enum {
  IDSOPT_CMN_PCIE_CORE_LCLK_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_CORE_LCLK_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_PCIE_CORE_LCLK_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_PCIE_CORE_LCLK_GATING;

///PCIe Dynamic PG
/// Enable/Disable Pcie Core Dynamic Power Gating
typedef enum {
  IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_PCIE_CORE_DYNAMIC_POWER_GATING;

///NBIO Global CG Override
typedef enum {
  IDSOPT_CMN_NBIO_GLOBAL_CG_OVERRIDE_AUTO = 0xf,///<Auto
  IDSOPT_CMN_NBIO_GLOBAL_CG_OVERRIDE_DISABLED = 0,///<Disabled
} IDSOPT_CMN_NBIO_GLOBAL_CG_OVERRIDE;

///IOHC LCLK Clock Gating
///LCLK Gating (IOC)
typedef enum {
  IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_PMM_IOHC_LCLK_GATING;

///IOHC Power Gating
///IOHC Power Gating
typedef enum {
  IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOHC_POWER_GATING;

///IOMMU L1 Clock Gating
///IOMMU L1 LCLK Gating
typedef enum {
  IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOMMU_L1_CLOCK_GATING;

///IOMMU L2 Clock Gating
///IOMMU L2 LCLK Gating
typedef enum {
  IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOMMU_L2_CLOCK_GATING;

///IOMMU Dynamic Power Gating
///IOMMU Dynamic PG
typedef enum {
  IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOMMU_DYNAMIC_PG;

///IOMMU L2 Memory Power Gating
///IOMMU L2 Memory Power Gating
typedef enum {
  IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_NB_IOMMU_L2_MEMORY_PG;

///NBIF PG
///NBIF PG
typedef enum {
  IDSOPT_CMN_NBIO_NBIF_PG_AUTO = 0xf,///<Auto
  IDSOPT_CMN_NBIO_NBIF_PG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_NBIO_NBIF_PG_DISABLED = 0,///<Disabled
} IDSOPT_CMN_NBIO_NBIF_PG;

///SHUB PG
///SHUB PG
typedef enum {
  IDSOPT_CMN_NBIO_SHUB_PG_AUTO = 0xf,///<Auto
  IDSOPT_CMN_NBIO_SHUB_PG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_NBIO_SHUB_PG_DISABLED = 0,///<Disabled
} IDSOPT_CMN_NBIO_SHUB_PG;

///MMHUB SRAM Light Sleep
typedef enum {
  IDSOPT_CMN_PMM_MMHUB_SRAM_LS_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PMM_MMHUB_SRAM_LS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PMM_MMHUB_SRAM_LS_ENABLED = 1,///<Enabled
} IDSOPT_CMN_PMM_MMHUB_SRAM_LS;

///NBIF INTR OBFF
///OBFF enable for Interrupt : OBFF_EMU_INTR_EN
typedef enum {
  IDSOPT_CMN_PMM_NBIF_INTR_OBFF_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PMM_NBIF_INTR_OBFF_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PMM_NBIF_INTR_OBFF_ENABLED = 1,///<Enabled
} IDSOPT_CMN_PMM_NBIF_INTR_OBFF;

///NBIF DMA OBFF
///OBFF enable for DMA : OBFF_EMU_DMA_EN
typedef enum {
  IDSOPT_CMN_PMM_NBIF_DMA_OBFF_AUTO = 0xf,///<Auto
  IDSOPT_CMN_PMM_NBIF_DMA_OBFF_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PMM_NBIF_DMA_OBFF_ENABLED = 1,///<Enabled
} IDSOPT_CMN_PMM_NBIF_DMA_OBFF;

///I3C/I2C 0 Enable
typedef enum {
  IDSOPT_CMN_FCH_I3_C0_CONFIG_BOTHDISABLED = 0,///<Both Disabled
  IDSOPT_CMN_FCH_I3_C0_CONFIG_I3CENABLED = 1,///<I3C Enabled
  IDSOPT_CMN_FCH_I3_C0_CONFIG_I2CENABLED = 2,///<I2C Enabled
  IDSOPT_CMN_FCH_I3_C0_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C0_CONFIG;

///I3C 0 Mode
typedef enum {
  IDSOPT_CMN_FCH_I3_C0_MODE_I3C = 0,///<I3C
  IDSOPT_CMN_FCH_I3_C0_MODE_I2C = 1,///<I2C
  IDSOPT_CMN_FCH_I3_C0_MODE_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C0_MODE;

///I3C/I2C 1 Enable
typedef enum {
  IDSOPT_CMN_FCH_I3_C1_CONFIG_BOTHDISABLED = 0,///<Both Disabled
  IDSOPT_CMN_FCH_I3_C1_CONFIG_I3CENABLED = 1,///<I3C Enabled
  IDSOPT_CMN_FCH_I3_C1_CONFIG_I2CENABLED = 2,///<I2C Enabled
  IDSOPT_CMN_FCH_I3_C1_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C1_CONFIG;

///I3C 1 Mode
typedef enum {
  IDSOPT_CMN_FCH_I3_C1_MODE_I3C = 0,///<I3C
  IDSOPT_CMN_FCH_I3_C1_MODE_I2C = 1,///<I2C
  IDSOPT_CMN_FCH_I3_C1_MODE_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C1_MODE;

///I3C/I2C 2 Enable
typedef enum {
  IDSOPT_CMN_FCH_I3_C2_CONFIG_BOTHDISABLED = 0,///<Both Disabled
  IDSOPT_CMN_FCH_I3_C2_CONFIG_I3CENABLED = 1,///<I3C Enabled
  IDSOPT_CMN_FCH_I3_C2_CONFIG_I2CENABLED = 2,///<I2C Enabled
  IDSOPT_CMN_FCH_I3_C2_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C2_CONFIG;

///I3C 2 Mode
typedef enum {
  IDSOPT_CMN_FCH_I3_C2_MODE_I3C = 0,///<I3C
  IDSOPT_CMN_FCH_I3_C2_MODE_I2C = 1,///<I2C
  IDSOPT_CMN_FCH_I3_C2_MODE_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C2_MODE;

///I3C/I2C 3 Enable
typedef enum {
  IDSOPT_CMN_FCH_I3_C3_CONFIG_BOTHDISABLED = 0,///<Both Disabled
  IDSOPT_CMN_FCH_I3_C3_CONFIG_I3CENABLED = 1,///<I3C Enabled
  IDSOPT_CMN_FCH_I3_C3_CONFIG_I2CENABLED = 2,///<I2C Enabled
  IDSOPT_CMN_FCH_I3_C3_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C3_CONFIG;

///I3C 3 Mode
typedef enum {
  IDSOPT_CMN_FCH_I3_C3_MODE_I3C = 0,///<I3C
  IDSOPT_CMN_FCH_I3_C3_MODE_I2C = 1,///<I2C
  IDSOPT_CMN_FCH_I3_C3_MODE_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_I3_C3_MODE;

///SATA Controller
///Disable or enable OnChip SATA controller
typedef enum {
  IDSOPT_CMN_FCH_SATA_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_SATA_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_SATA_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SATA_ENABLE;

///SATA Auto Shutdown
///Disable SATA controller if there is no port connection.
typedef enum {
  IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SATA_AUTO_SHUT_DOWN;

///SATA Mode
///Select OnChip SATA Type. Don't select RAID for Raven FP5 platform. Raven FP5 doesn't support RAID.
typedef enum {
  IDSOPT_CMN_FCH_SATA_CLASS_AHCI = 2,///<AHCI
  IDSOPT_CMN_FCH_SATA_CLASS_AHCIASID0X7904 = 5,///<AHCI as ID 0x7904
  IDSOPT_CMN_FCH_SATA_CLASS_AUTO = 0xf,///<Auto
  IDSOPT_CMN_FCH_SATA_CLASS_RAID = 0x1,///<RAID
} IDSOPT_CMN_FCH_SATA_CLASS;

///Sata RAS Support
///Disable or enable Sata RAS Support
typedef enum {
  IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_SATA_RAS_SUPPORT_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SATA_RAS_SUPPORT;

///Sata Disabled AHCI Prefetch Function
///Disable or enable Sata Disabled AHCI Prefetch Function
typedef enum {
  IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SATA_AHCI_DIS_PREFETCH_FUNCTION;

///Aggresive SATA Device Sleep Port 0
typedef enum {
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_DISABLE = 0,///<Disable
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_ENABLE = 1,///<Enable
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0_AUTO = 0xF,///<Auto
} IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P0;

///DevSleep0 Port Number
///SATA port number map to DEVSLP pad 0: 0-7: SATA 0; 8-15: SATA 1
#define IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT0_NUM_MIN 0 ///< Min of DevSleep0 Port Number
#define IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT0_NUM_MAX 15 ///< Max of DevSleep0 Port Number

///Aggresive SATA Device Sleep Port 1
typedef enum {
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_DISABLE = 0,///<Disable
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_ENABLE = 1,///<Enable
  IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1_AUTO = 0xF,///<Auto
} IDSOPT_DBG_FCH_SATA_AGGRESIVE_DEV_SLP_P1;

///DevSleep1 Port Number
///SATA port number map to DEVSLP pad 1: 0-7: SATA 0 ports; 8-15: SATA 1 ports
#define IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT1_NUM_MIN 0 ///< Min of DevSleep1 Port Number
#define IDSOPT_DBG_FCH_SATA_DEV_SLP_PORT1_NUM_MAX 15 ///< Max of DevSleep1 Port Number

///USB0 controller enable
///Enable or disable USB3 controller.
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_ENABLE;

///USB1 controller enable
///Enable or disable USB3 controller.
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_ENABLE;

///USB2 controller enable
///Enable or disable USB3 controller.
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I2_ENABLE;

///USB0 2.0 Port 0
///Disable or Enable USB0 2.0 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT0;

///USB0 2.0 Port 1
///Disable or Enable USB0 2.0 port 1
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT1;

///USB0 2.0 Port 2
///Disable or Enable USB0 2.0 port 2
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT2;

///USB0 2.0 Port 3
///Disable or Enable USB0 2.0 port 3
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_HS_PORT3;

///USB1 2.0 Port 0
///Disable or Enable USB1 2.0 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT0;

///USB1 2.0 Port 1
///Disable or Enable USB1 2.0 port 1
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT1;

///USB1 2.0 Port 2
///Disable or Enable USB1 2.0 port 2
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_HS_PORT2;

///USB2 2.0 Port 0
///Disable or Enable USB2 2.0 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I2_HS_PORT0;

///USB3 2.0 port 0
///Disable or Enable USB3 2.0 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I3_HS_PORT0;

///USB4 2.0 port0
///Disable or Enable USB4 2.0 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I4_HS_PORT0;

///USB0 3.1 Port 0
///Disable or Enable USB0 3.1 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT0;

///USB0 3.1 Port 1
///Disable or Enable USB0 3.1 port 1
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I0_SS_PORT1;

///USB1 3.1 Port 0
///Disable or Enable USB1 3.1 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT0;

///USB1 3.1 Port 1
///Disable or Enable USB1 3.1 port 1
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I1_SS_PORT1;

///USB3 3.1 Port 0
///Disable or Enable USB3 3.1 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I3_SS_PORT0;

///USB4 3.1 Port 0
///Disable or Enable USB4 3.1 port 0
typedef enum {
  IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_USB_XHC_I4_SS_PORT0;

///Ac Loss Control
///Select Ac Loss Control Method
typedef enum {
  IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_ALWAYSOFF = 0,///<Always Off
  IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_ALWAYSON = 1,///<Always On
  IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_RESERVED = 2,///<Reserved
  IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_PREVIOUS = 3,///<Previous
  IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SYSTEM_PWR_FAIL_SHADOW;

///Uart 0 Enable
///Uart 0 has no HW FC if Uart 1 is enabled
typedef enum {
  IDSOPT_CMN_FCH_UART0_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART0_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_UART0_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART0_CONFIG;

///Uart 0 Legacy Options
typedef enum {
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X2E8 = 1,///<0x2E8
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X2F8 = 2,///<0x2F8
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X3E8 = 3,///<0x3E8
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_0X3F8 = 4,///<0x3F8
  IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART0_LEGACY_CONFIG;

///Uart 1 Enable (no HW FC)
typedef enum {
  IDSOPT_CMN_FCH_UART1_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART1_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_UART1_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART1_CONFIG;

///Uart 1 Legacy Options
typedef enum {
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X2E8 = 1,///<0x2E8
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X2F8 = 2,///<0x2F8
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X3E8 = 3,///<0x3E8
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_0X3F8 = 4,///<0x3F8
  IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART1_LEGACY_CONFIG;

///Uart 2 Enable
///Uart 2 has no HW FC if Uart 3 is enabled
typedef enum {
  IDSOPT_CMN_FCH_UART2_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART2_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_UART2_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART2_CONFIG;

///Uart 2 Legacy Options
typedef enum {
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X2E8 = 1,///<0x2E8
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X2F8 = 2,///<0x2F8
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X3E8 = 3,///<0x3E8
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_0X3F8 = 4,///<0x3F8
  IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART2_LEGACY_CONFIG;

///Uart 3 Enable (no HW FC)
typedef enum {
  IDSOPT_CMN_FCH_UART3_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART3_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_UART3_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART3_CONFIG;

///Uart 3 Legacy Options
typedef enum {
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X2E8 = 1,///<0x2E8
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X2F8 = 2,///<0x2F8
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X3E8 = 3,///<0x3E8
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_0X3F8 = 4,///<0x3F8
  IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART3_LEGACY_CONFIG;

///Uart 4 Enable
typedef enum {
  IDSOPT_CMN_FCH_UART4_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_UART4_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_UART4_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_UART4_CONFIG;

///ESPI Enable
typedef enum {
  IDSOPT_CMN_FCH_ESPI_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_ESPI_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_ESPI_CONFIG_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_FCH_ESPI_CONFIG;

///ESPI CRC Enable
/// Enable or Disable ESPI CRC feature
typedef enum {
  IDSOPT_CMN_FCH_ESPI_CRC_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_ESPI_CRC_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_ESPI_CRC_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_FCH_ESPI_CRC;

///XGBE0 enable
///Enable or disable Ethernet controller.
typedef enum {
  IDSOPT_CMN_FCH_XGBE0_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_XGBE0_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_XGBE0_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_XGBE0_ENABLE;

///XGBE1 enable
///Enable or disable Ethernet controller.
typedef enum {
  IDSOPT_CMN_FCH_XGBE1_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_XGBE1_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_XGBE1_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_XGBE1_ENABLE;

///XGBE Port0 LED1 GPIO
///GPIO number for XGBE Port0 LED1.\nWarning: Duplicated GPIO number should not entered for these GPIO number options and only unused GPIOs should be used for these LEDs otherwise behavior will be unknown
#define IDSOPT_CMN_FCH_XGBE_PORT0_LED1_GPIO_MIN 0 ///< Min of XGBE Port0 LED1 GPIO
#define IDSOPT_CMN_FCH_XGBE_PORT0_LED1_GPIO_MAX 0x9d ///< Max of XGBE Port0 LED1 GPIO

///XGBE Port0 LED2 GPIO
///GPIO number for XGBE Port0 LED2.\nWarning: Duplicated GPIO number should not entered for these GPIO number options and only unused GPIOs should be used for these LEDs otherwise behavior will be unknown
#define IDSOPT_CMN_FCH_XGBE_PORT0_LED2_GPIO_MIN 0 ///< Min of XGBE Port0 LED2 GPIO
#define IDSOPT_CMN_FCH_XGBE_PORT0_LED2_GPIO_MAX 0x9d ///< Max of XGBE Port0 LED2 GPIO

///XGBE Port1 LED1 GPIO
///GPIO number for XGBE Port1 LED1.\nWarning: Duplicated GPIO number should not entered for these GPIO number options and only unused GPIOs should be used for these LEDs otherwise behavior will be unknown
#define IDSOPT_CMN_FCH_XGBE_PORT1_LED1_GPIO_MIN 0 ///< Min of XGBE Port1 LED1 GPIO
#define IDSOPT_CMN_FCH_XGBE_PORT1_LED1_GPIO_MAX 0x9d ///< Max of XGBE Port1 LED1 GPIO

///XGBE Port1 LED2 GPIO
///GPIO number for XGBE Port1 LED2.\nWarning: Duplicated GPIO number should not entered for these GPIO number options and only unused GPIOs should be used for these LEDs otherwise behavior will be unknown
#define IDSOPT_CMN_FCH_XGBE_PORT1_LED2_GPIO_MIN 0 ///< Min of XGBE Port1 LED2 GPIO
#define IDSOPT_CMN_FCH_XGBE_PORT1_LED2_GPIO_MAX 0x9d ///< Max of XGBE Port1 LED2 GPIO

///USB4 pre-CM Enable
typedef enum {
  IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_CIO_PRE_CM_ENABLE;

///RT0 Router Enable
///Enable/Disable USB4 Router 0
typedef enum {
  IDSOPT_CMN_FCH_CIO_RT0_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_CIO_RT0_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_CIO_RT0_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_CIO_RT0_ENABLE;

///RT0 PCIe Tunnel
///Enable/Disable Router 0 PCIe tunnel
typedef enum {
  IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_CIO_RT0_PCIE_TUNNEL;

///RT1 Router Enable
///Enable/Disable USB4 Router 1
typedef enum {
  IDSOPT_CMN_FCH_CIO_RT1_ENABLE_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_CIO_RT1_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_CIO_RT1_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_CIO_RT1_ENABLE;

///RT1 PCIe Tunnel
///Enable/Disable Router 1 PCIe tunnel
typedef enum {
  IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_CIO_RT1_PCIE_TUNNEL;

///HFP Enable
typedef enum {
  IDSOPT_CMN_FCH_HFP_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_HFP_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_HFP_CONFIG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_HFP_CONFIG;

///HID Enable
typedef enum {
  IDSOPT_CMN_FCH_HID_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_HID_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_HID_CONFIG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_HID_CONFIG;

///HID Spi Read Mode
typedef enum {
  IDSOPT_CMN_FCH_HID_SPI_READ_MODE_QUADIO144 = 5,///<Quad IO 1-4-4
  IDSOPT_CMN_FCH_HID_SPI_READ_MODE_DUALIO122 = 4,///<Dual IO 1-2-2
  IDSOPT_CMN_FCH_HID_SPI_READ_MODE_FASTREAD111 = 7,///<Fast Read 1-1-1
} IDSOPT_CMN_FCH_HID_SPI_READ_MODE;

///HID Spi Speed
typedef enum {
  IDSOPT_CMN_FCH_HID_SPEED_133HMZ = 1,///<(1)33HMz
  IDSOPT_CMN_FCH_HID_SPEED_222NMZ = 2,///<(2)22NMz
  IDSOPT_CMN_FCH_HID_SPEED_3176MHZ = 3,///<(3)17.6Mhz
  IDSOPT_CMN_FCH_HID_SPEED_5800KHZ = 5,///<(5)800Khz
  IDSOPT_CMN_FCH_HID_SPEED_SPI_SPD6 = 6,///<spi_spd6
  IDSOPT_CMN_FCH_HID_SPEED_SPI_SPD7 = 7,///<spi_spd7
} IDSOPT_CMN_FCH_HID_SPEED;

///HID Spi Speed6 N
///Spi Speed6 = 200Mhz/HID Spi Speed6 N
#define IDSOPT_CMN_FCH_HID_SPI_SPEED6_N_MIN 4 ///< Min of HID Spi Speed6 N
#define IDSOPT_CMN_FCH_HID_SPI_SPEED6_N_MAX 63 ///< Max of HID Spi Speed6 N

///HID Spi Speed7 N
///Spi Speed7 = 200Mhz/HID Spi Speed7 N
#define IDSOPT_CMN_FCH_HID_SPI_SPEED7_N_MIN 4 ///< Min of HID Spi Speed7 N
#define IDSOPT_CMN_FCH_HID_SPI_SPEED7_N_MAX 63 ///< Max of HID Spi Speed7 N

///HID2 Enable
typedef enum {
  IDSOPT_CMN_FCH_HID2_CONFIG_DISABLED = 0,///<Disabled
  IDSOPT_CMN_FCH_HID2_CONFIG_ENABLED = 1,///<Enabled
  IDSOPT_CMN_FCH_HID2_CONFIG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_HID2_CONFIG;

///HID2 Spi Read Mode
typedef enum {
  IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_QUADIO144 = 5,///<Quad IO 1-4-4
  IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_DUALIO122 = 4,///<Dual IO 1-2-2
  IDSOPT_CMN_FCH_HID2_SPI_READ_MODE_FASTREAD111 = 7,///<Fast Read 1-1-1
} IDSOPT_CMN_FCH_HID2_SPI_READ_MODE;

///HID2 Spi Speed
typedef enum {
  IDSOPT_CMN_FCH_HID2_SPEED_133HMZ = 1,///<(1)33HMz
  IDSOPT_CMN_FCH_HID2_SPEED_222NMZ = 2,///<(2)22NMz
  IDSOPT_CMN_FCH_HID2_SPEED_3176MHZ = 3,///<(3)17.6Mhz
  IDSOPT_CMN_FCH_HID2_SPEED_5800KHZ = 5,///<(5)800Khz
  IDSOPT_CMN_FCH_HID2_SPEED_SPI_SPD6 = 6,///<spi_spd6
  IDSOPT_CMN_FCH_HID2_SPEED_SPI_SPD7 = 7,///<spi_spd7
} IDSOPT_CMN_FCH_HID2_SPEED;

///HID2 Spi Speed6 N
///Spi Speed6 = 200Mhz/HID2 Spi Speed6 N
#define IDSOPT_CMN_FCH_HID2_SPI_SPEED6_N_MIN 4 ///< Min of HID2 Spi Speed6 N
#define IDSOPT_CMN_FCH_HID2_SPI_SPEED6_N_MAX 63 ///< Max of HID2 Spi Speed6 N

///HID2 Spi Speed7 N
///Spi Speed7 = 200Mhz/HID2 Spi Speed7 N
#define IDSOPT_CMN_FCH_HID2_SPI_SPEED7_N_MIN 4 ///< Min of HID2 Spi Speed7 N
#define IDSOPT_CMN_FCH_HID2_SPI_SPEED7_N_MAX 63 ///< Max of HID2 Spi Speed7 N

///Reset after sync flood
///Enable AB to forward downstream sync-flood message to system\ncontroller.
typedef enum {
  IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_ENABLE = 1,///<Enable
  IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_DISABLE = 0,///<Disable
  IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE_AUTO = 0xf,///<Auto
} IDSOPT_CMN_FCH_SYNCFLOOD_ENABLE;

///System Configuration
///Warning: Select System Configuration may cause the system to hang, as some System Configuration may not be supported by your OPN.\n
typedef enum {
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_10W = 1,///<10W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_15W = 2,///<15W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_25W = 3,///<25W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_35W = 6,///<35W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_45W = 7,///<45W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_54W = 8,///<54W
  IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG_AUTO = 0xf,///<Auto
} IDSOPT_CMN_GNB_SMU_SYSTEM_CONFIG;

///Sustained Power Limit
///Sustained Power Limit
#define IDSOPT_DBG_SUSTAINED_POWER_LIMIT_MIN 0 ///< Min of Sustained Power Limit
#define IDSOPT_DBG_SUSTAINED_POWER_LIMIT_MAX 0xffffffff ///< Max of Sustained Power Limit

///PPT Control
///Auto = Use the default PPT Limits\nManual = User can set customized PPT Limits
typedef enum {
  IDSOPT_DBG_PPT_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_PPT_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_PPT_CTL;

///Fast PPT Limit
///Fast PPT Limit [mW]
#define IDSOPT_DBG_FAST_PPT_LIMIT_MIN 0 ///< Min of Fast PPT Limit
#define IDSOPT_DBG_FAST_PPT_LIMIT_MAX 0xffffffff ///< Max of Fast PPT Limit

///Slow PPT Limit
///Slow PPT Limit [mW]
#define IDSOPT_DBG_SLOW_PPT_LIMIT_MIN 0 ///< Min of Slow PPT Limit
#define IDSOPT_DBG_SLOW_PPT_LIMIT_MAX 0xffffffff ///< Max of Slow PPT Limit

///Slow PPT Time Constant
///Slow PPT Time Constant [seconds]
#define IDSOPT_DBG_SLOW_PPT_TIME_CONSTANT_MIN 0 ///< Min of Slow PPT Time Constant
#define IDSOPT_DBG_SLOW_PPT_TIME_CONSTANT_MAX 0xffffffff ///< Max of Slow PPT Time Constant

///STAPM Control
///Auto = Use the default STAPM settings\nManual = User can set customized STAPM settings
typedef enum {
  IDSOPT_DBG_STAPM_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_STAPM_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_STAPM_CTL;

///System Temperature Tracking
///0 = System Temperature Tracking disabled\n1 = System Temperature Tracking enabled
typedef enum {
  IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_AUTO = 0xF,///<Auto
  IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_1 = 1,///<1
  IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING_0 = 0,///<0
} IDSOPT_DBG_SYSTEM_TEMPERATURE_TRACKING;

///STAPM Boost Override
///1 = use value in STAPM_BOOST\n0 = use SMU FW default
typedef enum {
  IDSOPT_DBG_STAPM_BOOST_OVERRIDE_AUTO = 0xF,///<Auto
  IDSOPT_DBG_STAPM_BOOST_OVERRIDE_0 = 1,///<0
  IDSOPT_DBG_STAPM_BOOST_OVERRIDE_1 = 0,///<1
} IDSOPT_DBG_STAPM_BOOST_OVERRIDE;

///STAPM Boost
///0 = STAPM Boost Disabled\n1 = STAPM Boost Enabled
typedef enum {
  IDSOPT_DBG_STAPM_BOOST_AUTO = 0xF,///<Auto
  IDSOPT_DBG_STAPM_BOOST_1 = 1,///<1
  IDSOPT_DBG_STAPM_BOOST_0 = 0,///<0
} IDSOPT_DBG_STAPM_BOOST;

///Tskin Time Constant
///Tskin time constant [seconds]
#define IDSOPT_DBG_TSKIN_TIME_CONSTANT_MIN 0 ///< Min of Tskin Time Constant
#define IDSOPT_DBG_TSKIN_TIME_CONSTANT_MAX 0xffffffff ///< Max of Tskin Time Constant

///Thermal Control
///Auto = Use the default TctlMax\nManual = User can set customized TctlMax
typedef enum {
  IDSOPT_DBG_THM_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_THM_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_THM_CTL;

///TjMax
///Maximum operating temperature ['C] (IRM limit will be enforced)
#define IDSOPT_DBG_TJ_MAX_LIMIT_MIN 0 ///< Min of TjMax
#define IDSOPT_DBG_TJ_MAX_LIMIT_MAX 0xffffffff ///< Max of TjMax

///TDC Control
///Auto = Use the default TDC limits\nManual = User can set customized TDC limits
typedef enum {
  IDSOPT_DBG_TDC_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_TDC_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_TDC_CTL;

///TDC_VDDCR_VDD
///VDDCR_VDD TDC Limit [mA] (IRM limit will be enforced)
#define IDSOPT_DBG_TDCVDD_LIMIT_MIN 0 ///< Min of TDC_VDDCR_VDD
#define IDSOPT_DBG_TDCVDD_LIMIT_MAX 0xffffffff ///< Max of TDC_VDDCR_VDD

///TDC_VDDCR_SOC
///VDDCR_SOC TDC Limit [mA]
#define IDSOPT_DBG_TDCSOC_LIMIT_MIN 0 ///< Min of TDC_VDDCR_SOC
#define IDSOPT_DBG_TDCSOC_LIMIT_MAX 0xffffffff ///< Max of TDC_VDDCR_SOC

///TDC_VDDCR_GFX
///VDDCR_GFX TDC Limit [mA]
#define IDSOPT_DBG_TDCGFX_LIMIT_MIN 0 ///< Min of TDC_VDDCR_GFX
#define IDSOPT_DBG_TDCGFX_LIMIT_MAX 0xffffffff ///< Max of TDC_VDDCR_GFX

///TDC_VDDCR_CVIP
///VDDCR_CVIP TDC Limit [mA]
#define IDSOPT_DBG_TDCCVIP_LIMIT_MIN 0 ///< Min of TDC_VDDCR_CVIP
#define IDSOPT_DBG_TDCCVIP_LIMIT_MAX 0xffffffff ///< Max of TDC_VDDCR_CVIP

///EDC Control
///Auto = Use the default EDC limits\nManual = User can set customized EDC limits
typedef enum {
  IDSOPT_DBG_EDC_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_EDC_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_EDC_CTL;

///EDC_VDDCR_VDD
///VDDCR_VDD EDC Limit [mA]  (IRM limit will be enforced)
#define IDSOPT_DBG_EDCVDD_LIMIT_MIN 0 ///< Min of EDC_VDDCR_VDD
#define IDSOPT_DBG_EDCVDD_LIMIT_MAX 0xffffffff ///< Max of EDC_VDDCR_VDD

///EDC_VDDCR_SOC
///VDDCR_SOC EDC Limit [mA  (IRM limit will be enforced)
#define IDSOPT_DBG_EDCSOC_LIMIT_MIN 0 ///< Min of EDC_VDDCR_SOC
#define IDSOPT_DBG_EDCSOC_LIMIT_MAX 0xffffffff ///< Max of EDC_VDDCR_SOC

///EDC_VDDCR_GFX
///VDDCR_GFX EDC Limit [mA (IRM limit will be enforced)
#define IDSOPT_DBG_EDCGFX_LIMIT_MIN 0 ///< Min of EDC_VDDCR_GFX
#define IDSOPT_DBG_EDCGFX_LIMIT_MAX 0xffffffff ///< Max of EDC_VDDCR_GFX

///PSI3 Control
///Auto = Use the default PSI3 limits\nManual = User can set customized PSI3 limits
typedef enum {
  IDSOPT_DBG_PS_I3_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_PS_I3_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_PS_I3_CTL;

///PSI3_VDDCR_VDD
///VDDCR_VDD PSI3 Limit [mA] (IRM limit will be enforced)
#define IDSOPT_DBG_PS_I3_VDD_LIMIT_MIN 0 ///< Min of PSI3_VDDCR_VDD
#define IDSOPT_DBG_PS_I3_VDD_LIMIT_MAX 0xffffffff ///< Max of PSI3_VDDCR_VDD

///PSI3_VDDCR_GFX
///VDDCR_GFX PSI3 Limit [mA] (IRM limit will be enforced)
#define IDSOPT_DBG_PS_I3_GFX_LIMIT_MIN 0 ///< Min of PSI3_VDDCR_GFX
#define IDSOPT_DBG_PS_I3_GFX_LIMIT_MAX 0xffffffff ///< Max of PSI3_VDDCR_GFX

///PROCHOT Control
///Auto = Use the default PROCHOT deassertion ramp time\nManual = User can set customized PROCHOT deassertion ramp time
typedef enum {
  IDSOPT_DBG_PROCHOT_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_PROCHOT_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_PROCHOT_CTL;

///PROCHOT Deassertion Ramp Time
///PROCHOT deassertion ramp time [ms] (IRM limit will be enforced)
#define IDSOPT_DBG_PROCHOT_RAMP_TIME_MIN 0 ///< Min of PROCHOT Deassertion Ramp Time
#define IDSOPT_DBG_PROCHOT_RAMP_TIME_MAX 0xffffffff ///< Max of PROCHOT Deassertion Ramp Time

///STT Control
///Auto = Use the default STT controller settings\nManual = User can set customized STT controller settings
typedef enum {
  IDSOPT_DBG_STT_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_STT_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_STT_CTL;

///STT_PCB_SENSOR_COUNT
#define IDSOPT_CMN_STT_PCB_SENSOR_COUNT_MIN 0 ///< Min of STT_PCB_SENSOR_COUNT
#define IDSOPT_CMN_STT_PCB_SENSOR_COUNT_MAX 0xf ///< Max of STT_PCB_SENSOR_COUNT

///STT_MIN_POWER_LIMIT
///Specifies the minimum sustained power that the APU power is allowed to reduce to in mWatts.
#define IDSOPT_CMN_STT_MIN_LIMIT_MIN 0 ///< Min of STT_MIN_POWER_LIMIT
#define IDSOPT_CMN_STT_MIN_LIMIT_MAX 65535 ///< Max of STT_MIN_POWER_LIMIT

///STT_M1
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M1_MIN 0 ///< Min of STT_M1
#define IDSOPT_CMN_STT_M1_MAX 0xffff ///< Max of STT_M1

///STT_M2
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M2_MIN 0 ///< Min of STT_M2
#define IDSOPT_CMN_STT_M2_MAX 0xffff ///< Max of STT_M2

///STT_M3
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M3_MIN 0 ///< Min of STT_M3
#define IDSOPT_CMN_STT_M3_MAX 0xffff ///< Max of STT_M3

///STT_M4
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M4_MIN 0 ///< Min of STT_M4
#define IDSOPT_CMN_STT_M4_MAX 0xffff ///< Max of STT_M4

///STT_M5
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M5_MIN 0 ///< Min of STT_M5
#define IDSOPT_CMN_STT_M5_MAX 0xffff ///< Max of STT_M5

///STT_M6
///Signed Q6.10 encoding
#define IDSOPT_CMN_STT_M6_MIN 0 ///< Min of STT_M6
#define IDSOPT_CMN_STT_M6_MAX 0xffff ///< Max of STT_M6

///STT_C_APU
///Signed Q8.8 encoding
#define IDSOPT_CMN_STTCAPU_MIN 0 ///< Min of STT_C_APU
#define IDSOPT_CMN_STTCAPU_MAX 0xffff ///< Max of STT_C_APU

///STT_C_HS2
///Signed Q8.8 encoding
#define IDSOPT_CMN_STTC_HS2_MIN 0 ///< Min of STT_C_HS2
#define IDSOPT_CMN_STTC_HS2_MAX 0xffff ///< Max of STT_C_HS2

///STT_ALPHA_APU
#define IDSOPT_CMN_STT_ALPHA_APU_MIN 0 ///< Min of STT_ALPHA_APU
#define IDSOPT_CMN_STT_ALPHA_APU_MAX 0xffff ///< Max of STT_ALPHA_APU

///STT_ALPHA_HS2
#define IDSOPT_CMN_STT_ALPHA_H_S2_MIN 0 ///< Min of STT_ALPHA_HS2
#define IDSOPT_CMN_STT_ALPHA_H_S2_MAX 0xffff ///< Max of STT_ALPHA_HS2

///STT_SKIN_TEMPERATURE_LIMIT_APU
#define IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_APU_MIN 0 ///< Min of STT_SKIN_TEMPERATURE_LIMIT_APU
#define IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_APU_MAX 0xffff ///< Max of STT_SKIN_TEMPERATURE_LIMIT_APU

///STT_SKIN_TEMPERATURE_LIMIT_HS2
#define IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_HS2_MIN 0 ///< Min of STT_SKIN_TEMPERATURE_LIMIT_HS2
#define IDSOPT_CMN_STT_SKIN_TEMPERATURE_LIMIT_HS2_MAX 0xffff ///< Max of STT_SKIN_TEMPERATURE_LIMIT_HS2

///STT_ERROR_COEFF
#define IDSOPT_CMN_STT_ERROR_COEFF_MIN 0 ///< Min of STT_ERROR_COEFF
#define IDSOPT_CMN_STT_ERROR_COEFF_MAX 0xffff ///< Max of STT_ERROR_COEFF

///STT_ERROR_RATE_COEFF
#define IDSOPT_CMN_STT_ERROR_RATE_COEFF_MIN 0 ///< Min of STT_ERROR_RATE_COEFF
#define IDSOPT_CMN_STT_ERROR_RATE_COEFF_MAX 0xffff ///< Max of STT_ERROR_RATE_COEFF

///Fan Control
///Auto = Use the default fan controller settings\nManual = User can set customized fan controller settings
typedef enum {
  IDSOPT_DBG_FAN_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_FAN_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_FAN_CTL;

///Force PWM Control
///Unforce = Do not force the fan PWM\nForce = Force the fan PWM to the use specified value
typedef enum {
  IDSOPT_DBG_FORCE_PWM_CTL_FORCE = 1,///<Force
  IDSOPT_DBG_FORCE_PWM_CTL_UNFORCE = 0,///<Unforce
} IDSOPT_DBG_FORCE_PWM_CTL;

///Force PWM
///Specify the PWM to force the fan to [0-100]
#define IDSOPT_DBG_FORCE_PWM_MIN 0 ///< Min of Force PWM
#define IDSOPT_DBG_FORCE_PWM_MAX 100 ///< Max of Force PWM

///Fan Table Control
///Auto = Use the default fan table\nManual = User can set customized fan table
typedef enum {
  IDSOPT_DBG_FAN_TABLE_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_FAN_TABLE_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_FAN_TABLE_CTL;

///Low Temperature
///Low Temperature ['C]
#define IDSOPT_DBG_FAN_TABLE_TEMP_LOW_MIN 0 ///< Min of Low Temperature
#define IDSOPT_DBG_FAN_TABLE_TEMP_LOW_MAX 0xff ///< Max of Low Temperature

///Medium Temperature
///Medium Temperature ['C]
#define IDSOPT_DBG_FAN_TABLE_TEMP_MED_MIN 0 ///< Min of Medium Temperature
#define IDSOPT_DBG_FAN_TABLE_TEMP_MED_MAX 0xff ///< Max of Medium Temperature

///High Temperature
///High Temperature ['C]
#define IDSOPT_DBG_FAN_TABLE_TEMP_HIGH_MIN 0 ///< Min of High Temperature
#define IDSOPT_DBG_FAN_TABLE_TEMP_HIGH_MAX 0xff ///< Max of High Temperature

///Critical Temperature
///Critical Temperature ['C]
#define IDSOPT_DBG_FAN_TABLE_TEMP_CRITICAL_MIN 0 ///< Min of Critical Temperature
#define IDSOPT_DBG_FAN_TABLE_TEMP_CRITICAL_MAX 0xff ///< Max of Critical Temperature

///Low Pwm
///Low Pwm [0-100]
#define IDSOPT_DBG_FAN_TABLE_T_PWM_LOW_MIN 0 ///< Min of Low Pwm
#define IDSOPT_DBG_FAN_TABLE_T_PWM_LOW_MAX 100 ///< Max of Low Pwm

///Medium Pwm
///Medium Pwm [0-100]
#define IDSOPT_DBG_FAN_TABLE_PWM_MED_MIN 0 ///< Min of Medium Pwm
#define IDSOPT_DBG_FAN_TABLE_PWM_MED_MAX 101 ///< Max of Medium Pwm

///High Pwm
///High Pwm [0-100]
#define IDSOPT_DBG_FAN_TABLE_PWM_HIGH_MIN 0 ///< Min of High Pwm
#define IDSOPT_DBG_FAN_TABLE_PWM_HIGH_MAX 102 ///< Max of High Pwm

///Temperature Hysteresis
///Temperature Hysteresis ['C]
#define IDSOPT_DBG_FAN_TABLE_HYST_MIN 0 ///< Min of Temperature Hysteresis
#define IDSOPT_DBG_FAN_TABLE_HYST_MAX 0xff ///< Max of Temperature Hysteresis

///Pwm Frequency
///0 = 25kHz\n1 = 100Hz
typedef enum {
  IDSOPT_DBG_FAN_TABLE_PWM_FREQ_AUTO = 0xF,///<Auto
  IDSOPT_DBG_FAN_TABLE_PWM_FREQ_1 = 1,///<1
  IDSOPT_DBG_FAN_TABLE_PWM_FREQ_0 = 0,///<0
} IDSOPT_DBG_FAN_TABLE_PWM_FREQ;

///Fan polarity
///0 => Negative\n1 => Positive
typedef enum {
  IDSOPT_DBG_FAN_POLARITY_AUTO = 0xF,///<Auto
  IDSOPT_DBG_FAN_POLARITY_1 = 1,///<1
  IDSOPT_DBG_FAN_POLARITY_0 = 0,///<0
} IDSOPT_DBG_FAN_POLARITY;

///VDDP Voltage Control
///Auto = Use default VDDP voltage\nManual = User can set custom VDDP voltage
typedef enum {
  IDSOPT_DBG_VDDP_VOLTAGE_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_VDDP_VOLTAGE_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_VDDP_VOLTAGE_CTL;

///VDDP Voltage
///Specify the target VDDP voltage [mV]
#define IDSOPT_DBG_VDDP_VOLTAGE_MIN 0 ///< Min of VDDP Voltage
#define IDSOPT_DBG_VDDP_VOLTAGE_MAX 0x7d0 ///< Max of VDDP Voltage

///VDDM Voltage Control
///Auto = Use default VDDM voltage\nManual = User can set custom VDDM voltage
typedef enum {
  IDSOPT_DBG_VDDM_VOLTAGE_CTL_MANUAL = 1,///<Manual
  IDSOPT_DBG_VDDM_VOLTAGE_CTL_AUTO = 0,///<Auto
} IDSOPT_DBG_VDDM_VOLTAGE_CTL;

///VDDM Voltage
///Specify the target VDDM voltage [mV]
#define IDSOPT_DBG_VDDM_VOLTAGE_MIN 0 ///< Min of VDDM Voltage
#define IDSOPT_DBG_VDDM_VOLTAGE_MAX 0x7d0 ///< Max of VDDM Voltage

///Zstates (Z9 and Z10)
///Enable = Enable the feature : Disable = Disable the feature
typedef enum {
  IDSOPT_CMN_GNB_SMU_ZSTATES_DISABLED = 0,///<Disabled
  IDSOPT_CMN_GNB_SMU_ZSTATES_ENABLED = 1,///<Enabled
  IDSOPT_CMN_GNB_SMU_ZSTATES_AUTO = 0xF,///<Auto
} IDSOPT_CMN_GNB_SMU_ZSTATES;

///CclkFmaxOverride Control
///Auto = Use default CclkFmaxOverride settings\nManual = User can set custom CclkFmaxOverride
typedef enum {
  IDSOPT_CMN_CCLK_FMAX_CTL_MANUAL = 1,///<Manual
  IDSOPT_CMN_CCLK_FMAX_CTL_AUTO = 0,///<Auto
} IDSOPT_CMN_CCLK_FMAX_CTL;

///CclkFmaxOverride
///Specify the CCLK Fmax override [MHz]
#define IDSOPT_CMN_CCLK_FMAX_OVERRIDE_MIN 0 ///< Min of CclkFmaxOverride
#define IDSOPT_CMN_CCLK_FMAX_OVERRIDE_MAX 65535 ///< Max of CclkFmaxOverride

///DF Pstate P0 Ctl
typedef enum {
  IDSOPT_CMN_DF_PSTATE_P0_CTL_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_DF_PSTATE_P0_CTL_MANUAL = 1,///<Manual
} IDSOPT_CMN_DF_PSTATE_P0_CTL;

///DF Pstate Fclk P0
///Fclk frequency in MHZ
typedef enum {
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_333MHZ = 333,///<333MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_400MHZ = 400,///<400MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_500MHZ = 500,///<500MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_533MHZ = 533,///<533MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_667MHZ = 667,///<667MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_733MHZ = 733,///<733MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_800MHZ = 800,///<800MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_933MHZ = 933,///<933MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1067MHZ = 1067,///<1067MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1200MHZ = 1200,///<1200MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1300MHZ = 1300,///<1300MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1375MHZ = 1375,///<1375MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1466MHZ = 1466,///<1466MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1600MHZ = 1600,///<1600MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_1800MHZ = 1800,///<1800MHZ
  IDSOPT_CMN_DF_PSTATE_FCLK_P0_2000MHZ = 2000,///<2000MHZ
} IDSOPT_CMN_DF_PSTATE_FCLK_P0;

///DF Pstate MemClk P0
///Mem frequency in MHZ
typedef enum {
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_167MHZ = 167,///<167MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_200MHZ = 200,///<200MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_267MHZ = 267,///<267MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_333MHZ = 333,///<333MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_367MHZ = 367,///<367MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_400MHZ = 400,///<400MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_467MHZ = 467,///<467MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_533MHZ = 533,///<533MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_600MHZ = 600,///<600MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_687MHZ = 687,///<687MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_733MHZ = 733,///<733MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_800MHZ = 800,///<800MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_933MHZ = 933,///<933MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1000MHZ = 1000,///<1000MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1600MHZ = 1600,///<1600MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_1800MHZ = 1800,///<1800MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2000MHZ = 2000,///<2000MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2400MHZ = 2400,///<2400MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2600MHZ = 2600,///<2600MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_2800MHZ = 2800,///<2800MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_3000MHZ = 3000,///<3000MHZ
  IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0_3200MHZ = 3200,///<3200MHZ
} IDSOPT_CMN_DF_PSTATE_MEM_CLK_P0;

///Fixed FCLK
///Set the Fclk Value\nNote: Changing this will disable FCLK DPM and\nMore than one level can have the same frequency value
typedef enum {
  IDSOPT_CMN_FCLK_VALUE_LEVEL0 = 0,///<Level 0
  IDSOPT_CMN_FCLK_VALUE_LEVEL1 = 1,///<Level 1
  IDSOPT_CMN_FCLK_VALUE_LEVEL2 = 2,///<Level 2
  IDSOPT_CMN_FCLK_VALUE_LEVEL3 = 3,///<Level 3
  IDSOPT_CMN_FCLK_VALUE_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_FCLK_VALUE;

///Fixed LCLK
///Set the Lclk value\nNote: Changing this will disable LCLK DPM and\nMore than one level can have the same frequency value
typedef enum {
  IDSOPT_CMN_LCLK_FREQ_LEVEL0 = 0,///<Level 0
  IDSOPT_CMN_LCLK_FREQ_LEVEL1 = 1,///<Level 1
  IDSOPT_CMN_LCLK_FREQ_LEVEL2 = 2,///<Level 2
  IDSOPT_CMN_LCLK_FREQ_LEVEL3 = 3,///<Level 3
  IDSOPT_CMN_LCLK_FREQ_LEVEL4 = 4,///<Level 4
  IDSOPT_CMN_LCLK_FREQ_LEVEL5 = 5,///<Level 5
  IDSOPT_CMN_LCLK_FREQ_LEVEL6 = 6,///<Level 6
  IDSOPT_CMN_LCLK_FREQ_LEVEL7 = 7,///<Level 7
  IDSOPT_CMN_LCLK_FREQ_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_LCLK_FREQ;

///Reset on MP1 SMN Timeout
typedef enum {
  IDSOPT_CMN_RST_ON_MP1_SMN_TIMEOUT_DISABLED = 0,///<Disabled
  IDSOPT_CMN_RST_ON_MP1_SMN_TIMEOUT_ENABLED = 1,///<Enabled
} IDSOPT_CMN_RST_ON_MP1_SMN_TIMEOUT;

///SmartShift Control
typedef enum {
  IDSOPT_CMN_AA_CTL_AUTO = 0xF,///<Auto
  IDSOPT_CMN_AA_CTL_MANUAL = 1,///<Manual
} IDSOPT_CMN_AA_CTL;

///SmartShift Enable
///A+A Support Enable
typedef enum {
  IDSOPT_CMN_AA_EN_AUTO = 0xF,///<Auto
  IDSOPT_CMN_AA_EN_DISABLE = 0,///<Disable
  IDSOPT_CMN_AA_EN_ENABLE = 1,///<Enable
} IDSOPT_CMN_AA_EN;

///APU Only sPPT Limit
///APU Only sPPT Limit in mW
#define IDSOPT_CMN_APU_ONLY_SPPT_LIMIT_MIN 0 ///< Min of APU Only sPPT Limit
#define IDSOPT_CMN_APU_ONLY_SPPT_LIMIT_MAX 0xFFFFFFFF ///< Max of APU Only sPPT Limit

///Sustained PowerLimit
///PcdMsgSetSustainedPowerLimit
#define IDSOPT_CMN_MSG_SET_SUSTAINED_POWER_LIMIT_MIN 0 ///< Min of Sustained PowerLimit
#define IDSOPT_CMN_MSG_SET_SUSTAINED_POWER_LIMIT_MAX 0xFFFFFFFF ///< Max of Sustained PowerLimit

///Fast PPT Limit
///PcdMsgSetFastPPTLimit
#define IDSOPT_CMN_MSG_SET_FAST_PPT_LIMIT_MIN 0 ///< Min of Fast PPT Limit
#define IDSOPT_CMN_MSG_SET_FAST_PPT_LIMIT_MAX 0xFFFFFFFF ///< Max of Fast PPT Limit

///Slow PPT Limit
///PcdMsgSetSlowPPTLimit
#define IDSOPT_CMN_MSG_SE_SLOW_PPT_LIMIT_MIN 0 ///< Min of Slow PPT Limit
#define IDSOPT_CMN_MSG_SE_SLOW_PPT_LIMIT_MAX 0xFFFFFFFF ///< Max of Slow PPT Limit

///PSP RPMC Switch
///Control RPMC usage.\nEnable : Enable RMPC function\nDisable : Disable RMPC function\nAuto : Keep default behavior\n\nThis option is for test purpose only,\nNOT FOR PRODUCTION!!!
typedef enum {
  IDSOPT_CMN_PSP_RPMC_SWITCH_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_PSP_RPMC_SWITCH_DISABLED = 0,///<Disabled
  IDSOPT_CMN_PSP_RPMC_SWITCH_ENABLED = 1,///<Enabled
} IDSOPT_CMN_PSP_RPMC_SWITCH;

///Rom Armor support
///Enable/disable Rom Armor
typedef enum {
  IDSOPT_CMN_ROM_ARMOR_SUPPORT_AUTO = 0xF,///<Auto
  IDSOPT_CMN_ROM_ARMOR_SUPPORT_ENABLED = 3,///<Enabled
  IDSOPT_CMN_ROM_ARMOR_SUPPORT_DISABLED = 0,///<Disabled
} IDSOPT_CMN_ROM_ARMOR_SUPPORT;

///Rom Armor Spi Config Reg Lock
///Enable/disable Rom Armor Spi Config Reg Lock
typedef enum {
  IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_AUTO = 0xF,///<Auto
  IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_ENABLED = 1,///<Enabled
  IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK_DISABLED = 0,///<Disabled
} IDSOPT_CMN_ROM_ARMOR_SPI_CONFIG_REG_LOCK;

///ABL Console Out Control
///Enable : Enable ConsoleOut Function for ABL\nDisable : Disable ConsoleOut Function for ABL\nAuto : Keep default behavior
typedef enum {
  IDSOPT_CMN_SOC_ABL_CON_OUT_AUTO = 2,///<Auto
  IDSOPT_CMN_SOC_ABL_CON_OUT_ENABLE = 1,///<Enable
  IDSOPT_CMN_SOC_ABL_CON_OUT_DISABLE = 0,///<Disable
} IDSOPT_CMN_SOC_ABL_CON_OUT;

///ABL Console Out Serial Port
///Specifies the type of console output serial port.
typedef enum {
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_LPCUART = 0,///<LPC UART
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART0 = 1,///<FCH UART0
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART1 = 2,///<FCH UART1
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART2 = 3,///<FCH UART2
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_FCHUART3 = 4,///<FCH UART3
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_ESPIUART = 5,///<eSPI UART
} IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT;

///ABL Console Out Serial Port IO
///Select Legacy Uart (SIO or eSPI) IO base\n0x3F8:  Set  IO base to 0x3F8\n0x2F8:  Set  IO base to 0x2F8\n0x3E8:  Set  IO base to 0x3E8\n0x2E8:  Set  IO base to 0x2E8\nAuto : Keep default behavior\n  Please make sure the selected eSPI IO base and length has been filled in APCB_FCH_TYPE_ESPI_INIT or APCB_FCH_TYPE_ESPI1_INIT EspiInitConfiguration table.\n
typedef enum {
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X3F8 = 0,///<0x3F8
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X2F8 = 1,///<0x2F8
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X3E8 = 2,///<0x3E8
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_0X2E8 = 3,///<0x2E8
  IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_SOC_ABL_CON_OUT_SERIAL_PORT_IO;

///ABL Console Out Boot Mode Select
typedef enum {
  IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_ALL = 0xFF,///<ALL
  IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_S3S0I3ONLY = 2,///<S3/S0i3 only
  IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE_NORMALBOOTONLY = 1,///<Normal boot only
} IDSOPT_CMN_SOC_ABL_CON_OUT_BOOT_MODE;

///ABL PMU message Control
///To control the total number of PMU debug messages.\nSeveral major controls are listed below:\n1. Maximal debug messages (e.g., Eye contours)\n2. Detailed debug messages (e.g. Eye delays)\n3. Coarse debug messages (e.g. rank information)\n4. Stage completion\n5. Assertion messages\n6. Firmware completion messages only
typedef enum {
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_MAXIMALDEBUGMESSAGES = 0x4,///<Maximal debug messages
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_DETAILEDDEBUGMESSAGE = 0x05,///<Detailed debug message
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_COARSEDEBUGMESSAGE = 0x0A,///<Coarse debug message
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_STAGECOMPLETION = 0xC8,///<Stage completion
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_ASSERTIONMESSAGE = 0xC9,///<Assertion message
  IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL_FIRMWARECOMPLETIONMESSAGEONLY = 0xFE,///<Firmware completion message only
} IDSOPT_CMN_SOC_ABL_PMU_MSG_CTRL;

///Secured-core Auto enablement
///If set to Disable, AGESA will delete EFI variable "BuiltAsSecuredCorePC" if detected\nIf set to Enable, AGESA will set EFI variable "BuiltAsSecuredCorePC" to non-zero value\nIf "BuiltAsSecuredCorePC" set to non-zero, it will identify a device as Secure-core PC\nMore detials refer to "Secured-Core PC Identifier" in MSFT document.\nIf set to Auto, ignore, AGESA will do nothing
typedef enum {
  IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT_DISABLED = 0,///<Disabled
} IDSOPT_CMN_SOC_SCPC_AUTO_ENABLEMENT;

///Mixed DIMM config extended NUMA domain
///Mixed DIMM config extended NUMA domain in SRAT that contain only non-interleaving memory region but no processor.
typedef enum {
  IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_DISABLED = 0,///<Disabled
  IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_ENABLED = 1,///<Enabled
  IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN_AUTO = 0xFF,///<Auto
} IDSOPT_CMN_MIXED_DIMM_CONFIG_NUMA_DOMAIN;

///FAR enforcement state
///Enabled = FAR is permanently enforced in the CPU, the system can only boot from BIOS with update to date firmware stack as defined by SPL table. \nDisabled  = FAR is NOT enforced in the CPU
typedef enum {
  IDSOPT_CMN_SOC_FAR_ENFORCED_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SOC_FAR_ENFORCED_DISABLED = 0,///<Disabled
} IDSOPT_CMN_SOC_FAR_ENFORCED;

///SPL value in the CPU fuse
///The current SPL value in the CPU fuse which is converted from fuse bitmask
#define IDSOPT_CMN_SOC_SPL_FUSE_MIN 0 ///< Min of SPL value in the CPU fuse
#define IDSOPT_CMN_SOC_SPL_FUSE_MAX 0xff ///< Max of SPL value in the CPU fuse

///SPL value in the SPL table
///if Initial SPL value is set to 0, the SPL fuse in the CPU will be upgraded to the SPL value in the SPL table at next boot.
#define IDSOPT_CMN_SOC_SPL_VALUE_IN_TBL_MIN 0 ///< Min of SPL value in the SPL table
#define IDSOPT_CMN_SOC_SPL_VALUE_IN_TBL_MAX 0xff ///< Max of SPL value in the SPL table

///Initial SPL value for FAR
///Initial SPL value is only applicable when FAR switch is enabled.\n\n0 = initial SPL value is not set, SPL fuse will always follow the SPL value in the SPL table.\n\nNot 0 = if FAR has been enforced, BIOS will keep SPL fuse as it is. otherwise,  BIOS will enforce FAR and set SPL fuse to this initial SPL value.
#define IDSOPT_CMN_SOC_INIT_SPL_FOR_FAR_MIN 0 ///< Min of Initial SPL value for FAR
#define IDSOPT_CMN_SOC_INIT_SPL_FOR_FAR_MAX 0xff ///< Max of Initial SPL value for FAR

///FAR Switch
///Enabled  = if initial SPL value is 0, and FAR is NOT enforced, BIOS will enforce FAR and set SPL fuse to SPL value in the SPL table.\n	       if initial SPL value is 0, and FAR is enforced, BIOS will update SPL fuse to SPL value in the SPL table.\n		   if initial SPL value is not 0, and FAR is NOT enforced, BIOS will enforce FAR and set SPL fuse to initial SPL value.\n		   if initial SPL value is not 0, and FAR is enforced, BIOS will keep SPL fuse as it is.\n\nDisabled = BIOS will NOT change FAR enforcement state and NOT set SPL fuse.
typedef enum {
  IDSOPT_CMN_SOC_FAR_SWITCH_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SOC_FAR_SWITCH_DISABLED = 0,///<Disabled
} IDSOPT_CMN_SOC_FAR_SWITCH;

///Pluton (HSP) X86 Firmware Support
///Enable/Disable X86 firmware HSP related code path, including AGESA HSP module, SBIOS HSP related drivers.\nAuto - Depends on PcdAmdHspCoreEnable build value\nNOTE: PSP directory entry 0xB BIT36 have the highest priority.\nNOTE: This option will NOT put HSP hardware in disable state, to disable HSP hardware, you need setup PSP directory entry 0xB, BIT36 to 1.\n// EntryValue[36] = 0: Enable, HSP core is enabled.\n// EntryValue[36] = 1: Disable, HSP core is disabled then PSP will gate the HSP clock, no further PSP to HSP commands. System will boot without HSP.\n\n
typedef enum {
  IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_X86_FW_SUPPORT;

///Pluton (HSP) UART
///HSP Firmware will print debug information to FCH UART, select to enable/disable this function, By default it will be disabled.
typedef enum {
  IDSOPT_CMN_SO_C_MISC_HSP_UART_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_HSP_UART_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_HSP_UART_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_HSP_UART;

///Pluton (HSP) UART Port
///HSP Firmware will print debug information to FCH UART, select which FCH UART port to use. If auto selected, UART2 will be used
typedef enum {
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART0 = 0,///<UART 0
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART1 = 1,///<UART 1
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART2 = 2,///<UART 2
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART3 = 3,///<UART 3
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART4 = 4,///<UART 4
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART5 = 5,///<UART 5
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART6 = 6,///<UART 6
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_UART7 = 7,///<UART 7
  IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_HSP_UART_PORT;

///Pluton (HSP) FIPS
///Enable FIPS mode for HSP
typedef enum {
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_HSP_FIPS;

///Pluton (HSP) FIPS zeroization
///HSP FIPS zeroization enable/disable
typedef enum {
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_HSP_FIPS_ZEROIZATION;

///Pluton (HSP) ACPI Fence Enable
///HSP ACPI fence enable/disable, 0: PCI Fence, 1: ACPI Fence\nThis for debug only, no need to set in production.\nIf set to Auto, ACPI Fence will be set by default except RMB A0\n
typedef enum {
  IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_FALSE = 0,///<FALSE
  IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_TRUE = 1,///<TRUE
  IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_HSP_ACPI_FENCE_ENABLE;

///AGESA MPM support
///Only works on the platform which is AIM-T capable, turn off all MPM related feature for both wired KVM and wireless manageability
typedef enum {
  IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_MPM_SUPPORT;

///KVM for Wired Manageability
///Only works on the platform which is AIM-T capable, Switch to turn off Wired KVM
typedef enum {
  IDSOPT_CMN_SO_C_MISC_WIRED_KVM_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_WIRED_KVM_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_WIRED_KVM_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_WIRED_KVM;

///Wireless Manageability
///Only works on the platform which is AIM-T capable, Turn off All WirelessManageability related functions: KVM, TCR, PLDM, ALERT
typedef enum {
  IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_WIRELESS_MANAGEABILITY;

///KVM for Wireless Manageability
///Only works on the platform which is AIM-T capable , Switch to turn off Wireless KVM
typedef enum {
  IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_WIRELESS_KVM;

///MPM attribute control
///Used to control the active behavior of Wireless Manageability and KVM for Wired Manageability options
typedef enum {
  IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_0 = 0,///<0
  IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_1 = 1,///<1
  IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_2 = 2,///<2
  IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL_3 = 3,///<3
} IDSOPT_CMN_SOC_MISC_MPM_ATTR_CNTL;

///TCR for Wireless Manageability
///Only works on the platform which is AIM-T capable \nSwitch for MPM Text console redirection\n
typedef enum {
  IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_DISABLED = 0,///<Disabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_ENABLED = 1,///<Enabled
  IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR_AUTO = 0xf,///<AUTO
} IDSOPT_CMN_SO_C_MISC_WIRELESS_TCR;

///Intrusion Detection Control
///Intrusion Detection settings should be configured by system administrator only, please input correct password.to show related settings
#define IDSOPT_CMN_INTRUSION_DETECTION_CTL_MIN 0x0000 ///< Min of Intrusion Detection Control
#define IDSOPT_CMN_INTRUSION_DETECTION_CTL_MAX 0xFFFF ///< Max of Intrusion Detection Control

///Intrusion Detection Enable
///Control Intrusion detection feature enablement\n\nEnabled=Intrusion detection feature is enabled\nDisabled=Intrusion detection feature is disabled
typedef enum {
  IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_AUTO = 0xFF,///<Auto
  IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_ENABLED = 1,///<Enabled
  IDSOPT_CMN_INTRUSION_DETECTION_ENABLE_DISABLED = 0,///<Disabled
} IDSOPT_CMN_INTRUSION_DETECTION_ENABLE;

///Log Intrusion Event
///Intrusion event should be logged or not when intrusion is detected\n\nEnabled=System will log intrusion event when intrusion is detected\nDisabled=System will not log intrusion event when intrusion is detected
typedef enum {
  IDSOPT_CMN_INTRUSION_LOG_EVENT_ENABLED = 1,///<Enabled
  IDSOPT_CMN_INTRUSION_LOG_EVENT_DISABLED = 0,///<Disabled
} IDSOPT_CMN_INTRUSION_LOG_EVENT;

///Clear TPM
///TPM should be cleared or not when intrusion is detected\n\nEnabled=System will clear TPM when intrusion is detected\nDisabled=System will not clear TPM when intrusion is detected
typedef enum {
  IDSOPT_CMN_INTRUSION_CLEAR_TPM_ENABLED = 1,///<Enabled
  IDSOPT_CMN_INTRUSION_CLEAR_TPM_DISABLED = 0,///<Disabled
} IDSOPT_CMN_INTRUSION_CLEAR_TPM;

///Power Off System
///System will be powered off forcibly or not when intrusion is detected\n\nEnabled=System will be powered off forcibly when intrusion is detected\nDisabled=System will not be powered off forcibly when intrusion is detected
typedef enum {
  IDSOPT_CMN_INTRUSION_POWER_OFF_ENABLED = 1,///<Enabled
  IDSOPT_CMN_INTRUSION_POWER_OFF_DISABLED = 0,///<Disabled
} IDSOPT_CMN_INTRUSION_POWER_OFF;

///PCIe Port 0
///Enable/Disable Switch Downstream Port 0
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P0;

///PCIe Port 1
///Enable/Disable Switch Downstream Port 1
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P1;

///PCIe Port 2
///Enable/Disable Switch Downstream Port 2
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P2;

///PCIe Port 3
///Enable/Disable Switch Downstream Port 3
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P3;

///PCIe Port 4
///Enable/Disable Switch Downstream Port 4
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P4;

///PCIe Port 5
///Enable/Disable Switch Downstream Port 5
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P5;

///PCIe Port 6
///Enable/Disable Switch Downstream Port 6
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P6;

///PCIe Port 7
///Enable/Disable Switch Downstream Port 7
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P7;

///PCIe Port 8
///Enable/Disable Switch Downstream Port 8
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P8;

///PCIe Port 9
///Enable/Disable Switch Downstream Port 9
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P9;

///PCIe Port 10
///Enable/Disable Switch Downstream Port 10
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P10_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P10_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P10_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P10;

///PCIe Port 11
///Enable/Disable Switch Downstream Port 11
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P11_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P11_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P11_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P11;

///PCIe Port 12
///Enable/Disable Switch Downstream Port 12\n(Note: Disable this port will also disable USB xHCI controller.)
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P12_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P12_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P12_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P12;

///PCIe Port 13
///Enable/Disable Switch Downstream Port 13\n(Note: Disable this port will also disable SATA AHCI controller.)
typedef enum {
  IDSOPT_CMN_P_T21_PCIE_P13_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_PCIE_P13_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_PCIE_P13_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_PCIE_P13;

///SATA Mode
///Select Promontory 21 SATA Type
typedef enum {
  IDSOPT_CMN_P_T21_SATA_CLASS_AHCI = 0,///<AHCI
  IDSOPT_CMN_P_T21_SATA_CLASS_RAID = 1,///<RAID
  IDSOPT_CMN_P_T21_SATA_CLASS_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_SATA_CLASS;

///SATA Port 0
///AHCI SATA port 0 enable
typedef enum {
  IDSOPT_CMN_P_T21_SATA_PORT0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_SATA_PORT0_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_SATA_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_SATA_PORT0;

///SATA Port 1
///AHCI SATA port 1 enable
typedef enum {
  IDSOPT_CMN_P_T21_SATA_PORT1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_SATA_PORT1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_SATA_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_SATA_PORT1;

///SATA Port 2
///AHCI SATA port 2 enable
typedef enum {
  IDSOPT_CMN_P_T21_SATA_PORT2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_SATA_PORT2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_SATA_PORT2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_SATA_PORT2;

///SATA Port 3
///AHCI SATA port 3 enable
typedef enum {
  IDSOPT_CMN_P_T21_SATA_PORT3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_SATA_PORT3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_SATA_PORT3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_SATA_PORT3;

///Device Sleep for AHCI Port 0
///Enable/Disable Device Sleep for AHCI port 0
typedef enum {
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_DISABLE = 0,///<Disable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_ENABLE = 1,///<Enable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P0;

///Device Sleep for AHCI Port 1
///Enable/Disable Device Sleep for AHCI port 1
typedef enum {
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_DISABLE = 0,///<Disable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_ENABLE = 1,///<Enable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P1;

///Device Sleep for AHCI Port 2
///Enable/Disable Device Sleep for AHCI port 2
typedef enum {
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_DISABLE = 0,///<Disable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_ENABLE = 1,///<Enable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P2;

///Device Sleep for AHCI Port 3
///Enable/Disable Device Sleep for AHCI port 3
typedef enum {
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_DISABLE = 0,///<Disable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_ENABLE = 1,///<Enable
  IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_SATA_AGGRESSIVE_DEV_SLP_P3;

///USB3 Port 0
///Enable/Disable xHCI GEN2(<-> 5G) for Port 0
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P0_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P0_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P0_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P0_GEN;

///USB3 Port 1
///Enable/Disable xHCI GEN2(<-> 5G) for Port 1
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P1_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P1_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P1_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P1_GEN;

///USB3 Port 2
///Enable/Disable xHCI GEN2(<-> 5G) for Port 2
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P2_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P2_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P2_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P2_GEN;

///USB3 Port 3
///Enable/Disable xHCI GEN2(<-> 5G) for Port 3
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P3_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P3_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P3_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P3_GEN;

///USB3 Port 4
///Enable/Disable xHCI GEN2(<-> 5G) for Port 4
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P4_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P4_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P4_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P4_GEN;

///USB3 Port 5
///Enable/Disable xHCI GEN2(<-> 5G) for Port 5
typedef enum {
  IDSOPT_DBG_P_T21_XHCI_P5_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_P_T21_XHCI_P5_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_P_T21_XHCI_P5_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_P_T21_XHCI_P5_GEN;

///USB3 Port 0
///USB3.2 Gen2 Port 0
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P0_GEN2X1 = 1,///<Gen2x1
  IDSOPT_CMN_P_T21_USB3_P0_GEN2X2 = 2,///<Gen2x2
  IDSOPT_CMN_P_T21_USB3_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P0;

///USB3 Port 1
///USB3.2 Gen2 Port 1
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB3_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P1;

///USB3 Port 2
///USB3.2 Gen2 Port 2
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB3_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P2;

///USB3 Port 3
///USB3.2 Gen2 Port 3
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB3_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P3;

///USB3 Port 4
///USB3.2 Gen2 Port 4
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB3_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P4;

///USB3 Port 5
///USB3.2 Gen2 Port 5
typedef enum {
  IDSOPT_CMN_P_T21_USB3_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB3_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB3_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB3_P5;

///USB2 Port 0
///USB2 Port 0
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P0;

///USB2 Port 1
///USB2 Port 1
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P1;

///USB2 Port 2
///USB2 Port 2
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P2;

///USB2 Port 3
///USB2 Port 3
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P3;

///USB2 Port 4
///USB2 Port 4
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P4;

///USB2 Port 5
///USB2 Port 5
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P5;

///USB2 Port 6
///USB2 Port 6
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P6;

///USB2 Port 7
///USB2 Port 7
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P7;

///USB2 Port 8
///USB2 Port 8
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P8;

///USB2 Port 9
///USB2 Port 9
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P9;

///USB2 Port 10
///USB2 Port 10
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P10_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P10_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P10_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P10;

///USB2 Port 11
///USB2 Port 11
typedef enum {
  IDSOPT_CMN_P_T21_USB2_P11_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_USB2_P11_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_USB2_P11_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_USB2_P11;

///USB3 Port 0
///USB3.2 Gen2 Port 0
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB3_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB3_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB3_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB3_P0;

///USB3 Port 1
///USB3.2 Gen2 Port 1
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB3_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB3_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB3_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB3_P1;

///USB3 Port 2
///USB3.2 Gen2 Port 2
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB3_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB3_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB3_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB3_P2;

///USB3 Port 3
///USB3.2 Gen2 Port 3
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB3_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB3_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB3_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB3_P3;

///USB2 Port 0
///USB2 Port 0
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P0;

///USB2 Port 1
///USB2 Port 1
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P1;

///USB2 Port 2
///USB2 Port 2
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P2;

///USB2 Port 3
///USB2 Port 3
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P3;

///USB2 Port 4
///USB2 Port 4
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P4;

///USB2 Port 5
///USB2 Port 5
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P5;

///USB2 Port 6
///USB2 Port 6
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P6;

///USB2 Port 7
///USB2 Port 7
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P7;

///USB2 Port 8
///USB2 Port 8
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P8;

///USB2 Port 9
///USB2 Port 9
typedef enum {
  IDSOPT_CMN_P_T21_L4_USB2_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_P_T21_L4_USB2_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_P_T21_L4_USB2_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_P_T21_L4_USB2_P9;

///PCIe Port 0
///Enable/Disable Switch Downstream Port 0
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P0;

///PCIe Port 1
///Enable/Disable Switch Downstream Port 1
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P1;

///PCIe Port 2
///Enable/Disable Switch Downstream Port 2
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P2;

///PCIe Port 3
///Enable/Disable Switch Downstream Port 3
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P3;

///PCIe Port 4
///Enable/Disable Switch Downstream Port 4
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P4;

///PCIe Port 5
///Enable/Disable Switch Downstream Port 5
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P5;

///PCIe Port 6
///Enable/Disable Switch Downstream Port 6
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P6;

///PCIe Port 7
///Enable/Disable Switch Downstream Port 7
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P7;

///PCIe Port 8
///Enable/Disable Switch Downstream Port 8
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P8;

///PCIe Port 9
///Enable/Disable Switch Downstream Port 9
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P9;

///PCIe Port 10
///Enable/Disable Switch Downstream Port 10
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P10_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P10_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P10_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P10;

///PCIe Port 11
///Enable/Disable Switch Downstream Port 11
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P11_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P11_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P11_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P11;

///PCIe Port 12
///Enable/Disable Switch Downstream Port 12\n(Note: Disable this port will also disable USB xHCI controller.)
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P12_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P12_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P12_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P12;

///PCIe Port 13
///Enable/Disable Switch Downstream Port 13\n(Note: Disable this port will also disable SATA AHCI controller.)
typedef enum {
  IDSOPT_CMN_SEC_P_T21_PCIE_P13_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_PCIE_P13_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_PCIE_P13_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_PCIE_P13;

///SATA Mode
///Select Promontory 21 SATA Type
typedef enum {
  IDSOPT_CMN_SEC_P_T21_SATA_CLASS_AHCI = 0,///<AHCI
  IDSOPT_CMN_SEC_P_T21_SATA_CLASS_RAID = 1,///<RAID
  IDSOPT_CMN_SEC_P_T21_SATA_CLASS_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_SATA_CLASS;

///SATA Port 0
///AHCI SATA port 0 enable
typedef enum {
  IDSOPT_CMN_SEC_P_T21_SATA_PORT0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT0_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_SATA_PORT0;

///SATA Port 1
///AHCI SATA port 1 enable
typedef enum {
  IDSOPT_CMN_SEC_P_T21_SATA_PORT1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_SATA_PORT1;

///SATA Port 2
///AHCI SATA port 2 enable
typedef enum {
  IDSOPT_CMN_SEC_P_T21_SATA_PORT2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_SATA_PORT2;

///SATA Port 3
///AHCI SATA port 3 enable
typedef enum {
  IDSOPT_CMN_SEC_P_T21_SATA_PORT3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_SATA_PORT3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_SATA_PORT3;

///Device Sleep for AHCI Port 0
///Enable/Disable Device Sleep for AHCI port 0
typedef enum {
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_DISABLE = 0,///<Disable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_ENABLE = 1,///<Enable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P0;

///Device Sleep for AHCI Port 1
///Enable/Disable Device Sleep for AHCI port 1
typedef enum {
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_DISABLE = 0,///<Disable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_ENABLE = 1,///<Enable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P1;

///Device Sleep for AHCI Port 2
///Enable/Disable Device Sleep for AHCI port 2
typedef enum {
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_DISABLE = 0,///<Disable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_ENABLE = 1,///<Enable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P2;

///Device Sleep for AHCI Port 3
///Enable/Disable Device Sleep for AHCI port 3
typedef enum {
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_DISABLE = 0,///<Disable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_ENABLE = 1,///<Enable
  IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_SATA_AGGRESIVE_DEV_SLP_P3;

///USB3 Port 0
///Enable/Disable xHCI GEN2(<-> 5G) for Port 0
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P0_GEN;

///USB3 Port 1
///Enable/Disable xHCI GEN2(<-> 5G) for Port 1
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P1_GEN;

///USB3 Port 2
///Enable/Disable xHCI GEN2(<-> 5G) for Port 2
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P2_GEN;

///USB3 Port 3
///Enable/Disable xHCI GEN2(<-> 5G) for Port 3
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P3_GEN;

///USB3 Port 4
///Enable/Disable xHCI GEN2(<-> 5G) for Port 4
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P4_GEN;

///USB3 Port 5
///Enable/Disable xHCI GEN2(<-> 5G) for Port 5
typedef enum {
  IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_GEN15G = 0,///<GEN1 (5G)
  IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_GEN2 = 1,///<GEN2
  IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN_AUTO = 0xf,///<Auto
} IDSOPT_DBG_SEC_P_T21_XHCI_P5_GEN;

///USB3 Port 0
///USB3.2 Gen2 Port 0
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P0_GEN2X1 = 1,///<Gen2x1
  IDSOPT_CMN_SEC_P_T21_USB3_P0_GEN2X2 = 2,///<Gen2x2
  IDSOPT_CMN_SEC_P_T21_USB3_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P0;

///USB3 Port 1
///USB3.2 Gen2 Port 1
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB3_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P1;

///USB3 Port 2
///USB3.2 Gen2 Port 2
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB3_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P2;

///USB3 Port 3
///USB3.2 Gen2 Port 3
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB3_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P3;

///USB3 Port 4
///USB3.2 Gen2 Port 4
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB3_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P4;

///USB3 Port 5
///USB3.2 Gen2 Port 5
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB3_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB3_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB3_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB3_P5;

///USB2 Port 0
///USB2 Port 0
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P0;

///USB2 Port 1
///USB2 Port 1
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P1;

///USB2 Port 2
///USB2 Port 2
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P2;

///USB2 Port 3
///USB2 Port 3
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P3;

///USB2 Port 4
///USB2 Port 4
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P4;

///USB2 Port 5
///USB2 Port 5
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P5;

///USB2 Port 6
///USB2 Port 6
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P6;

///USB2 Port 7
///USB2 Port 7
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P7;

///USB2 Port 8
///USB2 Port 8
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P8;

///USB2 Port 9
///USB2 Port 9
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P9;

///USB2 Port 10
///USB2 Port 10
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P10_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P10_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P10_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P10;

///USB2 Port 11
///USB2 Port 11
typedef enum {
  IDSOPT_CMN_SEC_P_T21_USB2_P11_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_USB2_P11_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_USB2_P11_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_USB2_P11;

///USB3 Port 0
///USB3.2 Gen2 Port 0
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB3_P0;

///USB3 Port 1
///USB3.2 Gen2 Port 1
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB3_P1;

///USB3 Port 2
///USB3.2 Gen2 Port 2
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB3_P2;

///USB3 Port 3
///USB3.2 Gen2 Port 3
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB3_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB3_P3;

///USB2 Port 0
///USB2 Port 0
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P0_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P0;

///USB2 Port 1
///USB2 Port 1
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P1_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P1;

///USB2 Port 2
///USB2 Port 2
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P2_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P2;

///USB2 Port 3
///USB2 Port 3
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P3_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P3;

///USB2 Port 4
///USB2 Port 4
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P4_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P4;

///USB2 Port 5
///USB2 Port 5
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P5_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P5;

///USB2 Port 6
///USB2 Port 6
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P6_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P6;

///USB2 Port 7
///USB2 Port 7
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P7_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P7;

///USB2 Port 8
///USB2 Port 8
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P8_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P8;

///USB2 Port 9
///USB2 Port 9
typedef enum {
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_DISABLE = 0,///<Disable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_ENABLE = 1,///<Enable
  IDSOPT_CMN_SEC_P_T21_L4_USB2_P9_AUTO = 0xf,///<Auto
} IDSOPT_CMN_SEC_P_T21_L4_USB2_P9;


#endif //_IDS_NV_DEF_RMB_H_
