/** @file
  Implement TCM Physical Presence related command.

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

#pragma pack(1)

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
} TCM_CMD_PHYSICAL_PRESENCE_ENABLE;

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
} TCM_CMD_PHYSICAL_PRESENCE_DISABLE;

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  BOOLEAN               Deactivated;
} TCM_CMD_PHYSICAL_PRESENCE_SET_DEACTIVATED;

#pragma pack()

/**
  Send TCM_ORD_PhysicalEnable command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceEnable (
  VOID
  )
{
  EFI_STATUS                        Status;
  TCM_CMD_PHYSICAL_PRESENCE_ENABLE  Command;
  TCM_RSP_COMMAND_HDR               Response;
  UINT32                            Length;

  //
  // send Tcm command TSC_ORD_PhysicalPresenceEnable
  //
  Command.Hdr.tag          = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize    = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal      = SwapBytes32 (TCM_ORD_PhysicalEnable);
  Length = sizeof (Response);

  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32(Response.returnCode) != TCM_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "TcmPhysicalPresence: Response Code error! 0x%08x\r\n", SwapBytes32(Response.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Send TCM_ORD_PhysicalDisable command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceDisble (
  VOID
  )
{
  EFI_STATUS                        Status;
  TCM_CMD_PHYSICAL_PRESENCE_DISABLE Command;
  TCM_RSP_COMMAND_HDR               Response;
  UINT32                            Length;

  //
  // send Tcm command TSC_ORD_PhysicalPresenceEnable
  //
  Command.Hdr.tag          = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize    = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal      = SwapBytes32 (TCM_ORD_PhysicalDisable);
  Length = sizeof (Response);

  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32(Response.returnCode) != TCM_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "TcmPhysicalPresence: Response Code error! 0x%08x\r\n", SwapBytes32(Response.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Send TCM_ORD_PhysicalSetDeactivated command to TCM.

  @param[in] Deactivated        The state to set the TCMs deactivated flag.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceSetDeactivated (
  IN BOOLEAN  Deactivated
  )
{
  EFI_STATUS                                 Status;
  TCM_CMD_PHYSICAL_PRESENCE_SET_DEACTIVATED  Command;
  TCM_RSP_COMMAND_HDR                        Response;
  UINT32                                     Length;

  //
  // send Tcm command TCM_ORD_PhysicalSetDeactivated
  //
  Command.Hdr.tag          = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize    = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal      = SwapBytes32 (TCM_ORD_PhysicalSetDeactivated);
  Command.Deactivated      = Deactivated;
  Length = sizeof (Response);

  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32(Response.returnCode) != TCM_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "TcmPhysicalPresence: Response Code error! 0x%08x\r\n", SwapBytes32(Response.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

