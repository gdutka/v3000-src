/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmPldmBcc.h"
#include "BiosStringTable.h"

#define FILECODE MPM_MPMPLDM_MPMPLDMBCC_BIOSSTRINGTABLE_FILECODE
/**
  Append a record of a specified ASCII string to the BIOS String Table.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.
  @param [in] BiosStringHandle          BIOS String Handle
  @param [in] BiosString                BIOS String

  @retval EFI_SUCCESS                   Successful
  @retval EFI_INVALID_PARAMETER         Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate memory.

**/
EFI_STATUS
EFIAPI
AppendBiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER   *BiosStringTableBuffer,
  IN UINT16 BiosStringHandle,
  IN CHAR8  *BiosString
  )
{
  UINT16    BiosStringLength;
  UINT16    NewDataLen;
  BIOS_STRING_TABLE_ENTRY   *BiosStringTableEntriesTail;

  if ((BiosStringTableBuffer == NULL) || (BiosString == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Allocate/re-allocate a new/larger BIOS-String-Table buffer when it is NIL/full.
  BiosStringLength = (UINT16) AsciiStrLen (BiosString);
  NewDataLen = sizeof(BIOS_STRING_TABLE_BUFFER) + BiosStringLength;
  NewDataLen = PaddingCount (NewDataLen, 4) + 4; // CRC32 takes the final additional 4 bytes.
  if ((BiosStringTableBuffer->BiosStringTableEntries == NULL) ||
      (BiosStringTableBuffer->BufferSize < (BiosStringTableBuffer->AppendedDataSize + NewDataLen)))
  {
    // The "BIOS String Table" buffer is implicitly:
    //   (1) zero-ed
    //   (2) cloned from the original copy if it exists.
    BiosStringTableBuffer->BiosStringTableEntries =
      (BIOS_STRING_TABLE_ENTRY*) ReallocatePool (
                                   BiosStringTableBuffer->BufferSize,
                                   BiosStringTableBuffer->BufferSize+GRANULE_BIOS_TABLE_BUFFER_SIZE,
                                   BiosStringTableBuffer->BiosStringTableEntries
                                   );
    if (BiosStringTableBuffer->BiosStringTableEntries == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    BiosStringTableBuffer->BufferSize += GRANULE_BIOS_TABLE_BUFFER_SIZE;
  }

  // Setup and append the specified "BIOS String" to "BIOSStringTableData".
  BiosStringTableEntriesTail = (BIOS_STRING_TABLE_ENTRY*) \
    ((UINT8*)(BiosStringTableBuffer->BiosStringTableEntries) + \
    BiosStringTableBuffer->AppendedDataSize);
  BiosStringTableEntriesTail->BiosStringHandle = BiosStringHandle;
  BiosStringTableEntriesTail->BiosStringLength = BiosStringLength;
  CopyMem (BiosStringTableEntriesTail->BiosString,BiosString, BiosStringLength); // no trailing '\0'
  BiosStringTableBuffer->AppendedDataSize += sizeof(BIOS_STRING_TABLE_ENTRY) + BiosStringLength;

  return EFI_SUCCESS;
}

/**
  Calculate and add CRC32 to the end of BIOS String Table.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.

**/
EFI_STATUS
EFIAPI
Crc32BiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
  )
{
  UINT32 *BiosStringTableEntriesTail;

  // Round off the 4-byte aligned buffer with 0 or 1 or 2 or 3-byte padding.
  BiosStringTableBuffer->AppendedDataSize += PaddingCount (
                                               BiosStringTableBuffer->AppendedDataSize, 4
                                               );

  BiosStringTableEntriesTail = (UINT32*) \
    ((UINT8*)(BiosStringTableBuffer->BiosStringTableEntries) + \
    BiosStringTableBuffer->AppendedDataSize);
  *BiosStringTableEntriesTail = CalculateCrc32 (
                                  BiosStringTableBuffer->BiosStringTableEntries,
                                  BiosStringTableBuffer->AppendedDataSize
                                  );
  BiosStringTableBuffer->AppendedDataSize += sizeof (UINT32);
  return EFI_SUCCESS;
}

GENERIC_BIOS_STRING gGenericBiosStrings[] = {
  PLDM_BIOS_STRING_HANDLE_Disabled,         "Disabled",
  PLDM_BIOS_STRING_HANDLE_Enabled,          "Enabled",
  PLDM_BIOS_STRING_HANDLE_On,               "On",
  PLDM_BIOS_STRING_HANDLE_Off,              "Off",
  PLDM_BIOS_STRING_HANDLE_NumLock_LED,      "NumLock LED",
  PLDM_BIOS_STRING_HANDLE_USB_Emulation,    "USB Emulation",
  PLDM_BIOS_STRING_HANDLE_BIOS_ADMIN_PASSWORD, "BIOS Admin Password",
  PLDM_BIOS_STRING_HANDLE_BIOS_USER_PASSWORD, "BIOS User Password",
  0, NULL
};

/**
  Set up the generic BIOS String Table.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.
  @param [in] GenericBiosStrings        Pointer to a structured table with uint16-char8* entries.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful
**/
EFI_STATUS
EFIAPI
SetupGenericBiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer,
  IN GENERIC_BIOS_STRING *GenericBiosStrings
  )
{
  EFI_STATUS Status;

  if ((BiosStringTableBuffer == NULL) || (GenericBiosStrings == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  for (; GenericBiosStrings->StringLiteral != NULL; GenericBiosStrings++) {
    Status = AppendBiosStringTable (
               BiosStringTableBuffer,
               GenericBiosStrings->StringHandle,
               GenericBiosStrings->StringLiteral
               );
    if (EFI_ERROR (Status)) {
      break;
    }
  }
  return Status;
}

/**
  Set up the BIOS String Tables.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful
**/
EFI_STATUS
EFIAPI
SetupBiosStringTables (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT16        *BootOrderList = NULL;  // "Boot####"
  UINTN         BootOrderListSize;
  CHAR16        BootString[9];          // 9 : (sizeof(L"Boot####")/sizeof(CHAR16))
  VOID          *OptionFromVar = NULL;
  UINTN         BootOptionSize = 0;
  CHAR16        *DescriptionW;
  UINTN         DescriptionLen;
  CHAR8         *DescriptionA = NULL;
  UINTN         index;

  Status = SetupGenericBiosStringTable (BiosStringTableBuffer, gGenericBiosStrings);
  if (EFI_ERROR (Status)) goto Exit;

  Status = AppendBiosStringTable (
               BiosStringTableBuffer,
               PLDM_BIOS_STRING_HANDLE_BOOT_CONFIG_SETTING,
               "BIOS Boot Config Setting"
               );
  if (EFI_ERROR (Status)) goto Exit;

  // Get Boot Order List.
  Status = GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrderList, &BootOrderListSize);
  if (EFI_ERROR (Status) || (BootOrderList == NULL) || (BootOrderListSize == 0)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  // Collect the description of every "Boot####" entry.
  for (index=0;  index < (BootOrderListSize / sizeof(UINT16)); index++) {
    OptionFromVar = NULL;
    BootOptionSize = 0;
    UnicodeSPrint (BootString, sizeof(BootString), L"Boot%04X", BootOrderList[index]);
    if ((GetEfiGlobalVariable2 (BootString, &OptionFromVar, &BootOptionSize) != EFI_SUCCESS) ||
        (OptionFromVar == NULL) ||
        (BootOptionSize == 0))
    {
      // This is not likely to happen when a "Boot####" is enlisted in Boot-Order-List.
      goto Next;
    }

    //TODO: skip the hidden boot option?

    DescriptionW = (CHAR16*)(((UINT8*)OptionFromVar) + sizeof(EFI_LOAD_OPTION));
    DescriptionLen = StrLen (DescriptionW);
    DescriptionA = (CHAR8*) AllocatePool (DescriptionLen+1);
    if (DescriptionA == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    // Warning: This ASCII conversion may break a boot-option's description when it is physically
    //   using UTF-16 encoding for non-Western characters.
    UnicodeStrToAsciiStrS (DescriptionW, DescriptionA, DescriptionLen+1);
    Status = AppendBiosStringTable (
               BiosStringTableBuffer,
               PLDM_BIOS_STRING_HANDLE_EFI_BOOT_OPTION + BootOrderList[index],
               DescriptionA
               );
    if (EFI_ERROR (Status)) goto Exit;

  Next:
    if (DescriptionA != NULL) {
      FreePool (DescriptionA);
      DescriptionA = NULL;
    }
    if (OptionFromVar != NULL) {
      FreePool (OptionFromVar);
      OptionFromVar = NULL;
    }
  }

Exit:
  if (DescriptionA != NULL) {
    FreePool (DescriptionA);
    DescriptionA = NULL;
  }
  if (OptionFromVar != NULL) {
    FreePool (OptionFromVar);
    OptionFromVar = NULL;
  }
  if (BootOrderList != NULL) {
    FreePool (BootOrderList);
    BootOrderList = NULL;
  }
  return Status;
}

/**
  Get BIOS String Table Data's address and size.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
EFIAPI
GetBiosStringTableBuffer (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  )
{
  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Buffer = BiosStringTableBuffer->BiosStringTableEntries;
  *BufferSize = BiosStringTableBuffer->AppendedDataSize;
  if ((*Buffer == NULL) || (*BufferSize == 0)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Free any allocated buffer for BIOS String Table Data.

  @param [in] BiosStringTableBuffer     Pointer to BIOS String Table buffer.

**/
VOID
EFIAPI
CleanUpBiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
  )
{
  BiosStringTableBuffer->BufferSize = 0;
  BiosStringTableBuffer->AppendedDataSize = 0;
  if (BiosStringTableBuffer->BiosStringTableEntries != NULL) {
    FreePool (BiosStringTableBuffer->BiosStringTableEntries);
    BiosStringTableBuffer->BiosStringTableEntries = NULL;
  }
}
