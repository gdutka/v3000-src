/** @file
  Implement TCM PCR related commands.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

**/

#include <PiPei.h>
#include <Library/TcmCommandLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TcmDeviceLib.h>

#pragma pack(1)

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  TCM_PCRINDEX          PcrIndex;
  TCM_DIGEST            TcmDigest;
} TCM_CMD_EXTEND;

typedef struct {
  TCM_RSP_COMMAND_HDR   Hdr;
  TCM_DIGEST            TcmDigest;
} TCM_RSP_EXTEND;

#pragma pack()

/**
  Extend a TCM PCR.

  @param[in]  DigestToExtend    The 160 bit value representing the event to be recorded.
  @param[in]  PcrIndex          The PCR to be updated.
  @param[out] NewPcrValue       New PCR value after extend.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmExtend (
  IN  TCM_DIGEST    *DigestToExtend,
  IN  TCM_PCRINDEX  PcrIndex,
  OUT TCM_DIGEST    *NewPcrValue
  )
{
  EFI_STATUS      Status;
  TCM_CMD_EXTEND  Command;
  TCM_RSP_EXTEND  Response;
  UINT32          Length;

  //
  // send Tcm command TCM_ORD_Extend
  //
  Command.Hdr.tag       = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal   = SwapBytes32 (TCM_ORD_Extend);
  Command.PcrIndex      = SwapBytes32 (PcrIndex);
  CopyMem (&Command.TcmDigest, DigestToExtend, sizeof (Command.TcmDigest));
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32(Response.Hdr.returnCode) != TCM_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "TcmExtend: Response Code error! 0x%08x\r\n", SwapBytes32(Response.Hdr.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  if (NewPcrValue != NULL) {
    CopyMem (NewPcrValue, &Response.TcmDigest, sizeof (*NewPcrValue));
  }

  return Status;
}
