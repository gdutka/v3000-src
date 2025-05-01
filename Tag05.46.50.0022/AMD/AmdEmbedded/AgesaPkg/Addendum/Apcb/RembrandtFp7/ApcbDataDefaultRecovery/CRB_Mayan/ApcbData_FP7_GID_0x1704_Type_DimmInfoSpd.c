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
  APCB_GROUP_MEMORY,                    // GroupId
  APCB_MEM_TYPE_DIMM_INFO_SMBUS,        // TypeId
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


DIMM_INFO_SMBUS  DimmInfoSmbus [] =
{
  //
  // P0 configs
  //
  FALSE, 0, 0, 0, BLDCFG_SPD_P0_CH_A_DIMM0_ADDRESS, BLDCFG_I2C_MUX_P0_ADRRESS, BLDCFG_I2C_MUX_P0_COMMAND_CODE, BLDCFG_I2C_MUX_P0_CH_A_DIMM0_DATA_BYTE,
  FALSE, 0, 0, 1, BLDCFG_SPD_P0_CH_A_DIMM1_ADDRESS, BLDCFG_I2C_MUX_P0_ADRRESS, BLDCFG_I2C_MUX_P0_COMMAND_CODE, BLDCFG_I2C_MUX_P0_CH_A_DIMM1_DATA_BYTE,

  FALSE, 0, 1, 0, BLDCFG_SPD_P0_CH_B_DIMM0_ADDRESS, BLDCFG_I2C_MUX_P0_ADRRESS, BLDCFG_I2C_MUX_P0_COMMAND_CODE, BLDCFG_I2C_MUX_P0_CH_B_DIMM0_DATA_BYTE,
  FALSE, 0, 1, 1, BLDCFG_SPD_P0_CH_B_DIMM1_ADDRESS, BLDCFG_I2C_MUX_P0_ADRRESS, BLDCFG_I2C_MUX_P0_COMMAND_CODE, BLDCFG_I2C_MUX_P0_CH_B_DIMM1_DATA_BYTE,

};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

