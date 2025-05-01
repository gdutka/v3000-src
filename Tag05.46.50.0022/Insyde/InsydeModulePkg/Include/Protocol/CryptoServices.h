/** @file
  Crypto Service Protocol

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

/*++
  The crypto service protocol.
  Only limited crypto primitives (SHA-256 and RSA) are provided for runtime
  authenticated variable service.

Copyright (c) 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

--*/

#ifndef __CRYPTO_SERVICES_PROTOCOL_H__
#define __CRYPTO_SERVICES_PROTOCOL_H__

#include <Library/BaseCryptLib.h>

//
// Crypto Service Protocol GUID.
//
#define CRYPTO_SERVICES_PROTOCOL_GUID \
  { \
    0xe1475e0c, 0x1746, 0x4802, { 0x86, 0x2e, 0x1, 0x1c, 0x2c, 0x2d, 0x9d, 0x86 } \
  }

//
// Crypto Service2 Protocol GUID.
//
#define CRYPTO_SERVICES2_PROTOCOL_GUID \
  { \
    0xc67fd6da, 0x1d11, 0x4dc6, { 0x96, 0x36, 0x2c, 0xbe, 0xe9, 0x85, 0x60, 0xae } \
  }


typedef struct _CRYPTO_SERVICES_PROTOCOL  CRYPTO_SERVICES_PROTOCOL;
typedef struct _CRYPTO_SERVICES2_PROTOCOL CRYPTO_SERVICES2_PROTOCOL;

typedef
UINTN
(EFIAPI *CRYPTO_SERVICES_GET_CONTEXT_SIZE) (
  VOID
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_DUPLICATE) (
  IN   CONST VOID                           *Context,
  OUT  VOID                                 *NewContext
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_UPDATE) (
  IN OUT  VOID                              *Context,
  IN      CONST VOID                        *Data,
  IN      UINTN                             DataSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_FINAL) (
  IN OUT  VOID                              *Context,
  OUT     UINT8                             *Value
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_HASH_INIT) (
  OUT  VOID  *HashContext
  );


typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_INIT) (
  OUT  VOID                                 *Context,
  IN   CONST UINT8                          *Key,
  IN   UINTN                                KeyLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_HASH_ALL) (
  IN   CONST VOID                           *Data,
  IN   UINTN                                DataSize,
  OUT  UINT8                                *HashValue
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_ECB_ENCRYPT) (
  IN   VOID                                 *Context,
  IN   CONST UINT8                          *Input,
  IN   UINTN                                InputSize,
  OUT  UINT8                                *Output
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_ECB_DECRYPT) (
  IN   VOID                                 *Context,
  IN   CONST UINT8                          *Input,
  IN   UINTN                                InputSize,
  OUT  UINT8                                *Output
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_CBC_ENCRYPT) (
  IN   VOID                                 *Context,
  IN   CONST UINT8                          *Input,
  IN   UINTN                                InputSize,
  IN   CONST UINT8                          *Ivec,
  OUT  UINT8                                *Output
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_CBC_DECRYPT) (
  IN   VOID                                  *Context,
  IN   CONST UINT8                           *Input,
  IN   UINTN                                 InputSize,
  IN   CONST UINT8                           *Ivec,
  OUT  UINT8                                 *Output
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_GET_KEY) (
  IN OUT VOID                                  *RsaContext,
  IN     RSA_KEY_TAG                           KeyTag,
  OUT    UINT8                                 *BigNumber,
  IN OUT UINTN                                 *BnLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_PKCS1_SIGN) (
  IN      VOID                              *RsaContext,
  IN      CONST UINT8                       *MessageHash,
  IN      UINTN                             HashSize,
  OUT     UINT8                             *Signature,
  IN OUT  UINTN                             *SigSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_GET_PRIVATE_KEY_FROM_PEM) (
  IN   CONST UINT8                                       *PemData,
  IN   UINTN                                             PemSize,
  IN   CONST CHAR8                                       *Password,
  OUT  VOID                                              **RsaContext
  );

