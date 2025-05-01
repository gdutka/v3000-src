/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains the definition of the SMU11
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: Nbio/SMU
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */
#ifndef SMU13_BIOS_IF_RMB_H
#define SMU13_BIOS_IF_RMB_H

//
// Assume standard IA-32 alignment.
// BugBug: Need to check portability of long long
//
#ifndef SMU_DATA_TYPES_H
#define SMU_DATA_TYPES_H
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
#endif
// *** IMPORTANT ***
// SMU TEAM: Always increment the interface version if
// any structure is changed in this file
#define SMU13_BIOS_IF_VERSION 18

typedef struct {
  uint32_t Version;

  //PCD infrastructure limits
  uint32_t SUSTAINED_POWER_LIMIT;           //Power [mWatts] (STAPM)
  uint32_t FAST_PPT_LIMIT;                  //Power [mWatts] (fPPT)
  uint32_t SLOW_PPT_LIMIT;                  //Power [mWatts] (sPPT)
  uint32_t SLOW_PPT_TIME_CONSTANT;          //Time constant (seconds)
  uint8_t  SYSTEM_TEMPERATURE_TRACKING;     //Tskin feedback from EC; 1 = enabled; 0 = disabled
  uint8_t  STAPM_BOOST_OVERRIDE;            //1 = use value in STAPM_BOOST; 0 = use SMU FW default
  uint8_t  STAPM_BOOST;                     //1 = enabled; 0 = disabled
  uint8_t  AA_MODE_EN;                      //A+A power/thermal sharing algorithm; 1 = enable; 0 = disable
  uint32_t STAPM_TIME_CONSTANT;             //Time constant (seconds)
  uint32_t THERMAL_LIMIT;                   //TjMax ['C] (THM)
  uint32_t VRM_VDD_CURRENT_LIMIT;           //Current [mA] (VDDCR_VDD TDC)
  uint32_t VRM_VDD_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_VDD EDC)
  uint32_t VRM_SOC_CURRENT_LIMIT;           //Current [mA] (VDDCR_SOC TDC)
  uint32_t VRM_SOC_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_SOC EDC)
  uint32_t VRM_SR_CURRENT_LIMIT;            //Current [mA] (VDDCR_SR  TDC)
  uint32_t VRM_SR_MAXIMUM_CURRENT_LIMIT;    //Current [mA] (VDDCR_SR  EDC)
  uint32_t PSI3_VDD_CURRENT_LIMIT;          //Current [mA] (VDDCR_VDD PSI3)
  uint32_t PROCHOT_L_DEASSERTION_RAMP_TIME; //Time [ms] (PROCHOT)
  uint32_t P3T_LIMIT;                       //Power [Watts] (P3T - Peak Package Power)
  uint8_t  SYSTEM_CONFIG;                   //0 = use fused default; >0 = user specified value
  uint8_t  SW_SCI_GPE_ID;                   //GPE Event ID [0-31] used by MP1 to trigger software SCI (needs to be edge configured)
  uint8_t  EmbeddedPlatformEn;  //unit8 spare0[2] to the approved as of now will use LS bit as 1 to indicate the embedded platform and other bits can be used for future reference
  uint8_t  iTempPart;
  uint8_t  FUSAReset;                       //Indicates MP1 action in case of SMN timeout or SLV error
  uint8_t  spare0[3];


  //STT parameters/co-efficients
  uint8_t  STT_PCB_SENSOR_COUNT;           //2 = APU only; 3 = APU+dGPU; all other values are invalid
  uint8_t  STT_spare;
  uint16_t STT_MIN_POWER_LIMIT;            //Unsigned Q8.8 encoding, Minimum APU power limit for STT control
  int16_t  STT_M1;                         //Signed Q6.10 encoding
  int16_t  STT_M2;                         //Signed Q6.10 encoding
  int16_t  STT_M3;                         //Signed Q6.10 encoding
  int16_t  STT_M4;                         //Signed Q6.10 encoding
  int16_t  STT_M5;                         //Signed Q6.10 encoding
  int16_t  STT_M6;                         //Signed Q6.10 encoding
  int16_t  STT_C_APU;                      //Signed Q8.8 encoding
  int16_t  STT_C_HS2;                      //Signed Q8.8 encoding
  uint16_t STT_ALPHA_APU;                  //Unsigned Q0.16 encoding
  uint16_t STT_ALPHA_HS2;                  //Unsigned Q0.16 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_APU; //Unsigned Q8.8 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_HS2; //Unsigned Q8.8 encoding
  uint16_t STT_ERROR_COEFF;                //Unsigned Q0.16 encoding
  uint16_t STT_ERROR_RATE_COEFF;           //Unsigned Q0.16 encoding

  //PCD/CBS fan table
  uint8_t FanTable_Override;     //[1 means use the full set of data specified below; 0 means use default fan table]
  uint8_t FanTable_Hysteresis;
  uint8_t FanTable_TempLow;
  uint8_t FanTable_TempMed;
  uint8_t FanTable_TempHigh;
  uint8_t FanTable_TempCritical;
  uint8_t FanTable_PwmLow;
  uint8_t FanTable_PwmMed;
  uint8_t FanTable_PwmHigh;
  uint8_t FanTable_PwmFreq;      //[0 = 25kHz; 1 = 100Hz]
  uint8_t FanTable_Polarity;     //[0 = negative; 1 = positive]
  uint8_t FanTable_spare;

  //CBS debug options
  int16_t  CoreDldoPsmMargin;           //[PSM count] [signed]
  int16_t  GfxDldoPsmMargin;            //[PSM count] [signed]
  int16_t  VddcrVddVoltageMargin;       //[mV] [signed]
  int16_t  VddcrSocVoltageMargin;       //[mV] [signed]
  int16_t  VddcrSrVoltageMargin;        //[mV] [signed]
  uint16_t ForceVddcrVddVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrSocVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrSrVoltage;         //[mV; 0 means no force]

  uint8_t  C1eEnabled;                  //[0 = disable C1e; 1 = enable C1e]
  uint8_t  CCX_VdciAsync;               //[0 = predictive mode; 1 = async mode]
  uint8_t  Voltage_Calc_Workaround;     //[0 = use telemetry voltage] [1 = calculate load voltage as SetVoltage - Current]
  uint8_t  ForceFanPwmEn;               //[1 means use the ForceFanPwm value below]
  uint8_t  ForceFanPwm;                 //[% 0-100]
  uint8_t  DfsBypassOff;                //PMFW-995
  uint8_t  CoreCountControlEn;          //[0 = Disable 1 = Enable CoreCountControl feature]
  uint8_t  HTFmaxTemperature;           //High temperature Fmax --- temperature ['C]
  uint16_t HTFmaxFrequency;             //High temperature Fmax --- frequency [MHz]
  uint16_t ForceCclkFrequency;          //Force the CCLK frequency to the specified frequency [MHz]
  uint16_t CclkFmaxOverride;            //Specify the CCLK Fmax override [MHz]
  uint16_t CclkFminOverride;            //Specify the CCLK Fmin override [MHz]
  uint16_t ForceGfxclkFrequency;        //Force the GFXCLK frequency to the specified frequency [MHz]
  uint16_t GfxclkFmaxOverride;          //Specify the GFXCLK Fmax override [MHz]
  uint8_t  PostCodeLevel;               //Postcode Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  uint8_t  StbVerbosityLevel;           //STB Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  uint8_t  DisableZ10;
  uint8_t  ZstatesMaskEnable;
  uint32_t ZscIpIdleMask;
  uint32_t ZscWakeEventMask0;
  uint32_t ZscWakeEventMask1;
  uint32_t ZscIpFenceAckMask;

  //PCD/CBS Telemetry setup
  uint32_t TelemetryVddcrVddSlope;
  int32_t  TelemetryVddcrVddOffset;
  uint32_t TelemetryVddcrSocSlope;
  int32_t  TelemetryVddcrSocOffset;
  uint32_t TelemetryVddcrSrSlope;
  int32_t  TelemetryVddcrSrOffset;

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  uint8_t  OC_DISABLE; //1=disable overclocking; 0=allow overclocking
  uint8_t  OC_MINVID;  //[VID] SVI2 VID encoding
  uint16_t OC_FREQMAX; //MHz

  //DF CSTATE CONFIG
  uint8_t DfCstateConfigOverride;
  uint8_t DfCstateMmStutterOptEn;
  uint8_t DfCstateMmPwrGateOptEn;
  uint8_t DfCstateDceElptModeEn;
  uint8_t DfCstatePwrGateEn;
  uint8_t DfCstateClkPwrDnEn;
  uint8_t DfCstateSelfRefrEn;
  uint8_t DfCstateSpare;

  //DF Light CSTATE CONFIG
  uint8_t DfLightCstateConfigOverride;
  uint8_t CcmHystDly;
  uint8_t Ccmhystscale;
  uint8_t Ccmmonitorsuccdly;
  uint8_t Ccmmonitorsuccscale;
  uint8_t Ccmmonitorsatlvl;
  uint8_t Ccmmonitorsuccthr;
  uint8_t Ccmmonitorfaildec;
  uint8_t Ccmmonitorburstfilter;
  uint8_t Ccmmonitorburstscale;
  uint8_t spare2[2];

  //CPPC Constraints
  uint8_t CppcConstraintsEnabled;
  uint8_t spare3[3];
  uint8_t PerfLimit_MaxRange;
  uint8_t PerfLimit_MinRange;
  uint8_t EPP_MaxRange;
  uint8_t EPP_MinRange;

  //Misc
  uint32_t System;
  uint32_t MinS0i3SleepTimeInMs;
  uint32_t SystemRtcTime;

  //GFX OC limits
  uint8_t  OC_GFX_DISABLE;  //1=disable overclocking; 0=allow overclocking
  uint8_t  OC_GFX_MINVID;   //[VID] SVI2 VID encoding=
  uint16_t OC_GFX_FREQMAX;  //MHz

  //GFXCLK/GFXOFF related parameters
  uint8_t  GceaLinkMgrIdleThreshold;        //Set by SMU FW during enablment of SOC_ULV. Controls delay for GFX SDP port disconnection during idle events
  uint8_t  GfxClkSource;                    // 0 = PLL, 1 = DFLL
  uint8_t  DisableMsmuSecurityRestore;      //Set to 1 to use PSP instead of MSMU for security restore. MSMU is the POR path.
  uint8_t  ApccEnable;                      //1 = Enable APCC, should be mutually exclusive to GFX EDC controller

  uint16_t GfxVinit;                        // Voltage required to support Finit during GFX power up/GFXOFF exit (in mV)
  uint16_t GfxclkFinit;                     // in Mhz. Options are 800, 600, 400, 300, 200, 100

  uint16_t GfxclkFgfxoffEntry;              // in Mhz
  uint16_t GfxclkFidle;                     // in MHz
  uint16_t GfxclkSlewRate;                  // for PLL babystepping???
  uint16_t GfxclkFminOverride;              // Specify the GFXCLK Fmin override [MHz]

  uint16_t GfxMinVoltage;                   //Minimum GFX voltage, in mV
  uint16_t GfxMaxVoltage;                   //Maximum GFX voltage, in mV

  //GFX temperature dependent Vmin feature
  uint8_t  VDDGFX_TVminOverrideEn;
  uint8_t  spare4[3];
  uint16_t VDDGFX_TVmin;       //Celcius
  uint16_t VDDGFX_Vmin_HiTemp; // mV Q2
  uint16_t VDDGFX_Vmin_LoTemp; // mV Q2
  uint16_t VDDGFX_TVminHystersis; // Celcius

  // Enable automated phase shedding and diode emulation (PSI7)
  uint8_t  VddcrVddPsi7En;
  uint8_t  VddcrSocPsi7En;
  uint8_t  VddcrSrPsi7En;
  uint8_t  PsiDecayConditionVddRail;
  uint8_t  PsiDecayConditionSocRail;
  uint8_t  PsiDecayConditionSrRail;
  uint8_t  PsiDecayConditionOverride; // Override bit to PSI Decay Condition
  uint8_t  spare5;

  uint8_t  VddmCldoFastStartupEn;
  uint8_t  VddmCldoOvDetEn;
  uint16_t PllSscSpreadPercentage; // PLL Spread down percentage, Q8 format. e.g. 192 gives 0.75%
  uint8_t  FclkDpmOptimizationEn;  //[0: disable; 1: enable]
  uint8_t  DisplayIdleOptimizationEn; //[0: disable; 1: enable]
  uint8_t  ApplyDefaultNrepFlopSel;            //[0: enable; 1: disable], ApplyDefaultNrepFlopSel is controlled by PcdPll5SscDisable
  uint8_t  GfxDldoBypass;

  uint16_t LclkMaxFreq;            // in MHz
  uint16_t LclkMinFreq;            // in MHz

  uint16_t VDD_MISC_Voltage;       // Voltage in mV (used in AOD menu)
  uint16_t spare6;

  // Slew rate control for VDDCR_VDD rail
  uint8_t  SlewRateOverrideVdd; // Set this field to indicate the Slew Rate must change
  uint8_t  SlewRateUpIndexVdd;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  uint8_t  SlewRateDownCntlVdd; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  spare7;

  int16_t  PerCoreDldoPsmMargin[8]; //PSM count for each core [signed]

  // Slew rate control for VDDCR_SOC and VDDCR_SR rails
  uint8_t  SlewRateOverrideSoc; // Set this field to indicate the Slew Rate must change
  uint8_t  SlewRateUpIndexSoc;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  uint8_t  SlewRateDownCntlSoc; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  spare8;

  uint8_t  ForceFCLKLevel;
  uint8_t  ForceLCLKLevel;
  uint8_t  spare9[2];

  //SPARE FOR FUTURE USE
  uint32_t spare10[32];
} Smu13BiosIfTable_t;

