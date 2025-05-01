/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHTASKLAUNCHER_FILECODE

AGESA_STATUS
FchTaskLauncher (
  IN       FCH_TASK_ENTRY     **TaskPtr,
  IN       VOID               *FchCfg,
  IN       AGESA_POST_CODE           TestPoint
  )
{
  AGESA_TESTPOINT (TestPoint, *(AMD_CONFIG_PARAMS **) FchCfg);
  while (*TaskPtr != NULL) {
    (*TaskPtr) (FchCfg);
    TaskPtr++;
  }
  return AGESA_SUCCESS;
}


