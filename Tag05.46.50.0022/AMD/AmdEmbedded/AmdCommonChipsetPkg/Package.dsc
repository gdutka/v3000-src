## @file
#  Platform Package Description file
#
#*******************************************************************************
#* Copyright (c) 2013 - 2021, Insyde Software Corporation. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#*******************************************************************************

[Defines]

[LibraryClasses]
  AmdExtCmosLib|AmdCommonChipsetPkg/Library/AmdExtCmosLibNull/AmdExtCmosLibNull.inf
#[-start-220126-IB14740206-add]#
  PciHostBridgeLib|AgesaModulePkg/Library/DxeAmdPciHostBridgeLib/PciHostBridgeLib.inf
#[-end-220126-IB14740206-add]#
#[-start-221207-IB14740272-add]#
  IoApicLib|PcAtChipsetPkg/Library/BaseIoApicLib/BaseIoApicLib.inf
#[-end-221207-IB14740272-add]#

[LibraryClasses.common.SEC]

[LibraryClasses.common.PEI_CORE]

[LibraryClasses.common.PEIM]
  AmdExtCmosLib|AmdCommonChipsetPkg/Library/AmdExtCmosLibNull/AmdExtCmosLibNull.inf

[LibraryClasses.common.DXE_CORE]

[LibraryClasses.common.DXE_RUNTIME_DRIVER]

[LibraryClasses.common.UEFI_DRIVER]

[LibraryClasses.common.DXE_DRIVER]

[LibraryClasses.common.DXE_SMM_DRIVER]

[LibraryClasses.common.COMBINED_SMM_DXE]

[LibraryClasses.common.SMM_CORE]

[LibraryClasses.common.UEFI_APPLICATION]

[PcdsFeatureFlag]

[PcdsFixedAtBuild]

[PcdsDynamicDefault]

[Components.$(PEI_ARCH)]
  AmdCommonChipsetPkg/SpeakerPei/LegacySpeakerPei.inf

[Components.$(DXE_ARCH)]
  AmdCommonChipsetPkg/SmartTimerDxe/SmartTimerDxe.inf
#[-start-220126-IB14740206-add]#
  AmdCommonChipsetPkg/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <LibraryClasses>
      SmbusLib|MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  }
#[-end-220126-IB14740206-add]#
  AmdCommonChipsetPkg/SpeakerDxe/LegacySpeakerDxe.inf
  AmdCommonChipsetPkg/SetSsidSvidDxe/SetSsidSvidDxe.inf

  AmdCommonChipsetPkg/TcgDmarDxe/TcgDmarDxe.inf
#[-start-221207-IB14740272-add]#
  AmdCommonChipsetPkg/Override/EDK2/PcAtChipsetPkg/HpetTimerDxe/HpetTimerDxe.inf
#[-end-221207-IB14740272-add]#