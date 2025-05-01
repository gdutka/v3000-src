/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_PBS_CONFIG_H_
#define _AMD_PBS_CONFIG_H_


// {A339D746-F678-49B3-9FC7-54CE0F9DF226}
#define AMD_PBS_SETUP_GUID \
  { 0xa339d746, 0xf678, 0x49b3, 0x9f, 0xc7, 0x54, 0xce, 0xf, 0x9d, 0xf2, 0x26 }

extern EFI_GUID gAmdPbsSystemConfigurationGuid;

#define AMD_PBS_SETUP_VARIABLE_NAME  L"AMD_PBS_SETUP"

// {B863B959-0EC6-4033-99C1-8FD89F040222}
#define AMD_PBS_SETUP_FORMSET_GUID \
  { 0xb863b959, 0xec6, 0x4033, 0x99, 0xc1, 0x8f, 0xd8, 0x9f, 0x4, 0x2, 0x22 }


#define AMD_PBS_SETUP_FORM_ID               11
#define AMD_PBS_FWVER_FORM_ID               12
#define AMD_PBS_PMF_FORM_ID                 13
#define AMD_PBS_APMF_FN_3_FORM_ID           14
#define AMD_PBS_APMF_FN_5_FORM_ID           16
#define AMD_PBS_APMF_FN_6_FORM_ID           17
#define AMD_PBS_APMF_FN_9_FORM_ID           18
#define AMD_PBS_APMF_FN_11_FORM_ID          19
#define AMD_PBS_APMF_FN_12_FORM_ID          20
#define AMD_PBS_PCIE_CONFIG_FORM_ID         21
#define AMD_PBS_POWER_SAVING_CONFIG_FORM_ID 22
#define AMD_PBS_GRAPHICS_CONFIG_FORM_ID     23
#define AMD_PBS_DISPLAY_CONFIG_FORM_ID      24
#define AMD_PBS_AUDIO_CONFIG_FORM_ID        25
#define AMD_PBS_I2C_CONFIG_FORM_ID          26
#define AMD_PBS_THERMAL_CONFIG_FORM_ID      27
#define AMD_PBS_MP2_CONFIG_FORM_ID          28
#define AMD_PBS_EC_PD_CONFIG_FORM_ID        29
#define AMD_PBS_DEBUG_CONFIG_FORM_ID        30
#define AMD_PBS_USBTBT_CONFIG_FORM_ID       31
#define AMD_PBS_APMF_FN_13_FORM_ID          32
#define AMD_PBS_APMF_FN_1_FORM_ID           33
#define AMD_PBS_APMF_FN_4_FORM_ID           34
#define AMD_PBS_APMF_FN_14_FORM_ID          35
#define AMD_PBS_RAS_FORM_ID                 36

#define AMD_PBS_SETUP_CLASS                1
#define AMD_PBS_SETUP_SUB_CLASS            0
#define KEY_DT_SLOT_SETTING             2013
#define KEY_SPECIAL_VGA_FEATURE_SETTING 2014
#define KEY_D3_COLD_SUPPORT_SETTING     2015
#define KEY_DGPU_MACO_SETTING           2020
#define KEY_SATA_SSD_CONFIG0_SETTING    2023
#define KEY_SATA_SSD_CONFIG1_SETTING    2024
#define KEY_SATA_SSD_CONFIG2_SETTING    2025
#define KEY_ODD_POWER_CONFIG0_SETTING   2028
#define KEY_ODD_POWER_CONFIG1_SETTING   2029
#define KEY_ODD_POWER_CONFIG2_SETTING   2030
#define KEY_REDUCE_HARDWARE_SETTING     2035
#define KEY_MODERN_STANDBY_SETTING      2036
#define KEY_TOUCH_PANEL_SETTING         2037
#define KEY_TOUCH_PAD_SETTING           2038
#define KEY_NFC_SENSOR_SETTING          2039
#define KEY_AMD_LIGHT_SENSOR_SETTING    2040
#define KEY_WOV_SETTING                 2041
#define KEY_GPP89_SELECT_SETTING        2042
#define KEY_WWAN_SELECT_SETTING         2043
#define KEY_PCIEX1_SELECT_SETTING       2044
#define KEY_ODD_SELECT_SETTING          2045
#define KEY_HDD_SELECT_SETTING          2046
#define KEY_PWR_SENSORS_ROUTING_SELECT  2047
#define KEY_MP2_FW_SELECT               2048
#define KEY_DGPU_SSID_SETTING           2050
#define KEY_DP1_CAPS_ADJUST_SETTING     2051
#define KEY_DP2_CAPS_ADJUST_SETTING     2052
#define KEY_DP1_CAPS_NUM_SETTING        2053
#define KEY_DP2_CAPS_NUM_SETTING        2054
#define KEY_XGBE_PATH_SELECT            2055
#define KEY_XGBE_PORT0_TYPE_SWITCH      2056
#define KEY_XGBE_PORT1_TYPE_SWITCH      2057
#define KEY_XGBE_AIC_TYPE_SWITCH        2058
#define KEY_XGBE_PORT0_MODE_SEL         2059
#define KEY_XGBE_PORT1_MODE_SEL         2060
#define KEY_XGBE_AIC_MODE_SEL           2061
#define KEY_XGBE_AIC2_TYPE_SEL          2062
#define KEY_XGBE_AIC3_TYPE_SEL          2063

#define AMD_CPM_USED_PBS_SIZE ((UINT16) (OFFSET_OF (AMD_PBS_SETUP_OPTION,ReservedBufferLast)))

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

//
// Do not changed the DXIO_DDI_COUNT, Follow RMB PPR, the count is 5
//
#define DXIO_DDI_COUNT                     5

//
// PLAT-79844 [RMB][Lilic] PBS Variables align to 256 bytes.
// Caution: Please do not broken any priority order of PBS Setup item .
//

