/** @file
  x64 CPU Exception Handler.

  Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <PiSmm.h>
#include <Protocol/SmmBase2.h>
#include <Library/SmmSupervisorMemMgmtLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Library/SmmSupervisorPageMgmtLib.h>

//On 5 Level Page table configuration, Max support 1024 * 16 pages shadow, currently on typical system it use around 8200 pages
#define MAX_SMMSUPV_PAGE_TABLE_PAGES_5LEVEL    (1024 * 16)

//On 4 Level Page table configuration, Max support 1024 pages shadow, currently on typical system it use around 550 pages
#define MAX_SMMSUPV_PAGE_TABLE_PAGES_4LEVEL    (1024 * 1)

#define MAX_MEM_BLOCK_COUNT_TO_PROTECT  0x20

static SMM_SUPV_MEM_BLOCK              mMemBlockList[MAX_MEM_BLOCK_COUNT_TO_PROTECT];
static UINT32                          mMemBlockListCursor = 0;
//Global variable used to record the max pages to be allocated for SmmSupv page tables
//The value is depended on if 5 Level page table supported or not.
//MAX_SMMSUPV_PAGE_TABLE_PAGES_XLEVEL Macros defines the MAX number for 4 level and 5 level.
static UINT32                          mMaxSmmSupvPageTablePages = 0;

typedef struct {
  UINTN BufferBase;
  UINTN CurrentPage;
} SMMSUPV_PAGE_TABLE_STRUCT;

static SMMSUPV_PAGE_TABLE_STRUCT       mSmmSupvPageTableStruct;

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
  )
{
  EFI_STATUS  Status  = EFI_SUCCESS;
  VOID        *Buffer  = NULL;
  if (mMemBlockListCursor < MAX_MEM_BLOCK_COUNT_TO_PROTECT) {
    Status = gSmst->SmmAllocatePages (
                      AllocateAnyPages,
                      EfiRuntimeServicesData,
                      Pages,
                      (EFI_PHYSICAL_ADDRESS *) &Buffer
                      );
    if (Status == EFI_SUCCESS) {
      mMemBlockList[mMemBlockListCursor].BaseAddress = (UINT64) Buffer;
      mMemBlockList[mMemBlockListCursor].Length = Pages * SIZE_4KB; //1 page = 4K bytes
      DEBUG ((DEBUG_INFO, "%a mMemBlockListCursor:%x, BaseAddress:%lx, Length:%x \n", __FUNCTION__, mMemBlockListCursor, \
              mMemBlockList[mMemBlockListCursor].BaseAddress, mMemBlockList[mMemBlockListCursor].Length));
      mMemBlockListCursor++;
      return Buffer;
    } else {
      DEBUG ((DEBUG_ERROR, "%a allocate pages fail: %x\n", __FUNCTION__, Status));
      return NULL;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a Out of resource for heap allocation\n", __FUNCTION__));
    return NULL;
  }
}


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
  )
{
  UINTN RemainPages;
  UINTN AllocatedAddr;
  if (Pages == 0) {
    return NULL;
  }
  DEBUG ((DEBUG_VERBOSE, "AllocateSmmSupvPageTableMemory [%d] \n", Pages));
  ASSERT (mMaxSmmSupvPageTablePages != 0);
  if (mSmmSupvPageTableStruct.CurrentPage + Pages > mMaxSmmSupvPageTablePages) {
    DEBUG ((DEBUG_ERROR, "AllocateSmmSupvPageTableMemory Failed, out of resource\n"));
    DEBUG ((DEBUG_ERROR, "SmmSupvPageTableMemory [%d] pages allocated, [%d] pages want to allocate \n", mSmmSupvPageTableStruct.CurrentPage, Pages));
    WriteErrorData (SMMSUPV_ALLOCATE_MEM_POOL_FAIL);
    ASSERT (FALSE);
    return NULL;
  }
  AllocatedAddr = mSmmSupvPageTableStruct.BufferBase + (EFI_PAGE_SIZE * mSmmSupvPageTableStruct.CurrentPage);
  mSmmSupvPageTableStruct.CurrentPage += Pages;
  RemainPages = mMaxSmmSupvPageTablePages;
  RemainPages -= mSmmSupvPageTableStruct.CurrentPage;
  DEBUG ((DEBUG_VERBOSE, "SmmSupvPageTableMemory [%d] pages allocated, [%d] pages remains \n", mSmmSupvPageTableStruct.CurrentPage, RemainPages));
  return (VOID *) (AllocatedAddr);
}

STATIC
EFI_STATUS
InitSmst (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_SMM_BASE2_PROTOCOL  *InternalSmmBase2;

  InternalSmmBase2 = NULL;
  //
  // Retrieve SMM Base2 Protocol,  Do not use gBS from UefiBootServicesTableLib on purpose
  // to prevent inclusion of gBS, gST, and gImageHandle from SMM Drivers unless the
  // SMM driver explicitly declares that dependency.
  //
  Status = SystemTable->BootServices->LocateProtocol (
                                        &gEfiSmmBase2ProtocolGuid,
                                        NULL,
                                        (VOID **)&InternalSmmBase2
                                        );
  ASSERT_EFI_ERROR (Status);
  ASSERT (InternalSmmBase2 != NULL);

  //
  // We are in SMM, retrieve the pointer to SMM System Table
  //
  InternalSmmBase2->GetSmstLocation (InternalSmmBase2, &gSmst);
  ASSERT (gSmst != NULL);

  return EFI_SUCCESS;
}


/**
  Reinitialize SMM Supervisor memory.
**/
VOID
EFIAPI
ResetSmmSupvPageTableMemory (
  VOID
  )
{
  //Reinit the SmmSupervisorMem
  mSmmSupvPageTableStruct.CurrentPage = 0;
}

