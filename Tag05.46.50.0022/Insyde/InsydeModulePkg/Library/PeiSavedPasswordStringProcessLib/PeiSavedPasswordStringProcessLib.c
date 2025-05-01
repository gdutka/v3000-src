/** @file
  Decrypt HDD Password String Process

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <PiPei.h>
#include <Guid/HddPasswordVariable.h>
#include <Ppi/H2OCryptoServices.h>
#include <Ppi/TpmInitialized.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/H2OLib.h>
#include <Library/SavedPasswordStringProcessLib.h>
#include <Library/H2OHddPasswordTableLib.h>
#include <Library/LockBoxLib.h>
#include <Protocol/Tcg2Protocol.h>

#define KEY_SIZE (HDD_PASSWORD_ROOT_KEY_SIZE + HDD_PASSWORD_RANDOM_NUM_SIZE)
EFI_BOOT_MODE                  BootMode;

/**
  Get Root Key and Random number from TPM.

  @param  RandomNum                         Pointer to the RandomNum.
  @param  RootKey                           Pointer to the RootKey.
  @param  NvIndex                           The index to be read.

  @retval EFI_SUCCESS                       The operation completed successfully.
  @retval Others                            An unexpected error occurred.

**/
EFI_STATUS
GetKeyFromTpm (
  IN OUT UINT8                       *RandomNum,
  IN OUT UINT8                       *RootKey,
  IN UINT32                          NvIndex
  )
{
  EFI_STATUS                        Status;
  TPM2B_MAX_BUFFER                  *OutData;
  TPMS_AUTH_COMMAND                 *AuthSession;
  UINT8                             Index;

  if (RandomNum == NULL || RootKey == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_UNSUPPORTED;
  }

  if (CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceNoneGuid) ||
      CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTpm12Guid) ||
      CompareGuid (PcdGetPtr(PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTcmGuid)) {
    return EFI_UNSUPPORTED;
  }

  AuthSession = NULL;
  OutData = AllocateZeroPool (sizeof (TPM2B_MAX_BUFFER));
  if (OutData == NULL){
    return EFI_OUT_OF_RESOURCES;
  } 
  Status = Tpm2NvRead (TPM_RH_PLATFORM, NvIndex, AuthSession, KEY_SIZE, 0, OutData);
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HDD_PASSWORD_ROOT_KEY_SIZE; Index++) {   
      RootKey[Index] = OutData->buffer[Index];
    }
    
    for (Index = HDD_PASSWORD_ROOT_KEY_SIZE; Index < KEY_SIZE; Index++) {   
      RandomNum[Index - HDD_PASSWORD_ROOT_KEY_SIZE] = OutData->buffer[Index];
    }
  } else {
    H2OFreePool ((VOID**)&OutData);
    return Status;
  }
  H2OFreePool ((VOID**)&OutData);
  return EFI_SUCCESS;
}

/**
  SHA256 Hash function.
  
  @param  CryptoPpi                         Pointer to crypto service protocol instance.
  @param  DataPtr                           The data to be calculated
  @param  DataSize                          The size in byte of the data

  @retval UINT8 *                           Pointer to the Hash data.

**/
UINT8 *
Sha256Hash (
  IN  H2O_CRYPTO_SERVICES_PPI               *CryptoPpi,
  IN  UINT8                                 *DataPtr,
  IN  UINTN                                 DataSize
  )
{
  UINTN                                     CtxSize;
  UINT8                                     *HashData;
  VOID                                      *HashCtx;

  HashCtx               = NULL;
  HashData              = NULL;

  if ((DataPtr == NULL) || (DataSize == 0)) {
    return NULL;
  }
  
  HashData = AllocateZeroPool (SHA256_DIGEST_SIZE);
  if (HashData == NULL) {
    return NULL;
  }

  CtxSize = CryptoPpi->Sha256GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  if (HashCtx == NULL) {
    return NULL;
  }

  if (!CryptoPpi->Sha256Init (HashCtx)) {
    return NULL;
  }
  if(!CryptoPpi->Sha256Update (HashCtx, DataPtr, DataSize)) {
    return NULL;
  }
  if(!CryptoPpi->Sha256Final (HashCtx, HashData)) {
    return NULL;
  }

  H2OFreePool ((VOID**)&HashCtx);
  return HashData;
}

