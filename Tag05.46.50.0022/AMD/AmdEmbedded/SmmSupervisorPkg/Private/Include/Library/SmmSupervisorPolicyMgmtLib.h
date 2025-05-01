/** @file

Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#pragma once

#include <Uefi.h>
#include <SmmSecurePolicy.h>

//MAX size to hold SMM policy data
#define SMM_POLICY_DATA_SIZE  256 * 1024 //Need to adjust to realistic value

#define MEM_DESC_UNINIT_BASEADDR 0xDEADBEEF

/**
  Initializes internal policy data based on SMM Isolation Policy.

  @param[in] SmmIsolationPolicy   SMM Isolation Policy stored in FW.
  @param[in] Size                 Size of SMM Isolation Policy.
**/
EFI_STATUS
EFIAPI
InitializePolicyData (
  IN VOID   *SmmIsolationPolicy,
  IN UINT32 Size
  );

/**
  Initialize the Desc passed in with Version, Type, and Size.

  @param[in, out] Desc Policy Memory Descriptor.
**/
VOID
EFIAPI
InitPolicyMemDescHdr (
  IN OUT SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *Desc
  );

/**
  Update the policy memory description.
**/
VOID
EFIAPI
UpdateLegacyMemoryDesc (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR **pMemoryPolicy,
  UINT32 *pMemoryPolicyCount,
  UINT32 MemoryAttr,
  UINT64 PageTableBaseAddress,
  UINT64 Size
  );

/**
  Update the policy memory description.
**/
VOID
EFIAPI
UpdateMemoryDesc (
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 **pMemoryPolicy,
  UINT32 *pMemoryPolicyCount,
  UINT32 MemoryAttr,
  UINT64 PageTableBaseAddress,
  UINT64 Size
  );

/**
  Helper function that populates memory policy on demands.

  @param[in] SmmPolicyBuffer   Input buffer points to the entire v1.0 policy.
  @param[in] Cr3               CR3 value to be converted, if input is zero, check the real HW register.
  @param[in] IsShadow          This function will shadow current pagetable if set.

  @param[in] CpuIndex Logical number assigned to CPU.
**/
EFI_STATUS
EFIAPI
PopulateMemoryPolicyEntries (
  IN  SMM_SUPV_SECURE_POLICY_DATA_V1_0    *SmmPolicyBuffer,
  IN  UINT64                              Cr3,
  IN  BOOLEAN                             IsShadow
  );

/**
  Called on Ready To Lock for each CPU thread after Ready to lock. Generate memory policy Descriptors by traverse pagetables and shadow current pagetable.

  @param[in] CpuIndex Logical number assigned to CPU.
**/
VOID
EFIAPI
PageTblReadyToLockCallback (
  IN        UINT64                     CpuIndex
  );

/**
  Called after SMM Read to lock,
  Update memory policy,
  Perform policy sanity check
  @param[in] CpuIndex Logical number assigned to CPU.
**/
VOID
EFIAPI
UpdateMemoryPolicy (
  IN        UINT64                     CpuIndex
  );

/**
  Return pointer to internal SMM Policy Data.

  @retval Pointer to internal SMM Policy Data.
**/
SMM_SUPV_SECURE_POLICY_DATA_V1_0 *
EFIAPI
GetSmmPolicyData (
  VOID
);

/**
  Return pointer to internal SMM Temp Policy Data.

  @retval Pointer to internal SMM Temp Policy Data.
**/
SMM_SUPV_SECURE_POLICY_DATA_V1_0 *
EFIAPI
GetSmmTempPolicyData (
  VOID
);
