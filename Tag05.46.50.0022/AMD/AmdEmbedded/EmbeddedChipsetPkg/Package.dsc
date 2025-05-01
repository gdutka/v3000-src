## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2019 - 2021, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

[Defines]
  DEFINE   AGESA_PKG_PATH   = AgesaModulePkg
  DEFINE   AGESA_PKG_DEC    = AgesaFp7RmbModulePkg
  DEFINE   FCH_PKG_PATH     = $(AGESA_PKG_PATH)/Fch/Keith
  DEFINE   FCH_PKG_DEC      = FchKeith
  DEFINE   CHIPSET_PKG      = EmbeddedChipsetPkg
!include $(CHIPSET_PKG)/Package.env

[Packages]
  IntelFrameworkPkg/IntelFrameworkPkg.dec|InsydeModulePkg/InsydeModulePkg.dec

[LibraryClasses]
  PostCodeLib|$(CHIPSET_PKG)/Library/BasePostCodeLib/BasePostCodeLib.inf
  PlatformHookLib|AmdCpmPkg/Library/CommonLib/BasePlatformHookLibAmdSmsc1100/BasePlatformHookLibAmdSmsc1100.inf
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/BaseTscTimerLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/Library/BaseXApicX2ApicLib
  }
  PlatformBdsLib|$(CHIPSET_PKG)/Library/PlatformBdsLib/PlatformBdsLib.inf
  SpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/SpiAccessLib/SpiAccessLib.inf
#[-start-220208-IB14740211-add]#
  FchMmioSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/FchMmioSpiAccessLib/FchMmioSpiAccessLib.inf
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmorSpiAccessLibNull/RomArmorSpiAccessLibNull.inf
#[-end-220208-IB14740211-add]#
  FlashWriteEnableLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/FlashWriteEnableLib/FlashWriteEnableLib.inf
  SpiAccessInitLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/DxeSpiAccessInitLib/DxeSpiAccessInitLib.inf
  AmdExtCmosLib|$(CHIPSET_PKG)/Library/AmdExtCmosLib/AmdExtCmosLib.inf
  FchInitHookLib|$(CHIPSET_PKG)/Library/FchInitHookLib/FchInitHookLib.inf
  ProcessPendingCapsuleLib|$(CHIPSET_PKG)/Library/ProcessPendingCapsuleLib/BaseProcessPendingCapsuleLib.inf
  ResetSystemLib|$(CHIPSET_PKG)/Library/ResetSystemLib/ResetSystemLib.inf
#[-start-220125-IB14740205-add]#
  BaseOemSvcChipsetLib|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf
  BaseOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLibDefault.inf
#[-end-220125-IB14740205-add]#

  #
  # Overrided LibraryClasses
  #
  FdSupportLib|InsydeModulePkg/Library/FlashDeviceSupport/FdSupportLib/FdSupportLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Library/FlashDeviceSupport/FdSupportLib
  }

  BaseSynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/MdePkg/Library/BaseSynchronizationLib
  }

  SmmCpuFeaturesLib|UefiCpuPkg/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/Library/SmmCpuFeaturesLib
  }

  GenericBdsLib|InsydeModulePkg/Library/GenericBdsLib/GenericBdsLib.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Library/GenericBdsLib
  }

