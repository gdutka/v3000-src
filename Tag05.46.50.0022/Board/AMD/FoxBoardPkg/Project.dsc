## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

#[-start-220221-IB14740215-modify]#
!import MdePkg/Package.dsc
!import MdeModulePkg/Package.dsc
!import UefiCpuPkg/Package.dsc
!import FatPkg/Package.dsc
!import ShellPkg/Package.dsc
!import SecurityPkg/Package.dsc
!import PcAtChipsetPkg/Package.dsc
!import UnitTestFrameworkPkg/Package.dsc
!import InsydeOemServicesPkg/Package.dsc
!import InsydeModulePkg/Package.dsc
!import InsydeSetupPkg/Package.dsc
!import InsydeNetworkPkg/Package.dsc
!import InsydeFlashDevicePkg/Package.dsc
!import SioDummyPkg/Package.dsc
#[-start-220301-IB14740220-modify]#
!import AgesaModulePkg/Package.dsc
!import AmdCbsPkg/Package.dsc
!import AmdCpmPkg/Package.dsc
!import AmdSupervisorPkg/Package.dsc
#[-end-220301-IB14740220-modify]#
!import AmdPspFeaturePkg/Package.dsc
!import SegFeaturePkg/Package.dsc
!import AmdCommonChipsetPkg/Package.dsc
!import InsydeCrPkg/Package.dsc
!import InsydeH2OUvePkg/Package.dsc
!import InsydePostCodePkg/Package.dsc
!import InsydeDebugAssistPkg/Package.dsc
!import InsydePlatformInfoPkg/Package.dsc
!import EmbeddedChipsetPkg/Package.dsc
#[-end-220221-IB14740215-modify]#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                   = Fox
  PLATFORM_GUID                   = E7D92858-1196-4889-9D90-90410FB33E91
  PLATFORM_VERSION                = 0.1
  DSC_SPECIFICATION               = 0x00010005
  OUTPUT_DIRECTORY                = Build/$(PROJECT_PKG)
  SUPPORTED_ARCHITECTURES         = IA32|X64
  BUILD_TARGETS                   = DEBUG|RELEASE
  SKUID_IDENTIFIER                = DEFAULT
  FLASH_DEFINITION                = Build/$(PROJECT_PKG)/Project.fdf

  !include $(PROJECT_PKG)/Project.env

  !if $(PROMONTORY_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DPROMONTORY_SUPPORT
    DEFINE ASM_FLAGS              = $(ASM_FLAGS) -DPROMONTORY_SUPPORT
    DEFINE VFR_FLAGS              = $(VFR_FLAGS) -DPROMONTORY_SUPPORT
  !endif

  !if $(MODERN_STANDBY_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DMODERN_STANDBY_SUPPORT
  !endif

  !if $(REDUCED_HARDWARE_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DREDUCED_HARDWARE_SUPPORT
  !endif

  !if $(VN_16M_AB_CFG) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DVN_16M_AB_CFG
  !endif

  !if $(VN_32M_AB_COMBO_CFG) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DVN_32M_AB_COMBO_CFG
  !endif

  !if $(LPDDR5) == YES
    DEFINE CC_FLAGS              = $(CC_FLAGS) -DLPDDR5
  !endif

  EDK_GLOBAL  CSM_VERSION         = 096

## DEFINE for AmdCpmPkg with oem board support
  DEFINE CPM_PKG_PATH     = AmdCpmPkg/Addendum/Oem/Fox
  DEFINE CPM_PKG_DEC      = AmdCpmOemPkg

  !if $(THUNDERBOLT_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DTHUNDERBOLT_SUPPORT
  !endif

  !if $(SMM_ISOLATION_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DSMM_ISOLATION_SUPPORT
    DEFINE PP_FLAGS               = $(PP_FLAGS) -DSMM_ISOLATION_SUPPORT
  !endif

#[-start-220301-IB14740220-add]#
  !if $(SMMSUPV_SOURCE_BUILD) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DSMMSUPV_SOURCE_BUILD
    DEFINE PP_FLAGS               = $(PP_FLAGS) -DSMMSUPV_SOURCE_BUILD
  !endif
#[-end-220301-IB14740220-add]#

  !if $(KVM_SUPPORT) == YES
    DEFINE CC_FLAGS               = $(CC_FLAGS) -DKVM_SUPPORT
  !endif

  !if $(CRB_ONLY) == YES
    DEFINE CC_FLAGS              = $(CC_FLAGS) -DCRB_ONLY
    DEFINE ASL_FLAGS             = $(ASL_FLAGS) -DCRB_ONLY
  !endif

## DEFINE for NbioTableBlastLib, replace LibPcdGet with LibPcdGetEx.
  DEFINE CC_FLAGS               = $(CC_FLAGS) -DDynamicToDynamicEx

  DEFINE CBS_DSC_FDF_RELTIVE_PATH =AmdCbsPkg/Library/Family/0x19/RMB/External

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this Platform.
# If cpu dead loop in post code 0x8E (PEI_BOARD_ID_SETUP_FAILED), it should check
# the BoardId value is whether in both [SkuIds] and SKUID_IDENTIFIER of Project.dsc.
#
################################################################################
[SkuIds]
  0|DEFAULT

################################################################################
#
# Include AGESA V9 DSC file
#
################################################################################
#[-start-220301-IB14740220-remove]#
#!if $(LPDDR5) == YES
#  !include AgesaModulePkg/AgesaFp7RmbModulePkg.inc.dsc
#!else
#  !include AgesaModulePkg/AgesaFp7r2RmbModulePkg.inc.dsc
#!endif
#[-end-220301-IB14740220-remove]#


#[-start-220301-IB14740220-remove]#
#  !include $(CBS_DSC_FDF_RELTIVE_PATH)/CbsRembrandt.inc.dsc
#[-end-220301-IB14740220-remove]#

#[-start-220301-IB14740220-remove]#
#!if $(SMM_ISOLATION_SUPPORT)
#   !include AmdSupervisorPkg/AmdSupervisorPkg.inc.dsc
#!endif
#[-end-220301-IB14740220-remove]#

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################
[PcdsFeatureFlag]
  gInsydeTokenSpaceGuid.PcdH2ODdtSupported|$(INSYDE_DEBUGGER)
!if $(EFI_DEBUG) == YES
#[-start-230811-IB20840013-modify]#
# gInsydeTokenSpaceGuid.PcdStatusCodeUseDdt|$(INSYDE_DEBUGGER)
  gInsydeTokenSpaceGuid.PcdStatusCodeUseDdt|FALSE
#[-end-230811-IB20840013-modify]#

  gInsydeTokenSpaceGuid.PcdStatusCodeUseUsb|$(USB_DEBUG_SUPPORT)
!endif
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE

  gInsydeTokenSpaceGuid.PcdFrontPageSupported|$(FRONTPAGE_SUPPORT)
  gInsydeTokenSpaceGuid.PcdCrisisRecoverySupported|$(CRISIS_RECOVERY_SUPPORT)
  gInsydeTokenSpaceGuid.PcdUseFastCrisisRecovery|$(USE_FAST_CRISIS_RECOVERY)
  gChipsetPkgTokenSpaceGuid.PcdPeiGopSupported|$(PEI_GOP_SUPPORT)
  gInsydeTokenSpaceGuid.PcdSecureFlashSupported|$(SECURE_FLASH_SUPPORT)
  gInsydeTokenSpaceGuid.PcdUnsignedFvSupported|$(UNSIGNED_FV_SUPPORT)
  gInsydeTokenSpaceGuid.PcdH2ONetworkSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2ONetworkIpv6Supported|TRUE
  gInsydeTokenSpaceGuid.PcdH2ONetworkIscsiSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OTpmSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OTpm2Supported|TRUE
  gAmdPspFeaturePkgTokenSpaceGuid.PcdPspFirmwareTpmSupported|TRUE
  gInsydeTokenSpaceGuid.PcdSysPasswordInCmos|$(SYS_PASSWORD_IN_CMOS)
  gInsydeTokenSpaceGuid.PcdSysPasswordSupportUserPswd|$(SUPPORT_USER_PASSWORD)
  gInsydeTokenSpaceGuid.PcdH2OHddPasswordSupported|TRUE
  gInsydeTokenSpaceGuid.PcdReturnDialogCycle|$(RETURN_DIALOG_CYCLE)
  gInsydeTokenSpaceGuid.PcdFvbAccessThroughSmi|TRUE
  gInsydeTokenSpaceGuid.PcdH2OUsbSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OAhciSupported|$(SEAMLESS_AHCI_SUPPORT)
  gInsydeTokenSpaceGuid.PcdH2OIdeSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OSdhcSupported|FALSE
  gInsydeTokenSpaceGuid.PcdSnapScreenSupported|$(SNAPSCREEN_SUPPORT)
  gInsydeTokenSpaceGuid.PcdTextModeFullScreenSupport|$(TEXT_MODE_FULL_SCREEN_SUPPORT)
  gInsydeTokenSpaceGuid.PcdUefiPauseKeyFunctionSupport|$(UEFI_PAUSE_KEY_FUNCTION_SUPPORT)
  gInsydeTokenSpaceGuid.PcdOnlyUsePrimaryMonitorToDisplay|$(ONLY_USE_PRIMARY_MONITOR_TO_DISPLAY)
  gInsydeTokenSpaceGuid.PcdDynamicHotKeySupported|$(DYNAMIC_HOTKEY_SUPPORT)
!if ($(TEXT_SUPPORT) == YES)
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|FALSE
!else
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|TRUE
!endif

!errif (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported == FALSE) and (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported == FALSE), "Must have at least one display engine enabled in Project.dsc"

  gInsydeTokenSpaceGuid.PcdBvdtGenBiosBuildTimeSupported|$(BUILD_TIME_FLAG)
  gPlatformPkgTokenSpaceGuid.PcdRtkUsbLanSupport|$(RTK_USBLAN_SUPPORT)
  gInsydeTokenSpaceGuid.PcdShellBinSupported|TRUE
  gInsydeTokenSpaceGuid.PcdShellBuildSupported|FALSE

  gChipsetPkgTokenSpaceGuid.PcdH2OLegacyFreeSupported|$(LEGACY_FREE_SUPPORT)
#[-start-220902-IB14740260-add]#
  gInsydeTokenSpaceGuid.PcdH2OCsmSupported|$(LEGACY_FREE_SUPPORT)
#[-end-220902-IB14740260-add]#

  gAmdCpmPkgTokenSpaceGuid.PcdReduceHardwareSupport|$(REDUCED_HARDWARE_SUPPORT)
  gChipsetPkgTokenSpaceGuid.PcdH2ONbIoApicSupport|$(NB_IOAPIC_SUPPORT)
  gAmdCpmPkgTokenSpaceGuid.PcdModernStandbySupport|$(MODERN_STANDBY_SUPPORT)
  gChipsetPkgTokenSpaceGuid.PcdH2OCrbOnlySupported|$(CRB_ONLY)
  gChipsetPkgTokenSpaceGuid.PcdH2OXhciSupported|$(XHCI_SUPPORT)

  # For SimNow
  gChipsetPkgTokenSpaceGuid.PcdH2OSpiMmioAccessSupported|FALSE

  #
  # BVM Support
  #
  gPlatformPkgTokenSpaceGuid.PcdBvmSupport|$(BVM_SUPPORT)

  #
  # Recovery long run test support
  #
  gChipsetPkgTokenSpaceGuid.PcdH2OBdsReoveryLongRunSupport|FALSE

  gH2OFlashDeviceEnableGuid.PcdSst25lf040SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst25lf040aSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst25lf080aSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst25vf080bSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst25vf016bSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdAtmel25df041SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdAtmel25df081aSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdAtmel26df161SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdAtmel26df321SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdWinbondW25q64bvSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdWinbondW25q128bvSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdAtmel25df641SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdMxic25l8005SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdMxic25l1605aSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdMxic25l6405dSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdMxic25l12805dSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdMxic25l25635eSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdStM25pe80SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdPmc25lv040SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdGd25lq16SpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdGd25lq64cSpiEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst49lf008aLpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst39vf080LpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdIntel82802acLpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSst49lf004aLpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdPmc49fl004tLpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdWinbond39v040fpaLpcEnable|FALSE
  gH2OFlashDeviceEnableGuid.PcdSt50flw040aLpcEnable|FALSE
!if $(EMULATION_ONLY) == YES
  gPlatformPkgTokenSpaceGuid.PcdEmulationOnly|TRUE
!endif
  gAmdPspFeaturePkgTokenSpaceGuid.PcdPspComboSupport|$(PSP_COMBO_SUPPORT)
  gAmdPspFeaturePkgTokenSpaceGuid.PcdKvmSupport|$(KVM_SUPPORT)

  gInsydeTokenSpaceGuid.PcdH2OI2cSupported|TRUE
#>>EnableAbove4GBMmio++
  gInsydeTokenSpaceGuid.Pcd64BitAmlSupported|TRUE
#<<EnableAbove4GBMmio++

  gInsydeTokenSpaceGuid.PcdH2OBdsOemBadgingSupported|TRUE
#[-start-220221-IB14740215-modify]#
  gSioGuid.PcdH2OSioDummySupported|FALSE
#[-end-220221-IB14740215-modify]#
#[-start-220902-IB14740260-modify]#
!if $(LEGACY_FREE_SUPPORT) == YES
  #
  #  Set TRUE for preventing the PCI resource degradation in Legacy/Dual Mode.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDegradeResourceForOptionRom|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDegradeResourceForOptionRom|FALSE
!endif
#[-end-220902-IB14740260-modify]#
  #
  #  Set FALSE for DASH default.
  #
  gChipsetPkgTokenSpaceGuid.PcdDashSupported|FALSE

!if $(L2A_ONLY) == YES
  gChipsetPkgTokenSpaceGuid.PcdH2OBiosUpdateFlashSequenceEnabled|FALSE
!endif
#[-start-220114-IB19600008-add]#
  #
  # CMOS_LOAD_DEFAULT_SUPPORT
  # Note:This feature need enable PcdH2OPeiCpForceVariableDefaultsSupported
  #
  gChipsetPkgTokenSpaceGuid.PcdH2OCmosLoadDefaultSupported|TRUE
!if gChipsetPkgTokenSpaceGuid.PcdH2OCmosLoadDefaultSupported
  gInsydeTokenSpaceGuid.PcdH2OPeiCpForceVariableDefaultsSupported|TRUE
!endif
#[-end-220114-IB19600008-add]#

#[-start-220217-IB14740212-add]#
  gH2ODebugAssistTokenSpaceGuid.PcdH2ODebugAssistEventTraceSupported|FALSE
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortSupported|FALSE
#[-end-220217-IB14740212-add]#

#[-start-230811-IB20840013-add]#
!if $(INSYDE_DEBUGGER) == YES and $(EFI_DEBUG) == YES
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSupported|FALSE
!endif
#[-end-230811-IB20840013-add]#

[PcdsDynamicHii]
#[-start-221003-IB14740265-add]#
  gChipsetPkgTokenSpaceGuid.PcdUsbEfiTimerPolling|L"Setup"|gSystemConfigurationGuid|264|1 #SystemConfig.UsbHwSmiSupport
#[-end-221003-IB14740265-add]#

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdPort80DataWidth|32

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPspDirUsing16MAddress|FALSE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspAutoPsb|TRUE

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintEnable|$(EFI_DEBUG)

  # PcdSerialPortSelect   # 0 = FCH UART 0
                          # 1 = FCH UART 1
                          # 2 = Simnow ITE8712 Sio Serial Port
                          # 3 = Emulation Port 0x80 out
                          # 4 = Simnow console input from Simnow ITE8712 SIO serial port, and Console output to Simnow Console Output
                          # 5 = SMSC Sio Serial Port
                          # 6 = Simnow SMSC 1100 Sio Serial Port
                          # 7 = Unify Message
                          # 8 = FCH UART 2
                          # 9 = FCH UART 3
                          # 10 = FCH UART 4
  !if $(SIMNOW_SERIAL_PORT_ITE) == YES
    gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect|2
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0x3F8
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
  !elseif $(SIMNOW_SERIAL_PORT_SMSC) == YES
    gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect|6
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0x3F8
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
  !elseif $(REDIRECT_SERIAL_MESSAGE_TO_PORT_80) == YES
    gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect|3
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintRedirectIOEnable|TRUE
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortEnable|FALSE
  !elseif $(UNIFY_MESSAGE_SUPPORT) == YES
    gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect|7
#[-start-210802-IB19520005-modify]#
    gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage|0x08
    #[-start-210909-IB19600001-add] set correct baud rate
    gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|115200
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartBaudRate|115200
    #[-end-210909-IB19600001-add]
#[-end-210802-IB19520005-modify]#
  !else
    gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect|5
  !endif

  !if (gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 2 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 5 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 6)
    # Legacy COM port
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0x3F8
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
  !endif

  !if (gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 1 || gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 9)
    # FCH UART 1/3
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartFifoControl|0x00
  !endif
  !if gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 0
    # FCH UART 0
    gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|115200
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartBaudRate|115200
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDC9000
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 1
    # FCH UART 1
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCA000
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 8
    # FCH UART 2
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartBaudRate|115200
    gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|115200
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCE000
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 9
    # FCH UART 3
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCF000
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 10
    # FCH UART 4
    gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDD1000
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 7
    # Enable debug message (BIT[x] = 0 means disable serial out)
    # Bit[0]: 1 - enable ESPI UART serail out, including UDC(unversial debug card) or EC ESPI UART
    # Bit[1]: 1 - enable LPC UART serail out
    # Bit[2]: 1 - enable port80 redirect serail out
    # Bit[3]: 1 - enable FCH UART0 serail out
    # Bit[4]: 1 - enable FCH UART1 serail out
    # Bit[5]: 1 - enable FCH UART2 serail out
    # Bit[6]: 1 - enable FCH UART3 serail out
    # Bit[7]: 1 - enable FCH UART4 serail out
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x01) == 0x01                # ESPI UART 0x3F8
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0x3F8
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x04) == 0x04                # Port80 redirect
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintRedirectIOEnable|TRUE
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortEnable|FALSE
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x08) == 0x08                # FCH UART0
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDC9000
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartBaudRate|115200
      gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|115200
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x10) == 0x10                # FCH UART1
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCA000
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartFifoControl|0x00
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x20) == 0x20                # FCH UART2
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCE000
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x40) == 0x40                # FCH UART3
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDCF000
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortHardwareFlowControl|FALSE
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsUartFifoControl|0x00
    !endif
    !if (gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage & 0x80) == 0x80                # FCH UART4
      gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPort|0xFEDD1000
    !endif

  !else
  !endif

