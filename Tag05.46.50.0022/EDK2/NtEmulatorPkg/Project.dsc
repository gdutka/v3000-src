## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
!import NtEmulatorPkg/Package.dsc
!import ShellPkg/Package.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = NtEmulatorPkg
  PLATFORM_GUID                  = 7f48118c-2f97-4d39-972a-e836358b7433
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PROJECT_PKG)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Build/$(PROJECT_PKG)/Project.fdf

  #
  # Defines for default states.  These can be changed on the command line.
  # -D FLAG=VALUE
  #
  !include $(PROJECT_PKG)/Project.env
  DEFINE SECURE_BOOT_ENABLE      = FALSE
  RFC_LANGUAGES                  = "en-US;fr-FR;zh-TW;ja-JP;x-UEFI-ns"


################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]
!if $(EMULATOR_NOSCU) == FALSE
  #
  # ChipsetPkg OemSvc
  #
  BaseOemSvcChipsetLib|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf
  BaseOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLibDefault.inf
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf

  #
  # Project.dsc override OemSvc, need change $(PROJECT_PKG) => $(BOARD_PKG)
  # PROJECT_PKG mean NtEmulatorPkg
  #

  # override InsydeOemServicesPkg
  BaseOemSvcKernelLib|$(BOARD_PKG)/Library/BaseOemSvcKernelLib/BaseOemSvcKernelLib.inf
  DxeOemSvcKernelLib|$(BOARD_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

  # override ChipsetPkg
  BaseOemSvcChipsetLib|$(BOARD_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf

  #
  # SetupDataProvider / ChipsetSvcDxe / SetupUtilityDxe relative library
  #
  TbtLib|$(CHIPSET_PKG)/Library/TbtLib/TbtLib.inf

  BaseInsydeChipsetLib|$(CHIPSET_PKG)/Library/BaseInsydeChipsetLib/BaseInsydeChipsetLib.inf

  PchPsfPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchPsfPrivateLib/PeiDxeSmmPchPsfPrivateLib$(PCH).inf
  DxeMeLib|$(PLATFORM_SI_PACKAGE)/Me/Library/DxeMeLib/DxeMeLib.inf
  MeChipsetLib|$(PLATFORM_SI_PACKAGE)/Me/Library/PeiDxeMeChipsetLib/PeiDxeMeChipsetLib.inf
  MmPciLib|$(PLATFORM_SI_PACKAGE)/Library/PeiDxeSmmMmPciLib/PeiDxeSmmMmPciLib.inf
  PchCycleDecodingLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchCycleDecodingLib/PeiDxeSmmPchCycleDecodingLib.inf
  PchPcrLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
  PchInfoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib$(PCH).inf
  GpioLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGpioLib/PeiDxeSmmGpioLib.inf
  InsydeChipsetEcLib|$(CHIPSET_PKG)/Library/BaseInsydeChipsetEcLib/BaseInsydeChipsetEcLib.inf
  PeiInsydeChipsetLib|$(CHIPSET_PKG)/Library/PeiInsydeChipsetLib/PeiInsydeChipsetLib.inf
  PchSbiAccessLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSbiAccessLib/PeiDxeSmmPchSbiAccessLib.inf
  PttHeciLib|$(PLATFORM_SI_PACKAGE)/Me/Library/DxePttHeciLib/DxePttHeciLib.inf
  CpuPlatformLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf
  PchPcieRpLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchPcieRpLib/PeiDxeSmmPchPcieRpLib.inf
  DxeInsydeChipsetLib|$(CHIPSET_PKG)/Library/DxeInsydeChipsetLib/DxeInsydeChipsetLib.inf
  CpuMailboxLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuMailboxLib/PeiDxeSmmCpuMailboxLib.inf
  PchGbeLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchGbeLib/PeiDxeSmmPchGbeLib.inf
  DxeOverClockLib|$(CHIPSET_PKG)/Features/OverClocking/OverClockInit/DxeOverClockLib.inf
  AslUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  PchSerialIoLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSerialIoLib/PeiDxeSmmPchSerialIoLibCnl.inf
  BootGuardLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmBootGuardLib/PeiDxeSmmBootGuardLib.inf
  DxeAmtLib|$(PLATFORM_SI_PACKAGE)/Me/Library/DxeAmtLib/DxeAmtLib.inf
  TbtCommonLib|$(CHIPSET_PKG)/Library/TbtCommonLib/TbtCommonLib.inf
  GpioExpanderLib|$(CHIPSET_PKG)/Library/BaseGpioExpanderLib/BaseGpioExpanderLib.inf
  I2cAccessLib|$(CHIPSET_PKG)/Library/PeiI2cAccessLib/PeiI2cAccessLib.inf
  DxeTbtPolicyLib|$(CHIPSET_PKG)/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.inf
  PmcPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPmcPrivateLib/PeiDxeSmmPmcPrivateLibCnl.inf
  GpioPrivateLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmGpioPrivateLib/PeiDxeSmmGpioPrivateLibCnl.inf
  SataLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmSataLib/PeiDxeSmmSataLib$(PCH).inf
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf
  PmcLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPmcLib/PeiDxeSmmPmcLib.inf
  GpioNameBufferLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/DxeGpioNameBufferLib/DxeGpioNameBufferLib.inf
  PchDmiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchDmiLib/PeiDxeSmmPchDmiLib.inf
  SaPlatformLib|$(PLATFORM_SI_PACKAGE)/SystemAgent/Library/PeiDxeSmmSaPlatformLib/PeiDxeSmmSaPlatformLib.inf
  PchEspiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchEspiLib/PeiDxeSmmPchEspiLib.inf
  PchFiaLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchFiaLib/PeiDxeSmmPchFiaLib$(PCH).inf
  DimmInfoLib|$(CHIPSET_PKG)/UefiSetupUtilityDxe/Advance/DimmInfoLib.inf
  CnviLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmCnviLib/PeiDxeSmmCnviLib.inf
  GbeMdiLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmGbeMdiLib/PeiDxeSmmGbeMdiLib.inf
  DxeTbtSecurityLib|$(CHIPSET_PKG)/Features/Tbt/Library/DxeTbtSecurityLib/DxeTbtSecurityLib.inf
  PchSerialIoUartLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchSerialIoUartLib/PeiDxeSmmPchSerialIoUartLib.inf
  PchPciExpressHelpersLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchPciExpressHelpersLib/PeiDxeSmmPchPciExpressHelpersLib.inf
  PchInitCommonLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/Private/PeiDxeSmmPchInitCommonLib/PeiDxeSmmPchInitCommonLib.inf


!endif

[LibraryClasses.common.PEIM]
!if $(EMULATOR_NOSCU) == FALSE
  #
  # Project.dsc override OemSvc, need change $(PROJECT_PKG) => $(BOARD_PKG)
  # PROJECT_PKG mean NtEmulatorPkg
  #
  # override InsydeOemServicesPkg
  PeiOemSvcKernelLib|$(BOARD_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf

  #
  # SetupDataProvider relative library
  #
  KernelConfigLib|$(CHIPSET_PKG)/Library/PeiKernelConfigLib/PeiKernelConfigLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
!endif

[PcdsFeatureFlag]
  gEfiNtEmulatorPkgTokenSpaceGuid.PcdEdk2FormBrowserSupported|FALSE
  #
  # Network
  #
  gInsydeTokenSpaceGuid.PcdH2ONetworkSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ONetworkHttpSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ONetworkTlsSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ONetworkIscsiSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ONetworkIpv6Supported|FALSE

  gInsydeTokenSpaceGuid.PcdH2OBdsCpSendFormAfterSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OSetupChangeDisplaySupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDEShowFpsSupport|TRUE

  gInsydeTokenSpaceGuid.PcdShellBinSupported|TRUE
  gInsydeTokenSpaceGuid.PcdShellBuildSupported|FALSE

  gInsydeTokenSpaceGuid.PcdH2ORotateScreenSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ORotateShellSupported|FALSE
!errif (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported == FALSE) and (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported == FALSE), "Must have at least one display engine enabled in Project.dsc"
!errif (gInsydeTokenSpaceGuid.PcdShellBinSupported == TRUE) and (gInsydeTokenSpaceGuid.PcdShellBuildSupported == TRUE), "Must NOT set both ShellBin and ShellBuild PCDs as TRUE, otherwise it will build fail by duplicated file GUID."

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|0
  gEfiMdePkgTokenSpaceGuid.PcdMaximumUnicodeStringLength|0
  #
  # Network
  #
  gEfiNetworkPkgTokenSpaceGuid.PcdAllowHttpConnections|TRUE

  gInsydeTokenSpaceGuid.PcdScuFormsetGuidList|{ \
    GUID("C1E0B01A-607E-4B75-B8BB-0631ECFAACF2"), \ # Main
    GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC"), \ # Avance
    GUID("5204F764-DF25-48A2-B337-9EC122B85E0D"), \ # Security
    GUID("A6712873-925F-46C6-90B4-A40F86A0917B"), \ # Power
    GUID("2D068309-12AC-45AB-9600-9187513CCDD8"), \ # Boot
    GUID("B6936426-FB04-4A7B-AA51-FD49397CDC01"), \ # Exit
    GUID("00000000-0000-0000-0000-000000000000")}

  gInsydeTokenSpaceGuid.PcdScuFormsetFlagList|{ \
    UINT8(0), \ # Main
    UINT8(0), \ # Avance
    UINT8(0), \ # Security
    UINT8(0), \ # Power
    UINT8(0), \ # Boot
    UINT8(0), \ # Exit
    UINT8(0xFF)}

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{GUID("7C04A583-9E3E-4F1C-AD65-E05268D0B4D1")}


[PcdsDynamicExDefault]
  gInsydeTokenSpaceGuid.PcdH2ORotateScreenIs90DegreeClockwise|FALSE
  gInsydeTokenSpaceGuid.PcdH2OBoardId|0
  gInsydeTokenSpaceGuid.PcdH2OBoardIds|{0}
  gInsydeTokenSpaceGuid.PcdH2OBoardIdList|{0}
  gInsydeTokenSpaceGuid.PcdH2OBoardIdSupportedList|{0}

[Components]
!if $(EMULATOR_NOSCU) == FALSE

  #
  # Below driver will provide some dummy protocol to solve driver dependency.
  #
  $(PROJECT_PKG)/Override/$(BOARD_REL_PATH)/$(BOARD_PKG)/ChipsetWinNtPei/ChipsetWinNtPei.inf
  $(PROJECT_PKG)/Override/$(BOARD_REL_PATH)/$(BOARD_PKG)/ChipsetWinNtDxe/ChipsetWinNtDxe.inf

  #
  # When library use $(PROJECT_PKG)\Project.dec in INF file, PROJECT_PKG is NtEmulatorPkg
  # it can't find header files in $(PROJECT_PKG)\Include.
  # it need add below line
  # *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_PKG)\Include
  #
  #

  #
  # If need override .h header file, please add the header file to below folder
  # *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PROJECT_PKG)\Override\$(CHIPSET_PKG)\Include
  #

  $(CHIPSET_PKG)/UefiSetupUtilityDxe/SetupUtilityDxe.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PROJECT_REL_PATH)\$(PROJECT_PKG)\Override\$(CHIPSET_REL_PATH)\$(CHIPSET_PKG)\Include \
                       -I$(WORKSPACE)\$(PROJECT_REL_PATH)\$(PROJECT_PKG)\Override\$(CHIPSET_REL_PATH)\$(PLATFORM_SI_PACKAGE)\Include \
                        -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
#    <LibraryClasses>
#      # unresolved external symbol _GetRandomNumber64
#      NULL|MdePkg/Library/BaseRngLib/BaseRngLib.inf
    <SOURCE_OVERRIDE_PATH>
       $(PROJECT_PKG)/Override/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/UefiSetupUtilityDxe

    <LibraryClasses>
       NULL|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  }

  $(CHIPSET_PKG)/ChipsetSvcDxe/ChipsetSvcDxe.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
    <LibraryClasses>
      NULL|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  }

  $(BOARD_PKG)/Library/BaseOemSvcKernelLib/BaseOemSvcKernelLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
  }

  $(BOARD_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
  }

  $(BOARD_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
    <SOURCE_OVERRIDE_PATH>
      $(PROJECT_PKG)/Override/$(BOARD_REL_PATH)/$(BOARD_PKG)/Library/DxeOemSvcKernelLib
  }

  $(BOARD_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
  }

  $(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
  }

  $(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(BOARD_REL_PATH)\$(BOARD_PKG)\Include
  }

  $(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib/PeiDxeSmmCpuPlatformLib.inf {
    <SOURCE_OVERRIDE_PATH>
      NtEmulatorPkg/Override/$(CHIPSET_REL_PATH)/$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeSmmCpuPlatformLib
  }

  $(CHIPSET_PKG)/Library/DxeInsydeChipsetLib/DxeInsydeChipsetLib.inf {
	<SOURCE_OVERRIDE_PATH>
      NtEmulatorPkg/Override/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Library/DxeInsydeChipsetLib
  }

#  $(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib.inf {
#    <BuildOptions>
#      *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PROJECT_REL_PATH)\$(PROJECT_PKG)\Override\$(PLATFORM_SI_PACKAGE)\Pch\Include
#  }


!endif

[BuildOptions]
  MSFT:*_*_*_CC_FLAGS            =  $(RC_FLAGS)
  MSFT:RELEASE_*_*_CC_FLAGS      = -D MDEPKG_NDEBUG
!if $(EFI_DEBUG) == NO
  MSFT:DEBUG_*_*_CC_FLAGS        = -D MDEPKG_NDEBUG
!endif
  *_*_*_VFRPP_FLAGS              =  $(RC_FLAGS)
