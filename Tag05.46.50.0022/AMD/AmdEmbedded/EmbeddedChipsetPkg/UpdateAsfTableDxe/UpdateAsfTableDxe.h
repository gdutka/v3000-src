/** @file
  The header file for Update ASF table DXE module.
  
;*******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/
#ifndef _UPDATE_ASF_TABLE_DXE_H_
#define _UPDATE_ASF_TABLE_DXE_H_

#include <Uefi.h>
#include <PiDxe.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSupport.h>
#include <Protocol/DashIoCfg.h>
//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

#define IBV_ID          L"INSYDE"
#define IBV_TABLE_ID    L"EDK2"
#define OEM_REVISION    0x1

//
// We use different composition of EFI_ACPI_ASF_ALRT, EFI_ACPI_ASF_RCTL and EFI_ACPI_ASF_ADDR structures.
// The standard version remove DeviceArray[], ControlArray[] and FixedSmbusAddresses[] but ours keep
// these attributes.
//
// #include <IndustryStandard/AlertStandardFormatTable.h>

//
// ASF Definitions
//
#define EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE_SIGNATURE  0x21465341
#define EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE_REVISION   0x20
#define ASF_ADDR_DEVICE_ARRAY_LENGTH                  16

#define ASF_REMOTE_CTRL_ADR                           0x0E
#pragma pack (1)

//
// Information Record header that appears at the beginning of each record
//
typedef struct {
  UINT8                                Type;
  UINT8                                Reserved;
  UINT16                               RecordLength;
} EFI_ACPI_ASF_RECORD_HEADER;

#define ASF_RECORE_TYPE_LAST           0x80
//
// This structure contains information that identifies the system type
// and configuration
//
#define ASF_RECORE_TYPE_INFO           0x00
typedef struct {
  EFI_ACPI_ASF_RECORD_HEADER           RecordHeader;
  UINT8                                MinWatchDogResetValue;
  UINT8                                MinPollingInterval;
  UINT16                               SystemID;
  UINT32                               IANAManufactureID;
  UINT8                                FeatureFlags;// 7:1  Reserved for future definition by this specification;set to all 0's. 
                                                     // 0    Set to 1b to indicate that the platform supports ASF SMBus protocols to add-in 
                                                     //      alert-sending devices. 
  UINT8                                Reserved[3];
} EFI_ACPI_ASF_INFO;

//
// Alert sensors definition
//
#define ASF_RECORE_TYPE_ALRT           0x01
#define ASF_ALERT_DEVICE_COUNT         0x03

typedef struct { 
  UINT8  DeviceAddress;  //7:1  Contains the SMBus address of the legacy sensor device. 
                         //  0  Identifies whether (1) or not (0) the alerting condition (event) is based on an exact match.
  UINT8  Command;
  UINT8  DataMask;
  UINT8  CompareValue;
  UINT8  EventSensorType;
  UINT8  EventType;
  UINT8  EventOffset;
  UINT8  EventSourceType;
  UINT8  EventSeverity;
  UINT8  SensorNumber;
  UINT8  Entity;
  UINT8  EntityInstance;
} EFI_ASF_ALERTDATA;

typedef struct {
  EFI_ACPI_ASF_RECORD_HEADER           RecordHeader;
  UINT8                                AssertionEventBitMask;
  UINT8                                DeassertionEventBitMask;
  UINT8                                NumberOfAlerts;
  UINT8                                ArrayElementLength;
  EFI_ASF_ALERTDATA                    DeviceArray[ASF_ALERT_DEVICE_COUNT];
} EFI_ACPI_ASF_ALRT;

//
// Alert Remote Control System Actions
//
#define ASF_RECORE_TYPE_RCTL           0x02
#define ASF_RCTL_WITH_PEC              BIT0  // Remote control command must include a PEC
#define ASF_RCTL_SUPPORT_COMMAND_COUNT 0x04

typedef struct  { 
  UINT8                                Function;
  UINT8                                DeviceAddress;
  UINT8                                Command;
  UINT8                                DataValue;
} EFI_ASF_CONTROLDATA;

typedef struct {
  EFI_ACPI_ASF_RECORD_HEADER           RecordHeader;
  UINT8                                NumberOfControls;
  UINT8                                ArrayElementLength;
  UINT16                               RctlReserved;
  EFI_ASF_CONTROLDATA                  ControlArray[ASF_RCTL_SUPPORT_COMMAND_COUNT];
} EFI_ACPI_ASF_RCTL;

//
// Remote Control Capabilities
//
#define ASF_RECORE_TYPE_RMCP           0x03
#define ASF_RECORE_TYPE_RMC_COUNT      0x07

typedef struct {
  EFI_ACPI_ASF_RECORD_HEADER           RecordHeader;
  UINT8                                RemoteControlCapabilities[ASF_RECORE_TYPE_RMC_COUNT];
  UINT8                                RMCPCompletionCode;
  UINT32                               RMCPIANA;
  UINT8                                RMCPSpecialCommand;
  UINT8                                RMCPSpecialCommandParameter[2];
  UINT8                                RMCPBootOptions[2];
  UINT8                                RMCPOEMParameters[2];
} EFI_ACPI_ASF_RMCP;

//
// SMBus Devices with fixed addresses
//
#define ASF_RECORE_TYPE_ADDR           0x04
typedef struct {
  EFI_ACPI_ASF_RECORD_HEADER           RecordHeader;
  UINT8                                SEEPROMAddress;
  UINT8                                NumberOfDevices;
  UINT8                                FixedSmbusAddresses[ASF_ADDR_DEVICE_ARRAY_LENGTH];
} EFI_ACPI_ASF_ADDR;

//
// ASF Description Table
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER          Header;
  EFI_ACPI_ASF_INFO                    AsfInfo;
  EFI_ACPI_ASF_ALRT                    AsfAlert;
  EFI_ACPI_ASF_RCTL                    AsfRctl;
  EFI_ACPI_ASF_RMCP                    AsfRmcp;
  EFI_ACPI_ASF_ADDR                    AsfAddr;
} EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE;

#pragma pack ()

#endif
