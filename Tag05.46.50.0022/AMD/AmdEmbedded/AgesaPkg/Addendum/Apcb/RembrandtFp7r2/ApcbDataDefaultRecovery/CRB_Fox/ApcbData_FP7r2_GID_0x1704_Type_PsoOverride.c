/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#ifndef PSO_ENTRY
#define PSO_ENTRY UINT8
#endif

#define ANY_SOCKET     0xFF    ///<  Apply to all sockets
#define ANY_CHANNEL    0xFF    ///<  Apply to all Memory channels
#define ALL_DIMMS      0xFF    ///<  Apply to all dimm slots

#define LAYERS_4     0x0

#define PSO_END           0     ///< Table End
#define PSO_CKE_TRI       1     ///< CKE Tristate Map
#define PSO_ODT_TRI       2     ///< ODT Tristate Map
#define PSO_CS_TRI        3     ///< CS Tristate Map
#define PSO_MAX_DIMMS     4     ///< Max Dimms per channel
#define PSO_CLK_SPEED     5     ///< Clock Speed
#define PSO_DIMM_TYPE     6     ///< Dimm Type
#define PSO_MEMCLK_DIS    7     ///< MEMCLK Disable Map
#define PSO_MAX_CHNLS     8     ///< Max Channels per Socket
#define PSO_BUS_SPEED     9     ///< Max Memory Bus Speed
#define PSO_MAX_CHIPSELS 10     ///< Max Chipsel per Channel
#define PSO_MEM_TECH     11     ///< Channel Memory Type
#define PSO_MAX_SOLDERED_DOWN_DIMMS 19    ///< Max Soldered-down Dimms per channel
#define PSO_MEMORY_POWER_POLICY 20        ///< Memory power policy override
#define PSO_MOTHER_BOARD_LAYERS 21        ///< Mother board layer design

#define MEMCLK_DIS_MAP(SocketID, ChannelID, Bit0Map, Bit1Map, Bit2Map, Bit3Map, Bit4Map, Bit5Map, Bit6Map, Bit7Map) \
  PSO_MEMCLK_DIS, 11, SocketID, ChannelID, ALL_DIMMS, Bit0Map, Bit1Map, Bit2Map, Bit3Map, Bit4Map, Bit5Map, Bit6Map \
  , Bit7Map
#define CKE_TRI_MAP(SocketID, ChannelID, Bit0Map, Bit1Map, Bit2Map, Bit3Map) \
  PSO_CKE_TRI, 7, SocketID, ChannelID, ALL_DIMMS, Bit0Map, Bit1Map, Bit2Map, Bit3Map
#define ODT_TRI_MAP(SocketID, ChannelID, Bit0Map, Bit1Map, Bit2Map, Bit3Map) \
  PSO_ODT_TRI, 7, SocketID, ChannelID, ALL_DIMMS, Bit0Map, Bit1Map, Bit2Map, Bit3Map
#define CS_TRI_MAP(SocketID, ChannelID, Bit0Map, Bit1Map, Bit2Map, Bit3Map, Bit4Map, Bit5Map, Bit6Map, Bit7Map) \
  PSO_CS_TRI, 11, SocketID, ChannelID, ALL_DIMMS, Bit0Map, Bit1Map, Bit2Map, Bit3Map, Bit4Map, Bit5Map, Bit6Map, Bit7Map
#define NUMBER_OF_DIMMS_SUPPORTED(SocketID, ChannelID, NumberOfDimmSlotsPerChannel) \
  PSO_MAX_DIMMS, 4, SocketID, ChannelID, ALL_DIMMS, NumberOfDimmSlotsPerChannel
#define NUMBER_OF_SOLDERED_DOWN_DIMMS_SUPPORTED(SocketID, ChannelID, NumberOfSolderedDownDimmsPerChannel) \
  PSO_MAX_SOLDERED_DOWN_DIMMS, 4, SocketID, ChannelID, ALL_DIMMS, NumberOfSolderedDownDimmsPerChannel
#define NUMBER_OF_CHIP_SELECTS_SUPPORTED(SocketID, ChannelID, NumberOfChipSelectsPerChannel) \
  PSO_MAX_CHIPSELS, 4, SocketID, ChannelID, ALL_DIMMS, NumberOfChipSelectsPerChannel
#define NUMBER_OF_CHANNELS_SUPPORTED(SocketID, NumberOfChannelsPerSocket) \
  PSO_MAX_CHNLS, 4, SocketID, ANY_CHANNEL, ALL_DIMMS, NumberOfChannelsPerSocket
