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

[Components.$(PEI_ARCH)]
  AmdCbsPkg/CbsBasePei/CbsBasePeiRMB.inf {
    <LibraryClasses>
       CbsPeiFuncLib|AmdCbsPkg/Library/Family/0x19/RMB/External/CbsFuncLibPei.inf
       CbsSetAgesaPcdLib|AmdCbsPkg/Build/ResourceRMB/CbsSetAgesaPcdLibRMB.inf
  }

[Components.$(DXE_ARCH)]
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