#[-start-240202-IB20840047-add]#
  CcExitLib|$(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf
#[-end-240202-IB20840047-add]#

  ChipsetCapsuleLib|$(CHIPSET_PKG)/CapsuleIFWU/CapsuleLib/ChipsetCapsuleLib.inf
  ChipsetSignatureLib|$(CHIPSET_PKG)/CapsuleIFWU/CapsuleLib/ChipsetSignatureLib.inf
  ChipsetCapsuleRecoveryLib|$(CHIPSET_PKG)/CapsuleIFWU/CapsuleLib/ChipsetCapsuleRecoveryLib.inf
  FwUpdateLib|$(CHIPSET_PKG)/Library/FWUpdateLib/FWUpdateLib.inf
#[-start-220126-IB14740206-add]#
  AmdMcmRbUpdateLib|$(CHIPSET_PKG)/Library/AmdMcmRbUpdateLib/AmdMcmRbUpdateLib.inf
#[-end-220126-IB14740206-add]#
#[-start-230309-IB14740277-add]#
  XhciEralyInitLib|$(CHIPSET_PKG)/Library/XhciEralyInitLib/XhciEralyInitLib.inf
#[-end-230309-IB14740277-add]#
[LibraryClasses.common.SEC]
  PlatformSecLib|$(CHIPSET_PKG)/Library/PlatformSecLib/PlatformSecLib.inf
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/BaseTscTimerLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
#[-start-220208-IB14740211-add]#
  FchMmioSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/FchMmioSpiAccessLib/FchMmioSpiAccessLib.inf
#[-end-220208-IB14740211-add]#

  #
  # Chipset level Oem service libraries
  #
  SecOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/SecOemSvcChipsetLib/SecOemSvcChipsetLibDefault.inf
  SecOemSvcChipsetLib|$(CHIPSET_PKG)/Library/SecOemSvcChipsetLib/SecOemSvcChipsetLib.inf

[LibraryClasses.common.PEI_CORE]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/PeiTscTimerLib.inf

[LibraryClasses.common.PEIM]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/PeiTscTimerLib.inf
  SmbusLib|MdePkg/Library/PeiSmbusLibSmbus2Ppi/PeiSmbusLibSmbus2Ppi.inf
  KernelConfigLib|$(CHIPSET_PKG)/Library/PeiKernelConfigLib/PeiKernelConfigLib.inf
  ChipsetConfigLib|$(CHIPSET_PKG)/Library/ChipsetConfigLib/PeiChipsetConfigLib.inf
  AmdExtCmosLib|$(CHIPSET_PKG)/Library/AmdExtCmosLib/AmdExtCmosLib.inf
  SCUVarInfoLib|$(CHIPSET_PKG)/Library/PeiSCUVarInfoLib/PeiSCUVarInfoLib.inf

[LibraryClasses.common.DXE_CORE]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/DxeTscTimerLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/DxeTscTimerLib.inf

#[-start-220308-IB14740223-add]#
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmor3SpiAccessLib/RomArmor3SpiAccessLib.inf
#[-end-220308-IB14740223-add]#

[LibraryClasses.common.UEFI_DRIVER]
  DebugPrintErrorLevelLib|InsydeModulePkg/Library/DxeDebugPrintErrorLevelLib/DxeDebugPrintErrorLevelLib.inf
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/DxeTscTimerLib.inf

[LibraryClasses.common.DXE_DRIVER]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/DxeTscTimerLib.inf
  ChipsetConfigLib|$(CHIPSET_PKG)/Library/ChipsetConfigLib/DxeChipsetConfigLib.inf
  EfiRegTableLib|$(CHIPSET_PKG)/Library/EfiRegTableLib/EfiRegTableLib.inf

  #
  # Chipset level Oem service libraries
  #
  DxeOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLibDefault.inf
  DxeOemSvcChipsetLib|$(CHIPSET_PKG)/Library/DxeOemSvcChipsetLib/DxeOemSvcChipsetLib.inf

  SCUVarInfoLib|$(CHIPSET_PKG)/Library/DxeSCUVarInfoLib/DxeSCUVarInfoLib.inf

#[-start-220308-IB14740223-add]#
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmor3SpiAccessLib/RomArmor3SpiAccessLib.inf
#[-end-220308-IB14740223-add]#

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmmLib|$(CHIPSET_PKG)/Library/FchSmmLib/FchSmmLib.inf
  #
  # Chipset level Oem service libraries
  #
  SmmOemSvcChipsetLibDefault|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLibDefault.inf
  SmmOemSvcChipsetLib|$(CHIPSET_PKG)/Library/SmmOemSvcChipsetLib/SmmOemSvcChipsetLib.inf

#[-start-220308-IB14740223-add]#
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmor3SpiAccessLib/RomArmor3SpiAccessLib.inf
#[-end-220308-IB14740223-add]#

[LibraryClasses.common.COMBINED_SMM_DXE]
#[-start-220308-IB14740223-add]#
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmor3SpiAccessLib/RomArmor3SpiAccessLib.inf
#[-end-220308-IB14740223-add]#

[LibraryClasses.common.SMM_CORE]
  #
  # due to DDT will restart when boot into OS and in SMM trace code,
  # so stop WDT when in SMM.
  #
!if $(INSYDE_DEBUGGER) == YES
  SmmCorePlatformHookLib|$(CHIPSET_PKG)/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf
!endif

#[-start-220308-IB14740223-add]#
  RomArmorSpiAccessLib|$(CHIPSET_PKG)/Library/FlashDeviceSupport/RomArmor3SpiAccessLib/RomArmor3SpiAccessLib.inf
#[-end-220308-IB14740223-add]#

[LibraryClasses.common.UEFI_APPLICATION]
  TimerLib|$(CHIPSET_PKG)/Library/TscTimerLib/DxeTscTimerLib.inf

[PcdsFeatureFlag]
  gInsydeTokenSpaceGuid.PcdH2OBdsCpPciEnumUpdateDevInfoSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OBdsCpUpdateAcpiDescHdrSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2ODxeCpUpdateAcpiDescHdrSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OBdsCpBootDeviceEnumAfterSupported|TRUE

  gInsydeTokenSpaceGuid.PcdFvbAccessThroughSmi|TRUE
  gInsydeTokenSpaceGuid.PcdH2OPeiCpInitChipsetPolicySupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFdmChainOfTrustSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OAcpiWsmtSupport|TRUE
  gInsydeTokenSpaceGuid.PcdH2OIhisiCmdBufferSupported|TRUE
  #
  # For NV HG feature, Chipset set FALSE as default.
  # If project need to use it, please set it to TRUE at dsc file in board package.
  #
  gChipsetPkgTokenSpaceGuid.PcdHybridGraphicsSupported|FALSE
  gChipsetPkgTokenSpaceGuid.PcdNvidiaOptimusSupported|FALSE

  #
  # Support TPM NV index
  #
  gInsydeTokenSpaceGuid.PcdH2OTpm2CreateMsftIndexC0Supported|TRUE

#[-start-220125-IB14740205-modify]#
  gChipsetPkgTokenSpaceGuid.PcdEcCapsuleUpdateSupported|FALSE
#[-end-220125-IB14740205-modify]#

  gInsydeTokenSpaceGuid.PcdH2OPeiCpInitPlatformStage1Supported|TRUE
[PcdsFixedAtBuild]

  gInsydeTokenSpaceGuid.PcdTemporaryRamSizeWhenRecovery|0x10000

  gInsydeTokenSpaceGuid.PcdSoftwareSmiPort|0xB0
  gInsydeTokenSpaceGuid.PcdSmBusBaseAddress|0x0B00
  gInsydeTokenSpaceGuid.PcdSmBusBusDevFunc|0x140000             # bit[31:24] : bus, bit[23:16] : device, bit[15:8] : function, bit[0:7] : register
  #
  # CMOS Debug Code Table, wait for implementation
  #
  gInsydeTokenSpaceGuid.PcdPeiChipsetDebugCodeTable|{0}
  gInsydeTokenSpaceGuid.PcdDxeChipsetDebugCodeTable|{0}
  gInsydeTokenSpaceGuid.PcdMemorySpaceSize|48
  gEfiMdeModulePkgTokenSpaceGuid.PcdUse1GPageTable|TRUE
  #
  # AMD I/O Virtualization Technology (IOMMU) is not supported on
  # non-FM2 APU (FS1r2 and FP2). It is only supported on Trinity
  # FM2 APU (Virgo).
  # Therefore, all of the PCI-SIG I/O Virtualizations need to be
  # turned off to avoid a system hang on PC 0x0213.
  #
#[-start-221003-IB14740266-remove]#
#  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
#  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
#  gEfiMdeModulePkgTokenSpaceGuid.PcdMrIovSupport|FALSE
#[-end-221003-IB14740266-remove]#
  #
  # Pcie MMIO range
  # Auto arrange PciExpressBaseAddress according to PcdH2OPciExpressSize
  #
#[-start-220126-IB14740206-modify]#
!if $(MAX_SUPPORTED_PCI_BUSES) == 256
  gChipsetPkgTokenSpaceGuid.PcdH2OPciExpressSize|0x10000000
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSizeMsrValue|0x21
!elseif $(MAX_SUPPORTED_PCI_BUSES) == 128
  gChipsetPkgTokenSpaceGuid.PcdH2OPciExpressSize|0x8000000
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xF0000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSizeMsrValue|0x1D
!elseif $(MAX_SUPPORTED_PCI_BUSES) == 64
  gChipsetPkgTokenSpaceGuid.PcdH2OPciExpressSize|0x4000000
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xF0000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSizeMsrValue|0x19
!elseif $(MAX_SUPPORTED_PCI_BUSES) == 32
  gChipsetPkgTokenSpaceGuid.PcdH2OPciExpressSize|0x2000000
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xF8000000
  gChipsetPkgTokenSpaceGuid.PcdPciExpressSizeMsrValue|0x15
!endif
#[-end-220126-IB14740206-modify]#

#[-start-220803-IB14740254-modify]#
  # Increase S3 ACPI reserve memory size to 16MB
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x1000000
#[-end-220803-IB14740254-modify]#

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdFchOemBeforePciRestoreSwSmi|0xEA
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdFchOemAfterPciRestoreSwSmi|0xEB
  gAmdCpmPkgTokenSpaceGuid.AcpiDisableDgpuAudioSwSmi|0xED

  # gAmdCpmPkgTokenSpaceGuid.PcdBspStackSize|0x40000
  ## Stack size in the temporary RAM.
  #   0 means half of PcdTemporaryRamSizeWhenRecovery.
  gInsydeTokenSpaceGuid.PcdPeiTemporaryRamStackSizeWhenRecovery|0x8000

  gChipsetPkgTokenSpaceGuid.PcdH2OFchLimitProtectSizePerSet|0x1000000

  ## Specifies the initial value for Register_D in RTC.
  # @Prompt Initial value for Register_D in RTC.
  gPcAtChipsetPkgTokenSpaceGuid.PcdInitialValueRtcRegisterD|0x80

#
# Chipset value of reserved memroy size for secure flash (Kernel Default: 9MB)
#
!if $(VN_32M_AB_COMBO_CFG) == YES
  gInsydeTokenSpaceGuid.PcdReservedMemorySizeForSecureFlash|0x02200000
!else
  gInsydeTokenSpaceGuid.PcdReservedMemorySizeForSecureFlash|0x01200000
!endif
  #
  # For Support Sata Freeze feature (Related with Chipset ASL code)
  #
  gChipsetPkgTokenSpaceGuid.PcdH2OSataFreezeByAslSupported|FALSE

  #
  # Enable PcdShadowPeimOnS3Boot to load PEIM into memory during S3 resume
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPcdCallBackNumberPerPcdEntry|0x10

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x8000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds|10

  #
  # Please check PPR when setting
  #
  gChipsetPkgTokenSpaceGuid.I2cTotalControllerNumber|0x04
  gChipsetPkgTokenSpaceGuid.I2cGpioBase|0xFED81500
  gChipsetPkgTokenSpaceGuid.I2cBase|0xFEDC2000

#[-start-221104-IB14740270-modify]#
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspS3WakeFromSmm|FALSE
#[-end-221104-IB14740270-modify]#


[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdPcieResizableBarSupport|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdFchWdtEnable|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdApicMode|0x1                 #  0x01 (xApicMode) - force legacy xApic mode

[PcdsDynamicExDefault]
  # Crisis recovery supported devices
  gH2ODeviceInfo2TokenSpaceGuid.PcdH2ODeviceInfoOnboradXhci0|{DEVICE_PATH("PciRoot(0x0)/Pci(0x8,0x1)/Pci(0x0,0x3)"), "pcie_pei_enable=crisisrecovery", "pcie_pei_bar0=0xD0300000"}
  gH2ODeviceInfo2TokenSpaceGuid.PcdH2ODeviceInfoOnboradAhci0|{DEVICE_PATH("PciRoot(0x0)/Pci(0x8,0x2)/Pci(0x0,0x0)"), "pcie_pei_enable=crisisrecovery", "pcie_pei_bar5=0xD0085000"}
  gH2ODeviceInfo2TokenSpaceGuid.PcdH2ODeviceInfoM2Nvme|{DEVICE_PATH("PciRoot(0x0)/Pci(0x2,0x4)/Pci(0x0,0x0)"), "pcie_pei_enable=crisisrecovery", "pcie_pei_bar0=0xD0060000"}

  #
  # Preserve Memory Table
  #
  gInsydeTokenSpaceGuid.PcdPreserveMemoryTable|{ \
     UINT32(0x09), UINT32(0x80  ),  \ # Preserve 256K(0x40 pages) for ASL
     UINT32(0x0a), UINT32(0x2000 ),  \ # Preserve 3M(0x300 pages) for S3, SMM, etc
     UINT32(0x00), UINT32(0x1000 ),  \ # Preserve 4M(0x400 pages) for BIOS reserved memory
     UINT32(0x06), UINT32(0x1000 ),  \ # Preserve 3M(0x300 pages) for UEFI OS runtime data to make S4 memory consistency
     UINT32(0x05), UINT32(0x800 ),  \ # Preserve 2M(0x200 pages) for UEFI OS runtime drivers to make S4 memory consistency
     UINT32(0x03), UINT32(0x2000),  \ # Preserve 16M(0x1000 pages) for boot service drivers to reduce memory fragmental
     UINT32(0x04), UINT32(0xA000),  \ # Preserve 64M(0x4000 pages) for boot service data to reduce memory fragmental
     UINT32(0x01), UINT32(0x200 ),  \ # Preserve 2M(0x200 pages) for UEFI OS boot loader to keep on same address
     UINT32(0x02), UINT32(0x00  ),  \
     UINT32(0x0e), UINT32(0x00  )}    #EndEntry



  #
  # Port number mapping table Define
  #
  gInsydeTokenSpaceGuid.PcdPortNumberMapTable|{ \
0x00, 0x11, 0x00, 0, 0, 0x00, \
0x00, 0x11, 0x00, 0, 1, 0x02, \
0x00, 0x11, 0x00, 1, 0, 0x01, \
0x00, 0x11, 0x00, 1, 1, 0x03, \
0x00, 0x14, 0x01, 0, 0, 0x04, \
0x00, 0x14, 0x01, 0, 1, 0x05, \
0x00, 0x14, 0x01, 1, 0, 0x06, \
0x00, 0x14, 0x01, 1, 1, 0x07, \
0x00, 0x00, 0x00, 0, 0, 0x00} #EndEntry

  # PIRQ link value                                  PIRQA, PIRQB, PIRQC, PIRQD, PIRQE, PIRQF, PIRQG, PIRQH
  gChipsetPkgTokenSpaceGuid.PcdH2OPirqLinkValueArray|{0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07} #EndEntry                                                 0x00, 0x01, 0x02, 0x03, \

  # List all bridges in the system.
  # The ones that require a bus fix up, you need to put the multibus attribute on them.
  # Also, to support a bridge behind a bridge behind a root bridge, the order IS important.
  # The parent bridges must come first in the table.
  gChipsetPkgTokenSpaceGuid.PcdH2OVirtualBusTable|{  0x00, 0x01, 0x01, 0x01, \
                                                     0x00, 0x01, 0x02, 0x02, \
                                                     0x00, 0x01, 0x03, 0x03, \
                                                     0x00, 0x01, 0x04, 0x04, \
                                                     0x00, 0x01, 0x05, 0x05, \
                                                     0x00, 0x01, 0x06, 0x06, \
                                                     0x00, 0x01, 0x07, 0x07, \
                                                     0x00, 0x02, 0x01, 0x08, \
                                                     0x00, 0x03, 0x01, 0x09, \
                                                     0x00, 0x03, 0x02, 0x0A, \
                                                     0x00, 0x03, 0x03, 0x0B, \
                                                     0x00, 0x03, 0x04, 0x0C, \
                                                     0x00, 0x03, 0x05, 0x0D, \
                                                     0x00, 0x03, 0x06, 0x0E, \
                                                     0x00, 0x03, 0x07, 0x0F, \
                                                     0x00, 0x04, 0x01, 0x10, \
                                                     0x00, 0x07, 0x01, 0x11, \
                                                     0x00, 0x08, 0x01, 0x12} #EndEntry

#0x80 => Dev 0x10
#0x88 => Dev 0x11
#0x90 => Dev 0x12
#0xA0 => Dev 0x14

#0x08 => Dev 0x01 IGD
#0x10 => Dev 0x02
#0x18 => Dev 0x03
#0x20 => Dev 0x04

#0x38 => Dev 0x07
#0x40 => Dev 0x08

#0x48 => Dev 0x09

  #
  #Bus, Dev,  INT#A,IrqMask,        INT#B,IrqMask,        INT#C,IrqMask,        INT#D,IrqMask,        Slot, Reserved, DevIpRegValue, ProgrammableIrq.
  #
  gChipsetPkgTokenSpaceGuid.PcdH2OControllerDeviceIrqRoutingEntry| { \
  0x00, 0x08, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x00, 0x00, UINT32(0x00000000), 0x00, \
  0x00, 0x40, 0x03, UINT16(0xDC90), 0x04, UINT16(0xDC90), 0x00, UINT16(0xDC90), 0x00, UINT16(0xDC90), 0x00, 0x00, UINT32(0x00000000), 0x00, \
  0x00, 0xA0, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x00, 0x00, UINT32(0x00000000), 0x00, \

  0x01, 0x00, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x01, 0xFF, UINT32(0x00000000), 0x00, \
  0x02, 0x00, 0x04, UINT16(0xDC90), 0x05, UINT16(0xDC90), 0x06, UINT16(0xDC90), 0x07, UINT16(0xDC90), 0x02, 0xFF, UINT32(0x00000000), 0x00, \
  0x03, 0x00, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x03, 0xFF, UINT32(0x00000000), 0x00, \
  0x04, 0x00, 0x04, UINT16(0xDC90), 0x05, UINT16(0xDC90), 0x06, UINT16(0xDC90), 0x07, UINT16(0xDC90), 0x04, 0xFF, UINT32(0x00000000), 0x00, \
  0x05, 0x00, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x05, 0xFF, UINT32(0x00000000), 0x00, \
  0x06, 0x00, 0x04, UINT16(0xDC90), 0x05, UINT16(0xDC90), 0x06, UINT16(0xDC90), 0x07, UINT16(0xDC90), 0x06, 0xFF, UINT32(0x00000000), 0x00, \
  0x07, 0x00, 0x00, UINT16(0xDC90), 0x01, UINT16(0xDC90), 0x02, UINT16(0xDC90), 0x03, UINT16(0xDC90), 0x07, 0xFF, UINT32(0x00000000), 0x00, \

  0x08, 0x00, 0x04, UINT16(0xDC90), 0x05, UINT16(0xDC90), 0x06, UINT16(0xDC90), 0x07, UINT16(0xDC90), 0x08, 0xFF, UINT32(0x00000000), 0x00, \
  0x09, 0x00, 0x06, UINT16(0xDC90), 0x07, UINT16(0xDC90), 0x04, UINT16(0xDC90), 0x05, UINT16(0xDC90), 0x09, 0xFF, UINT32(0x00000000), 0x00} #EndEntry

  gChipsetPkgTokenSpaceGuid.PcdH2OIrqPoolTable|{03, 0x00, \ #IRQ03
                                                05, 0x00, \ #IRQ05
                                                06, 0x00, \ #IRQ06
                                                10, 0x00, \ #IRQ10
                                                11, 0x00, \ #IRQ11
                                                00, 0x00}   #IRQ00

  gChipsetPkgTokenSpaceGuid.PcdH2OPirqPriorityTable|{0,  \# PIRQ A
                                                     0,  \# PIRQ B
                                                     0,  \# PIRQ C
                                                     10, \# PIRQ D
                                                     0,  \# PIRQ E
                                                     0,  \# PIRQ F
                                                     0,  \# PIRQ G
                                                     0}   # PIRQ H

  gEfiAmdAgesaPkgTokenSpaceGuid.PcdGlobalStbVerbosityControl|0x00

  #Make STB related PCD to be Dynamic
  #Swtich for STB related features, including STB write function, STB register config, send STB verbosity to FWs
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbEnable|TRUE

  #Switch for write agesa testpoint to STB
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAgesaTestPointToStb|FALSE

  #Swtich for STB FilterMask feature
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbFilterMaskEnable|TRUE

  #Only active when PcdStbFilterMaskEnable set to TRUE
  #Value set to MP2_POSTCODE_IP_0, need be adjust for different IBV
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdStbIbvSourceId|0xB3

[PcdsDynamicExDefault]
  gH2OFlashDeviceMfrNameGuid.PcdWinbondW25q256jwSpiMfrName|"WINBOND"
  gH2OFlashDevicePartNameGuid.PcdWinbondW25q256jwSpiPartName|"W25Q256JW"
  gH2OFlashDeviceGuid.PcdWinbondW25q256jwSpi|{ \ # WINBOND  W25Q256JW
    0x03, 0x00, 0x00, 0x00, 0xef, 0x60, 0x19, 0x00,  \  # DeviceType = 03  Id = 001960EF
    0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x20   \  # ExtId = 00000000  BlockSize = 0010  BlockCount = 2000
  }

  gH2OFlashDeviceConfigGuid.PcdWinbondW25q256jwSpiConfig|{ \ # WINBOND  W25Q256JW
    0x28, 0x00, 0x00, 0x00, 0x9f, 0x20, 0x02, 0x01,  \  # Size = 00000028  ReadIdOp = 9F  EraseOp = 20  WriteOp = 02  WriteStatusOp = 01
    0x03, 0x05, 0x00, 0x00, 0x7c, 0xf2, 0x06, 0x06,  \  # ReadOp = 03  ReadStatusOp = 05  OpType = F27C  WriteEnablePrefix = 06  WriteStatusEnablePrefix = 06
    0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x01, 0x00,  \  # GlobalProtectAvailable = 00  BLockProtectAvailable = 00  BlockProtectCodeRequired = 00  MultiByteProgramAvailable = 01  BytesOfId = 03  MinBytePerProgRead = 01  NoVolStatusAvailable = 01
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,  \  # GlobalProtectCode = 00  GlobalUnprotectCode = 00  BlockProtectCode = 00  BlockUnprotectCode = 00  DeviceSize = 02000000
    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   \  # BlockEraseSize = 00001000  BlockProtectSize = 00000000
  }

  #
  # The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
  #
  # BIT0: FIXED_COMM_BUFFERS, If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
  # BIT1: COMM_BUFFER_NESTED_PTR_PROTECTION. If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
  #       that lie entirely within the expected fixed memory regions.
  # BIT2: SYSTEM_RESOURCE_PROTECTION, Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
  # BIT3-31: Reserved, must return 0 when read.
  #
  gInsydeTokenSpaceGuid.PcdH2OAcpiWsmtProtectionFlags|0x07

[Components.$(PEI_ARCH)]
  #
  # Helper tool modules
  #
  $(CHIPSET_PKG)/Tools/GenOemRomSig/GenOemRomSig.inf
#[-start-220114-IB19600008-add]#
!if gChipsetPkgTokenSpaceGuid.PcdH2OCmosLoadDefaultSupported
  $(CHIPSET_PKG)/CMOSLoadDefaultDetectPei/CMOSLoadDefaultDetectPei.inf
!endif
#[-end-220114-IB19600008-add]#

  $(CHIPSET_PKG)/DisableCacheAsRamPei/DisableCacheAsRamPeiBin.inf
  $(CHIPSET_PKG)/ChipsetSvcPei/ChipsetSvcPei.inf
  $(CHIPSET_PKG)/MemoryIntiPei/CpuInstallEfiMemoryPei.inf
  $(CHIPSET_PKG)/MemoryIntiPei/CrisisMemoryInitPei.inf
  $(CHIPSET_PKG)/SmmAccessPei/SmmAccessPei.inf
  $(CHIPSET_PKG)/SmmControlPei/SmmControlPei.inf
  $(CHIPSET_PKG)/S3NotifyDispatchPei/S3NotifyDispatchPei.inf
!if gInsydeTokenSpaceGuid.PcdCrisisRecoverySupported
  $(CHIPSET_PKG)/RecoveryCpuInitPei/RecoveryCpuInitPei.inf
  $(CHIPSET_PKG)/RecoveryFchInitPei/RecoveryFchInitPei.inf
!endif
  $(CHIPSET_PKG)/InitPortConfig/InitPortConfig.inf
!if gInsydeTokenSpaceGuid.PcdMultiConfigSupported
  $(CHIPSET_PKG)/MultiConfigVarHookPei/MultiConfigVarHookPei.inf
!endif
!if gInsydeTokenSpaceGuid.PcdH2ODdtSupported
  $(CHIPSET_PKG)/FchUartInitPei/FchUartInitPei.inf
#[-start-220224-IB14740217-add]#
  !disable InsydeModulePkg/H2ODebug/$(H2O_DDT_DEBUG_IO)DebugIoPei/$(H2O_DDT_DEBUG_IO)DebugIoPei.inf
  !disable InsydeModulePkg/H2ODebug/DebugEnginePei/DebugEnginePei.inf
  $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/H2ODebug/$(H2O_DDT_DEBUG_IO)DebugIoPei/$(H2O_DDT_DEBUG_IO)DebugIoPei.inf
  $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/H2ODebug/DebugEnginePei/DebugEnginePei.inf
#[-end-220224-IB14740217-add]#
!endif

  $(CHIPSET_PKG)/GpioCfgPei/GpioCfgPei.inf
  #
  # Compatibility
  #
  $(CHIPSET_PKG)/Compatibility/AcpiVariableHobOnSmramReserveHobThunk/AcpiVariableHobOnSmramReserveHobThunk.inf

!if gChipsetPkgTokenSpaceGuid.PcdHybridGraphicsSupported
  $(CHIPSET_PKG)/HybridGraphicsPei/HybridGraphicsPei.inf
!endif

#[-start-230504-IB14740286-add]#
!if gInsydeTokenSpaceGuid.PcdH2OFdmChainOfTrustSupported
  $(CHIPSET_PKG)/FdmHashVerifyPei/FdmHashVerifyPei.inf
!endif
#[-end-230504-IB14740286-add]#

################################################################################
#
# Components.IA32 Override
#
################################################################################
  UefiCpuPkg/SecCore/SecCore.inf
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf

  !disable UefiCpuPkg/CpuMpPei/CpuMpPei.inf
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/CpuMpPei
    <LibraryClasses>
      NULL|AgesaModulePkg/Library/CcxMpServiceDepexLib/CcxMpServicePpiDepexLib.inf
  }

  !disable InsydeModulePkg/Universal/CommonPolicy/PlatformStage2Pei/PlatformStage2Pei.inf
  InsydeModulePkg/Universal/CommonPolicy/PlatformStage2Pei/PlatformStage2Pei.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/CommonPolicy/PlatformStage2Pei
  }

  #
  # AMD feature:
  #   Enable debug message on PeiMain driver by comment out MDEPKG_NDEBUG marco,
  #   it need sync with MdeModulePkg\Package.dsc
  #
  !disable MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Core/Pei/PeiMain.inf {
    <SOURCE_OVERRIDE_PATH>
      MdeModulePkg/Override/Core/Pei
#   <BuildOptions>
#     *_*_*_CC_FLAGS = -D MDEPKG_NDEBUG
  }

  #
  # For A/B recovery feature
  #
  !disable InsydeModulePkg/Universal/Variable/VariablePei/VariablePei.inf
  InsydeModulePkg/Universal/Variable/VariablePei/VariablePei.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/Variable/VariablePei
  }

  #
  # For HSP Ftpm
  #
  !disable InsydeModulePkg/Universal/Security/Tcg/Tcg2Pei/Tcg2Pei.inf
  InsydeModulePkg/Universal/Security/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|InsydeModulePkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
    <Depex>
      gEfiPeiMasterBootModePpiGuid AND
      gEfiPeiReadOnlyVariable2PpiGuid AND
      gEfiTpmDeviceSelectedGuid AND
      gPeiTpm2InterfacePpiGuid
  }

