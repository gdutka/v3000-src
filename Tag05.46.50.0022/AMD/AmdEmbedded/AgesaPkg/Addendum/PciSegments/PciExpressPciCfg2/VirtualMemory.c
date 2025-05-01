/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/** @file
  x64 Virtual Memory Management Services in the form of an IA-32 driver.
  Used to establish a 1:1 Virtual to Physical Mapping that is required to
  enter Long Mode (x64 64-bit mode).

  While we make a 1:1 mapping (identity mapping) for all physical pages
  we still need to use the MTRR's to ensure that the cachability attributes
  for all memory regions is correct.

  The basic idea is to use 2MB page table entries where ever possible. If
  more granularity of cachability is required then 4K page tables are used.

  References:
    1) IA-32 Intel(R) Architecture Software Developer's Manual Volume 1:Basic Architecture, Intel
    2) IA-32 Intel(R) Architecture Software Developer's Manual Volume 2:Instruction Set Reference, Intel
    3) IA-32 Intel(R) Architecture Software Developer's Manual Volume 3:System Programmer's Guide, Intel

Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2017, AMD Incorporated. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Register/Intel/Cpuid.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
//#include "DxeIpl.h"
#include "VirtualMemory.h"

//
// Global variable to keep track current available memory used as page table.
//
PAGE_TABLE_POOL   *mPageTablePool = NULL;

/**
  The function will check if Execute Disable Bit is available.

  @retval TRUE      Execute Disable Bit is available.
  @retval FALSE     Execute Disable Bit is not available.

**/
BOOLEAN
IsExecuteDisableBitAvailable (
  VOID
  )
{
  UINT32            RegEax;
  UINT32            RegEdx;
  BOOLEAN           Available;

  Available = FALSE;
  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000001) {
    AsmCpuid (0x80000001, NULL, NULL, NULL, &RegEdx);
    if ((RegEdx & BIT20) != 0) {
      //
      // Bit 20: Execute Disable Bit available.
      //
      Available = TRUE;
    }
  }

  return Available;
}

/**
  Check if Execute Disable Bit (IA32_EFER.NXE) should be enabled or not.

  @retval TRUE    IA32_EFER.NXE should be enabled.
  @retval FALSE   IA32_EFER.NXE should not be enabled.

**/
BOOLEAN
IsEnableNonExecNeeded (
  VOID
  )
{
  if (!IsExecuteDisableBitAvailable ()) {
    return FALSE;
  }

  //
  // XD flag (BIT63) in page table entry is only valid if IA32_EFER.NXE is set.
  // Features controlled by Following PCDs need this feature to be enabled.
  //
  return (PcdGetBool (PcdSetNxForStack) ||
          PcdGet64 (PcdDxeNxMemoryProtectionPolicy) != 0 ||
          PcdGet32 (PcdImageProtectionPolicy) != 0);
}

/**
  Enable Execute Disable Bit.

**/
VOID
EnableExecuteDisableBit (
  VOID
  )
{
  UINT64           MsrRegisters;

  MsrRegisters = AsmReadMsr64 (0xC0000080);
  MsrRegisters |= BIT11;
  AsmWriteMsr64 (0xC0000080, MsrRegisters);
}

/**
  Initialize a buffer pool for page table use only.

  To reduce the potential split operation on page table, the pages reserved for
  page table should be allocated in the times of PAGE_TABLE_POOL_UNIT_PAGES and
  at the boundary of PAGE_TABLE_POOL_ALIGNMENT. So the page pool is always
  initialized with number of pages greater than or equal to the given PoolPages.

  Once the pages in the pool are used up, this method should be called again to
  reserve at least another PAGE_TABLE_POOL_UNIT_PAGES. But usually this won't
  happen in practice.

  @param PoolPages  The least page number of the pool to be created.

  @retval TRUE    The pool is initialized successfully.
  @retval FALSE   The memory is out of resource.
**/
BOOLEAN
InitializePageTablePool (
  IN UINTN           PoolPages
  )
{
  VOID          *Buffer;

  //
  // Always reserve at least PAGE_TABLE_POOL_UNIT_PAGES, including one page for
  // header.
  //
  PoolPages += 1;   // Add one page for header.
  Buffer = AllocatePages (PoolPages);
  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR: Out of aligned pages\r\n"));
    return FALSE;
  }

  //
  // Link all pools into a list for easier track later.
  //
  if (mPageTablePool == NULL) {
    mPageTablePool = Buffer;
    mPageTablePool->NextPool = mPageTablePool;
  } else {
    ((PAGE_TABLE_POOL *)Buffer)->NextPool = mPageTablePool->NextPool;
    mPageTablePool->NextPool = Buffer;
    mPageTablePool = Buffer;
  }

  //
  // Reserve one page for pool header.
  //
  mPageTablePool->FreePages  = PoolPages - 1;
  mPageTablePool->Offset = EFI_PAGES_TO_SIZE (1);

  return TRUE;
}

