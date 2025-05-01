/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/AmdFtpmLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Tpm20.h>
#include <Hsp.h>
#include <Library/HspLib.h>
#include <Filecode.h>

#define PATCH_BASESIZE   1

#define FILECODE LIBRARY_AMDHSPFTPMLIB_AMDHSPFTPMLIB_FILECODE



/**
 *  Set Command,Response buffer registers, Buffer address equal to NULL will skip the setting
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 *
 */

EFI_STATUS
FtpmAssignMemory (
  IN     UINTN                CommandBuffer,
  IN     UINT32               CommandSize,
  IN     UINTN                ResponseBuffer,
  IN     UINT32               ResponseSize
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.Ftpm FtpmAssignMemory no function\n");
  return (EFI_UNSUPPORTED);
}

/**
 *  GET TPM related Info
 *
 * @param[in,out] FtpmStatus              Used to hold more detail info (Unused Currently)
 *
 * @return       EFI_SUCCESS              Ftpm function supported
 * @return       EFI_UNSUPPORTED          Ftpm function unsupported
 *
 */
EFI_STATUS
FtpmGetInfo (
  IN OUT   UINTN                *HspfTPMStatus
  )
{
  HSP_MAILBOX_CHANNEL HspMailboxChannel;

  HspMailboxChannel = X86_VTL0_TPM;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.Ftpm FtpmGetInfo function\n");
  return IsHspReady ((VOID*)&HspMailboxChannel) ? (EFI_SUCCESS) : (EFI_UNSUPPORTED);
}

