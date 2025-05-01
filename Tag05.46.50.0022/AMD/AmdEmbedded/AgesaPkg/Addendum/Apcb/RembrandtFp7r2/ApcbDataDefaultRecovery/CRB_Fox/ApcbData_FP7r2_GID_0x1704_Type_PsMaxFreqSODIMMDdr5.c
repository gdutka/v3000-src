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

/// Number of DRAM devices or DIMM slots
typedef enum {
  _1DIMM = 0x01,               ///< _1DIMM
  _2DIMM = 0x02,               ///< _2DIMM
  _3DIMM = 0x04,               ///< _3DIMM
  _4DIMM = 0x08,               ///< _4DIMM
  _DIMM_NONE = 0xF0,           ///< _DIMM_NONE (no DIMM slot)
  NOD_DONT_CARE = 0xFF         ///< NOD_DONT_CARE
} NOD_SUPPORTED;

/// UDIMM&RDIMM Max. Frequency
typedef union {
  struct {                       ///< PSCFG_MAXFREQ_ENTRY
    _1BYTE_ALIGN UINT8 DimmPerCh;             ///< Dimm slot per chanel
    _2BYTE_ALIGN UINT16 Dimms;              ///< Number of Dimms on a channel
    _2BYTE_ALIGN UINT16 SR;                 ///< Number of single-rank Dimm
    _2BYTE_ALIGN UINT16 DR;                 ///< Number of dual-rank Dimm
    _2BYTE_ALIGN UINT16 QR;                 ///< Number of quad-rank Dimm
    _2BYTE_ALIGN UINT16 Speed1_5V;            ///< Speed limit with voltage 1.5V
    _2BYTE_ALIGN UINT16 Speed1_35V;           ///< Speed limit with voltage 1.35V
    _2BYTE_ALIGN UINT16 Speed1_25V;           ///< Speed limit with voltage 1.25V
  } _MAXFREQ_ENTRY;
  struct {
    _1BYTE_ALIGN UINT8 DimmSlotPerCh;
    _2BYTE_ALIGN UINT16 CDN;                  ///< Condition
    _2BYTE_ALIGN UINT16 CDN1;                  ///< Condition
    _2BYTE_ALIGN UINT16 CDN2;                  ///< Condition
    _2BYTE_ALIGN UINT16 CDN3;                  ///< Condition
    _2BYTE_ALIGN UINT16 Speed[3];             ///< Speed limit
  } MAXFREQ_ENTRY;
} PSCFG_MAXFREQ_ENTRY;

APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_PS_SODIMM_DDR5_MAX_FREQ,      // TypeId
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

PSCFG_MAXFREQ_ENTRY MaxFeqTbl [] = {
  // Max Freq.
  // Format :
  // DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_2V,   Speed_TBD1,   Speed_TBD2
  //
  { _1DIMM, 1, 1, 0, 0, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY },
  { _1DIMM, 1, 0, 1, 0, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY },
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

