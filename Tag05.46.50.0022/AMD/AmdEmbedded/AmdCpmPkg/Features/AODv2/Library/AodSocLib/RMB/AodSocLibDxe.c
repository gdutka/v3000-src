
/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

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
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HiiLib.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/ApobCommonServiceLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <AmdSoc.h>
#include <AMD.h>
#include <RMB/ApcbV3TokenUid.h>
#include <RMB/ApcbV3Priority.h>
#include <Features/AODv2/Include/Protocol/AodCmdProtocol.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include <Features/AODv2/Include/Aod.h>
#include "AodSocLib.h"
#include "AodBitMapDownCore.h"
#include "AodVoltage.h"
#include "AodCurveOptimizer.h"
#include "AodSyncApcb.h"
#include "AodRmp.h"
//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of PreparePackageList() to
// create a packagelist (which contains Form packages, String packages, etc).
//

extern UINT8 AodHiiBin[];

//
// This is the generated String package data for all Unicode files.
// This data array is ready to be used as input of PreparePackageList() to
// create a packagelist (which contains Form packages, String packages, etc).
//

extern UINT8 AodSocLibRmbStrings[];



AOD_MOUDLE_NAME ("AodSocLibDxe")
AOD_SOC_NAME ("RMB")
UINT8 mAodComboFlag = 0;
EFI_GUID mAodFormSetGuid = AOD_SETUP_FORMSET_GUID;
AMD_APCB_SERVICE_PROTOCOL        *mApcbDxeServiceProtocol = NULL;
UINT8  mRmpEnabled = 0;
//
// DDR5 FP7r2/AM5 WMI list
//
struct {
  UINT32                CmdId;
  CONST CHAR8           *pName;
  UINT8                 Property;
  UINT32                DataCount;
  UINT32                pValue[100];
} mWmiCmidList[] = {
  AOD_COMMON_WMI_LIST,
  AOD_WMI_LIST_RMB,
  {0, "", 0, 0, {0,}},
};

//
// Lower Power DDR5 fp7 WMI list
//
struct {
  UINT32                CmdId;
  CONST CHAR8           *pName;
  UINT8                 Property;
  UINT32                DataCount;
  UINT32                pValue[100];
} mLDdr5WmiCmidList[] = {
    AOD_COMMON_WMI_LIST,
    AOD_WMI_LIST_RMB_FP7,
    {0, "", 0, 0, {0,}},
  };
UINT16 MemClkList[] = {
  AOD_DDR267_FREQUENCY ,  ///< DDR 267
  AOD_DDR400_FREQUENCY ,  ///< DDR 400
  AOD_DDR533_FREQUENCY ,  ///< DDR 533
  AOD_DDR667_FREQUENCY ,  ///< DDR 667
  AOD_DDR800_FREQUENCY ,  ///< DDR 800
  AOD_DDR1066_FREQUENCY,  ///< DDR 1066
  AOD_DDR1333_FREQUENCY,  ///< DDR 1333
  AOD_DDR1375_FREQUENCY,  ///< DDR 1375
  AOD_DDR1500_FREQUENCY,  ///< DDR 1500
  AOD_DDR1600_FREQUENCY,  ///< DDR 1600
  AOD_DDR1866_FREQUENCY,  ///< DDR 1867
  AOD_DDR2000_FREQUENCY,  ///< DDR 2000
  AOD_DDR2100_FREQUENCY,  ///< DDR 2100
  AOD_DDR2133_FREQUENCY,  ///< DDR 2133
  AOD_DDR2400_FREQUENCY,  ///< DDR 2400
  AOD_DDR2667_FREQUENCY,  ///< DDR 2667
  AOD_DDR2733_FREQUENCY,  ///< DDR 2733
  AOD_DDR2750_FREQUENCY,  ///< DDR 2750
  AOD_DDR2800_FREQUENCY,  ///< DDR 2800
  AOD_DDR2867_FREQUENCY,  ///< DDR 2867
  AOD_DDR2933_FREQUENCY,  ///< DDR 2933
  AOD_DDR3000_FREQUENCY,  ///< DDR 3000
  AOD_DDR3067_FREQUENCY,  ///< DDR 3067
  AOD_DDR3133_FREQUENCY,  ///< DDR 3133
  AOD_DDR3200_FREQUENCY,  ///< DDR 3200
  AOD_DDR3267_FREQUENCY,  ///< DDR 3267
  AOD_DDR3333_FREQUENCY,  ///< DDR 3333
  AOD_DDR3400_FREQUENCY,  ///< DDR 3400
  AOD_DDR3467_FREQUENCY,  ///< DDR 3467
  AOD_DDR3533_FREQUENCY,  ///< DDR 3533
  AOD_DDR3600_FREQUENCY,  ///< DDR 3600
  AOD_DDR3667_FREQUENCY,  ///< DDR 3667
  AOD_DDR3733_FREQUENCY,  ///< DDR 3733
  AOD_DDR3800_FREQUENCY,  ///< DDR 3800
  AOD_DDR3867_FREQUENCY,  ///< DDR 3867
  AOD_DDR3933_FREQUENCY,  ///< DDR 3933
  AOD_DDR4000_FREQUENCY,  ///< DDR 4000
  AOD_DDR4067_FREQUENCY,  ///< DDR 4067
  AOD_DDR4133_FREQUENCY,  ///< DDR 4133
  AOD_DDR4200_FREQUENCY,  ///< DDR 4200
  AOD_DDR4267_FREQUENCY,  ///< DDR 4267
  AOD_DDR4333_FREQUENCY,  ///< DDR 4333
  AOD_DDR4400_FREQUENCY,  ///< DDR 4400
  AOD_DDR4467_FREQUENCY,  ///< DDR 4467
  AOD_DDR4533_FREQUENCY,  ///< DDR 4533
  AOD_DDR4600_FREQUENCY,  ///< DDR 4600
  AOD_DDR4667_FREQUENCY,  ///< DDR 4667
  AOD_DDR4733_FREQUENCY,  ///< DDR 4733
  AOD_DDR4800_FREQUENCY,  ///< DDR 4800
  AOD_DDR4867_FREQUENCY,  ///< DDR 4867
  AOD_DDR4933_FREQUENCY,  ///< DDR 4933
  AOD_DDR5000_FREQUENCY,  ///< DDR 5000
  AOD_DDR5100_FREQUENCY,  ///< DDR 5100
  AOD_DDR5200_FREQUENCY,  ///< DDR 5200
  AOD_DDR5300_FREQUENCY,  ///< DDR 5300
  AOD_DDR5400_FREQUENCY,  ///< DDR 5400
  AOD_DDR5500_FREQUENCY,  ///< DDR 5500
  AOD_DDR5600_FREQUENCY,  ///< DDR 5600
  AOD_DDR5700_FREQUENCY,  ///< DDR 5700
  AOD_DDR5800_FREQUENCY,  ///< DDR 5800
  AOD_DDR5900_FREQUENCY,  ///< DDR 5900
  AOD_DDR6000_FREQUENCY,  ///< DDR 6000
  AOD_DDR6100_FREQUENCY,  ///< DDR 6100
  AOD_DDR6200_FREQUENCY,  ///< DDR 6200
  AOD_DDR6300_FREQUENCY,  ///< DDR 6300
  AOD_DDR6400_FREQUENCY,  ///< DDR 6400
  AOD_DDR6500_FREQUENCY,  ///< DDR 6500
  AOD_DDR6600_FREQUENCY,  ///< DDR 6600
  AOD_DDR6700_FREQUENCY,  ///< DDR 6700
  AOD_DDR6800_FREQUENCY,  ///< DDR 6800
  AOD_DDR6900_FREQUENCY,  ///< DDR 6900
  AOD_DDR7000_FREQUENCY,  ///< DDR 7000
  AOD_DDR7100_FREQUENCY,  ///< DDR 7100
  AOD_DDR7200_FREQUENCY,  ///< DDR 7200
  AOD_DDR7300_FREQUENCY,  ///< DDR 7300
  AOD_DDR7400_FREQUENCY,  ///< DDR 7400
  AOD_DDR7467_FREQUENCY,  ///< DDR 7467
  AOD_DDR7500_FREQUENCY,  ///< DDR 7500
  AOD_DDR7600_FREQUENCY,  ///< DDR 7600
  AOD_DDR7700_FREQUENCY,  ///< DDR 7700
  AOD_DDR7800_FREQUENCY,  ///< DDR 7800
  AOD_DDR7900_FREQUENCY,  ///< DDR 7900
  AOD_DDR8000_FREQUENCY,  ///< DDR 8000
  AOD_DDR8100_FREQUENCY,  ///< DDR 8100
  AOD_DDR8200_FREQUENCY,  ///< DDR 8200
  AOD_DDR8300_FREQUENCY,  ///< DDR 8300
  AOD_DDR8400_FREQUENCY,  ///< DDR 8400
  AOD_DDR8500_FREQUENCY,  ///< DDR 8500
  AOD_DDR8533_FREQUENCY,  ///< DDR 8533
  AOD_DDR8600_FREQUENCY,  ///< DDR 8600
  AOD_DDR8700_FREQUENCY,  ///< DDR 8700
  AOD_DDR8800_FREQUENCY,  ///< DDR 8800
};
/**
  This function used to get fused information of CCLK_Fmax

  @retval 0               the Smn addres is invalid
  @retval CCLK_Fmax value
**/
UINT16
AodRmbGetCpuBoostMaxFreqBase (
  VOID
  )
{
  UINT32 Value32Lower;

  Value32Lower = 0;
  AodSmnRead (0, 0x5D744, &Value32Lower);
  if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
    return (UINT16)(((Value32Lower >> 4) & 0xFF) * 25);
  }
  return 0;
}

/**
  This function used to get fused information of GFXCCLK_Fmax

  @retval 0               the Smn addres is invalid
  @retval GFXCCLK_Fmax value
**/
UINT16
AodRmbGetGfxBoostMaxFreqBase (
  VOID
  )
{
  UINT32 Value32Lower;

  Value32Lower = 0;
  AodSmnRead (0, 0x5D748, &Value32Lower);
  if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
    return (UINT16)(((Value32Lower >> 5) & 0xFF) * 25);
  }
  return 0;
}