#[-start-220308-IB14740223-add]#
  !disable InsydeModulePkg/Universal/CommonPolicy/PlatformStage1Pei/PlatformStage1Pei.inf
  InsydeModulePkg/Universal/CommonPolicy/PlatformStage1Pei/PlatformStage1Pei.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/CommonPolicy/PlatformStage1Pei
  }
#[-end-220308-IB14740223-add]#

[Components.$(DXE_ARCH)]
  $(CHIPSET_PKG)/ChipsetSvcDxe/ChipsetSvcDxe.inf
  $(CHIPSET_PKG)/ChipsetSvcSmm/ChipsetSvcSmm.inf
  $(CHIPSET_PKG)/LegacyRegion2Dxe/LegacyRegion2Dxe.inf
#[-start-220126-IB14740206-remove]#
#  $(CHIPSET_PKG)/PciHostBridgeDxe/PciHostBridgeDxe.inf {
#    <LibraryClasses>
#      SmbusLib|MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
#  }
#[-end-220126-IB14740206-remove]#
  #
  #  I2C Platform Specific drvier.
  #
  $(CHIPSET_PKG)/I2cPlatformSpecificDxe/I2cPlatformSpecificDxe.inf
  #
  #  I2C Master drvier.
  #
  $(CHIPSET_PKG)/I2cMaster/Dxe/I2cMasterDxe.inf

  $(CHIPSET_PKG)/SataControllerDxe/SataControllerDxe.inf
  $(CHIPSET_PKG)/SmmAccess2Dxe/SmmAccess2Dxe.inf
  $(CHIPSET_PKG)/OemBadgingSupportDxe/OEMBadgingSupportDxe.inf
  $(CHIPSET_PKG)/UnexpectedIrqWADxe/UnexpectedIrqWA.inf
  $(CHIPSET_PKG)/PlatformSmm/PlatformSmm.inf
