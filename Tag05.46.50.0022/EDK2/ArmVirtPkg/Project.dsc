## @file
#
#******************************************************************************
#* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
#

!import ArmVirtPkg/Package.dsc

[Packages]
  CryptoPkg/CryptoPkg.dec|InsydeModulePkg/InsydeModulePkg.dec

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = ArmVirtQemu
  PLATFORM_GUID                  = 37d7e986-f7e9-45c2-8067-e371421a626c
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PROJECT_PKG)
  SUPPORTED_ARCHITECTURES        = AARCH64|ARM
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
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
# SKU Identification section - list of all SKU IDs supported by this Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses.common]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf {
    <SOURCE_OVERRIDE_PATH>
      MdePkg/Override/Library/BaseLib
  }
  PostCodeLib|MdePkg/Library/PeiDxePostCodeLibReportStatusCode/PeiDxePostCodeLibReportStatusCode.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  LockBoxLib|MdeModulePkg/Library/LockBoxNullLib/LockBoxNullLib.inf
  #
  # HII
  #
  HiiLib|InsydeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  HiiDbLib|InsydeModulePkg/Library/H2OHiiDbLib/H2OHiiDbLib.inf
  HiiStringLib|InsydeModulePkg/Library/H2OHiiStringLib/H2OHiiStringLib.inf
  HiiConfigAccessLib|InsydeModulePkg/Library/H2OHiiConfigAccessLib/H2OHiiConfigAccessLib.inf
  HiiExLib|InsydeModulePkg/Library/UefiHiiExLib/UefiHiiExLib.inf
  BitBltLib|InsydeModulePkg/Library/BitBltLib/BitBltLib.inf
  CollationLib|InsydeModulePkg/Library/H2OUnicodeCollationLib/H2OUnicodeCollationLib.inf
  ConsoleLib|InsydeModulePkg/Library/ConsoleLib/ConsoleLib.inf
  RectLib|InsydeModulePkg/Library/RectLib/RectLib.inf
  H2OImageDecoderLib|InsydeModulePkg/Library/H2OImageDecoderLib/H2OImageDecoderLib.inf
  #
  # H2O Form Browser / Display Engine
  #
  LayoutLib|InsydeSetupPkg/Library/LayoutSupportLib/LayoutSupportLib.inf
  H2ODisplayEngineLib|InsydeSetupPkg/Library/H2ODisplayEngineLib/H2ODisplayEngineLib.inf
  Win32Lib|InsydeModulePkg/Library/Win32Lib/Win32Lib.inf
  #
  # Setup Utility
  #
  SetupUtilityLib|InsydeModulePkg/Library/SetupUtilityLib/SetupUtilityLib.inf
  KernelConfigLib|InsydeModulePkg/Library/DxeKernelConfigLib/DxeKernelConfigLib.inf
  BaseSetupDefaultLib|InsydeModulePkg/Library/SetupDefaultLib/SetupDefaultLib.inf
  H2OHddPasswordTableLib|InsydeModulePkg/Library/H2OHddPasswordTableLib/H2OHddPasswordTableLib.inf
  SavedPasswordStringProcessLib|InsydeModulePkg/Library/SavedPasswordStringProcessLib/SavedPasswordStringProcessLib.inf
  #
  # BDS
  #
  GenericBdsLib|InsydeModulePkg/Library/GenericBdsLib/GenericBdsLib.inf
  GenericUtilityLib|InsydeModulePkg/Library/GenericUtilityLib/GenericUtilityLib.inf
  #
  # TCG
  #
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf {
    <SOURCE_OVERRIDE_PATH>
    SecurityPkg/Override/Library/Tpm2CommandLib
  }
  TcgStorageCoreLib|SecurityPkg/Library/TcgStorageCoreLib/TcgStorageCoreLib.inf
  TcgStorageOpalLib|InsydeModulePkg/Library/TcgStorageOpalLib/TcgStorageOpalLib.inf
  Tcg2PhysicalPresenceLib|InsydeModulePkg/Library/DxeTcg2PhysicalPresenceLib/DxeTcg2PhysicalPresenceLib.inf
  #
  # Insyde kernel libraries
  #
  H2OLib|InsydeModulePkg/Library/DxeH2OLib/DxeH2OLib.inf
  H2OCpLib|InsydeModulePkg/Library/DxeH2OCpLib/DxeH2OCpLib.inf
  BvdtLib|InsydeModulePkg/Library/BvdtLib/BvdtLib.inf
  CmosLib|InsydeModulePkg/Library/CmosLibNull/CmosLibNull.inf
  StdLib|InsydeModulePkg/Library/StdLib/StdLib.inf
  H2ODebugTraceErrorLevelLib|InsydeModulePkg/Library/H2ODebugTraceErrorLevelLib/H2ODebugTraceErrorLeveLib.inf
  ImageRelocationLib|InsydeModulePkg/Library/ImageRelocationLib/ImageRelocationLib.inf
  OemGraphicsLib|InsydeModulePkg/Library/OemGraphicsLib/OemGraphicsLib.inf

  IrsiRegistrationLib|InsydeModulePkg/Library/Irsi/IrsiRegistrationLib/IrsiRegistrationLib.inf
  MultiConfigBaseLib|InsydeModulePkg/Library/MultiConfigBaseLib/MultiConfigBaseLib.inf
  VariableSupportLib|InsydeModulePkg/Library/VariableSupportLib/VariableSupportLib.inf

  DxeOemSvcKernelLib|InsydeOemServicesPkg/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  DxeOemSvcKernelLibDefault|InsydeOemServicesPkg/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLibDefault.inf
  DxeChipsetSvcLib|InsydeModulePkg/Library/DxeChipsetSvcLib/DxeChipsetSvcLib.inf

  SmmConfidentialMemLib|InsydeModulePkg/Library/SmmConfidentialMemLib/SmmConfidentialMemLib.inf
  SmmServicesTableLib|InsydeModulePkg/Library/SmmServicesTableLibNull/SmmServicesTableLibNull.inf

  SeamlessRecoveryLib|InsydeModulePkg/Library/SeamlessRecoveryLib/SeamlessRecoveryLib.inf
  H2OIhisiLib|InsydeModulePkg/Library/H2OIhisiLib/H2OIhisiLib.inf
  SpiAccessLib|InsydeModulePkg/Library/FlashDeviceSupport/SpiAccessLibNull/SpiAccessLibNull.inf
  FdSupportLib|InsydeModulePkg/Library/FlashDeviceSupport/FdSupportLib/FdSupportLib.inf
  FlashWriteEnableLib|InsydeModulePkg/Library/FlashDeviceSupport/FlashWriteEnableLibNull/FlashWriteEnableLibNull.inf

  IntrinsicLib|InsydeModulePkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|InsydeNetworkPkg/Library/OpensslLib/OpensslLib.inf
  InternalBaseCryptLib|InsydeModulePkg/Library/BaseCryptLib/InternalRuntimeCryptLib.inf
  #
  # QEMU customized libraries.
  #
  PlatformBdsLib|ArmVirtPkg/Override/EDK2/ArmVirtPkg/Library/PlatformBootManagerLib/PlatformBdsLib.inf
  FlashDevicesLib|ArmVirtPkg/Override/Insyde/InsydeModulePkg/Library/FlashDevicesLibNull/FlashDevicesLibNull.inf
  FlashRegionLib|InsydeModulePkg/Library/FlashRegionLib/DxeFlashRegionLib/DxeFlashRegionLib.inf {
    <SOURCE_OVERRIDE_PATH>
      ArmVirtPkg/Override/Insyde/InsydeModulePkg/Library/FlashRegionLib/DxeFlashRegionLib
  }
  VariableLib|InsydeModulePkg/Library/DxeVariableLib/DxeVariableLib.inf {
    <SOURCE_OVERRIDE_PATH>
      ArmVirtPkg/Override/Insyde/InsydeModulePkg/Library/DxeVariableLib
  }
  BaseCryptLib|InsydeModulePkg/Library/BaseCryptLib/BaseCryptLib.inf

