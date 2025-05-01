/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_GET_CORE_COF_H_
#define _AMD_GET_CORE_COF_H_

#define AMD_MSR_PSTATE_STS        0xC0010063
#define CURRENT_PSTATE_MASK       0x00000007
#define AMD_MSR_PSTATE_0          0xC0010064

#ifndef AMD_CPUID_FMF
  #define AMD_CPUID_FMF             0x80000001ul  // Family Model Features information
#endif

#ifndef CPUID_EXT_FAMILY_MASK
  #define CPUID_EXT_FAMILY_MASK     0x0FF00000ul
#endif
#ifndef CPUID_EXT_FAMILY_OFFSET
  #define CPUID_EXT_FAMILY_OFFSET   20
#endif

#ifndef CPUID_BASE_FAMILY_MASK
  #define CPUID_BASE_FAMILY_MASK    0x00000F00ul
#endif
#ifndef CPUID_BASE_FAMILY_OFFSET
  #define CPUID_BASE_FAMILY_OFFSET  8
#endif

#define FAMILY_19                   0x19
#define FAMILY_1A                   0x1A

///MSR
/// Z3, Z4 P-state MSR
typedef union {
  struct {                             ///< Bitfields of P-state MSR
    UINT64 CpuFid_7_0:8;               ///< CpuFid[7:0]
    UINT64 CpuDfsId:6;                 ///< CpuDfsId
    UINT64 CpuVid:8;                   ///< CpuVid
    UINT64 IddValue:8;                 ///< IddValue
    UINT64 IddDiv:2;                   ///< IddDiv
    UINT64 :31;                        ///< Reserved
    UINT64 PstateEn:1;                 ///< Pstate Enable
  } Field;
  UINT64  Value;
} PSTATE_MSR_V1;

/**
  Amd method to get current operating frequency of a given clock domain in MHz.

  @return The Core current operating frequency in MHz.

**/
UINT64
AmdGetCoreCOF (
  VOID
  );

#endif
