/** @file
Agent Module to load other modules to deploy SMM Entry Vector for X86 CPU.

Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmSupervisorExceptionHandlerLib.h>
#include <Protocol/SmmSupervisorProtocol.h>

#include "SmmSupervisor.h"

/**
 To allocate GDT table buffer

 Allocates pages for GDT table

 @param  CpuIndex Input Cpu Index

 @param  Cpl0GdtTssTable The Gdt Table base address

 @param  Cpl0GdtTssTableSize The Gdt Table Size

 @return Status

**/
EFI_STATUS
EFIAPI
AllocateGdtBuffer (
  IN  UINT64                 CpuIndex,
  OUT EFI_PHYSICAL_ADDRESS  *Cpl0GdtTssTable,
  OUT UINTN                 *Cpl0GdtTssTableSize
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory;

  *Cpl0GdtTssTableSize = (mSmiGdtr.Limit + 1 + TSS_SIZE + 7) & ~7;

  Status = gSmst->SmmAllocatePages (AllocateAnyPages, EfiRuntimeServicesCode, EFI_SIZE_TO_PAGES (*Cpl0GdtTssTableSize), &Memory);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *Cpl0GdtTssTable = (EFI_PHYSICAL_ADDRESS)Memory;

  DEBUG((DEBUG_INFO, "mSmmSupvGdtData[%d].Tablebase = 0x%x, TableSize = 0x%x\n", CpuIndex, Memory, *Cpl0GdtTssTableSize));
  mSmmSupvGdtData[CpuIndex].Tablebase = (UINT64) Memory;
  mSmmSupvGdtData[CpuIndex].TableSize = (UINT64)*Cpl0GdtTssTableSize;
  return Status;
}

#define CALL_GATE_DESCRIPTOR_OFFSET       0x60
#define CALL_GATE_IRET_DESCRIPTOR_OFFSET  0x70
#define TSS_DESCRIPTOR_OFFSET             0x80

typedef struct {
   UINT16 Offset1; // offset bits 0..15
   UINT16 Selector; // a code segment selector in GDT or LDT
   UINT8  Ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   UINT8  TypeAttr; // type and attributes
   UINT16 Offset2; // offset bits 16..31
   UINT32 Offset3; // offset bits 32..63
   UINT32 Zero;     // reserved
} AMD64_IDT_GATE_DESCRIPTOR;

