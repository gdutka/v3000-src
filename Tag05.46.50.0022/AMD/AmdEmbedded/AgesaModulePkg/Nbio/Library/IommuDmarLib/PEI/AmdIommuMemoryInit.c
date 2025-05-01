/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/**
 * @file
 *
 * IOMMU Initialization and memory preparation code
 *
 *
 */

#include "AmdNbioIOMMU.h"
#include <cpuRegisters.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Library/BaseLib.h>

#define FILECODE        NBIO_LIBRARY_IOMMUDMARLIB_PEI_AMDIOMMUMEMORYINIT_FILECODE

/// The structure of the page directory and page translation entries is indicated below
///
/// |DT|->PD32[x]->PTE[x]    // for 32 bit addresses assigned only
///
///     where x = [1,512-1]
///
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock32 = NULL;
IOMMU_DEVICE_TABLE                   *mDeviceTable = NULL;

// Context data per IOMMU/NBIO. Eventually move to 2d array of [socket,nbio] for more precise cache invalidation
IOMMU_CONTEXT_DATA                   mIommuContextData[IOMMU_CONTEXT_DATA_ENTRIES];

EFI_STATUS
EFIAPI
IommuEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR   mIommuEndofPeiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  IommuEndOfPei
};

/**
  Flush page from IOMMU hardware so the latest data is cached instead of stale

  @param[in]  Base              The base address of memory to be flushed.
  @param[in]  Size              The size of memory in bytes to be flushed.
**/
VOID
FlushPageTableMemory (
  IN UINTN  Base,
  IN UINTN  Size
  )
{
  WriteBackDataCacheRange ((VOID *)Base, Size);
}

/**
  Allocate pages that have been zeroed out first

  @param PagesToAllocate      Number of pages to allocate

  @return void*               Pointer to allocated memory. Returns NULL if allocation failed
**/
VOID *
EFIAPI
AllocateZeroedPages (
  IN UINTN  PagesToAllocate
  )
{
  VOID *AddressPointer;

  ASSERT(PagesToAllocate > 0);

  AddressPointer = AllocatePages (PagesToAllocate);
  if (AddressPointer == NULL) {
    return NULL;
  }
  ZeroMem (AddressPointer, EFI_PAGES_TO_SIZE(PagesToAllocate));
  return AddressPointer;
}

/**
  Get the pages available at a current directory level

  @param Level     Entry Level

  @return UINT32   Pages Available
**/
UINT32
GetMaxPagesAvailablePerLevel (
  UINTN     Level
  )
{
  UINT32   TotalPagesAvailable;

  switch (Level) {
    case 1:
      TotalPagesAvailable = LEVEL1_PAGES_AVAILABLE;
      break;
    case 2:
      TotalPagesAvailable = LEVEL2_PAGES_AVAILABLE;
      break;
    case 3:
      TotalPagesAvailable = LEVEL3_PAGES_AVAILABLE;
      break;
    default:
      ASSERT(Level <= 3);
      TotalPagesAvailable = 0;
  }
  return TotalPagesAvailable;
}

/**
  Determines if a given number of 4k pages will fit within a page directory. If it can it returns
  the parameters needed to call the allocation function.

  @param PageDirectory             Pointer to the page directory entry to check
  @param DirectoryLevel            Level of page directory (from DTE or other PDE)
  @param PagesNeeded               Number of pages needed
  @param FreeEntryIfFound          Pointer to free translation entry
  @param IndexOfFreeEntryIfFound   Index of free entry within translation block

  @return BOOLEAN
**/
BOOLEAN
WillSomethingFitInThisDirectory (
  IN   IOMMU_PAGE_DIRECTORY_ENTRY    *PageDirectory,
  IN   UINTN                         DirectoryLevel,
  IN   UINTN                         PagesNeeded,
  OUT  IOMMU_PAGE_TRANSLATION_ENTRY  **FreeEntryIfFound,
  OUT  UINTN                         *IndexOfFreeEntryIfFound
  )
{
  IOMMU_PAGE_TRANSLATION_ENTRY        *TranslationEntryBlockStart;
  IOMMU_PAGE_TRANSLATION_ENTRY        *StartOfLastFreeEntryChunk;
  UINTN   PagesPerTranslationEntry;
  UINTN   FreeEntriesInThisChunk;
  UINTN   StartOfLastFreeEntryChunkIndex;
  UINTN   PageTranslationEntryCounter;
  UINTN   DirectoryNextLevel;
  UINTN   MaxPagesInThisDirectory;

  ASSERT(PageDirectory != NULL);
  ASSERT((DirectoryLevel < DXE_FIRST_PAGE_DIR_LEVEL64) && (DirectoryLevel > 0));
  ASSERT(PagesNeeded > 0);

  *FreeEntryIfFound = NULL;
  StartOfLastFreeEntryChunk = NULL;
  *IndexOfFreeEntryIfFound = 0;
  PageTranslationEntryCounter = 0;
  StartOfLastFreeEntryChunkIndex = 0;
  FreeEntriesInThisChunk = 0;
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Entry \n", __FUNCTION__);

  // Get total pages available for empty allocated directory
  // If not enough pages at this step its physically impossible to fulfill the request.
  if (PagesNeeded > GetMaxPagesAvailablePerLevel(DirectoryLevel)) {
    ASSERT(PagesNeeded > GetMaxPagesAvailablePerLevel(DirectoryLevel));
    return FALSE;
  }
  // See if its practical to use this directory based on allocation block size
  DirectoryNextLevel = (UINTN)PageDirectory->Field.NextLevel;
  MaxPagesInThisDirectory = (UINTN)GetMaxPagesAvailablePerLevel (DirectoryNextLevel);
  PagesPerTranslationEntry = MaxPagesInThisDirectory/LEVEL1_PAGES_AVAILABLE;
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
                   "%a Pages needed %d, dir level %d pages per entry %d . Total pages in this directory level %d\n",
                   __FUNCTION__,
                   PagesNeeded,
                   DirectoryNextLevel,
                   PagesPerTranslationEntry,
                   MaxPagesInThisDirectory
                   );

  if (PagesNeeded > MaxPagesInThisDirectory) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
          "%a Will not fit because pages needed are greater than are possible with this directory\n", __FUNCTION__);
    return FALSE;
  }

  // NOTE FOR THE FUTURE:
  // If we ever have problems running out of allocation entries we could add a check here to return false
  // if the page size wastes too much space at this level. This would allow of preservation of some address space
  // for larger allocations and keep the small ones in neighboring directories.

  // Get base for block of page translations
  TranslationEntryBlockStart = (IOMMU_PAGE_TRANSLATION_ENTRY *)SHIFT_LEFT_POINTER_ADDR(
                                                        PageDirectory->Field.NextAddress, SHIFT_4K_BOUNDARY);

  // Now see if there is a contiguous chunk available.
  for (PageTranslationEntryCounter = 0; PageTranslationEntryCounter < TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE;
                                     PageTranslationEntryCounter++ ) {
    if(TranslationEntryBlockStart[PageTranslationEntryCounter].Field.Pr == 0) {
      // Mark this as the beginning of a free region. If we want to use it later we have a pointer to the start of it
      if (FreeEntriesInThisChunk == 0) {
        IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a First free entry we have found, beginning count \n", __FUNCTION__);
        StartOfLastFreeEntryChunk = &(TranslationEntryBlockStart[PageTranslationEntryCounter]);
        StartOfLastFreeEntryChunkIndex = PageTranslationEntryCounter;
      }
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found a free entry at index %d \n",
                                                    __FUNCTION__, PageTranslationEntryCounter);
      FreeEntriesInThisChunk++;
      // If we have enough free entries that add up to enough pages
      //return true with the information needed to allocate this chunk
      if (MultU64x64 (FreeEntriesInThisChunk, PagesPerTranslationEntry) >= PagesNeeded) {
        *FreeEntryIfFound = StartOfLastFreeEntryChunk;
        *IndexOfFreeEntryIfFound = StartOfLastFreeEntryChunkIndex;
        IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
                     "%a Found block big enough. Memory Location 0x%x start index %d end index %d \n", __FUNCTION__,
                         *FreeEntryIfFound,
                         StartOfLastFreeEntryChunkIndex,
                         PageTranslationEntryCounter
                         );
        return TRUE;
      }
    }
    else {
      // Present entry was detected so reset free entry counter
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
         "%a Found occupied entry at %d so resetting chunk counter\n", __FUNCTION__, PageTranslationEntryCounter);
      FreeEntriesInThisChunk = 0;
    }
  }
  return FALSE;
}

