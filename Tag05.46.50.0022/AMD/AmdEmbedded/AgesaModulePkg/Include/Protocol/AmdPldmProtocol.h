/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef __AMD_PLDM_PROTOCOL_H__
#define __AMD_PLDM_PROTOCOL_H__

#include <Protocol/AmdMpmGetSensorInfoProtocol.h>

typedef struct _AMD_PLDM_PROTOCOL AMD_PLDM_PROTOCOL;

#pragma pack(1)

// General definitions from Platform Level Data Model (PLDM) Base Specification (DMTF DSP0240)
typedef struct  {
  UINT8 InstanceId              : 5;
  UINT8 Reserved                : 1;
  UINT8 DatagramBit             : 1;
  UINT8 RequestBit              : 1;
  UINT8 PldmType                : 6;
  UINT8 HeaderVersion           : 2;
  UINT8 PldmCommandCode;
} PLDM_HEADER;

#define PLDM_COMPLETION_CODE_SUCCESS                      0x00
#define PLDM_COMPLETION_CODE_ERROR                        0x01
#define PLDM_COMPLETION_CODE_ERROR_INVALID_DATA           0x02
#define PLDM_COMPLETION_CODE_ERROR_INVALID_LENGTH         0x03
#define PLDM_COMPLETION_CODE_ERROR_NOT_READY              0x04
#define PLDM_COMPLETION_CODE_ERROR_UNSUPPORTED_PLDM_CMD   0x05
#define PLDM_COMPLETION_CODE_ERROR_INVALID_PLDM_TYPE      0x20


// Type Code definitions from Platform Level Data Model (PLDM) IDs and Codes Specification (DMTF SDP0245)
#define PLDM_TYPE_MESSAGE_CONTROL_AND_DISCOVERY    0x00
#define PLDM_TYPE_SMBIOS                           0x01
#define PLDM_TYPE_PLATFORM_MONITORING_AND_CONTROL  0x02
#define PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION   0x03

// Smbios-related definitions from PLDM for SMBIOS Transfer Specification (DMTF DSP0246)
#define PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE   0x01
#define PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE   0x02
#define PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE            0x03
#define PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE            0x04
#define PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND_CODE          0x05
#define PLDM_GET_SMBIOS_STRUCTURE_BY_HANDLE_COMMAND_CODE        0x06

#define PLDM_TRANSFER_FLAG_START         0x01
#define PLDM_TRANSFER_FLAG_MIDDLE        0x02
#define PLDM_TRANSFER_FLAG_END           0x04
#define PLDM_TRANSFER_FLAG_START_AND_END 0x05

#define PLDM_TRANSFER_OPERATION_FLAG_GET_NEXT_PART  0x00
#define PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART 0x01

#define PLDM_COMPLETION_CODE_INVALID_DATA_TRANSFER_HANDLE        0x80
#define PLDM_COMPLETION_CODE_INVALID_TRANSFER_OPERATION_FLAG     0x81
#define PLDM_COMPLETION_CODE_INVALID_TRANSFER_FLAG               0x82
#define PLDM_COMPLETION_CODE_NO_SMBIOS_STRUCTURE_TABLE_METADATA  0x83
#define PLDM_COMPLETION_CODE_INVALID_DATA_INTEGRITY_CHECK        0x84
#define PLDM_COMPLETION_CODE_SMBIOS_STRUCTURE_TABLE_UNAVAILABLE  0x85

typedef struct {
  PLDM_HEADER PldmHeader;
} PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT;

typedef struct {
  UINT8   CompletionCode;
  UINT8   SmbiosMajorVersion;
  UINT8   SmbiosMinorVersion;
  UINT16  MaximumStructureSize;
  UINT16  SmbiosStructureTableLength;
  UINT16  NumberOfSmbiosStructures;
  UINT32  SmbiosStructureTableIntegrityChecksum;
} PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT8       SmbiosMajorVersion;
  UINT8       SmbiosMinorVersion;
  UINT16      MaximumStructureSize;
  UINT16      SmbiosStructureTableLength;
  UINT16      NumberOfSmbiosStructures;
  UINT32      SmbiosStructureTableIntegrityChecksum;
} PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT;

typedef struct {
  UINT8 CompletionCode;
} PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
} PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
  UINT8  TransferFlag;
} PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
} PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
} PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
  UINT8       Type;
  UINT16      StructureInstanceId;
} PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
  UINT8  TransferFlag;
} PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT;

// Definitions from PLDM for BIOS Control and Configuration Spec (DMTF DSP0247)
#define PLDM_GET_BIOS_TABLE_COMMAND_CODE                              0x01
#define PLDM_SET_BIOS_TABLE_COMMAND_CODE                              0x02
#define PLDM_UPDATE_BIOS_TABLE_COMMAND_CODE                           0x03
#define PLDM_GET_BIOS_TABLE_TAGS_COMMAND_CODE                         0x04
#define PLDM_SET_BIOS_TABLE_TAGS_COMMAND_CODE                         0x05
#define PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE       0x06
#define PLDM_SET_BIOS_ATTRIBUTE_CURRENT_VALUE_COMMAND_CODE            0x07
#define PLDM_GET_BIOS_ATTRIBUTE_CURRENT_VALUE_BY_HANDLE_COMMAND_CODE  0x08
#define PLDM_GET_BIOS_ATTRIBUTE_PENDING_VALUE_BY_HANDLE_COMMAND_CODE  0x09
#define PLDM_GET_BIOS_ATTRIBUTE_CURRENT_VALUE_BY_TYPE_COMMAND_CODE    0x0a
#define PLDM_GET_BIOS_ATTRIBUTE_PENDING_VALUE_BY_TYPE_COMMAND_CODE    0x0b
#define PLDM_GET_DATE_TIME_COMMAND_CODE                               0x0c
#define PLDM_SET_DATE_TIME_COMMAND_CODE                               0x0d
#define PLDM_GET_BIOS_STRING_TABLE_STRING_TYPE_COMMAND_CODE           0x0e
#define PLDM_SET_BIOS_STRING_TABLE_STRING_TYPE_COMMAND_CODE           0x0f