/**
  This function used to update AOD capbility in L"AodSetup" Header

  @param[in]  AodVariable               Points to the L"AodSetup" variable

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
EFIAPI
AodUpdateVariableHeader (
  IN VOID         *AodVariable
  )
{
  AOD_VARIABLE_HEADER                     *AodHeader;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL      *NbioSmuServices;
  UINT32                                  SmuArg[6];
  UINT8                                   CpuOverClocking;
  UINT8                                   MocOverClocking;
  UINT8                                   GfxOverClocking;
  UINT8                                   MemOverClocking;
  UINT8                                   PBOAdvancedOverClocking;
  UINT8                                   GfxPrecisedOverClocking;
  UINT8                                   CpuPrecisedOverClocking;
  EFI_STATUS                              Status;

  AodHeader = (AOD_VARIABLE_HEADER *)AodVariable;
  AodHeader->AodComboFlag   = mAodComboFlag;
  AodHeader->AodVariableSize = sizeof(AOD_CONFIG); // Update Variable Size
  AOD_DEBUG_SOC ((EFI_D_INFO, "Aod AodComboFlag %x Size: %x\n",AodHeader->AodComboFlag, AodHeader->AodVariableSize));
  //
  // Get Aod Over Clocking Capbility
  //
  NbioSmuServices = NULL;
  SmuArg[0] = INVALID_ARG;
  Status = gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, &NbioSmuServices);
  if (!EFI_ERROR (Status)) {

    Status = NbioSmuServices->AmdSmuServiceRequest (NbioSmuServices, 0, SMC_MSG_GetOverclockCap_RMB, SmuArg, SmuArg);
    if (EFI_ERROR (Status)) return EFI_UNSUPPORTED;
    CpuOverClocking = !!(SmuArg[0] & CPU_OVERCLOCKING_CAPABLE);
    MocOverClocking = !!(SmuArg[0] & MOC_CAPABLE);
    GfxOverClocking = !!(SmuArg[0] & GFX_OVERCLOCKING_CAPABLE);
    MemOverClocking = !!(SmuArg[0] & MEM_OVERCLOCKING_CAPABLE);
    PBOAdvancedOverClocking = !!(SmuArg[0] & PBO_ADVANCED_CAPABLE);
    GfxPrecisedOverClocking = !!(SmuArg[0] & GFX_PAD_OVERCLOCKING_CAPABLE);
    CpuPrecisedOverClocking = !!(SmuArg[0] & CPU_PAD_OVERCLOCKING_CAPABLE);
    AOD_DEBUG_SOC ((\
      EFI_D_INFO, \
      "Aod Capbility: SmuArg[0]=%x CPU:%x Moc:%x PBOAdvanced:%x Gfx:%x Mem:%x GfxP&D:%x CPUP&D:%x\n",\
      SmuArg[0],\
      CpuOverClocking,\
      MocOverClocking,\
      PBOAdvancedOverClocking,\
      GfxOverClocking,\
      MemOverClocking,\
      GfxPrecisedOverClocking,\
      CpuPrecisedOverClocking
      ));
  }
  if ((CpuOverClocking != AodHeader->CpuOverClocking) ||\
      (MocOverClocking != AodHeader->MocOverClocking) ||\
      (GfxOverClocking != AodHeader->GfxOverClocking) ||\
      (MemOverClocking != AodHeader->MemOverClocking) ||\
      (PBOAdvancedOverClocking != AodHeader->PBOAdvancedOverClocking) ||\
      (CpuPrecisedOverClocking != AodHeader->CpuPrecisedOverClocking) ||\
      (GfxPrecisedOverClocking != AodHeader->GfxPrecisedOverClocking)) {
    AodHeader->CpuOverClocking = CpuOverClocking;
    AodHeader->GfxOverClocking = GfxOverClocking;
    AodHeader->MocOverClocking = MocOverClocking;
    AodHeader->MemOverClocking = MemOverClocking;
    AodHeader->GfxPrecisedOverClocking = GfxPrecisedOverClocking;
    AodHeader->CpuPrecisedOverClocking = CpuPrecisedOverClocking;
    AodHeader->PBOAdvancedOverClocking = PBOAdvancedOverClocking;
  }
  if ((0 == AodHeader->CpuOverClocking) &&\
      (0 == AodHeader->MocOverClocking) &&\
      (0 == AodHeader->GfxOverClocking) &&\
      (0 == AodHeader->MemOverClocking) &&\
      (0 == AodHeader->PBOAdvancedOverClocking)) {
    return EFI_UNSUPPORTED;
  }
  return Status;
}
/**
  This function used to load L"AodSetup" Default.

  @param[in]  AodVariable               WMI Command Id

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AodLoadDefalutValue (
  IN VOID         *AodVariable
  )
{

  AOD_CONFIG                              *AodConfig;
  UINT16                                  Default,Min,Max,Step;
  AodConfig = (AOD_CONFIG *)AodVariable;
  //
  // Dynamic Items
  //
  ZeroMem (AodConfig->AodPerCoreCurveOptimizerSign, 64*sizeof (UINT8));
  ZeroMem (AodConfig->AodPerCoreCurveOptimizerMagnitude, 64*sizeof (UINT16));
  //Below is generated by script.
  AodConfig->AodCpuPst0Freq = 0;                                     // CPU Frequency
  AodConfig->AodCpuPst0Voltage = 0;                                  // CPU Voltage
  AodConfig->AodCpuSmtCtrl = 1;                                      // SMT Control
  AodConfig->AodGfxOverclockingFreq = 0;                             // GFX Clock Frequency
  AodConfig->AodGfxOverclockingVoltage = 0;                          // GFX Voltage
  AodConfig->AodExpo = 0xff;                                         // EXPO
  AodConfig->AodExpoProfile = 0;                                      // RMP Profile
  AodConfig->AodMemTimingSettingDdr = 0xFF;                          // Active Memory Timing Settings
  AodConfig->AodMemTargetSpeedDdr = 3200;                            // Memory Target Speed
  AodConfig->AodMemTimingTclCtrlDdr = 0;                             // Tcl Ctrl
  AodConfig->AodMemTimingTclDdr = 22;                                // Tcl
  AodConfig->AodMemTimingTrcdCtrlDdr = 0;                            // Trcd Ctrl
  AodConfig->AodMemTimingTrcdDdr = 8;                                // Trcd
  AodConfig->AodMemTimingTrpCtrlDdr = 0;                             // Trp Ctrl
  AodConfig->AodMemTimingTrpDdr = 8;                                 // Trp
  AodConfig->AodMemTimingTrasCtrlDdr = 0;                            // Tras Ctrl
  AodConfig->AodMemTimingTrasDdr = 30;                               // Tras
  AodConfig->AodMemTimingTrcCtrlDdr = 0;                             // Trc Ctrl
  AodConfig->AodMemTimingTrcDdr = 32;                                // Trc
  AodConfig->AodMemTimingTwrCtrlDdr = 0;                             // Twr Ctrl
  AodConfig->AodMemTimingTwrDdr = 48;                                // Twr
  AodConfig->AodMemTimingTrfc1CtrlDdr = 0;                           // Trfc1 Ctrl
  AodConfig->AodMemTimingTrfc1Ddr = 50;                              // Trfc1
  AodConfig->AodMemTimingTrfc2CtrlDdr = 0;                           // Trfc2 Ctrl
  AodConfig->AodMemTimingTrfc2Ddr = 50;                              // Trfc2
  AodConfig->AodMemTimingTrfcSbCtrlDdr = 0;                          // TrfcSb Ctrl
  AodConfig->AodMemTimingTrfcSbDdr = 50;                             // TrfcSb
  AodConfig->AodMemTimingTrtpCtrlDdr = 0;                            // Trtp Ctrl
  AodConfig->AodMemTimingTrtpDdr = 9;                                // Trtp
  AodConfig->AodMemTimingTrrdLCtrlDdr = 0;                           // TrrdL Ctrl
  AodConfig->AodMemTimingTrrdLDdr = 4;                               // TrrdL
  AodConfig->AodMemTimingTrrdSCtrlDdr = 0;                           // TrrdS Ctrl
  AodConfig->AodMemTimingTrrdSDdr = 4;                               // TrrdS
  AodConfig->AodMemTimingTfawCtrlDdr = 0;                            // Tfaw Ctrl
  AodConfig->AodMemTimingTfawDdr = 26;                               // Tfaw
  AodConfig->AodMemTimingTwtrLCtrlDdr = 0;                           // TwtrL Ctrl
  AodConfig->AodMemTimingTwtrLDdr = 8;                               // TwtrL
  AodConfig->AodMemTimingTwtrSCtrlDdr = 0;                           // TwtrS Ctrl
  AodConfig->AodMemTimingTwtrSDdr = 3;                               // TwtrS
  AodConfig->AodMemTimingTrdrdScLCtrlDdr = 0;                        // TrdrdScL Ctrl
  AodConfig->AodMemTimingTrdrdScLDdr = 1;                            // TrdrdScL
  AodConfig->AodMemTimingTrdrdScCtrlDdr = 0;                         // TrdrdSc Ctrl
  AodConfig->AodMemTimingTrdrdScDdr = 1;                             // TrdrdSc
  AodConfig->AodMemTimingTrdrdSdCtrlDdr = 0;                         // TrdrdSd Ctrl
  AodConfig->AodMemTimingTrdrdSdDdr = 3;                             // TrdrdSd
  AodConfig->AodMemTimingTrdrdDdCtrlDdr = 0;                         // TrdrdDd Ctrl
  AodConfig->AodMemTimingTrdrdDdDdr = 3;                             // TrdrdDd
  AodConfig->AodMemTimingTwrwrScLCtrlDdr = 0;                        // TwrwrScL Ctrl
  AodConfig->AodMemTimingTwrwrScLDdr = 1;                            // TwrwrScL
  AodConfig->AodMemTimingTwrwrScCtrlDdr = 0;                         // TwrwrSc Ctrl
  AodConfig->AodMemTimingTwrwrScDdr = 1;                             // TwrwrSc
  AodConfig->AodMemTimingTwrwrSdCtrlDdr = 0;                         // TwrwrSd Ctrl
  AodConfig->AodMemTimingTwrwrSdDdr = 3;                             // TwrwrSd
  AodConfig->AodMemTimingTwrwrDdCtrlDdr = 0;                         // TwrwrDd Ctrl
  AodConfig->AodMemTimingTwrwrDdDdr = 3;                             // TwrwrDd
  AodConfig->AodMemTimingTwrrdCtrlDdr = 0;                           // Twrrd Ctrl
  AodConfig->AodMemTimingTwrrdDdr = 5;                               // Twrrd
  AodConfig->AodMemTimingTrdwrCtrlDdr = 0;                           // Trdwr Ctrl
  AodConfig->AodMemTimingTrdwrDdr = 5;                               // Trdwr
  AodConfig->AodMemDrvStrenCaDdr = 0xFF;                             // Processor CA drive strengths
  AodConfig->AodMemDrvStrenDqDdr = 0xFF;                             // Processor DQ drive strengths
  AodConfig->AodMemDataBusCfgProcOdtDdr = 0xFF;                      // Processor ODT impedance
  AodConfig->AodMemDramDrvStrenDqDdr = 0xFF;                         // Dram DQ drive strengths
  AodConfig->AodMemDramOdtImpedRttNomWrDdr = 0xFF;                   // Dram ODT impedance RTT_NOM_WR
  AodConfig->AodMemDramOdtImpedRttNomRdDdr = 0xff;                   // Dram ODT impedance RTT_NOM_RD
  AodConfig->AodMemDramOdtImpedRttWrDdr = 0xFF;                      // Dram ODT impedance RTT_WR
  AodConfig->AodMemDramOdtImpedRttParkDdr = 0xff;                    // Dram ODT impedance RTT_PARK
  AodConfig->AodMemDramOdtImpedRttParkDqsDdr = 0xff;                 // Dram ODT impedance DQS_RTT_PARK
  AodConfig->AodMemCtrllerPowerDownEnDdr = 0xFF;                     // Power Down Enable
  AodConfig->AodFclkFrequency = 0xFFFF;                              // Infinity Fabric Frequency and Dividers
  AodConfig->AodUclkDiv1Mode = 0xff;                                 // UCLK DIV1 MODE
  AodConfig->AodNbioPBOCtl = 0xff;                                   // Precision Boost Overdrive
  AodConfig->AodNbioPBOAdvancedCtrl = 0xff;                          // PBO Limits
  AodConfig->AodNbioSocketPPTLimit = 0;                              // PPT Limit [mW]
  AodConfig->AodNbioTDCVDDLimit = 0;                                 // TDC Limit [mA]
  AodConfig->AodNbioEDCVDDLimit = 0;                                 // EDC Limit [mA]
  AodConfig->AodNbioTDCSOCLimit = 0;                                 // SOC TDC Limit [mA]
  AodConfig->AodNbioEDCSOCLimit = 0;                                 // SOC EDC Limit [mA]
  AodConfig->AodNbioPBOScalarCtl = 0xff;                             // Precision Boost Overdrive Scalar Ctrl
  AodConfig->AodNbioPBOScalar = 100;                                 // Precision Boost Overdrive Scalar
  AodConfig->AodCpuBoostClock = 0;                                   // CPU Boost Clock Override
  AodConfig->AodFMaxFreqIncrease = 25;                               // Max CPU Boost Clock Override(+)
  AodConfig->AodFMaxFreqDecrease = 25;                               // Max CPU Boost Clock Override(-)
  AodConfig->AodGpuBoostClock = 0;                                   // GPU Boost Clock Override
  AodConfig->AodGfxFMaxFreq = 0;                                     // Max GPU Boost Clock Override
  AodConfig->AodNbiocHTCCtl = 0;                                     // Platform Thermal Throttle Ctrl
  AodConfig->AodNbiocHTC = 0;                                        // Platform Thermal Throttle Limit
  AodConfig->AodCurveOptimizer = 0;                                  // Curve Optimizer
  AodConfig->AodAllCoreCurveOptimizerSign = 0;                       // All Core Curve Optimizer Sign
  AodConfig->AodAllCoreCurveOptimizerMagnitude = 0;                  // All Core Curve Optimizer Magnitude
  AodConfig->AodGfxCurveOptimizer = 0;                               // GFX Curve Optimizer
  AodConfig->AodGfxCurveOptimizerSign = 0;                           // GFX Curve Optimizer Sign
  AodConfig->AodGfxCurveOptimizerMagnitude = 0;                      // GFX Curve Optimizer Magnitude
  AodConfig->AodGnbFclkOcFlag = 0xff;                                // SoC/Uncore OC Mode
  AodConfig->AodSocOverclockingVoltage = 0;                          // SoC Voltage
  AodConfig->AodLN2Mode = 0xff;                                      // LN2 Mode
  AodConfig->AodVddIoCtrl = 0;                                       // VDDIO Ctrl
  AodConfig->AodDimmVdd = 1100;                                      // DIMM VDD Adjust
  AodConfig->AodDimmVddq = 1100;                                     // DIMM VDDQ Adjust
  if (!EFI_ERROR (AodGetOnBoardVoltageInfo (AOD_VOLTAGE_APU_VDDIO, &Default, &Min, &Max, &Step))) {
    // APU VDDIO Adjust
    AodConfig->AodApuVddIo = Default;
  }
  AodConfig->AodVppCtrl = 0;                                         // VPP Ctrl
  AodConfig->AodVppVoltage = 1800;                                   // VPP Adjust
  AodConfig->AodVddMemCtrl = 0;                                      // VDD_MEM Ctrl
  if (!EFI_ERROR (AodGetOnBoardVoltageInfo (AOD_VOLTAGE_VDD_MEM, &Default, &Min, &Max, &Step))) {
    // VDD_MEM Adjust
    AodConfig->AodVddMemAdjust = Default;
  }
  AodConfig->AodMemIntlv = 7;                                        // Memory interleaving
  AodConfig->AodMemIntlvSize = 7;                                    // Memory interleaving size
  AodConfig->AodMaxCcdCount = 0;                                     // Max Ccd Count
  AodConfig->AodMaxCorePerCcd = 0;                                   // Max Core Count Per Ccd
  AodConfig->AodCcdBitMap = 0;                                       // Ccd Bit Map
  AodConfig->AodVddpVoltageCtrl = 0xff;                              // VDDP Voltage Control
  AodConfig->AodVddpVoltage = 0;                                     // VDDP Voltage Adjust
  AodConfig->AodLclkCtrl = 0;                                        // LCLK Frequency Control
  AodConfig->AodMinLclkFreq = 150;                                   // Minimun Frequency
  AodConfig->AodMaxLclkFreq = 2500;                                  // Maximum Frequency
  AodConfig->AodVddMiscCtrl = 0;                                     // VDD Misc Control
  AodConfig->AodVddMiscVoltage = 800;                                // VDD Misc Voltage
  ZeroMem (AodConfig->AodCoreBitMap, AOD_MAX_CCD_COUNT);             // Core Status
  return EFI_SUCCESS;
}

/**
  This function used to update Aod Nvs information, this used by rymaster run up.

  @param[in]  Context               Points to the Aod global NVS

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_NOT_FOUND          The UMC base is not found
**/
EFI_STATUS
EFIAPI
AodUpdateNvs (
  IN  VOID  *Context
  )
{
  EFI_STATUS                           Status;
  UINT8                                MaxDimmsPerChannel;
  UINT8                                MaxChannelsPerSocket;
  AodNvAreaStruct                      *pNvs;
  UINT32                               Index;
  APOB_SPD_STRUCT                      *DimmSmbusInfoPtr;

  pNvs = (AodNvAreaStruct   *)Context;
  // Copy MEM DIMM SPD information from APOB to AOD NV structure
  // Note since size of SPD data is big, we'll not use AodCpuBiosStruct to transfer
  // Instead we'll copy the SPD data to AOD NV strcutre directly
  Status = ApobGetMaxDimmsPerChannel (0, &MaxDimmsPerChannel);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "  APOB_MEM_DIMM_SPD_DATA_TYPE is not found\n"));
  } else {
    pNvs->AodMemDimmSpdData.MaxDimmsPerChannel = MaxDimmsPerChannel;
    AOD_DEBUG ((EFI_D_INFO, "  MaxDimmsPerChannel: %X\n", pNvs->AodMemDimmSpdData.MaxDimmsPerChannel));
  }

  Status = ApobGetMaxChannelsPerSocket (0, &MaxChannelsPerSocket);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "  APOB_MEM_DIMM_SPD_DATA_TYPE is not found\n"));
  } else {
    pNvs->AodMemDimmSpdData.MaxChannelsPerSocket = MaxChannelsPerSocket;
    AOD_DEBUG ((EFI_D_INFO, "  MaxChannelsPerSocket: %X\n", pNvs->AodMemDimmSpdData.MaxChannelsPerSocket));
  }

  // In case APOB service returns incorrect channel and dimm number, we need to make sure it doesn't exceed AOD buffer area
  if (MaxDimmsPerChannel * MaxChannelsPerSocket > AOD_MAX_DIMMS_PER_CHANNEL * AOD_MAX_CHANNELS_PER_SOCKET) {
    AOD_DEBUG ((EFI_D_ERROR, "  ERR: The DIMM * Channel has exceeded the max allowed DIMM number on the system\n"));
    pNvs->AodMemDimmSpdData.MaxDimmsPerChannel = 0;
    pNvs->AodMemDimmSpdData.MaxChannelsPerSocket = 0;
  } else {
    for (Index = 0; Index < (UINT32)(MaxDimmsPerChannel * MaxChannelsPerSocket); Index++) {
      Status = ApobGetDimmSmbusInfo (0, Index, &DimmSmbusInfoPtr);
      if (Status != EFI_SUCCESS) {
        AOD_DEBUG ((EFI_D_ERROR, "  ERR: Failed to get DIMM SPD data [%d]\n", Index));
      } else {
        gBS->CopyMem (
          (VOID *) ((APOB_SPD_STRUCT *) &pNvs->AodMemDimmSpdData.DimmSmbusInfo + Index),
          (VOID *) DimmSmbusInfoPtr,
          sizeof (APOB_SPD_STRUCT)
        );
        AOD_DEBUG ((EFI_D_INFO, "  AodNvAreaBufferPtr->AodMemDimmSpdData.DimmSmbusInfo[%d]: %X\n",\
          Index,\
          (UINTN)((APOB_SPD_STRUCT *) &pNvs->AodMemDimmSpdData.DimmSmbusInfo + Index)));
      }
    }
  }
  return EFI_SUCCESS;
}
/**
  This function used to get memory info from APOB by Id

  @param[in]  ElementId               Element Id

  @retval memory current element value, 0 maybe unknown
**/
UINT32
GetMemGenInfo (
  IN  UINT32    ElementId
  )
{
  UINT16  ElementSizeInByte;
  UINT64  ElementValue = 0;
  ApobGetMemGenInfoElement(0, ElementId, &ElementSizeInByte, &ElementValue);
  switch (ElementSizeInByte) {
    case 1:
      return (UINT8)ElementValue;
    case 2:
      return (UINT16)ElementValue;
    case 4:
      return (UINT32)ElementValue;
    default :
      ASSERT (TRUE);
      break;
  }
  return 0;
}

