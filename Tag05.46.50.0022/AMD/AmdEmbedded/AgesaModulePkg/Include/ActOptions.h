/******************************************************************************
 *   Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************/

#ifndef ACT_OPTIONS_
#define ACT_OPTIONS_

/**
 * @brief   OC Mode                                          0x2DDABC0F
 * @details
 *    Select overclock operation modes
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >
 **/
#define    ACT_CFG_UID_CmnCpuOcMode                                              0x2DDABC0F

/**
 * @brief   Custom Pstate0                                   0x2E87F71B
 * @details
 *    Disable - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP0                                            0x2E87F71B

/**
 * @brief   Custom Pstate1                                   0x03129096
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP1                                            0x03129096

/**
 * @brief   Custom Pstate2                                   0x88D1AFF8
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP2                                            0x88D1AFF8

/**
 * @brief   Custom Pstate3                                   0xE7D93C54
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP3                                            0xE7D93C54

/**
 * @brief   Custom Pstate4                                   0x747C0D9E
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP4                                            0x747C0D9E

/**
 * @brief   Custom Pstate5                                   0x8445A852
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP5                                            0x8445A852

/**
 * @brief   Custom Pstate6                                   0x005A8312
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP6                                            0x005A8312

/**
 * @brief   Custom Pstate7                                   0x7E6C3138
 * @details
 *    Disabled - disable this Pstate
 *    Custom - customize this Pstate, applicable only if PcdOcDisable=FALSE
 *    WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF
 *    SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR
 *    AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S
 *    WARRANTY.
 *    Operating your AMD processor outside of specification or in excess of
 *    factory settings, including but not limited to overclocking, may damage
 *    or shorten the life of your processor or other system components, create
 *    system instabilities (e.g., data loss and corrupted images) and in
 *    extreme cases may result in total system failure. AMD does not provide
 *    support or service for issues or damages related to use of an AMD
 *    processor outside of processor specifications or in excess of factory
 *    settings.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  Custom Core Pstates >  Pstate_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CpuPstCustomP7                                            0x7E6C3138

/**
 * @brief   CCD Control                                      0x2794DDB8
 * @details
 *    Sets the number of active CCDs.  Once this option has been used to remove
 *    any CCDs, a POWER CYCLE is required in order for future selections to
 *    take effect.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  CCD/Core/Thread Enablement >
 **/
#define    ACT_CFG_UID_CpuCcdCtrlSsp                                             0x2794DDB8

/**
 * @brief   Core control                                     0xAE4E8917
 * @details
 *    Sets the number of cores to be used. Once this option has been used to
 *    remove any cores, a POWER CYCLE is required in order for future
 *    selections to take effect.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >  CCD/Core/Thread Enablement >
 **/
#define    ACT_CFG_UID_CpuCoreCtrl                                               0xAE4E8917

/**
 * @brief   SMT Control                                      0xD3AE4439
 * @details
 *    Can be used to disable symmetric multithreading. To re-enable SMT, a
 *    POWER CYCLE is needed after selecting the "Enable" option. Select "Auto"
 *    base on BIOS PCD (PcdAmdSmtMode) default setting. WARNING - S3 is NOT
 *    SUPPORTED on systems where SMT is disabled.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Performance >
 **/
#define    ACT_CFG_UID_CpuSmtCtrl                                                0xD3AE4439

/**
 * @brief   REP-MOV/STOS Streaming                           0x7735D64F
 * @details
 *    Allow REP-MOVS/STOS to use non-caching streaming stores for large sizes
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuRMSS                                                0x7735D64F

/**
 * @brief   L1 Stream HW Prefetcher                          0x62B78DF1
 * @details
 *    Option to Enable | Disable L1 Stream HW Prefetcher
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Prefetcher settings >
 **/
#define    ACT_CFG_UID_CmnCpuL1StreamHwPrefetcher                                0x62B78DF1

/**
 * @brief   L1 Stride Prefetcher                             0xFBD77A6D
 * @details
 *    Uses memory access history of individual instructions to fetch additional
 *    lines when each access is a constant distance from the previous.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Prefetcher settings >
 **/
#define    ACT_CFG_UID_CmnCpuL1StridePrefetcher                                  0xFBD77A6D

/**
 * @brief   L1 Region Prefetcher                             0xEEFD8030
 * @details
 *    Uses memory access history to fetch additional lines when the data access
 *    for a given instruction tends to be followed by other data accesses.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Prefetcher settings >
 **/
#define    ACT_CFG_UID_CmnCpuL1RegionPrefetcher                                  0xEEFD8030

/**
 * @brief   L2 Stream HW Prefetcher                          0xDC173155
 * @details
 *    Option to Enable | Disable L2 Stream HW Prefetcher
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Prefetcher settings >
 **/
#define    ACT_CFG_UID_CmnCpuL2StreamHwPrefetcher                                0xDC173155

/**
 * @brief   L2 Up/Down Prefetcher                            0x1314A694
 * @details
 *    Uses memory access history to determine whether to fetch the next or
 *    previous line for all memory accesses.
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Prefetcher settings >
 **/
#define    ACT_CFG_UID_CmnCpuL2UpDownPrefetcher                                  0x1314A694

/**
 * @brief   Core Watchdog Timer Enable                       0x79F79A6F
 * @details
 *    Enable or disable CPU Watchdog Timer
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Core Watchdog >
 **/
#define    ACT_CFG_UID_DbgCpuGenCpuWdt                                           0x79F79A6F

/**
 * @brief   Core Watchdog Timer Interval                     0x9A514716
 * @details
 *    Select CPU Watchdog Timer interval
 *    Data Type: Word
 *    Category:  CPU Common Options >  Core Watchdog >
 **/
#define    ACT_CFG_UID_DbgCpuGenCpuWdtTimeout                                    0x9A514716

/**
 * @brief   Core Watchdog Timer Severity                     0x0732375A
 * @details
 *    Specify the CPU watch dog timer severity
 *    (MSRC001_0074[CpuWdTmrCfgSeverity]).
 *    Data Type: Byte
 *    Category:  CPU Common Options >  Core Watchdog >
 **/
#define    ACT_CFG_UID_CmnCpuWdtSev                                              0x0732375A

/**
 * @brief   RedirectForReturnDis                             0x60D54609
 * @details
 *    From a workaround for GCC/C000005 issue for XV Core on CZ A0, setting
 *    MSRC001_1029 Decode Configuration (DE_CFG) bit 14
 *    [DecfgNoRdrctForReturns] to 1
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuGenWA05                                             0x60D54609

/**
 * @brief   Platform First Error Handling                    0x7831D45A
 * @details
 *    Enable/disable PFEH, cloak individual banks, and mask deferred error
 *    interrupts from each bank.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuPfeh                                                0x7831D45A

/**
 * @brief   Core Performance Boost                           0xDC086EFC
 * @details
 *    Disable CPB
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuCpb                                                 0xDC086EFC

/**
 * @brief   Global C-state Control                           0xFDF9384F
 * @details
 *    Controls IO based C-state generation and DF C-states.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuGlobalCstateCtrl                                    0xFDF9384F

/**
 * @brief   Power Supply Idle Control                        0xDADC845F
 * @details
 *    Power Supply Idle Control.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbPowerSupplyIdleCtrl                                 0xDADC845F

/**
 * @brief   SEV ASID Count                                   0x2931909F
 * @details
 *    This fields specifies the maximum valid ASID, which affects the maximum
 *    system physical address space. 16TB of physical address space is
 *    available for systems that support 253 ASIDs, while 8TB of physical
 *    address space is available for systems that support 509 ASIDs.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuSevAsidCount                                        0x2931909F

/**
 * @brief   SEV-ES ASID Space Limit Control                  0x48A4EC8B
 * @details
 *    Select SEV-ES ASID Space Limit operation modes
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuSevAsidSpaceCtrl                                    0x48A4EC8B

/**
 * @brief   Streaming Stores Control                         0x71F3579B
 * @details
 *    Enables or disables the streaming stores functionality
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuStreamingStoresCtrl                                 0x71F3579B

/**
 * @brief   Local APIC Mode                                  0x5E5558BF
 * @details
 *    Select local APIC operation modes
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_DbgCpuLApicMode                                           0x5E5558BF

/**
 * @brief   ACPI _CST C1 Declaration                         0xD953BF4E
 * @details
 *    Determines whether or not to declare the C1 state to the OS.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuCstC1Ctrl                                           0xD953BF4E

/**
 * @brief   MCA error thresh enable                          0x7BB8786C
 * @details
 *    Enable MCA error thresholding.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuMcaErrThreshEn                                      0x7BB8786C

/**
 * @brief   MCA FruText                                      0x0DD762BC
 * @details
 *    Enable MCA FruText
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuMcaFruTextEn                                        0x0DD762BC

/**
 * @brief   SMU and PSP Debug Mode                           0x950A3B57
 * @details
 *    When this option is enabled, uncorrected errors detected by the PSP FW or
 *    SMU FW that should cause a cold reset, will hang and not reset the system
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuSmuPspDebugMode                                     0x950A3B57


/**
 * @brief   SMU and PSP Debug Mode                           0x950A3B57
 * @details
 *    When this option is enabled, uncorrected errors detected by the PSP FW or
 *    SMU FW that should cause a cold reset, will hang and not reset the system
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuSmuPspDebugMode                                     0x950A3B57

/**
 * @brief   Xtrig7 Workaround                                0x63C3EE9F
 * @details
 *    This workaround is only applicable for Rev A.
 *    For Rev A, by default (Auto), the Bronze workaround is applied.
 *    Bronze workaround: DbReq and PDM function as expected, breakpoint
 *    redirect capability compromised
 *    Silver workaround: DbReq, PDM, and breakpoint redirect function as
 *    expected, SCAN capability compromised
 *    For Rev B, no workaround is applied and changing the selection for this
 *    option will not result in any change.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuXtrig7Workaround                                    0x63C3EE9F

/**
 * @brief   PPIN Opt-in                                      0x7632DA3E
 * @details
 *    Turn on PPIN feature
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuPpinCtrl                                            0x7632DA3E

/**
 * @brief   SNP Memory (RMP Table) Coverage                  0x9BC35D37
 * @details
 *    Enabled = ENTIRE system memory is covered.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_DbgCpuSnpMemCover                                         0x9BC35D37

/**
 * @brief   SMEE                                             0x536AEC32
 * @details
 *    Control secure memory encryption enable
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuSmee                                                0x536AEC32

/**
 * @brief   Action on BIST Failure                           0xE27D04EE
 * @details
 *    Action to take when a CCD BIST failure is detected.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnActionOnBistFailure                                    0xE27D04EE

/**
 * @brief   Fast Short REP MOVSB (FSRM)                      0x18D544D1
 * @details
 *    Default is 1, can be set to zero for analysis purposes as long as OS
 *    supports it.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuFSRM                                                0x18D544D1

/**
 * @brief   Enhanced REP MOVSB/STOSB (ERSM)                  0x04BDD2BB
 * @details
 *    Default is 1, can be set to zero for analysis purposes as long as OS
 *    supports it.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuERMS                                                0x04BDD2BB

/**
 * @brief   Log Transparent Errors                           0xEDF1BE49
 * @details
 *    Log transparent errors in MCA in addition to debug registers.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuLogTransparentErrors                                0xEDF1BE49

/**
 * @brief   AVX512                                           0x1F31995D
 * @details
 *    Enable/Disable AVX512.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuAvx512                                              0x1F31995D

/**
 * @brief   MONITOR and MWAIT disable                        0x165F1620
 * @details
 *    The MONITOR, MWAIT, MONITORX, and MWAITX opcodes become invalid, when
 *    Enabled.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnCpuMonMwaitDis                                         0x165F1620

/**
 * @brief   MONITOR and MWAIT disable                        0X82A8B533
 * @details
 *    The MONITOR, MWAIT, MONITORX, and MWAITX opcodes become invalid, when enabled.
 *    Data Type: Byte
 *    Category:  CPU Common Options >
 **/
