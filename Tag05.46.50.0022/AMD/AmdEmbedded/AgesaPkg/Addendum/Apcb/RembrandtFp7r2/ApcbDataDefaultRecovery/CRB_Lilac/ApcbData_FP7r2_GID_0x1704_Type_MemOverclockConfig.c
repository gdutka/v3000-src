/*****************************************************************************
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * @e \$Revision:$   @e \$Date:$
 */

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

//MEM OVERCLOCK MATRIX
APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_MEM_OVERCLOCK_CONFIG,         // TypeId
  sizeof(ApcbTypeHeader),        // SizeOfType, will be fixed up by tool
  0,                             // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,    // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK         // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

MEM_OC_CONFIG_STRUCT  MemOcVddioConfigStruct =
{
  MEM_OC_CONFIG_VDDIO,
  {
    MEM_OC_CONTROL_METHOD_EN,
    0x04,
    {MEM_OC_CONTROL_ACCESS_TYPE_INDEX_IO, 0x72, 0x73, 0xA0},
    {MEM_OC_CONTROL_ACCESS_TYPE_DIS,      0x72, 0x73, 0xA1},
    {
      {0x666, 0x662, 0x90, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0xF8, 0x00},
      {0x666, 0x662, 0x91, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_UPDATABLE,   0x00, 0x00},
      {0x666, 0x662, 0x92, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0x60, 0x00},
      {0x666, 0x662, 0x92, MEM_OC_CONTROL_EC_PULL_NE, MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0xCC, 0x00},
    }
  }
};

MEM_OC_CONFIG_STRUCT  MemOcVddConfigStruct =
{
  MEM_OC_CONFIG_VDD,
  {
    MEM_OC_CONTROL_METHOD_EN,
    0x04,
    {MEM_OC_CONTROL_ACCESS_TYPE_INDEX_IO, 0x72, 0x73, 0xA2},
    {MEM_OC_CONTROL_ACCESS_TYPE_DIS,      0x72, 0x73, 0xA3},
    {
      {0x666, 0x662, 0x90, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0xFB, 0x00},
      {0x666, 0x662, 0x91, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_UPDATABLE,   0x00, 0x00},
      {0x666, 0x662, 0x92, MEM_OC_CONTROL_EC_WRITE,   MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0x60, 0x00},
      {0x666, 0x662, 0x92, MEM_OC_CONTROL_EC_PULL_NE, MEM_OC_CONTROL_EC_ATTRIBUTE_FIXED,       0xCC, 0x00},
    }
  }
};

#if 0
MEM_OC_SAFE_BOOT_CONFIG_STRUCT MemOcSafeBootConfigStruct =
{
  MEM_OC_CONFIG_SAFE_BOOT,
  {
    MEM_OC_SAFE_BOOT_CONTROL_METHOD_DIS,
    0x72, 0x73, 0x04,
    0x55,
    0xAA,
    0,0
  }
};
#endif

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();
