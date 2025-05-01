/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/**
 * @file
 *
 * SMU Features enablement
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

#ifndef _SMU_FEATURES_ENABLE_V13_H_
#define _SMU_FEATURES_ENABLE_V13_H_

#include "smu13_RMB.h"

#define FP7_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (0 << (FEATURE_ULV_BIT                 %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (1 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (1 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (1 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (1 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (1 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (0 << (FEATURE_ZSTATES_BIT             %32)) | \
  (1 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (0 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP1CLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP0CLK_BIT           %32)) \
)


#define FP7_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (1 << (FEATURE_CLK_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (1 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (1 << (FEATURE_CPUOFF_BIT              %32)) | \
  (1 << (FEATURE_STAPM_BIT               %32)) | \
  (1 << (FEATURE_S0I3_BIT                %32)) | \
  (1 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (0 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (1 << (FEATURE_RSMU_LOW_POWER_BIT      %32)) | \
  (0 << (FEATURE_SMN_LOW_POWER_BIT       %32)) | \
  (1 << (FEATURE_THM_LOW_POWER_BIT       %32)) | \
  (1 << (FEATURE_SMUIO_LOW_POWER_BIT     %32)) | \
  (1 << (FEATURE_MP1_LOW_POWER_BIT       %32)) | \
  (1 << (FEATURE_DS_VCN_BIT              %32)) | \
  (1 << (FEATURE_CPPC_BIT                %32)) | \
  (1 << (FEATURE_CPPC_PREFERRED_CORES    %32)) | \
  (0 << (FEATURE_SMART_SHIFT_BIT         %32)) | \
  (1 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) | \
  (0 << (FEATURE_SOC_VOLTAGE_MON_BIT     %32)) | \
  (1 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_HSPCLK_BIT           %32)) \
)

#define AM5_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (0 << (FEATURE_ULV_BIT                 %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (0 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (0 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (0 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (0 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (0 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (0 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (0 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (0 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (0 << (FEATURE_ZSTATES_BIT             %32)) | \
  (0 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (0 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (0 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_MP1CLK_BIT           %32)) | \
  (0 << (FEATURE_DS_MP0CLK_BIT           %32)) \
)

#define AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (0 << (FEATURE_CLK_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (0 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (0 << (FEATURE_CPUOFF_BIT              %32)) | \
  (0 << (FEATURE_STAPM_BIT               %32)) | \
  (0 << (FEATURE_S0I3_BIT                %32)) | \
  (0 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (1 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (0 << (FEATURE_RSMU_LOW_POWER_BIT      %32)) | \
  (0 << (FEATURE_SMN_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_THM_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_SMUIO_LOW_POWER_BIT     %32)) | \
  (0 << (FEATURE_MP1_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_DS_VCN_BIT              %32)) | \
  (0 << (FEATURE_CPPC_BIT                %32)) | \
  (0 << (FEATURE_CPPC_PREFERRED_CORES    %32)) | \
  (0 << (FEATURE_SMART_SHIFT_BIT         %32)) | \
  (0 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) | \
  (0 << (FEATURE_SOC_VOLTAGE_MON_BIT     %32)) | \
  (0 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_HSPCLK_BIT           %32)) \
)


#define FP7_SMU_FEATURES_ENABLE_64_DEFAULT  0
#define AM5_SMU_FEATURES_ENABLE_64_DEFAULT  0

#endif
