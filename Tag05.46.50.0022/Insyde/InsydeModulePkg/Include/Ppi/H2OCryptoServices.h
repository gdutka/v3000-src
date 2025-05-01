/** @file
  Crypto Service PPI

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

#ifndef _H2O_CRYPTO_SERVICES_PPI_H_
#define _H2O_CRYPTO_SERVICES_PPI_H_

#include <Library/BaseCryptLib.h>

//
// Crypto Service Protocol GUID.
//
#define H2O_CRYPTO_SERVICES_PPI_GUID \
  { \
    0xe1475e0c, 0x1746, 0x4802, { 0x86, 0x2e, 0x1, 0x1c, 0x2c, 0x2d, 0x9d, 0x86 } \
  }

typedef struct _H2O_CRYPTO_SERVICES_PPI  H2O_CRYPTO_SERVICES_PPI;

typedef
UINTN
(EFIAPI * CRYPTO_SERVICES_GET_CONTEXT_SIZE) (
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
(EFIAPI *CRYPTO_SERVICES_INIT) (
  OUT  VOID  *ShaContext
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_CBC_DECRYPT) (
  IN   VOID                                 *AesContext,
  IN   CONST UINT8                          *Input,
  IN   UINTN                                InputSize,
  IN   CONST UINT8                          *Ivec,
  OUT  UINT8                                *Output
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_AES_INIT) (
  OUT  VOID                                 *Context,
  IN   CONST UINT8                          *Key,
  IN   UINTN                                KeyLength
  );

typedef
BOOLEAN
(EFIAPI *CRYPTO_SERVICES_PBKDF2_CREATE_KEY) (
  IN   CONST UINT8                          *Salt,
  IN   UINTN                                SaltLen,
  IN   UINTN                                Algorithm,
  IN   UINTN                                IterationCount,
  IN   CONST UINT8                          *Password,
  IN   UINTN                                PasswordLen,
  IN   UINTN                                DerivedKeyLen,
  OUT  UINT8                                *DerivedKey
  );

//
// H2O Crypto Service PPI Structure.
//
struct _H2O_CRYPTO_SERVICES_PPI {
  UINT32                                        Size;

  CRYPTO_SERVICES_GET_CONTEXT_SIZE              Sha256GetContextSize;
  CRYPTO_SERVICES_INIT                          Sha256Init;
  CRYPTO_SERVICES_DUPLICATE                     Sha256Duplicate;
  CRYPTO_SERVICES_UPDATE                        Sha256Update;
  CRYPTO_SERVICES_FINAL                         Sha256Final;

  CRYPTO_SERVICES_CBC_DECRYPT                   AesCbcDecrypt;
  CRYPTO_SERVICES_GET_CONTEXT_SIZE              AesGetContextSize;
  CRYPTO_SERVICES_AES_INIT                      AesInit;
  CRYPTO_SERVICES_PBKDF2_CREATE_KEY             Pbkdf2CreateKey;
};

extern EFI_GUID gH2OCryptoServicesPpiGuid;

#endif
