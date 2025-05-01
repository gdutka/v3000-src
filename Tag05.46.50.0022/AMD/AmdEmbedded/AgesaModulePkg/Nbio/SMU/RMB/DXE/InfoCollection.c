/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Filecode.h>
#include "SmuV13Dxe.h"
#include <Library/AmdBaseLib.h>
#include <GnbDxio.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/UefiLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/GnbPciLib.h>
#include <Library/PcdLib.h>
#include "smu13_bios_if_RMB.h"
#include "smu13_RMB.h"
#include "Smu13FeaturesEnable.h"
#include <Library/BaseMemoryLib.h>
#include <Library/AmdStbLib.h>
#include <AmdPcieComplex.h>

#define FILECODE        NBIO_SMU_RMB_DXE_INFOCOLLECTION_FILECODE
#define CPPCPERFLIMITMAXRANGE 100
#define CPPCEPPMAXRANGE 100

extern Smu13BiosIfTable_t PPTableDefault;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define LC_OTHER_SIDE_SUPPORTS_GEN4 (1 << 24)
#define LC_OTHER_SIDE_SUPPORTS_GEN3 (1 << 22)
#define LC_OTHER_SIDE_SUPPORTS_GEN2 (1 << 20)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  PcieLaneMappingTable_t      *SmuMappingData;
  UINT8                       PortCount[4][2];
  UINT8                       SocketNumber;
} LANE_MAPPING_CTL_STRUCT;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
CppcPopulateApicIdsInPPTable (
  IN Smu13BiosIfTable_t  *PpTable
  );

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
AGESA_STATUS
STATIC
PcieLinkStatusCallback (
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  PcieLaneMappingTable_t      *SmuMappingData;
  UINT8                       LinkWidth;
  UINT32                      Value;
  LANE_MAPPING_CTL_STRUCT     *MappingStruct;
  UINT8                       PortIndex;

  Wrapper = PcieConfigGetParentWrapper(Engine);
  GnbHandle = (GNB_HANDLE *)PcieConfigGetParentSilicon(Engine);
  MappingStruct = (LANE_MAPPING_CTL_STRUCT *) Buffer;
  if ((Wrapper->WrapId < 2) && (GnbHandle->SocketId == MappingStruct->SocketNumber)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcieLinkStatusCallback for Socket %d, RBIndex %d, Wrapper %d, Port %d\n", GnbHandle->SocketId, GnbHandle->RBIndex, Wrapper->WrapId, Engine->Type.Port.PortId);
    SmuMappingData = MappingStruct->SmuMappingData;
    PortIndex = MappingStruct->PortCount[GnbHandle->RBIndex][Wrapper->WrapId];

    if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
        (Engine->Type.Port.PortData.LinkHotplug != HotplugInboard))) {

      if (Engine->EngineData.EndLane > Engine->EngineData.StartLane) {
        LinkWidth = (UINT8) ((Engine->EngineData.EndLane - Engine->EngineData.StartLane) + 1);
      } else {
        LinkWidth = (UINT8) ((Engine->EngineData.StartLane - Engine->EngineData.EndLane) + 1);
      }
      SmuMappingData->PCIe_LaneWidth[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = LinkWidth;

      if ((Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
          (Engine->Type.Port.PortData.LinkHotplug != HotplugInboard)) {
        if (Engine->Type.Port.PortData.LinkSpeedCapability == DxioGenMaxSupported) {
          SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = DxioGen4;
        } else {
          SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = Engine->Type.Port.PortData.LinkSpeedCapability;
        }
      } else {
        SmnRegisterRead (GnbHandle->Address.Address.Bus,
                         PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS),
                         &Value
                         );
        SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = DxioGen1;
        if ((Value & LC_OTHER_SIDE_SUPPORTS_GEN2) != 0) {
          SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = DxioGen2;
        }
        if ((Value & LC_OTHER_SIDE_SUPPORTS_GEN3) != 0) {
          SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = DxioGen3;
        }
        if ((Value & LC_OTHER_SIDE_SUPPORTS_GEN4) != 0) {
          SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex] = DxioGen4;
        }
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "In Linkspeed %d, LinkWidth %d on Entry %d\n",
                       SmuMappingData->PCIe_GenSpeed[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex],
                       SmuMappingData->PCIe_LaneWidth[GnbHandle->RBIndex][Wrapper->WrapId][PortIndex],
                       PortIndex
                       );
      PortIndex++;
      MappingStruct->PortCount[GnbHandle->RBIndex][Wrapper->WrapId] = PortIndex;
    }
  }
  return AGESA_SUCCESS;
}
/**
  Populate SMU table for LCLK DPM


  @param GnbHandle        First Instance Instance of GNB handle
  @param SmuMappingData   Pointer to PcieLaneMappingTable_t structure (memory must be pre allocated)
**/
VOID
PopulatePcieLinkStatusTable (
  GNB_HANDLE                           *GnbHandle,
  PcieLaneMappingTable_t               *SmuMappingData
  )
{
  LANE_MAPPING_CTL_STRUCT            MappingStruct;

  // Default for all values is 0 (no device)
  ZeroMem (SmuMappingData, sizeof (PcieLaneMappingTable_t));
  ZeroMem (&MappingStruct, sizeof (LANE_MAPPING_CTL_STRUCT));
  MappingStruct.SmuMappingData = SmuMappingData;
  MappingStruct.SocketNumber = GnbHandle->SocketId;
  PcieConfigRunProcForAllEngines (DESCRIPTOR_ALL_ENGINES, PcieLinkStatusCallback, (VOID *) &MappingStruct, PcieConfigGetPlatform(GnbHandle));
}

