/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include "AmdTscTimerLibInternal.h"

UINT64 mTscFrequency;
UINT16 mCpuFamily = FAMILY_19;

/**
 * @brief Get CpuFamily Value from CPUID_Fn80000001_EAX
 * @details CpuFamily = ExtFamily + BaseFamily
 *
 * @return CpuFamily Value
 */
UINT16
GetCpuidCpuFamilyValue (
  VOID
  )
{
  UINT32 EAX_Reg;
  UINT16 CpuFamily;

  EAX_Reg   = 0;
  CpuFamily = 0;

  AsmCpuid (
    AMD_CPUID_FMF,
    &EAX_Reg,
    NULL,
    NULL,
    NULL
    );

  CpuFamily = (UINT16)(((EAX_Reg & CPUID_BASE_FAMILY_MASK) >> CPUID_BASE_FAMILY_OFFSET) +
                       ((EAX_Reg & CPUID_EXT_FAMILY_MASK)  >> CPUID_EXT_FAMILY_OFFSET ));

  return CpuFamily;
}

/** The constructor function determines the actual TSC frequency.

  First, Get TSC frequency from system configuration table with TSC frequency GUID,
  if the table is not found, install it.
  This function will always return EFI_SUCCESS.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
AmdDxeTscTimerLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  UINT64      *TscFrequency;

  Status = SystemTable->BootServices->AllocatePool (EfiBootServicesData, sizeof (UINT64), (VOID **) &TscFrequency);
  ASSERT_EFI_ERROR (Status);

  mCpuFamily = GetCpuidCpuFamilyValue ();

  *TscFrequency = InternalCalculateTscFrequency ();

  mTscFrequency = *TscFrequency;

  return EFI_SUCCESS;
}

/**  Get TSC frequency.

  @return The number of TSC counts per second.

**/
UINT64
InternalGetTscFrequency (
  VOID
  )
{
  return mTscFrequency;
}