#[-start-221104-IB14740270-modify]#
#[-start-220906-IB14740262-modify]#
  $(CHIPSET_PKG)/SmmIhisi/IhisiSmm.inf
#[-end-220906-IB14740262-modify]#
#[-end-221104-IB14740270-modify]#
#[-start-221104-IB14740270-add]#
  $(CHIPSET_PKG)/ApcbTokensUpdateSmm/AodApcbTokensUpdateSmm.inf {
    <LibraryClasses>
      NULL|AmdCpmPkg/Features/AODv2/Library/AodSocLib/RMB/AodSocLibSmm.inf
  }
  $(CHIPSET_PKG)/ApcbTokensUpdateSmm/CbsApcbTokensUpdateSmm.inf
#[-end-221104-IB14740270-add]#
#[-start-220126-IB14740206-remove]#
#  $(CHIPSET_PKG)/AcpiTables/AcpiTables.inf
#[-end-220126-IB14740206-remove]#
  $(CHIPSET_PKG)/UefiSetupUtilityDxe/SetupUtilityDxe.inf
  $(CHIPSET_PKG)/FrontPageDxe/FrontPageDxe.inf
  $(CHIPSET_PKG)/PlatformDxe/PlatformDxe.inf
  $(CHIPSET_PKG)/LegacyBiosPlatformHookDxe/LegacyBiosPlatformHookDxe.inf
  $(CHIPSET_PKG)/OemInt15CallBackSmm/OemInt15CallBackSmm.inf
  $(CHIPSET_PKG)/CmosChecksum/CmosChecksum.inf
  $(CHIPSET_PKG)/CpuMpDxe/CpuMpDxe.inf

  !disable InsydeModulePkg/Universal/Security/HddPassword/HddPassword.inf
  InsydeModulePkg/Universal/Security/HddPassword/HddPassword.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/Security/HddPassword
  }

  !disable  InsydeModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf
  InsydeModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Csm/LegacyBiosDxe
  }

  #
  # Support UEFI RAID DRIVER
  #
  $(CHIPSET_PKG)/Binary/RAID/UefiRaid/RaidBus.inf

