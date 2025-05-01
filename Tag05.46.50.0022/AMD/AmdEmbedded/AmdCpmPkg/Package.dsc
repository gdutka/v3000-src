## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright 2022 Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]

[LibraryClasses]
  #
  # Cpm specific common libraries
  #
  AmdCpmBaseLib|AmdCpmPkg/Library/Proc/Base/AmdCpm3BaseLib.inf
  AmdCpmCpuLib|AmdCpmPkg/Library/Proc/Cpu/AmdCpm3Cpu.inf
  AmdCpmFchLib|AmdCpmPkg/Library/Proc/Fch/AmdCpm3Fch.inf
  AodPlatformLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AodPlatformLib/AodPlatformLib.inf
  OemClkReqControlLib|AmdCpmPkg/Addendum/Oem/Fox/Library/ClkReqControlLib/ClkReqControlLib.inf
  AmdCpmRasLib|AmdCpmPkg/Library/Proc/RasRmb/AmdCpmRasRmbLib.inf

  ABRecoveryBaseLib|AmdCpmPkg/Features/ABRecovery/Library/ABRecoveryBaseLib.inf

[LibraryClasses.common.PEIM]
  OemAgesaCcxPlatformLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AgesaCcxPlatformLib/AgesaCcxPlatformLib.inf
  FchInitHookLibPei|AmdCpmPkg/Addendum/Oem/Fox/Library/FchInitHookLib/FchInitHookPeiLib.inf
  AmdCalloutLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdCalloutLib/AmdCalloutPeiLib.inf
  AmdPbsConfigLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdPbsConfigLib/AmdPbsConfigPeiLib.inf
  OemGpioResetControlLib|AmdCpmPkg/Addendum/Oem/Fox/Library/GpioResetControlLib/GpioResetControlLib.inf
  AodBaseLib|AmdCpmPkg/Features/AODv2/Library/AodBaseLib/AodBasePeiLib.inf

[LibraryClasses.common.DXE_DRIVER]
  FchInitHookLibDxe|AmdCpmPkg/Addendum/Oem/Fox/Library/FchInitHookLib/FchInitHookDxeLib.inf
  AmdCalloutLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdCalloutLib/AmdCalloutDxeLib.inf
  AmdPbsConfigLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdPbsConfigLib/AmdPbsConfigDxeLib.inf
  AodBaseLib|AmdCpmPkg/Features/AODv2/Library/AodBaseLib/AodBaseDxeLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  AmdPbsConfigLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdPbsConfigLib/AmdPbsConfigDxeLib.inf
  AodBaseLib|AmdCpmPkg/Features/AODv2/Library/AodBaseLib/AodBaseSmmLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  AmdPbsConfigLib|AmdCpmPkg/Addendum/Oem/Fox/Library/AmdPbsConfigLib/AmdPbsConfigDxeLib.inf

[LibraryClasses.X64.DXE_SMM_DRIVER]
  DisplayCbsLib|AmdCpmPkg/Features/DisplayFeature/Library/DisplayCbsLib/DisplayCbsLib/DisplayCbsLib.inf

[Components.$(PEI_ARCH)]
  AmdCpmPkg/Addendum/Oem/Fox/Pei/AmdCpmOemInitPei/AmdCpm3OemInitPeim.inf
  !if $(MODERN_STANDBY_SUPPORT) == YES
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Pei/AmdCpmModernStandbyInitPei.inf
  !endif
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmPmfBoard/Pei/AmdCpmPmfBoardPeim.inf
  AmdCpmPkg/Kernel/Pei/AmdCpm3InitPeim.inf
  AmdCpmPkg/Features/GpioInit/Pei/AmdCpm3GpioInitPeim.inf
  AmdCpmPkg/Features/DisplayFeature/Pei/AmdCpm3DisplayFeaturePeim.inf
  AmdCpmPkg/Features/ZeroPowerOdd/Pei/AmdCpm3ZeroPowerOddPeim.inf
  AmdCpmPkg/Features/PcieInit/Pei/AmdCpm3PcieInitPeim.inf
