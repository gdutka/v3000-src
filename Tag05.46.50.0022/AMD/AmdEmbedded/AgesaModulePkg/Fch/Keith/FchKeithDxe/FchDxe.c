/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "FchDxe.h"
#include "GnbDxio.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#define FILECODE FCH_KEITH_FCHKEITHDXE_FCHDXE_FILECODE

extern EFI_GUID gFchResetDataHobGuid;
extern EFI_GUID gEfiPciIoProtocolGuid;
extern FCH_DATA_BLOCK InitEnvCfgDefault;

//
// Private Function Declaration
//
VOID
InvokeFchInitMid (
  IN       EFI_EVENT        Event,
  IN       VOID             *Context
  )
{
  FCH_DATA_BLOCK        *FchPolicy;
  AGESA_STATUS          AgesaStatus;

  DEBUG ((DEBUG_INFO, "[FchInitMid] Fch Init - After PCI Scan ...Start\n"));
  FchPolicy = (FCH_DATA_BLOCK*) Context;
  AgesaStatus = FchInitMid (FchPolicy);
  DEBUG ((DEBUG_INFO, "[FchInitMid] Fch Init - After PCI Scan ...Complete\n"));

  gBS->CloseEvent (Event);
}

VOID
InvokeFchInitLate (
  IN       EFI_EVENT        Event,
  IN       VOID             *Context
  )
{
  FCH_DATA_BLOCK        *FchPolicy;
  AGESA_STATUS          AgesaStatus;

  DEBUG ((DEBUG_INFO, "[FchInitLate] Fch Init - Before Boot ...Start\n"));
  FchPolicy = (FCH_DATA_BLOCK*) Context;
  AgesaStatus = FchInitLate (FchPolicy);
  DEBUG ((DEBUG_INFO, "[FchInitLate] Fch Init - Before Boot ...Complete\n"));

  gBS->CloseEvent (Event);
}

VOID
FchPciEnumerationCompleteCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  FCH_DATA_BLOCK    *LocalCfgPtr;

  DEBUG ((DEBUG_INFO, "FchPciEnumerationCompleteCallBack Start\n"));

  if (Context != NULL) {
    LocalCfgPtr = (FCH_DATA_BLOCK*) Context;
    FchDisEspiMasCtlRegWr (LocalCfgPtr);
  }

  DEBUG ((DEBUG_INFO, "FchPciEnumerationCompleteCallBack Complete\n"));

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
 * @brief Allocate iLa1MTrace Memory if enabled
 *
 * @return EFI_STATUS
 */
EFI_STATUS
ProgramFchiLa1MTraceMemoryEn (
  VOID
  )
{
  EFI_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT64                 MmioBase;
  UINT64                 Length;

  Status = EFI_SUCCESS;
  if (PcdGetBool (PcdFchiLa1MTraceMemoryEn)) {
    IDS_HDT_CONSOLE (FCH_TRACE, "ProgramFchiLa1MTraceMemoryEn is started!\n");
    Length = 0x100000;
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = 0;
    MmioTarget.RbNum =0;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    MmioBase  = 0;
    Status = FabricAllocateMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "[ProgramFchiLa1MTraceMemoryEn] Cannot allocate MMIO!!!\n");
      return Status;
    }

    PcdSet32S (PcdFchiLa1MTraceMemoryBase, (UINT32) MmioBase);
    IDS_HDT_CONSOLE (FCH_TRACE, "iLa1MTraceMemoryBase=0x%x\n",MmioBase);
    IDS_HDT_CONSOLE (FCH_TRACE, "ProgramFchiLa1MTraceMemoryEn is completed!\n");
    return Status;
  }
  return Status;
}


