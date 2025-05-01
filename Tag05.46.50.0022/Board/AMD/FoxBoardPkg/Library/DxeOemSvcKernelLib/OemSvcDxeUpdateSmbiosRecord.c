/** @file
  Provide OEM to modifying each MISC BIOS record.

;******************************************************************************
;* Copyright (c) 2013 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/BaseLib.h>
#include <Library/DxeOemSvcKernelLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/SmBios.h>


STATIC
VOID
EFIAPI
GetStringFieldSize (
  IN  CHAR8                                *StringField,
  OUT UINTN                                *Size,
  OUT UINTN                                *Number
  )
{
  CHAR8                                   *StrPtr;

  StrPtr  = StringField;
  *Size   = 0;
  *Number = 0;


  do {
    // Skip the string
    while (*StrPtr != '\0') {
      StrPtr++;
    }
    *Number += 1;
    // Point after the null
    StrPtr++;
    // Is it a second NULL, we're done.
    if (*StrPtr == '\0') {
      break;
    }
  } while (TRUE);

  *Size = (StrPtr - StringField) + 1;

  return ;
}

STATIC
EFI_STATUS
EFIAPI
UpdateBiosInfo (
  IN OUT EFI_SMBIOS_TABLE_HEADER          *RecordBuffer
  )
{
  CHAR8                                   *StrFldPtr;
  CHAR8                                   *StrFldCopyPtrHead;
  CHAR8                                   *StrFldCopyPtr;
  UINTN                                   StrFldSize;
  UINTN                                   StrNum;
  UINTN                                   Index;
  UINTN                                   OrgStrSize, NewStrSize;
  UINTN                                   RemainStrFldSize;
  EFI_STATUS                              Status;

  Status = EFI_MEDIA_CHANGED;

  StrFldPtr = (CHAR8*) RecordBuffer + RecordBuffer->Length;
  GetStringFieldSize (StrFldPtr, &StrFldSize, &StrNum);

  StrFldCopyPtrHead = AllocateCopyPool (StrFldSize, StrFldPtr);
  if (StrFldCopyPtrHead == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StrFldCopyPtr = StrFldCopyPtrHead;
  ZeroMem (StrFldPtr, StrFldSize);

  RemainStrFldSize = SMBIOS_TABLE_MAX_LENGTH - RecordBuffer->Length;

  for (Index = 0; Index < StrNum; Index++) {

    OrgStrSize = AsciiStrnSizeS (StrFldCopyPtr, PcdGet32 (PcdMaximumAsciiStringLength));

    switch (Index) {
      case 1:
        //
        // Firmware version
        //
        Status = UnicodeStrToAsciiStrS ((CHAR16*) PcdGetPtr (PcdFirmwareVersionString), StrFldPtr, RemainStrFldSize);
        NewStrSize = AsciiStrnSizeS (StrFldPtr, PcdGet32 (PcdMaximumAsciiStringLength));
        break;
      case 2:
        //
        // Firmware release date.
        //
        Status = UnicodeStrToAsciiStrS ((CHAR16*) PcdGetPtr (PcdFirmwareReleaseDateString), StrFldPtr, RemainStrFldSize);
        NewStrSize = AsciiStrnSizeS (StrFldPtr, PcdGet32 (PcdMaximumAsciiStringLength));
        break;
      default:
        Status = AsciiStrCpyS (StrFldPtr, RemainStrFldSize, StrFldCopyPtr);
        NewStrSize = OrgStrSize;
        break;
    }

    if (EFI_ERROR (Status)) {
      break;
    }

    StrFldCopyPtr += OrgStrSize;
    StrFldPtr += NewStrSize;
    RemainStrFldSize -= NewStrSize;
  }

  if (EFI_ERROR (Status)) {
    //
    // Update SMBIOS string failed. Restore original string field.
    //
    StrFldPtr = (CHAR8*) RecordBuffer + RecordBuffer->Length;
    ZeroMem (StrFldPtr, SMBIOS_TABLE_MAX_LENGTH - RecordBuffer->Length);
    CopyMem (StrFldPtr, StrFldCopyPtrHead, StrFldSize);
  }

  FreePool (StrFldCopyPtrHead);
  return Status;
}

/**
  This service will be call by each time add MISC SMBIOS record.
  OEM can modifying MISC BIOS in run time.

  @param[in, out]  *RecordBuffer          Each MISC SMBIOS record data.

  @retval      EFI_UNSUPPORTED            Returns unsupported by default.
  @retval      EFI_SUCCESS                Updatable SMBIOS success.
  @retval      EFI_MEDIA_CHANGED          The value of IN OUT parameter is changed.
**/
EFI_STATUS
OemSvcDxeUpdateSmbiosRecord (
  IN OUT EFI_SMBIOS_TABLE_HEADER          *RecordBuffer
  )
{
  EFI_STATUS                              Status;

  Status = EFI_UNSUPPORTED;

  switch (RecordBuffer->Type) {
    case EFI_SMBIOS_TYPE_BIOS_INFORMATION:
      // Status = UpdateBiosInfo (RecordBuffer);
      break;
    default:
      break;
  }

  return Status;
}
