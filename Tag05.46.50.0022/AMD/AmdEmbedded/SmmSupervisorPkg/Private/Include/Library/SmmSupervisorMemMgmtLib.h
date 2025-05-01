
/** @file
 * SmmSupv Post Code definition

Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#pragma once

#include <Uefi.h>

typedef struct {
  UINT64 BaseAddress;      //base address
  UINT64 Length;           //size in bytes
} SMM_SUPV_MEM_BLOCK;

/**
  Allocates one or more pages of type EfiBootServicesData.

  Allocates the number of 4KB pages of type EfiBootServicesData and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateSmmSupvPages (
  IN     UINTN         Pages
  );

/**
  This API provides a way to allocate memory for SmmSupv page table.

  This API can be called more once to allocate memory for page tables.

  Allocates the number of 4KB pages from mSmmSupvPageTableBuffer and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.
**/
VOID *
EFIAPI
AllocateSmmSupvPageTableMemory (
  IN UINTN           Pages
  );


/**
  Reinitialize SMM Supervisor memory.
**/
VOID
EFIAPI
ResetSmmSupvPageTableMemory (
  VOID
  );

/**
  Report number of SMM Supervisor memory regions.

  @return  Number of memory regions.
**/
UINT32
EFIAPI
NumberOfMemoryRegions (
  VOID
  );

/**
  Report Smm Supervisor Memory Block

  @param Index Index of memory block.

  @retval  Pointer to memory block.
  @retval  NULL if out of range.
**/
SMM_SUPV_MEM_BLOCK *
EFIAPI
GetSmmSupvMemBlock (
  IN UINT32 Index
  );

/**
  Check overlap status between two region.

  @param  Address1         The start address of region 1.
  @param  Size1            The size of region 1.
  @param  Address2         The start address of region 2.
  @param  Size2            The offset of region 2.
  @param  IsOverlapping    Boolean to return if it's overlap.

  @retval EFI_SUCCESS      There aren't any overflow occurred and overlap status have checked.
  @retval EFI_SECURITY_VIOLATION   There is a overflow occurred.
**/
EFI_STATUS
OverlapStatus (
  IN  UINTN         Address1,
  IN  UINTN         Size1,
  IN  UINTN         Address2,
  IN  UINTN         Size2,
  OUT BOOLEAN       *IsOverlapping
  );