!if gChipsetPkgTokenSpaceGuid.PcdHybridGraphicsSupported
  $(CHIPSET_PKG)/HybridGraphicsDxe/HybridGraphicsDxe.inf
  $(CHIPSET_PKG)/HybridGraphicsSmm/HybridGraphicsSmm.inf
  $(CHIPSET_PKG)/HybridGraphicsAcpi/N20/NvidiaOptimusSsdt.inf
  $(CHIPSET_PKG)/HybridGraphicsAcpi/N20/NvidiaDiscreteSsdt.inf
  $(CHIPSET_PKG)/HybridGraphicsAcpi/N18/NvidiaOptimusSsdt.inf
  $(CHIPSET_PKG)/HybridGraphicsAcpi/N18/NvidiaDiscreteSsdt.inf
  $(CHIPSET_PKG)/HybridGraphicsAcpi/N17/NvidiaOptimusSsdt.inf
!endif
  #
  # PXE Binary
  #
#  $(CHIPSET_PKG)/Binary/UEFIPxeDriver/LxUndiDxe.inf

  #
  # Install Bios Protect Interface
  #
#[-start-220208-IB14740210-remove]#
#  $(CHIPSET_PKG)/BiosWriteProtectSmm/SpiLockSmi.inf
#  $(CHIPSET_PKG)/BiosRegionLockDxe/BiosRegionLockDxe.inf
#[-end-220208-IB14740210-remove]#

  #
  # USB legacy support
  #
