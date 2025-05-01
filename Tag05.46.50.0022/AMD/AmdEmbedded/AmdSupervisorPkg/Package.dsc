## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]

[LibraryClasses]
  SysCallLib|SmmSupervisorPkg/Library/SysCallLib/SysCallLib.inf
  SmmSupervisorFeatureLib|SmmSupervisorPkg/Library/SmmSupervisorFeatureLib/SmmSupervisorFeatureLib.inf
  SmmSupvTestPointLib|SmmSupervisorPkg/Library/SmmSupvTestPointLib/SmmSupvTestPointLib.inf
!if $(TARGET) == DEBUG
  # DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  !if $(SMMSUPV_SIMNOW_SUPPORT) == TRUE
    SerialPortLib|AmdCpmPkg/Library/CommonLib/AmdUnifyMessageLib/AmdUnifyMessageLib.inf
  !else
    SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
  !endif
  MemLogLib|AmdSupervisorPkg/Library/MemLogLib/MemLogLib.inf
  ErrorDataLogLib|AmdSupervisorPkg/Library/ErrorDataLogLibDebug/ErrorDataLogLibDebug.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  MemLogLib|SmmSupervisorPkg/Library/MemLogLibNull/MemLogLibNull.inf
  ErrorDataLogLib|AmdSupervisorPkg/Library/ErrorDataLogLib/ErrorDataLogLib.inf
!endif

[LibraryClasses.Common.DXE_SMM_DRIVER]
  SmmPolicyGateLib|SmmSupervisorPkg/Library/SmmPolicyGateLib/SmmPolicyGateLib.inf
!if $(SMM_ISOLATION_SUPPORT) == YES
  BaseLib|SmmSupervisorPkg/Library/BaseLibSysCall/BaseLib.inf
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
  CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLibSysCall.inf
  UefiCpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseUefiCpuLibDummy.inf
  TimerLib|AmdSupervisorPkg/Library/AmdTscTimerLib/AmdDxeTscTimerLib.inf
  SmmErrorReportOemLib|SmmSupervisorPkg/Library/SmmErrorReportOemLibNull/SmmErrorReportOemLibNull.inf
  IhvPageSupervisionLib|AmdSupervisorPkg/Library/IhvPageSupervisionLib/IhvPageSupervisionLib.inf
  IhvSmmSaveStateSupervisionLib|AmdSupervisorPkg/Library/IhvSmmSaveStateSupervisionLib/IhvSmmSaveStateSupervisionLib.inf
!endif
  ## SmmSupervisorPkg Private Library
  SmmSupervisorExceptionHandlerLib|SmmSupervisorPkg/Private/Library/SmmSupervisorExceptionHandlerLib/SmmSupervisorExceptionHandlerLib.inf
  SmmSupervisorPageMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPageMgmtLib/SmmSupervisorPageMgmtLib.inf
  SmmSupervisorMemMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorMemMgmtLib/SmmSupervisorMemMgmtLib.inf
  SmmSupervisorPolicyMgmtLib|SmmSupervisorPkg/Private/Library/SmmSupervisorPolicyMgmtLib/SmmSupervisorPolicyMgmtLib.inf
  SmmSupvSmnLib|AmdSupervisorPkg/Private/Library/SmmSupvSmnLib/SmmSupvSmnLib.inf
  SmmSupvPspMmioLib|AmdSupervisorPkg/Private/Library/SmmSupvPspMmioLib/SmmSupvPspMmioLib.inf

[LibraryClasses.common.SMM_CORE]
!if $(SMM_ISOLATION_SUPPORT) == YES
  IoLib|SmmSupervisorPkg/Library/BaseIoLibIntrinsicSysCall/BaseIoLibIntrinsic.inf
!endif

[Components.$(DXE_ARCH)]
!if $(SMM_ISOLATION_SUPPORT) == YES
!if $(SMMSUPV_SOURCE_BUILD) == YES
  SmmSupervisorPkg/SmmSupervisor/SmmSupervisor.inf {
    <LibraryClasses>
      ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchForceResetSystemLib.inf
      BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
      IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
      CpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseCpuLib.inf
      UefiCpuLib|SmmSupervisorPkg/Library/BaseCpuLib/BaseUefiCpuLibDummy.inf
!if $(TARGET) == DEBUG
      DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
!else
   DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
   SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif
      PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      TimerLib|AmdSupervisorPkg/Library/AmdTscTimerLib/AmdDxeTscTimerLib.inf
      S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
#[-start-240202-IB20840047-add]#
      CcExitLib|$(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf
#[-end-240202-IB20840047-add]#
      IhvDrtmLib|AmdSupervisorPkg/Library/IhvDrtmLib/IhvDrtmLib.inf
      SmmSupvEntryLocateLib|AmdSupervisorPkg/Library/SmmSupvEntryLocateLib/SmmSupvEntryLocateLib.inf
      IhvPageProtectionLib|AmdSupervisorPkg/Library/IhvPageProtectionLib/IhvPageProtectionLib.inf
  }
!else
!if $(TARGET) == DEBUG
  AmdSupervisorPkg/Binary/SmmSupervisor/Debug/SmmSupervisorBin.inf
!else
  AmdSupervisorPkg/Binary/SmmSupervisor/Release/SmmSupervisorBin.inf
!endif
!endif
  SmmSupervisorPkg/SmmSupvErrorReport/SmmSupvErrorReport.inf {
    <LibraryClasses>
      ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchForceResetSystemLib.inf
!if $(SMM_PROTECTION_TEST_SUPPORT) == TRUE
      SmmErrorReportOemLib|AgesaModulePkg/Universal/AmdTestPkg/Library/SmmErrorReportOemLib/SmmErrorReportOemLib.inf
!endif
  }
!endif