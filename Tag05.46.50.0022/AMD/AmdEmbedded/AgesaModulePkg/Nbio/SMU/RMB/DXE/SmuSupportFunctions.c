/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU V13 DXE Support Functions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  SmuV13Dxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include "SmuV13Dxe.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <GnbDxio.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Guid/HobList.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/SmnAccessLib.h>
#include <Library/BaseLib.h>
#include <Smu13FeaturesEnable.h>
#include <Library/GnbLib.h>

#define FILECODE        NBIO_SMU_RMB_DXE_SMUSUPPORTFUNCTIONS_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

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

/*----------------------------------------------------------------------------------------*/
/**
 * Initilaize data for SMU Dxe phase.
 *
 *
 *
 * @param[out]  Pcie                    Get Pcie data from hob.
 *
 * @retval     EFI_SUCCESS
 */
EFI_STATUS
SmuDxeInitialize (
  OUT PCIe_PLATFORM_CONFIG           **Pcie
  )
{
  EFI_STATUS                           Status;
  GNB_PCIE_INFORMATION_DATA_HOB        *GnbPcieInformationDataHob;

  // Correcting Pcie information from Hob

  //
  // Get information data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &GnbPcieInformationDataHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GnbPcieInformationDataHob = GetNextGuidHob (&gGnbPcieHobInfoGuid, GnbPcieInformationDataHob);
  if (GnbPcieInformationDataHob == NULL) {
    return EFI_NOT_FOUND;
  }
  *Pcie = &(GnbPcieInformationDataHob->PciePlatformConfigHob);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
  Enable or disable deep sleep in supported IP Blocks

  @param GnbHandle            Gnbhandle of this NBIO
  @param FeatureDsLclk        FEATURE_DS_LCLK_BIT
  @param FeatureDsSocClk      FEATURE_DS_SOCCLK_BIT
  @param FeatureDsShubClk     FEATURE_DS_SHUBCLK_BIT
  @param FeatureDsVcn         FEATURE_DS_VCN_BIT
  @param FeatureDsDcFClk      FEATURE_DS_DCFCLK_BIT
**/
VOID
EnableOrDisableIpDeepSleep (
  GNB_HANDLE      *GnbHandle,
  BOOLEAN         FeatureDsLclk,
  BOOLEAN         FeatureDsSocClk,
  BOOLEAN         FeatureDsShubClk,
  BOOLEAN         FeatureDsVcn,
  BOOLEAN         FeatureDsDcFClk
  )
{
  NBIF_DS_CTRL_LCLK_STRUCT          DsCtrlLClk;
  SYSHUB_DS_CTRL_NICCLK_STRUCT      DsCtrlNicClk;
  SYSHUB_DS_CTRL_SHUBCLK_STRUCT     DsCtrlShubClk;
  PCTL_SLICE0_MISC_STRUCT           Pctl0;
  SYSHUB_DS_CTRL_SOCCLK_STRUCT      DscCtlSocClk;
  NBIF_DS_CTRL_LCLK_STRUCT          NbifDsCtrlLcClk;
  SYSHUB_DS_CTRL_SHUBCLK_STRUCT     SyshubDsCtlShubClk;
  PCTL_MMHUB_DEEPSLEEP_IB_STRUCT    MmhubDsIb;

  // LCLK
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_NBIF_DS_CTRL_LCLK_ADDRESS,
    &(DsCtrlLClk.Value)
    );
  DsCtrlLClk.Field.NBIF_LCLK_DS_EN = FeatureDsLclk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_NBIF_DS_CTRL_LCLK_ADDRESS,
    &(DsCtrlLClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_NICCLK_ADDRESS,
    &(DsCtrlNicClk.Value)
    );
  DsCtrlNicClk.Field.SYSHUB_NICCLK_DS_EN = FeatureDsLclk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_NICCLK_ADDRESS,
    &(DsCtrlNicClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(DsCtrlShubClk.Value)
    );
  DsCtrlShubClk.Field.SYSHUB_SHUBCLK_DS_EN = FeatureDsLclk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(DsCtrlShubClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );

  // SOCCLK
  // 1. PCTL_MMHUB_DEEPSLEEP_IB = 0x80000001
  if (FeatureDsSocClk) {
    MmhubDsIb.Value = 0x80000001;
    SmnRegisterWrite (
      GnbHandle->Address.Address.Bus,
      SMN_PCTL_MMHUB_DEEPSLEEP_IB_ADDRESS,
      &(MmhubDsIb.Value),
      GNB_REG_ACC_FLAG_S3SAVE
      );
  }
  // 2. PCTL_SLICE0_MISC. DEEPSLEEP_DISCSDP = 1
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_PCTL_SLICE0_MISC_ADDRESS,
    &(Pctl0.Value)
    );
  Pctl0.Field.DEEPSLEEP_DISCSDP = FeatureDsSocClk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_PCTL_SLICE0_MISC_ADDRESS,
    &(Pctl0.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );
  // 3. PCTL_MMHUB_DEEPSLEEP_IB = 0x00000001
  if (FeatureDsSocClk) {
    MmhubDsIb.Value = 0x00000001;
    SmnRegisterWrite (
      GnbHandle->Address.Address.Bus,
      SMN_PCTL_MMHUB_DEEPSLEEP_IB_ADDRESS,
      &(MmhubDsIb.Value),
      GNB_REG_ACC_FLAG_S3SAVE
      );
  }
  // 4. SYSHUB_DS_CTRL_SOCCLK. SYSHUB_SOCCLK_DS_EN = 1
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SOCCLK_ADDRESS,
    &(DscCtlSocClk.Value)
    );
  DscCtlSocClk.Field.SYSHUB_SOCCLK_DS_EN = FeatureDsSocClk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SOCCLK_ADDRESS,
    &(DscCtlSocClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );
  // 5. NBIF_DS_CTRL_LCLK. NBIF_LCLK_DS_EN = 1
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_NBIF_DS_CTRL_LCLK_ADDRESS,
    &(NbifDsCtrlLcClk.Value)
    );
  NbifDsCtrlLcClk.Field.NBIF_LCLK_DS_EN = FeatureDsSocClk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_NBIF_DS_CTRL_LCLK_ADDRESS,
    &(NbifDsCtrlLcClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );

  // SHUBCLK
  SmnRegisterRead (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(SyshubDsCtlShubClk.Value)
    );
  SyshubDsCtlShubClk.Field.SYSHUB_SHUBCLK_DS_EN = FeatureDsShubClk ? 1 : 0;
  SyshubDsCtlShubClk.Field.DMA_CL3_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE = FeatureDsShubClk ? 1 : 0;
  SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(SyshubDsCtlShubClk.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );
}

