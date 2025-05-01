/** @file
  Implement TCM NV storage related command.

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

//
// Max TCM NV value length
//
#define TCMNVVALUELENGTH  1024

#pragma pack(1)

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  TCM12_NV_DATA_PUBLIC  PubInfo;
  TCM_ENCAUTH           EncAuth;
} TCM_CMD_NV_DEFINE_SPACE;

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  TCM_NV_INDEX          NvIndex;
  UINT32                Offset;
  UINT32                DataSize;
} TCM_CMD_NV_READ_VALUE;

typedef struct {
  TCM_RSP_COMMAND_HDR   Hdr;
  UINT32                DataSize;
  UINT8                 Data[TCMNVVALUELENGTH];
} TCM_RSP_NV_READ_VALUE;

typedef struct {
  TCM_RQU_COMMAND_HDR   Hdr;
  TCM_NV_INDEX          NvIndex;
  UINT32                Offset;
  UINT32                DataSize;
  UINT8                 Data[TCMNVVALUELENGTH];
} TCM_CMD_NV_WRITE_VALUE;

#pragma pack()

/**
  Send NV DefineSpace command to TCM.

  @param PubInfo           The public parameters of the NV area.
  @param EncAuth           The encrypted AuthData, only valid if the attributes require subsequent authorization.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvDefineSpace (
  IN TCM12_NV_DATA_PUBLIC  *PubInfo,
  IN TCM_ENCAUTH           *EncAuth
  )
{
  EFI_STATUS               Status;
  TCM_CMD_NV_DEFINE_SPACE  Command;
  TCM_RSP_COMMAND_HDR      Response;
  UINT32                   Length;

  //
  // send Tcm command TCM_ORD_NV_DefineSpace
  //
  Command.Hdr.tag         = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize   = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal     = SwapBytes32 (TCM_ORD_NV_DefineSpace);
  Command.PubInfo.tag     = SwapBytes16 (PubInfo->tag);
  Command.PubInfo.nvIndex = SwapBytes32 (PubInfo->nvIndex);
  Command.PubInfo.pcrInfoRead.pcrSelection.sizeOfSelect  = SwapBytes16 (PubInfo->pcrInfoRead.pcrSelection.sizeOfSelect);
  Command.PubInfo.pcrInfoRead.pcrSelection.pcrSelect[0]  = PubInfo->pcrInfoRead.pcrSelection.pcrSelect[0];
  Command.PubInfo.pcrInfoRead.pcrSelection.pcrSelect[1]  = PubInfo->pcrInfoRead.pcrSelection.pcrSelect[1];
  Command.PubInfo.pcrInfoRead.pcrSelection.pcrSelect[2]  = PubInfo->pcrInfoRead.pcrSelection.pcrSelect[2];
  Command.PubInfo.pcrInfoRead.localityAtRelease          = PubInfo->pcrInfoRead.localityAtRelease;
  CopyMem (&Command.PubInfo.pcrInfoRead.digestAtRelease, &PubInfo->pcrInfoRead.digestAtRelease, sizeof(PubInfo->pcrInfoRead.digestAtRelease));
  Command.PubInfo.pcrInfoWrite.pcrSelection.sizeOfSelect = SwapBytes16 (PubInfo->pcrInfoWrite.pcrSelection.sizeOfSelect);
  Command.PubInfo.pcrInfoWrite.pcrSelection.pcrSelect[0] = PubInfo->pcrInfoWrite.pcrSelection.pcrSelect[0];
  Command.PubInfo.pcrInfoWrite.pcrSelection.pcrSelect[1] = PubInfo->pcrInfoWrite.pcrSelection.pcrSelect[1];
  Command.PubInfo.pcrInfoWrite.pcrSelection.pcrSelect[2] = PubInfo->pcrInfoWrite.pcrSelection.pcrSelect[2];
  Command.PubInfo.pcrInfoWrite.localityAtRelease         = PubInfo->pcrInfoWrite.localityAtRelease;
  CopyMem (&Command.PubInfo.pcrInfoWrite.digestAtRelease, &PubInfo->pcrInfoWrite.digestAtRelease, sizeof(PubInfo->pcrInfoWrite.digestAtRelease));
  Command.PubInfo.permission.tag        = SwapBytes16 (PubInfo->permission.tag);
  Command.PubInfo.permission.attributes = SwapBytes32 (PubInfo->permission.attributes);
  Command.PubInfo.bReadSTClear          = PubInfo->bReadSTClear;
  Command.PubInfo.bWriteSTClear         = PubInfo->bWriteSTClear;
  Command.PubInfo.bWriteDefine          = PubInfo->bWriteDefine;
  Command.PubInfo.dataSize              = SwapBytes32 (PubInfo->dataSize);
  CopyMem (&Command.EncAuth, EncAuth, sizeof(*EncAuth));
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "TcmNvDefineSpace - ReturnCode = %x\n", SwapBytes32 (Response.returnCode)));
  switch (SwapBytes32 (Response.returnCode)) {
  case TCM_SUCCESS:
    return EFI_SUCCESS;
  default:
    return EFI_DEVICE_ERROR;
  }
}

/**
  Send NV ReadValue command to TCM.

  @param NvIndex           The index of the area to set.
  @param Offset            The offset into the area.
  @param DataSize          The size of the data area.
  @param Data              The data to set the area to.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvReadValue (
  IN TCM_NV_INDEX  NvIndex,
  IN UINT32        Offset,
  IN OUT UINT32    *DataSize,
  OUT UINT8        *Data
  )
{
  EFI_STATUS             Status;
  TCM_CMD_NV_READ_VALUE  Command;
  TCM_RSP_NV_READ_VALUE  Response;
  UINT32                 Length;

  //
  // send Tcm command TCM_ORD_NV_ReadValue
  //
  Command.Hdr.tag       = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal   = SwapBytes32 (TCM_ORD_NV_ReadValue);
  Command.NvIndex       = SwapBytes32 (NvIndex);
  Command.Offset        = SwapBytes32 (Offset);
  Command.DataSize      = SwapBytes32 (*DataSize);
  Length = sizeof (Response);
  Status = TcmSubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "TcmNvReadValue - ReturnCode = %x\n", SwapBytes32 (Response.Hdr.returnCode)));
  switch (SwapBytes32 (Response.Hdr.returnCode)) {
  case TCM_SUCCESS:
    break;
  default:
    return EFI_DEVICE_ERROR;
  }

  //
  // Return the response
  //
  if (SwapBytes32 (Response.DataSize) > *DataSize) {
    return EFI_BUFFER_TOO_SMALL;
  }
  *DataSize = SwapBytes32 (Response.DataSize);
  ZeroMem (Data, *DataSize);
  CopyMem (Data, &Response.Data, *DataSize);

  return EFI_SUCCESS;
}

/**
  Send NV WriteValue command to TCM.

  @param NvIndex           The index of the area to set.
  @param Offset            The offset into the NV Area.
  @param DataSize          The size of the data parameter.
  @param Data              The data to set the area to.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvWriteValue (
  IN TCM_NV_INDEX  NvIndex,
  IN UINT32        Offset,
  IN UINT32        DataSize,
  IN UINT8         *Data
  )
{
  EFI_STATUS              Status;
  TCM_CMD_NV_WRITE_VALUE  Command;
  UINT32                  CommandLength;
  TCM_RSP_COMMAND_HDR     Response;
  UINT32                  ResponseLength;

  if (DataSize > sizeof (Command.Data)) {
    return EFI_UNSUPPORTED;
  }

  //
  // send Tcm command TCM_ORD_NV_WriteValue
  //
  Command.Hdr.tag       = SwapBytes16 (TCM_TAG_RQU_COMMAND);
  CommandLength = sizeof (Command) - sizeof(Command.Data) + DataSize;
  Command.Hdr.paramSize = SwapBytes32 (CommandLength);
  Command.Hdr.ordinal   = SwapBytes32 (TCM_ORD_NV_WriteValue);
  Command.NvIndex       = SwapBytes32 (NvIndex);
  Command.Offset        = SwapBytes32 (Offset);
  Command.DataSize      = SwapBytes32 (DataSize);
  CopyMem (Command.Data, Data, DataSize);
  ResponseLength = sizeof (Response);
  Status = TcmSubmitCommand (CommandLength, (UINT8 *)&Command, &ResponseLength, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "TcmNvWritedValue - ReturnCode = %x\n", SwapBytes32 (Response.returnCode)));
  switch (SwapBytes32 (Response.returnCode)) {
  case TCM_SUCCESS:
    return EFI_SUCCESS;
  default:
    return EFI_DEVICE_ERROR;
  }
}
