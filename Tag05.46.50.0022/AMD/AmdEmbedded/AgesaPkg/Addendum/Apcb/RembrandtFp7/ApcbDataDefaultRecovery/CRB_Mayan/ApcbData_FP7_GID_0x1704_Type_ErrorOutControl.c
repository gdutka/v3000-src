/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                            // GroupId
  APCB_MEM_TYPE_ERROR_OUT_EVENT_CONTROL,        // TypeId
  sizeof(ApcbTypeHeader),       // SizeOfType, will be fixed up by tool
  0,                            // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,        // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK             // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool


///< Platform Specific Overrides
PSP_ERROR_OUT_CONTROL_STRUCT PspEventOutControl = {
  FALSE,
  FALSE,
  FALSE,
  FALSE,
  0x84,
  15000,
  0x80,
  FALSE,
  4,
  4,
  6,
  6,
  FALSE,
  {
    85,
    0x01,
    0xC0
  },
  {
    {BEEP_ERROR_TYPE_GENERAL|BEEP_ERROR_OPERATION_MASK,     0x5555,   {3, 3, 0}},
    {BEEP_ERROR_TYPE_MEMORY|BEEP_ERROR_OPERATION_MASK,      0x5555,   {5, 3, 0}},
    {BEEP_ERROR_TYPE_DF|BEEP_ERROR_OPERATION_MASK,          0x5555,   {7, 3, 0}},
    {BEEP_ERROR_TYPE_CCX|BEEP_ERROR_OPERATION_MASK,         0x5555,   {9, 3, 0}},
    {BEEP_ERROR_TYPE_GNB|BEEP_ERROR_OPERATION_MASK,         0x5555,   {11, 3, 0}},
    {BEEP_ERROR_TYPE_PSP|BEEP_ERROR_OPERATION_MASK,         0x5555,   {13, 3, 0}},
    {BEEP_ERROR_TYPE_SMU|BEEP_ERROR_OPERATION_MASK,         0x5555,   {15, 3, 0}},
    {BEEP_ERROR_TYPE_UNKNOW|BEEP_ERROR_OPERATION_MASK,      0x5555,   {3, 3, 0}}
  },
  FALSE
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

