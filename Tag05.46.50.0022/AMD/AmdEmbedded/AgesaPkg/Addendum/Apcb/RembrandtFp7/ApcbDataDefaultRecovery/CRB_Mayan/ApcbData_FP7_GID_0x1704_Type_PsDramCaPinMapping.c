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
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_MEM_DRAM_CA_PIN_MAP,          // TypeId
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

DRAM_CA_PIN_MAP MemCaToPinMapping [] = {
  // MA_CA Mapping
  {
    // MAA/MAB
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
  },
  // MB_CA Mapping
  {
    // MBA/MBB
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
  },
  // MC_CA Mapping
  {
    // MCA/MCB
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
  },
  // MD_CA Mapping
  {
    // MDA/MDB
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
    { MEM_MX_CA_00, MEM_MX_CA_01, MEM_MX_CA_02, MEM_MX_CA_03, MEM_MX_CA_04, MEM_MX_CA_05, MEM_MX_CA_06 },
  }
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