/**
 *  Send a TPM command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
FtpmSendCommand (
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize
  )
{
  TPM2_COMMAND_HEADER *TpmHdr;
  TPM_CC              TpmCommandCode;
  BOOLEAN             ExeStatus;
  HSP_MAILBOX_CHANNEL HspMailboxChannel;

  TpmHdr            = NULL;
  TpmCommandCode    = 0;
  ExeStatus         = FALSE;
  HspMailboxChannel = X86_VTL0_TPM;

  AGESA_TESTPOINT (TpHspfTpmSendCmdEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.Ftpm Send a TPM command\n");

  // Exit if HSP fTPM set to disable
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP fTPM disabled, exit\n");
    return EFI_UNSUPPORTED;
  }

  // Validate Input parameters
  if ( (CommandBuffer == NULL) || (CommandSize == 0) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters (HSP.Ftpm.SendCmd)\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( CommandSize > HSP_COMMANDRESPONSE_BUFFER_SIZE ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters (HSP.Ftpm.SendCmd), Command Size is bigger than 0x%x.\n", HSP_COMMANDRESPONSE_BUFFER_SIZE);
    return EFI_INVALID_PARAMETER;
  }

#if HSP_DEBUG_ENABLE
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tDump Send Command buffer (0x%lx) with size (%d, 0x%x): 64\n", CommandBuffer, CommandSize, CommandSize);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tDump Send Command buffer (0x%x) with size (%d, 0x%x): 32\n", CommandBuffer, CommandSize, CommandSize);
  }
  HspDumpData (CommandBuffer, CommandSize);
#endif

  // 1.  Wait for the doorbells to be ready
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWait for HSP to be ready...\n");
  ExeStatus = PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout1), &IsHspReady, (VOID*)&HspMailboxChannel);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet ExeStatus: 0x%x\n", ExeStatus);
  if (ExeStatus == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP is not ready, exit\n");
    return (EFI_NOT_READY);
  }

  // 2. Copy Command to HSP buffer
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tLoad the command\n", 1);
  CopyToHSPBuffer (CommandBuffer, CommandSize);

  // 3. Set TPM_BUFFERS.ControlArea.Request to 1
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet TPM_BUFFERS.ControlArea.Request: 0x%x \n", 1);
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_REQUEST,
    1
    );

  // 4. Set TPM_BUFFERS.ControlArea.Start to 1
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tIssue TPM_CONTROL_AREA.Start\n");
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_START,
    1
    );

  // 5. Notify HSP by doorbell
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tIssue VTL0 TPM doorbell.\n");
  NotifyHSP (X86_VTL0_TPM);

  // 6. Wait for HSP to finish
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWait for HSP...\n");
  TpmHdr = (TPM2_COMMAND_HEADER*)CommandBuffer;
  TpmCommandCode = SwapBytes32 (TpmHdr->commandCode);
  if ((TpmCommandCode == TPM_CC_CreatePrimary) ||
      (TpmCommandCode == TPM_CC_Create)) {
    // Time out in PcdAmdFtpmTimeout1 seconds
    ExeStatus = PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout1), &IsHspReady, (VOID*)&HspMailboxChannel);
  } else {
    // Time out in PcdAmdFtpmTimeout2 seconds
    ExeStatus = PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout2), &IsHspReady, (VOID*)&HspMailboxChannel);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet ExeStatus: 0x%x\n", ExeStatus);

  if (ExeStatus == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommand time out (HSP.fTPM.SendCmd)\n");
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_TIMEOUT);
  }

#if HSP_DEBUG_ENABLE
  FtpmGetValueMmio (HSP_C2H_TPM_L0_DOORBELL_SMN_BASE);
  FtpmGetValueMmio (HSP_H2C_TPM_L0_DOORBELL_SMN_BASE);
#endif

  if ( FtpmCheckStatus () ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommand error (HSP.fTPM.SendCmd)\n");
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_DEVICE_ERROR);
  }

  AGESA_TESTPOINT (TpHspfTpmSendCmdExit, NULL);
  return (EFI_SUCCESS);
}

/**
 *  Get a TPM command's response
 *
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
FtpmGetResponse (
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINT32                *ResponseSize
  )
{
  BOOLEAN             ExeStatus;
  UINT32              iResponseSize;
  HSP_MAILBOX_CHANNEL HspMailboxChannel;
  UINT8               TpmRespHdr[0x0C];

  ExeStatus         = FALSE;
  iResponseSize     = 0;
  HspMailboxChannel = X86_VTL0_TPM;

  AGESA_TESTPOINT (TpHspfTpmRcvCmdEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.Ftpm Get a TPM command's response.\n");

  //Validate Input parameters
  if ((ResponseBuffer == NULL) || (ResponseSize == NULL) || (*ResponseSize == 0)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters (HSP.fTPM.GetResponse)\n");
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_INVALID_PARAMETER);
  }

  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tResponseBuffer: 0x%lx 64\n", ResponseBuffer);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tResponseBuffer: 0x%x 32\n", ResponseBuffer);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tResponseSize  : %d, 0x%x\n", *ResponseSize, *ResponseSize);

  //Exit if HSP fTPM set to disable
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP fTPM disabled, exit\n");
    return (EFI_UNSUPPORTED);
  }

  // Wait for Ftpm to finish
  // Time out in PcdAmdFtpmTimeout2 seconds
  ExeStatus = PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout2), &IsHspReady, (VOID*)&HspMailboxChannel);
  if (ExeStatus == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommand time out (HSP.fTPM.GetResponse)\n");
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_TIMEOUT);
  }

  if (FtpmCheckStatus ()) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommand error (HSP.fTPM.GetResponse)\n");
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_DEVICE_ERROR);
  }

  iResponseSize = FtpmGetValueMmio (HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_RESPONSESIZE);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Control Area Response Size is %d (0x%x)\n", iResponseSize, iResponseSize);

  // Use TPM2 RESPONSE size as the response size.
  CopyFromHSPBuffer (TpmRespHdr, sizeof(TpmRespHdr));
  CopyMem (&iResponseSize, TpmRespHdr + 2, sizeof(iResponseSize));
  iResponseSize = SwapBytes32 (iResponseSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP TPM2 Response Size is %d (0x%x)\n", iResponseSize, iResponseSize);

  if ( iResponseSize > HSP_COMMANDRESPONSE_BUFFER_SIZE ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP Response Size is bigger than 0x%x. Something wrong in HSP (HSP.fTPM.GetResponse)\n", HSP_COMMANDRESPONSE_BUFFER_SIZE);
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_DEVICE_ERROR);
  }

  if ( *ResponseSize < iResponseSize ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tOutput buffer Size is too small (HSP.fTPM.GetResponse) Need %d bytes\n", iResponseSize);
    FtpmGotoIdle (X86_VTL0_TPM);
    return (EFI_INVALID_PARAMETER);
  }

  *ResponseSize = iResponseSize;

  if ( iResponseSize ) {
    CopyFromHSPBuffer (ResponseBuffer, iResponseSize);
  }

  FtpmGotoIdle (X86_VTL0_TPM);

#if HSP_DEBUG_ENABLE
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tDump Output ResponseBuffer (Size: %d):\n", *ResponseSize);
  HspDumpData (ResponseBuffer, *ResponseSize);
#endif

#if PATCH_BASESIZE  // TODO: Patch for MSFT HSP firmware. Remove it later when we get new FW drop from MSFT.
  SetVTL0TpmBaseSize ();
#endif

  AGESA_TESTPOINT (TpHspfTpmRcvCmdExit, NULL);
  return (EFI_SUCCESS);
}

/**
 *  Execute a TPM command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
EFI_STATUS
FtpmExecuteCommand (
  IN       VOID                 *CommandBuffer,
  IN       UINT32               CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINT32               *ResponseSize
  )
{
  EFI_STATUS Status;

  Status  = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("HSP.Ftpm Execute Cmd\n");

  //Exit if HSP fTPM set to disable
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP fTPM disabled, exit\n");
    return EFI_UNSUPPORTED;
  }

  Status = FtpmSendCommand (CommandBuffer, CommandSize);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSend Command Error (HSP.Ftpm Execute Cmd)\n");
    return Status;
  }

  Status = FtpmGetResponse (ResponseBuffer, ResponseSize);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSend Response Error (HSP.Ftpm Execute Cmd)\n");
    return Status;
  }
  return (EFI_SUCCESS);
}

