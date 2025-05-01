/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#pragma once

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/MpmLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdPldmProtocol.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#pragma pack(push, 1)

typedef struct {
  UINT16 StringHandle;
  CHAR8 *StringLiteral;
} GENERIC_BIOS_STRING;

// A "BIOSStringTableData" entry.
// Ref. DSP0247, version 1.0.0, Table 2 - PLDM Representation of BIOSStringTableData
typedef struct {
  UINT16 BiosStringHandle;
  UINT16 BiosStringLength;
  CHAR8 BiosString[0];
} BIOS_STRING_TABLE_ENTRY;

// A book-keeper's space to cope with the "BIOSStringTableData" buffer.
typedef struct {
  UINT16 BufferSize;
  UINT16 AppendedDataSize;
  BIOS_STRING_TABLE_ENTRY *BiosStringTableEntries;
} BIOS_STRING_TABLE_BUFFER;

#pragma pack(pop)

EFI_STATUS
EFIAPI
Crc32BiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
);

EFI_STATUS
EFIAPI
SetupBiosStringTables (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
  );

EFI_STATUS
EFIAPI
GetBiosStringTableBuffer (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  );

VOID
EFIAPI
CleanUpBiosStringTable (
  IN BIOS_STRING_TABLE_BUFFER *BiosStringTableBuffer
);