[LibraryClasses.common.PEI_CORE]
  MultiConfigBaseLib|InsydeModulePkg/Library/PeiMultiConfigLib/PeiMultiConfigLib.inf

[LibraryClasses.common.PEIM]
  H2OCpLib|InsydeModulePkg/Library/PeiH2OCpLib/PeiH2OCpLib.inf
  H2OLib|InsydeModulePkg/Library/PeiH2OLib/PeiH2OLib.inf
  MultiConfigBaseLib|InsydeModulePkg/Library/PeiMultiConfigLib/PeiMultiConfigLib.inf

  InternalBaseCryptLib|InsydeModulePkg/Library/BaseCryptLib/InternalPeiCryptLib.inf
  FlashRegionLib|InsydeModulePkg/Library/FlashRegionLib/PeiFlashRegionLib/PeiFlashRegionLib.inf
  H2OFvHashLib|InsydeModulePkg/Library/PeiH2OFvHashLib/PeiH2OFvHashLib.inf
  SeamlessRecoveryLib|InsydeModulePkg/Library/PeiSeamlessRecoveryLib/SeamlessRecoveryLib.inf
  VariableLib|InsydeModulePkg/Library/PeiVariableLib/PeiVariableLib.inf

[LibraryClasses.common.COMBINED_SMM_DXE]
  SmmOemSvcKernelLib|InsydeOemServicesPkg/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf
  SmmOemSvcKernelLibDefault|InsydeOemServicesPkg/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLibDefault.inf