/**
  This function used to get WMI command id current value

  @param[in]  CmdId               WMI Command Id

  @retval system command Id current value, 0 maybe unknown
**/
UINT32
AodGetWmiCmdCurrentDDR5 (
  IN  UINT32                  CmdId
  )
{
  EFI_STATUS                           Status;
  UINT8                                DieBusNum;
  UINT8                                StartChannelIndex;
  UINT8                                EndChannelIndex;
  UINT8                                ChannelIndex;
  UINT32                               ChSmnChannelBase;
  UINT32                               CtlSmnChannelBase;
  UINT8                                DimmPresent;
  UINT32                               Value;
  UINT32                               RegEbx;
  UINT32                               RegEcx;
  BOOLEAN                              SmtEnable;
  UINT8                                DieNum;
  UINT16                               ApuCurrent, DimmVdd, DimmVddq, DimmVpp, VddMemCurrent;
  UINT8                                RmpMode, RmpProfile, Purpose, PStateIndex;

  Status = EFI_SUCCESS;
  if (mApcbDxeServiceProtocol == NULL) {
    gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);
    mApcbDxeServiceProtocol->ApcbGetToken8 (
                                mApcbDxeServiceProtocol,
                                &Purpose,
                                APCB_TOKEN_UID_MEM_RMP_ENABLE_DDR,
                                &mRmpEnabled);
  }

  if (mRmpEnabled == 1) {
    PStateIndex = 1;
  } else {
    PStateIndex = 0;
  }
  DieBusNum = 0;
  ChSmnChannelBase = 0;
  CtlSmnChannelBase = 0;
  StartChannelIndex = 0;
  EndChannelIndex = 0;
  DimmPresent = 0;
  SmtEnable = FALSE;
  RegEbx = 0;
  RegEcx = 0;


  for (ChannelIndex = 0; ChannelIndex <= 7; ChannelIndex ++) {
    AodSmnRead (DieBusNum, CH_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000) + DRAM_CONFIGURATION(PStateIndex), &Value);
    if (Value & 0x7F) {  // bit [6:0]
      ChSmnChannelBase = CH_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000);
      CtlSmnChannelBase = CTL_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000);
     break;
    }
  }

  if (ChSmnChannelBase == 0) {
    // Should never be here
    return 0;
  }
  for (DimmPresent = 0; DimmPresent < 4; DimmPresent ++) {
    AodSmnRead (DieBusNum, ChSmnChannelBase + 4*DimmPresent, &Value);
    if (Value & 0x1) {
      break;
    }
  }
  if (DimmPresent == 4) {
    // Should never be here
    return 0;
  }
  switch (CmdId) {
    case GET_INTERLEVAVINGCAP:
      return GetMemGenInfo (InterLeaveCapability);
    case SET_INTERLEAVEMODE:
      return GetMemGenInfo (InterLeaveCurrentMode);
    case SET_INTERLEAVESIZE:
      return GetMemGenInfo (InterLeaveSize);
    case SET_SOFTWARE_DOWN_CORE:
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      SmtEnable = ((RegEbx >> 8) & 0xFF) ? TRUE : FALSE;
      DieNum = ((RegEcx >> 8) & 0x7) + 1;
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      return SmtEnable?(UINT16) (RegEcx / 2) : ((UINT16) RegEcx / DieNum);
    case SET_SMTEN:
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      return ((RegEbx >> 8) & 0xFF) ? 1 : 0;
    case GET_OC_DISABLE:
      return (UINT8)PcdGetBool (PcdOcDisable);
    case GET_OC_VOLATAGE_MAX:
      //return VID3_MAX_VOLTAGE - (VID3_TO_MV (PcdGet16 (PcdOcVoltageMax)));
      return VID3_MAX_VOLTAGE;
    case GET_OC_FREQUENCY_MAX:
      return PcdGet16 (PcdOcFrequencyMax);
    case GET_GFX_OC_VOLATAGE_MAX:
      //return VID3_MAX_VOLTAGE - (VID3_TO_MV (PcdGet16 (PcdOcGFXMinVID)));
      return VID3_MAX_VOLTAGE;
    case GET_GFX_OC_FREQUENCY_MAX:
      return PcdGet16 (PcdOcGFXFreqMax);
    case SET_MEM_CLOCK:
      // MCLK
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_CONFIGURATION(PStateIndex), &Value);
      return (UINT16)(Value & 0xfffF);
    case SET_TCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(PStateIndex), &Value);
      return (UINT8)(Value & 0x3F);
    case SET_TRCD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(PStateIndex), &Value);
      return (UINT8)((Value >> 16) & 0x3F);
    case SET_TRP:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING2(PStateIndex), &Value);
      return (UINT8)((Value >> 16) & 0x3F);
    case SET_TRAS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0x7F);
    case SET_TRC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING2(PStateIndex), &Value);
      return (UINT8)(Value & 0xFF);
    case SET_TWR:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING6(PStateIndex), &Value);
      return (UINT8)(Value & 0x7F);  // bit[6:0]
    case SET_TRFC1:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TRFC_TIMING_DIMM0(PStateIndex) + 4*DimmPresent, &Value);
      return (UINT16)(Value & 0xffF);
    case SET_TRFC2:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TRFC_TIMING_DIMM0(PStateIndex) + 4*DimmPresent, &Value);
      return (UINT16)((Value >> 16)& 0xFFF);
    case SET_TRFCSB:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_REF_TIMING(PStateIndex) + 4*DimmPresent, &Value);
      return (UINT16)(Value & 0x7FF);
    case SET_TRTP:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(PStateIndex), &Value);
      return (UINT8)((Value >> 24) & 0x1F);
    case SET_TRRDL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0x1F);
    case SET_TRRDS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(PStateIndex), &Value);
      return (UINT8)(Value & 0x1F);
    case SET_TFAW:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING4(PStateIndex), &Value);
      return (UINT8)(Value & 0x7F);
    case SET_TWTRL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING5(PStateIndex), &Value);
      return (UINT8)((Value >> 16) & 0x7F);
    case SET_TWTRS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING5(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0x1F);
    case SET_TRDRDSCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(PStateIndex), &Value);
      return (UINT8)((Value >> 24) & 0xF);
    case SET_TRDRDSC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(PStateIndex), &Value);
      return (UINT8)((Value >> 16) & 0xF);
    case SET_TRDRDSD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0xF);
    case SET_TRDRDDD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(PStateIndex), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TWRWRSCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(PStateIndex), &Value);
      return (UINT8)((Value >> 24) & 0x3F);
    case SET_TWRWRSC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(PStateIndex), &Value);
      return (UINT8)((Value >> 16) & 0xF);
    case SET_TWRWRSD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0xF);
    case SET_TWRWRDD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(PStateIndex), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TWRRD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING10(PStateIndex), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TRDWR:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING10(PStateIndex), &Value);
      return (UINT8)((Value >> 8) & 0x3F);
    case SET_CADBUSADDRCMDDRVSTREN:
      return GetMemGenInfo (MemBusCfgCaDrv);
    case SET_PROCDATADRIVESTRENGTH:
      return GetMemGenInfo (MemBusCfgDqDrv);
    case SET_PROCODT:
      return GetMemGenInfo (MemBusCfgPOdt);
    case SET_DRAMDATADRIVESTRENGTH:
      return GetMemGenInfo (MemBusCfgDramDrv);
    case SET_RTTNOMWR:
      return GetMemGenInfo (MemBusCfgRttNomWr);
    case SET_RTTNOMRD:
      return GetMemGenInfo (MemBusCfgRttNomRd);
    case SET_RTTWR:
      return GetMemGenInfo (MemBusCfgRttWr);
    case SET_RTTPARK:
      return GetMemGenInfo (MemBusCfgRttPark);
    case SET_RTTPARKDQS:
      return GetMemGenInfo (MemBusCfgDqsRttPark);
    case SET_POWERDWONEN:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_SPAZ_CONTROL, &Value);
      return (UINT8)!!(Value & BIT28);
    case SET_UCLK_DIV1:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_DEBUG_MISC, &Value);
      return (UINT8)!!(Value & BIT3);
    case SET_APU_VDDIO:
      Status = AodGetOnBoardVoltage (AOD_VOLTAGE_APU_VDDIO, &ApuCurrent);
      if (!EFI_ERROR (Status)) return ApuCurrent;
      return 0;
    case SET_VDD_MEM:
      Status = AodGetOnBoardVoltage (AOD_VOLTAGE_VDD_MEM, &VddMemCurrent);
      if (!EFI_ERROR (Status)) return VddMemCurrent;
      return 0;
    case SET_VPP:
      Status = AodDdr5ReadVpp (&DimmVpp);
      if (!EFI_ERROR (Status)) return DimmVpp;
      return 0;
    case SET_VDDIO:
      Status = AodDdr5ReadVddIo (&DimmVdd, &DimmVddq);
      if (!EFI_ERROR (Status)) return DimmVdd;
      return 0;
    case SET_DIMM_VDDQ:
      Status = AodDdr5ReadVddIo (&DimmVdd, &DimmVddq);
      if (!EFI_ERROR (Status)) return DimmVddq;
      return 0;
    case SET_RMP_MODE:
      Purpose = APCB_TYPE_PURPOSE_ADMIN;
      if (mApcbDxeServiceProtocol != NULL) {
        Status = mApcbDxeServiceProtocol->ApcbGetToken8 (
                                            mApcbDxeServiceProtocol,
                                            &Purpose,
                                            APCB_TOKEN_UID_MEM_RMP_ENABLE_DDR,
                                            &RmpMode);
        if (!EFI_ERROR (Status)) return RmpMode;
      }
      break;
    case SET_RMP_PROFILE:
      Purpose = APCB_TYPE_PURPOSE_ADMIN;
      if (mApcbDxeServiceProtocol != NULL) {
        Status = mApcbDxeServiceProtocol->ApcbGetToken8 (
                                            mApcbDxeServiceProtocol,
                                            &Purpose,
                                            APCB_TOKEN_UID_MEM_RMP_PROFILE_INDEX_DDR,
                                            &RmpProfile);
        if (!EFI_ERROR (Status)) return RmpProfile;
      }
      break;
    default:
      break;
  }

  return 0;
}
/**
  This function used to get WMI command id current value

  @param[in]  CmdId               WMI Command Id

  @retval system command Id current value, 0 maybe unknown
**/
UINT32
AodGetWmiCmdCurrentLDDR5 (
  IN  UINT32                  CmdId
  )
{
  EFI_STATUS                           Status;
  UINT8                                DieBusNum;
  UINT8                                StartChannelIndex;
  UINT8                                EndChannelIndex;
  UINT8                                ChannelIndex;
  UINT32                               ChSmnChannelBase;
  UINT32                               CtlSmnChannelBase;
  UINT8                                DimmPresent;
  UINT32                               Value;
  UINT32                               RegEbx;
  UINT32                               RegEcx;
  BOOLEAN                              SmtEnable;
  UINT8                                DieNum;
  UINT8                                WckRatioMode;
  UINT16                               UmcClk;
  UINTN                                Index;
  Status = EFI_SUCCESS;
  DieBusNum = 0;
  ChSmnChannelBase = 0;
  CtlSmnChannelBase = 0;
  StartChannelIndex = 0;
  EndChannelIndex = 0;
  SmtEnable = FALSE;
  RegEbx = 0;
  RegEcx = 0;

  for (ChannelIndex = 0; ChannelIndex <= 7; ChannelIndex ++) {
    AodSmnRead (DieBusNum, CH_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000) + DRAM_CONFIGURATION(0), &Value);
    if (Value & 0x7F) {  // bit [6:0]
      ChSmnChannelBase = CH_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000);
      CtlSmnChannelBase = CTL_SMN_BASE_CH0_MP0 + (ChannelIndex * 0x100000);
     break;
    }
  }

  if (ChSmnChannelBase == 0) {
    // Should never be here
    return 0;
  }
  for (DimmPresent = 0; DimmPresent < 4; DimmPresent ++) {
    AodSmnRead (DieBusNum, ChSmnChannelBase + 4*DimmPresent, &Value);
    if (Value & 0x1) {
      break;
    }
  }
  if (DimmPresent == 4) {
    // Should never be here
    return 0;
  }
  switch (CmdId) {
    case GET_INTERLEVAVINGCAP:
      return GetMemGenInfo (InterLeaveCapability);
    case SET_INTERLEAVEMODE:
      return GetMemGenInfo (InterLeaveCurrentMode);
    case SET_INTERLEAVESIZE:
      return GetMemGenInfo (InterLeaveSize);
    case SET_SOFTWARE_DOWN_CORE:
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      SmtEnable = ((RegEbx >> 8) & 0xFF) ? TRUE : FALSE;
      DieNum = ((RegEcx >> 8) & 0x7) + 1;
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      return SmtEnable?(UINT16) (RegEcx / 2) : ((UINT16) RegEcx / DieNum);
    case SET_SMTEN:
      AsmCpuid (0x8000001E, NULL, &RegEbx, &RegEcx, NULL);
      return ((RegEbx >> 8) & 0xFF) ? 1 : 0;
    case GET_OC_DISABLE:
      return (UINT8)PcdGetBool (PcdOcDisable);
    case GET_OC_VOLATAGE_MAX:
      //return VID3_MAX_VOLTAGE - (VID3_TO_MV (PcdGet16 (PcdOcVoltageMax)));
      return VID3_MAX_VOLTAGE;
    case GET_OC_FREQUENCY_MAX:
      return PcdGet16 (PcdOcFrequencyMax);
    case GET_GFX_OC_VOLATAGE_MAX:
      //return VID3_MAX_VOLTAGE - (VID3_TO_MV (PcdGet16 (PcdOcGFXMinVID)));
      return VID3_MAX_VOLTAGE;
    case GET_GFX_OC_FREQUENCY_MAX:
      return PcdGet16 (PcdOcGFXFreqMax);
    case SET_MEM_CLOCK:
      // MCLK
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_CONFIGURATION(0), &Value);
      UmcClk = (UINT16)(Value & 0xfffF);
      WckRatioMode = (UINT8)(Value >> 26) & 0x3;
      if ((WckRatioMode != 0) && (WckRatioMode != 3)) {
        for (Index = 0; Index < ARRAY_SIZE(MemClkList); Index++) {
          if (MemClkList[Index] >= (UINT16)(WckRatioMode*2*UmcClk)) {
            return MemClkList[Index];
          }
        }
      }
      return UmcClk;
    case SET_TRCPAGE:
      // Set Trcpage
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING7(0), &Value);
      return (UINT16)((Value >> 20) & 0xfff);
    case SET_TCWL:
      // Set Tcwl
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING5(0), &Value);
      return (UINT8)(Value& 0x3f);
    case SET_TCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(0), &Value);
      return (UINT8)(Value & 0x3F);
    case SET_TRCDRD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(0), &Value);
      return (UINT8)((Value >> 16) & 0x3F);
    case SET_TRCDWR:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(0), &Value);
      return (UINT8)((Value >> 24) & 0x3F);
    case SET_TRP:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING2(0), &Value);
      return (UINT8)((Value >> 16) & 0x3F);
    case SET_TRFC1:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TRFC_TIMING_DIMM0(0) + 4*DimmPresent, &Value);
      return (UINT16)(Value & 0xffF);
    case SET_TRAS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING1(0), &Value);
      return (UINT8)((Value >> 8) & 0x7F);
    case SET_TRC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING2(0), &Value);
      return (UINT8)(Value & 0xFF);
    case SET_TRRDL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(0), &Value);
      return (UINT8)((Value >> 8) & 0x1F);
    case SET_TRRDS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(0), &Value);
      return (UINT8)(Value & 0x1F);
    case SET_TFAW:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING4(0), &Value);
      return (UINT8)(Value & 0x7F);
    case SET_TWTRL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING5(0), &Value);
      return (UINT8)((Value >> 16) & 0x7F);
    case SET_TWTRS:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING5(0), &Value);
      return (UINT8)((Value >> 8) & 0x1F);
    case SET_TWR:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING6(0), &Value);
      return (UINT8)(Value & 0x7F);  // bit[6:0]
    case SET_TRDRDSCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(0), &Value);
      return (UINT8)((Value >> 24) & 0xF);
    case SET_TWRWRSCL:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(0), &Value);
      return (UINT8)((Value >> 24) & 0x3F);
    case SET_TRTP:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING3(0), &Value);
      return (UINT8)((Value >> 24) & 0x1F);
    case SET_TCKE:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING21(0), &Value);
      return (UINT8)((Value >> 24) & 0x1F);
    case SET_TRDRDSC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(0), &Value);
      return (UINT8)((Value >> 16) & 0xF);
    case SET_TRDRDDD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(0), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TRDRDSD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING8(0), &Value);
      return (UINT8)((Value >> 8) & 0xF);
    case SET_TWRWRSC:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(0), &Value);
      return (UINT8)((Value >> 16) & 0xF);
    case SET_TWRWRSD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(0), &Value);
      return (UINT8)((Value >> 8) & 0xF);
    case SET_TWRWRDD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING9(0), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TWRRD:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING10(0), &Value);
      return (UINT8)(Value & 0xF);
    case SET_TRDWR:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_TIMING10(0), &Value);
      return (UINT8)((Value >> 8) & 0x3F);
    case SET_POWERDWONEN:
      AodSmnRead (DieBusNum, ChSmnChannelBase + DRAM_SPAZ_CONTROL, &Value);
      return (UINT8)!!(Value & BIT28);
    case SET_ADDRCMDSETUP:
      return GetMemGenInfo (MemCtrllerAddrCmdSetupDddr4Current);
    case SET_CSODTSETUP:
      return GetMemGenInfo (MemCtrllerCsOdtSetupDddr4Current);
    default:
      break;
  }

  return 0;
}

