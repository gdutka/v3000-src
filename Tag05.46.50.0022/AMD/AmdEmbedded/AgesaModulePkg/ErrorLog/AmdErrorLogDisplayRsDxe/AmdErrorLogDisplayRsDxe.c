/*
****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/


#include <Filecode.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/AmdErrorLogLib.h>
#include <Protocol/AmdErrorLogProtocol.h>
#include <Protocol/AmdErrorLogServiceProtocol.h>
#include "AmdErrorLogDisplayRsDxe.h"
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/AmdSocBaseLib.h>
#include "Porting.h"
#include <Addendum/Apcb/Inc/RS/APOB.h>

#define FILECODE ERRORLOG_AMDERRORLOGDISPLAYRSDXE_AMDERRORLOGDISPLAYRSDXE_FILECODE

#define ABL_MEM_PMU_TRAIN_ERROR 0x4001
#define ABL_MEM_AGESA_MEMORY_TEST_ERROR 0x4003
#define ABL_MEM_ERROR_MIXED_ECC_AND_NON_ECC_DIMM_IN_SYSTEM   0x4020
#define ABL_MEM_ERROR_LRDIMM_MIXMFG 0x4033
#define ABL_CCD_BIST_FAILURE 0x4065
#define ABL_MEM_MEMORY_HEALING_BIST_ERROR 0x4067

extern  EFI_BOOT_SERVICES       *gBS;

//
// Driver Global Data
//
STATIC AMD_ERROR_BUFFER *ErrorLogBuffer;

typedef struct {
  UINT32    errorCode;
  CHAR16    *string;
} PMU_ERRORCODE_STRING;

STATIC CONST PMU_ERRORCODE_STRING String [] = {
  {0x000f0000, L"PMU: Error: start address of ACSM MPR read sequence must be aligned on even acsm addr position\n"},
  {0x00220001, L"PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n"},
  {0x00240001, L"PMU: Error: CS%d failed to find a DFIMRL setting that worked for channel A bytes during MaxRdLat training\n"},
  {0x00260001, L"PMU: Error: CS%d failed to find a DFIMRL setting that worked for channel B bytes during MaxRdLat training\n"},
  {0x00280000, L"PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"},
  {0x00290000, L"PMU: Error: No passing DFIMRL value found for any chip select for channel A during MaxRdLat training\n"},
  {0x002a0000, L"PMU: Error: No passing DFIMRL value found for any chip select for channel B during MaxRdLat training\n"},
  {0x00320003, L"PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"},
  {0x003a0001, L"PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n"},
  {0x003f0003, L"PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n"},
  {0x00420002, L"PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n"},
  {0x00460003, L"PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n"},
  {0x004b0001, L"PMU: Error: MRD/MWD training is not converging on rank %d after trying all possible RCD CmdDly\n"},
  {0x00540003, L"PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n"},
  {0x00600003, L"PMU: Error: D5 rd2D no passing region for rank %d, db %d, lane %d\n"},
  {0x00610002, L"PMU: Error: Wrong PBDly seed 0x%04x results in too small RxClkDly 0x%04x\n"},
  {0x006b0002, L"PMU: Error: tg %d nib %d RxClkDly had no passing region\n"},
  {0x00720002, L"PMU: Error: db %d lane %d vrefDAC had no passing region\n"},
  {0x00820002, L"PMU: Error: dbyte %d lane %d TxDqDly had no passing region\n"},
  {0x008c0001, L"PMU: Error: nib %d vrefDQ had no passing region\n"},
  {0x00a40003, L"PMU: Error: Dbyte %d nibble %d MRD passing region is too small (width = %d)\n"},
  {0x00a50003, L"PMU: Error: Dbyte %d nibble %d MWD passing region is too small (width = %d)\n"},
  {0x00b00002, L"PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n"},
  {0x00ba0002, L"PMU: Error: dbyte %d lane %d failed read deskew\n"},
  {0x00c60001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n"},
  {0x00c70001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n"},
  {0x00c80001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n"},
  {0x00c90000, L"PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"},
  {0x00d30000, L"PMU: Error: start address of ACSM RxEn sequence must be aligned on even acsm addr position\n"},
  {0x00d80001, L"PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n"},
  {0x00d80001, L"PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n"},
  {0x00ea0002, L"PMU: Error: Failed MREP for nib %d with %d one\n"},
  {0x00fc0003, L"PMU: Error: CSn %d Channel %d CS train passing region is too small (width = %d)\n"},
  {0x01050004, L"PMU: Error: CSn %d Channel %d Signal A%d CA train passing region is too small (width = %d)\n"},
  {0x010c0003, L"PMU: Error: CSn %d Channel %d VrefCS train passing region is too small (width = %d)\n"},
  {0x01130004, L"PMU: Error: CSn %d Channel %d Signal CA%d VrefCA train passing region is too small (width = %d)\n"},
  {0x01170003, L"PMU: Error: RCD CA DFE training failed for CS 0x%x Channel %d CA%d (no open eye was found)\n"},
  {0x01190003, L"PMU: Error: RCD CA DFE training could not calculate trained VrefCA center for CS 0x%x Channel %d CA%d\n"},
  {0x01300003, L"PMU5: Error: CSn %d Channel %d RCD CS train passing region is too small (width = %d), assume 0 as center\n"},
  {0x01320000, L"PMU: Error: start address of ACSM RCD_CA sequence must be aligned on even acsm addr position\n"},
  {0x01390002, L"PMU5: Error: CSn %d Channel %d RCD CA final delay too large, assume 0\n"},
  {0x01450002, L"PMU: Error: specified bank (BG:%d BA:%d) is not available for PPR\n"},
  {0x01490000, L"PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"},
  {0x01710000, L"PMU: Error: EnabledDQsChA must be > 0\n"},
  {0x01720000, L"PMU: Error: EnabledDQsChB must be > 0\n"},
  {0x01810000, L"PMU: Error: No dbiDisable without d4\n"},
  {0x01920000, L"PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"},
  {0x01e60000, L"PMU: Error: start address of ACSM WR/RD activate sequence must be aligned on even acsm addr position\n"},
  {0x01e80000, L"PMU: Error: start address of ACSM WR/RD program sequence must be aligned on even acsm addr position\n"},
  {0x01ea0000, L"PMU: Error: start address of ACSM DM sequence must be aligned on even acsm addr position\n"},
  {0x01eb0003, L"PMU: Error: Firmware was not able to detect swizzle setting for TG%d Dbyte%d DQ%d\n"},
  {0x01ec0005, L"PMU: Error: Wrong DqLnSelTg setting for TG%d Dbyte%d DQ%d: expected %d found %d\n"},
  {0x01ed0005, L"PMU: Error: Wrong DqLnSelTg setting for TG%d Dbyte%d: DQ%d and DQ%d have the same value (%d)\n"},
  {0x01ee0003, L"PMU: Error: Firmware was not able to detect swizzle setting for TG%d Dbyte%d DQ%d\n"},
  {0x01f80000, L"PMU: Error: internal error in d5_detect_dq_swizzle() cannot find unused mapping\n"},
  {0x01ff0000, L"PMU: Error: start address of ACSM WL sequence must be aligned on even acsm addr position\n"},
  {0x020b0001, L"PMU: Error: Failed DWL for nib %d\n"},
  {0x020e0002, L"PMU: Error: nib %d external WL %d underflow\n"},
  {0x02100000, L"PMU: Error: internal DWL error ACSM sequences overlap\n"},
  {0x02140000, L"PMU: Error: Some nibble didn't converge during internal WL\n"},
  {0x02160002, L"PMU: Error: nib %d internal WL %d overflow\n"},
  {0x021b0002, L"PMU: Error: nib %d external WL %d underflow\n"},
  {0x02210000, L"PMU: Error: Some nibble didn't converge during internal WL\n"},
  {0x02250002, L"PMU: Error: nib %d internal WL %d overflow\n"},
  {0x022a0002, L"PMU: Error: nib %d external WL %d overflow\n"},
  {0x022b0002, L"PMU: Error: nib %d external WL %d underflow\n"},
  {0x022d0002, L"PMU: Error: nib %d external WL %d overflow\n"},
  {0x022e0002, L"PMU: Error: nib %d external WL %d underflow\n"},
  {0x02440000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x02490001, L"PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n"},
  {0x02510002, L"PMU: Error: Failed DWL for nib %d with %d one\n"},
  {0x04060001, L"PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n"},
  {0x04070001, L"PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n"},
  {0x04090000, L"PMU: Error - acsm_set_lptr() invoked with wrong ptr value...\n"},
  {0x040a0000, L"PMU: Error - acsm_set_eptr() invoked with wrong ptr value...\n"},
  {0x040b0000, L"PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"},
  {0x04100004, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= %d, and %d, resp. CL=%d CWL=%d\n"},
  {0x04110002, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n"},
  {0x04130001, L"PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n"}
};


/*---------------------------------------------------------------------------------------*/
/**
 *
 * This function logs AGESA Errors into the Error log.
 *
 * It will put the information in a circular buffer consisting of 128 such log
 * entries. If the buffer gets full, then the next Error log entry will be written
 * over the oldest Error log entry.
 *
 * @param[in]   PeiServices, ClearBuffer
 * @param[out]  ErrorLogDataPtr
 *
 */
