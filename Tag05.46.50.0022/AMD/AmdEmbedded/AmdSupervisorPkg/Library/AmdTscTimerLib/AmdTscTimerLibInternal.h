/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_TSC_TIMER_LIB_INTERNAL_H_
#define _AMD_TSC_TIMER_LIB_INTERNAL_H_

#include <AmdGetCoreCOF.h>

#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

/**  Get TSC frequency.

  @return The number of TSC counts per second.

**/
UINT64
InternalGetTscFrequency (
  VOID
  );

/**  Calculate TSC frequency.

  @return The number of TSC counts per second.

**/
UINT64
InternalCalculateTscFrequency (
  VOID
  );

#endif