/**
  This function used to build default command list by gAodCmdProtocol

  @param[in]  Context               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_NOT_FOUND          The UMC base is not found
**/
EFI_STATUS
EFIAPI
AodBuildWmiCmdListDDR5 (
  IN  VOID    *Context
  )
{
  EFI_STATUS                       Status;
  AOD_CMD_PROTOCOL                 *pThis;
  UINTN                            Index;
  UINT8                            Count;
  UINT16                           CpuFreqBase;
  UINT16                           GfxFreqBase;
  UINT16                           Default = 0;
  UINT16                           Min = 0;
  UINT16                           Max = 0;
  UINT16                           Step = 0;
  APOB_MEM_RMP_INFO_STRUCT         *Rmp;
  BOOLEAN                          RmpSupported = FALSE;


  if (Context == NULL) return EFI_INVALID_PARAMETER;
  if (!EFI_ERROR (AodGetRmpData ((VOID **)&Rmp))) RmpSupported = TRUE;
  CpuFreqBase = AodRmbGetCpuBoostMaxFreqBase ();
  GfxFreqBase = AodRmbGetGfxBoostMaxFreqBase ();
  pThis = (AOD_CMD_PROTOCOL      *)Context;
  Index = 0;
  for (Index = 0; Index < ARRAY_SIZE (mWmiCmidList); Index ++) {
    if (mWmiCmidList[Index].CmdId == 0) break;
    //
    // If RMP unsupported, don't report RMP related WMIs
    //
    if (!RmpSupported &&
       ((mWmiCmidList[Index].CmdId == SET_RMP_MODE)   ||\
       (mWmiCmidList[Index].CmdId == SET_RMP_PROFILE) ||\
       (mWmiCmidList[Index].CmdId == SET_RMP_PSTATE))) {
      continue;
    }
    if (mWmiCmidList[Index].CmdId == SET_APU_VDDIO) {
      Status = AodGetOnBoardVoltageInfo (AOD_VOLTAGE_APU_VDDIO, &Default, &Min, &Max, &Step);
      if (EFI_ERROR (Status)) {
        continue;
      }
    }
    if (mWmiCmidList[Index].CmdId == SET_VDD_MEM) {
      if ((mAodComboFlag != AOD_RMB_FP7R2_COMBO_FLAG) || \
          EFI_ERROR (AodGetOnBoardVoltageInfo (AOD_VOLTAGE_VDD_MEM, &Default, &Min, &Max, &Step))) {
        continue;
      }
    }
    if ((mWmiCmidList[Index].CmdId == SET_VDD_MISC) && (mAodComboFlag != AOD_RMB_AM5_COMBO_FLAG)) {
      continue;
    }
    Status = pThis->Register (
                      pThis,
                      mWmiCmidList[Index].CmdId,
                      (CHAR8 *)mWmiCmidList[Index].pName
                      );
    AOD_DEBUG_SOC ((EFI_D_ERROR, "Register %a(%08X) Status =%r\n", \
      mWmiCmidList[Index].pName, mWmiCmidList[Index].CmdId, Status));
    if (!EFI_ERROR (Status) && (mWmiCmidList[Index].DataCount != 0)) {
      //
      // Special case to override the value per SOC
      //
      switch (mWmiCmidList[Index].CmdId) {
        case SET_VDD_MEM:
          mWmiCmidList[Index].pValue[0] = Min;
          mWmiCmidList[Index].pValue[1] = Max;
          mWmiCmidList[Index].pValue[2] = Step;
          break;
        case SET_APU_VDDIO:
          mWmiCmidList[Index].pValue[0] = Min;
          mWmiCmidList[Index].pValue[1] = Max;
          mWmiCmidList[Index].pValue[2] = Step;
          break;
        case SET_PPT_LIMIT:
          if (PcdGet32 (PcdFastPptLimit) != 0) {
            mWmiCmidList[Index].pValue[1] = PcdGet32 (PcdFastPptLimit);
          }
          break;
        case SET_TDC_LIMIT:
          if (PcdGet32 (PcdVrmCurrentLimit) != 0) {
            mWmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmCurrentLimit);
          }
          break;
        case SET_EDC_LIMIT:
          if (PcdGet32 (PcdVrmMaximumCurrentLimit) != 0) {
            mWmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmMaximumCurrentLimit);
          }
          break;
        case SET_SOC_TDC_LIMIT:
          if (PcdGet32 (PcdVrmSocCurrentLimit) != 0) {
            mWmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmSocCurrentLimit);
          }
          break;
        case SET_SOC_EDC_LIMIT:
          if (PcdGet32 (PcdVrmSocMaximumCurrentLimit) != 0) {
            mWmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmSocMaximumCurrentLimit);
          }
          break;
        case SET_CCLK_FMAX:
          mWmiCmidList[Index].pValue[0] += CpuFreqBase; // Min
          mWmiCmidList[Index].pValue[1] += CpuFreqBase; // Max
          break;
        case SET_GFXCCLK_FMAX:
          for (Count = 0; Count < mWmiCmidList[Index].DataCount; Count++) {
            mWmiCmidList[Index].pValue[Count] += GfxFreqBase;
          }
          break;
        default:
          break;
      }
      Status = pThis->UpdateValue (
                        pThis,
                        mWmiCmidList[Index].CmdId,
                        mWmiCmidList[Index].Property,
                        mWmiCmidList[Index].DataCount,
                        mWmiCmidList[Index].pValue
                        );
      AOD_DEBUG_SOC ((EFI_D_ERROR, "UpdateValue %a(%08X) Status =%r\n", \
        mWmiCmidList[Index].pName, mWmiCmidList[Index].CmdId, Status));
      //
      // Update Current value
      //
      Status = pThis->UpdateCurrent (
                       pThis,
                       mWmiCmidList[Index].CmdId,
                       AodGetWmiCmdCurrentDDR5 (mWmiCmidList[Index].CmdId)
                       );
      AOD_DEBUG_SOC ((EFI_D_ERROR, "UpdateCurrent %a(%08X) Current = %08X, Status =%r\n", mWmiCmidList[Index].pName,\
        mWmiCmidList[Index].CmdId, AodGetWmiCmdCurrentDDR5 (mWmiCmidList[Index].CmdId), Status));
    }
  }
  return EFI_SUCCESS;
}


