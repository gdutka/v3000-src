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
  ASSERTION_MSG = 0xC9,        ///< Assertion message
  FIRMWARE_COMPLETION_MSG_ONLY = 0xFE   ///< Firmware completion message only, 0xFE to instead of 0xFF, because 0xFF is for Auto
} ABL_PMU_MESSAGE_CONTROL;

/// Build Configuration values for BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT & BLDCFG_MEMORY_CLOCK_SELECT
#define DDR400_FREQUENCY    200     ///< DDR 400
#define DDR533_FREQUENCY    266     ///< DDR 533
#define DDR667_FREQUENCY    333     ///< DDR 667
#define DDR800_FREQUENCY    400     ///< DDR 800
#define DDR1066_FREQUENCY   533     ///< DDR 1066
#define DDR1333_FREQUENCY   667     ///< DDR 1333
#define DDR1600_FREQUENCY   800     ///< DDR 1600
#define DDR1866_FREQUENCY   933     ///< DDR 1867
#define DDR2000_FREQUENCY   1000    ///< DDR 2000
#define DDR2100_FREQUENCY   1050    ///< DDR 2100
#define DDR2133_FREQUENCY   1067    ///< DDR 2133
#define DDR2400_FREQUENCY   1200    ///< DDR 2400
#define DDR2667_FREQUENCY   1333    ///< DDR 2667
#define DDR2733_FREQUENCY   1367    ///< DDR 2733
#define DDR2800_FREQUENCY   1400    ///< DDR 2800
#define DDR2867_FREQUENCY   1433    ///< DDR 2867
#define DDR2933_FREQUENCY   1467    ///< DDR 2933
#define DDR3000_FREQUENCY   1500    ///< DDR 3000
#define DDR3067_FREQUENCY   1533    ///< DDR 3067
#define DDR3133_FREQUENCY   1567    ///< DDR 3133
#define DDR3200_FREQUENCY   1600    ///< DDR 3200
#define DDR3267_FREQUENCY   1633    ///< DDR 3267
#define DDR3333_FREQUENCY   1667    ///< DDR 3333
#define DDR3400_FREQUENCY   1700    ///< DDR 3400
#define DDR3467_FREQUENCY   1733    ///< DDR 3467
#define DDR3533_FREQUENCY   1767    ///< DDR 3533
#define DDR3600_FREQUENCY   1800    ///< DDR 3600
#define DDR3667_FREQUENCY   1833    ///< DDR 3667
#define DDR3733_FREQUENCY   1867    ///< DDR 3733
#define DDR3800_FREQUENCY   1900    ///< DDR 3800
#define DDR3867_FREQUENCY   1933    ///< DDR 3867
#define DDR3933_FREQUENCY   1967    ///< DDR 3933
#define DDR4000_FREQUENCY   2000    ///< DDR 4000
#define DDR4067_FREQUENCY   2033    ///< DDR 4067
#define DDR4133_FREQUENCY   2067    ///< DDR 4133
#define DDR4200_FREQUENCY   2100    ///< DDR 4200
#define DDR4267_FREQUENCY   2133    ///< DDR 4267
#define DDR4333_FREQUENCY   2167    ///< DDR 4333
#define DDR4400_FREQUENCY   2200    ///< DDR 4400
#define DDR4467_FREQUENCY   2233    ///< DDR 4467
#define DDR4533_FREQUENCY   2267    ///< DDR 4533
#define DDR4600_FREQUENCY   2300    ///< DDR 4600
#define DDR4667_FREQUENCY   2333    ///< DDR 4667
#define DDR4733_FREQUENCY   2367    ///< DDR 4733
#define DDR4800_FREQUENCY   2400    ///< DDR 4800
#define DDR4867_FREQUENCY   2433    ///< DDR 4867
#define DDR4933_FREQUENCY   2467    ///< DDR 4933
#define DDR5000_FREQUENCY   2500    ///< DDR 5000
#define DDR5100_FREQUENCY   2550    ///< DDR 5100
#define DDR5200_FREQUENCY   2600    ///< DDR 5200
#define DDR5300_FREQUENCY   2650    ///< DDR 5300
#define DDR5400_FREQUENCY   2700    ///< DDR 5400
#define DDR5500_FREQUENCY   2750    ///< DDR 5500
#define DDR5600_FREQUENCY   2800    ///< DDR 5600
#define DDR5700_FREQUENCY   2850    ///< DDR 5700
#define DDR5800_FREQUENCY   2900    ///< DDR 5800
#define DDR5900_FREQUENCY   2950    ///< DDR 5900
#define DDR6000_FREQUENCY   3000    ///< DDR 6000
#define DDR6100_FREQUENCY   3050    ///< DDR 6100
#define DDR6200_FREQUENCY   3100    ///< DDR 6200
#define DDR6300_FREQUENCY   3150    ///< DDR 6300
#define DDR6400_FREQUENCY   3200    ///< DDR 6400
#define DDR6500_FREQUENCY   3250    ///< DDR 6500
#define DDR6600_FREQUENCY   3300    ///< DDR 6600
#define DDR6700_FREQUENCY   3350    ///< DDR 6700
#define DDR6800_FREQUENCY   3400    ///< DDR 6800
#define DDR6900_FREQUENCY   3450    ///< DDR 6900
#define DDR7000_FREQUENCY   3500    ///< DDR 7000
#define DDR7100_FREQUENCY   3550    ///< DDR 7100
#define DDR7200_FREQUENCY   3600    ///< DDR 7200
#define DDR7300_FREQUENCY   3650    ///< DDR 7300
#define DDR7400_FREQUENCY   3700    ///< DDR 7400
#define DDR7500_FREQUENCY   3750    ///< DDR 7500
#define DDR7600_FREQUENCY   3800    ///< DDR 7600
#define DDR7700_FREQUENCY   3850    ///< DDR 7700
#define DDR7800_FREQUENCY   3900    ///< DDR 7800
#define DDR7900_FREQUENCY   3950    ///< DDR 7900
#define DDR8000_FREQUENCY   4000    ///< DDR 8000
#define DDR8100_FREQUENCY   4050    ///< DDR 8100
#define DDR8200_FREQUENCY   4100    ///< DDR 8200
#define DDR8300_FREQUENCY   4150    ///< DDR 8300
#define DDR8400_FREQUENCY   4200    ///< DDR 8400
#define DDR8500_FREQUENCY   4250    ///< DDR 8500
#define DDR8600_FREQUENCY   4300    ///< DDR 8600
#define DDR8700_FREQUENCY   4350    ///< DDR 8700
#define DDR8800_FREQUENCY   4400    ///< DDR 8800
#define DDR8900_FREQUENCY   4450    ///< DDR 8900
#define DDR9000_FREQUENCY   4500    ///< DDR 9000
#define DDR9100_FREQUENCY   4550    ///< DDR 9100
#define DDR9200_FREQUENCY   4600    ///< DDR 9200
#define DDR9300_FREQUENCY   4650    ///< DDR 9300
#define DDR9400_FREQUENCY   4700    ///< DDR 9400
#define DDR9500_FREQUENCY   4750    ///< DDR 9500
#define DDR9600_FREQUENCY   4800    ///< DDR 9600
#define DDR9700_FREQUENCY   4850    ///< DDR 9700
#define DDR9800_FREQUENCY   4900    ///< DDR 9800
#define DDR9900_FREQUENCY   4950    ///< DDR 9900
#define DDR10000_FREQUENCY  5000    ///< DDR 10000
#define DDR10100_FREQUENCY  5050    ///< DDR 10100
#define DDR10200_FREQUENCY  5100    ///< DDR 10200
#define DDR10300_FREQUENCY  5150    ///< DDR 10300
#define DDR10400_FREQUENCY  5200    ///< DDR 10400
#define DDR10500_FREQUENCY  5250    ///< DDR 10500
#define DDR10600_FREQUENCY  5300    ///< DDR 10600
#define DDR10700_FREQUENCY  5350    ///< DDR 10700
#define DDR10800_FREQUENCY  5400    ///< DDR 10800
#define DDR10900_FREQUENCY  5450    ///< DDR 10900
#define DDR11000_FREQUENCY  5500    ///< DDR 10000
#define DDR11100_FREQUENCY  5550    ///< DDR 11100
#define DDR11200_FREQUENCY  5600    ///< DDR 11200
#define DDR11300_FREQUENCY  5650    ///< DDR 11300
#define DDR11400_FREQUENCY  5700    ///< DDR 11400
#define DDR11500_FREQUENCY  5750    ///< DDR 11500
#define DDR11600_FREQUENCY  5800    ///< DDR 11600
#define DDR11700_FREQUENCY  5850    ///< DDR 11700
#define DDR11800_FREQUENCY  5900    ///< DDR 11800
#define DDR11900_FREQUENCY  5950    ///< DDR 11900
#define DDR12000_FREQUENCY  6000    ///< DDR 12000
#define UNSUPPORTED_DDR_FREQUENCY 6001 ///< Highest limit of DDR frequency

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */

#endif




