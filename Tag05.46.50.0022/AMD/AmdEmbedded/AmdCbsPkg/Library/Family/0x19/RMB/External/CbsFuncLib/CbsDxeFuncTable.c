/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "CBSFchFunDXE.h"
#include "AmdSoc.h"
#include "CbsDxe.h"
#include "CbsIDSCommonDxe.h"

extern DXE_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(IdsDxeFunTable,_Common)[];

extern DXE_CBS_FUNCTION_STRUCT CBS_FUNCTABLE_DECLARE(FchDxeFunTable,_RMB)[];

DXE_CBS_COMPONENT_STRUCT CbsCommonFuncTable[] =
{
  {0xFFFFFFFF,    AMD_RESERVE_ALL, (DXE_CBS_FUNCTION_STRUCT*) CBS_FUNCTABLE_DECLARE(IdsDxeFunTable,_Common) },

  {F19_RMB_RAW_ID, AMD_RESERVE_ALL, (DXE_CBS_FUNCTION_STRUCT*) CBS_FUNCTABLE_DECLARE(FchDxeFunTable,_RMB) },

  //
  // end of table.
  //
  {0, 0, NULL}
};

