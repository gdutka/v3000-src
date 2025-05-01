IntelTGHDDPassword
   Provide sample code to support Intel TG device's HDD Password feature.
SMARTStatusCodeReportHandlerSample
   Provide a sample code for adding a SMART report status code handler.
ChangeFontWhenChangingLanguage
   Provide a sample code to change font when user change language on SCU. 
   It also can change font during POST time. 
ASM106XCompatSettings
    Provide a sample code that follows ASMEDIA software programming supplement(rev 1.0) and ASM106X system BIOS programming notes(v3.2).
    Please add the driver to your project if needed.  This driver is mainly for ASMEdia 106X SATA controller's optimal compatibility settings. 
HddSpindown (SMM phase)
    Provide protocol interface in SMM to issue "STANDBY IMMEDIATE" command to ATA device in IDE/AHCI mode. 
    This is the sample code for register a protocol in SMM.
EmmcFieldFirmwareUpdate
    This is the sample code for eMMC 5.0 FFU process that uses the Firmware Management Protocol distributed by SDHCD
CustomizeNtEmulator
    This is the sample code for customize NtEmulatorPkg. 
    User need copy a project package to $(WORKSPACE), and then modifies Project.xxx to assign chipset / platform package.
    If needed, modifies Include/SaCommonDefinitions.h to avoid MmPci macro access invalid address in NtEmulator. 
    ChipsetWinNtDriverDxe/Pei provide dummy protocol instance for SetupUtilityDxe dependency.
    nmake nt32 : build code 
    nmake run  : execute emulator
UpdateEmmcBaseAddressForS3
    Since some platform (e.g. BayTrail-M) will change the base address of EMMC host controller before booting OS, 
    which cause H2O SDHC driver can't access the device via the base address recorded while POST when S3 resuming, and lead a password-locked EMMC device can't be unlocked on S3 resume.
    This is the sample code that describes how to update the modified base address of EMMC host controller for S3 resume on BayTrail-M.
FotaIgnoreSecurityCheck
    This sample code will default adopt a empty FirmwareAuthenticationLib to ignore security check at PEI phase. Project able to replace this library
    to verify specific image if needed.
MultiBoardExpandFd
	When use command "pb uefi64" to build for MultiBoard feature, the FD size will be expanded from 7MB to 8MB. The ME must be re-flashed for 8MB BIOS.
	This folder includes ME "Skylake_C0_05.21.39_11.0.10.1002.bin" for 8MB BIOS. It only supports Skylake U RVP7 (Board: SKL-U A0 DDR3L RVP7 FAB1) currently.
Usage of VerbTableCfg
    To use this mechanism to get verbtable
    1. Must have these three pcd turn on in Project.cfg and set Layout = PackUp or PackDown in your [Fd.] section of Project.fdf.

       1)gInsydeTokenSpaceGuid.PcdH2OMultiBoardSupported=TRUE
           Manage current multiboard project config, in particular, VerbTableCfg need expanded space managed by this pcd.

       2)gInsydeTokenSpaceGuid.PcdH2OMultiBoardVerbTableCfgSupported=TRUE
           Manage VerbTableCfg related ".inf" and "switch tools behavior"
           Important : For project who doesn't support VerbTableCfg, this pcd must set FALSE to make sure build process clean.

       3)gInsydeTokenSpaceGuid.PcdH2OBuildAutoAdjust=TRUE
           Set with "Layout = PackUp or PackDown", then it will enable region resize mechanism.

    2. Must have user editable GenVerbTableCfgRegionData folder and its inf added into your project.

    3. Must use GetVerbTable() produced by VerbTableCfgLib to get COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER in your HDA init code.

    4. The example in SamplePkg shows how to use this lib in InstallPchHdaVerbTablePei.

    5. The example in SamplePkg shows where we include the VerbTableSubFdf file in a expanded 8M flash layout.
       User may include VerbTableSubFdf to the place where contains at least 6 block alignment empty space in their flash layout.

    6. For user who want to get verbtable directly in chipset init code.
       The place where to add this lib depends on where the HDA init code is in.
       To do this, user must have a concept that what this lib provide is a COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER.

    7. For more detail, please refer to P:\Engineering\ARM\Presentation_CTO_Project\Multi-Boards\VerbTableCfg_1103.ppt
H2OCheckpoint
	Sample code about how to add new checkpoint in different four different phases (PEI, DXE, SMM, BDS).
	Can find corresponding signature H2OCpSamplePei, H2OCpSampleDxe, H2OCpSampleSmm, H2OCpSampleBds for different phase sample code.
H2OSerialConfig
    Because Chipset will override SerialPortLib, so it should make sure final SerialPortLib is InsydeModulePkg's. 
    Can find the chipset modified in sample code. 
    When user implement customize BaseH2OSerialConfigLib and PeiH2OSerialConfigLib, it should be care used library dependency and service. 
    In some time, the PeiService or PEI hob isn't ready.
ReadPcdValuesFromAsl
    This is the sample code for use the method to read PCD values from ASL.
ReadPcdValuesFromVfr
    This is the sample code for use the method to read PCD values from VFR.
BoardSupportedMacro
    This is the sample code for use the two macros H2O_BOARD_SUPPORTED and H2O_BOARD_NAME_SUPPORTED in ASL code.
    The two macros also can be used in the .dsc, .fdf and .c files.
    For more detail, please refer to P:\Engineering\Kernel\Core\H2O 5.0\InsydeH2O Build Technical Reference 130.docx
IgnoreDeviceSample
    Provide sample code on how to use gInsydeTokenSpaceGuid.PcdH2OSataIgnoredDeviceList for NVME.       
Bh720Support
    A sample code to demonstrate how to enable BH720 to be able to work with H2O SDHC driver.
BdsCpUpdateMacAddress
    This is a sample driver to update MAC address.
    MAC modification check point function:
      This CP has been registered in MnpDxe driver.
      It will trigger to find the request if exist.
      Sample BdsCpUpdateMacAddress driver will check the Variable to do MAC address modification.
  
      You could register the MAC modification via shell command to set variable.
      
      setvar ModifyMacAddrVarString -guid 6efd5d42-7951-4396-b223-7f6cdc6066d8 -bs -nv =068888888887889049FA55660006000EC6AA5352000EC6AA55660600E04C6814A100E04C6888AA00

        06 888888888788 9049FA556600 06 000EC6AA5352 000EC6AA5566 06 00E04C6814A1 00E04C6888AA 00
        In this sample:  
        Variable content:
         Len    Original     Modify
        1.   06     888888888788 9049FA556600    
        2.   06     000EC6AA5352 000EC6AA5566
        3.   06     00E04C6814A1 00E04C6888AA
        4.   00  (END)
ControlNetworkStack
    Sample to demonstrate how to override policy in NetworkLocker protocol.
    You could choice UNDI device to append override network policy by MAC address.
    To set variable follow below command in shell.
    setvar PolicyOverride -guid 0d5e6bad-c9b5-47c2-b788-c756be54eadd -bs -nv =0688888888878806000EC6AA53520600E04C6814A100
    
    06 888888888788 06 000EC6AA5352 06 00E04C6814A1 00
    In this sample, devices with the same MAC address will be installed override policy on it's device handle.
      Variable content:
          Len      Original
      1.   06     888888888788
      2.   06     000EC6AA5352
      3.   06     00E04C6814A1
      4.   00  (END)