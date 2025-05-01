/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#include "DashMctpSmbusDxe.h"

EFI_SMBUS_HC_PROTOCOL *mSmBusHc;
EFI_SMBUS_DEVICE_ADDRESS mSmbusDeviceAddr;
DASH_MCTP_IO_PROTOCOL mDashMctpIoProtocol;

/**
  @brief This function initial of Asf controller.

  @return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AsfControllerInit(VOID)
{
  DEBUG ((DEBUG_INFO, "AsfControllerInit Entry!!\n"));

  // 1. Program SMBus slave address
  DashSmbusIoWrite(ASF_LISTEN_ADR, MCTP_SOURCE_SLAVE_ADDR);
  DashSmbusIoWrite(ASF_REMOTECTRL_ADR, (MCTP_SOURCE_SLAVE_ADDR & 0xFE));

  // 2. Program PECEnable and PECAppend accordingly to enable/ disable PEC checking and generation
  DashSmbusIoWrite(ASF_HOST_CONTROL, BIT7); // PEC Enable

  // 3. Clear all status bit in ASF_0x00 and ASF_0x0D
  DashSmbusIoWrite(ASF_HOST_STATUS, 0xFE);
  DashSmbusIoWrite(ASF_SLAVE_STATUS, 0x0F);

  // 4. Clear ASF received command status bits in ASF_0x0A
  DashSmbusIoWrite(ASF_STATUS, 0x7F);

  DEBUG ((DEBUG_INFO, "AsfControllerInit Exit!!\n"));
  return EFI_SUCCESS;
}

/**
  @brief This function will change ASF mode to master or slave.

  @param[in] SetToMasterMode    - Mode change.

  @return EFI_STATUS
**/
EFI_STATUS
ChangeAsfSmbusModeToMaster(
  BOOLEAN   SetToMasterMode
)
{
  UINT8 AsfHostStatus;
  UINT8 AsfStatus;
  UINT8 SemaphoreStatus;

  if (SetToMasterMode) {
    // Check HostBusy and SlaveBusy are idle
    do{
      AsfHostStatus = DashSmbusIoRead(ASF_HOST_STATUS);
      AsfStatus = DashSmbusIoRead(ASF_STATUS);
    }while((AsfHostStatus & BIT0) || (AsfStatus & BIT7));

    DashAsfMasterEnable(TRUE);
    DashResetAsfSlave();

    //
    // Read EcSemaphre to check HostSemaphore
    //
    do{
      SemaphoreStatus = DashSmbusIoRead(ASF_SEMAPHORE);
    }while(SemaphoreStatus & BIT2);

    SemaphoreStatus |= BIT0;
    DashSmbusIoWrite(ASF_SEMAPHORE, SemaphoreStatus);
  } else {
    //
    // Clear HostSemaphore
    //
    SemaphoreStatus = DashSmbusIoRead(ASF_SEMAPHORE);
    SemaphoreStatus |= BIT1;
    DashSmbusIoWrite(ASF_SEMAPHORE, SemaphoreStatus);

    DashAsfMasterEnable(FALSE);
  }

  return EFI_SUCCESS;
}

/**
  @brief This function calculates the number of SMBus blocks required to carry
         a MCTP message.

  @param[in] MessageSize      - The number of SMBus blocks carry the MCTP message.
  @param[out] LastSize        - Returned indicates the size of the final SMBus block.

  @return PacketCounter
**/
UINTN
MctpPackageCounter (
  IN UINT32 MessageSize,
  OUT UINTN *LastSize
)
{

  UINTN PackageCounters = 0;

  MessageSize = MessageSize - MCTP_MSG_SIZE;

  while (MessageSize >= MCTP_SMBUS_TRANSFER_DATA_LENGTH) {
    PackageCounters++;
    MessageSize = MessageSize - MCTP_SMBUS_TRANSFER_DATA_LENGTH;
  }

  if (MessageSize != 0){
    PackageCounters++;
  } else {
    MessageSize = MCTP_SMBUS_TRANSFER_DATA_LENGTH;
  }
  *LastSize = MessageSize;

  DEBUG ((DEBUG_INFO, "MctpPackageCounter MessageSize = 0x%x \n", *LastSize));
  DEBUG ((DEBUG_INFO, "MctpPackageCounter PackageCounters = 0x%x \n", PackageCounters));
  return PackageCounters;
}

