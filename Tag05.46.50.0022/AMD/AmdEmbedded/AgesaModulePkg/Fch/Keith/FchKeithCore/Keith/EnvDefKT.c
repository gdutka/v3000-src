/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "FchPlatform.h"
#include  "Filecode.h"
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 * Default FCH interface settings at InitEnv phase.
 *----------------------------------------------------------------------------------------
 */
CONST FCH_INTERFACE FchInterfaceDefault = {
  SdVer3,              // SdConfig
  AzEnable,            // AzaliaControl
  IrRxTx0Tx1,          // IrConfig
  TRUE,                // UmiGen2
  0,                   // SataClass
  TRUE,                // SataEnable
  TRUE,                // IdeEnable
  TRUE,                // SataIdeMode
  TRUE,                // Ohci1Enable
  TRUE,                // Ohci2Enable
  TRUE,                // Ohci3Enable
  TRUE,                // Ohci4Enable
  TRUE,                // GppEnable
  AlwaysOff            // FchPowerFail
};


/*----------------------------------------------------------------
 *  InitEnv Phase Data Block Default (Failsafe)
 *----------------------------------------------------------------
 */
FCH_DATA_BLOCK   InitEnvCfgDefault = {
  {
    0xF8000000,              ///< PcieMmioBase
    0,                       ///< FchDeviceEnableMap
    0,                       ///< FchDeviceD3ColdMap //D3Cold of XHCI is enabled BIT23
    0,                       ///< XhciRomSourceBase
    0,                       ///< XhciRomTargetBase
    0,                       ///< XhciRamSourceBase
    0,                       ///< XhciRamTargetBase
    0,                       ///< SataDevSlpPort0S5Pin - Reserved
    0,                       ///< SataDevSlpPort1S5Pin - Reserved
    0xE400,                  ///< Al2Ahb Legacy Uart Io Enable
    0,                       ///< I3CMode - BIT[3:0] for channel [3:0], 0:I3C,1:I2C
    0x03,                    ///< Uart 0 Irq
    0x04,                    ///< Uart 1 Irq
    0x03,                    ///< Uart 2 Irq
    0x04,                    ///< Uart 3 Irq
    0x0F,                    ///< Uart 4 Irq
    0x0A,                    ///< I2c 0 Irq
    0x0B,                    ///< I2c 1 Irq
    0x04,                    ///< I2c 2 Irq
    0x06,                    ///< I2c 3 Irq
    0x0E,                    ///< I2c 4 Irq

  },
  NULL,                      // StdHeader

  {                          // FCH_ACPI
    0xB00,                   // Smbus0BaseAddress
    0xB20,                   // Smbus1BaseAddress
    0xE00,                   // SioPmeBaseAddress
    0xFEC000F0,              // WatchDogTimerBase
    0x400,                   // AcpiPm1EvtBlkAddr
    0x404,                   // AcpiPm1CntBlkAddr
    0x408,                   // AcpiPmTmrBlkAddr
    0x410,                   // CpuControlBlkAddr
    0x420,                   // AcpiGpe0BlkAddr
    0x00B0,                  // SmiCmdPortAddr
    0xFE00,                  // AcpiPmaCntBlkAddr
    TRUE,                    // AnyHt200MhzLink
    TRUE,                    // SpreadSpectrum
    UsePrevious,             // PwrFailShadow
    0,                       // StressResetMode
    FALSE,                   // MtC1eEnable
    NULL,                    // OemProgrammingTablePtr
    1,                       // SpreadSpectrumOptions
    FALSE,                   // PwrDownDisp2ClkPcieR
    FALSE,                   // NoClearThermalTripSts
    0xFED80000,              // FchAcpiMmioBase
    FALSE,                   // FchAlinkRasSupport
    FALSE,                   // FchAoacProgramEnable
    FALSE,                   // FchHfpEnable
  },

  {                          // FCH_AB
    FALSE,                   // AbMsiEnable
    1,                       // ALinkClkGateOff
    1,                       // BLinkClkGateOff
    3,                       // GPP Clock Request.
    3,                       // GPP Clock Request.
    3,                       // GPP Clock Request.
    3,                       // GPP Clock Request.
    1,                       // GPP Clock Request.
    1,                       // AbClockGating
    0,                       // GppClockGating
    0,                       // UmiL1TimerOverride
    0,                       // UmiLinkWidth
    0,                       // UmiDynamicSpeedChange
    0,                       // PcieRefClockOverClocking
    0,                       // UmiGppTxDriverStrength
    FALSE,                   // NbSbGen2
    0,                       // FchPcieOrderRule
    1,                       // SlowSpeedAbLinkClock
//    TRUE,                    // ResetCpuOnSyncFlood
    FALSE,                    // ResetCpuOnSyncFlood
    TRUE,                    // AbDmaMemoryWrtie3264B
    TRUE,                    // AbMemoryPowerSaving
    FALSE,                   // SbgDmaMemoryWrtie3264ByteCount
    TRUE,                    // SbgMemoryPowerSaving
    TRUE,                    // SBG Clock Gate
    FALSE,                   // XDMA DMA Write 16 byte mode
    FALSE,                   // XDMA memory power saving
    0,                       // XDMA PENDING NPR THRESHOLD
    FALSE,                   // XDMA DNCPL ORDER DIS
    3,                       // SLT GFX Clock Request.
    3,                       // SLT GFX Clock Request.
    FALSE,                   // SdphostBypassDataPack
    FALSE,                   // SdphostDisNpmwrProtect
  },

  {                          // FCH_GPP
    {                        // Array of FCH_GPP_PORT_CONFIG       PortCfg[4]
      {
        FALSE,               // PortPresent
        FALSE,               // PortDetected
        FALSE,               // PortIsGen2
        FALSE,               // PortHotPlug
        0,                   // PortMisc
      },
      {
        FALSE,               // PortPresent
        FALSE,               // PortDetected
        FALSE,               // PortIsGen2
        FALSE,               // PortHotPlug
        0,                   // PortMisc
      },
      {
        FALSE,               // PortPresent
        FALSE,               // PortDetected
        FALSE,               // PortIsGen2
        FALSE,               // PortHotPlug
        0,                   // PortMisc
      },
      {
        FALSE,               // PortPresent
        FALSE,               // PortDetected
        FALSE,               // PortIsGen2
        FALSE,               // PortHotPlug
        0,                   // PortMisc
      },
    },
    PortA1B1C1D1,            // GppLinkConfig
    FALSE,                   // GppFunctionEnable
    FALSE,                   // GppToggleReset
    0,                       // GppHotPlugGeventNum
    0,                       // GppFoundGfxDev
    FALSE,                   // GppGen2
    0,                       // GppGen2Strap
    FALSE,                   // GppMemWrImprove
    FALSE,                   // GppUnhidePorts
    0,                       // GppPortAspm
    FALSE,                   // GppLaneReversal
    FALSE,                   // GppPhyPllPowerDown
    FALSE,                   // GppDynamicPowerSaving
    FALSE,                   // PcieAer
    FALSE,                   // PcieRas
    FALSE,                   // PcieCompliance
    FALSE,                   // PcieSoftwareDownGrade
    FALSE,                   // UmiPhyPllPowerDown
    FALSE,                   // SerialDebugBusEnable
    0,                       // GppHardwareDownGrade
    0,                       // GppL1ImmediateAck
    FALSE,                   // NewGppAlgorithm
    0,                       // HotPlugPortsStatus
    0,                       // FailPortsStatus
    40,                      // GppPortMinPollingTime
    FALSE,                   // IsCapsuleMode
  },

  {                          // FCH_USB
    TRUE,                    // Ohci1Enable
    TRUE,                    // Ohci2Enable
    TRUE,                    // Ohci3Enable
    TRUE,                    // Ohci4Enable
    TRUE,                    // Ehci1Enable
    TRUE,                    // Ehci2Enable
    TRUE,                    // Ehci3Enable
    FALSE,                   // Xhci0Enable
    FALSE,                   // Xhci1Enable
    FALSE,                   // UsbMsiEnable
    0,                       // OhciSsid
    0,                       // Ohci4Ssid
    0,                       // EhciSsid
    0,                       // XhciSsid
    FALSE,                   // UsbPhyPowerDown
    0,                       // UserDefineXhciRomAddr
    {0x21, 0x21, 0x21, 0x21, 0x22},      // Ehci18Phy
    {0x22, 0x22, 0x22, 0x21, 0x21},      // Ehci19Phy
    {0x21, 0x21, 0x21, 0x21},            // Ehci22Phy
    {0x24, 0x24, 0x21, 0x21},            // Xhci20Phy
    0,                                   // Ehci1DebugPortSel
    0,                                   // Ehci2DebugPortSel
    0,                                   // Ehci3DebugPortSel
    TRUE,                    // SsicEnable
    0                        // SsicSsid
  },

  { //SATA
    {TRUE,                               //SataEnable
     TRUE,                               //SataSetMaxGen2
     FALSE,                              //SataMsiEnable
     0x00000000,                         //SataRaidSsid
     0x00000000,                         //SataRaid5Ssid
     0x00000000,                         //SataAhciSsid
     SataAhci,                           //SataClass
     0,                                  //SataEspPort
     0,                                  //SataPortPower
     0,                                  //SataPortMd
     1,                                  //SataAggrLinkPmCap
     1,                                  //SataPortMultCap
     1,                                  //SataClkAutoOff
     1,                                  //SataPscCap
     0,                                  //BiosOsHandOff
     1,                                  //SataFisBasedSwitching
     0,                                  //SataCccSupport
     1,                                  //SataSscCap
     0,                                  //SataTargetSupport8Device
     0,                                  //SataDisableGenericMode
     FALSE,                              //SataAhciEnclosureManagement
     0,                                  //SataSgpio0
     0,                                  //SataSgpio1
     0,                                  //SataPhyPllShutDown
     FALSE,                              //SataOobDetectionEnh
     FALSE,                              //SataRasSupport
     TRUE,                               //SataAhciDisPrefetchFunction
     FALSE,                              //SataDevSlpPort0
     FALSE,                              //SataDevSlpPort1
     0,                                  //SataDevSlpPort0Num
     0,                                  //SataDevSlpPort1Num
     0,                                  //TempMmio
     FALSE,                              //SataControllerAutoShutdown
     {0},                                //Reserved[18]
    },

    {TRUE,                               //SataEnable
     TRUE,                               //SataSetMaxGen2
     FALSE,                              //SataMsiEnable
     0x00000000,                         //SataRaidSsid
     0x00000000,                         //SataRaid5Ssid
     0x00000000,                         //SataAhciSsid
     SataAhci,                           //SataClass
     0,                                  //SataEspPort
     0,                                  //SataPortPower
     0,                                  //SataPortMd
     1,                                  //SataAggrLinkPmCap
     1,                                  //SataPortMultCap
     1,                                  //SataClkAutoOff
     1,                                  //SataPscCap
     0,                                  //BiosOsHandOff
     1,                                  //SataFisBasedSwitching
     0,                                  //SataCccSupport
     1,                                  //SataSscCap
     0,                                  //SataTargetSupport8Device
     0,                                  //SataDisableGenericMode
     FALSE,                              //SataAhciEnclosureManagement
     0,                                  //SataSgpio0
     0,                                  //SataSgpio1
     0,                                  //SataPhyPllShutDown
     FALSE,                              //SataOobDetectionEnh
     FALSE,                              //SataRasSupport
     TRUE,                               //SataAhciDisPrefetchFunction
     FALSE,                              //SataDevSlpPort0
     FALSE,                              //SataDevSlpPort1
     0,                                  //SataDevSlpPort0Num
     0,                                  //SataDevSlpPort1Num
     0,                                  //TempMmio
     FALSE,                              //SataControllerAutoShutdown
     {0},                                //Reserved[18]
    },

    {TRUE,                               //SataEnable
     TRUE,                               //SataSetMaxGen2
     FALSE,                              //SataMsiEnable
     0x00000000,                         //SataRaidSsid
     0x00000000,                         //SataRaid5Ssid
     0x00000000,                         //SataAhciSsid
     SataAhci,                           //SataClass
     0,                                  //SataEspPort
     0,                                  //SataPortPower
     0,                                  //SataPortMd
     1,                                  //SataAggrLinkPmCap
     1,                                  //SataPortMultCap
     1,                                  //SataClkAutoOff
     1,                                  //SataPscCap
     0,                                  //BiosOsHandOff
     1,                                  //SataFisBasedSwitching
     0,                                  //SataCccSupport
     1,                                  //SataSscCap
     0,                                  //SataTargetSupport8Device
     0,                                  //SataDisableGenericMode
     FALSE,                              //SataAhciEnclosureManagement
     0,                                  //SataSgpio0
     0,                                  //SataSgpio1
     0,                                  //SataPhyPllShutDown
     FALSE,                              //SataOobDetectionEnh
     FALSE,                              //SataRasSupport
     TRUE,                               //SataAhciDisPrefetchFunction
     FALSE,                              //SataDevSlpPort0
     FALSE,                              //SataDevSlpPort1
     0,                                  //SataDevSlpPort0Num
     0,                                  //SataDevSlpPort1Num
     0,                                  //TempMmio
     FALSE,                              //SataControllerAutoShutdown
     {0},                                //Reserved[18]
    },

    {TRUE,                               //SataEnable
     TRUE,                               //SataSetMaxGen2
     FALSE,                              //SataMsiEnable
     0x00000000,                         //SataRaidSsid
     0x00000000,                         //SataRaid5Ssid
     0x00000000,                         //SataAhciSsid
     SataAhci,                           //SataClass
     0,                                  //SataEspPort
     0,                                  //SataPortPower
     0,                                  //SataPortMd
     1,                                  //SataAggrLinkPmCap
     1,                                  //SataPortMultCap
     1,                                  //SataClkAutoOff
     1,                                  //SataPscCap
     0,                                  //BiosOsHandOff
     1,                                  //SataFisBasedSwitching
     0,                                  //SataCccSupport
     1,                                  //SataSscCap
     0,                                  //SataTargetSupport8Device
     0,                                  //SataDisableGenericMode
     FALSE,                              //SataAhciEnclosureManagement
     0,                                  //SataSgpio0
     0,                                  //SataSgpio1
     0,                                  //SataPhyPllShutDown
     FALSE,                              //SataOobDetectionEnh
     FALSE,                              //SataRasSupport
     TRUE,                               //SataAhciDisPrefetchFunction
     FALSE,                              //SataDevSlpPort0
     FALSE,                              //SataDevSlpPort1
     0,                                  //SataDevSlpPort0Num
     0,                                  //SataDevSlpPort1Num
     0,                                  //TempMmio
     FALSE,                              //SataControllerAutoShutdown
     {0},                                //Reserved[18]
    },
  },

  {                          // FCH_SMBUS
    0x00000000               // SmbusSsid
  },

  {                          // FCH_IDE
    TRUE,                    // IdeEnable
    FALSE,                   // IdeMsiEnable
    0x00000000               // IdeSsid
  },

  {                          // FCH_AZALIA
    AzDisable,               // AzaliaEnable
    FALSE,                   // AzaliaMsiEnable
    0x00000000,              // AzaliaSsid
    0,                       // AzaliaPinCfg
    0,                       // AzaliaFrontPanel
    0,                       // FrontPanelDetected
    0,                       // AzaliaSnoop
    0,                       // AzaliaDummy
    {                        // AZALIA_PIN
      0,                     // AzaliaSdin0
      0,                     // AzaliaSdin1
      0,                     // AzaliaSdin2
      0,                     // AzaliaSdin3
    },
    NULL,                    // *AzaliaOemCodecTablePtr
    NULL,                    // *AzaliaOemFpCodecTablePtr
  },

  {                          // FCH_SPI
    FALSE,                   // LpcMsiEnable
    0x00000000,              // LpcSsid
    0,                       // RomBaseAddress
    0,                       // Speed
    0,                       // FastSpeed
    0,                       // WriteSpeed
    0,                       // Mode
    0,                       // AutoMode
    0,                       // BurstWrite
    TRUE,                    // LpcClk0
    TRUE,                    // LpcClk1
    0,                       // SPI100_Enable
    {0},                     // SpiDeviceProfile
    FALSE,                   // DisEspiMasCtlRegWr
  },

  {                          // FCH_PCIB
    FALSE,                   // PcibMsiEnable
    0x00000000,              // PcibSsid
    0x0F,                    // PciClks
    0,                       // PcibClkStopOverride
    FALSE,                   // PcibClockRun
  },

  {                          // FCH_GEC
    FALSE,                   // GecEnable
    0,                       // GecPhyStatus
    0,                       // GecPowerPolicy
    0,                       // GecDebugBus
    0xFED61000,              // GecShadowRomBase
    NULL,                    // *PtrDynamicGecRomAddress
  },

  {                          // FCH_SD
    SdDisable,               // SdConfig
    0,                       // Speed
    0,                       // BitWidth
    0x00000000,              // SdSsid
    Sd50MhzTraceCableLengthWithinSixInches,  // SdClockControl
    FALSE,
    0,
    1,
    3,
    0,                       // SdSlotType
    FALSE,                   // SdForce18
    0,                       // SdDbgConfig
  },

  {0},                       // FCH_HWM

  {0,                        // FCH_IR
   0x23,                     // IrPinControl
  },

  {                          // FCH_HPET
    TRUE,                    // HpetEnable
    TRUE,                    // HpetMsiDis
    0xFED00000               // HpetBase
  },

  {                          // FCH_GCPU
    0,                       // AcDcMsg
    0,                       // TimerTickTrack
    0,                       // ClockInterruptTag
    0,                       // OhciTrafficHanding
    0,                       // EhciTrafficHanding
    0,                       // GcpuMsgCMultiCore
    0,                       // GcpuMsgCStage
  },

  {0},                       // FCH_IMC

  {                          // FCH_MISC
    TRUE,                   // NativePcieSupport
    FALSE,                   // S3Resume
    FALSE,                   // RebootRequired
    0,                       // FchVariant
    0,                       // CG2PLL
    {                        // TIMER_SMI-LongTimer
      FALSE,                 // Enable
      FALSE,                 // StartNow
      1000                   // CycleDuration
    },
    {                        // TIMER_SMI-ShortTimer
      FALSE,                 // Enable
      FALSE,                 // StartNow
      0x7FFF                 // CycleDuration
    },
    0,                       // FchCpuId
    FALSE,                   // NoneSioKbcSupport
    {0},                     // FchCsSupport
    0,                       // AmdEnvironmentFlag -  0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite.
    TRUE,                    // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Enable
    0,                       // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Base
    0xD7,                    // FchReadyToBootSmi - Fch Ready To Boot Smi
  },

  {0, 0},                    // FCH_IOMUX

  NULL,                      // PostOemGpioTable

  {                          // FCH_EMMC
    TRUE,                    // EmmcEnable
    0,                       // BusSpeedMode
    0,                       // BusWidth
    0,                       // RetuneMode
    FALSE,                   // ClockMultiplier
  },

  {                          // FCH_XGBE
    {{0}, {0}, {0}, {0},},   // Port[4]
    FALSE,                   // XgbeMdio0Enable
    FALSE,                   // XgbeMdio1Enable	
    FALSE,                   // XgbeMdio0Enable
    FALSE,                   // XgbeMdio1Enable
    FALSE,                   // XgbeSfpEnable
    0,                       // MaxPortNum
   },

  {                          // FCH_WIFI
    TRUE,                    // WifiWOLEnable
    TRUE,                    // WifiWOBEnable
  },

  {                          // FCH_USB4
    {
      TRUE,                  // Usb4InitEnable
      TRUE,                  // Usb4HostEnable
      FALSE,                 // Usb3HCDisable
      FALSE,                 // Usb4PcieTunnelingDisable
      FALSE,                 // Usb4Usb3TunnelingDisable
      FALSE,                 // Usb4DPTunnelingDisable
      FALSE,                 // Usb4Tbt3NotSupport
      FALSE,                 // Usb4PcieAdpHidden
      FALSE,                 // Usb4Usb3AdpHidden
      FALSE,                 // Usb4DPAdpHidden
      FALSE,                 // Usb4Gen3Disable
      {0},                   // Reserved
    },

    {
      TRUE,                  // Usb4InitEnable
      TRUE,                  // Usb4HostEnable
      FALSE,                 // Usb3HCDisable
      FALSE,                 // Usb4PcieTunnelingDisable
      FALSE,                 // Usb4Usb3TunnelingDisable
      FALSE,                 // Usb4DPTunnelingDisable
      FALSE,                 // Usb4Tbt3NotSupport
      FALSE,                 // Usb4PcieAdpHidden
      FALSE,                 // Usb4Usb3AdpHidden
      FALSE,                 // Usb4DPAdpHidden
      FALSE,                 // Usb4Gen3Disable
      {0},                   // Reserved
    },
  },

  NULL                       // FchResetDataBlock
};