#[-start-210802-IB19520005-add]#
  gInsydeCrTokenSpaceGuid.PcdH2OCrDevice|{  \
    UINT8(0x01), L"UART0"         , L"VenHw(E76FD4E9-0A30-4CA9-9540-D799534CC4FF,0090DCFE00000000)",  \
    UINT8(0xFF), UINT8(0xFF)      , UINT8(0xFF) \
    }
  gInsydeCrTokenSpaceGuid.PcdH2OCrHsUartSerialClock|0x2DB4000
  gInsydeCrTokenSpaceGuid.PcdH2OCrPciHsUartDeviceList|{UINT32(0xFF), UINT32(0xFF), UINT32(0xFF), UINT32(0xFFFF), UINT32(0xFFFF)} # All 0xFF indicates end of list.
  gInsydeCrTokenSpaceGuid.PcdH2OCrPchHsUartMmioAddress|{ UINT8(0x01), UINT32(0xFEDC9000),UINT8(0x03), \
                                                         UINT8(0xFF), UINT32(0xFFFFFFFF),UINT8(0xFF) \
                                                         } # All 0xFF indicates end of list.
  gInsydeCrTokenSpaceGuid.PcdH2OCrSerialIoPchPcrAddress|0
#[-end-210802-IB19520005-add]#

