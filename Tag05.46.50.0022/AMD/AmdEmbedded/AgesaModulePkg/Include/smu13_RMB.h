/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains the definition of the SMU13
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: Nbio/SMU
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */
#ifndef SMU13_RMB_H
#define SMU13_RMB_H

//
// Assume standard IA-32 alignment.
// BugBug: Need to check portability of long long
//
#ifndef SMU_DATA_TYPES_H
#define SMU_DATA_TYPES_H
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
#endif

#pragma pack(push, 1)

#define FEATURE_CCLK_DPM_BIT                 0
#define FEATURE_FAN_CONTROLLER_BIT           1
#define FEATURE_DATA_CALCULATION_BIT         2
#define FEATURE_PPT_BIT                      3
#define FEATURE_TDC_BIT                      4
#define FEATURE_THERMAL_BIT                  5
#define FEATURE_FIT_BIT                      6
#define FEATURE_EDC_BIT                      7
#define FEATURE_PLL_POWER_DOWN_BIT           8
#define FEATURE_ULV_BIT                      9
#define FEATURE_VDDOFF_BIT                  10
#define FEATURE_VCN_DPM_BIT                 11
#define FEATURE_CSTATE_BOOST_BIT            12
#define FEATURE_FCLK_DPM_BIT                13
#define FEATURE_SOCCLK_DPM_BIT              14
#define FEATURE_MP0CLK_DPM_BIT              15
#define FEATURE_LCLK_DPM_BIT                16
#define FEATURE_SHUBCLK_DPM_BIT             17
#define FEATURE_DCFCLK_DPM_BIT              18
#define FEATURE_GFX_DPM_BIT                 19
#define FEATURE_DS_GFXCLK_BIT               20
#define FEATURE_DS_SOCCLK_BIT               21
#define FEATURE_DS_LCLK_BIT                 22
#define FEATURE_DS_DCFCLK_BIT               23
#define FEATURE_DS_SHUBCLK_BIT              24
#define FEATURE_GFX_TEMP_VMIN_BIT           25
#define FEATURE_ZSTATES_BIT                 26
#define FEATURE_WHISPER_MODE_BIT            27
#define FEATURE_DS_FCLK_BIT                 28
#define FEATURE_DS_SMNCLK_BIT               29
#define FEATURE_DS_MP1CLK_BIT               30
#define FEATURE_DS_MP0CLK_BIT               31
#define FEATURE_CLK_LOW_POWER_BIT           32
#define FEATURE_FUSE_PG_BIT                 33
#define FEATURE_GFX_DEM_BIT                 34
#define FEATURE_PSI_BIT                     35
#define FEATURE_PROCHOT_BIT                 36
#define FEATURE_CPUOFF_BIT                  37
#define FEATURE_STAPM_BIT                   38
#define FEATURE_S0I3_BIT                    39
#define FEATURE_DF_LIGHT_CSTATE             40   // shift the order or DFCstate annd DF light Cstate
#define FEATURE_PERF_LIMIT_BIT              41
#define FEATURE_CORE_DLDO_BIT               42
#define FEATURE_RSMU_LOW_POWER_BIT          43
#define FEATURE_SMN_LOW_POWER_BIT           44
#define FEATURE_THM_LOW_POWER_BIT           45
#define FEATURE_SMUIO_LOW_POWER_BIT         46
#define FEATURE_MP1_LOW_POWER_BIT           47
#define FEATURE_DS_VCN_BIT                  48
#define FEATURE_CPPC_BIT                    49
#define FEATURE_CPPC_PREFERRED_CORES        50
#define FEATURE_SMART_SHIFT_BIT             51
#define FEATURE_DF_CSTATES_BIT              52
#define FEATURE_MSMU_LOW_POWER_BIT          53
#define FEATURE_SOC_VOLTAGE_MON_BIT         54
#define FEATURE_ATHUB_PG_BIT                55
#define FEATURE_VDDOFF_ECO_BIT              56
#define FEATURE_ZSTATES_ECO_BIT             57
#define FEATURE_CC6_BIT                     58
#define FEATURE_DS_UMCCLK_BIT               59
#define FEATURE_DS_HSPCLK_BIT               60
#define NUM_FEATURES                        61


