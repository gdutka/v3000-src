/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#pragma once

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdDashPldmProtocol.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

// The "BIOSAttributeValueTableData" Attribute Type.
// Ref. DSP0247, version 1.0.0, Table 14 - PLDM Representation of BIOSAttributeValueTableData :: AttributeType[0]
typedef enum {
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSEnumeration               = 0x00,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSString                    = 0x01,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPassword                  = 0x02,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSInteger                   = 0x03,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSBootConfigSetting         = 0x04,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSCollection                = 0x05,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSConfigSet                 = 0x06,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSEnumerationReadOnly       = 0x80,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSStringReadOnly            = 0x81,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPasswordReadOnly          = 0x82,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSIntegerReadOnly           = 0x83,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSBootConfigSettingReadOnly = 0x84,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSCollectionReadOnly        = 0x85,
  PLDM_ATTRIBUTE_VALUE_TYPE_BIOSConfigSetReadOnly         = 0x86
} PLDM_ATTRIBUTE_VALUE_TYPE;

#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_Unknown               0x00
#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_Default               0x01
#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_Next                  0x02
#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_DefaultAndNext        0x03
#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_Onetime               0x04
#define BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_DefaultAndOnetime     0x05

#define BIOS_ATTRIBUTE_VALUE_ORDERANDFAILTHROUGHMODE_UnorderedAndLimitedFailThrough     0x00
#define BIOS_ATTRIBUTE_VALUE_ORDERANDFAILTHROUGHMODE_UnorderedAndFailThrough            0x01
#define BIOS_ATTRIBUTE_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndLimitedFailThrough       0x02
#define BIOS_ATTRIBUTE_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough              0x03

#pragma pack(push, 1)

// A "BIOSStringTableData" entry.
// Ref. DSP0247, version 1.0.0, Table 14 - PLDM Representation of BIOSAttributeValueTableData
typedef struct {
  UINT16 BiosAttributeHandle;
  UINT8  BiosAttributeType;
  CHAR8  BiosAttributeTypeSpecific[0];
} BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY;

// A book-keeper's space to cope with the "BIOSAttributeValueTableData" buffer.
typedef struct {
  UINT16 BufferSize;
  UINT16 AppendedDataSize;
  BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY *BiosAttributeValueTableEntries;
} BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER;

#pragma pack(pop)

EFI_STATUS
EFIAPI
SetupBiosAttributeValueTables (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer
);

EFI_STATUS
EFIAPI
Crc32BiosAttributeValueTable (
    IN VOID   *ExpandableBuffer
);

EFI_STATUS
EFIAPI
GetBiosAttributeValueTableBuffer (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
);

VOID
EFIAPI
CleanUpBiosAttributeValueTable (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer
);
