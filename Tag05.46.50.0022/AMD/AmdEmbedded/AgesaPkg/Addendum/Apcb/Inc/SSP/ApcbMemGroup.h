/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_MEM_GROUP_H_
#define _APCB_MEM_GROUP_H_
//
// @APCB_START
//

#define APCB_MEM_TYPE_SPD_INFO                      0x0030

#define APCB_MEM_TYPE_DIMM_INFO_SMBUS               0x0031

#define APCB_MEM_TYPE_DIMM_CONFIG_INFO_ID           0x0032

#define APCB_MEM_TYPE_MEM_OVERCLOCK_CONFIG          0x0033
#define APCB_MEM_TYPE_MEM_OVERCLOCK_MATRIX          0x0034

#define APCB_MEM_TYPE_ERROR_HANDLE_CONFIG           0x0036

#define APCB_MEM_TYPE_PSO_DATA                      0x0040

#define APCB_MEM_TYPE_PS_UDIMM_DDR4_ODT_PAT         0x0041
#define APCB_MEM_TYPE_PS_UDIMM_DDR4_CAD_BUS         0x0042
#define APCB_MEM_TYPE_PS_UDIMM_DDR4_DATA_BUS        0x0043
#define APCB_MEM_TYPE_PS_UDIMM_DDR4_MAX_FREQ        0x0044
#define APCB_MEM_TYPE_PS_UDIMM_DDR4_STRETCH_FREQ    0x0045

#define APCB_MEM_TYPE_PS_RDIMM_DDR4_ODT_PAT         0x0046
#define APCB_MEM_TYPE_PS_RDIMM_DDR4_CAD_BUS         0x0047
#define APCB_MEM_TYPE_PS_RDIMM_DDR4_DATA_BUS        0x0048
#define APCB_MEM_TYPE_PS_RDIMM_DDR4_MAX_FREQ        0x0049
#define APCB_MEM_TYPE_PS_RDIMM_DDR4_STRETCH_FREQ    0x004A
#define APCB_MEM_TYPE_PS_3DS_RDIMM_DDR4_MAX_FREQ    0x004B
#define APCB_MEM_TYPE_PS_3DS_RDIMM_DDR4_STRETCH_FREQ 0x004C
#define APCB_MEM_TYPE_PS_3DS_RDIMM_DDR4_DATA_BUS    0x004D

#define APCB_MEM_TYPE_CONSOLE_OUT_CONTROL           0x0050

#define APCB_MEM_TYPE_EVENT_CONTROL                 0x0051

#define APCB_MEM_TYPE_ERROR_OUT_EVENT_CONTROL       0x0052

#define APCB_MEM_TYPE_EXT_VOLTAGE_CONTROL           0x0053

#define APCB_MEM_TYPE_PS_LRDIMM_DDR4_ODT_PAT        0x0054
#define APCB_MEM_TYPE_PS_LRDIMM_DDR4_CAD_BUS        0x0055
#define APCB_MEM_TYPE_PS_LRDIMM_DDR4_DATA_BUS       0x0056
#define APCB_MEM_TYPE_PS_LRDIMM_DDR4_MAX_FREQ       0x0057
#define APCB_MEM_TYPE_PS_LRDIMM_DDR4_STRETCH_FREQ   0x0058

#define APCB_MEM_TYPE_PS_SODIMM_DDR4_ODT_PAT        0x0059
#define APCB_MEM_TYPE_PS_SODIMM_DDR4_CAD_BUS        0x005A
#define APCB_MEM_TYPE_PS_SODIMM_DDR4_DATA_BUS       0x005B
#define APCB_MEM_TYPE_PS_SODIMM_DDR4_MAX_FREQ       0x005C
#define APCB_MEM_TYPE_PS_SODIMM_DDR4_STRETCH_FREQ   0x005D

#define APCB_MEM_TYPE_DDR_POST_PACKAGE_REPAIR       0x005E

#define APCB_MEM_TYPE_PS_DRAMDOWN_DDR4_ODT_PAT      0x0070
#define APCB_MEM_TYPE_PS_DRAMDOWN_DDR4_CAD_BUS      0x0071
#define APCB_MEM_TYPE_PS_DRAMDOWN_DDR4_DATA_BUS     0x0072
#define APCB_MEM_TYPE_PS_DRAMDOWN_DDR4_MAX_FREQ     0x0073
#define APCB_MEM_TYPE_PS_DRAMDOWN_DDR4_STRETCH_FREQ 0x0074

