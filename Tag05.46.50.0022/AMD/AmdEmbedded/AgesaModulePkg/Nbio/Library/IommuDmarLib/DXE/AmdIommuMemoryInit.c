/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
#include "AmdNbioIOMMU.h"
#include <cpuRegisters.h>
#include <Library/UefiLib.h>
#include <Protocol/Usb2HostController.h>
#include <Library/BaseLib.h>

#define FILECODE        NBIO_LIBRARY_IOMMUDMARLIB_DXE_AMDIOMMUMEMORYINIT_FILECODE

#define IOMMU_HDT_CONSOLE(f, s, ...)
// To support IOMMU logging use the following instead
// #define IOMMU_HDT_CONSOLE(f, s, ...) IDT_HDT_CONSOLE(f, s, ...)

extern
EFI_GUID  gEfiUsb2HcProtocolGuid;

/// The structure of the page directory and page translation entries is indicated below
///
/// Level4         Level3           Level2     Level1
///
/// |DT|->PD64[0]  ->PD32_2M[0]     ->PD32_4K->PTE[1,511]          // for 32 bit addresses assigned
///                ->PD32_2M[1,511] ->PTE[0,511]                   // Total 32 bit address space is 4k*511 + 2M*512
/// |DT|->PD64[1]  ->PD64_2M[0]     ->PD64_4K->PTE[0,511]          // for 64 bit addresses assigned
///                ->PD64_2M[1,511] ->PTE[0,511]                   // Total 64 bit address space is 4k*512 + 2M*512
///
///  All PCIe devices share the same Device Table structure. We may want to change that at some point for further security.
///  Drivers should not leave memory mapped any longer than they actually need it for DMA.
///
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock64 = NULL;
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock64_2M = NULL;
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock64_4K = NULL;
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock32_2M = NULL;
IOMMU_PAGE_DIRECTORY_ENTRY           *mPageDirectoryBlock32_4K = NULL;
IOMMU_DEVICE_TABLE                   *mDeviceTable = NULL;

// These counters cache the last directory checked for free space. After going through the entire address space
// they are reset. This is done for efficiency
UINTN                                mPageDirectoryBlock64_2M_LastDir = 1;
UINTN                                mPageDirectoryBlock64_4K_LastDir = 0;
UINTN                                mPageDirectoryBlock32_2M_LastDir = 1;
UINTN                                mPageDirectoryBlock32_4K_LastDir = 0;

// Context data per IOMMU/NBIO. Eventually move to 2d array of [socket,nbio] for more precise cache invalidation
IOMMU_CONTEXT_DATA                   mIommuContextData[IOMMU_CONTEXT_DATA_ENTRIES];
SPIN_LOCK_DATA                       mSpinLock;

/**
  set spin lock

  @param[in]  LockData    Spin lock data
**/
EFI_STATUS SpinLockSave (
  IN SPIN_LOCK_DATA *LockData
  )
{
  if (LockData->Lock) {
    return EFI_ACCESS_DENIED;
  }
  LockData->Lock = TRUE;
  LockData->InterruptState = SaveAndDisableInterrupts ();
  return EFI_SUCCESS;
}

/**
  restore spin lock

  @param[in]  LockData    Spin lock data
**/
EFI_STATUS SpinLockRestore (
  IN SPIN_LOCK_DATA *LockData
  )
{
  if (!LockData->Lock) {
    return EFI_NOT_STARTED;
  }
  LockData->Lock = FALSE;
  SetInterruptState (LockData->InterruptState);
  LockData->InterruptState = FALSE;
  return EFI_SUCCESS;
}

VOID
StopUsb (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_USB2_HC_PROTOCOL  *HandleProtocol;
  EFI_HANDLE            HandleList[10];
  UINTN                 BufferSize;
  UINTN                 Index;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT,"%a Entry\n", __FUNCTION__);

  BufferSize = 10 * sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (ByProtocol, &gEfiUsb2HcProtocolGuid, NULL, &BufferSize, HandleList);
  if (EFI_ERROR (Status)) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT,"USB PROTOCOL HANDLES NOT FOUND\n");
    return;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT,"LocateHandle returned %d handles\n", BufferSize / sizeof (EFI_HANDLE));
  for (Index = 0; Index < (BufferSize / sizeof (EFI_HANDLE)); Index++) {
    Status = gBS->HandleProtocol (HandleList[Index], &gEfiUsb2HcProtocolGuid, (VOID **)&HandleProtocol);
    HandleProtocol->SetState (HandleProtocol, EfiUsbHcStateHalt);
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT,"%a Exit\n", __FUNCTION__);
}

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
  Returns the highest populated bit in a number, starting from 0

  @param InputNumber    Number to get highest bit of

  @return UINT32        The highest bit number, 0 based
**/
UINT32
GetHighestBitNumber (
  IN UINT32      InputNumber
  )
{
  UINT32   PagesZeroMask;
  UINT32   ShiftCount;

  ShiftCount = 0;
  PagesZeroMask = 0;

  // Shift left bits until ZeroMask covers entire number
  while ((InputNumber & PagesZeroMask) != InputNumber) {
    PagesZeroMask = PagesZeroMask << 1;
    PagesZeroMask |= 1;

    ShiftCount++;
  }

  return ShiftCount;
}

/**
  Clear out page table cache for a certain group of pages

  @param DeviceAddress Device address issued by IOMMU
  @param Pages         Length of memory after address to clear cache in pages
**/
VOID
InvalidateDeviceAddressMemoryRange (
  IN UINT64      DeviceAddress,
  IN UINT32      Pages
  )
{
  UINT64      DerivedAddress;

  //IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL,"%s Address %lx Pages %d \n", __FUNCTION__, DeviceAddress, Pages);
  if (Pages > 1) {
    // if more than one page, flush everything
    DerivedAddress = IOMMU_INVALID_ALL_PAGES_ADDRESS;
    SendInvdPagesCommand (DerivedAddress, TRUE);
  } else {
    SendInvdPagesCommand (DeviceAddress, FALSE);
  }
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

  @return UINT64   Pages Available
**/
UINT64
GetMaxPagesAvailablePerLevel (
  UINTN     Level
  )
{
  UINT64   TotalPagesAvailable;

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
    case 4:
      TotalPagesAvailable = LEVEL4_PAGES_AVAILABLE;
      break;
    default:
      ASSERT(Level <= 4);
      TotalPagesAvailable = 0;
  }
  return TotalPagesAvailable;
}