/**
  Translate certificate content from PEM to DER-encoded.

  @param[in]      PemData       Pointer to the PEM file of certificate.
  @param[in]      PemSize       Size of the OEM file of certificate in bytes.
  @param[out]     DerData       Pointer to the binary in DER-encoded.
  @param[out]     DerDataSize   Size of DER-encoded binary in bytes.

  @retval  TRUE   The certificate binary translate from PEM to DER successfully.
  @retval  FALSE  Invalid PEM content of certificate.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_CA_CERT_FROM_PEM_TO_DER) (
  IN   CONST UINT8                     *PemData,
  IN   UINTN                           PemSize,
  OUT  UINT8                           **DerData,
  OUT  UINTN                           *DerDataSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_GET_PUBLIC_KEY_FROM_X509) (
  IN   CONST UINT8                                       *Cert,
  IN   UINTN                                             CertSize,
  OUT  VOID                                              **RsaContext
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_GET_SUBJECT_NAME) (
  IN      CONST UINT8                                    *Cert,
  IN      UINTN                                          CertSize,
  OUT     UINT8                                          *CertSubject,
  IN OUT  UINTN                                          *SubjectSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_VERIFY_CERT) (
  IN  CONST UINT8                                    *Cert,
  IN  UINTN                                          CertSize,
  IN  CONST UINT8                                    *CACert,
  IN  UINTN                                          CACertSize
  );

typedef
VOID *
(EFIAPI *CRYPTO_SERVICES_DH_NEW) (
  VOID
  );

typedef
VOID
(EFIAPI *CRYPTO_SERVICES_DH_FREE) (
  IN  VOID                                               *DhContext
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_DH_GENERATE_PARAMETER) (
  IN OUT  VOID                                           *DhContext,
  IN      UINTN                                          Generator,
  IN      UINTN                                          PrimeLength,
  OUT     UINT8                                          *Prime
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_DH_SET_PARAMETER) (
  IN OUT  VOID                                           *DhContext,
  IN      UINTN                                          Generator,
  IN      UINTN                                          PrimeLength,
  IN      CONST UINT8                                    *Prime
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_DH_GENERATE_KEY) (
  IN OUT  VOID                                           *DhContext,
  OUT     UINT8                                          *PublicKey,
  IN OUT  UINTN                                          *PublicKeySize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_DH_COMPUTE_KEY) (
  IN OUT  VOID                                           *DhContext,
  IN      CONST UINT8                                    *PeerPublicKey,
  IN      UINTN                                          PeerPublicKeySize,
  OUT     UINT8                                          *Key,
  IN OUT  UINTN                                          *KeySize
  );

typedef
VOID *
(EFIAPI *CRYPTO_SERVICES_RSA_NEW) (
  VOID
  );

typedef
VOID
(EFIAPI *CRYPTO_SERVICES_RSA_FREE) (
  IN  VOID                                  *RsaContext
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_SET_KEY) (
  IN OUT VOID                                  *RsaContext,
  IN     RSA_KEY_TAG                           KeyTag,
  IN     CONST UINT8                           *BigNumber,
  IN     UINTN                                 BnLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_PKCS1_VERIFY) (
  IN  VOID                                  *RsaContext,
  IN  CONST UINT8                           *MessageHash,
  IN  UINTN                                 HashLength,
  IN  UINT8                                 *Signature,
  IN  UINTN                                 SigLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PKCS7_VERIFY) (
  IN  CONST UINT8                           *P7Data,
  IN  UINTN                                 P7Length,
  IN  CONST UINT8                           *TrustedCert,
  IN  UINTN                                 CertLength,
  IN  CONST UINT8                           *InData,
  IN  UINTN                                 DataLength,
  IN  BOOLEAN                               AuthentiCodeFlag
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PKCS7_VERIFY_USING_PUBKEY) (
  IN	CONST UINT8                                        *P7Data,
  IN  UINTN 	                                           P7Length,
  IN	CONST UINT8                                        *TrustedPubKey,
  IN	UINTN		                                           TrustedPubKeyLen,
  IN  CONST UINT8                                        *InData,
  IN  UINTN 	                                           DataLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_AUTHENTICODE_VERIFY) (
  IN  CONST UINT8                                        *AuthData,
  IN  UINTN                                              DataLength,
  IN  CONST UINT8                                        *TrustedCert,
  IN  UINTN                                              CertLength,
  IN  CONST UINT8                                        *ImageHash,
  IN  UINTN                                              HashLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RAMDOM_SEED) (
  IN  CONST  UINT8                                       *Seed  OPTIONAL,
  IN  UINTN                                              SeedSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RANDOM_BYTES) (
  OUT  UINT8                                             *Output,
  IN   UINTN                                             Size
  );

typedef
BOOLEAN
(EFIAPI *EFI_CRYPTO_SERVICE_PKCS7_GET_SIGNERS) (
  IN  CONST UINT8  *P7Data,
  IN  UINTN        P7Length,
  OUT UINT8        **CertStack,
  OUT UINTN        *StackLength,
  OUT UINT8        **TrustedCert,
  OUT UINTN        *CertLength
  );

typedef
VOID
(EFIAPI *EFI_CRYPTO_SERVICE_PKCS7_FREE_SIGNERS) (
  IN  UINT8        *Certs
  );

/**
  Decrypt the RSA-SSA signature with EMSA-PKCS1-v1_5 encoding scheme defined in
  RSA PKCS#1.

  @param[in]     RsaContext     Pointer to RSA context for signature verification.
  @param[in]     Signature      Pointer to RSA PKCS1-v1_5 signature to be verified.
  @param[in]     SignatureSize  Size of signature in bytes.
  @param[out]    Message        Pointer to buffer to receive RSA PKCS1-v1_5 decrypted message.
  @param[in,out] MessageSize    Size of the decrypted message in bytes.

  @retval  EFI_SUCCESS            Decrypt input signature successfully.
  @retval  EFI_BUFFER_TOO_SMALL   The Message buffer was too small. The current buffer size needed to hold the
                                  message is returned in MessageSize.
  @retval  EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the results.
  @retval  EFI_INVALID_PARAMETER  1. RsaContext is NULL, Signature is NULL or SignatureSize is 0
                                  2. MessageSize is NULL or the Content of MessageSize isn't 0 and Message is NULL.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_CRYPTO_SERVICE_RSA_PKCS1_DECRYPT) (
  IN     VOID        *RsaContext,
  IN     CONST UINT8 *Signature,
  IN     UINTN       SignatureSize,
  OUT    UINT8       *Message,
  IN OUT UINTN       *MessageSize
  );

/**
  Retrieve the issuer bytes from one X.509 certificate.

  @param[in]      Cert         Pointer to the DER-encoded X509 certificate.
  @param[in]      CertSize     Size of the X509 certificate in bytes.
  @param[out]     CertIssuer   Pointer to the retrieved certificate issuer bytes.
  @param[in, out] IssuerSize   The size in bytes of the CertIssuer buffer on input,
                               and the size of buffer returned CertIssuer on output.

  If Cert is NULL, then return FALSE.
  If SubjectSize is NULL, then return FALSE.

  @retval  TRUE   The certificate issuer retrieved successfully.
  @retval  FALSE  Invalid certificate, or the IssuerSize is too small for the result.
                  The IssuerSize will be updated with the required size.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_GET_ISSUER_NAME) (
  IN      CONST UINT8                                    *Cert,
  IN      UINTN                                          CertSize,
  OUT     UINT8                                          *CertIssuer,
  IN OUT  UINTN                                          *IssuerSize
  );

/**
  Get subject content and translate to string from one X.509 certificate.
  e.g.
    /C=SL/ST=Western/L=Colombo/OU=Insyde/CN=Insyde.com

  @param[in]      Cert           Pointer to the DER-encoded X509 certificate.
  @param[in]      CertSize       Size of the X509 certificate in bytes.
  @param[out]     SubjectString  Pointer to the string of subject content in UTF-8
  @param[out]     SubjectSize    The size in bytes of the SubjectString buffer,

  If Cert is NULL, then return FALSE.
  If SubjectSize is NULL, then return FALSE.

  @retval  TRUE   The certificate subject retrieved successfully.
  @retval  FALSE  Invalid certificate, or the SubjectSize is too small for the result.
                  The SubjectSize will be updated with the required size.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_GET_SUBJECT_NAME_EX) (
  IN      CONST UINT8                  *Cert,
  IN      UINTN                        CertSize,
  OUT     UINT16                       **SubjectString,
  OUT     UINTN                        *SubjectSize
  );

/**
  Get subject content and translate to string from one X.509 certificate.
  e.g.
    /C=SL/ST=Western/L=Colombo/OU=Insyde/CN=Insyde.com

  @param[in]      Cert           Pointer to the DER-encoded X509 certificate.
  @param[in]      CertSize       Size of the X509 certificate in bytes.
  @param[out]     IssuerString   Pointer to the string of subject content in UTF-8
  @param[out]     IssuerSize    The size in bytes of the SubjectString buffer,

  If Cert is NULL, then return FALSE.
  If IssuerSize is NULL, then return FALSE.

  @retval  TRUE   The certificate subject retrieved successfully.
  @retval  FALSE  Invalid certificate, or the IssuerSize is too small for the result.
                  The IssuerSize will be updated with the required size.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_GET_ISSUER_NAME_EX) (
  IN      CONST UINT8                  *Cert,
  IN      UINTN                        CertSize,
  OUT     UINT16                       **IssuerString,
  OUT     UINTN                        *IssuerSize
  );

/**
  Verifies the validility of a RFC3161 Timestamp CounterSignature embedded in PE/COFF Authenticode
  signature.

  If AuthData is NULL, then return FALSE.

  @param[in]  AuthData     Pointer to the Authenticode Signature retrieved from signed
                           PE/COFF image to be verified.
  @param[in]  DataSize     Size of the Authenticode Signature in bytes.
  @param[in]  TsaCert      Pointer to a trusted/root TSA certificate encoded in DER, which
                           is used for TSA certificate chain verification.
  @param[in]  CertSize     Size of the trusted certificate in bytes.
  @param[out] SigningTime  Return the time of timestamp generation time if the timestamp
                           signature is valid.

  @retval  TRUE   The specified Authenticode includes a valid RFC3161 Timestamp CounterSignature.
  @retval  FALSE  No valid RFC3161 Timestamp CounterSignature in the specified Authenticode data.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_IMAGE_TIMESTAMP_VERIFY) (
  IN  CONST UINT8          *AuthData,
  IN  UINTN                DataSize,
  IN  CONST UINT8          *TsaCert,
  IN  UINTN                CertSize,
  OUT EFI_TIME             *SigningTime
  );

/**
  Retrieve the TBSCertificate from one given X.509 certificate.

  @param[in]      Cert         Pointer to the given DER-encoded X509 certificate.
  @param[in]      CertSize     Size of the X509 certificate in bytes.
  @param[out]     TBSCert      DER-Encoded To-Be-Signed certificate.
  @param[out]     TBSCertSize  Size of the TBS certificate in bytes.

  If Cert is NULL, then return FALSE.
  If TBSCert is NULL, then return FALSE.
  If TBSCertSize is NULL, then return FALSE.

  @retval  TRUE   The TBSCertificate was retrieved successfully.
  @retval  FALSE  Invalid X.509 certificate.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_X509_GET_TBS_CERT) (
  IN  CONST UINT8       *Cert,
  IN  UINTN             CertSize,
  OUT UINT8             **TBSCert,
  OUT UINTN             *TBSCertSize
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICE_PKCS7_GET_CERTIFICATES_LIST) (
  IN  CONST UINT8  *P7Data,
  IN  UINTN        P7Length,
  OUT UINT8        **SignerChainCerts,
  OUT UINTN        *ChainLength,
  OUT UINT8        **UnchainCerts,
  OUT UINTN        *UnchainLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PBKDF2_CREATE_KEY) (
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltLen,
  IN   UINTN        Algorithm,
  IN   UINTN        IterationCount,
  IN   CONST UINT8  *Password,
  IN   UINTN        PasswordLen,
  IN   UINTN        DerivedKeyLen,
  OUT  UINT8        *DerivedKey
  );

/**
  Carries out the RSA-SSA signature generation with EMSA-PKCS1-v1_5 encoding scheme.

  @param[in]       RsaContext    Pointer to RSA context for signature generation.
  @param[in]       Message       Pointer to octet message data to be signed.
  @param[in]       MessageSize   Size of the message data in bytes.
  @param[out]      Signature     Pointer to buffer to receive RSA PKCS1-v1_5 signature.
  @param[in, out]  SigSize       On input, the size of Signature buffer in bytes.
                                 On output,the size of data returned in Signature buffer in bytes.

  @retval  EFI_SUCCESS           Signature successfully generated in PKCS1-v1_5.
  @retval  EFI_BUFFER_TOO_SMALL  Signature generation failed.
  @retval  EFI_INVALID_PARAMETER 1. RsaContext is NULL, Signature is NULL, SigSize is NULL or Message is NULL.
                                 2. input signature size is not equal to key size.
                                 3. The message size is too large. The max size should be smaller than (keysize - 11)
                                    bytes. (ex: RSA2048 ---> Max message size is 245 == 2048/8 - 11)
**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICE_RSA_PKCS1_ENCRYPT) (
  IN      VOID         *RsaContext,
  IN      CONST UINT8  *Message,
  IN      UINTN        MessageSize,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  );


/**
  Carries out the RSA-SSA signature generation with RSAES-OAEP encoding scheme.
  (The supported hash algorithm is SHA1, SHA224, SHA256, SHA384,
  SHA512).

  @param[in]       RsaContext    Pointer to RSA context for signature generation.
  @param[in]       Algorithm     Specify hash algorithm.
  @param[in]       Message       Pointer to octet message data to be signed.
  @param[in]       MessageSize   Size of the message data in bytes.
  @param[out]      Signature     Pointer to buffer to receive RSA RSAES-OAEP signature.
  @param[in, out]  SigSize       On input, the size of Signature buffer in bytes.
                                 On output,the size of data returned in Signature buffer in bytes.

  @retval  EFI_SUCCESS           Signature successfully generated in RSAES-OAEP.
  @retval  EFI_BUFFER_TOO_SMALL  Signature generation failed.
  @retval  EFI_INVALID_PARAMETER 1. RsaContext is NULL, Signature is NULL, SigSize is NULL or Message is NULL.
                                    HashAlgorithm is unsupported.
                                 2. input signature size is not equal to key size.
                                 3. The message size is too large. The max size should be maller than
                                    (keysize - (2 * digest size)  - 2) bytes (ex: RSA2048 with SHA256 = 256 - (2 * 32) - 2 = 190 bytes)
**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICE_RSA_PKCS1_OAEP_ENCRYPT) (
  IN      VOID         *RsaContext,
  IN      EFI_GUID     *HashAlgorithm,
  IN      CONST UINT8  *Message,
  IN      UINTN        MessageSize,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  );

/**
  Decrypt the RSA-SSA signature with RSAES-OAEP encoding scheme defined in
  RSA PKCS#1. (The supported hash algorithm is SHA1, SHA224, SHA256, SHA384,
  SHA512).

  @param[in]      RsaContext       Pointer to RSA context for signature verification.
  @param[in]      Algorithm       Specify hash algorithm.
  @param[in]      Signature        Pointer to RSA RSAES-OAEP signature to be verified.
  @param[in]      SignatureSize    Size of signature in bytes.
  @param[out]     Message          Pointer to buffer to receive RSA RSAES-OAEP decrypted message.
  @param[in,out]  MessageSize      Size of the decrypted message in bytes.

  @retval  EFI_SUCCESS             Decrypt input signature successfully.
  @retval  EFI_BUFFER_TOO_SMALL    The Message buffer was too small.  The current buffer size needed
                                   to hold the message is returned in MessageSize.
                                   2. MessageSize is NULL or the Content of MessageSize isn't 0 and Message is NULL.
                                      HashAlgorithm is unsupported.
  @retval  EFI_OUT_OF_RESOURCES    There is not enough pool memory to store the results.
  @retval  EFI_INVALID_PARAMETER   1. RsaContext is NULL, Signature is NULL or SignatureSize is 0
                                   2. MessageSize is NULL or the Content of MessageSize isn't 0 and Message is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICE_RSA_PKCS1_OAEP_DECRYPT) (
  IN     VOID        *RsaContext,
  IN     EFI_GUID    *HashAlgorithm,
  IN     CONST UINT8 *Signature,
  IN     UINTN       SignatureSize,
  OUT    UINT8       *Message,
  IN OUT UINTN       *MessageSize
  );

/**
  Verifies the RSA-SSA signature with EMSA-PSS encoding scheme defined in
  RSA PKCS#1. (The supported hash algorithm is SHA1, SHA224, SHA256, SHA384,
  SHA512).

  If RsaContext    is NULL, then return FALSE.
  If HashAlgorithm is NULL, then return FALSE.
  If Message       is NULL, then return FALSE.
  If Signature     is NULL, then return FALSE.

  @param[in]  RsaContext     Pointer to RSA context for signature verification.
  @param[in]  HashAlgorithm  Specify hash algorithm.
  @param[in]  Message        Pointer to octet message to be checked.
  @param[in]  MessageSize    Size of the message in bytes.
  @param[in]  Signature      Pointer to RSASSA-PSS signature to be verified.
  @param[in]  SigSize        Size of signature in bytes.
  @param[in]  SaltLen        The length of salt value in bytes.

  @retval  TRUE   Valid signature encoded in  EMSA-PSS.
  @retval  FALSE  Invalid signature or invalid RSA context.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICE_RSA_PKCS1_PSS_VERIFY) (
  IN  VOID         *RsaContext,
  IN  EFI_GUID     *HashAlgorithm,
  IN  CONST UINT8  *Message,
  IN  UINTN        MessageSize,
  IN  UINT8        *Signature,
  IN  UINTN        SigSize,
  IN  UINTN        SaltLen
  );

/**
  Derive key data using HMAC-SHAxxx based KDF.

  @param[in]   HashAlgorithm    Specify hash algorithm. The GUID is defined in UEFI spec.
  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[Out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICE_HKDF_EXTRACT_AND_EXPAND) (
  IN   EFI_GUID     *HashAlgorithm,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,    OPTIONAL
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,    OPTIONAL
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  );


/**
  Allocates and initializes one HMAC_CTX context for subsequent HMAC-MD5 use.

  If this interface is not supported, then return NULL.

  @return  Pointer to the HMAC_CTX context that has been initialized.
           If the allocations fails, HmacMd5New() returns NULL.
  @retval  NULL  This interface is not supported.

**/
typedef
VOID *
(EFIAPI *CRYPTO_SERVICES_NEW) (
  VOID
  );