#[-start-220217-IB14740213-remove]#
#  AmdCpmPkg/Features/EcInit/Pei/AmdCpm3EcInitPeim.inf
#[-end-220217-IB14740213-remove]#
  AmdCpmPkg/Features/Ucsi/Pei/AmdCpm3UcsiPeim.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/I2cMaster/Pei/AmdI2cMasterPei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Devices/M24LC128/Pei/M24Lc128Pei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Devices/Pca9535a/Pei/Pca9535aPei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Devices/Pca9555a/Pei/Pca9555aPei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Devices/Pca9545a/Pei/Pca9545aPei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Pei/PlatformMemoryConfigurationPei/PlatformMemoryConfigurationPei.inf
  AmdCpmPkg/Addendum/Oem/Fox/Pei/PlatformCustomizePei/PlatformCustomizePei.inf

  AmdCpmPkg/Features/AODv2/Pei/AodPei.inf {
    <LibraryClasses>
      NULL|AmdCpmPkg/Features/AODv2/Library/AodSocLib/RMB/AodSocLibPei.inf
  }
  # A/B Recovery
  AmdCpmPkg/Features/ABRecovery/Pei/AmdCpmABRecovery.inf
  AmdCpmPkg/Addendum/Oem/Fox/Pei/AmdXgbeWorkaround/AmdXgbeWorkaround.inf

[Components.$(DXE_ARCH)]
  AmdCpmPkg/Kernel/Asl/AmdCpm3InitAsl.inf
  AmdCpmPkg/Kernel/Dxe/AmdCpm3InitDxe.inf
  AmdCpmPkg/Kernel/Smm/AmdCpm3InitSmm.inf
  AmdCpmPkg/Features/AcpPowerGating/Dxe/AmdCpmAcpPowerGatingDxe.inf
  AmdCpmPkg/Features/DisplayFeature/Dxe/AmdCpm3DisplayFeatureDxe.inf
  AmdCpmPkg/Features/DisplayFeature/Smm/AmdCpm3DisplayFeatureSmm.inf
  AmdCpmPkg/Features/ZeroPowerOdd/Asl/Fm17/AmdCpm3ZeroPowerOddFm17Asl.inf
  AmdCpmPkg/Features/ZeroPowerOdd/Dxe/AmdCpm3ZeroPowerOddDxe.inf
  AmdCpmPkg/Features/PcieInit/Asl/ExpressCard/AmdCpmExpressCardAsl.inf
  AmdCpmPkg/Features/PcieInit/Asl/LowPowerEthernet/AmdCpmLowPowerEthernetAsl.inf
  AmdCpmPkg/Features/PcieInit/Asl/WirelessLanRecovery/AmdCpmWirelessLanRecoveryAsl.inf
  AmdCpmPkg/Features/PcieInit/Asl/OtherHotplug/AmdCpmOtherHotplugAsl.inf
  AmdCpmPkg/Features/PcieInit/Asl/WirelessButton/AmdCpmWirelessButtonAsl.inf
  AmdCpmPkg/Features/PcieInit/Dxe/AmdCpm3PcieInitDxe.inf
  AmdCpmPkg/Features/GpioInit/Dxe/AmdCpm3GpioInitDxe.inf
  AmdCpmPkg/Features/GpioInit/Smm/AmdCpm3GpioInitSmm.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/I2cMaster/Dxe/AmdI2cMasterDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/xGbEI2cMaster/xGbEI2cMasterDxe.inf
