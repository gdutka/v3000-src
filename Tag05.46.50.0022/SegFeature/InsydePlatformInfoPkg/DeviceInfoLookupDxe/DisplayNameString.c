/** @file


;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
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
#include <Library/UefiLib.h>

#include "DeviceInfoLookupDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC
BOOLEAN
IsSpace (
  IN INT32  Char
  )
{
  return (Char == L' ' || (Char <= L'\r' && Char >= L'\t'));
}

/**

  @retval NULL
  @retval Other

**/
CHAR8 *
EFIAPI
CreateDisplayNameString (
  IN CONST CHAR8  *SourceString
  )
{
  UINTN  Length;
  CHAR8  *Buffer;
  UINTN  Index;

  if (SourceString == NULL) {
    return NULL;
  }

  while (IsSpace (*SourceString)) {
    ++SourceString;
  }
  //
  // Determine length based on the last non-whitespace character.
  //
  Length = 0;
  for (Index = 0; SourceString[Index] != '\0'; Index++) {
    if (!IsSpace (SourceString[Index])) {
      Length = Index + 1;
    }
  }

  Buffer = AllocatePool (Length + sizeof (""));
  if (Buffer == NULL) {
    return NULL;
  }

  //
  // Copy and sanitize characters.
  //
  for (Index = 0; Index < Length; Index++) {
    if (IsSpace (SourceString[Index])) {
      Buffer[Index] = ' ';
      continue;
    }
    if (SourceString[Index] <= 0x1F || SourceString[Index] >= 0x7F) {
      //
      // For now only ASCII strings is allowed in device attributes.
      // (InsydeH2O Chipset Technical Reference section "Device Information")
      // Replace bytes values 0x80-0xFF until an extension encoding is decided
      // in the future (ISO 8859-1, Windows-1252 or UTF-8).
      // If we feed the bytes 0x80-0xFF unsanitized to PrintLib "%a" format,
      // the values would be interpreted as ISO 8859-1 code points. Better not
      // let OEMs expect that behavior.
      //
      Buffer[Index] = '?';
      continue;
    }
    Buffer[Index] = SourceString[Index];
  }
  Buffer[Index] = '\0';
  return Buffer;
}

STATIC
VOID
UpdateDisplayNameIndices (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT16                                   NewNameIndex
  )
{
  UINTN                              Index;
  H2O_DEVICE_INFO_LOOKUP_INFO_ENTRY  *Entry;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  ASSERT (PrivateData->IsBeingWritten);

  for (Index = 0; Index < PrivateData->InfoEntryCount; Index++) {
    if (PrivateData->InfoEntries == NULL) {
      break;
    }
    Entry = &PrivateData->InfoEntries[Index];
    if (Entry->DisplayNameIndex >= NewNameIndex &&
        Entry->DisplayNameIndex < MAX_UINT16)
    {
      Entry->DisplayNameIndex++;
      ASSERT (Entry->DisplayNameIndex < MAX_UINT16);
    }
  }

#ifdef H2O_DEVICE_INFO_USB_LOOKUP_INTERFACE_SUPPORT
  UpdateUsbPortInfoDisplayNameIndices (PrivateData, NewNameIndex);
#endif
}