typedef struct {
  //PCIe LANE MAPPING [NBIO_QUADRANT][PCIe_CORE][DEVICE]
  uint8_t PCIe_GenSpeed[4][2][8];    //0=NOT_CONNECTED; 1=GEN1; 2=GEN2; 3=GEN3; 4=GEN4; 5=GEN4+(ESM)
  uint8_t PCIe_LaneWidth[4][2][8];   //LANE_WIDTH [0=NOT_CONNECTED; 1=x1; 2=x2; 4=x4; 8=x8; 16=x16]
} PcieLaneMappingTable_t;

typedef union _ZSC_IP_IDLE_MASK_STRUCT {
  struct {
    UINT32                            VCN_IDLE_MASK:1;     // bit0
    UINT32                            ATHUB_IDLE_MASK:1;   // bit1
    UINT32                            MMHUB_IDLE_MASK:1;   // bit2
    UINT32                            NBDXIO_IDLE_MASK:1;  // bit3
    UINT32                            OSSSYS_IDLE_MASK:1;  // bit4
    UINT32                            HDP_IDLE_MASK:1;     // bit5
    UINT32                            DF_IDLE_MASK:1;      // bit6
    UINT32                            HSP_IDLE_MASK:1;     // bit7
    UINT32                            ACP_IDLE_MASK:1;     // bit8
    UINT32                            MP0_IDLE_MASK:1;     // bit9
    UINT32                            MP1_IDLE_MASK:1;     // bit10
    UINT32                            MP2_IDLE_MASK:1;     // bit11
    UINT32                            FCH_IDLE_MASK:1;     // bit12
    UINT32                            SSTFCH_IDLE_MASK:1;  // bit13
    UINT32                            rsvd_0:2;            // bit[15:14]
    UINT32                            DF_STG2_IDLE_MASK:1; // bit16
    UINT32                            rsvd_1:15;           // bit[31:17]
  } Field;
  UINT32 Value;
} ZSC_IP_IDLE_MASK_STRUCT;

