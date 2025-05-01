/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ASF_TABLE_ASL_H_
#define _ASF_TABLE_ASL_H_

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

#include <Protocol/AcpiTable.h>

#include <IndustryStandard/Acpi.h>

#define ASF_TABLE_DESCRIPTION_SIGNATURE       SIGNATURE_32('A', 'S', 'F', '!')
#define ASF_TABLE_DESCRIPTION_REVISION        0x20
#define ASF_ALRT_DATA_LENGTH                  12
#define ASF_RCTL_DATA_LENGTH                  16
#define ASF_ADDR_FIXED_SMBUS_ADDRESSES_LENGTH 8

#pragma pack (1)
//
// ACPI_ASF_RECORD_HEADER
//
typedef struct {
  UINT8                     Type;
  UINT8                     Reserved;
  UINT16                    RecordLength;
} ACPI_ASF_RECORD_HEADER;

//
// ACPI_TABLE_ASF_INFO
//
typedef struct {
  ACPI_ASF_RECORD_HEADER    RecordHeader;
  UINT8                     MinimumWatchDogResetValue;
  UINT8                     MinimumPollingInterval;
  UINT16                    SystemID;
  UINT32                    IANAManufactureID;
  UINT8                     FeatureFlags;
  UINT8                     Reserved[3];
} ACPI_TABLE_ASF_INFO;

//
// ACPI_TABLE_ASF_ALRT
//
typedef struct {
  ACPI_ASF_RECORD_HEADER    RecordHeader;
  UINT8                     AssertionEventBitMask;
  UINT8                     DeassertionEventBitMask;
  UINT8                     NumberOfAlerts;
  UINT8                     ArrayElementLength;
  UINT8                     AsfAlertData[ASF_ALRT_DATA_LENGTH];
} ACPI_TABLE_ASF_ALRT;

//
// ACPI_TABLE_ASF_RCTL
//
typedef struct {
  ACPI_ASF_RECORD_HEADER    RecordHeader;
  UINT8                     NumberOfControls;
  UINT8                     ArrayElementLength;
  UINT16                    Reserved;
  UINT8                     AsfControlData[ASF_RCTL_DATA_LENGTH];
} ACPI_TABLE_ASF_RCTL;

//
// ACPI_TABLE_ASF_RMCP
//
typedef struct {
  ACPI_ASF_RECORD_HEADER    RecordHeader;
  UINT8                     RemoteControlCapabilities[7];
  UINT8                     BootOptionsCompletionCode;
  UINT32                    IANAEnterpriseID;
  UINT8                     SpecialCommand;
  UINT8                     SpecialCommandParameter[2];
  UINT8                     BootOptions[2];
  UINT8                     OEMParameters[2];
} ACPI_TABLE_ASF_RMCP;

//
// ACPI_TABLE_ASF_ADDR
//
typedef struct {
  ACPI_ASF_RECORD_HEADER    RecordHeader;
  UINT8                     SEEPROMAddress;
  UINT8                     NumberOfDevices;
  UINT8                     FixedSmbusAddresses[ASF_ADDR_FIXED_SMBUS_ADDRESSES_LENGTH];
} ACPI_TABLE_ASF_ADDR;

//
// ASF Table Description
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER   Header;
  ACPI_TABLE_ASF_INFO           AsfInfo;
  ACPI_TABLE_ASF_ALRT           AsfAlert;
  ACPI_TABLE_ASF_RCTL           AsfRctl;
  ACPI_TABLE_ASF_RMCP           AsfRmcp;
  ACPI_TABLE_ASF_ADDR           AsfAddr;
} ASF_TABLE_DESCRIPTION;
#pragma pack ()
#endif
