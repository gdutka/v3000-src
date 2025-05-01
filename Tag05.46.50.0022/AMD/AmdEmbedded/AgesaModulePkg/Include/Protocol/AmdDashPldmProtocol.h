/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef __AMD_DASH_PLDM_PROTOCOL_H__
#define __AMD_DASH_PLDM_PROTOCOL_H__

typedef struct _AMD_DASH_PLDM_PROTOCOL AMD_DASH_PLDM_PROTOCOL;

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
} DASH_PLDM_HEADER;

#define DASH_PLDM_COMPLETION_CODE_SUCCESS                      0x00
#define DASH_PLDM_COMPLETION_CODE_ERROR                        0x01
#define DASH_PLDM_COMPLETION_CODE_ERROR_INVALID_DATA           0x02
#define DASH_PLDM_COMPLETION_CODE_ERROR_INVALID_LENGTH         0x03
#define DASH_PLDM_COMPLETION_CODE_ERROR_NOT_READY              0x04
#define DASH_PLDM_COMPLETION_CODE_ERROR_UNSUPPORTED_PLDM_CMD   0x05
#define DASH_PLDM_COMPLETION_CODE_ERROR_INVALID_PLDM_TYPE      0x20


// Type Code definitions from Platform Level Data Model (PLDM) IDs and Codes Specification (DMTF SDP0245)
#define DASH_PLDM_TYPE_MESSAGE_CONTROL_AND_DISCOVERY    0x00
#define DASH_PLDM_TYPE_SMBIOS                           0x01
#define DASH_PLDM_TYPE_PLATFORM_MONITORING_AND_CONTROL  0x02
#define DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION   0x03

// Smbios-related definitions from PLDM for SMBIOS Transfer Specification (DMTF DSP0246)
#define DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE   0x01
#define DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE   0x02
#define DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE            0x03
#define DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE            0x04
#define DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND_CODE          0x05
#define DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_HANDLE_COMMAND_CODE        0x06

#define DASH_PLDM_TRANSFER_FLAG_START         0x01
#define DASH_PLDM_TRANSFER_FLAG_MIDDLE        0x02
#define DASH_PLDM_TRANSFER_FLAG_END           0x04
#define DASH_PLDM_TRANSFER_FLAG_START_AND_END 0x05

#define DASH_PLDM_TRANSFER_OPERATION_FLAG_GET_NEXT_PART  0x00
#define DASH_PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART 0x01

#define DASH_PLDM_COMPLETION_CODE_INVALID_DATA_TRANSFER_HANDLE        0x80
#define DASH_PLDM_COMPLETION_CODE_INVALID_TRANSFER_OPERATION_FLAG     0x81
#define DASH_PLDM_COMPLETION_CODE_INVALID_TRANSFER_FLAG               0x82
#define DASH_PLDM_COMPLETION_CODE_NO_SMBIOS_STRUCTURE_TABLE_METADATA  0x83
#define DASH_PLDM_COMPLETION_CODE_INVALID_DATA_INTEGRITY_CHECK        0x84
#define DASH_PLDM_COMPLETION_CODE_SMBIOS_STRUCTURE_TABLE_UNAVAILABLE  0x85

typedef struct {
  DASH_PLDM_HEADER  PldmHeader;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT8       SmbiosMajorVersion;
  UINT8       SmbiosMinorVersion;
  UINT16      MaximumStructureSize;
  UINT16      SmbiosStructureTableLength;
  UINT16      NumberOfSmbiosStructures;
  UINT32      SmbiosStructureTableIntegrityChecksum;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT8       SmbiosMajorVersion;
  UINT8       SmbiosMinorVersion;
  UINT16      MaximumStructureSize;
  UINT16      SmbiosStructureTableLength;
  UINT16      NumberOfSmbiosStructures;
  UINT32      SmbiosStructureTableIntegrityChecksum;
} DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      Data;         // Add this item for DASF FW response data
} DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
  UINT8       TransferFlag;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
} DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
} DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
  UINT8       Type;
  UINT16      StructureInstanceId;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
  UINT8       TransferFlag;
} DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT;

// Definitions from PLDM for BIOS Control and Configuration Spec (DMTF DSP0247)
#define DASH_PLDM_GET_BIOS_TABLE_COMMAND_CODE                              0x01
#define DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE                              0x02
#define DASH_PLDM_UPDATE_BIOS_TABLE_COMMAND_CODE                           0x03
#define DASH_PLDM_GET_BIOS_TABLE_TAGS_COMMAND_CODE                         0x04
#define DASH_PLDM_SET_BIOS_TABLE_TAGS_COMMAND_CODE                         0x05
#define DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE       0x06
#define DASH_PLDM_SET_BIOS_ATTRIBUTE_CURRENT_VALUE_COMMAND_CODE            0x07
#define DASH_PLDM_GET_BIOS_ATTRIBUTE_CURRENT_VALUE_BY_HANDLE_COMMAND_CODE  0x08
#define DASH_PLDM_GET_BIOS_ATTRIBUTE_PENDING_VALUE_BY_HANDLE_COMMAND_CODE  0x09
#define DASH_PLDM_GET_BIOS_ATTRIBUTE_CURRENT_VALUE_BY_TYPE_COMMAND_CODE    0x0a
#define DASH_PLDM_GET_BIOS_ATTRIBUTE_PENDING_VALUE_BY_TYPE_COMMAND_CODE    0x0b
#define DASH_PLDM_GET_DATE_TIME_COMMAND_CODE                               0x0c
#define DASH_PLDM_SET_DATE_TIME_COMMAND_CODE                               0x0d
#define DASH_PLDM_GET_BIOS_STRING_TABLE_STRING_TYPE_COMMAND_CODE           0x0e
#define DASH_PLDM_SET_BIOS_STRING_TABLE_STRING_TYPE_COMMAND_CODE           0x0f