/**
  This API provides a way to allocate memory for page table.

  This API can be called more than once to allocate memory for page tables.

  Allocates the number of 4KB pages and returns a pointer to the allocated
  buffer. The buffer returned is aligned on a 4KB boundary.

  If Pages is 0, then NULL is returned.
  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
AllocatePageTableMemory (
  IN UINTN           Pages
  )
{
  VOID          *Buffer;

  if (Pages == 0) {
    return NULL;
  }

  //
  // Renew the pool if necessary.
  //
  if (mPageTablePool == NULL ||
      Pages > mPageTablePool->FreePages) {
    if (!InitializePageTablePool (Pages)) {
      return NULL;
    }
  }

  Buffer = (UINT8 *)mPageTablePool + mPageTablePool->Offset;

  mPageTablePool->Offset     += EFI_PAGES_TO_SIZE (Pages);
  mPageTablePool->FreePages  -= Pages;

  return Buffer;
}

/**
  Split 2M page to 4K.

  @param[in]      PhysicalAddress       Start physical address the 2M page covered.
  @param[in, out] PageEntry2M           Pointer to 2M page entry.
  @param[in]      StackBase             Stack base address.
  @param[in]      StackSize             Stack size.
  @param[in]      GhcbBase              GHCB page area base address.
  @param[in]      GhcbSize              GHCB page area size.

**/
VOID
Split2MPageTo4K (
  IN EFI_PHYSICAL_ADDRESS               PhysicalAddress,
  IN OUT UINT64                         *PageEntry2M,
  IN EFI_PHYSICAL_ADDRESS               StackBase,
  IN UINTN                              StackSize,
  IN EFI_PHYSICAL_ADDRESS               GhcbBase,
  IN UINTN                              GhcbSize
  )
{
  EFI_PHYSICAL_ADDRESS                  PhysicalAddress4K;
  UINTN                                 IndexOfPageTableEntries;
  PAGE_TABLE_4K_ENTRY                   *PageTableEntry;
  UINT64                                AddressEncMask;

  //
  // Make sure AddressEncMask is contained to smallest supported address field
  //
  AddressEncMask = PcdGet64 (PcdPteMemoryEncryptionAddressOrMask) & PAGING_1G_ADDRESS_MASK_64;

  PageTableEntry = AllocatePageTableMemory (1);
  ASSERT (PageTableEntry != NULL);

  //
  // Fill in 2M page entry.
  //
  *PageEntry2M = (UINT64) (UINTN) PageTableEntry | AddressEncMask | IA32_PG_P | IA32_PG_RW;

  PhysicalAddress4K = PhysicalAddress;
  for (IndexOfPageTableEntries = 0; IndexOfPageTableEntries < 512; IndexOfPageTableEntries++, PageTableEntry++, PhysicalAddress4K += SIZE_4KB) {
    //
    // Fill in the Page Table entries
    //
    PageTableEntry->Uint64 = (UINT64) PhysicalAddress4K;

    //
    // The GHCB range consists of two pages per CPU, the GHCB and a
    // per-CPU variable page. The GHCB page needs to be mapped as an
    // unencrypted page while the per-CPU variable page needs to be
    // mapped encrypted. These pages alternate in assignment.
    //
    if ((GhcbBase == 0)
        || (PhysicalAddress4K < GhcbBase)
        || (PhysicalAddress4K >= GhcbBase + GhcbSize)
        || (((PhysicalAddress4K - GhcbBase) & SIZE_4KB) != 0)) {
      PageTableEntry->Uint64 |= AddressEncMask;
    }
    PageTableEntry->Bits.ReadWrite = 1;

      PageTableEntry->Bits.Present = 1;
  }
}