#define    ACT_CFG_UID_CmnMonMwaitDis                                               0X82A8B533

/**
 * @brief   NUMA nodes per socket                            0xF6A95070
 * @details
 *    Specifies the number of desired NUMA nodes per socket.  Zero will attempt
 *    to interleave the two sockets together.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnDramNps                                              0xF6A95070

/**
 * @brief   Memory interleaving                              0x08F433C5
 * @details
 *    Allows for disabling memory interleaving.  Note that NUMA nodes per
 *    socket will be honored regardless of this setting.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnMemIntlv                                             0x08F433C5

/**
 * @brief   Memory interleaving size                         0x93421F50
 * @details
 *    Controls the memory interleaving size. The valid values are AUTO, 256
 *    bytes, 512 bytes, 1 Kbytes, 2 Kbytes and 4 Kbytes. This determines the
 *    starting address of the interleave (bit 8, 9, 10, 11 or 12).
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnMemIntlvSize                                         0x93421F50

/**
 * @brief   1TB remap                                        0x00F9900A
 * @details
 *    Attempt to remap DRAM out of the space just below the 1TB boundary.  The
 *    ability to remap depends on DRAM configuration, NPS, and interleaving
 *    selection, and may not always be possible.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnRemapAt1TB                                           0x00F9900A

/**
 * @brief   DRAM map inversion                               0x5D873B8C
 * @details
 *    Inverting the map will cause the highest memory channels to get assigned
 *    the lowest addresses in the system.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnDramMapInversion                                     0x5D873B8C

/**
 * @brief   Location of private memory regions               0xDD59CE61
 * @details
 *    Controls whether or not the private memory regions (PSP, SMU and CC6) are
 *    at the top of DRAM, at the top of 1st DRAM pair or distributed. Note that
 *    distributed requires memory on all dies. Note that it will always be at
 *    the top of DRAM if some dies don't have memory regardless of this
 *    option's setting.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Memory Addressing >
 **/
#define    ACT_CFG_UID_DfCmnCc6AllocationScheme                                  0xDD59CE61

/**
 * @brief   ACPI SRAT L3 Cache As NUMA Domain                0xA0A2CAF6
 * @details
 *    Enabled: Each CCX in the system will be declared as a separate NUMA
 *    domain.
 *    Disabled: Memory Addressing \ NUMA nodes per socket will be declared.
 *    Data Type: Byte
 *    Category:  DF Common Options >  ACPI >
 **/
#define    ACT_CFG_UID_DfCmnAcpiSratL3Numa                                       0xA0A2CAF6

/**
 * @brief   ACPI SLIT Distance Control                       0xADF285F4
 * @details
 *    Determines how the SLIT distances are declared.
 *    Data Type: Byte
 *    Category:  DF Common Options >  ACPI >
 **/
#define    ACT_CFG_UID_DfCmnAcpiSlitDistCtrl                                     0xADF285F4

/**
 * @brief   ACPI SLIT remote relative distance               0x169F7BC8
 * @details
 *    Set the remote socket distance for 2P systems as near (2.8) or far (3.2).
 *    Data Type: Byte
 *    Category:  DF Common Options >  ACPI >
 **/
#define    ACT_CFG_UID_DfCmnAcpiSlitRemoteFar                                    0x169F7BC8

/**
 * @brief   GMI encryption control                           0xE39BA05A
 * @details
 *    Control GMI link encryption
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmnGmiEncryption                                        0xE39BA05A

/**
 * @brief   xGMI encryption control                          0x2F42235E
 * @details
 *    Control xGMI link encryption
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmnXGmiEncryption                                       0x2F42235E

/**
 * @brief   xGMI Link Configuration                          0x9EAB4FFF
 * @details
 *    Configures the number of xGMI2 links used on a multi-socket system.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfDbgXgmiLinkCfg                                          0x9EAB4FFF

/**
 * @brief   4-link xGMI max speed                            0x6802DB09
 * @details
 *    Specifies the max frequency used for XGMI PState in a 4-link topology.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmn4LinkMaxXgmiSpeed                                    0x6802DB09

/**
 * @brief   3-link xGMI max speed                            0x539E387B
 * @details
 *    Specifies the max frequency used for XGMI PState in a 3-link topology.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmn3LinkMaxXgmiSpeed                                    0x539E387B

/**
 * @brief   2-link xGMI max speed                            0x71CFC55B
 * @details
 *    Specifies the max frequency used for XGMI PState in a 2-link topology.
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmn2LinkMaxXgmiSpeed                                    0x71CFC55B

/**
 * @brief   xGMI TXEQ Mode                                   0x3FEA42E6
 * @details
 *    Select XGMI TXEQ/RX vetting Mode
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DfCmnXgmiTxEqMode                                         0x3FEA42E6

/**
 * @brief   xGMI 18GACOFC                                    0x79540626
 * @details
 *    xGMI 18GACOFC control
 *    Data Type: Byte
 *    Category:  DF Common Options >  Link >
 **/
#define    ACT_CFG_UID_DbgGnbxGMI18GACOFC                                        0x79540626

/**
 * @brief   DF Watchdog Timer Interval                       0x7DDDAF46
 * @details
 *    Configure the Data Fabric watchdog timer interval.
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnWdtInterval                                          0x7DDDAF46

/**
 * @brief   Disable DF to external IP SyncFloodPropagation         0x2B3F18B1
 * @details
 *    Disable SyncFlood to UMC & downstream slaves.
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnExtIpSyncFloodProp                                   0x2B3F18B1

/**
 * @brief   Sync Flood Propagation to DF Components          0x64F23E2B
 * @details
 *    Control DF::PIEConfig[DisSyncFloodProp]
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnDisSyncFloodProp                                     0x64F23E2B

/**
 * @brief   Freeze DF module queues on error                 0xF757127D
 * @details
 *    Controls DF::PIEConfig[DisImmSyncFloodOnFatalError]
 *    Disabling this option sets DF:PIEConfig[DisImmSyncFloodOnFatalError]
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnFreezeQueueError                                     0xF757127D

/**
 * @brief   CC6 memory region encryption                     0x924C1F27
 * @details
 *    Control whether or not the CC6 save/restore memory is encrypted
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnCc6MemEncryption                                     0x924C1F27

/**
 * @brief   System probe filter                              0x4F7A0430
 * @details
 *    Controls whether or not the probe filter is enabled. Has no effect on
 *    parts where the probe filter is fuse disabled.
 *    Data Type: Byte
 *    Category:  DF Common Options >
 **/
#define    ACT_CFG_UID_DfCmnSysProbeFilter                                       0x4F7A0430

/**
 * @brief   Chipselect Interleaving                          0x1C6C8CE0
 * @details
 *    Interleave memory blocks across the DRAM chip selects for node 0.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Addressing Options >
 **/
