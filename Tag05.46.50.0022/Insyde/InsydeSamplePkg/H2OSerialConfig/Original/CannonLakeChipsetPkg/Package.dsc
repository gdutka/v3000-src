## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]
  DEFINE    CHIPSET_PKG = CannonLakeChipsetPkg
  DEFINE    CLIENT_COMMON_PACKAGE = ClientCommonPkg
!include $(CHIPSET_PKG)/Package.env
!include $(PROJECT_PKG)/Project.env

[Packages]

[Libraries]

[LibraryClasses]
  SpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/SpiAccessLib/SpiAccessLib.inf
  FlashWriteEnableLib|InsydeModulePkg/Library/FlashDeviceSupport/FlashWriteEnableLibNull/FlashWriteEnableLibNull.inf
  SpiAccessInitLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/DxeSmmSpiAccessInitLib/DxeSmmSpiAccessInitLib.inf
  SmmLib|$(CHIPSET_PKG)/Library/PchSmmLib/PchSmmLib.inf
  FwUpdateLib|$(CHIPSET_PKG)/Library/FWUpdateLib/FWUpdateLib.inf
  PlatformBdsLib|$(CHIPSET_PKG)/Library/PlatformBdsLib/PlatformBdsLib.inf
  ResetSystemLib|$(CHIPSET_PKG)/Library/ResetSystemLib/ResetSystemLib.inf
  SmbusLib|$(CHIPSET_PKG)/Library/SmbusLib/SmbusLib.inf
  SmmPchLib|$(CHIPSET_PKG)/Library/SmmPchLib/SmmPchLib.inf
  PlatformHookLib|$(CHIPSET_PKG)/Board/Library/BasePlatformHookLib/BasePlatformHookLib.inf
  BaseOemSvcChipsetLib|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf
  BaseOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLibDefault.inf
  TbtLib|$(CHIPSET_PKG)/Library/TbtLib/TbtLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  BootGuardPlatformLib|$(CHIPSET_PKG)/Library/BootGuardPlatformLib/BootGuardPlatformLib.inf
  FitPlatformLib|$(CHIPSET_PKG)/Library/FitPlatformLib/FitPlatformLib.inf
  BootGuardTpmEventLogLib|$(CHIPSET_PKG)/Library/BootGuardTpmEventLogLib/BootGuardTpmEventLogLib.inf
  BaseInsydeChipsetLib|$(CHIPSET_PKG)/Library/BaseInsydeChipsetLib/BaseInsydeChipsetLib.inf
  PeiDxeSmmThunkLib|$(CHIPSET_PKG)/Library/PeiDxeSmmThunkLib/PeiDxeSmmThunkLib.inf

  BasePciLibPciExpress|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf

#
# Fix system will trigger recovery function with non-signed BIOS image when BIOS Guard is enabled.
#
  GenericBdsLib|InsydeModulePkg/Library/GenericBdsLib/GenericBdsLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/InsydeModulePkg/Library/GenericBdsLib
  }

!if gSiPkgTokenSpaceGuid.PcdTraceHubEnable
  BaseTraceHubDebugLib |$(CHIPSET_PKG)/Library/BaseTraceHubDebugLib/BaseTraceHubDebugLib.inf
  BasePlatformPostCodeMapLib|$(CHIPSET_PKG)/Library/BasePlatformPostCodeMapLib/BasePlatformPostCodeMapLib.inf
  BaseDebugPrintErrorLevelLib|$(CHIPSET_PKG)/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
!endif
  SerialPortLib|$(CHIPSET_PKG)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
  I2cAccessLib|$(CHIPSET_PKG)/Library/PeiI2cAccessLib/PeiI2cAccessLib.inf
  GpioExpanderLib|$(CHIPSET_PKG)/Library/BaseGpioExpanderLib/BaseGpioExpanderLib.inf

!if $(TARGET) == DEBUG
  GpioCheckConflictLib|$(CHIPSET_PKG)/Library/BaseGpioCheckConflictLib/BaseGpioCheckConflictLib.inf
!else
  GpioCheckConflictLib|$(CHIPSET_PKG)/Library/BaseGpioCheckConflictLibNull/BaseGpioCheckConflictLibNull.inf
!endif

  TbtCommonLib|$(CHIPSET_PKG)/Features/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.inf
  DxeTbtPolicyLib|$(CHIPSET_PKG)/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.inf
  DxeTbtSecurityLib|$(CHIPSET_PKG)/Features/Tbt/Library/DxeTbtSecurityLib/DxeTbtSecurityLib.inf

  UefiBootManagerLib|$(CHIPSET_PKG)/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  ChipsetCapsuleLib|$(CHIPSET_PKG)/CapsuleIFWU/CapsuleLib/ChipsetCapsuleLib.inf
  ChipsetSignatureLib|$(CHIPSET_PKG)/CapsuleIFWU/CapsuleLib/ChipsetSignatureLib.inf
  InsydeChipsetGpioLib|$(CHIPSET_PKG)/Library/BaseInsydeChipsetGpioLib/BaseInsydeChipsetGpioLib.inf
###
### Override
###
!if gChipsetPkgTokenSpaceGuid.PcdUseCrbEcFlag == TRUE
  EcMiscLib|$(CHIPSET_PKG)/Library/BaseEcMiscLib/BaseEcMiscLib.inf
  EcLib|$(CHIPSET_PKG)/Library/BaseEcLib/BaseEcLib.inf
  EcHwLib|$(CHIPSET_PKG)/Library/BaseEcHwLib/BaseEcHwLib.inf
!else
  EcMiscLib|$(CHIPSET_PKG)/Library/BaseEcMiscLibNull/BaseEcMiscLibNull.inf
  EcLib|$(CHIPSET_PKG)/Library/BaseEcLibNull/BaseEcLibNull.inf
!endif

  PeiWdtAppLib|$(CHIPSET_PKG)/Library/PeiWdtAppLib/PeiWdtAppLib.inf

!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == TRUE
  PlatformInitLib|$(CHIPSET_PKG)/Library/PlatformInitLib/PlatformInitLib.inf
  FspPolicyInitLib|$(CHIPSET_PKG)/FspWrapper/Library/PeiFspPolicyInitLib/PeiFspPolicyInitLibCnl.inf
  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf
  FspWrapperApiTestLib|IntelFsp2WrapperPkg/Library/PeiFspWrapperApiTestLib/PeiFspWrapperApiTestLib.inf
  FspWrapperPlatformLib|$(CHIPSET_PKG)/FspWrapper/Library/BaseFspWrapperPlatformLib/BaseFspWrapperPlatformLib.inf
  FspWrapperHobProcessLib|$(CHIPSET_PKG)/FspWrapper/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf
  CacheAsRamLib|IntelFsp2Pkg/Library/BaseCacheAsRamLibNull/BaseCacheAsRamLibNull.inf
!endif

  IoApicLib|PcAtChipsetPkg/Library/BaseIoApicLib/BaseIoApicLib.inf
  SmiHandlerProfileLib|MdeModulePkg/Library/SmmSmiHandlerProfileLib/SmmSmiHandlerProfileLib.inf
  BoardIdsLib|$(CHIPSET_PKG)/Library/PeiDxeBoardIdsLib/PeiDxeBoardIdsLib.inf

!if gSiPkgTokenSpaceGuid.PcdTraceHubEnable == TRUE
  TraceHubHookLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/PeiDxeSmmTraceHubHookLib/PeiDxeSmmTraceHubHookLib.inf
!else
  TraceHubHookLib|$(CHIPSET_PKG)/Library/TraceHubHookLibNull/TraceHubHookLibNull.inf
!endif

  FlashDevicesLib|InsydeFlashDevicePkg/Library/FlashDevicesLib/FlashDevicesLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/InsydeFlashDevicePkg/Library/FlashDevicesLib
  }

  DimmInfoLib|$(CHIPSET_PKG)/UefiSetupUtilityDxe/Advance/DimmInfoLib.inf
  PlatformFvAddressLib|$(CHIPSET_PKG)/Library/PlatformFvAddressLib/PlatformFvAddressLib.inf

[LibraryClasses.X64]
  DxeFirmwareVersionInfoLib|$(CHIPSET_PKG)/Library/DxeFirmwareVersionInfoLib/DxeFirmwareVersionInfoLib.inf

  #BluetoothLib|BpCommonPkg/Bluetooth/Library/BluetoothLib/BluetoothLib.inf
  #HidLib|BpCommonPkg/Bluetooth/Library/HidLib/HidLib.inf

[LibraryClasses.common.SEC]
!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == FALSE
  PlatformSecLib|$(CHIPSET_PKG)/Library/PlatformSecLib/PlatformSecLib.inf
!else
  PlatformSecLib|$(CHIPSET_PKG)/FspWrapper/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf
!endif
  SecOemSvcChipsetLib|$(CHIPSET_PKG)/Library/SecOemSvcChipsetLib/SecOemSvcChipsetLib.inf
  SecOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/SecOemSvcChipsetLib/SecOemSvcChipsetLibDefault.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf{
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/MdeModulePkg/Library/PeiPerformanceLib
      MdeModulePkg/Override/Library/PeiPerformanceLib
  }
  PeiPostcodeToScratchpadRegLib|ClientSiliconPkg/Library/PeiPostcodeToScratchpadRegLibNull/PeiPostcodeToScratchpadRegLibNull.inf

[LibraryClasses.IA32]
  #
  # Following RC code to override DxeMpInitLib for upgrade UefiCpuPkg
  # SIPO-CNL-0001 SIPO-CNL-0004
  #!disable MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  MpInitLib|$(PLATFORM_SI_PACKAGE)/Override/UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf

[LibraryClasses.common.PEI_CORE]
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf{
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/MdeModulePkg/Library/PeiPerformanceLib
      MdeModulePkg/Override/Library/PeiPerformanceLib
  }

[LibraryClasses.common.PEIM]
  TimerLib|$(CHIPSET_PKG)/Override/PcAtChipsetPkg/Library/TscAcpiTimerLib/PeiTscTimerLib.inf
  KernelConfigLib|$(CHIPSET_PKG)/Library/PeiKernelConfigLib/PeiKernelConfigLib.inf
  PeiOemSvcChipsetLib|$(CHIPSET_PKG)/Library/PeiOemSvcChipsetLib/PeiOemSvcChipsetLib.inf
  PeiOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/PeiOemSvcChipsetLib/PeiOemSvcChipsetLibDefault.inf
  PeiInsydeChipsetLib|$(CHIPSET_PKG)/Library/PeiInsydeChipsetLib/PeiInsydeChipsetLib.inf
!if gChipsetPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  PeiTbtPolicyLib|$(CHIPSET_PKG)/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLib.inf
  PeiTbtTaskDispatchLib|$(CHIPSET_PKG)/Features/Tbt/Library/PeiTbtTaskDispatchLib/PeiTbtTaskDispatchLib.inf
  PeiTbtCommonInitLib|$(CHIPSET_PKG)/Features/Tbt/Library/Private/PeiTbtCommonInitLib/PeiTbtCommonInitLib.inf
  PeiDTbtInitLib|$(CHIPSET_PKG)/Features/Tbt/Library/Private/PeiDTbtInitLib/PeiDTbtInitLib.inf