EFI_STATUS
EFIAPI
SmmSupervisorMemMgmtLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;

  // No ordering on constructors, so this may need to be initialized here.
  if (gSmst == NULL) {
    Status = InitSmst (ImageHandle, SystemTable);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //Allocate buffer for Smmsupv pagetable
  mSmmSupvPageTableStruct.BufferBase = 0;
  mSmmSupvPageTableStruct.CurrentPage = 0;
  if (CalculateMaximumSupportAddress () > 48) {
    mMaxSmmSupvPageTablePages = MAX_SMMSUPV_PAGE_TABLE_PAGES_5LEVEL;
  } else {
    mMaxSmmSupvPageTablePages = MAX_SMMSUPV_PAGE_TABLE_PAGES_4LEVEL;
  }
  DEBUG ((DEBUG_INFO, "Allocate SmmSupvPageTableBuffer pages:%d\n", mMaxSmmSupvPageTablePages));
  mSmmSupvPageTableStruct.BufferBase = (UINTN)AllocateSmmSupvPages (mMaxSmmSupvPageTablePages);
  DEBUG ((DEBUG_INFO, "mSmmSupvPageTableBuffer %x\n", mSmmSupvPageTableStruct.BufferBase));
  if (mSmmSupvPageTableStruct.BufferBase == 0) {
    //TESTPOINT (SMMSUPV_ALLOCATE_PAGE_FAIL);
    return RETURN_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

/**
  Report number of SMM Supervisor memory regions.

  @return  Number of memory regions.
**/
UINT32
EFIAPI
NumberOfMemoryRegions (
  VOID
  )
{
  return mMemBlockListCursor;
}

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
  )
{
  if (Index >= mMemBlockListCursor) {
    return NULL;
  }

  return &mMemBlockList[Index];
}

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
  )
{
  UINTN End1 = Address1 + Size1 - 1;
  UINTN End2 = Address2 + Size2 - 1;
  //Potential underflow
  if ((Size1 == 0) || (Size2 == 0)) {
    return EFI_SECURITY_VIOLATION;
  }
  //Overflow
  if (End1 < Address1) {
    return EFI_SECURITY_VIOLATION;
  }
  //Overflow
  if (End2 < Address2) {
    return EFI_SECURITY_VIOLATION;
  }

  if ((Address1 <= End2) && (Address2 <= End1)) {
    *IsOverlapping = TRUE;
  } else {
    *IsOverlapping = FALSE;
  }

  return EFI_SUCCESS;
}

