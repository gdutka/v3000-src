/** @file

Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include "SmmSupervisor.h"

#include <Library/SmmSupervisorPageMgmtLib.h>

/**
  This function sets SMM save state buffer to be CPL0.
**/
VOID
PatchCpL0SmmSaveStateMap (
  VOID
  )
{
  UINTN Index;
  //  +=============================+  <-- Base of allocated buffer + 32 KB
  //  |   CPU 1 Save State          |
  //  +-----------------------------+ 0xFE00
  //  |   CPU 1 Extra Data          |
  //  +-----------------------------+ 0xFB00
  //  |   Padding                   |
  //  +-----------------------------+
  //  |   CPU m SMI Entry           |
  //  +#############################+  <-- Base of allocated buffer + 32 KB == CPU 0 SMBASE + 64 KB

  //We simply set the region [SMM_BASE + 0xF000 - SMM_BASE + 0xFFFF] to CPL0 only
  DEBUG ((DEBUG_INFO, "PatchCpL0SmmSaveStateMap:\n"));
  for (Index = 0; Index < mSmmSupvCpuData.NumberOfCpus; Index++) {
    DEBUG ((DEBUG_INFO, "CPU %d SmmBase: 0x%x\n", Index, mSmmSupvCpuData.SmBase[Index]));
    SmmSetMemoryAttributes (mSmmSupvCpuData.SmBase[Index] + 0xF000, 0x1000, EFI_MEMORY_SP, TRUE);
    //Set SMI entry to R/W, to allow DRTMBoot flag able to clear
    SmmClearMemoryAttributes (mSmmSupvCpuData.SmBase[Index] + 0x8000, 0x1000, EFI_MEMORY_RO, TRUE);
    //Set SMI entry to CPL0
    SmmSetMemoryAttributes (mSmmSupvCpuData.SmBase[Index] + 0x8000, 0x1000, EFI_MEMORY_SP, TRUE);
  }
}

/**
  This function set required SmmSupvCode to be accessed by CPL0 only
**/
VOID
PatchCpL0SmmSupvCode (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length
  )
{

}

/**
  This function set required SmmSupvStack to be accessed by CPL0 only
**/
VOID
PatchCpL0SmmSupvStack (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length
  )
{

}

/**
  This function set required SmmSupvHeap to be accessed by CPL0 only
**/
VOID
PatchCpL0SmmSupvHeap (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length
  )
{

}

/**
  This function set required Gdt to be accessed by CPL0 only
**/
VOID
PatchCpL0Gdt (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length
  )
{

}

/**
  This function set required Idt to be accessed by CPL0 only
**/
VOID
PatchCpL0Idt (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length
  )
{

}

/**
  This function set required memory regions to be accessed by CPL0 only
**/
VOID
PatchSmmSupvPageTbl (
  SMM_SUPV_CPL0_REGION *Cpl0Regions,
  UINTN                NumOfRegion
  )
{
  SetMemoryAttributesForCpl0Regions (Cpl0Regions, NumOfRegion);

  DEBUG ((DEBUG_INFO, "PatchCpL0SmmSaveStateMap\n"));
  // PatchCpL0 SMM Savestate including SmmEntry and PSD
  PatchCpL0SmmSaveStateMap ();
}