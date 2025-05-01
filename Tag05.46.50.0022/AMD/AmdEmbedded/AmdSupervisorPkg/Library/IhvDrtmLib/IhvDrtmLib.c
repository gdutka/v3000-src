/*
*******************************************************************************

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
Copyright (c) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/PspMboxSmmBufferAddressProtocol.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/AmdPspMboxLibV2.h>

#define CPUID_FEATURE_EXT2            0x80000021
#define CPUID_SMM_PG_CFG_LOCK_BIT     BIT3

#define MSR_HWCR                      0xC0010015
#define MSR_HWCR_SMM_PG_CFG_LOCK_BIT  BIT33

extern EFI_BOOT_MODE            mBootMode;
DRTM_INFO                       mDrtmInfo;
DRTM_INFO                       mDrtmInfo1;
BOOLEAN                         DrtmInfoStored = FALSE;
UINT8                           *mPspMboxSmmBuffer = NULL;
BOOLEAN                         *mPspMboxSmmFlagAddr = NULL;

/**
  This routine is to be used for Smm supervisor to store critical DRTM
  information into silicon specific secure region prior to DRTM event.
  This function should be called only once per boot.

  @retval EFI_SUCCESS     The DRTM secure store operation succeeds.
  @retval Error           Some error occurs during DRTM store process.

**/
EFI_STATUS
EFIAPI
PreDrtmInfoSecureStore (
  IN  UINT64      SupvImageLoadAddr,
  IN  UINT64      SupvImageSize,
  IN  UINT64      SupvDirectoryAddr,
  IN  UINT64      SupvDirectorySize,
  IN  UINT64      SupvPolicyAddr,
  IN  UINT64      SupvPolicySize,
  IN  UINT64      SupvSmiEntryFunc,
  IN  UINT64      SupvSmiExitFunc
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  mDrtmInfo1.SmmSupvAddrLo = (UINT32) ((UINTN) SupvImageLoadAddr & 0xFFFFFFFF);
  mDrtmInfo1.SmmSupvAddrHi = (UINT32) ((UINTN) SupvImageLoadAddr >> 32);
  mDrtmInfo1.SmmSupvSize   = (UINT32) SupvImageSize;

  mDrtmInfo1.SmmSupvDirAddrLo = (UINT32) ((UINTN) SupvDirectoryAddr & 0xFFFFFFFF);
  mDrtmInfo1.SmmSupvDirAddrHi = (UINT32) ((UINTN) SupvDirectoryAddr >> 32);
  mDrtmInfo1.SmmSupvDirSize   = (UINT32) (UINTN) SupvDirectorySize;

  mDrtmInfo1.SmmPolicyDataAddrLo = (UINT32) ((UINTN) SupvPolicyAddr & 0xFFFFFFFF);
  mDrtmInfo1.SmmPolicyDataAddrHi = (UINT32) ((UINTN) SupvPolicyAddr >> 32);
  mDrtmInfo1.SmmPolicyDataSize = (UINT32) SupvPolicySize;

  // Save DrtmInfo to mDrtmInfo, mDrtmInfo will be reinited after DRTM boot done
  CopyMem (&mDrtmInfo, &mDrtmInfo1, sizeof (DRTM_INFO));
  mDrtmInfo1.SmmSupvSmmEntryAddrLo = (UINT32) ((UINTN) SupvSmiEntryFunc & 0xFFFFFFFF);
  mDrtmInfo1.SmmSupvSmmEntryAddrHi = (UINT32) ((UINTN) SupvSmiEntryFunc >> 32);

  mDrtmInfo1.SmmSupvSmmExitAddrLo = (UINT32) ((UINTN) SupvSmiExitFunc & 0xFFFFFFFF);
  mDrtmInfo1.SmmSupvSmmExitAddrHi = (UINT32) ((UINTN) SupvSmiExitFunc >> 32);

  // In Capsule update path, do NOT report DRTM info to PSP as PSP only accepts this command once.
  // When PSP receives this command again, it will treat the command as unexpected, an error will be returned.
  // System will be asserted in the debug version of SmmSupervisor driver entry.
  if ((mBootMode != BOOT_ON_FLASH_UPDATE) && (mBootMode != BOOT_IN_RECOVERY_MODE)) {
    Status = PspMboxBiosDrtmInfo (&mDrtmInfo1);
  }
  if (!EFI_ERROR (Status)) {
    DrtmInfoStored = TRUE;
  }

  return Status;
}