#[-start-220217-IB14740213-remove]#
#  AmdCpmPkg/Features/EcInit/Asl/AmdCpm3EcInitAsl.inf
#  AmdCpmPkg/Features/EcInit/Dxe/AmdCpm3EcInitDxe.inf
#[-end-220217-IB14740213-remove]#
  AmdCpmPkg/Features/Ucsi/Asl/AmdCpm3UcsiAsl.inf
  AmdCpmPkg/Features/Ucsi/Asl/AmdCpm3Ucsi3PortsAsl.inf
  AmdCpmPkg/Features/Ucsi/Dxe/AmdCpm3UcsiDxe.inf
  AmdCpmPkg/Features/SensorFusion/Asl/AmdCpm3SensorFusionAsl.inf
  AmdCpmPkg/Features/SensorFusion/Dxe/AmdCpm3SensorFusionDxe.inf
  AmdCpmPkg/Features/ModernStandbyFeature/Dxe/AmdCpmModernStandbyFeatureDxe.inf
  AmdCpmPkg/Features/ModernStandbyFeature/Dxe/AmdCpmModernStandbyAcpiTableInstall.inf
  AmdCpmPkg/Features/PlatformOsc/AmdCpmPlatformOscTableInstall.inf
  AmdCpmPkg/Features/WirelessButtonHid/Asl/WirelessButtonHid.inf
  AmdCpmPkg/Features/WirelessButtonHid/Dxe/AmdCpmWirelessButtonHidDxe.inf
  AmdCpmPkg/Features/PMF/Asl/AmdCpmPmfAsl.inf
  AmdCpmPkg/Features/PMF/Dxe/AmdCpmPmfDxe.inf
  AmdCpmPkg/Features/PciHotPlugInit/Dxe/AmdCpmPciHotPlugInitDxe.inf

  AmdCpmPkg/Addendum/Oem/Fox/Devices/Pca9535a/Dxe/Pca9535aDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Devices/Pca9545a/Dxe/Pca9545aDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/AmdXgbeWorkaround/AmdXgbeWorkaround.inf
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/AmdCpmOemAcpi/AmdCpmOemAcpi.inf
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/PspPlatformDriver/PspPlatform.inf
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/AmdPbsSetupDxe/AmdPbsSetupDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/AmdCpmOemInitDxe/AmdCpmOemInitDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Smm/AmdCpmOemSmm.inf
  !if $(MODERN_STANDBY_SUPPORT) == YES
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Asl/Gpp0/AmdCpmGpp0Asl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Asl/Nvme/AmdCpmNvmeD3Asl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Asl/Nvme/AmdCpmNvmeRtd3Asl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Asl/GppMs/AmdCpmGppMsAsl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Asl/uPEP/AmdCpmUpepAsl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmModernStandbyFeature/Dxe/AmdCpmModernStandbyInitDxe/AmdCpmModernStandbyInitDxe.inf
  !endif
#  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmWwanFeature/Asl/Wwan/AmdCpmWwanFibocomL860RPlusAsl.inf
#  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmWwanFeature/Dxe/AmdCpmWwanInitDxe/AmdCpmWwanInitDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmPmfBoard/Asl/AmdCpmPmfBoardAsl.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/AmdCpmPmfBoard/Dxe/AmdCpmPmfBoardDxe.inf


  AmdCpmPkg/Features/AODv2/Dxe/AodDxe.inf {
    <LibraryClasses>
      NULL|AmdCpmPkg/Features/AODv2/Library/AodSocLib/RMB/AodSocLibDxe.inf
  }
  AmdCpmPkg/Features/AODv2/Smm/AodSmm.inf {
    <LibraryClasses>
      NULL|AmdCpmPkg/Features/AODv2/Library/AodSocLib/RMB/AodSocLibSmm.inf
  }


#
# MPM Battery Info
#
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/PlatformDataCollection/PlatformBatteryInfo/PlatformBatteryInfoDxe.inf

#
# MPM Sensor Info
#
  AmdCpmPkg/Addendum/Oem/Fox/Dxe/PlatformDataCollection/PlatformSensorInfo/PlatformSensorInfoDxe.inf
  AmdCpmPkg/Addendum/Oem/Fox/Features/Mpm/AmdMpmSensor.inf

 AmdCpmPkg/Features/ABRecovery/Smm/AmdCpmABRecoverySmm.inf

 AmdCpmPkg/Features/ABRecovery/Dxe/AmdCpmABRecoveryDxe.inf

 AmdCpmPkg/Features/SoundWire/Dxe/AmdCpmSoundWireDxe.inf

  AmdCpmPkg/Addendum/Oem/Fox/Dxe/MemTest/MemTest.inf

  AmdCpmPkg/Features/PlatformRas/Rmb/Dxe/AmdPlatformRasRmbDxe.inf
  AmdCpmPkg/Features/PlatformRas/Rmb/Asl/PlatformRasRmbAsl.inf

[PcdsFeatureFlag]

