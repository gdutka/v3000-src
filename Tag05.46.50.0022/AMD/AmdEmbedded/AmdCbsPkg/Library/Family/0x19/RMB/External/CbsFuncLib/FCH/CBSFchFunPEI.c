/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "CBSFchFunPEI.h"

PEI_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(FchPeiFunTable,_RMB)[] =
{
  {
    0,        // F19_RMB_RAW_ID,
    0,        // AMD_RESERVE_ALL,
    NULL,     // CBS_FUNC_DECLARE(FchFunc_Init_PEI,_RMB),
    0         // CBS_CALLOUT_ID
  }

};

