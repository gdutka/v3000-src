/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* This file includes code originally published under the following license. */

/** @file
  A Dxe Timer Library implementation which uses the Time Stamp Counter in the processor.

  For Pentium 4 processors, Intel Xeon processors (family [0FH], models [03H and higher]);
    for Intel Core Solo and Intel Core Duo processors (family [06H], model [0EH]);
    for the Intel Xeon processor 5100 series and Intel Core 2 Duo processors (family [06H], model [0FH]);
    for Intel Core 2 and Intel Xeon processors (family [06H], display_model [17H]);
    for Intel Atom processors (family [06H], display_model [1CH]):
  the time-stamp counter increments at a constant rate.
  That rate may be set by the maximum core-clock to bus-clock ratio of the processor or may be set by
  the maximum resolved frequency at which the processor is booted. The maximum resolved frequency may
  differ from the maximum qualified frequency of the processor.

  The specific processor configuration determines the behavior. Constant TSC behavior ensures that the
  duration of each clock tick is uniform and supports the use of the TSC as a wall clock timer even if
  the processor core changes frequency. This is the architectural behavior moving forward.

  A Processor's support for invariant TSC is indicated by CPUID.0x80000007.EDX[8].

  Copyright (c) 2009 - 2011, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include "TscTimerLibInternal.h"

#define FILECODE LIBRARY_CCXTSCTIMERLIB_DXETSCTIMERLIB_FILECODE

extern EFI_GUID gAmdTscFrequencyGuid;

AMD_TSC_FREQUENCY_INFORMATION mFrequencyInfo;

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
DxeTscTimerLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                        Status;
  AMD_TSC_FREQUENCY_INFORMATION     *FrequencyInfo;

  FrequencyInfo = NULL;
  //
  // Get TSC frequency from system configuration table with TSC frequency GUID.
  //
  Status = EfiGetSystemConfigurationTable (&gAmdTscFrequencyGuid, (VOID **) &FrequencyInfo);
  if ((Status == EFI_SUCCESS) && (FrequencyInfo != NULL)) {
    if (AsmReadMsr64 (MSR_PSTATE_0 + FrequencyInfo->CurrentPState) != FrequencyInfo->CurrentPStateReg) {
      InternalGetTscFrequencyInformation(FrequencyInfo);
    }
    mFrequencyInfo.TscFrequency = FrequencyInfo->TscFrequency;
    mFrequencyInfo.CurrentPStateReg = FrequencyInfo->CurrentPStateReg;
    mFrequencyInfo.CurrentPState = FrequencyInfo->CurrentPState;

    return EFI_SUCCESS;
  }

  //
  // TSC frequency GUID system configuration table is not found, install it.
  //
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (AMD_TSC_FREQUENCY_INFORMATION), (VOID **) &FrequencyInfo);
  ASSERT_EFI_ERROR (Status);
  if (FrequencyInfo == NULL) {
    return EFI_SUCCESS;
  }

  InternalGetTscFrequencyInformation(FrequencyInfo);
  mFrequencyInfo.TscFrequency = FrequencyInfo->TscFrequency;
  mFrequencyInfo.CurrentPStateReg = FrequencyInfo->CurrentPStateReg;
  mFrequencyInfo.CurrentPState = FrequencyInfo->CurrentPState;

  gBS->InstallConfigurationTable (&gAmdTscFrequencyGuid, FrequencyInfo);

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
  EFI_STATUS                        Status;
  AMD_TSC_FREQUENCY_INFORMATION     *FrequencyInfo;

  if (AsmReadMsr64 (MSR_PSTATE_0 + mFrequencyInfo.CurrentPState) != mFrequencyInfo.CurrentPStateReg) {
    Status = EfiGetSystemConfigurationTable (&gAmdTscFrequencyGuid, (VOID **) &FrequencyInfo);
    ASSERT_EFI_ERROR(Status);

    InternalGetTscFrequencyInformation(FrequencyInfo);
    mFrequencyInfo.TscFrequency = FrequencyInfo->TscFrequency;
    mFrequencyInfo.CurrentPStateReg = FrequencyInfo->CurrentPStateReg;
    mFrequencyInfo.CurrentPState = FrequencyInfo->CurrentPState;
  }

  return mFrequencyInfo.TscFrequency;
}

