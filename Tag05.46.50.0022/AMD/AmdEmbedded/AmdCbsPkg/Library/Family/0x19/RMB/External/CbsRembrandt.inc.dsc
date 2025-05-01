#;*****************************************************************************
#;
#; Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
#;
#;******************************************************************************

[Defines]

[LibraryClasses]

[LibraryClasses.Common.PEIM]
  CbsPeiFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibPei.inf
  AmdIdsHookExtLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsIdsLibPei.inf
  CbsSetAgesaPcdLib|AmdCbsPkg/Build/ResourceRMB/CbsSetAgesaPcdLibRMB.inf

[LibraryClasses.Common.DXE_DRIVER]
  CbsSetupLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsSetupLib.inf
  CbsDxeFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibDxe.inf
  AmdIdsHookExtLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsIdsLibDxe.inf

[LibraryClasses.Common.DXE_SMM_DRIVER]
  CbsBctSmmLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsBctSmmLib.inf

[Components.IA32]
  AmdCbsPkg/CbsBasePei/CbsBasePeiRMB.inf {
    <LibraryClasses>
       CbsPeiFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibPei.inf
       CbsSetAgesaPcdLib|AmdCbsPkg/Build/ResourceRMB/CbsSetAgesaPcdLibRMB.inf
  }

[Components.X64]
  AmdCbsPkg/BctBaseSmm/BctBaseSmmRMB.inf {
    <LibraryClasses>
       CbsBctSmmLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsBctSmmLib.inf
  }
  AmdCbsPkg/CbsSetupDxe/CbsSetupDxeRMB.inf {
    <LibraryClasses>
       CbsSetupLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsSetupLib.inf
       CbsDxeFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibDxe.inf
  }
  AmdCbsPkg/CbsBaseDxe/CbsBaseDxeRMB.inf {
    <LibraryClasses>
       CbsDxeFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibDxe.inf
  }