/**
  @brief This function is package the MCTP Message that got from Smbus.

  @param[in] MctpMessage  - A pointer to MCTP_MSG.
  @param[in] Buffer       - A pointer to MCTP_MSG.
  @param[in] BufferSize   - Size of MCTP_MSG.
  @param[in] TotalSize    - A pointer to TotalSize.

  @return BOOLEAN
**/
BOOLEAN
PackageGetMctpMessage (
  IN MCTP_MSG *MctpMessage,
  IN MCTP_MSG *Buffer,
  IN UINTN    BufferSize,
  IN UINTN    *TotalSize
)
{
  MCTP_MSG  *BufferData, *MctpMessageData;
  BOOLEAN   IsEndPackage = FALSE;

  BufferData = Buffer;
  MctpMessageData = MctpMessage;

  //
  // Single Package
  //
  if(IsMctpSingle(Buffer)) {
    IsEndPackage = TRUE;
    CopyMem (MctpMessageData, BufferData, BufferSize);
    *TotalSize = BufferSize;
    return IsEndPackage;
  }

  //
  // Start Package
  //
  if(IsMctpSom (Buffer)) {
    IsEndPackage = FALSE;
    CopyMem (MctpMessageData, BufferData, BufferSize);
    *TotalSize = BufferSize;
    return IsEndPackage;
  }

  //
  // Ene Package
  //
  if(IsMctpEom (Buffer)) {
    IsEndPackage = TRUE;
  }

  //
  // Middle Package
  //
  BufferData = (MCTP_MSG *)((UINTN)BufferData + MCTP_MSG_SIZE);
  MctpMessageData = (MCTP_MSG *)((UINTN)MctpMessageData + ((UINTN)*TotalSize));

  CopyMem (MctpMessageData, BufferData, (BufferSize - MCTP_MSG_SIZE));
  *TotalSize =+ (BufferSize - MCTP_MSG_SIZE);

  return IsEndPackage;
}

/**
  @brief This function is package the MCTP Message that will send.

  @param[in] MctpMessage      - A pointer to MCTP_MSG.
  @param[in] MctpMessageSize  - The size of the MCTP message.
  @param[in] Buffer           - A pointer to MCTP_MSG.
  @param[out] BufferSize      - Size of MCTP_MSG.
  @param[in] CurrentCount     - Current MCTP_MSG number.
  @param[in] MaxCount         - Maximal MCTP_MSG number.

  @return EFI_STATUS
**/
EFI_STATUS
PackageSendMctpMessage (
  IN  CONST MCTP_MSG  *MctpMessage,
  IN  UINT32          MctpMessageSize,
  IN  MCTP_MSG        *Buffer,
  OUT UINTN           *BufferSize,
  IN  UINTN           CurrentCount,
  IN  UINTN           MaxCount
)
{
  MCTP_MSG  *MctpMessageData, *BufferData;
  UINTN     Value;

  MctpMessageData = (MCTP_MSG *)MctpMessage;
  BufferData = Buffer;

  ZeroMem (BufferData, MCTP_SMBUS_BLOCK_LENGTH);

  MctpMessageData->SlaveAddress = MCTP_SOURCE_SLAVE_ADDR;
  MctpMessageData->MctpVersion = MCTP_MSG_HDR_VER_1;
  MctpMessageData->DestEid = MCTP_EID_VALUE;
  MctpMessageData->SrcEid = MCTP_EID_VALUE - 1;
  MctpMessageData->Flags = 0xC0;    // Workaround for sometimes send/get Mctp message fail.
  MctpMessageData->Flags = (MctpMessageData->Flags) | MCTP_MSG_TAG_OWNER;

  if (MaxCount == 1) {
    MctpMessageData->Flags = (MctpMessageData->Flags) | MCTP_MSG_SOM_EOM;
  } else if (CurrentCount == 1){
    MctpMessageData->Flags = (MctpMessageData->Flags) | MCTP_MSG_SOM;
  } else if (CurrentCount == MaxCount) {
    MctpMessageData->Flags = (MctpMessageData->Flags) | MCTP_MSG_EOM;
  }

  //
  // Single Packet
  //
  if (MaxCount == 1) {
    MctpPackageCounter (MctpMessageSize, &Value);
    CopyMem (BufferData, MctpMessageData, (Value + MCTP_MSG_SIZE));
    *BufferSize = (Value + MCTP_MSG_SIZE);

    return EFI_SUCCESS;
  }

  //
  // Start Packet.
  //
  CopyMem(BufferData, MctpMessageData, MCTP_MSG_SIZE);
  MctpMessageData = (MCTP_MSG *)((UINT8 *)MctpMessageData + MCTP_MSG_SIZE);
  BufferData = (MCTP_MSG *)((UINT8 *)BufferData + MCTP_MSG_SIZE);

  if (CurrentCount == 1) {
    CopyMem (BufferData, MctpMessageData, MCTP_SMBUS_TRANSFER_DATA_LENGTH);
    *BufferSize = MCTP_SMBUS_BLOCK_LENGTH;

    return EFI_SUCCESS;
  }

  MctpMessageData = (MCTP_MSG *)((UINT8 *)MctpMessageData + ((CurrentCount - 1) * MCTP_SMBUS_TRANSFER_DATA_LENGTH));

  //
  // End Packet
  //
  if (CurrentCount == MaxCount) {
    MctpPackageCounter (MctpMessageSize, &Value);
    CopyMem (BufferData, MctpMessageData, Value);
    *BufferSize = (Value + MCTP_MSG_SIZE);

    return EFI_SUCCESS;
  }

  //
  // Middle Packet
  //
  CopyMem (BufferData, MctpMessageData, MCTP_SMBUS_TRANSFER_DATA_LENGTH);
  *BufferSize = MCTP_SMBUS_BLOCK_LENGTH;

  return EFI_SUCCESS;
}

