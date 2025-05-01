/**

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

UINT64
EFIAPI
AsmWriteMsr64Worker (
  IN UINT32  Index,
  IN UINT64  Value
  );


/**
  Write data to MSR.

  @param  Index                The register index of MSR.
  @param  Value                Data wants to be written.

  @return Value written to MSR.

**/
UINT64
EFIAPI
AsmWriteMsr64 (
  IN UINT32  Index,
  IN UINT64  Value
  )
{
  UINT16 CS;

  CS = AsmReadCs();

  if ((CS & CPL_BITMASK) == SYSCALL_REQUIRED_CPL) {
    SysCall (SMM_SC_WRMSR, Index, (UINTN) Value, 0);
  }
  else {
    AsmWriteMsr64Worker (Index, Value);
  }
  
  return Value;
}