typedef union _ZSC_WAKE_EVENT_MASK_0_STRUCT {
  struct {
    UINT32                            WAKE_EVENT_MASK_TIMER:1;       // bit0
    UINT32                            WAKE_EVENT_MASK_S2NS_TIMER:1;  // bit1
    UINT32                            rsvd_0:2;                      // bit[3:2]
    UINT32                            WAKE_EVENT_MASK_FCH:1;         // bit4
    UINT32                            WAKE_EVENT_MASK_ACP:1;         // bit5
    UINT32                            WAKE_EVENT_MASK_MP2:1;         // bit6
    UINT32                            WAKE_EVENT_MASK_DBREQ:1;       // bit7
    UINT32                            WAKE_EVENT_MASK_DCN_S:1;       // bit8
    UINT32                            WAKE_EVENT_MASK_DCN_NS:1;      // bit9
    UINT32                            rsvd_1:6;                      // bit[15:10]
    UINT32                            WAKE_EVENT_MASK_DF:1;          // bit16
    UINT32                            rsvd_2:15;                     // bit[31:17]
  } Field;
  UINT32 Value;
} ZSC_WAKE_EVENT_MASK_0_STRUCT;

typedef union _ZSC_WAKE_EVENT_MASK_1_STRUCT {
  struct {
    UINT32                            WAKE_EVENT_MASK_FCH_PWROK:1;    // bit0
    UINT32                            rsvd_0:3;                       // bit[3:1]
    UINT32                            WAKE_EVENT_MASK_FCH_RESETN:1;   // bit4
    UINT32                            rsvd_1:3;                       // bit[7:5]
    UINT32                            WAKE_EVENT_MASK_HSP_PWROKRAW:1; // bit8
    UINT32                            rsvd_2:3;                       // bit[11:9]
    UINT32                            WAKE_EVENT_MASK_HSP_PWROK:1;    // bit12
    UINT32                            rsvd_3:3;                       // bit[15:13]
    UINT32                            WAKE_EVENT_MASK_HSP_RESETN:1;   // bit16
    UINT32                            rsvd_4:15;                      // bit[31:17]
  } Field;
  UINT32 Value;
} ZSC_WAKE_EVENT_MASK_1_STRUCT;

