/** @file
  Public include file for extend TPM2 commands

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

#ifndef _TPM2_EXT_COMMAND_LIB_H_
#define _TPM2_EXT_COMMAND_LIB_H_

#include <IndustryStandard/Tpm20.h>

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
  );

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
  );

#endif