#define OVERRIDE_DDR_BUS_SPEED(SocketID, ChannelID, TimingMode, BusSpeed) \
  PSO_BUS_SPEED, 11, SocketID, ChannelID, ALL_DIMMS, TimingMode, (TimingMode >> 8), (TimingMode >> 16), (TimingMode >> 24), \
  BusSpeed, (BusSpeed >> 8), (BusSpeed >> 16), (BusSpeed >> 24)
#define DRAM_TECHNOLOGY(SocketID, MemTechType) \
  PSO_MEM_TECH, 7, SocketID, ANY_CHANNEL, ALL_DIMMS, MemTechType, (MemTechType >> 8), (MemTechType >> 16), (MemTechType >> 24)
#define MOTHER_BOARD_LAYERS(Layers) \
  PSO_MOTHER_BOARD_LAYERS, 4, ANY_SOCKET, ANY_CHANNEL, ALL_DIMMS, Layers

#define MEMCLK_DIS_MAP_SIZE  13
#define CKE_TRI_MAP_SIZE  9
#define ODT_TRI_MAP_SIZE  9
#define CS_TRI_MAP_SIZE 13
#define NUMBER_OF_DIMMS_SUPPORTED_SIZE  6
#define NUMBER_OF_SOLDERED_DOWN_DIMMS_SUPPORTED_SIZE  6
#define NUMBER_OF_CHIP_SELECTS_SUPPORTED_SIZE  6
#define NUMBER_OF_CHANNELS_SUPPORTED_SIZE  6
#define OVERRIDE_DDR_BUS_SPEED_SIZE  13
#define DRAM_TECHNOLOGY_SIZE  9
#define SOLDERED_DOWN_SODIMM_TYPE_SIZE  6
#define MEMORY_POWER_POLICY_OVERRIDE_SIZE  6
#define MOTHER_BOARD_LAYERS_SIZE  6



/** PSP_PSO_STRUCT
*    @GROUPID: APCB_GROUP_MEMORY
*    @TYPEID: APCB_MEM_TYPE_PSO_DATA
*    This data structure is used to pass memory PSO override data
*/
typedef struct _PSP_PSO_STRUCT {
  PSO_ENTRY MemClkDisMap[MEMCLK_DIS_MAP_SIZE];
  PSO_ENTRY CkeTriMap[CKE_TRI_MAP_SIZE];
  PSO_ENTRY OdtTriMap[ODT_TRI_MAP_SIZE];
  PSO_ENTRY CsTriMap[CS_TRI_MAP_SIZE];
  PSO_ENTRY NumberDimmSupported[NUMBER_OF_DIMMS_SUPPORTED_SIZE];
  PSO_ENTRY NumberSolderedDownDimmsSupported[NUMBER_OF_SOLDERED_DOWN_DIMMS_SUPPORTED_SIZE];
  PSO_ENTRY NumberChipSelectsSupported[NUMBER_OF_CHIP_SELECTS_SUPPORTED_SIZE];
  PSO_ENTRY NumberChannelsSupported[NUMBER_OF_CHANNELS_SUPPORTED_SIZE];
  PSO_ENTRY OverrideBusSpeed[OVERRIDE_DDR_BUS_SPEED_SIZE];
  PSO_ENTRY DramTechnology[DRAM_TECHNOLOGY_SIZE];
  PSO_ENTRY SolderedDownSoDimmType[SOLDERED_DOWN_SODIMM_TYPE_SIZE];
  PSO_ENTRY MemoryPowerPolicyOverride[MEMORY_POWER_POLICY_OVERRIDE_SIZE];
  PSO_ENTRY MotherboardLayers[MOTHER_BOARD_LAYERS_SIZE];
} PSP_PSO_STRUCT;

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_PSO_DATA,                     // TypeId
  sizeof(ApcbTypeHeader),                     // SizeOfType, will be fixed up by tool
  0,                                          // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,          // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK               // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

///< Platform Specific Overrides
PSO_ENTRY PlatformMemoryConfiguration[] = {
  //-----------------------------------------
  // Platform Memory Configuration Data Block
  //-----------------------------------------
    BLDCFG_MEM_CLK_MAP
    BLDCFG_CKE_TRI_MAP
    BLDCFG_ODT_TRI_MAP
    BLDCFG_CS_TRI_MAP
    BLDCFG_NUM_DIMMS_SUPPORTED
    BLDCFG_NUM_CHNL_SUPPORTED
    BLDCFG_NUM_MOTHERBOARD_LAYERS
    BLDCFG_NUM_SOLDERED_DOWN_DIMM_SUPPORTED
    BLDCFG_NUM_CS_SUPPORTED

    BLDCFG_BUS_SPEED_OVERRIDE
    BLDCFG_DRAM_TECH_OVERRIDE
    BLDCFG_SOLDRD_DOWN_DIMM_TYPE
    BLDCFG_MEM_POWER_POLICY_OVERRIDE
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