/**
  @brief This function is get MCTP Package follow Management Conponent Transport protocol(MCTP).
  
  @param[in] This             - A pointer to DASH_MCTP_IO_PROTOCOL instance.
  @param[out] ReplyDataSize   - The actual size of the reply.
  @param[out] ReplyData       - Non-NULL, the points to a buffer that will hold the reply data.
                                NULL, no reply data.
  @return EFI_STATUS
**/
EFI_STATUS
GetMctpMessage (
  IN DASH_MCTP_IO_PROTOCOL  *This,
  OUT UINT32                *ReplyDataSize,
  OUT MCTP_MSG              *ReplyData
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_STATUS  StatusFreePool = EFI_SUCCESS;
  UINTN       BufferSize = 0;
  UINTN       TotalSize = 0;
  UINTN       AllocateSize = 69;    // Dash FW payload size should be 69 Bytes define in RealManage Implementation Guide
  MCTP_MSG    *Buffer, *MctpMSG;
  BOOLEAN     Finished = FALSE;

  DEBUG ((DEBUG_INFO, "GetMctpMessage: Entry\n"));

  MctpMSG = AllocateZeroPool(AllocateSize);
  Buffer = AllocateZeroPool(AllocateSize);
  if (Buffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "GetMctpMessage fail!! (%r)\n", Status));
    return Status;
  }

  while (Finished != TRUE){
    Status = DashWaitForSmbusBuf();

    if (EFI_ERROR(Status)) {
      *ReplyDataSize = 0;
      goto ErrorReturn;
    } else {
      Status = DashListenModeRead((UINT8 *)&BufferSize, (UINT8 *)Buffer);
      if (EFI_ERROR(Status)) {
        *ReplyDataSize = 0;
        goto ErrorReturn;
      }
    }
    Finished = PackageGetMctpMessage (MctpMSG, Buffer, BufferSize, &TotalSize);
  }

  if(*ReplyDataSize < (UINT32)TotalSize) {
    *ReplyDataSize = (UINT32)TotalSize;
    Status =  EFI_BUFFER_TOO_SMALL;
  } else {
    *ReplyDataSize = (UINT32)TotalSize;
    CopyMem(ReplyData, MctpMSG, TotalSize);
  }

ErrorReturn:
      StatusFreePool = gBS->FreePool((VOID *)Buffer);
      if (EFI_ERROR(StatusFreePool)) {
        DEBUG ((DEBUG_INFO, "GetMctpMessage fail: Free Buffer Pool Fail!!! (%r)\n", StatusFreePool));
        ASSERT_EFI_ERROR (StatusFreePool);
      }
      StatusFreePool = gBS->FreePool((VOID *)MctpMSG);
      if (EFI_ERROR(StatusFreePool)) {
        DEBUG ((DEBUG_INFO, "GetMctpMessage fail: Free MctpMSG Pool Fail!!! (%r)\n", StatusFreePool));
        ASSERT_EFI_ERROR (StatusFreePool);
      }

      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "GetMctpMessage fail!!! (%r)\n", Status));
      }

  DEBUG ((DEBUG_INFO, "GetMctpMessage: Exit\n"));
  return Status;
}

