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

#define PT_DONT_CARE 0xFF
#define NP 1
#define V1_5  1
#define V1_35  2
#define V1_25  4
#define V1_2   1
#define V_TBD1 2
#define V_TBD2 4
#define V1_1   1
#define V0_6   2

#define DIMM_SR 2
#define DIMM_DR 4
#define DIMM_QR 8

/// On die termination encoding for LPDDR4
typedef enum _RTT_ODT {
  RTT_ODT_DISABLE = 0,      ///< Disabled
  RTT_ODT_240,              ///< RZQ/1
  RTT_ODT_120,              ///< RZQ/2
  RTT_ODT_80,               ///< RZQ/3
  RTT_ODT_60,               ///< RZQ/4
  RTT_ODT_48,               ///< RZQ/5
  RTT_ODT_40                ///< RZQ/6
} RTT_ODT;

#define DDR400  ((UINT32) 1 << (DDR400_FREQUENCY  / 66))
#define DDR533  ((UINT32) 1 << (DDR533_FREQUENCY  / 66))
#define DDR667  ((UINT32) 1 << (DDR667_FREQUENCY  / 66))
#define DDR800  ((UINT32) 1 << (DDR800_FREQUENCY  / 66))
#define DDR1066 ((UINT32) 1 << (DDR1066_FREQUENCY / 66))
#define DDR1333 ((UINT32) 1 << (DDR1333_FREQUENCY / 66))
#define DDR1600 ((UINT32) 1 << (DDR1600_FREQUENCY / 66))
#define DDR1866 ((UINT32) 1 << (DDR1866_FREQUENCY / 66))
#define DDR2133 ((UINT32) 1 << (DDR2133_FREQUENCY / 66))
#define DDR2400 ((UINT32) 1 << (DDR2400_FREQUENCY / 66))
#define DDR2667 ((UINT32) 1 << (DDR2667_FREQUENCY / 66))
#define DDR2933 ((UINT32) 1 << (DDR2933_FREQUENCY / 66))
#define DDR3200 ((UINT32) 1 << (DDR3200_FREQUENCY / 66))

/// Data Bus configuration for LPDDR5 Soldered Down DRAM
typedef struct {
  _1BYTE_ALIGN UINT8  DimmPerCh;   ///< Bitmap of Dimm slot per chanel
  _2BYTE_ALIGN UINT16 DDRrate;     ///< DDR rate
  _1BYTE_ALIGN UINT8  Dimm0;       ///< Bitmap of rank type of Dimm0
  _1BYTE_ALIGN UINT8  Width;       ///< DRAM device width
  _1BYTE_ALIGN UINT8  MPStates;    ///< Bitmap of MPStates
  _4BYTE_ALIGN UINT32 DQ_ODT;      ///< DQ_ODT
  _4BYTE_ALIGN UINT32 ODT_PDDS;    ///< PDDS
  _4BYTE_ALIGN UINT32 DqStrength;  ///< Data drive strength
  _4BYTE_ALIGN UINT32 DqsStrength; ///< DQS drive strength
  _4BYTE_ALIGN UINT32 OdtStrength; ///< ODT drive strength
  _4BYTE_ALIGN UINT32 NtDqOdt;     ///< Non Target ODT Termination
  _4BYTE_ALIGN UINT32 SlewRate;    ///< Slew Rate
  _4BYTE_ALIGN UINT32 PMUPhyVref;  ///< Message Block Seed for Host Vref
  _4BYTE_ALIGN UINT32 VrefDq;      ///< Message Block Seed for DRAM MR6 Vref
  _1BYTE_ALIGN UINT8  RxBias;      ///< Rx Bias
} PSCFG_DRAMDN_LPDDR5_DATABUS_ENTRY;

#define MP0                      0x01
#define MP1                      0x02
#define MP2                      0x04
#define MP3                      0x08
#define MP_ALL                   0xFF

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                           // GroupId
  APCB_MEM_TYPE_PS_DRAMDOWN_LPDDR5_DATA_BUS,   // TypeId
  sizeof(ApcbTypeHeader),                      // SizeOfType, will be fixed up by tool
  0,                                           // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,           // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK                // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

