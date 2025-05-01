/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef _AOD_SOC_LIB_H_
#define _AOD_SOC_LIB_H_
#include <Features/AODv2/Include/AodSetup.h>
#include AOD_SOC_SETUP_DEFINITION(Rpl)

///
/// RPL ComboFalg
///
#define AOD_RPL_AM5_COMBO_FLAG    1

///
/// RPL AOD Core Information
///
#define AOD_DIE_PER_SOCKET                  1
#define AOD_CCD_PER_DIE                     1
#define AOD_CCX_PER_CCD                     1
#define AOD_CORE_PER_CCX                    8
#define INVALID_ARG                         0xFEFEFEFE
#define SMC_MSG_GetOverclockCap_RPL         0x5E
///
/// RPL AOD Memory UMC Information
///
#define CHANNEL_MAX              2
#define CH_SMN_BASE_CH0_MP0       0x00050000
#define CH_SMN_BASE_CH1_MP0       0x00150000
#define CTL_SMN_BASE_CH0_MP0      0x00051000
#define CTL_SMN_BASE_CH1_MP0      0x00151000

#define DRAM_CS_BASE_ADDR_DIMM0     0x00000000
#define DRAM_CS_BASE_ADDR_DIMM1     0x00000008

#define DRAM_CONFIGURATION          0x00000200
#define DRAM_TIMING1                0x00000204
#define DRAM_TIMING2                0x00000208
#define DRAM_TIMING3                0x0000020C
#define DRAM_TIMING4                0x00000210
#define DRAM_TIMING5                0x00000214
#define DRAM_TIMING6                0x00000218
#define DRAM_TIMING7                0x0000021C
#define DRAM_TIMING8                0x00000220
#define DRAM_TIMING9                0x00000224
#define DRAM_TIMING10               0x00000228
#define DRAM_TIMING11               0x0000022C
#define DRAM_TIMING12               0x00000230
#define DRAM_TIMING13               0x00000234
#define DRAM_TIMING14               0x00000238
#define DRAM_TIMING15               0x0000023C
#define DRAM_TIMING17               0x00000244
#define DRAM_TIMING20               0x00000250
#define DRAM_TIMING21               0x00000254
#define DRAM_TIMING22               0x00000258
#define DRAM_TIMING23               0x0000025C
#define DRAM_TRFC_TIMING_DIMM0      0x00000260
#define DRAM_TRFC_TIMING_DIMM1      0x00000264
#define DRAM_ODT_TIMING             0x00000280
#define DRAM_SPAZ_CONTROL           0x0000012C
#define DRAM_REF_TIMING             0x000002C0
#define DRAM_DEBUG_MISC             0x00000D6C
#define AOD_DDR267_FREQUENCY  133     ///< DDR 267
#define AOD_DDR400_FREQUENCY  200     ///< DDR 400
#define AOD_DDR533_FREQUENCY  266     ///< DDR 533
#define AOD_DDR667_FREQUENCY  333     ///< DDR 667
#define AOD_DDR800_FREQUENCY  400     ///< DDR 800
#define AOD_DDR1066_FREQUENCY 533     ///< DDR 1066
#define AOD_DDR1333_FREQUENCY 667     ///< DDR 1333
#define AOD_DDR1375_FREQUENCY 687     ///< DDR 1375
#define AOD_DDR1500_FREQUENCY 750     ///< DDR 1500
#define AOD_DDR1600_FREQUENCY 800     ///< DDR 1600
#define AOD_DDR1866_FREQUENCY 933     ///< DDR 1867
#define AOD_DDR2000_FREQUENCY 1000    ///< DDR 2000
#define AOD_DDR2100_FREQUENCY 1050    ///< DDR 2100
#define AOD_DDR2133_FREQUENCY 1067    ///< DDR 2133
#define AOD_DDR2400_FREQUENCY 1200    ///< DDR 2400
#define AOD_DDR2667_FREQUENCY 1333    ///< DDR 2667
#define AOD_DDR2733_FREQUENCY 1367    ///< DDR 2733
#define AOD_DDR2750_FREQUENCY 1375    ///< DDR 2750
#define AOD_DDR2800_FREQUENCY 1400    ///< DDR 2800
#define AOD_DDR2867_FREQUENCY 1433    ///< DDR 2867
#define AOD_DDR2933_FREQUENCY 1467    ///< DDR 2933
#define AOD_DDR3000_FREQUENCY 1500    ///< DDR 3000
#define AOD_DDR3067_FREQUENCY 1533    ///< DDR 3067
#define AOD_DDR3133_FREQUENCY 1567    ///< DDR 3133
#define AOD_DDR3200_FREQUENCY 1600    ///< DDR 3200
#define AOD_DDR3267_FREQUENCY 1633    ///< DDR 3267
#define AOD_DDR3333_FREQUENCY 1667    ///< DDR 3333
#define AOD_DDR3400_FREQUENCY 1700    ///< DDR 3400
#define AOD_DDR3467_FREQUENCY 1733    ///< DDR 3467
#define AOD_DDR3533_FREQUENCY 1767    ///< DDR 3533
#define AOD_DDR3600_FREQUENCY 1800    ///< DDR 3600
#define AOD_DDR3667_FREQUENCY 1833    ///< DDR 3667
#define AOD_DDR3733_FREQUENCY 1867    ///< DDR 3733
#define AOD_DDR3800_FREQUENCY 1900    ///< DDR 3800
#define AOD_DDR3867_FREQUENCY 1933    ///< DDR 3867
#define AOD_DDR3933_FREQUENCY 1967    ///< DDR 3933
#define AOD_DDR4000_FREQUENCY 2000    ///< DDR 4000
#define AOD_DDR4067_FREQUENCY 2033    ///< DDR 4067
#define AOD_DDR4133_FREQUENCY 2067    ///< DDR 4133
#define AOD_DDR4200_FREQUENCY 2100    ///< DDR 4200
#define AOD_DDR4267_FREQUENCY 2133    ///< DDR 4267
#define AOD_DDR4333_FREQUENCY 2167    ///< DDR 4333
#define AOD_DDR4400_FREQUENCY 2200    ///< DDR 4400
#define AOD_DDR4467_FREQUENCY 2233    ///< DDR 4467
#define AOD_DDR4533_FREQUENCY 2267    ///< DDR 4533
#define AOD_DDR4600_FREQUENCY 2300    ///< DDR 4600
#define AOD_DDR4667_FREQUENCY 2333    ///< DDR 4667
#define AOD_DDR4733_FREQUENCY 2367    ///< DDR 4733
#define AOD_DDR4800_FREQUENCY 2400    ///< DDR 4800
#define AOD_DDR4867_FREQUENCY 2433    ///< DDR 4867
#define AOD_DDR4933_FREQUENCY 2467    ///< DDR 4933
#define AOD_DDR5000_FREQUENCY 2500    ///< DDR 5000
#define AOD_DDR5100_FREQUENCY 2550    ///< DDR 5100
#define AOD_DDR5200_FREQUENCY 2600    ///< DDR 5200
#define AOD_DDR5300_FREQUENCY 2650    ///< DDR 5300
#define AOD_DDR5400_FREQUENCY 2700    ///< DDR 5400
#define AOD_DDR5500_FREQUENCY 2750    ///< DDR 5500
#define AOD_DDR5600_FREQUENCY 2800    ///< DDR 5600
#define AOD_DDR5700_FREQUENCY 2850    ///< DDR 5700
#define AOD_DDR5800_FREQUENCY 2900    ///< DDR 5800
#define AOD_DDR5900_FREQUENCY 2950    ///< DDR 5900
#define AOD_DDR6000_FREQUENCY 3000    ///< DDR 6000
#define AOD_DDR6100_FREQUENCY 3050    ///< DDR 6100
#define AOD_DDR6200_FREQUENCY 3100    ///< DDR 6200
#define AOD_DDR6300_FREQUENCY 3150    ///< DDR 6300
#define AOD_DDR6400_FREQUENCY 3200    ///< DDR 6400
#define AOD_DDR6500_FREQUENCY 3250    ///< DDR 6500
#define AOD_DDR6600_FREQUENCY 3300    ///< DDR 6600
#define AOD_DDR6700_FREQUENCY 3350    ///< DDR 6700
#define AOD_DDR6800_FREQUENCY 3400    ///< DDR 6800
#define AOD_DDR6900_FREQUENCY 3450    ///< DDR 6900
#define AOD_DDR7000_FREQUENCY 3500    ///< DDR 7000
#define AOD_DDR7100_FREQUENCY 3550    ///< DDR 7100
#define AOD_DDR7200_FREQUENCY 3600    ///< DDR 7200
#define AOD_DDR7300_FREQUENCY 3650    ///< DDR 7300
#define AOD_DDR7400_FREQUENCY 3700    ///< DDR 7400
#define AOD_DDR7467_FREQUENCY 3733    ///< DDR 7467
#define AOD_DDR7500_FREQUENCY 3750    ///< DDR 7500
#define AOD_DDR7600_FREQUENCY 3800    ///< DDR 7600
#define AOD_DDR7700_FREQUENCY 3850    ///< DDR 7700
#define AOD_DDR7800_FREQUENCY 3900    ///< DDR 7800
#define AOD_DDR7900_FREQUENCY 3950    ///< DDR 7900
#define AOD_DDR8000_FREQUENCY 4000    ///< DDR 8000
#define AOD_DDR8100_FREQUENCY 4050    ///< DDR 8100
#define AOD_DDR8200_FREQUENCY 4100    ///< DDR 8200
#define AOD_DDR8300_FREQUENCY 4150    ///< DDR 8300
#define AOD_DDR8400_FREQUENCY 4200    ///< DDR 8400
#define AOD_DDR8500_FREQUENCY 4250    ///< DDR 8500
#define AOD_DDR8533_FREQUENCY 4267    ///< DDR 8533
#define AOD_DDR8600_FREQUENCY 4300    ///< DDR 8600
#define AOD_DDR8700_FREQUENCY 4350    ///< DDR 8700
#define AOD_DDR8800_FREQUENCY 4400    ///< DDR 8800
// Section ID
#define AOD_SETUP_FORM_ID                                                       0x098
#define AOD_SETUP_DECLINE_FORM_ID                                               0x099
#define SETUP_ACCEPT_LABLE                                                      0x100
#define SETUP_MANUAL_CPU_OVERCLOCKING_LABLE                                     0x101
#define SETUP_DDR_AND_INFINITY_FABRIC_FREQUENCY_TIMINGS_LABLE                   0x102
#define SETUP_PRECISION_BOOST_OVERDRIVE_LABLE                                   0x103
#define SETUP_VDDG_VOLTAGE_CONTROL_LABLE                                        0x104
#define SETUP_VDDP_VOLTAGE_CONTROL_LABLE                                        0x105
#define SETUP_SOC_UNCORE_OC_MODE_LABLE                                          0x106
#define SETUP_SOC_VOLTAGE_LABLE                                                 0x107
#define SETUP_ECO_MODE_LABLE                                                    0x108
#define SETUP_LN2_MODE_LABLE                                                    0x109
#define SETUP_VDD_MISC_LABLE                                                    0x10A
#define SETUP_LCLK_FREQUENCY_CONTROL_LABLE                                      0x10B
#define SETUP_ONBOARD_VOLTAGE_CONTROL_LABLE                                     0x10C
#define SETUP_CPU_CORE_COUNT_CONTROL_LABLE                                      0x10D
#define SETUP_DDR_OPTIONS_LABLE                                                 0x111
#define SETUP_INFINITY_FABRIC_FREQUENCY_AND_DIVIDERS_LABLE                      0x112
#define SETUP_DDR_TIMING_CONFIGURATION_LABLE                                    0x113
#define SETUP_DDR_BUS_CONFIGURATION_LABLE                                       0x114
#define SETUP_DDR_CONTROLLER_CONFIGURATION_LABLE                                0x115
#define SETUP_DDR_SPD_TIMING_LABLE                                              0x116
#define SETUP_DDR_NON_SPD_TIMING_LABLE                                          0x117
#define SETUP_DDR_DATA_BUS_CONFIGURATION_LABLE                                  0x118
#define SETUP_DDR_POWER_OPTIONS_LABLE                                           0x119
#define SETUP_GFX_CURVE_OPTIMIZER_LABLE                                         0x11A
#define SETUP_CCD_FREQUENCY_OPTIMIZER_LABLE                                     0x11B
#define SETUP_CURVE_OPTIMIZER_LABLE                                             0x11C
#define SETUP_VDDIO_VOLTAGE_CONTROL_LABLE                                       0x12D
#define SETUP_VPP_VOLTAGE_CONTROL_LABLE                                         0x12E