!else
  PeiTbtTaskDispatchLib|$(CHIPSET_PKG)/Features/Tbt/Library/PeiTbtTaskDispatchLibNull/PeiTbtTaskDispatchLibNull.inf
  PeiDTbtInitLib|$(CHIPSET_PKG)/Features/Tbt/Library/Private/PeiDTbtInitLibNull/PeiDTbtInitLibNull.inf
!endif
  SpiAccessInitLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/PeiSpiAccessInitLib/PeiSpiAccessInitLib.inf
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf{
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/MdeModulePkg/Library/PeiPerformanceLib
      MdeModulePkg/Override/Library/PeiPerformanceLib
  }
  SCUVarInfoLib|$(CHIPSET_PKG)/Library/PeiSCUVarInfoLib/PeiSCUVarInfoLib.inf
  PeiPlatformHookLib|$(CHIPSET_PKG)/Board/Library/PeiPlatformHookLib/PeiPlatformHooklib.inf
  PeiOverClockLib|$(CHIPSET_PKG)/Features/OverClocking/OverClockInit/PeiOverClockLib.inf
  PeiPolicyInitLib|$(CHIPSET_PKG)/Library/PeiPolicyInitLib/PeiPolicyInitLib.inf
  PeiPolicyUpdateLib|$(CHIPSET_PKG)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf
  PeiPolicyDebugLib|$(CHIPSET_PKG)/Library/PeiPolicyDebugLib/PeiPolicyDebugLib.inf
  BoardConfigLib|$(CHIPSET_PKG)/Board/Library/PeiBoardConfigLib/PeiBoardConfigLib.inf
###
### Override
###
  MultiPlatSupportLib|$(CHIPSET_PKG)/Library/PeiMultiPlatSupportLib/PeiMultiPlatSupportLibOptSize.inf
!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == TRUE
  FspWrapperPlatformResetLib|$(CHIPSET_PKG)/FspWrapper/Library/PeiFspWrapperPlatformResetLib/PeiFspWrapperPlatformResetLib.inf
!endif
  PeiPolicyBoardConfigLib|$(CHIPSET_PKG)/Library/PeiPolicyBoardConfigLib/PeiPolicyBoardConfigLib.inf
  ProcessorTraceMemoryAllocationLib|$(PLATFORM_SI_PACKAGE)/SampleCode/Library/PeiProcessorTraceMemoryAllocationLib/PeiProcessorTraceMemoryAllocationLib.inf
  GetVolDataLib|$(CHIPSET_PKG)/GetVariableHook/Library/GetVolDataLib/PeiGetVolDataLib.inf

  #
  # Following RC code to override DxeMpInitLib for upgrade UefiCpuPkg
  # SIPO-CNL-0001 SIPO-CNL-0004
  !disable MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  MpInitLib|$(PLATFORM_SI_PACKAGE)/Override/UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf

[LibraryClasses.common.DXE_CORE]
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  TimerLib|$(CHIPSET_PKG)/Override/PcAtChipsetPkg/Library/TscAcpiTimerLib/DxeTscTimerLib.inf
  GetVolDataLib|$(CHIPSET_PKG)/GetVariableHook/Library/GetVolDataLib/DxeGetVolDataLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  DxeInsydeChipsetLib|$(CHIPSET_PKG)/Library/DxeInsydeChipsetLib/DxeInsydeChipsetLib.inf

[LibraryClasses.common.DXE_DRIVER]
  TimerLib|$(CHIPSET_PKG)/Override/PcAtChipsetPkg/Library/TscAcpiTimerLib/DxeTscTimerLib.inf
  DxeInsydeChipsetLib|$(CHIPSET_PKG)/Library/DxeInsydeChipsetLib/DxeInsydeChipsetLib.inf
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf
  DxeOverClockLib|$(CHIPSET_PKG)/Features/OverClocking/OverClockInit/DxeOverClockLib.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  AtGraphicsLib|$(CHIPSET_PKG)/Library/AtGraphicsLite/AtGraphics.inf
  SCUVarInfoLib|$(CHIPSET_PKG)/Library/DxeSCUVarInfoLib/DxeSCUVarInfoLib.inf
  DxePolicyUpdateLib|$(CHIPSET_PKG)/Library/DxePolicyUpdateLib/DxePolicyUpdateLib.inf

!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == TRUE
  FspWrapperPlatformResetLib|$(CHIPSET_PKG)/FspWrapper/Library/DxeFspWrapperPlatformResetLib/DxeFspWrapperPlatformResetLib.inf
!endif
  DxePolicyBoardConfigLib|$(CHIPSET_PKG)/Library/DxePolicyBoardConfigLib/DxePolicyBoardConfigLib.inf
  DxeBoardConfigLib|$(CHIPSET_PKG)/Board/Library/DxeBoardConfigLib/DxeBoardConfigLib.inf

  #
  # Following RC code to override DxeMpInitLib for upgrade UefiCpuPkg
  # SIPO-CNL-0001 SIPO-CNL-0004
  !disable MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf
  MpInitLib|$(PLATFORM_SI_PACKAGE)/Override/UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmmOemSvcChipsetLib|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLib.inf
  SmmOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLibDefault.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  SmmStmConfTableLib|$(CHIPSET_PKG)/Library/SmmStmConfTableLibNull/SmmStmConfTableLibNull.inf
  SmmIoLib|$(CHIPSET_PKG)/Library/SmmIo/SmmIoLib.inf
  SmmHeciLib|$(CHIPSET_PKG)/Library/SmmHeciLib/SmmHeciLib.inf
###
### Override
###
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf


[LibraryClasses.common.COMBINED_SMM_DXE]
  SmmOemSvcChipsetLib|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLib.inf
  SmmOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLibDefault.inf
  TbtLib|$(CHIPSET_PKG)/Library/TbtLib/TbtLib.inf
  DxeAsfLib|$(CHIPSET_PKG)/Library/DxeAsfLibNull/DxeAsfLibNull.inf

[LibraryClasses.common.SMM_CORE]
  TimerLib|$(CHIPSET_PKG)/Override/PcAtChipsetPkg/Library/TscAcpiTimerLib/DxeTscTimerLib.inf

[LibraryClasses.common]
  PchTraceHubInitLib|ClientSiliconPkg/Library/BasePchTraceHubInitLib/BasePchTraceHubInitLib.inf
  DxeSmbiosFirmwareVersionInfoLib|$(CLIENT_COMMON_PACKAGE)/Library/DxeSmbiosFirmwareVersionInfoLib/DxeSmbiosFirmwareVersionInfoLib.inf # RPPO-KBL-0059: RoyalParkOverrideContent

[LibraryClasses.common.UEFI_APPLICATION]

[PcdsFeatureFlag]
  #
  # The PCD for Microsoft HID over I2C devices, the bits definition below
  #
  # Bit  0 ~  6 : Slave address
  # Bit  7      : Interrupt GPIO pin active level, 0 = low active, 1 = high active
  # Bit  8 ~ 15 : Interrupt GPIO pin number
  # Bit 16 ~ 31 : HID descriptor register number
  # Bit 32 ~ 47 : Device type,
  #               0x0000 -> (AutoDetect)
  #               0x0d00 -> Touch panel
  #               0x0102 -> Mouse
  #               0x0106 -> Keyboard
  # Bit 48 ~ 51 : Host controller number
  #               0x00      -> (AutoDetect)
  #               0x01~0x0f -> I2C host controller 0~14 (One based)
  # Bit 52 ~ 55 : Device Speed Override
  #               0x01      -> V_SPEED_STANDARD
  #               0x02      -> V_SPEED_FAST
  #               0x03      -> V_SPEED_HIGH
  #
  #
  # Required checkpoints
  #
  gInsydeTokenSpaceGuid.PcdH2OBaseCpPostCodeSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OPeiCpInitChipsetPolicySupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OPeiCpSetBootModeBeforeSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OBdsCpRecoveryCompleteSupported|TRUE

  #
# Always set TRUE.
# Set FALSE:Fwblockservice.c will install gEfiFirmwareVolumeBlockProtocolGuid, it will change driver dispath sequence.
#
  gInsydeTokenSpaceGuid.PcdFvbAccessThroughSmi|TRUE
  gChipsetPkgTokenSpaceGuid.PcdUseCrbEcFlag|$(USE_INTEL_CRB_H8_EC)

!if $(APAC_SUPPORT) == YES
  gChipsetPkgTokenSpaceGuid.PcdEnableEconFlag|FALSE
!else
  gChipsetPkgTokenSpaceGuid.PcdEnableEconFlag|$(ENABLE_ASL_ECON)
!endif

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdStatusCodeUseDataHub|true
  gInsydeTokenSpaceGuid.PcdEndOfDxeEventSupported|FALSE
  gChipsetPkgTokenSpaceGuid.PcdHstiSupported|$(HSTI_SUPPORT)
  gH2OFlashDeviceEnableGuid.PcdCommonvidCommondidSpiEnable|$(SFDP_SUPPORT)

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE

  gChipsetPkgTokenSpaceGuid.PcdMeCapsuleUpdateSupported|TRUE
  gChipsetPkgTokenSpaceGuid.PcdIshCapsuleUpdateSupported|TRUE
  gChipsetPkgTokenSpaceGuid.PcdPdtCapsuleUpdateSupported|TRUE
  gChipsetPkgTokenSpaceGuid.PcdEcCapsuleUpdateSupported|TRUE
#
#  Reduce POST Time when issuing device reset, soluiton IB05300634 from KBL.
#
  gEfiMdeModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPs2MouseExtendedVerification|FALSE
  gInsydeTokenSpaceGuid.PcdH2OBdsCpUpdateAcpiDescHdrSupported|TRUE
  #
  # Build Switch for bin files.
  #
  gPlatformModuleTokenSpaceGuid.PcdRstOneClickEnable|TRUE               #RstOneClickEnable.efi
  gPlatformModuleTokenSpaceGuid.PcdIntelGopEnable|TRUE                  #LegacyVideoRom.bin and IntelGopDriver.efi
  gSiPkgTokenSpaceGuid.PcdMinTreeEnable|FALSE                           # AdvancedFeaturesContent

[PcdsFixedAtBuild]
  gSiPkgTokenSpaceGuid.PcdSmbusBaseAddress|0xEFA0
  gInsydeTokenSpaceGuid.PcdSmBusBaseAddress|gSiPkgTokenSpaceGuid.PcdSmbusBaseAddress
  ## Stack size in the temporary RAM.
  #   0 means half of PcdTemporaryRamSizeWhenRecovery.
  #
  #  WARNING:
  #  If you change the value of PcdPeiTemporaryRamStackSizeWhenRecovery,
  #  you also need to adjust the value of gChipsetPkgTokenSpaceGuid.PcdNemDataStackSize.
  #  These PCDs values must be same.
  gInsydeTokenSpaceGuid.PcdPeiTemporaryRamStackSizeWhenRecovery|0x0
!if gInsydeTokenSpaceGuid.PcdH2ODdtSupported
  #
  # Insyde DDT needs the PcdCpuSmmCodeAccessCheckEnable to be set as FALSE.
  #
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable|FALSE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x40000

