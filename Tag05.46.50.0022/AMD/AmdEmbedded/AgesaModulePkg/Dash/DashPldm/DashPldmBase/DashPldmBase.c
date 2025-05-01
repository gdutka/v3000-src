/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Protocol/AmdDashPldmProtocol.h>
#include <DashPldmBase.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Protocol/Smbios.h>
#include <Library/IdsLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/MctpIo.h>

UINT8 mInstanceId;

AMD_DASH_PLDM_PROTOCOL mPldmFun = {
  DashPldmGetSmbiosStructureTableMetaData,
  DashPldmSetSmbiosStructureTableMetaData,
  DashPldmGetSmbiosStructureTable,
  DashPldmSetSmbiosStructureTable,
  DashPldmGetSmbiosStructureByType,
  DashPldmGetBiosTable,
  DashPldmSetBiosTable,
  DashPldmGetBiosTableTags,
  DashPldmSetBiosTableTags,
  DashPldmAcceptBiosAttributesPendingValues,
  DashPldmBiosGetDateTime,
  DashPldmBiosSetDateTime
};

EFI_STATUS
EFIAPI
PldmTransferFlagAndData (
  IN     VOID    *RequestBuffer,
  IN     UINT32  RequestBufferSize,
  IN OUT VOID    **ReturnRequestBuffer,
  IN OUT UINT32  *ReturnRequestBufferSize,
  IN     UINT8   CommandCode
  )
{
  UINT32        PacketCount, MessageSize, PacketMaxLength, RequestBufferSizeTemp;
  UINT32        FinalSize, RequestFormatSize, IncreaseCounters;
  MCTP_MSG_TYPE *RequestPldmTransferFlag;
  VOID          *RequestMessageData, *ReturnData;

  DEBUG ((DEBUG_INFO,"PldmTransferFlagAndData Entry\n"));

  MessageSize = 0;
  PacketMaxLength = 0;
  RequestBufferSizeTemp = 0;
  FinalSize = 0;
  RequestFormatSize = 0;

  RequestMessageData = NULL;
  if(CommandCode == DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE){
    RequestFormatSize = sizeof(DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT);
    RequestMessageData = AllocateZeroPool(sizeof(DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT));
    CopyMem ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData, RequestBuffer, RequestFormatSize);
  } else if (CommandCode == DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE){
    RequestFormatSize = sizeof(DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT);
    RequestMessageData = AllocateZeroPool(sizeof(DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT));
    CopyMem ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData, RequestBuffer, RequestFormatSize);
  } else if (CommandCode == DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE){
    RequestFormatSize = sizeof(DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT);
    RequestMessageData = AllocateZeroPool(sizeof(DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT));
    CopyMem ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData, RequestBuffer, RequestFormatSize);
  }
  PacketMaxLength = MCTP_SMBUS_TRANSFER_DATA_LENGTH - sizeof(MCTP_MSG_TYPE) - RequestFormatSize;

  IncreaseCounters = 0;
  RequestBufferSizeTemp = RequestBufferSize;
  RequestBufferSizeTemp -= RequestFormatSize;
  while (RequestBufferSizeTemp > PacketMaxLength) {
    IncreaseCounters++;
    RequestBufferSizeTemp -= PacketMaxLength;
  }
  IncreaseCounters++;
  FinalSize = (RequestBufferSize - RequestFormatSize) + ((sizeof(MCTP_MSG_TYPE) + RequestFormatSize) * IncreaseCounters);
  *ReturnRequestBufferSize = FinalSize;

  ReturnData = AllocateZeroPool(FinalSize);
  *ReturnRequestBuffer = ReturnData;

  PacketCount = 1;
  RequestPldmTransferFlag = AllocateZeroPool(sizeof(MCTP_MSG_TYPE));
  RequestPldmTransferFlag->MctpMsgType = MCTP_PLDM_TYPE;
  RequestBuffer = (VOID *)((UINT8 *)RequestBuffer + RequestFormatSize);

  MessageSize = RequestBufferSize - RequestFormatSize;

  while (MessageSize > PacketMaxLength) {
    CopyMem(ReturnData, (VOID *)RequestPldmTransferFlag, sizeof(MCTP_MSG_TYPE));
    ReturnData = (VOID *)((UINT8 *)ReturnData + sizeof(MCTP_MSG_TYPE));

    if(PacketCount == 1){
      if(CommandCode == DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE){
        ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START;
        ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      } else if (CommandCode == DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE){
        ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START;
        ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      } else if (CommandCode == DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE){
        ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START;
        ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      }
    } else if (PacketCount > 1) {
      if(CommandCode == DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE){
        ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_MIDDLE;
        ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      } else if (CommandCode == DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE){
        ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_MIDDLE;
        ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      } else if (CommandCode == DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE){
        ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_MIDDLE;
        ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
      }
    }
    CopyMem(ReturnData, RequestMessageData, RequestFormatSize);
    ReturnData = (VOID *)((UINT8 *)ReturnData + RequestFormatSize);
    CopyMem(ReturnData, RequestBuffer, PacketMaxLength);
    ReturnData = (VOID *)((UINT8 *)ReturnData + PacketMaxLength);

    RequestBuffer = (VOID *)((UINT8 *)RequestBuffer + PacketMaxLength);
    MessageSize = MessageSize - PacketMaxLength;
    PacketCount++;
    mInstanceId++;
    mInstanceId = mInstanceId & 0x1F;
  }

  if(PacketCount == 1){
    CopyMem(ReturnData, (VOID *)RequestPldmTransferFlag, sizeof(MCTP_MSG_TYPE));
    ReturnData = (VOID *)((UINT8 *)ReturnData + sizeof(MCTP_MSG_TYPE));

    ((DASH_PLDM_HEADER *)RequestMessageData)->InstanceId = mInstanceId;
    CopyMem(ReturnData, RequestMessageData, RequestFormatSize);
    ReturnData = (VOID *)((UINT8 *)ReturnData + RequestFormatSize);
    CopyMem(ReturnData, RequestBuffer, MessageSize);
  } else if (PacketCount > 1){
    CopyMem(ReturnData, (VOID *)RequestPldmTransferFlag, sizeof(MCTP_MSG_TYPE));
    ReturnData = (VOID *)((UINT8 *)ReturnData + sizeof(MCTP_MSG_TYPE));

    if(CommandCode == DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE){
      ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_END;
      ((DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
    } else if (CommandCode == DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE){
      ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_END;
      ((DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
    } else if (CommandCode == DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE){
      ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->TransferFlag = DASH_PLDM_TRANSFER_FLAG_END;
      ((DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *)RequestMessageData)->PldmHeader.InstanceId = mInstanceId;
    }
    CopyMem(ReturnData, RequestMessageData, RequestFormatSize);
    ReturnData = (VOID *)((UINT8 *)ReturnData + RequestFormatSize);
    CopyMem(ReturnData, RequestBuffer, MessageSize);
  }

  mInstanceId++;
  mInstanceId = mInstanceId & 0x1F;

  DEBUG ((DEBUG_INFO,"PldmTransferFlagAndData Exit\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BiosPldmToMctpData (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize,
  IN OUT VOID   *ResponseBuffer,
  IN OUT UINT32 *ResponseBufferSize,
  IN     UINT8  PldmType,
  IN     UINT8  CommandCodeValue
)
{
  EFI_STATUS            Status;
  VOID                  *RequestMessage, *ResponseMassage, *ResponseRequestBuffer;
  MCTP_PLDM_MSG         *MctpRequestPldmMsg;
  UINT32                PldmTotalSize, ResponseRequestBufferSize;
  DASH_MCTP_IO_PROTOCOL *Mctp;

  Status = gBS->LocateProtocol (
              &gDashMctpIoProtocolGuid,
              NULL,
              (VOID **) &Mctp);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"Locate MCTP IO protocol failed.\n"));
    return Status;
  }

  PldmTotalSize = 0;
  ResponseRequestBufferSize = 0;

  if (((PldmType == DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION) && (CommandCodeValue == DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE)) ||
      ((PldmType == DASH_PLDM_TYPE_SMBIOS) && (CommandCodeValue == DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE)) ||
      ((PldmType == DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION) && (CommandCodeValue == DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE))){
    PldmTransferFlagAndData(RequestBuffer, RequestBufferSize, (VOID **)&ResponseRequestBuffer, &ResponseRequestBufferSize, CommandCodeValue);

    PldmTotalSize = ResponseRequestBufferSize + sizeof(MCTP_MSG);
    RequestMessage = NULL;
    RequestMessage = AllocateZeroPool(PldmTotalSize);
    CopyMem ((UINT8 *)RequestMessage + sizeof (MCTP_MSG), ResponseRequestBuffer, ResponseRequestBufferSize);
  } else {
    ((DASH_PLDM_HEADER *)RequestBuffer)->InstanceId = mInstanceId;
    mInstanceId++;
    mInstanceId = mInstanceId & 0x1F;

    PldmTotalSize = RequestBufferSize + sizeof(MCTP_PLDM_MSG);
    RequestMessage = AllocateZeroPool(PldmTotalSize);
    CopyMem ((UINT8 *)RequestMessage + sizeof (MCTP_PLDM_MSG), RequestBuffer, RequestBufferSize);
    MctpRequestPldmMsg = RequestMessage;
    MctpRequestPldmMsg->MsgType.MctpMsgType = MCTP_PLDM_TYPE;
  }

  *ResponseBufferSize += (sizeof(MCTP_PLDM_MSG) + sizeof(DASH_PLDM_HEADER));
  ResponseMassage = NULL;
  ResponseMassage = AllocateZeroPool((UINTN)*ResponseBufferSize);

  Status = Mctp->SendMessage (
            Mctp,
            PldmTotalSize,
            (CONST MCTP_MSG *)RequestMessage,
            ResponseBufferSize,
            (MCTP_MSG *)ResponseMassage);

  if (!EFI_ERROR (Status)) {
    ResponseMassage = (VOID *)((UINT8 *)ResponseMassage + sizeof(MCTP_PLDM_MSG) + sizeof(DASH_PLDM_HEADER));
    *ResponseBufferSize -= (sizeof(MCTP_PLDM_MSG) + sizeof(DASH_PLDM_HEADER));
    CopyMem (ResponseBuffer, ResponseMassage, *ResponseBufferSize);
  }

  return Status;
}

/**
  Get Smbios Structure Table Meta Data from the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing SMBIOS Structure Table Meta
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureTableMetaData (
  AMD_DASH_PLDM_PROTOCOL                                        *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
  )
{
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT Format;
  EFI_STATUS Status;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmGetSmbiosStructureTableMetaData!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, Buffer, BufferSize, DASH_PLDM_TYPE_SMBIOS, DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE);

  return Status;
}

/**
  Set Smbios Structure Table Meta Data to the management controller

  @param        This             Pointer to AMD_DASH_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmSetSmbiosStructureTableMetaData (
  AMD_DASH_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry;
  UINT32 Crc32;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT ResponseFormat;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmSetSmbiosStructureTableMetaData!\n"));

  ResponseFormat.CompletionCode = DASH_PLDM_COMPLETION_CODE_SUCCESS;

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
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE;

  Format.SmbiosMajorVersion = SmbiosEntry->MajorVersion;
  Format.SmbiosMinorVersion = SmbiosEntry->MinorVersion;
  Format.MaximumStructureSize = SmbiosEntry->MaxStructureSize;
  Format.SmbiosStructureTableLength = SmbiosEntry->TableLength;
  Format.NumberOfSmbiosStructures = SmbiosEntry->NumberOfSmbiosStructures;
  Format.SmbiosStructureTableIntegrityChecksum = Crc32;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_SMBIOS, DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_METADATA_COMMAND_CODE);

  return Status;
}

/**
  Get Smbios table from the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing the Smbios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureTable (
  AMD_DASH_PLDM_PROTOCOL                               *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
  )
{
  EFI_STATUS Status;
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmGetSmbiosStructureTable!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = DASH_PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, Buffer, BufferSize, DASH_PLDM_TYPE_SMBIOS, DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE);

  return Status;
}

/**
  Set Smbios table to the management controller

  @param        This             Pointer to AMD_DASH_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmSetSmbiosStructureTable (
  AMD_DASH_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry;
  UINT32 PaddingSize;
  UINT8 *PointerToPadding;
  UINT8 *PointerToChecksum;
  UINT32 Crc32;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT *Format;
  DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;
  EFI_SMBIOS_TABLE_HEADER *Record;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  EFI_SMBIOS_PROTOCOL *Smbios;

  DEBUG ((DEBUG_INFO, "Enter PldmSetSmbiosStructureTable!\n"));

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
  RequestBufferSize = (UINT32) sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT) + SmbiosEntry->TableLength + PaddingSize + 4;
  RequestBuffer = NULL;
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in smbios tables
  CopyMem ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT), (VOID *)(UINTN) SmbiosEntry->TableAddress, SmbiosEntry->TableLength);

  // Fill in padding
  PointerToPadding = (UINT8 *) RequestBuffer + sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT) + SmbiosEntry->TableLength;
  ZeroMem (PointerToPadding, PaddingSize);

  // Fill in checksum
  gBS->CalculateCrc32 ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_REQUEST_FORMAT), SmbiosEntry->TableLength + PaddingSize, &Crc32);
  PointerToChecksum = PointerToPadding + PaddingSize;
  CopyMem (PointerToChecksum, &Crc32, 4);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = DASH_PLDM_TYPE_SMBIOS;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START_AND_END;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_SMBIOS, DASH_PLDM_SET_SMBIOS_STRUCTURE_TABLE_COMMAND_CODE);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get Smbios table by type from the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing the Smbios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureByType (
  AMD_DASH_PLDM_PROTOCOL                               *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize,
  UINT8                                           TypeId,
  UINT16                                          StructureInstanceId
  )
{
  EFI_STATUS Status;
  DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmGetSmbiosStructureByType!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_SMBIOS;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = DASH_PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;
  Format.Type = TypeId;
  Format.StructureInstanceId = StructureInstanceId;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_REQUEST_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, Buffer, BufferSize, DASH_PLDM_TYPE_SMBIOS, DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_COMMAND_CODE);

  return Status;
}

/**
  Get bios table from the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE or PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE
  @param [out]  Buffer           Buffer storing the bios table
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmGetBiosTable (
  AMD_DASH_PLDM_PROTOCOL                   *This,
  UINT8                               TableType,
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
  )
{
  EFI_STATUS Status;
  DASH_PLDM_GET_BIOS_TABLE_REQUEST_FORMAT Format;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmGetBiosTable!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_BIOS_TABLE_COMMAND_CODE;
  Format.DataTransferHandle = 0;
  Format.TransferOperationFlag = DASH_PLDM_TRANSFER_OPERATION_FLAG_GET_FIRST_PART;
  Format.TableType = TableType;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_BIOS_TABLE_REQUEST_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, Buffer, BufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_GET_BIOS_TABLE_COMMAND_CODE);

  return Status;
}

/**
  Set bios table to the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [in]   Buffer           Buffer storing the bios table
  @param [in]   BufferSize       Number of bytes of the bios table

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmSetBiosTable (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
  )
{
  EFI_STATUS Status;
  DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT *Format;
  DASH_PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmSetBiosTable!\n"));

  RequestBufferSize = (*BufferSize) + sizeof (DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT);
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in bios table
  CopyMem ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_SET_BIOS_TABLE_REQUEST_FORMAT), Buffer, *BufferSize);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START_AND_END;
  Format->TableType = TableType;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_SET_BIOS_TABLE_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_SET_BIOS_TABLE_COMMAND_CODE);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get one Bios Table Tag from the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [out]  TableTag         Pointer to Bios Table Tag

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmGetBiosTableTags (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
  )
{
  EFI_STATUS Status;
  DASH_PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT Format;
  DASH_PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmGetBiosTableTags!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_BIOS_TABLE_TAGS_COMMAND_CODE;
  Format.NumberOfTables = 1;
  Format.TableType = TableType;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_BIOS_TABLE_TAGS_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_GET_BIOS_TABLE_TAGS_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_GET_BIOS_TABLE_TAGS_COMMAND_CODE);

  if (ResponseFormat.CompletionCode == DASH_PLDM_COMPLETION_CODE_SUCCESS) {
    if (TableType == DASH_PLDM_BIOS_STRING_TABLE) {
      *TableTag = ResponseFormat.TableTag1;
    } else if (TableType == DASH_PLDM_BIOS_ATTRIBUTE_TABLE) {
      *TableTag = ResponseFormat.TableTag2;
    } else if (TableType == DASH_PLDM_BIOS_ATTRIBUTE_VALUE_TABLE) {
      *TableTag = ResponseFormat.TableTag3;
    }
  }

  return Status;
}

/**
  Set one Bios Table Tag to the management controller

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [in]   TableType        PLDM_BIOS_STRING_TABLE or PLDM_BIOS_ATTRIBUTE_TABLE or PLDM_BIOS_ATTRIBUTE_VALUE_TABLE
  @param [in]   TableTag         Bios Table Tag

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmSetBiosTableTags (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
  )
{
  EFI_STATUS Status;
  DASH_PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT Format;
  DASH_PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmSetBiosTableTags!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_SET_BIOS_TABLE_TAGS_COMMAND_CODE;
  Format.NumberOfTables = 1;
  Format.TableType = TableType;
  Format.TableTag = TableTag;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_SET_BIOS_TABLE_TAGS_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_SET_BIOS_TABLE_TAGS_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_SET_BIOS_TABLE_TAGS_COMMAND_CODE);

  return Status;
}

/**
  Accept BIOS Attribute Pending Values

  @param [in]   This                         Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [in]   NumberOfAttributeHandles     Number of BIOS Attribute Handles with Pending Values to be accepted
                                             If all the pending attribute values are rejected, the NumberOfAttributeHandles shall be set to 0
  @param [in]   AttributeHandles             Array of Attribute Handles

  @retval EFI_SUCCESS                        Successful
  @return Other values                       Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmAcceptBiosAttributesPendingValues (
  AMD_DASH_PLDM_PROTOCOL *This,
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
  DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT *Format;
  DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmAcceptBiosAttributesPendingValues!\n"));

  // padding requirement (0 or 2 bytes)
  if ((NumberOfAttributeHandles % 2) == 1) {
    PaddingSize = 2;
  } else {
    PaddingSize = 0;
  }

  // The structure of BIOSAttributesHandles is defined in Spec DSP0247 Table 38.
  // BIOSAttributesHandlesSize = NumberOfAttributeHandles (2 bytes) + AttributeHandles (2 * NumberOfAttributeHandles) + Pad (0 or 2 bytes) + CheckSum (4 bytes)
  BIOSAttributesHandlesSize = 2 + 2 * NumberOfAttributeHandles + PaddingSize + 4;
  RequestBufferSize = (UINT32) sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + BIOSAttributesHandlesSize;
  RequestBuffer = NULL;
  RequestBuffer = AllocatePool (RequestBufferSize);
  if (RequestBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorOut;
  }

  // Fill in NumberOfAttributeHandles
  CopyMem ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT), (VOID *) &NumberOfAttributeHandles, 2);

  // Fill in AttributeHandles
  CopyMem ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + 2, (VOID *) AttributeHandles, (2 * NumberOfAttributeHandles));

  // Fill in padding
  PointerToPadding = (UINT8 *) RequestBuffer + sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT) + 2 + (2 * NumberOfAttributeHandles);
  ZeroMem (PointerToPadding, PaddingSize);

  // Fill in checksum
  gBS->CalculateCrc32 ((UINT8 *) RequestBuffer + sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_REQUEST_FORMAT), BIOSAttributesHandlesSize - 4, &Crc32);
  PointerToChecksum = PointerToPadding + PaddingSize;
  CopyMem (PointerToChecksum, &Crc32, 4);

  Format = RequestBuffer;
  Format->PldmHeader.InstanceId = 0;
  Format->PldmHeader.Reserved = 0;
  Format->PldmHeader.DatagramBit = 0;
  Format->PldmHeader.RequestBit = 1;
  Format->PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format->PldmHeader.HeaderVersion = 0;
  Format->PldmHeader.PldmCommandCode = DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE;
  Format->DataTransferHandle = 0;
  Format->TransferFlag = DASH_PLDM_TRANSFER_FLAG_START_AND_END;

  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_ACCEPT_BIOS_ATTRIBUTES_PENDING_VALUES_COMMAND_CODE);

ErrorOut:
  if (RequestBuffer != NULL) {
    FreePool (RequestBuffer);
  }

  return Status;
}

/**
  Get Date Time from the management controller.

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL
  @param [out]  Buffer           Buffer storing Date Time information
  @param [in, out] BufferSize    Input: Number of bytes allocated by the caller
                                 Output: Number of bytes needed

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmBiosGetDateTime (
  AMD_DASH_PLDM_PROTOCOL                  *This,
  DASH_PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
  )
{
  DASH_PLDM_GET_DATE_TIME_REQUEST_FORMAT Format;
  EFI_STATUS Status;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmBiosGetDateTime!\n"));

  Format.PldmHeader.InstanceId = 0;
  Format.PldmHeader.Reserved = 0;
  Format.PldmHeader.DatagramBit = 0;
  Format.PldmHeader.RequestBit = 1;
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_GET_DATE_TIME_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_GET_DATE_TIME_REQUEST_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, Buffer, BufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_GET_DATE_TIME_COMMAND_CODE);

  return Status;
}

/**
  Set Date Time to the management controller.

  @param [in]   This             Pointer to AMD_DASH_PLDM_PROTOCOL

  @retval EFI_SUCCESS            Successful
  @return Other values           Unsuccessful

**/
EFI_STATUS
EFIAPI
DashPldmBiosSetDateTime (
  AMD_DASH_PLDM_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  EFI_TIME Time;
  DASH_PLDM_SET_DATE_TIME_REQUEST_FORMAT Format;
  DASH_PLDM_SET_DATE_TIME_RESPONSE_FORMAT ResponseFormat;
  VOID *RequestBuffer;
  UINT32 RequestBufferSize;
  VOID *ResponseBuffer;
  UINT32 ResponseBufferSize;

  DEBUG ((DEBUG_INFO, "Enter PldmBiosSetDateTime!\n"));

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
  Format.PldmHeader.PldmType = DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION;
  Format.PldmHeader.HeaderVersion = 0;
  Format.PldmHeader.PldmCommandCode = DASH_PLDM_SET_DATE_TIME_COMMAND_CODE;

  RequestBuffer = &Format;
  RequestBufferSize = sizeof (DASH_PLDM_SET_DATE_TIME_REQUEST_FORMAT);
  ResponseBuffer = &ResponseFormat;
  ResponseBufferSize = sizeof (DASH_PLDM_SET_DATE_TIME_RESPONSE_FORMAT);

  Status = BiosPldmToMctpData (RequestBuffer, RequestBufferSize, ResponseBuffer, &ResponseBufferSize, DASH_PLDM_TYPE_BIOS_CONTROL_AND_CONFIGURATION, DASH_PLDM_SET_DATE_TIME_COMMAND_CODE);

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
DashPldmBaseEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            mHandle;
  COMMON_SETUP_OPTIONS  CommonSetupOptions;

  DEBUG ((DEBUG_INFO, "DashPldmBaseEntry Entry.\n"));

  Status = GetCommonSetupOptions (&CommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[DashPldmBaseEntry] Get Dash variable fail.\n"));
    return Status;
  }

  if (CommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO, "[DashPldmBaseEntry] Dash variable is Disabled.\n"));
    return Status;
  }

  mInstanceId = 0;
  mHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &mHandle,
                  &gAmdDashPldmProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPldmFun
                  );

  DEBUG ((DEBUG_INFO,"DashPldmBaseEntry Exit.\n"));
  return Status;
}
