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

//
// Helper Marco
//
#define  SOCKET_0     0x00
#define  SOCKET_1     0x01
#define  CHANNEL_A    0x00
#define  CHANNEL_B    0x01
#define  CHANNEL_C    0x02
#define  CHANNEL_D    0x03
#define  CHANNEL_E    0x04
#define  CHANNEL_F    0x05
#define  CHANNEL_G    0x06
#define  CHANNEL_H    0x07
#define  CHANNEL_I    0x08
#define  CHANNEL_J    0x09
#define  CHANNEL_K    0x0A
#define  CHANNEL_L    0x0B
#define  DIMM_0       0x00
#define  DIMM_1       0x01
#define  BLOCK_0      0x00
#define  BLOCK_1      0x01
#define  BLOCK_2      0x02
#define  BLOCK_3      0x03
#define  BLOCK_4      0x04
#define  BLOCK_5      0x05
#define  BLOCK_6      0x06
#define  BLOCK_7      0x07
#define  BLOCK_8      0x08
#define  BLOCK_9      0x09
#define  BLOCK_A      0x0A
#define  BLOCK_B      0x0B
#define  BLOCK_C      0x0C
#define  BLOCK_D      0x0D
#define  BLOCK_E      0x0E
#define  BLOCK_F      0x0F

#define  __ANONYMOUS1(type, var, line)  type  var##line
#define  _ANONYMOUS0(type, line)  __ANONYMOUS1(type, _anonymous, line)
#define  ANONYMOUS(type)  _ANONYMOUS0(type, __LINE__)
#define  MAKE_ID(Socket, Channel, Dimm, Block)  ((((UINT16) (Socket)) << 12) | (((UINT16)(Channel)) << 8) | (((UINT16)(Dimm)) << 4) | ((UINT16)(Block)))
#define  MAKE_SPD_INFO_REVISION(Revision)  ANONYMOUS(SPD_INFO_REVISION_S)={{0,sizeof(SPD_INFO_REVISION_S)},Revision};
#define  MAKE_SPD_DATA_MIRROR(Socket, Channel, Dimm, RemapS, RemapC, RemapD)  ANONYMOUS(SPD_INFO_SPD_MIRROR_S)={{1,sizeof(SPD_INFO_SPD_MIRROR_S)},MAKE_ID(Socket, Channel, Dimm, 0),MAKE_ID(RemapS, RemapC, RemapD, 0)};
#define  MAKE_SPD_DATA_BLOCK(Socket, Channel, Dimm, Block, Data)  ANONYMOUS(SPD_INFO_SPD_BLOCK_S)={{2,sizeof(SPD_INFO_SPD_BLOCK_S)},MAKE_ID(Socket, Channel, Dimm, Block),0,{Data}};

//
// Blob structures
//
typedef struct {
  UINT16 Type;
  UINT16 Length;
} SPD_INFO_COMMON_HEADER;

typedef struct {
  SPD_INFO_COMMON_HEADER    Header;
  UINT32                    Revision;
} SPD_INFO_REVISION_S;

typedef struct {
  SPD_INFO_COMMON_HEADER    Header;
  UINT16                    Key;       // <4:4:4:4> = <Socket:Channel:Dimm:Reserve>
  UINT16                    Remap;     // <4:4:4:4> = <Socket:Channel:Dimm:Reserve>
} SPD_INFO_SPD_MIRROR_S;

typedef struct {
  SPD_INFO_COMMON_HEADER    Header;
  UINT16                    Key;       // <4:4:4:4> = <Socket:Channel:Dimm:Block>
  UINT16                    Reserved;  // Padding
  UINT8                     Data[64];  // 64 Bytes for eack block
} SPD_INFO_SPD_BLOCK_S;


APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_SPD_INFO,                     // TypeId
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

//
// Blobs
//
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_0
BLDCFG_SPD_INFO_DATA_BLOB_0
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1
BLDCFG_SPD_INFO_DATA_BLOB_1
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_2
BLDCFG_SPD_INFO_DATA_BLOB_2
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_3
BLDCFG_SPD_INFO_DATA_BLOB_3
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_4
BLDCFG_SPD_INFO_DATA_BLOB_4
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_5
BLDCFG_SPD_INFO_DATA_BLOB_5
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_6
BLDCFG_SPD_INFO_DATA_BLOB_6
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_7
BLDCFG_SPD_INFO_DATA_BLOB_7
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_8
BLDCFG_SPD_INFO_DATA_BLOB_8
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_9
BLDCFG_SPD_INFO_DATA_BLOB_9
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_A
BLDCFG_SPD_INFO_DATA_BLOB_A
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_B
BLDCFG_SPD_INFO_DATA_BLOB_B
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_C
BLDCFG_SPD_INFO_DATA_BLOB_C
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_D
BLDCFG_SPD_INFO_DATA_BLOB_D
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_E
BLDCFG_SPD_INFO_DATA_BLOB_E
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_F
BLDCFG_SPD_INFO_DATA_BLOB_F
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_10
BLDCFG_SPD_INFO_DATA_BLOB_10
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_11
BLDCFG_SPD_INFO_DATA_BLOB_11
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_12
BLDCFG_SPD_INFO_DATA_BLOB_12
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_13
BLDCFG_SPD_INFO_DATA_BLOB_13
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_14
BLDCFG_SPD_INFO_DATA_BLOB_14
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_15
BLDCFG_SPD_INFO_DATA_BLOB_15
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_16
BLDCFG_SPD_INFO_DATA_BLOB_16
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_17
BLDCFG_SPD_INFO_DATA_BLOB_17
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_18
BLDCFG_SPD_INFO_DATA_BLOB_18
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_19
BLDCFG_SPD_INFO_DATA_BLOB_19
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1A
BLDCFG_SPD_INFO_DATA_BLOB_1A
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1B
BLDCFG_SPD_INFO_DATA_BLOB_1B
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1C
BLDCFG_SPD_INFO_DATA_BLOB_1C
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1D
BLDCFG_SPD_INFO_DATA_BLOB_1D
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1E
BLDCFG_SPD_INFO_DATA_BLOB_1E
#endif
#ifdef BLDCFG_SPD_INFO_DATA_BLOB_1F
BLDCFG_SPD_INFO_DATA_BLOB_1F
#endif

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