[PcdsFixedAtBuild]
  gAmdCpmPkgTokenSpaceGuid.ApuSupportMask|0x00020000  # RMB==Bit17
  gAmdCpmPkgTokenSpaceGuid.EcRamBaseAddr|0x00662
  gAmdCpmPkgTokenSpaceGuid.EcRamGpioOffset|0x9F
  gAmdCpmPkgTokenSpaceGuid.EcMutexAslName|0x3930305A
  gAmdCpmPkgTokenSpaceGuid.PcdSerialIoDecode|TRUE

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspAutoPsb|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdFchOemBeforePciRestoreSwSmi|0xEA
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdFchOemAfterPciRestoreSwSmi|0xEB
  gAmdCpmPkgTokenSpaceGuid.AcpiDisableDgpuAudioSwSmi|0xED

  # Please also update PcdPeiCoreMaxPeiStackSize when you change this value
  gAmdCpmPkgTokenSpaceGuid.PcdBspStackSize|0x100000

  #
  # AmdCommonPcd
  #
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgIommuMMIOAddressReservedEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdBottomMmioReservedForPrimaryRb|0xFEC00000

  # V9 Pcds
  # IDS Debug Lib Pcds
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintRedirectIOEnable|FALSE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartInitialize|TRUE
  #Specify the IO port for serial out
  #   ESPI or LPC COM1: 0x3F8, COM2: 2F8, COM3: 3E8, COM4: 2E8,
  #              UART0: 0xFEDC9000, UART1: 0xFEDCA000, UART2: 0xFEDCE000, UART3: 0xFEDCF000, UART4: 0xFEDD1000.
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDC9000
  #UART BaudRate 3M (PcdAmdIdsUartBaudRate = 3000000), 1.5M (PcdAmdIdsUartBaudRate = 1500000),  1M (PcdAmdIdsUartBaudRate = 1000000),
  #            750K (PcdAmdIdsUartBaudRate = 750000),  600K (PcdAmdIdsUartBaudRate = 600000), 500K (PcdAmdIdsUartBaudRate = 500000),
  #            429K (PcdAmdIdsUartBaudRate = 428571),  375K (PcdAmdIdsUartBaudRate = 375000), 333K (PcdAmdIdsUartBaudRate = 333333), and so on, ...
  #To receive data correctly from UART with BaudRate > 1M, you may use X_S_h_e_l_l instead of P_u_t_t_y.
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartBaudRate|115200
  #Switch to enable UART hardware flow control. If UARTx cannot support it, please set it to FALSE.
  #   Recommand to enable it if BaudRate > 500K, and you have to connect TXD, CTS and Ground pin at least.
  #   Usage: UART TXD <-> TTL cable RXD, UART CTS <-> TTL cable RTS and UART Ground <-> TTL cable Ground.
  #   For example, RMB UART0/2/4 can support Hardware Flow Control but UART1/3.
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|TRUE
  #Auto detect emulation platform to instead of PcdAmdIdsDebugPrintRedirectIOEnable and PcdAmdIdsDebugPrintSerialPortEnable
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintEmulationAutoDetect|TRUE
# TEST_POINT(60) MAIN_FLOW(50) FCH_TRACE(40) PSP_TRACE(31) GFX_MISC(27) NB_MISC(25) PCIE_MISC(21) GNB_TRACE(20) CPU_TRACE(10) MEM_STATUS(3)
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintFilter|0x100401008A300408
# MAIN_FLOW(50) MEMORY_TRACE_RSV6(9) MEMORY_TRACE_RSV5(8) MEMORY_TRACE_RSV4(7) MEMORY_TRACE_RSV3(6) MEM_PMU(5) MEM_UNDEF_BF(4) MEM_STATUS(3) MEM_FLOW(2) MEM_GETREG(1) MEM_SETREG(0)
# gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintFilter|0x000400000000003C

  # Customized DIMM Vendor Strings
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord0|0x2C00
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord1|0xAD00
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord2|0xC100
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord3|0xCE00
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord4|0x4F00
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord5|0x9801
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord6|0xFE02
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord7|0x0B03
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord8|0x2503
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord9|0x8303
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord10|0xCB04
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorWord11|0xC106

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr0|"Micron Technology"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr1|"Hynix"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr2|"Infineon (Siemens)"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr3|"Samsung"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr4|"Transcend Information"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr5|"Kingston"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr6|"ELPIDA"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr7|"Nanya Technology"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr8|"Kingmax Semiconductor"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr9|"Buffalo (Formerly Melco)"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr10|"A-DATA Technology"
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosDimmVendorStr11|"ASint Technology"

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosSocketDesignationSocket0|"FP7"

  #Fch
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFchCfgSioPmeBaseAddress|0x1640

