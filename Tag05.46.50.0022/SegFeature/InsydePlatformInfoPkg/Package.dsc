## @file
# Insyde Platform Information package Platform Description File.
#
#******************************************************************************
#* Copyright (c) 2022 - 2024, Insyde Software Corporation. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
#

[Defines]

[LibraryClasses]
  BinarySearchExLib|InsydePlatformInfoPkg/Library/BinarySearchExLib/BinarySearchExLib.inf
  BlockIoCapacityLib|InsydePlatformInfoPkg/Library/BlockIoCapacityLib/BlockIoCapacityLib.inf
  SataDriveInfoLib|InsydePlatformInfoPkg/Library/SataDriveInfoLib/SataDriveInfoLib.inf
  SmbiosCommonLib|InsydePlatformInfoPkg/Library/SmbiosCommonLib/SmbiosCommonLib.inf
  MemoryDeviceInfoLib|InsydePlatformInfoPkg/Library/MemoryDeviceInfoLib/MemoryDeviceInfoLib.inf
!ifdef INSYDEIPMIPKG_IMPORTED
  BmcInfoLib|InsydePlatformInfoPkg/Library/BmcInfoLib/BmcInfoLib.inf
!else
  BmcInfoLib|InsydePlatformInfoPkg/Library/BmcInfoLibNull/BmcInfoLibNull.inf
!endif
  CpuMiscLib|InsydePlatformInfoPkg/Library/CpuMiscLib/CpuMiscLib.inf
  CpuDeviceInfoLib|InsydePlatformInfoPkg/Library/CpuDeviceInfoLib/CpuDeviceInfoLib.inf
  HiiStringIdManageLib|InsydePlatformInfoPkg/Library/HiiStringIdManageLib/HiiStringIdManageLib.inf
  SDEmmcInfoLib|InsydePlatformInfoPkg/Library/SDEmmcInfoLib/SDEmmcInfoLib.inf
  UsbHandleRoutineLib|InsydePlatformInfoPkg/Library/DxeUsbHandleRoutineLib/DxeUsbHandleRoutineLib.inf
!if gInsydeTokenSpaceGuid.PcdH2OCcbVersion >= 0x05422400
  #
  # Support DeviceInfo2Lib and gH2ODeviceInfo2TokenSpaceGuid.
  # Requires H2O kernel version 5.42.24 or later.
  #
  DeviceInfoLookupDataLib|InsydePlatformInfoPkg/Library/DeviceInfoLookupDataLib/DeviceInfoLookupDataLibDeviceInfo2.inf
!else
  DeviceInfoLookupDataLib|InsydePlatformInfoPkg/Library/DeviceInfoLookupDataLib/DeviceInfoLookupDataLibDeviceInfo.inf
!endif
  NvmeInfoLib|InsydePlatformInfoPkg/Library/NvmeInfoLib/NvmeInfoLib.inf
  LanguageDataLib|InsydePlatformInfoPkg/Library/LanguageDataLib/LanguageDataLib.inf

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpDeviceIdTableSupported == TRUE
  PciDeviceIdTableLib|InsydePlatformInfoPkg/Library/PciDeviceIdTableLib/PciDeviceIdTableLib.inf
!else
  PciDeviceIdTableLib|InsydePlatformInfoPkg/Library/PciDeviceIdTableLibNull/PciDeviceIdTableLibNull.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpVendorIdTableSupported == TRUE
  PciVendorIdTableLib|InsydePlatformInfoPkg/Library/PciVendorIdTableLib/PciVendorIdTableLib.inf
!else
  PciVendorIdTableLib|InsydePlatformInfoPkg/Library/PciVendorIdTableLibNull/PciVendorIdTableLibNull.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpClassCodeTableSupported == TRUE
  PciClassCodeTableLib|InsydePlatformInfoPkg/Library/PciClassCodeTableLib/PciClassCodeTableLib.inf
!else
  PciClassCodeTableLib|InsydePlatformInfoPkg/Library/PciClassCodeTableLibNull/PciClassCodeTableLibNull.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpCapabilityIdTableSupported == TRUE
  PciCapabilityIdTableLib|InsydePlatformInfoPkg/Library/PciCapabilityIdTableLib/PciCapabilityIdTableLib.inf
!else
  PciCapabilityIdTableLib|InsydePlatformInfoPkg/Library/PciCapabilityIdTableLibNull/PciCapabilityIdTableLibNull.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpCapabilityIntnTableSupported == TRUE
  PciCapabilityIntnTableLib|InsydePlatformInfoPkg/Library/PciCapabilityIntnTableLib/PciCapabilityIntnTableLib.inf
!else
  PciCapabilityIntnTableLib|InsydePlatformInfoPkg/Library/PciCapabilityIntnTableLibNull/PciCapabilityIntnTableLibNull.inf
!endif