#define    ACT_CFG_UID_CmnMemCsInterleaveDdr                                     0x1C6C8CE0

/**
 * @brief   Address Hash Bank                                0x476E46DE
 * @details
 *    Enable or disable bank address hashing
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Addressing Options >
 **/
#define    ACT_CFG_UID_CmnMemAddressHashBankDdr                                  0x476E46DE

/**
 * @brief   Address Hash CS                                  0x44828966
 * @details
 *    Enable or disable CS address hashing
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Addressing Options >
 **/
#define    ACT_CFG_UID_CmnMemAddressHashCsDdr                                    0x44828966

/**
 * @brief   Address Hash Rm                                  0x8CAAFAC6
 * @details
 *    Enable or disable RM address hashing
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Addressing Options >
 **/
#define    ACT_CFG_UID_CmnMemAddressHashRmDdr                                    0x8CAAFAC6

/**
 * @brief   BankSwapMode                                     0x273D73BE
 * @details
 *    BankSwapMode value: 0=Disabled, 1=SwapCPU
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Addressing Options >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerBankSwapModeDdr                              0x273D73BE

/**
 * @brief   Power Down Enable                                0xFCAFD979
 * @details
 *    Enable or disable DDR power down mode
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DDR Power Options >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerPwrDnEnDdr                                   0xFCAFD979


/**
 * @brief   Power Down Enable                                0xAB6ADD05
 * @details
 *    Enable or disable DDR power down mode
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DRAM Power Options >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerPwrDnEnDdr4                                  0xAB6ADD05

/**
 * @brief   DRAM Maximum Activate Count                      0xB50B5939
 * @details
 *    Override DIMM SPD Byte 7 [3:0] Maximum Activate Count (MAC)
 *    Auto: based on SPD setting
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DRAM Power Options >
 **/
#define    ACT_CFG_UID_CmnMemMaxActivateCount1                                   0xB50B5939

/**
 * @brief   DRAM Refresh Rate                                0xF3987A9E
 * @details
 *    DRAM refresh rate: 1.95us or 3.9us (default)
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DDR Power Options >
 **/
#define    ACT_CFG_UID_CmnMemDramRefreshRate                                     0xF3987A9E

/**
 * @brief   Self-Refresh Exit Staggering                     0x588C0A71
 * @details
 *    Tcksrx += (Trfc/n * (UMC_Number % 4)), here n = 3 or 4
 *    Disabled: does not apply the extra addition
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DRAM Power Options >
 **/
#define    ACT_CFG_UID_CmnMemSelfRefreshExitStaggering                           0x588C0A71

/**
 * @brief   PMIC Fault Recovery                              0xE81B77E6
 * @details
 *    0 - Always; 1 - Never (default); 2 - Once
 *    Always - PMIC will ignore previous boot errors. No channel disabled
 *    Never - PMIC disables the channel with errors from previous boot.
 *    Once - PMIC will ignore the previous boot errors once. More than once
 *    channel will be disabled
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DDR Power Options >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerPmicFaultRecovery                            0xE81B77E6

/**
 * @brief   PMIC Operation Mode                              0xEF1EF8DA
 * @details
 *    1 - Programmable Mode Operation (default); 0 - Secure Mode Operation
 *    Programmable mode allows certain registers to be programmed after VR
 *    enable else they will be in secure mode
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >  DDR Power Options >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerPmicOpMode                                   0xEF1EF8DA

/**
 * @brief   MBIST Enable                                     0xEE5BC9B2
 * @details
 *    Enable or disable Memory MBIST
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistEnDdr                                          0xEE5BC9B2

/**
 * @brief   MBIST Test Mode                                  0xFBA81CEA
 * @details
 *    Select MBIST Test Mode -Interface Mode (Tests Single and Multiple CS
 *    transactions and Basic Connectivity) or Data Eye Mode (Measures Voltage
 *    vs. Timing)
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistTestmodeDdr                                    0xFBA81CEA

/**
 * @brief   MBIST Aggressors                                 0xF74E4799
 * @details
 *    Enable or disable MBIST Aggressor test
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistAggressorsDdr                                  0xF74E4799

/**
 * @brief   MBIST Per Bit Slave Die Reporting                0x35F4FC31
 * @details
 *    Reports 2D Data Eye Results in ABL Log for each DQ, Chipselect, and
 *    Channel
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistPerBitSlaveDieReportDdr                        0x35F4FC31


/**
 * @brief   Cmd2T                                            0xDC3B70ED
 * @details
 *    Select between 1T and 2T mode on ADDR/CMD
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >
 **/
#define    ACT_CFG_UID_CmnMemCtrller2TModeDdr4                                   0xDC3B70ED

/**
 * @brief   Gear Down Mode                                   0xB1293E61
 * @details
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Controller Configuration >
 **/
#define    ACT_CFG_UID_CmnMemGearDownModeDdr4                                    0xB1293E61

/**
 * @brief   Pattern Select                                   0x84008876
 * @details
 *    MBIST Data Eye Pattern Type. 0 - PRBS (default), 1 - SSO, 2 - Both
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistPatternSelect                                  0x84008876


/**
 * @brief   MBIST Enable                                     0x533DCE78
 * @details
 *    Enable or disable Memory MBIST
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistEn                                             0x533DCE78

/**
 * @brief   MBIST Test Mode                                  0xBB0B832C
 * @details
 *    Select MBIST Test Mode -Interface Mode (Tests Single and Multiple CS
 *    transactions and Basic Connectivity) or Data Eye Mode (Measures Voltage
 *    vs. Timing)
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistTestmode                                       0xBB0B832C

/**
 * @brief   MBIST Aggressors                                 0xE21E0449
 * @details
 *    Enable or disable MBIST Aggressor test
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistAggressors                                     0xE21E0449

/**
 * @brief   MBIST Per Bit Slave Die Reporting                0xEEBBF7B0
 * @details
 *    Reports 2D Data Eye Results in ABL Log for each DQ, Chipselect, and
 *    Channel
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_CmnMemMbistPerBitSlaveDieReport                           0xEEBBF7B0

/**
 * @brief   Pattern Select                                   0x84008876
 * @details
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistPatternSelect                                  0x84008876

/**
 * @brief   Aggressor Channel                                0x6D12710D
 * @details
 *    This helps read the aggressors channels. If it is enabled, you can read
 *    from one or more than one aggressor channel. The default is set to
 *    disabled.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistAggressorsChnl                                 0x6D12710D

/**
 * @brief   Aggressor Static Lane Control                    0x51C6DCAA
 * @details
 *    This option, if enabled, will control the Aggressor Static Lane Controls.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistAggrStaticLaneCtrl                             0x51C6DCAA

/**
 * @brief   Target Static Lane Control                       0x0BB78A53
 * @details
 *    Enable Mbist Target Static Lane Control
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistTgtStaticLaneCtrl                              0x0BB78A53

/**
 * @brief   Data Eye Type                                    0xE40BA9E6
 * @details
 *    This options determines which results are expected to be captured for
 *    Data Eye. Supported options are 1D Voltage Sweep, 1D Timing Sweep, 2D
 *    Full Data Eye and Worst Case Margin only.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistDataEyeType                                    0xE40BA9E6

/**
 * @brief   Worst Case Margin Granularity                    0x94584811
 * @details
 *    Mbist Worst Case Margin Granularity
 *    0 = Per Chip Select, 1 = Per Nibble
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistWorstCaseGran                                  0x94584811

/**
 * @brief   Read Voltage Sweep Step Size                     0xE8745D0D
 * @details
 *    This option determines the step size for Read Data Eye voltage sweep,
 *    Supported options are 1,2 and 4
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistReadDataEyeVoltageStep                         0xE8745D0D

/**
 * @brief   Read Timing Sweep Step Size                      0x0FE11E67
 * @details
 *    This options supports step size for Read Data Eye. Supported options are
 *    1, 2 and 4
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistReadDataEyeTimingStep                          0x0FE11E67

/**
 * @brief   Write Voltage Sweep Step Size                    0x9F78BE61
 * @details
 *    This option determines the step size for write Data Eye voltage sweep,
 *    Supported options are 1,2 and 4
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistWriteDataEyeVoltageStep                        0x9F78BE61

/**
 * @brief   Write Timing Sweep Step Size                     0x08D4A0BB
 * @details
 *    This options supports step size for write Data Eye. Supported options are
 *    1, 2 and 4
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >  Data Eye >
 **/
#define    ACT_CFG_UID_CmnMemMbistWriteDataEyeTimingStep                         0x08D4A0BB

/**
 * @brief   Memory Healing BIST                              0xAB0FC59E
 * @details
 *    Enable a full memory test. The testing will increase the boot time.\nBIOS
 *    Mem BIST tests the full memory after training. Failing memory will be
 *    repaired using soft or hard PPR depending on the PPC configuratoin. The
 *    test will take 3 minutes per 16GB of installed memory.\nSelf-Healing BIST
 *    runs the JEDEC DRAM self healing if the device supports and DIMM support
 *    the feature. The DRAM will do a hard repair for failing memory. The test
 *    will take 10 seconds per memory rank per channel.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_MemHealBistEn                                             0xAB0FC59E

/**
 * @brief   Mem BIST Test Select                             0xA35AD296
 * @details
 *    Select the vendor specific tests to use with BIOS memory healing BIST
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_MemHealTestEn                                             0xA35AD296

/**
 * @brief   Mem BIST Post Package Repair Type                0x0962F854
 * @details
 *    For DRAM errors found in the BIOS memory BIST select the repair type,
 *    soft, hard or test only and do not attempt to repair.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR MBIST Options >
 **/