#define DASH_PLDM_BIOS_STRING_TABLE                  0x00
#define DASH_PLDM_BIOS_ATTRIBUTE_TABLE               0x01
#define DASH_PLDM_BIOS_ATTRIBUTE_VALUE_TABLE         0x02
#define DASH_PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE 0x03

#define DASH_PLDM_COMPLETION_CODE_BIOS_TABLE_UNAVAILABLE                    0x83
#define DASH_PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_DATA_INTEGRITY_CHECK   0x84
#define DASH_PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_TYPE                   0x85
#define DASH_PLDM_COMPLETION_CODE_BIOS_TABLE_TAG_UNAVAILABLE                0x86
#define DASH_PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_TAG_TYPE               0x87
#define DASH_PLDM_COMPLETION_CODE_INVALID_BIOS_ATTR_HANDLE                  0x88
#define DASH_PLDM_COMPLETION_CODE_INVALID_BIOS_ATTR_TYPE                    0x89
#define DASH_PLDM_COMPLETION_CODE_NO_DATE_TIME_INFO_AVAILABLE               0x8A
#define DASH_PLDM_COMPLETION_CODE_INVALID_STRING_TYPE                       0x8B

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferOperationFlag;
  UINT8       TableType;
} DASH_PLDM_GET_BIOS_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
  UINT8       TransferFlag;
} DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
  UINT8       TableType;
} DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
} DASH_PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT8       NumberOfTables;
  UINT8       TableType;
} DASH_PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      TableTag1;		// Add this item for DASF FW response data
  UINT32      TableTag2;		// Add this item for DASF FW response data
  UINT32      TableTag3;		// Add this item for DASF FW response data
} DASH_PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT8       NumberOfTables;
  UINT8       TableType;
  UINT32      TableTag;
} DASH_PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      Data;         // Add this item for DASF FW response data
} DASH_PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT32      DataTransferHandle;
  UINT8       TransferFlag;
} DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT32      NextDataTransferHandle;
} DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
} DASH_PLDM_GET_DATE_TIME_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
  UINT8       Seconds;
  UINT8       Minutes;
  UINT8       Hours;
  UINT8       Day;
  UINT8       Month;
  UINT16      Year;
} DASH_PLDM_GET_DATE_TIME_RESPONSE_FORMAT;

typedef struct {
  DASH_PLDM_HEADER PldmHeader;
  UINT8       Seconds;
  UINT8       Minutes;
  UINT8       Hours;
  UINT8       Day;
  UINT8       Month;
  UINT16      Year;
} DASH_PLDM_SET_DATE_TIME_REQUEST_FORMAT;

typedef struct {
  UINT8       CompletionCode;
} DASH_PLDM_SET_DATE_TIME_RESPONSE_FORMAT;

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL                                   *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL                          *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL                            *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize,
  UINT8                                           TypeId,
  UINT16                                          StructureInstanceId
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_BIOS_TABLE_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL              *This,
  UINT8                               TableType,
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_SET_BIOS_TABLE_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_BIOS_TABLE_TAGS_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_SET_BIOS_TABLE_TAGS_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_GET_DATE_TIME_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL                  *This,
  DASH_PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
);

typedef
EFI_STATUS
(EFIAPI *DASH_PLDM_SET_DATE_TIME_COMMAND) (
  AMD_DASH_PLDM_PROTOCOL *This
);

struct _AMD_DASH_PLDM_PROTOCOL {
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND   DashGetSmbiosStructureTableMetaData;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND   DashSetSmbiosStructureTableMetaData;
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND            DashGetSmbiosStructureTable;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND            DashSetSmbiosStructureTable;
  DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND          DashGetSmbiosStructureByType;
  DASH_PLDM_GET_BIOS_TABLE_COMMAND                        DashGetBiosTable;
  DASH_PLDM_SET_BIOS_TABLE_COMMAND                        DashSetBiosTable;
  DASH_PLDM_GET_BIOS_TABLE_TAGS_COMMAND                   DashGetBiosTableTags;
  DASH_PLDM_SET_BIOS_TABLE_TAGS_COMMAND                   DashSetBiosTableTags;
  DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND DashAcceptBiosAttributesPendingValues;
  DASH_PLDM_GET_DATE_TIME_COMMAND                         DashGetDateTime;
  DASH_PLDM_SET_DATE_TIME_COMMAND                         DashSetDateTime;
};
#pragma pack()

extern EFI_GUID gAmdDashPldmProtocolGuid;

#endif // __AMD_DASH_PLDM_PROTOCOL_H__
