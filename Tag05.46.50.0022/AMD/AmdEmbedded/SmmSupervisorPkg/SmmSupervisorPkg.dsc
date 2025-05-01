# *******************************************************************************
#
# Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.
# Copyright (c) Microsoft Corporation.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# *******************************************************************************



[Defines]
  PLATFORM_NAME                  = SmmSupervisor
  PLATFORM_GUID                  = B1F262CE-AE71-4ADE-BD16-4465335DDFDC
  PLATFORM_VERSION               = .10
  DSC_SPECIFICATION              = 0x0001001A
  OUTPUT_DIRECTORY               = Build/SmmSupervisorPkg
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses.X64.DXE_SMM_DRIVER]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLib.inf
  UefiCpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseUefiCpuLibDummy.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicLib/BaseXApicLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
!if $(TARGET) == DEBUG
  SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
  ResetSystemLib|PcAtChipsetPkg/Library/ResetSystemLib/ResetSystemLib.inf
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
#[-start-240202-IB20840047-add]#
  CcExitLib|$(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf
#[-end-240202-IB20840047-add]#
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf

  ## SmmSupervisorPkg Private Library
  SmmSupervisorExceptionHandlerLib|SmmSupervisorPkg/Private/Library/SmmSupervisorExceptionHandlerLib/SmmSupervisorExceptionHandlerLib.inf
  SmmSupervisorMemMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorMemMgmtLib/SmmSupervisorMemMgmtLib.inf
  SmmSupervisorPageMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPageMgmtLib/SmmSupervisorPageMgmtLib.inf
  SmmSupervisorPolicyMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPolicyMgmtLib/SmmSupervisorPolicyMgmtLib.inf

  SmmSupvTestPointLib|SmmSupervisorPkg/Library/SmmSupvTestPointLib/SmmSupvTestPointLib.inf
  SysCallLib|SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf
  SmmPolicyGateLib|SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
  SmmSupervisorFeatureLib|SmmSupervisorPkg/Library/SmmSupervisorFeatureLib/SmmSupervisorFeatureLib.inf
  SmmErrorReportOemLib|SmmSupervisorPkg/Library/SmmErrorReportOemLibNull/SmmErrorReportOemLibNull.inf
  IhvDrtmLib|SmmSupervisorPkg/Library/IhvDrtmLibNull/IhvDrtmLibNull.inf
  SmmSupvEntryLocateLib|SmmSupervisorPkg/Library/SmmSupvEntryLocateLibNull/SmmSupvEntryLocateLibNull.inf
  ErrorDataLogLib|SmmSupervisorPkg/Library/ErrorDataLogLibNull/ErrorDataLogLibNull.inf
  MemLogLib|SmmSupervisorPkg/Library/MemLogLibNull/MemLogLibNull.inf
  IhvPageProtectionLib|SmmSupervisorPkg/Library/IhvPageProtectionLibNull/IhvPageProtectionLibNull.inf
  IhvPageSupervisionLib|SmmSupervisorPkg/Library/IhvPageSupervisionLibNull/IhvPageSupervisionLibNull.inf
  IhvSmmSaveStateSupervisionLib|SmmSupervisorPkg/Library/IhvSmmSaveStateSupervisionLibNull/IhvSmmSaveStateSupervisionLibNull.inf
!if $(TARGET) == DEBUG
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  NULL|MdePkg/Library/BaseBinSecurityLibRng/BaseBinSecurityLibRng.inf
  BaseBinSecurityLib|MdePkg/Library/BaseBinSecurityLibRng/BaseBinSecurityLibRng.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  BaseBinSecurityLib|MdePkg/Library/BaseBinSecurityLibNull/BaseBinSecurityLibNull.inf
!endif

[PcdsFeatureFlag]
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnostics2Disable                             | TRUE
  gEfiMdePkgTokenSpaceGuid.PcdComponentName2Disable                                 | TRUE

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress                                 | 0xF0000000
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize                             | 0x3E80

!if $(TARGET) == DEBUG
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel                                  | 0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask                                     | 0x3F
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel                             | 0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue                                 | 0xAF
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask                                     | 0x0
!endif

[Components.X64]
  SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLibSysCall.inf
  SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
  SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf
  SmmSupervisorPkg/Library/IhvDrtmLibNull/IhvDrtmLibNull.inf
  SmmSupervisorPkg/Library/MemLogLibNull/MemLogLibNull.inf
  SmmSupervisorPkg/Library/SmmErrorReportOemLibNull/SmmErrorReportOemLibNull.inf
  SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
  SmmSupervisorPkg/Library/SmmSupervisorFeatureLib/SmmSupervisorFeatureLib.inf
  SmmSupervisorPkg/Library/SmmSupvEntryLocateLibNull/SmmSupvEntryLocateLibNull.inf
  SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf
  SmmSupervisorPkg/Library/ErrorDataLogLibNull/ErrorDataLogLibNull.inf

  ## SmmSupervisorPkg Private Library
  SmmSupervisorPkg/Private/Library/SmmSupervisorExceptionHandlerLib/SmmSupervisorExceptionHandlerLib.inf
  SmmSupervisorPkg/Private/Library/SmmSupervisorMemMgmtLib/SmmSupervisorMemMgmtLib.inf
  SmmSupervisorPkg/Private/Library/SmmSupervisorPageMgmtLib/SmmSupervisorPageMgmtLib.inf
  SmmSupervisorPkg/Private/Library/SmmSupervisorPolicyMgmtLib/SmmSupervisorPolicyMgmtLib.inf

  SmmSupervisorPkg/Library/SmmSupvTestPointLib/SmmSupvTestPointLib.inf
  SmmSupervisorPkg/Library/IhvPageProtectionLibNull/IhvPageProtectionLibNull.inf
  SmmSupervisorPkg/Library/IhvPageSupervisionLibNull/IhvPageSupervisionLibNull.inf
  SmmSupervisorPkg/Library/IhvSmmSaveStateSupervisionLibNull/IhvSmmSaveStateSupervisionLibNull.inf

  SmmSupervisorPkg/SmmSupervisor/SmmSupervisor.inf
  SmmSupervisorPkg/SmmSupvErrorReport/SmmSupvErrorReport.inf

[BuildOptions.common.EDKII.DXE_SMM_DRIVER]
  DEFINE DEBUG_BUILD_OPTIONS = /D EFI_DEBUG /D DEBUG_MODE=1

!if $(TARGET) == DEBUG
  DEFINE TARGET_BUILD_OPTIONS = /D DEBUG_TARGET
!else
  DEFINE TARGET_BUILD_OPTIONS = /D RELEASE_TARGET
!endif

  MSFT:*_*_*_CC_FLAGS  = $(DEBUG_BUILD_OPTIONS) $(TARGET_BUILD_OPTIONS)
  # MSFT:*_*_*_CC_FLAGS  = /FAcs /X /GS /Od

  #DLink flags to cut out project names from binaries
  MSFT:*_*_*_DLINK_FLAGS = /PDB:$(MODULE_NAME).pdb /PDBALTPATH:$(MODULE_NAME).pdb

  #Force modules to 4K alignment
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096