#
# Following intel to set SmmSatackSize to avoid Smm Stack overflow.
#
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x20000

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|1000

!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == FALSE
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x25000
!else
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148

!if gSiPkgTokenSpaceGuid.PcdCflCpuEnable == TRUE
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x24000
!if $(FIRMWARE_PERFORMANCE) == YES
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0xC700  # ((PcdTemporaryRamSize - PcdFspTemporaryRamSize)/2) - PcdFspReservedBufferSize - 0x1880
!endif
!else
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x24000
!if $(FIRMWARE_PERFORMANCE) == YES
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0xCB00  # ((PcdTemporaryRamSize - PcdFspTemporaryRamSize)/2) - PcdFspReservedBufferSize - 0x1400
!endif
!endif

!endif

  gSiPkgTokenSpaceGuid.PcdCpuSmmProcTraceEnable|FALSE

  gInsydeTokenSpaceGuid.PcdTemporaryRamSizeWhenRecovery|0x40000
  gInsydeTokenSpaceGuid.PcdUmaDeviceNumber|0x02                 # 0xFF no Uma port.
  gInsydeTokenSpaceGuid.PcdPegDeviceNumber|0x01                 # 0xFF no Peg port.

  gEfiMdeModulePkgTokenSpaceGuid.PcdSmiHandlerProfilePropertyMask|1

!if $(PCI_EXPRESS_SIZE) == 256
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSize|256
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000
!else
  #
  # Size = 128 or 64 , PciExpressBaseAddress both set as 0xF0000000
  #
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xF0000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSize|$(PCI_EXPRESS_SIZE)

!if $(PCI_EXPRESS_SIZE) == 128
  gChipsetPkgTokenSpaceGuid.PcdPciExpressMaxBusNumber|0x7F
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x8000000
!else
  gChipsetPkgTokenSpaceGuid.PcdPciExpressMaxBusNumber|0x3F
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x4000000
!endif
!endif


!if gSiPkgTokenSpaceGuid.PcdCflCpuEnable == FALSE
  gPlatformModuleTokenSpaceGuid.PcdHeciF4BaseAddress|0xFEDA4000
  gPlatformModuleTokenSpaceGuid.PcdDmiBaseAddress|0xFEDA0000
  gPlatformModuleTokenSpaceGuid.PcdEpBaseAddress|0xFEDA1000
  gSiPkgTokenSpaceGuid.PcdRegBarBaseAddress|0xFC000000

  gSiPkgTokenSpaceGuid.PcdSiliconInitVersionMinor|0x01
  ##
  ## As per CNL settings
  ##
  gSiPkgTokenSpaceGuid.PcdCpuTraceHubMtbBarSize|0x100000
  gSiPkgTokenSpaceGuid.PcdCpuTraceHubFwBarSize|0x200000
!else
  ##
  ## As per CFL settings
  ##
  gSiPkgTokenSpaceGuid.PcdCpuTraceHubMtbBarSize|0x10000
  gSiPkgTokenSpaceGuid.PcdCpuTraceHubFwBarSize|0x20000
  gSiPkgTokenSpaceGuid.PcdSiliconInitVersionMinor|0x00

!endif # gSiPkgTokenSpaceGuid.PcdCflCpuEnable == FALSE
  gPlatformModuleTokenSpaceGuid.PcdVtioEnable|TRUE
  #
  # Enable PcdShadowPeimOnS3Boot to load PEIM into memory during S3 resume
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE

  #
  # CMOS Debug Code Table
  #
  gInsydeTokenSpaceGuid.PcdPeiChipsetDebugCodeTable|{ \
    GUID("145971E9-AD52-4094-A8C8-BE5B3FECC82D"), 0x40, 0x02, \ #CpuPeim
    GUID("C866BD71-7C79-4BF1-A93B-066B830D8F9A"), 0x40, 0x06, \ #Init MP for S3
    GUID("ABB74F50-FD2D-4072-A321-CAFC72977EFA"), 0x40, 0x07, \ #PeiSmmRelocate
    GUID("A89EC8E0-0BA1-40AA-A03E-ABDDA5295CDE"), 0x43, 0x02, \ #PciExpress
    GUID("D4EE25EA-0B48-43AE-A016-4D6E8B6C43B3"), 0x43, 0x04, \ #CantigaMemoryInit
    GUID("921B35BF-0255-4722-BF5A-5B8B69093593"), 0x46, 0x02, \ #IchInit
    GUID("64f3DF77-F312-42ED-81CC-1B1F57E18AD6"), 0x46, 0x03, \ #IchSmbusArpDisabled
    GUID("C5442B7B-58CF-4375-B152-4383132607FC"), 0x46, 0x05, \ #For recover PEIM
    GUID("C3619722-EF37-4605-8C35-C7A37B51EDD2"), 0x49, 0x02, \ #ClockGen and ck505
    GUID("50EE664D-7703-42C3-9E69-8C89DE70D1D5"), 0x4C, 0x02, \ #SioInit
    GUID("8A78B107-0FDD-4CC8-B7BA-DC3E13CB8524"), 0x4C, 0x03, \ #PeiCpuIoPciCfg
    GUID("A85027FC-0E09-4FA9-A407-CAD206FB4F1D"), 0x4C, 0x04, \ #PlatformStage1
    GUID("1191BBF5-DCB9-44F4-827E-95359744C987"), 0x4C, 0x05, \ #PlatformStage2
    GUID("5479E09C-2E74-481B-89F8-B0172E388D1F"), 0x4F, 0x01, \ #Start Watch Dog PEIM driver
    GUID("4862AFF3-667C-5458-B274-A1C62DF8BA80"), 0x4F, 0x02, \ #Framework PEIM to HECI
    GUID("CA9D8617-D652-403B-B6C5-BA47570116AD"), 0x50, 0x01, \ #Txt PEIM
    GUID("AE265864-CF5D-41A8-913D-71C155E76442"), 0x51, 0x0D, \ #CpuIoPei
    GUID("E2FC5838-16A9-4ED7-96E0-9A75F5AF711D"), 0x51, 0x0F, \ #EmuPeiHelperPei
    GUID("EAA96391-9BE3-4488-8AF3-B3E6EFD157D5"), 0x51, 0x10, \ #EmuSecPei
    GUID("A56FAD72-A264-4370-85C5-00584654DCE2"), 0x51, 0x11, \ #InstallVerbTablePei
    GUID("6D3D1021-ECFC-42C2-B301-ECE9DB198287"), 0x51, 0x12, \ #LegacySpeakerPei
    GUID("1C8B7F78-1699-40E6-AF33-9B995D16B043"), 0x51, 0x15, \ #PiSmmCommunicationPei
    GUID("FDB3B9A7-1E82-4C77-9C6C-4305C851F253"), 0x51, 0x1A, \ #ProgClkGenPeim
    GUID("00000000-0000-0000-0000-000000000000"), 0x00, 0x00} #EndEntry


  gInsydeTokenSpaceGuid.PcdDxeChipsetDebugCodeTable|{ \
    GUID("62D171CB-78CD-4480-8678-C6A2A797A8DE"), 0x41, 0x01, \ #MpCpu
    GUID("BAE7599F-3C6B-43BC-BDF0-9CE07AA91AA6"), 0x41, 0x02, \ #CpuIo
    GUID("5552575A-7E00-4D61-A3A4-F7547351B49E"), 0x42, 0x02, \ #SmmBase
    GUID("9CC55D7D-FBFF-431C-BC14-334EAEA6052B"), 0x42, 0x04, \ #SmmCoreDispatcher
    GUID("7FED72EE-0170-4814-9878-A8FB1864DFAF"), 0x42, 0x06, \ #SmmRelocate
    GUID("8D3BE215-D6F6-4264-BEA6-28073FB13AEA"), 0x42, 0x07, \ #SmmThunk
    GUID("3151F203-546B-4683-AD72-D8B16BC7D75E"), 0x44, 0x02, \ #DxeMchInit
    GUID("8D6756B9-E55E-4D6A-A3A5-5E4D72DDF772"), 0x44, 0x03, \ #PciHostBridge
    GUID("EDA791A4-384B-4906-89D1-EE4DC972FE4A"), 0x44, 0x04, \ #SmbiosMemory
    GUID("CDC204A8-F5DB-44F6-BDC6-446EEE54316F"), 0x44, 0x05, \ #IgdOpRegion
    GUID("EFFC8F05-B526-4EB5-B36B-8CD889923C0C"), 0x44, 0x06, \ #LegacyRegion
    GUID("B09CB87C-67D8-412B-BB9D-9F4B214D720A"), 0x44, 0x07, \ #VTd
    GUID("90F1E37A-A26B-4E50-9A19-F9DD65F9F173"), 0x44, 0x08, \ #HDCP
    GUID("F50251DA-B608-4177-991A-7DF4278C9753"), 0x44, 0x09, \ #SmmAccess
    GUID("3AA01781-5E40-4524-B6CA-1ACB3B45578C"), 0x45, 0x02, \ #HDCPSMI
    GUID("21094ECB-9F20-4781-AE4B-50728B389A6E"), 0x47, 0x02, \ #DxeIchInit
    GUID("BB65942B-521F-4EC3-BAF9-A92540CF60D2"), 0x47, 0x04, \ #SataController
    GUID("E052D8A6-224A-4C32-8D37-2E0AE162364D"), 0x47, 0x05, \ #DxeIchSmbus
    GUID("A0BAD9F7-AB78-491B-B583-C52B7F84B9E0"), 0x47, 0x06, \ #SmmControl
    GUID("6F0198AA-1F1D-426D-AE3E-39AB633FCC28"), 0x47, 0x07, \ #Cf9Reset
    GUID("C1C418F9-591D-461C-82A2-B9CD96DFEA86"), 0x47, 0x08, \ #IntelIchLegacyInterrupt
    GUID("90CB75DB-71FC-489D-AACF-943477EC7212"), 0x47, 0x09, \ #SmartTimer
    GUID("C194C6EA-B68C-4981-B64B-9BD271474B20"), 0x47, 0x0A, \ #RuntimeDxeIchSpi
    GUID("FC1B7640-3466-4C06-B1CC-1C935394B5C2"), 0x47, 0x0B, \ #IchSerialGpio
    GUID("75521DD9-DBF3-4F4D-80B7-F5144FD74BF8"), 0x47, 0x0C, \ #AhciBus
    GUID("B0D6ED53-B844-43F5-BD2F-61095264E77E"), 0x48, 0x01, \ #IchSmmDispatcher
    GUID("2374EDDF-F203-4FC0-A20E-61BAD73089D6"), 0x48, 0x03, \ #IoTrap
    GUID("DE5B1E13-1427-453F-ACC4-CDEB0E15797E"), 0x48, 0x04, \ #IchS3Save
    GUID("07A9330A-F347-11D4-9A49-0090273FC14D"), 0x4A, 0x02, \ #LegacyMetronome
    GUID("378D7B65-8DA9-4773-B6E4-A47826A833E1"), 0x4A, 0x03, \ #PcRtc
    GUID("79CA4208-BBA1-4A9A-8456-E1E66A81484E"), 0x4A, 0x04, \ #Legacy8259
    GUID("27835690-BEBE-4929-BC60-94D77C6862F7"), 0x4A, 0x05, \ #IsaAcpiDriver (Sio1007)
    GUID("A6F691AC-31C8-4444-854C-E2C1A6950F92"), 0x4D, 0x01, \ #Bds
    GUID("1967DD9B-B72C-4328-8C80-D4ACFC83FDF8"), 0x4D, 0x02, \ #PciHotPlug
    GUID("FE3542FE-C1D3-4EF8-657C-8048606FF670"), 0x4D, 0x03, \ #SetupUtility
    GUID("94EDD12A-419B-447F-9434-9B3B70783903"), 0x4D, 0x04, \ #DxePlatform
    GUID("FDA14FA3-AFFC-469A-B7BB-34BCDD4AC096"), 0x4D, 0x05, \ #PlatformIde
    GUID("EF0C99B6-B1D3-4025-9405-BF6A560FE0E0"), 0x4D, 0x07, \ #MiscSubclass
    GUID("AFC04099-0D39-405D-BE46-846F08C51A31"), 0x4D, 0x08, \ #AcpiPlatform
    GUID("E3932A34-5729-4F24-9FB1-D7409B456A15"), 0x4D, 0x09, \ #OemBadgingSupport
    GUID("F84CFFF4-511E-41C8-B829-519F5152F444"), 0x4D, 0x0A, \ #LegacyBiosPlatform
    GUID("E2441B64-7EF4-41FE-B3A3-8CAA7F8D3017"), 0x4D, 0x0B, \ #PciPlatform
    GUID("99C20A37-042A-46E2-80F4-E4027FDBC86F"), 0x4E, 0x01, \ #SmmPlatform
    GUID("77A6009E-116E-464D-8EF8-B35201A022DD"), 0x4E, 0x05, \ #DigitalThermalSensor
    GUID("F7731B4C-58A2-4DF4-8980-5645D39ECE58"), 0x4E, 0x06, \ #ProcessorPowerManagement
    GUID("76AD6E27-6181-4769-9188-233CEA0A81F3"), 0x4F, 0x03, \ #Create a dataHub for AMT
    GUID("7945C163-58D4-4DA8-A57B-081130AF3D21"), 0x4F, 0x04, \ #Heci driver core
    GUID("D739F969-FB2D-4BC2-AFE7-081327D3FEDE"), 0x4F, 0x05, \ #AMT driver
    GUID("55E76644-78A5-4A82-A900-7126A5798892"), 0x4F, 0x06, \ #Heci driver
    GUID("32C1C9F8-D53F-41C8-94D0-F6739F231011"), 0x4F, 0x07, \ #AMT Bios Extensions
    GUID("C4F2D007-37FD-422D-B63D-7ED73886E6CA"), 0x4F, 0x08, \ #IDE_CONTROLLER_INIT protocol
    GUID("33C6406D-2F6B-41B5-8705-52BAFB633C09"), 0x4F, 0x09, \ #ASF messages
    GUID("A7FA30F2-2D70-4EE6-B1F2-44ECD5056011"), 0x4F, 0x0A, \ #AMT module on BDS
    GUID("FB142B99-DF57-46CB-BC69-0BF858A734F9"), 0x4F, 0x0B, \ #AMT PCI serial
    GUID("EB78CE7E-4107-4EF5-86CB-22E8D8AC4950"), 0x4F, 0x0C, \ #SmmAsfInit
    GUID("69EF78BC-3B71-4ECC-834F-3B74F9148430"), 0x4F, 0x0D, \ #SmmFlashWriteProtect
    GUID("FF917E22-A228-448D-BDAA-68EFCCDDA5D3"), 0x50, 0x02, \ #TXT dxe driver
    GUID("F2765DEC-6B41-11D5-8E71-00902707B35E"), 0x51, 0x01, \ #8254Timer
    GUID("4FB2CE1F-1A3A-42E3-BD0C-7B84F954189A"), 0x51, 0x03, \ #AcpiCallBacksSmm
    GUID("CB933912-DF8F-4305-B1F9-7B44FA11395C"), 0x51, 0x04, \ #AcpiPlatformDxe
    GUID("A8913EC1-C00C-4C3A-A245-077C0CA35738"), 0x51, 0x05, \ #AspmOverrideDxe
    GUID("C7D4F4E3-DAEA-40B0-8846-F4CAF3135CE8"), 0x51, 0x06, \ #BiosProtectDxe
    GUID("B7358BEB-6A52-4D50-98F9-7EDD70B4B320"), 0x51, 0x09, \ #CommonPciPlatformDxe
    GUID("A19B1FE7-C1BC-49F8-875F-54A5D542443F"), 0x51, 0x0A, \ #CpuIo2Dxe
    GUID("A47EE2D8-F60E-42FD-8E58-7BD65EE4C29B"), 0x51, 0x0B, \ #CpuIo2Smm
    GUID("26452F27-45DE-4A94-807A-0E6FDC1CB962"), 0x51, 0x0E, \ #EmuPeiGateDxe
    GUID("51739E2A-A022-4D73-ADB9-91F0C9BC7142"), 0x51, 0x13, \ #MpServicesOnFrameworkMpServicesThunk
    GUID("E21F35A8-42FF-4050-82D6-93F7CDFA7073"), 0x51, 0x16, \ #PiSmmCommunicationSmm
    GUID("A3FF0EF5-0C28-42F5-B544-8C7DE1E80014"), 0x51, 0x17, \ #PiSmmCpuDxeSmm
    GUID("96BDEA61-C364-4513-B6B3-037E9AD54CE4"), 0x51, 0x1B, \ #SetSsidSvidDxe
    GUID("EF0C99B6-B1D3-4025-9405-BF6A560FE0E0"), 0x51, 0x1C, \ #SmbiosMiscDxe
    GUID("00000000-0000-0000-0000-000000000000"), 0x00, 0x00} #EndEntry

  #
  # LegacyBios Configuration
  #
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdLowPmmMemorySize|0x20000

  #
  # Intel RAID oprom need 130MB PMM memory for Intel Smart Response Technology(ISRT).
  # Setting the size to 134MB (130MB for ISRT + 4MB for kerenl default).
  #
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdHighPmmMemorySize|0x8600000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|1000
#
# When PcdRestoreCmosfromVariableFlag|TRUE. Save cmos in variable will be enabled.
# This function will follow PcdSaveCmosFieldCompareList to compare Cmos value and CmosInVariable.
# If they are different, current Cmos value will be copied to CmosInVariable.
#
  gChipsetPkgTokenSpaceGuid.PcdSaveCmosFieldCompareList|{ \
# Offset  Length                      # Bank0: 0x00~0x7F
                                      # Bank1: 0x80~0xFF
    0x10, 0x2F,                     \ # 0x10 ~ 0x3F
    0x44, 0xBC,                     \ # 0x44 ~ 0xFF
    0x00, 0x0}                        # EndEntry

