## @file
#  Platform Package Description file
#
#******************************************************************************
#* Copyright (c) 2014 - 2016, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

!import MdePkg/Package.dsc
!import MdeModulePkg/Package.dsc
!import UefiCpuPkg/Package.dsc
!import PerformancePkg/Package.dsc
!import SecurityPkg/Package.dsc
!import PcAtChipsetPkg/Package.dsc
#!import IA32FamilyCpuPkg/Package.dsc
!import SkylakeIA32FamilyCpuPkg/Package.dsc
!import IntelFrameworkModulePkg/Package.dsc
!import InsydeOemServicesPkg/Package.dsc
!import InsydeModulePkg/Package.dsc
!import InsydeSetupPkg/Package.dsc
!import InsydeNetworkPkg/Package.dsc
!import FatBinPkg/Package.dsc
!import ShellPkg/Package.dsc
!import InsydeFlashDevicePkg/Package.dsc
!import SkylakeSiPkg/Package.dsc
!import SkylakeChipsetPkg/Package.dsc
!import SioDummyPkg/Package.dsc
!import SioIt8728fPkg/Package.dsc
#!import InsydeMediaPlayerPkg/Package.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = Skylake
  PLATFORM_GUID                  = C197CED3-B91A-4544-8F97-A458CA0AAFDC
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PROJECT_PKG)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Build/$(PROJECT_PKG)/Project.fdf

  !include $(PROJECT_PKG)/Project.env
  DEFINE SECURE_FLASH_CERTIFICATE_FILE_PATH = $(PROJECT_PKG)/FactoryCopyInfo/SecureFlash.cer

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################
[PcdsFeatureFlag]
  gInsydeTokenSpaceGuid.PcdH2ODdtSupported|$(INSYDE_DEBUGGER)
!if $(EFI_DEBUG) == YES
  gInsydeTokenSpaceGuid.PcdStatusCodeUseDdt|$(INSYDE_DEBUGGER)
  gInsydeTokenSpaceGuid.PcdStatusCodeUseUsb|$(USB_DEBUG_SUPPORT)
!if $(USB_DEBUG_SUPPORT) == NO
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
#
# Running crisis will be fail if the CRB bios image is "nmake uefi64" and the USB dongle BIOS image is " uefi64 efidebug"
#
  gEfiTraceHubTokenSpaceGuid.PcdStatusCodeUseTraceHub|FALSE
!endif
!endif

#
# FRONTPAGE_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdFrontPageSupported|$(FRONTPAGE_SUPPORT)

#
# CRISIS RECOVERY
#
  gInsydeTokenSpaceGuid.PcdCrisisRecoverySupported|$(CRISIS_RECOVERY_SUPPORT)

#
# USE_FAST_CRISIS_RECOVERY
#
  gInsydeTokenSpaceGuid.PcdUseFastCrisisRecovery|$(USE_FAST_CRISIS_RECOVERY)

#
# SECURE_FLASH_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdSecureFlashSupported|$(SECURE_FLASH_SUPPORT)
  gInsydeTokenSpaceGuid.PcdBackupSecureBootSettingsSupported|$(BACKUP_SECURE_BOOT_SETTINGS_SUPPORT)

#
# UNSIGNED_FV_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdUnsignedFvSupported|$(UNSIGNED_FV_SUPPORT)

#
# ME_5MB_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdMe5MbSupported|$(ME_5MB_SUPPORT)

#
# CONSOLE_REDIRECTION_SUPPORT
#
# gInsydeTokenSpaceGuid.PcdH2OConsoleRedirectionSupported|FALSE

#
# UEFI_NETWORK_SUPPORTED
#
  gInsydeTokenSpaceGuid.PcdH2ONetworkSupported|TRUE

#
# DUAL_NETWORK_ENABLE
#
  gInsydeTokenSpaceGuid.PcdH2ONetworkIpv6Supported|TRUE

!if $(GCC) == Gcc
  gEfiTraceHubTokenSpaceGuid.PcdStatusCodeUseTraceHub|FALSE
!endif

#
# THUNDERBOLT_SUPPORT
#
!if $(APAC_SUPPORT) == YES
  gChipsetPkgTokenSpaceGuid.PcdThunderBoltSupported|TRUE
!else
  gChipsetPkgTokenSpaceGuid.PcdThunderBoltSupported|$(THUNDERBOLT_SUPPORT)
!endif
#
# SYS_PASSWORD_IN_CMOS
#
  gInsydeTokenSpaceGuid.PcdSysPasswordInCmos|$(SYS_PASSWORD_IN_CMOS)

#
# SUPPORT_USER_PASSWORD
#
  gInsydeTokenSpaceGuid.PcdSysPasswordSupportUserPswd|$(SUPPORT_USER_PASSWORD)

#
# SUPPORT_HDD_PASSWORD
#
  gInsydeTokenSpaceGuid.PcdH2OHddPasswordSupported|TRUE

#
# SNAP_SCREEN switch
#
  gInsydeTokenSpaceGuid.PcdSnapScreenSupported|$(SNAPSCREEN_SUPPORT)

  gInsydeTokenSpaceGuid.PcdBvdtGenBiosBuildTimeSupported|$(BUILD_TIME_FLAG)


#
# MEMORY_SPD_PROTECTION
#
  gChipsetPkgTokenSpaceGuid.PcdMemSpdProtectionSupported|$(MEMORY_SPD_PROTECTION)

#
# XTU_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdXtuSupported|$(XTU_SUPPORT)

#
# TXT_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdTXTSupported|$(TXT_SUPPORT)

#
# SUS_WELL_RESTORE
#
  gChipsetPkgTokenSpaceGuid.PcdSusWellRestore|$(SUS_WELL_RESTORE)

#
# TPM_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdH2OTpmSupported|TRUE
!if gChipsetPkgTokenSpaceGuid.PcdTXTSupported
  gInsydeTokenSpaceGuid.PcdH2OTpmSupported|TRUE
!endif

#
# TPM2_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdH2OTpm2Supported|TRUE


#
# SWITCHABLE_GRAPHICS_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdSwitchableGraphicsSupported|$(SWITCHABLE_GRAPHICS_SUPPORT)

#
# NVIDIA_OPTIMUS_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdNvidiaOptimusSupported|$(NVIDIA_OPTIMUS_SUPPORT)

#
# AMD_POWERXPRESS_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdAmdPowerXpressSupported|$(AMD_POWERXPRESS_SUPPORT)
  gChipsetPkgTokenSpaceGuid.PcdSgAslCodeForWptLynxPointLp|$(SG_ASLCODE_FOR_WPT_LynxPointLp)
#
# SMM_INT10_ENABLE
#
  gChipsetPkgTokenSpaceGuid.PcdSmmInt10Enable|$(SMM_INT10_ENABLE)
#
# AHCI_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdH2OAhciSupported|TRUE
#
# EC_SPI_INTERFACE
#
  gChipsetPkgTokenSpaceGuid.PcdEcSpiInterfaceSupported|$(EC_SPI_INTERFACE)

#
# EC_SHARED_FLASH_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdEcSharedFlashSupported|$(EC_SHARED_FLASH_SUPPORT)

#
# EC_IDLE_PER_WRITE_BLOCK
#
  gChipsetPkgTokenSpaceGuid.PcdEcIdlePerWriteBlockSupported|$(EC_IDLE_PER_WRITE_BLOCK)

#
# BIOS_GUARD_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdBiosGuardAcmSupport|$(BIOS_GUARD_SUPPORT)

#
# BOOT_GUARD_SUPPORT_FLAG
#
  gChipsetPkgTokenSpaceGuid.PcdBootGuardSupportFlag|$(BOOT_GUARD_SUPPORT)
