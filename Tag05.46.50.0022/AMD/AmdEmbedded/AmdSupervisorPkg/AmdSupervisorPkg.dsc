# *******************************************************************************
#
# Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  SMMSUPV_SIMNOW_SUPPORT         = FALSE   #Must set to false in formal release

  !include MdePkg/MdeLibs.dsc.inc
[LibraryClasses.X64.DXE_SMM_DRIVER]
  AmdBaseLib|AgesaModulePkg/Library/AmdBaseLib/AmdBaseLib.inf
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf
  AmdIdsDebugPrintLib|AgesaModulePkg/Library/AmdIdsDebugPrintLib/AmdIdsDebugPrintLib.inf
  AmdPspBaseLibV2|AgesaModulePkg/Library/AmdPspBaseLibV2/AmdPspBaseLibV2.inf
  AmdPspMboxLibV2|AgesaModulePkg/Library/AmdPspMboxLibV2/AmdPspMboxLibV2.inf
  AmdPspRegBaseLib|AgesaModulePkg/Library/AmdPspRegBaseLib/AmdPspRegBaseLib.inf
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  AmdSocBaseLib|AgesaModulePkg/Library/AmdSocBaseLib/AmdSocBaseLib.inf
  CcxBaseX86Lib|AgesaModulePkg/Library/CcxBaseX86Lib/CcxBaseX86Lib.inf
  CcxRolesLib|AgesaModulePkg/Library/CcxRolesX86Lib/CcxRolesX86Lib.inf
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerLibNull/FabricResourceManagerLibNull.inf
  FchBaseLib|AgesaModulePkg/Library/FchBaseLib/FchBaseLib.inf
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom2Lib.inf
  IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  IdsMiscLib|AgesaModulePkg/Library/IdsMiscLib/IdsMiscLib.inf
  ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchForceResetSystemLib.inf

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
  !if $(SMMSUPV_SIMNOW_SUPPORT) == TRUE
    SerialPortLib|AmdCpmPkg/Library/CommonLib/AmdUnifyMessageLib/AmdUnifyMessageLib.inf
  !else
    SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
  !endif
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  CcExitLib|UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf

  ## SmmSupervisorPkg Private Library
  SmmSupervisorExceptionHandlerLib|SmmSupervisorPkg/Private/Library/SmmSupervisorExceptionHandlerLib/SmmSupervisorExceptionHandlerLib.inf
  SmmSupervisorMemMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorMemMgmtLib/SmmSupervisorMemMgmtLib.inf
  SmmSupervisorPageMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPageMgmtLib/SmmSupervisorPageMgmtLib.inf
  SmmSupervisorPolicyMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPolicyMgmtLib/SmmSupervisorPolicyMgmtLib.inf
  SmmSupvSmnLib|AmdSupervisorPkg/Private/Library/SmmSupvSmnLib/SmmSupvSmnLib.inf
  SmmSupvPspMmioLib|AmdSupervisorPkg/Private/Library/SmmSupvPspMmioLib/SmmSupvPspMmioLib.inf

  SmmSupvTestPointLib|SmmSupervisorPkg/Library/SmmSupvTestPointLib/SmmSupvTestPointLib.inf
  SysCallLib|SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf
  SmmPolicyGateLib|SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
  TimerLib|AmdSupervisorPkg/Library/AmdTscTimerLib/AmdDxeTscTimerLib.inf
  SmmSupervisorFeatureLib|SmmSupervisorPkg/Library/SmmSupervisorFeatureLib/SmmSupervisorFeatureLib.inf
  IhvDrtmLib|AmdSupervisorPkg/Library/IhvDrtmLib/IhvDrtmLib.inf
  SmmSupvEntryLocateLib|AmdSupervisorPkg/Library/SmmSupvEntryLocateLib/SmmSupvEntryLocateLib.inf
  IhvPageProtectionLib|AmdSupervisorPkg/Library/IhvPageProtectionLib/IhvPageProtectionLib.inf
  IhvPageSupervisionLib|AmdSupervisorPkg/Library/IhvPageSupervisionLib/IhvPageSupervisionLib.inf
  IhvSmmSaveStateSupervisionLib|AmdSupervisorPkg/Library/IhvSmmSaveStateSupervisionLib/IhvSmmSaveStateSupervisionLib.inf

  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf #FIXME need to find a way to remove this depx from release build