/// Structure for Variables to be used for CPM
/// Caution: If you want to reserve space, please naming "ReservedBufferX", X represent number.
typedef struct {
  //
  // Offset Start [0x00](PlatformId), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT16        PlatformId;                     ///< ((PcdGet16 (OriginalPlatformId) & BIT2)>>2)
  UINT16        CurrentId;                      ///< ((PcdGet16 (CurrentPlatformId) < 0x0008) ? 0 : 1)
  UINT8         SpecialVgaFeature;              ///< Special Feature
                                                ///<  0: Disabled
                                                ///<  4: HybridGraphics
  UINT8         DgpuAudio;                      ///< Discrete GPU's Audio
                                                ///<  0: Disabled
                                                ///<  1: Keep HW Default Setting
  UINT8         PrimaryVideoAdaptor;            ///< Primary Video Adaptor
                                                ///<  1: Int Graphics (IGD)
                                                ///<  2: Int Graphics (PEG)
  UINT8         TouchPanel;                     ///< Touch Panel Support
                                                ///< ECRAMxA9, bit 6. Pwr: ECRAMxA9, bit7
                                                ///<  0: Disabled
                                                ///<  1: Under I2C 0 Bus
                                                ///<  2: Under I2C 1 Bus
                                                ///<  3: Under I2C 2 Bus
                                                ///<  4: Under I2C 3 Bus
                                                ///<  5: ELAN TSP Under I2C 0 Bus
                                                ///<  6: ELAN TSP Under I2C 1 Bus
                                                ///<  7: ELAN TSP Under I2C 2 Bus
                                                ///<  8: ELAN TSP Under I2C 3 Bus
  UINT8         TouchPad;                       ///< Touch Pad Support
                                                ///<  0: Disabled                       ECRAMxA9, bit 0
                                                ///<  1: Under I2C 0 Bus
                                                ///<  2: Under I2C 1 Bus
                                                ///<  3: Under I2C 2 Bus
                                                ///<  4: Under I2C 3 Bus
  UINT8         NfcSensor;                      ///< Nfc Support
                                                ///<  0: Disabled
                                                ///<  1: Under I2C 0 Bus
                                                ///<  2: Under I2C 1 Bus
                                                ///<  3: Under I2C 2 Bus
                                                ///<  4: Under I2C 3 Bus
  UINT8         amdLightSensor;                 ///< ALS Support
                                                ///<  0: Disabled
                                                ///<  1: Under I2C 0 Bus
                                                ///<  2: Under I2C 1 Bus
                                                ///<  3: Under I2C 2 Bus
                                                ///<  4: Under I2C 3 Bus
  UINT8         SerialDbgMsgUnderOs;            ///< Serial Debug Message Under OS
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DbgPrintInAsl;                  ///< Debug Print In ASL
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DtSlotPowerControl;             ///< DT Slot Power Enable             ECRAMxA7, bit 2
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  //
  // Offset Start [0x10](WlanPowerControl), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         WlanPowerControl;               ///< WLAN/WIFI Power Enable           ECRAMxAA, bit 4
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         LanPowerControl;                ///< LAN Power Enable                 ECRAMxAA, bit 7
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         WwanPowerEn;                    ///< WWAN Power Enable                ECRAMxA7, bit 2
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         EvalSlotPowerEnable;            ///< Eval, Gfx                        ECRAMxA1, bit 1
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         Pciex1PowerEn;                  //                                    ECRAMxA1, bit 0
                                                //
  UINT8         FrontCamera0PowerEn;            ///< Camera 0 Enable
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         FrontCamera1PowerEn;            ///< Camera 1 Enable
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         UcsiEn;                         ///< UCSI Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         UcsiLocation;                   ///< UCSI tunnel location
                                                ///<  0: ECRAM
                                                ///<  1: MMIO
  UINT8         ChgByPassEn;                    ///< Charger BYPASS mode
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         MITTWITTControl;                ///< MITT/WITT Selection
                                                ///<  0: MITT Only
                                                ///<  1: WITT Only
                                                ///<  2: Both disable
  UINT8         UnusedGppClkOff;                ///< Unused GPP Clocks Off
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest0;                  ///< Clock PM: CLK_REQ0
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest1;                  ///< Clock PM: CLK_REQ1
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest2;                  ///< Clock PM: CLK_REQ2
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest3;                  ///< Clock PM: CLK_REQ3
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest4;                  ///< Clock PM: CLK_REQ4
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ClockRequest5;                  ///< Clock PM: CLK_REQ5
                                                ///<  0: Disabled
                                                ///<  1: Enabled

  //
  // Offset Start [0x20](ClockRequest6), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         ClockRequest6;                  ///< Clock PM: CLK_REQ6
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         KbcSupport;                     ///< KBC Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         amdAcDcSwitch;                  ///< AcDcSwitch
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         amdDcTimer;                     ///< AmdDcTimer
                                                ///<  minimum: 0
                                                ///<  maximum: 255
  UINT8         amdAcTimer;                     ///< AmdAcTimer
                                                ///<  minimum: 0
                                                ///<  maximum: 255
  UINT8         MemVddq;                        ///< MEM VDDQ voltage
  UINT8         MemVddio;                       ///< MEM VDDIO voltage
  UINT8         Alw18v;                         ///< 1.8V ALW voltage
  UINT8         VddpRun;                        ///< VDDP RUN voltage
  UINT8         VddIo11;                        ///< VDD11 voltage
                                                ///<  0: voltage (0.1.1V)
                                                ///<  1: voltage (+10mv)
                                                ///<  2: voltage (+20mv)
                                                ///<  3: voltage (+30mv)
                                                ///<  4: voltage (+40mv)
                                                ///<  5: voltage (+50mv)
                                                ///<  6: voltage (+60mv)
                                                ///<  7: voltage (+70mv)
                                                ///<  8: voltage (+80mv)
                                                ///<  9: voltage (+90mv)
                                                ///<  10: voltage (+100mv)
                                                ///<  11: voltage (-10mv)
                                                ///<  12: voltage (-20mv)
                                                ///<  13: voltage (-30mv)
                                                ///<  14: voltage (-40mv)
                                                ///<  15: voltage (-50mv)
                                                ///<  16: voltage (-60mv)
                                                ///<  17: voltage (-70mv)
                                                ///<  18: voltage (-80mv)
                                                ///<  19: voltage (-90mv)
                                                ///<  20: voltage (-100mv)
  UINT8         VddMiscS5;                      /// VDD MISC S5
  UINT8         AmdDptcControl;                 ///< AMD DPTC interface
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         SttSensorsRpt;                  ///< STT sensor reporting
                                                ///<  0: Disabled
                                                ///<  1: Report onboard sensors
                                                ///<  2: Report onboard and eval card sensors
  UINT8         SLTConfigCheck;                 ///< SLT Configuration Check
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DgpuHotPlugMode;                ///< Discrete GPU Hotplug Mode
                                                ///<  0: Basic Mode
                                                ///<  1: Enhanced Mode
  UINT8         DgpuHpdCircuitry;               ///< Discrete GPU HPD Circuitry
                                                ///<  0: OR Circuitry
                                                ///<  1: Pulse Circuitry
  //
  // Offset Start [0x30](DgpuMaco), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         DgpuMaco;                       ///< Discrete GPU BOMACO Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         TimeMacoUpPwrDn;                ///< MACO-PWR Timing   (us)
                                                ///<  minimum: 10
                                                ///<  maximum: 255
  UINT8         TimePwrDnRstDn;                 ///< PWROFF-RST Timing (ms)
                                                ///<  minimum: 1
                                                ///<  maximum: 255
  UINT8         TimePwrUpRstUp;                 ///< PWRON-RST Timing  (ms)
                                                ///<  minimum: 1
                                                ///<  maximum: 255
  UINT8         TimeRstUpMacoDn;                ///< RST-MACO Timing   (ms)
                                                ///<  minimum: 1
                                                ///<  maximum: 255
  UINT8         AmdPspKvmMouseProtocol;         ///< AMD KVM Mouse Protocol
                                                ///<  0: Absolute
                                                ///<  1: Simple
                                                ///<  3: Auto
  UINT8         AmdBlinkLed;                    ///< BLINK LED
                                                ///<  0: Disabled
                                                ///<  1: Enabled
                                                ///<  2: GPIO 11 Output Low
                                                ///<  3: GPIO 11 Output High
  UINT16        AmdCpmUsedPbsSize;              ///< AMD_CPM_USED_PBS_SIZE
  UINT8         ModernStandbyEn;                ///< Modern Standby Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         StallForUnlock;                 ///< StallForUnlock Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         HdtBreakPointBoot;              ///< HdtBreakPointBoot
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         HdtBreakPointS3;                ///< HdtBreakPointS3
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ReduceHardwareEn;               ///< ReduceHardwareSupport
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         Above4GBMmioLimit;              ///< Above 4GB MMIO Limit
                                                ///<  35: 35bit (32GB)
                                                ///<  36: 36bit (64GB)
                                                ///<  37: 37bit (128GB)
                                                ///<  38: 38bit (256GB)
                                                ///<  39: 39bit (512GB)
                                                ///<  40: 40bit (1TB)
                                                ///<  41: 41bit (2TB)
                                                ///<  42: 42bit (4TB)
                                                ///<  43: 43bit (8TB)
                                                ///<  44: 44bit (16TB)
                                                ///<  45: 45bit (32TB)
                                                ///<  46: 46bit (64TB)
                                                ///<  47: 47bit (128TB)
                                                ///<  48: 48bit (256TB)
  UINT8         D3ColdSupport;                  ///< D3Cold Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
                                                ///<  2: Dummy D3Cold
  //
  // Offset Start [0x40](SensorFusionEn), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         SensorFusionEn;                 ///< Sensor Fusion User Mode Driver
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         NonEvalDgpuSupport;             ///< Non-Eval Discrete GPU Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ModernStandbyType;              ///< Modern Standby type select
                                                ///<  0: s0i2
                                                ///<  1: s0i3
                                                ///<  2: s0i2+s0i3
  UINT8         WakeOnPME;                      ///< WakeOnPME
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ReservedBuffer0 [0x0A];
  UINT8         PdTBT3AltMode;                  ///<  PD TBT3 ATL MODE
                                                ///<  0: Enable
                                                ///<  1: Disable
  UINT8         iLaTraceMemoryEn;               ///< iLA TraceMemoryEn
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  //
  // Offset Start [0x50](CpuType), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         CpuType;                        ///< CpuType
                                                ///<  0: PCO (Bx)
                                                ///<  1: PCO DS or RV2
  UINT8         S5_5V;                          ///< 5V_S5 voltage
                                                ///<  0x6c: 5.06v(Default)
                                                ///<  0x6f: 5.12v
                                                ///<  0x72: 5.16v
  UINT8         WlanRecovery;                   ///< Wireless LAN Recovery
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         UsbCameraPowerEn;               ///< Camera 1 Enable
                                                ///< ECRAMxAA, bit 2, Lilac Did not have this.
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         PowerSensorsRoutingSelect;      ///< Power Sensors Routing Select
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         Mp2FwSelection;                 ///< MP2 FW selection
                                                ///<  0: MP2_SFH
                                                ///<  1: MP2_WalleLite
  UINT8         EvalCardTdiodeRoutingSelect;    ///< EVAL CARD T-Diode Routing Select
                                                ///<  0: EC
                                                ///<  1: APU SMBUS0
  UINT8         WoV;                            ///< Wake On Voice
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         AcpPowerGating;                 ///< ACP Power Gating
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         GPP89_Select;                   ///< GPP8/9 Select
                                                ///<  0: SATA
                                                ///<  1: PCIE
  //
  // Offset Start [0x58], Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         NvmeRaid;                       ///< NVMe RAID mode
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DgpuD3ColdHpdSupport;           ///< Discrete GPU D3Cold HPD Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         PmeTurnOffSupport;              ///< PME Turn Off Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DP[DXIO_DDI_COUNT];             ///< DP Switch
                                                ///<  0: Default by Board ID
                                                ///<  1: EDP
                                                ///<  2: DP
                                                ///<  3: HDMI
  //
  // Offset Start [0x62](AcpClockGating), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         AcpClockGating;                 ///< AcpClockGating
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         UsbCrsEnable;                   ///< USB MS Resource
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         NVDgpuPowerEnable;              ///< NVIDIA DGPU Power Enable
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DgpuUsbPort;                    ///< Discrete GPU's USB Port
                                                ///<  0: Keep Default Setting
                                                ///<  1: Disabled
  UINT8         ReservedBuffer1 [0x02];
  UINT8         D3ColdGen1Support;              ///< D3Cold Force Gen1
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DgpuDsmFunASupport;             ///< Discrete GPU _DSM Function A
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         DgpuDsmFunBSupport;             ///< Discrete GPU _DSM Function B
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         ReservedBuffer2 [0x01];
  UINT8         DgpuSsidSvid;                   ///< Discrete GPU's SSID/SVID
                                                ///<  0: Keep Default Setting
                                                ///<  1: Program by Vendor
  UINT8         AcMaximumPerformanceLimit;      ///< AC Maximum Performance Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  UINT8         AcBetterPerformanceLimit;       ///< AC Better Performance Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  UINT8         AcBetterBatteryLimit;           ///< AC Better Battery Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100

  //
  // Offset Start [0x70](DcMaximumPerformanceLimit), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         DcMaximumPerformanceLimit;      ///< DC Maximum Performance Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  UINT8         DcBetterPerformanceLimit;       ///< DC Better Performance Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  UINT8         DcBetterBatteryLimit;           ///< DC Better Battery Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  UINT8         DcBatterySaverLimit;            ///< DC Battery Saver Limit
                                                ///<  minimum: 0
                                                ///<  maximum: 100
  //
  //  For Lilac/Mayan Porting Add Mux selection.
  //

  // Main feature (CLK switch)
  UINT8         WwanX1Mux;                      // 0: Wwan,   1: X1 slot  ECRAMxAC, Bit4
  UINT8         DtM2ssd1Mux;                    // 0: DT,     1: M2SSD1.  ECRAMxAC, Bit1

  // Mux feature selection
  UINT8         OddSsd1Mux;                     // 0: Odd,    1: Ssd1Mux  ECRAMxAC, Bit5
  UINT8         ZeroPowerOddEn;
  UINT8         Ssd1HddMux;                     // 0: Ssd1,   1: Hdd      ECRAMxAC, Bit6

  UINT8         WwanLomMux;                     // 0: Wwan,   1:Lom       ECRAMxAC, Bit7

  // Pwr Enable PIN
  UINT8         Ssd0PwrEn;                      //  ECRAMxAA, bit 6
  UINT8         Ssd1PwrEn;                      //  ECRAMxA9, bit 5
  UINT8         OddPwrEn;                       //  ECRAMxAA, bit 0
  UINT8         HDDPwrEn;                       //  ECRAMxAA, bit 1
  UINT8         UsbFprPwrEn;                    //  ECRAMxA9, bit 3
  UINT8         SdCardPwrEn;                    //  ECRAMxAA, bit 5

  //
  // Offset Start [0x80](PBSPlatformSel), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // PBS Selection
  UINT8         PBSPlatformSel;                 // HighByte[7:4]  used for SLT
                                                  // 2 - D595 Lilac SLT
                                                  // 3 - D597 Mayan SLT
                                                // LowByte [3:0]  used for CRB
                                                  // 0 - D595 Lilac
                                                  // 1 - D597 Mayan
                                                  // 2 - D596 Lilac DAP
                                                  // 3 - D598 Mayan DAP
                                                  // 4 - D599 Moonglow
                                                  // 5 - D884 Lilac_KD
                                                  // 6 - D883 Mayan_KD
  UINT8         SecurityLvl3SetupDxe;

  UINT8         SLTCheck;                       ///< Enable SLTCheck or not
                                                ///<  0: Not check
                                                ///<  1: Need check
  UINT8         ProcessorAggregatorDevice;      ///< Processor Aggregator Device
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         CoreCountControl;               ///< Core Count Control
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         AtifFunction23Support;          ///< ATIF Function 23 Support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         VariBrightMaxPerformance;       ///< Vari-Bright Maximum Performance
                                                ///<  minimum: 0
                                                ///<  maximum: 4
  UINT8         VariBrightBetterPerformance;    ///< Vari-Bright Better Performance
                                                ///<  minimum: 0
                                                ///<  maximum: 4
  UINT8         VariBrightBetterBattery;        ///< Vari-Bright Better Battery
                                                ///<  minimum: 0
                                                ///<  maximum: 4
  UINT8         VariBrightBatterySaver;         ///< Vari-Bright Battery Saver
                                                ///<  minimum: 0
                                                ///<  maximum: 4

  UINT8         VddIoAdjustEn;                  ///< Use for Vddio/Vdd/ .. tunel for board design.
  UINT8         KeepWlanPowerInS3S4;            ///<
  //
  // Offset Start [0x80], Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT16        Tpvperl;                        ///< Minimum time from powerrails within
                                                ///< specified to leranceto PERST# inactive.
                                                ///<  minimum: 50
                                                ///<  maximum: 1000
  UINT16        Trstcfg;                        ///< Minimum period following the end of a Conventional Reset
                                                ///< of device before it is permitted to issue
                                                ///< Configuration Requests to the device.
                                                ///<  minimum: 0
                                                ///<  maximum: 1000
  //
  // Offset Start [0x90](TimeSlotPwrToPwrEn), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         TimeSlotPwrToPwrEn;             ///< SLOTPWR-PWREN Timing(ms)
                                                ///<  minimum: 0
                                                ///<  maximum: 30
  UINT8         TimePwrEnToPeRst;               ///< PWREN-RST Timing (ms)
                                                ///<  minimum: 0
                                                ///<  maximum: 32
  UINT8         TimePeRstToWakeL23;             ///< PERST-WAKEL23 Timing (ms)
                                                ///<  minimum: 1
                                                ///<  maximum: 30
  UINT8         TimeDlActToCfgAcc;              ///< DLACT-CFGACC Timing (ms)
                                                ///<  minimum: 1
                                                ///<  maximum: 100
  UINT8         BtPldr;                         ///< Bluetooth PLDR
                                                ///<   0: Disable
                                                ///<   1: Enable
                                                ///<   2: Dummy Reset
  UINT8         CoreCountSwSciGpeId;            ///< Core Count SW_SCI_GPE_ID
                                                ///<  minimum: 0
                                                ///<  maximum: 31
  UINT8         WirelessBotton;                 ///< Wireless Botton Support
                                                ///<  0: Diabled
                                                ///<  3: HID Based Wireless Botton
  UINT32        DP1DisplayCaps;                 ///< Use for DP1 DDI settings
  UINT32        DP2DisplayCaps;                 ///< Use for DP2 DDI settings
  UINT8         NvmeD3cold;                     ///< Nvme D3cold Support
                                                ///< 0: Disabled
                                                ///< 1: Enabled
  //
  // Offset Start [0xA0](TurnOffXtalS3S5), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         TurnOffXtalS3S5;                ///< Use for turn off Xtal on S3/S5
  UINT8         PdUsb4PlatfromControl;          ///< PD USB4 Control
                                                ///< 0:Disable
                                                ///< 1:Enable

  //
  // Offset Start [0xA2](AmdPmfSupport), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         AmdPmfSupport;                  ///< AMD PMF driver support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  // APMF Function 3 settings, 36 bytes
  UINT8         AmdPmfFn3_Enable;                                ///< APMF Function 3 support
  UINT8         ApmfFn3_AC_Best_Performance_Limit;
  UINT32        ApmfFn3_Fan_ID_AC_Best_Performance_Limit;
  UINT8         ApmfFn3_AC_Better_Performance_Limit;
  UINT32        ApmfFn3_Fan_ID_AC_Better_Performance_Limit;
  UINT8         ApmfFn3_AC_Better_Battery_Limit;
  UINT32        ApmfFn3_Fan_ID_AC_Better_Battery_Limit;
  UINT8         ApmfFn3_DC_Best_Performance_Limit;
  UINT32        ApmfFn3_Fan_ID_DC_Best_Performance_Limit;
  UINT8         ApmfFn3_DC_Better_Performance_Limit;
  UINT32        ApmfFn3_Fan_ID_DC_Better_Performance_Limit;
  UINT8         ApmfFn3_DC_Better_Battery_Limit;
  UINT32        ApmfFn3_Fan_ID_DC_Better_Battery_Limit;
  UINT8         ApmfFn3_DC_Battery_Saver_Limit;
  UINT32        ApmfFn3_Fan_ID_DC_Battery_Saver_Limit;
  // APMF Function 4 settings, 49 bytes
  UINT8         AmdPmfFn4Reserved[49];                           ///< APMF Function 4 support

  //
  // Offset Start [0xf8](AmdPmfFn5_Enable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // APMF Function 5 settings, 145 bytes
  UINT8         AmdPmfFn5_Enable;                                //< APMF Function 5 support
  UINT32        ApmfFn5_T_Balanced_to_Perf;
  UINT32        ApmfFn5_T_Perf_to_Balanced;
  UINT32        ApmfFn5_T_Quiet_to_Balanced;
  UINT32        ApmfFn5_T_Balanced_to_Quiet;
  UINT32        ApmfFn5_Pfloor_Perf;
  UINT32        ApmfFn5_Pfloor_Balanced;
  UINT32        ApmfFn5_Pfloor_Quiet;
  UINT32        ApmfFn5_P_delta_Balanced_to_Perf;
  UINT32        ApmfFn5_P_delta_Perf_to_Balanced;
  UINT32        ApmfFn5_P_delta_Quiet_to_Balanced;
  UINT32        ApmfFn5_P_delta_Balanced_to_Quiet;
  UINT8         ApmfFn5_STT_SkinTempLimit_APU_Perf_on_Lap;
  UINT8         ApmfFn5_STT_SkinTempLimit_HS2_Perf_on_Lap;
  UINT8         ApmfFn5_STT_SkinTempLimit_APU_Perf;
  UINT8         ApmfFn5_STT_SkinTempLimit_HS2_Perf;
  UINT8         ApmfFn5_STT_SkinTempLimit_APU_Balanced;
  UINT8         ApmfFn5_STT_SkinTempLimit_HS2_Balanced;
  UINT8         ApmfFn5_STT_SkinTempLimit_APU_Quiet;
  UINT8         ApmfFn5_STT_SkinTempLimit_HS2_Quiet;
  UINT32        ApmfFn5_STTMinLimit_Perf_on_lap;
  UINT32        ApmfFn5_STTMinLimit_Perf;
  UINT32        ApmfFn5_STTMinLimit_Balanced;
  UINT32        ApmfFn5_STTMinLimit_Quiet;
  UINT32        ApmfFn5_FPPT_Perf_on_lap;
  UINT32        ApmfFn5_SPPT_Perf_on_lap;
  UINT32        ApmfFn5_SPL_Perf_on_lap;
  UINT32        ApmfFn5_SPPT_APUOnly_Perf_on_Lap;
  UINT32        ApmfFn5_FPPT_Perf;
  UINT32        ApmfFn5_SPPT_Perf;
  UINT32        ApmfFn5_SPL_Perf;
  UINT32        ApmfFn5_SPPT_APUOnly_Perf;
  UINT32        ApmfFn5_FPPT_Balanced;
  UINT32        ApmfFn5_SPPT_Balanced;
  UINT32        ApmfFn5_SPL_Balanced;
  UINT32        ApmfFn5_SPPT_APUOnly_Balanced;
  UINT32        ApmfFn5_FPPT_Quiet;
  UINT32        ApmfFn5_SPPT_Quiet;
  UINT32        ApmfFn5_SPL_Quiet;
  UINT32        ApmfFn5_SPPT_APUOnly_Quiet;
  UINT32        ApmfFn5_Fan_ID_Perf;
  UINT32        ApmfFn5_Fan_ID_Balanced;
  UINT32        ApmfFn5_Fan_ID_Quiet;

  //
  // Offset Start [0x189](AmdPmfFn6_Enable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // APMF Function 6 settings, 51 bytes
  UINT8         AmdPmfFn6_Enable;                                //< APMF Function 6 support
  UINT8         ApmfFn6_ConditionTemp_In_Bag_Policy;
  UINT8         ApmfFn6_ConditionTemp_In_Bag_S4_Policy;
  UINT32        ApmfFn6_FPPT_In_Bag_Policy;
  UINT32        ApmfFn6_SPPT_In_Bag_Policy;
  UINT32        ApmfFn6_SPPT_ApuOnly_In_Bag_Policy;
  UINT32        ApmfFn6_SPL_In_Bag_Policy;
  UINT32        ApmfFn6_STTMinLimit_In_Bag_Policy;
  UINT8         ApmfFn6_STT_SkinTempLimit_APU_In_Bag_Policy;
  UINT8         ApmfFn6_STT_SkinTempLimit_HS2_In_Bag_Policy;
  UINT32        ApmfFn6_Fan_ID_In_Bag_Policy;
  UINT32        ApmfFn6_FPPT_In_Bag_Exit_Policy;
  UINT32        ApmfFn6_SPPT_In_Bag_Exit_Policy;
  UINT32        ApmfFn6_SPPT_ApuOnly_In_Bag_Exit_Policy;
  UINT32        ApmfFn6_SPL_In_Bag_Exit_Policy;
  UINT32        ApmfFn6_STTMinLimit_In_Bag_Exit_Policy;
  UINT8         ApmfFn6_STT_SkinTempLimit_APU_In_Bag_Exit_Policy;
  UINT8         ApmfFn6_STT_SkinTempLimit_HS2_In_Bag_Exit_Policy;

  //
  // Offset Start [0x1bc](AmdPmfFn9_Enable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // APMF Function 9 settings, 183 bytes
  UINT8         AmdPmfFn9_Enable;                                //< APMF Function 9 support
  UINT32        ApmfFn9_fPPT_BestPerfAC;
  UINT32        ApmfFn9_sPPT_BestPerfAC;
  UINT32        ApmfFn9_sPPT_APUOnlyBestPerfAC;
  UINT32        ApmfFn9_SPL_BestPerfAC;
  UINT32        ApmfFn9_STTMinLimit_BestPerfAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BestPerfAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BestPerfAC;
  UINT32        ApmfFn9_Fan_ID_BestPerfAC;
  UINT32        ApmfFn9_fPPT_BetterPerfAC;
  UINT32        ApmfFn9_sPPT_BetterPerfAC;
  UINT32        ApmfFn9_sPPT_APUOnlyBetterPerfAC;
  UINT32        ApmfFn9_SPL_BetterPerfAC;
  UINT32        ApmfFn9_STTMinLimit_BetterPerfAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BetterPerfAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BetterPerfAC;
  UINT32        ApmfFn9_Fan_ID_BetterPerfAC;
  UINT32        ApmfFn9_fPPT_BetterBatteryAC;
  UINT32        ApmfFn9_sPPT_BetterBatteryAC;
  UINT32        ApmfFn9_sPPT_APUOnlyBetterBatteryAC;
  UINT32        ApmfFn9_SPL_BetterBatteryAC;
  UINT32        ApmfFn9_STTMinLimit_BetterBatteryAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BetterBatteryAC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BetterBatteryAC;
  UINT32        ApmfFn9_Fan_ID_BetterBatteryAC;
  UINT32        ApmfFn9_fPPT_BestPerfDC;
  UINT32        ApmfFn9_sPPT_BestPerfDC;
  UINT32        ApmfFn9_sPPT_APUOnlyBestPerfDC;
  UINT32        ApmfFn9_SPL_BestPerfDC;
  UINT32        ApmfFn9_STTMinLimit_BestPerfDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BestPerfDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BestPerfDC;
  UINT32        ApmfFn9_Fan_ID_BestPerfDC;
  UINT32        ApmfFn9_fPPT_BetterPerfDC;
  UINT32        ApmfFn9_sPPT_BetterPerfDC;
  UINT32        ApmfFn9_sPPT_APUOnlyBetterPerfDC;
  UINT32        ApmfFn9_SPL_BetterPerfDC;
  UINT32        ApmfFn9_STTMinLimit_BetterPerfDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BetterPerfDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BetterPerfDC;
  UINT32        ApmfFn9_Fan_ID_BetterPerfDC;
  UINT32        ApmfFn9_fPPT_BetterBatteryDC;
  UINT32        ApmfFn9_sPPT_BetterBatteryDC;
  UINT32        ApmfFn9_sPPT_APUOnlyBetterBatteryDC;
  UINT32        ApmfFn9_SPL_BetterBatteryDC;
  UINT32        ApmfFn9_STTMinLimit_BetterBatteryDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BetterBatteryDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BetterBatteryDC;
  UINT32        ApmfFn9_Fan_ID_BetterBatteryDC;
  UINT32        ApmfFn9_fPPT_BatterySaverDC;
  UINT32        ApmfFn9_sPPT_BatterySaverDC;
  UINT32        ApmfFn9_sPPT_APUOnlyBetterBatterySaverDC;
  UINT32        ApmfFn9_SPL_BatterySaverDC;
  UINT32        ApmfFn9_STTMinLimit_BatterySaverDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_APU_BatterySaverDC;
  UINT8         ApmfFn9_STT_SkinTempLimit_HS2_BatterySaverDC;
  UINT32        ApmfFn9_Fan_ID_BatterySaverDC;

  //
  // Offset Start [0x273](TouchPadSlaveAddr), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         TouchPadSlaveAddr;
  UINT8         TouchPadHidAddr;

  UINT8         ApuProcHotSetting;
  UINT8         IntPcieGpp0D3;
  UINT8         SocGpuD3;
  UINT8         SocHdAudioD3;
  UINT8         SocUsb31D3;
  UINT8         SocAcpD3;
  UINT8         SocAzaliaD3;
  UINT8         IntPcieGpp2D3;
  UINT8         SocUsb20D3;
  UINT8         SocUsb31ForUsb4D3;
  UINT8         SocUsb4D3;
  UINT8         IntUsb4PcieTunnelingD3;
  UINT8         SocUsb4PcieEndpointD3;

  //
  // Offset Start [0x277](AmdPmfFn11_Enable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // APMF Function 11 settings, 147 bytes
  UINT8         AmdPmfFn11_Enable;                                //< APMF Function 11 support
  UINT16        ApmfFn11_Flags;
  UINT32        ApmfFn11_T_Perf_to_Turbo;
  UINT32        ApmfFn11_T_Balanced_to_Perf;
  UINT32        ApmfFn11_T_Quiet_to_Balanced;
  UINT32        ApmfFn11_T_Balanced_to_Quiet;
  UINT32        ApmfFn11_T_Perf_to_Balanced;
  UINT32        ApmfFn11_T_Turbo_to_Perf;
  UINT32        ApmfFn11_Pfloor_Turbo;
  UINT32        ApmfFn11_FPPT_Turbo;
  UINT32        ApmfFn11_SPPT_Turbo;
  UINT32        ApmfFn11_SPPT_APUOnly_Turbo;
  UINT32        ApmfFn11_SPL_Turbo;
  UINT32        ApmfFn11_STT_MinLimit_Turbo;
  UINT8         ApmfFn11_STT_SkinTempAPU_Turbo;
  UINT8         ApmfFn11_STT_SkinTempHS2_Turbo;
  UINT32        ApmfFn11_Fan_ID_Turbo;
  UINT32        ApmfFn11_Pfloor_Performance;
  UINT32        ApmfFn11_FPPT_Performance;
  UINT32        ApmfFn11_SPPT_Performance;
  UINT32        ApmfFn11_SPPT_APUOnly_Performance;
  UINT32        ApmfFn11_SPL_Performance;
  UINT32        ApmfFn11_STT_MinLimit_Performance;
  UINT8         ApmfFn11_STT_SkinTempAPU_Performance;
  UINT8         ApmfFn11_STT_SkinTempHS2_Performance;
  UINT32        ApmfFn11_Fan_ID_Performance;
  UINT32        ApmfFn11_Pfloor_Balanced;
  UINT32        ApmfFn11_FPPT_Balanced;
  UINT32        ApmfFn11_SPPT_Balanced;
  UINT32        ApmfFn11_SPPT_APUOnly_Balanced;
  UINT32        ApmfFn11_SPL_Balanced;
  UINT32        ApmfFn11_STT_MinLimit_Balanced;
  UINT8         ApmfFn11_STT_SkinTempAPU_Balanced;
  UINT8         ApmfFn11_STT_SkinTempHS2_Balanced;
  UINT32        ApmfFn11_Fan_ID_Balanced;
  UINT32        ApmfFn11_Pfloor_Quiet;
  UINT32        ApmfFn11_FPPT_Quiet;
  UINT32        ApmfFn11_SPPT_Quiet;
  UINT32        ApmfFn11_SPPT_APUOnly_Quiet;
  UINT32        ApmfFn11_SPL_Quiet;
  UINT32        ApmfFn11_STT_MinLimit_Quiet;
  UINT8         ApmfFn11_STT_SkinTempAPU_Quiet;
  UINT8         ApmfFn11_STT_SkinTempHS2_Quiet;
  UINT32        ApmfFn11_Fan_ID_Quiet;

  //
  // Offset Start [0x30a](AmdPmfFn12_Enable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  // APMF Function 12 settings, 147 bytes
  UINT8         AmdPmfFn12_Enable;                                //< APMF Function 12 support
  UINT16        ApmfFn12_Flags;
  UINT32        ApmfFn12_T_Perf_to_Turbo;
  UINT32        ApmfFn12_T_Balanced_to_Perf;
  UINT32        ApmfFn12_T_Quiet_to_Balanced;
  UINT32        ApmfFn12_T_Balanced_to_Quiet;
  UINT32        ApmfFn12_T_Perf_to_Balanced;
  UINT32        ApmfFn12_T_Turbo_to_Perf;
  UINT32        ApmfFn12_Pfloor_Turbo;
  UINT32        ApmfFn12_FPPT_Turbo;
  UINT32        ApmfFn12_SPPT_Turbo;
  UINT32        ApmfFn12_SPPT_APUOnly_Turbo;
  UINT32        ApmfFn12_SPL_Turbo;
  UINT32        ApmfFn12_STT_MinLimit_Turbo;
  UINT8         ApmfFn12_STT_SkinTempAPU_Turbo;
  UINT8         ApmfFn12_STT_SkinTempHS2_Turbo;
  UINT32        ApmfFn12_Fan_ID_Turbo;
  UINT32        ApmfFn12_Pfloor_Performance;
  UINT32        ApmfFn12_FPPT_Performance;
  UINT32        ApmfFn12_SPPT_Performance;
  UINT32        ApmfFn12_SPPT_APUOnly_Performance;
  UINT32        ApmfFn12_SPL_Performance;
  UINT32        ApmfFn12_STT_MinLimit_Performance;
  UINT8         ApmfFn12_STT_SkinTempAPU_Performance;
  UINT8         ApmfFn12_STT_SkinTempHS2_Performance;
  UINT32        ApmfFn12_Fan_ID_Performance;
  UINT32        ApmfFn12_Pfloor_Balanced;
  UINT32        ApmfFn12_FPPT_Balanced;
  UINT32        ApmfFn12_SPPT_Balanced;
  UINT32        ApmfFn12_SPPT_APUOnly_Balanced;
  UINT32        ApmfFn12_SPL_Balanced;
  UINT32        ApmfFn12_STT_MinLimit_Balanced;
  UINT8         ApmfFn12_STT_SkinTempAPU_Balanced;
  UINT8         ApmfFn12_STT_SkinTempHS2_Balanced;
  UINT32        ApmfFn12_Fan_ID_Balanced;
  UINT32        ApmfFn12_Pfloor_Quiet;
  UINT32        ApmfFn12_FPPT_Quiet;
  UINT32        ApmfFn12_SPPT_Quiet;
  UINT32        ApmfFn12_SPPT_APUOnly_Quiet;
  UINT32        ApmfFn12_SPL_Quiet;
  UINT32        ApmfFn12_STT_MinLimit_Quiet;
  UINT8         ApmfFn12_STT_SkinTempAPU_Quiet;
  UINT8         ApmfFn12_STT_SkinTempHS2_Quiet;
  UINT32        ApmfFn12_Fan_ID_Quiet;

  //
  // Offset Start [0x39d](APICSwEnable), Caution: Please do not broken any priority order of PBS Setup item .
  //
  UINT8         APICSwEnable;
  UINT8         Allocate100KBForMP2;            ///< Enable option to allocate 100KB for MP2 customize request

  UINT32        AmdPcdCpmDgpuSsid;              ///< Discrete GPU's VGA SSID/SVID
  UINT32        AmdPcdCpmDgpuAudioSsid;         ///< Discrete GPU's AUDIO SSID/SVID

  UINT8         Usb4BusReserved;                ///< USB4 Bus reserved
  UINT16        Usb4NonPrefetchMemoryReserved;  ///< USB4 Non-Prefetch Memory Reserved
  UINT16        Usb4PrefetchMemoryReserved;     ///< USB4 Prefetch Memory Reserved
  UINT16        Usb4NonPrefetchMemoryAlign;     ///< USB4 Non-Prefetch Memory Reserved Align
  UINT16        Usb4PrefetchMemoryReservedAlign;///< USB4 Prefetch Memory Reserved Align

  UINT8         PbsDbgGnbDxioTimingControlEnable;                 ///< Pcie Dxio Timing ControlEnable
  UINT32        PbsDbgGnbPcieLinkReceiverDetectionPolling;        ///< PCIE Link Receiver Detection Polling
  UINT32        PbsDbgGnbPcieLinkL0Polling;                       ///< PCIE Link L0 Polling

  UINT8         PbsWWANDeviceSupport;                             ///< WWAN device support

  UINT8         UsbPortHwDisable;
  UINT8         ReconfigRebalanceResources;                       ///< Reconfigure / Rebalance the PCI resources of the PCIe RP under USB4 by OS
  UINT8         NonZ10CapPcieZstateSupport;      ///< Non Z10 Capable Pcie Z10 support
  // APMF Function 13 settings, 145 bytes
  UINT8         AmdPmfFn13_Enable;                                //< APMF Function 13 support
  UINT32        ApmfFn13_T_Balanced_to_Perf;
  UINT32        ApmfFn13_T_Perf_to_Balanced;
  UINT32        ApmfFn13_T_Quiet_to_Balanced;
  UINT32        ApmfFn13_T_Balanced_to_Quiet;
  UINT32        ApmfFn13_Pfloor_Perf;
  UINT32        ApmfFn13_Pfloor_Balanced;
  UINT32        ApmfFn13_Pfloor_Quiet;
  UINT32        ApmfFn13_P_delta_Balanced_to_Perf;
  UINT32        ApmfFn13_P_delta_Perf_to_Balanced;
  UINT32        ApmfFn13_P_delta_Quiet_to_Balanced;
  UINT32        ApmfFn13_P_delta_Balanced_to_Quiet;
  UINT8         ApmfFn13_STT_SkinTempLimit_APU_Perf_on_Lap;
  UINT8         ApmfFn13_STT_SkinTempLimit_HS2_Perf_on_Lap;
  UINT8         ApmfFn13_STT_SkinTempLimit_APU_Perf;
  UINT8         ApmfFn13_STT_SkinTempLimit_HS2_Perf;
  UINT8         ApmfFn13_STT_SkinTempLimit_APU_Balanced;
  UINT8         ApmfFn13_STT_SkinTempLimit_HS2_Balanced;
  UINT8         ApmfFn13_STT_SkinTempLimit_APU_Quiet;
  UINT8         ApmfFn13_STT_SkinTempLimit_HS2_Quiet;
  UINT32        ApmfFn13_STTMinLimit_Perf_on_lap;
  UINT32        ApmfFn13_STTMinLimit_Perf;
  UINT32        ApmfFn13_STTMinLimit_Balanced;
  UINT32        ApmfFn13_STTMinLimit_Quiet;
  UINT32        ApmfFn13_FPPT_Perf_on_lap;
  UINT32        ApmfFn13_SPPT_Perf_on_lap;
  UINT32        ApmfFn13_SPL_Perf_on_lap;
  UINT32        ApmfFn13_SPPT_APUOnly_Perf_on_Lap;
  UINT32        ApmfFn13_FPPT_Perf;
  UINT32        ApmfFn13_SPPT_Perf;
  UINT32        ApmfFn13_SPL_Perf;
  UINT32        ApmfFn13_SPPT_APUOnly_Perf;
  UINT32        ApmfFn13_FPPT_Balanced;
  UINT32        ApmfFn13_SPPT_Balanced;
  UINT32        ApmfFn13_SPL_Balanced;
  UINT32        ApmfFn13_SPPT_APUOnly_Balanced;
  UINT32        ApmfFn13_FPPT_Quiet;
  UINT32        ApmfFn13_SPPT_Quiet;
  UINT32        ApmfFn13_SPL_Quiet;
  UINT32        ApmfFn13_SPPT_APUOnly_Quiet;
  UINT32        ApmfFn13_Fan_ID_Perf;
  UINT32        ApmfFn13_Fan_ID_Balanced;
  UINT32        ApmfFn13_Fan_ID_Quiet;
  UINT32        ApmfFn1_HeartbeatTimeInterval;
  UINT8         AmdPmfFn4_Enable;                                //< APMF Function 4 support

  UINT8         DynamicP3TLimitEnable;
  UINT8         fakeDcLevel;
  UINT8         AmdPmfFn14_Enable;
  UINT8         EcPostLedTurnOn;
  UINT8         EcSmartMuxMode;                 ///< Smart Mux Support
                                                ///<  0: Diabled
                                                ///<  1: Discrete Mode
                                                ///<  2: Hybrid Graphics Mode
                                                ///<  3: Smart Mux 1.5
                                                ///<  4: Smart Mux 2.0
  UINT8         WakeOnHpdWa;
  UINT8         SoundWireSsdtSupport;
  UINT8         Usb4DepSupport;                 ///< USB4 ACPI _DEP Support
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AdjustDdi1;
  UINT8         AdjustDdi2;
  UINT8         Tdm2_din_PullUp;                // TDM2_DIN_PIN Pull High state
  UINT8         AtifNotifyCommandCode;          ///< ATIF Notify Command Code
                                                ///<  0:    Notify VGA 0x81
                                                ///<  0xD0: Notify VGA 0xD0
                                                ///<  0xD1: Notify VGA 0xD1
                                                ///<  0xD2: Notify VGA 0xD2
                                                ///<  0xD3: Notify VGA 0xD3
                                                ///<  0xD4: Notify VGA 0xD4
                                                ///<  0xD5: Notify VGA 0xD5
                                                ///<  0xD6: Notify VGA 0xD6
                                                ///<  0xD7: Notify VGA 0xD7
                                                ///<  0xD8: Notify VGA 0xD8
                                                ///<  0xD9: Notify VGA 0xD9
  UINT8         AtifFun21Support;               ///< ATIF Function 21 Support
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AtifFun21ExternalGfxPort;       ///<   External Graphics Port
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AtifFun21HideXConnectGui;       ///<   Hide XConnect GUI
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AtifFun21RunTimePMandD3;        ///<   Run Time PM and D3
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AtifFun21SupportAtifAtpx;       ///<   Support ATIF ATPX
                                                ///<  0: Diabled
                                                ///<  1: Enabled
  UINT8         AtifFun22Support;               ///< ATIF Function 22 Support
                                                ///<  0: Diabled
                                                ///<  1: Undefined
                                                ///<  2: Integrated Graphics
                                                ///<  3: Discrete Graphics
  UINT32        AtifFun22Value;                 ///< ATIF Function 22 Value
  UINT8         SmartMuxAcpiControl;            ///< Smart Mux Acpi Control
                                                ///<  0: Diabled
                                                ///<  1: Enabled without _DEP
                                                ///<  3: Enabled
  UINT8         DisplayPanelMultiplexer;        ///< Display Panel Multiplexer
                                                ///<  0 ~ 7: Key-in by users
  UINT8         SmartMuxHIDSelection;           ///< Smart Mux _HID Selection
                                                ///<  0: SMUX1234
                                                ///<  1: MSFT0005
  UINT8         SmartMuxMDMSupportLevel;        ///< Smart Mux MDM Support Level
                                                ///<  0: No Support
                                                ///<  1: Development Support
                                                ///<  2: Experimental Support
                                                ///<  3: Full Support
  UINT8         SmartMuxFirstConnectedGPU;      ///< Smart Mux First Connected GPU
                                                ///<  0: Integrated Graphics
                                                ///<  1: Discrete Graphics
  UINT8         SmartMuxAcpiMethodLocation;     ///< Smart Mux ACPI Method Location
                                                ///<  1: Under Mux
                                                ///<  2: Under dGPU
                                                ///<  3: Under Mux & dGPU
												
  UINT8         PcieSlot1;                      ///< PCIe x4 Slot-1
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         M2PcieSlotSelect;               ///< Select M.2 or PCIe Slot
                                                ///<  0 - M.2 NVMe (x4) 
                                                ///<  1 - M.2 NVMe (x2)
                                                ///<  2 - PCIe Slot (x4)
  UINT8         XgbePathSelect;
  UINT8         Port0TypeSwitch;
  UINT8         Port1TypeSwitch;
  UINT8         AicTypeSwitch;
  UINT8         Aic1TypeSel;
  UINT8         Aic2TypeSel;
  UINT8         Aic3TypeSel;
  UINT8         Aic3RevSel;
  UINT8         Port0SpeedSel;
  UINT8         Port1SpeedSel;
  UINT8         Port0ModeSel;
  UINT8         Port1ModeSel;
  UINT8         AICSpeedSel;
  UINT8         AICModeSel;
  UINT8         PxeBoot;
  UINT8         PdFwUpdate;
  UINT8         GppHotPlugEnable;               ///< PCIe x4 Slot hotplug support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         dGpuHotPlugEnable;              ///< PCIe x8 Slot hotplug support
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         PCIE2X8;                         ///< PCIe 2 x8 Slots 
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT8         AmdCpmSrisModeDebug;            ///< SRIS Debug Mode
                                                ///<  0: Disabled
                                                ///<  1: Enabled
  UINT32        AmdCpmPcieRpCorrErrMask;
  UINT32        AmdCpmPcieRpUnCorrErrMask;
  UINT32        AmdCpmPcieRpUnCorrectedErrorSev;
  UINT32        AmdCpmPcieDevCorrErrMask;
  UINT32        AmdCpmPcieDevUnCorrErrMask;
  UINT32        AmdCpmPcieDevUnCorrErrorSev;
  UINT16        DelayBeforePcieSlotReset;
  UINT8         XgbePssOutputPort0;
  UINT8         XgbePssOutputPort1;
  UINT8         PromSupportEnable;
  UINT8         SpiCs3;
  UINT8         Agpio32ResetL;
  UINT8         ReservedBuffer49 [0x10];
  UINT8         ReservedBuffer4A [0x10];
  UINT8         ReservedBuffer4B [0x10];
  UINT8         ReservedBuffer4C [0x10];
  UINT8         ReservedBuffer4D [0x10];

  /*
   * Offset Start [0x4ff](ReservedBufferLast)
   * This is a fixed-size and fixed-offset variable. The Last AMD PBS Last field.
   *
   * */
  UINT8         ReservedBufferLast;

  //
  // Offset Start [0x500] End of the AMD_PBS_SETUP_OPTION,
  // Caution: Please do not broken any priority order of PBS Setup item .
  //

} AMD_PBS_SETUP_OPTION;

#pragma pack()


#ifdef __cplusplus
}
#endif


#endif
