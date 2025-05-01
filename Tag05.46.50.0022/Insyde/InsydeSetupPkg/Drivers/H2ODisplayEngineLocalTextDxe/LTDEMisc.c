/** @file
  Common functions for H2O display engine driver.

;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <H2ODisplayEngineLocalText.h>

/**
 Copy HII value.

 @param[out] DstHiiValue         Pointer to destination HII value
 @param[in]  SrcHiiValue         Pointer to source HII value
**/
VOID
CopyHiiValue (
  OUT EFI_HII_VALUE                           *DstHiiValue,
  IN  EFI_HII_VALUE                           *SrcHiiValue
  )
{
  if (DstHiiValue == NULL || SrcHiiValue == NULL) {
    return;
  }

  CopyMem (DstHiiValue, SrcHiiValue, sizeof (EFI_HII_VALUE));
  if (SrcHiiValue->Buffer != NULL && SrcHiiValue->BufferLen != 0) {
    DstHiiValue->Buffer = AllocateCopyPool (SrcHiiValue->BufferLen, SrcHiiValue->Buffer);
  }
}

CHAR16 *
CreateString (
  IN UINT32                                   StringLength,
  IN CHAR16                                   StringChar
  )
{
  CHAR16                                      *String;
  UINT32                                      Index;

  String = AllocateZeroPool ((StringLength + 1) * sizeof (CHAR16));
  if (String == NULL) {
    return NULL;
  }

  for (Index = 0; Index < StringLength; Index++) {
    String[Index] = StringChar;
  }

  return String;
}

BOOLEAN
IsPointOnField (
  IN RECT                                     *ControlAbsField,
  IN INT32                                    Column,
  IN INT32                                    Row
  )
{
  if (ControlAbsField != NULL &&
      IN_RANGE (Row   , ControlAbsField->top , ControlAbsField->bottom) &&
      IN_RANGE (Column, ControlAbsField->left, ControlAbsField->right)) {
    return TRUE;
  }

  return FALSE;
}