#define PLDM_BIOS_STRING_TABLE                  0x00
#define PLDM_BIOS_ATTRIBUTE_TABLE               0x01
#define PLDM_BIOS_ATTRIBUTE_VALUE_TABLE         0x02
#define PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE 0x03

#define PLDM_COMPLETION_CODE_BIOS_TABLE_UNAVAILABLE                    0x83
#define PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_DATA_INTEGRITY_CHECK   0x84
#define PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_TYPE                   0x85
#define PLDM_COMPLETION_CODE_BIOS_TABLE_TAG_UNAVAILABLE                0x86
#define PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_TAG_TYPE               0x87
#define PLDM_COMPLETION_CODE_INVALID_BIOS_ATTR_HANDLE                  0x88
#define PLDM_COMPLETION_CODE_INVALID_BIOS_ATTR_TYPE                    0x89
#define PLDM_COMPLETION_CODE_NO_DATE_TIME_INFO_AVAILABLE               0x8A
#define PLDM_COMPLETION_CODE_INVALID_STRING_TYPE                       0x8B

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
  UINT8       TableType;
} PLDM_GET_BIOS_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
  UINT8  TransferFlag;
} PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
  UINT8       TableType;
} PLDM_SET_BIOS_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
} PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT8       NumberOfTables;
  UINT8       TableType;
} PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 TableTag;
} PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT8       NumberOfTables;
  UINT8       TableType;
  UINT32      TableTag;
} PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT;

typedef struct {
  UINT8 CompletionCode;
} PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
} PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT32 NextDataTransferHandle;
} PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
} PLDM_GET_DATE_TIME_REQUEST_FORMAT;

typedef struct {
  UINT8  CompletionCode;
  UINT8  Seconds;
  UINT8  Minutes;
  UINT8  Hours;
  UINT8  Day;
  UINT8  Month;
  UINT16 Year;
} PLDM_GET_DATE_TIME_RESPONSE_FORMAT;

typedef struct {
  PLDM_HEADER PldmHeader;
  UINT8       Seconds;
  UINT8       Minutes;
  UINT8       Hours;
  UINT8       Day;
  UINT8       Month;
  UINT16      Year;
} PLDM_SET_DATE_TIME_REQUEST_FORMAT;

typedef struct {
  UINT8 CompletionCode;
} PLDM_SET_DATE_TIME_RESPONSE_FORMAT;

// Definitions from PLDM for Platform Monitor And Control (DMTF DSP0248)
#define PLATFORM_EVENT_MESSAGE                        0xA

typedef struct PLDM_PLATFORM_EVENT_MESSAGE {
   PLDM_HEADER PldmHeader;
   PLDM_SENSOR_T RequestData;
} PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_REQUEST_FORMAT;

typedef struct {
  UINT8 CompletionCode;
} PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_RESPONSE_FORMAT;

// AMD PLDM Protocol definitions
typedef
EFI_STATUS
(EFIAPI *PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND) (
  AMD_PLDM_PROTOCOL                                        *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND) (
  AMD_PLDM_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND) (
  AMD_PLDM_PROTOCOL                               *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND) (
  AMD_PLDM_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND) (
  AMD_PLDM_PROTOCOL                               *This,
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize,
  UINT8                                           TypeId,
  UINT16                                          StructureInstanceId
);

typedef
EFI_STATUS
(EFIAPI *PLDM_GET_BIOS_TABLE_COMMAND) (
  AMD_PLDM_PROTOCOL                   *This,
  UINT8                               TableType,
  PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SET_BIOS_TABLE_COMMAND) (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *PLDM_GET_BIOS_TABLE_TAGS_COMMAND) (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SET_BIOS_TABLE_TAGS_COMMAND) (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
);

typedef
EFI_STATUS
(EFIAPI *PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND) (
  AMD_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

typedef
EFI_STATUS
(EFIAPI *PLDM_GET_DATE_TIME_COMMAND) (
  AMD_PLDM_PROTOCOL                  *This,
  PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SET_DATE_TIME_COMMAND) (
  AMD_PLDM_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_COMMAND) (
  AMD_PLDM_PROTOCOL *This,
  PLDM_SENSOR_T  *SensorData
);

struct _AMD_PLDM_PROTOCOL {
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND   GetSmbiosStructureTableMetaData;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND   SetSmbiosStructureTableMetaData;
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND            GetSmbiosStructureTable;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND            SetSmbiosStructureTable;
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND          GetSmbiosStructureByType;
  PLDM_GET_BIOS_TABLE_COMMAND                        GetBiosTable;
  PLDM_SET_BIOS_TABLE_COMMAND                        SetBiosTable;
  PLDM_GET_BIOS_TABLE_TAGS_COMMAND                   GetBiosTableTags;
  PLDM_SET_BIOS_TABLE_TAGS_COMMAND                   SetBiosTableTags;
  PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND AcceptBiosAttributesPendingValues;
  PLDM_GET_DATE_TIME_COMMAND                         GetDateTime;
  PLDM_SET_DATE_TIME_COMMAND                         SetDateTime;
  PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_COMMAND         SensorPlatformEventMessage;
};
#pragma pack()

extern EFI_GUID gAmdPldmProtocolGuid;

#endif // __AMD_PLDM_PROTOCOL_H__