/**
  Release the specified HMAC_CTX context.

  If this interface is not supported, then do nothing.

  @param[in]  HmacMd5Ctx  Pointer to the HMAC_CTX context to be released.

**/
typedef
VOID
(EFIAPI *CRYPTO_SERVICES_FREE) (
  IN  VOID  *HmacCtx
  );

/**
  Allocates and initializes one ECC context for subsequent use.

  @param[in]   KeyType   Which KeyType to be used to create key. The valid KeyType is below:
                         EC_P192_TYPE, EC_P224_TYPE, EC_P256_TYPE, EC_P384_TYPE or EC_P521_TYPE.

  @return  Pointer to the ECC context that has been initialized. If the allocations fails,
           EccNewKey() returns NULL.
**/
typedef
VOID *
(EFIAPI *CRYPTO_SERVICES_ECC_NEW_KEY) (
  IN    UINT8    KeyType
  );

/**
  Release the specified ECC context.

  If EccContext is NULL, then return FALSE.

  @param[in]  EccContext  Pointer to the ECC context to be released.
**/
typedef
VOID
(EFIAPI *CRYPTO_SERVICES_ECC_FREE) (
  IN  VOID  *EccContext
  );

/**
  Sets the ECC key component into the established ECC context.

  If EccContext is NULL, then return FALSE.

  @param[in, out]  EccContext  Pointer to ECC context being set.
  @param[in]       PubX        Pointer to public X key and it is big endian.
  @param[in]       PubY        Pointer to public Y key and it is big endian.
  @param[in]       PrimeLen    Size of public key in bytes.

  @retval  TRUE   ECC key component was set successfully.
  @retval  FALSE  ECC key component was set failed.
**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_ECC_SET_KEY) (
  IN OUT  VOID         *EccContext,
  IN      VOID         *PubX,
  IN      VOID         *PubY,
  IN      UINT32       PrimeLen
  );

/**
  Verifies the Ecdsa signature.

  If EccContext is NULL, then return FALSE.
  If MessageHash is NULL, then return FALSE.
  If Signature is NULL, then return FALSE.
  If HashSize is 0, then return FALSE.
  If SigR is NULL, then return FALSE.
  If SigS is NULL, then return FALSE.
  If SigLen is 0, then return FALSE.

  @param[in]  EccContext   Pointer to ECC context for signature verification.
  @param[in]  MessageHash  Pointer to octet message hash to be checked.
  @param[in]  HashSize     Size of the message hash in bytes.
  @param[in]  SigR         Pointer to signature R to be verified and it is big endian.
  @param[in]  SigS         Pointer to signature S to be verified and it is big endian.
  @param[in]  SigSize      Size of signature in bytes.

  @retval  TRUE   Valid signature encoded in ECDSA
  @retval  FALSE  Invalid signature or invalid ECC context.
**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_ECDSA_VERIFY) (
  IN VOID         *EccContext,
  IN CONST UINT8  *MessageHash,
  IN UINTN        HashSize,
  IN VOID         *SigR,
  IN VOID         *SigS,
  IN UINT32       SigLen
  );

/**
  Verifies the RSA signature with RSASSA-PSS signature scheme defined in RFC 8017.
  Implementation determines salt length automatically from the signature encoding.
  Mask generation function is the same as the message digest algorithm.
  Salt length should be equal to digest length.

  @param[in]  RsaContext      Pointer to RSA context for signature verification.
  @param[in]  Message         Pointer to octet message to be verified.
  @param[in]  MsgSize         Size of the message in bytes.
  @param[in]  Signature       Pointer to RSASSA-PSS signature to be verified.
  @param[in]  SigSize         Size of signature in bytes.
  @param[in]  DigestLen       Length of digest for RSA operation.
  @param[in]  SaltLen         Salt length for PSS encoding.

  @retval  TRUE   Valid signature encoded in RSASSA-PSS.
  @retval  FALSE  Invalid signature or invalid RSA context.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_PSS_VERIFY) (
  IN  VOID         *RsaContext,
  IN  CONST UINT8  *Message,
  IN  UINTN        MsgSize,
  IN  CONST UINT8  *Signature,
  IN  UINTN        SigSize,
  IN  UINT16       DigestLen,
  IN  UINT16       SaltLen
  );


/**
  Carries out the RSA-SSA signature generation with EMSA-PSS encoding scheme.

  This function carries out the RSA-SSA signature generation with EMSA-PSS encoding scheme defined in
  RFC 8017.
  Mask generation function is the same as the message digest algorithm.
  If the Signature buffer is too small to hold the contents of signature, FALSE
  is returned and SigSize is set to the required buffer size to obtain the signature.

  If RsaContext is NULL, then return FALSE.
  If Message is NULL, then return FALSE.
  If MsgSize is zero or > INT_MAX, then return FALSE.
  If DigestLen is NOT 32, 48 or 64, return FALSE.
  If SaltLen is not equal to DigestLen, then return FALSE.
  If SigSize is large enough but Signature is NULL, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[in]      RsaContext   Pointer to RSA context for signature generation.
  @param[in]      Message      Pointer to octet message to be signed.
  @param[in]      MsgSize      Size of the message in bytes.
  @param[in]      DigestLen    Length of the digest in bytes to be used for RSA signature operation.
  @param[in]      SaltLen      Length of the salt in bytes to be used for PSS encoding.
  @param[out]     Signature    Pointer to buffer to receive RSA PSS signature.
  @param[in, out] SigSize      On input, the size of Signature buffer in bytes.
                               On output, the size of data returned in Signature buffer in bytes.

  @retval  TRUE   Signature successfully generated in RSASSA-PSS.
  @retval  FALSE  Signature generation failed.
  @retval  FALSE  SigSize is too small.
  @retval  FALSE  This interface is not supported.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_RSA_PSS_SIGN) (
  IN      VOID         *RsaContext,
  IN      CONST UINT8  *Message,
  IN      UINTN        MsgSize,
  IN      UINT16       DigestLen,
  IN      UINT16       SaltLen,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  );

/**
  Using hash type which get from PKC7 data to hash input data.

  @param[in]      P7Data       Pointer to the PKCS#7 message to verify.
  @param[in]      P7Length     Length of the PKCS#7 message in bytes.
  @param[in]      InData       Pointer to the content to be verified.
  @param[in]      DataLength   Length of InData in bytes.
  @param[out]     HashData     poiter to output hash data.
  @param[in, out] HashSize     On input, it is the size of hash data buffer size in bytes.
                               On Output, it is size of hash data in bytes.


  @retval  EFI_SUCCESS             Hash input data successful.
  @retval  EFI_OUT_OF_RESOURCES    There was not enough memory in pool to store input data.
  @retval  EFI_INVALID_PARAMETER   P7Data is NULL, DataSize is 0, InData is NULL, InDataSize is 0,
                                   HashData is NULL or HashSize is NULL.
  @retval  EFI_NOT_FOUND           Cannot find hash data in signature.
**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICES_HASH_DATA_USING_PKC7_DATA_HASH_TYPE) (
  IN     CONST UINT8   *AuthData,
  IN     UINTN         DataSize,
  IN     CONST UINT8   *InData,
  IN     UINTN         InDataSize,
  OUT    UINT8         *HashData,
  IN OUT UINTN         *HashSize
  );

/**
  Verifies the validility of a PKCS#7 signed data as described in "PKCS #7: Cryptographic
  Message Syntax Standard".

  If P7Data is NULL, then ASSERT().

  @param[in]  P7Data       Pointer to the PKCS#7 message to verify.
  @param[in]  P7Length     Length of the PKCS#7 message in bytes.
  @param[in]  TrustedCert  Pointer to a trusted/root certificate encoded in DER, which
                           is used for certificate chain verification.
  @param[in]  CertLength   Length of the trusted certificate in bytes.
  @param[in]  HashData     Pointer to hash data.
  @param[in]  HashSize     Hash data size in bytes.

  @retval  TRUE  The specified PKCS#7 signed data is valid.
  @retval  FALSE Invalid PKCS#7 signed data.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PKCS7_VERIFY_USING_HASH) (
  IN  CONST UINT8  *P7Data,
  IN  UINTN        P7Length,
  IN  CONST UINT8  *TrustedCert,
  IN  UINTN        CertLength,
  IN  UINT8        *HashData,
  IN  UINTN        HashSize
  );

/**
  Retrieve the common name (CN) string from one X.509 certificate.

  @param[in]      Cert             Pointer to the DER-encoded X509 certificate.
  @param[in]      CertSize         Size of the X509 certificate in bytes.
  @param[out]     CommonName       Buffer to contain the retrieved certificate common
                                   name string (UTF8). At most CommonNameSize bytes will be
                                   written and the string will be null terminated. May be
                                   NULL in order to determine the size buffer needed.
  @param[in,out]  CommonNameSize   The size in bytes of the CommonName buffer on input,
                                   and the size of buffer returned CommonName on output.
                                   If CommonName is NULL then the amount of space needed
                                   in buffer (including the final null) is returned.

  @retval RETURN_SUCCESS           The certificate CommonName retrieved successfully.
  @retval RETURN_INVALID_PARAMETER If Cert is NULL.
                                   If CommonNameSize is NULL.
                                   If CommonName is not NULL and *CommonNameSize is 0.
                                   If Certificate is invalid.
  @retval RETURN_NOT_FOUND         If no CommonName entry exists.
  @retval RETURN_BUFFER_TOO_SMALL  If the CommonName is NULL. The required buffer size
                                   (including the final null) is returned in the
                                   CommonNameSize parameter.
  @retval RETURN_UNSUPPORTED       The operation is not supported.

**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICES_X509_GET_COMMON_NAME) (
  IN      CONST UINT8  *Cert,
  IN      UINTN        CertSize,
  OUT     CHAR8        *CommonName,  OPTIONAL
  IN OUT  UINTN        *CommonNameSize
  );

/**
  Retrieve the organization name (O) string from one X.509 certificate.

  @param[in]      Cert             Pointer to the DER-encoded X509 certificate.
  @param[in]      CertSize         Size of the X509 certificate in bytes.
  @param[out]     NameBuffer       Buffer to contain the retrieved certificate organization
                                   name string. At most NameBufferSize bytes will be
                                   written and the string will be null terminated. May be
                                   NULL in order to determine the size buffer needed.
  @param[in,out]  NameBufferSize   The size in bytes of the Name buffer on input,
                                   and the size of buffer returned Name on output.
                                   If NameBuffer is NULL then the amount of space needed
                                   in buffer (including the final null) is returned.

  @retval RETURN_SUCCESS           The certificate Organization Name retrieved successfully.
  @retval RETURN_INVALID_PARAMETER If Cert is NULL.
                                   If NameBufferSize is NULL.
                                   If NameBuffer is not NULL and *CommonNameSize is 0.
                                   If Certificate is invalid.
  @retval RETURN_NOT_FOUND         If no Organization Name entry exists.
  @retval RETURN_BUFFER_TOO_SMALL  If the NameBuffer is NULL. The required buffer size
                                   (including the final null) is returned in the
                                   CommonNameSize parameter.
  @retval RETURN_UNSUPPORTED       The operation is not supported.

**/
typedef
EFI_STATUS
(EFIAPI *CRYPTO_SERVICES_X509_GET_ORGANIZATION_NAME) (
  IN      CONST UINT8  *Cert,
  IN      UINTN        CertSize,
  OUT     CHAR8        *CommonName,  OPTIONAL
  IN OUT  UINTN        *CommonNameSize
  );