EFI_STATUS
EFIAPI
AmdAquireErrorLogWithFlagDxe (
  IN       DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *This,
  OUT      ERROR_LOG_DATA_STRUCT *ErrorLogDataPtr,
  IN       BOOLEAN ClearBuffer
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;

  AquireErrorLog (ErrorLogBuffer, ErrorLogDataPtr, ClearBuffer);
  //Reset Error Log buffer
  if (ClearBuffer) {
    ErrorLogBufferInit(ErrorLogBuffer);
  }

  return Status;
}

//                                     SoC channel: 0, 1, 2, 3, 4, 5, 6,  7,  8, 9, 10, 11
STATIC CONST UINT8  RsChannel2BoardChannelXlat[] = {2, 4, 5, 0, 1, 3, 8, 10, 11, 6,  7,  9};

VOID
DisplayMemPmuTrainingFailure (
  )
{
  EFI_HOB_GUID_TYPE   *HobAddr;
  PMU_TRAINING_FAILURE_INFO_ENTRY   *PmuFailure;
  UINT16    stringIndex;
  UINT16    stringLength;
  UINT8     BoardChannelId;
  UINT16    *errorCodeNotFound[] = {L"  --> ErrorCode not found in the message string table\n\n"};
  UINT16    i;


  HobAddr = GetFirstGuidHob (&gAmdMemPmuTrainingFailureHobGuid);

  PmuFailure = (PMU_TRAINING_FAILURE_INFO_ENTRY *) GET_GUID_HOB_DATA (HobAddr);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\nPMU Training Failure HOB address = %x\n", PmuFailure);
  for (i = 0; i < 80; i++) {  // PMU_TRAINING_FAILURE_INFO_ENTRY  FailureEntry[40], 40 entries per socket, 80 entries for two sockets
    if (0 == PmuFailure->ErrorCode) {  // ErrorCode == 0 indicating end of failure entry
      break;
    }

    BoardChannelId = RsChannel2BoardChannelXlat[PmuFailure->UmcId];
    Print (L"\n");
    Print (L"Socket %d, Board Channel %d (SoC Channel %d), ", PmuFailure->SocketId, BoardChannelId, PmuFailure->UmcId);
    Print (L"ErrorCode = %x, %x, %x, %x, %x\n", PmuFailure->ErrorCode, PmuFailure->Data[0], PmuFailure->Data[1], PmuFailure->Data[2], PmuFailure->Data[3]);
    stringLength = sizeof (String) / sizeof (String[0]);
    for (stringIndex = 0; stringIndex < stringLength; stringIndex++) {
      if (PmuFailure->ErrorCode == String[stringIndex].errorCode) {
        break;
      }
    }
    if (stringIndex == stringLength) {  // ErrorCode not found
      Print (errorCodeNotFound[0]);
    } else {
      Print (L"  --> ");
      Print (String[stringIndex].string, PmuFailure->Data[0], PmuFailure->Data[1], PmuFailure->Data[2], PmuFailure->Data[3]);
      Print (L"\n");
    }

    PmuFailure++;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This function aquires gAmdErrorLogServiceProtocolGuid and display any error of
 * ABL_MEM_PMU_TRAIN_ERROR & ABL_MEM_AGESA_MEMORY_TEST_ERROR to serial console & video
 * screen
 *
 */
VOID
DisplayAmdErrorLogCallBackRs (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *ErrorLogServices;
  ERROR_LOG_DATA_STRUCT   *ErrorLogDataPtr;
  UINT32                  i;
  UINT32                  Ccd;
  EFI_STATUS              Status;
  BOOLEAN                 anyError;
  UINT8                   SocketId;
  UINT8                   SocChannelId;
  UINT8                   BoardChannelId;
  EFI_HOB_GUID_TYPE       *HobAddr;
  PMU_TRAINING_FAILURE_INFO_ENTRY   *PmuFailure;
  UINT16                  stageCompletionCode;

  Status = gBS->LocateProtocol(&gAmdErrorLogServiceProtocolGuid, NULL, (VOID **)&ErrorLogServices);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate gAmdErrorLogServiceProtocolGuid fail at DisplayAmdErrorLogCallBackRs\n");
    return;
  }
  if (!FeaturePcdGet (PcdAmdIdsDebugPrintEnable) && !FeaturePcdGet (PcdAgesaPrintEventLogToConsole)) {
    return;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (ERROR_LOG_DATA_STRUCT), (VOID **)&ErrorLogDataPtr);
  if (EFI_ERROR (Status)) {
    return;
  }
  ZeroMem (ErrorLogDataPtr, sizeof (ERROR_LOG_DATA_STRUCT));

  ErrorLogServices->AmdAquireErrorLogWithFlagDxe (ErrorLogServices, ErrorLogDataPtr, FALSE);

  DisplayMemPmuTrainingFailure ();  // For Milan only
  //DisplayMemMbistDataEyeMargin ();

  // ABL_MEM_PMU_TRAIN_ERROR
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_MEM_PMU_TRAIN_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n********************************************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_MEM_PMU_TRAIN_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        SocketId       = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId   = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xf;
        BoardChannelId = RsChannel2BoardChannelXlat[SocChannelId];
        Print (L"* Memory PMU Training error at Socket %d, Board Channel %d (SoC Channel %d), ", SocketId, BoardChannelId, SocChannelId);

        // ABL pass failure info to DataParam1 as:
        // [7:0] = Socket #
        // [15:8] = Channel #
        // [19:16] = Bitmask of failed CS
        if (((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 3) != 0) {  // Isolate DIMM 0
          Print (L"DIMM 0 ");
          switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 3) {
            case 1:
              Print (L"Rank 0");
              break;
            case 2:
              Print (L"Rank 1");
              break;
            default:
              Print (L"Rank 0 & 1");
              break;
          }
        }
        if (((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 18) & 3) != 0) {  // Isolate DIMM 1
          Print (L"DIMM 1 ");
          switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 18) & 3) {
            case 1:
              Print (L"Rank 0\n");
              break;
            case 2:
              Print (L"Rank 1\n");
              break;
            default:
              Print (L"Rank 0 & 1\n");
              break;
          }
        }
        Print (L"\n");
      }
    }

    // Display Stage Completion info
    HobAddr = GetFirstGuidHob (&gAmdMemPmuTrainingFailureHobGuid);
    PmuFailure = (PMU_TRAINING_FAILURE_INFO_ENTRY *) GET_GUID_HOB_DATA (HobAddr);
    for (i = 0; i < 80; i++) {  // PMU_TRAINING_FAILURE_INFO_ENTRY  FailureEntry[40], 40 entries per socket, 80 entries for two sockets
      if (0 == PmuFailure->ErrorCode) {  // ErrorCode == 0 indicating end of failure entry
        break;
      }

      BoardChannelId = RsChannel2BoardChannelXlat[PmuFailure->UmcId];
      Print (L"\n");
      Print (L"Socket %d, Board Channel %d (SoC Channel %d), %dD training, following training stage completed\n",
             PmuFailure->SocketId, BoardChannelId, PmuFailure->UmcId, (PmuFailure->Train1d2d + 1));

      // Loop through all PmuFailure->stageCompletion, print its text message
      // Case 11 & 15 are useless, insert MJ_MSG_QCSQCA_TRAIN & MJ_MSG_MPR_RD_TRAIN in, must match to ABL Memory\Phy\Syn\DDR54\MemPmu.c MemPmuRunUntil
      for (stageCompletionCode = 0; stageCompletionCode < 16; stageCompletionCode ++) {
        if (((PmuFailure->stageCompletion >> stageCompletionCode) & 1) == 1) {
          switch (stageCompletionCode) {
            case 0:
              Print (L"\tEnd of initialization\n");
              break;
            case 1:
              Print (L"\tEnd of fine write leveling\n");
              break;
            case 2:
              Print (L"\tEnd of read enable training\n");
              break;
            case 3:
              Print (L"\tEnd of read delay center optimization\n");
              break;
            case 4:
              Print (L"\tEnd of write delay center optimization\n");
              break;
            case 5:
              Print (L"\tEnd of 2D read delay/voltage center optimization\n");
              break;
            case 6:
              Print (L"\tEnd of 2D write delay /voltage center optimization\n");
              break;
            case 7:
              Print (L"\tTraining has run successfully\n");
              break;
            case 9:
              Print (L"\tEnd of max read latency training\n");
              break;
            case 10:
              Print (L"\tEnd of read dq deskew training\n");
              break;
            case 11:  // Insert MJ_MSG_QCSQCA_TRAIN 0x1D here
              Print (L"\tEnd of RCD QCS/QCA training\n");
              break;
            case 12:
              Print (L"\tEnd of all DB triaining\n");
              break;
            case 13:
              Print (L"\tEnd of CA training\n");
              break;
            case 14:
              Print (L"\tEnd of Write leveling coarse delay\n");
              break;
            case 15:  // Insert MJ_MSG_MPR_RD_TRAIN 0xFD here
              Print (L"\tEnd of MPR read delay center optimization\n");
              break;
            default:
              Print (L"\tUnknown PMU Message\n");
              break;
          }
        }
      }

      PmuFailure++;
    }

    Print (L"********************************************************************************\n");
  }

  // ABL_MEM_AGESA_MEMORY_TEST_ERROR
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_MEM_AGESA_MEMORY_TEST_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n******************************************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_MEM_AGESA_MEMORY_TEST_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        SocketId       = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId   = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xf;
        BoardChannelId = RsChannel2BoardChannelXlat[SocChannelId];
        Print (L"* Agesa Memory Test error at Socket %d, Board Channel %d (SoC Channel %d), ", SocketId, BoardChannelId, SocChannelId);

        switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 3) {    // Isolate DIMM #
        case 1:
          Print (L"DIMM 0\n");
          break;
        case 2:
          Print (L"DIMM 1\n");
          break;
        case 3:
          Print (L"DIMM 0 & DIMM 1\n");
        }
      }
    }
    Print (L"******************************************************************************\n");
  }

  // ABL_MEM_ERROR_MIXED_ECC_AND_NON_ECC_DIMM_IN_SYSTEM
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_MEM_ERROR_MIXED_ECC_AND_NON_ECC_DIMM_IN_SYSTEM  == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n***********************************************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_MEM_ERROR_MIXED_ECC_AND_NON_ECC_DIMM_IN_SYSTEM == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        SocketId       = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId   = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xf;
        BoardChannelId = RsChannel2BoardChannelXlat[SocChannelId];
        Print (L"* Mixed ECC and non-ECC DIMM in system at Socket %d, Board Channel %d (SoC Channel %d)\n", SocketId, BoardChannelId, SocChannelId);
      }
    }
    Print (L"***********************************************************************************\n");
  }

  // ABL_MEM_ERROR_LRDIMM_MIXMFG
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_MEM_ERROR_LRDIMM_MIXMFG  == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n**********************************************************************************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_MEM_ERROR_LRDIMM_MIXMFG == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        SocketId       = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId   = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xf;
        BoardChannelId = RsChannel2BoardChannelXlat[SocChannelId];
        Print (L"* Mix certain vendor LRDIMM with other vendor LRDIMM in the same channel, at Socket %d, Board Channel %d (SoC Channel %d)\n",
               SocketId, BoardChannelId, SocChannelId);
      }
    }
    Print (L"**********************************************************************************************************************\n");
  }

  // ABL_CCD_BIST_FAILURE
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_CCD_BIST_FAILURE == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n**********************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_CCD_BIST_FAILURE == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        for(Ccd = 0; Ccd < CCD_MAX_CCDS_PER_DIE; Ccd++){
          if(ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & (1 << Ccd)){
            Print (L"* CCD BIST error at Socket %d Die %d CCD %d\n", (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 0xFF, (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xFF, Ccd);
          }
        }
      }
    }
    Print (L"**********************************************************\n");
  }

  // ABL_MEM_MEMORY_HEALING_BIST_ERROR
  anyError = FALSE;
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if (ABL_MEM_MEMORY_HEALING_BIST_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
      anyError = TRUE;
      break;
    }
  }

  if (anyError) {
    Print (L"\n******************************************************************************\n");
    for (i = 0; i < ErrorLogDataPtr->Count; i++) {
      if (ABL_MEM_MEMORY_HEALING_BIST_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) {
        SocketId       = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId   = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xf;
        BoardChannelId = RsChannel2BoardChannelXlat[SocChannelId];
        Print (L"* Memory Healing BIST error at Socket %d, Board Channel %d (SoC Channel %d), ", SocketId, BoardChannelId, SocChannelId);

        switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 3) {    // Isolate DIMM #
        case 1:
          Print (L"DIMM 0\n");
          break;
        case 2:
          Print (L"DIMM 1\n");
          break;
        case 3:
          Print (L"DIMM 0 & DIMM 1\n");
        }
      }
    }
    Print (L"******************************************************************************\n");
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  Status = gBS->FreePool (ErrorLogDataPtr);
}

/*********************************************************************************
 * Name: AmdErrorLogDisplayRsDxeInit
 *
 * Description
 *   Entry point of the AMD Error Log DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
AmdErrorLogDisplayRsDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  //EFI_EVENT           DisplayAmdErrorLogEvent;
  //VOID                *Registration;

  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Entry*********************\n"));

  DisplayAmdErrorLogCallBackRs(NULL, NULL);
/*
  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_NOTIFY,
      DisplayAmdErrorLogCallBackRs,
      NULL,
      NULL,
      &DisplayAmdErrorLogEvent
      );

  Status = gBS->RegisterProtocolNotify (
      &gAmdNbioIommuProtocolGuid,
      DisplayAmdErrorLogEvent,
      &Registration
      );
*/
  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Exit*********************\n"));
  return (Status);
}

