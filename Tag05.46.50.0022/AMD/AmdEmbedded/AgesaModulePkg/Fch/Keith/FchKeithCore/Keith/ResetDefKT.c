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
 * Default FCH interface settings at InitReset phase.
 *----------------------------------------------------------------------------------------
 */
CONST FCH_RESET_INTERFACE ROMDATA FchResetInterfaceDefault = {
  TRUE,                // UmiGen2
  TRUE,                // SataEnable
  TRUE,                // IdeEnable
  TRUE,                // GppEnable
  TRUE,                // Xhci0Enable
  TRUE                 // Xhci1Enable
};


/*----------------------------------------------------------------
 *  InitReset Phase Data Block Default (Failsafe)
 *----------------------------------------------------------------
 */
FCH_RESET_DATA_BLOCK   InitResetCfgDefault = {
  NULL,                  // StdHeader
  { TRUE,
    TRUE,
    FALSE,
    FALSE,
    TRUE,
    TRUE
    },                   // FchReset

  0,                     // FastSpeed
  3,                     // WriteSpeed
  0,                     // SpiTpmSpeed
  0,                     // Mode
  FALSE,                 // SpiClkEarlier
  0,                     // AutoMode
  0,                     // BurstWrite
  FALSE,                 // SataIdeCombMdPriSecOpt
  0,                     // Cg2Pll
  FALSE,                 // EcKbd
  FALSE,                 // LegacyFree
  FALSE,                 // SataSetMaxGen2
  1,                     // SataClkMode
  0,                     // SataModeReg
  FALSE,                 // SataInternal100Spread
  2,                     // SpiSpeed
//  0xFCFCFCFC,                     // 38
//  0x88FC,                     // 3c
//  0,                     // 1d_34
  1,                     // 20_0
  FALSE,                 // EcChannel0

  {                      // FCH_GPP
    {                    // Array of FCH_GPP_PORT_CONFIG       PortCfg[4]
      {
        FALSE,           // PortPresent
        FALSE,           // PortDetected
        FALSE,           // PortIsGen2
        FALSE,           // PortHotPlug
        0,               // PortMisc
      },
      {
        FALSE,           // PortPresent
        FALSE,           // PortDetected
        FALSE,           // PortIsGen2
        FALSE,           // PortHotPlug
        0,               // PortMisc
      },
      {
        FALSE,           // PortPresent
        FALSE,           // PortDetected
        FALSE,           // PortIsGen2
        FALSE,           // PortHotPlug
        0,               // PortMisc
      },
      {
        FALSE,           // PortPresent
        FALSE,           // PortDetected
        FALSE,           // PortIsGen2
        FALSE,           // PortHotPlug
        0,               // PortMisc
      },
    },
    PortA1B1C1D1,        // GppLinkConfig
    FALSE,               // GppFunctionEnable
    FALSE,               // GppToggleReset
    0,                   // GppHotPlugGeventNum
    0,                   // GppFoundGfxDev
    FALSE,               // GppGen2
    0,                   // GppGen2Strap
    FALSE,               // GppMemWrImprove
    FALSE,               // GppUnhidePorts
    0,                   // GppPortAspm
    FALSE,               // GppLaneReversal
    FALSE,               // GppPhyPllPowerDown
    FALSE,               // GppDynamicPowerSaving
    FALSE,               // PcieAer
    FALSE,               // PcieRas
    FALSE,               // PcieCompliance
    FALSE,               // PcieSoftwareDownGrade
    FALSE,               // UmiPhyPllPowerDown
    FALSE,               // SerialDebugBusEnable
    0,                   // GppHardwareDownGrade
    0,                   // GppL1ImmediateAck
    FALSE,               // NewGppAlgorithm
    0,                   // HotPlugPortsStatus
    0,                   // FailPortsStatus
    40,                  // GppPortMinPollingTime
    FALSE,               // IsCapsuleMode
  },
  {                      // FCH_SPI
    FALSE,               // LpcMsiEnable
    0x00000000,          // LpcSsid
    0,                   // RomBaseAddress
    0,                   // Speed
    0,                   // FastSpeed
    0,                   // WriteSpeed
    0,                   // Mode
    0,                   // AutoMode
    0,                   // BurstWrite
    TRUE,                // LpcClk0
    TRUE,                // LpcClk1
    0,                   // SPI100_Enable
    {0},                 // SpiDeviceProfile
    FALSE,               // DisEspiMasCtlRegWr
  },
  FALSE,                 // QeEnabled
  FALSE,                 // FCH OSCOUT1_CLK Continous
  0,                     // LpcClockDriveStrength
  0,                     // USB3 ECC SMI control
  0xFF,                  // USB3 Controller0 Port Num
  0xFF,                  // USB3 Controller1 Port Num
  0x01B3C953,            // USB3 LANEPARACTL0
  FALSE,                 // EspiEnable
  FALSE,                 // EspiIo80Enable
  FALSE,                 // EspiKbc6064Enable
  FALSE,                 // EspiDevice0Enable
  TRUE,                  // WdtEnable
//  NULL,                  // OemResetProgrammingTablePtr
  {
    {0,0,0,0,0,0},         // XHCI0 P0 Phy Parameters
    {0,0,0,0,0,0},         // XHCI0 P1 Phy Parameters
    {0,0,0,0,0,0},         // XHCI0 P2 Phy Parameters
    {0,0,0,0,0,0},         // XHCI0 P3 Phy Parameters
  },
  0,                     //  Xhci0DevRemovable
  0xFED80000,            //  FchAcpiMmioBase
  FALSE,                 //  XhciOcPolarityCfgLow
  0x00,                  //  XhciUsb3PortDisable
  0x00,                  //  XhciUsb2PortDisable
  0xFFFFFFFF,            //  XhciOCpinSelect
  0xFFFFFFFF,            //  Xhci1OCpinSelect
  0xFFFFFFFF,            //  Xhci2OCpinSelect
  0xFFFFFFFF,            //  Xhci3OCpinSelect
  0xFFFFFFFF,            //  Xhci4OCpinSelect
  0x00000000,            //  XhciUsbDdiModeEnable
  0x00,                  //  Usb3PortForceGen1
  TRUE,                  //  Xhci2Enable
  {TRUE, TRUE, TRUE, TRUE},  //Sata controller enable.
//  NULL,                  // EarlyOemGpioTable
  TRUE,                  // ToggleAllPwrGoodOnCf9
  TRUE,                  // SerialIrqEnable
  {//ESPI
    0,                   //  OperatingFreq
    0,                   //  IoMode
    {
      {
        {
          TRUE,          //  IoRange0Enable
          0x4F,          //  IoRange0Base
          0x00,          //  IoRange0Size
        },
        {
          TRUE,          //  IoRange1Enable
          0x4E,          //  IoRange1Base
          0x00,          //  IoRange1Size
        },
        {
          TRUE,          //  IoRange2Enable
          0x600,         //  IoRange2Base
          0xFF,          //  IoRange2Size
        },
        {
          TRUE,          //  IoRange3Enable
          0x4C,          //  IoRange3Base
          0x00,          //  IoRange3Size
        },
        {
          FALSE,         //  IoRange4Enable
          0x00,          //  IoRange4Base
          0x00,          //  IoRange4Size
        },
        {
          FALSE,         //  IoRange5Enable
          0x00,          //  IoRange5Base
          0x00,          //  IoRange5Size
        },
        {
          FALSE,         //  IoRange6Enable
          0x00,          //  IoRange6Base
          0x00,          //  IoRange6Size
        },
        {
          FALSE,         //  IoRange7Enable
          0x00,          //  IoRange7Base
          0x00,          //  IoRange7Size
        },
        {
          FALSE,         //  IoRange8Enable
          0x00,          //  IoRange8Base
          0x00,          //  IoRange8Size
        },
        {
          FALSE,         //  IoRange9Enable
          0x00,          //  IoRange9Base
          0x00,          //  IoRange9Size
        },
        {
          FALSE,         //  IoRange10Enable
          0x00,          //  IoRange10Base
          0x00,          //  IoRange10Size
        },
        {
          FALSE,         //  IoRange11Enable
          0x00,          //  IoRange11Base
          0x00,          //  IoRange11Size
        },
        {
          FALSE,         //  IoRange12Enable
          0x00,          //  IoRange12Base
          0x00,          //  IoRange12Size
        },
        {
          FALSE,         //  IoRange13Enable
          0x00,          //  IoRange13Base
          0x00,          //  IoRange13Size
        },
        {
          FALSE,         //  IoRange14Enable
          0x00,          //  IoRange14Base
          0x00,          //  IoRange14Size
        },
        {
          FALSE,         //  IoRange15Enable
          0x00,          //  IoRange15Base
          0x00,          //  IoRange15Size
        }
      },
      {
        {
          FALSE,         //  MmioRange0Enable
          0x00000000,    //  MmioRange0Base
          0x0000,        //  MmioRange0Size
        },
        {
          FALSE,         //  MmioRange1Enable
          0x00000000,    //  MmioRange1Base
          0x0000,        //  MmioRange1Size
        },
        {
          FALSE,         //  MmioRange2Enable
          0x00000000,    //  MmioRange2Base
          0x0000,        //  MmioRange2Size
        },
        {
          FALSE,         //  MmioRange3Enable
          0x00000000,    //  MmioRange3Base
          0x0000,        //  MmioRange3Size
        },
        {
          FALSE,         //  MmioRange4Enable
          0x00000000,    //  MmioRange4Base
          0x0000,        //  MmioRange4Size
        }
      }
    },
    0,                   //  IrqPolarity
    0x00FFFFFF,          //  IrqMask
    0xF,                 //  Channel
    0x1                  //  AlertMode
  },
  TRUE,                  //  BpX48M0ClockEnable
  FALSE,                 //  TurnOffXtalS3S5
  TRUE,                  //  DisableXhciPortLate
  0,                     //  AmdEnvironmentFlag
  {0},                   // OemUsbConfigurationTable
  {                      // FCH_USB4
    {
      TRUE,                  // Usb4InitEnable
      TRUE,                  // Usb4HostEnable
      0,                     // Usb3HCDisable
      0,                     // Usb4PcieTunnelingDisable
      0,                     // Usb4Usb3TunnelingDisable
      FALSE,                 // Usb4DPTunnelingDisable
      FALSE,                 // Usb4Tbt3NotSupport
      0,                     // Usb4PcieAdpHidden
      0,                     // Usb4Usb3AdpHidden
      0,                     // Usb4DPAdpHidden
      FALSE,                 // Usb4Gen3Disable
      0x1,                   // Usb4PhyEnable
      0x1,                   // Usb4Gen3Support
      0x00,                  // Usb4TxFFEMode
      0xFF,                  // TxFFEPreSetValue
      {0},                   // Reserved
    },
    {
      TRUE,                  // Usb4InitEnable
      TRUE,                  // Usb4HostEnable
      0,                     // Usb3HCDisable
      0,                     // Usb4PcieTunnelingDisable
      0,                     // Usb4Usb3TunnelingDisable
      FALSE,                 // Usb4DPTunnelingDisable
      FALSE,                 // Usb4Tbt3NotSupport
      0,                     // Usb4PcieAdpHidden
      0,                     // Usb4Usb3AdpHidden
      0,                     // Usb4DPAdpHidden
      FALSE,                 // Usb4Gen3Disable
      0x1,                   // Usb4PhyEnable
      0x1,                   // Usb4Gen3Support
      0x00,                  // Usb4TxFFEMode
      0xFF,                  // TxFFEPreSetValue
      {0},                   // Reserved
    },
  },
  FALSE,                 // Xtal48MPadPowerSaving
  0,                     // PdInterruptModeEn
  FALSE,                 // Usb4PhyTuningEnable
  0,                     // Usb4PhyTuningSetPtr
  0,                     // Usb4PhyTuningSetSize
  0,                     // DeviceEnableMap
  FALSE,                 // I2cI3cEarlyInit
  TRUE,                  //  UsbSparseModeEnable
  {0},                   // FchBldCfg
  NULL,                  // OemUsbConfigurationTablePtr
  0xFF,                  // i2c4_padctrl_i2crxsel

};
