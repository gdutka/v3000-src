/** @file
  Base Library CPU Functions for all architectures.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/



#include <Base.h>
#include <Library/BaseLib.h>
//[-start-220301-IB14740220-add]//
#include <Library/DebugLib.h>
//[-end-220301-IB14740220-add]//

//[-start-220301-IB14740220-add]//
#ifdef H2O_CPU_DEADLOOP
/**
  Executes an infinite loop.

  Forces the CPU to execute an infinite loop. A debugger may be used to skip
  past the loop and the code that follows the loop must execute properly. This
  implies that the infinite loop must not cause the code that follow it to be
  optimized away.

  @param[in]  FileName          The pointer to the name of the source file that call CpuDeadLoop.
  @param[in]  LineNumber        The line number in the source file that call CpuDeadLoop.
**/
VOID
EFIAPI
H2OCpuDeadLoop (
  IN  CONST CHAR8         *FileName,
  IN  UINTN               LineNumber
  )
{
  volatile UINTN  Index;

  DEBUG ((DEBUG_ERROR, "%a (Line: %d)\n", (FileName == NULL) ? "Unknown File" : FileName, LineNumber));
  for (Index = 0; Index == 0;);
}

#else
//[-end-220301-IB14740220-add]//
/**
  Executes an infinite loop.

  Forces the CPU to execute an infinite loop. A debugger may be used to skip
  past the loop and the code that follows the loop must execute properly. This
  implies that the infinite loop must not cause the code that follow it to be
  optimized away.

**/
VOID
EFIAPI
CpuDeadLoop (
  VOID
  )
{
  volatile UINTN  Index;

  for (Index = 0; Index == 0;);
}
//[-start-220301-IB14740220-add]//
#endif
//[-end-220301-IB14740220-add]//
