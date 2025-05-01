/*****************************************************************************
 *
 * Copyright (C) 2014-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_CPM_POST_CODE_H_
#define _AMD_CPM_POST_CODE_H_

/// <PostCodePrefix> 0xB0000000

#ifdef CPM_TP_BASE_VALUE
#define CpmTpBaseValue                        CPM_TP_BASE_VALUE
#else
#define CpmTpBaseValue                        0xB0000000
#endif

/// The CPM post code

typedef enum {
  // CPM Post Code for POST time - Region 0x0C00 ~ 0x0CFF
  // CPM Library Base Post Code
  CpmTpErrorNoEnoughSizeInHobBuffer =         0x0C01,           ///< No free table item or no enough size in Hob Buffer
  CpmTpErrorTableNumberOverflow =             0x0C02,           ///< Table number is greater than AMD_TABLE_LIST_ITEM_SIZE
  CpmTpErrorMainTablePointerInvalid =         0x0C03,           ///< Main Table pointer is invalid

  // CPM Features AdaptiveS4 Post Code
  CpmTpAdaptiveS4PeimDriverBegin =            0x0C16,           ///< Begin of Adaptive S4 PEIM driver
  CpmTpAdaptiveS4PeimDriverEnd =              0x0C17,           ///< End of Adaptive S4 PEIM driver
  CpmTpAdaptiveS4DxeDriverBegin =             0x0C18,           ///< Begin of Adaptive S4 DXE driver
  CpmTpAdaptiveS4DxeDriverEnd =               0x0C19,           ///< End of Adaptive S4 DXE driver
  CpmTpAdaptiveS4DxeDisplayTurnOffBegin =     0x0C1A,           ///< Begin to turn off display in Adaptive S4 DXE driver
  CpmTpAdaptiveS4DxeDisplayTurnOffEnd =       0x0C1B,           ///< End to turn off display in Adaptive S4 DXE driver
  CpmTpAdaptiveS4SmmDriverBegin =             0x0C1C,           ///< Begin of Adaptive S4 SMM driver
  CpmTpAdaptiveS4SmmDriverEnd =               0x0C1D,           ///< End of Adaptive S4 SMM driver
  CpmTpAdaptiveS4SmmRtcAlarmProgramBegin =    0x0C1E,           ///< Begin to program RTC alarm register in Adaptive S4 SMM driver
  CpmTpAdaptiveS4SmmRtcAlarmProgramEnd =      0x0C1F,           ///< End to program RTC alarm register in Adaptive S4 SMM driver

  // CPM Features DisplayFeature Post Code
  CpmTpDisplayFeaturePeimDriverBegin =        0x0C20,           ///< Begin of Display Feature PEIM driver
  CpmTpDisplayFeaturePeimDriverEnd =          0x0C21,           ///< End of Display Feature PEIM driver
  CpmTpDisplayFeatureDxeDriverBegin =         0x0C22,           ///< Begin of Display Feature DXE driver
  CpmTpDisplayFeatureDxeDriverEnd =           0x0C23,           ///< End of Display Feature DXE driver
  CpmTpDisplayFeatureVbiosShadowBegin =       0x0C24,           ///< Begin of Display Feature DXE VBIOS Shadow
  CpmTpDisplayFeatureVbiosShadowEnd =         0x0C25,           ///< End of Display Feature DXE VBIOS Shadow
  CpmTpDisplayFeatureDisableIoDecodeBegin =   0x0C26,           ///< Begin of Display Feature DXE Disable IO Decode of Possible Pcie Gfx
  CpmTpDisplayFeatureDisableIoDecodeEnd =     0x0C27,           ///< End of Display Feature DXE Disable IO Decode of Possible Pcie Gfx
  CpmTpDisplayFeatureDxeInitLateBegin =       0x0C28,           ///< Begin of Display Feature DXE Init Late
  CpmTpDisplayFeatureDxeInitLateEnd =         0x0C29,           ///< End of Display Feature DXE Init Late
  CpmTpDisplayFeatureSmmDriverBegin =         0x0C2A,           ///< Begin of Display Feature SMM driver
  CpmTpDisplayFeatureSmmDriverEnd =           0x0C2B,           ///< End of Display Feature SMM driver

  // CPM Features EcInit Post Code
  CpmTpEcInitPeimDriverBegin =                0x0C2C,           ///< Begin of EC Init PEIM driver
  CpmTpEcInitPeimDriverEnd =                  0x0C2D,           ///< End of EC Init PEIM driver
  CpmTpEcInitDxeDriverBegin =                 0x0C2E,           ///< Begin of EC Init DXE driver
  CpmTpEcInitDxeDriverEnd =                   0x0C2F,           ///< End of EC Init DXE driver

  // CPM Features GpioInit Post Code
  CpmTpGpioInitPeimDriverBegin =              0x0C30,           ///< Begin of GPIO Init PEIM driver
  CpmTpGpioInitPeimDriverEnd =                0x0C31,           ///< End of GPIO Init PEIM driver
  CpmTpGpioInitPeimResetDeviceBegin =         0x0C32,           ///< Begin to Reset Device in GPIO Init PEIM driver
  CpmTpGpioInitPeimResetDeviceEnd =           0x0C33,           ///< End to Reset Device in GPIO Init PEIM driver
  CpmTpGpioInitPeimSetMemVoltageBegin =       0x0C34,           ///< Begin to Set Mem Voltage in GPIO Init PEIM driver
  CpmTpGpioInitPeimSetMemVoltageEnd =         0x0C35,           ///< End to Set Mem Voltage in GPIO Init PEIM driver
  CpmTpGpioInitPeimSetVddpVddrVoltageBegin =  0x0C36,           ///< Begin to Set Vddp/Vddr Voltage in GPIO Init PEIM driver
  CpmTpGpioInitPeimSetVddpVddrVoltageEnd =    0x0C37,           ///< End to Set Vddp/Vddr Voltage in GPIO Init PEIM driver
  CpmTpGpioInitDxeDriverBegin =               0x0C38,           ///< Begin of GPIO Init DXE driver
  CpmTpGpioInitDxeDriverEnd =                 0x0C39,           ///< Begin of GPIO Init DXE driver
  CpmTpGpioInitDxePcieClockInitBegin =        0x0C3A,           ///< Begin to init PCIe Clock in GPIO Init DXE driver
  CpmTpGpioInitDxePcieClockInitEnd =          0x0C3B,           ///< Begin to init PCIe Clock in GPIO Init DXE driver
  CpmTpGpioInitSmmDriverBegin =               0x0C3C,           ///< Begin of GPIO Init SMM driver
  CpmTpGpioInitSmmDriverEnd =                 0x0C3D,           ///< Begin of GPIO Init SMM driver

  // CPM Features PcieInit Post Code
  CpmTpPcieInitPeimDriverBegin =              0x0C3E,           ///< Begin of PCIE Init PEIM driver
  CpmTpPcieInitPeimDriverEnd =                0x0C3F,           ///< End of PCIE Init PEIM driver
  CpmTpPcieInitDxeDriverBegin =               0x0C40,           ///< Begin of PCIE Init DXE driver
  CpmTpPcieInitDxeDriverEnd =                 0x0C41,           ///< End of PCIE Init DXE driver

  // CPM Features ZeroPowerOdd Post Code
  CpmTpZeroPowerOddPeimDriverBegin =          0x0C44,           ///< Begin of Zero Power Odd PEIM driver
  CpmTpZeroPowerOddPeimDriverEnd =            0x0C45,           ///< End of Zero Power Odd PEIM driver
  CpmTpZeroPowerOddDxeDriverBegin =           0x0C46,           ///< Begin of Zero Power Odd DXE driver
  CpmTpZeroPowerOddDxeDriverEnd =             0x0C47,           ///< End of Zero Power Odd DXE drive

  // CPM Features Ucsi Post Code
  CpmTpUcsiPeimDriverBegin =                  0x0C48,           ///< Begin of UCSI PEIM driver
  CpmTpUcsiPeimDriverEnd =                    0x0C49,           ///< End of UCSI PEIM drive
  CpmTpUcsiDxeDriverBegin =                   0x0C4A,           ///< Begin of UCSI DXE driver
  CpmTpUcsiDxeDriverEnd =                     0x0C4B,           ///< End of UCSI DXE drive

  // CPM Oem Post Code
  CpmTpOemInitDxeDriverBegin =                0x0C4C,           ///< Begin of OEM Init DXE driver
  CpmTpOemInitDxeDriverEnd =                  0x0C4D,           ///< End of OEM Init DXE drive

  // CPM Features SensorFusion Post Code
  CpmTpSensorFusionDxeDriverBegin =           0x0C4E,           ///< Begin of SensorFusion DXE driver
  CpmTpSensorFusionDxeDriverEnd =             0x0C4F,           ///< End of SensorFusion DXE drive

  // CPM Features AcpPowerGating Post Code
  CpmTpAcpPowerGatingDxeDriverBegin =         0x0C50,           ///< Begin of AcpPowerGating DXE driver
  CpmTpAcpPowerGatingDxeDriverEnd =           0x0C51,           ///< End of AcpPowerGating DXE drive

  // CPM Features Thunderbolt Post Code
  CpmTpThunderboltEnableForcePowerPeimBegin = 0x0C52,           ///< Begin of Thunderbolt Enable Force Power PEIM driver
  CpmTpThunderboltEnableForcePowerPeimEnd =   0x0C53,           ///< End of Thunderbolt Enable Force Power PEIM driver
  CpmTpThunderboltPeimDriverBegin =           0x0C54,           ///< Begin of Thunderbolt PEIM driver
  CpmTpThunderboltPeimDriverEnd =             0x0C55,           ///< End of Thunderbolt PEIM driver
  CpmTpThunderboltDxeEntryBegin =             0x0C56,           ///< Begin of Thunderbolt DXE Entry
  CpmTpThunderboltDxeEntryEnd =               0x0C57,           ///< End of Thunderbolt DXE Entry
  CpmTpThunderboltSmmDriverBegin =            0x0C58,           ///< Begin of Thunderbolt SMM driver
  CpmTpThunderboltSmmDriverEnd =              0x0C59,           ///< End of Thunderbolt SMM driver
  CpmTpThunderboltMidBegin =                  0x0C5A,           ///< Begin of Thunderbolt init Mid
  CpmTpThunderboltMidEnd =                    0x0C5B,           ///< End of Thunderbolt init Mid
  CpmTpThunderboltLateBegin =                 0x0C5C,           ///< Begin of Thunderbolt init Late
  CpmTpThunderboltLateEnd =                   0x0C5D,           ///< End of Thunderbolt init Late
  CpmTpPciHotPlugDxeEntryBegin =              0x0C5E,           ///< Begin of PciHotPlug DXE Entry
  CpmTpPciHotPlugDxeEntryEnd =                0x0C5F,           ///< End of PciHotPlug DXE Entry
  CpmTpThunderboltRestoreSmmBegin =           0x0C60,           ///< Begin of Thunderbolt Restore Smm driver
  CpmTpThunderboltRestoreSmmEnd =             0x0C61,           ///< End of Thunderbolt Restore Smm driver
  CpmTpThunderboltSmmCallbackBegin =          0x0C62,           ///< Begin of Thunderbolt Smm Callback driver
  CpmTpThunderboltSmmCallbackFail =           0x0C63,           ///< Fail of Thunderbolt Smm Callback driver
  CpmTpThunderboltSmmCallbackEnd =            0x0C64,           ///< End of Thunderbolt Smm Callback driver

  // CPM Features ABRecovery Post Code
  CpmTpABRecoveryPeimEntryBegin =             0x0C65,           ///< Begin of ABRecoveryPeim Entry
  CpmTpABRecoveryPeimEntryEnd =               0x0C66,           ///< End of ABRecoveryPeim Entry
  CpmTpABRecoveryPeimGetReasonBegin =         0x0C67,           ///< Begin of ABRecoveryPeim Get Recovery Reason
  CpmTpABRecoveryPeimGetReasonEnd =           0x0C68,           ///< End of ABRecoveryPeim Get Recovery Reason
  CpmTpABRecoveryPeimGetPartitionBegin =      0x0C69,           ///< Begin of ABRecoveryPeim Get Partition
  CpmTpABRecoveryPeimGetPartitionEnd =        0x0C6A,           ///< End of ABRecoveryPeim Get Partition
  CpmTpABRecoveryPeimSelfHealingBegin =       0x0C6B,           ///< Begin of ABRecoveryPeim Self Healing
  CpmTpABRecoveryPeimSelfHealingEnd =         0x0C6C,           ///< End of ABRecoveryPeim Self Healing

  // CPM Features WakeOnVoice Post Code
  CpmTpWakeOnVoiceDxeDriverBegin =            0x0C6D,           ///< Begin of WakeOnVoice DXE driver
  CpmTpWakeOnVoiceDxeDriverEnd =              0x0C6E,           ///< End of WakeOnVoice DXE drive

} AMD_CPM_POST_CODE;

#endif
