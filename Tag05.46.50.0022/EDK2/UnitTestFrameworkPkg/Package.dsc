## @file
# UnitTestFrameworkPkg
#
# Copyright (c) 2019 - 2021, Intel Corporation. All rights reserved.<BR>
# Copyright (c) 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  PLATFORM_NAME           = UnitTestFrameworkPkg
  PLATFORM_GUID           = 7420CC7E-334E-4EFF-B974-A39613455168
  PLATFORM_VERSION        = 1.00
  DSC_SPECIFICATION       = 0x00010005
  OUTPUT_DIRECTORY        = Build/UnitTestFrameworkPkg
  SUPPORTED_ARCHITECTURES = IA32|X64|ARM|AARCH64|RISCV64
  BUILD_TARGETS           = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER        = DEFAULT

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  UnitTestLib|UnitTestFrameworkPkg/Library/UnitTestLib/UnitTestLib.inf
  UnitTestPersistenceLib|UnitTestFrameworkPkg/Library/UnitTestPersistenceLibNull/UnitTestPersistenceLibNull.inf
  UnitTestResultReportLib|UnitTestFrameworkPkg/Library/UnitTestResultReportLib/UnitTestResultReportLibDebugLib.inf

[PcdsPatchableInModule]
!if gInsydeTokenSpaceGuid.PcdH2OUnitTestBuild
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x17
!endif

[Components]
  UnitTestFrameworkPkg/Library/UnitTestLib/UnitTestLib.inf
  UnitTestFrameworkPkg/Library/UnitTestPersistenceLibNull/UnitTestPersistenceLibNull.inf
  UnitTestFrameworkPkg/Library/UnitTestResultReportLib/UnitTestResultReportLibDebugLib.inf
  UnitTestFrameworkPkg/Library/UnitTestBootLibNull/UnitTestBootLibNull.inf
  UnitTestFrameworkPkg/Library/UnitTestResultReportLib/UnitTestResultReportLibConOut.inf
  UnitTestFrameworkPkg/Library/UnitTestBootLibUsbClass/UnitTestBootLibUsbClass.inf
  UnitTestFrameworkPkg/Library/UnitTestPersistenceLibSimpleFileSystem/UnitTestPersistenceLibSimpleFileSystem.inf
  UnitTestFrameworkPkg/Library/UnitTestDebugAssertLib/UnitTestDebugAssertLib.inf

[Components.$(DXE_ARCH)]
!if gInsydeTokenSpaceGuid.PcdH2OUnitTestBuild
  UnitTestFrameworkPkg/Test/UnitTest/Sample/SampleUnitTest/SampleUnitTestDxe.inf {
  <LibraryClasses>
    NULL|UnitTestFrameworkPkg/Library/UnitTestDebugAssertLib/UnitTestDebugAssertLib.inf
  }
  UnitTestFrameworkPkg/Test/UnitTest/Sample/SampleUnitTest/SampleUnitTestSmm.inf {
  <LibraryClasses>
    NULL|UnitTestFrameworkPkg/Library/UnitTestDebugAssertLib/UnitTestDebugAssertLib.inf
  }
  UnitTestFrameworkPkg/Test/UnitTest/Sample/SampleUnitTest/SampleUnitTestUefiShell.inf {
  <LibraryClasses>
    NULL|UnitTestFrameworkPkg/Library/UnitTestDebugAssertLib/UnitTestDebugAssertLib.inf
  }
!endif

[Components.$(PEI_ARCH)]
!if gInsydeTokenSpaceGuid.PcdH2OUnitTestBuild
  UnitTestFrameworkPkg/Test/UnitTest/Sample/SampleUnitTest/SampleUnitTestPei.inf {
  <LibraryClasses>
    NULL|UnitTestFrameworkPkg/Library/UnitTestDebugAssertLib/UnitTestDebugAssertLib.inf
  }
!endif

[BuildOptions]
!if gInsydeTokenSpaceGuid.PcdH2OUnitTestBuild
  MSFT:*_*_*_CC_FLAGS  = -D DISABLE_NEW_DEPRECATED_INTERFACES -D EDKII_UNIT_TEST_FRAMEWORK_ENABLED
  GCC:*_*_*_CC_FLAGS   = -D DISABLE_NEW_DEPRECATED_INTERFACES -D EDKII_UNIT_TEST_FRAMEWORK_ENABLED
  XCODE:*_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES -D EDKII_UNIT_TEST_FRAMEWORK_ENABLED
!endif