!if gChipsetPkgTokenSpaceGuid.PcdH2OLegacyFreeSupported == 1
  $(CHIPSET_PKG)/UsbLegacyControl/UsbLegacyControl.inf
!endif

  #
  # These module is for OEM to customize
  #
  $(CHIPSET_PKG)/OemAcpiPlatformDxe/OemAcpiPlatformDxe.inf

#[-start-140325-IB13080003-add]#
!if gInsydeTokenSpaceGuid.PcdDynamicHotKeySupported
 $(CHIPSET_PKG)/DynamicHotKeyDxe/DynamicHotKeyDxe.inf
!endif
#[-end-140325-IB13080003-add]#

  #
  # STIBP feature
  #
  $(CHIPSET_PKG)/StibpFeatureDxe/StibpFeatureDxe.inf

  $(CHIPSET_PKG)/AmdGopConfigDrvDxe/AmdGopConfigDrvDxe.inf

!if gChipsetPkgTokenSpaceGuid.PcdDashSupported == 1
  $(CHIPSET_PKG)/DashIoCfgDxe/DashIoCfgDxe.inf
  $(CHIPSET_PKG)/MctpSmbusDxe/MctpSmbusDxe.inf
  $(CHIPSET_PKG)/UpdateAsfTableDxe/UpdateAsfTableDxe.inf
  InsydeModulePkg/Universal/Dash/DashDxe/DashDxe.inf
  InsydeModulePkg/Universal/Dash/DashSmm/DashSmm.inf
