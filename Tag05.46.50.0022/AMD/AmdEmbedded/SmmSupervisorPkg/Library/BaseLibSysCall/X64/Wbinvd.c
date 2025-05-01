/** @file
  CpuBreakpoint function.

  Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
  Copyright (C), Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/**
  Microsoft Visual Studio 7.1 Function Prototypes for I/O Intrinsics.
**/

#include    <Uefi.h>
#include    <Library/BaseLib.h>
#include    <Library/SysCallLib.h>

VOID
EFIAPI
AsmWbinvdCPL0 (
  VOID
);

/**
  Executes a WBINVD instruction.

  Executes a WBINVD instruction. This function is only available on IA-32 and
  x64.

**/
VOID
EFIAPI
AsmWbinvd (
  VOID
  )
{
  UINT16 CS;

  CS = AsmReadCs();

  if ((CS & CPL_BITMASK) == SYSCALL_REQUIRED_CPL) {
    SysCall (SMM_SC_WBINVD, 0, 0, 0);
  }
  else {
    AsmWbinvdCPL0 ();
  }
}