#define    ACT_CFG_UID_MemHealPPRType                                            0x0962F854

/**
 * @brief   Data Poisoning                                   0x0D4C6653
 * @details
 *     Enable poison data creation on uncorrectable DDR DRAM ECC errors and
 *    poison propagation to CPU cores and caches. Requires ECC memory. When
 *    FALSE, a fatal error event will occur on DDR ECC errors
 *    sets UMC_CH::EccCtrl[UcFatalEn] when MC_CH::EccCtrl[WrEccEn] is set.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemDataPoisoningDdr                                    0x0D4C6653

/**
 * @brief   DRAM Boot Time Post Package Repair               0xF4BA768C
 * @details
 *    Enable or Disable DRAM Boot Time Post Package Repair.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemBootTimePostPackageRepair                           0xF4BA768C


/**
 * @brief   DRAM Post Package Repair                         0x80F59481
 * @details
 *    Enable or Disable DRAM  Post Package Repair.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemPostPackageRepair                                   0x80F59481

/**
 * @brief   RCD Parity                                       0x45F7B929
 * @details
 *    Enable RCD command and address parity.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemRcdParityDdr                                        0x45F7B929

/**
 * @brief   Write CRC                                        0x76C44C14
 * @details
 *    Enable write CRC on DDR5 DRAM. Program to UMC::RecCtrl.RecEn[1]
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemWriteCrcDdr                                         0x76C44C14

/**
 * @brief   Read CRC                                         0xEF103E79
 * @details
 *    Program to RecCtrl.RecEn [3]
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemReadCrcDdr                                          0xEF103E79

/**
 * @brief   Disable Memory Error Injection                   0x3A7997FD
 * @details
 *    0=Enable. 1=Disable. Specifies UMC error injection configuration
 *    writes are disabled.
 *    True: UMC::CH::MiscCfg[DisErrInj]=1
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemDisMemErrInj                                        0x3A7997FD

/**
 * @brief   DRAM ECC Symbol Size                             0x9C0D83FF
 * @details
 *    DRAM ECC Symbol Size (x4/x16) - UMC_CH::EccCtrl[EccSymbolSize16,
 *    EccSymbolSize]
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  ECC Configuration >
 **/
#define    ACT_CFG_UID_CmnMemDramEccSymbolSizeDdr                                0x9C0D83FF

/**
 * @brief   DRAM ECC Enable                                  0x644330C3
 * @details
 *    Use this option to enable / disable DRAM ECC. Auto will set ECC to
 *    enable.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  ECC Configuration >
 **/
#define    ACT_CFG_UID_CmnMemDramEccEnDdr                                        0x644330C3

/**
 * @brief   DRAM UECC Retry                                  0xF3BAC538
 * @details
 *    DRAM UECC Retry. Program to UMC::RecCtrl.RecEn [2]
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  ECC Configuration >
 **/
#define    ACT_CFG_UID_CmnMemDramUeccRetryDdr                                    0xF3BAC538

/**
 * @brief   Memory Clear                                     0x5326AA7D
 * @details
 *    Clear/Zero out Dram range [DramScrubBaseAddr: DramScrubLimitAddr].When
 *    this option is disabled, Memory is not cleared after training. ECC Dimms
 *    have memory clear enabled always. Non-ECC Dimms can choose to
 *    disable/enable using this option.
 *    Default = Memclear enabled
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  ECC Configuration >
 **/
#define    ACT_CFG_UID_CmnMemDramMemClrDdr                                       0x5326AA7D

/**
 * @brief   Address XOR after ECC                            0x3CA4964B
 * @details
 *    In order to provide data integrity when data is returned from the wrong
 *    address, UMC will hash the data after ECC with the normalized address
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  ECC Configuration >
 **/
#define    ACT_CFG_UID_CmnMemAddrXorAfterEcc                                     0x3CA4964B

/**
 * @brief   DRAM ECS Mode                                    0x42FE5FAE
 * @details
 *    0 = AutoECS Mode, 1 = ManualECS mode,
 *    Auto = will set ECS to Manual mode
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  DRAM Scrubbers >
 **/
#define    ACT_CFG_UID_CmnMemDramEcsModeDdr                                      0x42FE5FAE

/**
 * @brief   DRAM Redirect Scrubber Enable                    0xF1C960AA
 * @details
 *    Enable/Disable Dram Redirect Scrubber
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  DRAM Scrubbers >
 **/
#define    ACT_CFG_UID_CmnMemDramRedirectScrubEnDdr                              0xF1C960AA

/**
 * @brief   DRAM Scrub Redirection Limit                     0x4F9377D6
 * @details
 *    Dram ECC Scrub Redirection Limit: 0=8 scrubs, 1=4 scrubs, 2=2 scrubs, 3=1
 *    scrub
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  DRAM Scrubbers >
 **/
#define    ACT_CFG_UID_CmnMemDramRedirectScrubLimitDdr                           0x4F9377D6

/**
 * @brief   DRAM Patrol Scrubber Enable                      0xB00D39CE
 * @details
 *    Enable/Disable Dram Patrol Scrubber. Auto = Disabled by default.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >  DRAM Scrubbers >
 **/
#define    ACT_CFG_UID_CmnMemDramPatrolScrubEnDdr                                0xB00D39CE

/**
 * @brief   DRAM Corrected Error Counter Enable              0x0E61FCBE
 * @details
 *    Configure DRAM Corrected Error Counter function.
 *    Only meaningful when PcdAmdCcxCfgPFEHEnable
 *    is TRUE.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemCorrectedErrorCounterEnable                         0x0E61FCBE

/**
 * @brief   DRAM Corrected Error Counter Interrupt Enable         0xD384E37C
 * @details
 *    Enable SMI when DRAM Corrected Error Counter count exceeds the threshold
 *    value.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnMemCorrectedErrorCounterInterruptEnable                0xD384E37C

/**
 * @brief   PMIC Error Reporting                             0x158418C3
 * @details
 *    Enables support for PMIC Error Reporting.
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR RAS >
 **/
#define    ACT_CFG_UID_CmnPmicErrorReporting                                     0x158418C3

/**
 * @brief   Bus Configuration User Controls                  0x781581DE
 * @details
 *    Specify the mode for Bus configuration to Auto or Manual
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgCtlDdr                                        0x781581DE

/**
 * @brief   RttNom_Wr                                        0x6F274192
 * @details
 *    Specifies the RTT_NOM_WR
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgRttNomWrDdr                                   0x6F274192

/**
 * @brief   RttNom_Rd                                        0x4A8C8D4C
 * @details
 *    Specifies the RTT_NOM_RD
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgRttNomRdDdr                                   0x4A8C8D4C

/**
 * @brief   RttWr                                            0x58605DE1
 * @details
 *    Specifies the RTT_WR
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgRttWrDdr                                      0x58605DE1

/**
 * @brief   RttPark                                          0x6FFC0107
 * @details
 *    Specifies the RTT_PARK
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgRttParkDdr                                    0x6FFC0107

/**
 * @brief   RttPark_Dqs                                      0x820D946A
 * @details
 *    Specifies the RTT_PARK_DQS
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgRttParkDqsDdr                                 0x820D946A

/**
 * @brief   ProcODT                                          0x7FE3CF78
 * @details
 *    Specifies the Processor ODT
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemDataBusCfgProcOdtDdr                                0x7FE3CF78

/**
 * @brief   DRAM Data Drive Strength                         0x982CA88A
 * @details
 *    Specifies the DRAM drive impedance
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Bus Configuration >
 **/
#define    ACT_CFG_UID_CmnMemBusCfgDramDataDrvStrenDdr                           0x982CA88A

