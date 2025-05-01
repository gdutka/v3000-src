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
  APCB_MEM_TYPE_MEM_OVERCLOCK_MATRIX,         // TypeId
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

MEM_OVERCLOCK_MATRIX_ENTRY MemOverclockMatrix [] = {
  // Format :
  // MemoryFreq,       FabricFreq,       VddpVoltage,   VddcrSocVid
  //
  {DDR2000_FREQUENCY, DDR2000_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR2400_FREQUENCY, DDR2400_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR3200_FREQUENCY, DDR3200_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR3400_FREQUENCY, DDR3400_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR3600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR3800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR4000_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR4200_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR4400_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR4600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR4800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR5000_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR5200_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR5400_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR5600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR5800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR6000_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR6200_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR6400_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR6600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR6800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR7000_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR7200_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR7400_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR7600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR7800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR8000_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR8200_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR8400_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR8600_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
  {DDR8800_FREQUENCY, DDR3600_FREQUENCY, 0xFFFF,        0xFFFF },
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();




