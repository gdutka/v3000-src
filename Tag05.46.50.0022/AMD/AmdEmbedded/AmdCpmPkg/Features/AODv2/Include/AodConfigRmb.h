/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
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
#ifndef _AOD_CONFIG_RMB_H_
#define _AOD_CONFIG_RMB_H_
#include <Features/AODv2/Include/AodSetup.h>
#define AOD_MAX_CCD_COUNT 1
#define AOD_MAX_CORE_COUNT 8

#pragma pack(push,1)
typedef struct __AOD_CONFIG
{
  AOD_VARIABLE_HEADER Header;
  UINT32        AodCpuPst0Freq;                                    /* CPU Frequency */
  UINT16        AodCpuPst0Voltage;                                 /* CPU Voltage */
  UINT8         AodCpuSmtCtrl;                                     /* SMT Control */
  UINT8         AodCpuDownCoreMode;                                /* Down Core Mode */
  UINT8         AodCpuCcdCtrl;                                     /* Cpu Down Ccd Control */
  UINT8         AodCpuCoreCtrl;                                    /* Cpu Down Core Control */
  UINT32        AodGfxOverclockingFreq;                            /* GFX Clock Frequency */
  UINT16        AodGfxOverclockingVoltage;                         /* GFX Voltage */
  UINT8         AodExpo;                                           /* EXPO */
  UINT8         AodExpoProfile;                                    /* EXPO Profile*/
  UINT8         AodMemTimingSettingDdr;                            /* Active Memory Timing Settings */
  UINT16        AodMemTargetSpeedDdr;                              /* Memory Target Speed */
  UINT8         AodMemTimingTclCtrlDdr;                            /* Tcl Ctrl */
  UINT16        AodMemTimingTclDdr;                                /* Tcl */
  UINT8         AodMemTimingTrcdCtrlDdr;                           /* Trcd Ctrl */
  UINT16        AodMemTimingTrcdDdr;                               /* Trcd */
  UINT8         AodMemTimingTrpCtrlDdr;                            /* Trp Ctrl */
  UINT16        AodMemTimingTrpDdr;                                /* Trp */
  UINT8         AodMemTimingTrasCtrlDdr;                           /* Tras Ctrl */
  UINT16        AodMemTimingTrasDdr;                               /* Tras */
  UINT8         AodMemTimingTrcCtrlDdr;                            /* Trc Ctrl */
  UINT16        AodMemTimingTrcDdr;                                /* Trc */
  UINT8         AodMemTimingTwrCtrlDdr;                            /* Twr Ctrl */
  UINT16        AodMemTimingTwrDdr;                                /* Twr */
  UINT8         AodMemTimingTrfc1CtrlDdr;                          /* Trfc1 Ctrl */
  UINT16        AodMemTimingTrfc1Ddr;                              /* Trfc1 */
  UINT8         AodMemTimingTrfc2CtrlDdr;                          /* Trfc2 Ctrl */
  UINT16        AodMemTimingTrfc2Ddr;                              /* Trfc2 */
  UINT8         AodMemTimingTrfcSbCtrlDdr;                         /* TrfcSb Ctrl */
  UINT16        AodMemTimingTrfcSbDdr;                             /* TrfcSb */
  UINT8         AodMemTimingTrtpCtrlDdr;                           /* Trtp Ctrl */
  UINT16        AodMemTimingTrtpDdr;                               /* Trtp */
  UINT8         AodMemTimingTrrdLCtrlDdr;                          /* TrrdL Ctrl */
  UINT16        AodMemTimingTrrdLDdr;                              /* TrrdL */
  UINT8         AodMemTimingTrrdSCtrlDdr;                          /* TrrdS Ctrl */
  UINT16        AodMemTimingTrrdSDdr;                              /* TrrdS */
  UINT8         AodMemTimingTfawCtrlDdr;                           /* Tfaw Ctrl */
  UINT16        AodMemTimingTfawDdr;                               /* Tfaw */
  UINT8         AodMemTimingTwtrLCtrlDdr;                          /* TwtrL Ctrl */
  UINT16        AodMemTimingTwtrLDdr;                              /* TwtrL */
  UINT8         AodMemTimingTwtrSCtrlDdr;                          /* TwtrS Ctrl */
  UINT16        AodMemTimingTwtrSDdr;                              /* TwtrS */
  UINT8         AodMemTimingTrdrdScLCtrlDdr;                       /* TrdrdScL Ctrl */
  UINT16        AodMemTimingTrdrdScLDdr;                           /* TrdrdScL */
  UINT8         AodMemTimingTrdrdScCtrlDdr;                        /* TrdrdSc Ctrl */
  UINT16        AodMemTimingTrdrdScDdr;                            /* TrdrdSc */
  UINT8         AodMemTimingTrdrdSdCtrlDdr;                        /* TrdrdSd Ctrl */
  UINT16        AodMemTimingTrdrdSdDdr;                            /* TrdrdSd */
  UINT8         AodMemTimingTrdrdDdCtrlDdr;                        /* TrdrdDd Ctrl */
  UINT16        AodMemTimingTrdrdDdDdr;                            /* TrdrdDd */
  UINT8         AodMemTimingTwrwrScLCtrlDdr;                       /* TwrwrScL Ctrl */
  UINT16        AodMemTimingTwrwrScLDdr;                           /* TwrwrScL */
  UINT8         AodMemTimingTwrwrScCtrlDdr;                        /* TwrwrSc Ctrl */
  UINT16        AodMemTimingTwrwrScDdr;                            /* TwrwrSc */
  UINT8         AodMemTimingTwrwrSdCtrlDdr;                        /* TwrwrSd Ctrl */
  UINT16        AodMemTimingTwrwrSdDdr;                            /* TwrwrSd */
  UINT8         AodMemTimingTwrwrDdCtrlDdr;                        /* TwrwrDd Ctrl */
  UINT16        AodMemTimingTwrwrDdDdr;                            /* TwrwrDd */
  UINT8         AodMemTimingTwrrdCtrlDdr;                          /* Twrrd Ctrl */
  UINT16        AodMemTimingTwrrdDdr;                              /* Twrrd */
  UINT8         AodMemTimingTrdwrCtrlDdr;                          /* Trdwr Ctrl */
  UINT16        AodMemTimingTrdwrDdr;                              /* Trdwr */
  UINT8         AodMemDrvStrenCaDdr;                               /* Processor CA drive strengths */
  UINT8         AodMemDrvStrenDqDdr;                               /* Processor DQ drive strengths */
  UINT8         AodMemDataBusCfgProcOdtDdr;                        /* Processor ODT impedance */
  UINT8         AodMemDramDrvStrenDqDdr;                           /* Dram DQ drive strengths */
  UINT8         AodMemDramOdtImpedRttNomWrDdr;                     /* Dram ODT impedance RTT_NOM_WR */
  UINT8         AodMemDramOdtImpedRttNomRdDdr;                     /* Dram ODT impedance RTT_NOM_RD */
  UINT8         AodMemDramOdtImpedRttWrDdr;                        /* Dram ODT impedance RTT_WR */
  UINT8         AodMemDramOdtImpedRttParkDdr;                      /* Dram ODT impedance RTT_PARK */
  UINT8         AodMemDramOdtImpedRttParkDqsDdr;                   /* Dram ODT impedance DQS_RTT_PARK */
  UINT8         AodMemCtrllerPowerDownEnDdr;                       /* Power Down Enable */
  UINT16        AodFclkFrequency;                                  /* Infinity Fabric Frequency and Dividers */
  UINT8         AodUclkDiv1Mode;                                   /* UCLK DIV1 MODE */
  UINT8         AodNbioPBOCtl;                                     /* Precision Boost Overdrive */
  UINT8         AodNbioPBOAdvancedCtrl;                            /* PBO Limits */
  UINT32        AodNbioSocketPPTLimit;                             /* PPT Limit [mW] */
  UINT32        AodNbioTDCVDDLimit;                                /* TDC Limit [mA] */
  UINT32        AodNbioEDCVDDLimit;                                /* EDC Limit [mA] */
  UINT32        AodNbioTDCSOCLimit;                                /* SOC TDC Limit [mA] */
  UINT32        AodNbioEDCSOCLimit;                                /* SOC EDC Limit [mA] */
  UINT8         AodNbioPBOScalarCtl;                               /* Precision Boost Overdrive Scalar Ctrl */
  UINT32        AodNbioPBOScalar;                                  /* Precision Boost Overdrive Scalar */
  UINT8         AodCpuBoostClock;                                  /* CPU Boost Clock Override */
  UINT8         AodFMaxFreqIncrease;                               /* Max CPU Boost Clock Override(+) */
  UINT16        AodFMaxFreqDecrease;                               /* Max CPU Boost Clock Override(-) */
  UINT8         AodGpuBoostClock;                                  /* GPU Boost Clock Override */
  UINT8         AodGfxFMaxFreq;                                    /* Max GPU Boost Clock Override */
  UINT8         AodNbiocHTCCtl;                                    /* Platform Thermal Throttle Ctrl */
  UINT8         AodNbiocHTC;                                       /* Platform Thermal Throttle Limit */
  UINT8         AodCurveOptimizer;                                 /* Curve Optimizer */
  UINT8         AodAllCoreCurveOptimizerSign;                      /* All Core Curve Optimizer Sign */
  UINT16        AodAllCoreCurveOptimizerMagnitude;                 /* All Core Curve Optimizer Magnitude */
  UINT8         AodPerCoreCurveOptimizerSign[64];                  /* Per Core Curve Optimizer Sign */
  UINT16        AodPerCoreCurveOptimizerMagnitude[64];             /* Per Core Curve Optimizer Magnitude */
  UINT8         AodGfxCurveOptimizer;                              /* GFX Curve Optimizer */
  UINT8         AodGfxCurveOptimizerSign;                          /* GFX Curve Optimizer Sign */
  UINT16        AodGfxCurveOptimizerMagnitude;                     /* GFX Curve Optimizer Magnitude */
  UINT8         AodGnbFclkOcFlag;                                  /* SoC/Uncore OC Mode */
  UINT32        AodSocOverclockingVoltage;                         /* SoC Voltage */
  UINT8         AodLN2Mode;                                        /* LN2 Mode */
  UINT16        AodVddIoCurrent;                                   /* Current  VDDIO Voltage */
  UINT8         AodVddIoCtrl;                                      /* VDDIO Ctrl */
  UINT16        AodDimmVdd;                                        /* DIMM VDD Adjust */
  UINT16        AodDimmVddq;                                       /* DIMM VDDQ Adjust */
  UINT16        AodApuVddIo;                                       /* APU VDDIO Adjust */
  UINT16        AodVppVoltage;                                     /* VPP Adjust */
  UINT8         AodVppCtrl;                                        /* VPP Ctrl */
  UINT8         AodVddMemCtrl;                                     /* VDD_MEM Control */
  UINT16        AodVddMemAdjust;                                   /* VDD_MEM Adjust */
  UINT8         AodMemIntlv;                                       /* Memory interleaving */
  UINT8         AodMemIntlvSize;                                   /* Memory interleaving Size */
  UINT8         AodVddpVoltageCtrl;                                /* VDDP Voltage Control */
  UINT16        AodVddpVoltage;                                    /* VDDP Voltage Adjust */
  UINT8         AodMaxCcdCount;                                    /* Max Ccd Count */
  UINT8         AodMaxCorePerCcd;                                  /* Max Core Count Per Ccd */
  UINT8         AodCcdBitMap;                                      /* Ccd BIT Map */
  UINT8         AodCoreBitMap[AOD_MAX_CCD_COUNT];                  /* Core Status */
  UINT8         AodLclkCtrl;                                       /* LCLK Frequency Control */
  UINT16        AodMinLclkFreq;                                    /* Minimun Frequency */
  UINT16        AodMaxLclkFreq;                                    /* Maximum Frequency */
  UINT8         AodVddMiscCtrl;                                    /* VDD Misc Control */
  UINT16        AodVddMiscVoltage;                                 /* VDD Misc Voltage */

} AOD_CONFIG;

#pragma pack(pop)

#define AOD_SETUP_VARSTORE\
  varstore AOD_CONFIG, varid = 0xF000, name = AodSetup, guid = AOD_SETUP_GUID;

#endif