/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_ACPILIB_FILECODE


UINT8
ReadFchSleepType (
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT16     Value16;
  ReadPmio (FCH_PMIOA_REG62, AccessWidth16, &Value16, StdHeader);
  LibAmdIoRead (AccessWidth16, Value16, &Value16, StdHeader);
  return (UINT8) ((Value16 >> 10) & 7);
}