/**
  @brief This function is sned MCTP Package follow Management Conponent Transport protocol(MCTP).

  @param[in] This             - A pointer to DASH_MCTP_IO_PROTOCOL instance.
  @param[in] MessageDataSize  - Size of the MCTP message, including header and data, in bytes.
  @param[in] MessageData      - Pointer to message.
  @param[out] ReplyDataSize   - On entry, size of the buffer which will hold the reply.
                                On exit, the actual size of the reply.
  @param[out] ReplyData       - If non-NULL, points to a buffer that will hold the reply data from
                                the ASF SmBus controller.
                                If NULL, no reply.

  @return EFI_STATUS
**/
EFI_STATUS
SendMctpMessage (
  IN DASH_MCTP_IO_PROTOCOL  *This,
  IN UINT32                 MessageDataSize,
  IN CONST MCTP_MSG         *MessageData,
  OUT UINT32                *ReplyDataSize,
  OUT MCTP_MSG              *ReplyData
)
{
  UINTN       LoopCounters;
  UINTN       Counters;
  EFI_STATUS  Status;
  UINTN       BufferSize;
  MCTP_MSG    *BufferData;
  BOOLEAN     PecCheck = 1;

  DEBUG ((DEBUG_INFO, "SendMctpMessage: Entry\n"));

  if ((MessageDataSize == 0) || (MessageData == NULL) ) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "SendMctpMessage Fail!!! (%r)\n", Status));
    return Status;
  }

  BufferData = (MCTP_MSG *)AllocateZeroPool((UINTN)MCTP_SMBUS_BLOCK_LENGTH);
  if (BufferData == NULL) {
    Status = EFI_BAD_BUFFER_SIZE;
    DEBUG ((DEBUG_INFO, "SendMctpMessage Fail!!! (%r)\n", Status));
    return Status;
  }

  Counters = MctpPackageCounter (MessageDataSize, NULL);

  for (LoopCounters = 1; LoopCounters <= Counters; LoopCounters++) {
    ChangeAsfSmbusModeToMaster(TRUE);

    PackageSendMctpMessage (MessageData, MessageDataSize, BufferData, &BufferSize, LoopCounters, Counters);
    Status = mSmBusHc->Execute (
                       mSmBusHc,
                       mSmbusDeviceAddr,
                       MCTP_SMBUS_COMMAND_CODE,
                       EfiSmbusWriteBlock,
                       PecCheck,
                       (UINTN *)&BufferSize,
                       (VOID *)BufferData);

    ChangeAsfSmbusModeToMaster(FALSE);
    gBS->Stall(100000);   // Workaround for sometimes send/get Mctp message fail.
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "SendMctpMessage Fail!!! (%r)\n", Status));
      return Status;
    }
  }

  Status = gBS->FreePool((VOID *)BufferData);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SendMctpMessage Fail!!! (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  if (*ReplyDataSize !=0 || ReplyData !=NULL) {
    Status = GetMctpMessage(This, ReplyDataSize, ReplyData);
    DEBUG ((DEBUG_INFO, "MctpSendMessage: GetMessage Status: %r !!!\n", Status));
  }

  DEBUG ((DEBUG_INFO, "SendMctpMessage: Exit\n"));
  return Status;
}

/**
  @brief This function Install DashMctpIoProtocol.

  @return EFI_STATUS
**/
EFI_STATUS
InstallMctpProtocol (VOID)
{
  EFI_STATUS Status;
  EFI_HANDLE Handle;

  DEBUG ((DEBUG_INFO, "[InstallMctpProtocol] Install MctpIoProtocol \n"));

  Handle                          = NULL;
  mDashMctpIoProtocol.Size        = sizeof(DASH_MCTP_IO_PROTOCOL);
  mDashMctpIoProtocol.SendMessage = SendMctpMessage;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gDashMctpIoProtocolGuid,
                  &mDashMctpIoProtocol,
                  NULL);

  DEBUG ((DEBUG_INFO, "[InstallMctpProtocol] Exit (%r) \n", Status));

  return Status;
}

EFI_STATUS
EFIAPI
DxeEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS            Status;
  SMBUS_INSTANCE        *AsfSmbus;
  COMMON_SETUP_OPTIONS  CommonSetupOptions;

  DEBUG ((DEBUG_INFO, "DashMctpSmbusDxe DxeEntry Entry!!\n"));

  Status = GetCommonSetupOptions (&CommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"[DashMctpSmbusDxe] Get Dash variable fail.\n"));
    return Status;
  }

  if (CommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO,"[DashMctpSmbusDxe] Dash variable is Disabled.\n"));
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSmbusHcProtocolGuid,
                  NULL,
                  &mSmBusHc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AsfSmbus = SMBUS_INSTANCE_FROM_THIS(mSmBusHc);
  AsfSmbus++;   // ASF SMBUS
  mSmBusHc = (EFI_SMBUS_HC_PROTOCOL *)&AsfSmbus->SmbusController;

  mSmbusDeviceAddr.SmbusDeviceAddress = SMBUS_LIB_SLAVE_ADDRESS(LAN_SMBUS_SLAVE_ADDRESS);
  AsfControllerInit();
  DEBUG ((DEBUG_INFO, "mSmbusDeviceAddr.SmbusDeviceAddress = %x\n", mSmbusDeviceAddr.SmbusDeviceAddress));

  Status = InstallMctpProtocol();
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "DashMctpSmbusDxe DxeEntry Exit!! (%r)\n", Status));

  return Status;
}