/**
  This function used to build default command list by gAodCmdProtocol

  @param[in]  Context               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_NOT_FOUND          The UMC base is not found
**/
EFI_STATUS
EFIAPI
AodBuildWmiCmdListLDDR5 (
  IN  VOID    *Context
  )
{
  EFI_STATUS              Status;
  AOD_CMD_PROTOCOL        *pThis;
  UINTN                   Index;
  UINT8                   Count;
  UINT16                  CpuFreqBase;
  UINT16                  GfxFreqBase;

  if (Context == NULL) return EFI_INVALID_PARAMETER;

  CpuFreqBase = AodRmbGetCpuBoostMaxFreqBase ();
  GfxFreqBase = AodRmbGetGfxBoostMaxFreqBase ();
  pThis = (AOD_CMD_PROTOCOL      *)Context;
  Index = 0;
  for (Index = 0; Index < ARRAY_SIZE (mLDdr5WmiCmidList); Index++) {
    if (mLDdr5WmiCmidList[Index].CmdId == 0) break;

    Status = pThis->Register (
                      pThis,
                      mLDdr5WmiCmidList[Index].CmdId,
                      (CHAR8 *)mLDdr5WmiCmidList[Index].pName
                      );
    AOD_DEBUG_SOC ((EFI_D_ERROR, "Register %a(%08X) Status =%r\n", \
      mLDdr5WmiCmidList[Index].pName, mLDdr5WmiCmidList[Index].CmdId, Status));
    if (!EFI_ERROR (Status) && (mLDdr5WmiCmidList[Index].DataCount != 0)) {
      //
      // Special case to override the value per SOC
      //
      switch (mLDdr5WmiCmidList[Index].CmdId) {
        case SET_PPT_LIMIT:
          if (PcdGet32 (PcdFastPptLimit) != 0) {
            mLDdr5WmiCmidList[Index].pValue[1] = PcdGet32 (PcdFastPptLimit);
          }
          break;
        case SET_TDC_LIMIT:
          if (PcdGet32 (PcdVrmCurrentLimit) != 0) {
            mLDdr5WmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmCurrentLimit);
          }
          break;
        case SET_EDC_LIMIT:
          if (PcdGet32 (PcdVrmMaximumCurrentLimit) != 0) {
            mLDdr5WmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmMaximumCurrentLimit);
          }
          break;
        case SET_SOC_TDC_LIMIT:
          if (PcdGet32 (PcdVrmSocCurrentLimit) != 0) {
            mLDdr5WmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmSocCurrentLimit);
          }
          break;
        case SET_SOC_EDC_LIMIT:
          if (PcdGet32 (PcdVrmSocMaximumCurrentLimit) != 0) {
            mLDdr5WmiCmidList[Index].pValue[1] = PcdGet32 (PcdVrmSocMaximumCurrentLimit);
          }
          break;
        case SET_CCLK_FMAX:
          mLDdr5WmiCmidList[Index].pValue[0] += CpuFreqBase; // Min
          mLDdr5WmiCmidList[Index].pValue[1] += CpuFreqBase; // Max
          break;
        case SET_GFXCCLK_FMAX:
          for (Count = 0; Count < mLDdr5WmiCmidList[Index].DataCount; Count++) {
            mLDdr5WmiCmidList[Index].pValue[Count] += GfxFreqBase;
          }
          break;
        default:
          break;
      }
      Status = pThis->UpdateValue (
                        pThis,
                        mLDdr5WmiCmidList[Index].CmdId,
                        mLDdr5WmiCmidList[Index].Property,
                        mLDdr5WmiCmidList[Index].DataCount,
                        mLDdr5WmiCmidList[Index].pValue
                        );
      AOD_DEBUG_SOC ((EFI_D_ERROR, "UpdateValue %a(%08X) Status =%r\n", \
        mLDdr5WmiCmidList[Index].pName, mLDdr5WmiCmidList[Index].CmdId, Status));
      //
      // Update Current value
      //
      Status = pThis->UpdateCurrent (
                       pThis,
                       mLDdr5WmiCmidList[Index].CmdId,
                       AodGetWmiCmdCurrentLDDR5 (mLDdr5WmiCmidList[Index].CmdId)
                       );
      AOD_DEBUG_SOC ((EFI_D_ERROR, "Update %a(%08X) Current = %08X, Status =%r\n", mLDdr5WmiCmidList[Index].pName,\
        mLDdr5WmiCmidList[Index].CmdId, AodGetWmiCmdCurrentLDDR5 (mLDdr5WmiCmidList[Index].CmdId), Status));
    }
  }
  return EFI_SUCCESS;
}