#define FEATURE_CCLK_DPM_MASK            (1 << FEATURE_CCLK_DPM_BIT            ) // 0
#define FEATURE_FAN_CONTROLLER_MASK      (1 << FEATURE_FAN_CONTROLLER_BIT      ) // 1
#define FEATURE_DATA_CALCULATION_MASK    (1 << FEATURE_DATA_CALCULATION_BIT    ) // 2
#define FEATURE_PPT_MASK                 (1 << FEATURE_PPT_BIT                 ) // 3
#define FEATURE_TDC_MASK                 (1 << FEATURE_TDC_BIT                 ) // 4
#define FEATURE_THERMAL_MASK             (1 << FEATURE_THERMAL_BIT             ) // 5
#define FEATURE_FIT_MASK                 (1 << FEATURE_FIT_BIT                 ) // 6
#define FEATURE_EDC_MASK                 (1 << FEATURE_EDC_BIT                 ) // 7
#define FEATURE_PLL_POWER_DOWN_MASK      (1 << FEATURE_PLL_POWER_DOWN_BIT      ) // 8
#define FEATURE_ULV_MASK                 (1 << FEATURE_ULV_BIT                 ) // 9
#define FEATURE_VDDOFF_MASK              (1 << FEATURE_VDDOFF_BIT              ) // 10
#define FEATURE_VCN_DPM_MASK             (1 << FEATURE_VCN_DPM_BIT             ) // 11
#define FEATURE_CSTATE_BOOST_MASK        (1 << FEATURE_CSTATE_BOOST_BIT        ) // 12
#define FEATURE_FCLK_DPM_MASK            (1 << FEATURE_FCLK_DPM_BIT            ) // 13
#define FEATURE_SOCCLK_DPM_MASK          (1 << FEATURE_SOCCLK_DPM_BIT          ) // 14
#define FEATURE_MP0CLK_DPM_MASK          (1 << FEATURE_MP0CLK_DPM_BIT          ) // 15
#define FEATURE_LCLK_DPM_MASK            (1 << FEATURE_LCLK_DPM_BIT            ) // 16
#define FEATURE_SHUBCLK_DPM_MASK         (1 << FEATURE_SHUBCLK_DPM_BIT         ) // 17
#define FEATURE_DCFCLK_DPM_MASK          (1 << FEATURE_DCFCLK_DPM_BIT          ) // 18
#define FEATURE_GFX_DPM_MASK             (1 << FEATURE_GFX_DPM_BIT             ) // 19
#define FEATURE_DS_GFXCLK_MASK           (1 << FEATURE_DS_GFXCLK_BIT           ) // 20
#define FEATURE_DS_SOCCLK_MASK           (1 << FEATURE_DS_SOCCLK_BIT           ) // 21
#define FEATURE_DS_LCLK_MASK             (1 << FEATURE_DS_LCLK_BIT             ) // 22
#define FEATURE_DS_DCFCLK_MASK           (1 << FEATURE_DS_DCFCLK_BIT           ) // 23
#define FEATURE_DS_SHUBCLK_MASK          (1 << FEATURE_DS_SHUBCLK_BIT          ) // 24
#define FEATURE_GFX_TEMP_VMIN_MASK       (1 << FEATURE_GFX_TEMP_VMIN_BIT       ) // 25
#define FEATURE_ZSTATES_MASK             (1 << FEATURE_ZSTATES_BIT             ) // 26
#define FEATURE_WHISPER_MODE_MASK        (1 << FEATURE_WHISPER_MODE_BIT        ) // 27
#define FEATURE_DS_FCLK_MASK             (1 << FEATURE_DS_FCLK_BIT             ) // 28
#define FEATURE_DS_SMNCLK_MASK           (1 << FEATURE_DS_SMNCLK_BIT           ) // 29
#define FEATURE_DS_MP1CLK_MASK           (1 << FEATURE_DS_MP1CLK_BIT           ) // 30
#define FEATURE_DS_MP0CLK_MASK           (1 << FEATURE_DS_MP0CLK_BIT           ) // 31