!if gChipsetPkgTokenSpaceGuid.PcdBootGuardSupportFlag
  gChipsetPkgTokenSpaceGuid.PcdBootGuardVerifyFv|TRUE
!endif

#
# Q2LSERVICE_SUPPORT
#
  gInsydeTokenSpaceGuid.PcdH2OQ2LServiceSupported|$(Q2LSERVICE_SUPPORT)

#
# PTT_SUPPORT
#
  gChipsetPkgTokenSpaceGuid.PcdPttSupported|$(PTT_SUPPORT)

#
# OVERCLOCK_ENABLE
#
  gChipsetPkgTokenSpaceGuid.PcdOverClockSupported|$(OVERCLOCK_ENABLE)

  gInsydeTokenSpaceGuid.PcdH2OUsbSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OSdhcSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2ONetworkIscsiSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OIdeSupported|TRUE
  gInsydeTokenSpaceGuid.PcdGraphicsSetupSupported|$(GRAPHICS_SETUP_SUPPORT)
  gInsydeTokenSpaceGuid.PcdUefiPauseKeyFunctionSupport|$(UEFI_PAUSE_KEY_FUNCTION_SUPPORT)
  gInsydeTokenSpaceGuid.PcdTextModeFullScreenSupport|$(TEXT_MODE_FULL_SCREEN_SUPPORT)
  gInsydeTokenSpaceGuid.PcdOnlyUsePrimaryMonitorToDisplay|$(ONLY_USE_PRIMARY_MONITOR_TO_DISPLAY)
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported|TRUE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|TRUE

!errif (gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported == TRUE) and (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported == FALSE) and (gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported == FALSE), "Must have at least one display engine enabled in Project.dsc when PcdH2OFormBrowserSupported == TRUE; Otherwise set PcdH2OFormBrowserSupported = FALSE in Project.dsc"

!if gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported == TRUE
  gInsydeTokenSpaceGuid.PcdGraphicsSetupSupported|FALSE
!else
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalTextDESupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserLocalMetroDESupported|FALSE
!endif
  gChipsetPkgTokenSpaceGuid.PcdRestoreCmosfromVariableFlag|FALSE

  gChipsetPkgTokenSpaceGuid.PcdMeUnconfigOnRtcSupported|FALSE

  gInsydeTokenSpaceGuid.PcdMultiConfigSupported|$(MULTI_CONFIG_SUPPORT)

  gInsydeTokenSpaceGuid.PcdDynamicHotKeySupported|$(DYNAMIC_HOTKEY_SUPPORT)
#
# PcdS3SaveGpioTableFlag
# TRUE  | Save GPIO status when system into S3, if GPIO was changed.
# FALSE | GPIO status will return to default when S3 resume.
#
  gChipsetPkgTokenSpaceGuid.PcdS3SaveGpioTableFlag|FALSE
  gInsydeTokenSpaceGuid.PcdH2OI2cSupported|TRUE

  gInsydeTokenSpaceGuid.PcdShellBinSupported|FALSE
  gInsydeTokenSpaceGuid.PcdShellBuildSupported|FALSE
  gChipsetPkgTokenSpaceGuid.PcdDisableScuAggressiveLpmSupportForPchH|TRUE
  gChipsetPkgTokenSpaceGuid.PcdApacSupport|$(APAC_SUPPORT)
!if gChipsetPkgTokenSpaceGuid.PcdApacSupport
  gSioGuid.PcdSioDummySupported|FALSE
  gSioGuid.PcdSioIt8728fSupported|TRUE
!else
  gSioGuid.PcdSioDummySupported|TRUE
  gSioGuid.PcdSioIt8728fSupported|FALSE
!endif
  gChipsetPkgTokenSpaceGuid.PcdDebugUsePchComPort|$(DEBUG_USE_PCH_COMPORT)
!if gChipsetPkgTokenSpaceGuid.PcdMe5MbSupported
  gInsydeCrTokenSpaceGuid.PcdH2OCRPciSerialSupported|FALSE
  gInsydeTokenSpaceGuid.PcdH2OFormBrowserSupported|FALSE
  gInsydeTokenSpaceGuid.PcdDisplayOemHotkeyString|TRUE
!endif
  gChipsetPkgTokenSpaceGuid.PcdGopOverrideDriverEnable|FALSE

[PcdsFixedAtBuild]
  gEfiCpuTokenSpaceGuid.PcdCpuIEDEnabled|TRUE

!if $(FIRMWARE_PERFORMANCE) == YES
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|150
!else
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
!endif

!if $(H2O_PORT_80_DEBUG) == YES
  gChipsetPkgTokenSpaceGuid.PcdH2OPort80DebugEnable|1
!else
  gChipsetPkgTokenSpaceGuid.PcdH2OPort80DebugEnable|0
!endif

!if $(EFI_PORT_80_DEBUG) == YES
  gChipsetPkgTokenSpaceGuid.PcdEfiPort80DebugEnable|1
!else
  gChipsetPkgTokenSpaceGuid.PcdEfiPort80DebugEnable|0
!endif

!if $(SUPPORT_64BITS_AML) == YES
  gChipsetPkgTokenSpaceGuid.PcdDsdtRevision|0x02
!else
  gChipsetPkgTokenSpaceGuid.PcdDsdtRevision|0x01
!endif

!if $(SWITCHABLE_GRAPHICS_SUPPORT) == YES
  gChipsetPkgTokenSpaceGuid.PcdSwitchGraphicsSupport|1
!else
  gChipsetPkgTokenSpaceGuid.PcdSwitchGraphicsSupport|0
!endif

  gPerformancePkgTokenSpaceGuid.PcdPerfPkgAcpiIoPortBaseAddress|0x1800


#  WARNING:
#   This Pcd is the memory size for storing CAR data, it has to be always larger or equal to PcdTemporaryRamSize.
#   S3 resume may fail if PcdTemporaryRamSize > PcdS3AcpiReservedMemorySize.
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x80000
  #
  # Hot key Configuration
  # Platform Hot key Define
  # ScanCode, ShiftKey, AltKey, CtrlKey
  # ex:
  #    0x54, 0x0, 0x1, 0x0      F1(Combination Key ScanCode) + ShiftKey
  #    0x68, 0x0, 0x2, 0x0      F1(Combination Key ScanCode) + AltKey
  #    0x5f, 0x0, 0x4, 0x0      F1(Combination Key ScanCode) + CtrlKey
  #
  gInsydeTokenSpaceGuid.PcdPlatformKeyList|{ \
    0x3b, 0x0, 0x0, 0x0,                     \ # F1_KEY
    0x3c, 0x0, 0x0, 0x0,                     \ # F2_KEY
    0x53, 0x0, 0x0, 0x0,                     \ # DEL_KEY
    0x44, 0x0, 0x0, 0x0,                     \ # F10_KEY
    0x86, 0x0, 0x0, 0x0,                     \ # F12_KEY
    0x01, 0x0, 0x0, 0x0,                     \ # ESC_KEY
    0x40, 0x0, 0x0, 0x0,                     \ # UP_ARROW_KEY_BIT
    0x3d, 0x0, 0x0, 0x0,                     \ # F3_KEY
    0x43, 0x0, 0x0, 0x0,                     \ # F9_KEY
    0x00, 0x0, 0x0, 0x0}                       # EndEntry
  gChipsetPkgTokenSpaceGuid.PcdSmbiosType20PartitionRowPosition|0xFF
