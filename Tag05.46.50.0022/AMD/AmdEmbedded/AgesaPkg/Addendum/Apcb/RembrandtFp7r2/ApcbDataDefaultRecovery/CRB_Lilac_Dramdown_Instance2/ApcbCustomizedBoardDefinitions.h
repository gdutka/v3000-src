/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

//
// OEM CUSTOMIZABLE BOARD SPECIFIC DEFINITIONS. Any changes must be done here...
//

#ifndef _APCB_CUSTOMIZED_BOARD_DEFINITIONS_H_
#define _APCB_CUSTOMIZED_BOARD_DEFINITIONS_H_

// =================================================================================================
// IMPORTANT: Define board mask
//            If this file is copied to a board folder, then it applies to data files build for that board folder
// =================================================================================================
/*
  Possible value of board mask could be:
  1. Apply to single board:
     - APCB_BOARD_INSTANCE_0, APCB_BOARD_INSTANCE_1, ... or APCB_BOARD_INSTANCE_15
  2. Apply to multiple boards:
     - APCB_BOARD_INSTANCE_0 + APCB_BOARD_INSTANCE_1 + APCB_BOARD_INSTANCE_10
  3. Apply to all boards:      APCB_BOARD_INSTANCE_ALL_KNOWN
*/
#define BLDCFG_APCB_DATA_BOARD_MASK     APCB_BOARD_INSTANCE_2


// =================================================================================================
// APCB Token Value Customization
// =================================================================================================
/*
  To customize board specific setting, do undefine and redo define of the definition

  #undef APCB_TOKEN_UID_<CONTROL_NAME>
  #define APCB_TOKEN_UID_<CONTROL_NAME>  NewValue
*/


// =================================================================================================
// APCB Type Data Structure Member Customization
// =================================================================================================
/*
  To customize board specific setting, do undefine and redo define of the definition

  #undef BLDCFG_APCB_<DATA_CONTROL_NAME>
  #define BLDCFG_APCB_<DATA_CONTROL_NAME>  NewValue
*/

#include <ApcbDdr5Spd_H5CG46MEBDX_1R.h>

#define BLDCFG_SPD_INFO_DATA_BLOB_0  MAKE_SPD_INFO_REVISION(0)

#define BLDCFG_SPD_INFO_DATA_BLOB_2  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_A,DIMM_0,BLOCK_0,H5CG46MEBDX_1R_BLOCK_0)
#define BLDCFG_SPD_INFO_DATA_BLOB_3  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_A,DIMM_0,BLOCK_1,H5CG46MEBDX_1R_BLOCK_1)
#define BLDCFG_SPD_INFO_DATA_BLOB_4  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_A,DIMM_0,BLOCK_3,H5CG46MEBDX_1R_BLOCK_3)
#define BLDCFG_SPD_INFO_DATA_BLOB_5  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_A,DIMM_0,BLOCK_8,H5CG46MEBDX_1R_BLOCK_8)
#define BLDCFG_SPD_INFO_DATA_BLOB_6  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_B,DIMM_0,BLOCK_0,H5CG46MEBDX_1R_BLOCK_0)
#define BLDCFG_SPD_INFO_DATA_BLOB_7  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_B,DIMM_0,BLOCK_1,H5CG46MEBDX_1R_BLOCK_1)
#define BLDCFG_SPD_INFO_DATA_BLOB_8  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_B,DIMM_0,BLOCK_3,H5CG46MEBDX_1R_BLOCK_3)
#define BLDCFG_SPD_INFO_DATA_BLOB_9  MAKE_SPD_DATA_BLOCK(SOCKET_0,CHANNEL_B,DIMM_0,BLOCK_8,H5CG46MEBDX_1R_BLOCK_8)

#endif   //ifndef  _APCB_CUSTOMIZED_BOARD_DEFINITIONS_H_
