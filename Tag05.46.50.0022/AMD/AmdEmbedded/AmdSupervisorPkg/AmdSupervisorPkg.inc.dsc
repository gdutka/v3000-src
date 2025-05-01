# *******************************************************************************
#
# Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
# Copyright (c) Microsoft Corporation.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# *******************************************************************************

[Defines]

[LibraryClasses]
  SysCallLib|SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf
  SmmSupervisorFeatureLib|SmmSupervisorPkg/Library/SmmSupervisorFeatureLib/SmmSupervisorFeatureLib.inf
  SmmSupvTestPointLib|SmmSupervisorPkg/Library/SmmSupvTestPointLib/SmmSupvTestPointLib.inf
!if $(TARGET) == DEBUG
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  !if $(SMMSUPV_SIMNOW_SUPPORT) == TRUE
    SerialPortLib|AmdCpmPkg/Library/CommonLib/AmdUnifyMessageLib/AmdUnifyMessageLib.inf
  !else
    SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
  !endif
  MemLogLib|AmdSupervisorPkg/Library/MemLogLib/MemLogLib.inf
  ErrorDataLogLib|SmmSupervisorPkg/Library/ErrorDataLogLibNull/ErrorDataLogLibNull.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  MemLogLib|SmmSupervisorPkg/Library/MemLogLibNull/MemLogLibNull.inf
  ErrorDataLogLib|AmdSupervisorPkg/Library/ErrorDataLogLib/ErrorDataLogLib.inf
!endif

[LibraryClasses.X64.DXE_SMM_DRIVER]
  SmmPolicyGateLib|SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
  BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
  CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLibSysCall.inf
  UefiCpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseUefiCpuLibDummy.inf
  TimerLib|AmdSupervisorPkg/Library/AmdTscTimerLib/AmdDxeTscTimerLib.inf
  SmmErrorReportOemLib|SmmSupervisorPkg/Library/SmmErrorReportOemLibNull/SmmErrorReportOemLibNull.inf
  IhvPageSupervisionLib|AmdSupervisorPkg/Library/IhvPageSupervisionLib/IhvPageSupervisionLib.inf
  IhvSmmSaveStateSupervisionLib|AmdSupervisorPkg/Library/IhvSmmSaveStateSupervisionLib/IhvSmmSaveStateSupervisionLib.inf

  ## SmmSupervisorPkg Private Library
  SmmSupervisorExceptionHandlerLib|SmmSupervisorPkg/Private/Library/SmmSupervisorExceptionHandlerLib/SmmSupervisorExceptionHandlerLib.inf
  SmmSupervisorPageMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPageMgmtLib/SmmSupervisorPageMgmtLib.inf
  SmmSupervisorMemMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorMemMgmtLib/SmmSupervisorMemMgmtLib.inf
  SmmSupervisorPolicyMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPolicyMgmtLib/SmmSupervisorPolicyMgmtLib.inf
  SmmSupvSmnLib|AmdSupervisorPkg/Private/Library/SmmSupvSmnLib/SmmSupvSmnLib.inf
  SmmSupvPspMmioLib|AmdSupervisorPkg/Private/Library/SmmSupvPspMmioLib/SmmSupvPspMmioLib.inf

[LibraryClasses.X64.SMM_CORE]
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf

[Components.X64]
!if $(SMMSUPV_SOURCE_BUILD) == TRUE
  SmmSupervisorPkg/SmmSupervisor/SmmSupervisor.inf {
    <LibraryClasses>
      ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchForceResetSystemLib.inf
      BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
      IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
      CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLib.inf
      UefiCpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseUefiCpuLibDummy.inf
      !if $(TARGET) == DEBUG
        DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
        !if $(SMMSUPV_SIMNOW_SUPPORT) == TRUE
          SerialPortLib|AmdCpmPkg/Library/CommonLib/AmdUnifyMessageLib/AmdUnifyMessageLib.inf
        !else
          SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
        !endif
      !else
        DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
        SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
      !endif
      PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      TimerLib|AmdSupervisorPkg/Library/AmdTscTimerLib/AmdDxeTscTimerLib.inf
      S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
      CcExitLib|UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf
      IhvDrtmLib|AmdSupervisorPkg/Library/IhvDrtmLib/IhvDrtmLib.inf
      SmmSupvEntryLocateLib|AmdSupervisorPkg/Library/SmmSupvEntryLocateLib/SmmSupvEntryLocateLib.inf
      IhvPageProtectionLib|AmdSupervisorPkg/Library/IhvPageProtectionLib/IhvPageProtectionLib.inf
  }
!else
  !if $(SMMSUPV_BIN_DEBUG_VERSION) == TRUE
    AmdSupervisorPkg/Binary/SmmSupervisor/Debug/SmmSupervisorBin.inf
  !else
    AmdSupervisorPkg/Binary/SmmSupervisor/Release/SmmSupervisorBin.inf
  !endif
!endif
  SmmSupervisorPkg/SmmSupvErrorReport/SmmSupvErrorReport.inf {
    <LibraryClasses>
      ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchForceResetSystemLib.inf
  }

