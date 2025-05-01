/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Protocol/AmdPldmProtocol.h>
#include <MpmPldmBase.h>
#include <Library/MpmLib.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/IdsLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Filecode.h>

#define FILECODE MPM_MPMPLDM_MPMPLDMBASE_MPMPLDMBASE_FILECODE
AMD_PLDM_PROTOCOL mPldm = {
  PldmGetSmbiosStructureTableMetaData,
  PldmSetSmbiosStructureTableMetaData,
  PldmGetSmbiosStructureTable,
  PldmSetSmbiosStructureTable,
  PldmGetSmbiosStructureByType,
  PldmGetBiosTable,
  PldmSetBiosTable,
  PldmGetBiosTableTags,
  PldmSetBiosTableTags,
  PldmAcceptBiosAttributesPendingValues,
  PldmBiosGetDateTime,
  PldmBiosSetDateTime,
  PldmSensorPlatformEventMessage
};

/**
  Get Smbios Structure Table Meta Data from the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing SMBIOS Structure Table Meta
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmGetSmbiosStructureTableMetaData (
  AMD_PLDM_PROTOCOL                                        *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
  )
{
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT Format;
  EFI_STATUS Status;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, Buffer, BufferSize);

  return Status;
}

/**
  Set Smbios Structure Table Meta Data to the management controller

  @param        This             Pointer to AMD_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmSetSmbiosStructureTableMetaData (
  AMD_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry;
  UINT32 Crc32;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT ResponseFormat;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  ResponseFormat.CompletionCode = PLDM_COMPLETION_CODE_SUCCESS;

  Status = EfiGetSystemConfigurationTable (
             &gEfiSmbiosTableGuid,
             (VOID **)&SmbiosEntry
             );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Crc32 = 0;
  gBS->CalculateCrc32 ((VOID *)(UINTN) SmbiosEntry->TableAddress, SmbiosEntry->TableLength, &Crc32);

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE;

  Format.SmbiosMajorVersion = SmbiosEntry->MajorVersion;
  Format.SmbiosMinorVersion = SmbiosEntry->MinorVersion;
  Format.MaximumStructureSize = SmbiosEntry->MaxStructureSize;
  Format.SmbiosStructureTableLength = SmbiosEntry->TableLength;
  Format.NumberOfSmbiosStructures = SmbiosEntry->NumberOfSmbiosStructures;
  Format.SmbiosStructureTableIntegrityChecksum = Crc32;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

  return Status;
}

/**
  Get Smbios table from the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing the Smbios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmGetSmbiosStructureTable (
  AMD_PLDM_PROTOCOL                               *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
  )
{
  EFI_STATUS Status;
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, Buffer, BufferSize);

  return Status;
}

/**
  Set Smbios table to the management controller

  @param        This             Pointer to AMD_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmSetSmbiosStructureTable (
  AMD_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry;
  UINT32 PaddingSize;
  UINT8 *PointerToPadding;
  UINT8 *PointerToChecksum;
  UINT32 Crc32;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *Format;
  PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;
  EFI_SMBIOS_TABLE_HEADER *Record;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  EFI_SMBIOS_PROTOCOL *Smbios;

  Status = EfiGetSystemConfigurationTable (
             &gEfiSmbiosTableGuid,
             (VOID **)&SmbiosEntry
             );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //For debug using
  IDS_HDT_CONSOLE_PSP_TRACE ("PldmSetSmbiosStructureTable SmbiosTable:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("AnchorString                ------ '%c%c%c%c'\n",
                              SmbiosEntry->AnchorString[0],
                              SmbiosEntry->AnchorString[1],
                              SmbiosEntry->AnchorString[2],
                              SmbiosEntry->AnchorString[3]
                            );
  IDS_HDT_CONSOLE_PSP_TRACE ("EntryPointStructureChecksum ------ 0x%02x\n", SmbiosEntry->EntryPointStructureChecksum);
  IDS_HDT_CONSOLE_PSP_TRACE ("EntryPointLength            ------ 0x%02x\n", SmbiosEntry->EntryPointLength);
  IDS_HDT_CONSOLE_PSP_TRACE ("MajorVersion                ------ 0x%02x\n", SmbiosEntry->MajorVersion);
  IDS_HDT_CONSOLE_PSP_TRACE ("MinorVersion                ------ 0x%02x\n", SmbiosEntry->MinorVersion);
  IDS_HDT_CONSOLE_PSP_TRACE ("MaxStructureSize            ------ 0x%08x\n", SmbiosEntry->MaxStructureSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("EntryPointRevision          ------ 0x%02x\n", SmbiosEntry->EntryPointRevision);

  IDS_HDT_CONSOLE_PSP_TRACE ("IntermediateChecksum        ------ 0x%02x\n", SmbiosEntry->IntermediateChecksum);
  IDS_HDT_CONSOLE_PSP_TRACE ("TableLength                 ------ 0x%04x\n", SmbiosEntry->TableLength);
  IDS_HDT_CONSOLE_PSP_TRACE ("TableAddress                ------ 0x%08x\n", SmbiosEntry->TableAddress);
  IDS_HDT_CONSOLE_PSP_TRACE ("NumberOfSmbiosStructures    ------ 0x%04x\n", SmbiosEntry->NumberOfSmbiosStructures);
  IDS_HDT_CONSOLE_PSP_TRACE ("SmbiosBcdRevision           ------ 0x%02x\n", SmbiosEntry->SmbiosBcdRevision);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("Locate SmbiosProtocol Status %r\n", Status);
  if (!EFI_ERROR(Status)) {
    do {
      Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
      if (EFI_ERROR(Status)) {
        break;
      }
      IDS_HDT_CONSOLE_PSP_TRACE ("Smbios type %d\n", Record->Type);
    } while(Status == EFI_SUCCESS);
  }

  // padding requirement (0 ~ 3 bytes)
  PaddingSize = (4 - (SmbiosEntry->TableLength % 4)) % 4;

  // Total request buffer size = RequestFormat + Smbios tables + padding + checksum
  RequestBufferSize = (UINT32) sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT) + SmbiosEntry->TableLength + PaddingSize + 4;
  RequestBuffer = NULL;
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in smbios tables
  CopyMem ((UINT8 *) RequestBuffer + sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT), (VOID *)(UINTN) SmbiosEntry->TableAddress, SmbiosEntry->TableLength);

  // Fill in padding
  PointerToPadding = (UINT8 *) RequestBuffer + sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT) + SmbiosEntry->TableLength;
  ZeroMem (PointerToPadding, PaddingSize);

  // Fill in checksum
  gBS->CalculateCrc32 ((UINT8 *) RequestBuffer + sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT), SmbiosEntry->TableLength + PaddingSize, &Crc32);
  PointerToChecksum = PointerToPadding + PaddingSize;
  CopyMem (PointerToChecksum, &Crc32, 4);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = PLDM_TYPE_SMBIOS;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = PLDM_TRANSFER_FLAG_START_AND_END;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get Smbios table by type from the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing the Smbios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmGetSmbiosStructureByType (
  AMD_PLDM_PROTOCOL                               *This,
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize,
  UINT8                                           TypeId,
  UINT16                                          StructureInstanceId
  )
{
  EFI_STATUS Status;
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;
  Format.Type = TypeId;
  Format.StructureInstanceId = StructureInstanceId;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, Buffer, BufferSize);

  return Status;
}

/**
  Get bios table from the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE or PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE
  @param [out]  Buffer           Buffer storing the bios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmGetBiosTable (
  AMD_PLDM_PROTOCOL                   *This,
  UINT8                               TableType,
  PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
  )
{
  EFI_STATUS Status;
  PLDM_GET_BIOS_TABLE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_BIOS_TABLE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;
  Format.TableType = TableType;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_BIOS_TABLE_REQUEST_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, Buffer, BufferSize);

  return Status;
}

/**
  Set bios table to the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [in]   Buffer           Buffer storing the bios table
  @param [in]   BufferSize       Number of bytes of the bios table

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmSetBiosTable (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
  )
{
  EFI_STATUS Status;
  PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *Format;
  PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  RequestBufferSize = (*BufferSize) + sizeof (PLDM_SET_BIOS_TABLE_REQUEST_FORMAT);
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in bios table
  CopyMem ((UINT8 *) RequestBuffer + sizeof (PLDM_SET_BIOS_TABLE_REQUEST_FORMAT), Buffer, *BufferSize);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = PLDM_SET_BIOS_TABLE_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = PLDM_TRANSFER_FLAG_START_AND_END;
  Format->TableType = TableType;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get one Bios Table Tag from the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [out]  TableTag         Pointer to Bios Table Tag

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmGetBiosTableTags (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
  )
{
  EFI_STATUS Status;
  PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT Format;
  PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_BIOS_TABLE_TAGS_COMMAND_CODE;
  Format.NumberOfTables = 1;
  Format.TableType = TableType;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

  if (ResponseFormat.CompletionCode == PLDM_COMPLETION_CODE_SUCCESS) {
    *TableTag = ResponseFormat.TableTag;
  }

  return Status;
}

/**
  Set one Bios Table Tag to the management controller

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [in]   TableTag         Bios Table Tag

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmSetBiosTableTags (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
  )
{
  EFI_STATUS Status;
  PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT Format;
  PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_SET_BIOS_TABLE_TAGS_COMMAND_CODE;
  Format.NumberOfTables = 1;
  Format.TableType = TableType;
  Format.TableTag = TableTag;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

  return Status;
}

/**
  Accept BIOS Attribute Pending Values

  @param [in]   This                         Pointer to AMD_PLDM_PROTOCOL
  @param [in]   NumberOfAttributeHandles     Number of BIOS Attribute Handles with Pending Values to be accepted
                                             If all the pending attribute values are rejected, the NumberOfAttributeHandles shall be set to 0
  @param [in]   AttributeHandles             Array of Attribute Handles

  @retval EFI_SUCCESS                        Successful
  @return Other values                       Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmAcceptBiosAttributesPendingValues (
  AMD_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
  )
{
  EFI_STATUS Status;
  UINT32 PaddingSize;
  UINT32 BIOSAttributesHandlesSize;
  UINT8 *PointerToPadding;
  UINT8 *PointerToChecksum;
  UINT32 Crc32;
  PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *Format;
  PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  // padding requirement (0 or 2 bytes)
  if ((NumberOfAttributeHandles % 2) == 1) {
    PaddingSize = 2;
  } else {
    PaddingSize = 0;
  }

  // The structure of BIOSAttributesHandles is defined in Spec DSP0247 Table 38.
  // BIOSAttributesHandlesSize = NumberOfAttributeHandles (2 bytes) + AttributeHandles (2 * NumberOfAttributeHandles) + Pad (0 or 2 bytes) + CheckSum (4 bytes)
  BIOSAttributesHandlesSize = 2 + 2 * NumberOfAttributeHandles + PaddingSize + 4;
  RequestBufferSize = (UINT32) sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + BIOSAttributesHandlesSize;
  RequestBuffer = NULL;
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in NumberOfAttributeHandles
  CopyMem ((UINT8 *) RequestBuffer + sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT), (VOID *) &NumberOfAttributeHandles, 2);

  // Fill in AttributeHandles
  CopyMem ((UINT8 *) RequestBuffer + sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + 2, (VOID *) AttributeHandles, (2 * NumberOfAttributeHandles));

  // Fill in padding
  PointerToPadding = (UINT8 *) RequestBuffer + sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + 2 + (2 * NumberOfAttributeHandles);
  ZeroMem (PointerToPadding, PaddingSize);

  // Fill in checksum
  gBS->CalculateCrc32 ((UINT8 *) RequestBuffer + sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT), BIOSAttributesHandlesSize - 4, &Crc32);
  PointerToChecksum = PointerToPadding + PaddingSize;
  CopyMem (PointerToChecksum, &Crc32, 4);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = PLDM_TRANSFER_FLAG_START_AND_END;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get Date Time from the management controller.

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing Date Time information
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmBiosGetDateTime (
  AMD_PLDM_PROTOCOL                  *This,
  PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
  )
{
  PLDM_GET_DATE_TIME_REQUEST_FORMAT Format;
  EFI_STATUS Status;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_GET_DATE_TIME_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_GET_DATE_TIME_REQUEST_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, Buffer, BufferSize);

  return Status;
}

/**
  Set Date Time to the management controller.

  @param [in]   This             Pointer to AMD_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
PldmBiosSetDateTime (
  AMD_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  EFI_TIME Time;
  PLDM_SET_DATE_TIME_REQUEST_FORMAT Format;
  PLDM_SET_DATE_TIME_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  gRT->GetTime (&Time, NULL);

  Format.Seconds = DecimalToBcd8 (Time.Second);
  Format.Minutes = DecimalToBcd8 (Time.Minute);
  Format.Hours = DecimalToBcd8 (Time.Hour);
  Format.Day = DecimalToBcd8 (Time.Day);
  Format.Month = DecimalToBcd8 (Time.Month);
  Format.Year = 0x100 * DecimalToBcd8 ((UINT8)(Time.Year / 100)) + DecimalToBcd8 ((UINT8)(Time.Year % 100));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = PLDM_SET_DATE_TIME_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (PLDM_SET_DATE_TIME_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SET_DATE_TIME_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

  return Status;

}

/**
  BIOS send Platform_Event_Message command to MPM
  * @param [in]   This              Pointer to AMD_PLDM_PROTOCOL,
                                    Caller need prepare it before calling this function
  * @param[in]   SensorData         Caller need prepare the SensorData and pass it to this function

  * @retval EFI_STATUS              1: Success, others: fail
**/
EFI_STATUS
EFIAPI
PldmSensorPlatformEventMessage (
  IN AMD_PLDM_PROTOCOL *This,
  IN PLDM_SENSOR_T *SensorData
  )
{
  EFI_STATUS Status;
  PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_REQUEST_FORMAT RequestFormat;
  PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  RequestFormat.PldmHeader.InstanceId = 0;
  RequestFormat.PldmHeader.Reserved = 0;
  RequestFormat.PldmHeader.DatagramBit = 0;
  RequestFormat.PldmHeader.RequestBit = 1;
  RequestFormat.PldmHeader.PldmType = PLDM_TYPE_PLATFORM_MONITORING_AND_CONTROL;
  RequestFormat.PldmHeader.HeaderVersion = 0;
  RequestFormat.PldmHeader.PldmCommandCode = PLATFORM_EVENT_MESSAGE;

  CopyMem (&(RequestFormat.RequestData), SensorData, sizeof(RequestFormat.RequestData));
  RequestBuffer = &RequestFormat;
  RequestBufferSize = sizeof (PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_REQUEST_FORMAT) - sizeof (FANINFO_T);
  //only Fan sensor need FANINFO_T structure
  if (SensorData->Sensor.SensorType == SENSOR_TYPE_TACHOMETER) {
    RequestBufferSize = sizeof (PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_REQUEST_FORMAT);
  }
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (PLDM_SENSOR_PLATFORM_EVENT_MESSAGE_RESPONSE_FORMAT);

  Status = BiosMpmPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize);

  return Status;

}

/**
  Entry point of the driver.
  Install gAmdPldmProtocolGuid protocol.

  @param [in]  ImageHandle  ImageHandle of the loaded driver.
  @param [in]  SystemTable  Pointer to the EFI System Table.

  @retval  EFI_SUCCESS      This driver returns successfully.

**/
EFI_STATUS
EFIAPI
MpmPldmBaseEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HANDLE mHandle;

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }
  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  mHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &mHandle,
                  &gAmdPldmProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPldm
                  );

  return Status;
}
