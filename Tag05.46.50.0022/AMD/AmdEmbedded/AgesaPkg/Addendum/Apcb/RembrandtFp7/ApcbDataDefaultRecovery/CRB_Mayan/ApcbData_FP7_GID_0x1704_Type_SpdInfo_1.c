/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <ApcbMemGroup.h>
#include <ApcbMemSpdInfo.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#define BLDCFG_MICRON_8GB_DR_SPD_DATA  \
  0x23, 0x11, 0x13, 0x0E, 0x85, 0x19, 0xB5, 0x18, 0x00, 0x40, 0x00, 0x00, 0x0A, 0x02, 0x00, 0x00,\
  0x48, 0x00, 0x03, 0x0F, 0x92, 0x54, 0x05, 0x00, 0xAA, 0x00, 0x90, 0xA8, 0x90, 0x90, 0x06, 0xC0,\
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0xC1, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x54, 0x36, 0x32, 0x46, 0x31, 0x47,\
  0x33, 0x32, 0x44, 0x34, 0x44, 0x52, 0x2D, 0x30, 0x33, 0x31, 0x20, 0x57, 0x54, 0x00, 0x80, 0x2C,\
  0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_SPD_INFO,                     // TypeId
  sizeof(ApcbTypeHeader),        // SizeOfType, will be fixed up by tool
  1,                             // InstanceId
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

PSP_SPD_STRUCT  memPspSpd = {
  APCB_TOTAL_SYSTEM_DIMMS,  //< TotalDimms - Total number of DIMMs in the system
  {
    //
    // Socket 0, Channel 0, DIMM 0
    //
    BLDCFG_DRAMDOWN_CH_A_DIMM_0_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_A_DIMM_0_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA0,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                 //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    0,                                                //< ChannelNumber - Channel Number
    0,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 0, DIMM 1
    //
    BLDCFG_DRAMDOWN_CH_A_DIMM_1_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_A_DIMM_1_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA2,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                 //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    0,                                                //< ChannelNumber - Channel Number
    1,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 1, DIMM 0
    //
    BLDCFG_DRAMDOWN_CH_B_DIMM_0_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_B_DIMM_0_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA4,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                 //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    1,                                                //< ChannelNumber - Channel Number
    0,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 1, DIMM 1
    //
    BLDCFG_DRAMDOWN_CH_B_DIMM_1_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_B_DIMM_1_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA6,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                 //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    1,                                                //< ChannelNumber - Channel Number
    1,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 2, DIMM 0
    //
    BLDCFG_DRAMDOWN_CH_C_DIMM_0_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_C_DIMM_0_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA4,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                  //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                    //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    2,                                                //< ChannelNumber - Channel Number
    0,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 2, DIMM 1
    //
    BLDCFG_DRAMDOWN_CH_C_DIMM_1_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_C_DIMM_1_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA6,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                  //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                    //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    2,                                                //< ChannelNumber - Channel Number
    1,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },

    //
    // Socket 0, Channel 3, DIMM 0
    //
    BLDCFG_DRAMDOWN_CH_D_DIMM_0_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_D_DIMM_0_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA4,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                  //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                    //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    3,                                                //< ChannelNumber - Channel Number
    0,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    },
    //
    // Socket 0, Channel 3, DIMM 1
    //
    BLDCFG_DRAMDOWN_CH_D_DIMM_1_SPD_VALID,            //< SpdValid - Indicates that the SPD is valid
    BLDCFG_DRAMDOWN_CH_D_DIMM_1_DIMM_PRESENT,         //< DimmPresent - Indicates that the DIMM is present and Data is valid
    0,                                                //< PageAddress - Indicates that the DIMM is present and Data is valid
    0,                                                //< NvDimmPresent
    0,                                                //< DramManufacturersIDCode
    0,                                                //< Module Manufacture ID
    0xA6,                                             //< Address - Indicates the SMBUS address of the DIMM
    {0, 0, 0, 0},                                     //< SMBUS Mux Info
    LPDDR5_TECHNOLOGY,                                  //< Technology - Indicates the type of technology supported
    DRAMDOWN_PACKAGE,                                    //< Package - Package type
    0,                                                //< SocketNumber - Socket Number
    3,                                                //< ChannelNumber - Channel Number
    1,                                                //< DimmNumber - DIMM Number
    0,                                                //< Reserved
    {
      BLDCFG_MICRON_8GB_DR_SPD_DATA
    }
  }
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