/**
  Get alignment of input address and given length. This is important because the IOMMU only works with
   aligned memory addresses. Optional outputs are only populated if their respective pointers are NULL

  @param Address                      Address to evaluate
  @param InNumberOfBytes              Length of memory region in bytes
  @param NewAlignedAddress            Optional output represents the new aligned address that should be used
  @param Alignment                    Optional output represents the alignment that should be used
  @param Numberof4kPagesNeeded        Optional output represents the number of 4k pages ultimately mapped
  @param NumberOfNativePagesNeeded    Optional output represents the number of native pages mapped, size of page is Alignment

  @return BOOLEAN
**/
BOOLEAN
GetAppropriateAlignmentAndSpan (
  IN  UINT64       InAddress,
  IN  UINTN        InNumberOfBytes,
  OUT UINT64       *NewAlignedAddress OPTIONAL,
  OUT UINTN        *Alignment OPTIONAL,
  OUT UINTN        *Numberof4kPagesNeeded OPTIONAL,
  OUT UINTN        *NumberOfNativePagesNeeded OPTIONAL
  )
{
  UINT64   AlignedPageSize;
  UINT64   NativePagesNeeded;
  UINT64   AlignedPageAddress;
  UINT64   WastedSpace;
  UINT64   AvailableSpace;
  UINT64   PagesOfMemory4K;

  PagesOfMemory4K = EFI_SIZE_TO_PAGES(InNumberOfBytes);
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : Input %lx of length %x end %lx\n", __FUNCTION__, InAddress, InNumberOfBytes, InAddress + InNumberOfBytes);

  // We support PTEs in levels 1-2
  ASSERT (PagesOfMemory4K <= LEVEL2_PAGES_AVAILABLE);
  if (PagesOfMemory4K > LEVEL2_PAGES_AVAILABLE) {
    return FALSE;
  }

  // Level 2
  if (PagesOfMemory4K >= LEVEL1_PAGES_AVAILABLE) {
    AlignedPageSize = SIZE_2MB;
    NativePagesNeeded = PagesOfMemory4K/LEVEL1_PAGES_AVAILABLE;
    // Check for overflow
    if (PagesOfMemory4K%LEVEL1_PAGES_AVAILABLE) {
      NativePagesNeeded++;
    }
  }
  else {
    // Level 1
    AlignedPageSize = SIZE_4KB;
    NativePagesNeeded = PagesOfMemory4K;
  }

  // Get what the aligned native page start address
  AlignedPageAddress = InAddress & (~(AlignedPageSize - 1));

  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a : Aligned page address 0x%lx alignment 0x%lx\n", __FUNCTION__, AlignedPageAddress, AlignedPageSize);

  // If the start address is far enough down the aligned page, it will run into a new page
  // This is in addition to the overflow check above, both of these can happen theoretically
  if (AlignedPageAddress != InAddress) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : WARNING physical address value is not aligned for input size.\n", __FUNCTION__);
    ASSERT (AlignedPageAddress < InAddress);
    WastedSpace = InAddress - AlignedPageAddress;
    AvailableSpace = (AlignedPageSize*NativePagesNeeded) - WastedSpace;
    if (AvailableSpace < InNumberOfBytes) {
      NativePagesNeeded++;
    } else {
      // message here maybe?
    }
  }
  PagesOfMemory4K = EFI_SIZE_TO_PAGES(AlignedPageSize*NativePagesNeeded);

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a : Native Page Size %lx Native Pages Needed %ld total size %lx bytes\n", __FUNCTION__, AlignedPageSize, NativePagesNeeded, AlignedPageSize*NativePagesNeeded);

  if (Alignment != NULL) {
    *Alignment = (UINTN)AlignedPageSize;
  }

  if (Numberof4kPagesNeeded != NULL) {
    *Numberof4kPagesNeeded = (UINTN)PagesOfMemory4K;
  }

  if (NumberOfNativePagesNeeded != NULL) {
    *NumberOfNativePagesNeeded = (UINTN)NativePagesNeeded;
  }

  if (NewAlignedAddress != NULL) {
    *NewAlignedAddress = AlignedPageAddress;
  }

  return TRUE;
}
/**
  Determines if a given number of 4k pages will fit within a page directory. If it can it returns the parameters needed to call
  the allocation function.

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
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Entry \n", __FUNCTION__);

  // Get base for block of page translations
  TranslationEntryBlockStart = (IOMMU_PAGE_TRANSLATION_ENTRY *)SHIFT_LEFT_POINTER_ADDR(PageDirectory->Field.NextAddress, SHIFT_4K_BOUNDARY);

  // Check that this is in fact a translation block. Only PTE can have a NextLevel of 0
  if (TranslationEntryBlockStart[0].Field.NextLevel != 0) {
    return FALSE;
  }

  // Get total pages available for empty allocated directory
  // If not enough pages at this step its physically impossible to fulfill the request.
  if (PagesNeeded > GetMaxPagesAvailablePerLevel(DirectoryLevel)) {
    ASSERT(PagesNeeded > GetMaxPagesAvailablePerLevel(DirectoryLevel));
    return FALSE;
  }
  // See if its practical to use this directory based on allocation block size
  DirectoryNextLevel = PageDirectory->Field.NextLevel;
  MaxPagesInThisDirectory = GetMaxPagesAvailablePerLevel (DirectoryNextLevel);
  PagesPerTranslationEntry = MaxPagesInThisDirectory/LEVEL1_PAGES_AVAILABLE;
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Pages needed %d, dir level %d pages per entry %d . Total pages in this directory level %d\n",
                   __FUNCTION__,
                   PagesNeeded,
                   DirectoryNextLevel,
                   PagesPerTranslationEntry,
                   MaxPagesInThisDirectory
                   );

  if (PagesNeeded > MaxPagesInThisDirectory) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Will not fit because pages needed are greater than are possible with this directory\n", __FUNCTION__);
    return FALSE;
  }

  // NOTE FOR THE FUTURE:
  // If we ever have problems running out of allocation entries we could add a check here to return false
  // if the page size wastes too much space at this level. This would allow of preservation of some address space
  // for larger allocations and keep the small ones in neighboring directories.
  if ((PagesNeeded < LEVEL1_PAGES_AVAILABLE) && DirectoryNextLevel == 2) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Rejecting fit at this level because it wastes too much address space\n", __FUNCTION__);
    return FALSE;
  }

  // Now see if there is a contiguous chunk available.
  for (PageTranslationEntryCounter = 0; PageTranslationEntryCounter < TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE; PageTranslationEntryCounter++ ) {
    if(TranslationEntryBlockStart[PageTranslationEntryCounter].Field.Pr == 0) {
      // Mark this as the beginning of a free region. If we want to use it later we have a pointer to the start of it
      if (FreeEntriesInThisChunk == 0) {
        IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a First free entry we have found, beginning count \n", __FUNCTION__);
        StartOfLastFreeEntryChunk = &(TranslationEntryBlockStart[PageTranslationEntryCounter]);
        StartOfLastFreeEntryChunkIndex = PageTranslationEntryCounter;
      }
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Found a free entry at index %d \n", __FUNCTION__, PageTranslationEntryCounter);
      FreeEntriesInThisChunk++;
      // If we have enough free entries that add up to enough pages return true with the information needed to allocate this chunk
      if ((FreeEntriesInThisChunk*PagesPerTranslationEntry) >= PagesNeeded) {
        *FreeEntryIfFound = StartOfLastFreeEntryChunk;
        *IndexOfFreeEntryIfFound = StartOfLastFreeEntryChunkIndex;
        IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found block big enough. Memory Location 0x%x start index %d end index %d \n", __FUNCTION__,
                         *FreeEntryIfFound,
                         StartOfLastFreeEntryChunkIndex,
                         PageTranslationEntryCounter
                         );
        return TRUE;
      }
    }
    else {
      // Present entry was detected so reset free entry counter
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Found occupied entry at %d so resetting chunk counter\n", __FUNCTION__, PageTranslationEntryCounter);
      FreeEntriesInThisChunk = 0;
    }
  }
  return FALSE;
}

/**
  Assigns translation entries. This function just calculates how many entries are needed and truncates the last
  page segment if necessary. Does not check for entries being available, that already should be done by calling function.

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
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Will need remainder translation entry \n", __FUNCTION__);
    }
  }

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a PagesPerEntry %d. Pages requested %d so translation entries needed %d\n",
                   __FUNCTION__,
                   PagesPerEntry,
                   PagesNeeded,
                   TranslationEntriesNeeded
                   );
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Start translation entry at location 0x%x \n", __FUNCTION__, *FirstEntry);

  // All translation entries have a value of either 0 (use whole page) or 7 (use whole page up to zero fill)
  for (EntriesCompleted = 0; EntriesCompleted < TranslationEntriesNeeded; EntriesCompleted++) {
    ZeroMem(&((*FirstEntry)[EntriesCompleted]), sizeof(IOMMU_PAGE_TRANSLATION_ENTRY));
    (*FirstEntry)[EntriesCompleted].Field.PageAddress = SHIFT_RIGHT_POINTER_ADDR(HostBuffer, SHIFT_4K_BOUNDARY) + PagesPerEntry*EntriesCompleted;
    (*FirstEntry)[EntriesCompleted].Field.Pr = 1;
    (*FirstEntry)[EntriesCompleted].Field.NextLevel = 0;
    (*FirstEntry)[EntriesCompleted].Field.Iw = WritePerm;
    (*FirstEntry)[EntriesCompleted].Field.Ir = ReadPerm;
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Physical Page Address Pointed To 0x%x XXX, index %d\n", __FUNCTION__, (*FirstEntry)[EntriesCompleted].Field.PageAddress, EntriesCompleted);
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Exit\n", __FUNCTION__);
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

  // Figure out the optimal level for the number of pages we have
  NewLevel = 1;
  if (Pages >= LEVEL1_PAGES_AVAILABLE) {
    NewLevel = 2;
  }
  // If the caller asks for more than 1GB at once we need to write more code! (highly unlikely)
  ASSERT(Pages < LEVEL3_PAGES_AVAILABLE);

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picking NewLevel to be %d \n", __FUNCTION__, NewLevel);

  // Assign page for translation entry block. Then copy address into directory address. Set as read and write.
  // Translation entries are to ultimately set permissions for any devices
  TranslationEntryBlock = (IOMMU_PAGE_TRANSLATION_ENTRY *)((UINT64)AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_TRANSLATION_ENTRY))));
  PageDirectoryEntry->Field.NextAddress = SHIFT_RIGHT_POINTER_ADDR(TranslationEntryBlock, SHIFT_4K_BOUNDARY);
  PageDirectoryEntry->Field.Pr = 1;
  PageDirectoryEntry->Field.Ir = 1;
  PageDirectoryEntry->Field.Iw = 1;
  // This field determines how large the last level "pages" are
  PageDirectoryEntry->Field.NextLevel = NewLevel;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Translation block assigned at 0x%llx to directory 0x%llx\n", __FUNCTION__, TranslationEntryBlock, PageDirectoryEntry);

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
    return DeviceBufferAddress.Field.Level3;
  }
  if (TranslationLevel == 2) {
    return DeviceBufferAddress.Field.Level2;
  }
  if (TranslationLevel == 1) {
    return DeviceBufferAddress.Field.Level1;
  }

  // No levels specified which means that we are dealing with 0,0,0.
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Could not find useful index at 0,0,0. Returning 0 index \n", __FUNCTION__);
  return 0;
}

/**
  Sets the specified attributes of a device address or frees the entries associated with that address for disposal

  @param Device               PCI address of the device associated with this address. Optional parameter when freeing address
  @param DeviceAddress        Virtual device address that has previously been assigned to this device.
  @param BufferSizeInPages    Size of the buffer in pages.
  @param SetAttributesOnly    If true only read/write bits are set after table walk. If false the translation entries are freed.
  @param ReadPermissions      Only active if SetAttributesOnly is TRUE. This is passed on to the page translation entries.
  @param WritePermissions     Only active if SetAttributesOnly is TRUE. This is passed on to the page translation entries.

  @return BOOLEAN             Returns TRUE if successful. False if a problem was encountered and request could not be completed
**/
BOOLEAN
SetAttributesOrFreeDeviceAddress (
  IN PCI_ADDR         Device,
  IN UINT64           DeviceAddress,
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
  BOOLEAN                             Above4GAddress;


  EntriesToModify = 0;
  PagesPerEntry = 0;
  PageTranslationIndex = 0;
  PageTranslationLevel = 0;

  FinalAddress.Value = DeviceAddress;

  if (FinalAddress.Value > 0xFFFFFFFF) {
    Above4GAddress = TRUE;
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Detected Greater Than 4G Device Address \n", __FUNCTION__);
  }
  else {
    Above4GAddress = FALSE;
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Detected Less Than 4G Device Address \n", __FUNCTION__);
  }

  IOMMU_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL, "%a on: Level 1, 2, 3, 4 are %d %d %d %d Whole Address 0x%llx to PCIe device 0x%x\n",
    __FUNCTION__,
    FinalAddress.Field.Level1,
    FinalAddress.Field.Level2,
    FinalAddress.Field.Level3,
    FinalAddress.Field.Level4,
    FinalAddress.Value,
    Device.AddressValue
    );

  // Get the device's page directory entry. Will always be at the same level in this implementation
  if (Above4GAddress) {
    // FIRST_PAGE_DIR_LEVEL64
    if (BufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      PageDirectoryEntry =  &(mPageDirectoryBlock64_2M[FinalAddress.Field.Level3]);
    }
    else {
      PageDirectoryEntry =  &(mPageDirectoryBlock64_4K[FinalAddress.Field.Level2]);
    }
  }
  else {
    // FIRST_PAGE_DIR_LEVEL32
    if (BufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      PageDirectoryEntry =  &(mPageDirectoryBlock32_2M[FinalAddress.Field.Level3]);
    }
    else {
      PageDirectoryEntry =  &(mPageDirectoryBlock32_4K[FinalAddress.Field.Level2]);
    }
  }

  // Calculate translation level and then retrieve index, then find the device's page translation entry
  PageTranslationBlock = (IOMMU_PAGE_TRANSLATION_ENTRY *)SHIFT_LEFT_POINTER_ADDR(PageDirectoryEntry->Field.NextAddress, SHIFT_4K_BOUNDARY);
  PageTranslationLevel = PageDirectoryEntry->Field.NextLevel;
  PageTranslationIndex = GetPageTranslationIndex (FinalAddress, PageTranslationLevel);

  PageTranslationEntry = &(PageTranslationBlock[PageTranslationIndex]);

  IOMMU_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL, "%a Calculated Page Directory 0x%llx and Page Translation 0x%llx . Level of translation is %d\n",
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
      EntriesToModify++;
    }
  }

  IOMMU_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL,
    "%a Modifying %d entries of %d pages each for read %d write %d\n",
    __FUNCTION__,
    EntriesToModify,
    PagesPerEntry,
    ReadPermissions,
    WritePermissions
    );

  // Either clear out the whole block (if freeing) or set specified permissions (if assign))
  for (Counter = 0; Counter < EntriesToModify; Counter++) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Modifying PTE at 0x%x that is translating real memory address 0x%llx\n",
                     __FUNCTION__,
                     &(PageTranslationEntry[Counter]),
                     SHIFT_LEFT_POINTER_ADDR(PageTranslationEntry[Counter].Field.PageAddress, SHIFT_4K_BOUNDARY)
                     );
    if (SetAttributes) {
      PageTranslationEntry[Counter].Field.Ir = ReadPermissions;
      PageTranslationEntry[Counter].Field.Iw = WritePermissions;
      // Present bit should already have been hit
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Set %d / %d , Read %d Write %d\n", __FUNCTION__, Counter + 1, EntriesToModify, ReadPermissions, WritePermissions);
    }
    else {
      // Free entry
      ZeroMem(&(PageTranslationEntry[Counter]), sizeof(IOMMU_PAGE_TRANSLATION_ENTRY));
       IOMMU_HDT_CONSOLE (IOMMU_TRACE_VERBOSE, "%a Cleared %d / %d. \n", __FUNCTION__, Counter + 1, EntriesToModify);
    }

  }

  // Clear cache
  InvalidateDeviceAddressMemoryRange (DeviceAddress, (UINT32)BufferSizeInPages);

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Exit \n", __FUNCTION__);
  return TRUE;
}