typedef union _ZSC_IP_FENCE_ACK_STRUCT {
  struct {
    UINT32                            VCN_FENCE_ACK_MASK:1;     // bit0
    UINT32                            ATHUB_FENCE_ACK_MASK:1;   // bit1
    UINT32                            MMHUB_FENCE_ACK_MASK:1;   // bit2
    UINT32                            NBDXIO_FENCE_ACK_MASK:1;  // bit3
    UINT32                            OSSSYS_FENCE_ACK_MASK:1;  // bit4
    UINT32                            HDP_FENCE_ACK_MASK:1;     // bit5
    UINT32                            DF_FENCE_ACK_MASK:1;      // bit6
    UINT32                            HSP_FENCE_ACK_MASK:1;     // bit7
    UINT32                            ACP_FENCE_ACK_MASK:1;     // bit8
    UINT32                            MP0_FENCE_ACK_MASK:1;     // bit9
    UINT32                            MP1_FENCE_ACK_MASK:1;     // bit10
    UINT32                            MP2_FENCE_ACK_MASK:1;     // bit11
    UINT32                            FCH_FENCE_ACK_MASK:1;     // bit12
    UINT32                            SSTFCH_FENCE_ACK_MASK:1;  // bit13
    UINT32                            DCN_FENCE_ACK_MASK:1;     // bit14
    UINT32                            rsvd_0:1;                 // bit15
    UINT32                            DF_STG2_FENCE_ACK_MASK:1; // bit16
    UINT32                            rsvd_1:15;                // bit[31:17]
  } Field;
  UINT32 Value;
} ZSC_IP_FENCE_ACK_STRUCT;