!if $(KVM_SUPPORT) == YES
  $(CHIPSET_PKG)/KVMSmbiosDxe/KVMSmbiosDxe.inf
!endif
!endif

  #
  # Secure Biometrics Support
  #
  $(CHIPSET_PKG)/AcpiSdevTable/DXE/AcpiSdevTable.inf
  $(CHIPSET_PKG)/AcpiSdevTable/ASL/SecureBioCamAml.inf
  $(CHIPSET_PKG)/SecureFlashEspFlagSmm/SecureFlashEspFlagSmm.inf

!if gChipsetPkgTokenSpaceGuid.PcdEcCapsuleUpdateSupported == TRUE
  $(CHIPSET_PKG)/CapsuleIFWU/CapsuleEcDxe/EcCapsuleDxe.inf
!endif

!if $(EFI_DEBUG) == NO
  #
  # Replace the IdsLib instance with null lib to avoid debugging information in the normal build
  #
  !disable AgesaModulePkg/Usb4/AmdUsb4Dxe/AmdUsb4Dxe.inf
  AgesaModulePkg/Usb4/AmdUsb4Dxe/AmdUsb4Dxe.inf {
    <LibraryClasses>
      IdsLib|AMD/AmdEmbedded/AgesaModulePkg/Library/IdsLibNull/IdsLibNull.inf
  }