/**
  Assign a virtual address for a device's DMA memory buffer for usage with IOMMU.

  @param PhysicalBuffer               Pointer to beginning of physical address. Should be 4k aligned
  @param PhysicalBufferSizeInPages    Size of buffer in terms of 4k pages
  @param Above4GDeviceAddress         Does device support an over 4G device address

  @return UINT64                      Device address to hand to device that should be translatable by the IOMMU
**/
UINT64
AssignDeviceAddress (
  IN VOID             *HostBuffer,
  IN UINTN            PhysicalBufferSizeInPages,
  IN BOOLEAN          Above4GDeviceAddress
  )
{
  UINTN                               *PageDirectoryEntryCounter;
  UINTN                               PageDirectoryResets;
  IOMMU_PAGE_DIRECTORY_ENTRY          *PageDirectoryBlock;
  IOMMU_PAGE_TRANSLATION_ENTRY        *TranslationEntry;
  UINTN                               TranslationBlockIndex;
  BOOLEAN                             WillItFit;
  IOMMU_DEVICE_BUFFER_ADDRESS         FinalAddress;

  ASSERT(HostBuffer != NULL);
  ASSERT(PhysicalBufferSizeInPages > 0);

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Entry. Called with buffer address 0x%llx and %d pages\n", __FUNCTION__, (UINT64)HostBuffer, PhysicalBufferSizeInPages);

  TranslationBlockIndex = 0;
  PageDirectoryEntryCounter = NULL;

  // Find a free directory entry in the page block or one that will fit our requirements.
  if (Above4GDeviceAddress) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Use an above 4G address to conserve 32bit address space.\n", __FUNCTION__);
    if (PhysicalBufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      PageDirectoryBlock = mPageDirectoryBlock64_2M;
      PageDirectoryEntryCounter = &mPageDirectoryBlock64_2M_LastDir; // skip 0, which points to mPageDirectoryBlock64_4K
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picked 64b 2M directory block\n", __FUNCTION__);
    }
    else {
      PageDirectoryBlock = mPageDirectoryBlock64_4K;
      PageDirectoryEntryCounter = &mPageDirectoryBlock64_4K_LastDir;
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picked 64b 4k directory block\n", __FUNCTION__);
    }
  }
  else {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Must allocate address below 4G\n", __FUNCTION__);
    if (PhysicalBufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      PageDirectoryBlock = mPageDirectoryBlock32_2M;
      PageDirectoryEntryCounter = &mPageDirectoryBlock32_2M_LastDir; // skip 0, which points to mPageDirectoryBlock32_4K
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picked 32b 2M directory block\n", __FUNCTION__);
    }
    else {
      PageDirectoryBlock = mPageDirectoryBlock32_4K;
      PageDirectoryEntryCounter = &mPageDirectoryBlock32_4K_LastDir;
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Picked 32b 4k directory block\n", __FUNCTION__);
    }
  }

  ASSERT (PageDirectoryEntryCounter != NULL);

  // Loop to find free entries in page directory. Note *PageDirectoryEntryCounter is initialized above
  // Go through this loop at most twice, track with PageDirectoryResets
  for (PageDirectoryResets = 0; PageDirectoryResets < 2; (*PageDirectoryEntryCounter)++ ) {

    if (*PageDirectoryEntryCounter >= TRANSLATION_OR_DIRECTORY_ENTRIES_PER_PAGE) {
      PageDirectoryResets++;
      *PageDirectoryEntryCounter = 0;
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Had to flip IOMMU Page Directory Entry Counter\n", __FUNCTION__);
    }

    // If entry is not present, allocate one to fit our needs. Otherwise attempt to use the one already here.
    if(PageDirectoryBlock[*PageDirectoryEntryCounter].Field.Pr == 0) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found free directory entry at index %d. Allocating for use.\n", __FUNCTION__, *PageDirectoryEntryCounter);
        AllocateAndAssignDirectoryTranslation (
          &(PageDirectoryBlock[*PageDirectoryEntryCounter]),
          PhysicalBufferSizeInPages
          );
    }
    else {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Checking directory %d to see if there is room\n", __FUNCTION__, *PageDirectoryEntryCounter);
    }
    WillItFit = WillSomethingFitInThisDirectory (
                  &(PageDirectoryBlock[*PageDirectoryEntryCounter]),
                  PageDirectoryBlock[*PageDirectoryEntryCounter].Field.NextLevel,
                  PhysicalBufferSizeInPages,
                  &TranslationEntry,
                  &TranslationBlockIndex
                  );
    if (WillItFit) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Found directory entry with enough space at %d\n", __FUNCTION__, *PageDirectoryEntryCounter);
      break;
    }
    else {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Not enough space in the directory %d\n", __FUNCTION__, *PageDirectoryEntryCounter);
    }
  }
  // We should never realistically overflow unless someone is trying to make us do so, return just in case...
  ASSERT (PageDirectoryResets < 2);
  if (PageDirectoryResets >= 2) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a WARNING: Something went terribly wrong or we are out of address space!!!\n", __FUNCTION__);
    return 0;
  }

  AssignTranslationEntries (
    HostBuffer,
    PhysicalBufferSizeInPages,
    &TranslationEntry,
    PageDirectoryBlock[*PageDirectoryEntryCounter].Field.NextLevel,
    FALSE,
    FALSE
    );

  // Now, finally calculate address to hand to device
  // First index into level 4
  FinalAddress.Value = 0;
  if (Above4GDeviceAddress) {
    // FIRST_PAGE_DIR_LEVEL64
    FinalAddress.Field.Level4 = 1;
    if (PhysicalBufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      // FIRST_PAGE_DIR_LEVEL_2M
      FinalAddress.Field.Level3 = *PageDirectoryEntryCounter;
    }
    else {
      // FIRST_PAGE_DIR_LEVEL_4K
      FinalAddress.Field.Level2 = *PageDirectoryEntryCounter;
    }
  }
  else {
    if (PhysicalBufferSizeInPages >= LEVEL1_PAGES_AVAILABLE) {
      // FIRST_PAGE_DIR_LEVEL_2M
      FinalAddress.Field.Level3 = *PageDirectoryEntryCounter;
    }
    else {
      // FIRST_PAGE_DIR_LEVEL_4K
      FinalAddress.Field.Level2 = *PageDirectoryEntryCounter;
    }
  }

  // Then fill in whatever the next level ended up being
  switch (PageDirectoryBlock[*PageDirectoryEntryCounter].Field.NextLevel) {
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
      ASSERT(PageDirectoryBlock[*PageDirectoryEntryCounter].Field.NextLevel < DXE_FIRST_PAGE_DIR_LEVEL64);
      return 0;
  }

  IOMMU_HDT_CONSOLE (
    IOMMU_TRACE_NORMAL, "%a Generated device address: Level 1, 2, 3, 4 are %d %d %d %d Whole Address 0x%llx\n",
    __FUNCTION__,
    FinalAddress.Field.Level1,
    FinalAddress.Field.Level2,
    FinalAddress.Field.Level3,
    FinalAddress.Field.Level4,
    FinalAddress.Value
    );

  return FinalAddress.Value;
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
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Device table key bits: V %d Tv %d Mode %d HostPageTablePtr 0x%x 000 Domain 0x%x Ir %d Iw %d\n",
                  __FUNCTION__,
                  DeviceTableEntry->Field.V,
                  DeviceTableEntry->Field.Tv,
                  DeviceTableEntry->Field.Mode,
                  DeviceTableEntry->Field.HostPageTablePtr,
                  DeviceTableEntry->Field.DomainId,
                  DeviceTableEntry->Field.Ir,
                  DeviceTableEntry->Field.Iw
                  );
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword0 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_0_31);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword1 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_32_63);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword2 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_64_95);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword3 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_96_127);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword4 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_128_159);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword5 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_160_191);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword6 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_192_223);
  IOMMU_HDT_CONSOLE(IOMMU_TRACE_NORMAL, "%a Dword7 0x%x \n", __FUNCTION__, DeviceTableEntry->Dwords.Dword_224_255);
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

  // By default Guest Translation is disabled and we will leave it this way. Host->Device translation is all that is needed for UEFI
  for (Counter = 0; Counter < SizeOfTable; Counter++) {
    TableToInitialize[Counter].Field.V = 1;
    TableToInitialize[Counter].Field.Tv = 1;
    // Same domain ID because we share page tables for now among all 64 bit devices
    TableToInitialize[Counter].Field.DomainId = DOMAIN_ID64;
    // Start with Level 4 translation: provides a 48 bit address space
    // 32 bit assignments will skip translating the first directory level because page directory will be indexed by entry 0 of the 64 bit level
    TableToInitialize[Counter].Field.Mode = DXE_FIRST_PAGE_DIR_LEVEL64;
    // Read and write permissions. TODO: set these on demand only later on
    TableToInitialize[Counter].Field.Ir = 1;
    TableToInitialize[Counter].Field.Iw = 1;
    // Fixed and arbitrated interrupts are forwarded unmapped
    TableToInitialize[Counter].Field.IntCtl = 1;
    // Point to page directory block
    TableToInitialize[Counter].Field.HostPageTablePtr = SHIFT_RIGHT_POINTER_ADDR(mPageDirectoryBlock64, SHIFT_4K_BOUNDARY);
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
  IOMMU_EVENT_LOG_ENTRY        *Max;
  UINTN                        Count;
  UINT64                       EventLogBase;

  // Dump all IOMMU
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }

    // If present then dump any entries found
    EventLogBase = (UINT64)SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].EventLogBaseAddressStruct->Field.EVENT_BASE, SHIFT_4K_BOUNDARY);
    Head = (IOMMU_EVENT_LOG_ENTRY *)(EventLogBase + mIommuContextData[Count].EventLogHead->Field.Value);
    Tail = (IOMMU_EVENT_LOG_ENTRY *)(EventLogBase + mIommuContextData[Count].EventLogTail->Field.Value);

    // Check to see if we should flip the circular buffer
    Max = (IOMMU_EVENT_LOG_ENTRY *)(EventLogBase + IOMMU_COMMAND_OR_EVENT_1024_ENTRIES*IOMMU_COMMAND_BUFFER_ENTRIES_PER_PAGE*sizeof(IOMMU_EVENT_LOG_ENTRY));

    // If there are no events return
    if (Head == Tail) {
      return;
    }

    // Entries are typically errors
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_NORMAL, "%a Head 0x%llx Tail 0x%llx \n", __FUNCTION__, Head, Tail);

    while (Tail != Head) {
      IOMMU_HDT_CONSOLE(
        IOMMU_TRACE_URGENT, "%a IOMMU Event to investigate (probably bad news): Code 0x%x Op1 0x%llx Op2 0x%llx\n",
        __FUNCTION__,
        Head->EventCode,
        Head->FirstEventCodeOperand1,
        Head->SecondEventCodeOperand
        );

      // Advance head pointer
      Head++;
      // If we are over the end of the buffer, circularize the head pointer
      if (Head > Max) {
        IOMMU_HDT_CONSOLE (
          IOMMU_TRACE_URGENT,
          "%a Flipping event buffer, tail is at 0x%x IOMMU %d\n",
          __FUNCTION__,
          mIommuContextData[Count].EventLogHead->Field.Value,
          Count
          );
        Head = (IOMMU_EVENT_LOG_ENTRY *)(EventLogBase);
      }
    }
    // Buffer has been emptied, so make Head = Tail to de-queue entries
    mIommuContextData[Count].EventLogHead->Field.Value = mIommuContextData[Count].EventLogTail->Field.Value;

    // Make this error extra visible if watching the log
    gBS->Stall(5000);
  }
}

