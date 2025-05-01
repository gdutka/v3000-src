/** @file
  RSA encryption and decryption by Tpm2.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/Tpm2ExtCommandLib.h>

#pragma pack(1)

typedef struct {
  TPM2_COMMAND_HEADER       Header;
  TPMI_DH_OBJECT            KeyHandle;
  TPM2B_PUBLIC_KEY_RSA      Message;
  TPMT_RSA_DECRYPT          Scheme;
  TPM2B_DATA                Label;
} TPM2_RSA_EMCRYPT_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER      Header;
  TPM2B_PUBLIC_KEY_RSA      Message;
} TPM2_RSA_EMCRYPT_RESPONSE;

typedef struct {
  TPM2_COMMAND_HEADER       Header;
  TPMI_DH_OBJECT            KeyHandle;
  UINT32                    AuthSessionSize;
  TPMS_AUTH_COMMAND         AuthSession;
  TPM2B_PUBLIC_KEY_RSA      CipherText;
  TPMT_RSA_DECRYPT          Scheme;
  TPM2B_DATA                Label;
} TPM2_RSA_DECRYPT_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER      Header;
  UINT32                    AuthSessionSize;
  TPM2B_PUBLIC_KEY_RSA      Message;
  TPMS_AUTH_RESPONSE        AuthSession;
} TPM2_RSA_DECRYPT_RESPONSE;

#pragma pack()

/**
  This command is used to RSA encryption

  @param[in]      KeyHandle           Reference to public portion of RSA key for encryption.
  @param[in]      Message             Message to encrypted.
  @param[in]      Scheme              Padding scheme to use.
  @param[in]      SchemeWidthInBytes  Use width in bytes in Scheme.
  @param[in]      Label               Lable.
  @param[in, out] OutData             Encrypted output.

  @retval EFI_SUCCESS          Operation completed successfully.
  @retval EFI_DEVICE_ERROR     Unexpected device behavior.
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Tpm2RsaEncrypt (
  IN     TPMI_DH_OBJECT          KeyHandle,
  IN     TPM2B_PUBLIC_KEY_RSA    *Message,
  IN     TPMT_RSA_DECRYPT        *Scheme,
  IN     UINT8                   SchemeWidthInBytes,
  IN     TPM2B_DATA              *Label,
  IN OUT TPM2B_PUBLIC_KEY_RSA    *OutData
  )
{
  EFI_STATUS                        Status;
  TPM2_RSA_EMCRYPT_COMMAND          SendBuffer;
  TPM2_RSA_EMCRYPT_RESPONSE         RecvBuffer;
  UINT8                             *Buffer;
  UINT8                             *SchemPtr;
  UINT32                            SendBufferSize;
  UINT32                            RecvBufferSize;
  UINTN                             Index;

  if ((Message == NULL) || (Scheme == NULL) || (SchemeWidthInBytes == 0) || (Label == NULL) || (OutData == NULL)) {
    return EFI_UNSUPPORTED;
  }

  SendBuffer.Header.tag = SwapBytes16(TPM_ST_NO_SESSIONS);
  SendBuffer.Header.commandCode = SwapBytes32(TPM_CC_RSA_Encrypt);
  SendBuffer.KeyHandle = SwapBytes32(KeyHandle);

  Buffer = (UINT8 *)&SendBuffer.Message;
  WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (Message->size));
  Buffer += sizeof(UINT16);
  if (Message->size > sizeof (SendBuffer.Message.buffer)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  CopyMem (Buffer, Message->buffer, Message->size);
  Buffer += Message->size;

  if ((SchemeWidthInBytes > sizeof (TPMT_RSA_DECRYPT)) || ((SchemeWidthInBytes % sizeof (UINT16)) != 0)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  SchemPtr = (UINT8 *) Scheme;
  for (Index = 0; Index < (SchemeWidthInBytes / sizeof (UINT16)); Index++) {
    WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (ReadUnaligned16 ((UINT16 *)SchemPtr)));
    SchemPtr+= sizeof(UINT16);
    Buffer += sizeof(UINT16);
  }

  WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16(Label->size));
  Buffer += sizeof(UINT16);
  if (Label->size > sizeof (SendBuffer.Label.buffer)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  CopyMem (Buffer, Label->buffer, Label->size);
  Buffer += Label->size;

  SendBufferSize = (UINT32)((UINTN)Buffer - (UINTN)&SendBuffer);
  SendBuffer.Header.paramSize = SwapBytes32 (SendBufferSize);
  RecvBufferSize = sizeof (RecvBuffer);
  Status = Tpm2SubmitCommand (SendBufferSize, (UINT8 *)&SendBuffer, &RecvBufferSize, (UINT8 *)&RecvBuffer);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (RecvBufferSize < sizeof (TPM2_RESPONSE_HEADER)) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaEncrypt - RecvBufferSize Error - %x\n", RecvBufferSize));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }
  if (SwapBytes32(RecvBuffer.Header.responseCode) != TPM_RC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaEncrypt - responseCode - %x\n", SwapBytes32(RecvBuffer.Header.responseCode)));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }

  OutData->size = SwapBytes16 (RecvBuffer.Message.size);
  if (OutData->size > MAX_RSA_KEY_BYTES) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaEncrypt - OutData->size error %x\n", OutData->size));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }
  CopyMem (OutData->buffer, &RecvBuffer.Message.buffer, OutData->size);

Done:
  ZeroMem (&SendBuffer, sizeof(SendBuffer));
  ZeroMem (&RecvBuffer, sizeof(RecvBuffer));
  return Status;
}

/**
  This command is used to RSA decryption

  @param[in]      KeyHandle           RSA key for decryption.
  @param[in]      AuthSession         Auth Session
  @param[in]      CipherText          Cipher text to decrypted.
  @param[in]      Scheme              Padding scheme to use.
  @param[in]      SchemeWidthInBytes  Use width in bytes in Scheme.
  @param[in]      Label               Lable.
  @param[in, out] Message             Decrypted output.

  @retval EFI_SUCCESS             Operation completed successfully.
  @retval EFI_DEVICE_ERROR        Unexpected device behavior.
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Tpm2RsaDecrypt (
  IN     TPMI_DH_OBJECT          KeyHandle,
  IN     TPMS_AUTH_COMMAND       *AuthSession, OPTIONAL
  IN     TPM2B_PUBLIC_KEY_RSA    *CipherText,
  IN     TPMT_RSA_DECRYPT        *Scheme,
  IN     UINT8                   SchemeWidthInBytes,
  IN     TPM2B_DATA              *Label,
  IN OUT TPM2B_PUBLIC_KEY_RSA    *Message
  )
{
  EFI_STATUS                        Status;
  TPM2_RSA_DECRYPT_COMMAND          SendBuffer;
  TPM2_RSA_DECRYPT_RESPONSE         RecvBuffer;
  UINT8                             *Buffer;
  UINT8                             *SchemPtr;
  UINT32                            SendBufferSize;
  UINT32                            RecvBufferSize;
  UINT32                            SessionInfoSize;
  UINTN                             Index;

  if ((CipherText == NULL) || (SchemeWidthInBytes == 0) ||(Scheme == NULL) || (Label == NULL) || (Message == NULL)) {
    return EFI_UNSUPPORTED;
  }

  SendBuffer.Header.tag = SwapBytes16(TPM_ST_SESSIONS);
  SendBuffer.Header.commandCode = SwapBytes32(TPM_CC_RSA_Decrypt);
  SendBuffer.KeyHandle = SwapBytes32(KeyHandle);

  Buffer = (UINT8 *)&SendBuffer.AuthSession;

  SessionInfoSize = CopyAuthSessionCommand (AuthSession, Buffer);
  Buffer += SessionInfoSize;
  SendBuffer.AuthSessionSize = SwapBytes32(SessionInfoSize);

  WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (CipherText->size));
  Buffer += sizeof(UINT16);
  if (CipherText->size > sizeof (SendBuffer.CipherText.buffer)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  CopyMem (Buffer, CipherText->buffer, CipherText->size);
  Buffer += CipherText->size;

  if ((SchemeWidthInBytes > sizeof (TPMT_RSA_DECRYPT)) || ((SchemeWidthInBytes % sizeof (UINT16)) != 0)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  SchemPtr = (UINT8 *) Scheme;
  for (Index = 0; Index < (SchemeWidthInBytes / sizeof (UINT16)); Index++) {
    WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16 (ReadUnaligned16 ((UINT16 *)SchemPtr)));
    SchemPtr+= sizeof(UINT16);
    Buffer += sizeof(UINT16);
  }

  WriteUnaligned16 ((UINT16 *)Buffer, SwapBytes16(Label->size));
  Buffer += sizeof(UINT16);
  if (Label->size > sizeof (SendBuffer.Label.buffer)) {
    Status = EFI_UNSUPPORTED;
    goto Done;
  }
  CopyMem (Buffer, Label->buffer, Label->size);
  Buffer += Label->size;

  SendBufferSize = (UINT32)((UINTN)Buffer - (UINTN)&SendBuffer);
  SendBuffer.Header.paramSize = SwapBytes32 (SendBufferSize);
  RecvBufferSize = sizeof (RecvBuffer);
  Status = Tpm2SubmitCommand (SendBufferSize, (UINT8 *)&SendBuffer, &RecvBufferSize, (UINT8 *)&RecvBuffer);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (RecvBufferSize < sizeof (TPM2_RESPONSE_HEADER)) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaDecrypt - RecvBufferSize Error - %x\n", RecvBufferSize));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }
  if (SwapBytes32(RecvBuffer.Header.responseCode) != TPM_RC_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaDecrypt - responseCode - %x\n", SwapBytes32(RecvBuffer.Header.responseCode)));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }

  Message->size = SwapBytes16 (RecvBuffer.Message.size);
  if (Message->size > MAX_RSA_KEY_BYTES) {
    DEBUG ((DEBUG_ERROR, "Tpm2RsaDecrypt - OutData->size error %x\n", Message->size));
    Status = EFI_DEVICE_ERROR;
    goto Done;
  }
  CopyMem (Message->buffer, &RecvBuffer.Message.buffer, Message->size);

Done:
  ZeroMem (&SendBuffer, sizeof(SendBuffer));
  ZeroMem (&RecvBuffer, sizeof(RecvBuffer));
  return Status;
}
