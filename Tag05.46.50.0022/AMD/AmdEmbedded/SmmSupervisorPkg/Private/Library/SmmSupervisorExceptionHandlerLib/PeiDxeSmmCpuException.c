/** @file
  CPU Exception Library provides PEI/DXE/SMM CPU common exception handler.

Copyright (c) 2012 - 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/CcExitLib.h>
#include <Library/SmmSupervisorPageMgmtLib.h>
#include <Library/SmmSupvTestPointLib.h>
#include "CpuExceptionCommon.h"

#define TSS_SIZE              104
#define TSS_CPL0_STACK_ADJUSTMENT   0x100

#define ALIGN_TO_PAGE(Value) ((Value) & ~(EFI_PAGE_SIZE - 1))

SPIN_LOCK mTssStackLock;

extern VOID *mExceptionHandlerRetunAddress;

VOID
FlushTlbMemory (IN EFI_PHYSICAL_ADDRESS Memory);

VOID
FlushTlbRange (IN EFI_PHYSICAL_ADDRESS MemoryStart, IN UINTN Length)
{
  EFI_PHYSICAL_ADDRESS Memory = MemoryStart;
  EFI_PHYSICAL_ADDRESS MemoryEnd = MemoryStart + Length;

  while (Memory < MemoryEnd) {
    FlushTlbMemory (Memory);
    Memory += EFI_PAGE_SIZE;
  }
}

UINT64
GetRsp (
  VOID
);

VOID
TransferToDefaultExceptionHandler (
  IN EFI_EXCEPTION_TYPE            InterruptType,
  IN EFI_SYSTEM_CONTEXT            SystemContext,
  IN EFI_PHYSICAL_ADDRESS          Cpl3StackPointer
);


EFI_CPU_INTERRUPT_HANDLER     mDefaultInterruptHandler = NULL;

/**
  Execute default handler.

  @param ExceptionType         Exception type.
  @param SystemContext         Pointer to EFI_SYSTEM_CONTEXT.
**/
VOID
ExecuteDefaultHandler (
  IN EFI_EXCEPTION_TYPE          ExceptionType,
  IN EFI_SYSTEM_CONTEXT          SystemContext
)
{
  UINT64              GdtBase;
  UINT8               TssDescSeg;
  IA32_TSS_DESCRIPTOR *TssDesc;
  EFI_PHYSICAL_ADDRESS TssBase;
  EFI_PHYSICAL_ADDRESS PagedAlignedTssBase;
  UINT64               PageAlignedTssSize;
  UINT64               OldTaskCpl0Stack;
  UINT64               NewTaskCpl0Stack;
  UINT64               SystemContextRsp;
  UINT64               Cpl3StackPointer;
  EFI_SYSTEM_CONTEXT   Cpl3SystemContext;

  //
  //  If code is in ring 0, just execute the handler and return.
  //
  if ((SystemContext.SystemContextX64->Cs & 3) == 0) {
    mDefaultInterruptHandler (ExceptionType, SystemContext);
    return;
  }

  // CPU is in ring 3, copy SystemContext to user page.
  SystemContextRsp                  = SystemContext.SystemContextX64->Rsp;
  Cpl3SystemContext.SystemContextX64 = (EFI_SYSTEM_CONTEXT_X64*)((SystemContextRsp & 0xfffffff0) - sizeof (EFI_SYSTEM_CONTEXT_X64));
  CopyMem (Cpl3SystemContext.SystemContextX64, SystemContext.SystemContextX64, sizeof (EFI_SYSTEM_CONTEXT_X64));
  Cpl3StackPointer = (UINT64)Cpl3SystemContext.SystemContextX64;  // Set CPL3 stack below copy of SystemContext.

  // Get the base and size and the aligned values of TSS.
  GdtBase                       = SystemContext.SystemContextX64->Gdtr[0];
  TssDescSeg                    = (UINT8)SystemContext.SystemContextX64->Tr;
  TssDesc                       = (IA32_TSS_DESCRIPTOR *)((UINT8*)GdtBase + TssDescSeg);
  TssBase                       = (EFI_PHYSICAL_ADDRESS)((TssDesc->Bits.BaseMidh << 24) + (TssDesc->Bits.BaseMidl << 16) + TssDesc->Bits.BaseLow);
  PagedAlignedTssBase           = ALIGN_TO_PAGE (TssBase);
  PageAlignedTssSize            = ALIGN_VALUE (TssBase + TSS_SIZE - PagedAlignedTssBase, EFI_PAGE_SIZE);

  // Save the original stack pointer.
  OldTaskCpl0Stack              =  *(UINT64*)((UINT8*)TssBase + 4);

  // Only one processor at a time can change TSS stack. New TSS stack is updated to be lower than IDT stack for Syscall/Sysret.
  while (!AcquireSpinLockOrFail (&mTssStackLock)) {
    CpuPause ();
  }
  SmmClearMemoryAttributes (PagedAlignedTssBase, PageAlignedTssSize, EFI_MEMORY_RO, TRUE);
  FlushTlbRange (PagedAlignedTssBase, PageAlignedTssSize);

  // Set TSS Stack to current Interrupt CPL 0 Stack and move stack down due a small stack usage when transferring to CPL3 function. Align stack to 16 bytes.
  NewTaskCpl0Stack = (GetRsp () - TSS_CPL0_STACK_ADJUSTMENT) & 0xfffffffffffffff0;
  *(UINT64*)(TssBase + 4) = NewTaskCpl0Stack;
  SmmSetMemoryAttributes (PagedAlignedTssBase, PageAlignedTssSize, EFI_MEMORY_RO, TRUE);
  FlushTlbRange (PagedAlignedTssBase, PageAlignedTssSize);
  ReleaseSpinLock (&mTssStackLock);

  TransferToDefaultExceptionHandler (ExceptionType, Cpl3SystemContext, Cpl3StackPointer);

  while (!AcquireSpinLockOrFail (&mTssStackLock)) {
    CpuPause ();
  }
  SmmClearMemoryAttributes (PagedAlignedTssBase, PageAlignedTssSize, EFI_MEMORY_RO, TRUE);
  FlushTlbRange (PagedAlignedTssBase, PageAlignedTssSize);
  *(UINT64*)((UINT8*)TssBase + 4) = OldTaskCpl0Stack;
  SmmSetMemoryAttributes (PagedAlignedTssBase, PageAlignedTssSize, EFI_MEMORY_RO, TRUE);
  FlushTlbRange (PagedAlignedTssBase, PageAlignedTssSize);
  ReleaseSpinLock (&mTssStackLock);
}