/**
  Assigns translation entries. This function just calculates how many entries are needed and truncates the last
  page segment if necessary. Does not check for entries being available,
  that already should be done by calling function.

  @param HostBuffer                 Pointer to start of buffer translation entries should point to
  @param PagesNeeded                Length of the buffer in pages
  @param FirstEntry                 Pointer to the first (free) entry
  @param DirectoryEntryLevel        The directory level above these translation entries=
  @param ReadPerm                   Read permission to set in translation entries
  @param WritePerm                  Write permission to set in translation entries
**/
VOID
AssignTranslationEntries (
  IN VOID                          *HostBuffer,
  IN UINTN                         PagesNeeded,
  IN IOMMU_PAGE_TRANSLATION_ENTRY  **FirstEntry,
  IN UINTN                         DirectoryEntryLevel,
  IN BOOLEAN                       ReadPerm,
  IN BOOLEAN                       WritePerm
  )
{
  UINTN   PagesPerEntry;
  UINTN   EntriesCompleted;
  UINTN   TranslationEntriesNeeded;

  ASSERT(HostBuffer != NULL);
  ASSERT(PagesNeeded > 0);
  ASSERT(FirstEntry != NULL && *FirstEntry != NULL);
  ASSERT(DirectoryEntryLevel > 0);

  PagesPerEntry = GetMaxPagesAvailablePerLevel(DirectoryEntryLevel)/LEVEL1_PAGES_AVAILABLE;

  TranslationEntriesNeeded = 0;
  if (PagesPerEntry != 0) {
    TranslationEntriesNeeded = PagesNeeded/PagesPerEntry;
    if ((PagesNeeded % PagesPerEntry) > 0) {
      TranslationEntriesNeeded++;
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
          "%a Will need remainder zero extend page translation entry \n", __FUNCTION__);
    }
  }

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a PagesPerEntry %d. Pages requested %d so translation entries needed %d\n",
                   __FUNCTION__,
                   PagesPerEntry,
                   PagesNeeded,
                   TranslationEntriesNeeded
                   );
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Start translation entry at location 0x%x \n", __FUNCTION__, *FirstEntry);

  // All translation entries have a value of either 0 (use whole page) or 7 (use whole page up to zero fill)
  for (EntriesCompleted = 0; EntriesCompleted < TranslationEntriesNeeded; EntriesCompleted++) {
    ZeroMem(&((*FirstEntry)[EntriesCompleted]), sizeof(IOMMU_PAGE_TRANSLATION_ENTRY));
    (*FirstEntry)[EntriesCompleted].Field.PageAddress = SHIFT_RIGHT_POINTER_ADDR(HostBuffer, SHIFT_4K_BOUNDARY)
                                                             + MultU64x64 (PagesPerEntry, EntriesCompleted);
    (*FirstEntry)[EntriesCompleted].Field.Pr = 1;
    (*FirstEntry)[EntriesCompleted].Field.NextLevel = 0;
    (*FirstEntry)[EntriesCompleted].Field.Iw = WritePerm;
    (*FirstEntry)[EntriesCompleted].Field.Ir = ReadPerm;
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
            "%a Physical Page Address Pointed To 0x%x XXX, index %d\n",
            __FUNCTION__, (*FirstEntry)[EntriesCompleted].Field.PageAddress, EntriesCompleted);
  }
  // Truncate the last signifiant level of pages
  //if remainder was present to prevent device from accessing memory not allocated to it
  if (PagesPerEntry != 0) {
    if ((PagesNeeded%PagesPerEntry) > 0) {
        // TODO fix this once IOMMU works well, cheat for now.
        //(*FirstEntry)[EntriesCompleted].Field.NextLevel = PAGE_LEVEL_ZERO_EXTEND;
        (*FirstEntry)[EntriesCompleted].Field.NextLevel = 0;
    }
  }
}

