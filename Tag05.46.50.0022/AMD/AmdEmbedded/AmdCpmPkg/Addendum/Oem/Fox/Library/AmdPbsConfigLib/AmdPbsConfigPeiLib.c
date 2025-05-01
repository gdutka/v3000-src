/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>

EFI_STATUS
EFIAPI
LoadAmdPbsDefault (
  OUT AMD_PBS_SETUP_OPTION        *AmdPbsConfiguration
  );

/**
 Retrieve AMD PBS setup configuration data

 @param[out] AmdPbsConfig       Pointer to the structure of AMD_PBS_SETUP_OPTION,
                                this pointer must be allocated with sizeof(AMD_PBS_SETUP_OPTION)
                                before being called

 @retval EFI_SUCCESS            The AMD PBS configuration is successfully retrieved
 @retval EFI_INVALID_PARAMETER  NULL pointer for input AmdPbsConfig paramater
 @return others                 Failed to retrieve AMD PBS configuration
**/
EFI_STATUS
EFIAPI
GetAmdPbsConfiguration (
  OUT AMD_PBS_SETUP_OPTION        *AmdPbsConfiguration
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi;
  AMD_PBS_SETUP_OPTION            LocalAmdPbsConfiguration;
  UINTN                           Size;
  AMD_CPM_OEM_TABLE_PPI           *AmdCpmOemTablePpi = NULL;
  BOOLEAN                         OemTableReady;
  UINT16                          CurrentBoardId;

  OemTableReady = TRUE;

  //
  // For Lilac/Mayan, we just use defualt Get/Set variable.
  //
  ASSERT (AmdPbsConfiguration != NULL);
  if (AmdPbsConfiguration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariablePpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesLocatePpi (
             &gAmdCpmOemTablePpiGuid,
             0,
             NULL,
             (VOID **) &AmdCpmOemTablePpi
             );
  if (EFI_ERROR(Status)) {
    OemTableReady = FALSE;
    AmdCpmOemTablePpi = NULL;
  }

  ZeroMem (AmdPbsConfiguration, sizeof (AMD_PBS_SETUP_OPTION));
  ZeroMem (&LocalAmdPbsConfiguration, sizeof (AMD_PBS_SETUP_OPTION));
  CurrentBoardId = PcdGet16 (CurrentPlatformId);

  if (OemTableReady) {
    DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-PcdGet16(CurrentPlatformId) is not ready.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-PcdGet16(CurrentPlatformId): %d\n", CurrentBoardId));
  }
  DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-AMD_CPM_USED_PBS_SIZE: %d \n", AMD_CPM_USED_PBS_SIZE));

  Size = sizeof (AMD_PBS_SETUP_OPTION);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          AMD_PBS_SETUP_VARIABLE_NAME,
                          &gAmdPbsSystemConfigurationGuid,
                          NULL,
                          &Size,
                          &LocalAmdPbsConfiguration
                          );
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-Failed to Get Pbs Setup value. CurrentBoardId: 0x%X\n", CurrentBoardId));
    DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-LoadAmdPbsDefault (...)\n"));
    Status = LoadAmdPbsDefault (&LocalAmdPbsConfiguration);
  }

  // "CurrentId" will be used only for DXE phase. and AmdPbsSetupDxe.c alreayd handle it.
  // do {
  //   DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-AMD PBS Size: %d\n", Size));
  //   DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-AMD PBS CurrentId: %d\n", LocalAmdPbsConfiguration.CurrentId));
  //   DEBUG ((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-AMD PBS AmdCpmUsedPbsSize: %d\n",
  //     LocalAmdPbsConfiguration.AmdCpmUsedPbsSize));

  //   // Copy the original PBS NVRam data.
  //   CopyMem (AmdPbsConfiguration, &LocalAmdPbsConfiguration, Size);

  //   if (OemTableReady) {
  //     // Check Current Platform ID is match preview or not
  //     if (LocalAmdPbsConfiguration.CurrentId != CurrentBoardId) {
  //       /*
  //        * Load the CPM default, Using the skill, we still keep the OEM data original.
  //        * */
  //       Status = EFI_INVALID_PARAMETER;
  //       break;
  //     }
  //   }
  // } while (FALSE);

  // Check the Platform PBS LockDown Structure Size changed.
  if (LocalAmdPbsConfiguration.AmdCpmUsedPbsSize != AMD_CPM_USED_PBS_SIZE) {
      /*
      * Load the CPM default, Using the skill, we still keep the OEM data original.
      * It will have a potential problem, that all PBS structure field might changed.
      * */
    LocalAmdPbsConfiguration.AmdCpmUsedPbsSize = AMD_CPM_USED_PBS_SIZE;
  }
  CopyMem (AmdPbsConfiguration, &LocalAmdPbsConfiguration, Size);
  //
  // Later, It will be sync on SetAmdPbsConfiguration(...)
  //

  /*
  if (Status == EFI_SUCCESS)
  {
    if (LocalAmdPbsConfiguration.AmdCpmUsedPbsSize > 0)
    {
      DefaultUsedPbsSize = LocalAmdPbsConfiguration.AmdCpmUsedPbsSize;
    }
    if (OemTableReady)
    {
      // For LilacTv, Current, we make it reference to BIOS Setup temporary...
      if (LocalAmdPbsConfiguration.PlatformId != ((PcdGet16 (OriginalPlatformId) & BIT2) >> 2)) {
        DefaultUsedPbsSize = 0;     // Force to load all of PBS default value.
      }
      if (LocalAmdPbsConfiguration.CurrentId != (CurrentBoardId)) {
        DefaultUsedPbsSize = 0;     // Force to load all of PBS default value.
      }
    }
    if (DefaultUsedPbsSize < AMD_CPM_USED_PBS_SIZE)
    {
      Status = LoadAmdPbsDefault (&AmdPbsTempConfiguration);
      CopyMem (((UINT8 *) &LocalAmdPbsConfiguration) + DefaultUsedPbsSize, ((UINT8 *) &AmdPbsTempConfiguration) + DefaultUsedPbsSize, AMD_CPM_USED_PBS_SIZE - DefaultUsedPbsSize);
      for (Index = DefaultUsedPbsSize; Index < AMD_CPM_USED_PBS_SIZE; Index ++)
      {
        DEBUG((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-Update PBS Index[0x%X] = 0x%X to 0x%X\n", Index, ((UINT8 *) &AmdPbsTempConfiguration)[Index], ((UINT8 *) &LocalAmdPbsConfiguration)[Index]));
      }
      if (LocalAmdPbsConfiguration.AmdCpmUsedPbsSize != AMD_CPM_USED_PBS_SIZE) {
        LocalAmdPbsConfiguration.AmdCpmUsedPbsSize = AMD_CPM_USED_PBS_SIZE;
        DEBUG((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-Update AmdCpmUsedPbsSize to 0x%X\n", LocalAmdPbsConfiguration.AmdCpmUsedPbsSize));
      }
      if (LocalAmdPbsConfiguration.PlatformId != ((PcdGet16 (OriginalPlatformId) & BIT2) >> 2)) {
        LocalAmdPbsConfiguration.PlatformId = ((PcdGet16 (OriginalPlatformId) & BIT2) >> 2);
        DEBUG((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-Update PlatformId to 0x%X\n", LocalAmdPbsConfiguration.PlatformId));
      }
      if (LocalAmdPbsConfiguration.CurrentId != (CurrentBoardId)) {
        LocalAmdPbsConfiguration.CurrentId = CurrentBoardId;
        DEBUG((DEBUG_INFO, "OEM-PEI-GetAmdPbsConfiguration-Update CurrentId to 0x%X\n", LocalAmdPbsConfiguration.CurrentId));
      }
    }
    CopyMem (AmdPbsConfiguration, &LocalAmdPbsConfiguration, Size);
  } else {
    DEBUG((DEBUG_INFO, "Failed to  0x%X\n", LocalAmdPbsConfiguration.CurrentId));
    Status = LoadAmdPbsDefault (AmdPbsConfiguration);
  }
  */

  return Status;
}