#!if $(INTERNAL_IDS) == YES
#  #AOD
#  gAmdCpmPkgTokenSpaceGuid.PcdVfrForm_AOD_VDDIO|FALSE
#  gAmdCpmPkgTokenSpaceGuid.PcdVfrForm_AOD_VTT|FALSE
#  gAmdCpmPkgTokenSpaceGuid.PcdVfrForm_AOD_VPP|FALSE
#!endif



  gAmdCpmPkgTokenSpaceGuid.PcdCpmPEICompressSize|0xE0000

  #PLAT-90769
  #Switch for write agesa testpoint to STB
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAgesaTestPointToStb|FALSE

  gAmdCpmPkgTokenSpaceGuid.PcdCpmPmfStaticPolicyBinRomOffset|0x1031000
  gAmdCpmPkgTokenSpaceGuid.PcdCpmPmfStaticPolicySize|0x4B000

  # AMD PBS SETUP variable size
  # Note: If change sizeof(AMD_PBS_SETUP_OPTION) in AmdPbsConfig.h, must update this PCD value!!!
  gAmdCpmPkgTokenSpaceGuid.PcdAmdPbsSetupSize|0x500

[PcdsDynamicDefault]
  gAmdCpmPkgTokenSpaceGuid.PcdRmbBoardSelect|0x00                   # 0x00: LilacTv, 0x01: MayanTv
  gAmdCpmPkgTokenSpaceGuid.PcdRmbPcbVersion|0x00
  gAmdCpmPkgTokenSpaceGuid.PcdRmbSku|0x00
  gAmdCpmPkgTokenSpaceGuid.PcdAmdCpmPciHotPlugSupport|TRUE

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPeiGopEnable|TRUE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciUsb3PortDisable|0x00
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciUsb2PortDisable|0x00
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciOcPinSelect|0xFF000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhci1OcPinSelect|0xFF000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhci2OcPinSelect|0xFFFFFFF0
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhci3OcPinSelect|0xFFFFFF00
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhci4OcPinSelect|0xFFFFFF00
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciOcPolarityCfgLow|TRUE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPspEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspKvmEnable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgGnbIoapicAddress|0xFEC01000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdFchWdtEnable|TRUE

  ## Customize AGESA GNB
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgGnbIoapicAddress|0xFEC01000
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdS0i2Enable|0

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgGnbIoapicId|0x22
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgFchIoapicId|0x21

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgAzaliaSsid|0xD5951022
  # PLAT-78121 [RMB] Enable PSPP (AC = Gen4/DC = Gen3), Set PcdPsppPolicy = 2 (PsppBalance)
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPsppPolicy|0
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPhyA0DynamicControl|1
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPhyA1DynamicControl|1
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPhyA2DynamicControl|0
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPhyA34DynamicControl|1
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdNbioAudioSelect|0
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIvrsRelativeAddrNamesSupport|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgPCIePHYPowerGating|TRUE
  ## PLAT-77555 [RMB]Enable AER, remove the CPM override