!if $(AMD_APP_INSTEAD_EFI_SHELL_SUPPORT) == YES
  ## FFS filename to find the application. EmuLinuxLoader = F94964FD-41EC-FE97-28FB-F08C57C139CC
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{ 0xFD, 0x64, 0x49, 0xF9, 0xEC, 0x41, 0x97, 0xFE, 0x28, 0xFB, 0xF0, 0x8C, 0x57, 0xC1, 0x39, 0xCC }
!endif

  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|128
  gEfiMdePkgTokenSpaceGuid.PcdSpinLockTimeout|0

!if $(FIRMWARE_PERFORMANCE) == YES
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|128
!else
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
!endif

  #
  # ACPI 64Bit AML
  #
!if $(SUPPORT_64BITS_AML) == YES
  gChipsetPkgTokenSpaceGuid.PcdH2ODsdtRevision|0x02
!else
  gChipsetPkgTokenSpaceGuid.PcdH2ODsdtRevision|0x01
!endif

  #
  # Hot key Configuration
  # Platform Hot key Define
  # ScanCode, ShiftKey, AltKey, CtrlKey
  # ex:
  #    0x54, 0x0, 0x1, 0x0      F1(Combination Key ScanCode) + ShiftKey
  #    0x68, 0x0, 0x2, 0x0      F1(Combination Key ScanCode) + AltKey
  #    0x5f, 0x0, 0x4, 0x0      F1(Combination Key ScanCode) + CtrlKey
  #
  gInsydeTokenSpaceGuid.PcdPlatformKeyList|{ \
    0x3b, 0x0, 0x0, 0x0,                     \ # F1_KEY
    0x3c, 0x0, 0x0, 0x0,                     \ # F2_KEY
    0x53, 0x0, 0x0, 0x0,                     \ # DEL_KEY
    0x44, 0x0, 0x0, 0x0,                     \ # F10_KEY
    0x86, 0x0, 0x0, 0x0,                     \ # F12_KEY
    0x01, 0x0, 0x0, 0x0,                     \ # ESC_KEY
    0x40, 0x0, 0x0, 0x0,                     \ # UP_ARROW_KEY_BIT
    0x3d, 0x0, 0x0, 0x0,                     \ # F3_KEY
    0x43, 0x0, 0x0, 0x0,                     \ # F9_KEY
    0x00, 0x0, 0x0, 0x0}                       # EndEntry

  #
  # Note: If change sizeof(SYSTEM_CONFIGURATION) in SetupConfig.h, must update really structure size in here!!!
  #
  gChipsetPkgTokenSpaceGuid.PcdSetupConfigSize|700

  gInsydeTokenSpaceGuid.PcdScuFormsetGuidList|{ \
    GUID("C1E0B01A-607E-4B75-B8BB-0631ECFAACF2"), \ # Main
    GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC"), \ # Avance
    GUID("5204F764-DF25-48A2-B337-9EC122B85E0D"), \ # Security
    GUID("A6712873-925F-46C6-90B4-A40F86A0917B"), \ # Power
    GUID("2D068309-12AC-45AB-9600-9187513CCDD8"), \ # Boot
    GUID("B863B959-0EC6-4033-99C1-8FD89F040222"), \ # AmdPbs
    GUID("E39AC631-48ED-445E-9042-55B34F405843"), \ # AmdOverClocking
    GUID("B04535E3-3004-4946-9EB7-149428983053"), \ # AmdCbs
#   GUID("AA5D54A2-59C8-458B-80D8-E41944AD1D99"), \ # Raid
    GUID("B6936426-FB04-4A7B-AA51-FD49397CDC01"), \ # Exit
    GUID("00000000-0000-0000-0000-000000000000")}

  gInsydeTokenSpaceGuid.PcdScuFormsetFlagList|{ \
    UINT8(0), \ # Main
    UINT8(0), \ # Avance
    UINT8(0), \ # Security
    UINT8(0), \ # Power
    UINT8(0), \ # Boot
    UINT8(0), \ # AmdPbs
    UINT8(0), \ # AmdOverClocking
    UINT8(0), \ # AmdCbs
#   UINT8(0), \ # Raid
    UINT8(0), \ # Exit
    UINT8(0xFF)}

  #
  # SMbus Address
  #
  gInsydeTokenSpaceGuid.PcdSmbusAddrChA1|0xA0
  gInsydeTokenSpaceGuid.PcdSmbusAddrChA2|0xA4
  gInsydeTokenSpaceGuid.PcdSmbusAddrChB1|0xA2
  gInsydeTokenSpaceGuid.PcdSmbusAddrChB2|0xA6

  # //
  # // Declare bits for PcdFixedDebugPrintErrorLevel and the ErrorLevel parameter of DebugPrint()
  # //
  # DEBUG_INIT      0x00000001  // Initialization
  # DEBUG_WARN      0x00000002  // Warnings
  # DEBUG_LOAD      0x00000004  // Load events
  # DEBUG_FS        0x00000008  // EFI File system
  # DEBUG_POOL      0x00000010  // Alloc & Free's
  # DEBUG_PAGE      0x00000020  // Alloc & Free's
  # DEBUG_INFO      0x00000040  // Informational debug messages
  # DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
  # DEBUG_VARIABLE  0x00000100  // Variable
  # DEBUG_BM        0x00000400  // Boot Manager
  # DEBUG_BLKIO     0x00001000  // BlkIo Driver
  # DEBUG_NET       0x00004000  // SNI Driver
  # DEBUG_UNDI      0x00010000  // UNDI Driver
  # DEBUG_LOADFILE  0x00020000  // UNDI Driver
  # DEBUG_EVENT     0x00080000  // Event messages
  # DEBUG_GCD       0x00100000  // Global Coherency Database changes
  # DEBUG_CACHE     0x00200000  // Memory range cachability changes
  # DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may significantly impact boot performance
  # DEBUG_ERROR     0x80000000  // Error
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x8000004F
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8000004F

  #
  # SSID
  #
  gInsydeTokenSpaceGuid.PcdI2cControllerTable|{  \# The definition of I2C host controller number lookup table
    UINT64(0x00000000FEDC2000),                  \# [0] I2C0 (I2CA) controller is located on 0xFEDC2000
    UINT64(0x00000000FEDC3000),                  \# [1] I2C1 (I2CB) controller is located on 0xFEDC3000
    UINT64(0x00000000FEDC4000),                  \# [0] I2C2 (I2CC) controller is located on 0xFEDC4000
    UINT64(0x00000000FEDC5000),                  \# [1] I2C3 (I2CD) controller is located on 0xFEDC5000
    # UINT64(0x00000000FEDC6000),                  \# [2] I2C4 (I2CE) controller is located on 0xFEDC5000
    UINT64(0x0000000000000000)                   \# End of table
  }

  gInsydeTokenSpaceGuid.PcdI2cBusSpeedTable|{    \# The definition of I2C bus configuration lookup table
    UINT32(100000),                              \# Number 0 stands for 100 Khz
    UINT32(400000),                              \# Number 0 stands for 400 khz
    UINT32(1000000),                             \# Number 0 stands for 1  Mhz
    UINT32(0)                                    \# End of table
  }
  gInsydeTokenSpaceGuid.PcdH2OPeiMinMemorySize|0x10000000