/**
  This function used to add Hii need callback and front/string package

  @param[in]  Context               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS               The Results is processed successfully.
  @retval EFI_OUT_OF_RESOURCES      Can't Add Aod Hii Front/String Package.
**/
EFI_STATUS
EFIAPI
AodUpdateHiiPackage (
  VOID        *Context
  )
{
  EFI_HII_HANDLE                          HiiHandle = NULL;
  EFI_HANDLE                               DriverHandle;

  DriverHandle = (EFI_HANDLE)Context;

  HiiHandle = HiiAddPackages (
                &mAodFormSetGuid,
                DriverHandle,
                AodSocLibRmbStrings,
                AodHiiBin,
                NULL
                );
  if (HiiHandle == NULL) return EFI_OUT_OF_RESOURCES;
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add HiiPkg @ %x\n",  (UINTN)HiiHandle));

  //
  // We add dynamic hii resouce , don't care if we are run into setup
  // due to we need this data to memory under OS debug later or internal BCT Tool
  //
  AodGenerateCoreManageOptions (HiiHandle);
  AodGenerateApuVddIoOptions (HiiHandle);
  AodGenerateVddMemOptions (HiiHandle);
  GenPerCoreCurveOptimizerOptions (HiiHandle);
  AodGenerateRmpOptions (HiiHandle);
  return EFI_SUCCESS;
}
/**
  This function used to check if LCLK min <= max
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodLCLKCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  AOD_CONFIG                              *AodConfig;
  EFI_INPUT_KEY                           Key;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  AodConfig = (AOD_CONFIG *)IfrData;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if ((KeyValue != KEY_AOD_MIN_LCLK_FREQ) && (KeyValue != KEY_AOD_MAX_LCLK_FREQ)) {
    return EFI_UNSUPPORTED;
  }
  switch (KeyValue) {
    case KEY_AOD_MIN_LCLK_FREQ:
      if (Value->u16 > AodConfig->AodMaxLclkFreq) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Err:Input Min greater than Max Frequency",
          L"Press any key to continue",
          NULL
          );
      }
      break;
    case KEY_AOD_MAX_LCLK_FREQ:
      if (Value->u16 < AodConfig->AodMinLclkFreq) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Err:Input Max less than Min Frequency",
          L"Press any key to continue",
          NULL
          );
      }
      break;
  }
  return EFI_SUCCESS;
}

/**
  This function used to set VDD_MEM (bypass to VDDP)
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodMemoryTargetCallBack (
  IN EFI_HII_HANDLE                       HiiHandle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  AOD_CONFIG                              *AodConfig;

  if (IfrData == NULL) return EFI_UNSUPPORTED;
  AodConfig = (AOD_CONFIG *)IfrData;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_MEMORY_TARGET_SPEED) {
    return EFI_UNSUPPORTED;
  }
  //
  // If DDR speed > DDR 4800, set VDD_MEM to 950mV by default
  //
  if (Value->u16 > 4800) {
    AodConfig->AodVddMemCtrl = 1;
    AodConfig->AodVddMemAdjust = 950;
  } else {
    AodConfig->AodVddMemCtrl = 0;
    AodConfig->AodVddMemAdjust = 780;
  }
  return EFI_SUCCESS;
}
/**
  This function used to add Hii need callback and front/string package

  @param[in]  Context               Points to the gAodCmdProtocol protocol

  @retval EFI_SUCCESS               The Results is processed successfully.
  @retval EFI_OUT_OF_RESOURCES      Can't Add Aod Hii Front/String Package.
**/
EFI_STATUS
EFIAPI
AodUpdateHiiCallback (
  VOID        *Context
  )
{
  EFI_STATUS         Status;
  Status = AodAddSetupCallback (Context, FALSE, AodDownCoreSaveCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodDownCoreSaveCallback Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodDownCoreBitControlCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodDownCoreBitControlCallback Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodDownCoreDiscardCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodDownCoreDiscardCallback Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodDownCoreLoadDefaultCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodDownCoreLoadDefaultCallback Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, TRUE, AodUpdateStringCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodUpdateStringCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodDimmVddIoCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodDimmVddIoCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodApuVddIoCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodApuVddIoCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodVddIoCtrlCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodVddIoCtrlCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodLCLKCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodLCLKCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodApuVddMemCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodApuVddMemCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodExpoProfileSelectCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodExpoProfileSelectCallback Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodMemoryTargetCallBack);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodMemoryTargetCallBack Status %r\n",  Status));
  Status = AodAddSetupCallback (Context, FALSE, AodExpoCallback);
  AOD_DEBUG_SOC ((EFI_D_INFO, "  Add AodExpoCallback Status %r\n",  Status));
  return EFI_SUCCESS;
}