gInsydeTokenSpaceGuid.PcdI2cControllerTable|{  \ # The definition of I2C host controller number lookup table
  UINT64(0x0000000000001500),                  \ # Number 0 I2C controller is located on dev:0x15 fun:0x00
  UINT64(0x0000000000001501),                  \ # Number 1 I2C controller is located on dev:0x15 fun:0x01
  UINT64(0x0000000000001502),                  \ # Number 2 I2C controller is located on dev:0x15 fun:0x02
  UINT64(0x0000000000001503),                  \ # Number 3 I2C controller is located on dev:0x15 fun:0x03
  UINT64(0x0000000000001900),                  \ # Number 4 I2C controller is located on dev:0x19 fun:0x03
  UINT64(0x0000000000001901),                  \ # Number 5 I2C controller is located on dev:0x19 fun:0x03
  UINT64(0x0000000000001902),                  \ # Number 6 I2C controller is located on dev:0x19 fun:0x03
  UINT64(0x0000000000000000)                   \ # End of table
}

gInsydeTokenSpaceGuid.PcdI2cBusSpeedTable|{                                 \  # The definition of I2C bus configuration lookup table
  UINT16(100000),                                                           \  # Number 0 stands for 100 Khz
  UINT16(400000),                                                           \  # Number 0 stands for 400 khz
  UINT16(1000000),                                                          \  # Number 0 stands for 1  Mhz
  UINT64(00)                                                                \  # End of table
}
#
# The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
#
# BIT0: If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
# BIT1: If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
#       that lie entirely within the expected fixed memory regions.
# BIT2: Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
# BIT3-31: Reserved
#
gPlatformModuleTokenSpaceGuid.PcdWsmtProtectionFlags|0x7
gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxFvSupported|13
##
## Note: The value of gInsydeTokenSpaceGuid.PcdPeiMinMemorySize and PEI_MIN_MEMORY_SIZE should be the same.
## #define  PEI_MIN_MEMORY_SIZE               (10 * 0x800000 + 0x10000000)
##
  gInsydeTokenSpaceGuid.PcdH2OPeiMinMemorySize|0x15000000
#
  # Ensure all HpetBaseAddress PCD have the same value
  #
  gSiPkgTokenSpaceGuid.PcdSiHpetBaseAddress    |0xFED00000
  gSiPkgTokenSpaceGuid.PcdHpetBaseAddress | gSiPkgTokenSpaceGuid.PcdSiHpetBaseAddress
  gPcAtChipsetPkgTokenSpaceGuid.PcdHpetBaseAddress | gSiPkgTokenSpaceGuid.PcdSiHpetBaseAddress
  ## Specifies the number of variable MTRRs reserved for OS use. The default number of
  #  MTRRs reserved for OS use is 0.
  # @Prompt Number of reserved variable MTRRs.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0x0

  gClientCommonModuleTokenSpaceGuid.PcdSmbiosDefaultMaxSpeed|8300

[PcdsDynamicDefault]
## SIO Table
  gChipsetPkgTokenSpaceGuid.PcdPeiSioTable|{ \
    0x02, 0x88, \   #Power On UARTs
    0x28, 0x43, \   #IRQ of UARTs, UART2 IRQ=3,UART1 IRQ=4,
    0x29, 0x80, \   # SIRQ_CLKRUN_EN
    0x2A, 0x00, \
    0x2B, 0xDE, \
    0x2C, 0x02, \
    0x30, 0x60, \
    0x3B, 0x06, \
    0x3C, 0x10, \
    0x3A, 0x0A, \   # LPC Docking Enabling
    0x31, 0x04, \
    0x32, 0x04, \
    0x33, 0x04, \
    0x38, 0xFB, \
    0x35, 0x80, \
    0x36, 0x00, \
    0x37, 0xFE, \
    0x3A, 0x0B, \
    0x3C, 0x90, \
    0x39, 0x00, \
    0x34, 0x01, \
    0x12, 0x2E, \   # Relocate configuration ports base address to 0x2E
    0x13, 0x00}     # to ensure SIO config address can be accessed in OS