FCH_DATA_BLOCK*
FchInitDataBlock (
  IN OUT   FCH_DATA_BLOCK  *FchParams
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *FchHob;
  FCH_RESET_DATA_BLOCK      *FchResetParams;
  VOID                      *PcdDataPtr;
  UINT16                    IoEnable16;
  UINT16                    UartChannel;
  UINT8                     UartLegacy[4];
  PCI_ADDR                  NbioPciAddress;
  UINT32                    SmuArg[6];

  //load default
  gBS->CopyMem (
         FchParams,
         &InitEnvCfgDefault,
         sizeof (FCH_DATA_BLOCK)
         );

  //find HOB and update with reset data block
  FchHob = GetFirstGuidHob (&gFchResetDataHobGuid);
  if (FchHob == NULL) {
    DEBUG ((DEBUG_INFO, "FCH HOB Not located, Exiting.\n"));
    Status = EFI_UNSUPPORTED;
    ASSERT_EFI_ERROR (Status);
    return FchParams;
  }
  FchHob++;
  FchResetParams = (FCH_RESET_DATA_BLOCK *) ((UINTN)FchHob - sizeof (UINT32));
  //FchParams->FchResetDataBlock = FchResetParams;
  gBS->CopyMem (
         &FchParams->FchResetDataBlock.FchReset,
         (VOID *)((UINTN)FchResetParams + sizeof (UINT64)),
         (UINTN)&(FchResetParams->OemUsbConfigurationTablePtr) - (UINTN)&(FchResetParams->FchReset)
         );

  FchParams->Usb.Xhci0Enable              = FchResetParams->FchReset.Xhci0Enable;
  FchParams->Usb.Xhci1Enable              = FchResetParams->FchReset.Xhci1Enable;
  FchParams->Spi.SpiFastSpeed             = FchResetParams->FastSpeed;
  FchParams->Spi.WriteSpeed               = FchResetParams->WriteSpeed;
  FchParams->Spi.SpiMode                  = FchResetParams->Mode;
  FchParams->Spi.SpiSpeed                 = FchResetParams->SpiSpeed;
  FchParams->Spi.AutoMode                 = FchResetParams->AutoMode;
  FchParams->Spi.SpiBurstWrite            = FchResetParams->BurstWrite;
  FchParams->Misc.Cg2Pll                  = FchResetParams->Cg2Pll;
  FchParams->Sata[0].SataSetMaxGen2       = FchResetParams->SataSetMaxGen2;

  //Platform call out
  Status = FchPlatformOemDxeInit ((VOID *)FchParams);

  ASSERT_EFI_ERROR (Status);

  IDS_HOOK (IDS_HOOK_FCH_INIT_ENV, NULL, (VOID *)FchParams);

  //Dynamic PCDs
  FchParams->Ab.AbClockGating             = PcdGet8 (PcdAbClockGating);
  FchParams->Ab.ALinkClkGateOff           = PcdGet8 (PcdALinkClkGateOff);
  FchParams->Ab.BLinkClkGateOff           = PcdGet8 (PcdBLinkClkGateOff);
  FchParams->Ab.AbMemoryPowerSaving       = PcdGetBool (PcdAbMemoryPowerSaving);
  FchParams->Ab.SbgMemoryPowerSaving      = PcdGetBool (PcdSbgMemoryPowerSaving);
  FchParams->Ab.SbgClockGating            = PcdGetBool (PcdSbgClockGating);
  FchParams->Ab.XdmaDmaWrite16ByteMode    = PcdGetBool (PcdXdmaDmaWrite16ByteMode);
  FchParams->Ab.XdmaMemoryPowerSaving     = PcdGetBool (PcdXdmaMemoryPowerSaving);
  FchParams->Ab.XdmaPendingNprThreshold   = PcdGet8 (PcdXdmaPendingNprThreshold);
  FchParams->Ab.XdmaDncplOrderDis         = PcdGetBool (PcdXdmaDncplOrderDis);
  FchParams->Ab.SdphostBypassDataPack     = PcdGetBool (PcdSdphostBypassDataPack);
  FchParams->Ab.SdphostDisNpmwrProtect    = PcdGetBool (PcdSdphostDisNpmwrProtect);
  if (PcdGetBool (PcdResetCpuOnSyncFlood) && (!PcdGetBool (PcdSyncFloodToApml))) {
    FchParams->Ab.ResetCpuOnSyncFlood     = TRUE;
  } else {
    FchParams->Ab.ResetCpuOnSyncFlood     = FALSE;
  }

  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  if (PcdGetBool (PcdResetCpuOnSyncFlood)) {
    SmuArg[0] = 1;
  }
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_SetResetCpuOnSyncFlood, SmuArg,0);

  //Sata controller
  {
    UINT8 SataController;
    UINT8 SataEnable2;

    SataEnable2 = PcdGet8 (PcdSataEnable2);
    for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
      if (SataEnable2 & (1 << SataController)) {
        FchParams->Sata[SataController].SataEnable = TRUE;
      } else {
        FchParams->Sata[SataController].SataEnable = FALSE;
      }
    }
  }

  FchParams->Sata[0].SataClass               = PcdGet8 (PcdSataClass);
  if ((PcdGet8 (PcdSataClass) != SataRaid)
    && (PcdGet8 (PcdSataClass) != SataAhci)
    && (PcdGet8 (PcdSataClass) != SataAhci7804))
  {
    FchParams->Sata[0].SataClass             = SataAhci;
  }
  FchParams->Sata[0].SataAggrLinkPmCap       = PcdGet8 (PcdSataAggrLinkPmCap);
  FchParams->Sata[0].SataPortMultCap         = PcdGet8 (PcdSataPortMultCap);
  FchParams->Sata[0].SataPscCap              = PcdGet8 (PcdSataPscCap);
  FchParams->Sata[0].SataSscCap              = PcdGet8 (PcdSataSscCap);
  FchParams->Sata[0].SataClkAutoOff          = PcdGet8 (PcdSataClkAutoOff);
  FchParams->Sata[0].SataFisBasedSwitching   = PcdGet8 (PcdSataFisBasedSwitching);
  FchParams->Sata[0].SataCccSupport          = PcdGet8 (PcdSataCccSupport);
  FchParams->Sata[0].SataDisableGenericMode  = PcdGet8 (PcdSataDisableGenericMode);
  FchParams->Sata[0].SataTargetSupport8Device          = PcdGet8 (PcdSataTargetSupport8Device);
  FchParams->Sata[0].SataAhciEnclosureManagement       = PcdGet8 (PcdSataAhciEnclosureManagement);
  FchParams->Sata[0].SataMsiEnable           = PcdGetBool (PcdSataMsiEnable);
  FchParams->Sata[0].SataRasSupport          = PcdGetBool (PcdSataRasSupport);
  FchParams->Sata[0].SataAhciDisPrefetchFunction       = PcdGetBool (PcdSataAhciDisPrefetchFunction);

  FchParams->Sata[0].SataDevSlpPort0         = PcdGetBool (PcdSataDevSlpPort0);
  FchParams->Sata[0].SataDevSlpPort1         = PcdGetBool (PcdSataDevSlpPort1);
  FchParams->Sata[0].SataDevSlpPort0Num      = PcdGet8 (PcdSataDevSlpPort0Num);
  FchParams->Sata[0].SataDevSlpPort1Num      = PcdGet8 (PcdSataDevSlpPort1Num);
  FchParams->Sata[0].SataSgpio0              = PcdGet8 (PcdSataSgpio0);
  FchParams->Sata[0].SataControllerAutoShutdown        = PcdGetBool (PcdSataControllerAutoShutdown);

  //Sata common settings
  FchParams->Sata[1].SataClass               = FchParams->Sata[0].SataClass;
  FchParams->Sata[1].SataSetMaxGen2          = FchParams->Sata[0].SataSetMaxGen2;
  FchParams->Sata[1].SataAggrLinkPmCap       = FchParams->Sata[0].SataAggrLinkPmCap;
  FchParams->Sata[1].SataPortMultCap         = FchParams->Sata[0].SataPortMultCap;
  FchParams->Sata[1].SataPscCap              = FchParams->Sata[0].SataPscCap;
  FchParams->Sata[1].SataSscCap              = FchParams->Sata[0].SataSscCap;
  FchParams->Sata[1].SataClkAutoOff          = FchParams->Sata[0].SataClkAutoOff;
  FchParams->Sata[1].SataFisBasedSwitching   = FchParams->Sata[0].SataFisBasedSwitching;
  FchParams->Sata[1].SataCccSupport          = FchParams->Sata[0].SataCccSupport;
  FchParams->Sata[1].SataMsiEnable           = FchParams->Sata[0].SataMsiEnable;
  FchParams->Sata[1].SataTargetSupport8Device          = FchParams->Sata[0].SataTargetSupport8Device;
  FchParams->Sata[1].SataDisableGenericMode  = FchParams->Sata[0].SataDisableGenericMode;
  FchParams->Sata[1].SataAhciEnclosureManagement       = FchParams->Sata[0].SataAhciEnclosureManagement;
  FchParams->Sata[1].SataRasSupport          = FchParams->Sata[0].SataRasSupport;
  FchParams->Sata[1].SataAhciDisPrefetchFunction       = FchParams->Sata[0].SataAhciDisPrefetchFunction;
  FchParams->Sata[1].SataControllerAutoShutdown         = FchParams->Sata[0].SataControllerAutoShutdown;
  //Sata controller
  {
    UINT8 SataController;
    UINT8 SataSgpioEnable;
    UINT64 SataEspEnable;
    UINT64 SataPortShutdown;
    UINT64 SataPortMode;

    SataEspEnable    = PcdGet64 (PcdSataMultiDiePortESP);
    SataPortShutdown = PcdGet64 (PcdSataMultiDiePortShutDown);
    SataPortMode     = PcdGet64 (PcdSataIoDie0PortMode);
    SataSgpioEnable  = PcdGet8 (PcdSataSgpioMultiDieEnable);

    for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
      FchParams->Sata[SataController].SataEspPort   = (UINT8)(SataEspEnable >> (8 * SataController));
      FchParams->Sata[SataController].SataPortPower = (UINT8)(SataPortShutdown >> (8 * SataController));
      FchParams->Sata[SataController].SataPortMd    = (UINT16)(SataPortMode >> (16 * SataController));
      FchParams->Sata[SataController].SataSgpio0    = (UINT8)((SataSgpioEnable >> SataController) & BIT0);
    }
  }

  FchParams->Hpet.HpetEnable              = PcdGetBool (PcdHpetEnable);
  FchParams->Hpet.HpetMsiDis              = PcdGetBool (PcdHpetMsiDis);
  FchParams->Hpet.HpetBase                = 0xFED00000;

  FchParams->HwAcpi.SpreadSpectrum        = PcdGetBool (PcdSpreadSpectrum);
  FchParams->HwAcpi.WatchDogTimerBase     = 0xFEB00000;
  FchParams->Misc.NoneSioKbcSupport       = PcdGetBool (PcdNoneSioKbcSupport);
  FchParams->HwAcpi.PwrFailShadow         = PcdGet8 (PcdPwrFailShadow);
  if (PcdGetBool (PcdMpmEnable)) {
    FchParams->HwAcpi.PwrFailShadow       = 01;
  }
  FchParams->HwAcpi.StressResetMode       = PcdGet8 (PcdStressResetMode);
  FchParams->HwAcpi.NoClearThermalTripSts = PcdGetBool (PcdNoClearThermalTripSts);
  FchParams->HwAcpi.FchAoacProgramEnable  = PcdGetBool (PcdFchAoacInitEnable);
  FchParams->HwAcpi.FchHfpEnable          = PcdGetBool (PcdFchHfpEnable);
  FchParams->HwAcpi.FchAlinkRasSupport    = PcdGetBool (PcdAmdFchAlinkRasSupport);
  FchParams->HwAcpi.OemProgrammingTablePtr     = PcdGetPtr (PcdOemProgrammingTablePtr);
  FchParams->Gpp.SerialDebugBusEnable     = PcdGetBool (PcdSerialDebugBusEnable);
  FchParams->Gcpu.TimerTickTrack          = PcdGet8 (PcdTimerTickTrack);
  FchParams->Gcpu.ClockInterruptTag       = PcdGet8 (PcdClockInterruptTag);
  FchParams->Misc.NativePcieSupport       = PcdGetBool (PcdNativePcieSupport);
  FchParams->Misc.FchCsSupport.FchModernStandby          = PcdGetBool (PcdFchMiscModernStanbyEnable);
  FchParams->Misc.AmdEnvironmentFlag      = PcdGet32 (PcdAmdEnvironmentFlag);
  FchParams->Misc.FchReadyToBootSmi       = PcdGet8 (PcdFchReadyToBootSwSmi);
  FchParams->Spi.DisEspiMasCtlRegWr       = PcdGetBool (PcdDisEspiMasCtlRegWr);

  FchParams->Sd.SdConfig                  = PcdGet8 (PcdSdConfig);