/**
  Split 1G page to 2M.

  @param[in]      PhysicalAddress       Start physical address the 1G page covered.
  @param[in, out] PageEntry1G           Pointer to 1G page entry.
  @param[in]      StackBase             Stack base address.
  @param[in]      StackSize             Stack size.
  @param[in]      GhcbBase              GHCB page area base address.
  @param[in]      GhcbSize              GHCB page area size.

**/
VOID
Split1GPageTo2M (
  IN EFI_PHYSICAL_ADDRESS               PhysicalAddress,
  IN OUT UINT64                         *PageEntry1G,
  IN EFI_PHYSICAL_ADDRESS               StackBase,
  IN UINTN                              StackSize,
  IN EFI_PHYSICAL_ADDRESS               GhcbBase,
  IN UINTN                              GhcbSize
  )
{
  EFI_PHYSICAL_ADDRESS                  PhysicalAddress2M;
  UINTN                                 IndexOfPageDirectoryEntries;
  PAGE_TABLE_ENTRY                      *PageDirectoryEntry;
  UINT64                                AddressEncMask;

  //
  // Make sure AddressEncMask is contained to smallest supported address field
  //
  AddressEncMask = PcdGet64 (PcdPteMemoryEncryptionAddressOrMask) & PAGING_1G_ADDRESS_MASK_64;

  PageDirectoryEntry = AllocatePageTableMemory (1);
  ASSERT (PageDirectoryEntry != NULL);

  //
  // Fill in 1G page entry.
  //
  *PageEntry1G = (UINT64) (UINTN) PageDirectoryEntry | AddressEncMask | IA32_PG_P | IA32_PG_RW;

  PhysicalAddress2M = PhysicalAddress;
  for (IndexOfPageDirectoryEntries = 0; IndexOfPageDirectoryEntries < 512; IndexOfPageDirectoryEntries++, PageDirectoryEntry++, PhysicalAddress2M += SIZE_2MB) {
      //
      // Fill in the Page Directory entries
      //
      PageDirectoryEntry->Uint64 = (UINT64) PhysicalAddress2M | AddressEncMask;
      PageDirectoryEntry->Bits.ReadWrite = 1;
      PageDirectoryEntry->Bits.Present = 1;
      PageDirectoryEntry->Bits.MustBe1 = 1;
  }
}

