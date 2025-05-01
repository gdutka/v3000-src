## @file
# Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2015 - 2018, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
#

[Defines]
!include InsydeDebugAssistPkg/Package.env

[LibraryClasses]

[LibraryClasses.IA32, LibraryClasses.X64]


#[LibraryClasses.common.SEC]

#[LibraryClasses.common.PEI_CORE]

#[LibraryClasses.common.PEIM]

[LibraryClasses.common.DXE_CORE]
  MemoryDataAnalysisLib|InsydeDebugAssistPkg/Library/MemoryDataAnalysisLib/MemoryDataAnalysisLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]

[LibraryClasses.common.UEFI_DRIVER]

[LibraryClasses.common.DXE_DRIVER]
  MemoryDataAnalysisLib|InsydeDebugAssistPkg/Library/MemoryDataAnalysisLib/MemoryDataAnalysisLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  MemoryDataAnalysisLib|InsydeDebugAssistPkg/Library/MemoryDataAnalysisLib/MemoryDataAnalysisLib.inf

[LibraryClasses.common.COMBINED_SMM_DXE]

[LibraryClasses.common.SMM_CORE]
  MemoryDataAnalysisLib|InsydeDebugAssistPkg/Library/MemoryDataAnalysisLib/MemoryDataAnalysisLib.inf

[LibraryClasses.common.UEFI_APPLICATION]

[PcdsFeatureFlag]


[PcdsFixedAtBuild]

[PcdsDynamicDefault]

[PcdsDynamicExDefault]

[Components.IA32]

[Components.X64]
!if gH2ODebugAssistTokenSpaceGuid.PcdH2ODebugAssistEventTraceSupported
  InsydeDebugAssistPkg/DriverArpServiceDxe/DriverArpServiceDxe.inf
!endif

!if gH2ODebugAssistTokenSpaceGuid.PcdH2ODebugAssistEventTraceSupported
  InsydeDebugAssistPkg/EventTraceDxe/EventTraceDxe.inf
!endif
!if gH2ODebugAssistTokenSpaceGuid.PcdH2ODebugAssistMemoryDataAnalysisSupported
  InsydeDebugAssistPkg/MemoryDataAnalysisDxe/MemoryDataAnalysisDxe.inf
!endif