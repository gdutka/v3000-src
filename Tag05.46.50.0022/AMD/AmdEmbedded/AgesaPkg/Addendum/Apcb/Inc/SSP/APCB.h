/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * APCB.h
 *
 * AGESA PSP Customization Block
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem)
 * @e \$Revision: 192403 $ @e \$Date: 2012-12-12 15:46:25 -0600 (Wed, 12 Dec 2012) $
 *
 **/

#ifndef _APCB_H_
#define _APCB_H_

//
// @APCB_START
//
#define APCB_HEADER_VERSION         0x0300    // APCB Version
#define APCB_HEADER_STRUCT_VERSION  0x0000
#define APCB_HEADER_DATA_VERSION    0x0000

#define APCB_BUILD_SSP 1

#include "ApcbV3Arch.h"
#include "ApcbV3Priority.h"
#include "ApcbDataGroups.h"
#include "ApcbAblBreakpoint.h"

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */
/// ABL Serial Baud Rate
typedef enum {
  BAUD_RATE_2400 = 0,     ///< Baud rate 2400
  BAUD_RATE_3600 = 1,     ///< Baud rate 3600
  BAUD_RATE_4800 = 2,     ///< Baud rate 4800
  BAUD_RATE_7200 = 3,     ///< Baud rate 7200
  BAUD_RATE_9600 = 4,     ///< Baud rate 9600
  BAUD_RATE_19200 = 5,    ///< Baud rate 19200
  BAUD_RATE_38400 = 6,    ///< Baud rate 38400
  BAUD_RATE_57600 = 7,    ///< Baud rate 57600
  BAUD_RATE_115200 = 8    ///< Baud rate 115200
} ABL_SERIAL_BAUD_RATE;

/// PMU message control (HdtCtrl)
typedef enum {
  DETAILED_DBG_MSG = 0x05,     ///< Detailded debug message
  COARSE_DBG_MSG = 0x0A,       ///< Coarse debug message
  STAGE_COMPLETION = 0xC8,     ///< Stage completion
  FIRMWARE_COMPLETION_MSG_ONLY = 0xFE   ///< Firmware completion message only, 0xFE to instead of 0xFF, because 0xFF is for Auto
} ABL_PMU_MESSAGE_CONTROL;

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */

#endif




