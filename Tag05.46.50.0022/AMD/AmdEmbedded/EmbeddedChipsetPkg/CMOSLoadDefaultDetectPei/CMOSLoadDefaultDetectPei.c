/** @file
  This module forces BIOS load default if CMOS checksum corrupted

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/CmosLib.h>
#include <Library/H2OCpLib.h>
#include <Guid/H2OCp.h>

/**
 Check Cmos checksum to decide whether to load default at ForceVariableDefaults check point.

 @param[in] Event                      The Event this notify function registered to.
 @param[in] Handle                     The handle associated with a previously registered checkpoint handler.
**/
STATIC
VOID
EFIAPI
ForceVariableDefaultsCpHandler (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                               Status;
  H2O_PEI_CP_FORCE_VARIABLE_DEFAULTS_DATA  *ForceVariableDefaultsData;
  EFI_GUID                                 ForceVariableDefaultsGuid;

  Status = H2OCpLookup (Handle, (VOID **) &ForceVariableDefaultsData, &ForceVariableDefaultsGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }

  Status = ValidateCmosChecksum ();
  DEBUG ((DEBUG_INFO, "Cmos checksum is "));
  if (Status != EFI_SUCCESS){
    ForceVariableDefaultsData->Status = H2O_CP_TASK_UPDATE;
    ForceVariableDefaultsData->ForceVariableDefaults = TRUE;
    DEBUG ((DEBUG_INFO, "invalid\n"));
  } else {
    DEBUG ((DEBUG_INFO, "valid\n"));
  }
  H2OCpUnregisterHandler (Handle);
}

/**
  Initialize register ForceVariableDefaults check point in PEI phase.

  @param FileHandle             Handle of the file being invoked.
  @param PeiServices            Describes the list of possible PEI Services.

 @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
CMOSLoadDefaultDetectEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                            Status;
  H2O_CP_HANDLE                         CpHandle;

  DEBUG ((EFI_D_INFO, "%a Start\n", __FUNCTION__));
  Status = H2OCpRegisterHandler (&gH2OPeiCpForceVariableDefaultsGuid , ForceVariableDefaultsCpHandler , H2O_CP_MEDIUM, &CpHandle);
  DEBUG ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpForceVariableDefaultsGuid, Status));
  DEBUG ((EFI_D_INFO, "%a End\n", __FUNCTION__));
  
  return EFI_SUCCESS;
}
