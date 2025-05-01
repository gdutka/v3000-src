/** @file
    Provide support functions for sensitive variable.

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

#include "SensitiveVariable.h"

EFI_STATUS
EFIAPI
SmmSetVariable (
  IN CHAR16        *VariableName,
  IN EFI_GUID      *VendorGuid,
  IN UINT32        Attributes,
  IN UINTN         DataSize,
  IN VOID          *Data
  );

/**
  This function uses to set sensitive variable in SMM.

  @retval EFI_SUCCESS     Set sensitive variable successfully.
  @return Other           Set sensitive variable failed.

**/
EFI_STATUS
SmmSetSensitiveVariable (
  VOID
  )
{
  SMM_VAR_BUFFER    *SmmVarBuffer;
  CHAR16            *VariableName;
  UINT8             *VariableBuffer;
  EFI_STATUS        Status;

  if (VariableAtRuntime ()) {
    return EFI_UNSUPPORTED;
  }
  SmmVarBuffer = mVariableModuleGlobal->SmmVarBuf;
  if (SmmVarBuffer->Signature != SET_SENSITIVE_VARIABLE_SIGNATURE) {
    return EFI_UNSUPPORTED;
  }
  if ((SmmVarBuffer->Attributes & (EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)) != 0) {
    return EFI_ACCESS_DENIED;
  }

  VariableName = (CHAR16 *) (SmmVarBuffer + 1);
  VariableBuffer = ((UINT8 *) (SmmVarBuffer + 1)) + SmmVarBuffer->VariableNameSize;
  Status = SmmSetVariable (
             VariableName,
             &SmmVarBuffer->VarGuid,
             SmmVarBuffer->Attributes,
             SmmVarBuffer->DataSize,
             VariableBuffer
             );
  return Status;
}