!endif

#[-start-220414-IB14740233-add]#
  $(CHIPSET_PKG)/HstiDxe/HstiDxe.inf
#[-end-220414-IB14740233-add]#

#[-start-220422-IB14740236-add]#
!disable InsydeModulePkg/Universal/Security/BiosProtectDxe/BiosProtectDxe.inf
#[-end-220422-IB14740236-add]#

#[-start-220905-IB14740261-add]#
  !disable InsydeModulePkg/Universal/FirmwareVolume/FlashDeviceFvbRuntimeDxe/FlashDeviceFvbRuntimeDxe.inf
  InsydeModulePkg/Universal/FirmwareVolume/FlashDeviceFvbRuntimeDxe/FlashDeviceFvbRuntimeDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/FirmwareVolume/FlashDeviceFvbRuntimeDxe
  }
#[-end-220905-IB14740261-add]#

#[-start-220906-IB14740262-add]#
  $(CHIPSET_PKG)/VarApcbSyncDxe/VarApcbSyncDxe.inf
#[-end-220906-IB14740262-add]#

#[-start-240618-IB20840064-add]#
  !disable InsydeModulePkg/Universal/Security/SecurityStubDxe/SecurityStubDxe.inf
  InsydeModulePkg/Universal/Security/SecurityStubDxe/SecurityStubDxe.inf {
      <SOURCE_OVERRIDE_PATH>
        $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/Security/SecurityStubDxe
  }
#[-end-240618-IB20840064-add]#

#[-start-240806-IB20840077-add]#
  $(CHIPSET_PKG)/SnpDxe/SnpDxePort.inf
#[-end-240806-IB20840077-add]#

#[-start-240911-IB20840092-add]#
!if gChipsetPkgTokenSpaceGuid.PcdH2ODefaultSupervisorPwSupported OR gChipsetPkgTokenSpaceGuid.PcdH2ODefaultSecureBootPwSupported
  $(CHIPSET_PKG)/H2ODefaultPasswordDxe/H2ODefaultPasswordDxe.inf
!endif
#[-end-240911-IB20840092-add]#

#[-start-241007-IB20840100-add]#
  !disable InsydeModulePkg/Universal/Security/SysPasswordDxe/SysPasswordDxe.inf
  InsydeModulePkg/Universal/Security/SysPasswordDxe/SysPasswordDxe.inf {
      <SOURCE_OVERRIDE_PATH>
        $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/Security/SysPasswordDxe
  }
#[-end-241007-IB20840100-add]#

################################################################################
#
# Components.X64 Override
#
################################################################################

  !disable UefiCpuPkg/CpuDxe/CpuDxe.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/CpuDxe
  }
  !disable UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/UefiCpuPkg/PiSmmCpuDxeSmm
  }

  #
  # Enhance Smm performance
  #
  !disable MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf {
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/MdeModulePkg/Core/PiSmmCore
  }

  !disable MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf{
    <SOURCE_OVERRIDE_PATH>
      $(CHIPSET_PKG)/Override/EDK2/MdeModulePkg/Core/PiSmmCore
  }

#[-start-220414-IB14740233-remove]#
#  !disable InsydeModulePkg/Universal/Security/HstiDxe/HstiDxe.inf
#  InsydeModulePkg/Universal/Security/HstiDxe/HstiDxe.inf {
#    <SOURCE_OVERRIDE_PATH>
#      $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/Universal/Security/HstiDxe
#  }
#[-end-220414-IB14740233-remove]#

#[-start-220317-IB14740226-remove]#
#!if gInsydePlatformInfoTokenSpaceGuid.PcdH2OCpuDeviceInfoSupported
#  !disable InsydePlatformInfoPkg/CpuDeviceInfoDxe/CpuDeviceInfoDxe.inf
#  InsydePlatformInfoPkg/CpuDeviceInfoDxe/CpuDeviceInfoDxe.inf {
#    <SOURCE_OVERRIDE_PATH>
#      $(CHIPSET_PKG)/Override/Insyde/InsydePlatformInfoPkg/CpuDeviceInfoDxe
#  }
#!endif
#[-end-220317-IB14740226-remove]#

#[-start-220224-IB14740217-add]#
!if gInsydeTokenSpaceGuid.PcdH2ODdtSupported
  !disable InsydeModulePkg/H2ODebug/$(H2O_DDT_DEBUG_IO)DebugIoDxe/$(H2O_DDT_DEBUG_IO)DebugIoDxe.inf
  !disable InsydeModulePkg/H2ODebug/DebugEngineDxe/DebugEngineDxe.inf
  $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/H2ODebug/$(H2O_DDT_DEBUG_IO)DebugIoDxe/$(H2O_DDT_DEBUG_IO)DebugIoDxe.inf
  $(CHIPSET_PKG)/Override/Insyde/InsydeModulePkg/H2ODebug/DebugEngineDxe/DebugEngineDxe.inf
!endif
#[-end-220224-IB14740217-add]#
