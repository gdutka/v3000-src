/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "CbsIDSCommonPei.h"
extern PEI_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(IdsPeiFunTable,_Common)[];

extern PEI_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(FchPeiFunTable,_RMB)[];

PEI_CBS_COMPONENT_STRUCT CbsCommonFuncTable[] =
{
  {0xFFFFFFFF,    AMD_RESERVE_ALL, (PEI_CBS_FUNCTION_STRUCT*) CBS_FUNCTABLE_DECLARE(IdsPeiFunTable,_Common) },
  {F19_RMB_RAW_ID, AMD_RESERVE_ALL, (PEI_CBS_FUNCTION_STRUCT*) CBS_FUNCTABLE_DECLARE(FchPeiFunTable,_RMB) },

  //
  // end of table.
  //
  {0, 0, NULL}
};

