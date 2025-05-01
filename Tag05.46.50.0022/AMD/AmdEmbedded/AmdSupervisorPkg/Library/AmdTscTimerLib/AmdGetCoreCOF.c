/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "AmdTscTimerLibInternal.h"

/**
 * @brief Get Ccx P State Frequency (Zen3, Zen4 Architecture)
 * @details Zen4 and Zen3 P State Frequency are used
 * the same way to get P State Frequency from P State MSR
 * register. More details, please refer CcxGetPstateInfo func
 * in CcxPstatesZen3Lib.c, CcxPstatesZen4Lib.c
 *
 * @return The P-State's frequency in MegaHertz
 */
UINT64
GetCcxPstateFrequencyV1 (
  IN  UINT64  DedicatePstate
  )
{
  PSTATE_MSR_V1   PstateMsr;
  UINT32          CpuDfsId, CpuFid;
  UINT64          TscRateInMhz;

  // Preliminary Processor Programming Reference (PPR) for AMD Family 17h Model 60h, Revision A1 Processors
  // 2.1.2 Core Definitions Table 14: Definitions
  // Core current operating frequency in MHz. CoreCOF =
  // (Core::X86::Msr::PStateDef[CpuFid[7:0]]/Core::X86::Msr::PStateDef[CpuDfsId])*200

  TscRateInMhz = 0;

  PstateMsr.Value = AsmReadMsr64 (AMD_MSR_PSTATE_0 + (UINT32) DedicatePstate);

  CpuFid   = (UINT32) PstateMsr.Field.CpuFid_7_0;
  CpuDfsId = (UINT32) PstateMsr.Field.CpuDfsId;
  if (CpuDfsId == 0) {  // 00h : Off
    TscRateInMhz = 0;
  } else if ((CpuDfsId >= 8) && (CpuDfsId <= 0x3C)) { //CpuDfsId ValidValues: 0x08 ~ 0x30
    TscRateInMhz = (UINTN) ((200 * CpuFid) / CpuDfsId);
  }

  return TscRateInMhz;
}

VOID
GetDedicateStateFrequency (
  IN      UINT64                    DedicatePstate,
  OUT     UINT64                    *Frequency
  )
{
  // Z3, Z4 Architecture
  *Frequency = GetCcxPstateFrequencyV1 (DedicatePstate);
}

/**

  Amd method to get current operating frequency of a given clock domain in MHz.

  @return The Core current operating frequency in MHz.

**/
UINT64
AmdGetCoreCOF (
  VOID
  )
{
  UINT64               Frequency;
  UINT64               CurPstate;

  CurPstate = AsmReadMsr64 (AMD_MSR_PSTATE_STS) & CURRENT_PSTATE_MASK;

  GetDedicateStateFrequency (CurPstate, &Frequency);

  return Frequency;
}