[PcdsFeatureFlag.common]
  gInsydeTokenSpaceGuid.PcdSysPasswordInCmos|FALSE
  gInsydeTokenSpaceGuid.PcdH2OHddPasswordSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OBuildAutoAdjust|TRUE
  gInsydeTokenSpaceGuid.PcdH2OSecureBootSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OSetupChangeDisplaySupported|TRUE
  gInsydeTokenSpaceGuid.PcdMemoryMapConsistencyCheck|FALSE

[PcdsFixedAtBuild.common]
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultPlatformLangCodes|$(RFC_LANGUAGES)
  gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|0
  gEfiMdePkgTokenSpaceGuid.PcdMaximumUnicodeStringLength|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdImageProtectionPolicy|0x00000000
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{GUID("7C04A583-9E3E-4F1C-AD65-E05268D0B4D1")}

  # @PcdTypeStruct OEM_LOGO_RESOLUTION_DEFINITION {UINT32 LogoResolutionX; UINT32 LogoResolutionY; UINT32 ScuResolutionX; UINT32 ScuResolutionY;}
  # @PcdValueType OEM_LOGO_RESOLUTION_DEFINITION
  gInsydeTokenSpaceGuid.PcdDefaultLogoResolution|{0x20, 0x03, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00}

  gInsydeTokenSpaceGuid.PcdH2OBdsBootSuccessApp|{"VenHw(E0BA9B98-DD2D-4434-BB94-599CC9E4305D)"}
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
  gInsydeTokenSpaceGuid.PcdUefiOsFastBootList|L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"
  gInsydeTokenSpaceGuid.PcdFlashNvStorageBvdtSize|0x1000

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|40
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|128
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1024
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1024
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|128
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|40

[PcdsDynamicExDefault]
  #
  # Don't delete this dummy hot key desc PCD. Keep this to make sure BdsDxeStringsToIds[] will be generated in AutoGen.c for BdsDxe driver.
  #
  gH2OBdsHotKeyDescGuid.PcdH2OHotKeyDummyDesc|{STRING_TOKEN(STR_EMPTY_STRING)}

  gH2OBdsHotKeyGuid.PcdH2OHotKeyFrontPage|{GUID("00000000-0000-0000-0000-000000000000"), UINT32(0), "esc", "", "VenHw(E0BA9B98-DD2D-4434-BB94-599CC9E4305D)"}
  gH2OBdsHotKeyDescGuid.PcdH2OHotKeyFrontPageDesc|{STRING_TOKEN(STR_OEM_BADGING_STR_ESC), STRING_TOKEN(STR_OEM_BADGING_STR_ESC_SELECT),"x=0em;y=0em;xorigin=left;yorigin=bottom;"}
  gH2OBdsHotKeyDescQuietGuid.PcdH2OHotKeyFrontPageDescQuiet|{STRING_TOKEN(STR_OEM_BADGING_STR_ESC), STRING_TOKEN(STR_OEM_BADGING_STR_ESC_SELECT),"x=0em;y=0em;xorigin=center;yorigin=top(80%);"}

  gH2OBdsDefaultBootListGenericOsTokenSpaceGuid.MicrosoftOS|L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi\tWindows Boot Manager"
  gH2OBdsDefaultBootListGenericOsTokenSpaceGuid.RedHatOS|L"\\EFI\\redhat\\grub.efi\tRed Hat Linux"

