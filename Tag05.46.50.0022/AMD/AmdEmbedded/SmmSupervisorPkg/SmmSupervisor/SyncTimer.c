/** @file
SMM Timer feature support

Copyright (c) 2009 - 2015, Intel Corporation. All rights reserved.<BR>
Copyright (C) Microsoft Corporation.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include "SmmSupervisor.h"

#define   SUPV_NORM_SYNC_TIMEOUT_US     5000
#define   SUPV_DRTM_SYNC_SCALAR         2000
#define   SUPV_DRTM_SYNC_TIMEOUT_US     (SUPV_NORM_SYNC_TIMEOUT_US * SUPV_DRTM_SYNC_SCALAR)

UINT64   mTimeoutTicker = 0;
//
//  Number of counts in a roll-over cycle of the performance counter.
//
UINT64   mCycle = 0;
//
// Flag to indicate the performance counter is count-up or count-down.
//
BOOLEAN  mCountDown;

/**
  Initialize Timer for SMM AP Sync.

**/
VOID
InitializeSmmTimer (
  VOID
  )
{
  UINT64  TimerFrequency;
  UINT64  Start;
  UINT64  End;

  // This is hardcoded to be 5ms, default EDK2 value, to avoid absorbing new pcd
  TimerFrequency = GetPerformanceCounterProperties (&Start, &End);
  mTimeoutTicker = DivU64x32 (
                     MultU64x64(TimerFrequency, SUPV_NORM_SYNC_TIMEOUT_US),
                     1000 * 1000
                     );
  if (End < Start) {
    mCountDown = TRUE;
    mCycle = Start - End;
  } else {
    mCountDown = FALSE;
    mCycle = End - Start;
  }
}

/*
  This is a helper function to allow processor to evaluate whether this
  synchronization needs to wait longer instead of normal timeout
*/
BOOLEAN
EFIAPI
NeedsExtendedWait (
  IN UINT32           DrtmBootFlag
)
{
  BOOLEAN Ret;

  Ret = FALSE;

  // In general, whenever we need to call PostDrtmEntry routine, the wait
  // needs to be forever, otherwise AP will risk missing IDT and GDT reinit
  // and exposed security concerns
  if (DrtmBootFlag ||
      (mSmmIsoEnableEvent == SMM_ISO_ENABLE_EVENT_READY_TO_BOOT &&
       mSmmSupervisorPostReadyToLock == SMM_SUPV_PRE_LOCK_VALUE) ) {
    // If SMM isolation event has hit OR DRTM flag is set, we wait longer/forever
    Ret = TRUE;
  }

  return Ret;
}

/**
  Start Timer for SMM AP Sync.

**/
UINT64
EFIAPI
StartSyncTimer (
  VOID
  )
{
  return GetPerformanceCounter ();
}


/**
  Check if the SMM AP Sync timer is timeout.

  @param Timer  The start timer from the begin.

**/
BOOLEAN
EFIAPI
IsSyncTimerTimeout (
  IN      UINT64                    Timer,
  IN      UINT32                    DrtmBootFlag
  )
{
  UINT64  CurrentTimer;
  UINT64  Delta;
  UINT64  TimeoutTicker;

  CurrentTimer = GetPerformanceCounter ();
  //
  // We need to consider the case that CurrentTimer is equal to Timer
  // when some timer runs too slow and CPU runs fast. We think roll over
  // condition does not happen on this case.
  //
  if (mCountDown) {
    //
    // The performance counter counts down.  Check for roll over condition.
    //
    if (CurrentTimer <= Timer) {
      Delta = Timer - CurrentTimer;
    } else {
      //
      // Handle one roll-over.
      //
      Delta = mCycle - (CurrentTimer - Timer) + 1;
    }
  } else {
    //
    // The performance counter counts up.  Check for roll over condition.
    //
    if (CurrentTimer >= Timer) {
      Delta = CurrentTimer - Timer;
    } else {
      //
      // Handle one roll-over.
      //
      Delta = mCycle - (Timer - CurrentTimer) + 1;
    }
  }

  if (NeedsExtendedWait (DrtmBootFlag)) {
    TimeoutTicker = mTimeoutTicker * SUPV_DRTM_SYNC_SCALAR;
  }
  else {
    TimeoutTicker = mTimeoutTicker;
  }

  return (BOOLEAN) (Delta >= TimeoutTicker);
}