[PcdsFeatureFlag]
!if gInsydeTokenSpaceGuid.PcdH2OBdsCpDisplayBeforeSupported
!if gInsydeTokenSpaceGuid.PcdH2OBdsCpDisplayStringBeforeSupported
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OConsoleBootMessageSupported|TRUE
!endif
!endif

  #
  # Customized PciLookUpTable Size Pcds
  # NOTE: If needed to contol the lookup table feature, please edit from Define section
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpVendorIdTableSupported|TRUE
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpDeviceIdTableSupported|TRUE
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpClassCodeTableSupported|TRUE
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpCapabilityIdTableSupported|TRUE

  #
  # Customized supported PCI capability interpretation In PCI device information page
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookUpCapabilityIntnTableSupported|TRUE

[PcdsFixedAtBuild]
  #
  #  This pcd will update the current value during the build time
  #
  gInsydeTokenSpaceGuid.PcdH2OCcbVersion|0

  gInsydePlatformInfoTokenSpaceGuid.PcdH2ONvmeInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OMemoryInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OCpuInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OSataDriveInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OUsbViewerLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OSDEmmcInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciDeviceInfoLinkToFormsetGuid|{GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC")}

  #
  #  Setting The Memory Detail Info Form Display Item And Sequence
  #  Note : When Modify Below Number Sequence Can Change Display Sequence Or Remove Number To Hidden Item Display
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OMemoryDetailDisplayList| { \
    UINT8(0),  \  # (0)Device Locator
    UINT8(1),  \  # (1)Bank Locator
    UINT8(2),  \  # (2)Manufacture
    UINT8(3),  \  # (3)Memory Type
    UINT8(4),  \  # (4)DRAM Type
    UINT8(5),  \  # (5)Serial Number
    UINT8(6),  \  # (6)Part Number
    UINT8(7),  \  # (7)Memory Size
    UINT8(8),  \  # (8)Maximum Capable Memory Speed
    UINT8(9),  \  # (9)Configured Memory Speed
    UINT8(10)  \  # (10)Configured Voltage
    }

  #
  #  Setting The CPU Detail Info Form Display Item And Sequence
  #  Note : When Modify Below String Can Change Display Sequence Or Remove String To Hidden Item Display
  #
  #  Warning :
  #    CPU TDP will use Intel MSR/Amd SMU service to collect information.
  #    Since both of these will cause system hang or some other error.
  #    Please comfirm system support related MSR for Intel platform.
  #    Please comfirm related SMU message register which defined at NbioSmuVxxLib.h for Amd platform.
  #    SMU message register define at pcd should be same as NbioSmuVxxLib.h
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OCpuDetailDisplayList| { \
    UINT8(0),  \ # (0)Processor Type
    UINT8(1),  \ # (1)CPUID
    UINT8(2),  \ # (2)CPU Speed
    # UINT8(3),  \ # (3)CPU TDP
    UINT8(4),  \ # (4)CPU Voltage
    UINT8(5),  \ # (5)CPU Stepping
    UINT8(6),  \ # (6)L1 Data Cache (Per Core)
    UINT8(7),  \ # (7)L1 Instruction Cache (Per Core)
    UINT8(8),  \ # (8)L2 Cache (Per Core)
    UINT8(9),  \ # (9)L3 Cache (Per Socket)
    UINT8(10), \ # (10)Number Of Processors
    UINT8(11)  \ # (11)Microcode Patch Leve
    }

  #
  #  Setting The SD/eMMC Detail Info Form Display Item And Sequence
  #  Note : When Reorder or Remove Below Instance Field Can Reorder or Hidden Display Item.
  #         The Instance Order follow the mSDEmmcInfoParserTable Contents in SDEmmcInfoSetupUtilityDxe.
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OSDEmmcDetailDisplayList| { \
    UINT8(0x00), \  # Product Name
    UINT8(0x01), \  # Device Serial Number
    UINT8(0x02), \  # Device Revision
    UINT8(0x03), \  # OEM/Application ID
    UINT8(0x04), \  # Manufacture ID
    UINT8(0x05), \  # Manufacture Date
    UINT8(0x06), \  # Card Type
    UINT8(0x07), \  # Device Capacity
    UINT8(0x08), \  # Block Count
    UINT8(0x09)  \  # Block Size
    }

  #
  #  Setting The NVMe Detail Info Form Display Item and Sequence
  #  Note : Items from the same section should be listed together.
  #
  #----------------------------------------------------------------------
  # Section Number                    | Item Number                     |
  #----------------------------------------------------------------------
  # 0x00 = Controller Information     | 0xXX = Specific Section Item(s) |
  # 0x01 = Namespace Information      |                                 |
  # 0x02 = SMART / Health Information |                                 |
  #----------------------------------------------------------------------
  gInsydePlatformInfoTokenSpaceGuid.PcdH2ONvmeDetailDisplayList|{ \
    # Controller Information Section
    UINT8(0x00),                        UINT8(0x00), \ # Controller Info - EFI Device Path
    UINT8(0x00),                        UINT8(0x01), \ # Controller Info - PCI Location
    UINT8(0x00),                        UINT8(0x02), \ # Controller Info - Total Namespaces
    UINT8(0x00),                        UINT8(0x03), \ # Controller Info - Serial Number
    UINT8(0x00),                        UINT8(0x04), \ # Controller Info - Model Number
    UINT8(0x00),                        UINT8(0x05), \ # Controller Info - Firmware Revision
    UINT8(0x00),                        UINT8(0x06), \ # Controller Info - IEEE OUI
    # Namespace Information Section
    UINT8(0x01),                        UINT8(0x00), \ # Namespace Info - Namespace ID
    UINT8(0x01),                        UINT8(0x01), \ # Namespace Info - IEEE EUI-64
    UINT8(0x01),                        UINT8(0x02), \ # Namespace Info - Storage Capacity
    # SMART / Health Information Section
    UINT8(0x02),                        UINT8(0x00)  \ # SMART Info - All Section Items
  }
  
  #
  # Set the name of the Device Info attribute as standard value "name". It can
  # be configured to other values, such as "display_name". 
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2ODeviceInfoAttributeStr|"name"

  #
  # Users are not required to modify this PCD.
  #
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OPlatformInfoDummyStrToken|{STRING_TOKEN(STR_BLANK_STRING)}

  gInsydePlatformInfoTokenSpaceGuid.PcdH2OConsoleBootMessageMarginRight|0
  gInsydePlatformInfoTokenSpaceGuid.PcdH2OConsoleBootMessageMarginBottom|2

[Components.$(PEI_ARCH)]

[Components.$(DXE_ARCH)]
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPlatformInfoSupported

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OMemoryDeviceInfoSupported
  InsydePlatformInfoPkg/MemoryDeviceInfoDxe/MemoryDeviceInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupMemoryInfoSupported
  InsydePlatformInfoPkg/MemoryInfoSetupUtilityDxe/MemoryInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2ONvmeInfoSupported
  InsydePlatformInfoPkg/NvmeInfoDxe/NvmeInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupNvmeInfoSupported
  InsydePlatformInfoPkg/NvmeInfoSetupUtilityDxe/NvmeInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookupTableSupported
  InsydePlatformInfoPkg/PciLookupDxe/PciLookupDxe.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciDeviceInfoSupported
  InsydePlatformInfoPkg/PciDeviceInfoDxe/PciDeviceInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupPciDeviceInfoSupported && gInsydePlatformInfoTokenSpaceGuid.PcdH2OPciLookupTableSupported
  InsydePlatformInfoPkg/PciDeviceInfoSetupUtilityDxe/PciDeviceInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OCpuDeviceInfoSupported
  InsydePlatformInfoPkg/CpuDeviceInfoDxe/CpuDeviceInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupCpuInfoSupported
  InsydePlatformInfoPkg/CpuInfoSetupUtilityDxe/CpuInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSataDriveInfoSupported
  InsydePlatformInfoPkg/SataDriveInfoDxe/SataDriveInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupSataDriveInfoSupported
  InsydePlatformInfoPkg/SataDriveInfoSetupUtilityDxe/SataDriveInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OUsbTextLookupSupported
  InsydePlatformInfoPkg/UsbTextLookupDxe/UsbTextLookupDxe.inf
!endif
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupUsbViewerSupported
  InsydePlatformInfoPkg/UsbViewerDxe/UsbViewerDxe.inf
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSDEmmcInfoSupported
  InsydePlatformInfoPkg/SDEmmcInfoDxe/SDEmmcInfoDxe.inf
!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OSetupSDEmmcInfoSupported
  InsydePlatformInfoPkg/SDEmmcInfoSetupUtilityDxe/SDEmmcInfoSetupUtilityDxe.inf
!endif
!endif

!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OConsoleBootMessageSupported
  InsydePlatformInfoPkg/ConsoleBootMsgDxe/ConsoleBootMsgDxe.inf
  InsydePlatformInfoPkg/PlatformInfoBootMsgDxe/PlatformInfoBootMsgDxe.inf
!endif

  InsydePlatformInfoPkg/DeviceInfoLookupDxe/DeviceInfoLookupDxe.inf

!endif

[BuildOptions]
!ifdef INSYDEIPMIPKG_IMPORTED
  *_*_*_CC_FLAGS = -D INSYDEIPMIPKG_IMPORTED
!endif
!if gInsydeTokenSpaceGuid.PcdH2OCcbVersion < 0x05433000 OR gInsydeTokenSpaceGuid.PcdH2OCcbVersion >= 0x05502100 AND gInsydeTokenSpaceGuid.PcdH2OCcbVersion < 0x05513000
  #
  # The macro OLD_VFR_PCD_GUID_EXPANSION enables compatibility with H2O Kernel
  # 05.43.29 and earlier (specifically, without IB19060011).
  #
  *_*_*_VFRPP_FLAGS = -D OLD_VFR_PCD_GUID_EXPANSION
!endif
