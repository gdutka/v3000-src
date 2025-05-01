/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/MpmFuncLib.h>


#define FILECODE LIBRARY_MPMMBOXLIB_MPMMBOXLIB_FILECODE
// #define MPM_COMMAND_THROUGH_MMIO
#define SMN_MPMMMIOPUBLIC_BASE  0xFC00000
#define RETRY_COUNTER  1000000 //1s
#define RETRY_COUNTER_FOR_WIRELESS_KVM  30000000
#define MPM_MBOX_LIB_DBG_VERBOSE

MANAGEABILITY_CFG     mManagebilityCfg;
/**
  Return the MPM MMIO location

  @retval MPM MMIO location, Zero if error occurs
**/
UINT32
GetMpmBarAddr (
  VOID
  )
{
  UINT32 Value32;

  Value32 = 0;
  SmnRegisterRead (0, IOHC_MPM_BASE_ADDR_HI_SMN_ADDR, &Value32);
  //MPM MMIO should be assigned below 4G
  if (Value32 != 0) {
    return 0;
  }
  SmnRegisterRead (0, IOHC_MPM_BASE_ADDR_LO_SMN_ADDR, &Value32);
  Value32 &= ~(BIT0 + BIT1);
  return Value32;
}

UINT32
GetMboxRegOffset (
  UINT32 RegId
  )
{
  UINT32 mRegOffsetTbl[3] = {
   0x10500,// MPM_C2PMSG_0
   0x10560,// MPM_C2PMSG_24
   0x10564,// MPM_C2PMSG_25
  };

  return mRegOffsetTbl[RegId];
}

#ifdef MPM_COMMAND_THROUGH_MMIO
/**
 * Description: This routine used to send command to MPM
 *
 * Input:
 * BiosMpmMessage - point to the structure, which will be copy to C2P registers
 *
 * Output:
 * BiosMpmMessage - the structure will copy with the contents from C2P register which response by MPM
 *
 *  @retval if not AGESA_SUCCESS, some error occurs
 */
EFI_STATUS
SendMpmCommand (
  IN OUT BIOS_MPM_MBOX_COMMON  *BiosMpmMessage
  )
{
  volatile BIOS_MPM_MBOX_REG0 *MboxReg0;
  BIOS_MPM_MBOX_REG0 MboxReg0MemCopy;
  volatile UINT8  *MpmMmioBase;
  volatile UINT32 *MboxReg;
  UINT32 RetryCounter;
  UINT8  Value8;


  IDS_HDT_CONSOLE_PSP_TRACE ("\n>>>SendMpmCommand Reg0:0x%x Reg1:0x%x Reg2:0x%x\n", BiosMpmMessage->Reg0.Value, BiosMpmMessage->Reg1, BiosMpmMessage->Reg2);
  //Get PSP BAR address
  MpmMmioBase = (volatile UINT8  *) (UINTN) GetMpmBarAddr ();
  //Get the offset of MboxReg0
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmMmioBase 0x%x\n", MpmMmioBase);
  if (MpmMmioBase == 0) {
  }
  MboxReg0 = (BIOS_MPM_MBOX_REG0 *) (MpmMmioBase + GetMboxRegOffset (0));
  IDS_HDT_CONSOLE_PSP_TRACE ("Wait till MPM ready for receiving command MMIO 0x%x=0x%x\n", MboxReg0, *MboxReg0);
  RetryCounter = RETRY_COUNTER;
  do {
    //IO delay take approximate 2 us
    LibAmdIoRead (AccessWidth8, 0xEB, (UINT8*)&Value8, NULL);
    if (RetryCounter == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("TimeOut\n");
      return EFI_NOT_READY;
    }
    RetryCounter--;
  } while ( (!MboxReg0->Field.Ready) || (MboxReg0->Value == 0xFFFFFFFF));
  //Fill MboxReg1, MboxReg2
  MboxReg = (volatile UINT32 *) (MpmMmioBase + GetMboxRegOffset (1));
  *MboxReg = BiosMpmMessage->Reg1;

  MboxReg = (volatile UINT32 *) (MpmMmioBase + GetMboxRegOffset (2));
  *MboxReg = BiosMpmMessage->Reg2;

  //Do it on local variable 1st to avoid pontencial racing issue
  MboxReg0MemCopy.Value = BiosMpmMessage->Reg0.Value;
  //Clear Status
  MboxReg0MemCopy.Field.Status = 0;
  //Clear R bit to indicate to MPM that a new command is fired from x86
  MboxReg0MemCopy.Field.Ready = 0;

  MboxReg0 = (BIOS_MPM_MBOX_REG0 *) (MpmMmioBase + GetMboxRegOffset (0));
  IDS_HDT_CONSOLE_PSP_TRACE ("Send Command 0x%x\n", BiosMpmMessage->Reg0.Field.CommandId);
  MboxReg0->Value = MboxReg0MemCopy.Value;

  //Waiting for MPM finish processing the command
  RetryCounter = RETRY_COUNTER;
  do {
    //IO delay take approximate 2 us
    LibAmdIoRead (AccessWidth8, 0xEB, (UINT8*)&Value8, NULL);
    if (RetryCounter == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("TimeOut\n");
      return EFI_TIMEOUT;
    }
    RetryCounter--;
  } while ( (!MboxReg0->Field.Ready) || (MboxReg0->Value == 0xFFFFFFFF));

  //Copy response from C2P register
  MboxReg = (volatile UINT32 *) (MpmMmioBase + GetMboxRegOffset (0));
  BiosMpmMessage->Reg0.Value = *MboxReg;

  MboxReg = (volatile UINT32 *) (MpmMmioBase + GetMboxRegOffset (1));
  BiosMpmMessage->Reg1 = *MboxReg;

  MboxReg = (volatile UINT32 *) (MpmMmioBase + GetMboxRegOffset (2));
  BiosMpmMessage->Reg2 = *MboxReg;

  IDS_HDT_CONSOLE_PSP_TRACE ("<<<Command complete Reg0:0x%x Reg1:0x%x Reg2:0x%x\n\n", BiosMpmMessage->Reg0.Value, BiosMpmMessage->Reg1, BiosMpmMessage->Reg2);
  if (BiosMpmMessage->Reg0.Field.Status != 0) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}