/**
 * @brief   Active Memory Timing Settings                    0x4ADBC652
 * @details
 *    Active Memory Timing Settings
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CmnMemTimingSettingDdr                                    0x4ADBC652

/**
 * @brief   Tcl Ctrl                                         0xFD4046A4
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTclCtrlDdr                                    0xFD4046A4

/**
 * @brief   Trcd Ctrl                                        0x9DB6CB77
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrcdCtrlDdr                                   0x9DB6CB77

/**
 * @brief   Trp Ctrl                                         0xB90305BC
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrpCtrlDdr                                    0xB90305BC

/**
 * @brief   Tras Ctrl                                        0x3EC7C65C
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrasCtrlDdr                                   0x3EC7C65C

/**
 * @brief   Trc Ctrl                                         0x43F298B7
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrcCtrlDdr                                    0x43F298B7

/**
 * @brief   Twr Ctrl                                         0x29F05A68
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrCtrlDdr                                    0x29F05A68

/**
 * @brief   Trfc1 Ctrl                                       0xFFC91C67
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrfc1CtrlDdr                                  0xFFC91C67

/**
 * @brief   Trfc2 Ctrl                                       0x0E0E8BEE
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrfc2CtrlDdr                                  0x0E0E8BEE

/**
 * @brief   TrfcSb Ctrl                                      0x86B8EFDB
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrfcSbCtrlDdr                                 0x86B8EFDB

/**
 * @brief   Tcwl Ctrl                                        0xB7EBC2A3
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTcwlCtrlDdr                                   0xB7EBC2A3

/**
 * @brief   Trtp Ctrl                                        0x627B9324
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrtpCtrlDdr                                   0x627B9324

/**
 * @brief   TrrdL Ctrl                                       0x99AE4F69
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrrdLCtrlDdr                                  0x99AE4F69

/**
 * @brief   TrrdS Ctrl                                       0xB0B53EF8
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrrdSCtrlDdr                                  0xB0B53EF8

/**
 * @brief   Tfaw Ctrl                                        0xD3EB2A08
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTfawCtrlDdr                                   0xD3EB2A08

/**
 * @brief   TwtrL Ctrl                                       0xE1A5079C
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwtrLCtrlDdr                                  0xE1A5079C

/**
 * @brief   TwtrS Ctrl                                       0x1C3B6177
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwtrSCtrlDdr                                  0x1C3B6177

/**
 * @brief   TrdrdScL Ctrl                                    0x02F2C74F
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrdrdScLCtrlDdr                               0x02F2C74F

/**
 * @brief   TrdrdSc Ctrl                                     0x5AF35174
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrdrdScCtrlDdr                                0x5AF35174

/**
 * @brief   TrdrdSd Ctrl                                     0x19AB0688
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrdrdSdCtrlDdr                                0x19AB0688

/**
 * @brief   TrdrdDd Ctrl                                     0x419D8274
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrdrdDdCtrlDdr                                0x419D8274

/**
 * @brief   TwrwrScL Ctrl                                    0x6FFDD02A
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrwrScLCtrlDdr                               0x6FFDD02A

/**
 * @brief   TwrwrSc Ctrl                                     0x525092E0
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrwrScCtrlDdr                                0x525092E0

/**
 * @brief   TwrwrSd Ctrl                                     0xF2343E10
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrwrSdCtrlDdr                                0xF2343E10

/**
 * @brief   TwrwrDd Ctrl                                     0x54EE4DDD
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrwrDdCtrlDdr                                0x54EE4DDD

/**
 * @brief   Twrrd Ctrl                                       0x2CDD40F6
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTwrrdCtrlDdr                                  0x2CDD40F6

/**
 * @brief   Trdwr Ctrl                                       0xE235D03C
 * @details
 *    Auto: Follow default setting, Manual: Manually specify
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >  Non-SPD Timing >
 **/
#define    ACT_CFG_UID_CmnMemTimingTrdwrCtrlDdr                                  0xE235D03C

/**
 * @brief   DRAM PDA Enumerate ID Programming Mode           0x4BC368E4
 * @details
 *    Specify PDA enumeration mode
 *    Auto : default
 *    0 : Continuous DQS toggling PDA enumeration mode (default)
 *    1 : Legacy PDA enumeration mode
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Training Options >
 **/
#define    ACT_CFG_UID_CmnMemDramPdaEnumIdProgModeDdr                            0x4BC368E4

/**
 * @brief   TSME                                             0x626EE365
 * @details
 *    Transparent SME
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Security >
 **/
#define    ACT_CFG_UID_CmnMemTsmeEnableDdr                                       0x626EE365

/**
 * @brief   ProcODT                                          0x4FC4D6E3
 * @details
 *    Specifies the Processor ODT
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Timing Configuration >  DTC_Legal_Disclaimer_Accept >
 **/
#define    ACT_CFG_UID_CmnMemCtrllerProcOdtDdr4                                  0x4FC4D6E3

/**
 * @brief   AES                                              0x6C533014
 * @details
 *    AES mode: AES-128 or AES-256 (default)
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Security >
 **/
#define    ACT_CFG_UID_CmnMemAes                                                 0x6C533014

/**
 * @brief   Data Scramble                                    0x8D2B5138
 * @details
 *    Data scrambling: DataScrambleEn
 *    Data Type: Byte
 *    Category:  UMC Common Options >  DDR Security >
 **/
#define    ACT_CFG_UID_CmnMemDataScramble                                        0x8D2B5138

/**
 * @brief   IOMMU                                            0x580BBFB4
 * @details
 *    Enable/Disable IOMMU
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbNbIOMMU                                             0x580BBFB4

/**
 * @brief   DMAr Support                                     0xA586B84C
 * @details
 *    Enable DMAr system protection during POST.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnDmarSupport                                            0xA586B84C

/**
 * @brief   DRTM Virtual Device Support                      0xD1DAC381
 * @details
 *    Enable DRTM ACPI virtual device.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnDrtmSupport                                            0xD1DAC381

/**
 * @brief   DRTM Memory Reservation                          0x2D29C817
 * @details
 *    Reserve 128MB memory below Bottom IO for DRTM. It is required to be
 *    enabled for Secured-Core Server function.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnDrtmMemoryReservation                                  0x2D29C817

/**
 * @brief   ACS Enable                                       0x927F0930
 * @details
 *    AER must be enabled for ACS enable to work
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_DbgGnbDbgACSEnable                                        0x927F0930

/**
 * @brief   PCIe ARI Support                                 0x98C360BB
 * @details
 *    Enables Alternative Routing-ID Interpretation
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_GnbDbgPcieAriSupport                                      0x98C360BB

/**
 * @brief   PCIe ARI Enumeration                             0x2C4EF84D
 * @details
 *    ARI Forwarding Enable for each downstream port
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_GnbCmnPcieAriEnumeration                                  0x2C4EF84D

/**
 * @brief   PCIe Ten Bit Tag Support                         0x628C6349
 * @details
 *    Enables PCIe ten bit tags for supported devices.
 *    Auto = Disabled
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_GnbDbgPcieTbtSupport                                      0x628C6349

/**
 * @brief   TDP Control                                      0xED1626E2
 * @details
 *    Auto = Use the fused TDP\nManual = User can set customized TDP
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnTDPCtlRs                                               0xED1626E2

/**
 * @brief   PPT Control                                      0xF53E9099
 * @details
 *    Auto = Use the fused PPT\nManual = User can set customized PPT
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnPPTCtlRs                                               0xF53E9099

/**
 * @brief   Determinism Control                              0x8DBF848A
 * @details
 *    Auto = Use default performance determinism settings\nManual = User can
 *    set custom performance determinism settings
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnDeterminismCtlRs                                       0x8DBF848A

/**
 * @brief   Determinism Enable                               0x57884DF6
 * @details
 *    [0 = disable performance determinism; 1 = enable performance determinism]
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnDeterminismEnable                                      0x57884DF6

/**
 * @brief   xGMI Link Width Control                          0xE4ACE7C4
 * @details
 *    Auto = Use default xGMI link width controller settings\nManual = User can
 *    set custom xGMI link width controller settings
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnxGmiLinkWidthCtlRs                                     0xE4ACE7C4

/**
 * @brief   xGMI Force Link Width Control                    0x4B6E197A
 * @details
 *    Unforce = Do not force the xGMI to a fixed width\nForce = Force the xGMI
 *    link to the user specified width
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnxGmiForceLinkWidthCtlRs                                0x4B6E197A

/**
 * @brief   xGMI Force Link Width                            0x9B03705D
 * @details
 *    0 = Force xGMI link width to x2 \n1 = Force xGMI link width to x8
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnxGmiForceLinkWidthRs                                   0x9B03705D

/**
 * @brief   xGMI Max Link Width Control                      0x8CB6B416
 * @details
 *    Auto = Use default xGMI max supported link width\nManual = User can set
 *    custom xGMI max link width
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnxGmiMaxLinkWidthCtlRs                                  0x8CB6B416

/**
 * @brief   xGMI Max Link Width                              0xB5EB5735
 * @details
 *    0 = Set max xGMI link width to x8\n1 = Set max xGMI link width to x16
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnxGmiMaxLinkWidthRs                                     0xB5EB5735

/**
 * @brief   APBDIS                                           0x46254243
 * @details
 *    0 = not APBDIS (mission mode)\n1 = APBDIS
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnApbdis                                                 0x46254243

/**
 * @brief   Power Profile Selection                          0xEB7FB9FF
 * @details
 *    [0 = High Performance Mode (DEFAULT); 1 = Efficiency Mode; 2 = Maximum IO
 *    Performance Mode]
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnEfficiencyModeEnRs                                     0xEB7FB9FF

/**
 * @brief   BoostFmaxEn                                      0xCA6DE78C
 * @details
 *    Auto = Use the default Fmax\nManual = User can set the boost Fmax
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnBoostFmaxEnRs                                          0xCA6DE78C

/**
 * @brief   DF PState Frequency Optimizer                    0xB8519A72
 * @details
 *    Disabled - means disable the DFPstate CCLK effective frequency
 *    optimizer\nEnabled - means enable the DFPstate CCLK effective frequency
 *    optimizer
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSMUDffoRs                                           0xB8519A72

/**
 * @brief   DF PState Latency Optimizer                      0x5642475B
 * @details
 *    Disabled - means disable the DFPstate latency optimizer\nEnabled - means
 *    enable the DFPstate latency optimizer
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSMUDfloRs                                           0x5642475B

/**
 * @brief   DF Cstates                                       0x5E863B1E
 * @details
 *    Enable = Enable the feature : Disable = Disable the feature
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSmuDfCstatesRs                                      0x5E863B1E

/**
 * @brief   CPPC                                             0x4FED862B
 * @details
 *    Enable = Enable the feature : Disable = Disable the feature
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSmuCppcRs                                           0x4FED862B

/**
 * @brief   CPPC Preferred Cores                             0x9BBCFB7E
 * @details
 *    Enabled = CPPC highest capability register of each thread will have a
 *    different value to express the AMD recommended scheduling order to OS.
 *    CPPC must be enabled for this parameter to have effect. : Disabled = Does
 *    not declared AMD preferred scheduling order to OS.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSmuCppcPreferredCoresRs                             0x9BBCFB7E

/**
 * @brief   HSMP Support                                     0x156F930D
 * @details
 *    Select HSMP support enable or disable
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSMUHsmpSupport                                      0x156F930D

/**
 * @brief   SVI3 SVC Speed Control                           0xB43D977D
 * @details
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnSvi3SvcSpeedCtl                                        0xB43D977D

/**
 * @brief   SVI3 SVC Speed                                   0x413DD0D2
 * @details
 *    0=50.00MHz
 *    1=40.00MHz
 *    2=26.67MHz
 *    3=20.00MHz
 *    4=16.00MHz
 *    5=13.33MHz
 *    6=10.00MHz
 *    7=8.00MHz
 *    8=5.00MHz
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnSvi3SvcSpeed                                           0x413DD0D2

/**
 * @brief   3D V-Cache                                       0x5CB2E661
 * @details
 *    Override of X3D technology
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  SMU Common Options >
 **/
