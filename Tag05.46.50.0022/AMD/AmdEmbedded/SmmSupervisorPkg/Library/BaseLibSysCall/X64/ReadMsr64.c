/** @file
  CpuBreakpoint function.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  Copyright (C), Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/**
  Microsoft Visual Studio 7.1 Function Prototypes for I/O Intrinsics.
**/

unsigned __int64 __readmsr (int register);

#pragma intrinsic(__readmsr)

#include    <Uefi.h>
#include    <Library/BaseLib.h>
#include    <Library/SysCallLib.h>

/**
  Read data to MSR.

  @param  Index                Register index of MSR.

  @return Value read from MSR.

**/
UINT64
EFIAPI
AsmReadMsr64 (
  IN UINT32  Index
  )
{
  UINT64 ret;
  UINT16 CS;

  CS = AsmReadCs();

  if ((CS & CPL_BITMASK) == SYSCALL_REQUIRED_CPL) {
    ret = SysCall (SMM_SC_RDMSR, Index, 0, 0);
  }
  else {
    ret = __readmsr (Index);
  }
  return ret;
}