#else
/**
 * Description: This routine used to send command to MPM
 *
 * Input:
 * BiosMpmMessage - point to the structure, which will be copy to C2P registers
 *
 * Output:
 * BiosMpmMessage - the structure will copy with the contents from C2P register which response by MPM
 *
 *  @retval if not AGESA_SUCCESS, some error occurs
 */
EFI_STATUS
SendMpmCommand (
  IN OUT BIOS_MPM_MBOX_COMMON  *BiosMpmMessage
  )
{
  BIOS_MPM_MBOX_REG0 MboxReg0;
  UINT32 RetryCounter;
  UINT8  Value8;
  MPM_PRVIATE_HEAP_DATA_V1 *MpmData;
  EFI_STATUS Status;
  BOOLEAN WlkvmRequestBdcommand = FALSE;


  IDS_HDT_CONSOLE_PSP_TRACE (">>>SendMpmCommand Reg0:0x%x Reg1:0x%x Reg2:0x%x\n", BiosMpmMessage->Reg0.Value, BiosMpmMessage->Reg1, BiosMpmMessage->Reg2);
  Status = LocateMpmPrivateData (&MpmData);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  //Have a MPM command in progress, exit w/o touch any HW register
  if (MpmData->MpmCommandInProgressFlag == MPM_COMMAND_IN_PROGRESS) {
    Status =  EFI_NOT_READY;
    goto Exit;
  }
  MpmData->MpmCommandInProgressFlag = MPM_COMMAND_IN_PROGRESS;

  MboxReg0.Value = 0;
  SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (0), &MboxReg0.Value);
  IDS_HDT_CONSOLE_PSP_TRACE ("Wait till MPM ready for receiving command C2PMSG0=0x%x\n", MboxReg0.Value);
  RetryCounter = RETRY_COUNTER;
  do {
    //IO delay take approximate 2 us
    SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (0), &MboxReg0.Value);
    LibAmdIoRead (AccessWidth8, 0xEB, (UINT8*)&Value8, NULL);
    if (RetryCounter == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("TimeOut\n");
      Status = EFI_TIMEOUT;
      goto Exit;
    }
    RetryCounter--;
  } while ( (!MboxReg0.Field.Ready) || (MboxReg0.Value == 0xFFFFFFFF));
  IDS_HDT_CONSOLE_PSP_TRACE ("Wait time = %d us, Ready = 0x%x\n", 2*(RETRY_COUNTER - RetryCounter), MboxReg0.Field.Ready);
  //Fill MboxReg1, MboxReg2
  SmnRegisterWrite (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (1), &BiosMpmMessage->Reg1, 0);
  SmnRegisterWrite (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (2), &BiosMpmMessage->Reg2, 0);


  //Do it on local variable 1st
  MboxReg0.Value = BiosMpmMessage->Reg0.Value;
  //Clear Status
  MboxReg0.Field.Status = 0;
  //Clear R bit to indicate to MPM that a new command is fired from x86
  MboxReg0.Field.Ready = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("Send Command 0x%x\n", BiosMpmMessage->Reg0.Field.CommandId);
  SmnRegisterWrite (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (0), &MboxReg0.Value, 0);

  //Waiting for MPM finish processing the command
  IDS_HDT_CONSOLE_PSP_TRACE ("Waiting for MPM finish processing the command\n");
  RetryCounter = RETRY_COUNTER;
  if ((PcdGetBool (PcdMpmWirelessKvmEnable) == TRUE) && (MpmData->WirelessKvmPending) && (BiosMpmMessage->Reg0.Field.CommandId == BIOS_MPM_CMD_BOOT_DONE_MSG)) {
    RetryCounter = RETRY_COUNTER_FOR_WIRELESS_KVM;
    WlkvmRequestBdcommand = TRUE;
    IDS_HDT_CONSOLE_PSP_TRACE ("There is wireless KVM request, set waiting time to 60 seconds\n");
  }
  do {
    //IO delay take approximate 2 us
    SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (0), &MboxReg0.Value);
    LibAmdIoRead (AccessWidth8, 0xEB, (UINT8*)&Value8, NULL);
    if (RetryCounter == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("TimeOut\n");
      Status = EFI_TIMEOUT;
      goto Exit;
    }
    RetryCounter--;
  } while ( (!MboxReg0.Field.Ready) || (MboxReg0.Value == 0xFFFFFFFF));
  if (WlkvmRequestBdcommand == TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Wait time = %d us, Ready = 0x%x\n", 2*(RETRY_COUNTER_FOR_WIRELESS_KVM - RetryCounter), MboxReg0.Field.Ready);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Wait time = %d us, Ready = 0x%x\n", 2*(RETRY_COUNTER - RetryCounter), MboxReg0.Field.Ready);
  }

  //Copy response from C2P register
  SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (0), &BiosMpmMessage->Reg0.Value);

  SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (1), &BiosMpmMessage->Reg1);

  SmnRegisterRead (0, SMN_MPMMMIOPUBLIC_BASE + GetMboxRegOffset (2), &BiosMpmMessage->Reg2);
  Status = EFI_SUCCESS;