/**
  restart the IOMMU command processing after the IOMMU has halted it.

**/
VOID
RestartIommuCmdProcess (
  IN UINTN       Count
  )
{
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandHead;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandBase;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      *CommandPtr;
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      TempCommand;
  VOLATILE UINT32                          CmdBufTail;
  VOLATILE UINT32                          CmdBufHead;
  VOLATILE UINT32                          IommuMmioStatus0;
  UINT32                                   CmdCounter;
  UINT32                                   VaildCmdCounter;
  UINTN                                    Pages;
  UINTN                                    index;
  VOID                                     *MemoryAllocated;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a COMPLETION_WAIT timeout\n", __FUNCTION__);
  // Test to see if this IOMMU is present
  if (mIommuContextData[Count].Present == FALSE) {
    return;
  }

  // If present then fill in entry and advance log pointer
  CommandBase = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)
    SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].CommandBufferAddressStruct->Field.COM_BASE, SHIFT_4K_BOUNDARY);
  CommandHead = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)
    ((UINT64)CommandBase + mIommuContextData[Count].CommandBufferHead->Field.Value);

  CmdBufTail = (UINT32) mIommuContextData[Count].CommandBufferTail->Field.Value;
  CmdBufHead = (UINT32) mIommuContextData[Count].CommandBufferHead->Field.Value;
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  Command buffer: Head=0x%x, Tail=0x%x\n", CmdBufHead, CmdBufTail);

  IommuMmioStatus0 = *(UINT32*)((UINT8 *)(mIommuContextData[Count].IommuControlRegister) - IOMMU_MMIO_CNTRL_STRUCT_OFFSET + IOMMU_STATUS_OFFSET);
  if ((IommuMmioStatus0 & 0x2) != 0) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  IOMMU_STATUS=0x%x\n", IommuMmioStatus0);
    do {
      IommuMmioStatus0 = *(UINT32*)((UINT64)CommandBase + IOMMU_STATUS_OFFSET);
    } while ((IommuMmioStatus0 & 0x10) != 0);
    mIommuContextData[Count].IommuControlRegister->Field.CMD_BUF_EN = 0;
    if (CmdBufTail != CmdBufHead) {
      if (CmdBufTail > CmdBufHead) {
        CmdCounter = (CmdBufTail - CmdBufHead) / IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE;
      } else {
        CmdCounter = IOMMU_COMMAND_BUFFER_ENTRIES - ((CmdBufHead - CmdBufTail) / IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE);
      }
      Pages = (CmdCounter / IOMMU_COMMAND_BUFFER_ENTRIES_PER_PAGE) + 1;
      MemoryAllocated = AllocateZeroedPages (Pages);
      CommandPtr = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT*)MemoryAllocated;

      VaildCmdCounter = 0;
      for (index = 0; index < CmdCounter; index++) {
        TempCommand = *CommandHead;
        if ((TempCommand.Command == IOMMU_COMPLETION_WAIT_COMMAND) || (TempCommand.Command == IOMMU_INVALIDATE_ALL_COMMAND)) {
          *CommandPtr = TempCommand;
          CommandPtr++;
          VaildCmdCounter++;
        }
        CommandHead++;
        if (CommandHead >= (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT *)((UINT64)CommandBase + IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE * IOMMU_COMMAND_BUFFER_ENTRIES)) {
          CommandHead = CommandBase;
        }
      }
      ZeroMem (CommandBase, IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE * IOMMU_COMMAND_BUFFER_ENTRIES);
      CommandPtr = (IOMMU_INVALIDATE_ALL_COMMAND_STRUCT*)MemoryAllocated;
      for (index = 0; index < VaildCmdCounter; index++) {
        TempCommand = *CommandPtr;
        *CommandBase = TempCommand;
        CommandBase++;
        CommandPtr++;
      }
      mIommuContextData[Count].CommandBufferHead->Field.Value = 0;
      mIommuContextData[Count].CommandBufferTail->Field.Value = VaildCmdCounter * IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE;
      FreePages (MemoryAllocated, Pages);
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  Rebuild command buffer: Head=0x%x, Tail=0x%x\n", mIommuContextData[Count].CommandBufferHead->Field.Value, mIommuContextData[Count].CommandBufferTail->Field.Value);
    } else {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  restart the IOMMU command processing\n");
    }
  } else {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "  Command timeout, but nothing wrong\n");
  }

  // Write the IOMMU Control Register with CmdBufEn=1b
  if (mIommuContextData[Count].IommuControlRegister->Field.CMD_BUF_EN == 0) {
    mIommuContextData[Count].IommuControlRegister->Field.CMD_BUF_EN = 1;
  }
}