/**
  This function used to set the dispatch for dispatch init fucntion

  @param[in]  AodDispatchFlag               Aod Dispatch Flag

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
EFIAPI
AodDispatchComboFlagInit (
  OUT UINT16         *AodDispatchFlag
  )
{
  switch (mAodComboFlag) {
    case AOD_RMB_AM5_COMBO_FLAG:
    case AOD_RMB_FP7R2_COMBO_FLAG:
      *AodDispatchFlag = AOD_RMB_DISPATCH_INIT_DDR5_FLAG;
      break;
    case AOD_RMB_FP7_COMBO_FLAG:
      *AodDispatchFlag = AOD_RMB_DISPATCH_INIT_LDDR5_FLAG;
      break;
    default:
      *AodDispatchFlag = 0;
      break;
  }
  AOD_DEBUG_SOC ((DEBUG_INFO, "AodComboFlag = 0x%x AodDispatchFlag = 0x%x\n", mAodComboFlag, *AodDispatchFlag));
  return EFI_SUCCESS;
}

/**
  This function used to set the dispatch for dispatch init fucntion

  @param[in]  AodVariable               Pointer to L"AodSetup"

  @retval EFI_SUCCESS            The Results is processed successfully.

**/
EFI_STATUS
EFIAPI
AodConfigDebug (
  IN  VOID         *AodVariable
  )
{
  AOD_CONFIG          *AodConfig;
  UINTN               Index;
  if (AodVariable == NULL) return EFI_INVALID_PARAMETER;

  AodConfig = (AOD_CONFIG *)AodVariable;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dump setup variable start\n"));
  AOD_DEBUG_SOC ((EFI_D_INFO, "CPU Frequency                            = 0x%x \n", \
    AodConfig->AodCpuPst0Freq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "CPU Voltage                              = 0x%x \n", \
    AodConfig->AodCpuPst0Voltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Max Ccd Count                            = 0x%x \n", \
    AodConfig->AodMaxCcdCount));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Max Core Count Per Ccd                   = 0x%x \n", \
    AodConfig->AodMaxCorePerCcd));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Ccd BIT Map                              = 0x%x \n", \
    AodConfig->AodCcdBitMap));
  for (Index = 0; Index < AodConfig->AodMaxCcdCount; Index ++) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Core BIT Map[CCD%d]                      = 0x%x \n", \
      Index, AodConfig->AodCoreBitMap[Index]));
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "SMT Control                              = 0x%x \n", \
    AodConfig->AodCpuSmtCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GFX Clock Frequency                      = 0x%x \n", \
    AodConfig->AodGfxOverclockingFreq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GFX Voltage                              = 0x%x \n", \
    AodConfig->AodGfxOverclockingVoltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "RMP Mode                                 = 0x%x \n", \
    AodConfig->AodExpo));
  AOD_DEBUG_SOC ((EFI_D_INFO, "RMP  Profile                             = 0x%x \n", \
    AodConfig->AodExpoProfile));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Active Memory Timing Settings            = 0x%x \n", \
    AodConfig->AodMemTimingSettingDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Memory Target Speed                      = 0x%x \n", \
    AodConfig->AodMemTargetSpeedDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tcl Ctrl                                 = 0x%x \n", \
    AodConfig->AodMemTimingTclCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tcl                                      = 0x%x \n", \
    AodConfig->AodMemTimingTclDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trcd Ctrl                                = 0x%x \n", \
    AodConfig->AodMemTimingTrcdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trcd                                     = 0x%x \n", \
    AodConfig->AodMemTimingTrcdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trp Ctrl                                 = 0x%x \n", \
    AodConfig->AodMemTimingTrpCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trp                                      = 0x%x \n", \
    AodConfig->AodMemTimingTrpDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tras Ctrl                                = 0x%x \n", \
    AodConfig->AodMemTimingTrasCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tras                                     = 0x%x \n", \
    AodConfig->AodMemTimingTrasDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trc Ctrl                                 = 0x%x \n", \
    AodConfig->AodMemTimingTrcCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trc                                      = 0x%x \n", \
    AodConfig->AodMemTimingTrcDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Twr Ctrl                                 = 0x%x \n", \
    AodConfig->AodMemTimingTwrCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Twr                                      = 0x%x \n", \
    AodConfig->AodMemTimingTwrDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trfc1 Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTrfc1CtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trfc1                                    = 0x%x \n", \
    AodConfig->AodMemTimingTrfc1Ddr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trfc2 Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTrfc2CtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trfc2                                    = 0x%x \n", \
    AodConfig->AodMemTimingTrfc2Ddr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrfcSb Ctrl                              = 0x%x \n", \
    AodConfig->AodMemTimingTrfcSbCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrfcSb                                   = 0x%x \n", \
    AodConfig->AodMemTimingTrfcSbDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trtp Ctrl                                = 0x%x \n", \
    AodConfig->AodMemTimingTrtpCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trtp                                     = 0x%x \n", \
    AodConfig->AodMemTimingTrtpDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrrdL Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTrrdLCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrrdL                                    = 0x%x \n", \
    AodConfig->AodMemTimingTrrdLDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrrdS Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTrrdSCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrrdS                                    = 0x%x \n", \
    AodConfig->AodMemTimingTrrdSDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tfaw Ctrl                                = 0x%x \n", \
    AodConfig->AodMemTimingTfawCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Tfaw                                     = 0x%x \n", \
    AodConfig->AodMemTimingTfawDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwtrL Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTwtrLCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwtrL                                    = 0x%x \n", \
    AodConfig->AodMemTimingTwtrLDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwtrS Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTwtrSCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwtrS                                    = 0x%x \n", \
    AodConfig->AodMemTimingTwtrSDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdScL Ctrl                            = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdScLCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdScL                                 = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdScLDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdSc Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdScCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdSc                                  = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdScDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdSd Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdSdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdSd                                  = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdSdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdDd Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdDdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TrdrdDd                                  = 0x%x \n", \
    AodConfig->AodMemTimingTrdrdDdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrScL Ctrl                            = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrScLCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrScL                                 = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrScLDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrSc Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrScCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrSc                                  = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrScDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrSd Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrSdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrSd                                  = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrSdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrDd Ctrl                             = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrDdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TwrwrDd                                  = 0x%x \n", \
    AodConfig->AodMemTimingTwrwrDdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Twrrd Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTwrrdCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Twrrd                                    = 0x%x \n", \
    AodConfig->AodMemTimingTwrrdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trdwr Ctrl                               = 0x%x \n", \
    AodConfig->AodMemTimingTrdwrCtrlDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Trdwr                                    = 0x%x \n", \
    AodConfig->AodMemTimingTrdwrDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Processor CA drive strengths             = 0x%x\n", \
    AodConfig->AodMemDrvStrenCaDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Processor DQ drive strengths             = 0x%x\n", \
    AodConfig->AodMemDrvStrenDqDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Processor ODT impedance                  = 0x%x\n", \
    AodConfig->AodMemDataBusCfgProcOdtDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram DQ drive strengths                  = 0x%x\n", \
    AodConfig->AodMemDramDrvStrenDqDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram ODT impedance RTT_NOM_WR            = 0x%x\n", \
    AodConfig->AodMemDramOdtImpedRttNomWrDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram ODT impedance RTT_NOM_RD            = 0x%x\n", \
    AodConfig->AodMemDramOdtImpedRttNomRdDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram ODT impedance RTT_WR                = 0x%x\n", \
    AodConfig->AodMemDramOdtImpedRttWrDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram ODT impedance RTT_PARK              = 0x%x\n", \
    AodConfig->AodMemDramOdtImpedRttParkDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dram ODT impedance DQS_RTT_PARK          = 0x%x\n", \
    AodConfig->AodMemDramOdtImpedRttParkDqsDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Power Down Enable                        = 0x%x \n", \
    AodConfig->AodMemCtrllerPowerDownEnDdr));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Infinity Fabric Frequency and Dividers   = 0x%x \n", \
    AodConfig->AodFclkFrequency));
  AOD_DEBUG_SOC ((EFI_D_INFO, "UCLK DIV1 MODE                           = 0x%x \n", \
    AodConfig->AodUclkDiv1Mode));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Precision Boost Overdrive                = 0x%x \n", \
    AodConfig->AodNbioPBOCtl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "PBO Limits                               = 0x%x \n", \
    AodConfig->AodNbioPBOAdvancedCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "PPT Limit [mW]                           = 0x%x \n", \
    AodConfig->AodNbioSocketPPTLimit));
  AOD_DEBUG_SOC ((EFI_D_INFO, "TDC Limit [mA]                           = 0x%x \n", \
    AodConfig->AodNbioTDCVDDLimit));
  AOD_DEBUG_SOC ((EFI_D_INFO, "EDC Limit [mA]                           = 0x%x \n", \
    AodConfig->AodNbioEDCVDDLimit));
  AOD_DEBUG_SOC ((EFI_D_INFO, "SOC TDC Limit [mA]                       = 0x%x \n", \
    AodConfig->AodNbioTDCSOCLimit));
  AOD_DEBUG_SOC ((EFI_D_INFO, "SOC EDC Limit [mA]                       = 0x%x \n", \
    AodConfig->AodNbioEDCSOCLimit));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Precision Boost Overdrive Scalar Ctrl    = 0x%x \n", \
    AodConfig->AodNbioPBOScalarCtl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Precision Boost Overdrive Scalar         = 0x%x \n", \
    AodConfig->AodNbioPBOScalar));
  AOD_DEBUG_SOC ((EFI_D_INFO, "CPU Boost Clock Override                 = 0x%x \n", \
    AodConfig->AodCpuBoostClock));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Max CPU Boost Clock Override(+)          = 0x%x \n", \
    AodConfig->AodFMaxFreqIncrease));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Max CPU Boost Clock Override(-)          = 0x%x \n", \
    AodConfig->AodFMaxFreqDecrease));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GPU Boost Clock Override                 = 0x%x \n", \
    AodConfig->AodGpuBoostClock));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Max GPU Boost Clock Override             = 0x%x \n", \
    AodConfig->AodGfxFMaxFreq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Platform Thermal Throttle Ctrl           = 0x%x \n", \
    AodConfig->AodNbiocHTCCtl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Platform Thermal Throttle Limit          = 0x%x \n", \
    AodConfig->AodNbiocHTC));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Curve Optimizer                          = 0x%x \n", \
    AodConfig->AodCurveOptimizer));
  AOD_DEBUG_SOC ((EFI_D_INFO, "All Core Curve Optimizer Sign            = 0x%x \n", \
    AodConfig->AodAllCoreCurveOptimizerSign));
  AOD_DEBUG_SOC ((EFI_D_INFO, "All Core Curve Optimizer Magnitude       = 0x%x \n", \
    AodConfig->AodAllCoreCurveOptimizerMagnitude));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GFX Curve Optimizer                      = 0x%x \n", \
    AodConfig->AodGfxCurveOptimizer));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GFX Curve Optimizer Sign                 = 0x%x \n", \
    AodConfig->AodGfxCurveOptimizerSign));
  AOD_DEBUG_SOC ((EFI_D_INFO, "GFX Curve Optimizer Magnitude            = 0x%x \n", \
    AodConfig->AodGfxCurveOptimizerMagnitude));
  for (Index = 0; Index < 64; Index ++) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Per Core Curve Optimizer Sign[c%d]       = 0x%x \n", \
      Index, AodConfig->AodPerCoreCurveOptimizerSign[Index]));
    AOD_DEBUG_SOC ((EFI_D_INFO, "Per Core Curve Optimizer Magnitude[c%d]  = 0x%x \n", \
      Index, AodConfig->AodPerCoreCurveOptimizerMagnitude[Index]));
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "SoC/Uncore OC Mode                       = 0x%x \n", \
    AodConfig->AodGnbFclkOcFlag));
  AOD_DEBUG_SOC ((EFI_D_INFO, "SoC Voltage                              = 0x%x \n", \
    AodConfig->AodSocOverclockingVoltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "LN2 Mode                                 = 0x%x \n", \
    AodConfig->AodLN2Mode));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDDIO Ctrl                               = 0x%x \n", \
    AodConfig->AodVddIoCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "DIMM VDD Adjust                          = 0x%x \n", \
    AodConfig->AodDimmVdd));
  AOD_DEBUG_SOC ((EFI_D_INFO, "DIMM VDDQ Adjust                         = 0x%x \n", \
    AodConfig->AodDimmVddq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "APU VDDIO Adjust                         = 0x%x \n", \
    AodConfig->AodApuVddIo));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VPP Ctrl                                 = 0x%x \n", \
    AodConfig->AodVppCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VPP Adjust                               = 0x%x \n", \
    AodConfig->AodVppVoltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDD_MEM Control                          = 0x%x \n", \
    AodConfig->AodVddMemCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDD_MEM Adjust                           = 0x%x \n", \
    AodConfig->AodVddMemAdjust));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDDP Voltage Control                     = 0x%x \n", \
    AodConfig->AodVddpVoltageCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDDP Voltage Adjust                      = 0x%x \n", \
    AodConfig->AodVddpVoltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "LCLK Frequency Control                   = 0x%x \n", \
    AodConfig->AodLclkCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Minimun Frequency                        = 0x%x \n", \
    AodConfig->AodMinLclkFreq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Maximum Frequency                        = 0x%x \n", \
    AodConfig->AodMaxLclkFreq));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDD Misc Control                         = 0x%x \n", \
    AodConfig->AodVddMiscCtrl));
  AOD_DEBUG_SOC ((EFI_D_INFO, "VDD Misc Voltage                         = 0x%x \n", \
    AodConfig->AodVddMiscVoltage));
  AOD_DEBUG_SOC ((EFI_D_INFO, "Dump setup variable End\n"));
 return EFI_SUCCESS;
}
///
/// Aod Rmb Dxe init table
///
AOD_INIT_TABLE mRmbAodDxeInit [] = {
  { 0, AOD_DISPATCH_INIT,         AodDispatchComboFlagInit},
  { 0, AOD_CONFIG_LOAD_DEFAULT,   AodLoadDefalutValue},
  { 0, AOD_UPDATE_VARIABLE,       AodUpdateVariableHeader},
  { 0, AOD_UPDATE_NVS,            AodUpdateNvs},
  { 0, AOD_UPDATE_NVS,            AodRMPUpdateNvs},
  { 0, AOD_CONFIG_DEBUG,          AodConfigDebug},
  { AOD_RMB_DISPATCH_INIT_DDR5_FLAG,  AOD_BUILD_WMI_CMD, AodBuildWmiCmdListDDR5},
  { AOD_RMB_DISPATCH_INIT_LDDR5_FLAG, AOD_BUILD_WMI_CMD, AodBuildWmiCmdListLDDR5},
  { 0, AOD_UPDATE_HIIRESOURCE,    AodUpdateHiiPackage},
  { 0, AOD_UPDATE_HII_CALLBACK,   AodUpdateHiiCallback},
  { 0, 0, NULL}
};
/**
  The library constructuor.

  The function does the necessary initialization work for this library instance.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
DxeAodRmbConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SOC_ID_STRUCT       SocId;
  SOC_ID_STRUCT       SocId2;
  SOC_ID_STRUCT       SocId3;
  SOC_ID_STRUCT       SocId4;
  SOC_ID_STRUCT       SocId5;
  SOC_ID_STRUCT       SocId6;
  // Check Hardware Identification RMB AM5
  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_AM5;
  SocId6.SocFamilyID = F19_RMB_RAW_ID;
  SocId6.PackageType = ZEN3_PKG_AM5_B0;
  // Check Hardware Identification RMB Fp7R2
  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = ZEN3_PKG_FP7r2;
  // Check Hardware Identification RMB Fp7R2 B0
  SocId3.SocFamilyID = F19_RMB_RAW_ID;
  SocId3.PackageType = ZEN3_PKG_FP7r2_B0;
  // Check Hardware Identification RMB Fp7R2
  SocId4.SocFamilyID = F19_RMB_RAW_ID;
  SocId4.PackageType = ZEN3_PKG_FP7;
  // Check Hardware Identification RMB Fp7R2 B0
  SocId5.SocFamilyID = F19_RMB_RAW_ID;
  SocId5.PackageType = ZEN3_PKG_FP7_B0;
  if (SocHardwareIdentificationCheck (&SocId) || SocHardwareIdentificationCheck (&SocId6)) {
    mAodComboFlag = AOD_RMB_AM5_COMBO_FLAG;
    AOD_DEBUG_SOC ((DEBUG_INFO, "AodDispatchFlag = 0x%x\n", mAodComboFlag));
    return AodAddInitTable (mRmbAodDxeInit);
  }
  if (SocHardwareIdentificationCheck (&SocId2) || SocHardwareIdentificationCheck (&SocId3)) {
    mAodComboFlag = AOD_RMB_FP7R2_COMBO_FLAG;
    AOD_DEBUG_SOC ((DEBUG_INFO, "AodDispatchFlag = 0x%x\n", mAodComboFlag));
    return AodAddInitTable (mRmbAodDxeInit);
  }
  if (SocHardwareIdentificationCheck (&SocId4) || SocHardwareIdentificationCheck (&SocId5)) {
    AOD_DEBUG_SOC ((DEBUG_INFO, "AodDispatchFlag = 0x%x\n", mAodComboFlag));
    mAodComboFlag = AOD_RMB_FP7_COMBO_FLAG;
    return AodAddInitTable (mRmbAodDxeInit);
  }
  return EFI_SUCCESS;
}
