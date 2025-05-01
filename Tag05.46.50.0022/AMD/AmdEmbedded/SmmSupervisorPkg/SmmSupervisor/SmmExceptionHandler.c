/** @file
Agent Module to load other modules to deploy SMM Entry Vector for X86 CPU.

Copyright (c) 2009 - 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Pi/PiFirmwareFile.h>

#include <Protocol/DebugSupport.h>
#include <Protocol/LoadedImage.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/SmmSupervisorExceptionHandlerLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PeCoffGetEntryPointLib.h>

#include "SmmSupervisor.h"

#define     SUPV_IDT_LIMIT    (sizeof(IA32_IDT_GATE_DESCRIPTOR) * 32) - 1
UINT64      mIDTBaseBuffer     [EFI_SIZE_TO_PAGES(SUPV_IDT_LIMIT + 1) * EFI_PAGE_SIZE];

/**
  Search module name by input IP address and output it.

  @param CallerIpAddress   Caller instruction pointer.

**/
VOID
DumpModuleInfoByIp (
  IN  UINTN              CallerIpAddress
  )
{
  UINTN                                Pe32Data;
  VOID                                 *PdbPointer;

  //
  // Find Image Base
  //
  Pe32Data = PeCoffSearchImageBase (CallerIpAddress);
  if (Pe32Data != 0) {
    DEBUG ((DEBUG_ERROR, "It is invoked from the instruction before IP(0x%p)", (VOID *) CallerIpAddress));
    PdbPointer = PeCoffLoaderGetPdbPointer ((VOID *) Pe32Data);
    if (PdbPointer != NULL) {
      DEBUG ((DEBUG_ERROR, " in module (%a)\n", PdbPointer));
    }
  }
}

/**
  This function is for GpException handler from SMM supervisor.

  @param ExceptionType  Exception type.
  @param SystemContext  Pointer to EFI_SYSTEM_CONTEXT.

**/
STATIC
VOID
SmmSupervisorGpExceptionHandler (
  IN EFI_EXCEPTION_TYPE   InterruptType,
  IN EFI_SYSTEM_CONTEXT   SystemContext
  )
{
  BOOLEAN HandledException = FALSE;

  //If Handled, set HandledException.

  if (HandledException) {
    SetHandledExceptionFlag (InterruptType, SystemContext);
  }
}

/**
  Initialize spin lock for exception handler.
**/
STATIC
EFI_STATUS
InitExceptionHandlerSpinLock (
  VOID
)
{
  UINTN                    SpinLockSize;
  EFI_STATUS               Status;

  if (mCpuToken != NULL) {
    Status = EFI_ALREADY_STARTED;
    goto Exit;
  }

  SpinLockSize = GetSpinLockProperties ();
  mCpuToken = AllocatePool (SpinLockSize);

  if (mCpuToken == NULL) {
    WriteErrorData (SMMSUPV_INIT_SPINLOCK_ERROR);
    ASSERT (mCpuToken != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  InitializeSpinLock (mCpuToken);
  Status = EFI_SUCCESS;

Exit:
  return Status;
}

/**
  Initialize IDT to setup exception handlers for SMM.
**/
VOID
InitializeSmmIdt (
  IA32_DESCRIPTOR          *NewIdtr
  )
{
  EFI_STATUS               Status;
  BOOLEAN                  InterruptState;
  IA32_DESCRIPTOR          OldIdtr;

  if (NewIdtr == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = InitExceptionHandlerSpinLock ();
  if (Status != EFI_SUCCESS && Status != EFI_ALREADY_STARTED) {
    WriteErrorData (SMMSUPV_INIT_EXCEPTION_HANDLELER_SPINLOCK_FAIL);
    DEBUG ((DEBUG_ERROR, "%a - Failed to initialize spin lock for exception handler - %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // There are 32 (not 255) entries in it since only processor
  // generated exceptions will be handled.
  //
  NewIdtr->Limit = SUPV_IDT_LIMIT;
  //
  // Allocate page aligned IDT, because it might be set as read only.
  //
  NewIdtr->Base = (UINTN) mIDTBaseBuffer;
  ZeroMem ((VOID *)mIDTBaseBuffer, NewIdtr->Limit + 1);
  //
  // Disable Interrupt and save DXE IDT table
  //
  InterruptState = SaveAndDisableInterrupts ();
  AsmReadIdtr (&OldIdtr);
  //
  // Load SMM temporary IDT table
  //
  AsmWriteIdtr (NewIdtr);
  //
  // Setup SMM default exception handlers, SMM IDT table
  // will be updated and saved in mSmiIdtr
  //
  Status = InitializeCpuExceptionHandlers (NULL);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INIT_CPU_EXCEPTION_HANDLELER_FAIL);
    DEBUG ((DEBUG_ERROR, "%a - Failed to initialize handler - %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // Register GP fault handler with CPL0 here, note that this should be the one and only
  // handler under CPL0.
  //

  Status = RegisterCpuInterruptHandler (EXCEPT_IA32_GP_FAULT, SmmSupervisorGpExceptionHandler);
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_REGISTER_CPU_INTERRUPT_HANDLELER_FAIL);
    DEBUG ((DEBUG_ERROR, "%a - Failed to register gp handler - %r\n", __FUNCTION__, Status));
    goto Exit;
  }

  //
  // Restore DXE IDT table and CPU interrupt
  //
  AsmWriteIdtr ((IA32_DESCRIPTOR *) &OldIdtr);
  SetInterruptState (InterruptState);

  Status = EFI_SUCCESS;

Exit:
  if (EFI_ERROR (Status)) {
    WriteErrorData (SMMSUPV_INIT_SMM_IDT_FAIL);
    // Critical path, we hang here if unsuccessful
    ASSERT_EFI_ERROR (Status);
    CpuDeadLoop ();
  }
}
