/** @file
 Library Instance for PeiKernelConfigLib Library Class.

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/SetupDefaultHob.h>
#include <KernelSetupConfig.h>
#include <ChipsetSetupConfig.h>

#define MAX_SETUP_VARIABLE_SIZE 0x400

/**
 Retrieve kernel setup configuration data

 @param[out] KernelConfig       Pointer to the structure of KERNEL_CONFIGURATION,
                                this pointer must be allocated with sizeof(KERNEL_CONFIGURATION)
                                before being called

 @retval EFI_SUCCESS            The kernel configuration is successfully retrieved
 @retval EFI_INVALID_PARAMETER  NULL pointer for input KernelConfig paramater
 @return others                 Failed to retrieve kernel configuration
**/
EFI_STATUS
EFIAPI
GetKernelConfiguration (
  OUT KERNEL_CONFIGURATION         *KernelConfig
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariablePpi;
  UINT8                            SystemConfiguration[MAX_SETUP_VARIABLE_SIZE];
  UINTN                            Size;

  if (KernelConfig == NULL) {
    ASSERT_EFI_ERROR (KernelConfig != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariablePpi
             );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Size = MAX_SETUP_VARIABLE_SIZE;
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          SETUP_VARIABLE_NAME,
                          &gSystemConfigurationGuid,
                          NULL,
                          &Size,
                          SystemConfiguration
                          );
  if (Status == EFI_SUCCESS) {
    CopyMem (KernelConfig, SystemConfiguration, sizeof(KERNEL_CONFIGURATION));
  }

  return Status;
}

/**
 Retrieve the size of Setup variable

 @return                        Size of the setup variable data
**/
UINTN
EFIAPI
GetSetupVariableSize (
  VOID
  )
{
  return sizeof (CHIPSET_CONFIGURATION);
}