# Power On Demo bios Tseg is 0x1000000
# Demo BIOS Tseg size changes to 0x400000
# According to EDS, BIOS must program TSEGMB to a 8MB naturally aligned boundary, so change PcdSaTsegSize from 4MB to 8MB
  gChipsetPkgTokenSpaceGuid.PcdSaTsegSize|0x800000
  gSiPkgTokenSpaceGuid.PcdTraceHubEnable|1
  gInsydeTokenSpaceGuid.PcdScuFormsetGuidList|{ \
    GUID("C1E0B01A-607E-4B75-B8BB-0631ECFAACF2"), \ # Main
    GUID("C6D4769E-7F48-4D2A-98E9-87ADCCF35CCC"), \ # Avance
    GUID("5204F764-DF25-48A2-B337-9EC122B85E0D"), \ # Security
    GUID("A6712873-925F-46C6-90B4-A40F86A0917B"), \ # Power
    GUID("2D068309-12AC-45AB-9600-9187513CCDD8"), \ # Boot
    GUID("B6936426-FB04-4A7B-AA51-FD49397CDC01"), \ # Exit
    GUID("00000000-0000-0000-0000-000000000000")}

  gInsydeTokenSpaceGuid.PcdScuFormsetFlagList|{ \
    UINT8(0), \ # Main
    UINT8(0), \ # Avance
    UINT8(0), \ # Security
    UINT8(0), \ # Power
    UINT8(0), \ # Boot
    UINT8(0), \ # Exit
    UINT8(0xFF)}
#
#Register Ihisi sub function table list.
#Table struct define {CmdNumber, AsciiFuncGuid, Priority}
# UINT8(CmdNumber), Char8[20](AsciiFuncGuid), UINT8(Priority)
##================  ========================  ===============
gInsydeTokenSpaceGuid.PcdIhisiRegisterTable|{ \
  # Register IHISI AH=00h (VATSRead)
  UINT8(0x00),      "S00Kn_VatsRead00000",    UINT8(0x80), \

  # Register IHISI AH=01h (VATSWrite)
  UINT8(0x01),      "S01Kn_VatsWrite0000",    UINT8(0x80), \

  # Register IHISI AH=10h (FBTSGetSupportVersion)
  UINT8(0x10),      "S10Cs_GetPermission",    UINT8(0xE0), \
  UINT8(0x10),      "S10OemGetPermission",    UINT8(0xC0), \
  UINT8(0x10),      "S10OemGetAcStatus00",    UINT8(0xBB), \
  UINT8(0x10),      "S10OemBatterylife00",    UINT8(0xB6), \
  UINT8(0x10),      "S10Kn_GetVersion000",    UINT8(0x80), \
  UINT8(0x10),      "S10Kn_InitOemHelp00",    UINT8(0x7F), \
  UINT8(0x10),      "S10Kn_GetVendorID00",    UINT8(0x7E), \
  UINT8(0x10),      "S10Kn_GetBatteryLow",    UINT8(0x7D), \

  # Register IHISI AH=11h (FBTSGetPlatformInfo)
  UINT8(0x11),      "S11Kn_GetModelName0",    UINT8(0x80), \
  UINT8(0x11),      "S11Kn_GModelVersion",    UINT8(0x7F), \
  UINT8(0x11),      "S11OemFbtsApCheck00",    UINT8(0x40), \
  UINT8(0x11),      "S11Kn_UpExtPlatform",    UINT8(0x20), \

  # Register IHISI AH=12h (FBTSGetPlatformRomMap)
  UINT8(0x12),      "S12Kn_ProtectRomMap",    UINT8(0x80), \
  UINT8(0x12),      "S12Kn_PrivateRomMap",    UINT8(0x7F), \
  UINT8(0x12),      "S12Cs_PlatformRomMp",    UINT8(0x40), \
  UINT8(0x12),      "S12OemPlatformRomMp",    UINT8(0x20), \

  # Register IHISI AH=13h (FBTSGetFlashPartInfo)
  UINT8(0x13),      "S13Kn_FlashPartInfo",    UINT8(0x80), \

  # Register IHISI AH=14h (FBTSRead)
  UINT8(0x14),      "S14Cs_DoBeforeRead0",    UINT8(0xE0), \
  UINT8(0x14),      "S14Kn_FbtsReadProce",    UINT8(0x80), \
  UINT8(0x14),      "S14Cs_DoAfterRead00",    UINT8(0x20), \

  # Register IHISI AH=15h (FBTSWrite)
  UINT8(0x15),      "S15Cs_DoBeforeWrite",    UINT8(0xE0), \
  UINT8(0x15),      "S15Kn_FbtsWriteProc",    UINT8(0x80), \
  UINT8(0x15),      "S15Cs_DoAfterWrite0",    UINT8(0x40), \

  # Register IHISI AH=16h (FBTSComplete)
  UINT8(0x16),      "S16Cs_CApTerminalte",    UINT8(0xE0), \
  UINT8(0x16),      "S16Cs_CNormalFlash0",    UINT8(0xDF), \
  UINT8(0x16),      "S16Cs_CPartialFlash",    UINT8(0xDE), \
  UINT8(0x16),      "S16Kn_PurifyVariabl",    UINT8(0x80), \
  UINT8(0x16),      "S16Cs_FbtsComplete0",    UINT8(0x20), \
  UINT8(0x16),      "S16Cs_FbtsReboot000",    UINT8(0x1F), \
  UINT8(0x16),      "S16Cs_FbtsShutDown0",    UINT8(0x1E), \
  UINT8(0x16),      "S16Cs_FbtsDoNothing",    UINT8(0x1D), \

  # Register IHISI AH=17h (FBTSGetRomFileAndPlatformTable)
  UINT8(0x17),      "S17Cs_GetPlatformTb",    UINT8(0x80), \

  # Register IHISI AH=1Bh (FBTSSkipMcCheckAndBinaryTrans)
  UINT8(0x1B),      "S1BKn_SkipMcCheck00",    UINT8(0x80), \

  # Register IHISI AH=1Ch (FBTSGetATpInformation)
  UINT8(0x1C),      "S1CCs_GetATpInfo000",    UINT8(0x80), \

  # Register IHISI AH=1Eh (FBTSGetWholeBiosRomMap)
  UINT8(0x1E),      "S1EKn_WholeBiosRomp",    UINT8(0x80), \
  UINT8(0x1E),      "S1EOemWholeBiosRomp",    UINT8(0x40), \

  # Register IHISI AH=1Fh (FBTSApHookPoint)
  UINT8(0x1F),      "S1FKn_ApHookforBios",    UINT8(0x80), \
  UINT8(0x1F),      "S1FCs_ApHookForBios",    UINT8(0x40), \

  # Register IHISI AH=20h (FETSWrite)
  UINT8(0x20),      "S20OemDoBeforeWrite",    UINT8(0xE0), \
  UINT8(0x20),      "S20OemEcIdleTrue000",    UINT8(0xC0), \
  UINT8(0x20),      "S20OemFetsWrite0000",    UINT8(0x80), \
  UINT8(0x20),      "S20OemEcIdleFalse00",    UINT8(0x40), \
  UINT8(0x20),      "S20OemDoAfterWrite0",    UINT8(0x20), \
  UINT8(0x20),      "S20Cs_ShutdownMode0",    UINT8(0x1B), \

  # Register IHISI AH=21h (FETSGetEcPartInfo)
  UINT8(0x21),      "S21OemGetEcPartInfo",    UINT8(0x80), \

  # Register IHISI AH=41h (OEMSFOEMExCommunication)
  UINT8(0x41),      "S41Kn_CommuSaveRegs",    UINT8(0xFF), \
  UINT8(0x41),      "S41Cs_ExtDataCommun",    UINT8(0xE0), \
  UINT8(0x41),      "S41OemT01Vbios00000",    UINT8(0xC0), \
  UINT8(0x41),      "S41OemT54LogoUpdate",    UINT8(0xBB), \
  UINT8(0x41),      "S41OemT55CheckSignB",    UINT8(0xB6), \
  UINT8(0x41),      "S41OemReservedFun00",    UINT8(0xB1), \
  UINT8(0x41),      "S41Kn_T51EcIdelTrue",    UINT8(0x85), \
  UINT8(0x41),      "S41Kn_ExtDataCommun",    UINT8(0x80), \
  UINT8(0x41),      "S41Kn_T51EcIdelFals",    UINT8(0x7B), \
  UINT8(0x41),      "S41OemT50Oa30RWFun0",    UINT8(0x40), \

  # Register IHISI AH=42h (OEMSFOEMExDataWrite)
  UINT8(0x42),      "S42Cs_ExtDataWrite0",    UINT8(0xE0), \
  UINT8(0x42),      "S42Kn_T50EcIdelTrue",    UINT8(0x85), \
  UINT8(0x42),      "S42Kn_ExtDataWrite0",    UINT8(0x80), \
  UINT8(0x42),      "S42Kn_T50EcIdelFals",    UINT8(0x7B), \
  UINT8(0x42),      "S42Cs_DShutdownMode",    UINT8(0x20), \

  # Register IHISI AH=47h (OEMSFOEMExDataRead)
  UINT8(0x47),      "S47Cs_ExtDataRead00",    UINT8(0xE0), \
  UINT8(0x47),      "S47Kn_ExtDataRead00",    UINT8(0x80), \

  # Register IHISI AH=48h (FBTSOEMCapsuleSecureFlash)

  UINT8(0x48),      "S48Cs_CpSecureFlash",    UINT8(0xDE), \
  UINT8(0x48),      "S48Kn_CpSecureFlash",    UINT8(0x80), \

  # Register IHISI AH=49h (FBTSCommonCommunication)
  UINT8(0x49),      "S49Kn_ComDataCommun",    UINT8(0x80), \

  # Register IHISI AH=4Bh (FBTSCommonRead)
  UINT8(0x4B),      "S4BKn_ComDataRead00",    UINT8(0x80), \

  # Register IHISI AH=4Dh (FBTSPassImageFromTool)
  UINT8(0x4D),      "S4DCs_ImageCheck000",    UINT8(0xC0)  }