/**
  Internal worker function for common exception handler.

  @param ExceptionType         Exception type.
  @param SystemContext         Pointer to EFI_SYSTEM_CONTEXT.
  @param ExceptionHandlerData  Pointer to exception handler data.
**/
VOID
CommonExceptionHandlerWorker (
  IN EFI_EXCEPTION_TYPE      ExceptionType,
  IN EFI_SYSTEM_CONTEXT      SystemContext,
  IN EXCEPTION_HANDLER_DATA  *ExceptionHandlerData
  )
{
  EXCEPTION_HANDLER_CONTEXT  *ExceptionHandlerContext;
  RESERVED_VECTORS_DATA      *ReservedVectors;
  EFI_CPU_INTERRUPT_HANDLER  *ExternalInterruptHandler;

  ExceptionHandlerContext  = (EXCEPTION_HANDLER_CONTEXT *) (UINTN) (SystemContext.SystemContextX64);
  ReservedVectors          = ExceptionHandlerData->ReservedVectors;
  ExternalInterruptHandler = ExceptionHandlerData->ExternalInterruptHandler;

  switch (ReservedVectors[ExceptionType].Attribute) {
    case EFI_VECTOR_HANDOFF_HOOK_BEFORE:
      //
      // The new exception handler registered by RegisterCpuInterruptHandler() is executed BEFORE original handler.
      // Save the original handler to stack so the assembly code can jump to it instead of returning from handler.
      //
      ExceptionHandlerContext->ExceptionDataFlag = (mErrorCodeFlag & (1 << ExceptionType)) ? TRUE : FALSE;
      ExceptionHandlerContext->OldIdtHandler     = ReservedVectors[ExceptionType].ExceptonHandler;
      break;
    case EFI_VECTOR_HANDOFF_HOOK_AFTER:
      while (TRUE) {
        //
        // If spin-lock can be acquired, it's the first time entering here.
        //
        if (AcquireSpinLockOrFail (&ReservedVectors[ExceptionType].SpinLock)) {
          //
          // The new exception handler registered by RegisterCpuInterruptHandler() is executed AFTER original handler.
          // Save the original handler to stack but skip running the new handler so the original handler is executed
          // firstly.
          //
          ReservedVectors[ExceptionType].ApicId = GetApicId ();
          ArchSaveExceptionContext (ExceptionType, SystemContext, ExceptionHandlerData);
          ExceptionHandlerContext->ExceptionDataFlag = (mErrorCodeFlag & (1 << ExceptionType)) ? TRUE : FALSE;
          ExceptionHandlerContext->OldIdtHandler     = ReservedVectors[ExceptionType].ExceptonHandler;
          return;
        }

        //
        // If spin-lock cannot be acquired, it's the second time entering here.
        // 'break' instead of 'return' is used so the new exception handler can be executed.
        //
        if (ReservedVectors[ExceptionType].ApicId == GetApicId ()) {
          //
          // Old IDT handler has been executed, then restore CPU exception content to
          // run new exception handler.
          //
          ArchRestoreExceptionContext (ExceptionType, SystemContext, ExceptionHandlerData);
          //
          // Release spin lock for ApicId
          //
          ReleaseSpinLock (&ReservedVectors[ExceptionType].SpinLock);
          break;
        }

        CpuPause ();
      }

      break;
    case 0xffffffff:
      break;
    default:
      //
      // It should never reach here
      //
      WriteErrorDataAndReset (SMMSUPV_TRIGGER_EXCEPTION_FAIL_RESET);
      CpuDeadLoop ();
      break;
  }
  TESTPOINT (SMMSUPV_EXCEPTION_BASE | ((UINT8) ExceptionType));
  if (ExternalInterruptHandler != NULL &&
      ExternalInterruptHandler[ExceptionType] != NULL) {
    (ExternalInterruptHandler[ExceptionType]) (ExceptionType, SystemContext);
  }

  //
  // If exception is not handled, dump Context, execute default handler if available, then hang or reset.
  //

  if (ExceptionType < CPU_EXCEPTION_NUM && ExceptionHandlerContext->ExceptionHandled == 0) {
    //
    // Get Spinlock to display CPU information
    //
    while (!AcquireSpinLockOrFail (&ExceptionHandlerData->DisplayMessageSpinLock)) {
      CpuPause ();
    }
    //
    // Initialize the serial port before dumping.
    //
    SerialPortInitialize ();
    //
    // Display ExceptionType, CPU information and Image information
    //
    DumpImageAndCpuContent (ExceptionType, SystemContext);
    //
    // Release Spinlock of output message
    //
    ReleaseSpinLock (&ExceptionHandlerData->DisplayMessageSpinLock);

    if (mDefaultInterruptHandler != NULL) {
      ExecuteDefaultHandler (ExceptionType, SystemContext);
    }

    //
    // Enter a dead loop if needn't to execute old IDT handler further
    //
    if (ReservedVectors[ExceptionType].Attribute != EFI_VECTOR_HANDOFF_HOOK_BEFORE) {
      if (mExceptionHandlerRetunAddress == NULL) {
        WriteErrorDataAndReset (SMMSUPV_TRIGGER_EXCEPTION_FAIL_RESET);
        CpuDeadLoop ();
      }

     // Update return address on system context.
      SystemContext.SystemContextX64->Rip = (UINTN)mExceptionHandlerRetunAddress;
    }
  }
}

