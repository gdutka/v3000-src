/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file
 *
 * PSP Mailbox related functions
 *
 *
 */
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <FchRegistersCommon.h>
#include <Filecode.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/AmdPspMmioLib.h>

#define FILECODE LIBRARY_AMDPSPMBOXLIBV2_AMDPSPMBOXLIBV2_FILECODE
#define BIOS_CMD_TIMEOUT_WAIT_INFINITELY 0xFFFFFFFFL
#define TEE_ERROR_ITEM_NOT_FOUND         0x0008

/**
   Check if time has been used up, if not, sleep and count 1 microsecond down

   @param[in, out]  CountdownInMicroseconds        count down number in microseconds

   @retval BOOLEAN                TRUE: timedout, FALSE: not timedout

 **/
BOOLEAN
IsTimedOut (
  IN OUT UINT32  *CountdownInMicroseconds
  )
{
  //time has been used up, so timed out
  if ((*CountdownInMicroseconds) == 0) {
    return TRUE;
  }

  //wait infinitely, so never timed out
  if ((*CountdownInMicroseconds) == BIOS_CMD_TIMEOUT_WAIT_INFINITELY) {
    return FALSE;
  }

  //sleep 1 microsecond
  MicroSecondDelay(1);
  (*CountdownInMicroseconds)--;

  //not timed out this time
  return FALSE;
}

/**
   BIOS sends command to PSP
   This should be used for most cases, compared to SendPspCommandSmn.

   @param[in]  MboxBuffer        PSP mbox buffer
   @param[in]  Cmd               Psp Mailbox Command

   @retval BOOLEAN                1: Success, 0 Error

 **/
BOOLEAN
SendPspCommand (
  IN VOID          *MboxBuffer,
  IN MBOX_COMMAND  Cmd
  )
{
  BOOLEAN             Status;
  PSP_MBOX_V2         *PspMbox;
  PSP_MBOX_V2_CMD     PspCmd;
  MBOX_BUFFER_HEADER  *BufferHdrPtr;
  UINT32              i;
  UINT32              CountdownInMicroseconds;
  UINT32              PspMmioBase;
  UINT32              MboxOffsetBase;
  UINT64              FilterFlag;

  Status = TRUE;
  FilterFlag = PSP_TRACE;
  //print logs for all commands except MboxBiosCmdUsbConfig as it introduces huge logging
  if (Cmd == MboxBiosCmdUsbConfig) {
    FilterFlag = 0;
  }
  IDS_HDT_CONSOLE (FilterFlag, "Psp.SendC2PCMD [0x%x] Buffer:0x%x\n", Cmd, MboxBuffer);
  BufferHdrPtr = MboxBuffer;
  for (i = 0; i < BufferHdrPtr->TotalSize; i++) {
    IDS_HDT_CONSOLE (FilterFlag, "%02x ", *(((UINT8 *) BufferHdrPtr) + i) );
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE (FilterFlag, "\n");
    }
  }
  IDS_HDT_CONSOLE (FilterFlag, "\n");
  // Get PspMbox location. fail if not found
  if (GetPspMboxLocation (&PspMbox) == FALSE) {
    IDS_HDT_CONSOLE (FilterFlag, "GetPspMboxLocation Error\n");
    ASSERT (FALSE); // Assertion in the debug build
    return (FALSE);
  }
  // Get PspMmioBase location. fail if not found
  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    IDS_HDT_CONSOLE (FilterFlag, "GetPspMmioBase Error\n");
    ASSERT (FALSE); // Assertion in the debug build
    return (FALSE);
  }
  ASSERT ((UINT32) (UINTN) PspMbox >= PspMmioBase);
  MboxOffsetBase = (UINT32) (UINTN) PspMbox - PspMmioBase;

  PspCmd.Value = PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Cmd));
  IDS_HDT_CONSOLE (FilterFlag, "PspCmd.Value %x\n", PspCmd.Value);

  if (PcdGetBool (PcdAmdSendPspCommandIgnoreRecoveryFlag) == FALSE && PspCmd.Field.Recovery) {
    IDS_HDT_CONSOLE (FilterFlag, "Recovery Flag detected, ignore the command\n");
    return (FALSE);
  }

  AGESA_TESTPOINT (TpPspC2PmboxBeforeSendCmdBase + Cmd, NULL);

  // Wait till mailbox is ready and PSP finish processing previous command
  CountdownInMicroseconds = PcdGet32 (PcdAmdPspMboxReadinessTimeout);
  do {
    PspCmd.Value = PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Cmd));
    if (IsTimedOut (&CountdownInMicroseconds)) {
      IDS_HDT_CONSOLE (FilterFlag, "Timeout when waiting for PSP ready\n");
      return (FALSE);
    }
  } while ((!PspCmd.Field.Ready) || (PspCmd.Field.CommandId));

  IDS_HDT_CONSOLE (FilterFlag, "Clear the ready bit before send the command\n");
  //Clear the ready bit before send the command
  //Do it on local variable 1st to avoid pontencial racing issue
  PspCmd.Value = PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Cmd));
  PspCmd.Field.Status = 0;
  PspCmd.Field.Ready = 0;
  PspCmd.Field.CommandId = (UINT8) Cmd;
  //Lower 32bits
  PspMmioWrite (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Buffer), (UINT32) (UINTN) MboxBuffer);
  //High 32bits
  PspMmioWrite (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Buffer) + sizeof (UINT32), ((UINT64) (UINTN)MboxBuffer>>32));
  IDS_HDT_CONSOLE (FilterFlag, "PspCmd.MboxBuffer %x_%x\n", \
                   PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Buffer) + sizeof (UINT32)), \
                   PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Buffer)));
  // Now send the command
  PspMmioWrite (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Cmd), PspCmd.Value);

  AGESA_TESTPOINT (TpPspC2PmboxWaitCmdBase + Cmd, NULL);

  /// Wait for PSP to be done
  CountdownInMicroseconds = PcdGet32 (PcdAmdPspCmdCompletenessTimeout);
  do {
    PspCmd.Value = PspMmioRead (MboxOffsetBase + OFFSET_OF (PSP_MBOX_V2, Cmd));
    if (IsTimedOut (&CountdownInMicroseconds)) {
      IDS_HDT_CONSOLE (FilterFlag, "Timeout when waiting for PSP done\n");
      return (FALSE);
    }
  } while (PspCmd.Field.CommandId);

  AGESA_TESTPOINT (TpPspC2PmboxCmdDone, NULL);
  IDS_HDT_CONSOLE (FilterFlag, "\nBuffer dump after CMD finished:\n");
  BufferHdrPtr = MboxBuffer;
  BufferHdrPtr->Status = PspCmd.Field.Status;
  for (i = 0; i < BufferHdrPtr->TotalSize; i++) {
    IDS_HDT_CONSOLE (FilterFlag, "%02x ", *(((UINT8 *) BufferHdrPtr) + i) );
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\n");
    }
  }
  IDS_HDT_CONSOLE (FilterFlag, "\nPspCmd.Value %x\n", PspCmd.Value);
  //if Status set by PSP FW is not 0, then error occurred
  if (PspCmd.Field.Status != 0) {
    IDS_HDT_CONSOLE (FilterFlag, "C2P command [0x%x] execution error:0x%x\n", Cmd, PspCmd.Field.Status);
    return (FALSE);
  }
  IDS_HDT_CONSOLE (FilterFlag, "\nPsp.SendC2PCMD Exit\n");
  return Status;
}

/**
   BIOS sends command to PSP via Smn
   This should be used for some cases when SendPspCommand cannot be used,
   for example, in early PEI when PSP BAR has not been initialized

   @param[in]  MboxBuffer                      PSP mbox buffer
   @param[in]  Cmd                             Psp Mailbox Command
   @param[in]  IgnorePspRecoveryFlag           ignore Recovery Flag or not, for most cases, it should be FALSE
   @retval BOOLEAN                     1: Success, 0 Error

 **/
BOOLEAN
SendPspCommandSmn (
  IN VOID          *MboxBuffer,
  IN MBOX_COMMAND  Cmd,
  IN BOOLEAN       IgnorePspRecoveryFlag
  )
{
  BOOLEAN             Status;
  PSP_MBOX_V2_CMD     PspCmd;
  MBOX_BUFFER_HEADER  *BufferHdrPtr;
  UINT32              i;
  UINT32              CountdownInMicroseconds;
  UINT32              BufferHigh = 0;
  UINT32              BufferLow = 0;
  Status = TRUE;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.SendC2PCMD [0x%x] Buffer:0x%x\n", Cmd, MboxBuffer);
  BufferHdrPtr = MboxBuffer;
  for (i = 0; i < BufferHdrPtr->TotalSize; i++) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%02x ", *(((UINT8 *) BufferHdrPtr) + i) );
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\n");
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\n");

  SmnRegisterRead (0, MP0_C2PMSG_28_SMN_ADDR,  (VOID *) &PspCmd.Value);
  IDS_HDT_CONSOLE_PSP_TRACE ("PspCmd.Value %x\n", PspCmd.Value);

  if (PcdGetBool (PcdAmdSendPspCommandIgnoreRecoveryFlag) == FALSE &&
      !IgnorePspRecoveryFlag && PspCmd.Field.Recovery) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Recovery Flag detected, ignore the command\n");
    return (FALSE);
  }

  AGESA_TESTPOINT (TpPspC2PmboxBeforeSendCmdBase + Cmd, NULL);

  // Wait till mailbox is ready and PSP finish processing previous command
  CountdownInMicroseconds = PcdGet32 (PcdAmdPspMboxReadinessTimeout);
  while ((!PspCmd.Field.Ready) ||  (PspCmd.Field.CommandId)) {
    SmnRegisterRead (0, MP0_C2PMSG_28_SMN_ADDR,  (VOID *) &PspCmd.Value);
    if (IsTimedOut (&CountdownInMicroseconds)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Timeout when waiting for PSP ready\n");
      return (FALSE);
    }
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("Clear the ready bit before send the command\n");
  //Clear the ready bit before send the command
  //Do it on local variable 1st to avoid pontencial racing issue
  PspCmd.Field.Status = 0;
  PspCmd.Field.Ready = 0;
  PspCmd.Field.CommandId = (UINT8) Cmd;

  //Write command buffer address
  BufferHigh = ((UINT64) (UINTN) MboxBuffer) >> 32;
  BufferLow = ((UINT64) (UINTN) MboxBuffer) & 0xFFFFFFFF;
  SmnRegisterWrite (0,
                    MP0_C2PMSG_30_SMN_ADDR,
                    &BufferHigh,
                    AccessWidth32);
  SmnRegisterWrite (0,
                    MP0_C2PMSG_29_SMN_ADDR,
                    &BufferLow,
                    AccessWidth32);
  // Now send the command
  SmnRegisterWrite (0,
                    MP0_C2PMSG_28_SMN_ADDR,
                    (VOID *) &PspCmd,
                    AccessWidth32);

  AGESA_TESTPOINT (TpPspC2PmboxWaitCmdBase + Cmd, NULL);

  /// Wait for PSP to be done
  CountdownInMicroseconds = PcdGet32 (PcdAmdPspCmdCompletenessTimeout);
  while (PspCmd.Field.CommandId) {
    SmnRegisterRead (0, MP0_C2PMSG_28_SMN_ADDR,  (VOID *) &PspCmd.Value);
    if (IsTimedOut (&CountdownInMicroseconds)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Timeout when waiting for PSP done\n");
      return (FALSE);
    }
  }

  AGESA_TESTPOINT (TpPspC2PmboxCmdDone, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\nBuffer dump after CMD finished:\n");
  BufferHdrPtr = MboxBuffer;
  BufferHdrPtr->Status = PspCmd.Field.Status;
  for (i = 0; i < BufferHdrPtr->TotalSize; i++) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%02x ", *(((UINT8 *) BufferHdrPtr) + i) );
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\n");
    }
  }
  //if Status set by PSP FW is not 0, then error occurred
  if (PspCmd.Field.Status != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("C2P command [0x%x] execution error:0x%x\n", Cmd, PspCmd.Field.Status);
    return (FALSE);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\nPsp.SendC2PCMD Exit\n");
  return Status;
}