#
# These value are defined by RC (SkylakeSipkg) for SEC code.
#
!if $(MULTI_CONFIG_SUPPORT) == YES
  gSiPkgTokenSpaceGuid.PcdFlashMicrocodeFvBase|0xFFDB0000   # Must the same as Insyde PcdFlashNvStorageMicrocodeBase
!else
  gSiPkgTokenSpaceGuid.PcdFlashMicrocodeFvBase|0xFFDA0000   # Must the same as Insyde PcdFlashNvStorageMicrocodeBase
!endif
  gSiPkgTokenSpaceGuid.PcdFlashMicrocodeFvSize|0x000A0000   # Must the same as Insyde PcdFlashNvStorageMicrocodeSize
  gSiPkgTokenSpaceGuid.PcdNemCodeCacheBase|0x5C0000        # PcdNemCodeCacheBase is usally the same as PEI FV Offset
  
  gChipsetPkgTokenSpaceGuid.PcdNvmeRootPortAddress|0x00011d00
  
################################################################################
#
# Pcd Dynamic Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsDynamicDefault]
  #
  # Crisis File name definition
  #
  # New File Path Definition : //Volume_Label\\File_Path\\File_Name
  # Notice : "//" is signature that volume label start definition.
  #
  # Example path : //RECOVERY\\BIOS\\Current\\SHARKBAY.fd
  gInsydeTokenSpaceGuid.PcdPeiRecoveryFile|L"Skylake.fd"|VOID*|0x100
# GPIO Table
#  PchLp
#   BoardIdSkylakeA0Rvp3                    = PcdPeiGpioTable1
#   BoardIdSkylakeURvp7                     = PcdPeiGpioTable2
#  PchH
#   BoardIdSkylakeAioRvp10Crb               = PcdPeiGpioTable3
#  For OEM MB
#   if PcdDefaultBoardId = 0xFFFFFFFFFFFFFFFF (H2O_DEFAULT_BOARD_ID), alway load PcdPeiGpioTable1.
#  CRB ID: Please reference PlatformBoardid.h
#
# Example: If your MB is not intel CRB and customer hardware is referenced to A0 RVP3
#          PcdDefaultBoardId should fill 0xFFFFFFFFFFFFFFFF (This will affect GPIO setting)
#          PcdOemProjectReferenceIntelCrb should fill 0x24  (This will affect asl code)
#  gChipsetPkgTokenSpaceGuid.PcdDefaultBoardId|0xFFFFFFFFFFFFFFFF
  gChipsetPkgTokenSpaceGuid.PcdOemProjectReferenceIntelCrb|0x04