Exit:
  IDS_HDT_CONSOLE_PSP_TRACE ("<<<Command complete Reg0:0x%x Reg1:0x%x Reg2:0x%x\n", BiosMpmMessage->Reg0.Value, BiosMpmMessage->Reg1, BiosMpmMessage->Reg2);
  if (BiosMpmMessage->Reg0.Field.Status != 0) {
    Status = EFI_DEVICE_ERROR;
  }
  MpmData->MpmCommandInProgressFlag = MPM_COMMAND_IDLE;
  return Status;
}
#endif
/**
  Work function to get buffer from MPM
  to get the buffer which used for sending BIOS_MPM_CMD_PLDM
  * @param[in]  MpmCmdId  Mpm Command to get buffer
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetBufferOffsetSize (
  IN     UINT8  MpmCmdId,
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;

  if ((BufferOffset == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = MpmCmdId;

  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    *BufferOffset = 0;
    *BufferSize = 0;
    return Status;
  }
  *BufferSize  = BiosMpmMessage.Reg1 & 0xFFFF;
  *BufferOffset = BiosMpmMessage.Reg2;
  IDS_HDT_CONSOLE_PSP_TRACE ("BufferOffset %x BufferSize %x\n", *BufferOffset, *BufferSize);

  return EFI_SUCCESS;
}
/**
  BIOS send command BIOS_MPM_GET_PLDM_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_PLDM
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetPldmBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdGetPldmBufferOffsetSize\n");
  return BiosMpmCmdGetBufferOffsetSize (BIOS_MPM_GET_PLDM_BUFFER_OFFSET_SIZE, BufferOffset, BufferSize);
}

/**
  BIOS send command BIOS_MPM_GET_ALERT_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_ALERT
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetAlertBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdGetAlertBufferOffsetSize\n");
  return BiosMpmCmdGetBufferOffsetSize (BIOS_MPM_GET_ALERT_BUFFER_OFFSET_SIZE, BufferOffset, BufferSize);
}


/**
  BIOS send command BIOS_MPM_GET_TEXT_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_TEXT
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetTextBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdGetTextBufferOffsetSize\n");
  return BiosMpmCmdGetBufferOffsetSize (BIOS_MPM_GET_TEXT_BUFFER_OFFSET_SIZE, BufferOffset, BufferSize);
}


/**
  BIOS send command BIOS_MPM_GET_KVM_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_KVM
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetKvmBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdGetKvmBufferOffsetSize\n");
  return BiosMpmCmdGetBufferOffsetSize (BIOS_MPM_GET_KVM_BUFFER_OFFSET_SIZE, BufferOffset, BufferSize);
}


/**
  BIOS send command BIOS_MPM_CMD_INIT_COMMAND to MPM to get the bit mask of pending tasks and buffer to send NON-PLDM,ASF command
  * @param[in,out]  PendingTask  Point to the address to hold PendingTask bit masks

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdInit (
  IN OUT BIOS_MPM_MBOX_REG0_FIELD *PendingTask,
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdInit\n");

  if (PendingTask == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_INIT_COMMAND;

  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  if (EFI_ERROR (Status)) {
    *BufferOffset = 0;
    *BufferSize = 0;
    return Status;
  }
  *PendingTask = BiosMpmMessage.Reg0.Field;
  *BufferSize  = BiosMpmMessage.Reg1 & 0xFFFF;
  *BufferOffset = BiosMpmMessage.Reg2;
  IDS_HDT_CONSOLE_PSP_TRACE ("PendingTask: 0x%x BufferSize:0x%x BufferOffset:0x%x\n", *PendingTask, *BufferSize, *BufferOffset );
  return EFI_SUCCESS;
}


/**
  BIOS send command BIOS_MPM_CMD_PLDM to MPM to process PLDM message
  * @param[in]   RequestBuffer         Point to the PLDM request message memory buffer,
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the PLDM request message memory buffer, including header
  * @param[out]  ResponseBuffer        Point to the address of PLDM response message memory buffer,
                                       Keep as NULL, if message doesn't require response
                                       This field will be ignored and keep untouched, if response bit is not set by MPM
                                       For message require response, Caller need allocate the buffer prior calling this routine
                                       Callee will copy the content from MPM dedicate mailbox buffer the caller allocate one.
  * @param[out]  ResponseBufferSize    This field will be ignored and keep untouched, if response bit is not set by MPM
                                       On input, Point to the size of ResponseBuffer
                                       On Output, if ResponseBufferSize is smaller than required, update to the required value with
                                       Status EFI_BUFFER_TOO_SMALL

   @retval EFI_STATUS                  0: Success, NonZero Error
**/
//todo  verify especaily the response field
EFI_STATUS
BiosMpmCmdPldmMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize,
  IN OUT VOID   *ResponseBuffer,
  IN OUT UINT32 *ResponseBufferSize
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdPldmMsg\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_PLDM;
  ASSERT (Data->PldmBufferSize != 0);
  ASSERT (Data->PldmBufferOffset != 0);
  ASSERT (Data->PldmBufferAddr != 0);
  ASSERT ((Data->PldmBufferSize & 0xFFFF0000) == 0);
  if ((Data->PldmBufferSize == 0) || (Data->PldmBufferOffset == 0) ||
     (Data->PldmBufferAddr == 0) || ((Data->PldmBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PldmBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (RequestBufferSize > Data->PldmBufferSize) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Pldm buffer too small\n");
    return EFI_INVALID_PARAMETER;
  }
  //Check the PldmBufferOffset to make sure it is within the MpmDram buffer
  if (Data->PldmBufferOffset > MPM_DRAM_TOTAL_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PldmBufferOffset is out of MpmDram buffer\n");
    return EFI_INVALID_PARAMETER;
  }
  BiosMpmMessage.Reg1 = RequestBufferSize;
  BiosMpmMessage.Reg2 = Data->PldmBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->PldmBufferAddr, Data->PldmBufferSize);
  CopyMem ((VOID *) (UINTN) Data->PldmBufferAddr, RequestBuffer, RequestBufferSize);
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("Pldm request buffer dump:\n");
    IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) Data->PldmBufferAddr, RequestBufferSize);
  #endif
  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:0x%x\n");
  //Check if any response for the command
  if (BiosMpmMessage.Reg0.Field.Response) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Response Flag is set\n");
    ASSERT (BiosMpmMessage.Reg1 != 0);
    ASSERT (BiosMpmMessage.Reg2 != 0);
    if ((BiosMpmMessage.Reg1 == 0) || (BiosMpmMessage.Reg2 == 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Response buffer is NULL\n");
      return EFI_UNSUPPORTED;
    }
    #ifdef MPM_MBOX_LIB_DBG_VERBOSE
      IDS_HDT_CONSOLE_PSP_TRACE ("Pldm response buffer dump:\n");
      IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) (Data->MpmDramAddr + BiosMpmMessage.Reg2),  BiosMpmMessage.Reg1);
    #endif
    if (ResponseBuffer == NULL) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Skip copy response, as ResponseBuffer is NULL\n");
      return EFI_SUCCESS;
    }
    if (*ResponseBufferSize < BiosMpmMessage.Reg1) {
      IDS_HDT_CONSOLE_PSP_TRACE ("ResponseBuffer too small %x < %x\n", *ResponseBufferSize, BiosMpmMessage.Reg1);
      *ResponseBufferSize = BiosMpmMessage.Reg1;
      return EFI_BUFFER_TOO_SMALL;
    }
    *ResponseBufferSize = BiosMpmMessage.Reg1;
    CopyMem (ResponseBuffer, (VOID *) (UINTN) (Data->MpmDramAddr + BiosMpmMessage.Reg2), *ResponseBufferSize);
  }
  return EFI_SUCCESS;
}