#Register Ihisi sub function table list.
#Table struct define {CmdNumber, FuncSignature, Priority}
# UINT8(CmdNumber), Char8[20](FuncSignature), UINT8(Priority)
##================  ========================  ===============
gInsydeTokenSpaceGuid.PcdIhisiRegisterTable|{ \
  # Register IHISI AH=00h (VATSRead)
  UINT8(0x00),      "S00Kn_VatsRead00000",    UINT8(0x80), \

  # Register IHISI AH=01h (VATSWrite)
  UINT8(0x01),      "S01Kn_VatsWrite0000",    UINT8(0x80), \

  # Register IHISI AH=05h (VATSNext)
  UINT8(0x05),      "S05Kn_VatsGetNext00",    UINT8(0x80), \

  # Register IHISI AH=10h (FBTSGetSupportVersion)
  UINT8(0x10),      "S10Cs_GetPermission",    UINT8(0xE0), \
  UINT8(0x10),      "S10OemGetPermission",    UINT8(0xC0), \
  UINT8(0x10),      "S10OemGetAcStatus00",    UINT8(0xBB), \
  UINT8(0x10),      "S10OemBatterylife00",    UINT8(0xB6), \
  UINT8(0x10),      "S10Kn_GetVersion000",    UINT8(0x80), \
  UINT8(0x10),      "S10Kn_InitOemHelp00",    UINT8(0x7F), \
  UINT8(0x10),      "S10Kn_GetVendorID00",    UINT8(0x7E), \
  UINT8(0x10),      "S10Kn_GetBatteryLow",    UINT8(0x7D), \

  # Register IHISI AH=11h (FBTSGetPlatformInfo)
  UINT8(0x11),      "S11Kn_GetModelName0",    UINT8(0x80), \
  UINT8(0x11),      "S11Kn_GModelVersion",    UINT8(0x7F), \
  UINT8(0x11),      "S11OemFbtsApCheck00",    UINT8(0x40), \
  # Update flash sequence for A/B recovery
  UINT8(0x11),      "S11Cs_UpExtPlatform",    UINT8(0x20), \
  # UINT8(0x11),      "S11Kn_UpExtPlatform",    UINT8(0x20), \

  # Register IHISI AH=12h (FBTSGetPlatformRomMap)
  UINT8(0x12),      "S12Kn_ProtectRomMap",    UINT8(0x80), \
  UINT8(0x12),      "S12Kn_PrivateRomMap",    UINT8(0x7F), \
  UINT8(0x12),      "S12Cs_PlatformRomMp",    UINT8(0x40), \
  UINT8(0x12),      "S12OemPlatformRomMp",    UINT8(0x20), \

  # Register IHISI AH=13h (FBTSGetFlashPartInfo)
  UINT8(0x13),      "S13Kn_FlashPartInfo",    UINT8(0x80), \

  # Register IHISI AH=14h (FBTSRead)
  UINT8(0x14),      "S14Cs_DoBeforeRead0",    UINT8(0xE0), \
  UINT8(0x14),      "S14Kn_FbtsReadProce",    UINT8(0x80), \
  UINT8(0x14),      "S14Cs_DoAfterRead00",    UINT8(0x20), \

  # Register IHISI AH=15h (FBTSWrite)
  UINT8(0x15),      "S15Cs_DoBeforeWrite",    UINT8(0xE0), \
  UINT8(0x15),      "S15Kn_FbtsWriteProc",    UINT8(0x80), \
  UINT8(0x15),      "S15Cs_DoAfterWrite0",    UINT8(0x40), \

  # Register IHISI AH=16h (FBTSComplete)
  UINT8(0x16),      "S16Cs_CApTerminalte",    UINT8(0xE0), \
  UINT8(0x16),      "S16Cs_CNormalFlash0",    UINT8(0xDF), \
  UINT8(0x16),      "S16Cs_CPartialFlash",    UINT8(0xDE), \
  UINT8(0x16),      "S16Kn_PurifyVariabl",    UINT8(0x80), \
  UINT8(0x16),      "S16Cs_FbtsComplete0",    UINT8(0x20), \
  UINT8(0x16),      "S16Cs_FbtsReboot000",    UINT8(0x1F), \
  UINT8(0x16),      "S16Cs_FbtsShutDown0",    UINT8(0x1E), \
  UINT8(0x16),      "S16Cs_FbtsDoNothing",    UINT8(0x1D), \

  # Register IHISI AH=17h (FBTSGetRomFileAndPlatformTable)
  UINT8(0x17),      "S17Cs_GetPlatformTb",    UINT8(0x80), \

  # Register IHISI AH=1Bh (FBTSSkipMcCheckAndBinaryTrans)
  UINT8(0x1B),      "S1BKn_SkipMcCheck00",    UINT8(0x80), \

  # Register IHISI AH=1Ch (FBTSGetATpInformation)
  UINT8(0x1C),      "S1CCs_GetATpInfo000",    UINT8(0x80), \

  # Register IHISI AH=1Eh (FBTSGetWholeBiosRomMap)
  UINT8(0x1E),      "S1EKn_WholeBiosRomp",    UINT8(0x80), \
  UINT8(0x1E),      "S1EOemWholeBiosRomp",    UINT8(0x40), \

  # Register IHISI AH=1Fh (FBTSApHookPoint)
  UINT8(0x1F),      "S1FKn_ApHookforBios",    UINT8(0x80), \
  UINT8(0x1F),      "S1FCs_ApHookForBios",    UINT8(0x40), \

  # Register IHISI AH=20h (FETSWrite)
  UINT8(0x20),      "S20OemDoBeforeWrite",    UINT8(0xE0), \
  UINT8(0x20),      "S20OemEcIdleTrue000",    UINT8(0xC0), \
  UINT8(0x20),      "S20OemFetsWrite0000",    UINT8(0x80), \
  UINT8(0x20),      "S20OemEcIdleFalse00",    UINT8(0x40), \
  UINT8(0x20),      "S20OemDoAfterWrite0",    UINT8(0x20), \
  UINT8(0x20),      "S20Cs_ShutdownMode0",    UINT8(0x1B), \

  # Register IHISI AH=21h (FETSGetEcPartInfo)
  UINT8(0x21),      "S21OemGetEcPartInfo",    UINT8(0x80), \

  # Register IHISI AH=41h (OEMSFOEMExCommunication)
  UINT8(0x41),      "S41Kn_CommuSaveRegs",    UINT8(0xFF), \
  UINT8(0x41),      "S41Cs_ExtDataCommun",    UINT8(0xE0), \
  UINT8(0x41),      "S41OemT01Vbios00000",    UINT8(0xC0), \
  UINT8(0x41),      "S41OemT54LogoUpdate",    UINT8(0xBB), \
  UINT8(0x41),      "S41OemT55CheckSignB",    UINT8(0xB6), \
  UINT8(0x41),      "S41OemReservedFun00",    UINT8(0xB1), \
  UINT8(0x41),      "S41Kn_T51EcIdelTrue",    UINT8(0x85), \
  UINT8(0x41),      "S41Kn_ExtDataCommun",    UINT8(0x80), \
  UINT8(0x41),      "S41Kn_T51EcIdelFals",    UINT8(0x7B), \
  UINT8(0x41),      "S41OemT50Oa30RWFun0",    UINT8(0x40), \

  # Register IHISI AH=42h (OEMSFOEMExDataWrite)
  UINT8(0x42),      "S42Cs_ExtDataWrite0",    UINT8(0xE0), \
  UINT8(0x42),      "S42Kn_T50EcIdelTrue",    UINT8(0x85), \
  UINT8(0x42),      "S42Kn_ExtDataWrite0",    UINT8(0x80), \
  UINT8(0x42),      "S42Kn_T50EcIdelFals",    UINT8(0x7B), \
  UINT8(0x42),      "S42Cs_DShutdownMode",    UINT8(0x20), \

  # Register IHISI AH=47h (OEMSFOEMExDataRead)
  UINT8(0x47),      "S47Cs_ExtDataRead00",    UINT8(0xE0), \
  UINT8(0x47),      "S47Kn_ExtDataRead00",    UINT8(0x80), \

  # Register IHISI AH=48h (FBTSOEMCapsuleSecureFlash)

  UINT8(0x48),      "S48Cs_CpSecureFlash",    UINT8(0xDE), \
  UINT8(0x48),      "S48Kn_CpSecureFlash",    UINT8(0x80), \

  # Register IHISI AH=49h (FBTSCommonCommunication)
  UINT8(0x49),      "S49Kn_ComDataCommun",    UINT8(0x80), \

  # Register IHISI AH=4Bh (FBTSCommonRead)
  UINT8(0x4B),      "S4BKn_ComDataRead00",    UINT8(0x80), \

  # Register IHISI AH=4Dh (FBTSPassImageFromTool)
  UINT8(0x4D),      "S4DCs_ImageCheck000",    UINT8(0xC0), \
#[-start-220906-IB14740262-add]#
  # Register IHISI AH=52h (UveVariableConfirm)
#[-start-221104-IB14740270-modify]#
  UINT8(0x52),      "S52Cs_CbsApcbSync",      UINT8(0x60), \
  UINT8(0x52),      "S52Cs_AodApcbSync",      UINT8(0x61), \
#[-end-221104-IB14740270-modify]#
#[-end-220906-IB14740262-add]#
  # Register IHISI AH=80h (IhisiAuthStatus)
  UINT8(0x80),      "S80Kn_AuthStatus000",    UINT8(0x80), \
  # Register IHISI AH=81h (IhisiAuthLock)
  UINT8(0x81),      "S81Kn_AuthLock00000",    UINT8(0x80), \
  # Register IHISI AH=82h (IhisiAuthUnlock)
  UINT8(0x82),      "S82Kn_AuthUnlock000",    UINT8(0x80), \
  # Register IHISI AH=83h (IhisiGetCmdBuffer)
  UINT8(0x83),      "S83Kn_GetCmdBuf0000",    UINT8(0x80), \
  UINT8(0x83),      "S83Kn_GetImageBuf00",    UINT8(0x79), \
#[-start-231117-IB20840037-add]#
  # Register IHISI AH=84h (IhisiAuth)
  UINT8(0x84),      "S84Kn_Auth000000000",    UINT8(0x80) }
