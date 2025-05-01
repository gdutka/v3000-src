/** @file

Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#pragma once

#include <Uefi.h>
#include <SmmSecurePolicy.h>

/**
  Calculate the maximum support address.

  @return the maximum support address.
**/
UINT8
CalculateMaximumSupportAddress (
  VOID
  );

/**
  This function sets the attributes for the memory region specified by BaseAddress and
  Length from their current attributes to the attributes specified by Attributes.

  @param[in]   BaseAddress      The physical address that is the start address of a memory region.
  @param[in]   Length           The size in bytes of the memory region.
  @param[in]   Attributes       The bit mask of attributes to set for the memory region.
  @param[out]  IsSplitted       TRUE means page table splitted. FALSE means page table not splitted.

  @retval EFI_SUCCESS           The attributes were set for the memory region.
  @retval EFI_ACCESS_DENIED     The attributes for the memory resource range specified by
                                BaseAddress and Length cannot be modified.
  @retval EFI_INVALID_PARAMETER Length is zero.
                                Attributes specified an illegal combination of attributes that
                                cannot be set together.
  @retval EFI_OUT_OF_RESOURCES  There are not enough system resources to modify the attributes of
                                the memory resource range.
  @retval EFI_UNSUPPORTED       The processor does not support one or more bytes of the memory
                                resource range specified by BaseAddress and Length.
                                The bit mask of attributes is not support for the memory resource
                                range specified by BaseAddress and Length.

**/
EFI_STATUS
EFIAPI
SmmSetMemoryAttributesEx (
  IN  EFI_PHYSICAL_ADDRESS                       BaseAddress,
  IN  UINT64                                     Length,
  IN  UINT64                                     Attributes,
  OUT BOOLEAN                                    *IsSplitted  OPTIONAL,
  IN  BOOLEAN                                    UseShadowCr3
  );

/**
  This function clears the attributes for the memory region specified by BaseAddress and
  Length from their current attributes to the attributes specified by Attributes.

  @param[in]   BaseAddress      The physical address that is the start address of a memory region.
  @param[in]   Length           The size in bytes of the memory region.
  @param[in]   Attributes       The bit mask of attributes to clear for the memory region.
  @param[out]  IsSplitted       TRUE means page table splitted. FALSE means page table not splitted.

  @retval EFI_SUCCESS           The attributes were cleared for the memory region.
  @retval EFI_ACCESS_DENIED     The attributes for the memory resource range specified by
                                BaseAddress and Length cannot be modified.
  @retval EFI_INVALID_PARAMETER Length is zero.
                                Attributes specified an illegal combination of attributes that
                                cannot be cleared together.
  @retval EFI_OUT_OF_RESOURCES  There are not enough system resources to modify the attributes of
                                the memory resource range.
  @retval EFI_UNSUPPORTED       The processor does not support one or more bytes of the memory
                                resource range specified by BaseAddress and Length.
                                The bit mask of attributes is not supported for the memory resource
                                range specified by BaseAddress and Length.

**/
EFI_STATUS
EFIAPI
SmmClearMemoryAttributesEx (
  IN  EFI_PHYSICAL_ADDRESS                       BaseAddress,
  IN  UINT64                                     Length,
  IN  UINT64                                     Attributes,
  OUT BOOLEAN                                    *IsSplitted  OPTIONAL,
  IN  BOOLEAN                                    UseShadowCr3
  );

/**
  This function sets the attributes for the memory region specified by BaseAddress and
  Length from their current attributes to the attributes specified by Attributes.

  @param[in]  BaseAddress      The physical address that is the start address of a memory region.
  @param[in]  Length           The size in bytes of the memory region.
  @param[in]  Attributes       The bit mask of attributes to set for the memory region.

  @retval EFI_SUCCESS           The attributes were set for the memory region.
  @retval EFI_ACCESS_DENIED     The attributes for the memory resource range specified by
                                BaseAddress and Length cannot be modified.
  @retval EFI_INVALID_PARAMETER Length is zero.
                                Attributes specified an illegal combination of attributes that
                                cannot be set together.
  @retval EFI_OUT_OF_RESOURCES  There are not enough system resources to modify the attributes of
                                the memory resource range.
  @retval EFI_UNSUPPORTED       The processor does not support one or more bytes of the memory
                                resource range specified by BaseAddress and Length.
                                The bit mask of attributes is not supported for the memory resource
                                range specified by BaseAddress and Length.

**/
EFI_STATUS
EFIAPI
SmmSetMemoryAttributes (
  IN  EFI_PHYSICAL_ADDRESS                       BaseAddress,
  IN  UINT64                                     Length,
  IN  UINT64                                     Attributes,
  IN  BOOLEAN                                    UseShadowCr3
  );

/**
  This function clears the attributes for the memory region specified by BaseAddress and
  Length from their current attributes to the attributes specified by Attributes.

  @param[in]  BaseAddress      The physical address that is the start address of a memory region.
  @param[in]  Length           The size in bytes of the memory region.
  @param[in]  Attributes       The bit mask of attributes to clear for the memory region.

  @retval EFI_SUCCESS           The attributes were cleared for the memory region.
  @retval EFI_ACCESS_DENIED     The attributes for the memory resource range specified by
                                BaseAddress and Length cannot be modified.
  @retval EFI_INVALID_PARAMETER Length is zero.
                                Attributes specified an illegal combination of attributes that
                                cannot be cleared together.
  @retval EFI_OUT_OF_RESOURCES  There are not enough system resources to modify the attributes of
                                the memory resource range.
  @retval EFI_UNSUPPORTED       The processor does not support one or more bytes of the memory
                                resource range specified by BaseAddress and Length.
                                The bit mask of attributes is not supported for the memory resource
                                range specified by BaseAddress and Length.

**/
EFI_STATUS
EFIAPI
SmmClearMemoryAttributes (
  IN  EFI_PHYSICAL_ADDRESS                       BaseAddress,
  IN  UINT64                                     Length,
  IN  UINT64                                     Attributes,
  IN  BOOLEAN                                    UseShadowCr3
  );

/*
  Reports if CR3 shadow is enabled.

  @retval TRUE - CR3 shadow is enabled.
*/
BOOLEAN
IsCr3Shadowed (
  VOID
  );

/**
  Return page table base.

  @param[in]  UseShadowCr3   Identify if use Shadow Cr3 or actual CR3.

  @return page table base.
**/
UINTN
EFIAPI
GetPageTableBase (
  IN  BOOLEAN                                    UseShadowCr3
  );

/*
  Generate Memory policy DESCRIPTORs by traverse pagetables
  Shadow current pagetable if IsShadow set
  if input Cr3 is zero, check the real HW register

  @retval EFI_SUCCESS           Execute operation successfully
  @retval EFI_OUT_OF_RESOURCES  MemoryPolicySize is too small to hold all memory policy
  @retval EFI_UNSUPPORTED       Current register setting doesn't support page mode, if input CR3 is no-zero
*/
EFI_STATUS
GenMemPolicyAndShadowPageTable (
  IN       UINT64 Cr3,
  IN OUT   VOID *MemoryPolicyPtr,
  IN       UINTN MemoryPolicySize,
  IN OUT   UINT32 *MemoryPolicyCount,
  IN       BOOLEAN IsShadow,
  IN       BOOLEAN IsLegacy
  );
