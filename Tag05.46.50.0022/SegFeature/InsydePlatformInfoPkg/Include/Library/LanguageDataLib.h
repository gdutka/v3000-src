/** @file
  Language Data Library Definition.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __LANGUAGE_DATA_LIB_H__
#define __LANGUAGE_DATA_LIB_H__

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>


//
// This structure is reference defined at SetupUtilityLib.h
//
#define RFC_3066_ENTRY_SIZE             (42 + 1)

//
// This structure is reference defined at SetupUtilityLibCommon.h
//
typedef struct {
  UINTN    LangNum;
  UINT8    LangString[1];
} LANGUAGE_DATA;

/**
  Determines if a IETF (BCP 47) language tag matches the specified prefix.

  The language tags are compared in a case-insensitive manner.

  @param[in]  Language  A string containing an IETF language tag
  @param[in]  Prefix    A string containing a language prefix to match

  @return TRUE if the language tag matches the prefix.

**/
BOOLEAN
LanguageTagMatchesPrefix (
  IN CONST CHAR8  *Language,
  IN CONST CHAR8  *Prefix
  );

/**
  Get supported language database. This funciton will return supported language number
  and language string

  @param  LangNumber             Pointer to supported language number
  @param  LanguageString         A double pointer to save the start of supported language string

  @retval EFI_SUCCESS            Initialize supported language database successful
  @retval EFI_INVALID_PARAMETER  Input parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Unable allocate memory for language string.

**/
EFI_STATUS
GetSupportLang (
  OUT UINTN            *LangNumber,
  OUT UINT8            **LanguageString
  );

EFI_STRING_ID
HiiSetStringFromFormatStringVa (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID       DestinationStringId OPTIONAL,
  IN EFI_STRING_ID       FormatStringId,
  IN VA_LIST             Marker
  );

EFI_STRING_ID
HiiSetStringFromFormatString (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID       DestinationStringId OPTIONAL,
  IN EFI_STRING_ID       FormatStringId,
  ...
  );

#endif