/**
  This routine is to be used for Smm supervisor to validate critical
  DRTM information has not changed before and after DRTM information.

  @retval EFI_SUCCESS               The DRTM secure store operation succeeds.
  @retval EFI_SECURITY_VIOLATION    DRTM information has changed after DRTM.

**/
EFI_STATUS
EFIAPI
PostDrtmSecureValidation (
  VOID
)
{
  EFI_STATUS Status;

  //Send BIOS_CMD_POST_DRTM_INFO_QUERY to query the DRTMINFO PSP send during DRTM boot
  Status = PspMboxBiosCmdPostDrtmInfoQuery (
            &mDrtmInfo,
            mPspMboxSmmBuffer,
            mPspMboxSmmFlagAddr
            );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //Compare with the one we send during post
  if (CompareMem (&mDrtmInfo, &mDrtmInfo1, sizeof (DRTM_INFO)) != 0) {
    DEBUG ((DEBUG_INFO, "DRTM info not identical\n"));
    Status = EFI_SECURITY_VIOLATION;
    goto Exit;
  }

Exit:
  return Status;
}

/**
  This routine is to be used by Smm supervisor for querying securely stored
  DRTM information after DRTM event.

  @retval EFI_SUCCESS               The DRTM secure store operation succeeds.
  @retval EFI_SECURITY_VIOLATION    DRTM information has changed after DRTM.

**/
EFI_STATUS
EFIAPI
PostDrtmGetSecureStoredInfo (
  OUT UINT64      *SupvImageLoadAddr,   OPTIONAL
  OUT UINT64      *SupvImageSize,       OPTIONAL
  OUT UINT64      *SupvDirectoryAddr,   OPTIONAL
  OUT UINT64      *SupvDirectorySize,   OPTIONAL
  OUT UINT64      *SupvPolicyAddr,      OPTIONAL
  OUT UINT64      *SupvPolicySize,      OPTIONAL
  OUT UINT64      *SupvSmiEntryFunc,    OPTIONAL
  OUT UINT64      *SupvSmiExitFunc      OPTIONAL
)
{
  DRTM_INFO     *DrtmInfo;

  if (!DrtmInfoStored) {
    return EFI_SECURITY_VIOLATION;
  }

  DrtmInfo = &mDrtmInfo;  //mDrtmInfo get in PspMboxBiosCmdPostDrtmInfoQuery
  if (SupvImageLoadAddr) {
    *SupvImageLoadAddr = (((UINT64) DrtmInfo->SmmSupvAddrHi << 32) + DrtmInfo->SmmSupvAddrLo);
  }
  if (SupvImageSize) {
    *SupvImageSize = DrtmInfo->SmmSupvSize;
  }
  if (SupvDirectoryAddr) {
    *SupvDirectoryAddr = (((UINT64) DrtmInfo->SmmSupvDirAddrHi << 32) + DrtmInfo->SmmSupvDirAddrLo);
  }
  if (SupvDirectorySize) {
    *SupvDirectorySize = DrtmInfo->SmmSupvDirSize;
  }
  if (SupvPolicyAddr) {
    *SupvPolicyAddr = (((UINT64) DrtmInfo->SmmPolicyDataAddrHi << 32) + DrtmInfo->SmmPolicyDataAddrLo);
  }
  if (SupvPolicySize) {
    *SupvPolicySize = DrtmInfo->SmmPolicyDataSize;
  }
  if (SupvSmiEntryFunc) {
    *SupvSmiEntryFunc = (((UINT64) DrtmInfo->SmmSupvSmmEntryAddrHi << 32) + DrtmInfo->SmmSupvSmmEntryAddrLo);
  }
  if (SupvSmiExitFunc) {
    *SupvSmiExitFunc = (((UINT64) DrtmInfo->SmmSupvSmmExitAddrHi << 32) + DrtmInfo->SmmSupvSmmExitAddrLo);
  }

  return EFI_SUCCESS;
}