#[-end-231117-IB20840037-add]#

  #
  # Provide OemHook to sync HW I2C SCL signal
  #
  gChipsetPkgTokenSpaceGuid.OemHookI2cSclSdaEnable|FALSE
  gChipsetPkgTokenSpaceGuid.I2cIcSsSclHcnt|0x0285
  gChipsetPkgTokenSpaceGuid.I2cIcSsSclLcnt|0x0357
  gChipsetPkgTokenSpaceGuid.I2cIcSsSdaHold|0x48
  gChipsetPkgTokenSpaceGuid.I2cIcFsSclHcnt|0x0087
  gChipsetPkgTokenSpaceGuid.I2cIcFsSclLcnt|0x00F0
  gChipsetPkgTokenSpaceGuid.I2cIcFsSdaHold|0x30
  gChipsetPkgTokenSpaceGuid.I2cIcHsSclHcnt|0x0006
  gChipsetPkgTokenSpaceGuid.I2cIcHsSclLcnt|0x0010
  gChipsetPkgTokenSpaceGuid.I2cIcHsSdaHold|0x09

  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x80000

!if $(EFI_DEBUG) == YES
!if $(USB_DEBUG_SUPPORT) == NO
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!endif

!if $(INSYDE_DEBUGGER) == YES and $(H2O_DDT_DEBUG_IO) == Com
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!endif
!endif

#[-start-240221-IB20840050-remove]#
#[-start-210910-IB19600002-modify]#
#[-start-210802-IB19520005-add]#
# !if $(INSYDE_DEBUGGER) == YES and $(H2O_DDT_DEBUG_IO) == Com || $(EFI_DEBUG) == YES
#   gInsydeCrTokenSpaceGuid.PcdH2OConsoleRedirectionSupported|FALSE
# !endif
#[-end-210802-IB19520005-add]#
#[-end-210910-IB19600002-modify]#
#[-end-240221-IB20840050-remove]#

#[-start-230331-IB14740282-add]#
!if $(INSYDE_DEBUGGER) == YES
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAgesaTestPointEnable|FALSE
!endif
#[-end-230331-IB14740282-add]#

#[-start-210802-IB19520007-add]#
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortRegBase|0xFEDC9000
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortUseMmio|TRUE
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeDisplayLength|0x02
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterStride|4
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialClockRate|0x2DB4000
#[-end-210802-IB19520007-add]#

  #
  # SMM Isolation
  #
!if $(SMM_ISOLATION_SUPPORT)
  gSmmSupervisorPkgTokenSpaceGuid.PcdTurnOnSmmIsolationEvent|1
!endif

#[-start-240308-IB20840052-modify]#
[PcdsFixedAtBuild.X64]
#[-start-210827-IB19520013-add]#
  #
  #  On AMD platform, the SMN address for each platform may be different.
  #  MP1 C2P messabe Address in FP7 is defined in NbioSmuV13Lib.h
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgMessageAddress|0x3B10528
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgResponseAddress|0x3B10578
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument0Address|0x3B10998
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument1Address|0x3B1099C
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument2Address|0x3B109A0
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument3Address|0x3B109A4
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument4Address|0x3B109A8
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdMp1C2pmsgArgument5Address|0x3B109AC
  #
  #  TDP SMC request id in FP7 is defined in SMU_RMB_MsgDef.h
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OAmdBiosSmcMsgTdpRequestId|0x5F
#[-end-210827-IB19520013-add]#
#[-end-240308-IB20840052-modify]#

#[-start-240911-IB20840092-add]#
  gChipsetPkgTokenSpaceGuid.PcdH2ODefaultSupervisorPwSupported|$(DEFAULT_SUPERVISOR_PW_SUPPORTED)
  gChipsetPkgTokenSpaceGuid.PcdH2ODefaultSecureBootPwSupported|$(SECURE_BOOT_DEFAULT_PW_SUPPORTED)
  gChipsetPkgTokenSpaceGuid.PcdH2ODefaultSupervisorPwString|L"$(DEFAULT_SUPERVISOR_PW_STRING)"
#[-end-240911-IB20840092-add]#
#[-start-220425-IB14740237-modify]#
[PcdsPatchableInModule]
  #
  # Debug Masks
  #
  # //
  # // Declare bits for PcdDebugPropertyMask
  # //
  # DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED       0x01
  # DEBUG_PROPERTY_DEBUG_PRINT_ENABLED        0x02
  # DEBUG_PROPERTY_DEBUG_CODE_ENABLED         0x04
  # DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED       0x08
  # DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED  0x10
  # DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED    0x20
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0F
#[-end-220425-IB14740237-modify]#

################################################################################
#
# Pcd Dynamic Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicExDefault]
  #
  # Discrete TPM support (type 1: SPI TPM (default)  0: LPC TPM)
  #
  gChipsetPkgTokenSpaceGuid.PcdH2OSpiOrLpcTPMSupported|1

  #
  # Crisis File name definition
  #
  # New File Path Definition : //Volume_Label\\File_Path\\File_Name
  # Notice : "//" is signature that volume label start definition.
  #
  # Example path : //RECOVERY\\BIOS\\Current\\Mandolin.fd
  gInsydeTokenSpaceGuid.PcdPeiRecoveryFile|L"$(BIOSNAME).fd"|VOID*|0x100

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdLegacyFree|$(LEGACY_FREE_SUPPORT)
!if $(LEGACY_FREE_SUPPORT) == YES
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdEspiKbc6064Enable|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNoneSioKbcSupport|TRUE
!else
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdEspiKbc6064Enable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdNoneSioKbcSupport|FALSE
!endif

  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPspEnable|TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspKvmEnable|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgGnbIoapicAddress|0xFEC01000

#[-start-240626-IB20840067-add]#
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdUsbKTOemConfigurationTable|{ \
    #Version_Major, Minor, Reserve, Reserve
    0x0D, 0x09, 0x00, 0x00, \
    #Usb 2.0 PHY Parameters
    # 1,COMPDISTUNE         - Disconnect Threshold Adjustment. Range 0 - 0x7.
    # 2,SQRXTUNE            - Squelch Threshold Adjustment. Range 0 - 0x7.
    # 3,TXFSLSTUNE          - FS/LS Source Impedance Adjustment. Range 0 - 0xF.
    # 4,TXPREEMPAMPTUNE     - HS Transmitter Pre-Emphasis Curent Control. Range 0 - 0x3.
    # 5,TXPREEMPPULSETUNE   - HS Transmitter Pre-Emphasis Duration Control. Range: 0 - 0x1.
    # 6,TXRISETUNE          - HS Transmitter Rise/Fall Time Adjustment. Range: 0 - 0x3.
    # 7,TXVREFTUNE          - HS DC Voltage Level Adjustment. Range 0 - 0xF.
    # 8,TXHSXVTUNE          - Transmitter High-Speed Crossover Adjustment. Range 0 - 0x3.
    # 9,TXRESTUNE           - USB Source Impedance Adjustment. Range 0 - 0x3.
    #Controller0 Port0 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller0 Port1 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller0 Port2 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller0 Port3 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller1 Port0 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller1 Port1 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller1 Port2 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller2 Port0 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller3 Port0 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Controller4 Port0 Default
    0x01, 0x03, 0x03, 0x02, 0x00, 0x02, 0x03, 0x03, 0x02, \
    #Usb 3 PHY Parameters
    # 1,TX_TERM_CTRL        - Tx term control. Range 0 - 0x7.
    # 2,RX_TERM_CTRL        - Rx term control. Range 0 - 0x7.
    # 3,TX_VBOOST_LVL_EN    - Enable override value for tx_vboost_lvl. Range 0 - 0x1.
    # 4,TX_VBOOST_LVL       - Override value for tx_vboost_lvl. Range 0 - 0x7.
    #Controller0 Port0 Default
    0x02, 0x02, 0x00, 0x05, \
    #Controller0 Port1 Default
    0x02, 0x02, 0x00, 0x05, \
    #Controller1 Port0 Default
    0x02, 0x02, 0x00, 0x05, \
    #Controller1 Port1 Default
    0x02, 0x02, 0x00, 0x05, \
    #BatteryChargerEnable
    0x00, \
    #PhyP3CpmP4Support
    0x00, \
    #ComboPhyStaticConfig0 0-Type C, 1- USB only mode, 2- DP only mode, 3- USB + DP
    0x00, \
    #ComboPhyStaticConfig1 0-Type C, 1- USB only mode, 2- DP only mode, 3- USB + DP
    0x00, \
    #ComboPhyStaticConfig2 0-Type C, 1- USB only mode, 2- DP only mode, 3- USB + DP
    0x01, \
    #Reserved
    0x00, 0x00, 0x00 \
  }