/*----------------------------------------------------------------------------------------*/
/**
  Shortcut function for SMI service request

  @param GnbHandle        Instance of GNB Handle
  @param ServiceRequest   Service request type
  @param Arg0             Argument to pass
  @param Arg1             Argument to pass
  @param Arg2             Argument to pass
  @param Arg3             Argument to pass
  @param Arg4             Argument to pass
  @param Arg5             Argument to pass

  @return UINT32          Return status
**/
UINT32
SmuServiceRequest (
  GNB_HANDLE      *GnbHandle,
  UINT32          ServiceRequest,
  UINT32          Arg0,
  UINT32          Arg1,
  UINT32          Arg2,
  UINT32          Arg3,
  UINT32          Arg4,
  UINT32          Arg5
  )
{
  UINT32      SmuArg[6];

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Start\n", __FUNCTION__);
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  SmuArg[0] = Arg0;
  SmuArg[1] = Arg1;
  SmuArg[2] = Arg2;
  SmuArg[3] = Arg3;
  SmuArg[4] = Arg4;
  SmuArg[5] = Arg5;
  NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), ServiceRequest, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
  IDS_HDT_CONSOLE (GNB_TRACE, "%a End\n", __FUNCTION__);
  return SmuArg[0];
}

/*----------------------------------------------------------------------------------------*/
/**
 * Reads a fuse value based on an enumerated list of fuse "names"
 *
 *
 *
 * @param[in]  InstanceId              The unique identifier of the SMU instance associated with this socket
 * @param[out] MyHandle                Pointer to GNB_HANDLE* to return GNB_HANDLE of this InstanceId
 *
 * @retval     EFI_SUCCESS             The fuse was located and the FuseValue returned is valid
 * @retval     EFI_INVALID_PARAMETER   InstanceId did not reference a valid NBIO instance
 */
