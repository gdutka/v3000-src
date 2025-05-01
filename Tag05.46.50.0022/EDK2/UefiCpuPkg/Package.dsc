## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]

[LibraryClasses]
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
  SmmCpuPlatformHookLib|UefiCpuPkg/Library/SmmCpuPlatformHookLibNull/SmmCpuPlatformHookLibNull.inf
  SmmCpuFeaturesLib|UefiCpuPkg/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf
  VmgExitLib|UefiCpuPkg/Library/VmgExitLibNull/VmgExitLibNull.inf
  MicrocodeLib|UefiCpuPkg/Library/MicrocodeLib/MicrocodeLib.inf

[LibraryClasses.common.SEC]
  PlatformSecLib|UefiCpuPkg/Library/PlatformSecLibNull/PlatformSecLibNull.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf {
    <SOURCE_OVERRIDE_PATH>
      UefiCpuPkg/Override/Library/CpuExceptionHandlerLib
  }

[LibraryClasses.common.PEI_CORE]

[LibraryClasses.common.PEIM]
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  RegisterCpuFeaturesLib|UefiCpuPkg/Library/RegisterCpuFeaturesLib/PeiRegisterCpuFeaturesLib.inf
  CpuCacheInfoLib|UefiCpuPkg/Library/CpuCacheInfoLib/PeiCpuCacheInfoLib.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/PeiCpuExceptionHandlerLib.inf {
    <SOURCE_OVERRIDE_PATH>
      UefiCpuPkg/Override/Library/CpuExceptionHandlerLib
  }


[LibraryClasses.common.DXE_CORE]

[LibraryClasses.common.DXE_RUNTIME_DRIVER]

[LibraryClasses.common.UEFI_DRIVER]

[LibraryClasses.common.DXE_DRIVER]
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf {
    <SOURCE_OVERRIDE_PATH>
      UefiCpuPkg/Override/Library/CpuExceptionHandlerLib
  }
  MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf
  RegisterCpuFeaturesLib|UefiCpuPkg/Library/RegisterCpuFeaturesLib/DxeRegisterCpuFeaturesLib.inf
  CpuCacheInfoLib|UefiCpuPkg/Library/CpuCacheInfoLib/DxeCpuCacheInfoLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf {
    <SOURCE_OVERRIDE_PATH>
      UefiCpuPkg/Override/Library/CpuExceptionHandlerLib
  }

[LibraryClasses.common.COMBINED_SMM_DXE]

[LibraryClasses.common.SMM_CORE]

[LibraryClasses.common.UEFI_APPLICATION]

[PcdsFeatureFlag]

[PcdsFixedAtBuild]

[PcdsDynamicExDefault]

[Components.$(PEI_ARCH)]
  UefiCpuPkg/SecMigrationPei/SecMigrationPei.inf
  UefiCpuPkg/CpuIoPei/CpuIoPei.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf

[Components.$(DXE_ARCH)]
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf
  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf
!if gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiS3Enable
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf
!endif
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf
  #UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
  #      <SOURCE_OVERRIDE_PATH>
  #          UefiCpuPkg/Override/PiSmmCpuDxeSmm
  #}