/**
 Initialize Gdt for processor.

 @param Cpl0GdtTssTable To give the Gdt Tss Table
 @param TableSize Gdt table Size

 @return Status
         EFI_INVALID_PARAMETER Cpl0GdtTssTable input zero, or TableSize unexpected value
**/
EFI_STATUS
EFIAPI
InitGdtTable (
  IN EFI_PHYSICAL_ADDRESS Cpl0GdtTssTable,
  IN UINTN TableSize,
  IN UINTN Cpl0CallGateOffset,
  IN UINTN Cpl0StackPoint
  )
{
  UINTN                     TssBase;
  UINTN                     Cpl0GdtTssTableSize;
  IA32_SEGMENT_DESCRIPTOR   *GdtDescriptor;
  AMD64_IDT_GATE_DESCRIPTOR *CallgateDesc;
  UINTN Cpl0CallGateIretOffset = (UINTN)GetDefaultInterruptHandlerReturnAddress ();

  Cpl0GdtTssTableSize = (mSmiGdtr.Limit + 1 + TSS_SIZE + 7) & ~7; // 8 bytes aligned
  if (Cpl0GdtTssTable == 0 || TableSize != Cpl0GdtTssTableSize) {
    return EFI_INVALID_PARAMETER;
  }
  // DEBUG((DEBUG_INFO, "Copy Cpl0GdtTssTable %x <- mSmiGdtr.Base %x Size %x\n", (UINTN)Cpl0GdtTssTable, (UINTN)mSmiGdtr.Base, TableSize));
  CopyMem ((VOID*)(UINTN)Cpl0GdtTssTable, (VOID*)(UINTN)mSmiGdtr.Base, TableSize);
  //
  // Fixup TSS descriptors
  //
  // TSS Descriptor just after GDT
  TssBase = (UINTN)(Cpl0GdtTssTable + mSmiGdtr.Limit + 1);
  // TSS is last entry in GDT table, X64 is double sized IA32 GdtDesc, so substract 2
  GdtDescriptor = (IA32_SEGMENT_DESCRIPTOR *)(TssBase) - 2;
  GdtDescriptor->Bits.BaseLow = (UINT16)(UINTN)TssBase;
  GdtDescriptor->Bits.BaseMid = (UINT8)((UINTN)TssBase >> 16);
  GdtDescriptor->Bits.BaseHigh = (UINT8)((UINTN)TssBase >> 24);
  // Prepare for returning from ring 3 via CALL GATE
  CallgateDesc = (AMD64_IDT_GATE_DESCRIPTOR *) (Cpl0GdtTssTable + CALL_GATE_DESCRIPTOR_OFFSET);
  // Initialize CALL GATE Descriptor
  // In 64-bit mode, the codesegment descriptor base-address and limit fields are ignored. The target virtual-address is the 64-bit
  // offset field in the expanded call-gate descriptor.
  // Initial the offset as Cpl0CallGateOffset, when the call gate been called, it will jump to Cpl0CallGateOffset
    CallgateDesc->Offset1 = (UINT16) (Cpl0CallGateOffset & 0xFFFF);
    CallgateDesc->Offset2 = (UINT16) ((Cpl0CallGateOffset >> 16) & 0xFFFF);
    CallgateDesc->Offset3 = (UINT32) (Cpl0CallGateOffset >> 32);

    // Register address to transfer from CPL3 to CPL0 in interrupt handler for default exception handler.
    CallgateDesc = (AMD64_IDT_GATE_DESCRIPTOR *) (Cpl0GdtTssTable + CALL_GATE_IRET_DESCRIPTOR_OFFSET);
    CallgateDesc->Offset1 = (UINT16) (Cpl0CallGateIretOffset & 0xFFFF);
    CallgateDesc->Offset2 = (UINT16) ((Cpl0CallGateIretOffset >> 16) & 0xFFFF);
    CallgateDesc->Offset3 = (UINT32) (Cpl0CallGateIretOffset >> 32);

  // Initialize TSS segment
  // When switching to more-privileged software, as is done when transferring control using a call gate, the
  // processor uses the corresponding stack pointer (privilege-level 0, 1, or 2) stored in the task-state
  // segment (TSS).
  // Initial the Ring 0 call gate stack point
  //TssDescriptor:
      // DD      0                   ; Reserved
      // DQ      0                   ; RSP0
  *(UINT64 *) (TssBase + X64_TSS_RSP0_OFFSET) = Cpl0StackPoint;
  //Allocate the stack for exception IST
  //UINTN is to fixup, check File: ..\SmmSupervisorPkg\SmmSupervisor\SmmSupervisorProtocol.c for details
  // 406:   Cpl0Stack =  Cpl0Stack + SMM_CPL0_OVERALL_STACK_SIZE - sizeof (UINTN); //Substract UINTN to make SMI entry asm generic
  // Stack allocation map can be referred in the File: ..\SmmSupervisorPkg\SmmSupervisor\SmmSupervisor.h
  *(UINT64 *) (TssBase + X64_TSS_IST1_OFFSET + (sizeof (UINT64) * (SMMSUPV_STACK_IST_INDEX - 1))) = ALIGN_VALUE (Cpl0StackPoint, 0x1000) - SMM_CPL0_STACK_SIZE ;
  return EFI_SUCCESS;
}

/**
 Get Gdt table base and size.

 @param CpuIndex Input Cpu index

 @param *GdtTablebase Output Gdt table base

 @param *TableSize Output Gdt table Size

 @return Status
         EFI_UNSUPPORTED - Cannot find the GDT table base and size from this CpuIndex

**/
EFI_STATUS
EFIAPI
GetGdt (
  IN     UINT64 CpuIndex,
  IN OUT EFI_PHYSICAL_ADDRESS *GdtTablebase,
  IN OUT UINTN *TableSize
  )
{

  *GdtTablebase = mSmmSupvGdtData[CpuIndex].Tablebase;
  *TableSize    = mSmmSupvGdtData[CpuIndex].TableSize;

  if (*GdtTablebase == 0 || *TableSize == 0) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}