#  gEfiAmdAgesaPkgTokenSpaceGuid.PcdCfgAEREnable|FALSE
  ## PLAT-78008 [RMB] Request disable in RMB Platfrom
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgDxioAllowPointerSlipInterval|FALSE

  ##PLAT-77318 [Rembrandt FP7]DISPLAY_QUALITY: eDP Timing T9 had better to tune into >0ms
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPwrDownBloffToVaryBlOff|1

  ## AGPIO40 is using as GPIO output low function, so we can use GEVENT20 as SW SCI event PcdSwSciGpeID
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdCoreCountControlEnable|1
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSwSciGpeID|0x14

  ## Customize AGESA FCH
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdLegacyFree|FALSE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdHpetEnable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdHpetMsiDis|FALSE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdRichtekVrmPsi0Workaround|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdDxioSingleResetCallout|TRUE
##ACPI_REG_WRITE OEMFchPmioEPostInitTable[] =
##{
##  {           00,                  00,   0xB0, 0xAC}, // Signature
##  {SMI_BASE >> 8,       FCH_SMI_KT_WAKE,    0,    8}, // map WAKE_L to ACPI _L08
##  {SMI_BASE >> 8,   FCH_SMI_KT_Xhc0Wake,    0,   25}, // map XHCI-0 PME to ACPI _L19
##  {SMI_BASE >> 8,   FCH_SMI_KT_Xhc1Wake,    0,   25}, // map XHCI-1 PME to ACPI _L19
##  {SMI_BASE >> 8,   FCH_SMI_KT_Xhc2Wake,    0,   26}, // map XHCI-2 PME to ACPI _L1A
##  {SMI_BASE >> 8,       FCH_SMI_KT_TWRN,    0,    1},
##  {SMI_BASE >> 8,   FCH_SMI_KT_Xhc3Wake,    0,   26}, // map XHCI-3 PME to ACPI _L1A  For RMB B0 only
##  {SMI_BASE >> 8,   FCH_SMI_KT_Xhc4Wake,    0,   26}, // map XHCI-4 PME to ACPI _L1A  For RMB B0 only
##  {SMI_BASE >> 8, FCH_SMI_KT_CioFchPme0,    0,   26}, // map CIO-0  PME to ACPI _L1A  For RMB B0 only
##  {SMI_BASE >> 8, FCH_SMI_KT_CioFchPme1,    0,   26}, // map CIO-1  PME to ACPI _L1A  For RMB B0 only
##  {         0xFF,                0xFF,   0xFF, 0xFF},
##};
## 0x00, 0x00, 0xB0, 0xAC,
## 0x02, 0x48, 0x00, 0x08,
## 0x02, 0x78, 0x00, 0x19,
## 0x02, 0x79, 0x00, 0x19,
## 0x02, 0x6A, 0x00, 0x1A,
## 0x02, 0x70, 0x00, 0x01,
## 0xFF, 0xFF, 0xFF, 0xFF,        # don't delete it. Reserve it for B0 XHCI3
## 0xFF, 0xFF, 0xFF, 0xFF,        # don't delete it. Reserve it for B0 XHCI4
## 0xFF, 0xFF, 0xFF, 0xFF,        # don't delete it. Reserve it for B0 CIO0
## 0xFF, 0xFF, 0xFF, 0xFF,        # don't delete it. Reserve it for B0 CIO1
## 0xFF, 0xFF, 0xFF, 0xFF         # don't delete it. End Flag
## }
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdOemProgrammingTablePtr|{ 0x00, 0x00, 0xB0, 0xAC, 0x02, 0x48, 0x00, 0x08, 0x02, 0x78, 0x00, 0x19, 0x02, 0x79, 0x00, 0x19, 0x02, 0x6A, 0x00, 0x1A, 0x02, 0x70, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNativePcieSupport|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNoneSioKbcSupport|FALSE

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSataEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSataClass|0x06

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdXhci0Enable|TRUE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdLpcClockDriveStrength|0x3

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSmbusSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSataAhciSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSataRaidSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSataRaid5Ssid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSataIdeSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdLpcSsid|0x00000000
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSdSsid|0x00000000

  gEfiAmdAgesaModulePkgTokenSpaceGuid.FchRTSataDevSlpPort0S5Pin|0x18 #GPIO24

  # Disable UMA above 4 GB
 ## gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdMemCfgUmaAbove4G|FALSE

  ## Enable Ftpm
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspSystemTpmAlgSmx|0x1
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspSystemTpmConfig|0x1

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiEnable|TRUE
#  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdEspiEc0Enable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiDevice0Enable|TRUE
#  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIoMmioDecode|{0x01, 0x4F, 0x00, 0x00, 0x01, 0x4E, 0x00, 0x00, 0x01, 0x00, 0x06, 0xFF, 0x01, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0xEC, 0xFE, 0xFF, 0x00}
# Correct for EC IO decode
#  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIoMmioDecode|{0x01, 0xf8, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0xEC, 0xFE, 0xFF, 0x00}
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIoMmioDecodeV2|{ 0x01, 0xf8, 0x03, 0x07, 0x01, 0x00, 0x06, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0xEC, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIrqPolarity|0x00001002
## PLAT-82947 A spurious IO60/64 read is observed on Z9 exit
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIrqPolarity|0x00000000
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiIrqMask|0x00FFEFFD
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdEspiIo80Enable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdEspiKbc6064Enable|TRUE

  # PLAT-75230, Enable ACDC timer to platform default
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAcdcTimerEnable|TRUE

  # Turn off BP_X48M0 Clock Output - Lilac doesn't use BP_X48M0 clock, it can be disabled.
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdBpX48M0OutputEnable|FALSE

  #Turn on AGESA APCB recovery mechanism
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspApcbRecoveryEnable|TRUE

  #cpu
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCpuSsdtProcessorContainerInSb|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCpuSsdtProcessorContainerName|"PLTF"

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSerialIrqEnable|FALSE

  #FastSpeed = 16.5MHz
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdResetFastSpeed|4

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdBpX48M0ClockEnable|FALSE

  #W0V is only enabled for S0i3, not for S3/S5, therefore we don't need to keep the XTAL on during S3/S5. We only need the XTAL during S0i3.
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdTurnOffXtalS3S5|TRUE

  # ESPI Operating Frequency      - 0:16.7MHz, 1:33MHz, 2:66MHz, 0x0F:Auto
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEspiOperatingFreq|0

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPublicFanControlEnable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdVddOffVidCtrl|TRUE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdApicMode|0x1                 #  0x01 (xApicMode) - force legacy xApic mode

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNbifPgHysteresis|0x10
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNbifMgcgHysteresis|0x10
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSyshubMgcgHysteresis|0x10
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSyshubPgHysteresis|0x10
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbeMdio0|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbeMdio1|FALSE
  # This bit has no use from Fp7 chip onwords.
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbeSfp|FALSE
  # Port specific SFP setting identification added for Fox/Fp7.
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbeSfp0|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbeSfp1|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbePort0ConfigEn|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbePort1ConfigEn|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbePort0Table|{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXgbePort1Table|{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSpiCustomConfig|TRUE

  #PLAT-74900 [RMB_FP7][SSV_Stress]Unexpected reboot - workaround
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdGlobalStbVerbosityControl|0x00

  #Make STB related PCD to be Dynamic
  #Swtich for STB related features, including STB write function, STB register config, send STB verbosity to FWs
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbEnable|TRUE

  #Swtich for STB FilterMask feature
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbFilterMaskEnable|TRUE

  #Only active when PcdStbFilterMaskEnable set to TRUE
  #Value set to MP2_POSTCODE_IP_0, need be adjust for different IBV
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbIbvSourceId|0xB3

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdDxioSaveRestoreModes|0x2  #saveRestoreMode 0:FULL, 1:Select, 2:HYBRID, 3:unknown

  ### @li BIT6 - I2C1
  ### @li BIT8 - I2C3
  ### @li BIT13 - I3C1
  ## PLAT-90227 Enable TouchPad by Default (I2C1)
  gEfiAmdAgesaModulePkgTokenSpaceGuid.FchRTDeviceEnableMap|0x04010900

  ## PLAT-90978 Disable Pluton by default
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdHspCoreEnable|FALSE

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdSustainedPowerLimit|100000

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdResetSpiClkEarlier|TRUE

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdDisplayFixVoltageSwing|0

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCpuLpiC1MinRes|0x00000002
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCpuLpiC2MinRes|0x00000024
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCpuLpiC3MinRes|0x000002BC

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPdInterruptModeEn|0x0E

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgIOMMUDynamicPgEnable|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIommuL1ClockGatingEnable|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIommuL2ClockGatingEnable|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgIOMMUL2MemoryPGEnable|FALSE
  
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdUsb4WaitCL0USB4|4000000

[BuildOptions]
  GCC:RELEASE_*_*_CC_FLAGS             = -DAMD_TABLE_ADDITIONAL_BUFFER_SIZE=0x800
  INTEL:RELEASE_*_*_CC_FLAGS           = /D AMD_TABLE_ADDITIONAL_BUFFER_SIZE=0x800
  MSFT:RELEASE_*_*_CC_FLAGS            = /D AMD_TABLE_ADDITIONAL_BUFFER_SIZE=0x800
  RVCT:RELEASE_*_*_CC_FLAGS            = -DAMD_TABLE_ADDITIONAL_BUFFER_SIZE=0x800
  *_*_*_CC_FLAGS                       = -D AMD_TABLE_ADDITIONAL_BUFFER_SIZE=0x800
