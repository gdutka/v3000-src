/** @file
  IHV DRTM abstraction layer to perform silicon specific operations.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IHV_DRTM_LIB_H_
#define _IHV_DRTM_LIB_H_

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
  );

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
  );

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
  );

/**
  IHV specific function to be invoked after page table lockdown from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmPostPageTableLock (
  UINTN       CpuIndex
  );

/**
  IHV specific function to be invoked at ready to lock event from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmReadyToLockAtSmiEntry (
  UINTN       CpuIndex
  );

/**
  IHV specific function to be invoked at ready to lock event from
  supervisor SMI entry routine.

  @param  CpuIndex        The CPU index which this routine running on.

**/
VOID
EFIAPI
IhvDrtmReadyToLockAtSmiExit (
  UINTN       CpuIndex
  );

/**
  The initialization routine for IHV DRTM at the entry point of SMM supervisor.

  @retval EFI_SUCCESS     The initialization proceeds through properly.
  @retval Error           Some error occurs during initialization.

**/
EFI_STATUS
EFIAPI
IhvDrtmLibInit (
  VOID
  );

#endif