// KEY function ID
#define KEY_AOD_DOWN_CORE_DYNAMIC                                               0x1000  //AodDownCoreDynamic
#define KEY_AOD_DOWN_CORE_SAVE_DYNAMIC                                          0x1010  //AodDownCoreDynamic
#define KEY_AOD_DOWN_CORE_DISCARD_DYNAMIC                                       0x1011  //AodDownCoreDynamic
#define KEY_AOD_DIMM_VDDIO                                                      0x1012  //AodDimmVddio
#define KEY_AOD_VDDIO_CTRL                                                      0x1014  //AodVddIoCtrl
#define KEY_AOD_APU_VDDIO                                                       0x1013  //AodApuVddio
#define KEY_AOD_MIN_LCLK_FREQ                                                   0x1015  //AodMinLclkFreq
#define KEY_AOD_MAX_LCLK_FREQ                                                   0x1016  //AodMaxLclkFreq

// Label CBS ID
#define LABEL_CBS_AOD_DOWN_CORE_DYNAMIC_START                                   0x210B  //AodDownCoreDynamic Start Label
#define LABEL_CBS_AOD_DOWN_CORE_DYNAMIC_END                                     0x210C  //AodDownCoreDynamic End Label
#define LABEL_CBS_AOD_CCD_FREQUENCY_ADJUSTMENT_START                            0x211D  //AodCCDFrequencyAdjustment Start Label
#define LABEL_CBS_AOD_CCD_FREQUENCY_ADJUSTMENT_END                              0x211E  //AodCCDFrequencyAdjustment End Label
#define LABEL_CBS_AOD_PER_CORE_CURVE_OPTIMIZER_DYNAMIC_START                    0x211D  //AodPerCoreCurveOptimizerDynamic Start Label
#define LABEL_CBS_AOD_PER_CORE_CURVE_OPTIMIZER_DYNAMIC_END                      0x211E  //AodPerCoreCurveOptimizerDynamic End Label
#define LABEL_CBS_AOD_APU_VDD_IO_DYNAMIC_START                                  0x211F  //AodApuVddIo Start Label
#define LABEL_CBS_AOD_APU_VDD_IO_DYNAMIC_END                                    0x2120  //AodApuVddIo End Label
#define LABEL_CBS_AOD_VDDG_PER_CCD_VOLTAGE_START                                0x2121  //AodVddgPerCcdVoltage Start Label
#define LABEL_CBS_AOD_VDDG_PER_CCD_VOLTAGE_END                                  0x2122  //AodVddgPerCcdVoltage End Label

  /*Cmdid                        ,   Name               ,     Property, Data Count, Data*/