/**
  Create new directory based on amount of pages needed

  @param PageDirectory    Pointer to page directory to allocate
  @param Pages            Number of pages needed to index. Function will dynamically decide translation level

  @return BOOLEAN         Always returns TRUE
**/
BOOLEAN
AllocateAndAssignDirectoryTranslation (
  IN IOMMU_PAGE_DIRECTORY_ENTRY          *PageDirectoryEntry,
  IN UINTN                               Pages
  )
{
  UINTN                               NewLevel;
  IOMMU_PAGE_TRANSLATION_ENTRY        *TranslationEntryBlock;

  ASSERT(PageDirectoryEntry != NULL);
  ASSERT(Pages > 0);

  // Figure out the optimal level for the number of pages we have either 1, 2 or 3.
  NewLevel = 1;
  if (Pages > LEVEL1_PAGES_AVAILABLE) {
    NewLevel = 2;
  }
  if (Pages > LEVEL2_PAGES_AVAILABLE) {
    NewLevel = 3;
  }
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picking NewLevel to be %d \n", __FUNCTION__, NewLevel);

  // We can handle this case later if the caller asks for more than ~550GB.
  ASSERT(Pages < LEVEL3_PAGES_AVAILABLE);

  // Assign page for translation entry block. Then copy address into directory address. Set as read and write.
  // Translation entries are to ultimately set permissions for any devices
  TranslationEntryBlock = (IOMMU_PAGE_TRANSLATION_ENTRY *)((UINTN)AllocateZeroedPages
                                                        (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_TRANSLATION_ENTRY))));
  PageDirectoryEntry->Field.NextAddress = SHIFT_RIGHT_POINTER_ADDR(TranslationEntryBlock, SHIFT_4K_BOUNDARY);
  PageDirectoryEntry->Field.Pr = 1;
  PageDirectoryEntry->Field.Ir = 1;
  PageDirectoryEntry->Field.Iw = 1;
  // This field determines how large the last level "pages" are
  PageDirectoryEntry->Field.NextLevel = NewLevel;

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
           "%a Translation block assigned at 0x%x to directory 0x%x\n",
                      __FUNCTION__, TranslationEntryBlock, PageDirectoryEntry);

  // Clear cache
  FlushPageTableMemory ((UINTN)PageDirectoryEntry, sizeof (IOMMU_PAGE_DIRECTORY_ENTRY));

  return TRUE;
}

/**
  Find page translation level. Should be only 1, 2 or 3 with the assignment scheme we are using

  @param DeviceBufferAddress    Device address.

  @return UINTN                 Page Translation Index
**/
UINTN
GetPageTranslationIndex (
   IN   IOMMU_DEVICE_BUFFER_ADDRESS         DeviceBufferAddress,
   IN   UINTN                               TranslationLevel
  )
{

  ASSERT(TranslationLevel != 0);

  if (TranslationLevel == 3) {
    return (UINTN)DeviceBufferAddress.Field.Level3;
  }
  if (TranslationLevel == 2) {
    return (UINTN)DeviceBufferAddress.Field.Level2;
  }
  if (TranslationLevel == 1) {
    return (UINTN)DeviceBufferAddress.Field.Level1;
  }

  // No levels specified which means that we are dealing with 0,0,0.
  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Could not find useful index at 0,0,0. Returning 0 index \n", __FUNCTION__);
  return 0;
}

