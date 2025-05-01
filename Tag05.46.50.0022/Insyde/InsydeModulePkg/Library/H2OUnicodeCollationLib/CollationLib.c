/** @file
  Unicode Collation library.
  
;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/UnicodeCollation.h>

/**
  Check if strings are equal using Unicode Collation.
   
  @param[in] Str1                The first null-terminated string.
  @param[in] Str2                The second null-terminated string.
   
  @retval 0                      Strings are equal.
  @retval 1                      Str1 is lexically greater than Str2
  @retval -1                     Str1 is lexically less than Str2
  @retval -2                     Locate UnicodeCollation protocol fail.
**/
INT32 
CollationStriColl (
   IN CONST CHAR16 *Str1,
   IN CONST CHAR16 *Str2
  )
{
  EFI_STATUS                      Status;
  EFI_UNICODE_COLLATION_PROTOCOL  *UnicodeCollation;  

  UnicodeCollation = NULL;
  Status = gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid, NULL, (VOID **) &UnicodeCollation);
  
  if (EFI_ERROR(Status)) {
    return -2;
  }
   
  return (INT32) UnicodeCollation->StriColl(UnicodeCollation, (CHAR16 *)Str1, (CHAR16 *)Str2);
}
 
/**
  Check whether the string matches the pattern, using Unicode Collation

  @param[in] Str                 The string.
  @param[in] Pattern             The pattern.
   
  @retval    TRUE                Match
  @retval    FALSE               No match.
**/
BOOLEAN
CollationMetaiMatch(
  IN CONST CHAR16 *Str,
  IN CONST CHAR16 *Pattern
  )
{
  EFI_STATUS                      Status;
  EFI_UNICODE_COLLATION_PROTOCOL  *UnicodeCollation;  

  UnicodeCollation = NULL;
  Status = gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid, NULL, (VOID **) &UnicodeCollation);

  if (EFI_ERROR(Status)) {
    return FALSE;
  }
   
  return UnicodeCollation->MetaiMatch(UnicodeCollation, (CHAR16 *)Str, (CHAR16 *)Pattern);
}

/**
  Convert a string to all lower case characters.

  @param[in,out]   Str                 The string.
   
**/
VOID
CollationStrLwr(
  IN OUT CHAR16 *Str
  )
{
  EFI_STATUS                      Status;
  EFI_UNICODE_COLLATION_PROTOCOL  *UnicodeCollation;  

  UnicodeCollation = NULL;
  Status = gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid, NULL, (VOID **) &UnicodeCollation);

  if (EFI_ERROR(Status)) {
    return;
  }
   
  UnicodeCollation->StrLwr(UnicodeCollation, Str);
}

/**
  Convert a string to all upper case characters.
   
  @param[in,out]   Str                 The string.
   
**/
VOID
CollationStrUpr(
  IN OUT CHAR16 *Str
  )
{
  EFI_STATUS                      Status;
  EFI_UNICODE_COLLATION_PROTOCOL  *UnicodeCollation;  

  UnicodeCollation = NULL;
  Status = gBS->LocateProtocol (&gEfiUnicodeCollation2ProtocolGuid, NULL, (VOID **) &UnicodeCollation);

  if (EFI_ERROR(Status)) {
    return;
  }
   
  UnicodeCollation->StrUpr(UnicodeCollation, Str);
}
