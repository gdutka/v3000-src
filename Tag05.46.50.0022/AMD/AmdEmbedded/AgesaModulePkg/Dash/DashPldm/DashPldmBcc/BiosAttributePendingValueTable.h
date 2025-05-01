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

#define MAX_BIOS_PASSWORD_LENGTH 64
#define MIN_BIOS_PASSWORD_LENGTH  3
#define DEFAULT_BIOS_PASSWORD_LENGTH 32

// The "BIOSAttributeValueTableData" Attribute Type.
// Ref. DSP0247, version 1.0.0, Table 23 - PLDM Representation of BIOSAttributePendingValueTableData :: AttributeType[0]
typedef enum {
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSEnumeration               = 0x00,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSString                    = 0x01,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSPassword                  = 0x02,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSInteger                   = 0x03,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSBootConfigSetting         = 0x04,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSCollection                = 0x05,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSConfigSet                 = 0x06,
  PLDM_ATTRIBUTE_PENDING_VALUE_TYPE_BIOSCollectionReadOnly        = 0x85
} PLDM_ATTRIBUTE_PENDING_VALUE_TYPE;

typedef enum {
  PLDM_PENDING_VALUE_STATUS_REJECTED = -1,
  PLDM_PENDING_VALUE_STATUS_NIL = 0,
  PLDM_PENDING_VALUE_STATUS_ACCEPTED = 1,
} PLDM_PENDING_VALUE_STATUS;

#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Unknown           0x00
#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Default           0x01
#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Next              0x02
#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_DefaultAndNext    0x03
#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_Onetime           0x04
#define BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_TYPE_DefaultAndOnetime 0x05

#define BIOS_ATTRIBUTE_PENDING_VALUE_ORDERANDFAILTHROUGHMODE_UnorderedAndLimitedFailThrough 0x00
#define BIOS_ATTRIBUTE_PENDING_VALUE_ORDERANDFAILTHROUGHMODE_UnorderedAndFailThrough        0x01
#define BIOS_ATTRIBUTE_PENDING_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndLimitedFailThrough   0x02
#define BIOS_ATTRIBUTE_PENDING_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough          0x03

#pragma pack(push, 1)

// A "BIOSStringTableData" entry.
// Ref. DSP0247, version 1.0.0, Table 23 - PLDM Representation of BIOSAttributePendingValueTableData
typedef struct {
  UINT16 BiosAttributeHandle;
  UINT8  BiosAttributeType;
  //CHAR8  BiosAttributeTypeSpecific[0];
} BIOS_ATTRIBUTE_PENDING_VALUE_TABLE_ENTRY;


// Ref. DSP0247, version 1.0.0, Table 28 - Specific BIOS Attribute Pending Value Table Fields for the BIOSBootConfigSetting and Type
typedef struct {
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT PgbtrfHeader;
  BIOS_ATTRIBUTE_PENDING_VALUE_TABLE_ENTRY BapvteHeader;
  UINT8  BootConfigType;
  UINT8  OrderAndFailThroughMode;
  UINT8  NumberOfPendingBootSourceSettings;
  UINT8  BootSourceStringHandleIndex[0xFF]; // 0xFF: the maximum value of (NumberOfPendingBootSourceSettings-1)
} BIOS_ATTRIBUTE_PENDING_VALUE_BOOT_CONFIG_SETTING;

// Ref. DSP0247, version 1.0.0, Table 26 - Specific BIOS Attribute Pending Value Table Fields for the BIOSPassword
typedef struct {
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT PgbtrfHeader;
  BIOS_ATTRIBUTE_PENDING_VALUE_TABLE_ENTRY BapvteHeader;
  UINT16  PendingPasswordLength;
  CHAR8  *PendingPassword;
} BIOS_ATTRIBUTE_PENDING_VALUE_BIOS_PASSWORD_SETTING;

typedef struct {
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT PgbtrfHeader;
  BIOS_ATTRIBUTE_PENDING_VALUE_TABLE_ENTRY BapvteHeader;
} BIOS_ATTRIBUTE_PENDING_VALUE_TABLE;

#pragma pack(pop)



/**
  Update the EFI variable, L"BootOrder" based on the remote "BIOS Attribute Pending Value Table" data.

  @param [in] mPldmProtocol     Pointer to the instance of "AMD PLDM PROTOCOL"
  @param [in] Updated           TRUE - Boot options have been updated successfully. FLASE - No action.

  @retval EFI_SUCCESS           Successful
  @retval EFI_INVALID_PARAMETER Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES  Unable to allocate memory.
  @retval EFI_DEVICE_ERROR      Corrupted data from the remote device.

**/
EFI_STATUS
EFIAPI
UpdatePendingBootConfigSetting (
  IN AMD_DASH_PLDM_PROTOCOL *mPldmProtocol,
  IN OUT BOOLEAN *Updated
);