#define AOD_WMI_LIST_RPL\
  {GET_INTERLEVAVINGCAP          , "Get Interlevaing Cap"        , 1   , 3,    {0, 0xffffffff, 1,}},\
  {SET_SOFTWARE_DOWN_CORE        , "Software Downcore Config"    , 1   , 3,    {0, 0xffffffff, 1,}},\
  {SET_CURVE_OPT                 , "Set Curve Optimizer"         , 1   , 3,    {0, 30, 1,}},\
  {SET_GFX_CURVE_OPT             , "Set GFX Curve Optimizer"     , 1,    3,    {0, 30, 1,}},\
  {SET_SMTEN                     , "Set SMTEn"                   , 0   , 2,    {0,1,}}, \
  {SET_MEM_CLOCK                 , "Set Mem Clock"               , 0   , 0x2f, {1000, 1200, 0x640, 0x6a4, 0x708, 0x76c,\
   0x7d0, 0x834, 0x898, 0x8fc, 0x960, 0x9c4, 0xa28, 0xa8c, 0xaf0, 0xb54, 0xbb8, 0xc1c, 0xc80, 0xce4, 0xd48, 0xdac, \
   0xe10, 0xe74, 0xed8, 0xf3c, 0xfa0, 0x1004, 0x1068, 0x10cc, 0x1130, 0x1194, 0x11f8, 0x125c, 0x12c0, 0x1324, 0x1388, \
   0x13ec, 0x1450, 0x14b4, 0x1518, 0x157c, 0x15e0, 0x1644, 0x16a8, 0x170c, 0x1770,}}, \
  {SET_TCL                       , "Set Tcl"                     , 1   , 3   , {0x16, 0x40, 0x2,}}, \
  {SET_TRCD                      , "Set Trcd"                    , 1   , 3   , {0x8, 0x3E, 0x2,}}, \
  {SET_TRP                       , "Set Trp"                     , 1   , 3   , {0x8, 0x3E, 0x2,}}, \
  {SET_TRAS                      , "Set Tras"                    , 1   , 3   , {0x1E, 0x7E, 0x2,}}, \
  {SET_TRC                       , "Set Trc"                     , 1   , 3   , {0x20, 0xFF, 0x1,}}, \
  {SET_TWR                       , "Set Twr"                     , 1   , 3   , {0x30, 0x60, 0x2,}}, \
  {SET_TRFC1                     , "Set Trfc1"                   , 1   , 3   , {0x32, 0xFFF, 0x1,}}, \
  {SET_TRFC2                     , "Set Trfc2"                   , 1   , 3   , {0x32, 0xFFF, 0x1,}}, \
  {SET_TRFCSB                    , "Set TrfcSb"                  , 1   , 3   , {0x32, 0x7FF, 0x1,}}, \
  {SET_TRTP                      , "Set Trtp"                    , 1   , 3   , {0x5, 0x1F, 0x1,}}, \
  {SET_TRRDL                     , "Set TrrdL"                   , 1   , 3   , {0x4, 0x20, 0x1,}}, \
  {SET_TRRDS                     , "Set TrrdS"                   , 1   , 3   , {0x4, 0x14, 0x1,}}, \
  {SET_TFAW                      , "Set Tfaw"                    , 1   , 3   , {0x14, 0x50, 0x1,}}, \
  {SET_TWTRL                     , "Set TwtrL"                   , 1   , 3   , {0x8, 0x30, 0x1,}}, \
  {SET_TWTRS                     , "Set TwtrS"                   , 1   , 3   , {0x2, 0x10, 0x1,}}, \
  {SET_TRDRDSCL                  , "Set TrdrdScL"                , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TRDRDSC                   , "Set TrdrdSc"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TRDRDSD                   , "Set TrdrdSd"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TRDRDDD                   , "Set TrdrdDd"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TWRWRSCL                  , "Set TwrwrScL"                , 1   , 3   , {0x1, 0x3F, 0x1,}}, \
  {SET_TWRWRSC                   , "Set TwrwrSc"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TWRWRSD                   , "Set TwrwrSd"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TWRWRDD                   , "Set TwrwrDd"                 , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TWRRD                     , "Set Twrrd"                   , 1   , 3   , {0x1, 0xF, 0x1,}}, \
  {SET_TRDWR                     , "Set Trdwr"                   , 1   , 3   , {0x1, 0x1F, 0x1,}}, \
  {SET_RTTNOMWR                  , "Set RttNomWr"                , 1   , 3   , {0x0, 0x7, 0x1,}}, \
  {SET_RTTNOMRD                  , "Set RttNomRd"                , 1   , 3   , {0x0, 0x7, 0x1,}}, \
  {SET_RTTWR                     , "Set RttWr"                   , 1   , 3   , {0x0, 0x7, 0x1,}}, \
  {SET_RTTPARK                   , "Set RttPark"                 , 1   , 3   , {0x0, 0x7, 0x1,}}, \
  {SET_RTTPARKDQS                , "Set RttParkDqs"              , 1   , 3   , {0x0, 0x7, 0x1,}}, \
  {SET_PROCODT                   , "Set ProcODT"                 , 0   , 20  , {3,0,5,0xe,6,0x3e,0x3d,7,0x3c,0xf,0x1e,\
    0xd,0x1d,2,0x1c,0x1f, 1,0xc,0x3f,4,}}, \
  {SET_DRAMDATADRIVESTRENGTH     , "Set DRAMDataDriveStrength"   , 0   , 3   , {0,2,1,}}, \
  {SET_POWERDWONEN               , "Set PowerDwonEn"             , 0   , 2   , {1,0,}}, \
  {SET_PPT_LIMIT                 , "Set PPT Limit"               , 1   , 3   , {0, 0xffffffff, 0x1,}}, \
  {SET_TDC_LIMIT                 , "Set TDC Limit"               , 1   , 3   , {0, 0xffffffff, 0x1,}}, \
  {SET_EDC_LIMIT                 , "Set EDC Limit"               , 1   , 3   , {0, 0xffffffff, 0x1,}}, \
  {SET_SOC_TDC_LIMIT             , "Set Soc TDC Limit"           , 1   , 3   , {0, 0xffffffff, 0x1,}}, \
  {SET_SOC_EDC_LIMIT             , "Set Soc EDC Limit"           , 1   , 3   , {0, 0xffffffff, 0x1,}}, \
  {SET_SCALAR                    , "Set Scalar"                  , 0   , 11  , \
    {500,700,600,900,400,100,300,200,800,1000,0,}}, \
  {SET_CCLK_FMAX                 , "Set CCLK Fmax"               , 1   , 3   , {0xFFFFFC18, 200, 25}}, \
  {SET_FCLK_OC_MODE              , "Set FCLK OC Mode"            , 0   , 2   , {0,1,}}, \
  {SET_FCLK_FREQUENCY            , "Set FCLK Frequency"          , 0   , 50  , {1533,2550,1633,2950,2850,1933,2267,\
    2600,3000,2800,2067,1700,1467,1567,2433,2400,2200,2333,2167,933,1833,2650,2467,2000,2750,1067,1733,1867,1900,2900,\
    800,1500, 1967,2367,1433,1333,2100,2500,1600,1400,1667,1367,1800,2700,1200,1767,2033,2300, 2133,2233,}},\
  {SET_VDDIO                     , "Set VDDIO"                   , 1   , 3  , { 800, 1430, 10,}},\
  {SET_VPP                       , "Set VPP"                     , 1   , 3  , { 1500,2130, 10,}},\
  {SET_APU_VDDIO                 , "Set APU VDDIO"               , 1   , 3  , { 800, 1430, 10,}},\
  {SET_INTERLEAVEMODE            , "Set Interleave Mode"         , 0   , 1  , {0,}},\
  {SET_INTERLEAVESIZE            , "Set Interleave Size"         , 0   , 4  , {0, 1, 2, 3,}},\
  {SET_SOCVID                    , "Set SOC VID"                 , 1   , 3  , {0, 0x1FF, 1,}},\
  {SET_CLDO_VDDP                 , "Set CLDO_VDDP"               , 1   , 3  , {0, 0x7d0, 1,}},\
  {SET_LCLK_FREQ                 , "Set LCLK Freq"               , 1   , 3  , {150, 2500, 1,}},\
  {SET_VDD_MISC                  , "Set VDD_MISC"                , 1   , 3  , {500, 5600, 10,}},\
  {SET_VDDG                      , "Set CLDO VDDG"               , 1   , 3  , {600, 1500, 10,}},\
  {SET_VDDG_IOD                  , "Set VDDG IOD"                , 1   , 3  , {600, 1500, 10,}},\
  {SET_CCD_FREQ_OPT              , "Set CCD Freq Optimizer"      , 1   , 3  , {0, 0xffff, 1,}},\
  {SET_UCLK_DIV1                 , "Set UCLK DIV1"               , 0   , 2  , {0, 1,},}