/**
  Get Secure Key.

  @param  CryptoPpi                         Pointer to crypto service protocol instance.
  @param  RandomNum                         Pointer to random number buffer.
  @param  RootKey                           Pointer to rootkey buffer.
  @param  SecureKey                         Pointer to the Secure Key.

  @retval EFI_SUCCESS                       The operation completed successfully.
  @retval Others                            An unexpected error occurred.

**/
EFI_STATUS
GetSecureKey (
  IN  H2O_CRYPTO_SERVICES_PPI               *CryptoPpi,
  IN  UINT8                                 *RandomNum,
  IN  UINT8                                 *RootKey,
  OUT UINT8                                 **SecureKey
  )
{
  EFI_STATUS                                Status;
  BOOLEAN                                   CryptoSuccess;
  UINT8                                     *Nonce;
  UINT8                                     *DerivedKey;
  
  Status        = EFI_SUCCESS;
  CryptoSuccess = FALSE;
  Nonce         = NULL;
  DerivedKey    = NULL;

  if ((RandomNum == NULL) || (RootKey == NULL) || (SecureKey == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto GetSecureKeyError;
  }

  //
  // Nonce is 32 bytes (256 bits). SHA256 of random number
  //
  Nonce = Sha256Hash (CryptoPpi, RandomNum, HDD_PASSWORD_RANDOM_NUM_SIZE);

  if (Nonce == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto GetSecureKeyError;
  }

  DerivedKey = AllocateZeroPool (HDD_PASSWORD_SECURITY_KEY_SIZE);

  if (DerivedKey == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto GetSecureKeyError;
  }

  CryptoSuccess = CryptoPpi->Pbkdf2CreateKey (
                                   Nonce,                                            // Salt
                                   HDD_PASSWORD_NONCE_SIZE,                          // SaltLen
                                   SHA256_WITH_RSA_ENCRYPTION,                       // SHA256_WITH_RSA_ENCRYPTION (0x05)
                                   PasswordIterationCount,                           // 0x2710
                                   RootKey,                                          // Password
                                   HDD_PASSWORD_ROOT_KEY_SIZE,                       // PasswordLen
                                   HDD_PASSWORD_SECURITY_KEY_SIZE,                   // DerivedKeyLen
                                   DerivedKey
                                   );

  if (!CryptoSuccess) {
    Status = EFI_SECURITY_VIOLATION;
    goto GetSecureKeyError;
  }

  *SecureKey = DerivedKey;

  ZeroMem (RootKey, HDD_PASSWORD_ROOT_KEY_SIZE);
  ZeroMem (RandomNum, HDD_PASSWORD_RANDOM_NUM_SIZE);
  ZeroMem (Nonce, HDD_PASSWORD_NONCE_SIZE);
 
GetSecureKeyError:
  H2OFreePool ((VOID**)&Nonce);
  
  return Status;
}

/**
  AES CBC function.

  @param  CryptoPpi                         Pointer to crypto service protocol instance.
  @param  PasswordStr                       Pointer to password string.
  @param  TpmNvIndex                        Get decrypt key from TPM Nv index

  @retval EFI_SUCCESS                       Function has completed successfully.
  @retval Others                            An unexpected error occurred.

**/
EFI_STATUS
AesCbcFunc (
  IN     H2O_CRYPTO_SERVICES_PPI            *CryptoPpi,
  IN OUT CHAR16                             *PasswordStr,
  IN     UINT32                             TpmNvIndex
  )
{
  EFI_STATUS                                Status;
  BOOLEAN                                   CryptoSuccess;
  VOID                                      *AesCtx;
  UINT8                                     *SecureKey;
  UINT8                                     *RandomNum;
  UINT8                                     *RootKey;
  UINT8                                     *Iv;
  UINT8                                     *InputData;
  UINT8                                     *DecryptData;
  UINTN                                     PasswordStrSize;

  Status          = EFI_SUCCESS;
  CryptoSuccess   = FALSE;
  AesCtx          = NULL;
  SecureKey       = NULL;
  RandomNum       = NULL;
  RootKey         = NULL;
  Iv              = NULL;
  InputData       = NULL;
  DecryptData     = NULL;
  PasswordStrSize = sizeof (CHAR16) * HDD_PASSWORD_MAX_NUMBER;

  InputData = AllocateZeroPool (HDD_PASSWORD_ENCRYPT_SIZE);

  if (InputData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (InputData, PasswordStr, HDD_PASSWORD_ENCRYPT_SIZE);

  RandomNum = AllocateZeroPool (HDD_PASSWORD_RANDOM_NUM_SIZE);
  if (RandomNum == NULL) {
    goto DecryptError;
  }

  RootKey = AllocateZeroPool (HDD_PASSWORD_ROOT_KEY_SIZE);
  if (RootKey == NULL) {
    goto DecryptError;
  }

  Status = GetKeyFromTpm (RandomNum, RootKey, TpmNvIndex);
  if (EFI_ERROR (Status)) {
    goto DecryptError;
  }
  //
  // IV is 32 bytes (256 bits). SHA256 of random number
  //
  Iv = Sha256Hash (CryptoPpi, RandomNum, HDD_PASSWORD_RANDOM_NUM_SIZE);

  if (Iv == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto DecryptError;
  }

  //
  // A 256 bits AES key is needed for HDP encryption
  //
  Status = GetSecureKey (CryptoPpi, RandomNum, RootKey, &SecureKey);

  if (EFI_ERROR (Status)) {
    goto DecryptError;
  }

  DecryptData = AllocateZeroPool (HDD_PASSWORD_ENCRYPT_SIZE);

  if (DecryptData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto DecryptError;
  }

  AesCtx = AllocatePool (CryptoPpi->AesGetContextSize ());

  if (AesCtx == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto DecryptError;
  }

  CryptoSuccess = CryptoPpi->AesInit (AesCtx, SecureKey, (HDD_PASSWORD_SECURITY_KEY_SIZE * BITS_PER_BYTE));

  if (!CryptoSuccess) {
    Status = EFI_SECURITY_VIOLATION;
    goto DecryptError;
  }

  CryptoSuccess = CryptoPpi->AesCbcDecrypt (
                                AesCtx,
                                InputData,
                                HDD_PASSWORD_ENCRYPT_SIZE,
                                Iv,
                                DecryptData
                                );
  if (!CryptoSuccess) {
    Status = EFI_SECURITY_VIOLATION;
    goto DecryptError;
  }

  CopyMem (PasswordStr, DecryptData, HDD_PASSWORD_ENCRYPT_SIZE);
  

  ZeroMem (RootKey, HDD_PASSWORD_ROOT_KEY_SIZE);
  ZeroMem (RandomNum, HDD_PASSWORD_RANDOM_NUM_SIZE);
  ZeroMem (InputData, HDD_PASSWORD_ENCRYPT_SIZE);
  ZeroMem (Iv, HDD_PASSWORD_IV_SIZE);
  ZeroMem (SecureKey, HDD_PASSWORD_SECURITY_KEY_SIZE);
  
DecryptError:
  H2OFreePool ((VOID**)&InputData);
  H2OFreePool ((VOID**)&RootKey);
  H2OFreePool ((VOID**)&RandomNum);
  H2OFreePool ((VOID**)&Iv);
  H2OFreePool ((VOID**)&SecureKey);
  H2OFreePool ((VOID**)&DecryptData);
  H2OFreePool ((VOID**)&AesCtx);

  return Status;
}

/**
  Constructor for PeiSavedPasswordStringProcess library.
  This is used to get boot mode to check whether in S3 resume or not.

  @retval EFI_SUCEESS
  @return Others          Some error occurs.

**/
EFI_STATUS
EFIAPI
PeiSavedPasswordStringProcessLibConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                     Status;
  
  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Saved Password String Process.

  @param  ProcessType                       To do encrypt or decrypt.
  @param  HddPasswordTable                  Pointer to HddPasswordTable.
  @param  HddPasswordTableSize              Size of HddPasswordTable.

  @retval EFI_SUCCESS                       Function has completed successfully.
  @retval Others                            An unexpected error occurred.

**/
EFI_STATUS
SavedPasswordStringProcess (
  IN     UINT8                                 ProcessType,
  IN OUT HDD_PASSWORD_TABLE                    *HddPasswordTable,
  IN     UINTN                                 HddPasswordTableSize
  )
{
  EFI_STATUS                                Status;
  H2O_CRYPTO_SERVICES_PPI                   *CryptoPpi;
  UINTN                                     NumOfTable;
  HDD_PASSWORD_TABLE                        *HddPasswordTablePtr;
  UINTN                                     HddPasswordTableIndex;
  TCG_OPAL_EXT_DATA                         *TcgExtDataPtr;
  UINT8                                     DummyData;
  UINTN                                     RestoreTableSize;
  
  Status                = EFI_SUCCESS;
  CryptoPpi             = NULL;
  NumOfTable            = 0;
  HddPasswordTablePtr   = NULL;
  HddPasswordTableIndex = 0;
  TcgExtDataPtr         = NULL;
  RestoreTableSize      = 0;

  if (ProcessType == EncryptProcess) {
    return EFI_UNSUPPORTED;
  }

  if (ProcessType != DecryptProcess) {
    return EFI_INVALID_PARAMETER;
  }

  if (BootMode == BOOT_ON_S3_RESUME) {

    Status = RestoreLockBox (&gSaveHddPasswordGuid, &DummyData, &RestoreTableSize);
    if (Status != EFI_BUFFER_TOO_SMALL) {
      return EFI_INVALID_PARAMETER;
    } else {
      if (RestoreTableSize != HddPasswordTableSize) {
        return EFI_SECURITY_VIOLATION;
      }
      HddPasswordTablePtr = AllocatePool (RestoreTableSize);
      if (HddPasswordTablePtr == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      Status = RestoreLockBox (&gSaveHddPasswordGuid, (VOID *)HddPasswordTablePtr, &RestoreTableSize);
      if (EFI_ERROR (Status)) {
        H2OFreePool ((VOID**)&HddPasswordTable);
        return Status;
      }
      *HddPasswordTable = *HddPasswordTablePtr;
    }
    return EFI_SUCCESS;
  }
  
  Status = PeiServicesLocatePpi (
             &gH2OCryptoServicesPpiGuid,
             0,
             NULL,
             (VOID **)&CryptoPpi
             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  NumOfTable = NumOfHddPasswordTable (HddPasswordTable, HddPasswordTableSize);
  HddPasswordTablePtr = (HDD_PASSWORD_TABLE *) HddPasswordTable;

  for (HddPasswordTableIndex = 0; HddPasswordTableIndex < NumOfTable;
        HddPasswordTableIndex++, HddPasswordTablePtr = GetNextTableEntry (HddPasswordTablePtr)) {

    if (HddPasswordTablePtr->PasswordStr[0] == 0) {
      continue;
    }

    Status = AesCbcFunc (CryptoPpi, HddPasswordTablePtr->PasswordStr, HddPasswordTablePtr->TpmNvIndex);

    //
    // TCG Opal Ext Data
    //
    TcgExtDataPtr = (TCG_OPAL_EXT_DATA *) (HddPasswordTablePtr + 1);

    if (TcgExtDataPtr->Signature == TCG_OPAL_EXT_DATA_SIGNATURE) {

      if (TcgExtDataPtr->SIDPasswordStr[0] != 0) {
        AesCbcFunc (CryptoPpi, TcgExtDataPtr->SIDPasswordStr, HddPasswordTablePtr->TpmNvIndex);
      }

      if (TcgExtDataPtr->Admin1PasswordStr[0] != 0) {
        AesCbcFunc (CryptoPpi, TcgExtDataPtr->Admin1PasswordStr, HddPasswordTablePtr->TpmNvIndex);
      }

      if (TcgExtDataPtr->User1PasswordStr[0] != 0) {
        AesCbcFunc (CryptoPpi, TcgExtDataPtr->User1PasswordStr, HddPasswordTablePtr->TpmNvIndex);
      }

      if (TcgExtDataPtr->User2PasswordStr[0] != 0) {
        AesCbcFunc (CryptoPpi, TcgExtDataPtr->User2PasswordStr, HddPasswordTablePtr->TpmNvIndex);
      }
    }
  }

  return Status;
}
