/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include "MemTest.h"

VOID AsmCopyMem (UINT64 Start, UINT64 Mid, UINT64 Length);

STATIC UINT64 *mPatternArray;        // Pattern to fill memory.
STATIC VOID   *mTestMemory;          // Memory used to test patterns.

VOID
MemTestSetup (VOID)
{
  UINT64 Pattern1 = 0;
  UINT64 Pattern2 = 0x80000000800;
  UINTN  i;

  mPatternArray = (UINT64*)AllocatePool (EFI_PAGE_SIZE);

  for (i = 0; i < 64; i++) {
    mPatternArray[8 * i] = ~Pattern1;
    mPatternArray[8 * i + 1] = ~Pattern2;
    mPatternArray[8 * i + 2] = ~Pattern1;
    mPatternArray[8 * i + 3] = ~Pattern1;
    mPatternArray[8 * i + 4] = Pattern1;
    mPatternArray[8 * i + 5] = Pattern2;
    mPatternArray[8 * i + 6] = Pattern1;
    mPatternArray[8 * i + 7] = Pattern1;
  }

  mTestMemory = AllocatePages ((MEMTEST_BLOCK_SIZE/EFI_PAGE_SIZE) + 1);
}

VOID
MemTestCleanup (VOID)
{
  FreePool (mPatternArray);
  FreePages (mTestMemory, (MEMTEST_BLOCK_SIZE/EFI_PAGE_SIZE) + 1);
}

// Fill memory of size with the test pattern. It will be half the size of the total memory to test.
VOID FillMemoryWithTestPattern (UINT64 FillMemorySize)
{
  UINT32 MemoryOffset = 0;  // Offset of memory page to fill.

  while (MemoryOffset < FillMemorySize) {
    UINT32 QuadOffset = 0;  // Pattern offset.
    UINT32 MemoryIndexUint64 = MemoryOffset / sizeof (UINT64);    //8 Byte index of memory page.

    while (QuadOffset < (EFI_PAGE_SIZE / sizeof (UINT64))) {
        *((UINT64*)mTestMemory + MemoryIndexUint64 + QuadOffset) = mPatternArray[QuadOffset];
        QuadOffset++;

    }
    MemoryOffset += EFI_PAGE_SIZE;
  }
}

// Copy memory patherns between first half and second half. Lenght is in dwords.
VOID
CopyMemoryBlock (UINT64 Length)
{
  UINT64 HalfLength = Length / 2;
  UINT64 Mid = (UINT64)mTestMemory + HalfLength * sizeof (UINT32);
  UINT32 i;

  for (i = 0; i < MEMTEST_LOOP_COUNT; i++) {
    AsmCopyMem ((UINT64)mTestMemory, Mid, HalfLength);
  }
}

// Verify memory is copied correctly. Return Error Count.
UINTN
CompareMemoryBlock (UINT64 Length)
{
  UINT32 i;
  UINTN  ProcessorNumber;
  UINTN  ErrorCount = 0;

  gMpServices->WhoAmI (gMpServices, &ProcessorNumber);

  for (i = 0; i < Length / sizeof (UINT32); i += 2) {
    if (((UINT32*)mTestMemory)[i] != ((UINT32*)mTestMemory)[i + 1]) {
        ErrorCount++;
    }
  }

  return ErrorCount;
}

// Run Memory test and return Error count.
UINTN
MemTestRun (VOID)
{
  FillMemoryWithTestPattern (MEMTEST_BLOCK_SIZE / 2); //Execute on other cores
  CopyMemoryBlock (MEMTEST_BLOCK_SIZE / sizeof (UINT32));
  return CompareMemoryBlock (MEMTEST_BLOCK_SIZE);
}