#define MEMCLK_FREQ_TO_ID(Freq)  ((((Freq) * 3) + 50) / 100)
#define MEMCLK_ID_TO_FREQ(Id)  ((((Id) * 200) + 3) / 6)
#define MEMCLK_FREQ_TO_ID_V2(Freq)  (((Freq) <= 2500) ? (MEMCLK_FREQ_TO_ID(Freq)) : (MEMCLK_FREQ_TO_ID(2500) + (((Freq) - 2500) / 50)))
#define MEMCLK_ID_TO_FREQ_V2(Id)  (((Id) <= 0x4B) ? (MEMCLK_ID_TO_FREQ(Id)) : (MEMCLK_ID_TO_FREQ(0x4B) + (((Id) - 0x4B) * 50)))

//APCB Define
#ifndef APCB_TOKEN_UID_AOD_CCD0_CORE_CTRL
/// @DESC: AOD_CCD0_CORE_CTRL
  #define APCB_TOKEN_UID_AOD_CCD0_CORE_CTRL                                 0x3F7F8A94
#endif
#ifndef APCB_TOKEN_UID_AOD_CCD_CORE_CTRL
/// @DESC: AOD_CCD_CORE_CTRL
  #define APCB_TOKEN_UID_AOD_CCD_CORE_CTRL                                  0x530BB305
#endif

#ifndef APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE1
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_CCD_VDDG_VOLTAGE1                  0xC74D6F5B
#endif

#ifndef APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE1
  #define APCB_TOKEN_UID_CBS_CMN_GNB_SMU_IOD_VDDG_VOLTAGE1                  0x74BB23EF
#endif
#endif