/**
  Send IOMMU command buffer a command to wait for completion

**/
VOID
SendCompletionWaitCommand (
  IN UINTN       Count,
  IN VOID        *Cmd
  )
{
  // Head is set by IOMMU
  // Tail is set by software
  IOMMU_COMPLETION_WAIT_COMMAND_STRUCT      CompWaitCommand;
  IOMMU_COMPLETION_WAIT_COMMAND_STRUCT      *CommandTail;
  IOMMU_COMPLETION_WAIT_COMMAND_STRUCT      *CommandBase;
  UINTN                                     TimeoutCount;
  UINTN                                     CommandCount;
  VOLATILE UINT64                           CommandStore;
  VOLATILE UINT32                           CmdBufTail;
  VOLATILE UINT32                           CmdBufHead;

  // Test to see if this IOMMU is present
  if (mIommuContextData[Count].Present == FALSE) {
    return;
  }

  ZeroMem(&CompWaitCommand, sizeof(IOMMU_COMPLETION_WAIT_COMMAND_STRUCT));
  CompWaitCommand.Command = IOMMU_COMPLETION_WAIT_COMMAND;

  CompWaitCommand.Cmp_Store = 1;
  CompWaitCommand.StoreAddr = ((UINT64) &mIommuContextData[Count].CommandStore) >> 3;
  CompWaitCommand.StoreDataLo = 0xDEADBEEF;
  CompWaitCommand.StoreDataHi = 0xDEADBEEF;

  // If present then fill in entry
  CommandBase = (IOMMU_COMPLETION_WAIT_COMMAND_STRUCT *)
    SHIFT_LEFT_POINTER_ADDR(mIommuContextData[Count].CommandBufferAddressStruct->Field.COM_BASE, SHIFT_4K_BOUNDARY);

  // Wait for command buffer to sync (should be nearly instantaneous)
  TimeoutCount = 0;
  do {
    CmdBufTail = (UINT32) mIommuContextData[Count].CommandBufferTail->Field.Value;
    CmdBufHead = (UINT32) mIommuContextData[Count].CommandBufferHead->Field.Value;
    TimeoutCount++;
    if (TimeoutCount >= IOMMU_COMPLETION_WAIT_TIMEOUT) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Wait for CmdBufHead=CmdBufTail timeout, TimeoutCount=%d\n", TimeoutCount);
      break;
    }
  } while (CmdBufTail != CmdBufHead);

  TimeoutCount = 0;
  do {
    if (EFI_SUCCESS == SpinLockSave ((SPIN_LOCK_DATA *)&mSpinLock)) {
      CommandCount = 1;
      mIommuContextData[Count].CommandStore = 0;
      CommandTail = (IOMMU_COMPLETION_WAIT_COMMAND_STRUCT *)
        ((UINT64)CommandBase + mIommuContextData[Count].CommandBufferTail->Field.Value);
      if (Cmd != NULL) {
        *CommandTail = *(IOMMU_COMPLETION_WAIT_COMMAND_STRUCT *)Cmd;
        CommandTail++;
        CommandCount = 2;
      }
      *CommandTail = CompWaitCommand;
      if (mIommuContextData[Count].CommandBufferTail->Field.Value >= IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE*(IOMMU_COMMAND_BUFFER_ENTRIES - CommandCount)) {
        IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Flipping command buffer 0x%x IOMMU %d\n", __FUNCTION__, mIommuContextData[Count].CommandBufferTail->Field.Value, Count);
        mIommuContextData[Count].CommandBufferTail->Field.Value = 0;
      }
      else {
        mIommuContextData[Count].CommandBufferTail->Field.Value = mIommuContextData[Count].CommandBufferTail->Field.Value + IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE * CommandCount;
      }
      SpinLockRestore ((SPIN_LOCK_DATA *)&mSpinLock);
      break;
    } else {
      //wait for Spin lock;
      TimeoutCount++;
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "TimeOut: wait for Spin lock, TimeoutCount=%d\n", TimeoutCount);
    }
  }while (TimeoutCount < IOMMU_COMPLETION_WAIT_TIMEOUT);

  TimeoutCount = 0;
  do {
    CommandStore = mIommuContextData[Count].CommandStore;
    if (CommandStore != 0) {
      break;
    }
    gBS->Stall(1);
    TimeoutCount++;
  }while (TimeoutCount < IOMMU_COMPLETION_WAIT_TIMEOUT);

  if (TimeoutCount >= IOMMU_COMPLETION_WAIT_TIMEOUT) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "TimeOut: wait for Complation wait command, TimeoutCount=%d\n", TimeoutCount);
    RestartIommuCmdProcess (Count);
  }

  // Wait for command buffer to sync (should be nearly instantaneous)
  TimeoutCount = 0;
  do {
    CmdBufTail = (UINT32) mIommuContextData[Count].CommandBufferTail->Field.Value;
    CmdBufHead = (UINT32) mIommuContextData[Count].CommandBufferHead->Field.Value;
    if ((CmdBufHead == 0) && (CmdBufTail == 0)) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "Command buffer full, Clear Command Buffer\n");
      ZeroMem (CommandBase, IOMMU_COMMAND_OR_EVENT_ENTRY_SIZE * IOMMU_COMMAND_BUFFER_ENTRIES);
    }
    TimeoutCount++;
    if (TimeoutCount >= IOMMU_COMPLETION_WAIT_TIMEOUT) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "TimeOut: Wait for CmdBufHead=CmdBufTail, TimeoutCount=%d\n", TimeoutCount);
      break;
    }
  } while (CmdBufTail != CmdBufHead);
}