/**
  Derives a key from a password using a salt and iteration count, based on PKCS#5 v2.0
  password based encryption key derivation function PBKDF2, as specified in RFC 2898.

  If Password or Salt or OutKey is NULL, then return FALSE.
  If the hash algorithm could not be determined, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[in]  PasswordLength  Length of input password in bytes.
  @param[in]  Password        Pointer to the array for the password.
  @param[in]  SaltLength      Size of the Salt in bytes.
  @param[in]  Salt            Pointer to the Salt.
  @param[in]  IterationCount  Number of iterations to perform. Its value should be
                              greater than or equal to 1.
  @param[in]  DigestSize      Size of the message digest to be used (eg. SHA256_DIGEST_SIZE).
                              NOTE: DigestSize will be used to determine the hash algorithm.
                                    Only SHA1_DIGEST_SIZE or SHA256_DIGEST_SIZE is supported.
  @param[in]  KeyLength       Size of the derived key buffer in bytes.
  @param[out] OutKey          Pointer to the output derived key buffer.

  @retval  TRUE   A key was derived successfully.
  @retval  FALSE  One of the pointers was NULL or one of the sizes was too large.
  @retval  FALSE  The hash algorithm could not be determined from the digest size.
  @retval  FALSE  The key derivation operation failed.
  @retval  FALSE  This interface is not supported.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PKCS5_HASH_PASSWORD) (
  IN  UINTN        PasswordLength,
  IN  CONST CHAR8  *Password,
  IN  UINTN        SaltLength,
  IN  CONST UINT8  *Salt,
  IN  UINTN        IterationCount,
  IN  UINTN        DigestSize,
  IN  UINTN        KeyLength,
  OUT UINT8        *OutKey
  );

/**
  Derive key data using HMAC-SHA256 based KDF.

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_HKDF_SHA_256_EXTRACT_AND_EXPAND) (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  );



/**
  Parallel hash function ParallelHash256, as defined in NIST's Special Publication 800-185,
  published December 2016.

  @param[in]   Input            Pointer to the input message (X).
  @param[in]   InputByteLen     The number(>0) of input bytes provided for the input data.
  @param[in]   BlockSize        The size of each block (B).
  @param[out]  Output           Pointer to the output buffer.
  @param[in]   OutputByteLen    The desired number of output bytes (L).
  @param[in]   Customization    Pointer to the customization string (S).
  @param[in]   CustomByteLen    The length of the customization string in bytes.

  @retval TRUE   ParallelHash256 digest computation succeeded.
  @retval FALSE  ParallelHash256 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PARALLEL_HASH_256_HASH_ALL) (
  IN CONST VOID   *Input,
  IN       UINTN  InputByteLen,
  IN       UINTN  BlockSize,
  OUT      VOID   *Output,
  IN       UINTN  OutputByteLen,
  IN CONST VOID   *Customization,
  IN       UINTN  CustomByteLen
  );

//
// Crypto Service Protocol Structure.
// NOTE: this protocol deprecated. Suggest using CRYPTO_SERVICES2_PROTOCO not Below
// CRYPTO_SERVICES_PROTOCOL.
struct _CRYPTO_SERVICES_PROTOCOL {
  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha1GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha1Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha1Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha1Update;
  CRYPTO_SERVICES_FINAL                                 Sha1Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      HmacSha1GetContextSize;
  CRYPTO_SERVICES_INIT                                  HmacSha1Init;
  CRYPTO_SERVICES_DUPLICATE                             HmacSha1Duplicate;
  CRYPTO_SERVICES_UPDATE                                HmacSha1Update;
  CRYPTO_SERVICES_FINAL                                 HmacSha1Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      TdesGetContextSize;
  CRYPTO_SERVICES_INIT                                  TdesInit;
  CRYPTO_SERVICES_ECB_ENCRYPT                           TdesEcbEncrypt;
  CRYPTO_SERVICES_ECB_DECRYPT                           TdesEcbDecrypt;
  CRYPTO_SERVICES_CBC_ENCRYPT                           TdesCbcEncrypt;
  CRYPTO_SERVICES_CBC_DECRYPT                           TdesCbcDecrypt;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      AesGetContextSize;
  CRYPTO_SERVICES_INIT                                  AesInit;
  CRYPTO_SERVICES_ECB_ENCRYPT                           AesEcbEncrypt;
  CRYPTO_SERVICES_ECB_DECRYPT                           AesEcbDecrypt;
  CRYPTO_SERVICES_CBC_ENCRYPT                           AesCbcEncrypt;
  CRYPTO_SERVICES_CBC_DECRYPT                           AesCbcDecrypt;

  CRYPTO_SERVICES_RSA_NEW                               RsaNew;
  CRYPTO_SERVICES_RSA_FREE                              RsaFree;
  CRYPTO_SERVICES_RSA_GET_KEY                           RsaGetKey;
  CRYPTO_SERVICES_RSA_SET_KEY                           RsaSetKey;
  CRYPTO_SERVICES_RSA_PKCS1_SIGN                        RsaPkcs1Sign;
  CRYPTO_SERVICES_RSA_PKCS1_VERIFY                      RsaPkcs1Verify;
  CRYPTO_SERVICES_RSA_GET_PRIVATE_KEY_FROM_PEM          RsaGetPrivateKeyFromPem;
  CRYPTO_SERVICES_RSA_GET_PUBLIC_KEY_FROM_X509          RsaGetPublicKeyFromX509;
  CRYPTO_SERVICES_X509_GET_SUBJECT_NAME                 X509GetSubjectName;
  CRYPTO_SERVICES_X509_VERIFY_CERT                      X509VerifyCert;

  CRYPTO_SERVICES_PKCS7_VERIFY                          Pkcs7Verify;
  CRYPTO_SERVICES_PKCS7_VERIFY_USING_PUBKEY             Pkcs7VerifyUsingPubKey;

  CRYPTO_SERVICES_AUTHENTICODE_VERIFY                   AuthenticodeVerify;

  CRYPTO_SERVICES_DH_NEW                                DhNew;
  CRYPTO_SERVICES_DH_FREE                               DhFree;
  CRYPTO_SERVICES_DH_GENERATE_PARAMETER                 DhGenerateParameter;
  CRYPTO_SERVICES_DH_SET_PARAMETER                      DhSetParameter;
  CRYPTO_SERVICES_DH_GENERATE_KEY                       DhGenerateKey;
  CRYPTO_SERVICES_DH_COMPUTE_KEY                        DhComputeKey;

  CRYPTO_SERVICES_RAMDOM_SEED                           RandomSeed;
  CRYPTO_SERVICES_RANDOM_BYTES                          RandomBytes;

  //
  // For compatibility, add new supported funtion at the end of
  // EFI_CRYPTO_SERVICE_PROTOCOL.
  //
  EFI_CRYPTO_SERVICE_PKCS7_GET_SIGNERS                  Pkcs7GetSigners;
  EFI_CRYPTO_SERVICE_PKCS7_FREE_SIGNERS                 Pkcs7FreeSigners;
  EFI_CRYPTO_SERVICE_RSA_PKCS1_DECRYPT                  RsaPkcs1Decrypt;
  CRYPTO_SERVICES_X509_GET_ISSUER_NAME                  X509GetIssuerName;
  CRYPTO_SERVICES_IMAGE_TIMESTAMP_VERIFY                ImageTimestampVerify;
  CRYPTO_SERVICES_X509_GET_TBS_CERT                     X509GetTBSCert;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      HmacSha256GetContextSize;
  CRYPTO_SERVICES_INIT                                  HmacSha256Init;
  CRYPTO_SERVICES_DUPLICATE                             HmacSha256Duplicate;
  CRYPTO_SERVICES_UPDATE                                HmacSha256Update;
  CRYPTO_SERVICES_FINAL                                 HmacSha256Final;
  CRYPTO_SERVICE_PKCS7_GET_CERTIFICATES_LIST            Pkcs7GetCertificatesList;
  CRYPTO_SERVICES_PBKDF2_CREATE_KEY                     Pbkdf2CreateKey;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sm3GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sm3Init;
  CRYPTO_SERVICES_DUPLICATE                             Sm3Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sm3Update;
  CRYPTO_SERVICES_FINAL                                 Sm3Final;

  CRYPTO_SERVICE_RSA_PKCS1_ENCRYPT                      RsaPkcs1Encrypt;
  CRYPTO_SERVICE_RSA_PKCS1_OAEP_ENCRYPT                 RsaPkcs1OaepEncrypt;
  CRYPTO_SERVICE_RSA_PKCS1_OAEP_DECRYPT                 RsaPkcs1OaepDecrypt;
  CRYPTO_SERVICE_RSA_PKCS1_PSS_VERIFY                   RsaPkcs1PssVerify;
  CRYPTO_SERVICE_HKDF_EXTRACT_AND_EXPAND                HkdfExtractAndExpand;

  CRYPTO_SERVICES_X509_CA_CERT_FROM_PEM_TO_DER          X509GetCaCertificateFromPemToDer;

  CRYPTO_SERVICES_X509_GET_SUBJECT_NAME_EX              X509GetSubjectNameEx;
  CRYPTO_SERVICES_X509_GET_ISSUER_NAME_EX               X509GetIssuerNameEx;
  CRYPTO_SERVICES_NEW                                   HmacMd5New;
  CRYPTO_SERVICES_FREE                                  HmacMd5Free;
  CRYPTO_SERVICES_NEW                                   HmacSha1New;
  CRYPTO_SERVICES_FREE                                  HmacSha1Free;
  CRYPTO_SERVICES_NEW                                   HmacSha256New;
  CRYPTO_SERVICES_FREE                                  HmacSha256Free;
  CRYPTO_SERVICES_ECC_NEW_KEY                           EccNewKey;
  CRYPTO_SERVICES_ECC_FREE                              EccFree;
  CRYPTO_SERVICES_ECC_SET_KEY                           EccSetKey;
  CRYPTO_SERVICES_ECDSA_VERIFY                          EcdsaVerify;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha256GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha256Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha256Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha256Update;
  CRYPTO_SERVICES_FINAL                                 Sha256Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha384GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha384Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha384Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha384Update;
  CRYPTO_SERVICES_FINAL                                 Sha384Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha512GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha512Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha512Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha512Update;
  CRYPTO_SERVICES_FINAL                                 Sha512Final;

  CRYPTO_SERVICES_RSA_PSS_VERIFY                        RsaPssVerify;
  CRYPTO_SERVICES_RSA_PSS_SIGN                          RsaPssSign;
  CRYPTO_SERVICES_HASH_DATA_USING_PKC7_DATA_HASH_TYPE   HashDataUsingPkc7DataHashType;
  CRYPTO_SERVICES_PKCS7_VERIFY_USING_HASH               Pkcs7VerifyUsingHash;
  CRYPTO_SERVICES_HASH_ALL                              Md5HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha1HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha256HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha384HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha512HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sm3HashAll;
  CRYPTO_SERVICES_X509_GET_COMMON_NAME                  X509GetCommonName;
  CRYPTO_SERVICES_X509_GET_ORGANIZATION_NAME            X509GetOrganizationName;
  CRYPTO_SERVICES_PKCS5_HASH_PASSWORD                   Pkcs5HashPassword;
  CRYPTO_SERVICES_HKDF_SHA_256_EXTRACT_AND_EXPAND       HkdfSha256ExtractAndExpand;
  CRYPTO_SERVICES_PARALLEL_HASH_256_HASH_ALL            ParallelHash256HashAll;
};

//
// Crypto Service2 Protocol Structure.
//
struct _CRYPTO_SERVICES2_PROTOCOL {
  UINT32                                                Size;
  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha1GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha1Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha1Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha1Update;
  CRYPTO_SERVICES_FINAL                                 Sha1Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      HmacSha1GetContextSize;
  CRYPTO_SERVICES_INIT                                  HmacSha1Init;
  CRYPTO_SERVICES_DUPLICATE                             HmacSha1Duplicate;
  CRYPTO_SERVICES_UPDATE                                HmacSha1Update;
  CRYPTO_SERVICES_FINAL                                 HmacSha1Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      TdesGetContextSize;
  CRYPTO_SERVICES_INIT                                  TdesInit;
  CRYPTO_SERVICES_ECB_ENCRYPT                           TdesEcbEncrypt;
  CRYPTO_SERVICES_ECB_DECRYPT                           TdesEcbDecrypt;
  CRYPTO_SERVICES_CBC_ENCRYPT                           TdesCbcEncrypt;
  CRYPTO_SERVICES_CBC_DECRYPT                           TdesCbcDecrypt;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      AesGetContextSize;
  CRYPTO_SERVICES_INIT                                  AesInit;
  CRYPTO_SERVICES_ECB_ENCRYPT                           AesEcbEncrypt;
  CRYPTO_SERVICES_ECB_DECRYPT                           AesEcbDecrypt;
  CRYPTO_SERVICES_CBC_ENCRYPT                           AesCbcEncrypt;
  CRYPTO_SERVICES_CBC_DECRYPT                           AesCbcDecrypt;

  CRYPTO_SERVICES_RSA_NEW                               RsaNew;
  CRYPTO_SERVICES_RSA_FREE                              RsaFree;
  CRYPTO_SERVICES_RSA_GET_KEY                           RsaGetKey;
  CRYPTO_SERVICES_RSA_SET_KEY                           RsaSetKey;
  CRYPTO_SERVICES_RSA_PKCS1_SIGN                        RsaPkcs1Sign;
  CRYPTO_SERVICES_RSA_PKCS1_VERIFY                      RsaPkcs1Verify;
  CRYPTO_SERVICES_RSA_GET_PRIVATE_KEY_FROM_PEM          RsaGetPrivateKeyFromPem;
  CRYPTO_SERVICES_RSA_GET_PUBLIC_KEY_FROM_X509          RsaGetPublicKeyFromX509;
  CRYPTO_SERVICES_X509_GET_SUBJECT_NAME                 X509GetSubjectName;
  CRYPTO_SERVICES_X509_VERIFY_CERT                      X509VerifyCert;

  CRYPTO_SERVICES_PKCS7_VERIFY                          Pkcs7Verify;
  CRYPTO_SERVICES_PKCS7_VERIFY_USING_PUBKEY             Pkcs7VerifyUsingPubKey;

  CRYPTO_SERVICES_AUTHENTICODE_VERIFY                   AuthenticodeVerify;

  CRYPTO_SERVICES_DH_NEW                                DhNew;
  CRYPTO_SERVICES_DH_FREE                               DhFree;
  CRYPTO_SERVICES_DH_GENERATE_PARAMETER                 DhGenerateParameter;
  CRYPTO_SERVICES_DH_SET_PARAMETER                      DhSetParameter;
  CRYPTO_SERVICES_DH_GENERATE_KEY                       DhGenerateKey;
  CRYPTO_SERVICES_DH_COMPUTE_KEY                        DhComputeKey;

  CRYPTO_SERVICES_RAMDOM_SEED                           RandomSeed;
  CRYPTO_SERVICES_RANDOM_BYTES                          RandomBytes;

  //
  // For compatibility, add new supported funtion at the end of
  // EFI_CRYPTO_SERVICE_PROTOCOL.
  //
  EFI_CRYPTO_SERVICE_PKCS7_GET_SIGNERS                  Pkcs7GetSigners;
  EFI_CRYPTO_SERVICE_PKCS7_FREE_SIGNERS                 Pkcs7FreeSigners;
  EFI_CRYPTO_SERVICE_RSA_PKCS1_DECRYPT                  RsaPkcs1Decrypt;
  CRYPTO_SERVICES_X509_GET_ISSUER_NAME                  X509GetIssuerName;
  CRYPTO_SERVICES_IMAGE_TIMESTAMP_VERIFY                ImageTimestampVerify;
  CRYPTO_SERVICES_X509_GET_TBS_CERT                     X509GetTBSCert;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      HmacSha256GetContextSize;
  CRYPTO_SERVICES_INIT                                  HmacSha256Init;
  CRYPTO_SERVICES_DUPLICATE                             HmacSha256Duplicate;
  CRYPTO_SERVICES_UPDATE                                HmacSha256Update;
  CRYPTO_SERVICES_FINAL                                 HmacSha256Final;
  CRYPTO_SERVICE_PKCS7_GET_CERTIFICATES_LIST            Pkcs7GetCertificatesList;
  CRYPTO_SERVICES_PBKDF2_CREATE_KEY                     Pbkdf2CreateKey;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sm3GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sm3Init;
  CRYPTO_SERVICES_DUPLICATE                             Sm3Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sm3Update;
  CRYPTO_SERVICES_FINAL                                 Sm3Final;

  CRYPTO_SERVICE_RSA_PKCS1_ENCRYPT                      RsaPkcs1Encrypt;
  CRYPTO_SERVICE_RSA_PKCS1_OAEP_ENCRYPT                 RsaPkcs1OaepEncrypt;
  CRYPTO_SERVICE_RSA_PKCS1_OAEP_DECRYPT                 RsaPkcs1OaepDecrypt;
  CRYPTO_SERVICE_RSA_PKCS1_PSS_VERIFY                   RsaPkcs1PssVerify;
  CRYPTO_SERVICE_HKDF_EXTRACT_AND_EXPAND                HkdfExtractAndExpand;

  CRYPTO_SERVICES_X509_CA_CERT_FROM_PEM_TO_DER          X509GetCaCertificateFromPemToDer;

  CRYPTO_SERVICES_X509_GET_SUBJECT_NAME_EX              X509GetSubjectNameEx;
  CRYPTO_SERVICES_X509_GET_ISSUER_NAME_EX               X509GetIssuerNameEx;
  CRYPTO_SERVICES_NEW                                   HmacMd5New;
  CRYPTO_SERVICES_FREE                                  HmacMd5Free;
  CRYPTO_SERVICES_NEW                                   HmacSha1New;
  CRYPTO_SERVICES_FREE                                  HmacSha1Free;
  CRYPTO_SERVICES_NEW                                   HmacSha256New;
  CRYPTO_SERVICES_FREE                                  HmacSha256Free;

  CRYPTO_SERVICES_ECC_NEW_KEY                           EccNewKey;
  CRYPTO_SERVICES_ECC_FREE                              EccFree;
  CRYPTO_SERVICES_ECC_SET_KEY                           EccSetKey;
  CRYPTO_SERVICES_ECDSA_VERIFY                          EcdsaVerify;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha256GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha256Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha256Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha256Update;
  CRYPTO_SERVICES_FINAL                                 Sha256Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha384GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha384Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha384Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha384Update;
  CRYPTO_SERVICES_FINAL                                 Sha384Final;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE                      Sha512GetContextSize;
  CRYPTO_SERVICES_HASH_INIT                             Sha512Init;
  CRYPTO_SERVICES_DUPLICATE                             Sha512Duplicate;
  CRYPTO_SERVICES_UPDATE                                Sha512Update;
  CRYPTO_SERVICES_FINAL                                 Sha512Final;

  CRYPTO_SERVICES_RSA_PSS_VERIFY                        RsaPssVerify;
  CRYPTO_SERVICES_RSA_PSS_SIGN                          RsaPssSign;
  CRYPTO_SERVICES_HASH_DATA_USING_PKC7_DATA_HASH_TYPE   HashDataUsingPkc7DataHashType;
  CRYPTO_SERVICES_PKCS7_VERIFY_USING_HASH               Pkcs7VerifyUsingHash;
  CRYPTO_SERVICES_HASH_ALL                              Md5HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha1HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha256HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha384HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sha512HashAll;
  CRYPTO_SERVICES_HASH_ALL                              Sm3HashAll;
  CRYPTO_SERVICES_X509_GET_COMMON_NAME                  X509GetCommonName;
  CRYPTO_SERVICES_X509_GET_ORGANIZATION_NAME            X509GetOrganizationName;
  CRYPTO_SERVICES_PKCS5_HASH_PASSWORD                   Pkcs5HashPassword;
  CRYPTO_SERVICES_HKDF_SHA_256_EXTRACT_AND_EXPAND       HkdfSha256ExtractAndExpand;
  CRYPTO_SERVICES_PARALLEL_HASH_256_HASH_ALL            ParallelHash256HashAll;
};

extern EFI_GUID gCryptoServicesProtocolGuid;
extern EFI_GUID gCryptoServices2ProtocolGuid;

#endif
