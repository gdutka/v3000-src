/** @file
  Instance of AMD SMM SaveState supervision layer. This library provides
  supervision over SMM SaveState.

  Copyright (c) Microsoft Corporation.
  Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __IHV_SMM_SAVE_STATE_SUPERVISION_LIB__
#define __IHV_SMM_SAVE_STATE_SUPERVISION_LIB__

#define SMM_SAVE_STATE_CPU_DATA_VERSION_1 1
typedef struct {
  UINT64     Version;
  UINT64     NumberOfCpus;
  UINT64     *SmBase;           // Pointer to SmBase array with number specified by NumberOfCpus
} GATELIB_CPU_SMM_DATA;
/**
  @brief Given Smm save state address and access width, determine if it is
  allowed to access by parsing the policy

  @param SmmSecurityPolicy  - The address of applied SMM secure policy.
  @param CpuIndex           - Cpu
  @param SmmSaveStateAddr   - The physical address of SMM save state field
  @param Width         - Access width, need less than 8
  @param CpuSmmData         - Point to GATELIB_CPU_SMM_DATA

  @retval EFI_ACCESS_DENIED     The requested operation is not whitelisted by
                                the policy.
          EFI_INVALID_PARAMETER The SaveStateMapField needs to be within the
                                range of [0, SECURE_POLICY_INSTRUCTION_COUNT).
          EFI_SUCCESS           The requested operation is allowed by the
                                policy.
**/
EFI_STATUS
EFIAPI
IsIhvSmmSaveStateReadAllowed (
  IN SMM_SUPV_SECURE_POLICY_DATA_V1_0*  SmmSecurityPolicy,
  IN UINTN                              CpuIndex,
  IN UINTN                              SmmSaveStateAddr,
  IN UINTN                              Width,
  IN GATELIB_CPU_SMM_DATA               *CpuSmmData
  );

/**
  Read data from SmmSaveStateAddr with given Width

  @param Ret                - Point to return value
  @param CpuIndex           - Cpu
  @param SmmSaveStateAddr   - The physical address of SMM save state field
  @param Width              - Access width, need less than 8
**/
VOID
EFIAPI
IhvSmmSaveStateRead (
  IN OUT           VOID *Ret,
  IN UINTN         CpuIndex,
  IN UINTN         SmmSaveStateAddr,
  IN UINTN         Width
  );


#endif // !defined (__IHV_SMM_SAVE_STATE_SUPERVISION_LIB__)