/**
  BIOS send command BIOS_MPM_CMD_ALERT to MPM to process Alert message
  * @param[in]   RequestBuffer         Point to the Alert request message memory buffer
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the Alert request message memory buffer, including header
   @retval EFI_STATUS                  0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdAlertMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdAlertMsg\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_ALERT;
  ASSERT (Data->AlertBufferSize != 0);
  ASSERT (Data->AlertBufferOffset != 0);
  ASSERT (Data->AlertBufferAddr != 0);
  ASSERT ((Data->AlertBufferSize & 0xFFFF0000) == 0);
  if ((Data->AlertBufferSize == 0) || (Data->AlertBufferOffset == 0) ||
     (Data->AlertBufferAddr == 0) || ((Data->AlertBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("AlertBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (RequestBufferSize > Data->AlertBufferSize) {
    IDS_HDT_CONSOLE_PSP_TRACE ("AlertBuffer too small\n");
    return EFI_INVALID_PARAMETER;
  }
  BiosMpmMessage.Reg1 = RequestBufferSize;
  BiosMpmMessage.Reg2 = Data->AlertBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->AlertBufferAddr, Data->AlertBufferSize);
  CopyMem ((VOID *) (UINTN) Data->AlertBufferAddr, RequestBuffer, RequestBufferSize);
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("AlertMsg request buffer dump:\n");
    IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) Data->AlertBufferAddr, RequestBufferSize);
  #endif
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}


/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_KVM to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateKvm (
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdTerminateKvm\n");

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_TERMINATE_KVM;

  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}


/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_TEXT to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateText (
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdTerminateText\n");

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_TERMINATE_TEXT;

  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}

/**
 * @brief   BIOS send command BIOS_MPM_CMD_QUERY_TEXT to MPM
 *
 * @param Para Point to the address to hold Para
 * @return EFI_STATUS  0: Success, NonZero Error
 */
