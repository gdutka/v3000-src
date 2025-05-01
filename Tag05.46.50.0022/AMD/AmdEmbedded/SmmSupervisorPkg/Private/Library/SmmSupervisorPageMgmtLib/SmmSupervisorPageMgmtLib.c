/** @file

Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmSupervisorMemMgmtLib.h>
#include <Library/SmmSupervisorPolicyMgmtLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Register/Msr.h>


// Move to Header?

typedef enum {
  PageNone,
  Page4K,
  Page2M,
  Page1G,
} PAGE_ATTRIBUTE;

typedef struct {
  PAGE_ATTRIBUTE   Attribute;
  UINT64           Length;
  UINT64           AddressMask;
} PAGE_ATTRIBUTE_TABLE;

#define PAGING_PAE_INDEX_MASK  0x1FF

#define PAGING_4K_ADDRESS_MASK_64 0x000FFFFFFFFFF000ull
#define PAGING_2M_ADDRESS_MASK_64 0x000FFFFFFFE00000ull
#define PAGING_1G_ADDRESS_MASK_64 0x000FFFFFC0000000ull

//
// Page Table Entry 4KB
//
typedef union {
  struct {
    UINT64  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;           // 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                  // 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  PS:1;                     // In PDPE, PDE present PS bit, can be used determine if it is 1G or 2M page
    UINT64  Global:1;                 // 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;              // Available for use by system software
    UINT64  PageTableBaseAddress:40;  // Page Table Base Address
    UINT64  AvailableHigh:11;         // Available for use by system software
    UINT64  Nx:1;                     // 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;
} PAGE_TABLE_ENTRY;


///
/// Page Table Entry
///
#define IA32_PG_P                   BIT0
#define IA32_PG_RW                  BIT1
#define IA32_PG_U                   BIT2
#define IA32_PG_WT                  BIT3
#define IA32_PG_CD                  BIT4
#define IA32_PG_A                   BIT5
#define IA32_PG_D                   BIT6
#define IA32_PG_PS                  BIT7
#define IA32_PG_PAT_2M              BIT12
#define IA32_PG_PAT_4K              IA32_PG_PS
#define IA32_PG_PMNT                BIT62
#define IA32_PG_NX                  BIT63

#define PAGE_ATTRIBUTE_BITS         (IA32_PG_D | IA32_PG_A | IA32_PG_U | IA32_PG_RW | IA32_PG_P)


#define PAGING_4K_MASK  0xFFF
#define PAGING_2M_MASK  0x1FFFFF
#define PAGING_1G_MASK  0x3FFFFFFF


#define PAGE_PROGATE_BITS           (IA32_PG_NX | PAGE_ATTRIBUTE_BITS)


// Force some feature
STATIC CONST UINT64 mAddressEncMask = 0;
STATIC CONST UINTN  mInternalGr3 = 0;

//
// The flag indicates if execute-disable is supported by processor.
//
STATIC CONST BOOLEAN mXdSupported     = TRUE;


STATIC CONST PAGE_ATTRIBUTE_TABLE mPageAttributeTable[] = {
  {Page4K,  SIZE_4KB, PAGING_4K_ADDRESS_MASK_64},
  {Page2M,  SIZE_2MB, PAGING_2M_ADDRESS_MASK_64},
  {Page1G,  SIZE_1GB, PAGING_1G_ADDRESS_MASK_64},
};

  STATIC UINT64  mSmmSupvCr3 = 0;

/**
  This API provides a way to allocate memory and copy for one SmmSupv page table.

  Allocates one 4KB page from mSmmSupvPageTableBuffer and copy the input page to the allocated page
  returns a pointer to the allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Page             point to the table to be copied

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
AllocateCopyOnePage (
  IN PAGE_TABLE_ENTRY * Page
  )
{
  VOID * AllocatedPage;

  AllocatedPage = AllocateSmmSupvPageTableMemory (1);
  if (AllocatedPage == NULL) {
    return NULL;
  }
  //Attribute of Pagetable is correct, pointer to next level still need patched
  // DEBUG ((DEBUG_INFO, "AllocatedPage %x <- Page:%x \n", AllocatedPage, Page));

  CopyMem (AllocatedPage, Page, EFI_PAGE_SIZE);
  return AllocatedPage;
}

/*
  Reports if CR3 shadow is enabled.

  @retval TRUE - CR3 shadow is enabled.
*/
BOOLEAN
EFIAPI
IsCr3Shadowed (
  VOID
  )
{
  return mSmmSupvCr3 != 0;
}

/**
  Return page table base.

  @param[in]  UseShadowCr3   Identify if use Shadow Cr3 or actual CR3.

  @return page table base.
**/
UINTN
EFIAPI
GetPageTableBase (
  IN  BOOLEAN                                    UseShadowCr3
  )
{
  if (UseShadowCr3) {
    if ((mSmmSupvCr3 == 0) || ((mSmmSupvCr3 & 0xFFF) != 0)) {
      WriteErrorData (SMMSUPV_SHADOW_CR3_FAIL);
      ASSERT (mSmmSupvCr3 != 0);
      ASSERT((mSmmSupvCr3 & 0xFFF) == 0); //Should be 4K aligned
    }
    return (mSmmSupvCr3);
  } else {
    return (AsmReadCr3 () & PAGING_4K_ADDRESS_MASK_64);
  }
}


