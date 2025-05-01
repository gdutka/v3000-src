## @file
# MdeModulePkg DSC file used to build host-based unit tests.
#
# Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
# Copyright (C) Microsoft Corporation.
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  PLATFORM_NAME           = MdeModulePkgHostTest
  PLATFORM_GUID           = F74AF7C6-698C-4EBA-BA49-FF6816916354
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x00010005
  OUTPUT_DIRECTORY        = Build/MdeModulePkg/HostTest
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = NOOPT
  SKUID_IDENTIFIER        = DEFAULT

!include UnitTestFrameworkPkg/Override/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  ResetSystemLib|MdeModulePkg/Library/DxeResetSystemLib/DxeResetSystemLib.inf {
    <SOURCE_OVERRIDE_PATH>
    MdeModulePkg/Override/Library/DxeResetSystemLib
  }
  VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLib.inf {
    <SOURCE_OVERRIDE_PATH>
     MdeModulePkg/Override/Library/VariablePolicyLib
  }

[Components]
  MdeModulePkg/Library/DxeResetSystemLib/UnitTest/MockUefiRuntimeServicesTableLib.inf

  #
  # Build MdeModulePkg HOST_APPLICATION Tests
  #
  MdeModulePkg/Library/DxeResetSystemLib/UnitTest/DxeResetSystemLibUnitTestHost.inf {
    <LibraryClasses>
      UefiRuntimeServicesTableLib|MdeModulePkg/Library/DxeResetSystemLib/UnitTest/MockUefiRuntimeServicesTableLib.inf
  }

  MdeModulePkg/Universal/Variable/RuntimeDxe/RuntimeDxeUnitTest/VariableLockRequestToLockUnitTest.inf {
    <LibraryClasses>
      VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdAllowVariablePolicyEnforcementDisable|TRUE
  }

  MdeModulePkg/Library/UefiSortLib/UnitTest/UefiSortLibUnitTest.inf {
    <LibraryClasses>
      UefiSortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  }
