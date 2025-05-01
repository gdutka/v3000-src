/** @file
    The header file for variable lock service.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _VARIABLE_POLICY_H_
#define _VARIABLE_POLICY_H_
#include "Variable.h"

EFI_STATUS
EFIAPI
ProtocolDisableVariablePolicy (
  VOID
  );

EFI_STATUS
EFIAPI
ProtocolIsVariablePolicyEnabled (
  OUT BOOLEAN     *State
  );

EFI_STATUS
EFIAPI
ProtocolRegisterVariablePolicy (
  IN CONST VARIABLE_POLICY_ENTRY    *NewPolicy
  );

EFI_STATUS
EFIAPI
ProtocolDumpVariablePolicy (
  OUT UINT8             *Policy OPTIONAL,
  IN OUT UINT32         *Size
  );

EFI_STATUS
EFIAPI
ProtocolLockVariablePolicy (
  VOID
  );

VOID
ConvertVariablePolicyTable (
  VOID
  );

/**
  This API function checks to see whether the parameters to SetVariable would
  be allowed according to the current variable policies.

  @param[in]  VariableName       Same as EFI_SET_VARIABLE.
  @param[in]  VendorGuid         Same as EFI_SET_VARIABLE.
  @param[in]  Attributes         Same as EFI_SET_VARIABLE.
  @param[in]  DataSize           Same as EFI_SET_VARIABLE.
  @param[in]  Data               Same as EFI_SET_VARIABLE.

  @retval     EFI_SUCCESS             A matching policy allows this update.
  @retval     EFI_SUCCESS             There are currently no policies that restrict this update.
  @retval     EFI_SUCCESS             The protections have been disable until the next reboot.
  @retval     EFI_WRITE_PROTECTED     Variable is currently locked.
  @retval     EFI_INVALID_PARAMETER   Attributes or size are invalid.
  @retval     EFI_ABORTED             A lock policy exists, but an error prevented evaluation.
  @retval     EFI_NOT_READY           Library has not been initialized.

**/
EFI_STATUS
EFIAPI
ValidateSetVariable (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid,
  IN  UINT32                       Attributes,
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  );

/**
  Create varaible policy table in SMM.

  @retval     EFI_SUCCESS             Create variable policy table in SMM.
  @retval     EFI_OUT_OF_RESOURCES    There is not enough resource for variable policy table in SMM.
**/
EFI_STATUS
EFIAPI
CreatePolicyTable (
  VOID
  );

#endif
