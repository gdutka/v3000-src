/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AGESA_POST_CODE_H_
#define _AGESA_POST_CODE_H_

/// <PostCodePrefix> 0xB0000000

/**
 *  AGESA Post Codes
 *
 *  These are the values displayed to the user to indicate progress through boot.
 *
 */
typedef enum {
  // [0xA0XX] assigned for AGESA Common & Universal
  // Universal Post Code
  TpUniversalAcpiEntry                       = 0xA001, ///< Universal ACPI entry
  TpUniversalAcpiExit                        = 0xA002, ///< Universal ACPI exit
  TpUniversalAcpiAbort                       = 0xA003, ///< Universal ACPI abort
  TpUniversalSmbiosEntry                     = 0xA004, ///< Universal SMBIOS entry
  TpUniversalSmbiosExit                      = 0xA005, ///< Universal SMBIOS exit
  TpUniversalSmbiosAbort                     = 0xA006, ///< Universal SMBIOS abort

  // [0xA1XX] For CZ only memory Postcodes
  TpProcMemBeforeMemDataInit                 = 0xA101, ///< Memory structure initialization (Public interface)
  TpProcMemBeforeSpdProcessing               = 0xA102, ///< SPD Data processing  (Public interface)
  TpProcMemAmdMemAuto                        = 0xA103, ///< Memory configuration  (Public interface)
  TpProcMemDramInit                          = 0xA104, ///< DRAM initialization
  TpProcMemSPDChecking                       = 0xA105, ///< TpProcMemSPDChecking
  TpProcMemModeChecking                      = 0xA106, ///< TpProcMemModeChecking
  TpProcMemSpeedTclConfig                    = 0xA107, ///< Speed and TCL configuration
  TpProcMemSpdTiming                         = 0xA108, ///< TpProcMemSpdTiming
  TpProcMemDramMapping                       = 0xA109, ///< TpProcMemDramMapping
  TpProcMemPlatformSpecificConfig            = 0xA10A, ///< TpProcMemPlatformSpecificConfig
  TPProcMemPhyCompensation                   = 0xA10B, ///< TPProcMemPhyCompensation
  TpProcMemStartDcts                         = 0xA10C, ///< TpProcMemStartDcts
  TpProcMemBeforeDramInit                    = 0xA10D, ///< (Public interface)
  TpProcMemPhyFenceTraining                  = 0xA10E, ///< TpProcMemPhyFenceTraining
  TpProcMemSynchronizeDcts                   = 0xA10F, ///< TpProcMemSynchronizeDcts
  TpProcMemSystemMemoryMapping               = 0xA110, ///< TpProcMemSystemMemoryMapping
  TpProcMemMtrrConfiguration                 = 0xA111, ///< TpProcMemMtrrConfiguration
  TpProcMemDramTraining                      = 0xA112, ///< TpProcMemDramTraining
  TpProcMemBeforeAnyTraining                 = 0xA113, ///< (Public interface)
  TpProcMemWriteLevelizationTraining         = 0xA114, ///< TpProcMemWriteLevelizationTraining
  TpProcMemWlFirstPass                       = 0xA115, ///< Below 800Mhz first pass start
  TpProcMemWlSecondPass                      = 0xA116, ///< Above 800Mhz second pass start
  TpProcMemWlTrainTargetDimm                 = 0xA117, ///< Target DIMM configured
  TpProcMemWlPrepDimms                       = 0xA118, ///< Prepare DIMMS for WL
  TpProcMemWlConfigDimms                     = 0xA119, ///< Configure DIMMS for WL
  TpProcMemReceiverEnableTraining            = 0xA11A, ///< TpProcMemReceiverEnableTraining
  TpProcMemRcvrStartSweep                    = 0xA11B, ///< Start sweep loop
  TpProcMemRcvrSetDelay                      = 0xA11C, ///< Set receiver Delay
  TpProcMemRcvrWritePattern                  = 0xA11D, ///< Write test pattern
  TpProcMemRcvrReadPattern                   = 0xA11E, ///< Read test pattern
  TpProcMemRcvrTestPattern                   = 0xA11F, ///< Compare test pattern
  TpProcMemRcvrCalcLatency                   = 0xA120, ///< Calculate MaxRdLatency per channel
  TpProcMemReceiveDqsTraining                = 0xA121, ///< TpProcMemReceiveDqsTraining
  TpProcMemRcvDqsSetDelay                    = 0xA122, ///< Set Write Data delay
  TpProcMemRcvDqsWritePattern                = 0xA123, ///< Write test pattern
  TpProcMemRcvDqsStartSweep                  = 0xA124, ///< Start read sweep
  TpProcMemRcvDqsSetRcvDelay                 = 0xA125, ///< Set Receive DQS delay
  TpProcMemRcvDqsReadPattern                 = 0xA126, ///< Read Test pattern
  TpProcMemRcvDqsTstPattern                  = 0xA127, ///< Compare Test pattern
  TpProcMemRcvDqsResults                     = 0xA128, ///< Update results
  TpProcMemRcvDqsFindWindow                  = 0xA129, ///< Start Find passing window
  TpProcMemTransmitDqsTraining               = 0xA12A, ///< TpProcMemTransmitDqsTraining
  TpProcMemTxDqStartSweep                    = 0xA12B, ///< Start write sweep
  TpProcMemTxDqSetDelay                      = 0xA12C, ///< Set Transmit DQ delay
  TpProcMemTxDqWritePattern                  = 0xA12D, ///< Write test pattern
  TpProcMemTxDqReadPattern                   = 0xA12E, ///< Read Test pattern
  TpProcMemTxDqTestPattern                   = 0xA12F, ///< Compare Test pattern
  TpProcMemTxDqResults                       = 0xA130, ///< Update results
  TpProcMemTxDqFindWindow                    = 0xA131, ///< Start Find passing window
  TpProcMemMaxRdLatencyTraining              = 0xA132, ///< TpProcMemMaxRdLatencyTraining
  TpProcMemMaxRdLatStartSweep                = 0xA133, ///< Start sweep
  TpProcMemMaxRdLatSetDelay                  = 0xA134, ///< Set delay
  TpProcMemMaxRdLatWritePattern              = 0xA135, ///< Write test pattern
  TpProcMemMaxRdLatReadPattern               = 0xA136, ///< Read Test pattern
  TpProcMemMaxRdLatTestPattern               = 0xA137, ///< Compare Test pattern
  TpProcMemOnlineSpareInit                   = 0xA138, ///< Online Spare init
  TpProcMemBankInterleaveInit                = 0xA139, ///< Bank Interleave Init
  TpProcMemNodeInterleaveInit                = 0xA13A, ///< Node Interleave Init
  TpProcMemChannelInterleaveInit             = 0xA13B, ///< Channel Interleave Init
  TpProcMemEccInitialization                 = 0xA13C, ///< ECC initialization
  TpProcMemPlatformSpecificInit              = 0xA13D, ///< Platform Specific Init
  TpProcMemBeforeAgesaReadSpd                = 0xA13E, ///< Before callout for "AgesaReadSpd"
  TpProcMemAfterAgesaReadSpd                 = 0xA13F, ///< After callout for "AgesaReadSpd"
  TpProcMemBeforeAgesaHookBeforeDramInit     = 0xA140, ///< Before optional callout "AgesaHookBeforeDramInit"
  TpProcMemAfterAgesaHookBeforeDramInit      = 0xA141, ///< After optional callout "AgesaHookBeforeDramInit"
  TpProcMemBeforeAgesaHookBeforeDQSTraining  = 0xA142, ///< Before optional callout "AgesaHookBeforeDQSTraining"
  TpProcMemAfterAgesaHookBeforeDQSTraining   = 0xA143, ///< After optional callout "AgesaHookBeforeDQSTraining"
  TpProcMemBeforeAgesaHookBeforeExitSelfRef  = 0xA144, ///< Before optional callout "AgesaHookBeforeDramInit"
  TpProcMemAfterAgesaHookBeforeExitSelfRef   = 0xA145, ///< After optional callout "AgesaHookBeforeDramInit"
  TpProcMemAfterMemDataInit                  = 0xA146, ///< After MemDataInit
  TpProcMemInitializeMCT                     = 0xA147, ///< Before InitializeMCT
  TpProcMemLvDdr3                            = 0xA148, ///< Before LV DDR3
  TpProcMemInitMCT                           = 0xA149, ///< Before InitMCT
  TpProcMemOtherTiming                       = 0xA14A, ///< Before OtherTiming
  TpProcMemUMAMemTyping                      = 0xA14B, ///< Before UMAMemTyping
  TpProcMemSetDqsEccTmgs                     = 0xA14C, ///< Before SetDqsEccTmgs
  TpProcMemMemClr                            = 0xA14D, ///< Before MemClr
  TpProcMemOnDimmThermal                     = 0xA14E, ///< Before On DIMM Thermal
  TpProcMemDmi                               = 0xA14F, ///< Before DMI
  TpProcMemEnd                               = 0xA150, ///< End of memory code
  TpProcS3Init                               = 0xA151, ///< Entry point S3Init
  TpProcMemSendMRS2                          = 0xA180, ///< Sending MRS2
  TpProcMemSendMRS3                          = 0xA181, ///< Sedding MRS3
  TpProcMemSendMRS1                          = 0xA182, ///< Sending MRS1
  TpProcMemSendMRS0                          = 0xA183, ///< Sending MRS0
  TpProcMemContinPatternGenRead              = 0xA184, ///< Continuous Pattern Read
  TpProcMemContinPatternGenWrite             = 0xA185, ///< Continuous Pattern Write
  TpProcMem2dRdDqsTraining                   = 0xA186, ///< Mem: 2d RdDqs Training begin
  TpProcMemBefore2dTrainExtVrefChange        = 0xA187, ///< Mem: Before optional callout to platform BIOS to change External Vref during 2d Training
  TpProcMemAfter2dTrainExtVrefChange         = 0xA188, ///< Mem: After optional callout to platform BIOS to change External Vref during 2d Training
  TpProcMemConfigureDCTForGeneral            = 0xA189, ///< Configure DCT For General use begin
  TpProcMemProcConfigureDCTForTraining       = 0xA18A, ///< Configure DCT For training begin
  TpProcMemConfigureDCTNonExplicitSeq        = 0xA18B, ///< Configure DCT For Non-Explicit
  TpProcMemSynchronizeChannels               = 0xA18C, ///< Configure to Sync channels
  TpProcMemC6StorageAllocation               = 0xA18D, ///< Allocate C6 Storage
  TpProcMemLvDdr4                            = 0xA18E, ///< Before LV DDR4

  // BR CPU
  TpBrBeforeApLaunch                         = 0xA190, ///< BR before AP launch
  TpBrInstallApLaunchedPpi                   = 0xA191, ///< Install AP launched PPI
  TpBrAfterApLaunch                          = 0xA192, ///< BR after AP launch
  TpBrBeforePM                               = 0xA193, ///< Before CPU PM
  TpBrIoCstate                               = 0xA194, ///< Enable IO Cstate
  TpBrC6                                     = 0xA195, ///< Enable C6
  TpBrInstallCcxPeiCompletePpi               = 0xA196, ///< Install CCX PEI complete PPI
  TpBrMemoryDoenCallBackEntry                = 0xA197, ///< BR CPU memory done call back entry
  TpBrBeforeApmWeights                       = 0xA198, ///< Before APM weights
  TpBrAfterApmWeights                        = 0xA199, ///< After APM weights
  TpBrMemoryDoneCallBackEnd                  = 0xA19A, ///< BR CPU memory done call back end
  TpBrInitMidEntry                           = 0xA19B, ///< BR Init Mid entry
  TpBrApm                                    = 0xA19C, ///< BR enable APM
  TpBrInitMidInstallProtocol                 = 0xA19D, ///< BR Init Mid install protocol
  TpBrInitMidEnd                             = 0xA19E, ///< BR Init Mid end
  TpBrInitLateEntry                          = 0xA19F, ///< BR Init Late entry
  TpBrInitLateInstallProtol                  = 0xA1A0, ///< BR Init Late install protocol
  TpBrInitLateEnd                            = 0xA1A1, ///< BR Init Late end
  TpBrDxeInstallCompleteProtocol             = 0xA1A2, ///< BR DXE install complete protocol
  TpUnbInstallCompletePpi                    = 0xA1A3, ///< UNB install complete PPI
  TpUnbAfterApLaunchCallbackEntry            = 0xA1A4, ///< UNB AfterApLaunch callback entry
  TpUnbAfterApLaunchCallbackEnd              = 0xA1A5, ///< UNB AfterApLaunch callback end

  // S3 Interface Post Code
  TpIfBeforeAllocateS3SaveBuffer             = 0xA1EC, ///< Before the S3 save code calls out to allocate a buffer
  TpIfAfterAllocateS3SaveBuffer              = 0xA1ED, ///< After the S3 save code calls out to allocate a buffer
  TpIfBeforeAllocateMemoryS3SaveBuffer       = 0xA1EE, ///< Before the memory S3 save code calls out to allocate a buffer
  TpIfAfterAllocateMemoryS3SaveBuffer        = 0xA1EF, ///< After the memory S3 save code calls out to allocate a buffer
  TpIfBeforeLocateS3PciBuffer                = 0xA1F0, ///< Before the memory code calls out to locate a buffer
  TpIfAfterLocateS3PciBuffer                 = 0xA1F1, ///< After the memory code calls out to locate a buffer
  TpIfBeforeLocateS3CPciBuffer               = 0xA1F2, ///< Before the memory code calls out to locate a buffer
  TpIfAfterLocateS3CPciBuffer                = 0xA1F3, ///< After the memory code calls out to locate a buffer
  TpIfBeforeLocateS3MsrBuffer                = 0xA1F4, ///< Before the memory code calls out to locate a buffer
  TpIfAfterLocateS3MsrBuffer                 = 0xA1F5, ///< After the memory code calls out to locate a buffer
  TpIfBeforeLocateS3CMsrBuffer               = 0xA1F6, ///< Before the memory code calls out to locate a buffer
  TpIfAfterLocateS3CMsrBuffer                = 0xA1F7, ///< After the memory code calls out to locate a buffer
  // PMU Post Code
  TpProcMemPmuFailed                         = 0xA1F9, ///< Failed PMU training.

  // [0xA2XX ~ 0xA4XX] Reserved

  // [0xA5XX ~ 0xA6XX] assigned for AGESA PSP Module
  // PSP V1 Modules
  TpPspPeiV1Entry                            = 0xA501, ///< PspPeiV1 entry
  TpPspPeiV1Exit                             = 0xA502, ///< PspPeiV1 exit
  TpPspPeiV1MemDiscoveredPpiCallbackEntry    = 0xA503, ///< MemoryDiscoveredPpiCallback entry
  TpPspPeiV1MemDiscoveredPpiCallbackExit     = 0xA504, ///< MemoryDiscoveredPpiCallback exit
  TpPspDxeV1Entry                            = 0xA507, ///< PspDxeV1 entry
  TpPspDxeV1Exit                             = 0xA508, ///< PspDxeV1 exit
  TpPspDxeV1PciCallbackEntry                 = 0xA50A, ///< PspDxeV1 PspPciEnumerationCompleteCallBack entry
  TpPspDxeV1PciCallbackExit                  = 0xA50B, ///< PspDxeV1 PspPciEnumerationCompleteCallBack exit
  TpPspDxeV1RTBCallbackEntry                 = 0xA50C, ///< PspDxeV1 ready to boot entry
  TpPspDxeV1RTBCallbackExit                  = 0xA50D, ///< PspDxeV1 ready to boot exit
  TpPspSmmV1Entry                            = 0xA50E, ///< PspSmmV1 entry
  TpPspSmmV1Exit                             = 0xA50F, ///< PspSmmV1 exit
  TpPspSmmV1SwSmiCallbackEntry               = 0xA510, ///< PspSmmV1 SwSmiCallBack entry, build the S3 save area for resume
  TpPspSmmV1SwSmiCallbackExit                = 0xA511, ///< PspSmmV1 SwSmiCallBack exit, build the S3 save area for resume
  TpPspSmmV1BspRsmEntry                      = 0xA512, ///< PspSmmV1 BspSmmResumeVector entry
  TpPspSmmV1BspRsmExit                       = 0xA513, ///< PspSmmV1 BspSmmResumeVector exit
  TpPspSmmV1ApRsmEntry                       = 0xA514, ///< PspSmmV1 ApSmmResumeVector entry
  TpPspSmmV1ApRsmExit                        = 0xA515, ///< PspSmmV1 ApSmmResumeVector exit
  TpPspP2CmboxV1Entry                        = 0xA516, ///< PspP2CmboxV1 entry
  TpPspP2CmboxV1Exit                         = 0xA517, ///< PspP2CmboxV1 exit
  // PSP V2 Modules
  TpPspPeiV2Entry                            = 0xA521, ///< PspPeiV2 entry
  TpPspPeiV2Exit                             = 0xA522, ///< PspPeiV2 exit
  TpPspDxeV2Entry                            = 0xA523, ///< PspDxeV2 entry
  TpPspDxeV2Exit                             = 0xA524, ///< PspDxeV2 exit
  TpPspDxeV2MpCallBackEntry                  = 0xA525, ///< PspDxeV2 PspMpServiceCallBack entry
  TpPspDxeV2MpCallBackExit                   = 0xA526, ///< PspDxeV2 PspMpServiceCallBack exit
  TpPspDxeV2FlashAccCallBackEntry            = 0xA527, ///< PspDxeV2 FlashAccCallBack entry
  TpPspDxeV2FlashAccCallBackExit             = 0xA528, ///< PspDxeV2 FlashAccCallBack exit
  TpPspDxeV2RTBCallbackEntry                 = 0xA529, ///< PspDxeV2 ready to boot entry
  TpPspDxeV2RTBCallbackExit                  = 0xA52A, ///< PspDxeV2 ready to boot exit
  TpPspDxeV2ExitBsCallbackEntry              = 0xA52B, ///< PspDxeV2 exit boot serivce entry
  TpPspDxeV2ExitBsCallbackExit               = 0xA52C, ///< PspDxeV2 exit boot serivce exit
  TpPspSmmV2Entry                            = 0xA52D, ///< PspSmmV2 entry
  TpPspSmmV2Exit                             = 0xA52E, ///< PspSmmV2 exit
  TpPspSmmV2SwSmiCallbackEntry               = 0xA52F, ///< PspSmmV2 SwSmiCallBack entry, build the S3 save area for resume
  TpPspSmmV2SwSmiCallbackExit                = 0xA530, ///< PspSmmV2 SwSmiCallBack exit, build the S3 save area for resume
  TpPspSmmV2BspRsmEntry                      = 0xA531, ///< PspSmmV2 BspSmmResumeVector entry
  TpPspSmmV2BspRsmExit                       = 0xA532, ///< PspSmmV2 BspSmmResumeVector exit
  TpPspSmmV2ApRsmEntry                       = 0xA533, ///< PspSmmV2 ApSmmResumeVector entry
  TpPspSmmV2ApRsmExit                        = 0xA534, ///< PspSmmV2 ApSmmResumeVector exit
  TpPspP2CmboxV2Entry                        = 0xA535, ///< PspP2CmboxV2 entry
  TpPspP2CmboxV2Exit                         = 0xA536, ///< PspP2CmboxV2 exit
  TpPspRecoverApcbFail                       = 0xA537, ///< TpPspRecoverApcbFail
  TpPspDxeV2ApcbAccCallBackEntry             = 0xA539, ///< PspDxeV2 ApcbAccCallBack entry
  TpPspDxeV2ApcbAccCallBackExit              = 0xA53A, ///< PspDxeV2 ApcbAccCallBack exit
  TpEnableRdFail                             = 0xA53B, ///< Enable Rd Instruction Fail
  TpScpcAutoEnFailBigSizeVar                 = 0xA53C, ///< ScpcAutoEnablement failed due a variable with larger size detected
  TpScpcAutoEnFailSetVar                     = 0xA53D, ///< ScpcAutoEnablement failed due SetVariable
  TpScpcAutoEnFailDelVar                     = 0xA53E, ///< ScpcAutoEnablement failed due delete variable

  // PSP fTpm modules
  TpPspfTpmPeiEntry                          = 0xA540, ///< PspfTpmPei entry
  TpPspfTpmPeiExit                           = 0xA541, ///< PspfTpmPei exit
  TpPspfTpmPeiMemCallbackEntry               = 0xA542, ///< PspfTpmPei memory callback entry
  TpPspfTpmPeiMemCallbackExit                = 0xA543, ///< PspfTpmPei memory callback exit
  TpPspfTpmDxeEntry                          = 0xA544, ///< PspfTpmDxe entry
  TpPspfTpmDxeExit                           = 0xA545, ///< PspfTpmDxe exit
  // PSP dTpm modules
  TpPspdTpmPeiEntry                          = 0xA546, ///< PspdTpmPei entry
  TpPspdTpmPeiExit                           = 0xA547, ///< PspdTpmPei exit
  // HSP fTpm modules
  TpHspfTpmPeiEntry                          = 0xA548, ///< HspfTpmPei entry
  TpHspfTpmPeiExit                           = 0xA549, ///< HspfTpmPei exit
  TpHspfTpmPeiInitializeEntry                = 0xA54A, ///< HspfTpmPei Initialize Entry
  TpHspfTpmPeiInitializeExit                 = 0xA54B, ///< HspfTpmPei Initialize Exit
  TpHspfTpmDxeEntry                          = 0xA54C, ///< HspfTpmDxe entry
  TpHspfTpmDxeExit                           = 0xA54D, ///< HspfTpmDxe exit
  // Intrusion Detection [0xA56X]
  TpPspIntrusionDetectionPeiEntry            = 0xA560, ///< PspIntrusionDetectionPei Entry
  TpPspIntrusionDetectionPeiExit             = 0xA561, ///< PspIntrusionDetectionPei Exit
  TpPspIntrusionDetectionDxeEntry            = 0xA562, ///< PspIntrusionDetectionDxe Entry
  TpPspIntrusionDetectionDxeExit             = 0xA563, ///< PspIntrusionDetectionDxe Exit
  TpPspDoIntrusionDetectionActionDxe         = 0xA564, ///< Psp DoIntrusionDetectionAction in Dxe
  TpPspIntrusionDetectionClearTpm            = 0xA565, ///< Psp IntrusionDetection Clear Tpm
  TpPspIntrusionDetectionSmmEntry            = 0xA566, ///< PspIntrusionDetectionSmm Entry
  TpPspIntrusionDetectionSmmExit             = 0xA567, ///< PspIntrusionDetectionSmm Exit
  TpPspIntrusionDetectionSmiCallback         = 0xA568, ///< Psp IntrusionDetectionSmiCallback
  TpPspDoIntrusionDetectionActionSmm         = 0xA569, ///< Psp DoIntrusionDetectionAction in Smm
  TpPspIntrusionEventLoggingError            = 0xA56E, ///< Psp Intrusion Event Logging Error
  TpPspIntrusionGetEventLogError             = 0xA56F, ///< Psp Intrusion Get Event Log Error

  // P2C mailbox Handling [0xA59X]
  TpPspP2CmboxSpiGetAttribEntry              = 0xA591, ///< PspP2Cmbox Command SpiGetAttrib Handling entry
  TpPspP2CmboxSpiSetAttribEntry              = 0xA592, ///< PspP2Cmbox Command SpiSetAttrib Handling entry
  TpPspP2CmboxSpiGetBlockSizeEntry           = 0xA593, ///< PspP2Cmbox Command SpiGetBlockSize Handling entry
  TpPspP2CmboxSpiReadFVEntry                 = 0xA594, ///< PspP2Cmbox Command SpiReadFV Handling entry
  TpPspP2CmboxSpiWriteFVEntry                = 0xA595, ///< PspP2Cmbox Command SpiWriteFV Handling entry
  TpPspP2CmboxSpiEraseFVEntry                = 0xA596, ///< PspP2Cmbox Command SpiEraseFV Handling entry
  TpMboxPspCmdRpmcIncMcEntry                 = 0xA597, ///< PspP2Cmbox Command MboxPspCmdRpmcIncMc entry
  TpMboxPspCmdRpmcReqMcEntry                 = 0xA598, ///< PspP2Cmbox Command TpMboxPspCmdRpmcReqMc entry
  TpMboxPspCmdArsStatusEntry                 = 0xA599, ///< PspP2Cmbox Command TpMboxPspCmdArsStatus entry
  TpPspP2CmboxCmdExit                        = 0xA59E, ///< PspP2Cmbox Command Handling exit
  TpPspP2CmboxCmdFailExit                    = 0xA59F, ///< PspP2Cmbox Command Handling Fail exit

  // fTPM command Handling [0xA5FX]
  TpPspfTpmSendCmdEntry                      = 0xA5F0, ///< PspfTpm send TPM command entry
  TpPspfTpmSendCmdExit                       = 0xA5F1, ///< PspfTpm send TPM command exit
  TpPspfTpmRcvCmdEntry                       = 0xA5F2, ///< PspfTpm receive TPM command entry
  TpPspfTpmRcvCmdExit                        = 0xA5F3, ///< PspfTpm receive TPM command exit
  TpHspfTpmSendCmdEntry                      = 0xA5F4, ///< HspfTpm send TPM command entry
  TpHspfTpmSendCmdExit                       = 0xA5F5, ///< HspfTpm send TPM command exit
  TpHspfTpmRcvCmdEntry                       = 0xA5F6, ///< HspfTpm receive TPM command entry
  TpHspfTpmRcvCmdExit                        = 0xA5F7, ///< HspfTpm receive TPM command exit

  // C2P mailbox Handling [0xA601 ~ 0xA67F: before send C2P command, 0xA681 ~ 0xA6FF: wait C2P command]
  TpPspC2PmboxBeforeSendCmdBase                   = 0xA600, ///< PSP C2P mailbox entry base [0xA600 | Cmd]
  TpPspC2PmboxCmdDone                             = 0xA600, ///< C2P command done, post code reused from TpPspC2PmboxBeforeSendCmdBase
  TpPspC2PmboxBeforeCmdDramInfo                   = 0xA601, ///< Before send C2P command MboxBiosCmdDramInfo
  TpPspC2PmboxBeforeCmdSmmInfo                    = 0xA602, ///< Before send C2P command MboxBiosCmdSmmInfo
  TpPspC2PmboxBeforeCmdSxInfo                     = 0xA603, ///< Before send C2P command MboxBiosCmdSxInfo
  TpPspC2PmboxBeforeCmdRsmInfo                    = 0xA604, ///< Before send C2P command MboxBiosCmdRsmInfo
  TpPspC2PmboxBeforeCmdFtpmQuery                  = 0xA605, ///< Before send C2P command MboxBiosCmdFtpmQuery
  TpPspC2PmboxBeforeCmdBootDone                   = 0xA606, ///< Before send C2P command MboxBiosCmdBootDone
  TpPspC2PmboxBeforeCmdClearS3Sts                 = 0xA607, ///< Before send C2P command MboxBiosCmdClearS3Sts
  TpPspC2PmboxBeforeS3DataInfo                    = 0xA608, ///< Before send C2P command MboxBiosS3DataInfo
  TpPspC2PmboxBeforeCmdNop                        = 0xA609, ///< Before send C2P command MboxBiosCmdNop
  TpPspC2PmboxBeforeCmdHSTIQuery                  = 0xA614, ///< Before send C2P command MboxBiosCmdHSTIQuery
  TpPspC2PmboxBeforeCmdGetVersion                 = 0xA619, ///< Before send C2P command MboxBiosCmdGetVersion
  TpPspC2PmboxBeforeCmdLockDFReg                  = 0xA61B, ///< Before send C2P command MboxBiosCmdLockDFReg
  TpPspC2PmboxBeforeCmdClrSmmLock                 = 0xA61C, ///< Before send C2P command MboxBiosCmdClrSmmLock
  TpPspC2PmboxBeforeCmdSetApCsBase                = 0xA61D, ///< Before send C2P command MboxBiosCmdSetApCsBase
  TpPspC2PmboxBeforeCmdKvmInfo                    = 0xA61E, ///< Before send C2P command MboxBiosCmdKvmInfo
  TpPspC2PmboxBeforeCmdLockSpi                    = 0xA61F, ///< Before send C2P command MboxBiosCmdLockSpi
  TpPspC2PmboxBeforeCmdScreenOnGpio               = 0xA620, ///< Before send C2P command MboxBiosCmdScreenOnGpio
  TpPspC2PmboxBeforeCmdSpiOpWhiteList             = 0xA621, ///< Before send C2P command MboxBiosCmdSpiOpWhiteList
  TpPspC2PmboxBeforeCmdRasEinj                    = 0xA622, ///< Before send C2P command MboxBiosCmdRasEinj
  TpPspC2PmboxBeforeCmdStartArs                   = 0xA624, ///< Before send C2P command MboxBiosCmdStartArs
  TpPspC2PmboxBeforeCmdStopArs                    = 0xA625, ///< Before send C2P command MboxBiosCmdStopArs
  TpPspC2PmboxBeforeCmdSetBootPartitionId         = 0xA626, ///< Before send C2P command MboxBiosCmdSetBootPartitionId
  TpPspC2PmboxBeforeCmdPspCapsQuery               = 0xA627, ///< Before send C2P command MboxBiosCmdPspCapsQuery
  TpPspC2PmboxBeforeCmdArmorEnterSmmOnlyMode      = 0xA628, ///< Before send C2P command MboxBiosCmdArmorEnterSmmOnlyMode
  TpPspC2PmboxBeforeCmdArmorEnforceWhitelist      = 0xA629, ///< Before send C2P command MboxBiosCmdArmorEnforceWhitelist
  TpPspC2PmboxBeforeCmdArmorExecuteSpiCommand     = 0xA62A, ///< Before send C2P command MboxBiosCmdArmorExecuteSpiCommand
  TpPspC2PmboxBeforeCmdArmorSwitchCsMode          = 0xA62B, ///< Before send C2P command MboxBiosCmdArmorSwitchCsMode
  TpPspC2PmboxBeforeCmdDrtmInfoId                 = 0xA62C, ///< Before send C2P command MboxBiosCmdDrtmInfoId
  TpPspC2PmboxBeforeCmdLaterSplFuse               = 0xA62D, ///< Before send C2P command MboxBiosCmdLaterSplFuse
  TpPspC2PmboxBeforeCmdDtpmInfo                   = 0xA62E, ///< Before send C2P command MboxBiosCmdDtpmInfo
  TpPspC2PmboxBeforeCmdValidateManOsSignature     = 0xA62F, ///< Before send C2P command MboxBiosCmdValidateManOsSignature
  TpPspC2PmboxBeforeCmdLockFCHReg                 = 0xA630, ///< Before send C2P command MboxBiosCmdLockFCHReg
  TpPspC2PmboxBeforeCmdPostDrtmInfoQuery          = 0xA631, ///< Before send C2P command MboxBiosCmdPostDrtmInfoQuery
  TpPspC2PmboxBeforeCmdSignValidateHmacDataPreSmm = 0xA634, ///< Before send C2P command MboxBiosCmdSignValidateHmacDataPreSmm
  TpPspC2PmboxBeforeCmdSignValidateHmacDataSmm    = 0xA635, ///< Before send C2P command MboxBiosCmdSignValidateHmacDataSmm
  TpPspC2PmboxBeforeCmdGetBootPartitionId         = 0xA636, ///< Before send C2P command MboxBiosCmdGetBootPartitionId
  TpPspC2PmboxBeforeCmdSetRpmcAddress             = 0xA639, ///< Before send C2P command MboxBiosCmdSetRpmcAddress
  TpPspC2PmboxBeforeCmdSetGpioFencing             = 0xA63A, ///< Before send C2P command MboxBiosCmdSetGpioFencing
  TpPspC2PmboxBeforeCmdSendIvrsAcpiTable          = 0xA63F, ///< Before send C2P command MboxBiosCmdSendIvrsAcpiTable
  TpPspC2PmboxBeforeCmdTa                         = 0xA640, ///< Before send C2P command MboxBiosCmdTa
  TpPspC2PmboxBeforeCmdAcpiRasEinj                = 0xA641, ///< Before send C2P command MboxBiosCmdAcpiRasEinj
  TpPspC2PmboxBeforeCmdQueryTCGLog                = 0xA642, ///< Before send C2P command MboxBiosCmdQueryTCGLog
  TpPspC2PmboxBeforeCmdQuerySplFuse               = 0xA647, ///< Before send C2P command MboxBiosCmdQuerySplFuse
  TpPspC2PmboxBeforeCmdManageabilityCfg           = 0xA649, ///< Before send C2P command MboxBiosCmdManageabilityCfg
  TpPspC2PmboxBeforeCmdDisablePsb                 = 0xA64A, ///< Before send C2P command MboxBiosCmdDisablePsb
  TpPspC2PmboxBeforeCmdUsbConfig                  = 0xA64B, ///< Before send C2P command MboxBiosCmdUsbConfig
  TpPspC2PmboxBeforeCmdMpmPciAccess               = 0xA64D, ///< Before send C2P command MboxBiosCmdMpmPciAccess
  TpPspC2PmboxBeforeCmdStbVerbosity               = 0xA64F, ///< Before send C2P command MboxBiosCmdStbVerbosity
  TpPspC2PmboxBeforeCmdArmorEnterSmmOnlyMode2     = 0xA650, ///< Before send C2P command MboxBiosCmdArmorEnterSmmOnlyMode2
  TpPspC2PmboxBeforeCmdArmorSpiTransaction        = 0xA651, ///< Before send C2P command MboxBiosCmdArmorSpiTransaction
  TpPspC2PmboxBeforeCmdDeferredPsbFuse            = 0xA652, ///< Before send C2P command MboxBiosCmdDeferredPsbFuse
  TpPspC2PmboxBeforeCmdLoadWlanFw                 = 0xA653, ///< Before send C2P command MboxBiosCmdLoadWlanFw
  TpPspC2PmboxBeforeCmdQuerySplValue              = 0xA654, ///< Before send C2P command MboxBiosCmdQuerySplValue

  TpPspC2PmboxWaitCmdBase                         = 0xA680, ///< PSP C2P mailbox exit base [0xA680 | Cmd]
  TpPspC2PmboxWaitCmdDramInfo                     = 0xA681, ///< Wait C2P command MboxBiosCmdDramInfo finished
  TpPspC2PmboxWaitCmdSmmInfo                      = 0xA682, ///< Wait C2P command MboxBiosCmdSmmInfo finished
  TpPspC2PmboxWaitCmdSxInfo                       = 0xA683, ///< Wait C2P command MboxBiosCmdSxInfo finished
  TpPspC2PmboxWaitCmdRsmInfo                      = 0xA684, ///< Wait C2P command MboxBiosCmdRsmInfo finished
  TpPspC2PmboxWaitCmdFtpmQuery                    = 0xA685, ///< Wait C2P command MboxBiosCmdFtpmQuery finished
  TpPspC2PmboxWaitCmdBootDone                     = 0xA686, ///< Wait C2P command MboxBiosCmdBootDone finished
  TpPspC2PmboxWaitCmdClearS3Sts                   = 0xA687, ///< Wait C2P command MboxBiosCmdClearS3Sts finished
  TpPspC2PmboxWaitS3DataInfo                      = 0xA688, ///< Wait C2P command MboxBiosS3DataInfo finished
  TpPspC2PmboxWaitCmdNop                          = 0xA689, ///< Wait C2P command MboxBiosCmdNop finished
  TpPspC2PmboxWaitCmdHSTIQuery                    = 0xA694, ///< Wait C2P command MboxBiosCmdHSTIQuery finished
  TpPspC2PmboxWaitCmdGetVersion                   = 0xA699, ///< Wait C2P command MboxBiosCmdGetVersion finished
  TpPspC2PmboxWaitCmdLockDFReg                    = 0xA69B, ///< Wait C2P command MboxBiosCmdLockDFReg finished
  TpPspC2PmboxWaitCmdClrSmmLock                   = 0xA69C, ///< Wait C2P command MboxBiosCmdClrSmmLock finished
  TpPspC2PmboxWaitCmdSetApCsBase                  = 0xA69D, ///< Wait C2P command MboxBiosCmdSetApCsBase finished
  TpPspC2PmboxWaitCmdKvmInfo                      = 0xA69E, ///< Wait C2P command MboxBiosCmdKvmInfo finished
  TpPspC2PmboxWaitCmdLockSpi                      = 0xA69F, ///< Wait C2P command MboxBiosCmdLockSpi finished
  TpPspC2PmboxWaitCmdScreenOnGpio                 = 0xA6A0, ///< Wait C2P command MboxBiosCmdScreenOnGpio finished
  TpPspC2PmboxWaitCmdSpiOpWhiteList               = 0xA6A1, ///< Wait C2P command MboxBiosCmdSpiOpWhiteList finished
  TpPspC2PmboxWaitCmdRasEinj                      = 0xA6A2, ///< Wait C2P command MboxBiosCmdRasEinj finished
  TpPspC2PmboxWaitCmdStartArs                     = 0xA6A4, ///< Wait C2P command MboxBiosCmdStartArs finished
  TpPspC2PmboxWaitCmdStopArs                      = 0xA6A5, ///< Wait C2P command MboxBiosCmdStopArs finished
  TpPspC2PmboxWaitCmdSetBootPartitionId           = 0xA6A6, ///< Wait C2P command MboxBiosCmdSetBootPartitionId finished
  TpPspC2PmboxWaitCmdPspCapsQuery                 = 0xA6A7, ///< Wait C2P command MboxBiosCmdPspCapsQuery finished
  TpPspC2PmboxWaitCmdArmorEnterSmmOnlyMode        = 0xA6A8, ///< Wait C2P command MboxBiosCmdArmorEnterSmmOnlyMode finished
  TpPspC2PmboxWaitCmdArmorEnforceWhitelist        = 0xA6A9, ///< Wait C2P command MboxBiosCmdArmorEnforceWhitelist finished
  TpPspC2PmboxWaitCmdArmorExecuteSpiCommand       = 0xA6AA, ///< Wait C2P command MboxBiosCmdArmorExecuteSpiCommand finished
  TpPspC2PmboxWaitCmdArmorSwitchCsMode            = 0xA6AB, ///< Wait C2P command MboxBiosCmdArmorSwitchCsMode finished
  TpPspC2PmboxWaitCmdDrtmInfoId                   = 0xA6AC, ///< Wait C2P command MboxBiosCmdDrtmInfoId finished
  TpPspC2PmboxWaitCmdLaterSplFuse                 = 0xA6AD, ///< Wait C2P command MboxBiosCmdLaterSplFuse finished
  TpPspC2PmboxWaitCmdDtpmInfo                     = 0xA6AE, ///< Wait C2P command MboxBiosCmdDtpmInfo finished
  TpPspC2PmboxWaitCmdValidateManOsSignature       = 0xA6AF, ///< Wait C2P command MboxBiosCmdValidateManOsSignature finished
  TpPspC2PmboxWaitCmdLockFCHReg                   = 0xA6B0, ///< Wait C2P command MboxBiosCmdLockFCHReg finished
  TpPspC2PmboxWaitCmdPostDrtmInfoQuery            = 0xA6B1, ///< Wait C2P command MboxBiosCmdPostDrtmInfoQuery finished
  TpPspC2PmboxWaitCmdSignValidateHmacDataPreSmm   = 0xA6B4, ///< Wait C2P command MboxBiosCmdSignValidateHmacDataPreSmm finished
  TpPspC2PmboxWaitCmdSignValidateHmacDataSmm      = 0xA6B5, ///< Wait C2P command MboxBiosCmdSignValidateHmacDataSmm finished
  TpPspC2PmboxWaitCmdGetBootPartitionId           = 0xA6B6, ///< Wait C2P command MboxBiosCmdGetBootPartitionId finished
  TpPspC2PmboxWaitCmdSetRpmcAddress               = 0xA6B9, ///< Wait C2P command MboxBiosCmdSetRpmcAddress finished
  TpPspC2PmboxWaitCmdSetGpioFencing               = 0xA6BA, ///< Wait C2P command MboxBiosCmdSetGpioFencing finished
  TpPspC2PmboxWaitCmdSendIvrsAcpiTable            = 0xA6BF, ///< Wait C2P command MboxBiosCmdSendIvrsAcpiTable finished
  TpPspC2PmboxWaitCmdTa                           = 0xA6C0, ///< Wait C2P command MboxBiosCmdTa finished
  TpPspC2PmboxWaitCmdAcpiRasEinj                  = 0xA6C1, ///< Wait C2P command MboxBiosCmdAcpiRasEinj finished
  TpPspC2PmboxWaitCmdQueryTCGLog                  = 0xA6C2, ///< Wait C2P command MboxBiosCmdQueryTCGLog finished
  TpPspC2PmboxWaitCmdQuerySplFuse                 = 0xA6C7, ///< Wait C2P command MboxBiosCmdQuerySplFuse finished
  TpPspC2PmboxWaitCmdManageabilityCfg             = 0xA6C9, ///< Wait C2P command MboxBiosCmdManageabilityCfg finished
  TpPspC2PmboxWaitCmdDisablePsb                   = 0xA6CA, ///< Wait C2P command MboxBiosCmdDisablePsb finished
  TpPspC2PmboxWaitCmdUsbConfig                    = 0xA6CB, ///< Wait C2P command MboxBiosCmdUsbConfig finished
  TpPspC2PmboxWaitCmdMpmPciAccess                 = 0xA6CD, ///< Wait C2P command MboxBiosCmdMpmPciAccess finished
  TpPspC2PmboxWaitCmdStbVerbosity                 = 0xA6CF, ///< Wait C2P command MboxBiosCmdStbVerbosity finished
  TpPspC2PmboxWaitCmdArmorEnterSmmOnlyMode2       = 0xA6D0, ///< Wait C2P command MboxBiosCmdArmorEnterSmmOnlyMode2 finished
  TpPspC2PmboxWaitCmdArmorSpiTransaction          = 0xA6D1, ///< Wait C2P command MboxBiosCmdArmorSpiTransaction finished
  TpPspC2PmboxWaitCmdDeferredPsbFuse              = 0xA6D2, ///< Wait C2P command MboxBiosCmdDeferredPsbFuse finished
  TpPspC2PmboxWaitCmdLoadWlanFw                   = 0xA6D3, ///< Wait C2P command MboxBiosCmdLoadWlanFw finished
  TpPspC2PmboxWaitCmdQuerySplValue                = 0xA6D4, ///< Wait C2P command MboxBiosCmdQuerySplValue finished
  //MPM  related [0xA700 - 0xA77F]
  TpMpmPeiEntry                              = 0xA700, ///< MpmPei Driver entry
  TpMpmPeiExit                               = 0xA701, ///< MpmPei Driver exit
  TpMpmDxeEntry                              = 0xA702, ///< MpmDxe Driver entry
  TpMpmDxeExit                               = 0xA703, ///< MpmDxe Driver exit
  TpMpmDxeV2RTBCallbackEntry                 = 0xA704, ///< MPM RTB event entry
  TpMpmDxeV2RTBCallbackExit                  = 0xA705, ///< MPM RTB event exit
  TpMpmSerialIoEntry                         = 0xA706, ///< Mpm SerialIo  Entry
  TpMpmSerialIoExit                          = 0xA707, ///< Mpm SerialIo Exit
  //USB4 related [0xA780 - 0xA79F]
  TpUsb4PeiEntry                             = 0xA780, ///< Usb4Pei Driver entry
  TpUsb4PeiExit                              = 0xA781, ///< Usb4Pei Driver exit
  TpUsb4PeiMemoryDicoverPpiCallbackEntry     = 0xA782, ///< Usb4Pei MemoryDicoverPpiCallback entry
  TpUsb4PeiMemoryDicoverPpiCallbackExit      = 0xA783, ///< Usb4Pei MemoryDicoverPpiCallback exit
  TpUsb4DxeEntry                             = 0xA784, ///< Usb4Dxe Driver entry
  TpUsb4DxeExit                              = 0xA785, ///< Usb4Dxe Driver exit

  // [0xA7A0 ~ 0xA8XX] Reserved

  // [0xA9XX, 0xAAXX] assigned for AGESA NBIO Module
  // [0xAA00, 0xAA7F] assigned for SMU Message ID
  TpSmuMessageID                             = 0xAA00, ///< SmuMessageID
  // [0xA900, 0xA94F] NBIO PEIM/DXE Driver
  // NbioBase
  TpNbioBasePeiEntry                         = 0xA900, ///< AmdNbioBase PEIM driver entry
  TpNbioBasePeiExit                          = 0xA901, ///< AmdNbioBase PEIM driver exit
  TpNbioBaseDxeEntry                         = 0xA902, ///< AmdNbioBase DXE driver entry
  TpNbioBaseDxeExit                          = 0xA903, ///< AmdNbioBase DXE driver exit
  // PCIe
  TpNbioPCIePeiEntry                         = 0xA904, ///< AmdNbioPcie PEIM driver entry
  TpNbioPCIePeiExit                          = 0xA905, ///< AmdNbioPcie PEIM driver exit
  TpNbioPCIeDxeEntry                         = 0xA906, ///< AmdNbioPcie DXE driver entry
  TpNbioPCIeDxeExit                          = 0xA907, ///< AmdNbioPcie DXE driver exit
  // GFX
  TpNbioGfxPeiEntry                          = 0xA908, ///< AmdNbioGfx PEIM driver entry
  TpNbioGfxPeiExit                           = 0xA909, ///< AmdNbioGfx PEIM driver exit
  TpNbioGfxDxeEntry                          = 0xA90A, ///< AmdNbioGfx DXE driver entry
  TpNbioGfxDxeExit                           = 0xA90B, ///< AmdNbioGfx DXE driver exit
  // IOMMU
  TpNbioIommuDxeEntry                        = 0xA90C, ///< AmdNbioIommu DXE driver entry
  TpNbioIommuDxeExit                         = 0xA90D, ///< AmdNbioIommu DXE driver exit
  // ALIB
  TpNbioAlibDxeEntry                         = 0xA90E, ///< AmdNbioALIB DXE driver entry
  TpNbioAlibDxeExit                          = 0xA90F, ///< AmdNbioALIB DXE driver exit
  // SMU
  TpNbioSmuPeiEntry                          = 0xA910, ///< AmdSmu PEI driver entry
  TpNbioSmuPeiExit                           = 0xA911, ///< AmdSmu PEI driver exit
  TpNbioSmuDxeEntry                          = 0xA912, ///< AmdSmu DXE driver entry
  TpNbioSmuDxeExit                           = 0xA913, ///< AmdSmu DXE driver exit
  TpSmuServiceRequestEntry                   = 0xA914, ///< SmuServiceRequest entry
  TpSmuServiceRequestExit                    = 0xA915, ///< SmuServiceRequest exit
  // IOAPIC
  TpNbioIoapicPeiEntry                       = 0xA916, ///< AmdNbioIoApic PEI driver entry
  TpNbioIoapicPeiExit                        = 0xA917, ///< AmdNbioIoApic PEI driver exit

  // IOMMU PEIM
  TpNbioIommuPEIEntry                        = 0xA920, ///< AmdNbioIommu PEIM driver entry
  TpNbioIommuPEIExit                         = 0xA921, ///< AmdNbioIommu PEIM driver exit

  // APCB DXE
  TpApcbDxeEntry                             = 0xA922, ///< APCB DXE Entry
  TpApcbDxeExit                              = 0xA923, ///< APCB DXE Exit

  // APCB SMM
  TpApcbSmmEntry                             = 0xA924, ///< APCB SMM Entry
  TpApcbSmmExit                              = 0xA925, ///< APCB SMM Exit

  TpEarlyEntry                               = 0xA930, ///< Early Exit
  TpEarlyExit                                = 0xA931, ///< Early Exit
  TpEarlyTrained                             = 0xA932, ///< Early Training is completed

  // [0xA950, 0xA99F] NBIO PPI/PROTOCOL Callback
  TpNbioTopologyConfigureCallbackEntry       = 0xA950, ///< NbioTopologyConfigureCallback entry
  TpNbioTopologyConfigureCallbackExit        = 0xA951, ///< NbioTopologyConfigureCallback exit
  TpMemoryConfigDoneCallbackPpiEntry         = 0xA952, ///< MemoryConfigDoneCallbackPpi entry
  TpMemoryConfigDoneCallbackPpiExit          = 0xA953, ///< MemoryConfigDoneCallbackPpi exit
  TpDxioInitializationCallbackPpiEntry       = 0xA954, ///< DxioInitializationCallbackPpi entry
  TpDxioInitializationCallbackPpiExit        = 0xA955, ///< DxioInitializationCallbackPpi exit
  TpDispatchSmuV9CallbackEntry               = 0xA956, ///< DispatchSmuV9Callback entry
  TpDispatchSmuV9CallbackExit                = 0xA957, ///< DispatchSmuV9Callback exit
  TpDispatchSmuV10CallbackEntry              = 0xA958, ///< DispatchSmuV10Callback entry
  TpDispatchSmuV10CallbackExit               = 0xA959, ///< DispatchSmuV10Callback exit
  TpAmdPcieMiscInitEntry                     = 0xA95A, ///< AmdPcieMiscInit Event entry
  TpAmdPcieMiscInitExit                      = 0xA95B, ///< AmdPcieMiscInit Event exit
  TpNbioBaseHookReadyToBootEntry             = 0xA95C, ///< NbioBaseHookReadyToBoot Event entry
  TpNbioBaseHookReadyToBootExit              = 0xA95D, ///< NbioBaseHookReadyToBoot Event exit
  TpNbioBaseHookPciIOEntry                   = 0xA95E, ///< NbioBaseHookPciIO Event entry
  TpNbioBaseHookPciIOExit                    = 0xA95F, ///< NbioBaseHookPciIO Event exit
  TpDispatchSmuV13CallbackEntry              = 0xA960, ///< DispatchSmuV13Callback entry
  TpDispatchSmuV13CallbackExit               = 0xA961, ///< DispatchSmuV13Callback exit

  // [0xA980, 0xA99F] BR GNB task
  TpGnbEarlyInterfaceCZEntry                 = 0xA970, ///< GnbEarlyInterfaceCZ entry
  TpGnbEarlyInterfaceCZExit                  = 0xA971, ///< GnbEarlyInterfaceCZ exit
  TpPcieConfigurationInitEntry               = 0xA972, ///< PcieConfigurationInit entry
  TpPcieConfigurationInitExit                = 0xA973, ///< PcieConfigurationInit exit
  TpGnbEarlierInterfaceCZEntry               = 0xA974, ///< GnbEarlierInterfaceCZ entry
  TpGnbEarlierInterfaceCZExit                = 0xA975, ///< GnbEarlierInterfaceCZ exit
  TpPcieEarlyInterfaceCZEntry                = 0xA976, ///< PcieEarlyInterfaceCZ entry
  TpPcieEarlyInterfaceCZExit                 = 0xA977, ///< PcieEarlyInterfaceCZ exit
  TpPciePostEarlyInterfaceCZEntry            = 0xA978, ///< PciePostEarlyInterfaceCZ entry
  TpPciePostEarlyInterfaceCZExit             = 0xA979, ///< PciePostEarlyInterfaceCZ exit
  TpGfxConfigPostInterfaceCZEntry            = 0xA97A, ///< GfxConfigPostInterfaceCZ entry
  TpGfxConfigPostInterfaceCZExit             = 0xA97B, ///< GfxConfigPostInterfaceCZ exit
  TpGfxPostInterfaceCZEntry                  = 0xA97C, ///< GfxPostInterfaceCZ entry
  TpGfxPostInterfaceCZExit                   = 0xA97D, ///< GfxPostInterfaceCZ exit
  TpGnbPostInterfaceCZEntry                  = 0xA97E, ///< GnbPostInterfaceCZ entry
  TpGnbPostInterfaceCZExit                   = 0xA97F, ///< GnbPostInterfaceCZ exit
  TpPciePostInterfaceCZEntry                 = 0xA980, ///< PciePostInterfaceCZ entry
  TpPciePostInterfaceCZExit                  = 0xA981, ///< PciePostInterfaceCZ exit

  TpGnbEnvInterfaceCZEntry                   = 0xA982, ///< GnbEnvInterfaceCZ entry
  TpGnbEnvInterfaceCZExit                    = 0xA983, ///< GnbEnvInterfaceCZ exit
  TpGfxConfigEnvInterfaceEntry               = 0xA984, ///< GfxConfigEnvInterface entry
  TpGfxConfigEnvInterfaceExit                = 0xA985, ///< GfxConfigEnvInterface exit
  TpGfxEnvInterfaceCZEntry                   = 0xA986, ///< GfxEnvInterfaceCZ entry
  TpGfxEnvInterfaceCZExit                    = 0xA987, ///< GfxEnvInterfaceCZ exit
  TpGfxMidInterfaceCZEntry                   = 0xA988, ///< GfxMidInterfaceCZ entry
  TpGfxMidInterfaceCZExit                    = 0xA989, ///< GfxMidInterfaceCZ exit
  TpGfxIntInfoTableInterfaceCZEntry          = 0xA98A, ///< GfxIntInfoTableInterfaceCZ entry
  TpGfxIntInfoTableInterfaceCZExit           = 0xA98B, ///< GfxIntInfoTableInterfaceCZ exit
  TpPcieMidInterfaceCZEntry                  = 0xA98C, ///< PcieMidInterfaceCZ entry
  TpPcieMidInterfaceCZExit                   = 0xA98D, ///< PcieMidInterfaceCZ exit
  TpGnbMidInterfaceCZEntry                   = 0xA98E, ///< GnbMidInterfaceCZ entry
  TpGnbMidInterfaceCZExit                    = 0xA98F, ///< GnbMidInterfaceCZ exit
  TpGnbSmuMidInterfaceCZEntry                = 0xA990, ///< GnbSmuMidInterfaceCZ entry
  TpGnbSmuMidInterfaceCZExit                 = 0xA991, ///< GnbSmuMidInterfaceCZ exit
  TpInvokeAmdInitLateEntry                   = 0xA992, ///< InvokeAmdInitLate entry
  TpInvokeAmdInitLateExit                    = 0xA993, ///< InvokeAmdInitLate exit

  // [0xABXX] Reserved

  // [0xACXX] assigned for AGESA CCX Module
  TpCcxIdsAfterApLaunch                      = 0xAC10, ///< CCX IDS IDS_HOOK_CCX_AFTER_AP_LAUNCH

  TpCcxPeiEntry                              = 0xAC50, ///< CCX PEI entry
  TpCcxPeiDownCoreCallbackEntry              = 0xAC51, ///< CCX downcore entry
  TpCcxDxeEntry                              = 0xAC55, ///< CCX DXE entry
  TpCcxDxeMpCallbackEntry                    = 0xAC56, ///< CCX MP service callback entry
  TpCcxDxeRtbCallBackEntry                   = 0xAC57, ///< CCX Ready To Boot callback entry
  TpCcxDxeOcCallbackEntry                    = 0xAC58, ///< CCX oc service callback entry
  TpCcxSmmEntry                              = 0xAC5D, ///< CCX SMM entry


  TpCcxPeiStartLaunchApsForS3                = 0xAC70, ///< CCX PEI start to launch APs for S3
  TpCcxPeiEndLaunchApsForS3                  = 0xAC71, ///< CCX PEI end of launching APs for S3

  TpCcxDxeStartLaunchAp                      = 0xAC90, ///< CCX start to launch AP
  TpCcxDxeEndLaunchAp                        = 0xAC91, ///< CCX launch AP is ended
  TpCcxDxeLaunchApAbort                      = 0xAC92, ///< CCX launch AP abort
  TpCcxDxeMpCallbackAbort                    = 0xAC93, ///< CCX MP service abort
  TpCcxDxeCacWeights                         = 0xAC94, ///< CCX cac weights
  TpCcxDxeBeforeInstallCcxDoneProtocol       = 0xAC95, ///< CCX before install CcxDone
  TpCcxDxeAfterInstallCcxDoneProtocol        = 0xAC96, ///< CCX after install CcxDone

  TpCcxPeiExit                               = 0xACE0, ///< CCX PEI exit
  TpCcxPeiDownCoreCallbackExit               = 0xACE1, ///< CCX downcore exit
  TpCcxDxeExit                               = 0xACE5, ///< CCX DXE exit
  TpCcxDxeMpCallbackExit                     = 0xACE6, ///< CCX MP service callback exit
  TpCcxDxeRtbCallBackExit                    = 0xACE7, ///< CCX Ready To Boot callback exit
  TpCcxDxeOcCallbackExit                     = 0xACE8, ///< CCX OC service callback exit
  TpCcxSmmExit                               = 0xACED, ///< CCX SMM exit

  // [0xADXX] assigned for AGESA DF Module
  TpDfPeiEntry                               = 0xAD50, ///< DF PEI entry
  TpDfDxeEntry                               = 0xAD55, ///< DF DXE entry
  TpDfReadyToBootEntry                       = 0xAD56, ///< DF Ready to Boot entry
  TpDfNbioSmuServicesPpiCallbackEntry        = 0xAD57, ///< DF NbioSmuServicesPpiCallback entry
  TpDfNbioSmuServicesProtocolCallbackEntry   = 0xAD58, ///< DF NbioSmuServicesProtocolCallback entry
  TpFabricSmmEntry                           = 0xAD59, ///< DF SMM entry
  TpFabricPciEnumerationCallbackEntry        = 0xAD60, ///< DF FabricPciEnumerationCompleteCallBack entry

  TpDfPeiExit                                = 0xADE0, ///< DF PEI exit
  TpDfDxeExit                                = 0xADE5, ///< DF DXE exit
  TpDfReadyToBootExit                        = 0xADE6, ///< DF Ready to Boot exit
  TpDfNbioSmuServicesPpiCallbackExit         = 0xADE7, ///< DF NbioSmuServicesPpiCallback exit
  TpDfNbioSmuServicesProtocolCallbackExit    = 0xADE8, ///< DF NbioSmuServicesProtocolCallback exit
  TpFabricSmmExit                            = 0xADE9, ///< DF SMM exit
  TpFabricPciEnumerationCallbackExit         = 0xADEA, ///< DF FabricPciEnumerationCompleteCallBack exit

  // [0xAEXX] Reserved
  // [0xAFXX] assigned for AGESA FCH Module
  /// @todo Remove unused FCH PCs
  TpFchInitResetDispatching                  = 0xAF01, ///< FCH InitReset dispatch point
  TpFchInitEnvDispatching                    = 0xAF06, ///< FCH InitEnv dispatch point
  TpFchInitMidDispatching                    = 0xAF07, ///< FCH InitMid dispatch point
  TpFchInitLateDispatching                   = 0xAF08, ///< FCH InitLate dispatch point
  TpFchInitS3EarlyDispatching                = 0xAF0B, ///< FCH InitS3Early dispatch point
  TpFchInitS3LateDispatching                 = 0xAF0C, ///< FCH InitS3Late dispatch point
  TpFchInitS3EarlyDispatchingEnd             = 0xAF0D, ///< FCH InitS3Early dispatch finished
  TpFchInitS3LateDispatchingEnd              = 0xAF0E, ///< FCH InitS3Late dispatch finished
  TpFchPeiEntry                              = 0xAF10, ///< FCH Pei Entry
  TpFchPeiExit                               = 0xAF11, ///< FCH Pei Exit
  TpFchMultiFchPeiEntry                      = 0xAF12, ///< FCH MultiFch Pei Entry
  TpFchMultiFchPeiExit                       = 0xAF13, ///< FCH MultiFch Pei Exit
  TpFchDxeEntry                              = 0xAF14, ///< FCH Dxe Entry
  TpFchDxeExit                               = 0xAF15, ///< FCH Dxe Exit
  TpFchMultiFchDxeEntry                      = 0xAF16, ///< FCH MultiFch Dxe Entry
  TpFchMultiFchDxeExit                       = 0xAF17, ///< FCH MultiFch Dxe Exit
  TpFchSmmEntry                              = 0xAF18, ///< FCH Smm Entry
  TpFchSmmExit                               = 0xAF19, ///< FCH Smm Exit
  TpFchSmmDispatcherEntry                    = 0xAF20, ///< FCH Smm Dispatcher Entry
  TpFchSmmDispatcherExit                     = 0xAF21, ///< FCH Smm Dispatcher Exit
  TpFchInitResetHwAcpi                       = 0xAF40, ///< FCH InitReset HwAcpi
  TpFchInitResetAb                           = 0xAF41, ///< FCH InitReset AB Link
  TpFchInitResetLpc                          = 0xAF42, ///< FCH InitReset LPC
  TpFchInitResetSpi                          = 0xAF43, ///< FCH InitReset SPI
  TpFchInitResetEspi                         = 0xAF44, ///< FCH InitReset eSPI
  TpFchInitResetSd                           = 0xAF45, ///< FCH InitReset SD
  TpFchInitResetEmmc                         = 0xAF46, ///< FCH InitReset eMMC
  TpFchInitResetSata                         = 0xAF47, ///< FCH InitReset SATA
  TpFchInitResetUsb                          = 0xAF48, ///< FCH InitReset USB
  TpFchInitResetXgbe                         = 0xAF49, ///< FCH InitReset xGbE
  TpFchInitResetUsbReady                     = 0xAF4A, ///< FCH InitReset USB Ready
  TpFchInitResetUsb4Ready                    = 0xAF4B, ///< FCH InitReset USB4 Ready
  TpFchInitResetUsb4                         = 0xAF4E, ///< FCH InitReset USB4
  TpFchInitResetHwAcpiP                      = 0xAF4F, ///< FCH InitReset HwAcpiP
  TpFchInitEnvHwAcpi                         = 0xAF50, ///< FCH InitEnv HwAcpi
  TpFchInitEnvAb                             = 0xAF51, ///< FCH InitEnv AB Link
  TpFchInitEnvLpc                            = 0xAF52, ///< FCH InitEnv LPC
  TpFchInitEnvSpi                            = 0xAF53, ///< FCH InitEnv SPI
  TpFchInitEnvEspi                           = 0xAF54, ///< FCH InitEnv eSPI
  TpFchInitEnvSd                             = 0xAF55, ///< FCH InitEnv SD
  TpFchInitEnvEmmc                           = 0xAF56, ///< FCH InitEnv eMMC
  TpFchInitEnvSata                           = 0xAF57, ///< FCH InitEnv SATA
  TpFchInitEnvUsb                            = 0xAF58, ///< FCH InitEnv USB
  TpFchInitEnvXgbe                           = 0xAF59, ///< FCH InitEnv xGbE
  TpFchInitEnvUsb4                           = 0xAF5E, ///< FCH InitEnv USB4
  TpFchInitEnvHwAcpiP                        = 0xAF5F, ///< FCH InitEnv HwAcpiP
  TpFchInitMidHwAcpi                         = 0xAF60, ///< FCH InitMid HwAcpi
  TpFchInitMidAb                             = 0xAF61, ///< FCH InitMid AB Link
  TpFchInitMidLpc                            = 0xAF62, ///< FCH InitMid LPC
  TpFchInitMidSpi                            = 0xAF63, ///< FCH InitMid SPI
  TpFchInitMidEspi                           = 0xAF64, ///< FCH InitMid eSPI
  TpFchInitMidSd                             = 0xAF65, ///< FCH InitMid SD
  TpFchInitMidEmmc                           = 0xAF66, ///< FCH InitMid eMMC
  TpFchInitMidSata                           = 0xAF67, ///< FCH InitMid SATA
  TpFchInitMidUsb                            = 0xAF68, ///< FCH InitMid USB
  TpFchInitMidXgbe                           = 0xAF69, ///< FCH InitMid xGbE
  TpFchInitMidUsb4                           = 0xAF6E, ///< FCH InitMid USB4
  TpFchInitLateHwAcpi                        = 0xAF70, ///< FCH InitLate HwAcpi
  TpFchInitLateAb                            = 0xAF71, ///< FCH InitLate AB Link
  TpFchInitLateLpc                           = 0xAF72, ///< FCH InitLate LPC
  TpFchInitLateSpi                           = 0xAF73, ///< FCH InitLate SPI
  TpFchInitLateEspi                          = 0xAF74, ///< FCH InitLate eSPI
  TpFchInitLateSd                            = 0xAF75, ///< FCH InitLate SD
  TpFchInitLateEmmc                          = 0xAF76, ///< FCH InitLate eMMC
  TpFchInitLateSata                          = 0xAF77, ///< FCH InitLate SATA
  TpFchInitLateUsb                           = 0xAF78, ///< FCH InitLate USB
  TpFchInitLateXgbe                          = 0xAF79, ///< FCH InitLate xGbE
  TpFchPTLoadFWEntry                         = 0xAF7A, ///< FCH PT load FW Entry
  TpFchPTLoadFWExit                          = 0xAF7B, ///< FCH PT load FW Exit
  TpFchPTPlusLoadFWEntry                     = 0xAF7C, ///< FCH PT_Plus load FW Entry
  TpFchPTPlusLoadFWExit                      = 0xAF7D, ///< FCH PT_Plus load FW Exit
  TpFchInitLateUsb4                          = 0xAF7E, ///< FCH InitLate USB4
  TpFchPTStartToLoadFW                       = 0xAF7F, ///< FCH PT Start to load FW
  TpFchDeviceEnterDxStatus                   = 0xAF80, ///< FCH Device Enter D x Status 0xB000AFxx [4:0]:device ID; [6:5] 0:D0 3:D3; [7]==1 //PLAT-27512
                                                       ///< device ID
                                                       ///< I2C0, 5
                                                       ///< I2C1, 6
                                                       ///< I2C2, 7
                                                       ///< I2C3, 8
                                                       ///< I2C4, 9
                                                       ///< I2C5, 10
                                                       ///< UART0,11
                                                       ///< UART1,12
                                                       ///< SATA, 15
                                                       ///< UART2,16
                                                       ///< UART3,26
                                                       ///< ESPI, 27
                                                       ///< EMMC, 28
  TpFchPTLoadFWDelay1Sec                     = 0xAF81, ///< FCH PT load FW Delay 1 sec
  TpFchPTLoadFWDelay2Sec                     = 0xAF82, ///< FCH PT load FW Delay 2 sec
  TpFchPTLoadFWDelay3Sec                     = 0xAF83, ///< FCH PT load FW Delay 3 sec
  TpFchPTLoadFWDelay4Sec                     = 0xAF84, ///< FCH PT load FW Delay 4 sec
  TpFchPTLoadFWDelay5Sec                     = 0xAF85, ///< FCH PT load FW Delay 5 sec
  TpFchPTLoadFWDelay6Sec                     = 0xAF86, ///< FCH PT load FW Delay 6 sec
  TpFchPTLoadFWDelay7Sec                     = 0xAF87, ///< FCH PT load FW Delay 7 sec
  TpFchPTLoadFWDelay8Sec                     = 0xAF88, ///< FCH PT load FW Delay 8 sec
  TpFchPTLoadFWDelay9Sec                     = 0xAF89, ///< FCH PT load FW Delay 9 sec
  TpFchPTLoadFWDelay10Sec                    = 0xAF8A, ///< FCH PT load FW Delay 10 sec
  TpFchSecPTStartToLoadFW                    = 0xAF8B, ///< FCH Secondary PT Start to load FW
  TpFchTurnerLoadFWEntry                     = 0xAF90, ///< FCH Turner Load FW Entry
  TpFchTurnerLoadFWExit                      = 0xAF91, ///< FCH Turner Load FW Exit
  TpFchTurnerStartToLoadFW                   = 0xAF92, ///< FCH Turner Start to Load FW
  TpBixbyPeiEntry                            = 0xAFB0, ///< Bixby Pei Entry
  TpBixbyPeiExit                             = 0xAFB1, ///< Bixby Pei Exit
  TpBixbyDxeEntry                            = 0xAFB2, ///< Bixby Dxe Entry
  TpBixbyDxeExit                             = 0xAFB3, ///< Bixby Dxe Exit
  TpBixbySmmEntry                            = 0xAFB4, ///< Bixby Smm Entry
  TpBixbySmmExit                             = 0xAFB5, ///< Bixby Smm Exit
  TpBixbyInitResetDispatching                = 0xAFB6, ///< Bixby InitReset dispatch point
  TpBixbyInitMidDispatching                  = 0xAFB7, ///< Bixby InitMid dispatch point
  TpBixbyInitEnvDispatching                  = 0xAFB8, ///< Bixby InitEnv dispatch point
  TpBixbyInitLateDispatching                 = 0xAFB9, ///< Bixby InitLate dispatch point
  TpBixbyInitS3EarlyDispatching              = 0xAFBA, ///< Bixby InitS3Early dispatch point
  TpBixbyInitS3LateDispatching               = 0xAFBB, ///< Bixby InitS3Late dispatch point
  TpBixbyInitS3EarlyDispatchingEnd           = 0xAFBC, ///< Bixby InitS3Early dispatch finished
  TpBixbyInitS3LateDispatchingEnd            = 0xAFBD, ///< Bixby InitS3Late dispatch finished
  TpBixbyInitResetSataEntry                  = 0xAFBE, ///< Bixby InitReset SATA Entry
  TpBixbyInitResetSataExit                   = 0xAFBF, ///< Bixby InitReset SATA Exit
  TpBixbyInitMidSataEntry                    = 0xAFC0, ///< Bixby InitMid SATA Entry
  TpBixbyInitMidSataExit                     = 0xAFC1, ///< Bixby InitMid SATA Exit
  TpBixbyInitEnvSataEntry                    = 0xAFC2, ///< Bixby InitEnv SATA Entry
  TpBixbyInitEnvSataExit                     = 0xAFC3, ///< Bixby InitEnv SATA Exit
  TpBixbyInitLateSataEntry                   = 0xAFC4, ///< Bixby InitLate SATA Entry
  TpBixbyInitLateSataExit                    = 0xAFC5, ///< Bixby InitLate SATA Exit
  TpBixbyInitResetUsbEntry                   = 0xAFC6, ///< Bixby InitReset USB Entry
  TpBixbyInitResetUsbExit                    = 0xAFC7, ///< Bixby InitReset USB Exit
  TpBixbyInitMidUsbEntry                     = 0xAFC8, ///< Bixby InitMid USB Entry
  TpBixbyInitMidUsbExit                      = 0xAFC9, ///< Bixby InitMid USB Exit
  TpBixbyInitEnvUsbEntry                     = 0xAFCA, ///< Bixby InitEnv USB Entry
  TpBixbyInitEnvUsbExit                      = 0xAFCB, ///< Bixby InitEnv USB Exit
  TpBixbyInitLateUsbEntry                    = 0xAFCC, ///< Bixby InitLate USB Entry
  TpBixbyInitLateUsbExit                     = 0xAFCD, ///< Bixby InitLate USB Exit
  TpBixbyInitEnvHwAcpiP                      = 0xAFCE, ///< Bixby InitEnv HwAcpiP
  TpBixbyInitResetHwAcpiP                    = 0xAFCF, ///< Bixby InitReset HwAcpiP
  EndFchTestPoints                           = 0xAFFF, ///< End of TP range for FCH

  EndAgesaPcs                                = 0xFFFF, ///< Last defined AGESA PCs
} AGESA_POST_CODE;

#endif // _AGESA_POST_CODE_H_


