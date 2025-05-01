/**
 * @file
 *
 * Contains common CPPC definitions used among different AMD IP
 *
 */
/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _AMD_CPPC_COMMON_H_
#define _AMD_CPPC_COMMON_H_

#pragma pack (push, 1)

typedef enum {
  CPC_HIGHEST_PERF_INDEX            = 0 ,
  CPC_NOMINAL_PERF_INDEX            = 1 ,
  CPC_LOWEST_NONLINEAR_PERF_INDEX   = 2 ,
  CPC_LOWEST_PERF_INDEX             = 3 ,
  CPC_GUARANTEED_PERF_INDEX         = 4 ,
  CPC_DESIRED_PERF_INDEX            = 5 ,
  CPC_MIN_PERF_INDEX                = 6 ,
  CPC_MAX_PERF_INDEX                = 7 ,
  CPC_PERF_REDUCTION_TOL_INDEX      = 8 ,
  CPC_TIME_WINDOW_INDEX             = 9 ,
  CPC_COUNTER_WRAP_TIME_INDEX       = 10,
  CPC_NOMINAL_COUNT_INDEX           = 11,
  CPC_DELIVERED_COUNT_INDEX         = 12,
  CPC_PERF_LIMITED_INDEX            = 13,
  CPC_ENABLE_INDEX                  = 14,
  CPC_AUTONOMOUS_SEL_INDEX          = 15,
  CPC_AUTONOMOUS_ACT_INDEX          = 16,
  CPC_ENERGY_PERF_PREF_INDEX        = 17,
  CPC_REF_PERF_INDEX                = 18,
  CPC_LOWEST_FREQ_INDEX             = 19,
  CPC_NOMINAL_FREQ_INDEX            = 20,
  CPC_UNKNOWN                       = 0xFF
} CPPC_ACPI_ENTRY_INDEX;

#pragma pack (pop)
#endif /* _AMD_CPPC_COMMON_H_ */


