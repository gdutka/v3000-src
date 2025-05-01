/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "CBSFchFunDXE.h"
#include "AmdSoc.h"

DXE_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(FchDxeFunTable,_RMB)[] =
{
  {
    F19_RMB_RAW_ID,
    AMD_RESERVE_ALL,
    CBS_FUNC_DECLARE(FchInitEnvParams,_RMB),
    IDS_HOOK_FCH_INIT_ENV
  },

  {
    0,        // F19_RMB_RAW_ID,
    0,        // AMD_RESERVE_ALL,
    NULL,     // CBS_FUNC_DECLARE(FchFunc_Init_DXE,_RMB),
    0         // CBS_CALLOUT_ID
  }
};

VOID
CBS_FUNC_DECLARE(FchInitEnvParams,_RMB) (
  IN EFI_BOOT_SERVICES        *BootServices,
  IN OUT  VOID                *DataParams,
  IN AMD_SYSTEM_CONFIGURATION *SystemConfiguration
  )
{
}