!if $(TARGET) == DEBUG
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  MemLogLib|AmdSupervisorPkg/Library/MemLogLib/MemLogLib.inf
  ErrorDataLogLib|AmdSupervisorPkg/Library/ErrorDataLogLibDebug/ErrorDataLogLibDebug.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  MemLogLib|SmmSupervisorPkg/Library/MemLogLibNull/MemLogLibNull.inf
  ErrorDataLogLib|AmdSupervisorPkg/Library/ErrorDataLogLib/ErrorDataLogLib.inf
!endif
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf
  AmdDirectoryBaseLib|AgesaModulePkg/Library/AmdDirectoryBaseLib/AmdDirectoryBaseLib.inf

  SmnAccessLib|AgesaModulePkg/Library/SmnAccessLib/SmnAccessLib.inf
  #SmnAccessLib depx Start

  GnbCommonLib|AgesaModulePkg/Library/GnbCommonLib/GnbCommonLib.inf
  GnbMemAccLib|AgesaModulePkg/Library/GnbMemAccLib/GnbMemAccLib.inf
  GnbIoAccLib|AgesaModulePkg/Library/GnbIoAccLib/GnbIoAccLib.inf
  GnbPciAccLib|AgesaModulePkg/Library/GnbPciAccLib/GnbPciAccLib.inf
  GnbCpuAccLib|AgesaModulePkg/Library/GnbCpuAccLib/GnbCpuAccLib.inf
  GnbPciLib|AgesaModulePkg/Library/GnbPciLib/GnbPciLib.inf
  GnbLib|AgesaModulePkg/Library/GnbLib/GnbLib.inf
  #SmnAccessLib depx End
  AmdStbLib|AgesaModulePkg/Library/AmdStbLibNull/AmdStbLibNull.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibNull/AmdCfgPcdBufLibNull.inf
  AmdPostCodeLib|AgesaModulePkg/Library/AmdPostCodeLib/AmdPostCodeLib.inf
# FIXME+ - end

[PcdsFeatureFlag]
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnostics2Disable                             | TRUE
  gEfiMdePkgTokenSpaceGuid.PcdComponentName2Disable                                 | TRUE

[PcdsFixedAtBuild]
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAgesaAssertEnable                                | FALSE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdBottomMmioReservedForPrimaryRb                | 0xFEC00000
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintFilter                           | 0x100401008A300408
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortDetectCableConnection  | FALSE

  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress                                 | 0xF0000000
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize                             | 0x3E80

!if $(TARGET) == DEBUG
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintEnable                           | TRUE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortEnable                 | TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel                                  | 0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask                                     | 0x3F
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel                             | 0x80080246
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue                                 | 0xAF
  !if $(SMMSUPV_SIMNOW_SUPPORT) == TRUE
    # Enable debug message (BIT[x] = 0 means disable serial out)
    # Bit[0]: 1 - enable ESPI UART serail out, including UDC(unversial debug card) or EC ESIP UART
    # Bit[1]: 1 - enable LPC UART serail out
    # Bit[2]: 1 - enable port80 redirect serail out
    # Bit[3]: 1 - enable FCH UART0 serail out
    # Bit[4]: 1 - enable FCH UART1 serail out
    # Bit[5]: 1 - enable FCH UART2 serail out
    # Bit[6]: 1 - enable FCH UART3 serail out
    # Bit[7]: 1 - enable FCH UART4 serail out
    gAmdCpmPkgTokenSpaceGuid.PcdAmdUnifyMessage|0x04
  !endif
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask                                     | 0x0
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintEnable                           | FALSE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdIdsDebugPrintSerialPortEnable                 | FALSE
!endif

[PcdsDynamicDefault]
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdMmioAbove4GLimit                              | 0x0FFFFFFFFFF

[Components.X64]
  SmmSupervisorPkg/SmmSupervisor/SmmSupervisor.inf

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