/**
  Send IOMMU command buffer a command. This function should only be used for debug purposes and is not
  called during the course of normal operation.
**/
VOID
SendInvdAllCommand (
  VOID
  )
{
  // Head is set by IOMMU
  // Tail is set by software
  IOMMU_INVALIDATE_ALL_COMMAND_STRUCT      InvdAll;
  UINTN                                    Count;

  ZeroMem(&InvdAll, sizeof(IOMMU_INVALIDATE_ALL_COMMAND_STRUCT));
  InvdAll.Command = IOMMU_INVALIDATE_ALL_COMMAND;

  // Blast to all IOMMU. Eventually might be a good idea to only INVD the IOMMU that has a changed entry
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }
    SendCompletionWaitCommand (Count, (VOID *)&InvdAll);
  }
}


/**
  Send IOMMU command buffer a command to invalidate cache for a range of pages.

  @param DeviceAddress          Device address assigned by AssignDeviceAddress()
  @param InvdSignExtendAddress  FALSE, just invalidate 4k, TRUE sign extend the highest zero to invalidate a whole block
**/
VOID
SendInvdPagesCommand (
  IN UINT64      DeviceAddress,
  IN BOOLEAN     IsOverOnePage
  )
{
  // Head is set by IOMMU
  // Tail is set by software
  IOMMU_INVALIDATE_PAGES_COMMAND_STRUCT      InvdPagesCommand;
  UINTN                                      Count;

  // Clear memory cache
  AsmWbinvd ();

  ZeroMem(&InvdPagesCommand, sizeof(IOMMU_INVALIDATE_PAGES_COMMAND_STRUCT));
  InvdPagesCommand.Command = IOMMU_INVALIDATE_PAGES_COMMAND;

  InvdPagesCommand.GN = 0;                     // GV is not enabled so this is zero
  InvdPagesCommand.PASID = 0;                  // Two level translation is not enabled, so 0 PASID
  InvdPagesCommand.PDE = 1;                    // Clear translation and directory entries
  InvdPagesCommand.DomainId = DOMAIN_ID64;
  InvdPagesCommand.S = IsOverOnePage;          // Clear out just one page, or sign extend for a larger block
  InvdPagesCommand.Address = SHIFT_RIGHT_POINTER_ADDR(DeviceAddress, SHIFT_4K_BOUNDARY);

  // Blast to all IOMMU. Eventually might be a good idea to only INVD the IOMMU that has a changed entry
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {
    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }
    SendCompletionWaitCommand (Count, (VOID *)&InvdPagesCommand);
  }
}

