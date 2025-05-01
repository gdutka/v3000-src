/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/*++
Module Name:

  AmdSocFp7RmbPei.c
  Init Soc interface

Abstract:
--*/

#include <Filecode.h>
#include "AGESA.h"
#include <CcxCommon.h>
#include <Library/CcxSetMmioCfgBaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/PeiSocLogicalIdServicesLib.h>
#include <Library/PeiSocZen3ServicesLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/FabricResourceInitLib.h>
#include <Library/AmdPspApobLib.h>
#include <Library/PeiSocBistLogging2Lib.h>
#include <Library/PcdLib.h>
#include <Ppi/AmdSocPpi.h>
#include <Ppi/AmdSocPcdInitPpi.h>
#include <Ppi/AmdCcxPpi.h>
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include "AmdSocFp7RmbPei.h"
#include <Library/AmdEmulationFlagLib.h>
#include <Library/PresiliconControlRmbLib.h>
#include <Library/AmdStbLib.h>

#define FILECODE SOC_AMDSOCFP7RMBPEI_AMDSOCFP7RMBPEI_FILECODE

extern
EFI_STATUS
EFIAPI
BuildSocCoreInfoHob (
  VOID
  );

EFI_STATUS
EFIAPI
SocAfterPeiInitCompleteCallback (
  IN  CONST EFI_PEI_SERVICES                **PeiServices,
  IN        EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN        VOID                            *InvokePpi
  );

STATIC PEI_AMD_SOC_PPI mAmdSocInstallPpiTemplate = {
  AMD_SOC_PPI_REVISION,
};

STATIC PEI_AMD_PCD_INIT_READY_PPI mAmdPcdInitReadyPpi = {
  PEI_AMD_PCD_INIT_READY_PPI_REVISION,
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdPcdInitReady = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPcdInitReadyPpiGuid,
  &mAmdPcdInitReadyPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR   mAfterCcxPeiInitCompletePpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCcxPeiInitCompletePpiGuid,
  SocAfterPeiInitCompleteCallback
};

EFI_STATUS
SocPublishSiliconPpi (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN CONST EFI_GUID            *SiliconDriverId
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_PEI_PPI_DESCRIPTOR    *SocPpiDescriptorTemplate;

  // Allocate memory for the PPI descriptor
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (EFI_PEI_PPI_DESCRIPTOR),
                             (VOID **)&SocPpiDescriptorTemplate
                             );
  ASSERT_EFI_ERROR ( Status);

  SocPpiDescriptorTemplate->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  SocPpiDescriptorTemplate->Guid  = (EFI_GUID*)SiliconDriverId;
  SocPpiDescriptorTemplate->Ppi   = &mAmdSocInstallPpiTemplate;

  Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             SocPpiDescriptorTemplate
                             );
  return (Status);
}