/**
   BIOS to query generic UINT32 data from the PSP

   @param[in]  Cmd      The command id
   @param[in]  Data     Data got from PSP

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosQueryUint32 (
  IN       MBOX_COMMAND  Cmd,
  IN OUT   UINT32        *Data
  )
{
  MBOX_GET_UINT32_BUFFER *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;            // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.MboxBiosCmdPspCapsQuery\n");

  if (Data == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize = sizeof (MBOX_GET_UINT32_BUFFER);

  Buffer->Data.Value = 0; // Result of command will be placed here

  CmdSts = SendPspCommand (Buffer, Cmd);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  *Data = Buffer->Data.Value;
  IDS_HDT_CONSOLE_PSP_TRACE ("Data returned from PSP: %x\n", Buffer->Data.Value);
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS sends the SMM information to PSP. SMM information is used by PSP to pass data back to BIOS

   @param[in]  SmmInfoReq          Point to the structure containing SMMInfo command required data

   @retval EFI_STATUS              0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosCmdSmmInfo (
  IN SMM_REQ_BUFFER  *SmmInfoReq
  )
{
  MBOX_SMM_BUFFER        *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SmmInfo\n");

  Buffer->Header.TotalSize             = sizeof (MBOX_SMM_BUFFER);

  CopyMem (&Buffer->Req, SmmInfoReq, sizeof (SMM_REQ_BUFFER));
  IDS_HDT_CONSOLE_PSP_TRACE ("SMMBase %x SMMMask %x\n", SmmInfoReq->SMMBase, SmmInfoReq->SMMMask);
  IDS_HDT_CONSOLE_PSP_TRACE ("PSPSmmDataRegion %x PspSmmDataLength %x\n", SmmInfoReq->PSPSmmDataRegion,
                             SmmInfoReq->PspSmmDataLength);
  IDS_HDT_CONSOLE_PSP_TRACE ("SmmTrigInfo::Addr:0x%x AddrType:0x%x Width:0x%x AndMask:0x%x OrMask:0x%x\n",
                             SmmInfoReq->SmmTrigInfo.Address,
                             SmmInfoReq->SmmTrigInfo.AddressType,
                             SmmInfoReq->SmmTrigInfo.ValueWidth,
                             SmmInfoReq->SmmTrigInfo.ValueAndMask,
                             SmmInfoReq->SmmTrigInfo.ValueOrMask);

  IDS_HDT_CONSOLE_PSP_TRACE ("SmiEnb.Address:0x%x AddrType:0x%x Width:0x%x Mask:0x%x ExpectValue:0x%x\n",
                             SmmInfoReq->SmmRegInfo.SmiEnb.Address,
                             SmmInfoReq->SmmRegInfo.SmiEnb.AddressType,
                             SmmInfoReq->SmmRegInfo.SmiEnb.ValueWidth,
                             SmmInfoReq->SmmRegInfo.SmiEnb.RegBitMask,
                             SmmInfoReq->SmmRegInfo.SmiEnb.ExpectValue);


  IDS_HDT_CONSOLE_PSP_TRACE ("Eos.Address:0x%x AddrType:0x%x Width:0x%x Mask:0x%x ExpectValue:0x%x\n",
                             SmmInfoReq->SmmRegInfo.Eos.Address,
                             SmmInfoReq->SmmRegInfo.Eos.AddressType,
                             SmmInfoReq->SmmRegInfo.Eos.ValueWidth,
                             SmmInfoReq->SmmRegInfo.Eos.RegBitMask,
                             SmmInfoReq->SmmRegInfo.Eos.ExpectValue);


  IDS_HDT_CONSOLE_PSP_TRACE ("FakeSmiEn.Address:0x%x AddrType:0x%x Width:0x%x Mask:0x%x ExpectValue:0x%x\n",
                             SmmInfoReq->SmmRegInfo.FakeSmiEn.Address,
                             SmmInfoReq->SmmRegInfo.FakeSmiEn.AddressType,
                             SmmInfoReq->SmmRegInfo.FakeSmiEn.ValueWidth,
                             SmmInfoReq->SmmRegInfo.FakeSmiEn.RegBitMask,
                             SmmInfoReq->SmmRegInfo.FakeSmiEn.ExpectValue);

  IDS_HDT_CONSOLE_PSP_TRACE ("PspMboxSmmBufferAddress:0x%x PspMboxSmmFlagAddress:0x%x\n",
                             SmmInfoReq->PspMboxSmmBufferAddress,
                             SmmInfoReq->PspMboxSmmFlagAddress);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSmmInfo);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;

}


/**
   BIOS sends the Sx information to PSP. This mailbox command is send just prior of entering Sx state

   @param[in]  SleepRequest        SleepType in SleepRequest: the system is transition to.
                                    3 : S3, 4:S4, 5:S5
                                   Flag in SleepRequest: provide additional description for SleepType,
                                   for example, BIT1 in Flag for S3 means Fake S3 for Capsule Update

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdSxInfo (
  IN SX_REQ_BUFFER  *SleepRequest,
  IN UINT8          *SmmBuffer,
  IN BOOLEAN        *SmmFlag
  )
{
  EFI_STATUS      Status;
  MBOX_SX_BUFFER  *Buffer;
  BOOLEAN         CmdSts;

  CmdSts = FALSE;
  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SxInfo\n");
  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
    Buffer = (MBOX_SX_BUFFER*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_SX_BUFFER));
    Buffer->Header.TotalSize      = sizeof (MBOX_SX_BUFFER);
    Buffer->Req.SleepType         = SleepRequest->SleepType;
    Buffer->Req.Flag              = SleepRequest->Flag;
    Buffer->Req.Reserved          = 0;
    *SmmFlag = TRUE;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdSxInfo);
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
  }
  return Status;

}

/**
   BIOS sends the Resume information to PSP. PSP will use this information to measure the resume code.
   During resume path PSP will use this information before handing off to BIOS to ensure the resume
   code is not tampered

   @param[in]  S3ResumeAddress       Location of BIOS reset code that will first fetch on resume

   @param[in]  S3ResumeCodeSize      Size of resume code that PSP need to measure to ensure authenticity

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdResumeInfo (
  IN UINT64  S3ResumeAddress,
  IN UINT64  S3ResumeCodeSize
  )
{
///@todo Reopen this command
  return EFI_SUCCESS;
//  MBOX_RSM_BUFFER         *Buffer;
//  UNALIGNED_MBOX_BUFFER   UBuff;    // Unaligned buffer
//  BOOLEAN                 CmdSts;

//  CmdSts = FALSE;
//  Buffer = BALIGN32 (&UBuff);
//  PSP_DEBUG ("Psp.C2PMbox.ResumeInfo\n");
//
//  Buffer->Header.TotalSize      = sizeof (MBOX_RSM_BUFFER);
//
//  Buffer->Req.ResumeVecorAddress = S3ResumeAddress;
//  Buffer->Req.ResumeVecorLength  = S3ResumeCodeSize;
//
//  CmdSts = SendPspCommand (Buffer, MboxBiosCmdRsmInfo);
//  if (CmdSts == FALSE) {
//    return EFI_DEVICE_ERROR;
//  }
//  PSP_DEBUG ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
//  return EFI_SUCCESS;
}


/**
   BIOS sends this command to inform PSP that BIOS is handing off to OS/OROM. Any mailbox command
   after this message need to come from SMM space (the SMM info is provided earlier via mailbox)
   i.e. PSP should check the parameter address to ensure it falls in SMM region

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdExitBootServices ()
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.ExitBootServices\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdBootDone);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS requests the capabilities from the PSP

   @param[in]  Capabilities      PSP Writes capabilities into this field when it returns.

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosQueryCaps (
  IN OUT UINT32  *Capabilities
  )
{
  MBOX_CAPS_BUFFER       *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosQueryCaps\n");

  if (Capabilities == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize      = sizeof (MBOX_CAPS_BUFFER);

  Buffer->Req.Capabilities      = 1; // 1 to identify the command is sending from V9

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdFtpmQuery);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  *Capabilities = Buffer->Req.Capabilities;
  IDS_HDT_CONSOLE_PSP_TRACE ("Caps return %x\n", Buffer->Req.Capabilities);
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}


/**
   BIOS sends AP CS BASE

   @param[in]  ApCsBase       Address to update the code segment base on APs

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosSendApCsBase (
  IN UINT32  ApCsBase
  )
{
  MBOX_AP_CS_BASE_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;     // Unaligned buffer
  BOOLEAN                 CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxApCsBase\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_AP_CS_BASE_BUFFER);

  Buffer->Req.Value = ApCsBase;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSetApCsBase);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}


/**
   BIOS get FW versions from the PSP

   @param[in]  FwVersions      PSP Writes multiple FW versions into this buffer when it returns.

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosGetFwVersions (
  IN OUT VERSION_BUFFER  *FwVersions
  )
{
  MBOX_GET_VERSION_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER    UBuff;   // Unaligned buffer
  BOOLEAN                  CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosGetFwVersions\n");

  if (FwVersions == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize      = sizeof (MBOX_GET_VERSION_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdGetVersion);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  CopyMem (FwVersions, &Buffer->VersionBuffer, sizeof (VERSION_BUFFER));
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   Check if Ftpm is ready

   @retval EFI_STATUS  0: Success, NonZero Error

 **/