## SIO GPIO Table
  gChipsetPkgTokenSpaceGuid.PcdPeiSioGpioTable1|{ \
    0x00,0x00, \
    0x01,0x00, \
    0x02,0x1F, \
    0x03,0x03, \
    0x04,0xF2, \
    0x05,0x00, \
    0x06,0x00, \
    0x07,0x00, \
    0x08,0x1F, \
    0x09,0x04, \
    0x0A,0x00, \
    0x0B,0x00, \
    0x0D,0x00, \
    0x0E,0x32, \
    0x0F,0x00, \
    0x10,0x07, \
    0x20,0x00, \
    0x24,0x00}

  #
  # Preserve Memory Table
  #
  gInsydeTokenSpaceGuid.PcdPreserveMemoryTable|{ \
     UINT32(0x09  ), UINT32(0x80  ),  \ # Preserve 512K(0x80 pages) for ASL
     UINT32(0x0A  ), UINT32(0x6F0 ),  \ # Preserve 2M(0x200 pages) for S3, SMM, etc
     UINT32(0x00  ), UINT32(0x6F0 ),  \ # Preserve 6M(0x6F0 pages) for BIOS reserved memory
     UINT32(0x06  ), UINT32(0x7C0 ),  \ # Preserve 7.75M(0x7C0 pages) for UEFI OS runtime data to make S4 memory consistency
     UINT32(0x05  ), UINT32(0x3D0 ),  \ # Preserve 3.8M(0x3D0 pages) for UEFI OS runtime drivers to make S4 memory consistency
     UINT32(0x03  ), UINT32(0xE000),  \ # Preserve 224M(0xE000 pages) for boot service drivers to reduce memory fragmental
     UINT32(0x04  ), UINT32(0x8000),  \ # Preserve 128M(0x8000 pages) for boot service data to reduce memory fragmental
     UINT32(0x01  ), UINT32(0x200 ),  \ # Preserve 2M(0x200 pages) for UEFI OS boot loader to keep on same address
     UINT32(0x02  ), UINT32(0x00  ),  \
     UINT32(0x0e  ), UINT32(0x00  )}     #EndEntry
#  PcdBoardId
#  The file name is GPIO_xxxx.gpio, where xxxx matches [Skuids] in Project.dsc
#    PchLp
#      BoardIdSkylakeA0Rvp3                    = 0x04 (PcdPeiGpioTable1)
#      BoardIdSkylakeURvp7                     = 0x0B (PcdPeiGpioTable2)
#      BoardIdSkylakeULpddr3Rvp5               = 0x0A (PcdPeiGpioTable5)
#    PchH
#      BoardIdSkylakeAioRvp10Crb               = 0x43 (PcdPeiGpioTable3)
#      BoardIdSkylakeHaloDdr4Rvp11             = 0x44 (PcdPeiGpioTable4)
#    For OEM MB
#      PcdDefaultBoardId                       = 0xFFFFFFFFFFFFFFFF (H2O_DEFAULT_BOARD_ID)
#  if PcdDefaultBoardId = 0xFFFFFFFFFFFFFFFF, default reference PcdH2OBoardId setting.
  gChipsetPkgTokenSpaceGuid.PcdDefaultBoardId|0x04
#
# Define the devices which's SSID/SVID must be programmed before init.
# If SsidSvid is 0 means use PcdDefaultSsidSvid as default SsidSvid value.
# If SsidSvid is not 0, use the value as device's SsidSvid.
#
  gChipsetPkgTokenSpaceGuid.PcdDefaultSsidSvidPeiTable|{ \
#   Bus,  Dev,  Func, SsidSvid,
    0x00, 0x14, 0x00, UINT32(0x00000000), \ # XHCI Controller
    0x00, 0x1F, 0x03, UINT32(0x00000000), \ # HDA Controller
    0xFF, 0xFF, 0xFF, UINT32(0xFFFFFFFF)}   # EndEntry

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE

  ## Default OEM Table ID for ACPI table creation, it is "EDK2    ".
  #  According to ACPI specification, this field is particularly useful when
  #  defining a definition block to distinguish definition block functions.
  #  The OEM assigns each dissimilar table a new OEM Table ID.
  #  This PCD is ignored for definition block.
  # @Prompt Default OEM Table ID for ACPI table creation.
  # default set to "EDK2INTL", will be patched by AcpiPlatform per CPU family
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x4C544E49324B4445

  gChipsetPkgTokenSpaceGuid.PcdLowestSupportedMeVersion|0x3E8
  gChipsetPkgTokenSpaceGuid.PcdLowestSupportedIshVersion|0x3E8
  gChipsetPkgTokenSpaceGuid.PcdLowestSupportedEcVersion|0x3E8
!if gInsydeTokenSpaceGuid.PcdH2ODdtSupported
  #
  # Insyde DDT needs the PcdCpuSmmCodeAccessCheckEnable to be set as FALSE.
  #
   gSiPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable|FALSE
!endif

[PcdsDynamicExDefault]
  gInsydeTokenSpaceGuid.PcdH2OSdhcEmmcSupported|TRUE
  #
  # Port number mapping table Define
  #
  gInsydeTokenSpaceGuid.PcdPortNumberMapTable|{ \
    0x00, 0x17, 0x00, 0, 0, 0x00, \
    0x00, 0x17, 0x00, 0, 1, 0x02, \
    0x00, 0x17, 0x00, 1, 0, 0x01, \
    0x00, 0x17, 0x00, 1, 1, 0x03, \
    0x00, 0x16, 0x02, 0, 0, 0x06, \
    0x00, 0x16, 0x02, 0, 1, 0x07, \
    0x00, 0x16, 0x02, 1, 0, 0x08, \
    0x00, 0x16, 0x02, 1, 1, 0x09, \
    0x00, 0x00, 0x00, 0, 0, 0x00} #EndEntry
#
# PcdsDynamicEx that is defined by Chipset team put on here. Start from 0x80000000
#
  # PIRQ link value                               PIRQA, PIRQB, PIRQC, PIRQD, PIRQE, PIRQF, PIRQG, PIRQH
  gChipsetPkgTokenSpaceGuid.PcdPirqLinkValueArray|{0x60,  0x61,  0x62,  0x63,  0x68,  0x69,  0x6A,  0x6B} #EndEntry

# D18 => Dev 0x12 => 0x90
# D19 => Dev 0x13 => 0x98
# D20 => Dev 0x14 => 0xA0
# D21 => Dev 0x15 => 0xA8
# D22 => Dev 0x16 => 0xB0
# D23 => Dev 0x17 => 0xB8
# D24 => Dev 0x18 => 0xC0
# D25 => Dev 0x19 => 0xC8
# D26 => Dev 0x1A => 0xD0
# D27 => Dev 0x1B => 0xD8
# D28 => Dev 0x1C => 0xE0
# D29 => Dev 0x1D => 0xE8
# D31 => Dev 0x1F => 0xF8
#        Dev 0x01 => 0x08 PEG
#        Dev 0x02 => 0x10 IGD
#        Dev 0x03 => 0x18 SA Audio Device
#        Dev 0x04 => 0x20 SA Thermal Device

