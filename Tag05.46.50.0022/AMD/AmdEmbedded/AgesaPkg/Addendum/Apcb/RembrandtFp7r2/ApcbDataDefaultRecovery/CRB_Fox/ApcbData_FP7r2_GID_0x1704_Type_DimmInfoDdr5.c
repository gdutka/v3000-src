/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <ApcbMemGroup.h>
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
#define  I2C_0        0x00
#define  I2C_1        0x01
#define  I2C_2        0x02
#define  I2C_3        0x03
#define  I2C_4        0x04
#define  I2C_5        0x05
#define  I2C_6        0x06
#define  I2C_7        0x07
#define  SMB_0        0x00
#define  SMB_1        0x01
#define  I3C_0        0x00
#define  I3C_1        0x01
#define  I3C_2        0x02
#define  I3C_3        0x03
#define  I3C_4        0x04
#define  I3C_5        0x05
#define  I3C_6        0x06
#define  I3C_7        0x07

#define  __ANONYMOUS1(type, var, line)  type  var##line
#define  _ANONYMOUS0(type, line)  __ANONYMOUS1(type, _anonymous, line)
#define  ANONYMOUS(type)  _ANONYMOUS0(type, __LINE__)
#define  MAKE_ID(Socket, Channel, Dimm, Block)  ((((UINT16) (Socket)) << 12) | (((UINT16)(Channel)) << 8) | (((UINT16)(Dimm)) << 4) | ((UINT16)(Block)))
#define  MAKE_DIMM_INFO_REVISION(Revision)  ANONYMOUS(DIMM_INFO_REVISION_S)={{0,sizeof(DIMM_INFO_REVISION_S)},Revision};
#define  MAKE_DIMM_ADDR_I2C(Socket, Channel, Dimm, BusID, DevID)  ANONYMOUS(DIMM_INFO_DEV_ADDR_S)={{1,sizeof(DIMM_INFO_DEV_ADDR_S)},MAKE_ID(Socket, Channel, Dimm, 0),BusID,DevID};
#define  MAKE_DIMM_ADDR_SMB(Socket, Channel, Dimm, BusID, DevID)  ANONYMOUS(DIMM_INFO_DEV_ADDR_S)={{2,sizeof(DIMM_INFO_DEV_ADDR_S)},MAKE_ID(Socket, Channel, Dimm, 0),BusID,DevID};
#define  MAKE_DIMM_ADDR_I3C(Socket, Channel, Dimm, BusID, DevID)  ANONYMOUS(DIMM_INFO_DEV_ADDR_S)={{3,sizeof(DIMM_INFO_DEV_ADDR_S)},MAKE_ID(Socket, Channel, Dimm, 0),BusID,DevID};

//
// Blob structures
//
typedef struct {
  UINT16 Type;
  UINT16 Length;
} DIMM_INFO_COMMON_HEADER;

typedef struct {
  DIMM_INFO_COMMON_HEADER Header;
  UINT32                  Revision;
} DIMM_INFO_REVISION_S;

typedef struct {
  DIMM_INFO_COMMON_HEADER Header;     // Type --> I2C, I3C, SMBUS
  UINT16                  Key;        // <4:4:4:4> = <Socket:Channel:Dimm:Reserve>
  UINT8                   BusID;      // 0:I2C0, 1:I2C1, 2:I2C3, 3:I2C4
                                      // 1:SMBUS0, 1:SMBUS1(ASF)
                                      // 2:I3C0, 1:I3C1, 2:I3C3, 3:I3C4
  UINT8                   DevID;      // 7-bit Address of SPD5 Hub Device <4:3:1> = <1010b(LID):xxxb(HID):0b>
} DIMM_INFO_DEV_ADDR_S;


APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_DIMM_INFO_SMBUS,              // TypeId
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
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_0
BLDCFG_DIMM_INFO_DATA_BLOB_0
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1
BLDCFG_DIMM_INFO_DATA_BLOB_1
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2
BLDCFG_DIMM_INFO_DATA_BLOB_2
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3
BLDCFG_DIMM_INFO_DATA_BLOB_3
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_4
BLDCFG_DIMM_INFO_DATA_BLOB_4
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_5
BLDCFG_DIMM_INFO_DATA_BLOB_5
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_6
BLDCFG_DIMM_INFO_DATA_BLOB_6
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_7
BLDCFG_DIMM_INFO_DATA_BLOB_7
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_8
BLDCFG_DIMM_INFO_DATA_BLOB_8
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_9
BLDCFG_DIMM_INFO_DATA_BLOB_9
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_A
BLDCFG_DIMM_INFO_DATA_BLOB_A
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_B
BLDCFG_DIMM_INFO_DATA_BLOB_B
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_C
BLDCFG_DIMM_INFO_DATA_BLOB_C
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_D
BLDCFG_DIMM_INFO_DATA_BLOB_D
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_E
BLDCFG_DIMM_INFO_DATA_BLOB_E
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_F
BLDCFG_DIMM_INFO_DATA_BLOB_F
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_10
BLDCFG_DIMM_INFO_DATA_BLOB_10
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_11
BLDCFG_DIMM_INFO_DATA_BLOB_11
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_12
BLDCFG_DIMM_INFO_DATA_BLOB_12
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_13
BLDCFG_DIMM_INFO_DATA_BLOB_13
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_14
BLDCFG_DIMM_INFO_DATA_BLOB_14
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_15
BLDCFG_DIMM_INFO_DATA_BLOB_15
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_16
BLDCFG_DIMM_INFO_DATA_BLOB_16
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_17
BLDCFG_DIMM_INFO_DATA_BLOB_17
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_18
BLDCFG_DIMM_INFO_DATA_BLOB_18
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_19
BLDCFG_DIMM_INFO_DATA_BLOB_19
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1A
BLDCFG_DIMM_INFO_DATA_BLOB_1A
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1B
BLDCFG_DIMM_INFO_DATA_BLOB_1B
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1C
BLDCFG_DIMM_INFO_DATA_BLOB_1C
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1D
BLDCFG_DIMM_INFO_DATA_BLOB_1D
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1E
BLDCFG_DIMM_INFO_DATA_BLOB_1E
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_1F
BLDCFG_DIMM_INFO_DATA_BLOB_1F
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_20
BLDCFG_DIMM_INFO_DATA_BLOB_20
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_21
BLDCFG_DIMM_INFO_DATA_BLOB_21
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_22
BLDCFG_DIMM_INFO_DATA_BLOB_22
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_23
BLDCFG_DIMM_INFO_DATA_BLOB_23
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_24
BLDCFG_DIMM_INFO_DATA_BLOB_24
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_25
BLDCFG_DIMM_INFO_DATA_BLOB_25
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_26
BLDCFG_DIMM_INFO_DATA_BLOB_26
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_27
BLDCFG_DIMM_INFO_DATA_BLOB_27
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_28
BLDCFG_DIMM_INFO_DATA_BLOB_28
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_29
BLDCFG_DIMM_INFO_DATA_BLOB_29
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2A
BLDCFG_DIMM_INFO_DATA_BLOB_2A
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2B
BLDCFG_DIMM_INFO_DATA_BLOB_2B
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2C
BLDCFG_DIMM_INFO_DATA_BLOB_2C
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2D
BLDCFG_DIMM_INFO_DATA_BLOB_2D
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2E
BLDCFG_DIMM_INFO_DATA_BLOB_2E
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_2F
BLDCFG_DIMM_INFO_DATA_BLOB_2F
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_30
BLDCFG_DIMM_INFO_DATA_BLOB_30
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_31
BLDCFG_DIMM_INFO_DATA_BLOB_31
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_32
BLDCFG_DIMM_INFO_DATA_BLOB_32
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_33
BLDCFG_DIMM_INFO_DATA_BLOB_33
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_34
BLDCFG_DIMM_INFO_DATA_BLOB_34
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_35
BLDCFG_DIMM_INFO_DATA_BLOB_35
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_36
BLDCFG_DIMM_INFO_DATA_BLOB_36
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_37
BLDCFG_DIMM_INFO_DATA_BLOB_37
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_38
BLDCFG_DIMM_INFO_DATA_BLOB_38
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_39
BLDCFG_DIMM_INFO_DATA_BLOB_39
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3A
BLDCFG_DIMM_INFO_DATA_BLOB_3A
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3B
BLDCFG_DIMM_INFO_DATA_BLOB_3B
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3C
BLDCFG_DIMM_INFO_DATA_BLOB_3C
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3D
BLDCFG_DIMM_INFO_DATA_BLOB_3D
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3E
BLDCFG_DIMM_INFO_DATA_BLOB_3E
#endif
#ifdef BLDCFG_DIMM_INFO_DATA_BLOB_3F
BLDCFG_DIMM_INFO_DATA_BLOB_3F
#endif

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