EFI_STATUS
EFIAPI
CheckFtpmCapsV2 (
  IN OUT UINT32  *Caps
  )
{
  UINT32  Capabilities;

  if (PspMboxBiosQueryCaps (&Capabilities)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("QueryCaps command return status fail\n");
    return (EFI_UNSUPPORTED);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox return Caps %x\n", Capabilities);

  *Caps = Capabilities;

  return (EFI_SUCCESS);
}


/**
 * @brief Check PSP caps, e.g. NVRAM status which used for RPMC feature
 *
 * @param PspCaps Point to PSP caps
 * @return EFI_STATUS 0: Success, NonZero Error
 */
EFI_STATUS
CheckPspCapsV2 (
  IN OUT UINT32  *PspCaps
  )
{
  EFI_STATUS  Status;
  UINT32      PspCapabilities;

  PspCapabilities = 0;
  Status = PspMboxPspCapsQuery (&PspCapabilities);

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get PSP Capabilities fail, status = 0x%x\n", Status);
    return Status;
  }

  *PspCaps = PspCapabilities;

  return (EFI_SUCCESS);
}

/**
   BIOS requests the HSTI state from the PSP

   @param[in]  HSTIState             PSP Writes HSTIState into this field when it returns.

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosQueryHSTIState (
  IN OUT UINT32  *HSTIState
  )
{
  MBOX_HSTI_STATE        *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosQueryHSTIState\n");

  if (HSTIState == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize = sizeof (MBOX_HSTI_STATE);

  Buffer->HSTIState = 0; // Result of command will be placed here

  CmdSts = SendPspCommandSmn (Buffer, MboxBiosCmdHSTIQuery, FALSE);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  *HSTIState = Buffer->HSTIState;
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x HSTIState:0x%x\n", Buffer->Header.Status, Buffer->HSTIState);
  return EFI_SUCCESS;
}
/**
   BIOS sends the message to PSP o lock DF registers

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosLockDFReg (
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LockDFReg\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLockDFReg);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief BIOS send this command to PSP to fence off eSPI 0x00:0F
 *
 * @return EFI_STATUS   0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosLockeSPIReg (
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LockeSPIReg\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLockeSPIReg);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * BIOS sends the message to PSP to lock FCH registers
 *
 * @param[in]  FchDataPtr     Point to Fch Data buffer
 * @retval EFI_STATUS         0: Success, NonZero Error
 *
 **/
EFI_STATUS
PspMboxBiosLockFCHReg (
  IN VOID  *FchDataPtr
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LockFCHReg\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLockFCHReg);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * BIOS sends the message to PSP to lock Gpio pins
 *
 * @param[in]  FchDataPtr     Point to Fch Data buffer
 * @retval EFI_STATUS         0: Success, NonZero Error
 *
 **/
EFI_STATUS
PspMboxBiosLockGpio (
  IN VOID  *FchDataPtr
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LockFCHReg\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSetGpioFencing);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send this command to inform PSP the information KVM required
 *
 * @param[in]  KvmMboxMemBuffer           Point to KVM information buffer
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosCmdKvmInfo (
  MBOX_KVM  *KvmMboxMemBuffer
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  MBOX_KVM               *Buffer;

  BOOLEAN  CmdSts;
  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.KvmInfo\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmDmaAddr=0x%08x%08x\n DMABufferSize=0x%x\n", KvmMboxMemBuffer->KvmDmaAddrHi,
                             KvmMboxMemBuffer->KvmDmaAddrLo, KvmMboxMemBuffer->KvmDmaSize);

  Buffer->Header.TotalSize      = sizeof (MBOX_KVM);
  Buffer->KvmDmaAddrLo = KvmMboxMemBuffer->KvmDmaAddrLo;
  Buffer->KvmDmaAddrHi = KvmMboxMemBuffer->KvmDmaAddrHi;
  Buffer->KvmDmaSize = KvmMboxMemBuffer->KvmDmaSize;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdKvmInfo);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief BIOS send the command the PSP to let PSP tOS to validate the integrity of uCode patch
 *
 * @param Buffer        The buffer to store uCode patch
 * @return EFI_STATUS   0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdValidateBinary (
  IN MBOX_GENERIC_BUFFER      *UCodePatchBuffer
  )
{
  UNALIGNED_MBOX_BUFFER       UBuff;     // Unaligned buffer
  MBOX_GENERIC_BUFFER         *Buffer;

  BOOLEAN  CmdSts;
  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.ValidateBinary\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("UCodePatchBufferAddr=0x%08x%08x\n BufferSize=0x%x\n", UCodePatchBuffer->BufferAddrHi,
                             UCodePatchBuffer->BufferAddrLo, UCodePatchBuffer->BufferSize);

  Buffer->Header.TotalSize      = sizeof (MBOX_GENERIC_BUFFER);
  Buffer->BufferAddrLo = UCodePatchBuffer->BufferAddrLo;
  Buffer->BufferAddrHi = UCodePatchBuffer->BufferAddrHi;
  Buffer->BufferSize = UCodePatchBuffer->BufferSize;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdValidateBinary);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * Bios send this command to PSP to decide which RPMC address to use.
 * A warm reset should be issued after receiving BIOS_MBOX_OK (0) from PSP tOS, otherwise BIOS do nothing.
 * It's caller's responsbility to issue the warm reset.
 *
 * @param[in]  RpmcAddressToUse           which RPMC address to program. Value 0 to 3
 *
   @retval EFI_STATUS              0: Success, NonZero Error
 * for example: 0x14 means the PSP report RPMC not available
 **/