#
# IRQMask Define
#
#PIRQA  PIRQB  PIRQC  PIRQD  PIRQE  PIRQF  PIRQG  PIRQH  NPIRQ
#0xDEB8 0xDEB8 0xDEB8 0xDEB8 0xDEB8 0xDEB8 0xDEB8 0xDEB8 0xDEB8
  #
  #Bus,  Dev, INT#A,       IrqMask, INT#B,       IrqMask, INT#C,       IrqMask, INT#D,       IrqMask, Slot, Rsv.,      DevIpRegValue, ProgrammableIrq.
  #
  gChipsetPkgTokenSpaceGuid.PcdControllerDeviceIrqRoutingEntry| { \
  0x00, 0xF8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D31
  0x00, 0xE8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D29
  0x00, 0xE0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D28
  0x00, 0xD8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D27
  0x00, 0xB8, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D23
  0x00, 0xB0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D22
  0x00, 0xA0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D20
  0x00, 0x90, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D18
  0x00, 0x08, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D01
  0x00, 0x10, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D02
  0x00, 0x20, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D04
  0x00, 0x28, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D05
  0x00, 0x40, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D08
  0x04, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B04
  0x05, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B05
  0x06, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B06
  0x07, 0x00, 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B07
  0x08, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B08
  0x09, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B09
  0x0E, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0E
  0x0F, 0x00, 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0F
  0x02, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B02
  0x0A, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0A
  0x0B, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00} #EndEntry

  #
  #Bus,  Dev, INT#A,       IrqMask, INT#B,       IrqMask, INT#C,       IrqMask, INT#D,       IrqMask, Slot, Rsv.,      DevIpRegValue, ProgrammableIrq.
  #
  gChipsetPkgTokenSpaceGuid.Pcd2HControllerDeviceIrqRoutingEntry| { \
  0x00, 0xF8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D31
  0x00, 0xE8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D29
  0x00, 0xE0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D28
  0x00, 0xD8, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D27
  0x00, 0xB8, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D23
  0x00, 0xB0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D22
  0x00, 0xA0, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D20
  0x00, 0x90, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D18
  0x00, 0x08, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D01
  0x00, 0x10, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D02
  0x00, 0x20, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D04
  0x00, 0x28, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D05
  0x00, 0x40, 0x60, UINT16(0xDEB8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, UINT16(0xDEF8), 0x00, 0x00, UINT32(0x00000000), 0x00,  \  #D08
  0x04, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B04
  0x05, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B05
  0x06, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B06
  0x07, 0x00, 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B07
  0x08, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B08
  0x09, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B09
  0x0E, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0E
  0x0F, 0x00, 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0F
  0x02, 0x00, 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B02
  0x0A, 0x00, 0x61, UINT16(0xDEB8), 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00,  \  #B0A
  0x0B, 0x00, 0x62, UINT16(0xDEB8), 0x63, UINT16(0xDEB8), 0x60, UINT16(0xDEB8), 0x61, UINT16(0xDEB8), 0x00, 0xFF, UINT32(0x00000000), 0x00} #EndEntry

  gChipsetPkgTokenSpaceGuid.PcdVirtualBusTable|{ \
  0x00, 0x1c, 0x00, 0x04, \  # PCIE Root Port 1
  0x00, 0x1c, 0x01, 0x05, \  # PCIE Root Port 2
  0x00, 0x1c, 0x02, 0x06, \  # PCIE Root Port 3
  0x00, 0x1c, 0x03, 0x07, \  # PCIE Root Port 4
  0x00, 0x1c, 0x04, 0x08, \  # PCIE Root Port 5
  0x00, 0x1c, 0x05, 0x09, \  # PCIE Root Port 6
  0x00, 0x1c, 0x06, 0x0E, \  # PCIE Root Port 7
  0x00, 0x1c, 0x07, 0x0F, \  # PCIE Root Port 8
  0x00, 0x01, 0x00, 0x02, \  # P.E.G. Root Port D1F1
  0x00, 0x01, 0x01, 0x0A, \  # P.E.G. Root Port D1F2
  0x00, 0x01, 0x02, 0x0B}    # P.E.G. Root Port D1F0 #EndEntry

  gChipsetPkgTokenSpaceGuid.PcdIrqPoolTable|{11, 0x00, \ #IRQ11
                                             10, 0x00}   #IRQ10 #EndEntry

  gChipsetPkgTokenSpaceGuid.PcdPirqPriorityTable|{0,  \# PIRQ A
                                                  0,  \# PIRQ B
                                                  0,  \# PIRQ C
                                                  0,  \# PIRQ D
                                                  0,  \# PIRQ E
                                                  0,  \# PIRQ F
                                                  0,  \# PIRQ G
                                                  0}   # PIRQ H #EndEntry

#  gChipsetPkgTokenSpaceGuid.PcdH2OConsoleRedirectionClassGuid|{GUID("CB63151A-D635-49f6-8DE9-CDC992FF7477")}

#  gH2OFlashDeviceMfrNameGuid.PcdWinbondW25q256fvSpiMfrName|"WINBOND"
#  gH2OFlashDevicePartNameGuid.PcdWinbondW25q256fvSpiPartName|"W25Q256FV"
#  gH2OFlashDeviceGuid.PcdWinbondW25q256fvSpi|{ \ # WINBOND  W25Q256FV
#    0x03, 0x00, 0x00, 0x00, 0xef, 0x40, 0x19, 0x00,  \  # DeviceType = 03  Id = 001940EF
#    0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x20   \  # ExtId = 00000000  BlockSize = 0010  BlockCount = 2000
#  }
#
#  gH2OFlashDeviceConfigGuid.PcdWinbondW25q256fvSpiConfig|{ \ # WINBOND  W25Q256FV
#    0x28, 0x00, 0x00, 0x00, 0x9f, 0x20, 0x02, 0x01,  \  # Size = 00000028  ReadIdOp = 9F  EraseOp = 20  WriteOp = 02  WriteStatusOp = 01
#    0x03, 0x05, 0x00, 0x00, 0x7c, 0xf2, 0x06, 0x06,  \  # ReadOp = 03  ReadStatusOp = 05  OpType = F27C  WriteEnablePrefix = 06  WriteStatusEnablePrefix = 06
#    0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x01, 0x00,  \  # GlobalProtectAvailable = 00  BlockProtectAvailable = 00  BlockProtectCodeRequired = 00  MultiByteProgramAvailable = 01  BytesOfId = 03  MinBytePerProgRead = 01  NoVolStatusAvailable = 01
#    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,  \  # GlobalProtectCode = 00  GlobalUnprotectCode = 00  BlockProtectCode = 00  BlockUnprotectCode = 00  DeviceSize = 02000000
#    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   \  # BlockEraseSize = 00001000  BlockProtectSize = 00000000
#  }

#
#  Reduce POST Time when issuing device reset, soluiton IB05300634 from KBL.
#
  gEfiMdeModulePkgTokenSpaceGuid.PcdFastPS2Detection|FALSE
  gEfiSecurityPkgTokenSpaceGuid.PcdFirmwareDebuggerInitialized|FALSE
  gInsydeTokenSpaceGuid.PcdH2OSdhcEmmcHs200Supported|TRUE

[PcdsPatchableInModule]
!if $(TARGET) == DEBUG
  gSiPkgTokenSpaceGuid.PcdSerialIoUartDebugEnable|1
!endif

[PcdsDynamicHii.X64.DEFAULT]
  gSiPkgTokenSpaceGuid.PcdCpuSmmMsrSaveStateEnable|L"CpuSmm"|gCpuSmmGuid|0x0|0
  gSiPkgTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable|L"CpuSmm"|gCpuSmmGuid|0x1|1
  gSiPkgTokenSpaceGuid.PcdCpuSmmUseDelayIndication|L"CpuSmm"|gCpuSmmGuid|0x2|1
  gSiPkgTokenSpaceGuid.PcdCpuSmmUseBlockIndication|L"CpuSmm"|gCpuSmmGuid|0x3|1
  gSiPkgTokenSpaceGuid.PcdCpuSmmUseSmmEnableIndication|L"CpuSmm"|gCpuSmmGuid|0x4|1

[Components]


[Components.$(PEI_ARCH)]
  !disable MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Core/Pei/PeiMain.inf {
    <SOURCE_OVERRIDE_PATH>
      MdeModulePkg/Override/Core/Pei
    <LibraryClasses>
!if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
  }
#
# Initial Xhci Host Controller before DDT
#
!if gInsydeTokenSpaceGuid.PcdH2ODdtSupported
  $(CHIPSET_PKG)/PchDdtUsbPei/PchDdtUsbPei.inf
!endif
  $(CHIPSET_PKG)/GetVariableHook/GetVariableHookPei.inf
  $(CHIPSET_PKG)/EarlyProgramGpioPei/EarlyProgramGpioPei.inf

#!if $(H2O_DDT_DEBUG_IO) == Com
  $(CHIPSET_PKG)/InitSerialPortPei/InitSerialPortPei.inf
#!endif

!if gChipsetPkgTokenSpaceGuid.PcdRestoreCmosfromVariableFlag
  $(CHIPSET_PKG)/SaveRestoreCmos/RestoreCmosPei/RestoreCmosPei.inf
!endif

!if gChipsetPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  $(CHIPSET_PKG)/Features/Tbt/TbtInit/Pei/PeiTbtInit.inf
!endif

  $(CHIPSET_PKG)/SioInitPei/SioInitPei.inf
!if gInsydeTokenSpaceGuid.PcdCrisisRecoverySupported
  $(CHIPSET_PKG)/CommonChipset/SpeakerPei/LegacySpeakerPei.inf
  $(CHIPSET_PKG)/PchAhciPei/PchAhciPei.inf
  $(CHIPSET_PKG)/PchNvmePei/PchNvmePei.inf
  $(CHIPSET_PKG)/PchUfsPei/PchUfsPei.inf
  $(CHIPSET_PKG)/PchSdhcPei/PchSdhcPei.inf
!endif

!if gInsydeTokenSpaceGuid.PcdH2OUsbPeiSupported
  $(CHIPSET_PKG)/PchXhciPei/PchXhciPei.inf
!endif

  $(CHIPSET_PKG)/PlatformInit/PlatformInitPei/PlatformInitPreMem.inf {
    <LibraryClasses>
    !if gSiPkgTokenSpaceGuid.PcdCflCpuEnable == TRUE
        NULL|$(CHIPSET_PKG)/Library/PeiBootGuardEventLogLib/PeiBootGuardEventLogLib.inf
    !else
        NULL|$(CHIPSET_PKG)/Library/PeiBootGuardEventLogLib/PeiBootGuardEventLogLibCnl.inf
    !endif
  }

  $(CHIPSET_PKG)/PlatformInit/PlatformInitPei/PlatformInit.inf {
    <LibraryClasses>
      CpuPolicyLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiCpuPolicyLib/PeiCpuPolicyLib.inf
  }

  $(CHIPSET_PKG)/ChipsetSvcPei/ChipsetSvcPei.inf

!if gChipsetPkgTokenSpaceGuid.PcdSwitchableGraphicsSupported
  $(CHIPSET_PKG)/SwitchableGraphicsPei/SwitchableGraphicsPei.inf
!endif

!if gChipsetPkgTokenSpaceGuid.PcdPttSupported
  $(CHIPSET_PKG)/HciPei/HciPei.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdBootGuardEnable
!if gChipsetPkgTokenSpaceGuid.PcdBootGuardVerifyFv
  $(CHIPSET_PKG)/BootGuardRecoveryHookPei\BootGuardRecoveryHookPei.inf
  $(CHIPSET_PKG)/BootGuardVerifyFvMainPei/BootGuardVerifyFvMainPei.inf
!endif
!endif

!if gSiPkgTokenSpaceGuid.PcdTraceHubEnable
  $(CHIPSET_PKG)/TraceHubStatusCodeHandler/Pei/TraceHubStatusCodeHandlerPei.inf
!endif

!if gInsydeTokenSpaceGuid.PcdH2OPeiTimerSupported
  $(CHIPSET_PKG)/8259InterruptControllerPei/8259.inf
  $(CHIPSET_PKG)/CpuArchPei/CpuArchPei.inf
  $(CHIPSET_PKG)/SmartTimer/Pei/SmartTimer.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  $(CHIPSET_PKG)/Features/OverClocking/OverClockInit/PeiOverClock.inf
!endif

!include $(CHIPSET_PKG)/Board/PeiBoardInit.dsc

#[-start-171121-IB10560319-modify]#
!if gInsydeTokenSpaceGuid.PcdBackupSecureBootSettingsSupported == TRUE
#[-end-171121-IB10560319-modify]#
  $(PLATFORM_PACKAGE)/Platform/BiosInfoChecker/BiosInfoChecker.inf
!endif
!if gEfiTraceHubTokenSpaceGuid.PcdStatusCodeUseTraceHub
  $(CHIPSET_PKG)/TraceHubPostCodeHandler/Pei/TraceHubPostCodeHandlerPei.inf
!endif
################################################################################
#
# Components.IA32 Override
#
################################################################################

#
# The override should be removed after the DDT debug drivers are ready for SKL from kernel code.
#
# Fix Post hang on CP A2 issue
#

# !disable InsydeModulePkg/Universal/Recovery/CrisisRecoveryPei/CrisisRecoveryPei.inf
#!if gInsydeTokenSpaceGuid.PcdCrisisRecoverySupported
#  InsydeModulePkg/Universal/Recovery/CrisisRecoveryPei/CrisisRecoveryPei.inf {
#    <SOURCE_OVERRIDE_PATH>
#     $(CHIPSET_PKG)/Override/InsydeModulePkg/Universal/Recovery/CrisisRecoveryPei/
# }
#!endif

  $(CHIPSET_PKG)/GpioExpander/GpioExpanderPei.inf

#  !disable InsydeModulePkg/Universal/Variable/SetupDataProviderPei/SetupDataProviderPei.inf
#  $(CHIPSET_PKG)/Override/InsydeModulePkg/Universal/Variable/SetupDataProviderPei/SetupDataProviderPei.inf
#  $(CHIPSET_PKG)/Override/
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf

!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/IntelFsp2WrapperPkg/FspmWrapperPeim # RPPO-CNL-0062
    <LibraryClasses>
      MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  }

!else
  IntelFsp2WrapperPkg/FspmWrapperPeim/FspmWrapperPeim.inf {
    <LibraryClasses>
      MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  }
!endif
  IntelFsp2WrapperPkg/FspsWrapperPeim/FspsWrapperPeim.inf
  $(CHIPSET_PKG)/FspWrapper/FspWrapperPeim/FspWrapperPeim.inf
!endif
  $(CHIPSET_PKG)/DummyPcdPei/DummyPcdPei.inf
  $(CHIPSET_PKG)/InsydeReportFvPei/InsydeReportFvPei.inf

  #
  # Replace PlatformStage1Pei with PlatformInitPreMem
  #
  !disable InsydeModulePkg/Universal/CommonPolicy/PlatformStage1Pei/PlatformStage1Pei.inf
  !disable InsydeModulePkg/Universal/CommonPolicy/PlatformStage2Pei/PlatformStage2Pei.inf

[Components.$(DXE_ARCH)]
  !disable MdeModulePkg/Core/Dxe/DxeMain.inf
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      CpuExceptionHandlerLib|InsydeModulePkg/Library/CpuExceptionHandlerLib/ThunkCpuExceptionHandlerLib.inf
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/MdeModulePkg/Core/Dxe
      MdeModulePkg/Override/Core/Dxe
  }

  $(CHIPSET_PKG)/GetVariableHook/GetVariableHookDxe.inf