/**
  Event to be run on ExitBootServices() that disables all IOMMU translation

  @param Event    Ignored in this function, necessary for event function signature so left in.
  @param Context  Ignored in this function, necessary for event function signature so left in.
**/
VOID
EFIAPI
DisableIommuBeforeOsHandoff (
  IN EFI_EVENT                               Event,
  IN VOID                                    *Context
  )
{
  UINTN                               Count;
  UINT16                              CommandReg;
  PCI_ADDR                            PciAddress;
  VOID                                *IommuMmioBaseAddressRegister;
  IOMMU_MMIO_CMD_BASE_STRUCT          *CommandBufferAddressStruct;
  IOMMU_MMIO_EVENT_BASE_STRUCT        *EventBaseAddressStruct;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Running\n", __FUNCTION__);

  StopUsb();
  // Invalidate All
  SendInvdAllCommand ();

  // As a precaution, disable bus master for poorly documented UART device off the FCH
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x14, 5, 0);
  GnbLibPciRead (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);
  // Clear bus master bit in command register
  CommandReg = CommandReg & 0xFFFB;
  GnbLibPciWrite (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);

  // Enable BUS MASTER for this IOMMU
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 2, 0);
  GnbLibPciRead (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);
  // Clear bus master bit in command register
  CommandReg = CommandReg | 0x4;
  GnbLibPciWrite (PciAddress.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth16, &CommandReg, NULL);

  // Disable all IOMMU before OS load
  for (Count = 0; Count < IOMMU_CONTEXT_DATA_ENTRIES; Count++) {

    // Test to see if this IOMMU is present
    if (mIommuContextData[Count].Present == FALSE) {
      break;
    }
    IommuMmioBaseAddressRegister = (VOID *) ((UINT8 *)(mIommuContextData[Count].IommuControlRegister) - IOMMU_MMIO_CNTRL_STRUCT_OFFSET);
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "IommuMmioBase = 0x%x and IommuControl = 0x%x\n", IommuMmioBaseAddressRegister, mIommuContextData[Count].IommuControlRegister);

    DumpEventLog();

    mIommuContextData[Count].IommuControlRegister->Value = 0;
    mIommuContextData[Count].IommuControlRegister->Field.COHERENT = 1;
    mIommuContextData[Count].IommuControlRegister->Field.EPH_EN = 1;
    mIommuContextData[Count].IommuControlRegister->Field.Block_StopMark_En = 1;
    mIommuContextData[Count].Present = FALSE;

    mIommuContextData[Count].EventLogHead->Field.Value = 0;
    mIommuContextData[Count].EventLogTail->Field.Value = 0;

    // Clear Event Log Address
    EventBaseAddressStruct = (IOMMU_MMIO_EVENT_BASE_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_EVENT_BASE_STRUCT_OFFSET);
    EventBaseAddressStruct->Field.EVENT_LEN = 0x8;
    EventBaseAddressStruct->Field.EVENT_BASE = 0;
    // Clear Command Buffer Address
    CommandBufferAddressStruct = (IOMMU_MMIO_CMD_BASE_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_CMD_BASE_STRUCT_OFFSET);
    CommandBufferAddressStruct->Field.COM_LEN = 0x8;
    CommandBufferAddressStruct->Field.COM_BASE = 0;
    // Clear DEVTBL Buffer Address
    // DevtableAddressStruct->Field.DEV_TBL_SIZE = 0;
    // DevtableAddressStruct->Field.DEV_TBL_BASE = 0;
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
  IOMMU_PAGE_TRANSLATION_ENTRY        *Lowest32BitTranslationBlock;
  UINTN                               NbiosInitialized;
  EFI_EVENT                           ExitBootServicesEvent;
  MMIO_CFG_BASE_MSR                   MmioCfgBase;

  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Enter\n", __FUNCTION__);
  ASSERT(GnbHandle != NULL);
  NbiosInitialized = 0;
  IommuMmioBaseAddressRegister = NULL;

  mSpinLock.Lock = FALSE;
  mSpinLock.InterruptState = FALSE;

  ZeroMem (mIommuContextData, sizeof(IOMMU_CONTEXT_DATA)*IOMMU_CONTEXT_DATA_ENTRIES);

  // First thing is to allocate master page directory. We are using a unified page directory because we do not
  // know the PCIe address of the device consuming the memory until both allocate and map have been called.
  // Virtual device addresses will be generated when Map() is called and only associated with a device after SetAttributes()
  mPageDirectoryBlock64 =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINT64)(AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock64 == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate 64 bit page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a 64 bit directory block allocated at 0x%llx\n", __FUNCTION__, (UINT64)mPageDirectoryBlock64);

  mPageDirectoryBlock64_2M =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINT64)(AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock64_2M == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate 64 bit 2M page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a 64 bit directory block allocated at 0x%llx\n", __FUNCTION__, (UINT64)mPageDirectoryBlock64_2M);

  mPageDirectoryBlock64_4K =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINT64)(AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock64_4K == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate 64 bit 4k page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a 64 bit directory block allocated at 0x%llx\n", __FUNCTION__, (UINT64)mPageDirectoryBlock64_2M);

  mPageDirectoryBlock32_2M =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINT64)(AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock32_2M == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate 32 bit 2M page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a 32 bit directory block allocated at 0x%llx\n", __FUNCTION__, (UINT64)mPageDirectoryBlock32_2M);

  mPageDirectoryBlock32_4K =
    (IOMMU_PAGE_DIRECTORY_ENTRY *)((UINT64)(AllocateZeroedPages (EFI_SIZE_TO_PAGES (sizeof (IOMMU_PAGE_DIRECTORY_ENTRY)))));
  if (mPageDirectoryBlock32_4K == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate 32 bit 4k page directory block.\n", __FUNCTION__);
    return FALSE;
  }
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a 32 bit directory block allocated at 0x%llx\n", __FUNCTION__, (UINT64)mPageDirectoryBlock32_2M);

  // Point 64 bit PDE index 0 to the 32bit 2M page directory. All 32 bit addresses will fall through this page directory entry
  mPageDirectoryBlock64[0].Field.NextLevel = DXE_FIRST_PAGE_DIR_LEVEL_2M;
  mPageDirectoryBlock64[0].Field.NextAddress = RShiftU64 ((UINT64)(UINTN)mPageDirectoryBlock32_2M, SHIFT_4K_BOUNDARY);
  mPageDirectoryBlock64[0].Field.Ir = 1;
  mPageDirectoryBlock64[0].Field.Iw = 1;
  mPageDirectoryBlock64[0].Field.Pr = 1;

  // Point the first entry of the 32bit 2M page directory to the 32bit 4K directory
  mPageDirectoryBlock32_2M[0].Field.NextLevel = DXE_FIRST_PAGE_DIR_LEVEL_4K;
  mPageDirectoryBlock32_2M[0].Field.NextAddress = RShiftU64 ((UINT64)(UINTN)mPageDirectoryBlock32_4K, SHIFT_4K_BOUNDARY);
  mPageDirectoryBlock32_2M[0].Field.Ir = 1;
  mPageDirectoryBlock32_2M[0].Field.Iw = 1;
  mPageDirectoryBlock32_2M[0].Field.Pr = 1;

  // Avoid NULL entries by marking 0,0,0 present but no read or write permissions.
  // Assign first level translation block for the 32 bit page directory
  // Devices that DMA to a NULL address will flag page fault events in the log
  AllocateAndAssignDirectoryTranslation (
    &(mPageDirectoryBlock32_4K[0]),
    LEVEL1_PAGES_AVAILABLE
    );
  Lowest32BitTranslationBlock =
    (IOMMU_PAGE_TRANSLATION_ENTRY *)SHIFT_LEFT_POINTER_ADDR(mPageDirectoryBlock32_4K[0].Field.NextAddress, SHIFT_4K_BOUNDARY);
  Lowest32BitTranslationBlock[0].Field.Pr = 1;
  Lowest32BitTranslationBlock[0].Field.Ir = 0;
  Lowest32BitTranslationBlock[0].Field.Iw = 0;
  Lowest32BitTranslationBlock[0].Field.NextLevel = 0;
  Lowest32BitTranslationBlock[0].Field.PageAddress = 0;

  // Point 64 bit PDE index 1 to 64bit 2M page directory. All 64 bit DMA addresses will start with 1 in 33rd bit
  mPageDirectoryBlock64[1].Field.NextLevel = DXE_FIRST_PAGE_DIR_LEVEL_2M;
  mPageDirectoryBlock64[1].Field.NextAddress = RShiftU64 ((UINT64)(UINTN)mPageDirectoryBlock64_2M, SHIFT_4K_BOUNDARY);
  mPageDirectoryBlock64[1].Field.Ir = 1;
  mPageDirectoryBlock64[1].Field.Iw = 1;
  mPageDirectoryBlock64[1].Field.Pr = 1;

  // Point the first entry of the 64 bit 2M page directory to the 64bit 4K directory
  mPageDirectoryBlock64_2M[0].Field.NextLevel = DXE_FIRST_PAGE_DIR_LEVEL_4K;
  mPageDirectoryBlock64_2M[0].Field.NextAddress = RShiftU64 ((UINT64)(UINTN)mPageDirectoryBlock64_4K, SHIFT_4K_BOUNDARY);
  mPageDirectoryBlock64_2M[0].Field.Ir = 1;
  mPageDirectoryBlock64_2M[0].Field.Iw = 1;
  mPageDirectoryBlock64_2M[0].Field.Pr = 1;

  // Allocate space for the device table. The device table is allowed to be shared among all IOMMUs
  MmioCfgBase.Value = AsmReadMsr64 (MSR_MMIO_Cfg_Base);
  PciBusNumber = PCIE_MAX_BUS;
  if ((MmioCfgBase.Field.Enable == 1) && (MmioCfgBase.Field.BusRange <= 8)) {
    PciBusNumber = 1 << (MmioCfgBase.Field.BusRange);
  }
  DeviceTableEntries = PciBusNumber * PCIE_MAX_DEVICES * PCIE_MAX_FUNCTIONS;
  Pages = EFI_SIZE_TO_PAGES (sizeof (IOMMU_DEVICE_TABLE) * DeviceTableEntries);
  mDeviceTable = AllocateZeroedPages (Pages);
  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Allocated IOMMU device table at 0x%llx size in pages is %d\n", __FUNCTION__, mDeviceTable, Pages);
  InitializeDeviceTable (mDeviceTable, DeviceTableEntries);
  if (mDeviceTable == NULL) {
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a FATAL error cannot allocate device table.\n", __FUNCTION__);
    return FALSE;
  }

  while (GnbHandle != NULL) {
    // Retrieve PCIe location for this IOMMU to extract BAR location for programming with offsets
    GnbIommuPciAddress = NbioGetHostPciAddress (GnbHandle);
    GnbIommuPciAddress.Address.Function = PCICFG_AMD_IOMMU_FUNCTION;
    GnbLibPciRead (
      GnbIommuPciAddress.AddressValue | PCICFG_AMD_IOMMU_CAP_BASE_HI_OFFSET,
      AccessS3SaveWidth32,
      &BarReadValue,
      NULL
      );
    IommuMmioBaseAddressRegister = (VOID *)LShiftU64 ((UINT64)(UINTN)BarReadValue, 32);
    GnbLibPciRead (
      GnbIommuPciAddress.AddressValue | PCICFG_AMD_IOMMU_CAP_BASE_LO_OFFSET,
      AccessS3SaveWidth32,
      &BarReadValue,
      NULL
      );
    // Make sure IOMMU BAR is enabled
    if ((BarReadValue & 1) != 1) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a WARNING: IOMMU BAR enable bit has not been hit!\n", __FUNCTION__);
      return FALSE;
    }
    // Mask out enable bit and convert to pointer
    IommuMmioBaseAddressRegister = (VOID *)((BarReadValue + (UINT64)IommuMmioBaseAddressRegister) & 0xfffffffffffffffe);
    IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a IOMMU Bar 0x%x\n", __FUNCTION__, IommuMmioBaseAddressRegister);
    if ((UINT64)IommuMmioBaseAddressRegister == 0) {
      IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a ERROR IOMMU Bar is not setup! Exiting \n", __FUNCTION__);
      return FALSE;
    }
    // Disable IOMMU features if they were enabled earlier in PEI. These addresses are all stale and may even be in an erroneous state
    IommuControlRegister = (IOMMU_MMIO_CNTRL_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_CNTRL_STRUCT_OFFSET);
    IommuControlRegister->Value = 0;

    // Step 1: Setup device table address
    DevtableAddressStruct = (IOMMU_MMIO_DEVTBL_BASE_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_DEVTBL_BASE_STRUCT_OFFSET);
    DevtableAddressStruct->Field.DEV_TBL_SIZE = IOMMU_MAX_DEVICE_TABLE_ENTRIES - 1;
    DevtableAddressStruct->Field.DEV_TBL_BASE = RShiftU64 ((UINT64)(UINTN)mDeviceTable, SHIFT_4K_BOUNDARY);
    IOMMU_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Device table base MMIO register 0x%x, size %d\n", __FUNCTION__, DevtableAddressStruct->Dword.DwordLo, DevtableAddressStruct->Field.DEV_TBL_SIZE);

    // Step 2: Allocate and setup command buffer base address register
    Pages = IOMMU_COMMAND_BUFFER_ENTRIES/IOMMU_COMMAND_BUFFER_ENTRIES_PER_PAGE;
    MemoryAllocated = AllocateZeroedPages (Pages);
    CommandBufferAddressStruct = (IOMMU_MMIO_CMD_BASE_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_CMD_BASE_STRUCT_OFFSET);
    CommandBufferAddressStruct->Field.COM_LEN = IOMMU_COMMAND_OR_EVENT_1024_ENTRIES;
    CommandBufferAddressStruct->Field.COM_BASE = RShiftU64 ((UINT64)(UINTN)MemoryAllocated, SHIFT_4K_BOUNDARY);
    IOMMU_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Command Base Init 0x%x\n", __FUNCTION__, CommandBufferAddressStruct->Dword.DwordLo);

    // Step 3: Allocate Command Buffer Head Pointer Register
    // Step 4: Allocate command Buffer Tail Pointer Register
    CommandBufferHead = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_COMMAND_BUFFER_POINTER_HEAD_STRUCT_OFFSET);
    CommandBufferTail = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_COMMAND_BUFFER_POINTER_TAIL_STRUCT_OFFSET);
    CommandBufferHead->Value = 0;
    CommandBufferTail->Value = 0;

    // Step 5: Iommu Exclusion Base Register and Iommu Exclusion Range Limit Register
    // No need for this right now

    // Step 6: Event Log base address register and Event Log Pointer Registers (head/tail)
    Pages = IOMMU_EVENT_LOG_ENTRIES/IOMMU_EVENT_LOG_ENTRIES_PER_PAGE;
    MemoryAllocated = AllocateZeroedPages (Pages);
    EventBaseAddressStruct = (IOMMU_MMIO_EVENT_BASE_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_MMIO_EVENT_BASE_STRUCT_OFFSET);
    EventBaseAddressStruct->Field.EVENT_LEN = IOMMU_COMMAND_OR_EVENT_1024_ENTRIES;
    EventBaseAddressStruct->Field.EVENT_BASE = RShiftU64 ((UINT64)(UINTN)MemoryAllocated, SHIFT_4K_BOUNDARY);
    IOMMU_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a Event Base Init 0x%x\n", __FUNCTION__, EventBaseAddressStruct->Dword.DwordLo);

    // Step 7: Allocate Event Log Buffer Head Pointer
    // Step 8: Allocate Event Log Buffer Tail Pointer
    EventLogHead = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_EVENT_LOG_POINTER_HEAD_STRUCT_OFFSET);
    EventLogTail = (IOMMU_COMMAND_OR_LOG_POINTER_STRUCT *)((UINT64)IommuMmioBaseAddressRegister + IOMMU_EVENT_LOG_POINTER_TAIL_STRUCT_OFFSET);
    EventLogHead->Value = 0;
    EventLogTail->Value = 0;

    // Enable event log and command buffer (control register pointer set earlier in function)
    IommuControlRegister->Field.EVENT_LOG_EN = 1;
    IommuControlRegister->Field.CMD_BUF_EN = 1;
    IommuControlRegister->Field.COHERENT = 1;
    // Last Step: Setup IOMMU control register to begin protection. Device requests for DMA mapping will have to go through our code.
    // Any other permanent config should be done here
    IommuControlRegister->Field.IOMMU_EN = 1;
    IOMMU_HDT_CONSOLE(IOMMU_TRACE_URGENT, "%a IOMMU control register for NBIO %d is initialized to 0x%llx\n", __FUNCTION__, NbiosInitialized, IommuControlRegister->Value);

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

  // Invalidate All
  SendInvdAllCommand ();

  // Clear cache, just to be safe

  // Create event for disabling IOMMU before OS handoff
  gBS->CreateEventEx (
         EVT_NOTIFY_SIGNAL,
         TPL_CALLBACK,
         DisableIommuBeforeOsHandoff,
         NULL,
         &gEfiEventExitBootServicesGuid,
         &ExitBootServicesEvent
         );


  IOMMU_HDT_CONSOLE (IOMMU_TRACE_URGENT, "%a Exit\n", __FUNCTION__);
  return TRUE;
}
