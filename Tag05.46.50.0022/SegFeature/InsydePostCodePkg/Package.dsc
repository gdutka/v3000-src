## @file
#  PostCode package project build description file.
#
#******************************************************************************
#* Copyright (c) 2016 - 2019, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]

[LibraryClasses]
  PostCodeConfigLib|InsydePostCodePkg/Library/PostCodeConfigLib/PostCodeConfigLib.inf
  SerialPortPostCodeLib|InsydePostCodePkg/Library/SerialPortPostCodeLib/SerialPortPostCodeLib.inf
  VgaPostCodeLib|InsydePostCodePkg/Library/VgaPostCodeLib/VgaPostCodeLib.inf
  PostCodeMiscLib|InsydePostCodePkg/Library/PostCodeMiscLib/PostCodeMiscLib.inf

[LibraryClasses.common.SMM_CORE]
!if gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSupported == 1 && $(INSYDE_DEBUGGER) == 0
  SmmDebugRegisterConfigLib|InsydePostCodePkg/Library/SmmDebugRegisterConfigLib/SmmDebugRegisterConfigLib.inf
  SmmCorePlatformHookLib|InsydePostCodePkg/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf
!endif

[PcdsFixedAtBuild]
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortRegBase|0x03F8
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortUseMmio|FALSE
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeTrapDebugPortNotifyList|{ \
    GUID("ee16160a-e8be-47a6-820a-c6900db0250a"), \ # gEfiPeiMpServicesPpiGuid
    GUID("a0435fa4-2387-4b31-8679-98c932c7dbbb"), \ # gH2OCpuArchPpiGuid
    GUID("26BACCB1-6F42-11D4-BCE7-0080C73C8881"), \ # gEfiCpuArchProtocolGuid
    GUID("4EB6E09C-D256-4E1E-B50A-874BD284B3DE"), \ # gFspSiliconInitDonePpiGuid
    GUID("00000000-0000-0000-0000-000000000000")}   # End of List
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeNonTrapDebugPortNotifyList|{ \
    GUID("605EA650-C65C-42e1-BA80-91A52AB618C6"), \ # gEfiEndOfPeiSignalPpiGuid
    GUID("6e057ecf-fa99-4f39-95bc-59f9921d17e4"), \ # gEdkiiSmmReadyToBootProtocolGuid
    GUID("AEBFFA01-7EDC-49FF-8D88-CB848C5E8670"), \ # gSiPolicyPpiGuid
    GUID("00000000-0000-0000-0000-000000000000")}   # End of List

  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeTrapDebugPortCpNotifyList|{ \
#    GUID("5b02c9e2-7ce3-403a-997c-ca10d866507d"), \ # gH2OBdsCpEndOfDxeAfterGuid  
    GUID("00000000-0000-0000-0000-000000000000")}   # End of List

  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeNonTrapDebugPortCpNotifyList|{ \
#    GUID("6adc6c22-72e1-410b-a9f2-9ac6ede9fe5e"), \ # gH2OBdsCpEndOfDxeBeforeGuid  
    GUID("00000000-0000-0000-0000-000000000000")}   # End of List


  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeDebugPortAddress|0x0080
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeDisplayLength|0x01
  
[PcdsFeatureFlag]
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSupported|TRUE
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSerialPortSupported|TRUE
  gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeScreenSupported|TRUE

[Components.IA32]
!if gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSupported == 1 && $(INSYDE_DEBUGGER) == 0
  InsydePostCodePkg/PostCodePei/PostCodePei.inf
!endif

[Components.X64]
!if gH2OPostCodePkgTokenSpaceGuid.PcdH2OPostCodeSupported == 1 && $(INSYDE_DEBUGGER) == 0
  InsydePostCodePkg/PostCodeDxe/PostCodeDxe.inf
  InsydePostCodePkg/PostCodeSmm/PostCodeSmm.inf
!endif 
