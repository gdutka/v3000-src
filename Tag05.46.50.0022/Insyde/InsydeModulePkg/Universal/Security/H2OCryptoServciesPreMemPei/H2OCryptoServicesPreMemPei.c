/** @file
  Provide functions abstraction of BaseCryptLib.

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

#include <PiPei.h>
#include <Uefi.h>
#include <Ppi/H2OCryptoServices.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseCryptLib.h>

BOOLEAN
EFIAPI
DummyAesCbcDecrypt (
  IN   VOID         *AesContext,
  IN   CONST UINT8  *Input,
  IN   UINTN        InputSize,
  IN   CONST UINT8  *Ivec,
  OUT  UINT8        *Output
  )
{
  return FALSE;
}

UINTN
EFIAPI
DummyAesGetContextSize (
  VOID
  )
{
  return 0;
}

BOOLEAN
EFIAPI
DummyAesInit (
  OUT  VOID         *AesContext,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeyLength
  )
{
  return FALSE;
}

BOOLEAN
EFIAPI
DummyPbkdf2CreateKey (
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltLen,
  IN   UINTN        Algorithm,
  IN   UINTN        IterationCount,
  IN   CONST UINT8  *Password,
  IN   UINTN        PasswordLen,
  IN   UINTN        DerivedKeyLen,
  OUT  UINT8        *DerivedKey
  )
{
  return FALSE;
}

static H2O_CRYPTO_SERVICES_PPI mH2OCryptoServciesPpi = {
  sizeof (H2O_CRYPTO_SERVICES_PPI),

  Sha256GetContextSize,
  Sha256Init,
  Sha256Duplicate,
  Sha256Update,
  Sha256Final,

  DummyAesCbcDecrypt,
  DummyAesGetContextSize,
  DummyAesInit,
  DummyPbkdf2CreateKey
};


static EFI_PEI_PPI_DESCRIPTOR  mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gH2OCryptoServicesPpiGuid,
  &mH2OCryptoServciesPpi
};

/**
  Initializes H2O_CRYPTO_SERVICES_PROTOCOL_PPI instance.

  @param[in]  FfsHeader   - Header for file of Firmware File System
  @param[in]  PeiServices - The PEI core services table.

  @retval EFI_SUCCESS - Routine finishes initializing H2O_CRYPTO_SERVICES_PROTOCOL_PPI successfully.
  @retval Other       - Error occurs while locating PPI.
                      - Error occurs while re-installing Stall PPI.
                      - Error occurs while allocating memory pool.
                      - Error occurs while publishing notification with PEI Core.
**/
EFI_STATUS
EFIAPI
H2OCryptoServicesEntryPoint (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  return (**PeiServices).InstallPpi (PeiServices, &mPpiList);
}