/**
  Init global variable point to smm address to send C2P mailbox under SMM

  @param  Protocol   Points to the protocol's unique identifier.
  @param  Interface  Points to the interface instance.
  @param  Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS SmmReadyToLock protocol callback runs successfully.

**/
EFI_STATUS
EFIAPI
SmmSupvPspMboxSmmBufferAddressCallback (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS Status;

  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;

  //Save PspMboxSmmBuffer to globol variable
  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    DEBUG ((DEBUG_INFO, "\t Smmsupv mPspMboxSmmBuffer 0x%x\n", mPspMboxSmmBuffer));
    DEBUG ((DEBUG_INFO, "\t mPspMboxSmmFlagAddr 0x%x\n", mPspMboxSmmFlagAddr));
  }
  return EFI_SUCCESS;
}

/**
 * @brief Set Core::X86::Msr::HWCR[SmmPgCfgLock] if processor supported
 *
 * @return VOID
 */
VOID
SmmPageLock (
  VOID
  )
{
  UINT32                    Eax;
  // Core::X86::Cpuid::FeatureExt2Eax[SmmPgCfgLock] Specifies SMM page configuration locking is supported
  Eax = 0;
  AsmCpuid (CPUID_FEATURE_EXT2, &Eax, NULL, NULL, NULL);
  if (Eax & CPUID_SMM_PG_CFG_LOCK_BIT) {
    AsmMsrOr64 (MSR_HWCR, MSR_HWCR_SMM_PG_CFG_LOCK_BIT);
  }
}

/**
  IHV specific function to be invoked after page table lockdown from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmPostPageTableLock (
  UINTN       CpuIndex
  )
{
  //Lock Smm Page table through Core::X86::Msr::HWCR[SmmPgCfgLock], Start support from AMD Family 19h Models 40h-4Fh
  //Perform the lock after PostDrtmEntry, and SMM_SUPV_PRE_LOCK_MASK has been set, which no further page table registers will be accessed
  // If Core::X86::Msr::HWCR[SmmPgCfgLock], WRMSR of Core::X86::Msr::EFER results in an error.
  // If Core::X86::Msr::HWCR[SmmPgCfgLock], MOV CR0, CR3 and CR4 instructions result in an error.
  SmmPageLock ();
}

/**
  IHV specific function to be invoked at ready to lock event from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmReadyToLockAtSmiEntry (
  UINTN       CpuIndex
  )
{
}

/**
  IHV specific function to be invoked at ready to lock event from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmReadyToLockAtSmiExit (
  UINTN       CpuIndex
  )
{
}

/**
  The initialization routine for IHV DRTM at the entry point of SMM supervisor.

  @retval EFI_SUCCESS     The initialization proceeds through properly.
  @retval Error           Some error occurs during initialization.

**/
EFI_STATUS
EFIAPI
IhvDrtmLibInit (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *Registration;

  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;

  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    DEBUG ((DEBUG_INFO, "\t Smmsupv mPspMboxSmmBuffer 0x%x\n", mPspMboxSmmBuffer));
    DEBUG ((DEBUG_INFO, "\t mPspMboxSmmFlagAddr 0x%x\n", mPspMboxSmmFlagAddr));
  } else {
    // Register gPspMboxSmmBufferAddressProtocolGuid callback in order to
    // query the smm address to send C2P mailbox under SMM
    Registration  = NULL;
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gPspMboxSmmBufferAddressProtocolGuid,
                      SmmSupvPspMboxSmmBufferAddressCallback,
                      &Registration
                      );
  }

  return Status;
}