[Components.common]
  #
  # DXE
  #
  InsydeModulePkg/Tools/GenBvdt/GenBvdt.inf
  #
  # HII
  #
  InsydeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  InsydeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  InsydeModulePkg/Universal/Console/JpegDecoderDxe/JpegDecoderDxe.inf
  InsydeModulePkg/Universal/Console/GifDecoderDxe/GifDecoderDxe.inf
  InsydeModulePkg/Universal/Console/BmpDecoderDxe/BmpDecoderDxe.inf
  InsydeModulePkg/Universal/Console/PngDecoderDxe/PngDecoderDxe.inf
  InsydeModulePkg/Universal/UserInterface/EnglishDxe/EnglishDxe.inf
  InsydeModulePkg/Universal/UserInterface/HiiDatabaseDxe/HiiDatabaseDxe.inf
  InsydeModulePkg/Universal/UserInterface/H2OKeyDescDxe/H2OKeyDescDxe.inf
  #
  # H2O Form Browser / Display Engine
  #
  InsydeModulePkg/Universal/UserInterface/SetupMouseDxe/SetupMouseDxe.inf
  InsydeModulePkg/Universal/UserInterface/Fonts/TrueTypeDecoderDxe/TrueTypeDecoderDxe.inf
  InsydeSetupPkg/Drivers/DisplayTypeDxe/DisplayTypeDxe.inf
  InsydeSetupPkg/Drivers/HiiLayoutPkgDxe/HiiLayoutPkgDxe.inf
  InsydeSetupPkg/Drivers/H2OFormBrowserDxe/H2OFormBrowserDxe.inf
  InsydeSetupPkg/Drivers/H2OFormDialogDxe/H2OFormDialogDxe.inf
!if gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported
  InsydeSetupPkg/Drivers/H2ODisplayEngineLocalTextDxe/H2ODisplayEngineLocalTextDxe.inf
!endif
!if gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported
  InsydeModulePkg/Universal/UserInterface/MicrowindowsDxe/MicrowindowsDxe.inf
  InsydeSetupPkg/Drivers/H2ODisplayEngineLocalMetroDxe/H2ODisplayEngineLocalMetroDxe.inf
!endif
!if gInsydeTokenSpaceGuid.PcdH2OSetupChangeDisplaySupported
  InsydeSetupPkg/Drivers/H2OSetupChangeDxe/H2OSetupChangeDxe.inf
!endif
  #
  # VFRs
  #
  InsydeModulePkg/Universal/UserInterface/BootManagerDxe/BootManagerDxe.inf
  InsydeModulePkg/Universal/UserInterface/BootMaintDxe/BootMaintDxe.inf
  InsydeModulePkg/Universal/UserInterface/DeviceManagerDxe/DeviceManagerDxe.inf {
    <LibraryClasses>
      NULL|InsydeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
  }
!if gInsydeTokenSpaceGuid.PcdH2OSecureBootSupported
  InsydeModulePkg/Universal/UserInterface/SecureBootMgrDxe/SecureBootMgrDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      NtEmulatorPkg/Override/Insyde/InsydeModulePkg/Universal/UserInterface/SecureBootMgrDxe
  }
!endif
  #
  # Setup Utility
  #
  InsydeModulePkg/Universal/Security/SysPasswordDxe/SysPasswordDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      ArmVirtPkg/Override/Insyde/InsydeModulePkg/Universal/Security/SysPasswordDxe
  }
!if gInsydeTokenSpaceGuid.PcdH2OHddPasswordSupported
  InsydeModulePkg/Universal/Security/HddPassword/HddPassword.inf
!endif
  InsydeModulePkg/Universal/UserInterface/SetupUtilityApp/SetupUtilityApp.inf
  #
  # BDS
  #
  InsydeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      ArmVirtPkg/Override/Insyde/InsydeModulePkg/Universal/BdsDxe
  }
  InsydeModulePkg/Universal/CommonPolicy/BootOptionPolicyDxe/BootOptionPolicyDxe.inf
  InsydeModulePkg/Universal/MemoryTest/GenericMemoryTestDxe/GenericMemoryTestDxe.inf
  #
  # QEMU customized drivers.
  #
  ArmVirtPkg/Override/EDK2/ArmVirtPkg/FrontPageDxe/FrontPageDxe.inf
  ArmVirtPkg/Override/EDK2/ArmVirtPkg/ChipsetSvcDxe/ChipsetSvcDxe.inf
  ArmVirtPkg/Override/EDK2/ArmVirtPkg/UefiSetupUtilityDxe/SetupUtilityDxe.inf