!if gChipsetPkgTokenSpaceGuid.PcdPttSupported
  $(CHIPSET_PKG)/HciDxe/HciDxe.inf
!endif
!if gInsydeTokenSpaceGuid.PcdH2OSdhcSupported == FALSE
  $(CHIPSET_PKG)/Features/SdMmc/SdControllerDxe/MmcHostDxe.inf
  $(CHIPSET_PKG)/Features/SdMmc/SdMediaDeviceDxe/MmcMediaDeviceDxe.inf
!endif
  $(CHIPSET_PKG)/Universal/SmbiosProcessorDxe/SmbiosProcessorDxe.inf # RPPO-KBL-0036
  $(CHIPSET_PKG)/BiosGuard/BiosGuardStoreStatusDxe/BiosGuardStoreStatusDxe.inf
  $(CHIPSET_PKG)/RestoreMtrrDxe/RestoreMtrrDxe.inf
  $(CHIPSET_PKG)/Platform/SaveMemoryConfig/Dxe/SaveMemoryConfig.inf
  $(CHIPSET_PKG)/Platform/PlatformInfo/Dxe/PlatformInfoDxe.inf
  $(CHIPSET_PKG)/Features/SdMmc/PlatformEmmcDxe/PlatformEmmcDxe.inf
  $(CHIPSET_PKG)/UsbLegacyControlSmm/UsbLegacyControlSmm.inf
  $(CHIPSET_PKG)/UefiSetupUtilityDxe/SetupUtilityDxe.inf
  $(CHIPSET_PKG)/FrontPageDxe/FrontPageDxe.inf
  $(CHIPSET_PKG)/PlatformDxe/PlatformDxe.inf
  $(CHIPSET_PKG)/Features/SecureEraseDxe/SecureEraseDxe.inf
  $(CHIPSET_PKG)/SataDevSleepDxe/SataDevSleepDxe.inf
  $(CHIPSET_PKG)/PciPlatformDxe/PciPlatformDxe.inf
#  $(CHIPSET_PKG)/IsctDxe/IsctAcpiDxe.inf
#  $(CHIPSET_PKG)/IsctSmm/IsctSmm.inf
  $(CHIPSET_PKG)/Platform/SmmPlatform/Smm/SmmPlatform.inf {
    <LibraryClasses>
      ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
      ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
   }
  $(CHIPSET_PKG)/Int15MicrocodeSmm/Int15MicrocodeSmm.inf
  $(CHIPSET_PKG)/BbstableHookDxe/BbstableHookDxe.inf
#
# Accelerator for Windows 7 warm booting.
#
  $(CHIPSET_PKG)/CsmInt10BlockDxe/CsmInt10BlockDxe.inf
  $(CHIPSET_PKG)/CsmInt10HookSmm/CsmInt10HookSmm.inf

!if gInsydeTokenSpaceGuid.PcdH2OCsmSupported == TRUE
  $(CHIPSET_PKG)/CsmInt15HookSmm/CsmInt15HookSmm.inf {
    <LibraryClasses>
      ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
  }
!endif

  $(CHIPSET_PKG)/BiosRegionLockDxe/BiosRegionLockDxe.inf
  $(CHIPSET_PKG)/I2cBus/Dxe/I2cMasterDxe.inf
  $(CHIPSET_PKG)/BiosGuard/BiosGuardInstallGuidSmm/BiosGuardInstallGuidSmm.inf
#
#  Policy
#
  $(CHIPSET_PKG)/PlatformInit/PolicyInitDxe/PolicyInitDxe.inf
  $(CHIPSET_PKG)/PolicyInit/InsydeChipsetPolicy/GopPolicyDxe/GopPolicyDxe.inf

!if gSiPkgTokenSpaceGuid.PcdUseHpetTimer == TRUE
    $(CHIPSET_PKG)/Override/PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf
    $(CHIPSET_PKG)/Override/PcAtChipsetPkg/8254TimerDxe/8254Timer.inf
!else
    PcAtChipsetPkg/8254TimerDxe/8254Timer.inf
!endif
!if gChipsetPkgTokenSpaceGuid.PcdTbtEnable == TRUE
  $(CHIPSET_PKG)/Features/Tbt/TbtInit/Smm/TbtSmm.inf
  $(CHIPSET_PKG)/Features/Tbt/TbtInit/Dxe/TbtDxe.inf
!endif

!if gChipsetPkgTokenSpaceGuid.PcdTXTSupported == TRUE
!if gChipsetPkgTokenSpaceGuid.PcdTXTSinitAcmSupported == TRUE
  $(CHIPSET_PKG)/TxtLoadSinitAcmDxe/TxtLoadSinitAcmDxe.inf
!endif
!endif

  $(CHIPSET_PKG)/Features/Wdt/WdtAppDxe/WdtAppDxe.inf
  $(CHIPSET_PKG)/Features/Sata/SataController/SataController.inf
  $(CHIPSET_PKG)/GpioExpander/GpioExpanderDxe.inf
  $(CHIPSET_PKG)/OemModifyOpRegionDxe/OemModifyOpRegionDxe.inf
  $(CHIPSET_PKG)/OemAcpiPlatformDxe/OemAcpiPlatformDxe.inf
  $(CHIPSET_PKG)/UpdateDsdtByAcpiSdtDxe/UpdateDsdtByAcpiSdtDxe.inf
  $(CHIPSET_PKG)/SmmThunkSmm/SmmThunkSmm.inf
  $(CHIPSET_PKG)/MemInfoDxe/MemInfoDxe.inf
  $(CHIPSET_PKG)/VbiosHookSmm/VbiosHookSmm.inf
  $(CHIPSET_PKG)/Platform/SavePegConfig/Dxe/SavePegConfig.inf
  $(CHIPSET_PKG)/TxtOneTouchDxe/TxtOneTouchDxe.inf
#  $(CHIPSET_PKG)/PlatformResetRunTimeDxe/PlatformResetRunTimeDxe.inf
  $(CHIPSET_PKG)/OemBadgingSupportDxe/OEMBadgingSupportDxe.inf

  $(CHIPSET_PKG)/ChipsetSvcDxe/ChipsetSvcDxe.inf
  $(CHIPSET_PKG)/Features/Acpi/AcpiTables/AcpiFeatures.inf
  $(CHIPSET_PKG)/ChipsetSvcSmm/ChipsetSvcSmm.inf
  $(CHIPSET_PKG)/XhciSmiDispatcher/XhciSmiDispatcher.inf
  $(CHIPSET_PKG)/CommonChipset/SetSsidSvidDxe/SetSsidSvidDxe.inf
#  $(CHIPSET_PKG)/CommonChipset/AspmOverrideDxe/AspmOverrideDxe.inf
  $(CHIPSET_PKG)/CommonChipset/SpeakerDxe/LegacySpeakerDxe.inf

!if gChipsetPkgTokenSpaceGuid.PcdGopOverrideDriverEnable == TRUE
  $(CHIPSET_PKG)/Features/GopOverrideDriver/GopOverrideDriver.inf
!endif

!if gPlatformModuleTokenSpaceGuid.PcdIntelGopEnable == TRUE
!if gSiPkgTokenSpaceGuid.PcdMinTreeEnable == FALSE
  $(CHIPSET_PKG)/Features/Gop/GopDebugDxe/GopDebugDxe.inf
!endif
!endif

!if gSiPkgTokenSpaceGuid.PcdAmtEnable
  $(CHIPSET_PKG)/Features/AMT/AmtPetAlert/AmtPetAlert.inf
  $(CHIPSET_PKG)/Features/AMT/AmtSaveMebxConfigDxe/AmtSaveMebxConfigDxe.inf
#[-start-171129-IB06720506-add]#
  $(CHIPSET_PKG)/Features/AMT/AmtWrapperDxe/AmtWrapperDxe.inf
#[-end-171129-IB06720506-add]#
  $(CHIPSET_PKG)/Features/AMT/AmtLockPs2ConInDxe/AmtLockPs2ConInDxe.inf
  $(CHIPSET_PKG)/Features/AMT/AmtLockI2cConInDxe/AmtLockI2cConInDxe.inf
!if gInsydeTokenSpaceGuid.PcdH2OUsbSupported
  $(CHIPSET_PKG)/Features/AMT/AmtLockUsbConInDxe/AmtLockUsbConInDxe.inf
!endif
!endif

  $(CHIPSET_PKG)/IhisiSmm/IhisiSmm.inf
!if gChipsetPkgTokenSpaceGuid.PcdMeCapsuleUpdateSupported == TRUE || gChipsetPkgTokenSpaceGuid.PcdIshCapsuleUpdateSupported == TRUE || gChipsetPkgTokenSpaceGuid.PcdEcCapsuleUpdateSupported == TRUE || gChipsetPkgTokenSpaceGuid.PcdPdtCapsuleUpdateSupported == TRUE
!if gInsydeTokenSpaceGuid.PcdH2OIhisiFmtsSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsuleSmm/MeCapsuleSmm.inf
!endif
!endif
!if gChipsetPkgTokenSpaceGuid.PcdSwitchableGraphicsSupported
  $(CHIPSET_PKG)/SwitchableGraphicsDxe/SwitchableGraphicsDxe.inf
  $(CHIPSET_PKG)/SwitchableGraphicsSmm/SwitchableGraphicsSmm.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/AmdDiscreteSsdt.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/NvidiaDiscreteSsdt.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/AmdPowerXpressSsdt.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/NvidiaOptimusSsdt.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/AmdUltPowerXpressSsdt.inf
  $(CHIPSET_PKG)/SwitchableGraphicsAcpi/NvidiaUltOptimusSsdt.inf
!endif

!if gChipsetPkgTokenSpaceGuid.PcdMeCapsuleUpdateSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsuleDxe/MeCapsuleDxe.inf
!endif
!if gChipsetPkgTokenSpaceGuid.PcdIshCapsuleUpdateSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsuleIshDxe/IshCapsuleDxe.inf
!endif
!if gChipsetPkgTokenSpaceGuid.PcdPdtCapsuleUpdateSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsulePdtDxe/PdtCapsuleDxe.inf
!endif
!if gChipsetPkgTokenSpaceGuid.PcdEcCapsuleUpdateSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsuleEcDxe/EcCapsuleDxe.inf
!endif
  $(CHIPSET_PKG)/SmbiosUpdateDxe/SmbiosUpdateDxe.inf

!if gChipsetPkgTokenSpaceGuid.PcdRestoreCmosfromVariableFlag
  $(CHIPSET_PKG)/SaveRestoreCmos/SaveCmosDxe/SaveCmosDxe.inf
!endif

!if gInsydeTokenSpaceGuid.PcdDynamicHotKeySupported
 $(CHIPSET_PKG)/DynamicHotKeyDxe/DynamicHotKeyDxe.inf
!endif

  $(CHIPSET_PKG)/Tools/Source/Shell/KblRecovery/KblRecovery.inf