//  FchParams->Sd.SdClockMultiplier         = PcdGetBool (PcdSdClockMultiplier);
//  FchParams->Sd.SdDbgConfig               = PcdGet8 (PcdSdDbgConfig);

  FchParams->Emmc.EmmcEnable              = PcdGet8 (PcdEmmcEnable);
  FchParams->Emmc.EmmcDriverType          = PcdGet8 (PcdEmmcDriverType);
  FchParams->Emmc.EmmcBoot                = PcdGetBool (PcdEmmcBoot);
  FchParams->Emmc.EmmcAdma2Support        = PcdGetBool (PcdEmmcAdma2Support);
  FchParams->Emmc.EmmcAdmaSupport         = PcdGetBool (PcdEmmcAdmaSupport);
  FchParams->Emmc.EmmcSdmaSupport         = PcdGetBool (PcdEmmcSdmaSupport);
  FchParams->Emmc.EmmcA64bSupport         = PcdGetBool (PcdEmmcA64bSupport);
  FchParams->Emmc.EmmcD3Support           = PcdGetBool (PcdEmmcD3Support);

  FchParams->Wifi.WifiWOLEnable           = PcdGetBool (PcdWifiWOLEn);
  FchParams->Wifi.WifiWOBEnable           = PcdGetBool (PcdWifiWOBEn);

  FchParams->Xgbe.XgbeMdio0Enable         = PcdGetBool (PcdXgbeMdio0);
  FchParams->Xgbe.XgbeMdio1Enable         = PcdGetBool (PcdXgbeMdio1);
  FchParams->Xgbe.XgbeSfpEnable           = PcdGetBool (PcdXgbeSfp);
  FchParams->Xgbe.XgbeDisable             = PcdGet8 (PcdXgbeDisable);
  
  FchParams->Xgbe.Port[0].XgbePortConfig  = PcdGetBool (PcdXgbePort0ConfigEn);
  if (FchParams->Xgbe.Port[0].XgbePortConfig) {
    PcdDataPtr = PcdGetPtr (PcdXgbePort0Table);
    gBS->CopyMem (
           &FchParams->Xgbe.Port[0],
           PcdDataPtr,
           sizeof (FCH_XGBE_PORT)
           );
  }
  FchParams->Xgbe.Port[0].XgbePortConfig  = PcdGetBool (PcdXgbePort0ConfigEn);
  FchParams->Xgbe.Mac[0].XgbePortMacAddressLow  = (UINT32) PcdGet64 (PcdXgbePort0MAC);
  FchParams->Xgbe.Mac[0].XgbePortMacAddressHigh = (UINT32) RShiftU64 (PcdGet64 (PcdXgbePort0MAC), 32);

  FchParams->Xgbe.Port[1].XgbePortConfig  = PcdGetBool (PcdXgbePort1ConfigEn);
  if (FchParams->Xgbe.Port[1].XgbePortConfig) {
    PcdDataPtr = PcdGetPtr (PcdXgbePort1Table);
    gBS->CopyMem (
           &FchParams->Xgbe.Port[1],
           PcdDataPtr,
           sizeof (FCH_XGBE_PORT)
           );
  }
  FchParams->Xgbe.Port[1].XgbePortConfig  = PcdGetBool (PcdXgbePort1ConfigEn);
  FchParams->Xgbe.Mac[1].XgbePortMacAddressLow  = (UINT32) PcdGet64 (PcdXgbePort1MAC);
  FchParams->Xgbe.Mac[1].XgbePortMacAddressHigh = (UINT32) RShiftU64 (PcdGet64 (PcdXgbePort1MAC), 32);

  FchParams->Xgbe.Port[2].XgbePortConfig  = PcdGetBool (PcdXgbePort2ConfigEn);
  if (FchParams->Xgbe.Port[2].XgbePortConfig) {
    PcdDataPtr = PcdGetPtr (PcdXgbePort2Table);
    gBS->CopyMem (
           &FchParams->Xgbe.Port[2],
           PcdDataPtr,
           sizeof (FCH_XGBE_PORT)
           );
  }
  FchParams->Xgbe.Port[2].XgbePortConfig  = PcdGetBool (PcdXgbePort2ConfigEn);
  FchParams->Xgbe.Mac[2].XgbePortMacAddressLow  = (UINT32) PcdGet64 (PcdXgbePort2MAC);
  FchParams->Xgbe.Mac[2].XgbePortMacAddressHigh = (UINT32) RShiftU64 (PcdGet64 (PcdXgbePort2MAC), 32);

  FchParams->Xgbe.Port[3].XgbePortConfig  = PcdGetBool (PcdXgbePort3ConfigEn);
  if (FchParams->Xgbe.Port[3].XgbePortConfig) {
    PcdDataPtr = PcdGetPtr (PcdXgbePort3Table);
    gBS->CopyMem (
           &FchParams->Xgbe.Port[3],
           PcdDataPtr,
           sizeof (FCH_XGBE_PORT)
           );
  }
  FchParams->Xgbe.Port[3].XgbePortConfig  = PcdGetBool (PcdXgbePort3ConfigEn);
  FchParams->Xgbe.Mac[3].XgbePortMacAddressLow  = (UINT32) PcdGet64 (PcdXgbePort3MAC);
  FchParams->Xgbe.Mac[3].XgbePortMacAddressHigh = (UINT32) RShiftU64 (PcdGet64 (PcdXgbePort3MAC), 32);

  FchParams->FchRunTime.FchDeviceEnableMap = PcdGet32 (FchRTDeviceEnableMap);
  FchParams->FchRunTime.I3CMode = PcdGet8 (FchI3CMode);
  FchParams->FchRunTime.PcieMmioBase       = (UINT32) PcdGet64 (PcdPciExpressBaseAddress);
  if (FchParams->FchRunTime.FchDeviceEnableMap & BIT11) {
    UartLegacy[0] = PcdGet8 (FchUart0LegacyEnable);
  } else {
    UartLegacy[0] = 0;
  }
  if (FchParams->FchRunTime.FchDeviceEnableMap & BIT12) {
    UartLegacy[1] = PcdGet8 (FchUart1LegacyEnable);
  } else {
    UartLegacy[1] = 0;
  }
  if (FchParams->FchRunTime.FchDeviceEnableMap & BIT16) {
    UartLegacy[2] = PcdGet8 (FchUart2LegacyEnable);
  } else {
    UartLegacy[2] = 0;
  }
  if (FchParams->FchRunTime.FchDeviceEnableMap & BIT26) {
    UartLegacy[3] = PcdGet8 (FchUart3LegacyEnable);
  } else {
    UartLegacy[3] = 0;
  }

  IoEnable16 = 0;
  for (UartChannel = 0; UartChannel < 4; UartChannel++ ) {
    if (UartLegacy[UartChannel]) {
      IoEnable16 |= (BIT0 << (UartLegacy[UartChannel] - 1)) +
                    (UartChannel << (8 + ((UartLegacy[UartChannel] - 1) * 2)));
    }
  }

  // PLAT-86388
  DEBUG ((
    DEBUG_INFO,
    "[%a] Override UART settings. FchDeviceEnableMap=%x, PcdHspUartEnable=%x, PcdHspUartPort=%x\n",
    __FUNCTION__,
    FchParams->FchRunTime.FchDeviceEnableMap,
    PcdGetBool (PcdHspUartEnable),
    PcdGet8 (PcdHspUartPort)
    ));
  if ( PcdGetBool (PcdHspUartEnable) ) {
    switch ( PcdGet8 (PcdHspUartPort) ) {
      case 0:
        FchParams->FchRunTime.FchDeviceEnableMap |= BIT11;  // UART0
        FchParams->FchRunTime.UartOwnedByHSP |= BIT0;  // UART0
        break;
      case 1:
        FchParams->FchRunTime.FchDeviceEnableMap |= BIT12;  // UART1
        FchParams->FchRunTime.UartOwnedByHSP |= BIT1;  // UART1
        break;
      case 2:
        FchParams->FchRunTime.FchDeviceEnableMap |= BIT16;  // UART2
        FchParams->FchRunTime.UartOwnedByHSP |= BIT2;  // UART2
        break;
      case 3:
        FchParams->FchRunTime.FchDeviceEnableMap |= BIT26;  // UART3
        FchParams->FchRunTime.UartOwnedByHSP |= BIT3;  // UART3
        break;
      default:
        break;
    }
  }

  if (PcdGetBool (PcdSpiCustomConfig)) {
    FchParams->FchRunTime.FchDeviceEnableMap |= BIT28;
  }

  DEBUG ((
    DEBUG_INFO,
    "[%a] Final value of FchDeviceEnableMap=%x\n",
    __FUNCTION__,
    FchParams->FchRunTime.FchDeviceEnableMap
    ));

  FchParams->FchRunTime.Al2AhbLegacyUartIoEnable = IoEnable16;
  FchParams->FchRunTime.FchDeviceD3ColdMap = PcdGet32 (FchRTD3ColdEnableMap);

  FchParams->Smbus.SmbusSsid              = PcdGet32 (PcdSmbusSsid);
  FchParams->Sata[0].SataAhciSsid            = PcdGet32 (PcdSataAhciSsid);
  FchParams->Sata[0].SataRaid5Ssid           = PcdGet32 (PcdSataRaid5Ssid);
  FchParams->Sata[0].SataRaidSsid            = PcdGet32 (PcdSataRaidSsid);
  FchParams->Sata[1].SataAhciSsid            = PcdGet32 (PcdSataAhciSsid);
  FchParams->Sata[1].SataRaid5Ssid           = PcdGet32 (PcdSataRaid5Ssid);
  FchParams->Sata[1].SataRaidSsid            = PcdGet32 (PcdSataRaidSsid);
  FchParams->Spi.LpcSsid                  = PcdGet32 (PcdLpcSsid);
  FchParams->Sd.SdSsid                    = PcdGet32 (PcdSdSsid);
  FchParams->Usb.XhciSsid                 = PcdGet32 (PcdXhciSsid);

  FchParams->FchRunTime.Uart0Irq          = PcdGet8(PcdFchUart0Irq);
  FchParams->FchRunTime.Uart1Irq          = PcdGet8(PcdFchUart1Irq);
  FchParams->FchRunTime.Uart2Irq          = PcdGet8(PcdFchUart2Irq);
  FchParams->FchRunTime.Uart3Irq          = PcdGet8(PcdFchUart3Irq);
  FchParams->FchRunTime.Uart4Irq          = PcdGet8(PcdFchUart4Irq);
  FchParams->FchRunTime.I2c0Irq           = PcdGet8(PcdFchI2c0Irq);
  FchParams->FchRunTime.I2c1Irq           = PcdGet8(PcdFchI2c1Irq);
  FchParams->FchRunTime.I2c2Irq           = PcdGet8(PcdFchI2c2Irq);
  FchParams->FchRunTime.I2c3Irq           = PcdGet8(PcdFchI2c3Irq);
  FchParams->FchRunTime.I2c4Irq           = PcdGet8(PcdFchI2c4Irq);

  FchParams->FchRunTime.HidControl[0]        = *((HID_CONTROL *) PcdGetPtr (PcdFchHidControl));
  FchParams->FchRunTime.HidControl[1]        = *((HID_CONTROL *) PcdGetPtr (PcdFchHidControl) + 1 );

  return FchParams;
}