/**
  Sets the specified attributes of a device address or frees the entries associated with that address for disposal

  @param DeviceAddress        Virtual device address that has previously been assigned to this device.
  @param BufferSizeInPages    Size of the buffer in pages.
  @param SetAttributesOnly    If true only read/write bits are set after table walk. 
                               If false the translation entries are freed.
  @param ReadPermissions      Only active if SetAttributesOnly is TRUE. 
                              This is passed on to the page translation entries.
  @param WritePermissions     Only active if SetAttributesOnly is TRUE. 
                              This is passed on to the page translation entries.

  @return BOOLEAN             Returns TRUE if successful. 
                              False if a problem was encountered and request could not be completed
**/
BOOLEAN
SetAttributesOrFreeDeviceAddress (
  IN UINT32           DeviceAddress,
  IN UINTN            BufferSizeInPages,
  IN BOOLEAN          SetAttributes,
  IN BOOLEAN          ReadPermissions,
  IN BOOLEAN          WritePermissions
  )
{
  IOMMU_DEVICE_BUFFER_ADDRESS         FinalAddress;
  IOMMU_PAGE_DIRECTORY_ENTRY          *PageDirectoryEntry;
  IOMMU_PAGE_TRANSLATION_ENTRY        *PageTranslationBlock, *PageTranslationEntry;
  UINTN                               PageTranslationLevel, PageTranslationIndex;
  UINTN                               EntriesToModify, PagesPerEntry, Counter;
  UINTN                               PageLevel;

  EntriesToModify = 0;
  PagesPerEntry = 0;
  PageTranslationIndex = 0;
  PageTranslationLevel = 0;

  FinalAddress.Value = DeviceAddress;

  // FIRST_PAGE_DIR_LEVEL32
  PageLevel = (UINTN) FinalAddress.Field.Level2;
  PageDirectoryEntry =  &(mPageDirectoryBlock32[PageLevel]);

  // Calculate translation level and then retrieve index, then find the device's page translation entry
  PageTranslationBlock = (IOMMU_PAGE_TRANSLATION_ENTRY *)SHIFT_LEFT_POINTER_ADDR(PageDirectoryEntry->Field.NextAddress,
                                                                                     SHIFT_4K_BOUNDARY);
  PageTranslationLevel = (UINTN) PageDirectoryEntry->Field.NextLevel;
  PageTranslationIndex = GetPageTranslationIndex (FinalAddress, PageTranslationLevel);

  PageTranslationEntry = &(PageTranslationBlock[PageTranslationIndex]);

  IDS_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL, "%a Calculated Page Directory 0x%x and Page Translation 0x%x . Level of translation is %d\n",
    __FUNCTION__,
    PageDirectoryEntry,
    PageTranslationEntry,
    PageTranslationLevel
    );

  // Get pages available at this level to determine how many translation entries we should process (EntriesToModify)
  PagesPerEntry = GetMaxPagesAvailablePerLevel (PageTranslationLevel)/LEVEL1_PAGES_AVAILABLE;
  ASSERT(PagesPerEntry > 0);
  if (PagesPerEntry != 0) {
    EntriesToModify = BufferSizeInPages/PagesPerEntry;
    if ((BufferSizeInPages % PagesPerEntry) > 0) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Will need remainder zero extend page translation entry \n", __FUNCTION__);
      EntriesToModify++;
    }
  }

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Modifying %d entries of %d pages each\n",
                                              __FUNCTION__, EntriesToModify, PagesPerEntry);

  // Either clear out the whole block (if freeing) or set specified permissions (if assign))
  for (Counter = 0; Counter < EntriesToModify; Counter++) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Modifying PTE at 0x%x that is translating real memory address 0x%x\n",
                     __FUNCTION__,
                     &(PageTranslationEntry[Counter]),
                     SHIFT_LEFT_POINTER_ADDR(PageTranslationEntry[Counter].Field.PageAddress, SHIFT_4K_BOUNDARY)
                     );
    if (SetAttributes) {
      // Assign virtual address
      PageTranslationEntry[Counter].Field.Ir = ReadPermissions;
      PageTranslationEntry[Counter].Field.Iw = WritePermissions;
      // Present bit should already have been hit
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Set %d / %d , Read %d Write %d\n",
                                __FUNCTION__, Counter + 1, EntriesToModify, ReadPermissions, WritePermissions);
    }
    else {
      // Free entry
      ZeroMem(&(PageTranslationEntry[Counter]), sizeof(IOMMU_PAGE_TRANSLATION_ENTRY));
       IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Cleared %d / %d. \n", __FUNCTION__, Counter + 1, EntriesToModify);
    }

  }

  // Clear cache
  FlushPageTableMemory ((UINTN)PageTranslationEntry, (UINTN) (MultU64x64 (sizeof (IOMMU_PAGE_TRANSLATION_ENTRY),
                                                                                           EntriesToModify)));
  SendInvdAllCommand();

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Exit \n", __FUNCTION__);
  return TRUE;
}

/**
  Assign a virtual address for a device's DMA memory buffer for usage with IOMMU.

  @param PhysicalBuffer               Pointer to beginning of physical address. Should be 4k aligned
  @param PhysicalBufferSizeInPages    Size of buffer in terms of 4k pages

  @return UINT32                      Device address to hand to device that should be translatable by the IOMMU
**/
UINT32
AssignDeviceAddress (
  IN VOID             *HostBuffer,
  IN UINTN            PhysicalBufferSizeInPages
  )
{
  UINTN                               PageDirectoryEntryCounter;
  IOMMU_PAGE_DIRECTORY_ENTRY          *PageDirectoryBlock;
  IOMMU_PAGE_TRANSLATION_ENTRY        *TranslationEntry;
  UINTN                               TranslationBlockIndex;
  BOOLEAN                             WillItFit;
  IOMMU_DEVICE_BUFFER_ADDRESS         FinalAddress;

  ASSERT(HostBuffer != NULL);
  ASSERT(PhysicalBufferSizeInPages > 0);

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Entry. Called with buffer address 0x%x and %d pages\n",
                                              __FUNCTION__, (UINT32) HostBuffer, PhysicalBufferSizeInPages);

  IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Must allocate address below 4G\n", __FUNCTION__);
  PageDirectoryBlock = mPageDirectoryBlock32;

  TranslationBlockIndex = 0;

  // Find a free directory entry in the page block or one that will fit our requirement
  for (PageDirectoryEntryCounter = 1; PageDirectoryEntryCounter < TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE;
                                                                       PageDirectoryEntryCounter++ ) {
    // If entry is not present, allocate one to fit our needs. Otherwise attempt to use the one already here.
    if(PageDirectoryBlock[PageDirectoryEntryCounter].Field.Pr == 0) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found free directory entry at index %d. Allocating for use.\n",
                                                      __FUNCTION__, PageDirectoryEntryCounter);
      AllocateAndAssignDirectoryTranslation (
        &(PageDirectoryBlock[PageDirectoryEntryCounter]),
        PhysicalBufferSizeInPages
        );
    }
    else {
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Checking directory %d to see if there is room\n",
                                       __FUNCTION__, PageDirectoryEntryCounter);
    }
    WillItFit = WillSomethingFitInThisDirectory (
                  &(PageDirectoryBlock[PageDirectoryEntryCounter]),
                  (UINTN) PageDirectoryBlock[PageDirectoryEntryCounter].Field.NextLevel,
                  PhysicalBufferSizeInPages,
                  &TranslationEntry,
                  &TranslationBlockIndex
                  );
    if (WillItFit) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found directory entry with enough space at %d\n",
                                           __FUNCTION__, PageDirectoryEntryCounter);
      break;
    }
    else {
      IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Not enough space in the directory %d\n",
                                          __FUNCTION__, PageDirectoryEntryCounter);
    }
  }
  // We should never realistically overflow unless someone is trying to make us do so, return just in case...
  ASSERT(PageDirectoryEntryCounter < TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE);
  if (PageDirectoryEntryCounter >= TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL,
                   "%a Error finding directory space. Something went wrong or directory is full\n", __FUNCTION__);
    return 0;
  }

  AssignTranslationEntries (
    HostBuffer,
    PhysicalBufferSizeInPages,
    &TranslationEntry,
    (UINTN) PageDirectoryBlock[PageDirectoryEntryCounter].Field.NextLevel,
    FALSE,
    FALSE
    );

  // Now, finally calculate address to hand to device
  // First index into level 4
  FinalAddress.Value = 0;
  // FIRST_PAGE_DIR_LEVEL32
  FinalAddress.Field.Level2 = PageDirectoryEntryCounter;

  // Then fill in whatever the next level ended up being
  switch (PageDirectoryBlock[PageDirectoryEntryCounter].Field.NextLevel) {
    case 1:
      FinalAddress.Field.Level1 = TranslationBlockIndex;
      break;
    case 2:
      FinalAddress.Field.Level2 = TranslationBlockIndex;
      break;
    case 3:
      FinalAddress.Field.Level3 = TranslationBlockIndex;
      break;
    default:
      ASSERT(PageDirectoryBlock[PageDirectoryEntryCounter].Field.NextLevel < DXE_FIRST_PAGE_DIR_LEVEL64);
      return 0;
  }

  IDS_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL, "%a Generated device address: Level 1, 2, 3, 4 are %d %d %d %d Whole Address 0x%x\n",
    __FUNCTION__,
    FinalAddress.Field.Level1,
    FinalAddress.Field.Level2,
    FinalAddress.Field.Level3,
    FinalAddress.Field.Level4,
    FinalAddress.Value
    );

  // Try to flush cache along pathway of translation, then make sure DTE is valid
  FlushPageTableMemory ((UINTN) (&PageDirectoryBlock[PageDirectoryEntryCounter]), sizeof(IOMMU_PDPT_ENTRY));
  FlushPageTableMemory ((UINTN) TranslationEntry, sizeof(IOMMU_PDPT_ENTRY));
  SendInvdAllCommand();

  return (UINT32) FinalAddress.Value;
}