/**
  Populate PPTable with values from PCDs


  @param PPTable Pointer to a PPTable which must be pre allocated.
**/
VOID
PopulatePPTable (
  Smu13BiosIfTable_t    *PPTable
  )
{
  ZeroMem (PPTable, sizeof (Smu13BiosIfTable_t));
  CopyMem (PPTable, &PPTableDefault, sizeof (Smu13BiosIfTable_t));

  PPTable->SUSTAINED_POWER_LIMIT = PcdGet32 (PcdSustainedPowerLimit);
  PPTable->VRM_SOC_CURRENT_LIMIT = PcdGet32 (PcdVrmSocCurrentLimit);
  PPTable->VRM_VDD_MAXIMUM_CURRENT_LIMIT = PcdGet32 (PcdVrmMaximumCurrentLimit);
  PPTable->VRM_SOC_MAXIMUM_CURRENT_LIMIT = PcdGet32 (PcdVrmSocMaximumCurrentLimit);
  PPTable->PROCHOT_L_DEASSERTION_RAMP_TIME = PcdGet32 (PcdProchotlDeassertionRampTime);
  PPTable->SYSTEM_CONFIG = PcdGet8 (PcdCfgSystemConfiguration);

  //FAN TABLE
  PPTable->FanTable_Override = PcdGet8 (PcdFanTableOverride);  //Fan Table Control Options
  if (PPTable->FanTable_Override != 0 ) {
    PPTable->FanTable_Hysteresis = PcdGet8 (PcdFanTableHysteresis);
    PPTable->FanTable_TempLow = PcdGet8 (PcdFanTableTempLow);
    PPTable->FanTable_TempMed = PcdGet8 (PcdFanTableTempMed);
    PPTable->FanTable_TempHigh = PcdGet8 (PcdFanTableTempHigh);
    PPTable->FanTable_TempCritical = PcdGet8 (PcdFanTableTempCritical);
    PPTable->FanTable_PwmLow = PcdGet8 (PcdFanTablePwmLow);
    PPTable->FanTable_PwmMed = PcdGet8 (PcdFanTablePwmMed);
    PPTable->FanTable_PwmHigh = PcdGet8 (PcdFanTablePwmHigh);
    PPTable->FanTable_PwmFreq = PcdGet8 (PcdFanTablePwmFreq);
    PPTable->FanTable_Polarity = PcdGet8 (PcdFanTablePolarity);
  }

  //CBS debug options
  PPTable->CoreDldoPsmMargin = PcdGet8 (PcdDldoPsmMargin);
  PPTable->VddcrVddVoltageMargin = (UINT16) PcdGet32 (PcdVddcrCpuVoltageMargin);
  PPTable->VddcrSocVoltageMargin = (UINT16) PcdGet32 (PcdVddcrSocVoltageMargin);

  //TELEMETRY CALIBRATION
  PPTable->TelemetryVddcrVddSlope = PcdGet32 (PcdTelemetry_VddcrVddfull_Scale_Current);
  PPTable->TelemetryVddcrVddOffset = PcdGet32 (PcdTelemetry_VddcrVddOffset);
  PPTable->TelemetryVddcrSocSlope = PcdGet32 (PcdTelemetry_VddcrSocfull_Scale_Current);
  PPTable->TelemetryVddcrSocOffset = PcdGet32 (PcdTelemetry_VddcrSocOffset);
  PPTable->TelemetryVddcrSrSlope = PcdGet32 (PcdTelemetry_VddcrSrfull_Scale_Current);
  PPTable->TelemetryVddcrSrOffset = PcdGet32 (PcdTelemetry_VddcrSrOffset);

  //// OVERCLOCKING CONFIG
  PPTable->OC_DISABLE = (UINT8) PcdGetBool (PcdOcDisable);
  if (PPTable->OC_DISABLE == 0) {
    PPTable->OC_MINVID = (UINT8) PcdGet16 (PcdOcVoltageMax);
    PPTable->OC_FREQMAX = PcdGet16 (PcdOcFrequencyMax);
    PPTable->ForceGfxclkFrequency = PcdGet16 (PcdForceGfxclkFrequency);
    PPTable->ForceVddcrSocVoltage = PcdGet16 (PcdForceVddcrSocVoltage);
    PPTable->ForceVddcrVddVoltage = PcdGet16 (PcdForceVddcrCpuVoltage);
  }

  //CCLK FREQUENCY FORCE
  PPTable->CclkFmaxOverride = (UINT16) PcdGet32 (PcdFMaxFrequency);
  PPTable->iTempPart = (UINT8) PcdGet8 (Pcditemp);
  PPTable->GfxclkFmaxOverride = (UINT16) PcdGet16 (PcdGfxclkFmaxOverride);
  PPTable->CclkFminOverride = PcdGet16 (PcdCclkFminOverride);

  // STT parameters/co-efficients
  PPTable->SYSTEM_TEMPERATURE_TRACKING = PcdGet8 (PcdSttEnable);
  if (PcdGet8 (PcdSttEnable) == 0x1) {
    PPTable->STT_PCB_SENSOR_COUNT = PcdGet8 (PcdSttPcbSensorCount);
    PPTable->STT_MIN_POWER_LIMIT = PcdGet16 (PcdSttMinPowerLimit);
    PPTable->STT_M1 = PcdGet16 (PcdSttM1);
    PPTable->STT_M2 = PcdGet16 (PcdSttM2);
    PPTable->STT_M3 = PcdGet16 (PcdSttM3);
    PPTable->STT_M4 = PcdGet16 (PcdSttM4);
    PPTable->STT_M5 = PcdGet16 (PcdSttM5);
    PPTable->STT_M6 = PcdGet16 (PcdSttM6);
    PPTable->STT_C_APU = PcdGet16 (PcdSttCApu);
    PPTable->STT_C_HS2 = PcdGet16 (PcdSttCGpu);
    PPTable->STT_ALPHA_APU = PcdGet16 (PcdSttAlphaApu);
    PPTable->STT_ALPHA_HS2 = PcdGet16 (PcdSttAlphaGpu);
    PPTable->STT_SKIN_TEMPERATURE_LIMIT_APU = PcdGet16 (PcdSttSkinTemperatureLimitApu);
    PPTable->STT_SKIN_TEMPERATURE_LIMIT_HS2 = PcdGet16 (PcdSttSkinTemperatureLimitGpu);
    PPTable->STT_ERROR_COEFF = PcdGet16 (PcdSttErrorCoeff);
    PPTable->STT_ERROR_RATE_COEFF = PcdGet16 (PcdSttErrorRateCoeff);
  }

  // STAPM Control
  PPTable->STAPM_BOOST_OVERRIDE = PcdGet8 (PcdStapmBoost);
  if (PcdGet8 (PcdStapmBoost) != 0) {
    PPTable->STAPM_TIME_CONSTANT = PcdGet32 (PcdStapmTimeConstant);
  }

  // CPPC Control
  PPTable->CppcConstraintsEnabled = PcdGet8 (PcdCppcConstraintsEnabled);
  if (PPTable->CppcConstraintsEnabled == 0x1) {
    // set CPPC as enable
    PcdSet32S (PcdSmuFeatureControlDefinesExt, PcdGet32 (PcdSmuFeatureControlDefinesExt) | (UINT32) (FEATURE_CPPC_BIT % 32));
    PPTable->PerfLimit_MaxRange = PcdGet8 (PcdCppcPerfLimitMaxRange);
    PPTable->PerfLimit_MinRange = PcdGet8 (PcdCppcPerfLimitMinRange);
    PPTable->EPP_MaxRange = PcdGet8 (PcdCppcEppMaxRange);
    PPTable->EPP_MinRange = PcdGet8 (PcdCppcEppMinRange);
  }

  // PWM Control
  PPTable->ForceFanPwmEn = PcdGet8 (PcdForceFanPwmEn);
  if (PPTable->ForceFanPwmEn != 0 ) {
    PPTable->ForceFanPwm = PcdGet8 (PcdForceFanPwm);
  }

  PPTable->FAST_PPT_LIMIT = PcdGet32 (PcdFastPptLimit);
  PPTable->SLOW_PPT_LIMIT = PcdGet32 (PcdSlowPptLimit);
  PPTable->SLOW_PPT_TIME_CONSTANT = PcdGet32 (PcdSlowPptTimeConstant);
  PPTable->THERMAL_LIMIT = PcdGet32 (PcdCfgThermCtlValue);
  PPTable->VRM_VDD_CURRENT_LIMIT = PcdGet32 (PcdVrmCurrentLimit);

  PPTable->OC_GFX_DISABLE = PcdGetBool (PcdGfxOcDisable);
  if (PPTable->OC_GFX_DISABLE == 0) {
    PPTable->OC_GFX_MINVID = PcdGet8 (PcdOcGFXMinVID);
    PPTable->OC_GFX_FREQMAX = PcdGet16 (PcdOcGFXFreqMax);
  }

  PPTable->GfxclkFminOverride = PcdGet16 (PcdGfxclkFminOverride);

  // Core count control
  PPTable->SW_SCI_GPE_ID = PcdGet8 (PcdSwSciGpeID);
  PPTable->CoreCountControlEn= PcdGet8 (PcdCoreCountControlEnable);

  // PostCodeLevel
  PPTable->PostCodeLevel = (UINT8) PcdGet32 (PcdEnableSmuPostCodeLevel);

  // STB Verbosity Control SMU
  PPTable->StbVerbosityLevel = PcdGet8 (PcdStbSmuVerbosityControl);

  //FCLK and CCLK
  PPTable->ForceFCLKLevel = PcdGet8 (PcdFclkValue);
  PPTable->ForceLCLKLevel = PcdGet8 (PcdLclkValue);

  //Indicates MP1 action in case of SMN timeout or SLV error
  PPTable->FUSAReset = PcdGet8(PcdResetOnMP1SMNTimeout);

  // NREP FLOP Select
  if (PcdGet8 (PcdPll5SscDisable) != 0xf) {
    //ApplyDefaultNrepFlopSel is controlled by PcdPll5SscDisable
    PPTable->ApplyDefaultNrepFlopSel  = PcdGet8 (PcdPll5SscDisable); // Default is 0, use 1 to enable
  }

  // P3T Limit
  if (PcdGet8 (PcdP3TLimitCtrl) != 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdP3TLimit %x\n", PcdGet32 (PcdP3TLimit));
    PPTable->P3T_LIMIT = PcdGet32 (PcdP3TLimit);
  }
  // Lclk Frequency
  if (PcdGet32 (PcdCfgLclkFrequencyRange) != 0x0) {
    PPTable->LclkMaxFreq = (UINT16) PcdGet32 (PcdCfgLclkFrequencyRange);
    PPTable->LclkMinFreq = (UINT16) (PcdGet32 (PcdCfgLclkFrequencyRange) >> 16);
  }

  // VDD MISC Voltage in mV
  if (PcdGet16 (PcdVddMiscVoltage) != 0x0) {
    PPTable->VDD_MISC_Voltage = PcdGet16 (PcdVddMiscVoltage);
  }

  PPTable->DisplayIdleOptimizationEn = PcdGetBool (PcdDisplayIdleOptimizationEn);

  if (PcdGet8 (PcdAmdSmuVddcrVddSlewRateIndex) != 0xF) {
    PPTable->SlewRateOverrideVdd = 1;
    PPTable->SlewRateUpIndexVdd = PcdGet8 (PcdAmdSmuVddcrVddSlewRateIndex);
    PPTable->SlewRateDownCntlVdd = PcdGet8 (PcdAmdSmuVddcrVddSlewRateDownCntl);
  }

  PPTable->AA_MODE_EN = PcdGet8 (PcdCfgPeApmEnable);

  if (PcdGet8 (PcdAmdSmuPsiDecayConditionOverride)) {
    PPTable->PsiDecayConditionOverride = PcdGet8 (PcdAmdSmuPsiDecayConditionOverride);
    PPTable->PsiDecayConditionVddRail = PcdGet8 (PcdAmdSmuPsiDecayConditionVddRail);
    PPTable->PsiDecayConditionSocRail = PcdGet8 (PcdAmdSmuPsiDecayConditionSocRail);
    PPTable->PsiDecayConditionSrRail = PcdGet8 (PcdAmdSmuPsiDecayConditionSrRail);
  }

  if (PcdGet8 (PcdAmdSmuVddcrSocSlewRateOverride)) {
    PPTable->SlewRateOverrideSoc = PcdGet8 (PcdAmdSmuVddcrSocSlewRateOverride);
    PPTable->SlewRateUpIndexSoc = PcdGet8 (PcdAmdSmuVddcrSocSlewRateIndex);
    PPTable->SlewRateDownCntlSoc = PcdGet8 (PcdAmdSmuVddcrSocSlewRateDownCntl);
  }
}