#typedef struct {
#  UINT32  GpioPad;
#  UINT8   PadMode;
#  UINT8   HostSoftPadOwn;
#  UINT8   Direction;
#  UINT8   OutputState;
#  UINT8   InterruptConfig;
#  UINT8   PowerConfig;
#  UINT8   ElectricalConfig;
#  UINT8   LockConfig;
#  UINT8   OtherSettings;
#  UINT8   Rsvdbits;
#} PCD_GPIO_CONFIG;
#
## SkyLake BoardIdSkylakeA0Rvp3 Or Default GPIO Table Data
#
  gChipsetPkgTokenSpaceGuid.PcdPeiGpioTable1|{ \
#                     |        |HostSoft|         |Output |Interrupt|Power |Electrical|Lock    |Other   |
#  Pad                | PadMode|PadOwn  |Direction| State | Config  |Config|Config    |Config  |Settings|Rsvdbits
#  (1)                |  (2)   |  (3)   |  (4)    |  (5)  | (6)     | (7)  |(8)       |   (9)  |(10)    |(11)
  UINT32(0x02000007),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A07 LPC_PIRQA#
  UINT32(0x02000008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A08 LPC_CLKRUN#
  UINT32(0x0200000A),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_A10 LPC_CLKOUT_LPC1
  UINT32(0x0200000B),    0x01,    0x03,    0x0B,    0x00,    0x31,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A11 LPC_PME#
  UINT32(0x0200000C),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A12 BM_BUSY#/ISH_GP6
  UINT32(0x0200000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A13 LPC_SUSWARN#/SUSPWRDNACK
  UINT32(0x0200000F),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_A15 LPC_SUS_ACK#
  UINT32(0x02000010),    0x03,    0x03,    0x0B,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A16 SD_1P8_SEL
  UINT32(0x02000011),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A17 SD_PWR_EN#/ISH_GP7
  UINT32(0x02000012),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A18 ISH_GP0
  UINT32(0x02000013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A19 ISH_GP1
  UINT32(0x02000014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A20 ISH_GP2
  UINT32(0x02000015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A21 ISH_GP3
  UINT32(0x02000016),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A22 ISH_GP4
  UINT32(0x02000017),    0x01,    0x03,    0x0B,    0x00,    0x31,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_A23 ISH_GP5
  UINT32(0x02010000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B00 CORE_VID0
  UINT32(0x02010001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B01 CORE_VID1
  UINT32(0x02010002),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B02 VRALERT#
  UINT32(0x02010003),    0x01,    0x03,    0x0B,    0x00,    0x31,    0x05,    0x01,    0x00,    0x00,    0x00, \# GPP_B03 CPU_GP2
  UINT32(0x02010004),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B04 CPU_GP3
  UINT32(0x02010005),    0x01,    0x03,    0x0B,    0x00,    0x31,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B05 SRCCLKREQ0#
  UINT32(0x0201000B),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B11 EXT_PWR_GATE#
  UINT32(0x0201000C),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B12 SLP_S0#
  UINT32(0x0201000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B13 PLTRST#
  UINT32(0x0201000E),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_B14 SPKR
  UINT32(0x02010010),    0x01,    0x01,    0x1B,    0x00,    0x29,    0x05,    0x01,    0x00,    0x00,    0x00, \# GPP_B16 GSPI0_CLK
  UINT32(0x02010011),    0x01,    0x01,    0x1B,    0x00,    0x69,    0x05,    0x01,    0x00,    0x00,    0x00, \# GPP_B17 GSPI0_MISO
  UINT32(0x02010012),    0x01,    0x01,    0x1B,    0x00,    0x29,    0x05,    0x19,    0x00,    0x00,    0x00, \# GPP_B18 GSPI0_MOSI
  UINT32(0x02010013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_B19 GSPI1_CS#
  UINT32(0x02010014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_B20 GSPI1_CLK
  UINT32(0x02010015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_B21 GSPI1_MISO
  UINT32(0x02010016),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_B22 GSPI1_MOSI
  UINT32(0x02010017),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_B23 SML1ALERT#/PCHHOT#
  UINT32(0x02020000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C00 SMBCLK
  UINT32(0x02020001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_C01 SMBDATA
  UINT32(0x02020002),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_C02 SMBALERT#
  UINT32(0x02020003),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C03 SML0CLK
  UINT32(0x02020004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C04 SML0DATA
  UINT32(0x02020005),    0x01,    0x03,    0x1B,    0x00,    0x31,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_C05 SML0ALERT#
  UINT32(0x02020006),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C06 SML1CLK
  UINT32(0x02020007),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_C07 SML1DATA
  UINT32(0x02020008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C08 UART0_RXD
  UINT32(0x02020009),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C09 UART0_TXD
  UINT32(0x0202000A),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C10 UART0_RTS#
  UINT32(0x0202000B),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C11 UART0_CTS#
  UINT32(0x0202000C),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C12 UART1_RXD/ISH_UART1_RXD
  UINT32(0x0202000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C13 UART1_TXD/ISH_UART1_TXD
  UINT32(0x0202000E),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C14 UART1_RTS#/ISH_UART1_RTS#
  UINT32(0x0202000F),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C15 UART1_CTS#/ISH_UART1_CTS#
  UINT32(0x02020010),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C16 I2C0_SDA
  UINT32(0x02020011),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C17 I2C0_SCL
  UINT32(0x02020012),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C18 I2C1_SDA
  UINT32(0x02020013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C19 I2C1_SCL
  UINT32(0x02020014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C20 UART2_RXD
  UINT32(0x02020015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C21 UART2_TXD
  UINT32(0x02020016),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C22 UART2_RTS#
  UINT32(0x02020017),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_C23 UART2_CTS#
  UINT32(0x02030000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D00 SPI1_CS#
  UINT32(0x02030001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D01 SPI1_CLK
  UINT32(0x02030002),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D02 None
  UINT32(0x02030003),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D03 None
  UINT32(0x02030004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D04 FLASHTRIG
  UINT32(0x02030005),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D05 ISH_I2C0_SDA
  UINT32(0x02030006),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D06 ISH_I2C0_SCL
  UINT32(0x02030007),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D07 ISH_I2C1_SDA
  UINT32(0x02030008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D08 ISH_I2C1_SCL
  UINT32(0x02030009),    0x01,    0x03,    0x0B,    0x00,    0x21,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D09 None
  UINT32(0x0203000A),    0x01,    0x03,    0x0B,    0x00,    0x21,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D10 None
  UINT32(0x0203000B),    0x01,    0x03,    0x0B,    0x00,    0x21,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D11 None
  UINT32(0x0203000C),    0x01,    0x03,    0x0B,    0x00,    0x21,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D12 None
  UINT32(0x0203000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D13 ISH_UART0_RXD
  UINT32(0x0203000E),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D14 ISH_UART0_TXD
  UINT32(0x0203000F),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D15 ISH_UART0_RTS#
  UINT32(0x02030010),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D16 ISH_UART0_CTS#
  UINT32(0x02030011),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D17 DMIC_CLK1
  UINT32(0x02030012),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D18 DMIC_DATA1
  UINT32(0x02030013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D19 DMIC_CLK0
  UINT32(0x02030014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D20 DMIC_DATA0
  UINT32(0x02030015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D21 None
  UINT32(0x02030016),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D22 None
  UINT32(0x02030017),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_D23 I2S_MCLK
  UINT32(0x02040000),    0x01,    0x03,    0x1B,    0x00,    0x71,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E00 SATAXPCIE0/SATAGP0
  UINT32(0x02040001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E01 SATAXPCIE1/SATAGP1
  UINT32(0x02040002),    0x01,    0x03,    0x07,    0x00,    0xB1,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E02 SATAXPCIE2/SATAGP2
  UINT32(0x02040003),    0x01,    0x03,    0x05,    0x03,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPP_E03 CPU_GP0
  UINT32(0x02040004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E04 SATA_DEVSLP0
  UINT32(0x02040005),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E05 SATA_DEVSLP1
  UINT32(0x02040008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E08 SATA_LED#
  UINT32(0x02040009),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E09 USB2_OC0#
  UINT32(0x0204000A),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E10 USB2_OC1#
  UINT32(0x0204000B),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E11 USB2_OC2#
  UINT32(0x0204000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E13 DDPB_HPD0
  UINT32(0x0204000E),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E14 DDPC_HPD1
  UINT32(0x0204000F),    0x01,    0x01,    0x1B,    0x00,    0x65,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E15 DDPD_HPD2
  UINT32(0x02040010),    0x01,    0x01,    0x1B,    0x00,    0x29,    0x05,    0x01,    0x00,    0x00,    0x00, \# GPP_E16 DDPE_HPD3
  UINT32(0x02040011),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E17 EDP_HPD
  UINT32(0x02040012),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E18 DDPB_CTRLCLK
  UINT32(0x02040013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_E19 DDPB_CTRLDATA
  UINT32(0x02040014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E20 DDPC_CTRLCLK
  UINT32(0x02040015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_E21 DDPC_CTRLDATA
  UINT32(0x02040016),    0x01,    0x03,    0x1B,    0x00,    0x31,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_E22 None
  UINT32(0x02040017),    0x01,    0x03,    0x05,    0x03,    0x01,    0x03,    0x09,    0x00,    0x00,    0x00, \# GPP_E23 None
  UINT32(0x02050000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F00 I2S2_SCLK
  UINT32(0x02050001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F01 I2S2_SFRM
  UINT32(0x02050002),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F02 I2S2_TXD
  UINT32(0x02050003),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F03 I2S2_RXD
  UINT32(0x02050004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F04 I2C2_SDA
  UINT32(0x02050005),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F05 I2C2_SCL
  UINT32(0x02050006),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F06 I2C3_SDA
  UINT32(0x02050007),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F07 I2C3_SCL
  UINT32(0x02050008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F08 I2C4_SDA
  UINT32(0x02050009),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F09 I2C4_SCL
  UINT32(0x0205000A),    0x05,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F10 I2C5_SDA/ISH_I2C2_SDA
  UINT32(0x0205000B),    0x05,    0x03,    0x07,    0x00,    0x01,    0x03,    0x61,    0x00,    0x00,    0x00, \# GPP_F11 I2C5_SCL/ISH_I2C2_SCL
  UINT32(0x0205000C),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F12 EMMC_CMD
  UINT32(0x0205000D),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F13 EMMC_DATA0
  UINT32(0x0205000E),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F14 EMMC_DATA1
  UINT32(0x0205000F),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F15 EMMC_DATA2
  UINT32(0x02050010),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F16 EMMC_DATA3
  UINT32(0x02050011),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F17 EMMC_DATA4
  UINT32(0x02050012),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F18 EMMC_DATA5
  UINT32(0x02050013),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F19 EMMC_DATA6
  UINT32(0x02050014),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F20 EMMC_DATA7
  UINT32(0x02050015),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F21 EMMC_RCLK
  UINT32(0x02050016),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F22 EMMC_CLK
  UINT32(0x02050017),    0x01,    0x03,    0x0B,    0x00,    0x31,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_F23 None
  UINT32(0x02060000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G00 SD_CMD
  UINT32(0x02060001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G01 SD_DATA0
  UINT32(0x02060002),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G02 SD_DATA1
  UINT32(0x02060003),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G03 SD_DATA2
  UINT32(0x02060004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G04 SD_DATA3
  UINT32(0x02060005),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G05 SD_CD#
  UINT32(0x02060006),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G06 SD_CLK
  UINT32(0x02060007),    0x03,    0x03,    0x07,    0x00,    0x01,    0x03,    0x01,    0x00,    0x00,    0x00, \# GPP_G07 SD_WP
  UINT32(0x02070000),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD00 BATLOW#
  UINT32(0x02070001),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD01 ACPRESENT
  UINT32(0x02070002),    0x03,    0x01,    0x0B,    0x00,    0x29,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD02 LAN_WAKE#
  UINT32(0x02070003),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x19,    0x00,    0x00,    0x00, \# GPD03 PWRBTN#
  UINT32(0x02070004),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD04 SLP_S3#
  UINT32(0x02070005),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD05 SLP_S4#
  UINT32(0x02070006),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD06 SLP_A#
  UINT32(0x02070007),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD07 Reserved Functionality
  UINT32(0x02070008),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD08 SUSCLK
  UINT32(0x02070009),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD09 SLP_WLAN#
  UINT32(0x0207000A),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00, \# GPD10 SLP_S5#
  UINT32(0x0207000B),    0x03,    0x03,    0x07,    0x00,    0x01,    0x01,    0x01,    0x00,    0x00,    0x00  \# GPD11 LANPHYPC
  }

#   UINT16 Address  UINT16 Length
  gChipsetPkgTokenSpaceGuid.PcdPchLpcGenIoDecodeTable|{ \   # LPC offset 84h-93h
    UINT16(0x0000), UINT16(0x0000), \
    UINT16(0x0000), UINT16(0x0000), \
    UINT16(0x0000), UINT16(0x0000), \
    UINT16(0x0000), UINT16(0x0000)}
################################################################################
#
# SMBIOS Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicExDefault]
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType000|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType001|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType002|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType003|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType008|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType009|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType011|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType012|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType013|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType015|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType021|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType022|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType024|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType026|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType027|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType028|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType032|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType039|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType040|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType041|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType128|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType129|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType130|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType131|TRUE
  gSmbiosTokenSpaceGuid.PcdActiveSmbiosType136|TRUE
  gSmbiosTokenSpaceGuid.PcdSmbiosMaxMultiRecords |32
  gSmbiosTokenSpaceGuid.PcdSmbiosMultiRecordsType|{2, 3, 4, 7, 8, 9, 17, 21, 22, 26, 27, 28, 29, 39, 41}
  gSmbiosTokenSpaceGuid.PcdType000Record | { \
      0x00,                       \ # Type
      0x00,                       \ # Length
      UINT16(0x0000),             \ # Handle
      0xFF,                       \ # Vendor
      0xFF,                       \ # BIOS Version
      UINT16(0xE000),             \ # BIOS Starting Address Segment
      0xFF,                       \ # BIOS Release Date
      0xFF,                       \ # BIOS ROM Size
      UINT64(0x000000004BF99880), \ # BIOS Characteristics
      UINT16(0x0D03),             \ # BIOS Characteristics Extension Bytes
      0xFF,                       \ # System BIOS Major Release
      0xFF,                       \ # System BIOS Minor Release
      0xFF,                       \ # Embedded Controller Firmware Major Release
      0xFF                        \ # Embedded Controller Firmware Minor Release
  }
  gSmbiosTokenSpaceGuid.PcdType000Strings|"Insyde;05.21.11.0000;03/14/2016;"
  gSmbiosTokenSpaceGuid.PcdType001Record |{0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x78, 0x56, 0x34, 0x12, 0x34, 0x12, 0x78, 0x56, 0x90, 0xab, 0xcd, 0xde, 0xef, 0xaa, 0xbb, 0xcc, 0x06, 0x05, 0x06}
  gSmbiosTokenSpaceGuid.PcdType001Strings|"Insyde;Skylake;TBD by OEM;123456789;Type1Sku0;Type1Family;"
  gSmbiosTokenSpaceGuid.PcdType002Record000 |{0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x09, 0x06, 0xFF, 0xFF, 0x0A, 0x00}
  gSmbiosTokenSpaceGuid.PcdType002Strings000|"Type2 - Board Vendor Name1;Type2 - Board Product Name1;Type2 - Board Version;Type2 - Board Serial Number;Type2 - Board Asset Tag;Type2 - Board Chassis Location;"
  gSmbiosTokenSpaceGuid.PcdType003Record000 |{0x03, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x02, 0x03, 0x04, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x05}
  gSmbiosTokenSpaceGuid.PcdType003Strings000|"Chassis Manufacturer;Chassis Version;Chassis Serial Number;Chassis Asset Tag;SKU Number;"
  gSmbiosTokenSpaceGuid.PcdType008Record000|{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x0F, 0x0D}
  gSmbiosTokenSpaceGuid.PcdType008Strings000|"J1A1;Keyboard;"
  gSmbiosTokenSpaceGuid.PcdType008Record001 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x0F, 0x0E}
  gSmbiosTokenSpaceGuid.PcdType008Strings001|"J1A1;Mouse;"
  gSmbiosTokenSpaceGuid.PcdType008Record002 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x0D, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings002|"J2A1;TV OUT;"
  gSmbiosTokenSpaceGuid.PcdType008Record003 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x07, 0x1C}
  gSmbiosTokenSpaceGuid.PcdType008Strings003|"J2A2;CRT;"
  gSmbiosTokenSpaceGuid.PcdType008Record004 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x08, 0x09}
  gSmbiosTokenSpaceGuid.PcdType008Strings004|"J2A2;COM 1;"
  gSmbiosTokenSpaceGuid.PcdType008Record005 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings005|"J3A1;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record006 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings006|"J3A1;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record007 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings007|"J3A1;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record008 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings008|"J5A1;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record009 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings009|"J5A1;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record010 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x10}
  gSmbiosTokenSpaceGuid.PcdType008Strings010|"J5A2;USB;"
  gSmbiosTokenSpaceGuid.PcdType008Record011 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x0B, 0x1F}
  gSmbiosTokenSpaceGuid.PcdType008Strings011|"J5A1;Network;"
  gSmbiosTokenSpaceGuid.PcdType008Record012 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x17, 0x02, 0x00, 0xFF}
  gSmbiosTokenSpaceGuid.PcdType008Strings012|"J9G2;OnBoard Floppy Type;"
  gSmbiosTokenSpaceGuid.PcdType008Record013 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x16, 0x02, 0x00, 0xFF}
  gSmbiosTokenSpaceGuid.PcdType008Strings013|"J7J1;OnBoard Primary IDE;"
  gSmbiosTokenSpaceGuid.PcdType008Record014 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x1F, 0x1D}
  gSmbiosTokenSpaceGuid.PcdType008Strings014|"J30;Microphone In;"
  gSmbiosTokenSpaceGuid.PcdType008Record015 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x1F, 0x1D}
  gSmbiosTokenSpaceGuid.PcdType008Strings015|"J30;Line In;"
  gSmbiosTokenSpaceGuid.PcdType008Record016 |{0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x1F, 0x1D}
  gSmbiosTokenSpaceGuid.PcdType008Strings016|"J30;Speaker Out;"
  gSmbiosTokenSpaceGuid.PcdType009Record000 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xA6, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xE0}
  gSmbiosTokenSpaceGuid.PcdType009Strings000|"J6C1;"
  gSmbiosTokenSpaceGuid.PcdType009Record001 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xA6, 0x08, 0x00, 0x01, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xE1}
  gSmbiosTokenSpaceGuid.PcdType009Strings001|"J6D2;"
  gSmbiosTokenSpaceGuid.PcdType009Record002 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xA6, 0x08, 0x00, 0x01, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xE2}
  gSmbiosTokenSpaceGuid.PcdType009Strings002|"J7C1;"
  gSmbiosTokenSpaceGuid.PcdType009Record003 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xA6, 0x08, 0x00, 0x01, 0x04, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xE3}
  gSmbiosTokenSpaceGuid.PcdType009Strings003|"J7D1;"
  gSmbiosTokenSpaceGuid.PcdType009Record004 |{0x09, 0x00, 0x00, 0x00, 0x01, 0xA8, 0x0A, 0x00, 0x01, 0x05, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xE4}
  gSmbiosTokenSpaceGuid.PcdType009Strings004|"J8C1;"
  gSmbiosTokenSpaceGuid.PcdType011Record |{0x0B, 0x00, 0x00, 0x00, 0x03}
  gSmbiosTokenSpaceGuid.PcdType011Strings|"OemString1;OemString2;OemString3;"
  gSmbiosTokenSpaceGuid.PcdType012Record |{0x0C, 0x00, 0x00, 0x00, 0x03}
  gSmbiosTokenSpaceGuid.PcdType012Strings|"ConfigOptions1;ConfigOptions2;ConfigOptions3;"
  gSmbiosTokenSpaceGuid.PcdType013Record |{0x0D, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}
  gSmbiosTokenSpaceGuid.PcdType013Strings|"en|US|iso8859-1,0;fr|FR|iso8859-1,0;zh|TW|unicode,0;ja|JP|unicode,0;it|IT|iso8859-1,0;es|ES|iso8859-1,0;de|DE|iso8859-1,0;pt|PT|iso8859-1,0;"
  #gSmbiosTokenSpaceGuid.PcdType013Record |{0x0D, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}
  #gSmbiosTokenSpaceGuid.PcdType013Strings|"enUS,0;frFR,0;zhTW,0;jaJP,0;itIT,0;esES,0;deDE,0;ptPT,0;"
  gSmbiosTokenSpaceGuid.PcdType015Record |{0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x01, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x02, 0x07, 0x00, 0x08, 0x04, 0x16, 0x00}
  gSmbiosTokenSpaceGuid.PcdType021Record000 |{0x15, 0x00, 0x00, 0x00, 0x07, 0x04, 0x04}
  gSmbiosTokenSpaceGuid.PcdType022Record000 |{0x16, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType022Strings000|"Fake;-Virtual Battery 0-;08/08/2010;Battery 0;CRB Battery 0;;LithiumPolymer;"
  gSmbiosTokenSpaceGuid.PcdType024Record |{0x18, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType026Record000 |{0x1A, 0x00, 0x00, 0x00, 0x01, 0x42, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}
  gSmbiosTokenSpaceGuid.PcdType026Strings000|"Voltage Probe Description;"
  gSmbiosTokenSpaceGuid.PcdType027Record000 |{0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01}
  gSmbiosTokenSpaceGuid.PcdType027Strings000|"Cooling Device Description;"
  gSmbiosTokenSpaceGuid.PcdType028Record000 |{0x1C, 0x00, 0x00, 0x00, 0x01, 0x42, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}
  gSmbiosTokenSpaceGuid.PcdType028Strings000|"Temperature Probe Description;"
  gSmbiosTokenSpaceGuid.PcdType032Record |{0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType039Record000 |{0x27, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x4B, 0x00, 0xA4, 0x21, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
  gSmbiosTokenSpaceGuid.PcdType039Strings000|"OEM Define 0;OEM Define 1;OEM Define 2;OEM Define 3;OEM Define 4;OEM Define 5;OEM Define 6;"
  gSmbiosTokenSpaceGuid.PcdType040Record |{0x28, 0x00, 0x00, 0x00, 0x02, 0x06, 0x09, 0x00, 0x05, 0x01, 0xAA, 0x06, 0x00, 0x00, 0x05, 0x02, 0x00}
  gSmbiosTokenSpaceGuid.PcdType040Strings|"PCIExpressx16;Compiler Version: VC 9.0;"
  gSmbiosTokenSpaceGuid.PcdType041Record000 |{0x29, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x10}
  gSmbiosTokenSpaceGuid.PcdType041Strings000|"IGD;"
  gSmbiosTokenSpaceGuid.PcdType128Record |{0x80, 0x00, 0x00, 0x00, 0x55, 0xAA, 0x55, 0xAA}
  gSmbiosTokenSpaceGuid.PcdType128Strings|"Oem Type 128 Test 1;Oem Type 128 Test 2;"
  gSmbiosTokenSpaceGuid.PcdType129Record |{0x81, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x01}
  gSmbiosTokenSpaceGuid.PcdType129Strings|"Insyde_ASF_001;Insyde_ASF_002;"
  gSmbiosTokenSpaceGuid.PcdType130Record |{0x82, 0x00, 0x00, 0x00, 0x24, 0x41, 0x4D, 0x54, 0x01, 0x01, 0x01, 0x01, 0x01, 0xA5, 0xBF, 0x02, 0xC0, 0x00, 0x01, 0x00}
  gSmbiosTokenSpaceGuid.PcdType131Record | {0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x76, 0x50, 0x72, 0x6F, 0x00, 0x00, 0x00, 0x00}
  gSmbiosTokenSpaceGuid.PcdType136Record |{0x88, 0x00, 0x00, 0x00, 0xFF, 0xFF}
#
#  GPIO Controller Collection
#  --Pch L--
#  PID_GPIOCOM0  A   [0]
#  PID_GPIOCOM0  B   [1]
#  PID_GPIOCOM1  C   [2]
#  PID_GPIOCOM1  D   [3]
#  PID_GPIOCOM1  E   [4]
#  PID_GPIOCOM3  F   [5]
#  PID_GPIOCOM3  G   [6]
#  PID_GPIOCOM2  GPD [7]
#  --Pch H--
#  PID_GPIOCOM0  A   [0]
#  PID_GPIOCOM0  B   [1]
#  PID_GPIOCOM1  C   [2]
#  PID_GPIOCOM1  D   [3]
#  PID_GPIOCOM1  E   [4]
#  PID_GPIOCOM1  F   [5]
#  PID_GPIOCOM1  G   [6]
#  PID_GPIOCOM1  H   [7]
#  PID_GPIOCOM3  I   [8]
#  PID_GPIOCOM2  GPD [9]

  gI2cDeviceTokenSpaceGuid.PcdI2cTouchpad|{                                                 \  # The definition of I2C mouse PCD
    GUID({  0x234124E9,0x40B7,0x43EF,{0x9B,0x5E,0x97,0x47,0x08,0x08,0xD4,0x40}}),           \  # The unique GUID specific for this device, it will be part of device path node
    UINT32(0x00000020),                                                                     \  # Slave address
    UINT32(0x00000001),                                                                     \  # Hardware revision
    0x00,                                                                                   \  # Interrupt GPIO pin active level, 0 = low active, 1 = high active
    0x03,                                                                                   \  # Interrupt GPIO pin number
    UINT16(0x0020),                                                                         \  # HID descriptor register number
    UINT16(0x0102),                                                                         \  # HID device type, 0x0000 = Non-HID device, 0x0d00 = Touch panel, 0x0102 = Mouse, 0x0106 = Keyboard
    0x00,                                                                                   \  # Host controller number, 0 based
    0x01,                                                                                   \  # Bus configuration, 0x00 = V_SPEED_STANDARD, 0x01 = V_SPEED_FAST, 0x02 = V_SPEED_HIGH
    0x01                                                                                    \  # GPIO controller 0 based,
  }
[Libraries]

[LibraryClasses]
  HidDescriptorLib|InsydeModulePkg/Library/HidDescriptorLib/HidDescriptorLib.inf
  BaseOemSvcKernelLib|$(PROJECT_PKG)/Library/BaseOemSvcKernelLib/BaseOemSvcKernelLib.inf
  BaseOemSvcChipsetLib|$(PROJECT_PKG)/Library/BaseOemSvcChipsetLib/BaseOemSvcChipsetLib.inf
  StdLib|InsydeModulePkg/Library/StdLib/StdLib.inf
[LibraryClasses.common.SEC]

[LibraryClasses.common.PEI_CORE]
  PeiOemSvcKernelLib|$(PROJECT_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf
  PeiOemSvcChipsetLib|$(PROJECT_PKG)/Library/PeiOemSvcChipsetLib/PeiOemSvcChipsetLib.inf

[LibraryClasses.common.PEIM]
  PeiOemSvcKernelLib|$(PROJECT_PKG)/Library/PeiOemSvcKernelLib/PeiOemSvcKernelLib.inf
  PeiOemSvcChipsetLib|$(PROJECT_PKG)/Library/PeiOemSvcChipsetLib/PeiOemSvcChipsetLib.inf

[LibraryClasses.common.DXE_CORE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[LibraryClasses.common.DXE_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf

[LibraryClasses.common.COMBINED_SMM_DXE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf

[LibraryClasses.common.SMM_CORE]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
  SmmOemSvcKernelLib|$(PROJECT_PKG)/Library/SmmOemSvcKernelLib/SmmOemSvcKernelLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  DxeOemSvcKernelLib|$(PROJECT_PKG)/Library/DxeOemSvcKernelLib/DxeOemSvcKernelLib.inf
################################################################################
#
# Platform related components
#
################################################################################
[Components.$(PEI_ARCH)]
!if gSioGuid.PcdSioDummySupported
  !disable SioDummyPkg/SioDummyPei/SioDummyPei.inf
!endif

#
# Sample code to ignore security sign check by case
#
  !disable InsydeModulePkg/Universal/Security/SecureFlash/FirmwareAuthenticationPei/FirmwareAuthenticationPei.inf
  InsydeModulePkg/Universal/Security/SecureFlash/FirmwareAuthenticationPei/FirmwareAuthenticationPei.inf {
    <LibraryClasses>
    FirmwareAuthenticationLib|InsydeModulePkg/Library/FirmwareAuthenticationLibNull/FirmwareAuthenticationLibNull.inf
  }

[Components.$(DXE_ARCH)]

  !disable MdeModulePkg/Core/Dxe/DxeMain.inf
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
    CpuExceptionHandlerLib|InsydeModulePkg/Library/CpuExceptionHandlerLib/ThunkCpuExceptionHandlerLib.inf
    <SOURCE_OVERRIDE_PATH>
    MdeModulePkg/Override/Core/Dxe
  }
#
# AcpiTablesDxe.inf's files come from
# 1. RC asl/acpi
#   1.1 $(PLATFORM_SI_PACKAGE)/AcpiTables
#   1.2 $(PLATFORM_SI_PACKAGE)/Pch/AcpiTables/Dsdt
#   1.3 $(PLATFORM_SI_PACKAGE)/SystemAgent/AcpiTables
# 2. Insyde and RC sample asl/acpi
#   2.1 $(CHIPSET_PKG)/AcpiTablesDxe#
# Uncomment this code when you want to override chipset's asl code
#
# !disable $(PLATFORM_SI_PACKAGE)/AcpiTables/AcpiTablesDxe.inf
# $(PLATFORM_SI_PACKAGE)/AcpiTables/AcpiTablesDxe.inf {
#   <SOURCE_OVERRIDE_PATH>
#     $(PLATFORM_SI_PACKAGE)/Pch/AcpiTables/Dsdt
#     $(PLATFORM_SI_PACKAGE)/SystemAgent/AcpiTables
#     $(CHIPSET_PKG)/AcpiTablesDxe
#     $(PROJECT_PKG)/Override/$(PLATFORM_SI_PACKAGE)/AcpiTables
# }

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions.Common.EDK]
!if $(INSYDE_DEBUGGER) == YES
  *_*_IA32_CC_FLAGS    = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
!else
  *_*_IA32_CC_FLAGS    = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
!endif
  *_*_IA32_ASM_FLAGS   = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(ASM_FLAGS)
  *_*_IA32_VFRPP_FLAGS = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_IA32_APP_FLAGS   = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_IA32_PP_FLAGS    = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_IA32_ASLPP_FLAGS = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)

!if $(INSYDE_DEBUGGER) == YES
  *_*_X64_CC_FLAGS     = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS) /wd4028 /wd4031 /wd4334 /wd4718 /wd4090 /wd4213 -D EFI_FIRMWARE_VENDOR="L\"INSYDE Corp.\""  -D MDE_CPU_X64 -D PCH_DEBUG_INFO=1 /Od
!else
  *_*_X64_CC_FLAGS     = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS) /wd4028 /wd4031 /wd4334 /wd4718 /wd4090 /wd4213 -D EFI_FIRMWARE_VENDOR="L\"INSYDE Corp.\""  -D MDE_CPU_X64 -D PCH_DEBUG_INFO=1
!endif
  *_*_X64_ASM_FLAGS    = -DEFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(ASM_FLAGS)
  *_*_X64_VFRPP_FLAGS  = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_X64_APP_FLAGS    = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_X64_PP_FLAGS     = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_X64_ASLPP_FLAGS  = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)
  *_*_X64_ASLCC_FLAGS  = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(CC_FLAGS)

  *_*_*_VFR_FLAGS   =
  *_*_*_BUILD_FLAGS = -s

[BuildOptions.common.EDKII]
  GCC:*_*_IA32_CC_FLAGS          = -Wno-error -Wno-unused-local-typedefs -Wno-pointer-to-int-cast -Wno-unused-function -Wno-parentheses -DMDEPKG_NDEBUG $(RC_FLAGS)
  GCC:*_*_X64_CC_FLAGS           = -Wno-error -DMDEPKG_NDEBUG $(RC_FLAGS)
  GCC:*_*_IA32_JWASM_FLAGS       =
  GCC:*_*_X64_JWASM_FLAGS        =
  INTEL:*_*_*_CC_FLAGS           = /D MDEPKG_NDEBUG $(RC_FLAGS)
  MSFT:RELEASE_*_*_CC_FLAGS      = /D MDEPKG_NDEBUG $(RC_FLAGS)
!if $(EFI_DEBUG) == NO
  MSFT:DEBUG_*_*_CC_FLAGS        = /D MDEPKG_NDEBUG $(RC_FLAGS)
!else
  MSFT:DEBUG_*_*_CC_FLAGS        = $(RC_FLAGS)
!endif
  *_*_*_VFRPP_FLAGS         = $(CC_FLAGS)
  *_*_X64_ASLPP_FLAGS  = $(RC_FLAGS)
  *_*_X64_ASLCC_FLAGS  = $(RC_FLAGS)