/**
  Dump the contents of a device table entry, if you are curious

  @param DeviceTableEntry  Pointer to entry to dump
**/
VOID
DumpDTE (
  IOMMU_DEVICE_TABLE                 *DeviceTableEntry
  )
{
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL,
                   "%a Device table key bits: V %d Tv %d Mode %d HostPageTablePtr 0x%x 000 Domain 0x%x Ir %d Iw %d\n",
                  __FUNCTION__,
                  DeviceTableEntry->Field.V,
                  DeviceTableEntry->Field.Tv,
                  DeviceTableEntry->Field.Mode,
                  DeviceTableEntry->Field.HostPageTablePtr,
                  DeviceTableEntry->Field.DomainId,
                  DeviceTableEntry->Field.Ir,
                  DeviceTableEntry->Field.Iw
                  );
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword0 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_0_31);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword1 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_32_63);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword2 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_64_95);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword3 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_96_127);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword4 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_128_159);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword5 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_160_191);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword6 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_192_223);
  IDS_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword7 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_224_255);
}

/**
  Run this on new Device Tables

  @param TableToInitialize      Pointer to device table
  @param SizeOfTable            Number representing the size of the table
**/
VOID
InitializeDeviceTable (
  IOMMU_DEVICE_TABLE                 *TableToInitialize,
  UINTN                               SizeOfTable
  )
{
  UINTN   Counter;

  ASSERT(TableToInitialize != NULL);

  // By default Guest Translation is disabled and we will leave it this way.
  //Host->Device translation is all that is needed for UEFI
  for (Counter = 0; Counter < SizeOfTable; Counter++) {
    TableToInitialize[Counter].Field.V = 1;
    TableToInitialize[Counter].Field.Tv = 1;
    // Same domain ID because we share page tables for now among all 64 bit devices
    TableToInitialize[Counter].Field.DomainId = DOMAIN_ID32;
    // Start with Level 4 translation: provides a 48 bit address space
    // 32 bit assignments will skip translating the first directory level
    //because page directory will be indexed by entry 0 of the 64 bit level
    TableToInitialize[Counter].Field.Mode = PEI_FIRST_PAGE_DIR_LEVEL32;
    // Read and write permissions. TODO: set these on demand only later on
    TableToInitialize[Counter].Field.Ir = 1;
    TableToInitialize[Counter].Field.Iw = 1;
    // Point to page directory block
    TableToInitialize[Counter].Field.HostPageTablePtr = SHIFT_RIGHT_POINTER_ADDR(mPageDirectoryBlock32,
                                                                                    SHIFT_4K_BOUNDARY);
    // Interrupts and guest translation are disabled by default (zerod memory was used for initialization)
    if (Counter == 1) {
      DumpDTE(&TableToInitialize[Counter]);
    }
  }
}