PSCFG_DRAMDN_LPDDR5_DATABUS_ENTRY DataBusLpDdr5[] = {
  //                                                                                   DQ        DQS       ODT                                PMU
  // DPCH     Max Memclk      Dimm0  width  MPStates     DQ_ODT           PDDS,     Strength  Strength  Strength    NtDqOdt      SlewRate   PhyVref  VrefDq  RxBias
  {   1,  DDR1600_FREQUENCY, DIMM_SR,  16,   MP_ALL,  RTT_ODT_240,     RTT_ODT_40,     60,       60,       60,   RTT_ODT_DISABLE,  0x0C,     0x50,     40,    0x5   },
  {   1,  DDR1600_FREQUENCY, DIMM_DR,  16,   MP_ALL,  RTT_ODT_240,     RTT_ODT_40,     60,       60,       40,   RTT_ODT_DISABLE,  0x0C,     0x50,     40,    0x5   },
  {   1,  DDR1600_FREQUENCY, DIMM_DR,   8,   MP_ALL,  RTT_ODT_240,     RTT_ODT_40,     60,       60,       40,   RTT_ODT_DISABLE,  0x0C,     0x50,     25,    0x5   },
  {   1,  DDR3200_FREQUENCY, DIMM_SR,  16,   MP_ALL,  RTT_ODT_120,     RTT_ODT_40,     60,       60,       60,   RTT_ODT_DISABLE,  0x0C,     0x50,     40,    0x5   },
  {   1,  DDR3200_FREQUENCY, DIMM_DR,  16,   MP_ALL,  RTT_ODT_60,      RTT_ODT_48,     60,       60,       60,   RTT_ODT_240,      0x0C,     0x50,     40,    0x5   },
  {   1,  DDR3200_FREQUENCY, DIMM_DR,   8,   MP_ALL,  RTT_ODT_60,      RTT_ODT_48,     60,       60,       40,   RTT_ODT_240,      0x0C,     0x50,     25,    0x5   },
  {   1,  DDR4267_FREQUENCY, DIMM_SR,  16,   MP_ALL,  RTT_ODT_60,      RTT_ODT_40,     40,       40,       60,   RTT_ODT_DISABLE,  0x0C,     0x50,     40,    0x5   },
  {   1,  DDR4267_FREQUENCY, DIMM_DR,  16,   MP_ALL,  RTT_ODT_60,      RTT_ODT_48,     40,       40,       40,   RTT_ODT_240,      0x0C,     0x50,     40,    0x7   },
  {   1,  DDR4267_FREQUENCY, DIMM_DR,   8,   MP_ALL,  RTT_ODT_40,      RTT_ODT_48,     40,       40,       40,   RTT_ODT_240,      0x0C,     0x50,     25,    0x7   },
  {   1,  DDR5500_FREQUENCY, DIMM_SR,  16,   MP_ALL,  RTT_ODT_40,      RTT_ODT_40,     40,       40,       60,   RTT_ODT_DISABLE,  0x0C,     0x40,     40,    0x5   },
  {   1,  DDR5500_FREQUENCY, DIMM_DR,  16,   MP_ALL,  RTT_ODT_60,      RTT_ODT_40,     40,       40,       60,   RTT_ODT_240,      0x0C,     0x50,     40,    0x7   },
  {   1,  DDR5500_FREQUENCY, DIMM_DR,   8,   MP_ALL,  RTT_ODT_40,      RTT_ODT_40,     40,       40,       40,   RTT_ODT_240,      0x0C,     0x50,     25,    0x7   },
  {   1,  DDR6400_FREQUENCY, DIMM_SR,  16,   MP_ALL,  RTT_ODT_40,      RTT_ODT_40,     30,       30,       60,   RTT_ODT_DISABLE,  0x0C,     0x50,     40,    0x5   },
  {   1,  DDR6400_FREQUENCY, DIMM_DR,  16,   MP_ALL,  RTT_ODT_60,      RTT_ODT_40,     30,       30,       40,   RTT_ODT_240,      0x0C,     0x50,     40,    0x5   },
  {   1,  DDR6400_FREQUENCY, DIMM_DR,   8,   MP_ALL,  RTT_ODT_60,      RTT_ODT_40,     30,       30,       40,   RTT_ODT_240,      0x0C,     0x50,     25,    0x5   },
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

