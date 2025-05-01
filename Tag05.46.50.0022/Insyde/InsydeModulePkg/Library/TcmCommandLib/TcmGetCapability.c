/** @file
  Implement TCM Get Capabilities related commands.

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
  TCM_RQU_COMMAND_HDR  Hdr;
  UINT32               Capability;
  UINT32               CapabilityFlagSize;
  UINT32               CapabilityFlag;
} TCM_CMD_GET_CAPABILITY;

typedef struct {
  TCM_RSP_COMMAND_HDR  Hdr;
  UINT32               ResponseSize;
  TCM_PERMANENT_FLAGS  Flags;
} TCM_RSP_GET_CAPABILITY_PERMANENT_FLAGS;

typedef struct {
  TCM_RSP_COMMAND_HDR  Hdr;
  UINT32               ResponseSize;
  TCM_STCLEAR_FLAGS    Flags;
} TCM_RSP_GET_CAPABILITY_STCLEAR_FLAGS;

typedef struct {
  TCM_RSP_COMMAND_HDR       Hdr;
  UINT32                    ResponseSize;
  UINT8                     Owner;
} TCM_RSP_GET_CAPABILITY_PROP_OWNER;

typedef struct {
  TCM_RSP_COMMAND_HDR  Hdr;
  UINT32               ResponseSize;
  TCM_CAP_VERSION_INFO VersionInfo;
} TCM_RSP_GET_CAPABILITY_VERSION_INFO;

#pragma pack()

/**
  Get TCM capability permanent flags.

  @param[out] TcmPermanentFlags   Pointer to the buffer for returned flag structure.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityFlagPermanent (
  OUT TCM_PERMANENT_FLAGS  *TcmPermanentFlags
  )
{
  EFI_STATUS                              Status;
  TCM_CMD_GET_CAPABILITY                  Command;
  TCM_RSP_GET_CAPABILITY_PERMANENT_FLAGS  Response;
  UINT32                                  Length;

  //
  // send Tcm command TCM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TCM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TCM_CAP_FLAG);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TCM_CAP_FLAG_PERMANENT));
  Command.CapabilityFlag     = SwapBytes32 (TCM_CAP_FLAG_PERMANENT);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32 (Response.Hdr.returnCode) != TCM_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TcmGetCapabilityFlagPermanent: Response Code error! 0x%08x\r\n", SwapBytes32 (Response.Hdr.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  ZeroMem (TcmPermanentFlags, sizeof (*TcmPermanentFlags));
  CopyMem (TcmPermanentFlags, &Response.Flags, MIN (sizeof (*TcmPermanentFlags), SwapBytes32(Response.ResponseSize)));

  return Status;
}

/**
  Get TCM capability volatile flags.

  @param[out] VolatileFlags   Pointer to the buffer for returned flag structure.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityFlagVolatile (
  OUT TCM_STCLEAR_FLAGS  *VolatileFlags
  )
{
  EFI_STATUS                            Status;
  TCM_CMD_GET_CAPABILITY                Command;
  TCM_RSP_GET_CAPABILITY_STCLEAR_FLAGS  Response;
  UINT32                                Length;

  //
  // send Tcm command TCM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TCM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TCM_CAP_FLAG);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TCM_CAP_FLAG_VOLATILE));
  Command.CapabilityFlag     = SwapBytes32 (TCM_CAP_FLAG_VOLATILE);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32 (Response.Hdr.returnCode) != TCM_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TcmGetCapabilityFlagVolatile: Response Code error! 0x%08x\r\n", SwapBytes32 (Response.Hdr.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  ZeroMem (VolatileFlags, sizeof (*VolatileFlags));
  CopyMem (VolatileFlags, &Response.Flags, MIN (sizeof (*VolatileFlags), SwapBytes32(Response.ResponseSize)));

  return Status;
}

/**
  Get TCM capability property owner flags.

  @param[out] TcmOwner   Returned owner flag.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityPropertyOwner (
  OUT BOOLEAN                 *TcmOwner
  )
{
  EFI_STATUS                              Status;
  TCM_CMD_GET_CAPABILITY                  Command;
  TCM_RSP_GET_CAPABILITY_PROP_OWNER       Response;
  UINT32                                  Length;

  //
  // send Tcm command TCM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TCM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TCM_CAP_PROPERTY);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TCM_CAP_PROP_OWNER));
  Command.CapabilityFlag     = SwapBytes32 (TCM_CAP_PROP_OWNER);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32 (Response.Hdr.returnCode) != TCM_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TcmGetCapabilityFlagPermanent: Response Code error! 0x%08x\r\n", SwapBytes32 (Response.Hdr.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  *TcmOwner = Response.Owner;

  return Status;
}

/**
  Get TCM capability version value.

  @param[out] VersionInfo   Returned version info.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityVersionValue (
  OUT TCM_CAP_VERSION_INFO                *VersionInfo
  )
{
  EFI_STATUS                              Status;
  TCM_CMD_GET_CAPABILITY                  Command;
  TCM_RSP_GET_CAPABILITY_VERSION_INFO     Response;
  UINT32                                  Length;

  //
  // send Tcm command TCM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TCM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TCM_CAP_VERSION_VAL);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TCM_CAP_VERSION_VAL));
  Command.CapabilityFlag     = SwapBytes32 (TCM_CAP_VERSION_VAL);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SwapBytes32 (Response.Hdr.returnCode) != TCM_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TcmGetCapabilityFlagPermanent: Response Code error! 0x%08x\r\n", SwapBytes32 (Response.Hdr.returnCode)));
    return EFI_DEVICE_ERROR;
  }

  ZeroMem (VersionInfo, sizeof (*VersionInfo));
  CopyMem (VersionInfo, &Response.VersionInfo, MIN (sizeof (*VersionInfo), SwapBytes32(Response.ResponseSize)));

  return Status;
}

