/** @file
  Check PEI or DXE phase to use AllocatePool or AllocatePages

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/H2OImageDecoderLib.h>
#include "H2OImageDecoderLibCommon.h"

/**
 Check if system is in PEI or DXE phase.

 @retval TRUE              System is in PEI phase.
 @retval FALSE             System is in DXE phase.
**/
STATIC
BOOLEAN
IsPEIPhase (
  VOID
  )
{
  UINTN           *Buffer;

  STATIC BOOLEAN  IsPhaseCheckDone = FALSE;
  STATIC BOOLEAN  InPEI = FALSE;

  if (IsPhaseCheckDone) {
    return InPEI;
  }

  IsPhaseCheckDone = TRUE;

  Buffer = AllocatePool (0x10000);
  if (Buffer != NULL) {
    FreePool (Buffer);
    InPEI = FALSE;
  } else {
    InPEI = TRUE;
  }

  return InPEI;
}

/**
 Allocate memory resource by AllocatePages in PEI and by AllocatePool in DXE.

 @param [in]   AllocationSize   The number of bytes to allocate.

 @return A pointer to the allocated buffer or NULL if allocation fails.
**/
VOID *
EFIAPI
H2OImageDecoderLibAllocateMem (
  IN UINTN  AllocationSize
  )
{
  if (IsPEIPhase()) {
    return AllocatePages (EFI_SIZE_TO_PAGES(AllocationSize));
  } else {
    return AllocatePool (AllocationSize);
  }
}

/**
 Free memory resource by FreePages in PEI and by FreePool in DXE.

 @param [in]   Buffer           The pointer to the buffer of pages to free.
 @param [in]   Pages            The number of 4 KB pages to free.
**/
VOID
EFIAPI
H2OImageDecoderLibFreeMem (
  IN VOID   *Buffer,
  IN UINTN  Size
  )
{
  if (IsPEIPhase()) {
    FreePages (Buffer, EFI_SIZE_TO_PAGES (Size));
  } else {
    FreePool (Buffer);
  }
}

/**
 Allocate zero memory resource by AllocatePages and Zeromem in PEI and AllocateZeroPool in DXE.

 @param [in]   AllocationSize   The number of bytes to allocate.

 @return A pointer to the allocated buffer or NULL if allocation fails.
**/
VOID *
EFIAPI
H2OImageDecoderLibAllocateZeroMem (
  IN UINTN  AllocationSize
  )
{
  VOID  *Memory;

  if (IsPEIPhase()) {
    Memory = AllocatePages (EFI_SIZE_TO_PAGES(AllocationSize));
    if (Memory != NULL) {
      Memory = ZeroMem (Memory, AllocationSize);
    }
    return Memory;
  } else {
    return AllocateZeroPool (AllocationSize);
  }
}