#[-end-240626-IB20840067-add]#

  # Let platform option to decide Smm lock.
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdSmmLock|TRUE
    #[-start-210909-IB19600001-add] enable BIT11 for UART 0
  ### @brief FCH device selection
  ### @details This control selects which FCH devices are to be enabled during
  ###  runtime. This is a bit-mapped value where each bit represents an FCH device.
  ###  A bit=1 indicates the device is enabled/active; a bit=0 indicates the device
  ###  is turned off.
  ###  Bits marked as 'read-only' or 'Reserved' cannot be modified via this control.
  ### @li BITs0-4  - Reserved
  ### @li BIT5 - I2C0
  ### @li BIT6 - I2C1
  ### @li BIT7 - I2C2
  ### @li BIT8 - I2C3
  ### @li BIT9 - I2C4
  ### @li BIT10 - I2C5
  ### @li BIT11 - UART0
  ### @li BIT12 - UART1
  ### @li BIT13 - I3C1
  ### @li BIT14 - I3C2
  ### @li BIT15 - I3C3
  ### @li BIT16 - UART2
  ### @li BITs17-20  - Reserved
  ### @li BIT21 - I3C0
  ### @li BITs22-25  - Reserved
  ### @li BIT26 - UART3
  ### @li BIT27 - ESPI
  ### @li BIT28 - Reserved
  ### @li BIT29 - HFP
  ### @li BIT30 - HID0
  ### @li BIT31 - HID2
  gEfiAmdAgesaModulePkgTokenSpaceGuid.FchRTDeviceEnableMap|0x04010800
  #[-start-210909-IB19600001-end] 

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspSystemTpmConfig|0xFF

  #PcdMpmEnable switch to turn off all MPM AGESA code path execution
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdMpmEnable|FALSE

  #Note:switch to turn off Wired KVM, Only take effect when PcdMpmEnable set to TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdMpmWiredKvmEnable|FALSE

  #Note: Only take effect when PcdMpmEnable set to TRUE
  #Switch to turn off WirelessManageability
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdMpmWirelessManageability|FALSE

  #Note: Only take effect when PcdMpmEnable set to TRUE and PcdMpmWirelessManageability set to TRUE
  #Switch to turn off Wireless KVM
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdMpmWirelessKvmEnable|FALSE

  #Note: Switch between Absolute/Simple protocol for KVM
  #Only valid if PcdMpmWiredKvmEnable or PcdMpmWirelessKvmEnable set to TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdMpmKvmMouseProtocol|0x0

#[-start-220308-IB14740223-add]#
!if $(ROM_ARMOR_DEFAULT_ENABLE) == YES
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspRomArmorSelection|3
!endif
#[-end-220308-IB14740223-add]#

#[-start-220401-IB14740231-add]#
  gH2OBdsDefaultBootListGenericOsTokenSpaceGuid.DebianSecureOs|L"\\EFI\\debian\\shimx64.efi\tdebian\tSECURE"
  gH2OBdsDefaultBootListGenericOsTokenSpaceGuid.DebianNormalOs|L"\\EFI\\debian\\grubx64.efi\tdebian\tNORMAL"
#[-end-220401-IB14740231-add]#

#[-start-221003-IB14740266-add]#
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
#[-end-221003-IB14740266-add]#

#[-start-221104-IB14740270-add]#
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdSmmProtectionsTestSupport|$(SMM_PROTECTION_TEST_SUPPORT)
#[-end-221104-IB14740270-add]#

