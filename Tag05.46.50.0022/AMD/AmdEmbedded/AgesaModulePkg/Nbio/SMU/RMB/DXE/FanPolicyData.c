/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include "FanPolicy.h"

// automatic fan policy table
FAN_POLICY_TABLE  DefaultAM5FanTable[] = {
  // 35W TDP
  0x230000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    5,   // FanTable_Hysteresis
    0,   // FanTable_TempLow
    50,  // FanTable_TempMed
    90,  // FanTable_TempHigh
    100, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    0,   // FanTable_PwmMed
    4,   // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 45W TDP
  0x2D0000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    5,   // FanTable_Hysteresis
    0,   // FanTable_TempLow
    90,  // FanTable_TempMed
    97,  // FanTable_TempHigh
    100, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    7,   // FanTable_PwmMed
    100, // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 65W TDP
  0x410000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    5,   // FanTable_Hysteresis
    0,   // FanTable_TempLow
    50,  // FanTable_TempMed
    90,  // FanTable_TempHigh
    100, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    50,  // FanTable_PwmMed
    58,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // null - end of table
  0x0,
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

FAN_POLICY_TABLE  DefaultFP7FanTable[] = {
  // 15W TDP
  0xF0000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    52,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    8,   // FanTable_PwmMed
    15,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 28W TDP
  0x1C0000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    70,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    8,   // FanTable_PwmMed
    18,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 30W TDP
  0x1E0000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    73,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    8,   // FanTable_PwmMed
    18,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 35W TDP
  0x230000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    66,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    20,  // FanTable_PwmMed
    45,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 45W TDP
  0x2D0000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    72,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    20,  // FanTable_PwmMed
    53,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // 54W TDP
  0x360000,
  { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
    0,   // ForceFanPwm
    1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
    4,   // FanTable_Hysteresis
    50,  // FanTable_TempLow
    51,  // FanTable_TempMed
    79,  // FanTable_TempHigh
    105, // FanTable_TempCritical
    0,   // FanTable_PwmLow
    20,  // FanTable_PwmMed
    53,  // FanTable_PwmHigh
    0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
    1    // FanTable_Polarity [0 = negative; 1 = positive]
  },
  // null - end of table
  0x0,
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