#define    ACT_CFG_UID_CmnX3dStackOverride                                       0x5CB2E661

/**
 * @brief   NBIO RAS Control                                 0x937B5FE0
 * @details
 *    (0) Disabled, (1) MCA
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_CmnRASControl                                             0x937B5FE0

/**
 * @brief   NBIO SyncFlood Generation                        0x9EBC7927
 * @details
 *    This value may be used to mask SyncFlood caused by NBIO RAS options.
 *    When set to TRUE SyncFlood from NBIO is masked.  When set to FALSE NBIO
 *    is capable of generating SyncFlood.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_CmnNBIOSyncFloodGen                                       0x9EBC7927

/**
 * @brief   NBIO SyncFlood Reporting                         0x8EB823BF
 * @details
 *    This value may be used to enable SyncFlood reporting to APML.  When set
 *    to TRUE SyncFlood will be reported to APML.  When set to FALSE that
 *    reporting well be disabled
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_PcdSyncFloodToApml                                        0x8EB823BF

/**
 * @brief   SLINK Read Response OK                           0x2D81AB91
 * @details
 *    This value specifies whether SLINK read response errors are converted to
 *    an Okay response.  When this value is set to TRUE, read response errors
 *    are converted to Okay responses with data of all FFs.  When set to FALSE
 *    read response errors are not converted.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_PcdSlinkConvertReadResponseErrorsToOkay                   0x2D81AB91

/**
 * @brief   SLINK Read Response Error Handling               0xB3768E6B
 * @details
 *    This value specifies whether SLINK write response errors are converted to
 *    an Okay response.  When this value is set to 0, write response errors
 *    will be logged in the MCA.  When set to 1, write response errors will
 *    trigger an MCOMMIT error. When this value is set to 2, write response
 *    errors are converted to Okay responses.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_PcdSlinkWriteResponseErrorHandling                        0xB3768E6B

/**
 * @brief   Log Poison Data from SLINK                       0xDBFBF953
 * @details
 *    This value specifies whether poison data propagated from SLINK will
 *    generate a deferred error.  When set to TRUE, deferred errors are
 *    enabled.  When set to FALSE, errors are not generated.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_PcdAmdLogPoisonDataFromSLink                              0xDBFBF953

/**
 * @brief   PCIe Aer Reporting Mechanism                     0x75E75997
 * @details
 *    This value selects the method of reporting AER errors from PCI Express.
 *    A value of 1 allows OS First handling of the errors through generation of
 *    a system control interrupt (SCI).  A value of 2 provides for Firmware
 *    First handling of errors through generation of a system management
 *    interrupt (SMI).
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_PcdAmdPcieAerReportMechanism                              0x75E75997

/**
 * @brief   Edpc Control                                     0x0E0307B2
 * @details
 *    (0) Disabled; (1) Enabled; (3) Auto
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_EdpcControl                                               0x0E0307B2

/**
 * @brief   ACS RAS Request Value                            0x093DA343
 * @details
 *    Data Type: Word
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_AcsRasValue                                               0x093DA343

/**
 * @brief   NBIO Poison Consumption                          0x92AA5CDA
 * @details
 *    NBIO Poison Consumption
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_DbgPoisonConsumption                                      0x92AA5CDA

/**
 * @brief   Sync Flood on PCIe Fatal Error                   0x27AFD794
 * @details
 *    When "Sync Flood on PCIe Fatal Error" is True, PcdAmdPcieSyncFloodOnFatal
 *    should be set to True.
 *    When "Sync Flood on PCIe Fatal Error" is False,
 *    PcdAmdPcieSyncFloodOnFatal should be set to False.
 *    When "Sync Flood on PCIe Fatal Error" is Auto, PcdAmdPcieSyncFloodOnFatal
 *    should retain its AGESA default.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >  NBIO RAS Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbRasSyncfloodPcieFatalError                          0x27AFD794

/**
 * @brief   Enable AER Cap                                   0x6D48CA11
 * @details
 *    Enables Advanced Error Reporting Capability
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_DbgGnbDbgAERCAPEnable                                     0x6D48CA11

/**
 * @brief   Early Link Speed                                 0x87CEF64A
 * @details
 *    Set Early Link Speed
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnEarlyLinkSpeed                                         0x87CEF64A

/**
 * @brief   Hot Plug Handling mode                           0x9E8FC47C
 * @details
 *    Control the Hot Plug Handling mode
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_HotPlugHandlingMode                                       0x9E8FC47C

/**
 * @brief   Presence Detect Select mode                      0xD86552EE
 * @details
 *    Control the Presence Detect Select mode
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_PresenceDetectSelectmode                                  0xD86552EE

/**
 * @brief   Data Link Feature Cap                            0xFB5A4A80
 * @details
 *    Data Link Feature Capability
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbDataLinkFeatureCap                                  0xFB5A4A80

/**
 * @brief   Data Link Feature Exchange                       0x09CA41EB
 * @details
 *    Data Link Feature Exchange
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbDataLinkFeatureExchange                             0x09CA41EB

/**
 * @brief   CV test                                          0x969E3F1F
 * @details
 *    Set this to Enabled to support running PCIECV tool.\n  Auto - preserve
 *    h/w defaults
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnPcieCVTestWA                                           0x969E3F1F

/**
 * @brief   SEV-SNP Support                                  0x99AACF00
 * @details
 *    Enables support for Secure Encrypted Virtualization and Secure Nested
 *    Paging
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_SevSnpSupport                                             0x99AACF00

/**
 * @brief   Allow Compliance                                 0xA3A1EBE6
 * @details
 *    When enabled, allows the PCIe RP to enter Polling.Compliance state
 *    Data Type: Word
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_STRAP_COMPLIANCE_DIS                                      0xA3A1EBE6

/**
 * @brief   SRIS                                             0x5A189B9F
 * @details
 *    SRIS
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_CmnGnbSris                                                0x5A189B9F

/**
 * @brief   Multi Upstream Auto Speed Change                 0xFFB68BDA
 * @details
 *    Defines the setting of this feature for all PCIe devices.  "Auto" uses
 *    the DXIO default setting of 0 for Gen1 and 1 for Gen2/3
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_DbgLcMultUpstreamAuto                                     0xFFB68BDA

/**
 * @brief   Multi Auto Speed Change On Last Rate             0xB2A14D4B
 * @details
 *    Force PCIe link training speed to last advertised for all ports.
 *    Disabled=Use highest data rate ever advertised.
 *    Enabled=Use last data rate advertised.
 *    Data Type: Byte
 *    Category:  NBIO Common Options >
 **/
#define    ACT_CFG_UID_DbgNbioForceSpeedLastAdvertised                           0xB2A14D4B