################################################################################
#
# SMBIOS Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicExDefault]
  #
  # SMBIOS Pcd Section (Miscellaneous Types)
  # There are 2 kinds of SMBIOS you should take care of:
  # 1) Single record    - PcdType???Record    , without number suffix.
  # 2) Multiple record  - PcdType???Record*** , with number suffix.
  #
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType000|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType001|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType002|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType003|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType008|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType009|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType011|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType012|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType013|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType021|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType022|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType026|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType027|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType032|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType040|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType041|TRUE

  gSmbiosTokenSpaceGuid.PcdType000Record | { \
      0x00,                       \ # Type
      0x00,                       \ # Length
      UINT16(0x0000),             \ # Handle
      0xFF,                       \ # Vendor
      0xFF,                       \ # BIOS Version
      UINT16(0xE000),             \ # BIOS Starting Address Segment
      0xFF,                       \ # BIOS Release Date
      0xFF,                       \ # BIOS ROM Size
      UINT64(0x000000004BF99880), \ # BIOS Characteristics
      UINT16(0x0D03),             \ # BIOS Characteristics Extension Bytes
      0xFF,                       \ # System BIOS Major Release
      0xFF,                       \ # System BIOS Minor Release
      0xFF,                       \ # Embedded Controller Firmware Major Release
      0xFF,                       \ # Embedded Controller Firmware Minor Release
      0x00,                       \ # Extended BIOS ROM Size
      0x00                        \ # Extended BIOS ROM Size
  }
  gSmbiosTokenSpaceGuid.PcdType000Strings   |"Insyde Corp.;$(BIOSNAME)-RMB.05.46.50.0022;12/30/2024;"

  gSmbiosTokenSpaceGuid.PcdType001Record    |{0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x78, 0x56, 0x34, 0x12, 0x34, 0x12, 0x78, 0x56, 0x90, 0xAB, 0xCD, 0xDE, 0xEF, 0xAA, 0xBB, 0xCC, 0x06, 0x05, 0x06}
  gSmbiosTokenSpaceGuid.PcdType001Strings   |"AMD;$(BIOSNAME)-RMB;1;123456789;123456789;Rambrandt;"

  gSmbiosTokenSpaceGuid.PcdType002Record000 |{0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x09, 0x06, 0x03, 0x00, 0x0A, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType002Strings000|"AMD;$(BIOSNAME)-RMB;Base Board Version;Base Board Serial Number;Base Board Asset Tag;Base Board Chassis Location;"

#[-start-220225-IB14740219-modify]#
  gSmbiosTokenSpaceGuid.PcdType003Record000 |{0x03, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x03, 0x04, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05}
  gSmbiosTokenSpaceGuid.PcdType003Strings000|"Chassis Manufacturer;Chassis Version;Chassis Serial Number;Chassis Asset Tag;Chassis SKU;"

  gSmbiosTokenSpaceGuid.PcdType008Record000 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings000|"J3;USB 2.0 Type A;"
  gSmbiosTokenSpaceGuid.PcdType008Record001 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings001|"J46;USB 2.0 Type A;"
  gSmbiosTokenSpaceGuid.PcdType008Record002 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings002|"J63;USB 3.2 Type A;"
  gSmbiosTokenSpaceGuid.PcdType008Record003 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings003|"J89;USB 3.2 Type A;"
  gSmbiosTokenSpaceGuid.PcdType008Record004 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x23, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings004|"J50;USB 3.2 Type-C;"
  gSmbiosTokenSpaceGuid.PcdType008Record005 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x23, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings005|"J60;USB 3.2 Type-C;"
  gSmbiosTokenSpaceGuid.PcdType008Record006 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x0B, 0x1F}
  gSmbiosTokenSpaceGuid.PcdType008Strings006|"J180;Network;"
  gSmbiosTokenSpaceGuid.PcdType008Record007 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0xFF, 0x1F}
  gSmbiosTokenSpaceGuid.PcdType008Strings007|"J205;SFP;"
  gSmbiosTokenSpaceGuid.PcdType008Record008 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0xFF, 0x1F}
  gSmbiosTokenSpaceGuid.PcdType008Strings008|"J206;SFP;"
  gSmbiosTokenSpaceGuid.PcdType008Record009 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings009|"J37;DP;"
  gSmbiosTokenSpaceGuid.PcdType008Record010 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings010|"J48;DP;"
  gSmbiosTokenSpaceGuid.PcdType008Record011 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings011|"J53;DP;"
  gSmbiosTokenSpaceGuid.PcdType008Record012 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings012|"J48;DP;"
  gSmbiosTokenSpaceGuid.PcdType008Record013 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings013|"J64;HDMI;"
  gSmbiosTokenSpaceGuid.PcdType008Record014 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings014|"J128;DP;"
  gSmbiosTokenSpaceGuid.PcdType008Record015 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings015|"J129;HDMI;"
  gSmbiosTokenSpaceGuid.PcdType008Record016 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x1F, 0x1D}
  gSmbiosTokenSpaceGuid.PcdType008Strings016|"J9;Microphone;"
  gSmbiosTokenSpaceGuid.PcdType008Record017 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x1F, 0x1D}
  gSmbiosTokenSpaceGuid.PcdType008Strings017|"J10;Headphone;"
  gSmbiosTokenSpaceGuid.PcdType008Record018 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x22, 0x02, 0x00, 0x20}
  gSmbiosTokenSpaceGuid.PcdType008Strings018|"J87;iSATA;"
  gSmbiosTokenSpaceGuid.PcdType008Record019 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x22, 0x02, 0x00, 0x20}
  gSmbiosTokenSpaceGuid.PcdType008Strings019|"J88;iSATA;"

  gSmbiosTokenSpaceGuid.PcdType009Record000 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xBB, 0x0A, 0x03, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType009Strings000|"CON2;"
  gSmbiosTokenSpaceGuid.PcdType009Record001 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xBB, 0x0A, 0x03, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType009Strings001|"CON3;"
  gSmbiosTokenSpaceGuid.PcdType009Record002 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xBC, 0x0B, 0x03, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType009Strings002|"J31;"
  gSmbiosTokenSpaceGuid.PcdType009Record003 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xBC, 0x0B, 0x03, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType009Strings003|"J97;"
  gSmbiosTokenSpaceGuid.PcdType009Record004 |{0x09, 0x00, 0x00, 0x00, 0x01, 0x17, 0x0A, 0x03, 0x03, 0x03, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType009Strings004|"U93;"
#[-end-220225-IB14740219-modify]#

  gSmbiosTokenSpaceGuid.PcdType011Record    |{0x0B, 0x00, 0x00, 0x00, 0x05}
  gSmbiosTokenSpaceGuid.PcdType011Strings   |"String1 for Original Equipment Manufacturer;String2 for Original Equipment Manufacturer;String3 for Original Equipment Manufacturer;String4 for Original Equipment Manufacturer;String5 for Original Equipment Manufacturer;"

  gSmbiosTokenSpaceGuid.PcdType012Record    |{0x0C, 0x00, 0x00, 0x00, 0x04}
  gSmbiosTokenSpaceGuid.PcdType012Strings   |"String1 for Type12 Equipment Manufacturer;String2 for Type12 Equipment Manufacturer;String3 for Type12 Equipment Manufacturer;String4 for Type12 Equipment Manufacturer;"

  gSmbiosTokenSpaceGuid.PcdType013Record    |{0x0D, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}
  gSmbiosTokenSpaceGuid.PcdType013Strings   |"en|US|iso8859-1;fr|FR|iso8859-1;ja|JP|unicode;zh|TW|unicode;"

  gSmbiosTokenSpaceGuid.PcdType021Record000 |{0x15, 0x00, 0x00, 0x00, 0x07, 0x04, 0x04}
  gSmbiosTokenSpaceGuid.PcdType021Strings000|""

  gSmbiosTokenSpaceGuid.PcdType026Record000 |{0x1A, 0x00, 0x00, 0x00, 0x01, 0x42, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}
  gSmbiosTokenSpaceGuid.PcdType026Strings000|"Voltage Probe Description;"

  gSmbiosTokenSpaceGuid.PcdType032Record    |{0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType032Strings   |""

  #                                                                         Entry1 ---------------------------, Entry2----------------------------------
  gSmbiosTokenSpaceGuid.PcdType040Record    |{0x28, 0x00, 0x00, 0x00, 0x02, 0x06, 0x14, 0x00, 0x05, 0x01, 0xAA, 0x07, 0x00, 0x00, 0x05, 0x02, 0xDC, 0x05}
  gSmbiosTokenSpaceGuid.PcdType040Strings   |"PCIExpressx16;Compiler Version: VC 9.0;"

#[-start-220225-IB14740219-modify]#
  gSmbiosTokenSpaceGuid.PcdType041Record000 |{0x29, 0x00, 0x00, 0x00, 0x01, 0x85, 0x01, 0xFF, 0xFF, 0xFF, 0xFF}
  gSmbiosTokenSpaceGuid.PcdType041Strings000|"Marvell AQR113C;"
  gSmbiosTokenSpaceGuid.PcdType041Record001 |{0x29, 0x00, 0x00, 0x00, 0x01, 0x87, 0x01, 0xFF, 0xFF, 0xFF, 0xFF}
  gSmbiosTokenSpaceGuid.PcdType041Strings001|"Realtek ALC245;"
#[-end-220225-IB14740219-modify]#

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPad0|{                                                \  # The definition of I2C mouse PCD
    GUID({0xDB9DEEC2,0x9D01,0x4357,{0xAD,0xAA,0x0C,0xDF,0xBF,0x2D,0x57,0x11}}),             \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x0000002C),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x08,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0020),                                                                         \  # HID descriptor register number
    UINT16(0x0102),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x00,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C0
    0x01,                                                                                   \  # Bus configuration, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The GPIO host controller
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPad1|{                                                \  # The definition of I2C mouse PCD
    GUID({0x90944AAC,0xF33D,0x4F20,{0x8F,0x43,0x93,0xEB,0x5C,0xE6,0x8F,0x57}}),             \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x0000002C),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x08,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0020),                                                                         \  # HID descriptor register number
    UINT16(0x0102),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x01,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C1
    0x01,                                                                                   \  # Bus configuration, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The GPIO host controller
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPad2|{                                                \  # The definition of I2C mouse PCD
    GUID({0x29EF04B0,0x5A68,0x4A7C,{0x85,0x0B,0x29,0x5E,0xA5,0x70,0x27,0x18}}),             \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x0000002C),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x08,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0020),                                                                         \  # HID descriptor register number
    UINT16(0x0102),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x02,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C2
    0x00,                                                                                   \  # Bus configuration, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The GPIO host controller
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPad3|{                                                \  # The definition of I2C mouse PCD
    GUID({0xD7247D2A,0x43E9,0x4F10,{0xAC,0x02,0xDE,0x47,0x18,0x53,0x8B,0x2D}}),             \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x0000002C),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x08,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0020),                                                                         \  # HID descriptor register number
    UINT16(0x0102),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x03,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C3
    0x00,                                                                                   \  # Bus configuration, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The GPIO host controller
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPanel0|{                                              \  # The definition of I2C mouse PCD
    GUID({ 0x680F59EE, 0x8775, 0x4FE6, {0xB2, 0x67, 0xCC, 0xE0, 0xAB, 0xCA, 0x9C, 0x65} }), \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x00000010),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x20,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0001),                                                                         \  # HID descriptor register number
    UINT16(0x0d00),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x00,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C0
    0x01,                                                                                   \  # The index of bus configuration to the PcdI2cBusSpeedTable, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The number of GPIO controller, 0 based
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPanel1|{                                              \  # The definition of I2C mouse PCD
    GUID({ 0x8BF15CDC, 0xB6C1, 0x42DB, {0xBB, 0x2E, 0x6F, 0xAD, 0x8F, 0xAE, 0x92, 0x0A} }), \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x00000010),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x20,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0001),                                                                         \  # HID descriptor register number
    UINT16(0x0d00),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x01,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C1
    0x01,                                                                                   \  # The index of bus configuration to the PcdI2cBusSpeedTable, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The number of GPIO controller, 0 based
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPanel2|{                                              \  # The definition of I2C mouse PCD
    GUID({ 0x55443ED9, 0xB699, 0x4B29, {0x92, 0x48, 0xF4, 0x33, 0x08, 0x3F, 0x4F, 0xF8} }), \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x00000010),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x20,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0001),                                                                         \  # HID descriptor register number
    UINT16(0x0d00),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x02,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C2
    0x01,                                                                                   \  # The index of bus configuration to the PcdI2cBusSpeedTable, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The number of GPIO controller, 0 based
  }

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchPanel3|{                                              \  # The definition of I2C mouse PCD
    GUID({ 0x78BDEE0E, 0x7FB0, 0x4EDC, {0xBB, 0xC3, 0xC6, 0x16, 0xC4, 0x42, 0xD5, 0xBB} }), \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x00000010),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x20,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0001),                                                                         \  # HID descriptor register number
    UINT16(0x0d00),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x03,                                                                                   \  # The index of I2C Host controller to the PcdI2cControllerTable, 0 based, FCH I2C3
    0x01,                                                                                   \  # The index of bus configuration to the PcdI2cBusSpeedTable, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x00                                                                                    \  # The number of GPIO controller, 0 based
  }

[PcdsDynamicHii]

[Libraries]

[LibraryClasses]
  SmbusLib|MdePkg/Library/DxeSmbusLib/DxeSmbusLib.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  SysCallLib|SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf

  # PcdSerialPortSelect   # 0 = FCH UART 0
                          # 1 = FCH UART 1
                          # 2 = Simnow ITE8712 Sio Serial Port
                          # 3 = Emulation Port 0x80 out
                          # 4 = Simnow console input from Simnow ITE8712 SIO serial port, and Console output to Simnow Console Output
                          # 5 = SMSC Sio Serial Port
                          # 6 = Simnow SMSC 1100 Sio Serial Port
                          # 7 = Unify Message
!if $(EFI_DEBUG) == YES || $(USB_DEBUG_SUPPORT) == YES  || ($(INSYDE_DEBUGGER) == YES and $(H2O_DDT_DEBUG_IO) == Com)
  !if (gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 0 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 1 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 8 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 9 || \
       gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 10)
    PlatformHookLib|AmdCpmPkg/Library/CommonLib/BasePlatformHookLibAmdFchUart/BasePlatformHookLibAmdFchUart.inf
    SerialPortLib|AmdCpmPkg/Library/CommonLib/BaseSerialPortLib16550AmdFchUart/BaseSerialPortLib16550AmdFchUart.inf
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 2
    PlatformHookLib|AmdCpmPkg/Library/CommonLib/BasePlatformHookLibAmdIte8712/BasePlatformHookLibAmdIte8712.inf
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 3
    SerialPortLib|$(PROJECT_PKG)/Library/BaseSerialPortLibPort80/BaseSerialPortLibPort80.inf
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 6
    PlatformHookLib|AmdCpmPkg/Library/CommonLib/BasePlatformHookLibAmdSmsc1100/BasePlatformHookLibAmdSmsc1100.inf
  !elseif gPlatformPkgTokenSpaceGuid.PcdSerialPortSelect == 7
    SerialPortLib|AmdCpmPkg/Library/CommonLib/AmdUnifyMessageLib/AmdUnifyMessageLib.inf
  !endif
  !if $(USE_AGESA_DEBUGLIB) == YES
    DebugLib|AgesaModulePkg/Library/DebugLibIdsDp/DebugLibIdsDp.inf
  !else
    DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  !endif