/**
  Return page table entry to match the address.

  @param[in]   Address          The address to be checked.
  @param[out]  PageAttributes   The page attribute of the page entry.

  @return The page entry.
**/
VOID *
GetPageTableEntry (
  IN  PHYSICAL_ADDRESS                  Address,
  OUT PAGE_ATTRIBUTE                    *PageAttribute,
  IN  BOOLEAN                           UseShadowCr3
  )
{
  UINTN                 Index1;
  UINTN                 Index2;
  UINTN                 Index3;
  UINTN                 Index4;
  UINTN                 Index5;
  UINT64                *L1PageTable;
  UINT64                *L2PageTable;
  UINT64                *L3PageTable;
  UINT64                *L4PageTable;
  UINT64                *L5PageTable;
  IA32_CR4              Cr4;
  BOOLEAN               Enable5LevelPaging;

  Index5 = ((UINTN)RShiftU64 (Address, 48)) & PAGING_PAE_INDEX_MASK;
  Index4 = ((UINTN)RShiftU64 (Address, 39)) & PAGING_PAE_INDEX_MASK;
  Index3 = ((UINTN)Address >> 30) & PAGING_PAE_INDEX_MASK;
  Index2 = ((UINTN)Address >> 21) & PAGING_PAE_INDEX_MASK;
  Index1 = ((UINTN)Address >> 12) & PAGING_PAE_INDEX_MASK;

  // 5-Level Paging Enable (LA57) . Bit 12. Setting this bit to 1 while EFER[LMA]=1 enables 5-Level
  // paging. 5-Level paging allows for the translation of up to 57 virtual address bits
  Cr4.UintN = AsmReadCr4 ();
  Enable5LevelPaging = (BOOLEAN) (Cr4.Bits.LA57 == 1);

  if (sizeof(UINTN) == sizeof(UINT64)) {
    if (Enable5LevelPaging) {
      L5PageTable = (UINT64 *)GetPageTableBase (UseShadowCr3);
      if (L5PageTable[Index5] == 0) {
        *PageAttribute = PageNone;
        return NULL;
      }

      L4PageTable = (UINT64 *)(UINTN)(L5PageTable[Index5] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
    } else {
      L4PageTable = (UINT64 *)GetPageTableBase (UseShadowCr3);
    }
    if (L4PageTable[Index4] == 0) {
      *PageAttribute = PageNone;
      return NULL;
    }

    L3PageTable = (UINT64 *)(UINTN)(L4PageTable[Index4] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  } else {
    L3PageTable = (UINT64 *)GetPageTableBase (UseShadowCr3);
  }
  if (L3PageTable[Index3] == 0) {
    *PageAttribute = PageNone;
    return NULL;
  }
  if ((L3PageTable[Index3] & IA32_PG_PS) != 0) {
    // 1G
    *PageAttribute = Page1G;
    return &L3PageTable[Index3];
  }

  L2PageTable = (UINT64 *)(UINTN)(L3PageTable[Index3] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  if (L2PageTable[Index2] == 0) {
    *PageAttribute = PageNone;
    return NULL;
  }
  if ((L2PageTable[Index2] & IA32_PG_PS) != 0) {
    // 2M
    *PageAttribute = Page2M;
    return &L2PageTable[Index2];
  }

  // 4k
  L1PageTable = (UINT64 *)(UINTN)(L2PageTable[Index2] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  if ((L1PageTable[Index1] == 0) && (Address != 0)) {
    *PageAttribute = PageNone;
    return NULL;
  }
  *PageAttribute = Page4K;
  return &L1PageTable[Index1];
}

/**
  Return length according to page attributes.

  @param[in]  PageAttributes   The page attribute of the page entry.

  @return The length of page entry.
**/
UINTN
PageAttributeToLength (
  IN PAGE_ATTRIBUTE  PageAttribute
  )
{
  UINTN  Index;
  for (Index = 0; Index < sizeof(mPageAttributeTable)/sizeof(mPageAttributeTable[0]); Index++) {
    if (PageAttribute == mPageAttributeTable[Index].Attribute) {
      return (UINTN)mPageAttributeTable[Index].Length;
    }
  }
  return 0;
}

/**
  This function returns if there is need to split page entry.

  @param[in]  BaseAddress      The base address to be checked.
  @param[in]  Length           The length to be checked.
  @param[in]  PageEntry        The page entry to be checked.
  @param[in]  PageAttribute    The page attribute of the page entry.

  @retval SplitAttributes on if there is need to split page entry.
**/
PAGE_ATTRIBUTE
NeedSplitPage (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  UINT64                            *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute
  )
{
  UINT64                PageEntryLength;

  PageEntryLength = PageAttributeToLength (PageAttribute);

  if (((BaseAddress & (PageEntryLength - 1)) == 0) && (Length >= PageEntryLength)) {
    return PageNone;
  }

  if (((BaseAddress & PAGING_2M_MASK) != 0) || (Length < SIZE_2MB)) {
    return Page4K;
  }

  return Page2M;
}

/**
  This function splits one page entry to small page entries.

  @param[in]  PageEntry        The page entry to be splitted.
  @param[in]  PageAttribute    The page attribute of the page entry.
  @param[in]  SplitAttribute   How to split the page entry.

  @retval RETURN_SUCCESS            The page entry is splitted.
  @retval RETURN_UNSUPPORTED        The page entry does not support to be splitted.
  @retval RETURN_OUT_OF_RESOURCES   No resource to split page entry.
**/
RETURN_STATUS
SplitPage (
  IN  UINT64                            *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute,
  IN  PAGE_ATTRIBUTE                    SplitAttribute
  )
{
  UINT64   BaseAddress;
  UINT64   *NewPageEntry;
  UINTN    Index;

  ASSERT (PageAttribute == Page2M || PageAttribute == Page1G);

  if (PageAttribute == Page2M) {
    //
    // Split 2M to 4K
    //
    ASSERT (SplitAttribute == Page4K);
    if (SplitAttribute == Page4K) {
      NewPageEntry = AllocateSmmSupvPageTableMemory (1);
      DEBUG ((DEBUG_VERBOSE, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = *PageEntry & PAGING_2M_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_4KB * Index) | mAddressEncMask | ((*PageEntry) & PAGE_PROGATE_BITS);
      }
      (*PageEntry) = (UINT64)(UINTN)NewPageEntry | mAddressEncMask | PAGE_ATTRIBUTE_BITS;
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else if (PageAttribute == Page1G) {
    //
    // Split 1G to 2M
    // No need support 1G->4K directly, we should use 1G->2M, then 2M->4K to get more compact page table.
    //
    ASSERT (SplitAttribute == Page2M || SplitAttribute == Page4K);
    if ((SplitAttribute == Page2M || SplitAttribute == Page4K)) {
      NewPageEntry = AllocateSmmSupvPageTableMemory (1);
      DEBUG ((DEBUG_VERBOSE, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = *PageEntry & PAGING_1G_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_2MB * Index) | mAddressEncMask | IA32_PG_PS | ((*PageEntry) & PAGE_PROGATE_BITS);
      }
      (*PageEntry) = (UINT64)(UINTN)NewPageEntry | mAddressEncMask | PAGE_ATTRIBUTE_BITS;
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else {
    return RETURN_UNSUPPORTED;
  }
}

/**
  Modify memory attributes of page entry.

  @param[in]   PageEntry        The page entry.
  @param[in]   Attributes       The bit mask of attributes to modify for the memory region.
  @param[in]   IsSet            TRUE means to set attributes. FALSE means to clear attributes.
  @param[out]  IsModified       TRUE means page table modified. FALSE means page table not modified.
**/
VOID
ConvertPageEntryAttribute (
  IN  UINT64                            *PageEntry,
  IN  UINT64                            Attributes,
  IN  BOOLEAN                           IsSet,
  OUT BOOLEAN                           *IsModified
  )
{
  UINT64  CurrentPageEntry;
  UINT64  NewPageEntry;

  CurrentPageEntry = *PageEntry;
  NewPageEntry = CurrentPageEntry;
  if ((Attributes & EFI_MEMORY_RP) != 0) {
    if (IsSet) {
      NewPageEntry &= ~(UINT64)IA32_PG_P;
    } else {
      NewPageEntry |= IA32_PG_P;
    }
  }
  if ((Attributes & EFI_MEMORY_RO) != 0) {
    if (IsSet) {
      NewPageEntry &= ~(UINT64)IA32_PG_RW;
      if (mInternalGr3 != 0) {
        // Environment setup
        // ReadOnly page need set Dirty bit for shadow stack
        NewPageEntry |= IA32_PG_D;
        // Clear user bit for supervisor shadow stack
        NewPageEntry &= ~(UINT64)IA32_PG_U;
      } else {
        // Runtime update
        // Clear dirty bit for non shadow stack, to protect RO page.
        NewPageEntry &= ~(UINT64)IA32_PG_D;
      }
    } else {
      NewPageEntry |= IA32_PG_RW;
    }
  }
  if ((Attributes & EFI_MEMORY_XP) != 0) {
    if (mXdSupported) {
      if (IsSet) {
        NewPageEntry |= IA32_PG_NX;
      } else {
        NewPageEntry &= ~IA32_PG_NX;
      }
    }
  }
  if ((Attributes & EFI_MEMORY_SP) != 0) {
  // UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    if (IsSet) {
      // Clear user bit for supervisor shadow stack
      NewPageEntry &= ~(UINT64)IA32_PG_U;
    } else {
      NewPageEntry |= IA32_PG_U;
    }
  }
  *PageEntry = NewPageEntry;
  if (CurrentPageEntry != NewPageEntry) {
    *IsModified = TRUE;
    DEBUG ((DEBUG_VERBOSE, "ConvertPageEntryAttribute 0x%lx", CurrentPageEntry));
    DEBUG ((DEBUG_VERBOSE, "->0x%lx\n", NewPageEntry));
  } else {
    *IsModified = FALSE;
  }
}

/**
  Calculate the maximum support address.

  @return the maximum support address.
**/
UINT8
CalculateMaximumSupportAddress (
  VOID
  )
{
  UINT32                                        RegEax;
  UINT8                                         PhysicalAddressBits;

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  return PhysicalAddressBits;
}

/**
  This function modifies the page attributes for the memory region specified by BaseAddress and
  Length from their current attributes to the attributes specified by Attributes.

  Caller should make sure BaseAddress and Length is at page boundary.

  @param[in]   BaseAddress      The physical address that is the start address of a memory region.
  @param[in]   Length           The size in bytes of the memory region.
  @param[in]   Attributes       The bit mask of attributes to modify for the memory region.
  @param[in]   IsSet            TRUE means to set attributes. FALSE means to clear attributes.
  @param[out]  IsSplitted       TRUE means page table splitted. FALSE means page table not splitted.
  @param[out]  IsModified       TRUE means page table modified. FALSE means page table not modified.

  @retval RETURN_SUCCESS           The attributes were modified for the memory region.
  @retval RETURN_ACCESS_DENIED     The attributes for the memory resource range specified by
                                   BaseAddress and Length cannot be modified.
  @retval RETURN_INVALID_PARAMETER Length is zero.
                                   Attributes specified an illegal combination of attributes that
                                   cannot be set together.
  @retval RETURN_OUT_OF_RESOURCES  There are not enough system resources to modify the attributes of
                                   the memory resource range.
  @retval RETURN_UNSUPPORTED       The processor does not support one or more bytes of the memory
                                   resource range specified by BaseAddress and Length.
                                   The bit mask of attributes is not support for the memory resource
                                   range specified by BaseAddress and Length.
**/
RETURN_STATUS
EFIAPI
ConvertMemoryPageAttributes (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  UINT64                            Attributes,
  IN  BOOLEAN                           IsSet,
  OUT BOOLEAN                           *IsSplitted,  OPTIONAL
  OUT BOOLEAN                           *IsModified   OPTIONAL,
  IN  BOOLEAN                           UseShadowCr3
  )
{
  UINT64                            *PageEntry;
  PAGE_ATTRIBUTE                    PageAttribute;
  UINTN                             PageEntryLength;
  PAGE_ATTRIBUTE                    SplitAttribute;
  RETURN_STATUS                     Status;
  BOOLEAN                           IsEntryModified;
  EFI_PHYSICAL_ADDRESS              MaximumSupportMemAddress;
  UINT8                             PhysicalAddressBits;

  ASSERT (Attributes != 0);
  ASSERT ((Attributes & ~(EFI_MEMORY_RP | EFI_MEMORY_RO | EFI_MEMORY_XP | EFI_MEMORY_SP)) == 0);

  ASSERT ((BaseAddress & (SIZE_4KB - 1)) == 0);
  ASSERT ((Length & (SIZE_4KB - 1)) == 0);

  if (Length == 0) {
    return RETURN_INVALID_PARAMETER;
  }
  PhysicalAddressBits = CalculateMaximumSupportAddress ();
  MaximumSupportMemAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)(LShiftU64 (1, PhysicalAddressBits) - 1);
  if (BaseAddress > MaximumSupportMemAddress) {
    return RETURN_UNSUPPORTED;
  }
  if (Length > MaximumSupportMemAddress) {
    return RETURN_UNSUPPORTED;
  }
  if ((Length != 0) && (BaseAddress > MaximumSupportMemAddress - (Length - 1))) {
    return RETURN_UNSUPPORTED;
  }

//  DEBUG ((DEBUG_ERROR, "ConvertMemoryPageAttributes(%x) - %016lx, %016lx, %02lx\n", IsSet, BaseAddress, Length, Attributes));

  if (IsSplitted != NULL) {
    *IsSplitted = FALSE;
  }
  if (IsModified != NULL) {
    *IsModified = FALSE;
  }

  //
  // Below logic is to check 2M/4K page to make sure we don't waste memory.
  //
  while (Length != 0) {
    PageEntry = GetPageTableEntry (BaseAddress, &PageAttribute, UseShadowCr3);
    if (PageEntry == NULL) {
      return RETURN_UNSUPPORTED;
    }
    PageEntryLength = PageAttributeToLength (PageAttribute);
    SplitAttribute = NeedSplitPage (BaseAddress, Length, PageEntry, PageAttribute);
    if (SplitAttribute == PageNone) {
      ConvertPageEntryAttribute (PageEntry, Attributes, IsSet, &IsEntryModified);
      if (IsEntryModified) {
        if (IsModified != NULL) {
          *IsModified = TRUE;
        }
      }
      //
      // Convert success, move to next
      //
      BaseAddress += PageEntryLength;
      Length -= PageEntryLength;
    } else {
      Status = SplitPage (PageEntry, PageAttribute, SplitAttribute);
      if (RETURN_ERROR (Status)) {
        return RETURN_UNSUPPORTED;
      }
      if (IsSplitted != NULL) {
        *IsSplitted = TRUE;
      }
      if (IsModified != NULL) {
        *IsModified = TRUE;
      }
      //
      // Just split current page
      // Convert success in next around
      //
    }
  }

  return RETURN_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN  IsModified;

  Status = ConvertMemoryPageAttributes (BaseAddress, Length, Attributes, TRUE, IsSplitted, &IsModified, UseShadowCr3);

  return Status;
}

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
  )
{
  EFI_STATUS  Status;
  BOOLEAN  IsModified;

  Status = ConvertMemoryPageAttributes (BaseAddress, Length, Attributes, FALSE, IsSplitted, &IsModified, UseShadowCr3);

  return Status;
}

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
  )

{
  return SmmSetMemoryAttributesEx (BaseAddress, Length, Attributes, NULL, UseShadowCr3);
}

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
  )
{
  return SmmClearMemoryAttributesEx (BaseAddress, Length, Attributes, NULL, UseShadowCr3);
}

/**
  Update PageTableBaseAddress in PageEntry to the address of NewPageEntry
**/
VOID
UpdatePageTblPhyAddr (
  PAGE_TABLE_ENTRY *PageEntry,
  UINT64 NewPageEntryAddr
  )
{
  PageEntry->Bits.PageTableBaseAddress = (NewPageEntryAddr >> 12);
}

// comment to disable debug print
// #define PAGE_TBL_PRINT_EN

#ifdef PAGE_TBL_PRINT_EN
  #define PAGE_TBL_PRINT(Expression) DEBUG (Expression)
#else
  #define PAGE_TBL_PRINT(Expression)
#endif

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
  )
{
  MSR_IA32_EFER_REGISTER MsrEfer;
  IA32_CR0 Cr0;
  IA32_CR4  Cr4;
  UINT64 Cr3Value;
  UINT32 Pml5Index;
  UINT32 Pml4Index;
  UINT32 PdpeIndex;
  UINT32 PdeIndex;
  UINT32 PteIndex;
  PAGE_TABLE_ENTRY PML5FakeEntry;
  PAGE_TABLE_ENTRY *PML5Table;
  PAGE_TABLE_ENTRY *PML4Table;
  PAGE_TABLE_ENTRY *PDPETable;
  PAGE_TABLE_ENTRY *PDETable;
  PAGE_TABLE_ENTRY *PTETable;
  PAGE_TABLE_ENTRY *PML5Table_S;   //Shadow Copy
  PAGE_TABLE_ENTRY *PML4Table_S;   //Shadow Copy
  PAGE_TABLE_ENTRY *PDPETable_S;   //Shadow Copy
  PAGE_TABLE_ENTRY *PDETable_S;   //Shadow Copy
  PAGE_TABLE_ENTRY *PTETable_S;   //Shadow Copy
  UINT64 PageTableBaseAddress;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR *LegacyMemoryPolicy;
  SMM_SUPV_SECURE_POLICY_MEM_DESCRIPTOR_V1_0 *MemoryPolicy;
  UINT32 PML5MemoryAttr;
  UINT32 PML4MemoryAttr;
  UINT32 PDPEMemoryAttr;
  UINT32 PDEMemoryAttr;
  UINT32 MemoryAttr;
  BOOLEAN IgnoreNx;
  UINTN L5PageNum;
  UINTN L4PageNum;
  UINTN L3PageNum;
  UINTN L2PageNum;
  UINTN L1PageNum;
  UINTN MemPolicyBoundary;
  BOOLEAN Enable5LevelPaging;

  if (IsLegacy && !FeaturePcdGet (SupportLegacyMemoryPolicy)) {
    return EFI_UNSUPPORTED;
  }

  MemPolicyBoundary = (UINTN)MemoryPolicyPtr + MemoryPolicySize;
  *MemoryPolicyCount = 1;
  L5PageNum = 0;
  L4PageNum = 0;
  L3PageNum = 0;
  L2PageNum = 0;
  L1PageNum = 0;
  PML5Table_S = NULL;
  PML4Table_S = NULL;
  if (IsLegacy) {
    LegacyMemoryPolicy = MemoryPolicyPtr;
    InitPolicyMemDescHdr (LegacyMemoryPolicy);
    LegacyMemoryPolicy->BaseAddress = MEM_DESC_UNINIT_BASEADDR;
  } else {
    MemoryPolicy = MemoryPolicyPtr;
    MemoryPolicy->BaseAddress = MEM_DESC_UNINIT_BASEADDR;
  }
  DEBUG ((DEBUG_INFO, "GenMemPolicyAndShadowPageTable, MemoryPolicyPtr:%lx, MemoryPolicySize:%lx\n", (UINTN)MemoryPolicyPtr, MemoryPolicySize));
  //Code to traverse SMM page table to generate SMM policy structure
  //Input Cr3 is zero, check the real HW register
  if (Cr3 == 0) {
    //Only support long page table
    //Check if long mode
    MsrEfer.Uint64 = AsmReadMsr64 (MSR_IA32_EFER);
    DEBUG ((DEBUG_INFO, "EFER.NXE = %x\n", MsrEfer.Bits.NXE));
    if (MsrEfer.Bits.NXE == 0) {
      IgnoreNx = 1;
    }
    if ((MsrEfer.Bits.LME == 0) || (MsrEfer.Bits.LMA == 0)) {
      DEBUG ((DEBUG_ERROR, "EFER.LMA ==0 || EFER.LME == 0, exit\n"));
      return EFI_UNSUPPORTED;
    }
    //Check if Page enabled
    Cr0.UintN = AsmReadCr0 ();
    if (Cr0.Bits.PG == 0) {
      DEBUG ((DEBUG_ERROR, "Cr0.Bits.PG == 0, exit\n"));
      return EFI_UNSUPPORTED;
    }
    //Assuming CR0.WP enabled
    if (Cr0.Bits.WP == 0) {
      DEBUG ((DEBUG_ERROR, "Cr0.Bits.WP == 0, exit\n"));
      return EFI_UNSUPPORTED;
    }
    //Use Cr3Value to make the code more generic to support both 4 level and 5 Level table
    Cr3Value = AsmReadCr3 () & 0x000FFFFFFFFFF000ull;
  } else {
    //Get PML5 base from input
    Cr3Value = Cr3 & 0x000FFFFFFFFFF000ull;
  }

  Cr4.UintN          = AsmReadCr4 ();
  Enable5LevelPaging = (BOOLEAN)(Cr4.Bits.LA57 == 1);

  DEBUG ((DEBUG_INFO, "Cr3Value = 0x%x\n", Cr3Value));
  if (IsShadow) {
    ResetSmmSupvPageTableMemory ();
    //After copy, attribute of Pagetable is correct, pointer to next level still need patched
    //Create the 1st level
    mSmmSupvCr3 = (UINT64) AllocateCopyOnePage ((PAGE_TABLE_ENTRY *) Cr3Value);
    DEBUG ((DEBUG_INFO, "mSmmSupvCr3 0x%x\n", mSmmSupvCr3));
    if (Enable5LevelPaging) {
      PML5Table_S = (PAGE_TABLE_ENTRY *) mSmmSupvCr3;
    } else {
      PML4Table_S = (PAGE_TABLE_ENTRY *) mSmmSupvCr3;
    }
  }

  PML5Table = (PAGE_TABLE_ENTRY *) Cr3Value;
  //Make a fake PML5 with one entry in 4 level case
  if (!Enable5LevelPaging) {
   PML5FakeEntry.Uint64 = 0;
   PML5FakeEntry.Uint64 = Cr3Value | IA32_PG_P | IA32_PG_RW;  //The fake entry will not limit any access
   PML5Table = &PML5FakeEntry;
  }

  L5PageNum++;
  //In 4 level case, the Pml5 only loop once
  for (Pml5Index = 0; Pml5Index < (UINT32) (Enable5LevelPaging ? 512 : 1); Pml5Index++) {
    //If present not set, skip this page table entry
    if (PML5Table[Pml5Index].Bits.Present == 0) {
      continue;
    }
    PML4Table = (PAGE_TABLE_ENTRY *) (PML5Table[Pml5Index].Uint64 & 0x000FFFFFFFFFF000ull);
    if (Enable5LevelPaging) {
      PAGE_TBL_PRINT ((DEBUG_VERBOSE, "PML5[%d] = 0x%Lx \n", Pml5Index, PML5Table[Pml5Index].Uint64));
      PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
        PML5Table[Pml5Index].Bits.Present, \
        PML5Table[Pml5Index].Bits.ReadWrite, \
        PML5Table[Pml5Index].Bits.UserSupervisor, \
        PML5Table[Pml5Index].Bits.WriteThrough, \
        PML5Table[Pml5Index].Bits.CacheDisabled, \
        PML5Table[Pml5Index].Bits.Accessed, \
        PML5Table[Pml5Index].Bits.Dirty, \
        PML5Table[Pml5Index].Bits.PS, \
        PML5Table[Pml5Index].Bits.Global, \
        PML5Table[Pml5Index].Bits.Nx
      ));
      if (IsShadow) {
        //After copy, attribute of Pagetable is correct, pointer to next level still need patched
        PML4Table_S = AllocateCopyOnePage (PML4Table);
        UpdatePageTblPhyAddr (&PML5Table_S[Pml5Index], (UINT64) PML4Table_S);
      }
    }

    PML5MemoryAttr = 0;
    if (PML5Table[Pml5Index].Bits.ReadWrite == 0 && IsLegacy) {
      PML5MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
    } else if (PML5Table[Pml5Index].Bits.ReadWrite == 1 && !IsLegacy) {
      PML5MemoryAttr |= (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_READ);
    } else if (PML5Table[Pml5Index].Bits.ReadWrite == 0 && !IsLegacy) {
      PML5MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
    }
    if (PML5Table[Pml5Index].Bits.Nx == 1 && IsLegacy) {
      PML5MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS;
    } else if (PML5Table[Pml5Index].Bits.Nx == 0 && !IsLegacy) {
      PML5MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
    }
    L4PageNum++;
    //Loop 512 PML4 entries
    for (Pml4Index = 0; Pml4Index < 512; Pml4Index++) {
      PAGE_TBL_PRINT ((DEBUG_VERBOSE, "\tPML4[%d] = 0x%Lx \n", Pml4Index, PML4Table[Pml4Index].Uint64));
      //If present not set, skip this page table entry
      if (PML4Table[Pml4Index].Bits.Present == 0) {
        continue;
      }
      PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
        PML4Table[Pml4Index].Bits.Present, \
        PML4Table[Pml4Index].Bits.ReadWrite, \
        PML4Table[Pml4Index].Bits.UserSupervisor, \
        PML4Table[Pml4Index].Bits.WriteThrough, \
        PML4Table[Pml4Index].Bits.CacheDisabled, \
        PML4Table[Pml4Index].Bits.Accessed, \
        PML4Table[Pml4Index].Bits.Dirty, \
        PML4Table[Pml4Index].Bits.PS, \
        PML4Table[Pml4Index].Bits.Global, \
        PML4Table[Pml4Index].Bits.Nx
      ));
      PML4MemoryAttr = 0;
      if (PML4Table[Pml4Index].Bits.ReadWrite == 0 && IsLegacy) {
        PML4MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
      } else if (PML4Table[Pml4Index].Bits.ReadWrite == 1 && !IsLegacy) {
        PML4MemoryAttr |= (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_READ);
      } else if (PML4Table[Pml4Index].Bits.ReadWrite == 0 && !IsLegacy) {
        PML4MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
      }
      if (PML4Table[Pml4Index].Bits.Nx == 1 && IsLegacy) {
        PML4MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS;
      } else if (PML4Table[Pml4Index].Bits.Nx == 0 && !IsLegacy) {
        PML4MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
      }
      PDPETable = (PAGE_TABLE_ENTRY *) (PML4Table[Pml4Index].Uint64 & 0x000FFFFFFFFFF000ull);
      L3PageNum++;
      if (IsShadow) {
        //After copy, attribute of Pagetable is correct, pointer to next level still need patched
        PDPETable_S = AllocateCopyOnePage (PDPETable);
        UpdatePageTblPhyAddr (&PML4Table_S[Pml4Index], (UINT64) PDPETable_S);
      }
      //Loop 512 PDPE entries
      for (PdpeIndex = 0; PdpeIndex < 512; PdpeIndex++) {
        //If present not set, skip this page table entry
        if (PDPETable[PdpeIndex].Bits.Present == 0) {
          continue;
        }
        PAGE_TBL_PRINT ((DEBUG_VERBOSE, "\t\tPDPE[%d] = 0x%x ", PdpeIndex, PDPETable[PdpeIndex].Uint64));
        PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
          PDPETable[PdpeIndex].Bits.Present, \
          PDPETable[PdpeIndex].Bits.ReadWrite, \
          PDPETable[PdpeIndex].Bits.UserSupervisor, \
          PDPETable[PdpeIndex].Bits.WriteThrough, \
          PDPETable[PdpeIndex].Bits.CacheDisabled, \
          PDPETable[PdpeIndex].Bits.Accessed, \
          PDPETable[PdpeIndex].Bits.Dirty, \
          PDPETable[PdpeIndex].Bits.PS, \
          PDPETable[PdpeIndex].Bits.Global, \
          PDPETable[PdpeIndex].Bits.Nx \
        ));
        PDPEMemoryAttr = 0;
        if (PDPETable[PdpeIndex].Bits.ReadWrite == 0 && IsLegacy) {
          PDPEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
        } else if (PDPETable[PdpeIndex].Bits.ReadWrite == 1 && !IsLegacy) {
          PDPEMemoryAttr |= (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_READ);
        } else if (PDPETable[PdpeIndex].Bits.ReadWrite == 0 && !IsLegacy) {
          PDPEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
        }
        if (PDPETable[PdpeIndex].Bits.Nx == 1 && IsLegacy) {
          PDPEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS;
        } else if (PDPETable[PdpeIndex].Bits.Nx == 0 && !IsLegacy) {
          PDPEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
        }
        //PS resent in the lowest level of the page-translation
        if (PDPETable[PdpeIndex].Bits.PS == 1) {
          //1G Table reached
          PageTableBaseAddress = PDPETable[PdpeIndex].Uint64 & PAGING_1G_ADDRESS_MASK_64;
          if (PDPETable[PdpeIndex].Bits.UserSupervisor == 0) {
            DEBUG ((DEBUG_VERBOSE, "CPL0 ADDR[%lx-%lx]\n", PageTableBaseAddress, PageTableBaseAddress+ 0x40000000ull -1));
          }
          PAGE_TBL_PRINT ((DEBUG_VERBOSE, "ADDR[%lx-%lx]", PageTableBaseAddress, PageTableBaseAddress+ 0x40000000ull -1));
          PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
            PDPETable[PdpeIndex].Bits.Present, \
            PDPETable[PdpeIndex].Bits.ReadWrite, \
            PDPETable[PdpeIndex].Bits.UserSupervisor, \
            PDPETable[PdpeIndex].Bits.WriteThrough, \
            PDPETable[PdpeIndex].Bits.CacheDisabled, \
            PDPETable[PdpeIndex].Bits.Accessed, \
            PDPETable[PdpeIndex].Bits.Dirty, \
            PDPETable[PdpeIndex].Bits.PS, \
            PDPETable[PdpeIndex].Bits.Global, \
            PDPETable[PdpeIndex].Bits.Nx \
          ));
          //Set to R/W, execute enable as default
          MemoryAttr = 0;
          if (PDPETable[PdpeIndex].Bits.Present == 0 && IsLegacy) {
            MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ_DIS;
            MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
          } else if (PDPETable[PdpeIndex].Bits.Present == 1 && !IsLegacy) {
            MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
            MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
            MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE;
            // For non-present page, MemoryAttr will be 0, indicating this page will be R/W/X disabled
          }
          //Inherit upper table R/W attribute
          if (IsLegacy) {
            // For legacy, upper table R/W denial can be inherited
            MemoryAttr |= PDPEMemoryAttr;
            MemoryAttr |= PML4MemoryAttr;
          } else {
            // For new struct, upper table R/W allowance can be inherited
            MemoryAttr &= PDPEMemoryAttr;
            MemoryAttr &= PML4MemoryAttr;
          }
          if (IsLegacy) {
            UpdateLegacyMemoryDesc (&LegacyMemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x40000000ull);
            ASSERT ((UINTN) LegacyMemoryPolicy < MemPolicyBoundary);
            if ((UINTN) LegacyMemoryPolicy >= MemPolicyBoundary) {
              return EFI_OUT_OF_RESOURCES;
            }
          } else {
            UpdateMemoryDesc (&MemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x40000000ull);
            ASSERT ((UINTN) MemoryPolicy < MemPolicyBoundary);
            if ((UINTN) MemoryPolicy >= MemPolicyBoundary) {
              return EFI_OUT_OF_RESOURCES;
            }
          }
          continue;
        }
        PDETable = (PAGE_TABLE_ENTRY *) (PDPETable[PdpeIndex].Uint64 & 0x000FFFFFFFFFF000ull);
        L2PageNum++;
        if (IsShadow) {
          //After copy, attribute of Pagetable is correct, pointer to next level still need patched
          PDETable_S = AllocateCopyOnePage (PDETable);
          UpdatePageTblPhyAddr (&PDPETable_S[PdpeIndex], (UINT64) PDETable_S);
        }
        //Loop 512 PDE entries
        for (PdeIndex = 0; PdeIndex < 512; PdeIndex++) {
          //If present not set, skip this page table entry
          if (PDETable[PdeIndex].Bits.Present == 0) {
            continue;
          }
          PAGE_TBL_PRINT ((DEBUG_VERBOSE, "\t\t\tPDE[%d] = 0x%x ", PdeIndex, PDETable[PdeIndex].Uint64));
          PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
                  PDETable[PdeIndex].Bits.Present, \
                  PDETable[PdeIndex].Bits.ReadWrite, \
                  PDETable[PdeIndex].Bits.UserSupervisor, \
                  PDETable[PdeIndex].Bits.WriteThrough, \
                  PDETable[PdeIndex].Bits.CacheDisabled, \
                  PDETable[PdeIndex].Bits.Accessed, \
                  PDETable[PdeIndex].Bits.Dirty, \
                  PDETable[PdeIndex].Bits.PS, \
                  PDETable[PdeIndex].Bits.Global, \
                  PDETable[PdeIndex].Bits.Nx
                ));
          PDEMemoryAttr = 0;
          if (PDETable[PdeIndex].Bits.ReadWrite == 0 && IsLegacy) {
            PDEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
          } else if (PDETable[PdeIndex].Bits.ReadWrite == 1 && !IsLegacy) {
            PDEMemoryAttr |= (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_READ);
          } else if (PDETable[PdeIndex].Bits.ReadWrite == 0 && !IsLegacy) {
            PDEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
          }
          if (PDETable[PdeIndex].Bits.Nx == 1 && IsLegacy) {
            PDEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS;
          } else if (PDETable[PdeIndex].Bits.Nx == 0 && !IsLegacy) {
            PDEMemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
          }
          //PS resent in the lowest level of the page-translation
          if (PDETable[PdeIndex].Bits.PS == 1) {
            //2M Table reached
            PageTableBaseAddress = PDETable[PdeIndex].Uint64 & PAGING_2M_ADDRESS_MASK_64;
            if (PDETable[PdeIndex].Bits.UserSupervisor == 0) {
              DEBUG ((DEBUG_VERBOSE, "CPL0 ADDR[%lx-%lx]\n", PageTableBaseAddress, PageTableBaseAddress+ 0x200000ull -1));
            }
            PAGE_TBL_PRINT ((DEBUG_VERBOSE, "ADDR[%lx-%lx]", PageTableBaseAddress, PageTableBaseAddress+ 0x200000ull - 1));
            PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x PS:%x G:%x NX:%x\n", \
                    PDETable[PdeIndex].Bits.Present, \
                    PDETable[PdeIndex].Bits.ReadWrite, \
                    PDETable[PdeIndex].Bits.UserSupervisor, \
                    PDETable[PdeIndex].Bits.WriteThrough, \
                    PDETable[PdeIndex].Bits.CacheDisabled, \
                    PDETable[PdeIndex].Bits.Accessed, \
                    PDETable[PdeIndex].Bits.Dirty, \
                    PDETable[PdeIndex].Bits.PS, \
                    PDETable[PdeIndex].Bits.Global, \
                    PDETable[PdeIndex].Bits.Nx
                  ));
            //Set to R/W, execute enable as default
            MemoryAttr = 0;
            if (PDETable[PdeIndex].Bits.Present == 0 && IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ_DIS;
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
            } else if (PDETable[PdeIndex].Bits.Present == 1 && !IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE;
              // For non-present page, MemoryAttr will be 0, indicating this page will be R/W/X disabled
            }
            //Inherit upper table R/W attribute
            if (IsLegacy) {
              // For legacy, upper table R/W denial can be inherited
              MemoryAttr |= PDEMemoryAttr;
              MemoryAttr |= PDPEMemoryAttr;
              MemoryAttr |= PML4MemoryAttr;
            }
            else {
              // For new struct, upper table R/W allowance can be inherited
              MemoryAttr &= PDEMemoryAttr;
              MemoryAttr &= PDPEMemoryAttr;
              MemoryAttr &= PML4MemoryAttr;
            }
            if (IsLegacy) {
              UpdateLegacyMemoryDesc (&LegacyMemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x200000ull);
              ASSERT ((UINTN) LegacyMemoryPolicy < MemPolicyBoundary);
              if ((UINTN) LegacyMemoryPolicy >= MemPolicyBoundary) {
                return EFI_OUT_OF_RESOURCES;
              }
            } else {
              UpdateMemoryDesc (&MemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x200000ull);
              ASSERT ((UINTN) MemoryPolicy < MemPolicyBoundary);
              if ((UINTN) MemoryPolicy >= MemPolicyBoundary) {
                return EFI_OUT_OF_RESOURCES;
              }
            }
            continue;
          }
          L1PageNum++;
          PTETable = (PAGE_TABLE_ENTRY *) (PDETable[PdeIndex].Uint64 & 0x000FFFFFFFFFF000ull);
          if (IsShadow) {
            //After copy, attribute of Pagetable is correct, pointer to next level still need patched
            PTETable_S = AllocateCopyOnePage (PTETable);
            UpdatePageTblPhyAddr (&PDETable_S[PdeIndex], (UINT64) PTETable_S);
          }
          //Loop 512 PTE entries
          for (PteIndex = 0; PteIndex < 512; PteIndex++) {
            //If present not set, skip this page table entry
            if (PTETable[PteIndex].Bits.Present == 0) {
              continue;
            }
            //4K Table reached
            PageTableBaseAddress = PTETable[PteIndex].Uint64 & PAGING_4K_ADDRESS_MASK_64;
            if (PTETable[PteIndex].Bits.UserSupervisor == 0) {
              DEBUG ((DEBUG_VERBOSE, "CPL0 ADDR[%lx-%lx]\n", PageTableBaseAddress, PageTableBaseAddress+ 0x1000ull -1));
            }
            PAGE_TBL_PRINT ((DEBUG_VERBOSE, "ADDR[%lx-%lx]", PageTableBaseAddress, PageTableBaseAddress+ 0x1000ull -1));
            PAGE_TBL_PRINT ((DEBUG_VERBOSE, "P:%x R/W:%x U/S:%x PWT:%x PCD:%x A:%x D:%x G:%x NX:%x\n", \
                    PTETable[PteIndex].Bits.Present, \
                    PTETable[PteIndex].Bits.ReadWrite, \
                    PTETable[PteIndex].Bits.UserSupervisor, \
                    PTETable[PteIndex].Bits.WriteThrough, \
                    PTETable[PteIndex].Bits.CacheDisabled, \
                    PTETable[PteIndex].Bits.Accessed, \
                    PTETable[PteIndex].Bits.Dirty, \
                    PTETable[PteIndex].Bits.Global, \
                    PTETable[PteIndex].Bits.Nx
                  ));
            //Set to R/W, execute enable as default
            MemoryAttr = 0;
            if (PTETable[PteIndex].Bits.ReadWrite == 0 && IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
            } else if (PTETable[PteIndex].Bits.ReadWrite == 1 && !IsLegacy) {
              MemoryAttr |= (SECURE_POLICY_RESOURCE_ATTR_WRITE | SECURE_POLICY_RESOURCE_ATTR_READ);
            } else if (PTETable[PteIndex].Bits.ReadWrite == 0 && !IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ;
            }

            if (PTETable[PteIndex].Bits.Present == 0 && IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_READ_DIS;
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS;
            } else if (!IsLegacy) {
              if (PTETable[PteIndex].Bits.Present == 1) {
                if (PTETable[PteIndex].Bits.Nx == 0) {
                  MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE;
                }
              } else {
                MemoryAttr = 0;
              }
            }

            if (PTETable[PteIndex].Bits.Nx == 1 && IsLegacy) {
              MemoryAttr |= SECURE_POLICY_RESOURCE_ATTR_EXECUTE_DIS;
            }

            //Inherit upper table R/W attribute
            if (IsLegacy) {
              // For legacy, upper table R/W denial can be inherited
              MemoryAttr |= PDEMemoryAttr;
              MemoryAttr |= PDPEMemoryAttr;
              MemoryAttr |= PML4MemoryAttr;
            } else {
              // For new struct, upper table R/W allowance can be inherited
              MemoryAttr &= PDEMemoryAttr;
              MemoryAttr &= PDPEMemoryAttr;
              MemoryAttr &= PML4MemoryAttr;
            }
            if (IsLegacy) {
              UpdateLegacyMemoryDesc (&LegacyMemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x1000ull);
              ASSERT ((UINTN) LegacyMemoryPolicy < MemPolicyBoundary);
              if ((UINTN) LegacyMemoryPolicy >= MemPolicyBoundary) {
                return EFI_OUT_OF_RESOURCES;
              }
            } else {
              UpdateMemoryDesc (&MemoryPolicy, MemoryPolicyCount, MemoryAttr, PageTableBaseAddress, 0x1000ull);
              ASSERT ((UINTN) MemoryPolicy < MemPolicyBoundary);
              if ((UINTN) MemoryPolicy >= MemPolicyBoundary) {
                return EFI_OUT_OF_RESOURCES;
              }
            }
          } //for (PteIndex = 0; PteIndex < 512; PteIndex++)
        } // for (PdeIndex = 0; PdeIndex < 512; PdeIndex++)
      }
    }
  }
  DEBUG ((DEBUG_INFO, "== L5PageNum: %d, L4PageNum: %d, L3PageNum: %d, L2PageNum: %d, L1PageNum: %d ==\n", \
                           L5PageNum, L4PageNum, L3PageNum, L2PageNum, L1PageNum ));

  return EFI_SUCCESS;
}
