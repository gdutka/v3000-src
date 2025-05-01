## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2012 - 2014, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
##
!import NtEmulatorPkg/Package.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = StrawberryMountainNtEmulatorPkg
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
  RFC_LANGUAGES                  = "en-US;fr-FR;zh-TW;ja-JP"


################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]

  #
  # chipset / platform package
  #
  PlatformVfrLib|$(CHIPSET_PKG)/UefiSetupUtilityDxe/PlatformVfrLib.inf
  ChipsetCpuLib|$(CHIPSET_PKG)/Library/ChipsetCpuLib/ChipsetCpuLib.inf
  DxeOemSvcKernelLib|$(PLATFORM_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf


  CrVfrConfigLib|$(CHIPSET_PKG)/Library/CrVfrConfigLibNull/CrVfrConfigLibNull.inf
  CrConfigDefaultLib|$(CHIPSET_PKG)/Library/CrConfigDefaultLibNull/CrConfigDefaultLibNull.inf
  PostCodeLib|MdePkg/Library/PeiDxePostCodeLibReportStatusCode/PeiDxePostCodeLibReportStatusCode.inf
  IpmiHobLib|InsydeIpmiPkg/Library/BaseIpmiHobLib/BaseIpmiHobLib.inf
  IpmiCommonLib|InsydeIpmiPkg/Library/BaseIpmiCommonLib/BaseIpmiCommonLib.inf


[LibraryClasses.common.PEIM]
  PeiOemSvcKernelLib|$(PLATFORM_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf
  KernelConfigLib|$(CHIPSET_PKG)/Library/PeiKernelConfigLib/PeiKernelConfigLib.inf

[PcdsFeatureFlag]
  gEfiNtEmulatorPkgTokenSpaceGuid.PcdEdk2FormBrowserSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2ONetworkSupported|TRUE

  gInsydeTokenSpaceGuid.PcdGraphicsSetupSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|FALSE

!if gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported == TRUE
  gInsydeTokenSpaceGuid.PcdGraphicsSetupSupported|FALSE
!endif

!if gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported == FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|FALSE
!endif

[PcdsFixedAtBuild]

[Components]


  #
  #  BugBug: LibraryClass BuildOptions override need put in [Components] section
  #
  $(PLATFORM_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PLATFORM_PKG)\Include
    <SOURCE_OVERRIDE_PATH>
       $(PROJECT_PKG)/Override/$(PLATFORM_PKG)/Library/PeiOemSvcKernelLib
  }

  $(PLATFORM_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PLATFORM_PKG)\Include
    <SOURCE_OVERRIDE_PATH>
      $(PROJECT_PKG)/Override/$(PLATFORM_PKG)/Library/DxeOemSvcKernelLib
  }

  #
  # Chipset / Platform driver
  #
  $(PROJECT_PKG)/ChipsetWinNtDriverPei/ChipsetWinNtDriverPei.inf
  $(PROJECT_PKG)/ChipsetWinNtDriverDxe/ChipsetWinNtDriverDxe.inf

  $(CHIPSET_PKG)/ChipsetSvcDxe/ChipsetSvcDxe.inf {
    <LibraryClasses>
      NULL|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  }

  $(CHIPSET_PKG)/UefiSetupUtilityDxe/SetupUtilityDxe.inf {
    <BuildOptions>
       *_*_*_CC_FLAGS = -I$(WORKSPACE)\$(PROJECT_PKG)\Include
  }

