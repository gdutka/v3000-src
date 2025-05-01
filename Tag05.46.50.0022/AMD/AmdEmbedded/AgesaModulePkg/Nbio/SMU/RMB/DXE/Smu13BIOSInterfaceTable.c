/*
*****************************************************************************
*
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific SMU 13 Bios Interface Table
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2019-06-26 15:08:01 -0600 (Wed, 26 June 2019) $
 *
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific SMU 13 Bios Interface Table
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2019-06-26 15:08:01 -0600 (Wed, 26 June 2019) $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <smu13_bios_if_RMB.h>

#define FILECODE  NBIO_SMU_RMB_DXE_SMU13BIOSINTERFACETABLE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
Smu13BiosIfTable_t PPTableDefault = {

  SMU13_BIOS_IF_VERSION, //  uint32_t Version;

  //PCD infrastructure limits
  0, //  uint32_t SUSTAINED_POWER_LIMIT;           //Power [mWatts] (STAPM)
  0, //  uint32_t FAST_PPT_LIMIT;                  //Power [mWatts] (fPPT)
  0, //  uint32_t SLOW_PPT_LIMIT;                  //Power [mWatts] (sPPT)
  0, //  uint32_t SLOW_PPT_TIME_CONSTANT;          //Time constant (seconds)
  0, //  uint8_t  SYSTEM_TEMPERATURE_TRACKING;     //Tskin feedback from EC; 1 = enabled; 0 = disabled
  0, //  uint8_t  STAPM_BOOST_OVERRIDE;            //1 = use value in STAPM_BOOST; 0 = use SMU FW default
  0, //  uint8_t  STAPM_BOOST;                     //1 = enabled; 0 = disabled
  0, //  uint8_t  AA_MODE_EN;                      //A+A power/thermal sharing algorithm; 1 = enable; 0 = disable
  0, //  uint32_t STAPM_TIME_CONSTANT;             //Time constant (seconds)
  0, //  uint32_t THERMAL_LIMIT;                   //TjMax ['C] (THM)
  0, //  uint32_t VRM_VDD_CURRENT_LIMIT;           //Current [mA] (VDDCR_VDD TDC)
  0, //  uint32_t VRM_VDD_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_VDD EDC)
  0, //  uint32_t VRM_SOC_CURRENT_LIMIT;           //Current [mA] (VDDCR_SOC TDC)
  0, //  uint32_t VRM_SOC_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_SOC EDC)
  0, //  uint32_t VRM_SR_CURRENT_LIMIT;            //Current [mA] (VDDCR_SR  TDC)
  0, //  uint32_t VRM_SR_MAXIMUM_CURRENT_LIMIT;    //Current [mA] (VDDCR_SR  EDC)
  0, //  uint32_t PSI3_VDD_CURRENT_LIMIT;          //Current [mA] (VDDCR_VDD PSI3)
  0, //  uint32_t PROCHOT_L_DEASSERTION_RAMP_TIME; //Time [ms] (PROCHOT)
  0, //  uint32_t P3T_LIMIT;                       //Power [Watts] (P3T - Peak Package Power)
  0, //  uint8_t  SYSTEM_CONFIG;                   //0 = use fused default; >0 = user specified value
  0, //  uint8_t  SW_SCI_GPE_ID; //GPE Event ID [0-31] used by MP1 to trigger software SCI (needs to be edge configured)
  0, //  uint8_t  EmbeddedPlatformEn
  0, //  uint8_t  iTempPart
  0, //  uint8_t  FUSAReset
  {0,0,0}, //  uint8_t  spare0[3];

  //STT parameters/co-efficients
  0, //  uint8_t  STT_PCB_SENSOR_COUNT;           //2 = APU only; 3 = APU+dGPU; all other values are invalid
  0, //  uint8_t  STT_spare;
  0, //  uint16_t STT_MIN_POWER_LIMIT;            //Unsigned Q8.8 encoding, Minimum APU power limit for STT control
  0, //  int16_t  STT_M1;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M2;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M3;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M4;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M5;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M6;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_C_APU;                      //Signed Q8.8 encoding
  0, //  int16_t  STT_C_HS2;                      //Signed Q8.8 encoding
  0, //  uint16_t STT_ALPHA_APU;                  //Unsigned Q0.16 encoding
  0, //  uint16_t STT_ALPHA_HS2;                  //Unsigned Q0.16 encoding
  0, //  uint16_t STT_SKIN_TEMPERATURE_LIMIT_APU; //Unsigned Q8.8 encoding
  0, //  uint16_t STT_SKIN_TEMPERATURE_LIMIT_HS2; //Unsigned Q8.8 encoding
  0, //  uint16_t STT_ERROR_COEFF;                //Unsigned Q0.16 encoding
  0, //  uint16_t STT_ERROR_RATE_COEFF;           //Unsigned Q0.16 encoding

  //PCD/CBS fan table
  0, //  uint8_t FanTable_Override; //[1 means use the full set of data specified below; 0 means use default fan table]
  0, //  uint8_t FanTable_Hysteresis;
  0, //  uint8_t FanTable_TempLow;
  0, //  uint8_t FanTable_TempMed;
  0, //  uint8_t FanTable_TempHigh;
  0, //  uint8_t FanTable_TempCritical;
  0, //  uint8_t FanTable_PwmLow;
  0, //  uint8_t FanTable_PwmMed;
  0, //  uint8_t FanTable_PwmHigh;
  0, //  uint8_t FanTable_PwmFreq;      //[0 = 25kHz; 1 = 100Hz]
  0, //  uint8_t FanTable_Polarity;     //[0 = negative; 1 = positive]
  0, //  uint8_t FanTable_spare;

  //CBS debug options
  0, //  int16_t  CoreDldoPsmMargin;           //[PSM count] [signed]
  0, //  int16_t  GfxDldoPsmMargin;            //[PSM count] [signed]
  0, //  int16_t  VddcrVddVoltageMargin;       //[mV] [signed]
  0, //  int16_t  VddcrSocVoltageMargin;       //[mV] [signed]
  0, //  int16_t  VddcrSrVoltageMargin;       //[mV] [signed]
  0, //  uint16_t ForceVddcrVddVoltage;        //[mV; 0 means no force]
  0, //  uint16_t ForceVddcrSocVoltage;        //[mV; 0 means no force]
  0, //  uint16_t ForceVddcrSrVoltage;        //[mV; 0 means no force]

  0, //  uint8_t  C1eEnabled;                  //[0 = disable C1e; 1 = enable C1e]
  0, //  uint8_t  CCX_VdciAsync;               //[0 = predictive mode; 1 = async mode]
  0, //  uint8_t  Voltage_Calc_Workaround;     //[0 = use telemetry voltage] [1 = calculate load voltage as SetVoltage - Current]
  0, //  uint8_t  ForceFanPwmEn;               //[1 means use the ForceFanPwm value below]
  0, //  uint8_t  ForceFanPwm;                 //[% 0-100]
  0, //  uint8_t  DfsBypassOff;                //PMFW-995
  0, //  uint8_t  CoreCountControlEn;          //[0 = Disable 1 = Enable CoreCountControl feature]
  0, //  uint8_t  HTFmaxTemperature;           //High temperature Fmax --- temperature ['C]
  0, //  uint16_t HTFmaxFrequency;             //High temperature Fmax --- frequency [MHz]
  0, //  uint16_t ForceCclkFrequency;          //Force the CCLK frequency to the specified frequency [MHz]
  0, //  uint16_t CclkFmaxOverride;            //Specify the CCLK Fmax override [MHz]
  0, //  uint16_t CclkFminOverride;            //Specify the CCLK Fmin override [MHz]
  0, //  uint16_t ForceGfxclkFrequency;        //Force the GFXCLK frequency to the specified frequency [MHz]
  0, //  uint16_t GfxclkFmaxOverride;          //Specify the GFXCLK Fmax override [MHz]
  0, //  uint8_t PostCodeLevel; //Postcode and STB Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  0, //  uint8_t  StbVerbosityLevel;         //STB Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  0, //  uint8_t  DisableZ10;
  0, //  uint8_t  ZstatesMaskEnable;
  0, //  uint32_t ZscIpIdleMask;
  0, //  uint32_t ZscWakeEventMask0;
  0, //  uint32_t ZscWakeEventMask1;
  0, //  uint32_t ZscIpFenceAckMask;

  //PCD/CBS Telemetry setup
  0, //  uint32_t TelemetryVddcrVddSlope;
  0, //  int32_t  TelemetryVddcrVddOffset;
  0, //  uint32_t TelemetryVddcrSocSlope;
  0, //  int32_t  TelemetryVddcrSocOffset;
  0, //  uint32_t TelemetryVddcrSrSlope;
  0, //  int32_t  TelemetryVddcrSrOffset;

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  0, //  uint8_t  OC_DISABLE; //1=disable overclocking; 0=allow overclocking
  0, //  uint8_t  OC_MINVID;  //[VID] SVI2 VID encoding
  0, //  uint16_t OC_FREQMAX; //MHz

  //DF CSTATE CONFIG
  0, //  uint8_t DfCstateConfigOverride;
  0, //  uint8_t DfCstateMmStutterOptEn;
  0, //  uint8_t DfCstateMmPwrGateOptEn;
  0, //  uint8_t DfCstateDceElptModeEn;
  0, //  uint8_t DfCstatePwrGateEn;
  0, //  uint8_t DfCstateClkPwrDnEn;
  0, //  uint8_t DfCstateSelfRefrEn;
  0, //  uint8_t DfCstateSpare;

  //DF Light CSTATE CONFIG
  1, //  uint8_t DfLightCstateConfigOverride;
  8, //  uint8_t CcmHystDly;
  2, //  uint8_t Ccmhystscale;
  4, //  uint8_t Ccmmonitorsuccdly;
  2, //  uint8_t Ccmmonitorsuccscale;
  7, //  uint8_t Ccmmonitorsatlvl;
  6, //  uint8_t Ccmmonitorsuccthr;
  2, //  uint8_t Ccmmonitorfaildec;
  3, //  uint8_t Ccmmonitorburstfilter;
  0, //  uint8_t Ccmmonitorburstscale;
  {0, 0}, //  uint8_t  spare2[2];

  //CPPC Constraints
  0, //  uint8_t CppcConstraintsEnabled;
  {0, 0, 0}, //  uint8_t spare3[3];
  0, //  uint8_t PerfLimit_MaxRange;
  0, //  uint8_t PerfLimit_MinRange;
  0, //  uint8_t EPP_MaxRange;
  0, //  uint8_t EPP_MinRange;

  //Misc
  0, //  uint32_t System;
  0, //  uint32_t MinS0i3SleepTimeInMs;
  0, //  uint32_t SystemRtcTime;

  //GFX OC limits
  0, //  uint8_t  OC_GFX_DISABLE;  //1=disable overclocking; 0=allow overclocking
  0, //  uint8_t  OC_GFX_MINVID;   //[VID] SVI2 VID encoding=
  0, //  uint16_t OC_GFX_FREQMAX;  //MHz

  //GFXCLK/GFXOFF related parameters
  0, //  uint8_t   GceaLinkMgrIdleThreshold;        //Set by SMU FW during enablment of SOC_ULV. Controls delay for GFX SDP port disconnection during idle events
  0, //  uint8_t   GfxClkSource;                    // 0 = PLL, 1 = DFLL
  0, //  uint8_t   DisableMsmuSecurityRestore;      //Set to 1 to use PSP instead of MSMU for security restore. MSMU is the POR path.
  0, //  uint8_t   ApccEnable;//1 = Enable APCC, should be mutually exclusive to GFX EDC controller
  0, //  uint16_t  GfxVinit;                        // Voltage required to support Finit during GFX power up/GFXOFF exit (in mV)
  0, //  uint16_t  GfxclkFinit;                     // in Mhz. Options are 800, 600, 400, 300, 200, 100
  0, //  uint16_t  GfxclkFgfxoffEntry;              // in Mhz
  0, //  uint16_t  GfxclkFidle;                     // in MHz
  0, //  uint16_t  GfxclkSlewRate;                  // for PLL babystepping???
  0, //  uint16_t  GfxclkFminOverride;              // Specify the GFXCLK Fmin override [MHz]
  0, //  uint16_t  GfxMinVoltage;                   //Minimum GFX voltage, in mV
  0, //  uint16_t  GfxMaxVoltage;                   //Maximum GFX voltage, in mV

  //GFX temperature dependent Vmin feature
  0, //  uint8_t VDDGFX_TVminOverrideEn;
  {0, 0, 0}, //  uint8_t spare4[3];
  0, //  uint16_t VDDGFX_TVmin;       //Celcius
  0, //  uint16_t VDDGFX_Vmin_HiTemp; // mV Q2
  0, //  uint16_t VDDGFX_Vmin_LoTemp; // mV Q2
  0, //  uint16_t VDDGFX_TVminHystersis; // Celcius

  // Enable automated phase shedding and diode emulation (PSI7)
  1, //  uint8_t VddcrVddPsi7En;
  0, //  uint8_t VddcrSocPsi7En;
  0, //  uint8_t VddcrSrPsi7En;
  0, //  uint8_t PsiDecayConditionVddRail;
  0, //  uint8_t PsiDecayConditionSocRail;
  0, //  uint8_t PsiDecayConditionSrRail;
  1, //  uint8_t PsiDecayConditionOverride; // Override bit to PSI Decay Condition
  0, //  uint8_t spare5;


  0, //  uint8_t  VddmCldoFastStartupEn;
  0, //  uint8_t  VddmCldoOvDetEn;
  0, //  uint16_t PllSscSpreadPercentage; // PLL Spread down percentage, Q8 format. e.g. 192 gives 0.75%
  1, //  uint8_t  FclkDpmOptimizationEn;  //[0: disable; 1: enable]
  0, //  uint8_t  DisplayIdleOptimizationEn; //[0: disable; 1: enable]
  0, //  uint8_t  ApplyDefaultNrepFlopSel;    //ApplyDefaultNrepFlopSel is controlled by PcdPll5SscDisable
  0, //  uint8_t  GfxDldoBypass;
  0, //  uint16_t  LclkMaxFreq;
  0, //  uint16_t  LclkMinFreq;
  0, //  uint16_t VDD_MISC_Voltage;       // Voltage in mV (used in AOD menu)
  0, //  uint16_t spare6;

  0, //  uint8_t  SlewRateOverrideVdd; // Set this field to indicate the Slew Rate must change
  0, //  uint8_t  SlewRateUpIndexVdd;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  1, //  uint8_t  SlewRateDownCntlVdd; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  spare7;

  {0, 0, 0, 0, 0, 0, 0, 0}, //int16_t  PerCoreDldoPsmMargin[8]; //PSM count for each core [signed]

  // Slew rate control for VDDCR_SOC and VDDCR_SR rails
  0, //  uint8_t  SlewRateOverrideSoc; // Set this field to indicate the Slew Rate must change
  0, //  uint8_t  SlewRateUpIndexSoc;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  0, //  uint8_t  SlewRateDownCntlSoc; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  spare8;
  
  0, //uint8_t  ForceFCLKLevel;
  0, //uint8_t  ForceLCLKLevel;
  {0,0}, //uint8_t  spare9[2];

  //SPARE FOR FUTURE USE
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //  uint32_t spare10[32];

};


