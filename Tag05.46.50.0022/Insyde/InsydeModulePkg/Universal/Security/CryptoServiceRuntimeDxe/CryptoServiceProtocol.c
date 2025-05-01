/** @file
  CryptoService common function defination

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include "CryptoHash.h"
#include "CryptoServiceProtocol.h"

BOOLEAN
EFIAPI
H2OPkcsVerify (
  IN  CONST UINT8  *P7Data,
  IN  UINTN        P7Length,
  IN  CONST UINT8  *TrustedCert,
  IN  UINTN        CertLength,
  IN  CONST UINT8  *InData,
  IN  UINTN        DataLength,
  IN  BOOLEAN      AuthentiCodeFlag
  )
{
  return Pkcs7Verify (
           P7Data,
           P7Length,
           TrustedCert,
           CertLength,
           InData,
           DataLength
           );

}

/*
  Setup CryptoService Protocol Functions

  @parma ImageHandle          Image Handle
  @parma ProtocolServices     Protocol Services Interface
  @parma ByProtocol           @TRUE, the CryptoService protocol will replace with HashProtocol and CryptoService.
                              @FALSE, it will be put driver's function.
  @parma HashProtocol         Replace Hash protocol
  @parma HashProtocol         Replace Hash2 protocol
  @parma CryptoService        Replace CryptoService protocol
*/
VOID
SetupCryptoService (
  IN     EFI_HANDLE                    ImageHandle,
  IN OUT CRYPTO_SERVICE                *ProtocolServices,
  IN     BOOLEAN                       ByProtocol,
  IN     EFI_HASH_PROTOCOL             *HashProtocol,
  IN     EFI_HASH2_PROTOCOL            *Hash2Protocol,
  IN     CRYPTO_SERVICES_PROTOCOL      *CryptoProtocol,
  IN     CRYPTO_SERVICES2_PROTOCOL     *Crypto2Protocol
  )
{
   EFI_HASH_PROTOCOL                   *Hash;
   EFI_HASH2_PROTOCOL                  *Hash2;
   CRYPTO_SERVICES_PROTOCOL            *Crypto;
   CRYPTO_SERVICES2_PROTOCOL           *Crypto2;

  if (ByProtocol) {
    if ((HashProtocol == NULL) || (CryptoProtocol == NULL) || (Crypto2Protocol == NULL)) {
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return;
    }
  }

  ZeroMem (ProtocolServices, sizeof (CRYPTO_SERVICE));
  Crypto                          = &(ProtocolServices->CryptoServiceProtocol);
  Crypto2                         = &(ProtocolServices->CryptoService2Protocol);
  Hash                            = &(ProtocolServices->HashInstance.HashProtocol);
  Hash2                           = &(ProtocolServices->Hash2Instance.Hash2Protocol);

  ProtocolServices->Signature     = CRYPTO_SERVICE_SIGNATURE;
  ProtocolServices->Handle        = ImageHandle;

  ProtocolServices->HashInstance.Signature = CRYPTO_HASH_SIGNATURE;
  ProtocolServices->HashInstance.IsRoot    = TRUE;

  ProtocolServices->Hash2Instance.Signature = HASH2_INSTANCE_DATA_SIGNATURE;
  ProtocolServices->Hash2Instance.IsRoot    = TRUE;

  ProtocolServices->CryptoService2Protocol.Size = sizeof (CRYPTO_SERVICES2_PROTOCOL);
  //
  // Hash Protocol Initial
  //
  Hash->GetHashSize                 = (!ByProtocol) ? CryptGetHashSize          : HashProtocol->GetHashSize;
  Hash->Hash                        = (!ByProtocol) ? CryptHash                 : HashProtocol->Hash;
  //
  // Hash2 Protocol Initial
  //
  Hash2->GetHashSize                 = (!ByProtocol) ? BaseCrypto2GetHashSize   : Hash2Protocol->GetHashSize;
  Hash2->Hash                        = (!ByProtocol) ? BaseCrypto2Hash          : Hash2Protocol->Hash;
  Hash2->HashInit                    = (!ByProtocol) ? BaseCrypto2HashInit      : Hash2Protocol->HashInit;
  Hash2->HashUpdate                  = (!ByProtocol) ? BaseCrypto2HashUpdate    : Hash2Protocol->HashUpdate;
  Hash2->HashFinal                   = (!ByProtocol) ? BaseCrypto2HashFinal     : Hash2Protocol->HashFinal;

  //
  // Crytpto Services Protocol Initial
  //
  SET_CRYPTO_FUNCTION (Sha1GetContextSize);
  SET_CRYPTO_FUNCTION (Sha1Init);
  SET_CRYPTO_FUNCTION (Sha1Duplicate);
  SET_CRYPTO_FUNCTION (Sha1Update);
  SET_CRYPTO_FUNCTION (Sha1Final);

  SET_CRYPTO_FUNCTION (HmacSha1GetContextSize);
  SET_CRYPTO_FUNCTION (HmacSha1Init);
  SET_CRYPTO_FUNCTION (HmacSha1Duplicate);
  SET_CRYPTO_FUNCTION (HmacSha1Update);
  SET_CRYPTO_FUNCTION (HmacSha1Final);

  SET_CRYPTO_FUNCTION (TdesGetContextSize);
  SET_CRYPTO_FUNCTION (TdesInit);
  SET_CRYPTO_FUNCTION (TdesEcbEncrypt);
  SET_CRYPTO_FUNCTION (TdesEcbDecrypt);
  SET_CRYPTO_FUNCTION (TdesCbcEncrypt);
  SET_CRYPTO_FUNCTION (TdesCbcDecrypt);

  SET_CRYPTO_FUNCTION (AesGetContextSize);
  SET_CRYPTO_FUNCTION (AesInit);
  SET_CRYPTO_FUNCTION (AesEcbEncrypt);
  SET_CRYPTO_FUNCTION (AesEcbDecrypt);
  SET_CRYPTO_FUNCTION (AesCbcEncrypt);
  SET_CRYPTO_FUNCTION (AesCbcDecrypt);

  SET_CRYPTO_FUNCTION (RsaNew);
  SET_CRYPTO_FUNCTION (RsaFree);
  SET_CRYPTO_FUNCTION (RsaGetKey);
  SET_CRYPTO_FUNCTION (RsaSetKey);

  SET_CRYPTO_FUNCTION (RsaPkcs1Sign);
  SET_CRYPTO_FUNCTION (RsaPkcs1Verify);

  //
  // Original prototype of Pkcs7Verify is different with EDKII prototype. Change the prototype to same with
  // EDKII in BaseCryptLib. But we still need make sure the interface of CryptoProtocol isn't changed to
  // backward compatible. Convert the BascryptLib Pkc7Verify  to  CryptoProtocol Pkc7Verify here.
  //
  Crypto->Pkcs7Verify        = (!ByProtocol) ? H2OPkcsVerify : CryptoProtocol->Pkcs7Verify;
  Crypto2->Pkcs7Verify       = (!ByProtocol) ? H2OPkcsVerify : Crypto2Protocol->Pkcs7Verify;
//  SET_CRYPTO_FUNCTION (Pkcs7Verify);

  SET_CRYPTO_FUNCTION (Pkcs7VerifyUsingPubKey);

  SET_CRYPTO_FUNCTION (RsaGetPrivateKeyFromPem);
  SET_CRYPTO_FUNCTION (RsaGetPublicKeyFromX509);

  SET_CRYPTO_FUNCTION (X509GetSubjectName);
  SET_CRYPTO_FUNCTION (X509VerifyCert);

  SET_CRYPTO_FUNCTION (DhNew);
  SET_CRYPTO_FUNCTION (DhFree);
  SET_CRYPTO_FUNCTION (DhGenerateParameter);
  SET_CRYPTO_FUNCTION (DhSetParameter);
  SET_CRYPTO_FUNCTION (DhGenerateKey);
  SET_CRYPTO_FUNCTION (DhComputeKey);

  SET_CRYPTO_FUNCTION (RandomSeed);
  SET_CRYPTO_FUNCTION (RandomBytes);

  SET_CRYPTO_FUNCTION (AuthenticodeVerify);

  SET_CRYPTO_FUNCTION (Pkcs7GetSigners);
  SET_CRYPTO_FUNCTION (Pkcs7FreeSigners);
  SET_CRYPTO_FUNCTION (RsaPkcs1Decrypt);
  SET_CRYPTO_FUNCTION (X509GetIssuerName);
  SET_CRYPTO_FUNCTION (ImageTimestampVerify);
  SET_CRYPTO_FUNCTION (X509GetTBSCert);

  SET_CRYPTO_FUNCTION (HmacSha256GetContextSize);
  SET_CRYPTO_FUNCTION (HmacSha256Init);
  SET_CRYPTO_FUNCTION (HmacSha256Duplicate);
  SET_CRYPTO_FUNCTION (HmacSha256Update);
  SET_CRYPTO_FUNCTION (HmacSha256Final);
  SET_CRYPTO_FUNCTION (Pkcs7GetCertificatesList);
  SET_CRYPTO_FUNCTION (Pbkdf2CreateKey);

  SET_CRYPTO_FUNCTION (Sm3GetContextSize);
  SET_CRYPTO_FUNCTION (Sm3Init);
  SET_CRYPTO_FUNCTION (Sm3Duplicate);
  SET_CRYPTO_FUNCTION (Sm3Update);
  SET_CRYPTO_FUNCTION (Sm3Final);

  SET_CRYPTO_FUNCTION (RsaPkcs1Encrypt);
  SET_CRYPTO_FUNCTION (RsaPkcs1OaepEncrypt);
  SET_CRYPTO_FUNCTION (RsaPkcs1OaepDecrypt);
  SET_CRYPTO_FUNCTION (RsaPkcs1PssVerify);

  SET_CRYPTO_FUNCTION (HkdfExtractAndExpand);

  SET_CRYPTO_FUNCTION (X509GetCaCertificateFromPemToDer);

  SET_CRYPTO_FUNCTION (X509GetSubjectNameEx);
  SET_CRYPTO_FUNCTION (X509GetIssuerNameEx);

  SET_CRYPTO_FUNCTION (HmacMd5New);
  SET_CRYPTO_FUNCTION (HmacMd5Free);
  SET_CRYPTO_FUNCTION (HmacSha1New);
  SET_CRYPTO_FUNCTION (HmacSha1Free);
  SET_CRYPTO_FUNCTION (HmacSha256New);
  SET_CRYPTO_FUNCTION (HmacSha256Free);

  SET_CRYPTO_FUNCTION (EccNewKey);
  SET_CRYPTO_FUNCTION (EccFree);
  SET_CRYPTO_FUNCTION (EccSetKey);
  SET_CRYPTO_FUNCTION (EcdsaVerify);

  SET_CRYPTO_FUNCTION (Sha256GetContextSize);
  SET_CRYPTO_FUNCTION (Sha256Init);
  SET_CRYPTO_FUNCTION (Sha256Duplicate);
  SET_CRYPTO_FUNCTION (Sha256Update);
  SET_CRYPTO_FUNCTION (Sha256Final);

  SET_CRYPTO_FUNCTION (Sha384GetContextSize);
  SET_CRYPTO_FUNCTION (Sha384Init);
  SET_CRYPTO_FUNCTION (Sha384Duplicate);
  SET_CRYPTO_FUNCTION (Sha384Update);
  SET_CRYPTO_FUNCTION (Sha384Final);

  SET_CRYPTO_FUNCTION (Sha512GetContextSize);
  SET_CRYPTO_FUNCTION (Sha512Init);
  SET_CRYPTO_FUNCTION (Sha512Duplicate);
  SET_CRYPTO_FUNCTION (Sha512Update);
  SET_CRYPTO_FUNCTION (Sha512Final);

  SET_CRYPTO_FUNCTION (RsaPssVerify);
  SET_CRYPTO_FUNCTION (RsaPssSign);
  SET_CRYPTO_FUNCTION (HashDataUsingPkc7DataHashType);
  SET_CRYPTO_FUNCTION (Pkcs7VerifyUsingHash);
  SET_CRYPTO_FUNCTION (Md5HashAll);
  SET_CRYPTO_FUNCTION (Sha1HashAll);
  SET_CRYPTO_FUNCTION (Sha256HashAll);
  SET_CRYPTO_FUNCTION (Sha384HashAll);
  SET_CRYPTO_FUNCTION (Sha512HashAll);
  SET_CRYPTO_FUNCTION (Sm3HashAll);
  SET_CRYPTO_FUNCTION (X509GetCommonName);
  SET_CRYPTO_FUNCTION (X509GetOrganizationName);
  SET_CRYPTO_FUNCTION (Pkcs5HashPassword);
  SET_CRYPTO_FUNCTION (HkdfSha256ExtractAndExpand);
  SET_CRYPTO_FUNCTION (ParallelHash256HashAll);
}
