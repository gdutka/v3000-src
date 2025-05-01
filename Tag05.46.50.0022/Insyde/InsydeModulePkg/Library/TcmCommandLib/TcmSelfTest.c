/** @file
  Implement TCM NV Self Test related commands.

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
#include <Library/TcmDeviceLib.h>

/**
  Send TCM_ContinueSelfTest command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmContinueSelfTest (
  VOID
  )
{
  EFI_STATUS           Status;
  TCM_RQU_COMMAND_HDR  Command;
  TCM_RSP_COMMAND_HDR  Response;
  UINT32               Length;

  //
  // send Tcm command TCM_ORD_ContinueSelfTest
  //
  Command.tag       = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.paramSize = SwapBytes32 (sizeof (Command));
  Command.ordinal   = SwapBytes32 (TCM_ORD_ContinueSelfTest);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32 (Response.returnCode) != TCM_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TcmContinueSelfTest: Response Code error! 0x%08x\r\n", SwapBytes32 (Response.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}