EFI_STATUS
SmuGetGnbHandle (
  IN       UINT32                         InstanceId,
     OUT   GNB_HANDLE                     **MyHandle
  )
{
  EFI_STATUS                           Status;
  GNB_PCIE_INFORMATION_DATA_HOB        *GnbPcieInformationDataHob;
  PCIe_PLATFORM_CONFIG                 *Pcie;
  BOOLEAN                              InstanceFound;
  GNB_HANDLE                           *GnbHandle;

  //
  // Get information data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &GnbPcieInformationDataHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GnbPcieInformationDataHob = GetNextGuidHob (&gGnbPcieHobInfoGuid, GnbPcieInformationDataHob);
  if (GnbPcieInformationDataHob == NULL) {
    return EFI_NOT_FOUND;
  }
  Pcie = &(GnbPcieInformationDataHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  InstanceFound = FALSE;
  Status = EFI_INVALID_PARAMETER;
  while ((InstanceFound == FALSE) && (GnbHandle != NULL)) {
    if (GnbHandle->SocketId == InstanceId) {
      InstanceFound = TRUE;
      Status = EFI_SUCCESS;
    } else {
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }
  *MyHandle = GnbHandle;

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Dump Smu Feature Control.
 *
 *
 *
 * @param[in]  EnableFeatures
 *
 */
VOID
DumpSmuFeatureControl (
  IN       UINT32                         EnableFeatures,
  IN       UINT32                         EnableFeaturesExt,
  IN       UINT32                         EnableFeatures64
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "Smu Feature Control(0x%x) (0x%x) (0x%x):\n", EnableFeatures, EnableFeaturesExt, EnableFeatures64);
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CCLK_DPM            %a\n", ((EnableFeatures & FEATURE_CCLK_DPM_MASK           ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_FAN_CONTROLLER      %a\n", ((EnableFeatures & FEATURE_FAN_CONTROLLER_MASK     ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DATA_CALCULATION    %a\n", ((EnableFeatures & FEATURE_DATA_CALCULATION_MASK   ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_PPT                 %a\n", ((EnableFeatures & FEATURE_PPT_MASK                ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_TDC                 %a\n", ((EnableFeatures & FEATURE_TDC_MASK                ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_THERMAL             %a\n", ((EnableFeatures & FEATURE_THERMAL_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_FIT                 %a\n", ((EnableFeatures & FEATURE_FIT_MASK                ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_EDC                 %a\n", ((EnableFeatures & FEATURE_EDC_MASK                ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_PLL_POWER_DOWN      %a\n", ((EnableFeatures & FEATURE_PLL_POWER_DOWN_MASK     ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_ULV                 %a\n", ((EnableFeatures & FEATURE_ULV_MASK                ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_VDDOFF              %a\n", ((EnableFeatures & FEATURE_VDDOFF_MASK             ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_VCN_DPM             %a\n", ((EnableFeatures & FEATURE_VCN_DPM_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CSTATE_BOOST        %a\n", ((EnableFeatures & FEATURE_CSTATE_BOOST_MASK       ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_FCLK_DPM            %a\n", ((EnableFeatures & FEATURE_FCLK_DPM_MASK           ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SOCCLK_DPM          %a\n", ((EnableFeatures & FEATURE_SOCCLK_DPM_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_MP0CLK_DPM          %a\n", ((EnableFeatures & FEATURE_MP0CLK_DPM_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_LCLK_DPM            %a\n", ((EnableFeatures & FEATURE_LCLK_DPM_MASK           ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SHUBCLK_DPM         %a\n", ((EnableFeatures & FEATURE_SHUBCLK_DPM_MASK        ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DCFCLK_DPM          %a\n", ((EnableFeatures & FEATURE_DCFCLK_DPM_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_GFX_DPM             %a\n", ((EnableFeatures & FEATURE_GFX_DPM_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_GFXCLK           %a\n", ((EnableFeatures & FEATURE_DS_GFXCLK_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_SOCCLK           %a\n", ((EnableFeatures & FEATURE_DS_SOCCLK_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_LCLK             %a\n", ((EnableFeatures & FEATURE_DS_LCLK_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_DCFCLK           %a\n", ((EnableFeatures & FEATURE_DS_DCFCLK_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_SHUBCLK          %a\n", ((EnableFeatures & FEATURE_DS_SHUBCLK_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_GFX_TEMP_VMIN       %a\n", ((EnableFeatures & FEATURE_GFX_TEMP_VMIN_MASK      ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_ZSTATES             %a\n", ((EnableFeatures & FEATURE_ZSTATES_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_WHISPER_MODE        %a\n", ((EnableFeatures & FEATURE_WHISPER_MODE_MASK       ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_FCLK             %a\n", ((EnableFeatures & FEATURE_DS_FCLK_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_SMNCLK           %a\n", ((EnableFeatures & FEATURE_DS_SMNCLK_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_MP1CLK           %a\n", ((EnableFeatures & FEATURE_DS_MP1CLK_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_MP0CLK           %a\n", ((EnableFeatures & FEATURE_DS_MP0CLK_MASK          ) != 0) ? "Enable" : "Disable");

  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CLK_LOW_POWER       %a\n", ((EnableFeaturesExt & FEATURE_CLK_LOW_POWER_MASK   ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_FUSE_PG             %a\n", ((EnableFeaturesExt & FEATURE_FUSE_PG_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_GFX_DEM             %a\n", ((EnableFeaturesExt & FEATURE_GFX_DEM_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_PSI                 %a\n", ((EnableFeaturesExt & FEATURE_PSI_MASK             ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_PROCHOT             %a\n", ((EnableFeaturesExt & FEATURE_PROCHOT_MASK         ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CPUOFF              %a\n", ((EnableFeaturesExt & FEATURE_CPUOFF_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_STAPM               %a\n", ((EnableFeaturesExt & FEATURE_STAPM_MASK           ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_S0I3                %a\n", ((EnableFeaturesExt & FEATURE_S0I3_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DF_LIGHT_CSTATE     %a\n", ((EnableFeaturesExt & FEATURE_DF_LIGHT_CSTATE_MASK ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_PERF_LIMIT          %a\n", ((EnableFeaturesExt & FEATURE_PERF_LIMIT_MASK      ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CORE_DLDO           %a\n", ((EnableFeaturesExt & FEATURE_CORE_DLDO_MASK       ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_RSMU_LOW_POWER      %a\n", ((EnableFeaturesExt & FEATURE_RSMU_LOW_POWER_MASK  ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SMN_LOW_POWER       %a\n", ((EnableFeaturesExt & FEATURE_SMN_LOW_POWER_MASK   ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_THM_LOW_POWER       %a\n", ((EnableFeaturesExt & FEATURE_THM_LOW_POWER_MASK   ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SMUIO_LOW_POWER     %a\n", ((EnableFeaturesExt & FEATURE_SMUIO_LOW_POWER_MASK ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_MP1_LOW_POWER       %a\n", ((EnableFeaturesExt & FEATURE_MP1_LOW_POWER_MASK   ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_VCN              %a\n", ((EnableFeaturesExt & FEATURE_DS_VCN_MASK          ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CPPC                %a\n", ((EnableFeaturesExt & FEATURE_CPPC_MASK            ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CPPC_PREFERRED_CORES %a\n", ((EnableFeaturesExt & FEATURE_CPPC_PREFERRED_CORES_MASK ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SMART_SHIFT         %a\n", ((EnableFeaturesExt & FEATURE_SMART_SHIFT_MASK     ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DF_CSTATES          %a\n", ((EnableFeaturesExt & FEATURE_DF_CSTATES_MASK      ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_MSMU_LOW_POWER      %a\n", ((EnableFeaturesExt & FEATURE_MSMU_LOW_POWER_MASK  ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_SOC_VOLTAGE_MON     %a\n", ((EnableFeaturesExt & FEATURE_SOC_VOLTAGE_MON_MASK ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_ATHUB_PG            %a\n", ((EnableFeaturesExt & FEATURE_ATHUB_PG_MASK        ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_VDDOFF_ECO          %a\n", ((EnableFeaturesExt & FEATURE_VDDOFF_ECO_MASK      ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_ZSTATES_ECO         %a\n", ((EnableFeaturesExt & FEATURE_ZSTATES_ECO_MASK     ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_CC6                 %a\n", ((EnableFeaturesExt & FEATURE_CC6_MASK             ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_UMCCLK           %a\n", ((EnableFeaturesExt & FEATURE_DS_UMCCLK_MASK       ) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, " FEATURE_DS_HSPCLK           %a\n", ((EnableFeaturesExt & FEATURE_DS_HSPCLK_MASK       ) != 0) ? "Enable" : "Disable");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Dump Fan Policy.
 *
 *
 *
 * @param[in]  PPTable
 *
 */
VOID
DumpFanPolicyTable (
  IN       Smu13BiosIfTable_t          *PPTable
  )
{

  IDS_HDT_CONSOLE (GNB_TRACE, "Fan Policy:\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_Override = 0x%x\n", PPTable->FanTable_Override);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_Hysteresis = 0x%x\n", PPTable->FanTable_Hysteresis);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_TempLow = 0x%x\n", PPTable->FanTable_TempLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_TempMed = 0x%x\n", PPTable->FanTable_TempMed);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_TempHigh = 0x%x\n", PPTable->FanTable_TempHigh);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_TempCritical = 0x%x\n", PPTable->FanTable_TempCritical);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_PwmLow = 0x%x\n", PPTable->FanTable_PwmLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_PwmMed = 0x%x\n", PPTable->FanTable_PwmMed);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_PwmHigh = 0x%x\n", PPTable->FanTable_PwmHigh);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_PwmFreq = 0x%x\n", PPTable->FanTable_PwmFreq);
  IDS_HDT_CONSOLE (GNB_TRACE, "  FanTable_Polarity = 0x%x\n", PPTable->FanTable_Polarity);

  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Dump Fan PPTable
 *
 *
 *
 * @param[in]  PPTable
 *
 */
VOID
DumpPPTable (
  IN       Smu13BiosIfTable_t          *PPTable
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "\nSMU BIOS INTERFACE TABLE VALUES\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "Version = 0x%x\n", PPTable->Version);

  //PCD infrastructure limits
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nPCD infrastructure limits\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "SUSTAINED_POWER_LIMIT = 0x%x\n", PPTable->SUSTAINED_POWER_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "FAST_PPT_LIMIT = 0x%x\n", PPTable->FAST_PPT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "SLOW_PPT_LIMIT = 0x%x\n", PPTable->SLOW_PPT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "SLOW_PPT_TIME_CONSTANT = 0x%x\n", PPTable->SLOW_PPT_TIME_CONSTANT);
  IDS_HDT_CONSOLE (GNB_TRACE, "SYSTEM_TEMPERATURE_TRACKING = 0x%x\n", PPTable->SYSTEM_TEMPERATURE_TRACKING);
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_BOOST_OVERRIDE = 0x%x\n", PPTable->STAPM_BOOST_OVERRIDE);
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_BOOST = 0x%x\n", PPTable->STAPM_BOOST);
  IDS_HDT_CONSOLE (GNB_TRACE, "AA_MODE_EN = 0x%x\n", PPTable->AA_MODE_EN);
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_TIME_CONSTANT = 0x%x\n", PPTable->STAPM_TIME_CONSTANT);
  IDS_HDT_CONSOLE (GNB_TRACE, "THERMAL_LIMIT = 0x%x\n", PPTable->THERMAL_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_VDD_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_VDD_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_VDD_MAXIMUM_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_VDD_MAXIMUM_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_SOC_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_SOC_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_SOC_MAXIMUM_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_SOC_MAXIMUM_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_SR_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_SR_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "VRM_SR_MAXIMUM_CURRENT_LIMIT = 0x%x\n", PPTable->VRM_SR_MAXIMUM_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "PSI3_VDD_CURRENT_LIMIT = 0x%x\n", PPTable->PSI3_VDD_CURRENT_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "PROCHOT_L_DEASSERTION_RAMP_TIME = 0x%x\n", PPTable->PROCHOT_L_DEASSERTION_RAMP_TIME);
  IDS_HDT_CONSOLE (GNB_TRACE, "P3T_LIMIT = 0x%x\n", PPTable->P3T_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "SYSTEM_CONFIG = 0x%x\n", PPTable->SYSTEM_CONFIG);
  IDS_HDT_CONSOLE (GNB_TRACE, "SW_SCI_GPE_ID = 0x%x\n", PPTable->SW_SCI_GPE_ID);

  //STT parameters/co-efficients
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nSTT parameters/co-efficients\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_PCB_SENSOR_COUNT = 0x%x\n", PPTable->STT_PCB_SENSOR_COUNT);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_MIN_POWER_LIMIT = 0x%x\n", PPTable->STT_MIN_POWER_LIMIT);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M1 = 0x%x\n", PPTable->STT_M1);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M2 = 0x%x\n", PPTable->STT_M2);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M3 = 0x%x\n", PPTable->STT_M3);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M4 = 0x%x\n", PPTable->STT_M4);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M5 = 0x%x\n", PPTable->STT_M5);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_M6 = 0x%x\n", PPTable->STT_M6);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_C_APU = 0x%x\n", PPTable->STT_C_APU);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_C_HS2 = 0x%x\n", PPTable->STT_C_HS2);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_ALPHA_APU = 0x%x\n", PPTable->STT_ALPHA_APU);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_ALPHA_HS2 = 0x%x\n", PPTable->STT_ALPHA_HS2);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_SKIN_TEMPERATURE_LIMIT_APU = 0x%x\n", PPTable->STT_SKIN_TEMPERATURE_LIMIT_APU);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_SKIN_TEMPERATURE_LIMIT_HS2 = 0x%x\n", PPTable->STT_SKIN_TEMPERATURE_LIMIT_HS2);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_ERROR_COEFF = 0x%x\n", PPTable->STT_ERROR_COEFF);
  IDS_HDT_CONSOLE (GNB_TRACE, "STT_ERROR_RATE_COEFF = 0x%x\n", PPTable->STT_ERROR_RATE_COEFF);

  //PCD/CBS fan table
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nPCD/CBS fan table\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_Override = 0x%x\n", PPTable->FanTable_Override);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_Hysteresis = 0x%x\n", PPTable->FanTable_Hysteresis);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_TempLow = 0x%x\n", PPTable->FanTable_TempLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_TempMed = 0x%x\n", PPTable->FanTable_TempMed);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_TempHigh = 0x%x\n", PPTable->FanTable_TempHigh);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_TempCritical = 0x%x\n", PPTable->FanTable_TempCritical);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_PwmLow = 0x%x\n", PPTable->FanTable_PwmLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_PwmMed = 0x%x\n", PPTable->FanTable_PwmMed);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_PwmHigh = 0x%x\n", PPTable->FanTable_PwmHigh);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_PwmFreq = 0x%x\n", PPTable->FanTable_PwmFreq);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_Polarity = 0x%x\n", PPTable->FanTable_Polarity);
  IDS_HDT_CONSOLE (GNB_TRACE, "FanTable_spare = 0x%x\n", PPTable->FanTable_spare);

   //CBS debug options
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nCBS debug options\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "CoreDldoPsmMargin = 0x%x\n", PPTable->CoreDldoPsmMargin);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxDldoPsmMargin = 0x%x\n", PPTable->GfxDldoPsmMargin);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrVddVoltageMargin = 0x%x\n", PPTable->VddcrVddVoltageMargin);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrSocVoltageMargin = 0x%x\n", PPTable->VddcrSocVoltageMargin);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrSrVoltageMargin = 0x%x\n", PPTable->VddcrSrVoltageMargin);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceVddcrVddVoltage = 0x%x\n", PPTable->ForceVddcrVddVoltage);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceVddcrSocVoltage = 0x%x\n", PPTable->ForceVddcrSocVoltage);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceVddcrSrVoltage = 0x%x\n", PPTable->ForceVddcrSrVoltage);
  IDS_HDT_CONSOLE (GNB_TRACE, "C1eEnabled = 0x%x\n", PPTable->C1eEnabled);
  IDS_HDT_CONSOLE (GNB_TRACE, "CCX_VdciAsync = 0x%x\n", PPTable->CCX_VdciAsync);
  IDS_HDT_CONSOLE (GNB_TRACE, "Voltage_Calc_Workaround = 0x%x\n", PPTable->Voltage_Calc_Workaround);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceFanPwmEn = 0x%x\n", PPTable->ForceFanPwmEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceFanPwm = 0x%x\n", PPTable->ForceFanPwm);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfsBypassOff = 0x%x\n", PPTable->DfsBypassOff);
  IDS_HDT_CONSOLE (GNB_TRACE, "CoreCountControlEn = 0x%x\n", PPTable->CoreCountControlEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "HTFmaxTemperature = 0x%x\n", PPTable->HTFmaxTemperature);
  IDS_HDT_CONSOLE (GNB_TRACE, "HTFmaxFrequency = 0x%x\n", PPTable->HTFmaxFrequency);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceCclkFrequency = 0x%x\n", PPTable->ForceCclkFrequency);
  IDS_HDT_CONSOLE (GNB_TRACE, "CclkFmaxOverride = 0x%x\n", PPTable->CclkFmaxOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "CclkFminOverride = 0x%x\n", PPTable->CclkFminOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "ForceGfxclkFrequency = 0x%x\n", PPTable->ForceGfxclkFrequency);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkFmaxOverride = 0x%x\n", PPTable->GfxclkFmaxOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "PostCodeLevel = 0x%x\n", PPTable->PostCodeLevel);
  IDS_HDT_CONSOLE (GNB_TRACE, "StbVerbosityLevel = 0x%x\n", PPTable->StbVerbosityLevel);
  IDS_HDT_CONSOLE (GNB_TRACE, "DisableZ10 = 0x%x\n", PPTable->DisableZ10);
  IDS_HDT_CONSOLE (GNB_TRACE, "ZstatesMaskEnable = 0x%x\n", PPTable->ZstatesMaskEnable);
  IDS_HDT_CONSOLE (GNB_TRACE, "ZscIpIdleMask = 0x%x\n", PPTable->ZscIpIdleMask);
  IDS_HDT_CONSOLE (GNB_TRACE, "ZscWakeEventMask0 = 0x%x\n", PPTable->ZscWakeEventMask0);
  IDS_HDT_CONSOLE (GNB_TRACE, "ZscWakeEventMask1 = 0x%x\n", PPTable->ZscWakeEventMask1);
  IDS_HDT_CONSOLE (GNB_TRACE, "ZscIpFenceAckMask = 0x%x\n", PPTable->ZscIpFenceAckMask);

  //PCD/CBS Telemetry setup
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nPCD/CBS Telemetry setup\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrVddSlope = 0x%x\n", PPTable->TelemetryVddcrVddSlope);
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrVddOffset = 0x%x\n", PPTable->TelemetryVddcrVddOffset);
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrSocSlope = 0x%x\n", PPTable->TelemetryVddcrSocSlope);
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrSocOffset = 0x%x\n", PPTable->TelemetryVddcrSocOffset);
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrSrSlope = 0x%x\n", PPTable->TelemetryVddcrSrSlope);
  IDS_HDT_CONSOLE (GNB_TRACE, "TelemetryVddcrSrOffset = 0x%x\n", PPTable->TelemetryVddcrSrOffset);

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nPRECISE AND DIRECT OVERCLOCKING CONFIG\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_DISABLE = 0x%x\n", PPTable->OC_DISABLE);
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_MINVID = 0x%x\n", PPTable->OC_MINVID);
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_FREQMAX = 0x%x\n", PPTable->OC_FREQMAX);

  //DF CSTATE CONFIG
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nDF CSTATE CONFIG\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateConfigOverride = 0x%x\n", PPTable->DfCstateConfigOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateMmStutterOptEn = 0x%x\n", PPTable->DfCstateMmStutterOptEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateMmPwrGateOptEn = 0x%x\n", PPTable->DfCstateMmPwrGateOptEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateDceElptModeEn = 0x%x\n", PPTable->DfCstateDceElptModeEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstatePwrGateEn = 0x%x\n", PPTable->DfCstatePwrGateEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateClkPwrDnEn = 0x%x\n", PPTable->DfCstateClkPwrDnEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DfCstateSelfRefrEn = 0x%x\n", PPTable->DfCstateSelfRefrEn);

  //DF Light CSTATE CONFIG
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nDF Light CSTATE CONFIG\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "DfLightCstateConfigOverride = 0x%x\n", PPTable->DfLightCstateConfigOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "CcmHystDly = 0x%x\n", PPTable->CcmHystDly);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmhystscale = 0x%x\n", PPTable->Ccmhystscale);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorsuccdly = 0x%x\n", PPTable->Ccmmonitorsuccdly);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorsuccscale = 0x%x\n", PPTable->Ccmmonitorsuccscale);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorsatlvl = 0x%x\n", PPTable->Ccmmonitorsatlvl);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorsuccthr = 0x%x\n", PPTable->Ccmmonitorsuccthr);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorfaildec = 0x%x\n", PPTable->Ccmmonitorfaildec);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorburstfilter = 0x%x\n", PPTable->Ccmmonitorburstfilter);
  IDS_HDT_CONSOLE (GNB_TRACE, "Ccmmonitorburstscale = 0x%x\n", PPTable->Ccmmonitorburstscale);

  //CPPC Constraints
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nCPPC Constraints\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "CppcConstraintsEnabled = 0x%x\n", PPTable->CppcConstraintsEnabled);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerfLimit_MaxRange = 0x%x\n", PPTable->PerfLimit_MaxRange);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerfLimit_MinRange = 0x%x\n", PPTable->PerfLimit_MinRange);
  IDS_HDT_CONSOLE (GNB_TRACE, "EPP_MaxRange = 0x%x\n", PPTable->EPP_MaxRange);
  IDS_HDT_CONSOLE (GNB_TRACE, "EPP_MinRange = 0x%x\n", PPTable->EPP_MinRange);

  //Misc
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nMisc\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "System = 0x%x\n", PPTable->System);
  IDS_HDT_CONSOLE (GNB_TRACE, "MinS0i3SleepTimeInMs = 0x%x\n", PPTable->MinS0i3SleepTimeInMs);
  IDS_HDT_CONSOLE (GNB_TRACE, "SystemRtcTime = 0x%x\n", PPTable->SystemRtcTime);

  //GFX OC limits
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nGFX OC limits\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_GFX_DISABLE = 0x%x\n", PPTable->OC_GFX_DISABLE);
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_GFX_MINVID = 0x%x\n", PPTable->OC_GFX_MINVID);
  IDS_HDT_CONSOLE (GNB_TRACE, "OC_GFX_FREQMAX = 0x%x\n", PPTable->OC_GFX_FREQMAX);

  //GFXCLK/GFXOFF related parameters
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nGFXCLK/GFXOFF related parameters\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "GceaLinkMgrIdleThreshold = 0x%x\n", PPTable->GceaLinkMgrIdleThreshold);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxClkSource = 0x%x\n", PPTable->GfxClkSource);
  IDS_HDT_CONSOLE (GNB_TRACE, "DisableMsmuSecurityRestore = 0x%x\n", PPTable->DisableMsmuSecurityRestore);
  IDS_HDT_CONSOLE (GNB_TRACE, "ApccEnable = 0x%x\n", PPTable->ApccEnable);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxVinit = 0x%x\n", PPTable->GfxVinit);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkFinit = 0x%x\n", PPTable->GfxclkFinit);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkFgfxoffEntry = 0x%x\n", PPTable->GfxclkFgfxoffEntry);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkFidle = 0x%x\n", PPTable->GfxclkFidle);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkSlewRate = 0x%x\n", PPTable->GfxclkSlewRate);
  // Fmin Overrides
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxclkFminOverride = 0x%x\n", PPTable->GfxclkFminOverride);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxMinVoltage = 0x%x\n", PPTable->GfxMinVoltage);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxMaxVoltage = 0x%x\n", PPTable->GfxMaxVoltage);

  //GFX temperature dependent Vmin feature
  IDS_HDT_CONSOLE (GNB_TRACE,  "\nGFX temperature dependent Vmin feature related parameters\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_TVminOverrideEn = 0x%x\n", PPTable->VDDGFX_TVminOverrideEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_TVmin = 0x%x\n", PPTable->VDDGFX_TVmin);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_Vmin_HiTemp = 0x%x\n", PPTable->VDDGFX_Vmin_HiTemp);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_Vmin_LoTemp = 0x%x\n", PPTable->VDDGFX_Vmin_LoTemp);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_TVminHystersis = 0x%x\n", PPTable->VDDGFX_TVminHystersis);

  // Enable automated phase shedding and diode emulation (PSI7)
  IDS_HDT_CONSOLE (GNB_TRACE,  "\n Enable automated phase shedding and diode emulation (PSI7)\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrVddPsi7En = 0x%x\n", PPTable->VddcrVddPsi7En);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrSocPsi7En = 0x%x\n", PPTable->VddcrSocPsi7En);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddcrSrPsi7En = 0x%x\n", PPTable->VddcrSrPsi7En);
  IDS_HDT_CONSOLE (GNB_TRACE, "PsiDecayConditionVddRail = 0x%x\n", PPTable->PsiDecayConditionVddRail);
  IDS_HDT_CONSOLE (GNB_TRACE, "PsiDecayConditionSocRail = 0x%x\n", PPTable->PsiDecayConditionSocRail);
  IDS_HDT_CONSOLE (GNB_TRACE, "PsiDecayConditionSrRail = 0x%x\n", PPTable->PsiDecayConditionSrRail);
  IDS_HDT_CONSOLE (GNB_TRACE, "PsiDecayConditionOverride = 0x%x\n", PPTable->PsiDecayConditionOverride);

  IDS_HDT_CONSOLE (GNB_TRACE, "VddmCldoFastStartupEn = 0x%x\n", PPTable->VddmCldoFastStartupEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "VddmCldoOvDetEn = 0x%x\n", PPTable->VddmCldoOvDetEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "PllSscSpreadPercentage = 0x%x\n", PPTable->PllSscSpreadPercentage);
  IDS_HDT_CONSOLE (GNB_TRACE, "FclkDpmOptimizationEn = 0x%x\n", PPTable->FclkDpmOptimizationEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "DisplayIdleOptimizationEn = 0x%x\n", PPTable->DisplayIdleOptimizationEn);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcdPll5SscDisable = 0x%x\n", PPTable->ApplyDefaultNrepFlopSel);  //ApplyDefaultNrepFlopSel is controlled by PcdPll5SscDisable
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxDldoBypass = 0x%x\n", PPTable->GfxDldoBypass);

  IDS_HDT_CONSOLE (GNB_TRACE, "LclkMaxFreq = 0x%x\n", PPTable->LclkMaxFreq);
  IDS_HDT_CONSOLE (GNB_TRACE, "LclkMinFreq = 0x%x\n", PPTable->LclkMinFreq);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateOverrideVdd = 0x%x\n", PPTable->SlewRateOverrideVdd);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateUpIndexVdd = 0x%x\n", PPTable->SlewRateUpIndexVdd);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateDownCntlVdd = 0x%x\n", PPTable->SlewRateDownCntlVdd);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[0] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[1] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[1]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[2] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[2]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[3] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[3]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[4] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[4]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[5] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[5]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[6] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[6]);
  IDS_HDT_CONSOLE (GNB_TRACE, "PerCoreDldoPsmMargin[7] = 0x%x\n", PPTable->PerCoreDldoPsmMargin[7]);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateOverrideSoc = 0x%x\n", PPTable->SlewRateOverrideSoc);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateUpIndexSoc = 0x%x\n", PPTable->SlewRateUpIndexSoc);
  IDS_HDT_CONSOLE (GNB_TRACE, "SlewRateDownCntlSoc = 0x%x\n", PPTable->SlewRateDownCntlSoc);
  IDS_HDT_CONSOLE (GNB_TRACE, "iTempPart = 0x%x\n", PPTable->iTempPart);

  IDS_HDT_CONSOLE (GNB_TRACE, "Sending this to the SMU...\n");
  GnbLibDebugDumpBuffer ((VOID*) PPTable, sizeof (Smu13BiosIfTable_t), 1, 16);
  IDS_HDT_CONSOLE (GNB_TRACE, "\n");

  return;
}