//
// Driver Global Data
//

/*********************************************************************************
 * Name: FchDxeInit
 *
 * Description
 *   Entry point of the AMD FCH DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  FCH_DXE_PRIVATE     *FchPrivate;
  EFI_STATUS          Status;
  EFI_HANDLE          Handle;
  FCH_DATA_BLOCK      *FchDataBlock;
  EFI_EVENT           PciIoEvent;
  EFI_EVENT           Event;
  VOID                *Registration;

  AGESA_TESTPOINT (TpFchDxeEntry, NULL);
  //
  // Initialize EFI library
  //

  //
  // Initialize the configuration structure and private data area
  //
  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (FCH_DXE_PRIVATE),
                  &FchPrivate
                  );

  ASSERT_EFI_ERROR (Status);

  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (FCH_DATA_BLOCK),
                  &FchDataBlock
                  );

  ASSERT_EFI_ERROR (Status);

  FchDataBlock = FchInitDataBlock (FchDataBlock);
  // Initialize the private data structure
  FchPrivate->Signature = FCH_DXE_PRIVATE_DATA_SIGNATURE;
  // Initialize the FCHInit protocol
  FchPrivate->FchInit.Revision             = FCH_INIT_REV;
  FchPrivate->FchInit.FchRev               = FCH_VERSION_KEITH;
  FchPrivate->FchInit.FchPolicy            = (VOID*) FchDataBlock;
  FchPrivate->FchInit.FpUsbPortDisable2    = FchUsbDisablePort;

  //
  // Publish the FCHInit protocol
  //
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gFchInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &FchPrivate->FchInit
                  );

  if (EFI_ERROR (Status)) {
    return (Status);
  }

  //
  // Enviroment Init Entry
  //
  DEBUG ((DEBUG_INFO, "[FchInitEnv] Fch Init - Before PCI Scan ...Start\n"));
  Status = FchInitEnv (FchDataBlock);
  DEBUG ((DEBUG_INFO, "[FchInitEnv] Fch Init - Before PCI Scan ...complete\n"));

  Status = ProgramFchiLa1MTraceMemoryEn ();

  PcdSet32S (FchRTD3ColdEnableMap,FchDataBlock->FchRunTime.FchDeviceD3ColdMap);

  //
  // Register the event handling function for FchInitMid to be launched after
  // PciIo protocol
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             InvokeFchInitMid,
             FchPrivate->FchInit.FchPolicy,
             NULL,
             &PciIoEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciIoProtocolGuid,
              PciIoEvent,
              &Registration
              );

  //
  // Register the event handling function for FchInitLate to be launched after
  // Ready to Boot
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             InvokeFchInitLate,
             FchPrivate->FchInit.FchPolicy,
             &FchPrivate->EventReadyToBoot
             );

  //
  // Register the event handling when PCI enumeration completed
  //
  Event = NULL;
  Event = EfiCreateProtocolNotifyEvent (
            &gEfiPciEnumerationCompleteProtocolGuid,
            TPL_NOTIFY,
            FchPciEnumerationCompleteCallBack,
            FchPrivate->FchInit.FchPolicy,
            &Registration
            );
  ASSERT (Event != NULL);

  //
  // Install gFchInitDonePolicyProtocolGuid to signal Platform
  //
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gFchInitDonePolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );

  AGESA_TESTPOINT (TpFchDxeExit, NULL);
  return (Status);
}

EFI_STATUS
EFIAPI
FchUsbDisablePort (
  IN       CONST FCH_INIT_PROTOCOL   *This,
  IN       UINT8                     Socket,
  IN       UINT32                    USB3DisableMap,
  IN       UINT32                    USB2DisableMap
  )
{
  UINT32                      Usb2PortDisable;
  UINT32                      Usb3PortDisable;
  UINT32                      UsbPortDisable;

  Usb2PortDisable = USB2DisableMap;
  Usb3PortDisable = USB3DisableMap;

  DEBUG ((DEBUG_INFO,
          "[FCH]FchEnableUsbPort with XhciUsb3PortDisable = %x, XhciUsb2PortDisable = %x\n",
          Usb3PortDisable,
          Usb2PortDisable));

  // Port0-5 : Controller0
  UsbPortDisable = (Usb2PortDisable & 0x0F) + ((Usb3PortDisable & 0x3) << 16);
  DEBUG ((DEBUG_INFO,
          "[FCH]FchEnableUsbPort Update USB0 %x with %x %x\n",
          FCH_KT_USB0_SMN_BASE+FCH_KT_USB_PORT_DISABLE0,
          ~ (UINT32) (0x0003000F),
          UsbPortDisable));
  FchSmnRW (0, FCH_KT_USB0_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x0003000f), UsbPortDisable, NULL);

  // Port0-4 : Controller1
  Usb3PortDisable = Usb3PortDisable >> 2;
  Usb2PortDisable = Usb2PortDisable >> 4;
  UsbPortDisable = (Usb2PortDisable & 0x07) + ((Usb3PortDisable & 0x3) << 16);
  DEBUG ((DEBUG_INFO,
          "[FCH]FchEnableUsbPort Update USB1 %x with %x %x\n",
          FCH_KT_USB1_SMN_BASE+FCH_KT_USB_PORT_DISABLE0,
          ~ (UINT32) (0x00030007),
          UsbPortDisable));
  FchSmnRW (0, FCH_KT_USB1_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x00030007), UsbPortDisable, NULL);

  // Port0 : Controller2
  Usb3PortDisable = Usb3PortDisable >> 2;
  Usb2PortDisable = Usb2PortDisable >> 4;
  UsbPortDisable = (Usb2PortDisable & 0x01);
  DEBUG ((DEBUG_INFO,
          "[FCH]FchEnableUsbPort Update USB2 %x with %x %x\n",
          FCH_KT_USB2_SMN_BASE+FCH_KT_USB_PORT_DISABLE0,
          ~ (UINT32) (0x00000001),
          UsbPortDisable));
  FchSmnRW (0, FCH_KT_USB2_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x00000001), UsbPortDisable, NULL);

  if (FchCheckRmbB0()) {
    // Port0 : Controller3
    Usb3PortDisable = Usb3PortDisable >> 2;
    Usb2PortDisable = Usb2PortDisable >> 4;
    UsbPortDisable = (Usb2PortDisable & 0x01) + ((Usb3PortDisable & 0x1) << 16);
    DEBUG ((DEBUG_INFO,
            "[FCH]FchEnableUsbPort Update USB3 %x with %x %x\n",
            FCH_KT_USB3_SMN_BASE+FCH_KT_USB_PORT_DISABLE0,
            ~ (UINT32) (0x00010001),
            UsbPortDisable));
    FchSmnRW (0, FCH_KT_USB3_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x00010001), UsbPortDisable, NULL);

    // Port0 : Controller4
    Usb3PortDisable = Usb3PortDisable >> 2;
    Usb2PortDisable = Usb2PortDisable >> 4;
    UsbPortDisable = (Usb2PortDisable & 0x01) + ((Usb3PortDisable & 0x1) << 16);
    DEBUG ((DEBUG_INFO,
            "[FCH]FchEnableUsbPort Update USB4 %x with %x %x\n",
            FCH_KT_USB4_SMN_BASE+FCH_KT_USB_PORT_DISABLE0,
            ~ (UINT32) (0x00010001),
            UsbPortDisable));
    FchSmnRW (0, FCH_KT_USB4_SMN_BASE+FCH_KT_USB_PORT_DISABLE0, ~ (UINT32) (0x00010001), UsbPortDisable, NULL);
  }

  return EFI_SUCCESS;
}
