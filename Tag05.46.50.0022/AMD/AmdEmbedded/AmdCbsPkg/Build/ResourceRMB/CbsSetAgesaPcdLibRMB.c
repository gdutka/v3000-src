/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//This file is auto generated, don't edit it manually

#include <Base.h>
#include <Library/DebugLib.h>
#include "AmdCbsVariable.h"
#include <Library/PcdLib.h>

VOID
CbsSetAgesaPcds (
  IN       VOID *CbsVariable
  )
{
  CBS_CONFIG *Setup_Config;

  if (CbsVariable == NULL) {
    ASSERT (FALSE);
    return;
  }
  Setup_Config = (CBS_CONFIG *) CbsVariable;
  if (Setup_Config->CbsCmnCpuGenWA05 != 0xFF) {
    PcdSet8S (PcdAmdRedirectForReturnDis, Setup_Config->CbsCmnCpuGenWA05);
  }

  if (Setup_Config->CbsCmnCpuCpb != 1) {
    PcdSet8S (PcdAmdCpbMode, Setup_Config->CbsCmnCpuCpb);
  }

  if (Setup_Config->CbsCmnCpuGlobalCstateCtrl != 3) {
    PcdSet8S (PcdAmdCStateMode, Setup_Config->CbsCmnCpuGlobalCstateCtrl);
  }

  if (Setup_Config->CbsCmnCpuOpcacheCtrl != 0xFF) {
    PcdSet8S (PcdAmdOpcacheCtrl, Setup_Config->CbsCmnCpuOpcacheCtrl);
  }

  if (Setup_Config->CbsCmnCpuStreamingStoresCtrl != 0xFF) {
    PcdSet8S (PcdAmdStreamingStoresCtrl, Setup_Config->CbsCmnCpuStreamingStoresCtrl);
  }

  if (Setup_Config->CbsCpuLApicMode != 0xFF) {
    PcdSet8S (PcdAmdApicMode, Setup_Config->CbsCpuLApicMode);
  }

  if (Setup_Config->CbsCmnCpuCstC1Ctrl != 3) {
    PcdSetBoolS (PcdAmdAcpiCstC1, Setup_Config->CbsCmnCpuCstC1Ctrl);
  }

  if (Setup_Config->CbsCmnCpuPMCtlPss != 3) {
    PcdSetBoolS (PcdAmdAcpiCpuSsdtPss, Setup_Config->CbsCmnCpuPMCtlPss);
  }

  PcdSetBoolS (PcdMcaErrThreshEn, Setup_Config->CbsCmnCpuMcaErrThreshEn);

  //Check Display Condition CbsCmnCpuMcaErrThreshEn=True
  if ((Setup_Config->CbsCmnCpuMcaErrThreshEn == 0x1)) {
    PcdSet16S (PcdMcaErrThreshCount, Setup_Config->CbsCmnCpuMcaErrThreshCount);
  }

  //Check Display Condition CbsComboFlag=17|CbsComboFlag=18
  if (((Setup_Config->CbsComboFlag == 0x11)) ||((Setup_Config->CbsComboFlag == 0x12))) {
    PcdSetBoolS (PcdAmdEnableFSRM, Setup_Config->CbsCmnCpuFSRM);
  }

  //Check Display Condition CbsComboFlag=17|CbsComboFlag=18
  if (((Setup_Config->CbsComboFlag == 0x11)) ||((Setup_Config->CbsComboFlag == 0x12))) {
    PcdSetBoolS (PcdAmdEnableERMS, Setup_Config->CbsCmnCpuERMS);
  }

  if (Setup_Config->CbsCmnCpuIbsHardwareCtrl != 0) {
    PcdSetBoolS (PcdAmdIbsHardwareEn, Setup_Config->CbsCmnCpuIbsHardwareCtrl);
  }

  if (Setup_Config->CbsCpuPstCustomP0 != 2) {
    PcdSet8S (PcdAmdCcxP0Setting, Setup_Config->CbsCpuPstCustomP0);
  }

  //Check Display Condition CbsCpuPstCustomP0=Custom
  if ((Setup_Config->CbsCpuPstCustomP0 == 0x1)) {
    PcdSet32S (PcdAmdCcxP0Freq, Setup_Config->CbsCpuPst0Freq);
  }

  //Check Display Condition CbsCpuPstCustomP0=Custom
  if ((Setup_Config->CbsCpuPstCustomP0 == 0x1)) {
    PcdSet8S (PcdAmdCcxP0Vid, Setup_Config->CbsCpuPst0Vid);
  }

  if (Setup_Config->CbsCmnCpuL1StreamHwPrefetcher != 3) {
    PcdSetBoolS (PcdAmdL1StreamPrefetcher, Setup_Config->CbsCmnCpuL1StreamHwPrefetcher);
  }

  if (Setup_Config->CbsCmnCpuL2StreamHwPrefetcher != 3) {
    PcdSetBoolS (PcdAmdL2StreamPrefetcher, Setup_Config->CbsCmnCpuL2StreamHwPrefetcher);
  }

  if (Setup_Config->CbsDbgCpuGenCpuWdt != 3) {
    PcdSetBoolS (PcdAmdCpuWdtEn, Setup_Config->CbsDbgCpuGenCpuWdt);
  }

  //Check Display Condition CbsDbgCpuGenCpuWdt=Enabled
  if ((Setup_Config->CbsDbgCpuGenCpuWdt == 0x1)) {
    if (Setup_Config->CbsDbgCpuGenCpuWdtTimeout != 0xFFFF) {
      PcdSet16S (PcdAmdCpuWdtTimeout, Setup_Config->CbsDbgCpuGenCpuWdtTimeout);
    }
  }

  //Check Display Condition CbsDbgCpuGenCpuWdt=Enabled
  if ((Setup_Config->CbsDbgCpuGenCpuWdt == 0x1)) {
    if (Setup_Config->CbsCmnCpuWdtSev != 0xFF) {
      PcdSet8S (PcdAmdCpuWdtSeverity, Setup_Config->CbsCmnCpuWdtSev);
    }
  }

  if (Setup_Config->CbsCmnCpuCoreCtrlRmb != 0) {
    PcdSet8S (PcdAmdDownCoreMode, Setup_Config->CbsCmnCpuCoreCtrlRmb);
  }

  if (Setup_Config->CbsCmnCpuSmtCtrl != 0x1) {
    PcdSet8S (PcdAmdSmtMode, Setup_Config->CbsCmnCpuSmtCtrl);
  }

  if (Setup_Config->CbsCmnPersistentMemRetention != 0xFF) {
    PcdSet8S (PcdPmemRetention, Setup_Config->CbsCmnPersistentMemRetention);
  }

  if (Setup_Config->CbsDfCmnFreezeQueueError != 3) {
    PcdSetBoolS (PcdAmdFabricImmSyncFloodOnFatalErrCtrl, Setup_Config->CbsDfCmnFreezeQueueError);
  }

  if (Setup_Config->CbsDfCmnCstates != 0xFF) {
    PcdSet8S (PcdAmdFabricCstate, Setup_Config->CbsDfCmnCstates);
  }

  if (Setup_Config->CbsCmnMemDataPoisoningDdr4 != 0xFF) {
    PcdSetBoolS (PcdAmdFabricEccScrubPoisoned, Setup_Config->CbsCmnMemDataPoisoningDdr4);
  }

  if (Setup_Config->CbsCmnMemDramRedirectScrubEnDdr != 0xFF) {
    PcdSetBoolS (PcdAmdFabricEccScrubRedirection, Setup_Config->CbsCmnMemDramRedirectScrubEnDdr);
  }

  if (Setup_Config->CbsCmnMemDramRedirectScrubLimitDdr != 0xFF) {
    PcdSet8S (PcdAmdFabricEccScrubRedirectionLimit, Setup_Config->CbsCmnMemDramRedirectScrubLimitDdr);
  }

  PcdSet8S (PcdAmdFabricDramScrubTime, Setup_Config->CbsCmnMemDramScrubTime);

  if (Setup_Config->CbsCmnGnbNbIOMMU != 0xf) {
    PcdSetBoolS (PcdCfgIommuSupport, Setup_Config->CbsCmnGnbNbIOMMU);
  }

  //Check Display Condition CbsCmnGnbNbIOMMU=Enabled
  if ((Setup_Config->CbsCmnGnbNbIOMMU == 0x1)) {
    if (Setup_Config->CbsCmnDmarSupport != 0xF) {
      PcdSetBoolS (PcdIvInfoDmaReMap, Setup_Config->CbsCmnDmarSupport);
    }
  }

  //Check Display Condition CbsCmnGnbNbIOMMU=Enabled
  if ((Setup_Config->CbsCmnGnbNbIOMMU == 0x1)) {
    if (Setup_Config->CbsCmnDmaProtection != 0xF) {
      PcdSetBoolS (PcdDmaProtection, Setup_Config->CbsCmnDmaProtection);
    }
  }

  if (Setup_Config->CbsCmnGnbPcieAriSupport != 0xf) {
    PcdSetBoolS (PcdCfgPcieAriSupport, Setup_Config->CbsCmnGnbPcieAriSupport);
  }

  if (Setup_Config->CbsGnbCmnPcieAriEnumeration != 0xf) {
    PcdSetBoolS (PcdPcieAriForwardingEnable, Setup_Config->CbsGnbCmnPcieAriEnumeration);
  }

  if (Setup_Config->CbsDbgGnbDbgAERCAPEnable != 0xF) {
    PcdSetBoolS (PcdCfgAEREnable, Setup_Config->CbsDbgGnbDbgAERCAPEnable);
  }

  //Check Display Condition CbsDbgGnbDbgAERCAPEnable=Enable|CbsDbgGnbDbgAERCAPEnable=Auto
  if (((Setup_Config->CbsDbgGnbDbgAERCAPEnable == 0x1)) ||((Setup_Config->CbsDbgGnbDbgAERCAPEnable == 0xf))) {
    if (Setup_Config->CbsDbgGnbDbgACSEnable != 0xF) {
      PcdSetBoolS (PcdCfgACSEnable, Setup_Config->CbsDbgGnbDbgACSEnable);
    }
  }

  if (Setup_Config->CbsCmnGnbPciePsppPolicy != 0xf) {
    PcdSet8S (PcdPsppPolicy, Setup_Config->CbsCmnGnbPciePsppPolicy);
  }

  if (Setup_Config->CbsCmnRASControl != 0xF) {
    PcdSet8S (PcdAmdNbioRASControlV2, Setup_Config->CbsCmnRASControl);
  }

  PcdSet32S (PcdEgressPoisonSeverityHi, Setup_Config->PcdEgressPoisonSeverityHi);

  PcdSet32S (PcdEgressPoisonSeverityLo, Setup_Config->PcdEgressPoisonSeverityLo);

  if (Setup_Config->CbsCmnNBIOSyncFloodGen != 0xf) {
    PcdSetBoolS (PcdAmdMaskNbioSyncFlood, Setup_Config->CbsCmnNBIOSyncFloodGen);
  }

  if (Setup_Config->PcdSyncFloodToApml != 0xF) {
    PcdSetBoolS (PcdSyncFloodToApml, Setup_Config->PcdSyncFloodToApml);
  }

  PcdSet32S (PcdAmdNbioEgressPoisonMaskHi, Setup_Config->PcdAmdNbioEgressPoisonMaskHi);

  PcdSet32S (PcdAmdNbioEgressPoisonMaskLo, Setup_Config->PcdAmdNbioEgressPoisonMaskLo);

  PcdSet32S (PcdAmdNbioRASUcpMaskHi, Setup_Config->PcdAmdNbioRASUcpMaskHi);

  PcdSet32S (PcdAmdNbioRASUcpMaskLo, Setup_Config->PcdAmdNbioRASUcpMaskLo);

  PcdSet32S (PcdSyshubWdtTimerInterval, Setup_Config->PcdSyshubWdtTimerInterval);

  PcdSetBoolS (PcdAmdNbioPoisonConsumption, Setup_Config->CbsDbgPoisonConsumption);

  if (Setup_Config->CbsCmnGnbGfxUmaMode != 0xf) {
    PcdSet8S (PcdCfgIgpuContorl, Setup_Config->CbsCmnGnbGfxUmaMode);
  }

  if (Setup_Config->CbsCmnGnbGfxNbAzalia != 0xf) {
    PcdSetBoolS (PcdCfgAzaliaEnable, Setup_Config->CbsCmnGnbGfxNbAzalia);
  }

  if (Setup_Config->CbsDbgGnbAudioIOs != 0xFF) {
    PcdSet8S (PcdAmdNbioAudioSelect, Setup_Config->CbsDbgGnbAudioIOs);
  }

  if (Setup_Config->CbsCmnPcieAllPortsCAPLTR != 0xF) {
    PcdSetBoolS (PcdCfgPCIeLTREnable, Setup_Config->CbsCmnPcieAllPortsCAPLTR);
  }

  if (Setup_Config->CbsCmnPcieCoreDynamicPowerGating != 0xf) {
    PcdSetBoolS (PcdGppPcieDynPortGating, Setup_Config->CbsCmnPcieCoreDynamicPowerGating);
  }

  if (Setup_Config->CbsCmnNbioGlobalCGOverride != 0xf) {
    PcdSet8S (PcdNbioGlobalCgOverride, Setup_Config->CbsCmnNbioGlobalCGOverride);
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnGnbPmmIohcLclkGating != 0xf) {
      PcdSetBoolS (PcdIOHCClkGatingSupport, Setup_Config->CbsCmnGnbPmmIohcLclkGating);
    }
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnGnbNbIOHCPowerGating != 0xf) {
      PcdSetBoolS (PcdIOHCPgEnable, Setup_Config->CbsCmnGnbNbIOHCPowerGating);
    }
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnGnbNbIOMMUL1ClockGating != 0xf) {
      PcdSetBoolS (PcdIommuL1ClockGatingEnable, Setup_Config->CbsCmnGnbNbIOMMUL1ClockGating);
    }
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnGnbNbIOMMUL2ClockGating != 0xf) {
      PcdSetBoolS (PcdIommuL2ClockGatingEnable, Setup_Config->CbsCmnGnbNbIOMMUL2ClockGating);
    }
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnGnbNbIOMMUDynamicPG != 0xf) {
      PcdSetBoolS (PcdCfgIOMMUDynamicPgEnable, Setup_Config->CbsCmnGnbNbIOMMUDynamicPG);
    }
  }

  if (Setup_Config->CbsCmnGnbNbIOMMUL2MemoryPG != 0xf) {
    PcdSetBoolS (PcdCfgIOMMUL2MemoryPGEnable, Setup_Config->CbsCmnGnbNbIOMMUL2MemoryPG);
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnNbioNbifPg != 0xf) {
      PcdSetBoolS (PcdCfgNbifPgClkGating, Setup_Config->CbsCmnNbioNbifPg);
    }
  }

  //Check Display Condition CbsCmnNbioGlobalCGOverride=Auto
  if ((Setup_Config->CbsCmnNbioGlobalCGOverride == 0xf)) {
    if (Setup_Config->CbsCmnNbioShubPg != 0xf) {
      PcdSetBoolS (PcdSyshubPg, Setup_Config->CbsCmnNbioShubPg);
    }
  }

  if (Setup_Config->CbsCmnPmmMmhubSramLS != 0xf) {
    PcdSet8S (PcdMmhubSramLightSleep, Setup_Config->CbsCmnPmmMmhubSramLS);
  }

  if (Setup_Config->CbsCmnPmmNbifIntrOBFF != 0xf) {
    PcdSetBoolS (PcdEnableNbifOBFF, Setup_Config->CbsCmnPmmNbifIntrOBFF);
  }

  if (Setup_Config->CbsCmnPmmNbifDmaOBFF != 0xf) {
    PcdSetBoolS (PcdEnableNbifDmaOBFF, Setup_Config->CbsCmnPmmNbifDmaOBFF);
  }

  if (Setup_Config->CbsCmnFchSataEnable != 0xf) {
    PcdSetBoolS (PcdSataEnable, Setup_Config->CbsCmnFchSataEnable);
  }

  if (Setup_Config->CbsCmnFchSataAutoShutDown != 0xf) {
    PcdSetBoolS (PcdSataControllerAutoShutdown, Setup_Config->CbsCmnFchSataAutoShutDown);
  }

  //Check Display Condition CbsCmnFchSataEnable=Enabled,CbsComboFlag=2|CbsCmnFchSataEnable=Enabled,CbsComboFlag=5
  if (((Setup_Config->CbsCmnFchSataEnable == 0x1) && (Setup_Config->CbsComboFlag == 0x2)) ||((Setup_Config->CbsCmnFchSataEnable == 0x1) && (Setup_Config->CbsComboFlag == 0x5))) {
    if (Setup_Config->CbsCmnFchSataClass != 0xf) {
      PcdSet8S (PcdSataClass, Setup_Config->CbsCmnFchSataClass);
    }
  }

  if (Setup_Config->CbsCmnFchSataRasSupport != 0xf) {
    PcdSetBoolS (PcdSataRasSupport, Setup_Config->CbsCmnFchSataRasSupport);
  }

  if (Setup_Config->CbsCmnFchSataAhciDisPrefetchFunction != 0xf) {
    PcdSetBoolS (PcdSataAhciDisPrefetchFunction, Setup_Config->CbsCmnFchSataAhciDisPrefetchFunction);
  }

  if (Setup_Config->CbsDbgFchSataAggresiveDevSlpP0 != 0xF) {
    PcdSetBoolS (PcdSataDevSlpPort0, Setup_Config->CbsDbgFchSataAggresiveDevSlpP0);
  }

  //Check Display Condition CbsDbgFchSataAggresiveDevSlpP0=Enable
  if ((Setup_Config->CbsDbgFchSataAggresiveDevSlpP0 == 0x1)) {
    PcdSet8S (PcdSataDevSlpPort0Num, Setup_Config->CbsDbgFchSataDevSlpPort0Num);
  }

  if (Setup_Config->CbsDbgFchSataAggresiveDevSlpP1 != 0xF) {
    PcdSetBoolS (PcdSataDevSlpPort1, Setup_Config->CbsDbgFchSataAggresiveDevSlpP1);
  }

  //Check Display Condition CbsDbgFchSataAggresiveDevSlpP1=Enable
  if ((Setup_Config->CbsDbgFchSataAggresiveDevSlpP1 == 0x1)) {
    PcdSet8S (PcdSataDevSlpPort1Num, Setup_Config->CbsDbgFchSataDevSlpPort1Num);
  }

  if (Setup_Config->CbsCmnFchUsbXHCI0Enable != 0xf) {
    PcdSetBoolS (PcdXhci0Enable, Setup_Config->CbsCmnFchUsbXHCI0Enable);
  }

  if (Setup_Config->CbsCmnFchUsbXHCI1Enable != 0xf) {
    PcdSetBoolS (PcdXhci1Enable, Setup_Config->CbsCmnFchUsbXHCI1Enable);
  }

  if (Setup_Config->CbsCmnFchUsbXHCI2Enable != 0xf) {
    PcdSetBoolS (PcdXhci2Enable, Setup_Config->CbsCmnFchUsbXHCI2Enable);
  }

  if (Setup_Config->CbsCmnFchUsbXHCI3SsPort0 != 0xf) {
    PcdSetBoolS (PcdUsb40Enable, Setup_Config->CbsCmnFchUsbXHCI3SsPort0);
  }

  if (Setup_Config->CbsCmnFchUsbXHCI4SsPort0 != 0xf) {
    PcdSetBoolS (PcdUsb41Enable, Setup_Config->CbsCmnFchUsbXHCI4SsPort0);
  }

  if (Setup_Config->CbsCmnFchSystemPwrFailShadow != 0xf) {
    PcdSet8S (PcdPwrFailShadow, Setup_Config->CbsCmnFchSystemPwrFailShadow);
  }

  //Check Display Condition CbsCmnFchUart0Config = Enabled
  if ((Setup_Config->CbsCmnFchUart0Config == 0x1)) {
    if (Setup_Config->CbsCmnFchUart0LegacyConfig != 0xf) {
      PcdSet8S (FchUart0LegacyEnable, Setup_Config->CbsCmnFchUart0LegacyConfig);
    }
  }

  //Check Display Condition CbsCmnFchUart1Config = Enabled
  if ((Setup_Config->CbsCmnFchUart1Config == 0x1)) {
    if (Setup_Config->CbsCmnFchUart1LegacyConfig != 0xf) {
      PcdSet8S (FchUart1LegacyEnable, Setup_Config->CbsCmnFchUart1LegacyConfig);
    }
  }

  //Check Display Condition CbsCmnFchUart2Config = Enabled
  if ((Setup_Config->CbsCmnFchUart2Config == 0x1)) {
    if (Setup_Config->CbsCmnFchUart2LegacyConfig != 0xf) {
      PcdSet8S (FchUart2LegacyEnable, Setup_Config->CbsCmnFchUart2LegacyConfig);
    }
  }

  //Check Display Condition CbsCmnFchUart3Config = Enabled
  if ((Setup_Config->CbsCmnFchUart3Config == 0x1)) {
    if (Setup_Config->CbsCmnFchUart3LegacyConfig != 0xf) {
      PcdSet8S (FchUart3LegacyEnable, Setup_Config->CbsCmnFchUart3LegacyConfig);
    }
  }

  if (Setup_Config->CbsCmnFchEspiConfig != 0xf) {
    PcdSetBoolS (PcdEspiEnable, Setup_Config->CbsCmnFchEspiConfig);
  }

  if (Setup_Config->CbsCmnFchXgbe0Enable != 0xf) {
    PcdSetBoolS (PcdXgbePort0ConfigEn, Setup_Config->CbsCmnFchXgbe0Enable);
  }

  if (Setup_Config->CbsCmnFchXgbe1Enable != 0xf) {
    PcdSetBoolS (PcdXgbePort1ConfigEn, Setup_Config->CbsCmnFchXgbe1Enable);
  }

  PcdSet8S (PcdXgbePort0Led1Gpio, Setup_Config->CbsCmnFchXgbePort0Led1Gpio);

  PcdSet8S (PcdXgbePort0Led2Gpio, Setup_Config->CbsCmnFchXgbePort0Led2Gpio);

  PcdSet8S (PcdXgbePort1Led1Gpio, Setup_Config->CbsCmnFchXgbePort1Led1Gpio);

  PcdSet8S (PcdXgbePort1Led2Gpio, Setup_Config->CbsCmnFchXgbePort1Led2Gpio);

  if (Setup_Config->CbsCmnFchCioPreCmEnable != 0xf) {
    PcdSetBoolS (PcdAmdUsb4CmEnable, Setup_Config->CbsCmnFchCioPreCmEnable);
  }

  if (Setup_Config->CbsCmnFchCioRt0Enable != 0xf) {
    PcdSetBoolS (PcdUsb4Rt0En, Setup_Config->CbsCmnFchCioRt0Enable);
  }

  //Check Display Condition CbsCmnFchCioRt0Enable=Enabled
  if ((Setup_Config->CbsCmnFchCioRt0Enable == 0x1)) {
    if (Setup_Config->CbsCmnFchCioRt0PcieTunnel != 0xf) {
      PcdSetBoolS (PcdUsb4Rt0PcieTnlEn, Setup_Config->CbsCmnFchCioRt0PcieTunnel);
    }
  }

  if (Setup_Config->CbsCmnFchCioRt1Enable != 0xf) {
    PcdSetBoolS (PcdUsb4Rt1En, Setup_Config->CbsCmnFchCioRt1Enable);
  }

  //Check Display Condition CbsCmnFchCioRt1Enable=Enabled
  if ((Setup_Config->CbsCmnFchCioRt1Enable == 0x1)) {
    if (Setup_Config->CbsCmnFchCioRt1PcieTunnel != 0xf) {
      PcdSetBoolS (PcdUsb4Rt1PcieTnlEn, Setup_Config->CbsCmnFchCioRt1PcieTunnel);
    }
  }

  if (Setup_Config->CbsCmnFchSyncfloodEnable != 0xf) {
    PcdSetBoolS (PcdResetCpuOnSyncFlood, Setup_Config->CbsCmnFchSyncfloodEnable);
  }

  if (Setup_Config->CbsCmnGnbSMUSystemConfig != 0xf) {
    PcdSet8S (PcdCfgSystemConfiguration, Setup_Config->CbsCmnGnbSMUSystemConfig);
  }

  PcdSet32S (PcdSustainedPowerLimit, Setup_Config->CbsDbgSustainedPowerLimit);

  //Check Display Condition CbsDbgSTAPMCtl=Manual
  if ((Setup_Config->CbsDbgSTAPMCtl == 0x1)) {
    if (Setup_Config->CbsDbgSystemTemperatureTracking != 0xF) {
      PcdSet8S (PcdSttEnable, Setup_Config->CbsDbgSystemTemperatureTracking);
    }
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet8S (PcdSttPcbSensorCount, Setup_Config->CbsCmnSTTPcbSensorCount);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttMinPowerLimit, Setup_Config->CbsCmnSttMinLimit);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM1, Setup_Config->CbsCmnSTTM1);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM2, Setup_Config->CbsCmnSTTM2);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM3, Setup_Config->CbsCmnSTTM3);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM4, Setup_Config->CbsCmnSTTM4);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM5, Setup_Config->CbsCmnSTTM5);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttM6, Setup_Config->CbsCmnSTTM6);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttCApu, Setup_Config->CbsCmnSTTCAPU);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttCGpu, Setup_Config->CbsCmnSTTCHs2);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttAlphaApu, Setup_Config->CbsCmnSTTAlphaAPU);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttAlphaGpu, Setup_Config->CbsCmnSTTAlphaHS2);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttSkinTemperatureLimitApu, Setup_Config->CbsCmnSTTSkinTemperatureLimitApu);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttSkinTemperatureLimitGpu, Setup_Config->CbsCmnSTTSkinTemperatureLimitHs2);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttErrorCoeff, Setup_Config->CbsCmnSTTErrorCoeff);
  }

  //Check Display Condition CbsDbgSTTCtl=Manual
  if ((Setup_Config->CbsDbgSTTCtl == 0x1)) {
    PcdSet16S (PcdSttErrorRateCoeff, Setup_Config->CbsCmnSTTErrorRateCoeff);
  }

  if (Setup_Config->CbsCmnGnbSmuZstates != 0xF) {
    PcdSetBoolS (PcdAmdSmuZStateControlEnable, Setup_Config->CbsCmnGnbSmuZstates);
  }

  //Check Display Condition CbsCmnCclkFmaxCtl=Manual
  if ((Setup_Config->CbsCmnCclkFmaxCtl == 0x1)) {
    PcdSet32S (PcdFMaxFrequency, Setup_Config->CbsCmnCclkFmaxOverride);
  }

  if (Setup_Config->CbsCmnFclkValue != 0xFF) {
    PcdSet8S (PcdFclkValue, Setup_Config->CbsCmnFclkValue);
  }

  if (Setup_Config->CbsCmnLclkFreq != 0xFF) {
    PcdSet8S (PcdLclkValue, Setup_Config->CbsCmnLclkFreq);
  }

  PcdSet8S (PcdResetOnMP1SMNTimeout, Setup_Config->CbsCmnRstOnMp1SMNTimeout);

  //Check Display Condition CbsCmnAACtl=Manual
  if ((Setup_Config->CbsCmnAACtl == 0x1)) {
    if (Setup_Config->CbsCmnAAEn != 0xF) {
      PcdSet8S (PcdCfgPeApmEnable, Setup_Config->CbsCmnAAEn);
    }
  }

  //Check Display Condition CbsCmnAACtl=Manual
  if ((Setup_Config->CbsCmnAACtl == 0x1)) {
    PcdSet32S (PcdSlowPPTLimitApuOnly, Setup_Config->CbsCmnApuOnlySpptLimit);
  }

  //Check Display Condition CbsCmnAACtl=Manual
  if ((Setup_Config->CbsCmnAACtl == 0x1)) {
    PcdSet32S (PcdMsgSetSustainedPowerLimit, Setup_Config->CbsCmnMsgSetSustainedPowerLimit);
  }

  //Check Display Condition CbsCmnAACtl=Manual
  if ((Setup_Config->CbsCmnAACtl == 0x1)) {
    PcdSet32S (PcdMsgSetFastPPTLimit, Setup_Config->CbsCmnMsgSetFastPPTLimit);
  }

  //Check Display Condition CbsCmnAACtl=Manual
  if ((Setup_Config->CbsCmnAACtl == 0x1)) {
    PcdSet32S (PcdMsgSetSlowPPTLimit, Setup_Config->CbsCmnMsgSeSlowPPTLimit);
  }

  //Check Display Condition CbsComboFlag=0|CbsComboFlag=1|CbsComboFlag=2|CbsComboFlag=3|CbsComboFlag=4|CbsComboFlag=5
  if (((Setup_Config->CbsComboFlag == 0x0)) ||((Setup_Config->CbsComboFlag == 0x1)) ||((Setup_Config->CbsComboFlag == 0x2)) ||((Setup_Config->CbsComboFlag == 0x3)) ||((Setup_Config->CbsComboFlag == 0x4)) ||((Setup_Config->CbsComboFlag == 0x5))) {
    if (Setup_Config->CbsCmnRomArmorSupport != 0xF) {
      PcdSet8S (PcdAmdPspRomArmorSelection, Setup_Config->CbsCmnRomArmorSupport);
    }
  }

  if (Setup_Config->CbsCmnRomArmorSpiConfigRegLock != 0xF) {
    PcdSetBoolS (PcdAmdPspRomArmorSpiConfigRegLock, Setup_Config->CbsCmnRomArmorSpiConfigRegLock);
  }

  if (Setup_Config->CbsCmnSocScpcAutoEnablement != 0xFF) {
    PcdSet8S (PcdScpcAutoEnablement, Setup_Config->CbsCmnSocScpcAutoEnablement);
  }

  if (Setup_Config->CbsCmnMixedDimmConfigNumaDomain != 0xFF) {
    PcdSetBoolS (PcdAmdFabricMixedDimmConfigNumaDomain, Setup_Config->CbsCmnMixedDimmConfigNumaDomain);
  }

  PcdSet32S (PcdAmdPspAntiRollbackInitialSpl, Setup_Config->CbsCmnSocInitSplForFar);

  PcdSetBoolS (PcdAmdPspAntiRollbackLateSplFuse, Setup_Config->CbsCmnSocFarSwitch);

  if (Setup_Config->CbsCmnSoCMiscX86FwSupport != 0xf) {
    PcdSetBoolS (PcdAmdHspCoreEnable, Setup_Config->CbsCmnSoCMiscX86FwSupport);
  }

  if (Setup_Config->CbsCmnSoCMiscHspUart != 0xf) {
    PcdSetBoolS (PcdHspUartEnable, Setup_Config->CbsCmnSoCMiscHspUart);
  }

  //Check Display Condition CbsCmnSoCMiscHspUart=Enabled
  if ((Setup_Config->CbsCmnSoCMiscHspUart == 0x1)) {
    if (Setup_Config->CbsCmnSoCMiscHspUartPort != 0xf) {
      PcdSet8S (PcdHspUartPort, Setup_Config->CbsCmnSoCMiscHspUartPort);
    }
  }

  if (Setup_Config->CbsCmnSoCMiscHspFips != 0xf) {
    PcdSetBoolS (PcdHspFipsEnable, Setup_Config->CbsCmnSoCMiscHspFips);
  }

  if (Setup_Config->CbsCmnSoCMiscHspAcpiFenceEnable != 0xf) {
    PcdSetBoolS (PcdAmdHspAcpiFenceEnable, Setup_Config->CbsCmnSoCMiscHspAcpiFenceEnable);
  }

  if (Setup_Config->CbsCmnSoCMiscMpmSupport != 0xf) {
    PcdSetBoolS (PcdMpmEnable, Setup_Config->CbsCmnSoCMiscMpmSupport);
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1)) {
    if (Setup_Config->CbsCmnSoCMiscWiredKvm != 0xf) {
      PcdSetBoolS (PcdMpmWiredKvmEnable, Setup_Config->CbsCmnSoCMiscWiredKvm);
    }
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1)) {
    if (Setup_Config->CbsCmnSoCMiscWirelessManageability != 0xf) {
      PcdSetBoolS (PcdMpmWirelessManageability, Setup_Config->CbsCmnSoCMiscWirelessManageability);
    }
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled,CbsCmnSoCMiscWirelessManageability=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1) && (Setup_Config->CbsCmnSoCMiscWirelessManageability == 0x1)) {
    if (Setup_Config->CbsCmnSoCMiscWirelessKvm != 0xf) {
      PcdSetBoolS (PcdMpmWirelessKvmEnable, Setup_Config->CbsCmnSoCMiscWirelessKvm);
    }
  }

  //Check Display Condition CbsCmnSoCMiscMpmSupport=Enabled,CbsCmnSoCMiscWirelessManageability=Enabled
  if ((Setup_Config->CbsCmnSoCMiscMpmSupport == 0x1) && (Setup_Config->CbsCmnSoCMiscWirelessManageability == 0x1)) {
    if (Setup_Config->CbsCmnSoCMiscWirelessTcr != 0xf) {
      PcdSetBoolS (PcdMpmWirelessTcrEnable, Setup_Config->CbsCmnSoCMiscWirelessTcr);
    }
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    if (Setup_Config->CbsCmnIntrusionDetectionEnable != 0xFF) {
      PcdSetBoolS (PcdAmdPspIntrusionDetectionEnabled, Setup_Config->CbsCmnIntrusionDetectionEnable);
    }
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    PcdSetBoolS (PcdAmdPspLogEventIfIntrusionDetected, Setup_Config->CbsCmnIntrusionLogEvent);
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    PcdSetBoolS (PcdAmdPspClearTpmIfIntrusionDetected, Setup_Config->CbsCmnIntrusionClearTpm);
  }

  //Check Display Condition CbsCmnIntrusionDetectionCtl=18756
  if ((Setup_Config->CbsCmnIntrusionDetectionCtl == 0x4944)) {
    PcdSetBoolS (PcdAmdPspPowerOffSystemIfIntrusionDetected, Setup_Config->CbsCmnIntrusionPowerOff);
  }

  if (Setup_Config->CbsCmnPT21PcieP0 != 0xf) {
    PcdSet8S (PcdPT21PciePort0Enable, Setup_Config->CbsCmnPT21PcieP0);
  }

  if (Setup_Config->CbsCmnPT21PcieP1 != 0xf) {
    PcdSet8S (PcdPT21PciePort1Enable, Setup_Config->CbsCmnPT21PcieP1);
  }

  if (Setup_Config->CbsCmnPT21PcieP2 != 0xf) {
    PcdSet8S (PcdPT21PciePort2Enable, Setup_Config->CbsCmnPT21PcieP2);
  }

  if (Setup_Config->CbsCmnPT21PcieP3 != 0xf) {
    PcdSet8S (PcdPT21PciePort3Enable, Setup_Config->CbsCmnPT21PcieP3);
  }

  if (Setup_Config->CbsCmnPT21PcieP4 != 0xf) {
    PcdSet8S (PcdPT21PciePort4Enable, Setup_Config->CbsCmnPT21PcieP4);
  }

  if (Setup_Config->CbsCmnPT21PcieP5 != 0xf) {
    PcdSet8S (PcdPT21PciePort5Enable, Setup_Config->CbsCmnPT21PcieP5);
  }

  if (Setup_Config->CbsCmnPT21PcieP6 != 0xf) {
    PcdSet8S (PcdPT21PciePort6Enable, Setup_Config->CbsCmnPT21PcieP6);
  }

  if (Setup_Config->CbsCmnPT21PcieP7 != 0xf) {
    PcdSet8S (PcdPT21PciePort7Enable, Setup_Config->CbsCmnPT21PcieP7);
  }

  if (Setup_Config->CbsCmnPT21PcieP8 != 0xf) {
    PcdSet8S (PcdPT21PciePort8Enable, Setup_Config->CbsCmnPT21PcieP8);
  }

  if (Setup_Config->CbsCmnPT21PcieP9 != 0xf) {
    PcdSet8S (PcdPT21PciePort9Enable, Setup_Config->CbsCmnPT21PcieP9);
  }

  if (Setup_Config->CbsCmnPT21PcieP10 != 0xf) {
    PcdSet8S (PcdPT21PciePort10Enable, Setup_Config->CbsCmnPT21PcieP10);
  }

  if (Setup_Config->CbsCmnPT21PcieP11 != 0xf) {
    PcdSet8S (PcdPT21PciePort11Enable, Setup_Config->CbsCmnPT21PcieP11);
  }

  if (Setup_Config->CbsCmnPT21PcieP12 != 0xf) {
    PcdSet8S (PcdPT21PciePort12Enable, Setup_Config->CbsCmnPT21PcieP12);
  }

  if (Setup_Config->CbsCmnPT21PcieP13 != 0xf) {
    PcdSet8S (PcdPT21PciePort13Enable, Setup_Config->CbsCmnPT21PcieP13);
  }

  if (Setup_Config->CbsCmnPT21SataClass != 0xf) {
    PcdSet8S (PcdPT21SataMode, Setup_Config->CbsCmnPT21SataClass);
  }

  if (Setup_Config->CbsCmnPT21SataPort0 != 0xf) {
    PcdSet8S (PcdPT21SataPort0Enable, Setup_Config->CbsCmnPT21SataPort0);
  }

  if (Setup_Config->CbsCmnPT21SataPort1 != 0xf) {
    PcdSet8S (PcdPT21SataPort1Enable, Setup_Config->CbsCmnPT21SataPort1);
  }

  if (Setup_Config->CbsCmnPT21SataPort2 != 0xf) {
    PcdSet8S (PcdPT21SataPort2Enable, Setup_Config->CbsCmnPT21SataPort2);
  }

  if (Setup_Config->CbsCmnPT21SataPort3 != 0xf) {
    PcdSet8S (PcdPT21SataPort3Enable, Setup_Config->CbsCmnPT21SataPort3);
  }

  if (Setup_Config->CbsDbgPT21SataAggressiveDevSlpP0 != 0xf) {
    PcdSet8S (PcdPT21SataAggressiveDevSlpP0, Setup_Config->CbsDbgPT21SataAggressiveDevSlpP0);
  }

  if (Setup_Config->CbsDbgPT21SataAggressiveDevSlpP1 != 0xf) {
    PcdSet8S (PcdPT21SataAggressiveDevSlpP1, Setup_Config->CbsDbgPT21SataAggressiveDevSlpP1);
  }

  if (Setup_Config->CbsDbgPT21SataAggressiveDevSlpP2 != 0xf) {
    PcdSet8S (PcdPT21SataAggressiveDevSlpP2, Setup_Config->CbsDbgPT21SataAggressiveDevSlpP2);
  }

  if (Setup_Config->CbsDbgPT21SataAggressiveDevSlpP3 != 0xf) {
    PcdSet8S (PcdPT21SataAggressiveDevSlpP3, Setup_Config->CbsDbgPT21SataAggressiveDevSlpP3);
  }

  if (Setup_Config->CbsDbgPT21XhciP0Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP0Gen, Setup_Config->CbsDbgPT21XhciP0Gen);
  }

  if (Setup_Config->CbsDbgPT21XhciP1Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP1Gen, Setup_Config->CbsDbgPT21XhciP1Gen);
  }

  if (Setup_Config->CbsDbgPT21XhciP2Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP2Gen, Setup_Config->CbsDbgPT21XhciP2Gen);
  }

  if (Setup_Config->CbsDbgPT21XhciP3Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP3Gen, Setup_Config->CbsDbgPT21XhciP3Gen);
  }

  if (Setup_Config->CbsDbgPT21XhciP4Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP4Gen, Setup_Config->CbsDbgPT21XhciP4Gen);
  }

  if (Setup_Config->CbsDbgPT21XhciP5Gen != 0xf) {
    PcdSet8S (PcdPT21XhciP5Gen, Setup_Config->CbsDbgPT21XhciP5Gen);
  }

  if (Setup_Config->CbsCmnPT21Usb3P0 != 0xf) {
    PcdSet8S (PcdPT21Usb3P0, Setup_Config->CbsCmnPT21Usb3P0);
  }

  //Check Display Condition CbsCmnPT21Usb3P0=Disable|CbsCmnPT21Usb3P0=Gen2x1|CbsCmnPT21Usb3P0=Auto
  if (((Setup_Config->CbsCmnPT21Usb3P0 == 0x0)) ||((Setup_Config->CbsCmnPT21Usb3P0 == 0x1)) ||((Setup_Config->CbsCmnPT21Usb3P0 == 0xf))) {
    if (Setup_Config->CbsCmnPT21Usb3P1 != 0xf) {
      PcdSet8S (PcdPT21Usb3P1, Setup_Config->CbsCmnPT21Usb3P1);
    }
  }

  if (Setup_Config->CbsCmnPT21Usb3P2 != 0xf) {
    PcdSet8S (PcdPT21Usb3P2, Setup_Config->CbsCmnPT21Usb3P2);
  }

  if (Setup_Config->CbsCmnPT21Usb3P3 != 0xf) {
    PcdSet8S (PcdPT21Usb3P3, Setup_Config->CbsCmnPT21Usb3P3);
  }

  if (Setup_Config->CbsCmnPT21Usb3P4 != 0xf) {
    PcdSet8S (PcdPT21Usb3P4, Setup_Config->CbsCmnPT21Usb3P4);
  }

  if (Setup_Config->CbsCmnPT21Usb3P5 != 0xf) {
    PcdSet8S (PcdPT21Usb3P5, Setup_Config->CbsCmnPT21Usb3P5);
  }

  if (Setup_Config->CbsCmnPT21Usb2P0 != 0xf) {
    PcdSet8S (PcdPT21Usb2P0, Setup_Config->CbsCmnPT21Usb2P0);
  }

  if (Setup_Config->CbsCmnPT21Usb2P1 != 0xf) {
    PcdSet8S (PcdPT21Usb2P1, Setup_Config->CbsCmnPT21Usb2P1);
  }

  if (Setup_Config->CbsCmnPT21Usb2P2 != 0xf) {
    PcdSet8S (PcdPT21Usb2P2, Setup_Config->CbsCmnPT21Usb2P2);
  }

  if (Setup_Config->CbsCmnPT21Usb2P3 != 0xf) {
    PcdSet8S (PcdPT21Usb2P3, Setup_Config->CbsCmnPT21Usb2P3);
  }

  if (Setup_Config->CbsCmnPT21Usb2P4 != 0xf) {
    PcdSet8S (PcdPT21Usb2P4, Setup_Config->CbsCmnPT21Usb2P4);
  }

  if (Setup_Config->CbsCmnPT21Usb2P5 != 0xf) {
    PcdSet8S (PcdPT21Usb2P5, Setup_Config->CbsCmnPT21Usb2P5);
  }

  if (Setup_Config->CbsCmnPT21Usb2P6 != 0xf) {
    PcdSet8S (PcdPT21Usb2P6, Setup_Config->CbsCmnPT21Usb2P6);
  }

  if (Setup_Config->CbsCmnPT21Usb2P7 != 0xf) {
    PcdSet8S (PcdPT21Usb2P7, Setup_Config->CbsCmnPT21Usb2P7);
  }

  if (Setup_Config->CbsCmnPT21Usb2P8 != 0xf) {
    PcdSet8S (PcdPT21Usb2P8, Setup_Config->CbsCmnPT21Usb2P8);
  }

  if (Setup_Config->CbsCmnPT21Usb2P9 != 0xf) {
    PcdSet8S (PcdPT21Usb2P9, Setup_Config->CbsCmnPT21Usb2P9);
  }

  if (Setup_Config->CbsCmnPT21Usb2P10 != 0xf) {
    PcdSet8S (PcdPT21Usb2P10, Setup_Config->CbsCmnPT21Usb2P10);
  }

  if (Setup_Config->CbsCmnPT21Usb2P11 != 0xf) {
    PcdSet8S (PcdPT21Usb2P11, Setup_Config->CbsCmnPT21Usb2P11);
  }

  if (Setup_Config->CbsCmnPT21L4Usb3P0 != 0xf) {
    PcdSet8S (PcdPT21L4Usb3P0, Setup_Config->CbsCmnPT21L4Usb3P0);
  }

  if (Setup_Config->CbsCmnPT21L4Usb3P1 != 0xf) {
    PcdSet8S (PcdPT21L4Usb3P1, Setup_Config->CbsCmnPT21L4Usb3P1);
  }

  if (Setup_Config->CbsCmnPT21L4Usb3P2 != 0xf) {
    PcdSet8S (PcdPT21L4Usb3P2, Setup_Config->CbsCmnPT21L4Usb3P2);
  }

  if (Setup_Config->CbsCmnPT21L4Usb3P3 != 0xf) {
    PcdSet8S (PcdPT21L4Usb3P3, Setup_Config->CbsCmnPT21L4Usb3P3);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P0 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P0, Setup_Config->CbsCmnPT21L4Usb2P0);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P1 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P1, Setup_Config->CbsCmnPT21L4Usb2P1);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P2 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P2, Setup_Config->CbsCmnPT21L4Usb2P2);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P3 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P3, Setup_Config->CbsCmnPT21L4Usb2P3);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P4 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P4, Setup_Config->CbsCmnPT21L4Usb2P4);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P5 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P5, Setup_Config->CbsCmnPT21L4Usb2P5);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P6 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P6, Setup_Config->CbsCmnPT21L4Usb2P6);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P7 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P7, Setup_Config->CbsCmnPT21L4Usb2P7);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P8 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P8, Setup_Config->CbsCmnPT21L4Usb2P8);
  }

  if (Setup_Config->CbsCmnPT21L4Usb2P9 != 0xf) {
    PcdSet8S (PcdPT21L4Usb2P9, Setup_Config->CbsCmnPT21L4Usb2P9);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP0 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort0Enable, Setup_Config->CbsCmnSecPT21PcieP0);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP1 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort1Enable, Setup_Config->CbsCmnSecPT21PcieP1);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP2 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort2Enable, Setup_Config->CbsCmnSecPT21PcieP2);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP3 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort3Enable, Setup_Config->CbsCmnSecPT21PcieP3);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP4 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort4Enable, Setup_Config->CbsCmnSecPT21PcieP4);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP5 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort5Enable, Setup_Config->CbsCmnSecPT21PcieP5);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP6 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort6Enable, Setup_Config->CbsCmnSecPT21PcieP6);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP7 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort7Enable, Setup_Config->CbsCmnSecPT21PcieP7);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP8 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort8Enable, Setup_Config->CbsCmnSecPT21PcieP8);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP9 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort9Enable, Setup_Config->CbsCmnSecPT21PcieP9);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP10 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort10Enable, Setup_Config->CbsCmnSecPT21PcieP10);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP11 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort11Enable, Setup_Config->CbsCmnSecPT21PcieP11);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP12 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort12Enable, Setup_Config->CbsCmnSecPT21PcieP12);
  }

  if (Setup_Config->CbsCmnSecPT21PcieP13 != 0xf) {
    PcdSet8S (PcdSecPT21PciePort13Enable, Setup_Config->CbsCmnSecPT21PcieP13);
  }

  if (Setup_Config->CbsCmnSecPT21SataClass != 0xf) {
    PcdSet8S (PcdSecPT21SataMode, Setup_Config->CbsCmnSecPT21SataClass);
  }

  if (Setup_Config->CbsCmnSecPT21SataPort0 != 0xf) {
    PcdSet8S (PcdSecPT21SataPort0Enable, Setup_Config->CbsCmnSecPT21SataPort0);
  }

  if (Setup_Config->CbsCmnSecPT21SataPort1 != 0xf) {
    PcdSet8S (PcdSecPT21SataPort1Enable, Setup_Config->CbsCmnSecPT21SataPort1);
  }

  if (Setup_Config->CbsCmnSecPT21SataPort2 != 0xf) {
    PcdSet8S (PcdSecPT21SataPort2Enable, Setup_Config->CbsCmnSecPT21SataPort2);
  }

  if (Setup_Config->CbsCmnSecPT21SataPort3 != 0xf) {
    PcdSet8S (PcdSecPT21SataPort3Enable, Setup_Config->CbsCmnSecPT21SataPort3);
  }

  if (Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP0 != 0xf) {
    PcdSet8S (PcdSecPT21SataAggressiveDevSlpP0, Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP0);
  }

  if (Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP1 != 0xf) {
    PcdSet8S (PcdSecPT21SataAggressiveDevSlpP1, Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP1);
  }

  if (Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP2 != 0xf) {
    PcdSet8S (PcdSecPT21SataAggressiveDevSlpP2, Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP2);
  }

  if (Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP3 != 0xf) {
    PcdSet8S (PcdSecPT21SataAggressiveDevSlpP3, Setup_Config->CbsDbgSecPT21SataAggresiveDevSlpP3);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP0Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP0Gen, Setup_Config->CbsDbgSecPT21XhciP0Gen);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP1Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP1Gen, Setup_Config->CbsDbgSecPT21XhciP1Gen);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP2Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP2Gen, Setup_Config->CbsDbgSecPT21XhciP2Gen);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP3Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP3Gen, Setup_Config->CbsDbgSecPT21XhciP3Gen);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP4Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP4Gen, Setup_Config->CbsDbgSecPT21XhciP4Gen);
  }

  if (Setup_Config->CbsDbgSecPT21XhciP5Gen != 0xf) {
    PcdSet8S (PcdSecPT21XhciP5Gen, Setup_Config->CbsDbgSecPT21XhciP5Gen);
  }

  if (Setup_Config->CbsCmnSecPT21Usb3P0 != 0xf) {
    PcdSet8S (PcdSecPT21Usb3P0, Setup_Config->CbsCmnSecPT21Usb3P0);
  }

  //Check Display Condition CbsCmnSecPT21Usb3P0=Disable|CbsCmnSecPT21Usb3P0=Gen2x1|CbsCmnSecPT21Usb3P0=Auto
  if (((Setup_Config->CbsCmnSecPT21Usb3P0 == 0x0)) ||((Setup_Config->CbsCmnSecPT21Usb3P0 == 0x1)) ||((Setup_Config->CbsCmnSecPT21Usb3P0 == 0xf))) {
    if (Setup_Config->CbsCmnSecPT21Usb3P1 != 0xf) {
      PcdSet8S (PcdSecPT21Usb3P1, Setup_Config->CbsCmnSecPT21Usb3P1);
    }
  }

  if (Setup_Config->CbsCmnSecPT21Usb3P2 != 0xf) {
    PcdSet8S (PcdSecPT21Usb3P2, Setup_Config->CbsCmnSecPT21Usb3P2);
  }

  if (Setup_Config->CbsCmnSecPT21Usb3P3 != 0xf) {
    PcdSet8S (PcdSecPT21Usb3P3, Setup_Config->CbsCmnSecPT21Usb3P3);
  }

  if (Setup_Config->CbsCmnSecPT21Usb3P4 != 0xf) {
    PcdSet8S (PcdSecPT21Usb3P4, Setup_Config->CbsCmnSecPT21Usb3P4);
  }

  if (Setup_Config->CbsCmnSecPT21Usb3P5 != 0xf) {
    PcdSet8S (PcdSecPT21Usb3P5, Setup_Config->CbsCmnSecPT21Usb3P5);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P0 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P0, Setup_Config->CbsCmnSecPT21Usb2P0);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P1 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P1, Setup_Config->CbsCmnSecPT21Usb2P1);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P2 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P2, Setup_Config->CbsCmnSecPT21Usb2P2);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P3 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P3, Setup_Config->CbsCmnSecPT21Usb2P3);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P4 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P4, Setup_Config->CbsCmnSecPT21Usb2P4);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P5 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P5, Setup_Config->CbsCmnSecPT21Usb2P5);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P6 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P6, Setup_Config->CbsCmnSecPT21Usb2P6);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P7 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P7, Setup_Config->CbsCmnSecPT21Usb2P7);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P8 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P8, Setup_Config->CbsCmnSecPT21Usb2P8);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P9 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P9, Setup_Config->CbsCmnSecPT21Usb2P9);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P10 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P10, Setup_Config->CbsCmnSecPT21Usb2P10);
  }

  if (Setup_Config->CbsCmnSecPT21Usb2P11 != 0xf) {
    PcdSet8S (PcdSecPT21Usb2P11, Setup_Config->CbsCmnSecPT21Usb2P11);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb3P0 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb3P0, Setup_Config->CbsCmnSecPT21L4Usb3P0);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb3P1 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb3P1, Setup_Config->CbsCmnSecPT21L4Usb3P1);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb3P2 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb3P2, Setup_Config->CbsCmnSecPT21L4Usb3P2);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb3P3 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb3P3, Setup_Config->CbsCmnSecPT21L4Usb3P3);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P0 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P0, Setup_Config->CbsCmnSecPT21L4Usb2P0);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P1 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P1, Setup_Config->CbsCmnSecPT21L4Usb2P1);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P2 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P2, Setup_Config->CbsCmnSecPT21L4Usb2P2);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P3 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P3, Setup_Config->CbsCmnSecPT21L4Usb2P3);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P4 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P4, Setup_Config->CbsCmnSecPT21L4Usb2P4);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P5 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P5, Setup_Config->CbsCmnSecPT21L4Usb2P5);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P6 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P6, Setup_Config->CbsCmnSecPT21L4Usb2P6);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P7 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P7, Setup_Config->CbsCmnSecPT21L4Usb2P7);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P8 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P8, Setup_Config->CbsCmnSecPT21L4Usb2P8);
  }

  if (Setup_Config->CbsCmnSecPT21L4Usb2P9 != 0xf) {
    PcdSet8S (PcdSecPT21L4Usb2P9, Setup_Config->CbsCmnSecPT21L4Usb2P9);
  }


}
