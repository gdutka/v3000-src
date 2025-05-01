/** @file


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
#include <Uefi/UefiInternalFormRepresentation.h>

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>              // MdeModulePkg
#include <Library/LanguageDataLib.h>     // InsydePlatformInfoPkg

EFI_STRING_ID
HiiSetStringFromFormatStringVa (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID       DestinationStringId OPTIONAL,
  IN EFI_STRING_ID       FormatStringId,
  IN VA_LIST             Marker
  )
{
  EFI_STRING_ID  NewStringId;
  BOOLEAN        IsNewString;

  CHAR8          *Languages;
  CONST CHAR8    *Language;
  UINTN          CharIndex;
  BOOLEAN        IsLastLanguage;

  EFI_STRING     Format;
  VA_LIST        ExtraMarker;
  CHAR16         *StringBuffer;
  EFI_STRING_ID  StringId;

  ASSERT (HiiHandle != NULL);

  NewStringId = 0x0000;
  IsNewString = (DestinationStringId == 0x0000);

  Languages = HiiGetSupportedLanguages (HiiHandle);
  if (Languages == NULL) {
    goto NoLanguages;
  }

  Language = Languages;
  CharIndex = 0;
  while (TRUE) {
    while (Languages[CharIndex] != '\0' && Languages[CharIndex] != ';') {
      CharIndex++;
    }

    IsLastLanguage = (Languages[CharIndex] == '\0');

    Languages[CharIndex] = '\0';

    if (LanguageTagMatchesPrefix (Language, UEFI_CONFIG_LANG)) {
      //
      // Languages with "x-UEFI" prefix are reserved by EFI Config Keyword
      // Handler Protocol.
      //
      continue;
    }
    if (LanguageTagMatchesPrefix (Language, UEFI_CONFIG_LANG_2)) {
      continue;
    }

    Format = HiiGetString (HiiHandle, FormatStringId, Language);
    if (Format == NULL) {
      goto OutOfResources;
    }

    VA_COPY (ExtraMarker, Marker);

    StringBuffer = CatVSPrint (NULL, (CHAR16 *) Format, ExtraMarker);
    FreePool (Format);
    VA_END (ExtraMarker);
    if (StringBuffer == NULL) {
      goto OutOfResources;
    }

    StringId = HiiSetString (
                 HiiHandle,
                 DestinationStringId,
                 (EFI_STRING) StringBuffer,
                 Language
                 );
    FreePool (StringBuffer);
    if (StringId == 0x0000) {
      //
      // For a newly created string, return the string ID even when not all
      // languages can be set for it. This avoids string ID resource leak.
      // Note: HiiSetString() from EDK II HiiLib does leak string ID when
      // creating a string for multiple languages.
      //
      if (!IsNewString) {
        NewStringId = 0x0000;
      }
      goto OutOfResources;
    }

    NewStringId = StringId;
    if (DestinationStringId == 0x0000) {
      DestinationStringId = StringId;
    }

    if (IsLastLanguage) {
      break;
    }

    Languages[CharIndex] = ';';
    CharIndex++;
    Language = &Languages[CharIndex];
  }

OutOfResources:
  FreePool (Languages);
NoLanguages:
  return NewStringId;
}

EFI_STRING_ID
HiiSetStringFromFormatString (
  IN OUT EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID       DestinationStringId OPTIONAL,
  IN EFI_STRING_ID       FormatStringId,
  ...
  )
{
  VA_LIST        Marker;
  EFI_STRING_ID  NewStringId;

  VA_START (Marker, FormatStringId);

  NewStringId = HiiSetStringFromFormatStringVa (
                  HiiHandle,
                  DestinationStringId,
                  FormatStringId,
                  Marker
                  );

  VA_END (Marker);

  return NewStringId;
}