#define FEATURE_CLK_LOW_POWER_MASK       (1 << (FEATURE_CLK_LOW_POWER_BIT       - 32)) // 32
#define FEATURE_FUSE_PG_MASK             (1 << (FEATURE_FUSE_PG_BIT             - 32)) // 33
#define FEATURE_GFX_DEM_MASK             (1 << (FEATURE_GFX_DEM_BIT             - 32)) // 34
#define FEATURE_PSI_MASK                 (1 << (FEATURE_PSI_BIT                 - 32)) // 35
#define FEATURE_PROCHOT_MASK             (1 << (FEATURE_PROCHOT_BIT             - 32)) // 36
#define FEATURE_CPUOFF_MASK              (1 << (FEATURE_CPUOFF_BIT              - 32)) // 37
#define FEATURE_STAPM_MASK               (1 << (FEATURE_STAPM_BIT               - 32)) // 38
#define FEATURE_S0I3_MASK                (1 << (FEATURE_S0I3_BIT                - 32)) // 39
#define FEATURE_DF_LIGHT_CSTATE_MASK     (1 << (FEATURE_DF_LIGHT_CSTATE         - 32)) // 40
#define FEATURE_PERF_LIMIT_MASK          (1 << (FEATURE_PERF_LIMIT_BIT          - 32)) // 41
#define FEATURE_CORE_DLDO_MASK           (1 << (FEATURE_CORE_DLDO_BIT           - 32)) // 42
#define FEATURE_RSMU_LOW_POWER_MASK      (1 << (FEATURE_RSMU_LOW_POWER_BIT      - 32)) // 43
#define FEATURE_SMN_LOW_POWER_MASK       (1 << (FEATURE_SMN_LOW_POWER_BIT       - 32)) // 44
#define FEATURE_THM_LOW_POWER_MASK       (1 << (FEATURE_THM_LOW_POWER_BIT       - 32)) // 45
#define FEATURE_SMUIO_LOW_POWER_MASK     (1 << (FEATURE_SMUIO_LOW_POWER_BIT     - 32)) // 46
#define FEATURE_MP1_LOW_POWER_MASK       (1 << (FEATURE_MP1_LOW_POWER_BIT       - 32)) // 47
#define FEATURE_DS_VCN_MASK              (1 << (FEATURE_DS_VCN_BIT              - 32)) // 48
#define FEATURE_CPPC_MASK                (1 << (FEATURE_CPPC_BIT                - 32)) // 49
#define FEATURE_CPPC_PREFERRED_CORES_MASK (1 << (FEATURE_CPPC_PREFERRED_CORES   - 32)) // 50
#define FEATURE_SMART_SHIFT_MASK         (1 << (FEATURE_SMART_SHIFT_BIT         - 32)) // 51
#define FEATURE_DF_CSTATES_MASK          (1 << (FEATURE_DF_CSTATES_BIT          - 32)) // 52
#define FEATURE_MSMU_LOW_POWER_MASK      (1 << (FEATURE_MSMU_LOW_POWER_BIT      - 32)) // 53
#define FEATURE_SOC_VOLTAGE_MON_MASK     (1 << (FEATURE_SOC_VOLTAGE_MON_BIT     - 32)) // 54
#define FEATURE_ATHUB_PG_MASK            (1 << (FEATURE_ATHUB_PG_BIT            - 32)) // 55
#define FEATURE_VDDOFF_ECO_MASK          (1 << (FEATURE_VDDOFF_ECO_BIT          - 32)) // 56
#define FEATURE_ZSTATES_ECO_MASK         (1 << (FEATURE_ZSTATES_ECO_BIT         - 32)) // 57
#define FEATURE_CC6_MASK                 (1 << (FEATURE_CC6_BIT                 - 32)) // 58
#define FEATURE_DS_UMCCLK_MASK           (1 << (FEATURE_DS_UMCCLK_BIT           - 32)) // 59
#define FEATURE_DS_HSPCLK_MASK           (1 << (FEATURE_DS_HSPCLK_BIT           - 32)) // 60


// Firmware Header/Footer
struct SMU13_Firmware_Footer {
  uint32_t Signature;
};

typedef struct SMU13_Firmware_Footer SMU13_Firmware_Footer;
// PSP3.0 Header Definition
typedef struct {
  uint32_t ImageVersion;
  uint32_t ImageVersion2; // This is repeated because DW0 cannot be written in SRAM due to HW bug.
  uint32_t Padding0[3];
  uint32_t SizeFWSigned;
  uint32_t Padding1[25];
  uint32_t FirmwareType;
  uint32_t Filler[32];
} SMU_Firmware_Header;

typedef struct {
  // MP1_EXT_SCRATCH0
  uint32_t DpmHandlerID         : 8;
  uint32_t ActivityMonitorID    : 8;
  uint32_t DpmTimerID           : 8;
  uint32_t DpmHubID             : 4;
  uint32_t DpmHubTask           : 4;
  // MP1_EXT_SCRATCH1
  uint32_t GfxoffStatus         : 8;
  uint32_t GfxStatus            : 2;
  uint32_t CpuOff               : 2;
  uint32_t VddOff               : 1;
  uint32_t InUlv                : 1;
  uint32_t InWhisperMode        : 1;
  uint32_t spare0               : 1;
  uint32_t ZstateStatus         : 4;
  uint32_t spare1               :12;
  // MP1_EXT_SCRATCH2
  uint32_t P2JobHandler         :24;
  uint32_t RsmuPmiP2FinishedCnt : 8;
  // MP1_EXT_SCRATCH3
  uint32_t PostCode             :32;
  // MP1_EXT_SCRATCH4
  uint32_t MsgPortBusy          :15;
  uint32_t RsmuPmiP1Pending     : 1;
  uint32_t DfCstateExitPending  : 1;
  uint32_t Pc6EntryPending      : 1;
  uint32_t Pc6ExitPending       : 1;
  uint32_t WarmResetPending     : 1;
  uint32_t Mp0ClkPending        : 1;
  uint32_t spare2               : 3;
  uint32_t RsmuPmiP2PendingCnt  : 8;
  // MP1_EXT_SCRATCH5
  uint32_t IdleMask             :32;
  // MP1_EXT_SCRATCH6 = RTOS threads' status
  // MP1_EXT_SCRATCH7 = RTOS Current Job
} FwStatus_t;

#define TABLE_BIOS_IF            0 // Called by BIOS
#define TABLE_WATERMARKS         1 // Called by Driver
#define TABLE_CUSTOM_DPM         2 // Called by Driver
#define TABLE_BIOS_GPIO_CONFIG   3 // Called by BIOS
#define TABLE_DPMCLOCKS          4 // Called by Driver
#define TABLE_MOMENTARY_PM       5 // Called by Tools
#define TABLE_MODERN_STDBY       6 // Called by Tools for Modern Standby Log
#define TABLE_SMU_METRICS        7 // Called by Driver
#define TABLE_COUNT              8

#pragma pack(pop)

#endif