/**
  Dump the event log to the console. Nothing will be printed if there are no entries.

**/
VOID
DumpEventLog (
  VOID
  )
{
  // Head is set by software
  // Tail is set by IOMMU
  IOMMU_EVENT_LOG_ENTRY        *Head;
  IOMMU_EVENT_LOG_ENTRY        *Tail;
  IOMMU_EVENT_LOG_CODE1_ENTRY  *Code1;
  IOMMU_EVENT_LOG_CODE2_ENTRY  *Code2;
  UINTN                        Count;

  // Dump all IOMMU
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }

    // If present then dump any entries found
    Head = (IOMMU_EVENT_LOG_ENTRY *)
      (UINTN) (mIommuContextData[Count].EventLogHead->Field.Value +
                   (UINT64)SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].EventLogBaseAddressStruct->Field.EVENT_BASE
                   ,SHIFT_4K_BOUNDARY));
    Tail = (IOMMU_EVENT_LOG_ENTRY *)
                   (UINTN) (mIommuContextData[Count].EventLogTail->Field.Value +
                   (UINT64)SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].EventLogBaseAddressStruct->Field.EVENT_BASE
                   ,SHIFT_4K_BOUNDARY));

    // NOTE: Simplify this for now. Run through it once (POST is brief).
    // Circularize at a later time if we really have that many entries
    IDS_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Head 0x%llx Tail 0x%llx \n", __FUNCTION__, Head, Tail);
    if (Head == Tail) {
      return;
    }
    ASSERT(Tail >= Head);

    // Entries are typically errors
    while (Tail > Head) {
      IDS_HDT_CONSOLE(
        IOMMU_TRACE_URGENT, "%a IOMMU Event to investigate (probably bad news): Code 0x%x Op1 0x%llx Op2 0x%llx\n",
        __FUNCTION__,
        Head->EventCode,
        Head->FirstEventCodeOperand1,
        Head->SecondEventCodeOperand
        );

      // Decode the error if it is a common one
      if (Head->EventCode == 1) {
        Code1 = (IOMMU_EVENT_LOG_CODE1_ENTRY *)Head;
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a Page fault occurred with DeviceID x\n",
          __FUNCTION__,
          Code1->Fields.DeviceId
          );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
           "%a PASID %d %d \n",
           __FUNCTION__,
           Code1->Fields.PASID_9_16,
           Code1->Fields.PASID_15_0
           );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a GN %d I %d RW %d RZ %d TR %d \n",
           __FUNCTION__,
          Code1->Fields.GN,
          Code1->Fields.I,
          Code1->Fields.RW,
          Code1->Fields.RZ,
          Code1->Fields.TR
          );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a Address Low 0x%x Address High 0x%x\n",
          __FUNCTION__,
          Code1->Fields.AddressLow,
          Code1->Fields.AddressHigh
          );
      }
      if (Head->EventCode == 2) {
        Code2 = (IOMMU_EVENT_LOG_CODE2_ENTRY *)Head;
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a Issue with device table encoding on DeviceId 0x%x\n",
          __FUNCTION__,
          Code2->Fields.DeviceId
          );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
           "%a PASID 0x %x %x \n",
           __FUNCTION__,
           Code2->Fields.PASID_9_16,
           Code2->Fields.PASID_15_0
           );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a GN %d I %d RW %d RZ %d TR %d \n",
           __FUNCTION__,
          Code2->Fields.GN,
          Code2->Fields.I,
          Code2->Fields.RW,
          Code2->Fields.RZ,
          Code2->Fields.TR
          );
        IDS_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a Address Low 0x%x Address High 0x%x\n",
          __FUNCTION__,
          Code2->Fields.AddressLow,
          Code2->Fields.AddressHigh
          );
      }
      // Advance head pointer
      Head++;
    }
  }

}

/**
  Send IOMMU command buffer a command. For now just INVD but once we enhance caching strategy it will be other commands

**/
VOID
SendInvdAllCommand (
  VOID
  )
{
  // Head is set by IOMMU
  // Tail is set by software
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      InvdAll;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandTail;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandBase;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandEnd;
  UINTN                                    Count;

  ZeroMem(&InvdAll, sizeof(IOMMU_INVALIDATE_ALL_COMMAND_STRUCT));
  InvdAll.Command = IOMMU_INVALIDATE_ALL_COMMAND;

  // Blast to all IOMMU. Eventually might be a good idea to only INVD the IOMMU that has a changed entry
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }
    // If present then fill in entry and advance log pointer
    CommandBase = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)
      (UINTN) SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].CommandBufferAddressStruct->Field.COM_BASE,
                                                             SHIFT_4K_BOUNDARY);
    CommandTail = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)
      ((UINTN) CommandBase + mIommuContextData[Count].CommandBufferTail->Field.Value);
    CommandEnd  = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)
      ((UINTN) CommandBase + MultU64x64 (IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE, IOMMU_COMMAND_BUFFER_ENTRIES));

    *CommandTail = InvdAll;

    if (mIommuContextData[Count].CommandBufferTail->Field.Value >= MultU64x64 (IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE,
                                    (IOMMU_COMMAND_BUFFER_ENTRIES - 1))) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Flipping command buffer 0x%x IOMMU %d\n",
                                        __FUNCTION__, mIommuContextData[Count].CommandBufferTail->Field.Value, Count);
      mIommuContextData[Count].CommandBufferTail->Field.Value = 0;
    }
    else {
      mIommuContextData[Count].CommandBufferTail->Field.Value = mIommuContextData[Count].CommandBufferTail->Field.Value
                                                                              + IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE;
    }
  }
}

