/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Uefi.h"
#include <Library/AmdTpm2CommandLib.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDTPM2COMMANDLIB_AMDTPM2COMMANDLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                            L O C A L   F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Remove all TPM context associated with TPM_RH_PLATFORM
 *
 */
EFI_STATUS
EFIAPI
Tpm2Clear (
  IN EFI_TCG2_PROTOCOL           *Tcg2Protocol
  )
{
  EFI_STATUS                        Status;
  TPM2_CLEAR_COMMAND                Cmd;
  TPM2_CLEAR_RESPONSE               Res;
  UINT32                            ResultBufSize;
  UINT32                            CmdSize;
  UINT32                            RespSize;
  UINT8                             *Buffer;
  UINT8                             *AuthSizeOffset;

  ASSERT (Tcg2Protocol != NULL);
  Cmd.Header.tag         = SwapBytes16 (TPM_ST_SESSIONS);
  Cmd.Header.commandCode = SwapBytes32 (TPM_CC_Clear);
  Cmd.AuthHandle         = SwapBytes32 (TPM_RH_PLATFORM);

  Buffer = (UINT8 *)&Cmd.AuthorizationSize;

  //
  // Add in Auth session
  //
  AuthSizeOffset = Buffer;
  *(UINT32 *)Buffer = 0;
  Buffer += sizeof(UINT32);

  // authHandle
  *(UINT32 *)Buffer = SwapBytes32 (TPM_RS_PW);
  Buffer += sizeof(UINT32);

  // nonce = nullNonce
  *(UINT16 *)Buffer = 0;
  Buffer += sizeof(UINT16);

  // sessionAttributes = 0
  *(UINT8 *)Buffer = 0;
  Buffer += sizeof(UINT8);

  // auth = nullAuth
  *(UINT16 *)Buffer = 0;
  Buffer += sizeof(UINT16);

  // authorizationSize
  *(UINT32 *)AuthSizeOffset = SwapBytes32 ((UINT32)(Buffer - AuthSizeOffset - sizeof(UINT32)));

  CmdSize = (UINT32)(Buffer - (UINT8 *)&Cmd);
  Cmd.Header.paramSize   = SwapBytes32 (CmdSize);

  ResultBufSize = sizeof(Res);

  Status = Tcg2Protocol->SubmitCommand (Tcg2Protocol,
   CmdSize, (UINT8 *)&Cmd, ResultBufSize, (UINT8 *)&Res);
  if (EFI_ERROR(Status)) {
      return Status;
  }

  if (ResultBufSize > sizeof(Res)) {
      IDS_HDT_CONSOLE_PSP_TRACE ( "Clear: Failed ExecuteCommand: Buffer Too Small\r\n");
      Status = EFI_BUFFER_TOO_SMALL;
      goto ClearEND;
  }

  //
  // Validate response headers
  //
  RespSize = SwapBytes32 (Res.Header.paramSize);
  if (RespSize > sizeof(Res)) {
      IDS_HDT_CONSOLE_PSP_TRACE ( "Clear: Response size too large! %d\r\n", RespSize);
      Status = EFI_BUFFER_TOO_SMALL;
      goto ClearEND;
  }

  //
  // Fail if command failed
  //
  if (SwapBytes32 (Res.Header.responseCode) != TPM_RC_SUCCESS) {
      IDS_HDT_CONSOLE_PSP_TRACE ( "Clear: Response Code error! 0x%08x\r\n", SwapBytes32(Res.Header.responseCode));
      Status = EFI_DEVICE_ERROR;
      goto ClearEND;
  }

  //
  // Unmarshal the response
  //

  // None

  Status = EFI_SUCCESS;

ClearEND:
  return Status;
}
