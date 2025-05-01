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

#define DIMM_SR 2
#define DIMM_DR 4
#define DIMM_QR 8

/// On die termination encoding for LPDDR5
typedef enum _RTT_ODT {
  RTT_ODT_DISABLE = 0,      ///< Disabled
  RTT_ODT_240,              ///< RZQ/1
  RTT_ODT_120,              ///< RZQ/2
  RTT_ODT_80,               ///< RZQ/3
  RTT_ODT_60,               ///< RZQ/4
  RTT_ODT_48,               ///< RZQ/5
  RTT_ODT_40                ///< RZQ/6
} RTT_ODT;

/// CAD Bus configuration for LPDDR5 Soldered Down DRAM
typedef struct {
  _1BYTE_ALIGN UINT8  DimmPerCh;       ///< Bitmap of Dimm slot per chanel
  _2BYTE_ALIGN UINT16 DDRrate;         ///< DDR rate
  _1BYTE_ALIGN UINT8  Dimm0;           ///< Bitmap of rank type of Dimm0
  _1BYTE_ALIGN UINT8  Width;           ///< DRAM device width
  _1BYTE_ALIGN UINT8  MPStates;        ///< Bitmap of MPStates
  _4BYTE_ALIGN UINT32 AddrCmdCtl;      ///< AddrCmdCtl
  _1BYTE_ALIGN UINT8  CkeStrength;     ///< CKE drive strength
  _1BYTE_ALIGN UINT8  CsOdtStrength;   ///< CS ODT drive strength
  _1BYTE_ALIGN UINT8  AddrCmdStrength; ///< Addr Cmd drive strength
  _1BYTE_ALIGN UINT8  ClkStrength;     ///< CLK drive strength
  _1BYTE_ALIGN UINT8  DtoStrength;     ///< DTO drive strength
  _1BYTE_ALIGN UINT8  WckStrength;     ///< WCK drive strength
  _1BYTE_ALIGN UINT8  CA_ODT;          ///< CA_ODT
  _1BYTE_ALIGN UINT8  VrefCA;          ///< VrefCA
} PSCFG_DRAMDN_LPDDR5_CADBUS_ENTRY;

#define MP0                      0x01
#define MP1                      0x02
#define MP2                      0x04
#define MP3                      0x08
#define MP_ALL                   0xFF

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                               // GroupId
  APCB_MEM_TYPE_PS_DRAMDOWN_LPDDR5_CAD_BUS,        // TypeId
  sizeof(ApcbTypeHeader),                          // SizeOfType, will be fixed up by tool
  0,                                               // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,               // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK                    // Board specific APCB instance mask
  }
};

PSCFG_DRAMDN_LPDDR5_CADBUS_ENTRY CadBusLpDdr5[] = {
  // MP0
  //DimmPerCh,  DDRrate,          ranks,   Width, MPStates,  AddrCmdCtl, CkeStrength, CsOdtStrength, AddrCmdStrength, ClkStrength, DtoStrength, WckStrength, CA_ODT,       VrefCA
    {1,        DDR1600_FREQUENCY, DIMM_SR,  16,    MP_ALL,  0x00000000,   50,          60,            60,              60,          50,          60,       RTT_ODT_DISABLE, 48},
    {1,        DDR1600_FREQUENCY, DIMM_DR,  16,    MP_ALL,  0x00000000,   50,          60,            60,              60,          50,          60,       RTT_ODT_DISABLE, 48},
    {1,        DDR1600_FREQUENCY, DIMM_DR,   8,    MP_ALL,  0x00000000,   50,          60,            60,              60,          50,          60,       RTT_ODT_DISABLE, 48},
    {1,        DDR3200_FREQUENCY, DIMM_SR,  16,    MP_ALL,  0x00000000,   50,          60,            40,              40,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR3200_FREQUENCY, DIMM_DR,  16,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR3200_FREQUENCY, DIMM_DR,   8,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR4267_FREQUENCY, DIMM_SR,  16,    MP_ALL,  0x00000000,   50,          60,            40,              40,          50,          40,       RTT_ODT_240,     40},
    {1,        DDR4267_FREQUENCY, DIMM_DR,  16,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     40},
    {1,        DDR4267_FREQUENCY, DIMM_DR,   8,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     40},
    {1,        DDR5500_FREQUENCY, DIMM_SR,  16,    MP_ALL,  0x00000000,   50,          60,            40,              40,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR5500_FREQUENCY, DIMM_DR,  16,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR5500_FREQUENCY, DIMM_DR,   8,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          40,       RTT_ODT_240,     38},
    {1,        DDR6400_FREQUENCY, DIMM_SR,  16,    MP_ALL,  0x00000000,   50,          60,            40,              40,          50,          30,       RTT_ODT_240,     38},
    {1,        DDR6400_FREQUENCY, DIMM_DR,  16,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          30,       RTT_ODT_240,     38},
    {1,        DDR6400_FREQUENCY, DIMM_DR,   8,    MP_ALL,  0x00000000,   50,          60,            30,              30,          50,          30,       RTT_ODT_240,     38},
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

