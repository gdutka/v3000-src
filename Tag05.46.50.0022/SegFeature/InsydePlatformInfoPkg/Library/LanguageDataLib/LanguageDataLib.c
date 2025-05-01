/** @file
  Implementation of LanguageDataLib.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/VariableLib.h>
#include <Library/LanguageDataLib.h>

STATIC LANGUAGE_DATA  *mSupportedLangTable;

STATIC
CHAR8
AsciiCharToLower (
  IN CHAR8  Char
  )
{
  return ((Char >= 'A' && Char <= 'Z') ? Char + ('a' - 'A') : Char);
}

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
  )
{
  CHAR8  LastChar;
  UINTN  Index;
  CHAR8  LanguageChar;
  CHAR8  PrefixChar;

  if ((Language == NULL) || (Prefix == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Language/Prefix is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return FALSE;
  }

  LastChar = '\0';
  for (Index = 0; TRUE; Index++) {
    LanguageChar = AsciiCharToLower (Language[Index]);
    PrefixChar = AsciiCharToLower (Prefix[Index]);
    if (LanguageChar != PrefixChar || LanguageChar == '\0') {
      break;
    }
    LastChar = PrefixChar;
  }
  return (PrefixChar == '\0' &&
    (LanguageChar == '\0' || LanguageChar == '-' || LastChar == '-'));
}

/**
  Get next language from language code list (with separator ';').

  If LangCode is NULL, then ASSERT.
  If Lang is NULL, then ASSERT.

  @param  LangCode               On input: point to first language in the list.
                                 On output: point to next language in the list, or
                                 NULL if no more language in the list.
  @param  Lang                   The first language in the list.

**/
STATIC
VOID
EFIAPI
GetNextLanguage (
  IN OUT CHAR8      **LangCode,
  OUT CHAR8         *Lang
  )
{
  UINTN  Index;
  CHAR8  *StringPtr;

  if ((LangCode == NULL) || (Lang == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The LangCode/Lang is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  if (*LangCode == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The *LangCode is NULL, please check initialize content\n",
      __FUNCTION__
      ));
    return;
  }

  Index = 0;
  StringPtr = *LangCode;
  while (StringPtr[Index] != 0 && StringPtr[Index] != ';') {
    Index++;
  }

  CopyMem (Lang, StringPtr, Index);
  Lang[Index] = 0;

  if (StringPtr[Index] == ';') {
    Index++;
  }
  *LangCode = StringPtr + Index;
}

/**
  According the priority of langdef in UNI file to add the supported language code
  to supported language database.

  @retval EFI_SUCCESS    Initialize supported language database successful
  @retval Other          Get setup utility browser data fail

**/
STATIC
EFI_STATUS
InitializeSupportLanguage (
  VOID
  )
{
  EFI_STATUS                    Status;
  CHAR8                         *Language;
  CHAR8                         Lang[RFC_3066_ENTRY_SIZE];
  CHAR8                         *SuportedLanguage;
  UINTN                         SupportedLangCnt;
  UINTN                         DataSize;

  DataSize = 0;
  SuportedLanguage = NULL;

  Status = CommonGetVariableDataAndSize (
             L"PlatformLangCodes",
             &gEfiGlobalVariableGuid,
             &DataSize,
             (VOID **)&SuportedLanguage
             );
  if (EFI_ERROR (Status) && (SuportedLanguage == NULL)) {
    return Status;
  }

  SupportedLangCnt = 0;
  Language  = SuportedLanguage;
  while (*Language != 0) {
    GetNextLanguage (&Language, Lang);
    if (LanguageTagMatchesPrefix (Lang, UEFI_CONFIG_LANG)) {
      continue;
    }
    if (LanguageTagMatchesPrefix (Lang, UEFI_CONFIG_LANG_2)) {
      continue;
    }
    SupportedLangCnt++;
  }

  mSupportedLangTable = AllocateZeroPool (SupportedLangCnt * RFC_3066_ENTRY_SIZE + sizeof (UINTN));
  if (mSupportedLangTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  FreePool (SuportedLanguage);
  SuportedLanguage = NULL;
  Status = CommonGetVariableDataAndSize (
             L"PlatformLangCodes",
             &gEfiGlobalVariableGuid,
             &DataSize,
             (VOID **)&SuportedLanguage
             );
  if (EFI_ERROR (Status) && (SuportedLanguage == NULL)) {
    FreePool (mSupportedLangTable);
    mSupportedLangTable = NULL;
    return Status;
  }

  SupportedLangCnt = 0;
  Language  = SuportedLanguage;
  while (*Language != 0) {
    GetNextLanguage (&Language, Lang);
    if (LanguageTagMatchesPrefix (Lang, UEFI_CONFIG_LANG)) {
      continue;
    }
    if (LanguageTagMatchesPrefix (Lang, UEFI_CONFIG_LANG_2)) {
      continue;
    }
    AsciiStrCpyS ((CHAR8 *) &mSupportedLangTable->LangString[SupportedLangCnt * RFC_3066_ENTRY_SIZE], RFC_3066_ENTRY_SIZE, Lang);
    SupportedLangCnt++;
  }

  mSupportedLangTable->LangNum = SupportedLangCnt;
  FreePool (SuportedLanguage);

  return EFI_SUCCESS;

}

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
  )
{
  UINTN       TotalSize;
  EFI_STATUS  Status;

  if (LangNumber == NULL || LanguageString == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (mSupportedLangTable == NULL) {
    Status = InitializeSupportLanguage ();
    if (EFI_ERROR(Status) || mSupportedLangTable == NULL) {
      return Status;
    }
  }

  TotalSize = mSupportedLangTable->LangNum * RFC_3066_ENTRY_SIZE;
  *LanguageString = AllocateZeroPool (TotalSize);
  if (*LanguageString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (*LanguageString, mSupportedLangTable->LangString, TotalSize);
  *LangNumber = mSupportedLangTable->LangNum;

  return EFI_SUCCESS;
}