!endif

[LibraryClasses.common]

[LibraryClasses.common.SEC]
  SecOemSvcChipsetLib|$(PROJECT_PKG)/Library/SecOemSvcChipsetLib/SecOemSvcChipsetLib.inf

[LibraryClasses.common.PEI_CORE]
  PeiOemSvcKernelLib|$(PROJECT_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessSmnLib.inf

[LibraryClasses.common.PEIM]
  PeiOemSvcKernelLib|$(PROJECT_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf
  OemAgesaCcxPlatformLib|$(CHIPSET_PKG)/Library/AgesaCcxPlatformLib/AgesaCcxPlatformLib.inf
  GpioCfgLib|$(PROJECT_PKG)/Library/GpioCfgLib/GpioCfgLib.inf
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessSmnLib.inf

[LibraryClasses.common.DXE_CORE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  !if $(USE_AGESA_DEBUGLIB) == YES
    DebugLib|AgesaModulePkg/Library/DebugLibIdsDp/DebugLibIdsDp.inf
  !else
#    DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  !endif

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[LibraryClasses.common.DXE_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf
  !if $(USE_AGESA_DEBUGLIB) == YES
    DebugLib|AgesaModulePkg/Library/DebugLibIdsDp/DebugLibIdsDp.inf
  !else
#    DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  !endif
#[-start-220301-IB14740220-remove]#
#  !if $(SMM_ISOLATION_SUPPORT) == YES
#    BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf {
#      <SOURCE_OVERRIDE_PATH>
#        $(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/SmmSupervisorPkg/Library/BaseLibSysCall
#    }
#  !endif
#[-end-220301-IB14740220-remove]#
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf
#[-start-221104-IB14740270-add]#
!if $(SMM_ISOLATION_SUPPORT) == YES
  BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
#[-start-240202-IB20840047-modify]#
  CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLib.inf
#[-end-240202-IB20840047-modify]#
!endif
#[-end-221104-IB14740270-add]#

[LibraryClasses.common.COMBINED_SMM_DXE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf
#[-start-220301-IB14740220-remove]#
#  !if $(SMM_ISOLATION_SUPPORT) == YES
#    SmmPolicyGateLib|SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
#    BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf {
#      <SOURCE_OVERRIDE_PATH>
#        $(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/SmmSupervisorPkg/Library/BaseLibSysCall
#    }
#    IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
#    CpuLib|SmmSupervisorPkg/Library/BaseCpuLibSysCall/BaseCpuLib.inf
#  !endif
#[-end-220301-IB14740220-remove]#
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf
#[-start-221104-IB14740270-add]#
!if $(SMM_ISOLATION_SUPPORT) == YES
  BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
#[-start-240202-IB20840047-modify]#
  CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLib.inf
#[-end-240202-IB20840047-modify]#
!endif
#[-end-221104-IB14740270-add]#

[LibraryClasses.common.SMM_CORE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf
#[-start-221104-IB14740270-add]#
!if $(SMM_ISOLATION_SUPPORT) == YES
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
!endif
#[-end-221104-IB14740270-add]#

[LibraryClasses.common.UEFI_APPLICATION]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[Components.IA32]

################################################################################
#
# Platform related components
#
################################################################################
[Components.X64]

!if $(AMD_APP_INSTEAD_EFI_SHELL_SUPPORT) == YES
  EmulationToolsPkg/EmuLinuxLoader/EmuLinuxLoader.inf
  EmulationToolsPkg/EmuLinuxTest/EmuLinuxTest.inf
!endif

!if $(KVM_SUPPORT) == YES
  #
  # For Commercial Cezanne will support KVM.
  #
  AgesaModulePkg/Psp/AmdPspKvm/AmdPspKvm.inf
!endif

[Components.$(PEI_ARCH)]
#[-start-220221-IB14740215-remove]#
#!if gSioGuid.PcdSioDummySupported
#  !disable SioDummyPkg/SioDummyPei/SioDummyPei.inf
#!endif
#[-end-220221-IB14740215-remove]#

!if $(USE_FAST_CRISIS_RECOVERY) == NO
  $(CHIPSET_PKG)/GfxInitPei/GfxInitPei.inf
!endif

[Components.$(DXE_ARCH)]
  #
  # Replace DebugLib instance with null lib to avoid USB function not working with efidebug build
  #
  !disable AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDispatcher.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDispatcher.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }



  #
  # ACPI
  #
#[-start-220126-IB14740206-modify]#
  $(PROJECT_PKG)/AcpiTables/AcpiTables.inf {
#[-end-220126-IB14740206-modify]#
    <BuildOptions>
!if gChipsetPkgTokenSpaceGuid.PcdH2ONbIoApicSupport
      *_*_*_ASLCC_FLAGS        = -DNB_IOAPIC_SUPPORT
      *_*_*_ASLPP_FLAGS        = -DNB_IOAPIC_SUPPORT
!endif
!if gChipsetPkgTokenSpaceGuid.PcdH2OLegacyFreeSupported
      *_*_*_ASLCC_FLAGS        = -DLEGACY_FREE_SUPPORT
      *_*_*_ASLPP_FLAGS        = -DLEGACY_FREE_SUPPORT
!endif
!if gAmdCpmPkgTokenSpaceGuid.PcdModernStandbySupport
      *_*_*_ASLCC_FLAGS        = -DMODERN_STANDBY_SUPPORT
      *_*_*_ASLPP_FLAGS        = -DMODERN_STANDBY_SUPPORT
!endif
!if gPlatformPkgTokenSpaceGuid.PcdEmulationOnly
      *_*_*_ASLCC_FLAGS        = -DEMULATION_ONLY
      *_*_*_ASLPP_FLAGS        = -DEMULATION_ONLY
!endif
!if gAmdCpmPkgTokenSpaceGuid.PcdReduceHardwareSupport
      *_*_*_ASLCC_FLAGS        = -DREDUCED_HARDWARE_SUPPORT
      *_*_*_ASLPP_FLAGS        = -DREDUCED_HARDWARE_SUPPORT
!endif
  }

  $(PROJECT_PKG)/Binary/Bcm5762/PxeUefi/Bcm5762UndiX64.inf
  $(PROJECT_PKG)/Binary/RtkUsbUndiDxe/RtkUsbUndiDxe.inf
  $(PROJECT_PKG)/Binary/RtkLan/RtkUndiDxe/RtkUndiDxe.inf

#[-start-220301-IB14740220-remove]#
#!if $(SMM_ISOLATION_SUPPORT) == YES
#  !disable AmdSupervisorPkg/Binary/SmmSupervisor/Release/SmmSupervisorBin.inf
#  AmdSupervisorPkg/Binary/SmmSupervisor/Release/SmmSupervisorBin.inf {
#    <SOURCE_OVERRIDE_PATH>
#      $(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/AmdSupervisorPkg/Binary/SmmSupervisor/Release/
#  }
#!endif
#[-end-220301-IB14740220-remove]#

# Add for HSP ACPI repot from AGESA, good sample for Platform BIOS
# AgesaPkg/Addendum/Psp/HSPfTPMAcpi/HSPfTPMAcpi.inf
# Temporary just to avoid the compiler error
#[-start-220301-IB14740220-remove]#
#$(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/AgesaPkg/Addendum/Psp/HSPfTPMAcpi/HSPfTPMAcpi.inf
#  $(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/AgesaPkg/Addendum/Psp/HSPfTPMAcpi/HspAcpifTpmSmm.inf
#[-end-220301-IB14740220-remove]#

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions.common.EDKII]
  GCC:*_*_*_CC_FLAGS             = -DUSE_EDKII_HEADER_FILE -Wno-error $(CC_FLAGS)
  GCC:*_*_IA32_CC_FLAGS          = -Wno-unused-local-typedefs -Wno-pointer-to-int-cast -Wno-unused-function -Wno-parentheses
  GCC:RELEASE_*_*_CC_FLAGS       = -DMDEPKG_NDEBUG
  GCC:*_*_IA32_JWASM_FLAGS       =
  GCC:*_*_X64_JWASM_FLAGS        =
  INTEL:*_*_*_CC_FLAGS           = /D MDEPKG_NDEBUG $(CC_FLAGS)
  MSFT:RELEASE_*_*_CC_FLAGS      = /D MDEPKG_NDEBUG $(CC_FLAGS)
  MSFT:DEBUG_*_*_CC_FLAGS        = $(CC_FLAGS)
!if $(EFI_DEBUG) == NO
  MSFT:DEBUG_*_*_CC_FLAGS        = /D MDEPKG_NDEBUG $(CC_FLAGS)
!endif
#[-start-230811-IB20840013-remove]#
#!if $(INSYDE_DEBUGGER) == YES
#  MSFT:DEBUG_*_*_CC_FLAGS        = /Od
#!endif
#[-end-230811-IB20840013-remove]#
  *_*_*_ASLPP_FLAGS              = $(CC_FLAGS)

!if $(EMULATION_ONLY) == YES
  *_*_*_CC_FLAGS           = -DEMULATION_ONLY
!endif

!if $(LPDDR5) == YES
  MSFT:*_*_*_VFRPP_FLAGS = /D LPDDR5
!endif
