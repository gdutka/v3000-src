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

// The "BIOSAttributeTableData" Attribute Type.
// Ref. DSP0247, version 1.0.0, Table 5 - PLDM Representation of BIOSAttributeTableData :: AttributeType[0]
typedef enum {
  PLDM_ATTRIBUTE_TYPE_BIOSEnumeration               = 0x00,
  PLDM_ATTRIBUTE_TYPE_BIOSString                    = 0x01,
  PLDM_ATTRIBUTE_TYPE_BIOSPassword                  = 0x02,
  PLDM_ATTRIBUTE_TYPE_BIOSInteger                   = 0x03,
  PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSetting         = 0x04,
  PLDM_ATTRIBUTE_TYPE_BIOSCollection                = 0x05,
  PLDM_ATTRIBUTE_TYPE_BIOSConfigSet                 = 0x06,
  PLDM_ATTRIBUTE_TYPE_BIOSEnumerationReadOnly       = 0x80,
  PLDM_ATTRIBUTE_TYPE_BIOSStringReadOnly            = 0x81,
  PLDM_ATTRIBUTE_TYPE_BIOSPasswordReadOnly          = 0x82,
  PLDM_ATTRIBUTE_TYPE_BIOSIntegerReadOnly           = 0x83,
  PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSettingReadOnly = 0x84,
  PLDM_ATTRIBUTE_TYPE_BIOSCollectionReadOnly        = 0x85,
  PLDM_ATTRIBUTE_TYPE_BIOSConfigSetReadOnly         = 0x86
} PLDM_ATTRIBUTE_TYPE;

// DSP0247 page20: PasswordEncodingType
typedef enum {
  PASSWORD_ENCODING_TYPE_ASCII                     = 0x00,
  PASSWORD_ENCODING_TYPE_KBD                       = 0x01,
  PASSWORD_ENCODING_TYPE_PIN                       = 0x02,
  PASSWORD_ENCODING_TYPE_UTF8                      = 0x03,
  PASSWORD_ENCODING_TYPE_UTF16LE                   = 0x04,
  PASSWORD_ENCODING_TYPE_UEF16BE                   = 0x05,
  PASSWORD_ENCODING_TYPE_VENDOR_SPECIFIC           = 0x06
} PASSWORD_ENCODING_TYPE;

#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_Unknown               0x00
#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_Default               0x01
#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_Next                  0x02
#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_DefaultAndNext        0x03
#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_Onetime               0x04
#define BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_DefaultAndOnetime     0x05

#define BIOS_ATTRIBUTE_ORDERANDFAILTHROUGHMODE_UnorderedAndLimitedFailThrough     0x00
#define BIOS_ATTRIBUTE_ORDERANDFAILTHROUGHMODE_UnorderedAndFailThrough            0x01
#define BIOS_ATTRIBUTE_ORDERANDFAILTHROUGHMODE_OrderedAndLimitedFailThrough       0x02
#define BIOS_ATTRIBUTE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough              0x03

#pragma pack(push, 1)

// A "BIOSStringTableData" entry.
// Ref. DSP0247, version 1.0.0, Table 5 - PLDM Representation of BIOSAttributeTableData
typedef struct {
  UINT16 BiosAttributeHandle;
  UINT8  BiosAttributeType;
  UINT16 BiosAttributeNameHandle;
  CHAR8 BiosAttributeTypeSpecific[0];
} BIOS_ATTRIBUTE_TABLE_ENTRY;

// A book-keeper's space to cope with the "BIOSAttributeTableData" buffer.
typedef struct {
  UINT16 BufferSize;
  UINT16 AppendedDataSize;
  BIOS_ATTRIBUTE_TABLE_ENTRY *BiosAttributeTableEntries;
} BIOS_ATTRIBUTE_TABLE_BUFFER;

// Ref. DSP0247, version 1.0.0, Table 10 - Specific BIOS Attribute Table Fields for BIOSBootConfigSetting and BIOSBootConfigSettingReadOnly Types
typedef struct {
  PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT PgbtrfHeader;
  UINT8 BootConfigType;
  UINT8 SupportedOrderedAndFailThroughModes;
  UINT8 MinimumNumberOfBootSourceSettings;
  UINT8 MaximumNumberOfBootSourceSettings;
  UINT8 NumberOfPossibleBootSourceSettings;
  UINT8 PossibleBootSourceStringHandle[0];
} BIOS_ATTRIBUTE_TABLE_BOOT_CONFIG_SETTING;

#pragma pack(pop)

EFI_STATUS
EFIAPI
SetupBiosAttributeTables (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer
);

EFI_STATUS
EFIAPI
Crc32BiosAttributeTable (
    IN VOID   *ExpandableBuffer
);

EFI_STATUS
EFIAPI
GetBiosAttributeTableBuffer (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
);

VOID
EFIAPI
CleanUpBiosAttributeTable (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer
);
