/** @file
  Implement TCM Startup related command.

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
#include <Library/DebugLib.h>

#pragma pack(1)

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  TCM_STARTUP_TYPE      TcmSt;
} TCM_CMD_START_UP;

#pragma pack()

/**
  Send Startup command to TCM.

  @param TcmSt           Startup Type.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmStartup (
  IN TCM_STARTUP_TYPE  TcmSt
  )
{
  EFI_STATUS           Status;
  TCM_CMD_START_UP     Command;
  TCM_RSP_COMMAND_HDR  Response;
  UINT32               Length;

  //
  // send Tcm command TCM_ORD_Startup
  //
  Command.Hdr.tag       = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal   = SwapBytes32 (TCM_ORD_Startup);
  Command.TcmSt         = SwapBytes16 (TcmSt);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  switch (SwapBytes32(Response.returnCode)) {
  case TCM_SUCCESS:
    DEBUG ((DEBUG_INFO, "TCMStartup: TCM_SUCCESS\n"));
    return EFI_SUCCESS;
  case TCM_INVALID_POSTINIT:
    // In warm reset, TCM may response TCM_INVALID_POSTINIT
    DEBUG ((DEBUG_INFO, "TCMStartup: TCM_INVALID_POSTINIT\n"));
    return EFI_SUCCESS;
  default:
    return EFI_DEVICE_ERROR;
  }
}

/**
  Send SaveState command to TCM.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmSaveState (
  VOID
  )
{
  EFI_STATUS           Status;
  TCM_RQU_COMMAND_HDR  Command;
  TCM_RSP_COMMAND_HDR  Response;
  UINT32               Length;

  //
  // send Tcm command TCM_ORD_SaveState
  //
  Command.tag        = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.paramSize  = SwapBytes32 (sizeof (Command));
  Command.ordinal    = SwapBytes32 (TCM_ORD_SaveState);
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