/*++

Routine Description:

  SOC Driver Entry. Initialize SOC device and publish silicon driver installation PPI

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdSocFp7RmbPeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINT8               i;
  SOC_ID_STRUCT       SocId;
  SOC_ID_STRUCT       SocId2;
  EFI_STATUS          Status = EFI_SUCCESS;
  PEI_AMD_AGESA_PARAMETER_GROUP_ID_PPI *PeiAmdAgesaParameterGroupIdPpi;
  AMD_PCD_PLATFORM_GROUP_LIST *AmdPcdPlatformGroupList;
  EFI_GUID *PlalformGroupGuid = NULL;
  APOB_TYPE_HEADER *ApobTypeHeader;

  DEBUG ((EFI_D_ERROR, "*****************************PEI SOC FP7 RMB Driver Entry*********************\n"));

  // Check Hardware Identification
  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_FP7;

  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = ZEN3_PKG_FP7_B0;

  if (SocHardwareIdentificationCheck (&SocId) || SocHardwareIdentificationCheck (&SocId2)) {
    // Init PCIE Configuration Space
    CcxSetMmioCfgBaseLib ();
    // Init DF resource
    FabricResourceInit ();
    //Init APOB service
    AmdPspApobLibConstructor ();
    // Install fabric soc specific services PPI
///@todo    FabricSocSpecificServicesPpiInstall (PeiServices);

    //
    // Parameter initailization
    //
    Status = (*PeiServices)->LocatePpi (
                PeiServices,
                &gAmdSocAgesaGroupIdentificationPpiGuid,
                0,
                NULL,
                (VOID**)&PeiAmdAgesaParameterGroupIdPpi
                );

    if (EFI_SUCCESS == Status) {
      AmdPcdPlatformGroupList = PeiAmdAgesaParameterGroupIdPpi->AmdConfigurationPcdGroupList;

      //Pass SOC ID to platform BIOS before PCD init.
      Status = PeiAmdAgesaParameterGroupIdPpi->AmdSocCallBackPei (PeiServices, &SocId, &PlalformGroupGuid);
      ASSERT_EFI_ERROR (Status);
      if (NULL == PlalformGroupGuid) {
        PlalformGroupGuid = AmdPcdPlatformGroupList->PlalformGroupGuid;
      }

      //find Group Agesa configuration list
      for (i = 0; i < AmdPcdPlatformGroupList->NumberOfGroupsList; i++) {
        if (CompareGuid (PlalformGroupGuid, (EFI_GUID *)AmdPcdPlatformGroupList->GroupList[i].PlalformGroupGuid)) {
          // Update PCD
          AmdPcdInit (AmdPcdPlatformGroupList->GroupList[i].NumberOfPcdEntries, (AMD_PCD_LIST *)AmdPcdPlatformGroupList->GroupList[i].ListEntry);
        }
      }
    }

    //
    // Publish the logical ID PPI
    //
    Status = SocLogicalIdServicePpiInstall (PeiServices);
    ASSERT_EFI_ERROR (Status);

    //
    // Publish the Zen3 services PPI
    //
    Status = SocZen3ServicesPpiInstall (PeiServices);
    ASSERT_EFI_ERROR (Status);
    Status = BuildSocCoreInfoHob ();
    ASSERT_EFI_ERROR (Status);
    PcdSetBoolS (PcdAmdNewDownCoreSequence, TRUE);
    IDS_HOOK (IDS_HOOK_BEGINNING_OF_AGESA, NULL, NULL);

    //
    //Publish Silicon Driver Installation PPI
    //
    Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             &mAmdPcdInitReady
                             );
    ASSERT_EFI_ERROR (Status);

    //
    //Publish Silicon Driver Installation PPI
    //
    for (i = 0 ; i < SiliconDriverPpiListNumber; i++) {
      SocPublishSiliconPpi (PeiServices, SiliconDriverPpiList[i]);
    }

    (**PeiServices).NotifyPpi (PeiServices, &mAfterCcxPeiInitCompletePpiCallback);

    PcdSet32S(PcdAmdCcxEnabledFeatures, PcdGet32(PcdAmdCcxEnabledFeatures) | CCX_FEATURE_CSTATE_LPI);

    if (AmdPspGetApobEntryInstance (APOB_GEN, APOB_ENV_FLAGS_INFO_TYPE, 0, FALSE, &ApobTypeHeader) == EFI_SUCCESS) {
      PcdSet32S (PcdAmdPreSilCtrl0, ((APOB_ENV_FLAGS_STRUCT *)ApobTypeHeader)->Flags.PreSilCtrl0.Value);
      PcdSet32S (PcdAmdPreSilCtrl1, ((APOB_ENV_FLAGS_STRUCT *)ApobTypeHeader)->Flags.PreSilCtrl1.Value);
      PcdSet32S (PcdMpPostcodeIp0SmnAddress, RMB_MP_POSTCODE_IP0_SMN_ADDRESS);
    }

    PRE_SILICON_INIT(InitEmulationFlagRmb);

    DISPLAY_EMU_FLAG();

  }

  DEBUG ((EFI_D_ERROR, "*****************************PEI SOC FP7 RMB Driver Exit*********************\n"));
  return (Status);
}

/*++

Routine Description:

  Callback routine once gAmdCcxPeiInitCompletePpiGuid has been installed to ensure the following services have been published
  - Core Topology Services
  - Fabric Topology Services
  - Nbio SMU Services

Arguments:
  **PeiServices
  *NotifyDesc
  *InvokePpi

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
SocAfterPeiInitCompleteCallback (
  IN  CONST EFI_PEI_SERVICES                **PeiServices,
  IN        EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN        VOID                            *InvokePpi
  )
{

  LogBistStatusV2 (PeiServices);

  return EFI_SUCCESS;
}