/**
  Each NBIO has one IOMMU upstream.

  @param GnbHandle

  @return BOOLEAN
**/
BOOLEAN
SetupIommuOnEachNbio (
  IN GNB_HANDLE          *GnbHandle
  )
{
  UINTN                               Pages;
  VOID                                *MemoryAllocated;
  PCI_ADDR                            GnbIommuPciAddress;
  UINT32                              BarReadValue, DeviceTableEntries;
  UINT32                              PciBusNumber;
  VOID                                *IommuMmioBaseAddressRegister;
  IOMMU_MMIO_DEVTBL_BASE_STRUCT       *DevtableAddressStruct;
  IOMMU_MMIO_CMD_BASE_STRUCT          *CommandBufferAddressStruct;
  IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *CommandBufferHead;
  IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *CommandBufferTail;
  IOMMU_MMIO_EVENT_BASE_STRUCT        *EventBaseAddressStruct;
  IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *EventLogHead;
  IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *EventLogTail;
  IOMMU_MMIO_CNTRL_STRUCT             *IommuControlRegister;
  MMIO_0x30                           *SupportedRegisters;
  CONST EFI_PEI_SERVICES              **PeiServices;
  UINTN                               NbiosInitialized;
  MMIO_CFG_BASE_MSR                   MmioCfgBase;

  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Enter\n", __FUNCTION__);
  ASSERT(GnbHandle != NULL);
  NbiosInitialized = 0;
  IommuMmioBaseAddressRegister = NULL;

  ZeroMem (mIommuContextData, (UINTN) MultU64x64 (sizeof (IOMMU_CONTEXT_DATA), IOMMU_CONTEXT_DATA_ENTRIES));

  // First thing is to allocate master page directory. We are using master page directory because we do not
  // know the PCIe address of the device consuming the memory until both allocate and map have been called.
  // Virtual device addresses will be generated when Map() is called and assigned to devices after SetAttributes()
  mPageDirectoryBlock32 =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINTN)(AllocateZeroedPages
                                              (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock32 == NULL) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT,
                              "%a FATAL error cannot allocate 32 bit page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT,
                        "%a 32 bit directory block allocated at 0x%x\n", __FUNCTION__, (UINT32) mPageDirectoryBlock32);

  // Allocate space for the device table. The device table is allowed to be shared among all IOMMUs
  MmioCfgBase.Value = AsmReadMsr64 (MSR_MMIO_Cfg_Base);
  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  MmioCfgBase.Field.BusRange = 0x%x\n", MmioCfgBase.Field.BusRange);
  PciBusNumber = PCIE_MAX_BUS;
  if ((MmioCfgBase.Field.Enable == 1) && (MmioCfgBase.Field.BusRange >= 1) && (MmioCfgBase.Field.BusRange <= 8)) {
    PciBusNumber = 1 << (MmioCfgBase.Field.BusRange);
  }
  DeviceTableEntries = (UINT32) MultU64x64 (PciBusNumber, MultU64x64 (PCIE_MAX_DEVICES, PCIE_MAX_FUNCTIONS));
  Pages = EFI_SIZE_TO_PAGES ((UINTN) MultU64x64 (sizeof (IOMMU_DEVICE_TABLE), DeviceTableEntries));
  mDeviceTable = AllocateZeroedPages (Pages);
  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Allocated IOMMU device table at 0x%x size in pages is %d\n",
                                                                             __FUNCTION__, mDeviceTable, Pages);
  InitializeDeviceTable (mDeviceTable, DeviceTableEntries);
  if (mDeviceTable == NULL) {
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate device table.\n", __FUNCTION__);
    return EFI_OUT_OF_RESOURCES;
  }

  while (GnbHandle != NULL) {
    // Retrieve PCIe location for this IOMMU to extract BAR location for programming with offsets
    GnbIommuPciAddress = NbioGetHostPciAddress (GnbHandle);
    GnbIommuPciAddress.Address.Function = PCICFG_AMD_IOMMU_FUNCTION;

    GnbLibPciRead (
      GnbIommuPciAddress.AddressValue | PCICFG_AMD_IOMMU_CAP_BASE_LO_OFFSET,
      AccessWidth32,
      &BarReadValue,
      NULL
      );
    if ((BarReadValue & 1) == 0) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a : Hit enable bit for IOMMU BAR 0x%x\n", __FUNCTION__, BarReadValue);
      BarReadValue |= 1;
      GnbLibPciWrite (
          GnbIommuPciAddress.AddressValue | PCICFG_AMD_IOMMU_CAP_BASE_LO_OFFSET,
          AccessWidth32,
          &BarReadValue,
          NULL
        );
    }
    // Mask out enable bit and convert to pointer
    IommuMmioBaseAddressRegister = (VOID *)(BarReadValue & 0xfffffffe);
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a IOMMU Bar 0x%x\n", __FUNCTION__, IommuMmioBaseAddressRegister);
    if ((UINTN)IommuMmioBaseAddressRegister == 0) {
      IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a ERROR IOMMU Bar is not setup! Exiting \n", __FUNCTION__);
      return FALSE;
    }
    IommuControlRegister = (IOMMU_MMIO_CNTRL_STRUCT *)((UINTN)IommuMmioBaseAddressRegister + IOMMU_MMIO_CNTRL_STRUCT_OFFSET);
    IommuControlRegister->Value = 0;

    // Step 1: Setup device table address
    DevtableAddressStruct = (IOMMU_MMIO_DEVTBL_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                IOMMU_MMIO_DEVTBL_BASE_STRUCT_OFFSET);
    DevtableAddressStruct->Field.DEV_TBL_SIZE = IOMMU_MAX_DEVICE_TABLE_ENTRIES - 1;
    DevtableAddressStruct->Field.DEV_TBL_BASE = ((UINTN)mDeviceTable) >> SHIFT_4K_BOUNDARY;
    IDS_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Device table base MMIO register 0x%x, size %d\n",
                        __FUNCTION__, DevtableAddressStruct->Dword.DwordLo, DevtableAddressStruct->Field.DEV_TBL_SIZE);

    // Step 2: Allocate and setup command buffer base address register
    Pages = IOMMU_COMMAND_BUFFER_ENTRIES/IOMMU_COMMAND_BUFFER_ENTRIES_PER_PAGE;
    MemoryAllocated = AllocateZeroedPages (Pages);
    CommandBufferAddressStruct = (IOMMU_MMIO_CMD_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                   IOMMU_MMIO_CMD_BASE_STRUCT_OFFSET);
    CommandBufferAddressStruct->Field.COM_LEN = IOMMU_COMMAND_OR_EVENT_1024_ENTRIES;
    CommandBufferAddressStruct->Field.COM_BASE = ((UINTN) MemoryAllocated) >> SHIFT_4K_BOUNDARY;
    IDS_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Command Base Init 0x%x\n",
                                    __FUNCTION__, CommandBufferAddressStruct->Dword.DwordLo);

    // Step 3: Allocate Command Buffer Head Pointer Register
    // Step 4: Allocate command Buffer Tail Pointer Register
    CommandBufferHead = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                              IOMMU_COMMAND_BUFFER_POINTER_HEAD_STRUCT_OFFSET);
    CommandBufferTail = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                              IOMMU_COMMAND_BUFFER_POINTER_TAIL_STRUCT_OFFSET);
    CommandBufferHead->Value = 0;
    CommandBufferTail->Value = 0;

    // Step 5: Iommu Exclusion Base Register and Iommu Exclusion Range Limit Register
    // No need for this right now

    // Step 6: Event Log base address register and Event Log Pointer Registers (head/tail)
    Pages = IOMMU_EVENT_LOG_ENTRIES/IOMMU_EVENT_LOG_ENTRIES_PER_PAGE;
    MemoryAllocated = AllocateZeroedPages (Pages);
    EventBaseAddressStruct = (IOMMU_MMIO_EVENT_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                      IOMMU_MMIO_EVENT_BASE_STRUCT_OFFSET);
    EventBaseAddressStruct->Field.EVENT_LEN = IOMMU_COMMAND_OR_EVENT_1024_ENTRIES;
    EventBaseAddressStruct->Field.EVENT_BASE = ((UINTN) MemoryAllocated) >> SHIFT_4K_BOUNDARY;
    IDS_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Event Base Init 0x%x\n",
                                                 __FUNCTION__, EventBaseAddressStruct->Dword.DwordLo);

    // Step 7: Allocate Event Log Buffer Head Pointer
    // Step 8: Allocate Event Log Buffer Tail Pointer
    EventLogHead = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINTN)IommuMmioBaseAddressRegister
                                                               + IOMMU_EVENT_LOG_POINTER_HEAD_STRUCT_OFFSET);
    EventLogTail = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINTN)IommuMmioBaseAddressRegister
                                                               + IOMMU_EVENT_LOG_POINTER_TAIL_STRUCT_OFFSET);
    EventLogHead->Value = 0;
    EventLogTail->Value = 0;

    // Enable event log and command buffer
    IommuControlRegister->Field.EVENT_LOG_EN = 1;
    IommuControlRegister->Field.CMD_BUF_EN = 1;
    IommuControlRegister->Field.COHERENT = 1;
    // Last Step: Setup IOMMU control register to begin protection.
    //Device requests for DMA mapping will have to go through our code.
    // Any other permanent config should be done here
    IommuControlRegister->Field.IOMMU_EN = 1;
    IDS_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a IOMMU control register for NBIO %d is initialized to 0x%x\n",
                                           __FUNCTION__, NbiosInitialized, IommuControlRegister->Value);

    // Dump IOMMU Supported registers
    SupportedRegisters = (MMIO_0x30 *)((UINTN)IommuMmioBaseAddressRegister + MMIO_0x30_OFFET);
    IDS_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a IOMMU supported registers are 0x%x\n",
                                            __FUNCTION__, SupportedRegisters->Value);

    // Now fill in context data so that we may access this IOMMU later when allocations are requested
    mIommuContextData[NbiosInitialized].Present = TRUE;
    mIommuContextData[NbiosInitialized].CommandBufferAddressStruct = CommandBufferAddressStruct;
    mIommuContextData[NbiosInitialized].CommandBufferHead = CommandBufferHead;
    mIommuContextData[NbiosInitialized].CommandBufferTail = CommandBufferTail;
    mIommuContextData[NbiosInitialized].EventLogBaseAddressStruct = EventBaseAddressStruct;
    mIommuContextData[NbiosInitialized].EventLogHead = EventLogHead;
    mIommuContextData[NbiosInitialized].EventLogTail = EventLogTail;
    mIommuContextData[NbiosInitialized].IommuControlRegister = IommuControlRegister;

    NbiosInitialized++;
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  // Clear cache, just to be safe
  SendInvdAllCommand();

  // Create event for disabling IOMMU at the end of PEI
  PeiServices = GetPeiServicesTablePointer();
  (**PeiServices).NotifyPpi (PeiServices, &mIommuEndofPeiCallback);

  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Exit\n", __FUNCTION__);
  return TRUE;
}

