/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _AMD_CBS_VARIABLE_H_
#define _AMD_CBS_VARIABLE_H_

#pragma pack(push,1)

typedef struct _CBS_VARIABLE_HEADER
{
  UINT32 CbsVariableStructUniqueValue;                             ///< Will be different if CBS varialbe structure changes
  UINT32 NewRecordOffset;                                          ///< Record the offset of reserved region start, which also the offset of the new record
  UINT32 ApcbVariableHash;                                         ///< Record the APCB Hash Value
  UINT16 CbsComboChipsetFlag;                                      ///< Combo Chipset Flag
  UINT8  CbsChipsetVisibleFlag0;                                   ///< Chipset Visible Flag0
  UINT8  CbsChipsetVisibleFlag1;                                   ///< Chipset Visible Flag1
  UINT32 CbsRevisionNumber;                                        ///< CBS Revision Number
  UINT8  Reserved[12];                                             ///< Reserved for future use
} CBS_VARIABLE_HEADER;


typedef struct _CBS_CONFIG {
  CBS_VARIABLE_HEADER  Header;                                    ///< Variable header
  UINT8         CbsComboFlag;                                     ///< Combo CBS
  UINT8         CbsCmnCpuGenWA05;                                 ///< RedirectForReturnDis
  UINT8         CbsCmnCpuCpb;                                     ///< Core Performance Boost
  UINT8         CbsCmnCpuGlobalCstateCtrl;                        ///< Global C-state Control
  UINT8         CbsCmnCpuOpcacheGrayoutFlag;                      ///< Opcache grayout flag
  UINT8         CbsCmnCpuOpcacheCtrl;                             ///< Opcache Control
  UINT8         CbsCmnCpuSevAsidCount;                            ///< SEV ASID Count
  UINT8         CbsCmnCpuSevAsidSpaceCtrl;                        ///< SEV-ES ASID Space Limit Control
  UINT32        CbsCmnCpuSevAsidSpaceLimit;                       ///< SEV-ES ASID Space Limit
  UINT8         CbsCmnCpuStreamingStoresCtrl;                     ///< Streaming Stores Control
  UINT8         CbsCpuLApicMode;                                  ///< Local APIC Mode
  UINT8         CbsCmnCpuCstC1Ctrl;                               ///< ACPI _CST C1 Declaration
  UINT8         CbsCmnCpuPMCtlPss;                                ///< ACPI _PSS
  UINT8         CbsCmnCpuMcaErrThreshEn;                          ///< MCA error thresh enable
  UINT16        CbsCmnCpuMcaErrThreshCount;                       ///< MCA error thresh count
  UINT8         CbsCmnCpuSmuPspDebugMode;                         ///< SMU and PSP Debug Mode
  UINT8         CbsCmnCpuPpinCtrl;                                ///< PPIN Opt-in
  UINT8         CbsCmnCpuFSRM;                                    ///< Fast Short REP MOVSB
  UINT8         CbsCmnCpuERMS;                                    ///< Enhanced REP MOVSB/STOSB
  UINT8         CbsCmnActionOnBistFailure;                        ///< Action on BIST Failure
  UINT8         CbsCmnCpuIbsHardwareCtrl;                         ///< IBS hardware workaround
  UINT8         CbsCmnCpuOcMode;                                  ///< OC Mode
  UINT8         CbsCpuLegalDisclaimer;                            ///< Pstates Disclaimer
  UINT8         CbsCpuLegalDisclaimer1;                           ///< Pstates Disclaimer 1
  UINT8         CbsCpuPstCustomP0;                                ///< Custom Pstate0
  UINT32        CbsCpuPst0Freq;                                   ///< Pstate0 Freq (MHz)
  UINT8         CbsCpuPst0Vid;                                    ///< Pstate0 VID
  UINT8         CbsCmnCpuL1StreamHwPrefetcher;                    ///< L1 Stream HW Prefetcher
  UINT8         CbsCmnCpuL2StreamHwPrefetcher;                    ///< L2 Stream HW Prefetcher
  UINT8         CbsDbgCpuGenCpuWdt;                               ///< Core Watchdog Timer Enable
  UINT16        CbsDbgCpuGenCpuWdtTimeout;                        ///< Core Watchdog Timer Interval
  UINT8         CbsCmnCpuWdtSev;                                  ///< Core Watchdog Timer Severity
  UINT8         CbsCmnCpuCoreCtrlRmb;                             ///< Core control
  UINT8         CbsCmnCpuSmtCtrl;                                 ///< SMT Control
  UINT32        CbsCmnPersistentLow;                              ///< Persistent memory Base Low
  UINT32        CbsCmnPersistentHigh;                             ///< Persistent memory Base High
  UINT32        CbsCmnPersistentSizeLow;                          ///< Persistent memory reserved size Low
  UINT32        CbsCmnPersistentSizeHigh;                         ///< Persistent memory reserved size High
  UINT8         CbsCmnPersistentMemRetention;                     ///< Persistent memory Retention
  UINT8         CbsDfCmnCc6MemEncryption;                         ///< CC6 memory region encryption
  UINT8         CbsDfCmnMemClear;                                 ///< Memory Clear
  UINT8         CbsDfCmnExtIpSyncFloodProp;                       ///< Disable DF to external downstream IP SyncFloodPropagation
  UINT8         CbsDfCmnSyncFloodProp;                            ///< Disable DF sync flood propagation
  UINT8         CbsDfCmnFreezeQueueError;                         ///< Freeze DF module queues on error
  UINT8         CbsDfCmnCstates;                                  ///< DF Cstates
  UINT8         CbsMpExcepRecovHandle;                            ///< MP exception recovery handling
  UINT8         CbsMemClrTimeout;                                 ///< Memory clear timeout
  UINT8         CbsDfCmnMemIntlvSize;                             ///< Memory interleaving size
  UINT8         CbsDfCmnDramMapInversion;                         ///< DRAM map inversion
  UINT8         CbsCmnMemTimingLegalDisclaimerDdr;                ///< DRAM Timing Configuration Legal Disclaimer
  UINT8         CbsCmnMemTimingLegalDisclaimer1Ddr;               ///< DRAM Timing Configuration Legal Disclaimer 1
  UINT8         CbsCmnMemTimingSettingDdr;                        ///< Active Memory Timing Settings
  UINT16        CbsCmnMemTargetSpeedDdr;                          ///< Memory Target Speed
  UINT8         CbsCmnMemTimingTclCtrlDdr;                        ///< Tcl Ctrl
  UINT16        CbsCmnMemTimingTclDdr;                            ///< Tcl
  UINT8         CbsCmnMemTimingTrcdCtrlDdr;                       ///< Trcd Ctrl
  UINT16        CbsCmnMemTimingTrcdDdr;                           ///< Trcd
  UINT8         CbsCmnMemTimingTrpCtrlDdr;                        ///< Trp Ctrl
  UINT16        CbsCmnMemTimingTrpDdr;                            ///< Trp
  UINT8         CbsCmnMemTimingTrasCtrlDdr;                       ///< Tras Ctrl
  UINT16        CbsCmnMemTimingTrasDdr;                           ///< Tras
  UINT8         CbsCmnMemTimingTrcCtrlDdr;                        ///< Trc Ctrl
  UINT16        CbsCmnMemTimingTrcDdr;                            ///< Trc
  UINT8         CbsCmnMemTimingTwrCtrlDdr;                        ///< Twr Ctrl
  UINT16        CbsCmnMemTimingTwrDdr;                            ///< Twr
  UINT8         CbsCmnMemTimingTrfc1CtrlDdr;                      ///< Trfc1 Ctrl
  UINT16        CbsCmnMemTimingTrfc1Ddr;                          ///< Trfc1
  UINT8         CbsCmnMemTimingTrfc2CtrlDdr;                      ///< Trfc2 Ctrl
  UINT16        CbsCmnMemTimingTrfc2Ddr;                          ///< Trfc2
  UINT8         CbsCmnMemTimingTrfcSbCtrlDdr;                     ///< TrfcSb Ctrl
  UINT16        CbsCmnMemTimingTrfcSbDdr;                         ///< TrfcSb
  UINT8         CbsCmnMemTimingTrtpCtrlDdr;                       ///< Trtp Ctrl
  UINT16        CbsCmnMemTimingTrtpDdr;                           ///< Trtp
  UINT8         CbsCmnMemTimingTrrdLCtrlDdr;                      ///< TrrdL Ctrl
  UINT16        CbsCmnMemTimingTrrdLDdr;                          ///< TrrdL
  UINT8         CbsCmnMemTimingTrrdSCtrlDdr;                      ///< TrrdS Ctrl
  UINT16        CbsCmnMemTimingTrrdSDdr;                          ///< TrrdS
  UINT8         CbsCmnMemTimingTfawCtrlDdr;                       ///< Tfaw Ctrl
  UINT16        CbsCmnMemTimingTfawDdr;                           ///< Tfaw
  UINT8         CbsCmnMemTimingTwtrLCtrlDdr;                      ///< TwtrL Ctrl
  UINT16        CbsCmnMemTimingTwtrLDdr;                          ///< TwtrL
  UINT8         CbsCmnMemTimingTwtrSCtrlDdr;                      ///< TwtrS Ctrl
  UINT16        CbsCmnMemTimingTwtrSDdr;                          ///< TwtrS
  UINT8         CbsCmnMemTimingTrdrdScLCtrlDdr;                   ///< TrdrdScL Ctrl
  UINT16        CbsCmnMemTimingTrdrdScLDdr;                       ///< TrdrdScL
  UINT8         CbsCmnMemTimingTrdrdScCtrlDdr;                    ///< TrdrdSc Ctrl
  UINT16        CbsCmnMemTimingTrdrdScDdr;                        ///< TrdrdSc
  UINT8         CbsCmnMemTimingTrdrdSdCtrlDdr;                    ///< TrdrdSd Ctrl
  UINT16        CbsCmnMemTimingTrdrdSdDdr;                        ///< TrdrdSd
  UINT8         CbsCmnMemTimingTrdrdDdCtrlDdr;                    ///< TrdrdDd Ctrl
  UINT16        CbsCmnMemTimingTrdrdDdDdr;                        ///< TrdrdDd
  UINT8         CbsCmnMemTimingTwrwrScLCtrlDdr;                   ///< TwrwrScL Ctrl
  UINT16        CbsCmnMemTimingTwrwrScLDdr;                       ///< TwrwrScL
  UINT8         CbsCmnMemTimingTwrwrScCtrlDdr;                    ///< TwrwrSc Ctrl
  UINT16        CbsCmnMemTimingTwrwrScDdr;                        ///< TwrwrSc
  UINT8         CbsCmnMemTimingTwrwrSdCtrlDdr;                    ///< TwrwrSd Ctrl
  UINT16        CbsCmnMemTimingTwrwrSdDdr;                        ///< TwrwrSd
  UINT8         CbsCmnMemTimingTwrwrDdCtrlDdr;                    ///< TwrwrDd Ctrl
  UINT16        CbsCmnMemTimingTwrwrDdDdr;                        ///< TwrwrDd
  UINT8         CbsCmnMemTimingTwrrdCtrlDdr;                      ///< Twrrd Ctrl
  UINT16        CbsCmnMemTimingTwrrdDdr;                          ///< Twrrd
  UINT8         CbsCmnMemTimingTrdwrCtrlDdr;                      ///< Trdwr Ctrl
  UINT16        CbsCmnMemTimingTrdwrDdr;                          ///< Trdwr
  UINT8         CbsCmnMemDrvStrenCaDdr;                           ///< Processor CA drive strengths
  UINT8         CbsCmnMemDrvStrenCsDdr;                           ///< Processor CS drive strengths
  UINT8         CbsCmnMemDrvStrenCkDdr;                           ///< Processor CK drive strengths
  UINT8         CbsCmnMemDrvStrenDqDdr;                           ///< Processor DQ drive strengths
  UINT8         CbsCmnMemOdtImpedProcDdr;                         ///< Processor ODT impedance
  UINT8         CbsCmnMemDramDrvStrenDqDdr;                       ///< Dram DQ drive strengths
  UINT8         CbsCmnMemDramOdtImpedRttNomWrDdr;                 ///< Dram ODT impedance RTT_NOM_WR
  UINT8         CbsCmnMemDramOdtImpedRttNomRdDdr;                 ///< Dram ODT impedance RTT_NOM_RD
  UINT8         CbsCmnMemDramOdtImpedRttWrDdr;                    ///< Dram ODT impedance RTT_WR
  UINT8         CbsCmnMemDramOdtImpedRttParkDdr;                  ///< Dram ODT impedance RTT_PARK
  UINT8         CbsCmnMemDramOdtImpedRttParkDqsDdr;               ///< Dram ODT impedance DQS_RTT_PARK
  UINT8         CbsCmnMemCtrllerPowerDownEnDdr;                   ///< Power Down Enable
  UINT8         CbsCmnMemCtrllerPdPhyPowerSaveDisDdr;             ///< Phy Low Power Disable
  UINT8         CbsCmnMemEccDisErrInjectionDdr;                   ///< Disable Memory Error Injection
  UINT8         CbsCmnMemDataPoisoningDdr4;                       ///< Data Poisoning
  UINT8         CbsCmnMemEccEnDdr;                                ///< ECC
  UINT8         CbsCmnMemDramRedirectScrubEnDdr;                  ///< DRAM Redirect Scrubber Enable
  UINT8         CbsCmnMemDramRedirectScrubLimitDdr;               ///< DRAM Scrub Redirection Limit
  UINT8         CbsCmnMemDramScrubTime;                           ///< DRAM Scrub Time
  UINT8         CbsCmnMemTsmeEnableDdr;                           ///< TSME
  UINT8         CbsCmnMemDataCtrlDataScrambleEnDdr;               ///< Data Scramble
  UINT8         CbsCmnMemSpdCrcVerify;                            ///< Enable SPD CRC verification
  UINT8         CbsCmnMemSpdReadRetryCountCrc;                    ///< SPD read retry count on CRC failure
  UINT8         CbsCmnMemMappingBankInterleaveDdr;                ///< Chipselect Interleaving
  UINT8         CbsCmnMemAddressHashBankDdr;                      ///< Address Hash Bank
  UINT8         CbsCmnMemAddressHashCsDdr;                        ///< Address Hash CS
  UINT8         CbsCmnMemCtrllerBankSwapModeDdr;                  ///< BankSwapMode
  UINT32        CbsCmnMemRestoreValidDays;                        ///< Memory Restore valid days
  UINT8         CbsCmnMemCtrllerPmuTrainDfeDdr;                   ///< DFE Read Training
  UINT8         CbsCmnMemDramPdaEnumIdProgModeDdr;                ///< DRAM PDA Enumerate ID Programming Mode
  UINT8         CbsCmnMemMbistEnDdr;                              ///< MBIST Enable
  UINT8         CbsCmnMemMbistTestmodeDdr;                        ///< MBIST Test Mode
  UINT8         CbsCmnMemMbistAggressorsDdr;                      ///< MBIST Aggressors
  UINT8         CbsCmnMemMbistPerBitSlaveDieReportDdr;            ///< MBIST Per Bit Slave Die Reporting
  UINT8         CbsCmnMemMbistPatternSelectDdr;                   ///< Pattern Select
  UINT8         CbsCmnMemMbistPatternLengthDdr;                   ///< Pattern Length
  UINT8         CbsCmnMemMbistAggressorsChnlDdr;                  ///< Aggressor Channel
  UINT8         CbsCmnMemTimingLegalDisclaimer;                   ///< DRAM Timing Configuration Legal Disclaimer
  UINT8         CbsCmnMemTimingLegalDisclaimer1;                  ///< DRAM Timing Configuration Legal Disclaimer 1
  UINT8         CbsCmnMemOverclockLpddr;                          ///< Active Memory Timing Settings
  UINT8         CbsCmnMemSpeedLpddr;                              ///< Maximum Memory Data Clock Speed
  UINT8         CbsCmnMemTimingTrcpageCtrlLpddr;                  ///< Trcpage Ctrl
  UINT16        CbsCmnMemTimingTrcpageLpddr;                      ///< Trcpage
  UINT8         CbsCmnMemTimingTcwlLpddr;                         ///< Tcwl
  UINT8         CbsCmnMemTimingTclLpddr;                          ///< Tcl
  UINT8         CbsCmnMemTimingTrcdrdLpddr;                       ///< Trcdrd
  UINT8         CbsCmnMemTimingTrcdwrLpddr;                       ///< Trcdwr
  UINT8         CbsCmnMemTimingTrpLpddr;                          ///< Trp
  UINT8         CbsCmnMemTimingTrfcCtrlLpddr;                     ///< Trfc Ctrl
  UINT16        CbsCmnMemTimingTrfcLpddr;                         ///< Trfc
  UINT8         CbsCmnMemTimingTrasLpddr;                         ///< Tras
  UINT8         CbsCmnMemTimingTrcCtrlLpddr;                      ///< Trc Ctrl
  UINT8         CbsCmnMemTimingTrcLpddr;                          ///< Trc
  UINT8         CbsCmnMemTimingTrrdSLpddr;                        ///< TrrdS
  UINT8         CbsCmnMemTimingTrrdLLpddr;                        ///< TrrdL
  UINT8         CbsCmnMemTimingTfawCtrlLpddr;                     ///< Tfaw Ctrl
  UINT8         CbsCmnMemTimingTfawLpddr;                         ///< Tfaw
  UINT8         CbsCmnMemTimingTwtrSLpddr;                        ///< TwtrS
  UINT8         CbsCmnMemTimingTwtrLLpddr;                        ///< TwtrL
  UINT8         CbsCmnMemTimingTwrCtrlLpddr;                      ///< Twr Ctrl
  UINT8         CbsCmnMemTimingTwrLpddr;                          ///< Twr
  UINT8         CbsCmnMemTimingTrdrdScLCtrlLpddr;                 ///< TrdrdScL Ctrl
  UINT8         CbsCmnMemTimingTrdrdScLLpddr;                     ///< TrdrdScL
  UINT8         CbsCmnMemTimingTwrwrScLCtrlLpddr;                 ///< TwrwrScL Ctrl
  UINT8         CbsCmnMemTimingTwrwrScLLpddr;                     ///< TwrwrScL
  UINT8         CbsCmnMemTimingTrtpLpddr;                         ///< Trtp
  UINT8         CbsCmnMemTimingTckeLpddr;                         ///< Tcke
  UINT8         CbsCmnMemTimingTrdrdScLpddr;                      ///< TrdrdSc
  UINT8         CbsCmnMemTimingTrdrdDdLpddr;                      ///< TrdrdDd
  UINT8         CbsCmnMemTimingTrdrdSdLpddr;                      ///< TrdrdSd
  UINT8         CbsCmnMemTimingTwrwrScLpddr;                      ///< TwrwrSc
  UINT8         CbsCmnMemTimingTwrwrSdLpddr;                      ///< TwrwrSd
  UINT8         CbsCmnMemTimingTwrwrDdLpddr;                      ///< TwrwrDd
  UINT8         CbsCmnMemTimingTwrrdLpddr;                        ///< Twrrd
  UINT8         CbsCmnMemTimingTrdwrLpddr;                        ///< Trdwr
  UINT8         CbsCmnMemRefModeLpddr;                            ///< LPDDR Refresh Mode
  UINT8         CbsCmnMemRfmEnLpddr;                              ///< LPDDR RFM
  UINT8         CbsCmnMemWckAlwaysOnEnLpddr;                      ///< WCK Always On
  UINT8         CbsCmnMemRrwMemtestCtrlLpddr;                     ///< RRW Memory Test Control
  UINT8         CbsCmnMemCtrllerPwrDnEnLpddr;                     ///< Power Down Enable
  UINT8         CbsCmnMemCtrllerPdPhyPowerSaveDisLpddr;           ///< Phy Low Power Disable
  UINT8         CbsCmnMemCadBusTimingCtlLpddr;                    ///< CAD Bus Timing User Controls
  UINT8         CbsCmnMemAddrCmdSetupLpddr;                       ///< AddrCmdSetup
  UINT8         CbsCmnMemCsOdtSetupLpddr;                         ///< CsOdtSetup
  UINT16        CbsCmnMemPhyIoDrvStrenCaLpddr;                    ///< CA drive strengths
  UINT16        CbsCmnMemPhyIoDrvStrenCsLpddr;                    ///< CS drive strengths
  UINT16        CbsCmnMemPhyIoDrvStrenCkLpddr;                    ///< CK drive strengths
  UINT16        CbsCmnMemPhyIoDrvStrenWckLpddr;                   ///< WCK drive strengths
  UINT16        CbsCmnMemPhyIoDrvStrenDqLpddr;                    ///< DQ drive strengths
  UINT16        CbsCmnMemPhyIoDrvStrenDqsLpddr;                   ///< DQS drive strengths
  UINT16        CbsCmnMemPhyIoOdtStrenProcLpddr;                  ///< Processor ODT impedance
  UINT16        CbsCmnMemPhyIoOdtStrenProcCaLpddr;                ///< Processor CA ODT impedance
  UINT16        CbsCmnMemPhyIoOdtStrenProcCkLpddr;                ///< Processor CK ODT impedance
  UINT16        CbsCmnMemPhyIoOdtStrenProcWckLpddr;               ///< Processor WCK ODT impedance
  UINT8         CbsCmnMemDramIoOdtStrenCaLpddr;                   ///< Dram CA ODT impedance
  UINT8         CbsCmnMemDramIoOdtStrenDqLpddr;                   ///< Dram DQ ODT impedance
  UINT8         CbsCmnMemDramIoOdtStrenWckLpddr;                  ///< Dram WCK ODT impedance
  UINT8         CbsCmnMemDramIoOdtStrenNtdq;                      ///< Dram Non-Target ODT impedance
  UINT8         CbsCmnMemDramIoDrvStrenPdds;                      ///< Dram Pull-Down drive strengths
  UINT8         CbsCmnMemCtrllerDramRdLinkEccEnLpddr;             ///< DRAM Read Link ECC Enable
  UINT8         CbsCmnMemCtrllerDramWrLinkEccEnLpddr;             ///< DRAM Write Link ECC Enable
  UINT8         CbsCmnMemDisMemErrInjLpddr;                       ///< Disable Memory Error Injection
  UINT8         CbsCmnMemTsmeLpddr;                               ///< TSME
  UINT8         CbsCmnMemCtrllerDataScrambleLpddr;                ///< Data Scramble
  UINT8         CbsCmnMemMappingBankInterleaveLpddr;              ///< Chip Select Interleaving
  UINT8         CbsCmnMemCtrllerBankSwapLpddr;                    ///< Bank Swap
  UINT8         CbsCmnMemCtrllerBankGroupSwapLpddr;               ///< BankGroup Swap
  UINT8         CbsCmnMemAddressHashBankLpddr;                    ///< Address Hash Bank
  UINT8         CbsCmnMemAddressHashCsLpddr;                      ///< Address Hash CS
  UINT8         CbsCmnMemCtrllerPmuTrainDfeLpddr;                 ///< DFE Read Training
  UINT8         CbsCmnMemMbistEnLpddr;                            ///< MBIST Enable
  UINT8         CbsCmnMemMbistTestmodeLpddr;                      ///< MBIST Test Mode
  UINT8         CbsCmnMemMbistAggressorsLpddr;                    ///< MBIST Aggressors
  UINT8         CbsCmnMemMbistPatternSelectLpddr;                 ///< Pattern Select
  UINT8         CbsCmnMemMbistPatternLengthLpddr;                 ///< Pattern Length
  UINT8         CbsCmnMemMbistAggressorsChnlLpddr;                ///< Aggressor Channel
  UINT8         CbsCmnMemMbistReadDataEyeVoltageStepLpddr;        ///< Read Voltage Sweep Step Size
  UINT8         CbsCmnMemMbistReadDataEyeTimingStepLpddr;         ///< Read Timing Sweep Step Size
  UINT8         CbsCmnMemMbistWriteDataEyeVoltageStepLpddr;       ///< Write Voltage Sweep Step Size
  UINT8         CbsCmnMemMbistWriteDataEyeTimingStepLpddr;        ///< Write Timing Sweep Step Size
  UINT8         CbsCmnGnbNbIOMMU;                                 ///< IOMMU
  UINT8         CbsCmnDmarSupport;                                ///< DMAr Support
  UINT8         CbsCmnDmaProtection;                              ///< DMA Protection
  UINT8         CbsCmnScpcAttrCntl;                               ///< SCPC attribute control
  UINT8         CbsCmnGnbPcieAriSupport;                          ///< PCIe ARI Support
  UINT8         CbsGnbCmnPcieAriEnumeration;                      ///< PCIe ARI Enumeration
  UINT8         CbsDbgGnbDbgAERCAPEnable;                         ///< Enable AER Cap
  UINT8         CbsDbgGnbDbgACSEnable;                            ///< ACS Enable
  UINT8         CbsCmnGnbPciePsppPolicy;                          ///< PSPP Policy
  UINT8         CbsCmnPciePortAllPorts;                           ///< PCIe All Ports Settings
  UINT8         CbsCmnRASControl;                                 ///< NBIO RAS Control
  UINT32        PcdEgressPoisonSeverityHi;                        ///< Egress Poison Severity High
  UINT32        PcdEgressPoisonSeverityLo;                        ///< Egress Poison Severity Low
  UINT8         CbsCmnNBIOSyncFloodGen;                           ///< NBIO SyncFlood Generation
  UINT8         PcdSyncFloodToApml;                               ///< NBIO SyncFlood Reporting
  UINT32        PcdAmdNbioEgressPoisonMaskHi;                     ///< Egress Poison Mask High
  UINT32        PcdAmdNbioEgressPoisonMaskLo;                     ///< Egress Poison Mask Low
  UINT32        PcdAmdNbioRASUcpMaskHi;                           ///< Uncorrected Converted to Poison Enable Mask High
  UINT32        PcdAmdNbioRASUcpMaskLo;                           ///< Uncorrected Converted to Poison Enable Mask Low
  UINT32        PcdSyshubWdtTimerInterval;                        ///< System Hub Watchdog Timer
  UINT8         CbsDbgPoisonConsumption;                          ///< NBIO Poison Consumption
  UINT8         CbsCmnGnbGfxUmaMode;                              ///< iGPU Configuration
  UINT8         CbsCmnGnbGfxUmaVersion;                           ///< UMA Version
  UINT32        CbsCmnGnbGfxUmaFrameBufferSize;                   ///< UMA Frame buffer Size
  UINT8         CbsCmnGpuHostTranslation;                         ///< GPU Host Translation Cache
  UINT8         CbsCmnGnbGfxNbAzalia;                             ///< NB Azalia
  UINT8         CbsDbgGnbAudioIOs;                                ///< Audio IOs
  UINT8         CbsCmnPcieAllPortsCAPASPM;                        ///< ASPM
  UINT8         CbsCmnPcieAllPortsCAPPML1SS;                      ///< L1 PM SubState
  UINT8         CbsCmnPcieAllPortsCAPLTR;                         ///< LTR Support
  UINT8         CbsCmnPcieAllPortsPMLinkASPM;                     ///< ASPM
  UINT8         CbsCmnPcieAllPortsPML1SS;                         ///< L1 PM SubState
  UINT8         CbsCmnPcieCoreLclkGating;                         ///< LCLK Gating
  UINT8         CbsCmnPcieCoreDynamicPowerGating;                 ///< PCIe Dynamic PG
  UINT8         CbsCmnNbioGlobalCGOverride;                       ///< NBIO Global CG Override
  UINT8         CbsCmnGnbPmmIohcLclkGating;                       ///< IOHC LCLK Clock Gating
  UINT8         CbsCmnGnbNbIOHCPowerGating;                       ///< IOHC Power Gating
  UINT8         CbsCmnGnbNbIOMMUL1ClockGating;                    ///< IOMMU L1 Clock Gating
  UINT8         CbsCmnGnbNbIOMMUL2ClockGating;                    ///< IOMMU L2 Clock Gating
  UINT8         CbsCmnGnbNbIOMMUDynamicPG;                        ///< IOMMU Dynamic Power Gating
  UINT8         CbsCmnGnbNbIOMMUL2MemoryPG;                       ///< IOMMU L2 Memory Power Gating
  UINT8         CbsCmnNbioNbifPg;                                 ///< NBIF PG
  UINT8         CbsCmnNbioShubPg;                                 ///< SHUB PG
  UINT8         CbsCmnPmmMmhubSramLS;                             ///< MMHUB SRAM Light Sleep
  UINT8         CbsCmnPmmNbifIntrOBFF;                            ///< NBIF INTR OBFF
  UINT8         CbsCmnPmmNbifDmaOBFF;                             ///< NBIF DMA OBFF
  UINT8         CbsCmnFchI3C0Config;                              ///< I3C/I2C 0 Enable
  UINT8         CbsCmnFchI3C0Mode;                                ///< I3C 0 Mode
  UINT8         CbsCmnFchI3C1Config;                              ///< I3C/I2C 1 Enable
  UINT8         CbsCmnFchI3C1Mode;                                ///< I3C 1 Mode
  UINT8         CbsCmnFchI3C2Config;                              ///< I3C/I2C 2 Enable
  UINT8         CbsCmnFchI3C2Mode;                                ///< I3C 2 Mode
  UINT8         CbsCmnFchI3C3Config;                              ///< I3C/I2C 3 Enable
  UINT8         CbsCmnFchI3C3Mode;                                ///< I3C 3 Mode
  UINT8         CbsCmnFchSataEnable;                              ///< SATA Controller
  UINT8         CbsCmnFchSataAutoShutDown;                        ///< SATA Auto Shutdown
  UINT8         CbsCmnFchSataClass;                               ///< SATA Mode
  UINT8         CbsCmnFchSataRasSupport;                          ///< Sata RAS Support
  UINT8         CbsCmnFchSataAhciDisPrefetchFunction;             ///< Sata Disabled AHCI Prefetch Function
  UINT8         CbsDbgFchSataAggresiveDevSlpP0;                   ///< Aggresive SATA Device Sleep Port 0
  UINT8         CbsDbgFchSataDevSlpPort0Num;                      ///< DevSleep0 Port Number
  UINT8         CbsDbgFchSataAggresiveDevSlpP1;                   ///< Aggresive SATA Device Sleep Port 1
  UINT8         CbsDbgFchSataDevSlpPort1Num;                      ///< DevSleep1 Port Number
  UINT8         CbsCmnFchUsbXHCI0Enable;                          ///< USB0 controller enable
  UINT8         CbsCmnFchUsbXHCI1Enable;                          ///< USB1 controller enable
  UINT8         CbsCmnFchUsbXHCI2Enable;                          ///< USB2 controller enable
  UINT8         CbsCmnFchUsbXHCI0HsPort0;                         ///< USB0 2.0 Port 0
  UINT8         CbsCmnFchUsbXHCI0HsPort1;                         ///< USB0 2.0 Port 1
  UINT8         CbsCmnFchUsbXHCI0HsPort2;                         ///< USB0 2.0 Port 2
  UINT8         CbsCmnFchUsbXHCI0HsPort3;                         ///< USB0 2.0 Port 3
  UINT8         CbsCmnFchUsbXHCI1HsPort0;                         ///< USB1 2.0 Port 0
  UINT8         CbsCmnFchUsbXHCI1HsPort1;                         ///< USB1 2.0 Port 1
  UINT8         CbsCmnFchUsbXHCI1HsPort2;                         ///< USB1 2.0 Port 2
  UINT8         CbsCmnFchUsbXHCI2HsPort0;                         ///< USB2 2.0 Port 0
  UINT8         CbsCmnFchUsbXHCI3HsPort0;                         ///< USB3 2.0 port 0
  UINT8         CbsCmnFchUsbXHCI4HsPort0;                         ///< USB4 2.0 port0
  UINT8         CbsCmnFchUsbXHCI0SsPort0;                         ///< USB0 3.1 Port 0
  UINT8         CbsCmnFchUsbXHCI0SsPort1;                         ///< USB0 3.1 Port 1
  UINT8         CbsCmnFchUsbXHCI1SsPort0;                         ///< USB1 3.1 Port 0
  UINT8         CbsCmnFchUsbXHCI1SsPort1;                         ///< USB1 3.1 Port 1
  UINT8         CbsCmnFchUsbXHCI3SsPort0;                         ///< USB3 3.1 Port 0
  UINT8         CbsCmnFchUsbXHCI4SsPort0;                         ///< USB4 3.1 Port 0
  UINT8         CbsCmnFchSystemPwrFailShadow;                     ///< Ac Loss Control
  UINT8         CbsCmnFchUart0Config;                             ///< Uart 0 Enable
  UINT8         CbsCmnFchUart0LegacyConfig;                       ///< Uart 0 Legacy Options
  UINT8         CbsCmnFchUart1Config;                             ///< Uart 1 Enable (no HW FC)
  UINT8         CbsCmnFchUart1LegacyConfig;                       ///< Uart 1 Legacy Options
  UINT8         CbsCmnFchUart2Config;                             ///< Uart 2 Enable
  UINT8         CbsCmnFchUart2LegacyConfig;                       ///< Uart 2 Legacy Options
  UINT8         CbsCmnFchUart3Config;                             ///< Uart 3 Enable (no HW FC)
  UINT8         CbsCmnFchUart3LegacyConfig;                       ///< Uart 3 Legacy Options
  UINT8         CbsCmnFchUart4Config;                             ///< Uart 4 Enable
  UINT8         CbsCmnFchEspiConfig;                              ///< ESPI Enable
  UINT8         CbsCmnFchEspiCrc;                                 ///< ESPI CRC Enable
  UINT8         CbsCmnFchXgbe0Enable;                             ///< XGBE0 enable
  UINT8         CbsCmnFchXgbe1Enable;                             ///< XGBE1 enable
  UINT8         CbsCmnFchXgbePort0Led1Gpio;                       ///< XGBE Port0 LED1 GPIO
  UINT8         CbsCmnFchXgbePort0Led2Gpio;                       ///< XGBE Port0 LED2 GPIO
  UINT8         CbsCmnFchXgbePort1Led1Gpio;                       ///< XGBE Port1 LED1 GPIO
  UINT8         CbsCmnFchXgbePort1Led2Gpio;                       ///< XGBE Port1 LED2 GPIO
  UINT8         CbsCmnFchCioPreCmEnable;                          ///< USB4 pre-CM Enable
  UINT8         CbsCmnFchCioRt0Enable;                            ///< RT0 Router Enable
  UINT8         CbsCmnFchCioRt0PcieTunnel;                        ///< RT0 PCIe Tunnel
  UINT8         CbsCmnFchCioRt1Enable;                            ///< RT1 Router Enable
  UINT8         CbsCmnFchCioRt1PcieTunnel;                        ///< RT1 PCIe Tunnel
  UINT8         CbsCmnFchHfpConfig;                               ///< HFP Enable
  UINT8         CbsCmnFchHidConfig;                               ///< HID Enable
  UINT8         CbsCmnFchHidSpiReadMode;                          ///< HID Spi Read Mode
  UINT8         CbsCmnFchHidSpeed;                                ///< HID Spi Speed
  UINT8         CbsCmnFchHidSpiSpeed6N;                           ///< HID Spi Speed6 N
  UINT8         CbsCmnFchHidSpiSpeed7N;                           ///< HID Spi Speed7 N
  UINT8         CbsCmnFchHid2Config;                              ///< HID2 Enable
  UINT8         CbsCmnFchHid2SpiReadMode;                         ///< HID2 Spi Read Mode
  UINT8         CbsCmnFchHid2Speed;                               ///< HID2 Spi Speed
  UINT8         CbsCmnFchHid2SpiSpeed6N;                          ///< HID2 Spi Speed6 N
  UINT8         CbsCmnFchHid2SpiSpeed7N;                          ///< HID2 Spi Speed7 N
  UINT8         CbsCmnFchSyncfloodEnable;                         ///< Reset after sync flood
  UINT8         CbsCmnGnbSMUSystemConfig;                         ///< System Configuration
  UINT32        CbsDbgSustainedPowerLimit;                        ///< Sustained Power Limit
  UINT8         CbsDbgPPTCtl;                                     ///< PPT Control
  UINT32        CbsDbgFastPPTLimit;                               ///< Fast PPT Limit
  UINT32        CbsDbgSlowPPTLimit;                               ///< Slow PPT Limit
  UINT32        CbsDbgSlowPPTTimeConstant;                        ///< Slow PPT Time Constant
  UINT8         CbsDbgSTAPMCtl;                                   ///< STAPM Control
  UINT8         CbsDbgSystemTemperatureTracking;                  ///< System Temperature Tracking
  UINT8         CbsDbgStapmBoostOverride;                         ///< STAPM Boost Override
  UINT8         CbsDbgStapmBoost;                                 ///< STAPM Boost
  UINT32        CbsDbgTskinTimeConstant;                          ///< Tskin Time Constant
  UINT8         CbsDbgTHMCtl;                                     ///< Thermal Control
  UINT32        CbsDbgTjMaxLimit;                                 ///< TjMax
  UINT8         CbsDbgTDCCtl;                                     ///< TDC Control
  UINT32        CbsDbgTDCVDDLimit;                                ///< TDC_VDDCR_VDD
  UINT32        CbsDbgTDCSOCLimit;                                ///< TDC_VDDCR_SOC
  UINT32        CbsDbgTDCGFXLimit;                                ///< TDC_VDDCR_GFX
  UINT32        CbsDbgTDCCVIPLimit;                               ///< TDC_VDDCR_CVIP
  UINT8         CbsDbgEDCCtl;                                     ///< EDC Control
  UINT32        CbsDbgEDCVDDLimit;                                ///< EDC_VDDCR_VDD
  UINT32        CbsDbgEDCSOCLimit;                                ///< EDC_VDDCR_SOC
  UINT32        CbsDbgEDCGFXLimit;                                ///< EDC_VDDCR_GFX
  UINT8         CbsDbgPSI3Ctl;                                    ///< PSI3 Control
  UINT32        CbsDbgPSI3VDDLimit;                               ///< PSI3_VDDCR_VDD
  UINT32        CbsDbgPSI3GFXLimit;                               ///< PSI3_VDDCR_GFX
  UINT8         CbsDbgProchotCtl;                                 ///< PROCHOT Control
  UINT32        CbsDbgProchotRampTime;                            ///< PROCHOT Deassertion Ramp Time
  UINT8         CbsDbgSTTCtl;                                     ///< STT Control
  UINT8         CbsCmnSTTPcbSensorCount;                          ///< STT_PCB_SENSOR_COUNT
  UINT16        CbsCmnSttMinLimit;                                ///< STT_MIN_POWER_LIMIT
  UINT16        CbsCmnSTTM1;                                      ///< STT_M1
  UINT16        CbsCmnSTTM2;                                      ///< STT_M2
  UINT16        CbsCmnSTTM3;                                      ///< STT_M3
  UINT16        CbsCmnSTTM4;                                      ///< STT_M4
  UINT16        CbsCmnSTTM5;                                      ///< STT_M5
  UINT16        CbsCmnSTTM6;                                      ///< STT_M6
  UINT16        CbsCmnSTTCAPU;                                    ///< STT_C_APU
  UINT16        CbsCmnSTTCHs2;                                    ///< STT_C_HS2
  UINT16        CbsCmnSTTAlphaAPU;                                ///< STT_ALPHA_APU
  UINT16        CbsCmnSTTAlphaHS2;                                ///< STT_ALPHA_HS2
  UINT16        CbsCmnSTTSkinTemperatureLimitApu;                 ///< STT_SKIN_TEMPERATURE_LIMIT_APU
  UINT16        CbsCmnSTTSkinTemperatureLimitHs2;                 ///< STT_SKIN_TEMPERATURE_LIMIT_HS2
  UINT16        CbsCmnSTTErrorCoeff;                              ///< STT_ERROR_COEFF
  UINT16        CbsCmnSTTErrorRateCoeff;                          ///< STT_ERROR_RATE_COEFF
  UINT8         CbsDbgFanCtl;                                     ///< Fan Control
  UINT8         CbsDbgForcePwmCtl;                                ///< Force PWM Control
  UINT8         CbsDbgForcePwm;                                   ///< Force PWM
  UINT8         CbsDbgFanTableCtl;                                ///< Fan Table Control
  UINT32        CbsDbgFanTableTempLow;                            ///< Low Temperature
  UINT32        CbsDbgFanTableTempMed;                            ///< Medium Temperature
  UINT32        CbsDbgFanTableTempHigh;                           ///< High Temperature
  UINT32        CbsDbgFanTableTempCritical;                       ///< Critical Temperature
  UINT32        CbsDbgFanTableTPwmLow;                            ///< Low Pwm
  UINT32        CbsDbgFanTablePwmMed;                             ///< Medium Pwm
  UINT32        CbsDbgFanTablePwmHigh;                            ///< High Pwm
  UINT32        CbsDbgFanTableHyst;                               ///< Temperature Hysteresis
  UINT8         CbsDbgFanTablePwmFreq;                            ///< Pwm Frequency
  UINT8         CbsDbgFanPolarity;                                ///< Fan polarity
  UINT8         CbsDbgVddpVoltageCtl;                             ///< VDDP Voltage Control
  UINT16        CbsDbgVddpVoltage;                                ///< VDDP Voltage
  UINT8         CbsDbgVddmVoltageCtl;                             ///< VDDM Voltage Control
  UINT32        CbsDbgVddmVoltage;                                ///< VDDM Voltage
  UINT8         CbsCmnGnbSmuZstates;                              ///< Zstates (Z9 and Z10)
  UINT8         CbsCmnCclkFmaxCtl;                                ///< CclkFmaxOverride Control
  UINT32        CbsCmnCclkFmaxOverride;                           ///< CclkFmaxOverride
  UINT8         CbsCmnDfPstateP0Ctl;                              ///< DF Pstate P0 Ctl
  UINT16        CbsCmnDfPstateFclkP0;                             ///< DF Pstate Fclk P0
  UINT16        CbsCmnDfPstateMemClkP0;                           ///< DF Pstate MemClk P0
  UINT8         CbsCmnFclkValue;                                  ///< Fixed FCLK
  UINT8         CbsCmnLclkFreq;                                   ///< Fixed LCLK
  UINT8         CbsCmnRstOnMp1SMNTimeout;                         ///< Reset on MP1 SMN Timeout
  UINT8         CbsCmnAACtl;                                      ///< SmartShift Control
  UINT8         CbsCmnAAEn;                                       ///< SmartShift Enable
  UINT32        CbsCmnApuOnlySpptLimit;                           ///< APU Only sPPT Limit
  UINT32        CbsCmnMsgSetSustainedPowerLimit;                  ///< Sustained PowerLimit
  UINT32        CbsCmnMsgSetFastPPTLimit;                         ///< Fast PPT Limit
  UINT32        CbsCmnMsgSeSlowPPTLimit;                          ///< Slow PPT Limit
  UINT8         CbsCmnPspRpmcSwitch;                              ///< PSP RPMC Switch
  UINT8         CbsCmnRomArmorSupport;                            ///< Rom Armor support
  UINT8         CbsCmnRomArmorSpiConfigRegLock;                   ///< Rom Armor Spi Config Reg Lock
  UINT8         CbsCmnSocAblConOut;                               ///< ABL Console Out Control
  UINT8         CbsCmnSocAblConOutSerialPort;                     ///< ABL Console Out Serial Port
  UINT8         CbsCmnSocAblConOutSerialPortIO;                   ///< ABL Console Out Serial Port IO
  UINT8         CbsCmnSocAblConOutBootMode;                       ///< ABL Console Out Boot Mode Select
  UINT8         CbsCmnSocAblPmuMsgCtrl;                           ///< ABL PMU message Control
  UINT8         CbsCmnSocScpcAutoEnablement;                      ///< Secured-core Auto enablement
  UINT8         CbsCmnMixedDimmConfigNumaDomain;                  ///< Mixed DIMM config extended NUMA domain
  UINT8         CbsCmnSocFarEnforced;                             ///< FAR enforcement state
  UINT32        CbsCmnSocSplFuse;                                 ///< SPL value in the CPU fuse
  UINT32        CbsCmnSocSplValueInTbl;                           ///< SPL value in the SPL table
  UINT32        CbsCmnSocInitSplForFar;                           ///< Initial SPL value for FAR
  UINT8         CbsCmnSocFarSwitch;                               ///< FAR Switch
  UINT8         CbsCmnSoCMiscX86FwSupport;                        ///< Pluton (HSP) X86 Firmware Support
  UINT8         CbsCmnSoCMiscHspUart;                             ///< Pluton (HSP) UART
  UINT8         CbsCmnSoCMiscHspUartPort;                         ///< Pluton (HSP) UART Port
  UINT8         CbsCmnSoCMiscHspFips;                             ///< Pluton (HSP) FIPS
  UINT8         CbsCmnSoCMiscHspFipsZeroization;                  ///< Pluton (HSP) FIPS zeroization
  UINT8         CbsCmnSoCMiscHspAcpiFenceEnable;                  ///< Pluton (HSP) ACPI Fence Enable
  UINT8         CbsCmnSoCMiscMpmSupport;                          ///< AGESA MPM support
  UINT8         CbsCmnSoCMiscWiredKvm;                            ///< KVM for Wired Manageability
  UINT8         CbsCmnSoCMiscWirelessManageability;               ///< Wireless Manageability
  UINT8         CbsCmnSoCMiscWirelessKvm;                         ///< KVM for Wireless Manageability
  UINT8         CbsCmnSocMiscMpmAttrCntl;                         ///< MPM attribute control
  UINT8         CbsCmnSoCMiscWirelessTcr;                         ///< TCR for Wireless Manageability
  UINT32        CbsCmnIntrusionDetectionCtl;                      ///< Intrusion Detection Control
  UINT8         CbsCmnIntrusionDetectionEnable;                   ///< Intrusion Detection Enable
  UINT8         CbsCmnIntrusionLogEvent;                          ///< Log Intrusion Event
  UINT8         CbsCmnIntrusionClearTpm;                          ///< Clear TPM
  UINT8         CbsCmnIntrusionPowerOff;                          ///< Power Off System
  UINT8         CbsCmnPT21PcieP0;                                 ///< PCIe Port 0
  UINT8         CbsCmnPT21PcieP1;                                 ///< PCIe Port 1
  UINT8         CbsCmnPT21PcieP2;                                 ///< PCIe Port 2
  UINT8         CbsCmnPT21PcieP3;                                 ///< PCIe Port 3
  UINT8         CbsCmnPT21PcieP4;                                 ///< PCIe Port 4
  UINT8         CbsCmnPT21PcieP5;                                 ///< PCIe Port 5
  UINT8         CbsCmnPT21PcieP6;                                 ///< PCIe Port 6
  UINT8         CbsCmnPT21PcieP7;                                 ///< PCIe Port 7
  UINT8         CbsCmnPT21PcieP8;                                 ///< PCIe Port 8
  UINT8         CbsCmnPT21PcieP9;                                 ///< PCIe Port 9
  UINT8         CbsCmnPT21PcieP10;                                ///< PCIe Port 10
  UINT8         CbsCmnPT21PcieP11;                                ///< PCIe Port 11
  UINT8         CbsCmnPT21PcieP12;                                ///< PCIe Port 12
  UINT8         CbsCmnPT21PcieP13;                                ///< PCIe Port 13
  UINT8         CbsCmnPT21SataClass;                              ///< SATA Mode
  UINT8         CbsCmnPT21SataPort0;                              ///< SATA Port 0
  UINT8         CbsCmnPT21SataPort1;                              ///< SATA Port 1
  UINT8         CbsCmnPT21SataPort2;                              ///< SATA Port 2
  UINT8         CbsCmnPT21SataPort3;                              ///< SATA Port 3
  UINT8         CbsDbgPT21SataAggressiveDevSlpP0;                 ///< Device Sleep for AHCI Port 0
  UINT8         CbsDbgPT21SataAggressiveDevSlpP1;                 ///< Device Sleep for AHCI Port 1
  UINT8         CbsDbgPT21SataAggressiveDevSlpP2;                 ///< Device Sleep for AHCI Port 2
  UINT8         CbsDbgPT21SataAggressiveDevSlpP3;                 ///< Device Sleep for AHCI Port 3
  UINT8         CbsDbgPT21XhciP0Gen;                              ///< USB3 Port 0
  UINT8         CbsDbgPT21XhciP1Gen;                              ///< USB3 Port 1
  UINT8         CbsDbgPT21XhciP2Gen;                              ///< USB3 Port 2
  UINT8         CbsDbgPT21XhciP3Gen;                              ///< USB3 Port 3
  UINT8         CbsDbgPT21XhciP4Gen;                              ///< USB3 Port 4
  UINT8         CbsDbgPT21XhciP5Gen;                              ///< USB3 Port 5
  UINT8         CbsCmnPT21Usb3P0;                                 ///< USB3 Port 0
  UINT8         CbsCmnPT21Usb3P1;                                 ///< USB3 Port 1
  UINT8         CbsCmnPT21Usb3P2;                                 ///< USB3 Port 2
  UINT8         CbsCmnPT21Usb3P3;                                 ///< USB3 Port 3
  UINT8         CbsCmnPT21Usb3P4;                                 ///< USB3 Port 4
  UINT8         CbsCmnPT21Usb3P5;                                 ///< USB3 Port 5
  UINT8         CbsCmnPT21Usb2P0;                                 ///< USB2 Port 0
  UINT8         CbsCmnPT21Usb2P1;                                 ///< USB2 Port 1
  UINT8         CbsCmnPT21Usb2P2;                                 ///< USB2 Port 2
  UINT8         CbsCmnPT21Usb2P3;                                 ///< USB2 Port 3
  UINT8         CbsCmnPT21Usb2P4;                                 ///< USB2 Port 4
  UINT8         CbsCmnPT21Usb2P5;                                 ///< USB2 Port 5
  UINT8         CbsCmnPT21Usb2P6;                                 ///< USB2 Port 6
  UINT8         CbsCmnPT21Usb2P7;                                 ///< USB2 Port 7
  UINT8         CbsCmnPT21Usb2P8;                                 ///< USB2 Port 8
  UINT8         CbsCmnPT21Usb2P9;                                 ///< USB2 Port 9
  UINT8         CbsCmnPT21Usb2P10;                                ///< USB2 Port 10
  UINT8         CbsCmnPT21Usb2P11;                                ///< USB2 Port 11
  UINT8         CbsCmnPT21L4Usb3P0;                               ///< USB3 Port 0
  UINT8         CbsCmnPT21L4Usb3P1;                               ///< USB3 Port 1
  UINT8         CbsCmnPT21L4Usb3P2;                               ///< USB3 Port 2
  UINT8         CbsCmnPT21L4Usb3P3;                               ///< USB3 Port 3
  UINT8         CbsCmnPT21L4Usb2P0;                               ///< USB2 Port 0
  UINT8         CbsCmnPT21L4Usb2P1;                               ///< USB2 Port 1
  UINT8         CbsCmnPT21L4Usb2P2;                               ///< USB2 Port 2
  UINT8         CbsCmnPT21L4Usb2P3;                               ///< USB2 Port 3
  UINT8         CbsCmnPT21L4Usb2P4;                               ///< USB2 Port 4
  UINT8         CbsCmnPT21L4Usb2P5;                               ///< USB2 Port 5
  UINT8         CbsCmnPT21L4Usb2P6;                               ///< USB2 Port 6
  UINT8         CbsCmnPT21L4Usb2P7;                               ///< USB2 Port 7
  UINT8         CbsCmnPT21L4Usb2P8;                               ///< USB2 Port 8
  UINT8         CbsCmnPT21L4Usb2P9;                               ///< USB2 Port 9
  UINT8         CbsCmnSecPT21PcieP0;                              ///< PCIe Port 0
  UINT8         CbsCmnSecPT21PcieP1;                              ///< PCIe Port 1
  UINT8         CbsCmnSecPT21PcieP2;                              ///< PCIe Port 2
  UINT8         CbsCmnSecPT21PcieP3;                              ///< PCIe Port 3
  UINT8         CbsCmnSecPT21PcieP4;                              ///< PCIe Port 4
  UINT8         CbsCmnSecPT21PcieP5;                              ///< PCIe Port 5
  UINT8         CbsCmnSecPT21PcieP6;                              ///< PCIe Port 6
  UINT8         CbsCmnSecPT21PcieP7;                              ///< PCIe Port 7
  UINT8         CbsCmnSecPT21PcieP8;                              ///< PCIe Port 8
  UINT8         CbsCmnSecPT21PcieP9;                              ///< PCIe Port 9
  UINT8         CbsCmnSecPT21PcieP10;                             ///< PCIe Port 10
  UINT8         CbsCmnSecPT21PcieP11;                             ///< PCIe Port 11
  UINT8         CbsCmnSecPT21PcieP12;                             ///< PCIe Port 12
  UINT8         CbsCmnSecPT21PcieP13;                             ///< PCIe Port 13
  UINT8         CbsCmnSecPT21SataClass;                           ///< SATA Mode
  UINT8         CbsCmnSecPT21SataPort0;                           ///< SATA Port 0
  UINT8         CbsCmnSecPT21SataPort1;                           ///< SATA Port 1
  UINT8         CbsCmnSecPT21SataPort2;                           ///< SATA Port 2
  UINT8         CbsCmnSecPT21SataPort3;                           ///< SATA Port 3
  UINT8         CbsDbgSecPT21SataAggresiveDevSlpP0;               ///< Device Sleep for AHCI Port 0
  UINT8         CbsDbgSecPT21SataAggresiveDevSlpP1;               ///< Device Sleep for AHCI Port 1
  UINT8         CbsDbgSecPT21SataAggresiveDevSlpP2;               ///< Device Sleep for AHCI Port 2
  UINT8         CbsDbgSecPT21SataAggresiveDevSlpP3;               ///< Device Sleep for AHCI Port 3
  UINT8         CbsDbgSecPT21XhciP0Gen;                           ///< USB3 Port 0
  UINT8         CbsDbgSecPT21XhciP1Gen;                           ///< USB3 Port 1
  UINT8         CbsDbgSecPT21XhciP2Gen;                           ///< USB3 Port 2
  UINT8         CbsDbgSecPT21XhciP3Gen;                           ///< USB3 Port 3
  UINT8         CbsDbgSecPT21XhciP4Gen;                           ///< USB3 Port 4
  UINT8         CbsDbgSecPT21XhciP5Gen;                           ///< USB3 Port 5
  UINT8         CbsCmnSecPT21Usb3P0;                              ///< USB3 Port 0
  UINT8         CbsCmnSecPT21Usb3P1;                              ///< USB3 Port 1
  UINT8         CbsCmnSecPT21Usb3P2;                              ///< USB3 Port 2
  UINT8         CbsCmnSecPT21Usb3P3;                              ///< USB3 Port 3
  UINT8         CbsCmnSecPT21Usb3P4;                              ///< USB3 Port 4
  UINT8         CbsCmnSecPT21Usb3P5;                              ///< USB3 Port 5
  UINT8         CbsCmnSecPT21Usb2P0;                              ///< USB2 Port 0
  UINT8         CbsCmnSecPT21Usb2P1;                              ///< USB2 Port 1
  UINT8         CbsCmnSecPT21Usb2P2;                              ///< USB2 Port 2
  UINT8         CbsCmnSecPT21Usb2P3;                              ///< USB2 Port 3
  UINT8         CbsCmnSecPT21Usb2P4;                              ///< USB2 Port 4
  UINT8         CbsCmnSecPT21Usb2P5;                              ///< USB2 Port 5
  UINT8         CbsCmnSecPT21Usb2P6;                              ///< USB2 Port 6
  UINT8         CbsCmnSecPT21Usb2P7;                              ///< USB2 Port 7
  UINT8         CbsCmnSecPT21Usb2P8;                              ///< USB2 Port 8
  UINT8         CbsCmnSecPT21Usb2P9;                              ///< USB2 Port 9
  UINT8         CbsCmnSecPT21Usb2P10;                             ///< USB2 Port 10
  UINT8         CbsCmnSecPT21Usb2P11;                             ///< USB2 Port 11
  UINT8         CbsCmnSecPT21L4Usb3P0;                            ///< USB3 Port 0
  UINT8         CbsCmnSecPT21L4Usb3P1;                            ///< USB3 Port 1
  UINT8         CbsCmnSecPT21L4Usb3P2;                            ///< USB3 Port 2
  UINT8         CbsCmnSecPT21L4Usb3P3;                            ///< USB3 Port 3
  UINT8         CbsCmnSecPT21L4Usb2P0;                            ///< USB2 Port 0
  UINT8         CbsCmnSecPT21L4Usb2P1;                            ///< USB2 Port 1
  UINT8         CbsCmnSecPT21L4Usb2P2;                            ///< USB2 Port 2
  UINT8         CbsCmnSecPT21L4Usb2P3;                            ///< USB2 Port 3
  UINT8         CbsCmnSecPT21L4Usb2P4;                            ///< USB2 Port 4
  UINT8         CbsCmnSecPT21L4Usb2P5;                            ///< USB2 Port 5
  UINT8         CbsCmnSecPT21L4Usb2P6;                            ///< USB2 Port 6
  UINT8         CbsCmnSecPT21L4Usb2P7;                            ///< USB2 Port 7
  UINT8         CbsCmnSecPT21L4Usb2P8;                            ///< USB2 Port 8
  UINT8         CbsCmnSecPT21L4Usb2P9;                            ///< USB2 Port 9

  UINT8         Reserved[1024];                                   ///< Reserved for option growth
} CBS_CONFIG;


///CbsVariableStructUniqueValue 0xd24cf9ba
///ApcbVariableHash 0xd5eab3d5 

#pragma pack(pop)


#endif // _AMD_CBS_VARIABLE_H_