EFI_STATUS
BiosMpmCmdQueryText (
  IN OUT MBOX_QUERY_TEXT *Para
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdQueryText\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_QUERY_TEXT;
  ASSERT (Data->TextBufferSize != 0);
  ASSERT (Data->TextBufferOffset != 0);
  ASSERT (Data->TextBufferAddr != 0);
  ASSERT ((Data->TextBufferSize & 0xFFFF0000) == 0);
  if ((Data->TextBufferSize == 0) || (Data->TextBufferOffset == 0) ||
     (Data->TextBufferAddr == 0) || ((Data->TextBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (Data->TextBufferSize < sizeof (MBOX_QUERY_TEXT)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("TextBufferSize too small\n");
    return EFI_INVALID_PARAMETER;
  }
  //Check the TextBufferOffset to make sure it is within the MpmDram buffer
  if (Data->TextBufferOffset > MPM_DRAM_TOTAL_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("TextBufferOffset is out of MpmDram buffer\n");
    return EFI_INVALID_PARAMETER;
  }
  BiosMpmMessage.Reg1 = sizeof (MBOX_QUERY_TEXT);
  BiosMpmMessage.Reg2 = Data->TextBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->TextBufferAddr, Data->TextBufferSize);
  CopyMem ((VOID *) (UINTN) Data->TextBufferAddr, Para, sizeof (MBOX_QUERY_TEXT));
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_QUERY_TEXT.INPUT\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDidVid 0x%x\n", Para->DidVid);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrBufOffset 0x%x\n", Para->TcrBufOffset);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrBufSize 0x%x\n", Para->TcrBufSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufOffset 0x%x\n", Para->WlanBufOffset);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufSize 0x%x\n", Para->WlanBufSize);
  #endif
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  if (Status == EFI_SUCCESS) {
    //Command executed successfully, copy back the result.
    CopyMem (Para, (VOID *) (UINTN) (Data->MpmDramAddr + BiosMpmMessage.Reg2), sizeof (MBOX_QUERY_KVM));
    #ifdef MPM_MBOX_LIB_DBG_VERBOSE
      IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_QUERY_TEXT.OUTPUT\n");
      IDS_HDT_CONSOLE_PSP_TRACE ("\tDidVid 0x%x\n", Para->DidVid);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrBufOffset 0x%x\n", Para->TcrBufOffset);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrBufSize 0x%x\n", Para->TcrBufSize);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufOffset 0x%x\n", Para->WlanBufOffset);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufSize 0x%x\n", Para->WlanBufSize);
    #endif
  }
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_QUERY_KVM to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdQueryKvm (
  IN OUT MBOX_QUERY_KVM *Para
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdQueryKvm\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_QUERY_KVM;
  ASSERT (Data->KvmBufferSize != 0);
  ASSERT (Data->KvmBufferOffset != 0);
  ASSERT (Data->KvmBufferAddr != 0);
  ASSERT ((Data->KvmBufferSize & 0xFFFF0000) == 0);
  if ((Data->KvmBufferSize == 0) || (Data->KvmBufferOffset == 0) ||
     (Data->KvmBufferAddr == 0) || ((Data->KvmBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (Data->KvmBufferSize < sizeof (MBOX_QUERY_KVM)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBufferSize too small\n");
    return EFI_INVALID_PARAMETER;
  }
  //Check the KvmBufferOffset to make sure it is within the MpmDram buffer
  if (Data->KvmBufferOffset > MPM_DRAM_TOTAL_SIZE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBufferOffset is out of MpmDram buffer\n");
    return EFI_INVALID_PARAMETER;
  }
  BiosMpmMessage.Reg1 = sizeof (MBOX_QUERY_KVM);
  BiosMpmMessage.Reg2 = Data->KvmBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->KvmBufferAddr, Data->KvmBufferSize);
  CopyMem ((VOID *) (UINTN) Data->KvmBufferAddr, Para, sizeof (MBOX_QUERY_KVM));
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_QUERY_KVM.INPUT\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDidVid 0x%x\n", Para->DidVid);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tKeyboardBufOffset 0x%x\n", Para->KeyboardBufOffset);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tKeyboardBufSize 0x%x\n", Para->KeyboardBufSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tMouseBufOffset 0x%x\n", Para->MouseBufOffset);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tMouseBufSize 0x%x\n", Para->MouseBufSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufOffset 0x%x\n", Para->WlanBufOffset);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufSize 0x%x\n", Para->WlanBufSize);
  #endif
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:0x%x\n", Status);
  if (Status == EFI_SUCCESS) {
    //Command executed successfully, copy back the result.
    CopyMem (Para, (VOID *) (UINTN) (Data->MpmDramAddr + BiosMpmMessage.Reg2), sizeof (MBOX_QUERY_KVM));
    #ifdef MPM_MBOX_LIB_DBG_VERBOSE
      IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_QUERY_KVM.OUTPUT\n");
      IDS_HDT_CONSOLE_PSP_TRACE ("\tDidVid 0x%x\n", Para->DidVid);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tKeyboardBufOffset 0x%x\n", Para->KeyboardBufOffset);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tKeyboardBufSize 0x%x\n", Para->KeyboardBufSize);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tMouseBufOffset 0x%x\n", Para->MouseBufOffset);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tMouseBufSize 0x%x\n", Para->MouseBufSize);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufOffset 0x%x\n", Para->WlanBufOffset);
      IDS_HDT_CONSOLE_PSP_TRACE ("\tWlanBufSize 0x%x\n", Para->WlanBufSize);
    #endif
  }
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_START_TEXT to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdStartText (
  IN OUT MBOX_START_TEXT *Para
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdStartText\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_START_TEXT;
  ASSERT (Data->TextBufferSize != 0);
  ASSERT (Data->TextBufferOffset != 0);
  ASSERT (Data->TextBufferAddr != 0);
  ASSERT ((Data->TextBufferSize & 0xFFFF0000) == 0);
  if ((Data->TextBufferSize == 0) || (Data->TextBufferOffset == 0) ||
     (Data->TextBufferAddr == 0) || ((Data->TextBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("TextBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (Data->TextBufferSize < sizeof (MBOX_START_TEXT)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("TextBufferSize too small\n");
    return EFI_INVALID_PARAMETER;
  }
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_START_TEXT:\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPcieMmioCfgBaseAddr: 0x%x\n", Para->PcieMmioCfgBaseAddr);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tNetCardPciCfgAddr: 0x%x\n", Para->NetCardPciCfgAddr);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDMArEnabled: 0x%x\n", Para->DMArEnabled);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrDmaGpaAddrLo: 0x%x\n", Para->TcrDmaGpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrDmaGpaAddrHi: 0x%x\n", Para->TcrDmaGpaAddrHi);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrDmaHpaAddrLo: 0x%x\n", Para->TcrDmaHpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrDmaHpaAddrHi: 0x%x\n", Para->TcrDmaHpaAddrHi);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tcrMpmC2pMsg2GpaAddrLo: 0x%x\n", Para->TcrMpmC2pMsg2GpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrMpmC2pMsg2GpaAddrHi: 0x%x\n", Para->TcrMpmC2pMsg2GpaAddrHi);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrMpmC2pMsg2HpaAddrLo: 0x%x\n", Para->TcrMpmC2pMsg2HpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTcrMpmC2pMsg2HpaAddrHi: 0x%x\n", Para->TcrMpmC2pMsg2HpaAddrHi);
  #endif

  BiosMpmMessage.Reg1 = sizeof (MBOX_START_TEXT);
  BiosMpmMessage.Reg2 = Data->TextBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->TextBufferAddr, Data->TextBufferSize);
  CopyMem ((VOID *) (UINTN) Data->TextBufferAddr, Para, sizeof (MBOX_START_TEXT));
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_START_KVM to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdStartKvm (
  IN OUT MBOX_START_KVM *Para
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdStartKvm\n");

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_START_KVM;
  ASSERT (Data->KvmBufferSize != 0);
  ASSERT (Data->KvmBufferOffset != 0);
  ASSERT (Data->KvmBufferAddr != 0);
  ASSERT ((Data->KvmBufferSize & 0xFFFF0000) == 0);
  if ((Data->KvmBufferSize == 0) || (Data->KvmBufferOffset == 0) ||
     (Data->KvmBufferAddr == 0) || ((Data->KvmBufferSize & 0xFFFF0000) != 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBuffer data uninit\n");
    return EFI_INVALID_PARAMETER;
  }
  if (Data->KvmBufferSize < sizeof (MBOX_START_KVM)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmBufferSize too small\n");
    return EFI_INVALID_PARAMETER;
  }
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("MBOX_START_KVM:\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tNetCardPciCfgAddr: 0x%x\n", Para->NetCardPciCfgAddr);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tFrameBufferBaseLo: 0x%x\n", Para->FrameBufferBaseLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tFrameBufferBaseHi: 0x%x\n", Para->FrameBufferBaseHi);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tFrameBufferSize: 0x%x\n", Para->FrameBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDMArEnabled: 0x%x\n", Para->DMArEnabled);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tKvmDmaGpaAddrLo: 0x%x\n", Para->KvmDmaGpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tKvmDmaGpaAddrHi: 0x%x\n", Para->KvmDmaGpaAddrHi);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tKvmDmaHpaAddrLo: 0x%x\n", Para->KvmDmaHpaAddrLo);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPcieMmioCfgBaseAddr: 0x%x\n", Para->PcieMmioCfgBaseAddr);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tNetworkType=%d\n", Para->KvmIpInfo.NetworkType);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tIpv4Addr=%a\n", Para->KvmIpInfo.Ipv4Addr);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tIpv4SubMask=%a\n", Para->KvmIpInfo.Ipv4SubMask);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tIpv4DefGateway=%a\n", Para->KvmIpInfo.Ipv4DefGateway);
  #endif

  BiosMpmMessage.Reg1 = sizeof (MBOX_START_KVM);
  BiosMpmMessage.Reg2 = Data->KvmBufferOffset;
  //Copy input to MPM private buffer
  ZeroMem ((VOID *) (UINTN) Data->KvmBufferAddr, Data->KvmBufferSize);
  CopyMem ((VOID *) (UINTN) Data->KvmBufferAddr, Para, sizeof (MBOX_START_KVM));
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}