EFI_STATUS
EFIAPI
IommuEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  UINTN                               Count;
  UINT16                              CommandReg;
  PCI_ADDR                            PciAddress;
  VOID                                *IommuMmioBaseAddressRegister;
  IOMMU_MMIO_CMD_BASE_STRUCT          *CommandBufferAddressStruct;
  IOMMU_MMIO_EVENT_BASE_STRUCT        *EventBaseAddressStruct;
  IOMMU_MMIO_DEVTBL_BASE_STRUCT       *DevtableAddressStruct;

  IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Running\n", __FUNCTION__);

  // Invalidate All
  SendInvdAllCommand ();

  // Enable BUS MASTER for this IOMMU
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0);
  GnbLibPciRead (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);
  // Clear bus master bit in command register
  CommandReg &= ~0x4;
  GnbLibPciWrite (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);

  // Disable all IOMMU at the end of PEI
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }
    IommuMmioBaseAddressRegister = (VOID *) ((UINT8 *)(mIommuContextData[Count].IommuControlRegister) -
                                                             IOMMU_MMIO_CNTRL_STRUCT_OFFSET);
    IDS_HDT_CONSOLE (IOMMU_TRACE_URGENT, "IommuMmioBase = 0x%x and IommuControl = 0x%x\n",
                                IommuMmioBaseAddressRegister, mIommuContextData[Count].IommuControlRegister);

    DumpEventLog();

    mIommuContextData[Count].IommuControlRegister->Value = 0;
    mIommuContextData[Count].IommuControlRegister->Field.COHERENT = 1;
    mIommuContextData[Count].IommuControlRegister->Field.EPH_EN = 1;
    mIommuContextData[Count].IommuControlRegister->Field.Block_StopMark_En = 1;
    mIommuContextData[Count].Present = FALSE;

    mIommuContextData[Count].EventLogHead->Field.Value = 0;
    mIommuContextData[Count].EventLogTail->Field.Value = 0;

    // Clear Event Log Address
    EventBaseAddressStruct = (IOMMU_MMIO_EVENT_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                           IOMMU_MMIO_EVENT_BASE_STRUCT_OFFSET);
    EventBaseAddressStruct->Field.EVENT_LEN = 0x8;
    EventBaseAddressStruct->Field.EVENT_BASE = 0;
    // Clear Command Buffer Address
    CommandBufferAddressStruct = (IOMMU_MMIO_CMD_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                           IOMMU_MMIO_CMD_BASE_STRUCT_OFFSET);
    CommandBufferAddressStruct->Field.COM_LEN = 0x8;
    CommandBufferAddressStruct->Field.COM_BASE = 0;
    // Clear DEVTBL Buffer Address
    DevtableAddressStruct = (IOMMU_MMIO_DEVTBL_BASE_STRUCT *)((UINTN)IommuMmioBaseAddressRegister +
                                                                          IOMMU_MMIO_DEVTBL_BASE_STRUCT_OFFSET);
    DevtableAddressStruct->Field.DEV_TBL_SIZE = 0;
    DevtableAddressStruct->Field.DEV_TBL_BASE = 0;
  }

  return EFI_SUCCESS;
}