typedef enum {
  GPIO_ACTION_DO_NOT_TOUCH,
  GPIO_ACTION_DRIVE_OUTPUT_HIGH,
  GPIO_ACTION_DRIVE_OUTPUT_LOW,
  GPIO_ACTION_ENABLE_WakeCntrlz,
  GPIO_ACTION_CLEAR_WakeCntrlz_WakeSts,
  GPIO_ACTION_ENABLE_WakeS0i3,  //5
  GPIO_ACTION_CLEAR_WakeS0i3,   //6
  GPIO_ACTION_COUNT,
} GPIO_ACTION_e;


//Table is simple: There is room for 10 GPIOs for both Z9 and S0i3. For each GPIO 2 Dword is needed

// Entry/Exit Action  Description             Detail
//      0              Don't touch
//      1              Output high            outputvalue = 1
//      2              Output Low             outputvalue = 0
//      3              Enable WakCntrlz       wakecntrlz = 1
//      4       Clear WakeCntrlz and WakeSts  wakecntrlz = 0, interrupts = 1, and wakests = 1

// The following table is used by BIOS to pass platform GPIO configuration
typedef struct {
  uint32_t SmnAddress;  //GPIO SMN Address
  uint8_t  Feature;     //0: S0i3, 1: Z9
  uint8_t  EntryAction; //
  uint8_t  ExitAction;  //
  uint8_t  spare;
} GpioConfig_t;

typedef struct {
  GpioConfig_t GpioConfig[10];
} GpioTable_t;


#endif