#define APCB_MEM_TYPE_PLATFORM_TUNING               0x0075

#if ABL_BUILD
#include "PlatformMemoryConfiguration.h"
#include "mm.h"
#include "mn.h"
#include "mnreg.h"
//#include "mempmu.h"
#include "mp.h"
#else

//#pragma pack(push, 1)
/// Dram technology type
typedef enum {
  DDR2_TECHNOLOGY,        ///< DDR2 technology
  DDR3_TECHNOLOGY,        ///< DDR3 technology
  GDDR5_TECHNOLOGY,       ///< GDDR5 technology
  DDR4_TECHNOLOGY,        ///< DDR4 technology
  LPDDR3_TECHNOLOGY,      ///< LPDDR3 technology
  UNSUPPORTED_TECHNOLOGY, ///< Unsupported technology
} TECHNOLOGY_TYPE;

/// Dram technology type
typedef enum {
  SODIMM_PACKAGE,        ///< SODIMM package
  UDIMM_PACKAGE,         ///< UDIMM Package
  RDIMM_PACKAGE,         ///< RDIMM Package
  DRAMDOWN_PACKAGE,      ///< DRAM Down Package
  UNSUPPORTED_PACKAGE,   ///< Unsupported package
} DRAM_PACKAGE_TYPE;

/// I2C_MUX_INFO for SPD
typedef struct _SPD_MUX_INFO {
  UINT8  SpdMuxPresent;       ///< Spd I2C MUX present or not, TRUE - Present, FALSE - Not Present
  UINT8  MuxSmbusAddress;     ///< I2C Mux Address
  UINT8  MuxChannel;          ///< I2C Mux Channel assocaited iwth this SPD
  UINT8  Reserved;               ///< Reserved
} SPD_MUX_INFO;

///
/// Meory Board Layout Info
///
#define MEMORY_LAYOUT_TYPE_DIMM           1
#define MEMORY_LAYOUT_TYPE_ONBOARD        2

#include "ApcbMemMp.h"
#include "ApcbMemPsMemoryConfigurations.h"
//#include "ApcbMemMm.h"  // Equivalant to #include "ApcbMemPlatformTuning.h" below
#endif

#include "ApcbMemSpdInfo.h"                     // (0x30) APCB_MEM_TYPE_SPD_INFO
#include "ApcbMemDimmInfoSmbus.h"               // (0x31) APCB_MEM_TYPE_DIMM_INFO_SMBUS
#include "ApcbMemDimmConfig.h"                  // (0x32) APCB_MEM_TYPE_DIMM_CONFIG_INFO_ID
#include "ApcbMemOverclockConfig.h"             // (0x33) APCB_MEM_TYPE_MEM_OVERCLOCK_CONFIG
#include "ApcbMemOverclockMatrix.h"             // (0x34) APCB_MEM_TYPE_MEM_OVERCLOCK_MATRIX
#include "ApcbMemPsoOverride.h"                 // (0x40) APCB_MEM_TYPE_PSO_DATA
#include "ApcbMemPsUdimmDdr4OdtPat.h"           // (0x41) APCB_MEM_TYPE_PS_UDIMM_DDR4_ODT_PAT
#include "ApcbMemConsoleOut.h"                  // (0x50) APCB_MEM_TYPE_CONSOLE_OUT_CONTROL
#include "ApcbMemEventControl.h"                // (0x51) APCB_MEM_TYPE_EVENT_CONTROL
#include "ApcbMemErrorOutControl.h"             // (0x52) APCB_MEM_TYPE_ERROR_OUT_EVENT_CONTROL
#include "ApcbMemExtVoltageControl.h"           // (0x53) APCB_MEM_TYPE_EXT_VOLTAGE_CONTROL
#include "ApcbMemDdrPostPackageRepair.h"        // (0x5E) APCB_MEM_TYPE_DDR_POST_PACKAGE_REPAIR
#if !ABL_BUILD
#include "ApcbMemPlatformTuning.h"              // (0x75) APCB_MEM_TYPE_PLATFORM_TUNING
#endif

#endif // _APCB_MEM_GROUP_H_