/**
  Internal worker function to update IDT entries accordling to vector attributes.

  @param[in] IdtTable              Pointer to IDT table.
  @param[in] TemplateMap           Pointer to a buffer where the address map is
                                   returned.
  @param[in] ExceptionHandlerData  Pointer to exception handler data.

**/
VOID
UpdateIdtTable (
  IN IA32_IDT_GATE_DESCRIPTOR        *IdtTable,
  IN EXCEPTION_HANDLER_TEMPLATE_MAP  *TemplateMap,
  IN EXCEPTION_HANDLER_DATA          *ExceptionHandlerData
  )
{
  UINT16                 CodeSegment;
  UINTN                  Index;
  UINTN                  InterruptHandler;
  RESERVED_VECTORS_DATA  *ReservedVectors;

  ReservedVectors = ExceptionHandlerData->ReservedVectors;
  //
  // Use current CS as the segment selector of interrupt gate in IDT
  //
  CodeSegment = AsmReadCs ();

  for (Index = 0; Index < ExceptionHandlerData->IdtEntryCount; Index++) {
    IdtTable[Index].Bits.Selector = CodeSegment;
    //
    // Check reserved vectors attributes
    //
    switch (ReservedVectors[Index].Attribute) {
      case EFI_VECTOR_HANDOFF_DO_NOT_HOOK:
        //
        // Keep original IDT entry
        //
        continue;
      case EFI_VECTOR_HANDOFF_HOOK_AFTER:
        InitializeSpinLock (&ReservedVectors[Index].SpinLock);
        CopyMem (
          (VOID *)ReservedVectors[Index].HookAfterStubHeaderCode,
          (VOID *)TemplateMap->HookAfterStubHeaderStart,
          TemplateMap->ExceptionStubHeaderSize
          );
        AsmVectorNumFixup (
          (VOID *)ReservedVectors[Index].HookAfterStubHeaderCode,
          (UINT8)Index,
          (VOID *)TemplateMap->HookAfterStubHeaderStart
          );
      //
      // Go on the following code
      //
      case EFI_VECTOR_HANDOFF_HOOK_BEFORE:
        //
        // Save original IDT handler address
        //
        ReservedVectors[Index].ExceptonHandler = ArchGetIdtHandler (&IdtTable[Index]);
      //
      // Go on the following code
      //
      default:
        //
        // Update new IDT entry
        //
        InterruptHandler = TemplateMap->ExceptionStart + Index * TemplateMap->ExceptionStubHeaderSize;
        ArchUpdateIdtEntry (&IdtTable[Index], InterruptHandler);
        break;
    }
  }
}