/**
 * @brief   I3C/I2C 0 Enable                                 0xDA874D4F
 * @details
 *    Enable or disable Inter-Integrated Circuit Control 0
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C0Config                                          0xDA874D4F

/**
 * @brief   I3C 0 Mode                                       0x5ED642C1
 * @details
 *    choose I3C basic mode or I2C mode.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C0Mode                                            0x5ED642C1

/**
 * @brief   I3C/I2C 1 Enable                                 0x2E7C8DB1
 * @details
 *    Enable or disable Inter-Integrated Circuit Control 1
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C1Config                                          0x2E7C8DB1

/**
 * @brief   I3C 1 Mode                                       0xFD624D03
 * @details
 *    choose I3C basic mode or I2C mode.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C1Mode                                            0xFD624D03

/**
 * @brief   I3C/I2C 2 Enable                                 0x90941551
 * @details
 *    Enable or disable Inter-Integrated Circuit Control 2
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C2Config                                          0x90941551

/**
 * @brief   I3C 2 Mode                                       0xB38BF187
 * @details
 *    choose I3C basic mode or I2C mode.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C2Mode                                            0xB38BF187

/**
 * @brief   I3C/I2C 3 Enable                                 0x6EA04240
 * @details
 *    Enable or disable Inter-Integrated Circuit Control 3
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C3Config                                          0x6EA04240

/**
 * @brief   I3C 3 Mode                                       0x5B73B05F
 * @details
 *    choose I3C basic mode or I2C mode.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI3C3Mode                                            0x5B73B05F

/**
 * @brief   I2C 4 Enable                                     0xB5848497
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller4
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C4Config                                          0xB5848497

/**
 * @brief   I2C 5 Enable                                     0x8B7EAA3B
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller5
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C5Config                                          0x8B7EAA3B

/**
 * @brief   Release SPD Host Control                         0xA90572F0
 * @details
 *    Release SPD Host Control, so that BMC can take over the ownership of
 *    I2C/I3C bus
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I3C/I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchReleaseSpdHostControl                               0xA90572F0

/**
 * @brief   SATA Enable                                      0x89040012
 * @details
 *    Disable or enable OnChip SATA controller
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchSataEnable                                          0x89040012

/**
 * @brief   SATA Mode                                        0x54EAFE00
 * @details
 *    Select OnChip SATA Type
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchSataClass                                           0x54EAFE00

/**
 * @brief   SATA RAS Support                                 0xADD024CD
 * @details
 *    Disable or enable Sata RAS Support
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchSataRasSupport                                      0xADD024CD

/**
 * @brief   SATA Staggered Spin-up                           0x13950FB1
 * @details
 *    Enable or disable SATA staggered spin-up.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchSataStaggeredSpinup                                 0x13950FB1

/**
 * @brief   SATA Disabled AHCI Prefetch Function             0xC485DF8B
 * @details
 *    Disable or enable Sata Disabled AHCI Prefetch Function
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchSataAhciDisPrefetchFunction                         0xC485DF8B

/**
 * @brief   Aggresive SATA Device Sleep P0                   0x2BF839C8
 * @details
 *    Enable SATA DevSlp0. In SOC two DEVSLP pads are assigned.
 *    Aggressive Device Sleep enables the HBA to assert the DEVSLP signal as
 *    soon as there are no commands outstanding to the device and the port
 *    specific Device Sleep idle timer has expired.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_DbgFchSataAggresiveDevSlpP0                               0x2BF839C8

/**
 * @brief   Aggresive SATA Device Sleep P1                   0x942A4874
 * @details
 *    Enable SATA DevSlp1. In SOC two DEVSLP pads are assigned.
 *    Aggressive Device Sleep enables the HBA to assert the DEVSLP signal as
 *    soon as there are no commands outstanding to the device and the port
 *    specific Device Sleep idle timer has expired.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >
 **/
#define    ACT_CFG_UID_DbgFchSataAggresiveDevSlpP1                               0x942A4874

/**
 * @brief   Sata0 Enable                                     0x77775AA9
 * @details
 *    Enable or Disable Sata0.
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata0Enable                                         0x77775AA9

/**
 * @brief   Sata1 Enable                                     0xFAECF601
 * @details
 *    Enable or Disable Sata1.
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata1Enable                                         0xFAECF601

/**
 * @brief   Sata2 Enable                                     0x6DD6DE40
 * @details
 *    Enable or Disable Sata2.
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata2Enable                                         0x6DD6DE40

/**
 * @brief   Sata3 Enable                                     0x352921AF
 * @details
 *    Enable or Disable Sata3.
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata3Enable                                         0x352921AF

/**
 * @brief   Sata4 (Socket1) Enable                           0x60E0F968
 * @details
 *    Enable or Disable Sata4 on Socket 1 (IOD1)..
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata4Enable                                         0x60E0F968

/**
 * @brief   Sata5 (Socket1) Enable                           0xBEF46759
 * @details
 *    Enable or Disable Sata5 on Socket 1 (IOD1)..
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata5Enable                                         0xBEF46759

/**
 * @brief   Sata6 (Socket1) Enable                           0x9309B6E5
 * @details
 *    Enable or Disable Sata6 on Socket 1 (IOD1)..
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata6Enable                                         0x9309B6E5

/**
 * @brief   Sata7 (Socket1) Enable                           0x7CAB1335
 * @details
 *    Enable or Disable Sata7 on Socket 1 (IOD1)..
 *    Each IOD has 4 Sata Controllers.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller Enable >
 **/
#define    ACT_CFG_UID_DbgFchSata7Enable                                         0x7CAB1335

/**
 * @brief   Socket1 DevSlp0 Enable                           0x245537C0
 * @details
 *    Only Sata0 on each IOD/socket support DevSlp.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller DevSlp >  Socket1 DevSlp >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie4DevSlp0                                  0x245537C0

/**
 * @brief   Socket1 DevSlp1 Enable                           0x8B1A294D
 * @details
 *    Only Sata0 on each IOD/socket support DevSlp.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller DevSlp >  Socket1 DevSlp >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie4DevSlp1                                  0x8B1A294D

/**
 * @brief   Sata0 SGPIO                                      0x09BF5E95
 * @details
 *    Enable or Disable SataSgpio on Sata0
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataSgpio0                                          0x09BF5E95

/**
 * @brief   Sata1 SGPIO                                      0xAA6A037B
 * @details
 *    Enable or Disable SataSgpio on Sata1
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie1Sgpio0                                   0xAA6A037B

/**
 * @brief   Sata2 SGPIO                                      0xCD4F7C06
 * @details
 *    Enable or Disable SataSgpio on Sata2
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie2Sgpio0                                   0xCD4F7C06

/**
 * @brief   Sata3 SGPIO                                      0x5838BB1D
 * @details
 *    Enable or Disable SataSgpio on Sata3
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie3Sgpio0                                   0x5838BB1D

/**
 * @brief   Sata4 SGPIO                                      0x3D36817A
 * @details
 *    Enable or Disable SataSgpio on Sata4 (Socket1)
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie4Sgpio0                                   0x3D36817A

/**
 * @brief   Sata5 SGPIO                                      0x1A3AD3E7
 * @details
 *    Enable or Disable SataSgpio on Sata5 (Socket1)
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie5Sgpio0                                   0x1A3AD3E7

/**
 * @brief   Sata6 SGPIO                                      0xDA0E8C67
 * @details
 *    Enable or Disable SataSgpio on Sata6 (Socket1)
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie6Sgpio0                                   0xDA0E8C67

/**
 * @brief   Sata7 SGPIO                                      0x604E4288
 * @details
 *    Enable or Disable SataSgpio on Sata7 (Socket1)
 *    Data Type: Byte
 *    Category:  FCH Common Options >  SATA Configuration Options >  SATA Controller options >  SATA Controller SGPIO >
 **/
#define    ACT_CFG_UID_DbgFchSataMcmDie7Sgpio0                                   0x604E4288

/**
 * @brief   XHCI Controller0 enable                          0x05E9FC7D
 * @details
 *    Enable or disable USB3 controller.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  USB Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUsbXHCI0Enable                                      0x05E9FC7D

/**
 * @brief   XHCI Controller1 enable                          0xA0528895
 * @details
 *    Enable or disable USB3 controller.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  USB Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUsbXHCI1Enable                                      0xA0528895

/**
 * @brief   USB ecc SMI Enable                               0x830375CF
 * @details
 *    Enable Double Error Detection output signal for USB S0 ram.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  USB Configuration Options >
 **/
#define    ACT_CFG_UID_DbgFchUSBeccSMIEnable                                     0x830375CF

/**
 * @brief   XHCI2 enable (Socket1)                           0x0F906157
 * @details
 *    Enable or disable USB3 controller.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  USB Configuration Options >  MCM USB enable >
 **/
#define    ACT_CFG_UID_CmnFchUsbXHCI2Enable                                      0x0F906157

/**
 * @brief   XHCI3 enable (Socket1)                           0x04F988E7
 * @details
 *    Enable or disable USB3 controller.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  USB Configuration Options >  MCM USB enable >
 **/
#define    ACT_CFG_UID_CmnFchUsbXHCI3Enable                                      0x04F988E7

/**
 * @brief   Ac Loss Control                                  0xC0399970
 * @details
 *    Select Ac Loss Control Method
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Ac Power Loss Options >
 **/
#define    ACT_CFG_UID_CmnFchSystemPwrFailShadow                                 0xC0399970