EFI_STATUS
PspMboxBiosCmdSetRpmcAddress (
  UINT32  RpmcAddressToUse
  )
{
  MBOX_SET_RPMC_ADDRESS_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER         UBuff; // Unaligned buffer
  BOOLEAN                       CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SetRpmcAddress RpmcAddressToUse:%x\n", RpmcAddressToUse);

  Buffer->Header.TotalSize      = sizeof (MBOX_SET_RPMC_ADDRESS_BUFFER);
  Buffer->Header.Status         = 0;
  Buffer->RpmcAddressToUse = RpmcAddressToUse;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSetRpmcAddress);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send this command to PSP to validate the signature of manageability OS image
 *
 * @param[in]  ManOsMboxMemBuffer           Point to ManOs information buffer
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosCmdValidateManOsSignature (
  MBOX_MANOS  *ManOsMboxMemBuffer
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  MBOX_MANOS             *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.ManOsInfo\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("ManOsBufferAddr=0x%08x%08x\n ManOsBufferSize=0x%x\n", ManOsMboxMemBuffer->ManOsAddrHi,
                             ManOsMboxMemBuffer->ManOsAddrLo, ManOsMboxMemBuffer->ManOsBufferSize);

  Buffer->Header.TotalSize      = sizeof (MBOX_MANOS);
  Buffer->ManOsAddrLo = ManOsMboxMemBuffer->ManOsAddrLo;
  Buffer->ManOsAddrHi = ManOsMboxMemBuffer->ManOsAddrHi;
  Buffer->ManOsBufferSize = ManOsMboxMemBuffer->ManOsBufferSize;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdValidateManOsSignature);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS sends TA command to PSP

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
TEE_STATUS
PspMboxBiosTa (
  UINT64  TaCmdBufferAddress,
  UINT64  TaCmdBufferSize
  )
{
  MBOX_TA                *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosTa Buffer:%x Size:%x\n", TaCmdBufferAddress, TaCmdBufferSize);

  Buffer->Header.TotalSize      = sizeof (MBOX_TA);
  Buffer->Header.Status         = 0;
  Buffer->TaCmdBufferAddress = TaCmdBufferAddress;
  Buffer->TaCmdBufferSize    = TaCmdBufferSize;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdTa);
  if (CmdSts == FALSE) {
    return TEE_ERR_NWD_INVALID_SESSION;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS will send this command to lock SPI, X86 need in SMM mode when send this command

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosLockSpi (
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LockSpi\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLockSpi);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief A general BIOS-PSP command with command id but without other paramaters
 *
 * @param Cmd                      Command ID
 * @return EFI_STATUS              0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosGeneralCmd (
  IN       MBOX_COMMAND  Cmd
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.GeneralCmd\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, Cmd);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

EFI_STATUS
PspMboxBiosSetOcFuse (
  )
{
  return PspMboxBiosGeneralCmd (MboxBiosCmdSetOcFuse);
}

/**
 *
 *
 * Bios send this command to inform PSP the FCH GPIO require to turn on the screen
 *
 * @param[in]  GpioList           Point to GPIO list
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosScreenOnGpio (
  SCREENON_CONTROL_LIST  *GpioList
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  MBOX_SCREENON_GPIO     *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_SCREENON_GPIO);
  CopyMem (&Buffer->List, GpioList, sizeof (SCREENON_CONTROL_LIST));

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdScreenOnGpio);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send this command to pass SPI Operation whitelistn
 *
 * @param[in]  WhiteList           Point to SPI OP white list
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosSpiOpWhiteList (
  SPI_OP_LIST  *WhiteList,
  IN UINT8     *SmmBuffer,
  IN BOOLEAN   *SmmFlag
  )
{
  MBOX_SPI_OP_WHITELIST  *Buffer;
  EFI_STATUS             Status;
  UINT32                 i;
  UINTN                  PciAddr;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Status = EFI_INVALID_PARAMETER;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SpiOpWhiteList:");
  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
    //Init Buffer
    Buffer = (MBOX_SPI_OP_WHITELIST*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_SPI_OP_WHITELIST));
    //Print input parameters
    IDS_HDT_CONSOLE_PSP_TRACE ("NumOfOp:%d\n", WhiteList->NumOfOp);
    ASSERT (WhiteList->NumOfOp <= MAX_NUM_SPI_OP);

    IDS_HDT_CONSOLE_PSP_TRACE ("OpList:\n");
    for (i = 0; i < WhiteList->NumOfOp; i++) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t[%d]: OpCodes:0x%x  Options:0x%x Size:0x%x\n", i, WhiteList->Ops[i].OpCode,
                                 WhiteList->Ops[i].Options, WhiteList->Ops[i].Size);
    }
    //Fill buffer with input parameters
    Buffer->Header.TotalSize      = sizeof (MBOX_SPI_OP_WHITELIST);
    CopyMem (&Buffer->List, WhiteList, sizeof (SPI_OP_LIST));
    // Read LPC register, as it can't be accessed by SMU and PSP
    Buffer->ROMAddrRng2 = PciRead32 (PCI_LIB_ADDRESS (FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPC_REG6C));
    PciAddr = PCI_LIB_ADDRESS (FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPC_REG50);
    for (i = 0; i < 4; i++) {
      Buffer->RomProtect[i] = PciRead32 (PciAddr + 4 * i);
    }
    *SmmFlag = TRUE;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdSpiOpWhiteList);
    *SmmFlag = FALSE;

    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    return EFI_SUCCESS;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Invalid parameter\n");
  }
  return (Status);
}

/**
 *
 *
 * Bios send this command to pass RAS EINJ related action
 *
 * @param[in]  Action              Action of Einj, e.g. RAS_EINJ_ENABLE_INJECTION, RAS_EINJ_END_OPERATION
 * @param[in]  ActionStruct        Optional, Pass NULL for the action doesn't require extra parameters
 *                                 For RAS_EINJ_EXECUTE_OPERATION, point to RAS_EINJ_EXECUTE_OPERATION_STRUCT
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosRasEinj (
  UINT32      Action,
  VOID        *ActionStruct,
  IN UINT8    *SmmBuffer,
  IN BOOLEAN  *SmmFlag
  )
{
  MBOX_RAS_EINJ  *Buffer;
  EFI_STATUS     Status;
  MBOX_COMMAND   MboxBiosCmdId;
  BOOLEAN        CmdSts;

  CmdSts = FALSE;
  MboxBiosCmdId = (MBOX_COMMAND)((Action >> 16) & 0xFF);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.RasEinj Action: 0x%08x (MboxBiosCmdId: 0x%02x, Operation: 0x%02x)\n", Action,
                             MboxBiosCmdId, (Action & 0xFF));
  // report issue to AGESA PSP owner, if assert here
  ASSERT (sizeof (MBOX_BUFFER) >= (sizeof (MBOX_RAS_EINJ) + sizeof (RAS_EINJ_EXECUTE_OPERATION_STRUCT)));
  if (sizeof (MBOX_BUFFER) < (sizeof (MBOX_RAS_EINJ) + sizeof (RAS_EINJ_EXECUTE_OPERATION_STRUCT))) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Buffer too small\n");
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;
  Buffer = NULL;
  switch (Action) {
  case RAS_EINJ_ENABLE_INJECTION:
  case RAS_EINJ_BEGIN_INJECTION_OPERATION:
  case RAS_EINJ_END_OPERATION:
  case RAS_EINJ_EXECUTE_OPERATION:
  case RAS_ACPI_EINJ_ENABLE_INJECTION:
  case RAS_ACPI_EINJ_DISABLE_INJECTION:
    if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
      //Init Buffer
      Buffer = (MBOX_RAS_EINJ*)SmmBuffer;
      ZeroMem (Buffer, sizeof (MBOX_BUFFER));
    } else {
      Status = EFI_INVALID_PARAMETER;
    }
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("check Action: %x failed: %r\n", Action, Status);
    return Status;
  }

  Buffer->Header.TotalSize      = sizeof (MBOX_RAS_EINJ);
  Buffer->Action      = (Action & 0xFF);

  if (Action == RAS_EINJ_EXECUTE_OPERATION) {
    ASSERT (ActionStruct != NULL);
    if (ActionStruct == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    Buffer->Header.TotalSize += sizeof (RAS_EINJ_EXECUTE_OPERATION_STRUCT);
    CopyMem ((Buffer + 1), ActionStruct, sizeof (RAS_EINJ_EXECUTE_OPERATION_STRUCT));
  }

  if (SmmFlag != NULL) {
    *SmmFlag = TRUE;
  }
  CmdSts = SendPspCommand (Buffer, MboxBiosCmdId);
  if (SmmFlag != NULL) {
    *SmmFlag = FALSE;
  }

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS sends the message to PSP to fusing PSB fuse

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosPsbAutoFusing (
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PsbAutoFusing\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdPsbAutoFusing);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send these commands to PSP to start/stop Address Range Scrubber (ARS)
 *
 * @param[in]  Action              Action to preform (start/stop ARS)
 * @param[in]  OperationBuf        Point to ARS buffer
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosArs (
  IN UINT8       Action,
  IN ARS_BUFFER  *OperationBuf,
  IN UINT8       *SmmBuffer,
  IN BOOLEAN     *SmmFlag
  )
{
  EFI_STATUS        Status;
  MBOX_ARS_CMD_BUF  *Buffer;
  BOOLEAN           CmdSts;

  CmdSts = FALSE;
  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;

  switch (Action) {
  case ArsActionStart:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.StartArs\n");
    if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
      IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
      Buffer = (MBOX_ARS_CMD_BUF*)SmmBuffer;
      ZeroMem (Buffer, sizeof (MBOX_ARS_CMD_BUF));
      Buffer->Header.TotalSize = sizeof (MBOX_ARS_CMD_BUF);
      CopyMem (&Buffer->OperationBuf, OperationBuf, sizeof (ARS_BUFFER));
      *SmmFlag = TRUE;
      CmdSts = SendPspCommand (Buffer, MboxBiosCmdStartArs);
      *SmmFlag = FALSE;
      IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
      if (CmdSts == FALSE) {
        return EFI_DEVICE_ERROR;
      }
      Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    }
    break;
  case ArsActionStop:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.StopArs\n");
    if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
      IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
      *SmmFlag = TRUE;
      Buffer = (MBOX_ARS_CMD_BUF*)SmmBuffer;
      ZeroMem (Buffer, sizeof (MBOX_ARS_CMD_BUF));
      Buffer->Header.TotalSize = sizeof (MBOX_ARS_CMD_BUF);
      CopyMem (&Buffer->OperationBuf, OperationBuf, sizeof (ARS_BUFFER));
      CmdSts = SendPspCommand (Buffer, MboxBiosCmdStopArs);
      *SmmFlag = FALSE;
      IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
      if (CmdSts == FALSE) {
        return EFI_DEVICE_ERROR;
      }
      Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    }
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }
  return Status;
}

/**
 *
 *
 * Bios send this command to Get ActiveBootPartitionId
 *
 * @param[in]  BootPartitionId    Partition Index of PSP L1 Directory
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosGetActiveBootPartitionId (
  IN OUT UINT32  *BootPartitionId
  )
{
  UNALIGNED_MBOX_BUFFER                 UBuff;            // Unaligned buffer
  MBOX_ACTIVE_BOOT_PARTITION_ID_BUFFER  *Buffer;
  BOOLEAN                               CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.GetActiveBootPartitionId\n");

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_ACTIVE_BOOT_PARTITION_ID_BUFFER);
  Buffer->Header.Status         = 0;
  Buffer->BootPartitionId       = 0xFFFFFFFF;

  CmdSts = SendPspCommandSmn (Buffer, MboxBiosCmdGetBootPartitionId, TRUE);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  *BootPartitionId = Buffer->BootPartitionId;

  IDS_HDT_CONSOLE_PSP_TRACE ("ActiveBootPartitionId from PSP:%d\n", Buffer->BootPartitionId);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send this command to Set ActiveBootPartitionId
 *
 * @param[in]  BootPartitionId     Partition Index of PSP L1 Directory
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosSetActiveBootPartitionId (
  IN UINT32  BootPartitionId
  )
{
  UNALIGNED_MBOX_BUFFER                 UBuff;            // Unaligned buffer
  MBOX_ACTIVE_BOOT_PARTITION_ID_BUFFER  *Buffer;
  BOOLEAN                               CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SetBootPartitionId 0x%x\n", BootPartitionId);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_ACTIVE_BOOT_PARTITION_ID_BUFFER);
  Buffer->Header.Status         = 0;
  Buffer->BootPartitionId = BootPartitionId;

  CmdSts = SendPspCommandSmn (Buffer, MboxBiosCmdSetBootPartitionId, TRUE);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS check the PSP Capabilities from the PSP

   @param[in]  Capabilities      PSP Writes capabilities into this field when it returns.

   @retval EFI_STATUS                0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxPspCapsQuery (
  IN OUT UINT32  *PspCapabilities
  )
{
  MBOX_PSP_CAPS_BUFFER   *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;            // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.MboxBiosCmdPspCapsQuery\n");

  if (PspCapabilities == NULL) {
    return (EFI_NOT_FOUND);
  }

  Buffer->Header.TotalSize = sizeof (MBOX_PSP_CAPS_BUFFER);

  Buffer->PspCapabilities = 0; // Result of command will be placed here

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdPspCapsQuery);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  *PspCapabilities = Buffer->PspCapabilities;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp Capabilities return %x\n", Buffer->PspCapabilities);
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief Get SFFS fuse from PSP
 *
 * @param SffsFuse      SFFS fuse
 * @return EFI_STATUS   0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosQuerySffsFuse (
  IN OUT   UINT32 *SffsFuse
  )
{
  return PspMboxBiosQueryUint32 (MboxBiosCmdGetSffsFuse, SffsFuse);
}

/**
 *
 *
 * BIOS send this command to PSP about the information DRTM feature required
 *
 * @param[in]  DrtmInfo         Address of DrtmInfo Structure
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosDrtmInfo (
  IN DRTM_INFO  *DrtmInfo
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;                           // Unaligned buffer
  MBOX_DRTM_INFO_BUFFER  *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE (
    "Psp.C2PMbox.PspMboxBiosDrtmInfo 0x%x_%x 0x%x, 0x%x_%x 0x%x, 0x%x_%x 0x%x, 0x%x_%x, 0x%x_%x\n", \
    DrtmInfo->SmmSupvAddrHi, DrtmInfo->SmmSupvAddrLo, DrtmInfo->SmmSupvSize, \
    DrtmInfo->SmmSupvDirAddrHi, DrtmInfo->SmmSupvDirAddrLo, DrtmInfo->SmmSupvDirSize, \
    DrtmInfo->SmmPolicyDataAddrHi,  DrtmInfo->SmmPolicyDataAddrLo, DrtmInfo->SmmPolicyDataSize, \
    DrtmInfo->SmmSupvSmmEntryAddrHi, DrtmInfo->SmmSupvSmmEntryAddrLo, \
    DrtmInfo->SmmSupvSmmExitAddrHi, DrtmInfo->SmmSupvSmmExitAddrLo);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_DRTM_INFO_BUFFER);
  Buffer->Header.Status         = 0;

  Buffer->DrtmInfo.SmmSupvAddrLo =  DrtmInfo->SmmSupvAddrLo;       ///< Address of SmmSupv driver
  Buffer->DrtmInfo.SmmSupvAddrHi =  DrtmInfo->SmmSupvAddrHi;       ///< Address of SmmSupv driver
  Buffer->DrtmInfo.SmmSupvSize =  DrtmInfo->SmmSupvSize;       ///< Size of SmmSupv driver

  Buffer->DrtmInfo.SmmSupvDirAddrLo =  DrtmInfo->SmmSupvDirAddrLo;    ///< Address of SmmSupv directory
  Buffer->DrtmInfo.SmmSupvDirAddrHi =  DrtmInfo->SmmSupvDirAddrHi;    ///< Address of SmmSupv directory
  Buffer->DrtmInfo.SmmSupvDirSize =  DrtmInfo->SmmSupvDirSize;    ///< Size of SmmSupv directory

  Buffer->DrtmInfo.SmmPolicyDataAddrLo =  DrtmInfo->SmmPolicyDataAddrLo; ///< Address of SmmPolicy data
  Buffer->DrtmInfo.SmmPolicyDataAddrHi =  DrtmInfo->SmmPolicyDataAddrHi; ///< Address of SmmPolicy data
  Buffer->DrtmInfo.SmmPolicyDataSize =  DrtmInfo->SmmPolicyDataSize; ///< Size of SmmPolicy data

  Buffer->DrtmInfo.SmmSupvSmmEntryAddrLo =  DrtmInfo->SmmSupvSmmEntryAddrLo;       ///< Address of SmmSupvSmmEntry Struct
  Buffer->DrtmInfo.SmmSupvSmmEntryAddrHi =  DrtmInfo->SmmSupvSmmEntryAddrHi;       ///< Address of SmmSupvSmmEntry Struct

  Buffer->DrtmInfo.SmmSupvSmmExitAddrLo =  DrtmInfo->SmmSupvSmmExitAddrLo;       ///< Address of SmmSupvSmmExit Struct
  Buffer->DrtmInfo.SmmSupvSmmExitAddrHi =  DrtmInfo->SmmSupvSmmExitAddrHi;       ///< Address of SmmSupvSmmExit Struct

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdDrtmInfoId);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS to PSP: To query updated drtm information in post drtm phase.

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdPostDrtmInfoQuery (
  IN DRTM_INFO  *DrtmInfo,
  IN UINT8      *SmmBuffer,
  IN BOOLEAN    *SmmFlag
  )
{
  EFI_STATUS             Status;
  MBOX_DRTM_INFO_BUFFER  *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PostDrtmInfoQuery\n");
  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
    Buffer = (MBOX_DRTM_INFO_BUFFER*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_DRTM_INFO_BUFFER));
    Buffer->Header.TotalSize      = sizeof (MBOX_DRTM_INFO_BUFFER);
    *SmmFlag = TRUE;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdPostDrtmInfoQuery);
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    if (EFI_SUCCESS == Status) {
      CopyMem (DrtmInfo, &Buffer->DrtmInfo, sizeof (DRTM_INFO));
      IDS_HDT_CONSOLE_PSP_TRACE ("PostDrtmInfoQuery 0x%x_%x 0x%x, 0x%x_%x 0x%x, 0x%x_%x 0x%x, 0x%x_%x\n", \
                                 DrtmInfo->SmmSupvAddrHi, DrtmInfo->SmmSupvAddrLo, DrtmInfo->SmmSupvSize, \
                                 DrtmInfo->SmmSupvDirAddrHi, DrtmInfo->SmmSupvDirAddrLo, DrtmInfo->SmmSupvDirSize, \
                                 DrtmInfo->SmmPolicyDataAddrHi,  DrtmInfo->SmmPolicyDataAddrLo,
                                 DrtmInfo->SmmPolicyDataSize, \
                                 DrtmInfo->SmmSupvSmmEntryAddrHi, DrtmInfo->SmmSupvSmmEntryAddrLo);
    }
  }
  return Status;
}

/**
   BIOS sends the message to PSP for late SPL fuse

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxLateSplFusing (
  IN UINT32  SplValue
  )
{
  MBOX_FIRMWARE_ANTI_ROLLBACK_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER               UBuff;       // Unaligned buffer
  BOOLEAN                             CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.LaterSpiFusing\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);
  Buffer->SplValue = SplValue;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLaterSplFuse);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send IVRS buffer to PSP, PSP save it, then AMDSL will use another command to retrieve it back to the buffer.
 *
 * @param[in]  IvrsAcpiTableInfo         Address of IVRS_ACPI_TABLE_INFO Structure
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosSendIvrsAcpiTable (
  IN IVRS_ACPI_TABLE_INFO  *IvrsAcpiTableInfo
  )
{
  UNALIGNED_MBOX_BUFFER             UBuff;                // Unaligned buffer
  MBOX_Ivrs_Acpi_Table_INFO_BUFFER  *Buffer;
  BOOLEAN                           CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosSendIvrsAcpiTable 0x%x_%x 0x%x\n", \
                             IvrsAcpiTableInfo->IvrsBufferAddrHi, IvrsAcpiTableInfo->IvrsBufferAddrLo,
                             IvrsAcpiTableInfo->IvrsAcpiTableSize);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_Ivrs_Acpi_Table_INFO_BUFFER);
  Buffer->Header.Status         = 0;

  Buffer->IvrsAcpiTableInfo.IvrsBufferAddrLo =  IvrsAcpiTableInfo->IvrsBufferAddrLo;       ///< Address of Ivrs Buffer
  Buffer->IvrsAcpiTableInfo.IvrsBufferAddrHi =  IvrsAcpiTableInfo->IvrsBufferAddrHi;       ///< Address of Ivrs Buffer
  Buffer->IvrsAcpiTableInfo.IvrsAcpiTableSize =  IvrsAcpiTableInfo->IvrsAcpiTableSize;     ///< Size of Ivrs Buffer

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSendIvrsAcpiTable);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send these commands to PSP for Rom Armor support
 *
 * @param[in]  Action              Action to preform
 * @param[in]  ChipSelect          SPI Controller Chip Select
 * @param[in]  OperationBuf        Point to Armor TSEG buffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosArmor (
  IN UINT8    Action,
  IN UINT8    ChipSelect,
  IN VOID     *OperationBuf,
  IN BOOLEAN  *SmmFlag
  )
{
  EFI_STATUS              Status;
  MBOX_ROM_ARMOR_CMD_BUF  Buffer;

  Status = EFI_INVALID_PARAMETER;

  IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag Address: 0x%08x\n",  SmmFlag);

  if (SmmFlag == NULL) {
    return Status;
  }

  switch (Action) {
  case ArmorEnterSmmOnlyMode:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.EnterSmmOnlyMode\n");
    ZeroMem (&Buffer, sizeof (MBOX_ROM_ARMOR_CMD_BUF));
    Buffer.Header.TotalSize = sizeof (MBOX_ROM_ARMOR_CMD_BUF);
    Buffer.RomArmorCmdBuffer.TsegAddress = (UINT64)(UINTN)OperationBuf;
    Buffer.RomArmorCmdBuffer.ChipSelect = ChipSelect;
    *SmmFlag = TRUE;
    if (FALSE == SendPspCommand (&Buffer, MboxBiosCmdArmorEnterSmmOnlyMode)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("EnterSmmOnlyMode Command Error!!!\n");
      //Command Error
      *SmmFlag = FALSE;
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer.Header.Status);
    Status = (Buffer.Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    break;
  case ArmorEnforceWhitelist:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.EnforceWhitelist\n");
    ZeroMem (&Buffer, sizeof (MBOX_ROM_ARMOR_CMD_BUF));
    Buffer.Header.TotalSize = sizeof (MBOX_ROM_ARMOR_CMD_BUF);
    Buffer.RomArmorCmdBuffer.TsegAddress = (UINT64)(UINTN)OperationBuf;
    Buffer.RomArmorCmdBuffer.ChipSelect = ChipSelect;
    *SmmFlag = TRUE;
    if (FALSE == SendPspCommand (&Buffer, MboxBiosCmdArmorEnforceWhitelist)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("EnforceWhitelist Command Error!!!\n");
      //Command Error
      *SmmFlag = FALSE;
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer.Header.Status);
    Status = (Buffer.Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    break;
  case ArmorExecuteSpiCommand:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.ExecuteSpiCommand\n");
    ZeroMem (&Buffer, sizeof (MBOX_ROM_ARMOR_CMD_BUF));
    Buffer.Header.TotalSize = sizeof (MBOX_ROM_ARMOR_CMD_BUF);
    Buffer.RomArmorCmdBuffer.TsegAddress = 0;
    Buffer.RomArmorCmdBuffer.ChipSelect = 0;
    *SmmFlag = TRUE;
    if (FALSE == SendPspCommand (&Buffer, MboxBiosCmdArmorExecuteSpiCommand)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("ExecuteSpiCommand Command Error!!!\n");
      //Command Error
      *SmmFlag = FALSE;
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer.Header.Status);
    Status = (Buffer.Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    break;
  case ArmorSwitchCsMode:
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SwitchCsMode\n");
    ZeroMem (&Buffer, sizeof (MBOX_ROM_ARMOR_CMD_BUF));
    Buffer.Header.TotalSize = sizeof (MBOX_ROM_ARMOR_CMD_BUF);
    Buffer.RomArmorCmdBuffer.TsegAddress = 0;
    Buffer.RomArmorCmdBuffer.ChipSelect = ChipSelect;
    *SmmFlag = TRUE;
    if (FALSE == SendPspCommand (&Buffer, MboxBiosCmdArmorSwitchCsMode)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SwitchCsMode Command Error!!!\n");
      //Command Error
      *SmmFlag = FALSE;
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer.Header.Status);
    Status = (Buffer.Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    break;
  default:
    Status = EFI_UNSUPPORTED;
    break;
  }
  return Status;
}

/**
 *
 *
 * Bios send these commands to PSP to grant dTPM status and event log
 *
 * @param[out]      DesiredConfig       dTPM configuration requested
 * @param[out]      ConfigStatus        0 - success. non-zero failure.
 * @param[in/out]   LogDataSize         Size of LogData buffer
 * @param[out]      LogData             Point to allocated event log buffer
 *
 * @retval EFI_STATUS                   0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxGetDTPMData (
  OUT UINT32     *DesiredConfig,
  OUT UINT32     *ConfigStatus,
  IN OUT UINT32  *LogDataSize,
  OUT VOID       *LogData
  )
{
  EFI_STATUS          Status;
  MBOX_DTPM_INFO_BUF  MboxDtpmInfoBuf;

  Status = EFI_INVALID_PARAMETER;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.GetDTPMData\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("LogData Address: 0x%08x\n",  LogData);
  ZeroMem (&MboxDtpmInfoBuf, sizeof (MBOX_DTPM_INFO_BUF));
  MboxDtpmInfoBuf.Header.TotalSize = sizeof (MBOX_DTPM_INFO_BUF);

  MboxDtpmInfoBuf.MboxDtpmInfo.LogDataSize = *LogDataSize;
  MboxDtpmInfoBuf.MboxDtpmInfo.LogData = (UINT64)(UINTN)LogData;

  if (!SendPspCommand (&MboxDtpmInfoBuf, MboxBiosCmdDtpmInfo)) {
    //command failed
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", MboxDtpmInfoBuf.Header.Status);

  *DesiredConfig = MboxDtpmInfoBuf.MboxDtpmInfo.DesiredConfig;
  *ConfigStatus = MboxDtpmInfoBuf.MboxDtpmInfo.ConfigStatus;
  *LogDataSize = MboxDtpmInfoBuf.MboxDtpmInfo.LogDataSize;

  //check PSP return status.
  switch (MboxDtpmInfoBuf.Header.Status) {
  case  BL_OK:
    Status = EFI_SUCCESS;
    break;
  case  BL_ERR_DATA_LENGTH:
    Status = EFI_WARN_BUFFER_TOO_SMALL;
    break;
  case  BL_ERR_INVALID_ADDRESS:
    Status = EFI_INVALID_PARAMETER;
    break;
  default:
    Status = EFI_DEVICE_ERROR;
    break;
  }
  return Status;
}


/**
   BIOS will send this command to PSP for getting Tcg Logs
   - BIOS sends the Query TCG logs to PSP with Address and size both zero.
   - PSP will use reserve x86 shared memory to copy the TCG logs
   - PSP will response the command by filling physical memory and size.
   - BIOS will read the TCG logs from Physical address provided by PSP

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxGetTcgLogs (
  IN OUT UINT32  *TcgsBufferPhyAddressLo,
  IN OUT UINT32  *TcgsBufferPhyAddressHi,
  IN OUT UINT32  *TcgsBufferSize
  )
{
  MBOX_GET_TCG_LOGS      *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts = FALSE;

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxGetTcgLogs\n");

  Buffer->Header.TotalSize = sizeof (MBOX_GET_TCG_LOGS);

  Buffer->TcgsBufferPhyAddressLo = 0;
  Buffer->TcgsBufferPhyAddressHi = 0;
  Buffer->TcgsBufferSize         = 0;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdQueryTCGLog);
  if (CmdSts == FALSE) {
    return (EFI_DEVICE_ERROR);
  }

  *TcgsBufferPhyAddressLo = Buffer->TcgsBufferPhyAddressLo;
  *TcgsBufferPhyAddressHi = Buffer->TcgsBufferPhyAddressHi;
  *TcgsBufferSize         = Buffer->TcgsBufferSize;

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x TcgsBufferSize:0x%x\n", Buffer->Header.Status, Buffer->TcgsBufferSize);
  return ((Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS);
}

/**
 * @brief  BIOS will send this command to PSP for getting the current value of SPL_F (FW_ROLLBACK_CNT) fuse value
 *
 * @param SplBuffer  Buffer to get the spl values
 * @return EFI_STATUS             0: Success, NonZero Error
 */
EFI_STATUS
PspMboxQuerySplFuseMp (
  IN OUT GET_SPL_BUFFER *SplBuffer
  )
{
  MBOX_GET_SPL_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts = FALSE;

  if (SplBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxQuerySplFuseMp\n");

  Buffer->Header.TotalSize = sizeof (MBOX_GET_SPL_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdQuerySplFuse);
  if (CmdSts == FALSE) {
    return (EFI_DEVICE_ERROR);
  }

  CopyMem (SplBuffer, &Buffer->SplBuffer, sizeof (GET_SPL_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return ((Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS);
}

/**
   BIOS will send this command to PSP for getting the current value of SPL_F (FW_ROLLBACK_CNT) fuse value
   used for client program previously, keep for compatibility

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxQuerySplFuse (
  IN OUT UINT32  *SplFuse0,
  IN OUT UINT32  *SplFuse1,
  IN OUT UINT32  *SplFuse2,
  IN OUT UINT32  *SplFuse3
  )
{
  EFI_STATUS Status;
  GET_SPL_BUFFER SplBuffer;

  ZeroMem (&SplBuffer, sizeof (GET_SPL_BUFFER));
  Status = PspMboxQuerySplFuseMp (&SplBuffer);
  if (Status == EFI_SUCCESS) {
    *SplFuse0 = SplBuffer.SplFuse[0][0];
    *SplFuse1 = SplBuffer.SplFuse[0][1];
    *SplFuse2 = SplBuffer.SplFuse[0][2];
    *SplFuse3 = SplBuffer.SplFuse[0][3];
  }
  return Status;
}


/**
   BIOS will send this command to PSP for getting the current value of loaded SPL Value.

   @param[out]  SplValue           Loaded SPL Value

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxQuerySplValue (
  OUT UINT32  *SplValue
  )
{
  MBOX_GET_SPL_VALUE_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER      UBuff;     // Unaligned buffer
  BOOLEAN                    CmdSts = FALSE;

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosCmdQuerySplValue\n");

  Buffer->Header.TotalSize = sizeof (MBOX_GET_SPL_VALUE_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdQuerySplValue);
  if (CmdSts == FALSE) {
    return (EFI_DEVICE_ERROR);
  }

  *SplValue = Buffer->SplValue;

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x Loaded SPL Value:0x%x\n", Buffer->Header.Status, *SplValue);
  return ((Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS);
}

/**
   BIOS will send this command to PSP for getting Manageability related Config
 * @param[in,out]  Cfg point to the address to hold MANAGEABILITY_CFG structure
    If return status is success, the structure will be filled with information from PSP FW.

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxManageabilityCfg (
  MANAGEABILITY_CFG  *Cfg
  )
{
  MBOX_MANAGEABILITY_CFG_BUFFER  *Buffer;
  UNALIGNED_MBOX_BUFFER          UBuff; // Unaligned buffer
  BOOLEAN                        CmdSts = FALSE;

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxManageabilityCfg\n");

  Buffer->Header.TotalSize = sizeof (MBOX_MANAGEABILITY_CFG_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdManageabilityCfg);
  if (CmdSts == FALSE) {
    return (EFI_DEVICE_ERROR);
  }

  Cfg->ManageabilityConfig = Buffer->Cfg.ManageabilityConfig;
  Cfg->MpmDramAddrLo = Buffer->Cfg.MpmDramAddrLo;
  Cfg->MpmDramAddrHi = Buffer->Cfg.MpmDramAddrHi;

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x ManageabilityConfig:0x%x MpmDramAddrLo:0x%x MpmDramAddrHi:0x%x\n", \
                             Buffer->Header.Status, Cfg->ManageabilityConfig, Cfg->MpmDramAddrLo, Cfg->MpmDramAddrHi);
  return ((Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS);
}

/**
   BIOS send this command to PSP for setting SPI controller related Config

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxSetCfg (
  VOID
  )
{
  MBOX_GENERIC_CONFIG_BUFFER Buffer;
  BOOLEAN                    CmdSts = FALSE;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxSetCfg\n");
  ZeroMem (&Buffer, sizeof (MBOX_GENERIC_CONFIG_BUFFER));
  Buffer.Header.TotalSize = sizeof (MBOX_GENERIC_CONFIG_BUFFER);
  Buffer.Cfg.ConfigId = CMD_CONFIG_ID_OPEN_CS_NO_SPI_LOCK;
  Buffer.Cfg.Args0 = 1;

  CmdSts = SendPspCommand (&Buffer, MboxBiosCmdSetCfg);
  if (CmdSts == FALSE) {
    return (EFI_DEVICE_ERROR);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer.Header.Status);
  return ((Buffer.Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS);
}

/**
   BIOS send this command to PSP to update USB configuration

   @param[in out]  UsbConfigureData       Pointer to Usb Configure Data


   @retval EFI_STATUS                     0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdUsbConfig (
  IN OUT USB_CONFIG_DATA  *UsbConfigureData,
  IN UINT8                *SmmBuffer,
  IN BOOLEAN              *SmmFlag
  )
{
  EFI_STATUS              Status;
  MBOX_USB_CONFIG_BUFFER  *Buffer;
  BOOLEAN                 CmdSts;
  UINT64                  FilterFlag;

  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;
  CmdSts = FALSE;
  FilterFlag = 0; //change it to PSP_TRACE if you want logs

  IDS_HDT_CONSOLE (FilterFlag, "Psp.C2PMbox.UsbConfig\n");

  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE (FilterFlag, "SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE (FilterFlag, "SmmFlag: %x\n",  SmmFlag);
    Buffer = (MBOX_USB_CONFIG_BUFFER*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_USB_CONFIG_BUFFER));
    Buffer->Header.TotalSize                 = sizeof (MBOX_USB_CONFIG_BUFFER);
    Buffer->MboxUsbConfig.UsbConfigureOp     = UsbConfigureData->UsbConfigureOp;
    Buffer->MboxUsbConfig.UsbRegisterID      = UsbConfigureData->UsbRegisterID;
    Buffer->MboxUsbConfig.UsbConfigureData0  = UsbConfigureData->UsbConfigureData0;
    Buffer->MboxUsbConfig.UsbConfigureData1  = UsbConfigureData->UsbConfigureData1;
    *SmmFlag = TRUE;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdUsbConfig);
    *SmmFlag = FALSE;
    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    IDS_HDT_CONSOLE (FilterFlag, "Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    if (Status == EFI_SUCCESS) {
      if (UsbConfigureData->UsbConfigureOp == MBOX_USB_CONFIG_OP_RD) {
        UsbConfigureData->UsbConfigureData0 = Buffer->MboxUsbConfig.UsbConfigureData0;
      }
    }
  }

  return Status;
}


/**
   BIOS Command to PSP to enabled or disable the PCI access to MPM according to the input parameter

   @param[in out]  AccessFlag       Enable (1) or Disable (0) PCI Access to MPM
   @param[in out]  WirelessFlag       Enable (1) or Disable (0) PCI Access to MPM
   @retval EFI_STATUS  0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosMpmPciAccess (
  IN UINT32  AccessFlag,
  IN UINT32  WirelessFlag
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;          // Unaligned buffer
  MBOX_MPM_PCI_ACCESS    *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosMpmPciAccess 0x%x, WirelessFlag 0x%x\n", AccessFlag,
                             WirelessFlag);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_MPM_PCI_ACCESS);
  Buffer->Header.Status         = 0;
  Buffer->AccessFlag = AccessFlag;
  Buffer->WirelessFlag = WirelessFlag;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdMpmPciAccess);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 *
 *
 * Bios send these commands to PSP to disable Platform Secure Boot
 *
 * @param[out]      Result              0 - success. non-zero.
 *
 * @retval EFI_STATUS                   0: Success, NonZero Error
 *
 **/
EFI_STATUS
PspMboxBiosDisablePsb (
  OUT UINT32  *Result
  )
{
  EFI_STATUS               Status;
  MBOX_DIS_PSB_CMD_BUFFER  MboxDisPsbCmdBuffer;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.BiosDisablePsb\n");
  ZeroMem (&MboxDisPsbCmdBuffer, sizeof (MBOX_DIS_PSB_CMD_BUFFER));
  MboxDisPsbCmdBuffer.Header.TotalSize = sizeof (MBOX_DIS_PSB_CMD_BUFFER);

  MboxDisPsbCmdBuffer.PsbDisableVerifyId = PSB_DISABLE_VERIFY_ID;

  if (!SendPspCommand (&MboxDisPsbCmdBuffer, MboxBiosCmdDisablePsb)) {
    //command failed
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", MboxDisPsbCmdBuffer.Header.Status);

  *Result = MboxDisPsbCmdBuffer.Result;
  return Status;
}

/**
   Bios send these commands to PSP to deferred Platform Secure Boot fuse
   @retval EFI_STATUS                   0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosDeferredPsbFuse (
  VOID
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.DeferredPsbFuse\n");

  Buffer->Header.TotalSize = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdDeferredPsbFuse);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   Sign data with Hmac256 or validate its signature before SMM
 * @param[in]  Data                Point to the data to be protected
   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdSignValidateHmacDataPreSmm (
  IN OUT HMAC_PROTECTED_DATA  *Data
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  MBOX_HMAC_DATA_BUFFER  *Buffer;

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SignValidateHmacDataPreSmm\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("Data before command %x: 0x%x_%x %x, 0x%x %x, 0x%x %x, %x\n",
                             MboxBiosCmdSignValidateHmacDataPreSmm, Data->BufferAddrHi, Data->BufferddrLo,
                             Data->BufferSize,
                             Data->DataOffset, Data->DataSize,
                             Data->SignatureOffset, Data->SignatureSize, Data->Validate);

  Buffer->Header.TotalSize      = sizeof (UNALIGNED_MBOX_BUFFER);
  Buffer->Data.BufferAddrHi     = Data->BufferAddrHi;
  Buffer->Data.BufferddrLo      = Data->BufferddrLo;
  Buffer->Data.BufferSize       = Data->BufferSize;
  Buffer->Data.DataOffset       = Data->DataOffset;
  Buffer->Data.DataSize         = Data->DataSize;
  Buffer->Data.SignatureOffset  = Data->SignatureOffset;
  Buffer->Data.SignatureSize    = Data->SignatureSize;
  Buffer->Data.Validate         = Data->Validate;

  if (!SendPspCommand (Buffer, MboxBiosCmdSignValidateHmacDataPreSmm)) {
    //command failed
    return EFI_DEVICE_ERROR;
  }
  CopyMem (Data, &Buffer->Data, sizeof (HMAC_PROTECTED_DATA));
  IDS_HDT_CONSOLE_PSP_TRACE ("Data after command %x: 0x%x_%x 0x%x, 0x%x 0x%x, 0x%x 0x%x, 0x%x\n", \
                             MboxBiosCmdSignValidateHmacDataPreSmm, Data->BufferAddrHi, Data->BufferddrLo,
                             Data->BufferSize, \
                             Data->DataOffset, Data->DataSize, \
                             Data->SignatureOffset,  Data->SignatureSize, Data->Validate);

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   Sign data with Hmac256 or validate its signature in SMM
 * @param[in]  Data                Point to the data to be protected
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosCmdSignValidateHmacDataSmm (
  IN OUT HMAC_PROTECTED_DATA  *Data,
  IN UINT8                    *SmmBuffer,
  IN BOOLEAN                  *SmmFlag
  )
{
  EFI_STATUS             Status;
  MBOX_HMAC_DATA_BUFFER  *Buffer;

  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SignValidateHmacDataSmm\n");
  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Data before command %x: 0x%x_%x %x, 0x%x %x, 0x%x %x, %x\n",
                               MboxBiosCmdSignValidateHmacDataSmm, Data->BufferAddrHi,
                               Data->BufferddrLo, Data->BufferSize,
                               Data->DataOffset, Data->DataSize,
                               Data->SignatureOffset, Data->SignatureSize, Data->Validate);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
    Buffer = (MBOX_HMAC_DATA_BUFFER*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_HMAC_DATA_BUFFER));
    Buffer->Header.TotalSize      = sizeof (MBOX_HMAC_DATA_BUFFER);
    Buffer->Data.BufferAddrHi     = Data->BufferAddrHi;
    Buffer->Data.BufferddrLo      = Data->BufferddrLo;
    Buffer->Data.BufferSize       = Data->BufferSize;
    Buffer->Data.DataOffset       = Data->DataOffset;
    Buffer->Data.DataSize         = Data->DataSize;
    Buffer->Data.SignatureOffset  = Data->SignatureOffset;
    Buffer->Data.SignatureSize    = Data->SignatureSize;
    Buffer->Data.Validate         = Data->Validate;
    *SmmFlag = TRUE;
    if (!SendPspCommand (Buffer, MboxBiosCmdSignValidateHmacDataSmm)) {
      *SmmFlag = FALSE;
      //command failed
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    Status = (Buffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
    if (EFI_SUCCESS == Status) {
      CopyMem (Data, &Buffer->Data, sizeof (HMAC_PROTECTED_DATA));
      IDS_HDT_CONSOLE_PSP_TRACE ("Data after command %x: 0x%x_%x 0x%x, 0x%x 0x%x, 0x%x 0x%x, 0x%x\n", \
                                 MboxBiosCmdSignValidateHmacDataSmm, Data->BufferAddrHi, Data->BufferddrLo,
                                 Data->BufferSize, \
                                 Data->DataOffset, Data->DataSize, \
                                 Data->SignatureOffset,  Data->SignatureSize, Data->Validate);
    }
  }
  return Status;
}


/**
   BIOS Command to PSP to send Smart trace buffer Verbosity Control

   @param[in out]  Verbosity       Valid value: LOW_LEVEL_VERBOSITY,PROD_LEVEL_VERBOSITY,HIGH_LEVEL_VERBOSITY

   @retval EFI_STATUS  0: Success, NonZero Error

 **/
EFI_STATUS
PspMboxBiosStbVerbosity (
  IN UINT8  Verbosity
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;          // Unaligned buffer
  MBOX_STB_VERBOSITY     *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosStbVerbosity 0x%x\n", Verbosity);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_STB_VERBOSITY);
  Buffer->Header.Status         = 0;
  Buffer->Verbosity = (UINT32) Verbosity;

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdStbVerbosity);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * Enter RomArmor 2
 *
 * @param[in]  RomArmor2EnforceBuffer RomArmor2 Enforce buffer
 * @param[in]  SmmFlag                Point to allocated SmmFlag
 * @retval EFI_SUCCESS                Initial success
 * @retval Others                     Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdArmorEnterSmmOnlyMode2 (
  IN OUT MBOX_ROM_ARMOR_ENFORCE  *RomArmor2EnforceBuffer,
  IN BOOLEAN                     *SmmFlag
  )
{
  EFI_STATUS  Status;
  Status = EFI_INVALID_PARAMETER;
  if ((RomArmor2EnforceBuffer != NULL) && (SmmFlag != NULL)) {
    RomArmor2EnforceBuffer->Header.TotalSize = sizeof (MBOX_ROM_ARMOR_ENFORCE);

    *SmmFlag = TRUE;
    if (FALSE == SendPspCommand (RomArmor2EnforceBuffer, MboxBiosCmdArmorEnterSmmOnlyMode2)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("ArmorEnterSmmOnlyMode2 Command Error! Mbox.Hdr.Sts:0x%x\n",
                                 RomArmor2EnforceBuffer->Header.Status);
      //Command Error
      *SmmFlag = FALSE;
      return EFI_DEVICE_ERROR;
    }
    *SmmFlag = FALSE;

    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", RomArmor2EnforceBuffer->Header.Status);
    Status = (RomArmor2EnforceBuffer->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
  }
  return Status;
}

/**
 * Request execute SPI Transction provide in TSEG Transaction buffer.
 *
 * @param[in]  SpiTransction      SPi Transaction buffer
 * @param[in]  SmmFlag            Point to allocated SmmFlag
 * @retval EFI_SUCCESS            Initial success
 * @retval Others                 Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdArmorSpiTransction (
  IN MBOX_ROM_ARMOR_FLASH_COMMAND_BUFFER  *SpiTransction,
  IN BOOLEAN                              *SmmFlag
  )
{
  EFI_STATUS  Status;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.ArmorSpiTransction\n");
  *SmmFlag = TRUE;
  if (FALSE == SendPspCommand (SpiTransction, MboxBiosCmdArmorSpiTransaction)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ArmorSpiTransction Command Error! Mbox.Hdr.Sts:0x%x\n", SpiTransction->Header.Status);
    //Command Error
    *SmmFlag = FALSE;
    return EFI_DEVICE_ERROR;
  }
  *SmmFlag = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", SpiTransction->Header.Status);
  Status = (SpiTransction->Header.Status) ? EFI_DEVICE_ERROR : EFI_SUCCESS;

  return Status;
}



/**
 * X86 Allocate temporary memory, copy the WLAN FW from SPI to temp memory, allocate additional 64M DMA memory.
 * Send MboxBiosCmdLoadWlanFw to PSP FW, PSP will do the decompress & authentication, then copy the decompressed image to 64M DMA memory
 *
 * @param[in]  WlanFwBuf        Structure contain WLAN Buffer information
 * @retval EFI_SUCCESS            Initial success
 * @retval Others                 Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdLoadWlanFw (
  IN WLAN_FW_BUF  *WlanFwBuf
  )
{
  UNALIGNED_MBOX_BUFFER  UBuff;          // Unaligned buffer
  MBOX_LOAD_WLAN_FW      *Buffer;
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosCmdLoadWlanFw \n");

  IDS_HDT_CONSOLE_PSP_TRACE ("TempDramAddrLo %x\n", WlanFwBuf->TempDramAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("TempDramAddrHi %x\n", WlanFwBuf->TempDramAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("TempDramSize %x\n", WlanFwBuf->TempDramSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("AllocatedDramAddrLo %x\n", WlanFwBuf->AllocatedDramAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("AllocatedDramAddrHi %x\n", WlanFwBuf->AllocatedDramAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("AllocatedDramSize %x\n", WlanFwBuf->AllocatedDramSize);


  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_LOAD_WLAN_FW);
  Buffer->Header.Status         = 0;
  CopyMem (&Buffer->WlanFwBuf, WlanFwBuf, sizeof (WLAN_FW_BUF));

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLoadWlanFw);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief BIOS send this command to PSP to load PROM21 and TURNER FW Configuration
 *
 * @param FwConfigBuf  FW configuration buffer
 * @return EFI_STATUS  0: Success, NonZero Error
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdLoadFwConfig (
  IN LOAD_FW_CONFIG_DATA *FwConfigBuf
  )
{
  UNALIGNED_MBOX_BUFFER       UBuff;          // Unaligned buffer
  MBOX_LOAD_FW_CONFIG_BUFFER  *Buffer;
  BOOLEAN                     CmdSts;

  CmdSts = FALSE;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.PspMboxBiosCmdLoadFwConfig \n");

  IDS_HDT_CONSOLE_PSP_TRACE ("OperationCode %x\n", FwConfigBuf->OperationCode);
  IDS_HDT_CONSOLE_PSP_TRACE ("AvailableMemoryAddress %x\n", FwConfigBuf->AvailableMemoryAddress);
  IDS_HDT_CONSOLE_PSP_TRACE ("AvailableMemorySize %x\n", FwConfigBuf->AvailableMemorySize);

  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  Buffer->Header.TotalSize      = sizeof (MBOX_LOAD_FW_CONFIG_BUFFER);
  Buffer->Header.Status         = 0;
  CopyMem (&Buffer->LoadFwConfigData, FwConfigBuf, sizeof (LOAD_FW_CONFIG_DATA));

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdLoadFwConfig);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * BIOS send this command to PSP to Configure Range for encryption
 *
 * @param[in]  ConfigData         Point to Config Data
 *
 * @retval EFI_SUCCESS            Range has been configure successfully
 * @retval Others                 Error happens Configure the Range
 */

EFI_STATUS
PspMboxBiosConfigureRange(
  IN CONFIG_RANGE_DATA  *ConfigData
)
{
  UNALIGNED_MBOX_BUFFER       UBuff;     // Unaligned buffer
  MBOX_CONFIG_RANGE_BUFFER    *Buffer;
  QUERY_RANGE_DATA            QueryBuf;
  BOOLEAN                     CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  Buffer = BALIGN32 (&UBuff);

  IDS_HDT_CONSOLE_PSP_TRACE ("MboxBiosCmdEncryptRangeConfigure\n");

  PspMboxBiosQueryRange(&QueryBuf);

  if (QueryBuf.QueryStatus.Locked) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Range configuration is locked\n");
    return EFI_NOT_READY;
  }

  if (QueryBuf.QueryStatus.Available) {    // Number of ranges that can still be set up

    Buffer->Header.TotalSize      = sizeof (MBOX_CONFIG_RANGE_BUFFER);
    CopyMem (&Buffer->Data, ConfigData, sizeof (CONFIG_RANGE_DATA));

    CmdSts = SendPspCommand (Buffer, MboxBiosCmdEncryptRangeConfigure);

    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    return EFI_SUCCESS;
  }
  return EFI_NOT_FOUND;
}

/**
 * BIOS send this command to PSP to Query Range for encryption

 * @param[in out]  QueryData     Structure contain Query Range Data
 *                               - Version : Version number supported
 *                               - Ranges : Total number of ranges that can be loaded by BIOS
 *                               - Available : Number of ranges that can still be set up
 *                               - Locked : Indicates if range configuration is locked
 * @retval EFI_STATUS            0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosQueryRange(
  IN OUT QUERY_RANGE_DATA *QueryData
)
{
  UNALIGNED_MBOX_BUFFER       UBuff;     // Unaligned buffer
  MBOX_QUERY_RANGE_BUFFER     *QueryBuffer;
  BOOLEAN                     CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
  QueryBuffer = BALIGN32 (&UBuff);

  IDS_HDT_CONSOLE_PSP_TRACE ("MboxBiosCmdEncryptRangeQuery\n");

  QueryBuffer->Header.TotalSize      = sizeof (MBOX_QUERY_RANGE_BUFFER);

  CmdSts = SendPspCommand (QueryBuffer, MboxBiosCmdEncryptRangeQuery);

  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }

  CopyMem (QueryData, &QueryBuffer->Data, sizeof (QUERY_RANGE_DATA));

  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", QueryBuffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS send this command to PSP to check and set a Range Lock

  * @retval EFI_SUCCESS            Indicates range configuration is Unlocked
  * @retval Others                 Indicates range configuration is locked
**/
EFI_STATUS
PspGetBiosRangeLockFlag(
 )
{
  QUERY_RANGE_DATA     QueryBuf;
  UINT8                Flag = 0;
  EFI_STATUS           Status;


  IDS_HDT_CONSOLE_PSP_TRACE ("PspGetBiosRangeLockFlag\n");

  Status = PspMboxBiosQueryRange(&QueryBuf);

  if (EFI_ERROR(Status)) {
    return EFI_DEVICE_ERROR;
  }

  Flag = QueryBuf.QueryStatus.Locked;

  if (Flag) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Range configuration is locked\n");
    return EFI_NOT_READY;
  }
  return EFI_SUCCESS;
}

/**
 * BIOS sends the specified PCD value to PSP FW
 *
 * @param[in]  PcdValues         Point to PCD_VALUES structure
 *
 * @retval EFI_SUCCESS           The specified PCD values have been sent to PSP FW successfully
 * @retval Others                Error happens during sending PCD_VALUES structure to PSP FW
 **/
EFI_STATUS
PspMboxBiosSetPcdValues (
  IN PCD_VALUES  *PcdValues
  )
{
  MBOX_PCD_VALUES         *Buffer;
  UNALIGNED_MBOX_BUFFER   UBuff;     // Unaligned buffer
  BOOLEAN                 CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosSetPcdValues\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_PCD_VALUES);

  Buffer->Req.PcdSyncFloodToApml = PcdValues->PcdSyncFloodToApml;
  Buffer->Req.PcdResetCpuOnSyncFlood = PcdValues->PcdResetCpuOnSyncFlood;

  IDS_HDT_CONSOLE_PSP_TRACE ("[PSP] PspMboxBiosSetPcdValues: PcdSyncFloodToApml - 0x%02x, PcdResetCpuOnSyncFlood - 0x%02x\n",
    PcdValues->PcdSyncFloodToApml, PcdValues->PcdResetCpuOnSyncFlood);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSetPcdValues);
  if (CmdSts == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[PSP] PspMboxBiosSetPcdValues: EFI_DEVICE_ERROR. SendPspCommand: 0x%02x\n", CmdSts);
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("[PSP] PspMboxBiosSetPcdValues: EFI_SUCCESS. Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
   BIOS sends the message to PSP for SMM lock

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosSmmLock (
  )
{
  MBOX_DEFAULT_BUFFER    *Buffer;
  UNALIGNED_MBOX_BUFFER  UBuff;     // Unaligned buffer
  BOOLEAN                CmdSts;

  CmdSts = FALSE;
  Buffer = BALIGN32 (&UBuff);
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.SmmLock\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_DEFAULT_BUFFER);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdSmmLock);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
  return EFI_SUCCESS;
}

/**
 * @brief notify PSP an intrusion event happened, this command can be sent in SMM or non-SMM mode
 *
 * @param IntrusionEvent  intrusion event with masks from INTRUSION_EVENT
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
 * @return EFI_STATUS 0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdNotifyIntrusionEvent (
  IN UINT32                IntrusionEvent,
  IN UINT8                 *SmmBuffer,
  IN BOOLEAN               *SmmFlag
  )
{
  EFI_STATUS                    Status;
  MBOX_NOTIFY_INTRUSION_EVENT  *Buffer;
  UNALIGNED_MBOX_BUFFER         UBuff;     // Unaligned buffer
  BOOLEAN                       CmdSts;

  CmdSts = FALSE;
  Status = EFI_INVALID_PARAMETER;
  Buffer = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.C2PMbox.NotifyIntrusionEvent\n");
  // SMM mode
  if ((SmmBuffer != NULL) && (SmmFlag != NULL)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmBuffer: %x\n",  SmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("SmmFlag: %x\n",  SmmFlag);
    Buffer = (MBOX_NOTIFY_INTRUSION_EVENT*)SmmBuffer;
    ZeroMem (Buffer, sizeof (MBOX_NOTIFY_INTRUSION_EVENT));
    Buffer->Header.TotalSize      = sizeof (MBOX_NOTIFY_INTRUSION_EVENT);
    Buffer->Version = 0;
    Buffer->IntrusionEvent = IntrusionEvent;
    *SmmFlag = TRUE;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdNotifyIntrusionEvent);
    *SmmFlag = FALSE;
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    return EFI_SUCCESS;
  } else {  // non-SMM mode
    ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));
    Buffer = BALIGN32 (&UBuff);
    Buffer->Header.TotalSize      = sizeof (MBOX_NOTIFY_INTRUSION_EVENT);
    Buffer->Version = 0;
    Buffer->IntrusionEvent = IntrusionEvent;
    CmdSts = SendPspCommand (Buffer, MboxBiosCmdNotifyIntrusionEvent);
    IDS_HDT_CONSOLE_PSP_TRACE ("Mbox.Hdr.Sts:0x%x\n", Buffer->Header.Status);
    if (CmdSts == FALSE) {
      return EFI_DEVICE_ERROR;
    }
    return EFI_SUCCESS;
  }
}

/**
 * @brief get intrusion event logs from PSP
 *
 * @param IntrusionEventLogBuffer   intrusion event logs
 * @return EFI_STATUS               0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdGetIntrusionLog (
  IN OUT INTRUSION_EVENT_LOGS      *IntrusionEventLogBuffer
  )
{
  MBOX_INTRUSION_EVENT_LOGS   *Buffer;
  UNALIGNED_MBOX_BUFFER       UBuff;     // Unaligned buffer
  BOOLEAN                     CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosCmdGetIntrusionLog\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_INTRUSION_EVENT_LOGS);
  CmdSts = SendPspCommand (Buffer, MboxBiosCmdGetIntrusionLog);

  // if intrusion log become deleted or invalid
  // (by an attacker, flash memory corruption, or HW failure, etc.),
  // report this error as intrusion log not found
  if (Buffer->Header.Status == TEE_ERROR_ITEM_NOT_FOUND) {
    return EFI_NOT_FOUND;
  } else if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  } else {
    return EFI_SUCCESS;
  }
}

/**
 * @brief configure intrusion detection
 *
 * @param IntrusionCountermeasure   intrusion countermeasure with masks from INTRUSION_COUNTERMEASURE
 * @return EFI_STATUS               0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdCfgIntrusionDetection (
  IN UINT32      IntrusionCountermeasure
  )
{
  MBOX_CFG_INTRUSION_DETECTION *Buffer;
  UNALIGNED_MBOX_BUFFER        UBuff;     // Unaligned buffer
  BOOLEAN                      CmdSts;

  CmdSts = FALSE;
  ZeroMem (&UBuff, sizeof (UNALIGNED_MBOX_BUFFER));

  Buffer = BALIGN32 (&UBuff);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspMboxBiosCmdCfgIntrusionDetection\n");

  Buffer->Header.TotalSize      = sizeof (MBOX_CFG_INTRUSION_DETECTION);
  Buffer->Version = 0;
  Buffer->IntrusionCountermeasure = IntrusionCountermeasure;

  IDS_HDT_CONSOLE_PSP_TRACE ("IntrusionCountermeasure - 0x%x\n", IntrusionCountermeasure);

  CmdSts = SendPspCommand (Buffer, MboxBiosCmdCfgIntrusionDetection);
  if (CmdSts == FALSE) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}