/**

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
  @retval EFI_ALREADY_STARTED
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
InsertDisplayNameString (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN CONST CHAR16                             *Name,
  IN CONST CHAR16                             *NameSortKey OPTIONAL,
  OUT UINTN                                   *DisplayNameIndex
  )
{
  EFI_STATUS  Status;
  BOOLEAN     CanAddDisplayName;
  CHAR16      *NameStringBuffer;
  INTN        CompareResult;
  UINTN       Index;
  CHAR16      *SortKeyBuffer;
  UINTN       MemoryMoveSize;

  if ((PrivateData == NULL) || (Name == NULL) || (DisplayNameIndex == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/Name/DisplayNameIndex is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *DisplayNameIndex = MAX_UINT16;
  Status = EFI_SUCCESS;

  CanAddDisplayName = PrivateData->DisplayNameStringCount <= 0 ||
                        PrivateData->DisplayNameSortKeys != NULL;
  if (!CanAddDisplayName) {
    Status = EFI_UNSUPPORTED;
    goto End;
  }

  //
  // Create a name string.
  //
  NameStringBuffer = CatSPrint (NULL, L"%s", Name);
  if (NameStringBuffer == NULL) {
    goto NameStringBufferFail;
  }
  //
  // If sort key is unspecified, use the name string as the key.
  //
  if (NameSortKey == NULL) {
    NameSortKey = NameStringBuffer;
  }

  //
  // Find insert position of the name string.
  //
  CompareResult = 1;
  for (Index = PrivateData->DisplayNameStringCount; Index > 0; Index--) {
    if ((PrivateData->DisplayNameSortKeys == NULL) || (PrivateData->DisplayNameStrings == NULL)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a() - The PrivateData->DisplayNameSortKeys/DisplayNameStrings is NULL, please check input Param\n",
        __FUNCTION__
        ));
      Status = EFI_INVALID_PARAMETER;
      goto GrowBufferFail;
    }
    CompareResult = StrCmp (
                      NameSortKey,
                      PrivateData->DisplayNameSortKeys[Index - 1]
                      );
    if (CompareResult == 0) {
      CompareResult = StrCmp (
                        NameStringBuffer,
                        PrivateData->DisplayNameStrings[Index - 1]
                        );
    }
    if (CompareResult >= 0) {
      break;
    }
  }
  if (CompareResult == 0) {
    *DisplayNameIndex = Index - 1;
    FreePool (NameStringBuffer);
    Status = EFI_ALREADY_STARTED;
    goto End;
  }

  ASSERT (CompareResult > 0 || Index == 0);
  *DisplayNameIndex = Index;

  //
  // Grow DisplayNameSortKeys and DisplayNameStrings list buffers.
  //
  if (PrivateData->DisplayNameStringCount >= MAX_UINT16) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: %a(): can't store more than %u names.\n",
      gEfiCallerBaseName,
      __FUNCTION__,
      (unsigned int) MAX_UINT16
      ));
    goto GrowBufferFail;
  }
  Status = GrowBufferIfFull (
             (VOID **) &PrivateData->DisplayNameSortKeys,
             PrivateData->DisplayNameStringCount,
             1,
             sizeof (*PrivateData->DisplayNameSortKeys)
             );
  if (EFI_ERROR (Status)) {
    ASSERT (Status == EFI_OUT_OF_RESOURCES);
    goto GrowBufferFail;
  }
  Status = GrowBufferIfFull (
             (VOID **) &PrivateData->DisplayNameStrings,
             PrivateData->DisplayNameStringCount,
             1,
             sizeof (*PrivateData->DisplayNameStrings)
             );
  if (EFI_ERROR (Status)) {
    ASSERT (Status == EFI_OUT_OF_RESOURCES);
    goto GrowBufferFail;
  }

  //
  // Duplicate the sort key string.
  //
  SortKeyBuffer = CatSPrint (NULL, L"%s", NameSortKey);
  if (SortKeyBuffer == NULL) {
    goto SortKeyBufferFail;
  }

  //
  // Insert new name string.
  //
  ASSERT (Index <= PrivateData->DisplayNameStringCount);
  ASSERT (sizeof (*PrivateData->DisplayNameSortKeys) == sizeof (*PrivateData->DisplayNameStrings));
  MemoryMoveSize = (PrivateData->DisplayNameStringCount - Index) *
                     sizeof (*PrivateData->DisplayNameSortKeys);
  CopyMem (
    &PrivateData->DisplayNameSortKeys[Index + 1],
    &PrivateData->DisplayNameSortKeys[Index],
    MemoryMoveSize
    );
  CopyMem (
    &PrivateData->DisplayNameStrings[Index + 1],
    &PrivateData->DisplayNameStrings[Index],
    MemoryMoveSize
    );
  PrivateData->DisplayNameSortKeys[Index] = SortKeyBuffer;
  PrivateData->DisplayNameStrings[Index]  = NameStringBuffer;
  PrivateData->DisplayNameStringCount++;

  UpdateDisplayNameIndices (PrivateData, (UINT16) Index);

  //
  // Done.
  //
  DEBUG ((
    EFI_D_VERBOSE,
    "%a: %a(): inserted to index %lu (Name=\"%s\", SortKey=\"%s\").\n",
    gEfiCallerBaseName,
    __FUNCTION__,
    (UINT64) Index,
    NameStringBuffer,
    SortKeyBuffer
    ));
  goto End;

SortKeyBufferFail:
GrowBufferFail:
  FreePool (NameStringBuffer);
NameStringBufferFail:
  Status = EFI_OUT_OF_RESOURCES;

End:
  DEBUG ((
    EFI_D_VERBOSE,
    "%a: %a() status: %r\n",
    gEfiCallerBaseName,
    __FUNCTION__,
    Status
    ));
  return Status;
}
