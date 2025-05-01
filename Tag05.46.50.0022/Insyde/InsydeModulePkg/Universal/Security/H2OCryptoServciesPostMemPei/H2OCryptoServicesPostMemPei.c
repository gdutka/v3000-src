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

static H2O_CRYPTO_SERVICES_PPI mH2OCryptoServciesPpi = {
  sizeof (H2O_CRYPTO_SERVICES_PPI),

  Sha256GetContextSize,
  Sha256Init,
  Sha256Duplicate,
  Sha256Update,
  Sha256Final,

  AesCbcDecrypt,
  AesGetContextSize,
  AesInit,
  Pbkdf2CreateKey
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
  EFI_STATUS                        Status;
  H2O_CRYPTO_SERVICES_PPI           *CryptoServicesPpi;
  EFI_PEI_PPI_DESCRIPTOR            *CryptoPeiPpiDescriptor;
  
  //
  // Register so it will be automatically shadowed to memory
  //
  Status = PeiServicesRegisterForShadow (FileHandle);

  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gH2OCryptoServicesPpiGuid,
                             0,
                             &CryptoPeiPpiDescriptor,
                             (VOID **)&CryptoServicesPpi
                             );

  //
  // Reinstall the Interface using the memory-based descriptor
  //
  if (!EFI_ERROR (Status)) {
    //
    // Only reinstall the "Stall" PPI
    //
    Status = (**PeiServices).ReInstallPpi (
                               PeiServices,
                               CryptoPeiPpiDescriptor,
                               &mPpiList
                               );
  }

  return Status;

}