/**
  Allocates and fills in the Page Directory and Page Table Entries to
  establish a 1:1 Virtual to Physical mapping.

  @param[in] StackBase  Stack base address.
  @param[in] StackSize  Stack size.
  @param[in] GhcbBase   GHCB base address.
  @param[in] GhcbSize   GHCB size.

  @return The address of 4 level page map.

**/
UINTN
CreateIdentityMappingPageTables (
  IN EFI_PHYSICAL_ADDRESS   StackBase,
  IN UINTN                  StackSize,
  IN EFI_PHYSICAL_ADDRESS   GhcbBase,
  IN UINTN                  GhcbSize
  )
{
  UINT8                                         PhysicalAddressBits;
  EFI_PHYSICAL_ADDRESS                          PageAddress;
  UINTN                                         IndexOfPml5Entries;
  UINTN                                         IndexOfPml4Entries;
  UINTN                                         IndexOfPageDirectoryEntries;
  UINT32                                        NumberOfPml5EntriesNeeded;
  UINT32                                        NumberOfPml4EntriesNeeded;
  UINT32                                        NumberOfPdpEntriesNeeded;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageMapLevel5Entry;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageMapLevel4Entry;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageMap;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageDirectoryPointerEntry;
  UINTN                                         TotalPagesNum;
  UINTN                                         BigPageAddress;
  PAGE_TABLE_1G_ENTRY                           *PageDirectory1GEntry;
  UINT64                                        AddressEncMask;

  //
  // Set PageMapLevel5Entry to suppress incorrect compiler/analyzer warnings
  //
  PageMapLevel5Entry = NULL;

  //
  // Make sure AddressEncMask is contained to smallest supported address field
  //
  AddressEncMask = PcdGet64 (PcdPteMemoryEncryptionAddressOrMask) & PAGING_1G_ADDRESS_MASK_64;

  //
  // IA-32e paging translates 48-bit linear addresses to 52-bit physical addresses
  //  when 5-Level Paging is disabled,
  //  due to either unsupported by HW, or disabled by PCD.
  //
    PhysicalAddressBits = 42; //Reduce the required page number, make it can be used before installing memory.

  //
  // Calculate the table entries needed.
  //
  NumberOfPml5EntriesNeeded = 1;
  if (PhysicalAddressBits > 48) {
    NumberOfPml5EntriesNeeded = (UINT32) LShiftU64 (1, PhysicalAddressBits - 48);
    PhysicalAddressBits = 48;
  }

  NumberOfPml4EntriesNeeded = 1;
  if (PhysicalAddressBits > 39) {
    NumberOfPml4EntriesNeeded = (UINT32) LShiftU64 (1, PhysicalAddressBits - 39);
    PhysicalAddressBits = 39;
  }

  NumberOfPdpEntriesNeeded = 1;
  ASSERT (PhysicalAddressBits > 30);
  NumberOfPdpEntriesNeeded = (UINT32) LShiftU64 (1, PhysicalAddressBits - 30);

  //
  // Pre-allocate big pages to avoid later allocations.
  //
    TotalPagesNum = (NumberOfPml4EntriesNeeded + 1) * NumberOfPml5EntriesNeeded + 1;

  //
  // Substract the one page occupied by PML5 entries if 5-Level Paging is disabled.
  //
    TotalPagesNum--;

  DEBUG ((DEBUG_INFO, "Pml5=%u Pml4=%u Pdp=%u TotalPage=%Lu\n",
    NumberOfPml5EntriesNeeded, NumberOfPml4EntriesNeeded,
    NumberOfPdpEntriesNeeded, (UINT64)TotalPagesNum));

  BigPageAddress = (UINTN) AllocatePageTableMemory (TotalPagesNum);
  ASSERT (BigPageAddress != 0);

  //
  // By architecture only one PageMapLevel4 exists - so lets allocate storage for it.
  //
  PageMap         = (VOID *) BigPageAddress;
  PageAddress        = 0;

  for ( IndexOfPml5Entries = 0
      ; IndexOfPml5Entries < NumberOfPml5EntriesNeeded
      ; IndexOfPml5Entries++) {
    //
    // Each PML5 entry points to a page of PML4 entires.
    // So lets allocate space for them and fill them in in the IndexOfPml4Entries loop.
    // When 5-Level Paging is disabled, below allocation happens only once.
    //
    PageMapLevel4Entry = (VOID *) BigPageAddress;
    BigPageAddress    += SIZE_4KB;

    for ( IndexOfPml4Entries = 0
        ; IndexOfPml4Entries < (NumberOfPml5EntriesNeeded == 1 ? NumberOfPml4EntriesNeeded : 512)
        ; IndexOfPml4Entries++, PageMapLevel4Entry++) {
      //
      // Each PML4 entry points to a page of Page Directory Pointer entires.
      // So lets allocate space for them and fill them in in the IndexOfPdpEntries loop.
      //
      PageDirectoryPointerEntry = (VOID *) BigPageAddress;
      BigPageAddress += SIZE_4KB;

      //
      // Make a PML4 Entry
      //
      PageMapLevel4Entry->Uint64 = (UINT64)(UINTN)PageDirectoryPointerEntry | AddressEncMask;
      PageMapLevel4Entry->Bits.ReadWrite = 1;
      PageMapLevel4Entry->Bits.Present = 1;

        PageDirectory1GEntry = (VOID *) PageDirectoryPointerEntry;

        for (IndexOfPageDirectoryEntries = 0; IndexOfPageDirectoryEntries < 512; IndexOfPageDirectoryEntries++, PageDirectory1GEntry++, PageAddress += SIZE_1GB) {
            //
            // Fill in the Page Directory entries
            //
            PageDirectory1GEntry->Uint64 = (UINT64)PageAddress | AddressEncMask;
            PageDirectory1GEntry->Bits.ReadWrite = 1;
            PageDirectory1GEntry->Bits.Present = 1;
            PageDirectory1GEntry->Bits.MustBe1 = 1;
        }
    }

    //
    // For the PML4 entries we are not using fill in a null entry.
    //
    ZeroMem (PageMapLevel4Entry, (512 - IndexOfPml4Entries) * sizeof (PAGE_MAP_AND_DIRECTORY_POINTER));
  }

  //
  // Set IA32_EFER.NXE if necessary.
  //
  if (IsEnableNonExecNeeded ()) {
    EnableExecuteDisableBit ();
  }

  return (UINTN)PageMap;
}