/**
 * @brief   I2C 0 Enable                                     0xF07F520B
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller0
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C0Config                                          0xF07F520B

/**
 * @brief   I2C 1 Enable                                     0x79FBD5E7
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller1
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C1Config                                          0x79FBD5E7

/**
 * @brief   I2C 2 Enable                                     0x49E2E5D5
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller2
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C2Config                                          0x49E2E5D5

/**
 * @brief   I2C 3 Enable                                     0x5A738138
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller3
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C3Config                                          0x5A738138

/**
 * @brief   I2C 4 Enable                                     0xB5848497
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller4
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C4Config                                          0xB5848497

/**
 * @brief   I2C 5 Enable                                     0x8B7EAA3B
 * @details
 *    Enable or disable Inter-Integrated Circuit Controller5
 *    Data Type: Byte
 *    Category:  FCH Common Options >  I2C Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchI2C5Config                                          0x8B7EAA3B

/**
 * @brief   Uart 0 Enable                                    0xD689B34B
 * @details
 *    Enable or disable Uart0.
 *    Uart 0 has no HW flow control if Uart 2 is enabled
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart0Config                                         0xD689B34B

/**
 * @brief   Uart 0 Legacy Options                            0x0DA50CFB
 * @details
 *    Assign Uart0 to receive one of the I/O addresses in 0x3F8-0x3FF,
 *    0x3E8-0x3EF, 0x2F8-0x2FF and 0x2E8-0x2EF.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart0LegacyConfig                                   0x0DA50CFB

/**
 * @brief   Uart 1 Enable                                    0x10C5AE4C
 * @details
 *    Enable or disable Uart1.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart1Config                                         0x10C5AE4C

/**
 * @brief   Uart 1 Legacy Options                            0x1EED48F7
 * @details
 *    Assign Uart1 to receive one of the I/O addresses in 0x3F8-0x3FF,
 *    0x3E8-0x3EF, 0x2F8-0x2FF and 0x2E8-0x2EF.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart1LegacyConfig                                   0x1EED48F7

/**
 * @brief   Uart 2 Enable                                    0x0FECDD3F
 * @details
 *    Enable or disable Uart2.
 *    If Uart 2 is enable, Uart0 has no HW flow control.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart2Config                                         0x0FECDD3F

/**
 * @brief   Uart 2 Legacy Options                            0x0546F345
 * @details
 *    Assign Uart2 to receive one of the I/O addresses in 0x3F8-0x3FF,
 *    0x3E8-0x3EF, 0x2F8-0x2FF and 0x2E8-0x2EF.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart2LegacyConfig                                   0x0546F345

/**
 * @brief   Uart 3 Enable                                    0x536E27F6
 * @details
 *    Enable or disable Uart3.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart3Config                                         0x536E27F6

/**
 * @brief   Uart 3 Legacy Options                            0x699D1271
 * @details
 *    Assign Uart3 to receive one of the I/O addresses in 0x3F8-0x3FF,
 *    0x3E8-0x3EF, 0x2F8-0x2FF and 0x2E8-0x2EF.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Uart Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchUart3LegacyConfig                                   0x699D1271

/**
 * @brief   ESPI Enable                                      0xF6991E19
 * @details
 *    Data Type: Byte
 *    Category:  FCH Common Options >  ESPI Configuration Options >
 **/
#define    ACT_CFG_UID_CmnFchEspiConfig                                          0xF6991E19

/**
 * @brief   ALink RAS Support                                0x10AA4A5A
 * @details
 *    Enable FCH A-Link parity error
 *    Data Type: Byte
 *    Category:  FCH Common Options >  FCH RAS Options >
 **/
#define    ACT_CFG_UID_CmnFchAlinkRasSupport                                     0x10AA4A5A

/**
 * @brief   Reset After Sync-Flood                           0x3C8E196A
 * @details
 *    Enable AB to forward downstream sync-flood message to system
 *    controller.
 *    Data Type: Byte
 *    Category:  FCH Common Options >  FCH RAS Options >
 **/
#define    ACT_CFG_UID_DbgFchSyncfloodEnable                                     0x3C8E196A

/**
 * @brief   Boot Timer Enable                                0x6FB71261
 * @details
 *    Boot Timer enable.
 *    Enable : force PMx44 bit 27 = 1
 *    Disable : force PMx44 bit 27 = 0
 *    Auto:PMx44 bit 27 = PcdBootTimerEnable
 *    Data Type: Byte
 *    Category:  FCH Common Options >  Miscellaneous Options >
 **/
#define    ACT_CFG_UID_CmnBootTimerEnable                                        0x6FB71261

/**
 * @brief   Socket-0 P0 NTB Enable                           0xAE709F12
 * @details
 *    Enable NTB on Socket-0 P0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbP0P0                                             0xAE709F12

/**
 * @brief   Socket-0 P0 Link Speed                           0x08327CD7
 * @details
 *    Link Speed for Socket-0 P0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbLinkSpeedP0P0                                    0x08327CD7

/**
 * @brief   Socket-0 P0 NTB Mode                             0xEBC2CBB0
 * @details
 *    NTB Mode for Socket-0 P0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbModeP0P0                                         0xEBC2CBB0

/**
 * @brief   Socket-0 P2 NTB Enable                           0xCC3575B5
 * @details
 *    Enable NTB on Socket-0 P2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbP0P2                                             0xCC3575B5

/**
 * @brief   Socket-0 P2 Link Speed                           0x3982BFAB
 * @details
 *    Link Speed for Socket-0 P2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbLinkSpeedP0P2                                    0x3982BFAB

/**
 * @brief   Socket-0 P2 NTB Mode                             0xE78C762F
 * @details
 *    NTB Mode for Socket-0 P2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbModeP0P2                                         0xE78C762F

/**
 * @brief   Socket-0 G0 NTB Enable                           0xAAC49231
 * @details
 *    Enable NTB on Socket-0 G0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbP0G0                                             0xAAC49231

/**
 * @brief   Socket-0 G0 Link Speed                           0x65C44304
 * @details
 *    Link Speed for Socket-0 G0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbLinkSpeedP0G0                                    0x65C44304

/**
 * @brief   Socket-0 G0 NTB Mode                             0x4F558396
 * @details
 *    NTB Mode for Socket-0 G0 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbModeP0G0                                         0x4F558396

/**
 * @brief   Socket-0 G2 NTB Enable                           0x1A747A94
 * @details
 *    Enable NTB on Socket-0 G2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbP0G2                                             0x1A747A94

/**
 * @brief   Socket-0 G2 Link Speed                           0xDDA9C932
 * @details
 *    Link Speed for Socket-0 G2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbLinkSpeedP0G2                                    0xDDA9C932

/**
 * @brief   Socket-0 G2 NTB Mode                             0x696F046C
 * @details
 *    NTB Mode for Socket-0 G2 Link
 *    Data Type: Byte
 *    Category:  NTB Common Options >
 **/
#define    ACT_CFG_UID_DbgSP3NtbModeP0G2                                         0x696F046C

/**
 * @brief   ABL Console Out Control                          0xB23AB427
 * @details
 *    Enable : Enable ConsoleOut Function for ABL
 *    Disable : Disable ConsoleOut Function for ABL
 *    Auto : Keep default behavior
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_CmnSocAblConOut                                           0xB23AB427

/**
 * @brief   ABL Console Out Serial Port                      0x0715D12E
 * @details
 *    LPC UART : Enabled serial port through LPC UART
 *    SOC UART0 : Enabled serial port through SOC UART0
 *    SOC UART1 : Enabled serial port through SOC UART1
 *    Auto : Keep default behavior
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_CmnSocAblConOutSerialPort                                 0x0715D12E

/**
 * @brief   ABL Basic Console Out Control                    0xEB5B7D49
 * @details
 *    Enable : Enable Basic ConsoleOut Function for ABL
 *    Disable : Disable Basic ConsoleOut Function for ABL
 *    Auto : Keep default behavior
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_CmnSocAblConOutBasic                                      0xEB5B7D49

/**
 * @brief   ABL PMU message Control                          0x19CAF4F8
 * @details
 *    To control the total number of PMU debug messages.
 *    Several major controls are listed below:
 *    1. Detailed debug messages (e.g. Eye delays)
 *    2. Coarse debug messages (e.g. rank information)
 *    3. Stage completion
 *    4. Firmware completion messages only
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_CmnSocAblPmuMsgCtrl                                       0x19CAF4F8

/**
 * @brief   PSP error injection support                      0xC4D364D5
 * @details
 *    Enable EINJ support
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_DfCmnPspErrInj                                            0xC4D364D5

/**
 * @brief   DRAM Survives Warm Reset                         0x52C06E52
 * @details
 *    1 - Enabled (default); 0 - Disabled
 *    If enabled - Upon warm reset DRAM content is preserved, Training values
 *    are saved & retrieved.
 *    Data Type: Byte
 *    Category:  Soc Miscellaneous Control >
 **/
#define    ACT_CFG_UID_DramSurvivesWarmReset                                     0x52C06E52

/**
 * @brief   Workload Profile                                 0x829A1085
 * @details
 *    Select the profile for different workloads.
 *    Data Type: Byte
 *    Category:  Workload Tuning >
 **/
#define    ACT_CFG_UID_CmnWorkloadProfile                                        0x829A1085

/**
 * @brief   Performance Tracing                              0xB24877D7
 * @details
 *    Enable to allow capturing performance traces.
 *    Data Type: Byte
 *    Category:  Workload Tuning >
 **/
#define    ACT_CFG_UID_CmnPerformanceTracing                                     0xB24877D7

/**
 * @brief   CXL Control                                      0x3A5668F3
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >
 **/
#define    ACT_CFG_UID_CmnCxlControl                                             0x3A5668F3

/**
 * @brief   CXL ASPM                                         0xE9509831
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >
 **/
#define    ACT_CFG_UID_CmnCxlAspm                                                0xE9509831

/**
 * @brief   L1                                               0x1380EF3E
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >  CXL vLSM Power Management >  CXL.io >
 **/
#define    ACT_CFG_UID_CmnCxlIoL1                                                0x1380EF3E

/**
 * @brief   L2                                               0xD311D172
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >  CXL vLSM Power Management >  CXL.io >
 **/
#define    ACT_CFG_UID_CmnCxlIoL2                                                0xD311D172

/**
 * @brief   L1                                               0xBB612DD5
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >  CXL vLSM Power Management >  CXL.camem >
 **/
#define    ACT_CFG_UID_CmnCxlmemL1                                               0xBB612DD5

/**
 * @brief   L2                                               0x343048CD
 * @details
 *    Data Type: Byte
 *    Category:  CXL Common Options >  CXL vLSM Power Management >  CXL.camem >
 **/
#define    ACT_CFG_UID_CmnCxlmemL2                                               0x343048CD

#endif //  ACT_OPTIONS_