/**
  BIOS send command BIOS_MPM_CMD_BOOT_DONE_MSG to MPM to inform BIOS boot done

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdBootDone (
  VOID
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdBootDone\n");

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_BOOT_DONE_MSG;

  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);

  return EFI_SUCCESS;
}

/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_TEXT to MPM to terminate Tcr

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateTcr (
  VOID
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\niosMpmCmdTerminateTcr\n");

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_TERMINATE_TEXT;

  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand BiosMpmCmdTerminateTcr Status:%r\n", Status);

  return EFI_SUCCESS;
}

/**
  Prepare data to be sent for MPM command using common buffer
  * @param[in]  MpmCmdId  Cmd to be sent
  * @param[in,out]  BiosMpmMessage  Will be filled by this routine with data for comand use common buffer
  * @param[in,out]  CmnBufferAddr   point to the common buffer address

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PrepareCmnBufferCmdData (
  UINT8  MpmCmdId,
  BIOS_MPM_MBOX_COMMON  *BiosMpmMessage,
  VOID   **CmnBufferAddr
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  //Check if CmnBuffer has been initialized
  if (Data->CmnBufferAddr == (UINT64) NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return EFI_NOT_FOUND;
  }
  //Prepare input parameters
  *CmnBufferAddr = (VOID *) (UINTN) Data->CmnBufferAddr;
  ZeroMem (BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage->Reg0.Field.CommandId = MpmCmdId;
  ASSERT (Data->CmnBufferSize <= 0xFFFF);
  if (Data->CmnBufferSize > 0xFFFF) {
    return EFI_INVALID_PARAMETER;
  }
  BiosMpmMessage->Reg1 = Data->CmnBufferSize;
  BiosMpmMessage->Reg2 = Data->CmnBufferOffset;
  return EFI_SUCCESS;
}


/**
  BIOS send command BIOS_MPM_CMD_PCIE_MSG to MPM to pass PCI information
  * @param[in,out]  PciInfo  Point to PCI_INFO structure

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdPcieMsg (
  PCI_INFO *PciInfo
  )
{
  EFI_STATUS Status;
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  VOID *CmnBuffer;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdPcieMsg\n");
  CmnBuffer = NULL;
  Status = PrepareCmnBufferCmdData (BIOS_MPM_CMD_PCIE_MSG, &BiosMpmMessage, &CmnBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (CmnBuffer != NULL);
  if (CmnBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }

  //Dump PciInfo
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
  {
    UINT32  i;
    for (i = 0; i < PciInfo->NumberOfDevice; i++) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PciAddr:%lx VendorId:%x DeviceId:%x\n", PciInfo->PciDev[i].PciAddr, PciInfo->PciDev[i].Cfg.Device.Hdr.VendorId, PciInfo->PciDev[i].Cfg.Device.Hdr.DeviceId);
    }
  }
  #endif
  //Copy PciInfo
  CopyMem (CmnBuffer, PciInfo, sizeof (PCI_INFO) + (sizeof(PCI_DEV) * (PciInfo->NumberOfDevice - 1)));
  BiosMpmMessage.Reg1 = sizeof (PCI_INFO) + (sizeof(PCI_DEV) * (PciInfo->NumberOfDevice - 1));
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_AGESA_VER to MPM to pass AGESA version
  * @param[in,out]  AgesaVerString  AGESA version string Null-terminated, max size 256 bytes

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdAgesaVersion (
  CHAR8 * AgesaVerString
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  VOID *CmnBuffer;
  UINTN AgesaVerStrignLength;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdAgesaVersion %a\n", AgesaVerString);
  CmnBuffer = NULL;
  Status = PrepareCmnBufferCmdData (BIOS_MPM_CMD_AGESA_VER, &BiosMpmMessage, &CmnBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (CmnBuffer != NULL);
  if (CmnBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }

  AgesaVerStrignLength = AsciiStrLen(AgesaVerString);
  //Copy AGESA ASCII string
  AsciiStrCpyS (CmnBuffer, BIOS_MPM_CMD_AGESA_VER_MAX_SIZE, AgesaVerString);
  BiosMpmMessage.Reg1 = (UINT32)AgesaVerStrignLength;
  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_BATTERY_MSG to MPM to pass battery information
  * @param[in,out]  BatteryInfo  Point to the structure contain battery information

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdBatteryMsg (
  BATTERY_INFO_T * BatteryInfo
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  VOID *CmnBuffer;

  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdBatteryMsg\n");
  CmnBuffer = NULL;
  Status = PrepareCmnBufferCmdData (BIOS_MPM_CMD_BATTERY_MSG, &BiosMpmMessage, &CmnBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (CmnBuffer != NULL);
  if (CmnBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }

  //Copy Battery info
  CopyMem (CmnBuffer, BatteryInfo, sizeof (BATTERY_INFO_T));
  BiosMpmMessage.Reg1 = sizeof (BATTERY_INFO_T);
  #ifdef MPM_MBOX_LIB_DBG_VERBOSE
    IDS_HDT_CONSOLE_PSP_TRACE ("BATTERY_INFO_T:\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tBatteryStatus %x\n", BatteryInfo->BatteryStatus);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTimeOnBattery %x\n", BatteryInfo->TimeOnBattery);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tEstimatedRunTime %x\n", BatteryInfo->EstimatedRunTime);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tEstimatedChargeRemaining %x\n", BatteryInfo->EstimatedChargeRemaining);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tChemistry %x\n", BatteryInfo->Chemistry);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDesignCapacity %x\n", BatteryInfo->DesignCapacity);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tSmartBatteryVersion %s\n", BatteryInfo->SmartBatteryVersion);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tTimeToFullCharge %x\n", BatteryInfo->TimeToFullCharge);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tExpectedLife %x\n", BatteryInfo->ExpectedLife);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tMaxRechargeTime %x\n", BatteryInfo->MaxRechargeTime);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tRechargeCount %x\n", BatteryInfo->RechargeCount);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tMaxRechargeCount %x\n", BatteryInfo->MaxRechargeCount);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tRemainingCapacity %x\n", BatteryInfo->RemainingCapacity);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tOtherChemistryDescription %s\n", BatteryInfo->OtherChemistryDescription);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tChargingStatus %x\n", BatteryInfo->ChargingStatus);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tHealthPercent %x\n", BatteryInfo->HealthPercent);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tBatteryMsg buffer dump:\n");
    IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN) CmnBuffer, BiosMpmMessage.Reg1);
  #endif

  Status = SendMpmCommand (&BiosMpmMessage);
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return Status;
}

/**
  BIOS send command BIOS_MPM_CMD_UN_PROVISION_MSG to MPM to un-provision the system

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdUnProvision (
  VOID
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdUnProvision\n");

  //Prepare input parameters
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_UN_PROVISION_MSG;

  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);

  return EFI_SUCCESS;
}
EFI_STATUS
BiosMpmCmdDisableUnProvision (
  VOID
  )
{
  BIOS_MPM_MBOX_COMMON  BiosMpmMessage;
  EFI_STATUS Status;
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBiosMpmCmdDisableUnProvision\n");
  ZeroMem (&BiosMpmMessage, sizeof (BIOS_MPM_MBOX_COMMON));
  BiosMpmMessage.Reg0.Field.CommandId = BIOS_MPM_CMD_DISABLE_UN_PROVISION_MSG;
  Status = SendMpmCommand (&BiosMpmMessage);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("SendMpmCommand Status:%r\n", Status);
  return EFI_SUCCESS;
}