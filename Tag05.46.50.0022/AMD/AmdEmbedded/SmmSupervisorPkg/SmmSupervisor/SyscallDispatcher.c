/** @file
Agent Module to load other modules to deploy SMM Entry Vector for X86 CPU.
Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>
#include <SmmSecurePolicy.h>

#include <Protocol/SmmCpuIo2.h>

#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/SysCallLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmPolicyGateLib.h>
#include <Library/MemLogLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Library/IhvPageSupervisionLib.h>
#include <Library/IhvSmmSaveStateSupervisionLib.h>
#include "SmmSupervisor.h"

CHAR8*
FindSyscallName (
  UINTN         CallIndex
  )
{
  UINT32 i;
  UINT32 Length;
  Length = sizeof(SyscallIdNamePairs);
  for (i = 0; i < Length; i++) {
    if (SyscallIdNamePairs[i].Id == CallIndex) {
      return SyscallIdNamePairs[i].Name;
    }
  }

  return NULL;
}

/**
  Conduct Syscall dispatch.
**/
UINT64
EFIAPI
SyscallDispatcher (
  UINTN         CallIndex,
  UINTN         Arg1,
  UINTN         Arg2,
  UINTN         Arg3,
  UINTN         CallerAddr,
  BOOLEAN       *JumpToErrorHandler
  )
{
  UINT64      ApicBar;
  BOOLEAN     IsBsp;
  UINT64      Ret;
  BOOLEAN     IgnorePrint;
  GATELIB_CPU_SMM_DATA  CpuSmmData;
  EFI_STATUS  Status = EFI_SUCCESS;

  SMM_SUPV_SECURE_POLICY_DATA_V1_0 *SmmPolicyData = GetSmmPolicyData ();

  Ret = 0;

  IgnorePrint = FALSE;
  if ((CallIndex == SMM_SC_IO_READ && Arg1 == 0x3FD) ||
      (CallIndex == SMM_SC_IO_WRITE && Arg1 == 0x3F8)) {
    IgnorePrint = TRUE;
  }
  if (IgnorePrint == FALSE) {
    while (!AcquireSpinLockOrFail (mCpuToken)) {
      CpuPause ();
    }

    ApicBar = AsmReadMsr64 (0x0000001B);
    IsBsp = (BOOLEAN) ((ApicBar & BIT8) != 0);

    MEMLOG ("%a Enter... CallIndex: %a (%lx), IsBsp: %a, Arg1: %lx, Arg2: %lx, Arg3: %lx, CallerAddr: %p\n",
      __FUNCTION__,
      FindSyscallName (CallIndex),
      CallIndex,
      (IsBsp == TRUE ? "TRUE" : "FALSE"),
      Arg1,
      Arg2,
      Arg3,
      CallerAddr
      );
    ReleaseSpinLock (mCpuToken);
  }
  // The real policy come from DRTM event is copied over to SmmPolicyData
  switch (CallIndex) {
  case SMM_SC_RDMSR:
    Status = IsMsrReadWriteAllowed (
               SmmPolicyData,
               (UINT32)Arg1,
               SECURE_POLICY_RESOURCE_ATTR_READ
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Read MSR 0x%p blocked by policy - %r\n", __FUNCTION__, Arg1, Status));
      goto Exit;
    }
    Ret = AsmReadMsr64 ((UINT32)Arg1);
    DEBUG ((DEBUG_VERBOSE, "%a Read MSR %x got %x\n", __FUNCTION__, Arg1, Ret));
    break;
  case SMM_SC_WRMSR:
    Status = IsMsrReadWriteAllowed (
               SmmPolicyData,
               (UINT32)Arg1,
               SECURE_POLICY_RESOURCE_ATTR_WRITE
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Write MSR 0x%p blocked by policy - %r\n", __FUNCTION__, Arg1, Status));
      goto Exit;
    }
    AsmWriteMsr64 ((UINT32)Arg1, (UINT64)Arg2);
    DEBUG ((DEBUG_VERBOSE, "%a Write MSR %x with %x\n", __FUNCTION__, Arg1, Arg2));
    break;
  case SMM_SC_CLI:
    Status = IsInstructionExecutionAllowed (SmmPolicyData,
                                            SECURE_POLICY_INSTRUCTION_CLI);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Instruction execution CLI blocked by policy - %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    DisableInterrupts ();
    DEBUG ((DEBUG_VERBOSE, "%a Disable interrupts\n", __FUNCTION__));
    break;
  case SMM_SC_IO_READ:
    DEBUG ((DEBUG_VERBOSE, "%a Read IO type %d at %x got ", __FUNCTION__, Arg2, Arg1));
    if (Arg2 != SMM_IO_UINT8 && Arg2 != SMM_IO_UINT16 && Arg2 != SMM_IO_UINT32) {
      DEBUG ((DEBUG_ERROR, "%a Read IO incompatible size - %d\n", __FUNCTION__, Arg2));
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    Status = IsIoReadWriteAllowed(SmmPolicyData,
                                  (UINT32) Arg1,
                                  (EFI_SMM_IO_WIDTH) Arg2,
                                  SECURE_POLICY_RESOURCE_ATTR_READ);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Read IO port 0x%x with width type %d blocked by policy - %r\n", __FUNCTION__, Arg1, Arg2, Status));
      goto Exit;
    }
    if (Arg2 == SMM_IO_UINT8) {
      Ret = (UINT64) IoRead8 ((UINTN)Arg1);
    } else if (Arg2 == SMM_IO_UINT16) {
      Ret = (UINT64) IoRead16 ((UINTN)Arg1);
    } else if (Arg2 == SMM_IO_UINT32) {
      Ret = (UINT64) IoRead32 ((UINTN)Arg1);
    } else {
      // Should not happen
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    DEBUG ((DEBUG_VERBOSE, "%x\n", Ret));
    break;
  case SMM_SC_IO_WRITE:
    if (Arg2 != SMM_IO_UINT8 && Arg2 != SMM_IO_UINT16 && Arg2 != SMM_IO_UINT32) {
      DEBUG ((DEBUG_ERROR, "%a Read IO incompatible size - %d\n", __FUNCTION__, Arg2));
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    Status = IsIoReadWriteAllowed (
              SmmPolicyData,
              (UINT32) Arg1,
              (EFI_SMM_IO_WIDTH) Arg2,
              SECURE_POLICY_RESOURCE_ATTR_WRITE_DIS
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Write IO port 0x%x with width type %d blocked by policy - %r\n", __FUNCTION__, Arg1, Arg2, Status));
      goto Exit;
    }
    if (Arg2 == SMM_IO_UINT8) {
      IoWrite8 ((UINTN)Arg1, (UINT8)Arg3);
    } else if (Arg2 == SMM_IO_UINT16) {
      IoWrite16 ((UINTN)Arg1, (UINT16)Arg3);
    } else if (Arg2 == SMM_IO_UINT32) {
      IoWrite32 ((UINTN)Arg1, (UINT32)Arg3);
    } else {
      // Should not happen
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    DEBUG ((DEBUG_VERBOSE, "%a Write IO type %d at %x with %x\n", __FUNCTION__, Arg2, Arg1, Arg3));
    break;
  case SMM_SC_WBINVD:
    Status = IsInstructionExecutionAllowed (SmmPolicyData,
                                            SECURE_POLICY_INSTRUCTION_WBINVD);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Instruction execution WBINVD blocked by policy - %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_VERBOSE, "%a Write back and invalidate cache\n", __FUNCTION__));
    AsmWbinvd ();
    break;
  case SMM_SC_HLT:
    Status = IsInstructionExecutionAllowed (SmmPolicyData,
                                            SECURE_POLICY_INSTRUCTION_HLT);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Instruction execution HLT blocked by policy - %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_VERBOSE, "%a Cpu Halt\n", __FUNCTION__));
    CpuSleep ();
    break;
  case SMM_SC_SVST_READ:
    DEBUG ((DEBUG_VERBOSE, "%a Save state read CpuIndex:%d SmmSaveStateAddr:0x%x Width:%d\n", __FUNCTION__, Arg1, Arg2, Arg3));
    Ret = 0;
    //Arg 1 Cpu Index
    //Arg 2 SMM SAVESTATE register Address
    //Arg 3 Access size
    // Do not read if CpuIndex is out of bound or caller reaches out of save state region.
    if ((Arg1 < mSmmSupvCpuData.NumberOfCpus) &&
        (Arg2 >= mSmmSupvCpuData.SmBase[Arg1] + SMM_SAVE_STATE_OFFSET) &&
        ((Arg2 + Arg3) <= (mSmmSupvCpuData.SmBase[Arg1] + SMM_SAVE_STATE_OFFSET + SMM_SAVE_STATE_SIZE)) &&
        (Arg3 <= sizeof(UINT64))) {
      //Prepare the GATELIB_CPU_SMM_DATA for supervision lib
      CpuSmmData.Version = SMM_SAVE_STATE_CPU_DATA_VERSION_1;
      CpuSmmData.NumberOfCpus = mSmmSupvCpuData.NumberOfCpus;
      CpuSmmData.SmBase = mSmmSupvCpuData.SmBase;
      DEBUG ((DEBUG_VERBOSE, "SavestateOffset:0x%x\n",  Arg2 - mSmmSupvCpuData.SmBase[Arg1]));
      Status = IsIhvSmmSaveStateReadAllowed (SmmPolicyData, Arg1, Arg2, Arg3, &CpuSmmData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a SavestateRead Blocked by Policy - %r\n", __FUNCTION__, Status));
        DumpModuleInfoByIp (CallerAddr);
        goto Exit;
      }
      IhvSmmSaveStateRead (&Ret, Arg1, Arg2, Arg3);
    } else {
      Status = EFI_SECURITY_VIOLATION;
      goto Exit;
    }
    break;
  case SMM_SC_IHV_SUPV_READ:
    Ret = 0;
    //Arg 1 the IHV supervised region base
    //Arg 2 the the offset to IHV supervised region base
    DEBUG ((DEBUG_VERBOSE, "%a IHV supervised region READ %x - offset %x\n", __FUNCTION__, Arg1, Arg2));
    Status = IsIhvSupervisedReadAllowed (Arg1, Arg2);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a IHV supervised region read blocked by Level30 mandatory policy - %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    Ret = (UINT64) IhvSupervisedRead (Arg1, Arg2);
    break;
  case SMM_SC_IHV_SUPV_WRITE:
    //Arg 1 the IHV supervised region base
    //Arg 2 the offset to IHV supervised region base
    //Arg 3 the 32bits value write to the offset
    DEBUG ((DEBUG_VERBOSE, "%a IHV supervised region Write %x - offset %x size %x\n", __FUNCTION__, Arg1, Arg2, Arg3));
    Status = IsIhvSupervisedWriteAllowed (Arg1, Arg2);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a IHV supervised region write blocked by Level30 mandatory policy - %r\n", __FUNCTION__, Status));
      goto Exit;
    }
    if (Arg3 > 0xFFFFFFFFul) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_ERROR, "%a PspMmioWrite invalid value \n", __FUNCTION__));
      goto Exit;
    }
    IhvSupervisedWrite (Arg1, Arg2, (UINT32) Arg3);
    break;
  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

Exit:
  if (EFI_ERROR (Status)) {
    if (mEndOfPolicyViolationAction == SMM_SV_END_OF_POLICY_VIOLATION_ACTION_DEFAULT) {
      // Atomically increment the count for invalid syscalls, and avoid overflow.
      InterlockedIncrement (&mSyscallViolationCnt);
    }
    MEMLOG ("ERROR! It is invoked from the instruction before IP(0x%p)", (VOID *) CallerAddr);
    // More error handling here
    if (mExternalErrorLogger != NULL) {
      *JumpToErrorHandler = TRUE;
      return 0;
    }

    DumpModuleInfoByIp (CallerAddr);
    WriteErrorData (SMMSUPV_SYSCALL_FAIL);
    ASSERT_EFI_ERROR (Status);
    CpuDeadLoop ();
  }

  DEBUG ((DEBUG_VERBOSE, "%a Exit...\n", __FUNCTION__));
  return Ret;
}
