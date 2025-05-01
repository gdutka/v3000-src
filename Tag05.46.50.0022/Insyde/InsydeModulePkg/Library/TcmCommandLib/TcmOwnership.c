/** @file
  Implement TCM Ownership related command.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/TcmDeviceLib.h>

/**
  Send ForceClear command to TCM.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmForceClear (
  VOID
  )
{
  EFI_STATUS           Status;
  TCM_RQU_COMMAND_HDR  Command;
  TCM_RSP_COMMAND_HDR  Response;
  UINT32               Length;

  //
  // send Tcm command TCM_ORD_ForceClear
  //
  Command.tag        = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.paramSize  = SwapBytes32 (sizeof (Command));
  Command.ordinal    = SwapBytes32 (TCM_ORD_ForceClear);
  Length = sizeof (Response);

  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  switch (SwapBytes32 (Response.returnCode)) {
  case TCM_SUCCESS:
    return EFI_SUCCESS;
  default:
    return EFI_DEVICE_ERROR;
  }
}