!if gSiPkgTokenSpaceGuid.PcdTraceHubEnable
  $(CHIPSET_PKG)/TraceHubStatusCodeHandler/RuntimeDxe/TraceHubStatusCodeHandlerRuntimeDxe.inf
  $(CHIPSET_PKG)/TraceHubStatusCodeHandler/Smm/TraceHubStatusCodeHandlerSmm.inf
!endif

  $(CHIPSET_PKG)/Platform/SmramSaveInfoHandlerSmm/SmramSaveInfoHandlerSmm.inf

!if gChipsetPkgTokenSpaceGuid.PcdSupportUnLockedBarHandle
 $(CHIPSET_PKG)/UnLockedBarHandleSmm/UnLockedBarHandleSmm.inf
!endif

 $(CHIPSET_PKG)/Features/Cnv/CnvUefiVariables/CnvUefiVariables.inf

  #
  # IBV HSTI Platform Security State Interface alreday implement by Insyde Kernel.
  # (InsydeModulePkg/Universal/Security/HstiDxe/HstiDxe.inf)
  # Do not reference HstiIbvPlatformDxe from Intel.
  #
#  $(CHIPSET_PKG)/Features/Hsti/HstiIbvPlatformDxe/HstiIbvPlatformDxe.inf {
#  <LibraryClasses>
#    HstiLib|$(PLATFORM_SI_PACKAGE)/Override/MdePkg/Library/DxeHstiLib/DxeHstiLib.inf  # SICO-KBL-0003
#  }
#
# RST One Click switch module
#
!if gPlatformModuleTokenSpaceGuid.PcdRstOneClickEnable == TRUE
  $(CHIPSET_PKG)/Features/Rst/RstOneClickEnable/RstOneClickEnable.inf
!endif

!if gEfiTraceHubTokenSpaceGuid.PcdStatusCodeUseTraceHub
  $(CHIPSET_PKG)/TraceHubPostCodeHandler/Dxe/TraceHubPostCodeHandlerDxe.inf
!endif

  $(CHIPSET_PKG)/Features/Hsti/HstiResultDxe/HstiResultDxe.inf
  $(CHIPSET_PKG)/Features/Hsti/HstiIhvDxe/HstiIhvDxe.inf

#
# Crash Log Support
#
  $(CHIPSET_PKG)/Features/CrashLogDxe/CrashLogDxe.inf

#[-start-171121-IB10560319-modify]#
#
# Prepare for Wifi pre-boot
#

#!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
  #
  # UEFI network modules
  #
#  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
#  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
#  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
#  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
#  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
#  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
#  MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf
#  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
#  NetworkPkg/DnsDxe/DnsDxe.inf
#  NetworkPkg/HttpDxe/HttpDxe.inf
#  NetworkPkg/HttpUtilitiesDxe/HttpUtilitiesDxe.inf
#  NetworkPkg/HttpBootDxe/HttpBootDxe.inf
#  BpCommonPkg/Universal/WifiConnectionManagerDxe/WifiConnectionManagerDxe.inf
#!if gPlatformModuleTokenSpaceGuid.PcdTlsEnable == TRUE
#  NetworkPkg/TlsDxe/TlsDxe.inf
#  $(PLATFORM_PACKAGE)/Override/NetworkPkg/TlsAuthConfigDxe/TlsAuthConfigDxe.inf # RPPO-CNL-0071
#!endif
#
#!if gPlatformModuleTokenSpaceGuid.PcdNetworkIp6Enable == TRUE
#  NetworkPkg/Ip6Dxe/Ip6Dxe.inf
#  NetworkPkg/Dhcp6Dxe/Dhcp6Dxe.inf
#  NetworkPkg/IpSecDxe/IpSecDxe.inf
#  NetworkPkg/TcpDxe/TcpDxe.inf
#  NetworkPkg/Udp6Dxe/Udp6Dxe.inf
#  NetworkPkg/Mtftp6Dxe/Mtftp6Dxe.inf
#!endif
#
#!if gPlatformModuleTokenSpaceGuid.PcdNetworkIp6Enable == TRUE
#  NetworkPkg/UefiPxeBcDxe/UefiPxeBcDxe.inf
#!else
#  MdeModulePkg/Universal/Network/UefiPxeBcDxe/UefiPxeBcDxe.inf
#!endif
#!endif # gPlatformModuleTokenSpaceGuid.PcdNetworkEnable
#[-end-171121-IB10560319-modify]#

 $(CHIPSET_PKG)/AsfSecureBootSmm/AsfSecureBootSmm.inf
 $(CHIPSET_PKG)/AsfSecureBootDxe/AsfSecureBootDxe.inf

################################################################################
#
# Components.X64 Override
#
################################################################################
  !disable InsydeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  InsydeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/InsydeModulePkg/Bus/Pci/PciBusDxe
  }
  !disable InsydeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf
  InsydeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }
  !disable InsydeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  InsydeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf {
    <Depex>
      gEfiAcpiTableProtocolGuid AND
      gH2OChipsetServicesProtocolGuid AND
      gEfiFirmwareVolume2ProtocolGuid AND
      gEfiCpuIo2ProtocolGuid AND
      gEfiMpServiceProtocolGuid AND
      $(CHIPSET_REF_CODE_PKG)/SiPkg.dec|gCpuNvsAreaProtocolGuid AND
      gEfiSetupUtilityProtocolGuid AND
      $(CHIPSET_PKG)/$(CHIPSET_PKG).dec|gPlatformNvsAreaProtocolGuid
  }

  !disable UefiCpuPkg/CpuDxe/CpuDxe.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf {
    <Depex>
      InsydeModulePkg/InsydeModulePkg.dec|gEfiLegacy8259ProtocolGuid
  }

  !disable MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
    <LibraryClasses>
      NULL|InsydeModulePkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
!if gInsydeTokenSpaceGuid.PcdH2OTpmSupported
      NULL|InsydeModulePkg/Library/DxeTpmMeasureBootLib/DxeTpmMeasureBootLib.inf
!endif
!if gInsydeTokenSpaceGuid.PcdH2OTpm2Supported
      NULL|InsydeModulePkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
!endif
  }


!if gSiPkgTokenSpaceGuid.PcdAmtEnable
  IntelFrameworkModulePkg/Universal/StatusCode/DatahubStatusCodeHandlerDxe/DatahubStatusCodeHandlerDxe.inf
!endif

 !disable InsydeModulePkg/Universal/FirmwareVolume/FwBlockServiceSmm/FwBlockServiceSmm.inf
 InsydeModulePkg/Universal/FirmwareVolume/FwBlockServiceSmm/FwBlockServiceSmm.inf {
 <SOURCE_OVERRIDE_PATH>
   $(CHIPSET_PKG)/Override/InsydeModulePkg/Universal/FirmwareVolume/FwBlockServiceSmm
 }
#[-start-180504-IB1446105-add]#
!if gPlatformModuleTokenSpaceGuid.PcdVtioEnable == TRUE
$(CHIPSET_PKG)/Vtio/Dxe/VtioDxe.inf
!endif
#[-end-180504-IB1446105-add]#

  $(CHIPSET_PKG)/Features/Dptf/AcpiTables/DptfAcpiTables.inf
  $(CHIPSET_PKG)/Features/Dptf/AcpiTables/Sds/DptfAcpiTablesSds.inf
  $(CHIPSET_PKG)/Features/Dptf/Dxe/Dptf.inf
  $(CHIPSET_PKG)/Acpi/AcpiTables/AcpiTables.inf
  $(CHIPSET_PKG)/Features/Acpi/AcpiTables/SsdtIrmt/IrmtAcpiTables.inf
  $(CHIPSET_PKG)/Features/Pep/Smm/PepBccdSmm.inf
#  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

!if gChipsetPkgTokenSpaceGuid.PcdUseCrbEcFlag == TRUE
  $(CHIPSET_PKG)/EC/EC.inf
!endif
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  $(CHIPSET_PKG)/Features/OverClocking/OverClockInit/DxeOverClock.inf
  $(CHIPSET_PKG)/Features/OverClocking/OverclockInterface/OverclockInterface.inf
  $(CHIPSET_PKG)/Features/OverClocking/OverclockInterface/OverclockInterfaceACPI.inf
  $(CHIPSET_PKG)/Features/OverClocking/OverclockInterface/OverClockSmiHandler.inf
!endif
  $(CHIPSET_PKG)/Features/UsbTypeC/UsbTypeCDxe/UsbTypeCDxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
 <SOURCE_OVERRIDE_PATH>
   $(CHIPSET_PKG)/Override/MdeModulePkg/Bus/Pci/PciHostBridgeDxe
 }

!if gChipsetPkgTokenSpaceGuid.PcdFspWrapperEnable == TRUE
  IntelFsp2WrapperPkg/FspWrapperNotifyDxe/FspWrapperNotifyDxe.inf
!endif

!include $(CHIPSET_PKG)/Board/DxeBoardInit.dsc

  !disable InsydeModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf
    InsydeModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf {
      <SOURCE_OVERRIDE_PATH>
        $(CHIPSET_PKG)/Override/InsydeModulePkg/Csm/LegacyBiosDxe
  }

#
#  Reduce POST Time when issuing device reset, soluiton IB05300634 from KBL.
#
  # !disable InsydeModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2keyboardDxe.inf
  # IntelFrameworkModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2keyboardDxe.inf {
  #   <LibraryClasses>
  #     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  # }
#  !disable InsydeModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
#  IntelFrameworkModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
  # !disable InsydeModulePkg/Csm/BiosThunk/KeyboardDxe/KeyboardDxe.inf
  # IntelFrameworkModulePkg/Csm/BiosThunk/KeyboardDxe/KeyboardDxe.inf

#[-start-171121-IB10560319-modify]#
#
# Prepare for Wifi pre-boot
#

#  BpCommonPkg/Bluetooth/UsbBtHciDxe/UsbBtHciDxe.inf
#  BpCommonPkg/Bluetooth/BluetoothBusDxe/BluetoothBusDxe.inf
# Temporary workaround for GCC and Xcode build due to this driver causes system hang.
#!if ("MSFT" in $(FAMILY))
#  BpCommonPkg/Bluetooth/BluetoothConnectionManagerDxe/BluetoothConnectionManagerDxe.inf
#!endif
#  BpCommonPkg/Bluetooth/BluetoothHidDxe/BluetoothHidDxe.inf
#  BpCommonPkg/Bluetooth/HidKbDxe/HidKbDxe.inf
#  BpCommonPkg/Bluetooth/HidMouseDxe/HidMouseDxe.inf
#[-end-171121-IB10560319-modify]#
  #
  # Following RC code to override for upgrading UefiCpuPkg
  # SIPO-CNL-0003
  # !disable UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf
  # $(PLATFORM_SI_PACKAGE)/Override/UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
  #   <LibraryClasses>
  #     DebugLib|IntelFrameworkModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  #     ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  # }

  # !disable UefiCpuPkg/CpuDxe/CpuDxe.inf
  # UefiCpuPkg/CpuDxe/CpuDxe.inf {
  #   <SOURCE_OVERRIDE_PATH>
  #     $(CHIPSET_PKG)/Override/UefiCpuPkg/CpuDxe
  # }