/**
  Internal worker function to initialize exception handler.

  @param[in]      VectorInfo            Pointer to reserved vector list.
  @param[in, out] ExceptionHandlerData  Pointer to exception handler data.

  @retval EFI_SUCCESS           CPU Exception Entries have been successfully initialized
                                with default exception handlers.
  @retval EFI_INVALID_PARAMETER VectorInfo includes the invalid content if VectorInfo is not NULL.
  @retval EFI_UNSUPPORTED       This function is not supported.

**/
EFI_STATUS
InitializeCpuExceptionHandlersWorker (
  IN EFI_VECTOR_HANDOFF_INFO     *VectorInfo OPTIONAL,
  IN OUT EXCEPTION_HANDLER_DATA  *ExceptionHandlerData
  )
{
  EFI_STATUS                      Status;
  IA32_DESCRIPTOR                 IdtDescriptor;
  UINTN                           IdtEntryCount;
  EXCEPTION_HANDLER_TEMPLATE_MAP  TemplateMap;
  IA32_IDT_GATE_DESCRIPTOR        *IdtTable;
  RESERVED_VECTORS_DATA           *ReservedVectors;

  InitializeSpinLock (&mTssStackLock);

  ReservedVectors = ExceptionHandlerData->ReservedVectors;
  SetMem ((VOID *)ReservedVectors, sizeof (RESERVED_VECTORS_DATA) * CPU_EXCEPTION_NUM, 0xff);
  if (VectorInfo != NULL) {
    Status = ReadAndVerifyVectorInfo (VectorInfo, ReservedVectors, CPU_EXCEPTION_NUM);
    if (EFI_ERROR (Status)) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Read IDT descriptor and calculate IDT size
  //
  AsmReadIdtr (&IdtDescriptor);
  IdtEntryCount = (IdtDescriptor.Limit + 1) / sizeof (IA32_IDT_GATE_DESCRIPTOR);
  if (IdtEntryCount > CPU_EXCEPTION_NUM) {
    //
    // CPU exception library only setup CPU_EXCEPTION_NUM exception handler at most
    //
    IdtEntryCount = CPU_EXCEPTION_NUM;
  }

  IdtTable = (IA32_IDT_GATE_DESCRIPTOR *)IdtDescriptor.Base;
  AsmGetTemplateAddressMap (&TemplateMap);
  ASSERT (TemplateMap.ExceptionStubHeaderSize <= HOOKAFTER_STUB_SIZE);

  ExceptionHandlerData->IdtEntryCount = IdtEntryCount;
  UpdateIdtTable (IdtTable, &TemplateMap, ExceptionHandlerData);

  return EFI_SUCCESS;
}

/**
  Interrupt handler reports that exception was handled.

  An Interrupt handler, such as GPF or PF, may be handle the interrupt and return. Otherwise, it is unrecoverable. This is function is called if interrupt is handled.

  @param[in]  ExceptionType     Exception type of interrupt handler.
  @param[in]  SystemContext     System context of interrupt generated.
**/
VOID
EFIAPI
SetHandledExceptionFlag (
  IN EFI_EXCEPTION_TYPE          ExceptionType,
  IN EFI_SYSTEM_CONTEXT          SystemContext
  )
{
  EXCEPTION_HANDLER_CONTEXT      *ExceptionHandlerContext =  (EXCEPTION_HANDLER_CONTEXT *) (UINTN) (SystemContext.SystemContextX64);
  ExceptionHandlerContext->ExceptionHandled = 1;
}

/**
  Registers a function to be called from the processor interrupt handler.

  @param[in]  InterruptType        Defines which interrupt or exception to hook.
  @param[in]  InterruptHandler     A pointer to a function of type EFI_CPU_INTERRUPT_HANDLER that is called
                                   when a processor interrupt occurs. If this parameter is NULL, then the handler
                                   will be uninstalled
  @param[in] ExceptionHandlerData  Pointer to exception handler data.

  @retval EFI_SUCCESS           The handler for the processor interrupt was successfully installed or uninstalled.
  @retval EFI_ALREADY_STARTED   InterruptHandler is not NULL, and a handler for InterruptType was
                                previously installed.
  @retval EFI_INVALID_PARAMETER InterruptHandler is NULL, and a handler for InterruptType was not
                                previously installed.
  @retval EFI_UNSUPPORTED       The interrupt specified by InterruptType is not supported,
                                or this function is not supported.
**/
EFI_STATUS
RegisterCpuInterruptHandlerWorker (
  IN EFI_EXCEPTION_TYPE         InterruptType,
  IN EFI_CPU_INTERRUPT_HANDLER  InterruptHandler,
  IN EXCEPTION_HANDLER_DATA     *ExceptionHandlerData
  )
{
  UINTN                      EnabledInterruptNum;
  RESERVED_VECTORS_DATA      *ReservedVectors;
  EFI_CPU_INTERRUPT_HANDLER  *ExternalInterruptHandler;

  EnabledInterruptNum      = ExceptionHandlerData->IdtEntryCount;
  ReservedVectors          = ExceptionHandlerData->ReservedVectors;
  ExternalInterruptHandler = ExceptionHandlerData->ExternalInterruptHandler;

  if (InterruptType == -1) {
    mDefaultInterruptHandler = InterruptHandler;
  }

  if ((InterruptType < 0) || (InterruptType >= (EFI_EXCEPTION_TYPE)EnabledInterruptNum) ||
      (ReservedVectors[InterruptType].Attribute == EFI_VECTOR_HANDOFF_DO_NOT_HOOK))
  {
    return EFI_UNSUPPORTED;
  }

  if ((InterruptHandler == NULL) && (ExternalInterruptHandler[InterruptType] == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((InterruptHandler != NULL) && (ExternalInterruptHandler[InterruptType] != NULL)) {
    return EFI_ALREADY_STARTED;
  }

  ExternalInterruptHandler[InterruptType] = InterruptHandler;
  return EFI_SUCCESS;
}
