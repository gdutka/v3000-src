/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains the definition of the SMU13
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: Nbio/SMU
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */
#ifndef SMU13_BIOS_IF_H
#define SMU13_BIOS_IF_H

//
// Assume standard IA-32 alignment.
//
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;

// *** IMPORTANT ***
// SMU TEAM: Always increment the interface version if
// any structure is changed in this file
#define SMU13_BIOS_IF_VERSION 4

typedef struct {
  uint32_t Version;

  //PCD infrastructure limits (build time settings, used by customer)
  //These show up in the NDA BIOS sent to customer
  uint32_t SUSTAINED_POWER_LIMIT;           //Power [mWatts] (STAPM)
  uint32_t FAST_PPT_LIMIT;                  //Power [mWatts] (fPPT)
  uint32_t SLOW_PPT_LIMIT;                  //Power [mWatts] (sPPT)
  uint32_t SLOW_PPT_TIME_CONSTANT;          //Time constant (seconds)
  uint8_t  SYSTEM_TEMPERATURE_TRACKING;     //Tskin feedback from EC; 1 = enabled; 0 = disabled
  uint8_t  STAPM_BOOST_OVERRIDE;            //1 = use value in STAPM_BOOST; 0 = use SMU FW default
  uint8_t  STAPM_BOOST;                     //1 = enabled; 0 = disabled
  uint8_t  spare;
  uint32_t STAPM_TIME_CONSTANT;             //Time constant (seconds)
  uint32_t THERMAL_LIMIT;                   //TjMax ['C] (THM)
  uint32_t VRM_VDD_CURRENT_LIMIT;               //Current [mA] (VDDCR_VDD TDC)
  uint32_t VRM_SOC_CURRENT_LIMIT;           //Current [mA] (VDDCR_SOC TDC)
  uint32_t VRM_GFX_CURRENT_LIMIT;           //Current [mA] (VDDCR_GFX TDC)
  uint32_t VRM_CVIP_CURRENT_LIMIT;          //Current [mA] (VDDCR_CVIP TDC)
  uint32_t VRM_VDD_MAXIMUM_CURRENT_LIMIT;       //Current [mA] (VDDCR_VDD EDC)
  uint32_t VRM_SOC_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_SOC EDC)
  uint32_t VRM_GFX_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_GFX EDC)
  uint32_t VRM_CVIP_MAXIMUM_CURRENT_LIMIT;  //Current [mA] (VDDCR_CVIP EDC)
  uint32_t PSI3_CPU_CURRENT_LIMIT;          //Current [mA] (VDDCR_VDD PSI3)
  uint32_t PSI3_GFX_CURRENT_LIMIT;          //Current [mA] (VDDCR_GFX PSI3)
  uint32_t PROCHOT_L_DEASSERTION_RAMP_TIME; //Time [ms] (PROCHOT)
  uint8_t  SYSTEM_CONFIG;                   //0 = use fused default; >0 = user specified value
  uint8_t  SW_SCI_GPE_ID;                   //GPE Event ID [0-31] used by MP1 to trigger software SCI (needs to be edge configured)
  uint8_t  DebugEnabled;
  uint8_t  TargSvc;

  //STT parameters/co-efficients
  uint8_t  STT_PCB_SENSOR_COUNT;           //2 = APU only; 3 = APU+dGPU; all other values are invalid
  uint8_t  STT_spare;
  uint16_t STT_MIN_POWER_LIMIT;            //Minimum APU power limit for STT control [mWatts]
  int16_t  STT_M1;                         //Signed Q6.10 encoding
  int16_t  STT_M2;                         //Signed Q6.10 encoding
  int16_t  STT_M3;                         //Signed Q6.10 encoding
  int16_t  STT_M4;                         //Signed Q6.10 encoding
  int16_t  STT_M5;                         //Signed Q6.10 encoding
  int16_t  STT_M6;                         //Signed Q6.10 encoding
  int16_t  STT_C_APU;                      //Signed Q6.10 encoding
  int16_t  STT_C_GPU;                      //Signed Q6.10 encoding
  uint16_t STT_ALPHA_APU;                  //Unsigned Q0.16 encoding
  uint16_t STT_ALPHA_GPU;                  //Unsigned Q0.16 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_APU; //Unsigned Q8.8 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_GPU; //Unsigned Q8.8 encoding
  uint16_t STT_ERROR_COEFF;                //Unsigned Q0.16 encoding
  uint16_t STT_ERROR_RATE_COEFF;           //Unsigned Q0.16 encoding

  //PCD/CBS fan table
  uint8_t  FanTable_Override;     //[1 means use the full set of data specified below; 0 means use default fan table]
  uint8_t  FanTable_Hysteresis;
  uint8_t  FanTable_TempLow;
  uint8_t  FanTable_TempMed;
  uint8_t  FanTable_TempHigh;
  uint8_t  FanTable_TempCritical;
  uint8_t  FanTable_PwmLow;
  uint8_t  FanTable_PwmMed;
  uint8_t  FanTable_PwmHigh;
  uint8_t  FanTable_PwmFreq;      //[0 = 25kHz; 1 = 100Hz]
  uint8_t  FanTable_Polarity;     //[0 = negative; 1 = positive]
  uint8_t  FanTable_spare;

  //CBS debug options (not exposed to customers, only for internal debug)
  //In the NDA BIOS, the default values are populated to these options

  int16_t  CoreDldoPsmMargin;           //[PSM count] [signed]
  int16_t  GfxDldoPsmMargin;            //[PSM count] [signed]
  int16_t  VddcrCpuVoltageMargin;       //[mV] [signed]
  int16_t  VddcrSocVoltageMargin;       //[mV] [signed]
  int16_t  VddcrGfxVoltageMargin;       //[mV] [signed]
  int16_t  VddcrCvipVoltageMargin;      //[mV] [signed]
  uint16_t ForceVddcrCpuVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrSocVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrGfxVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrCvipVoltage;       //[mV; 0 means no force]
  uint8_t  CC1Dis;                      //[0 = CC1 enabled; 1 = CC1 disabled]
  uint8_t  CCX_VdciAsync;               //[0 = predictive mode; 1 = async mode]
  uint8_t  ForceFanPwmEn;               //[1 means use the ForceFanPwm value below]
  uint8_t  ForceFanPwm;                 //[% 0-100]
  uint8_t  DfsBypassOff;                //PMFW-995
  uint8_t  HTFmaxTemperature;           //High temperature Fmax --- temperature ['C]
  uint16_t HTFmaxFrequency;             //High temperature Fmax --- frequency [MHz]
  uint16_t ForceCclkFrequency;          //Force the CCLK frequency to the specified frequency [MHz]
  uint16_t CclkFmaxOverride;            //Specify the CCLK Fmax override [MHz]
  uint16_t ForceGfxclkFrequency;        //Force the GFXCLK frequency to the specified frequency [MHz]
  uint16_t GfxclkFmaxOverride;          //Specify the GFXCLK Fmax override [MHz]
  uint8_t  CcxMsmuDis;                  // [0 = Use CCX MSMU; 1 = Use DMA for save/restore regs during CPUOFF]
  uint8_t  CoreCountControlEn;          // [0 = Disable 1 = Enable CoreCountControl feature]
  uint8_t  spare2[2];

  //DF CSTATE CONFIG
  uint8_t DfCstateConfigOverride;
  uint8_t DfCstateMmStutterOptEn;
  uint8_t DfCstateMmPwrGateOptEn;
  uint8_t DfCstateDceElptModeEn;
  uint8_t DfCstatePwrGateEn;
  uint8_t DfCstateClkPwrDnEn;
  uint8_t DfCstateSelfRefrEn;
  uint8_t DfCstateSpare;

  //CPPC Constraints
  uint8_t CppcConstraintsEnabled;       //Enable constraints on the processor performance level and energy performance preference (EPP)
  uint8_t spare3[3];
  uint8_t PerfLimit_MaxRange;           //Max performance level processor may reach if CPPC constraint is enabled
  uint8_t PerfLimit_MinRange;           //Min performance level to be maintained if CPPC constraint is enabled
  uint8_t EPP_MaxRange;                 //Max EPP value allowed if CPPC constraint is enabled
  uint8_t EPP_MinRange;                 //Min EPP value allowed if CPPC constraint is enabled

  //Misc
  uint32_t MinS0i3SleepTimeInMs;

  //GFXCLK/GFXOFF related parameters
  uint8_t GfxClkSource;                    //0 = DFLL, 1 = PLL
  uint8_t DisableMsmuSecurityRestore;      //1 = SMU to handshake with PSP for security programming, 0 = use MSMU (default/POR)
  uint8_t ApccEnable;                      //1 = Enable APCC, should be mutually exclusive to GFX EDC controller
  uint8_t spare4;
  uint16_t GfxVinit;                        // Voltage required to support Finit during GFX power up/GFXOFF exit (in mV)
  uint16_t GfxclkFinit;                     // in Mhz. Options are 800, 600, 400, 300, 200, 100
  uint16_t GfxclkFgfxoffEntry;              // in Mhz
  uint16_t GfxclkFidle;                     // in MHz
  uint16_t GfxMinVoltage;                   //Minimum GFX voltage, in mV
  uint16_t GfxMaxVoltage;                   //Maximum GFX voltage, in mV

  //GFX temperature dependent Vmin feature
  uint8_t  VDDGFX_TVminOverrideEn;
  uint8_t  spare5[3];
  uint16_t VDDGFX_TVmin;       //Celcius
  uint16_t VDDGFX_Vmin_HiTemp; // mV Q2
  uint16_t VDDGFX_Vmin_LoTemp; // mV Q2
  uint16_t VDDGFX_TVminHystersis; // Celcius

  // Enable automated phase shedding and diode emulation (PSI7)
  uint8_t VddcrCpuPsi7En;
  uint8_t VddcrSocPsi7En;
  uint8_t VddcrGfxPsi7En;
  uint8_t VddcrCvipPsi7En;

  //PCD/CBS Telemetry setup
  int32_t TelemetryVddcrCpuOffset;
  int32_t TelemetryVddcrSocOffset;
  int32_t TelemetryVddcrGfxOffset;
  int32_t TelemetryVddcrCvipOffset;
  uint32_t padding[32]; // BIOS to populate the padding with 0x0. This is needed for backward compatibility.
} Smu13BiosIfTable_t;

typedef struct {
  //PCIe LANE MAPPING [NBIO_QUADRANT][PCIe_CORE][DEVICE]
  uint8_t PCIe_GenSpeed[4][2][8];    //0=NOT_CONNECTED; 1=GEN1; 2=GEN2; 3=GEN3; 4=GEN4; 5=GEN4+(ESM)
  uint8_t PCIe_LaneWidth[4][2][8];   //LANE_WIDTH [0=NOT_CONNECTED; 1=x1; 2=x2; 4=x4; 8=x8; 16=x16]
} PcieLaneMappingTable_t;

#endif
