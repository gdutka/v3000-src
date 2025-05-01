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
#include "AmdErrorLogDisplayGnDxe.h"
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/AmdSocBaseLib.h>
#include "Porting.h"
#include <Addendum/Apcb/Inc/GN/APOB.h>
#include <Guid/SSP/AmdMbistHob.h>
#include <Protocol/FabricTopologyServices2.h>

#define FILECODE ERRORLOG_AMDERRORLOGDISPLAYGNDXE_AMDERRORLOGDISPLAYGNDXE_FILECODE

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

// RDIMM PMU source: PLAT-57364 (PLAT-58083)
STATIC CONST PMU_ERRORCODE_STRING String1D [] = {
  {0x00040000, L"PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n"},
  {0x00110001, L"PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n"},
  {0x00130000, L"PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"},
  {0x00140003, L"PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"},
  {0x001c0003, L"PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"},
  {0x00240001, L"PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n"},
  {0x00280003, L"PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n"},
  {0x002b0002, L"PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n"},
  {0x00300003, L"PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n"},
  {0x00350001, L"PMU: Error: MRD/MWD training is not converging on rank %d after trying all possible RCD CmdDly\n"},
  {0x003d0003, L"PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n"},
  {0x00420002, L"PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n"},
  {0x004b0002, L"PMU: Error: dbyte %d lane %d failed read deskew\n"},
  {0x004e0000, L"PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n"},
  {0x00510001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n"},
  {0x00520001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n"},
  {0x00530001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n"},
  {0x00540001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n"},
  {0x00550001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4NV Type\n"},
  {0x00560001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n"},
  {0x00570000, L"PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"},
  {0x00730000, L"PMU: Error: RxEn training preamble not found\n"},
  {0x00750001, L"PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n"},
  {0x007e0002, L"PMU: Error: Failed MREP for nib %d with %d one\n"},
  {0x00ae0000, L"PMU: Error: CA Training Failed.\n"},
  {0x00be0000, L"PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"},
  {0x00dd0001, L"PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n"},
  {0x00de0001, L"PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n"},
  {0x00df0001, L"PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n"},
  {0x00e70001, L"PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n"},
  {0x00ef0000, L"PMU: Error: No dbiEnable with lp4\n"},
  {0x00f00000, L"PMU: Error: No dbiDisable with lp4\n"},
  {0x01000000, L"PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"},
  {0x01500001, L"PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n"},
  {0x01510001, L"PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n"},
  {0x01660000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x01700000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x01750001, L"PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n"},
  {0x017b0000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x01820002, L"PMU: Error: Failed DWL for nib %d with %d one\n"},
  {0x04000000, L"PMU: Error: Mailbox Buffer Overflowed.\n"},
  {0x04010000, L"PMU: Error: Mailbox Buffer Overflowed.\n"},
  {0x04070001, L"PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n"},
  {0x04080001, L"PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n"},
  {0x040a0000, L"PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"},
  {0x040e0002, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n"},
  {0x040f0002, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n"},
  {0x04110001, L"PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n"},
  {0x04150001, L"PMU: Error: Boot clock divider setting of %d is too small\n"},
  {0x04180000, L"PMU: Error: Delay too large in slomo\n"}
};

STATIC CONST PMU_ERRORCODE_STRING String2D [] = {
  {0x00080001, L"PMU: Error: Illegal timing group number ,%d, in getPtrVrefDq\n"},
  {0x001b0002, L"PMU: Error: LP4 rank %d cannot be mapped on tg %d\n"},
  {0x00430000, L"PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"},
  {0x00470000, L"PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"},
  {0x00520000, L"PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"},
  {0x00570001, L"PMU: Error: getCompoundEye Called on lane%d eye with non-compatible centers\n"},
  {0x00750003, L"PMU: Error: Dbyte %d nibble %d's optimal rxClkDly of 0x%x is out of bounds\n"},
  {0x007b0003, L"PMU: Error: Dbyte %d lane %d's optimal txDqDly of 0x%x is out of bounds\n"},
  {0x008e0002, L"PMU: Error: LP4 rank %d cannot be mapped on tg %d\n"},
  {0x009c0000, L"PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n"},
  {0x00a90001, L"PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n"},
  {0x00ab0000, L"PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"},
  {0x00ac0003, L"PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"},
  {0x00b40003, L"PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n"},
  {0x00bc0001, L"PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n"},
  {0x00c00003, L"PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n"},
  {0x00c30002, L"PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n"},
  {0x00c80003, L"PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n"},
  {0x00cd0001, L"PMU: Error: MRD/MWD training is not converging on rank %d after trying all possible RCD CmdDly\n"},
  {0x00d50003, L"PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n"},
  {0x00da0002, L"PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n"},
  {0x00e30002, L"PMU: Error: dbyte %d lane %d failed read deskew\n"},
  {0x00e60000, L"PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n"},
  {0x00e90001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n"},
  {0x00ea0001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n"},
  {0x00eb0001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n"},
  {0x00ec0001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n"},
  {0x00ed0001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4NV Type\n"},
  {0x00ee0001, L"PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n"},
  {0x00ef0000, L"PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"},
  {0x010b0000, L"PMU: Error: RxEn training preamble not found\n"},
  {0x010d0001, L"PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n"},
  {0x01160002, L"PMU: Error: Failed MREP for nib %d with %d one\n"},
  {0x01460000, L"PMU: Error: CA Training Failed.\n"},
  {0x01560000, L"PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"},
  {0x01750001, L"PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n"},
  {0x01760001, L"PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n"},
  {0x01770001, L"PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n"},
  {0x017f0001, L"PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n"},
  {0x01870000, L"PMU: Error: No dbiEnable with lp4\n"},
  {0x01880000, L"PMU: Error: No dbiDisable with lp4\n"},
  {0x01980000, L"PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"},
  {0x01e80001, L"PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n"},
  {0x01e90001, L"PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n"},
  {0x01fe0000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x02080000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x020d0001, L"PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n"},
  {0x02130000, L"PMU: Error: Failed write leveling coarse\n"},
  {0x021a0002, L"PMU: Error: Failed DWL for nib %d with %d one\n"},
  {0x04000000, L"PMU: Error: Mailbox Buffer Overflowed.\n"},
  {0x04010000, L"PMU: Error: Mailbox Buffer Overflowed.\n"},
  {0x04070001, L"PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n"},
  {0x04080001, L"PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n"},
  {0x040a0000, L"PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"},
  {0x040e0002, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n"},
  {0x040f0002, L"PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n"},
  {0x04110001, L"PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n"},
  {0x04150001, L"PMU: Error: Boot clock divider setting of %d is too small\n"},
  {0x04180000, L"PMU: Error: Delay too large in slomo\n"}
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

//                                         SoC channel: 0, 1, 2, 3, 4, 5, 6, 7
STATIC CONST UINT8   GnSp3Channel2BoardChannelXlat[] = {0, 1, 3, 2, 7, 6, 4, 5};

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

    BoardChannelId = GnSp3Channel2BoardChannelXlat[PmuFailure->UmcId];
    Print (L"\n");
    Print (L"Socket %d, Board Channel %d (SoC Channel %d), %dD training, attempt %d, ", PmuFailure->SocketId, BoardChannelId, PmuFailure->UmcId, (PmuFailure->Train1d2d + 1), PmuFailure->Train1dAttempt);
    Print (L"ErrorCode = %x, %x, %x, %x, %x\n", PmuFailure->ErrorCode, PmuFailure->Data[0], PmuFailure->Data[1], PmuFailure->Data[2], PmuFailure->Data[3]);
    if (0 == PmuFailure->Train1d2d) {  // 1D
      stringLength = sizeof (String1D) / sizeof (String1D[0]);
      for (stringIndex = 0; stringIndex < stringLength; stringIndex++) {
        if (PmuFailure->ErrorCode == String1D[stringIndex].errorCode) {
          break;
        }
      }
      if (stringIndex == stringLength) {  // ErrorCode not found
        Print (errorCodeNotFound[0]);
      } else {
        Print (L"  --> ");
        Print (String1D[stringIndex].string, PmuFailure->Data[0], PmuFailure->Data[1], PmuFailure->Data[2], PmuFailure->Data[3]);
        Print (L"\n");
      }

    } else {  // 2D
      stringLength = sizeof (String2D) / sizeof (String2D[0]);
      for (stringIndex = 0; stringIndex < stringLength; stringIndex++) {
        if (PmuFailure->ErrorCode == String2D[stringIndex].errorCode) {
          break;
        }
      }
      if (stringIndex == stringLength) {  // ErrorCode not found
        Print (errorCodeNotFound[0]);
      } else {
        Print (L"  --> ");
        Print (String2D[stringIndex].string, PmuFailure->Data[0], PmuFailure->Data[1], PmuFailure->Data[2], PmuFailure->Data[3]);
        Print (L"\n");
      }
    }

    PmuFailure++;
  }
}

VOID
DisplayMemMbistDataEyeMargin (
  )
{
  EFI_HOB_GUID_TYPE   *HobAddr;
  AMD_MEMORY_MBIST_HOB  *MbistResults;
  UINT8     BoardChannelId;
  UINT16    socket;
  UINT16    channel;
  UINT16    cs;
  UINT8     ReadDqPositive;
  UINT8     ReadDqNegative;
  UINT8     ReadVrefPositive;
  UINT8     ReadVrefNegative;
  UINT8     WriteDqPositive;
  UINT8     WriteDqNegative;
  UINT8     WriteVrefPositive;
  UINT8     WriteVrefNegative;
  BOOLEAN   MbistDataEyeMarginDataValid;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;
  UINTN     NumberOfInstalledProcessors;

  gBS->LocateProtocol(&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  FabricTopology->GetSystemInfo (FabricTopology, &NumberOfInstalledProcessors, NULL, NULL, NULL, NULL);

  HobAddr = GetFirstGuidHob (&gAmdMemoryMbistHobGuid);
  MbistResults = (AMD_MEMORY_MBIST_HOB *) GET_GUID_HOB_DATA (HobAddr);

  MbistDataEyeMarginDataValid = FALSE;
  for (socket = 0; socket < NumberOfInstalledProcessors; socket++) {
    for (channel = 0; channel < ABL_APOB_MAX_CHANNELS_PER_DIE; channel++) {
      for (cs = 0; cs < ABL_APOB_MAX_CS_PER_CHANNEL; cs++) {
        if (MbistResults->MbistDataEyeMargin[socket][0][channel][cs].IsDataEyeValid) {
          MbistDataEyeMarginDataValid = TRUE;
          break;
        }
      }
    }
  }

  if (MbistDataEyeMarginDataValid) {
    Print(L"\nMBIST Data Eye Margin Results\n");
    Print (L"\t          Channel              Rd DqDelay     Rd Vref      Wr DqDelay     Wr Vref\n");
    Print (L"\t Socket  Board (SoC)  ChipSel  left  right    +      -     left  right    +      -\n");
    Print (L"\t-----------------------------------------------------------------------------------\n");

    for (socket = 0; socket < NumberOfInstalledProcessors; socket++) {
      for (channel = 0; channel < ABL_APOB_MAX_CHANNELS_PER_DIE; channel++) {
        for (cs = 0; cs < ABL_APOB_MAX_CS_PER_CHANNEL; cs++) {
          ReadDqPositive    = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].ReadDqDelay.PositiveEdge;
          ReadDqNegative    = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].ReadDqDelay.NegativeEdge;
          ReadVrefPositive  = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].ReadVref.PositiveEdge;
          ReadVrefNegative  = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].ReadVref.NegativeEdge;
          WriteDqPositive   = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].WriteDqDelay.PositiveEdge;
          WriteDqNegative   = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].WriteDqDelay.NegativeEdge;
          WriteVrefPositive = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].WriteVref.PositiveEdge;
          WriteVrefNegative = MbistResults->MbistDataEyeMargin[socket][0][channel][cs].WriteVref.NegativeEdge;

          if (!MbistResults->MbistDataEyeMargin[socket][0][channel][cs].IsDataEyeValid) {
            continue;  // No data at this channel, cs
          }

          BoardChannelId = GnSp3Channel2BoardChannelXlat[channel];
          Print (L"\t    %d      %d   ( %d )     %d     -%02d    +%02d    +%02d    -%02d    -%02d    +%02d    +%02d    -%02d\n",
                 socket, BoardChannelId, channel, cs,
                 ReadDqNegative, ReadDqPositive, ReadVrefPositive, ReadVrefNegative,
                 WriteDqNegative, WriteDqPositive, WriteVrefPositive, WriteVrefNegative);
        }
      }
      Print (L"\n");
    }
    Print (L"\n");
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
DisplayAmdErrorLogCallBackGn (
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

  Status = gBS->LocateProtocol(&gAmdErrorLogServiceProtocolGuid, NULL, &ErrorLogServices);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate gAmdErrorLogServiceProtocolGuid fail at DisplayAmdErrorLogCallBackGn\n");
    return;
  }
  // PcdAmdIdsDebugPrintEnable = TRUE on internal build
  // Skip this function while external build and PcdAgesaPrintEventLogToConsole == FALSE
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
  DisplayMemMbistDataEyeMargin ();

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
        SocketId = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 7;
        BoardChannelId = GnSp3Channel2BoardChannelXlat[SocChannelId];
        Print (L"* Memory PMU Training error at Socket %d, Board Channel %d (SoC Channel %d), ", SocketId, BoardChannelId, SocChannelId);

        if (((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 1) == 1) {  // Isolate DIMM 0
          Print (L"DIMM 0 ");
          switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 24) & 3) {
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
        if (((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 2) == 2) {  // Isolate DIMM 1
          Print (L"DIMM 1 ");
          switch ((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 26) & 3) {
            case 1:
              Print (L"Rank 2\n");
              break;
            case 2:
              Print (L"Rank 3\n");
              break;
            default:
              Print (L"Rank 2 & 3\n");
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

      BoardChannelId = GnSp3Channel2BoardChannelXlat[PmuFailure->UmcId];
      Print (L"\n");
      Print (L"Socket %d, Board Channel %d (SoC Channel %d), %dD training, attempt %d, following training stage completed\n", PmuFailure->SocketId, BoardChannelId, PmuFailure->UmcId, (PmuFailure->Train1d2d + 1), PmuFailure->Train1dAttempt);

      // Loop through all PmuFailure->stageCompletion, print its text message
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
            case 11:
              Print (L"\tEnd of LCDL offset calibration\n");
              break;
            case 12:
              Print (L"\tEnd of LRDIMM Specific training (DWL, MREP, MRD and MWD)\n");
              break;
            case 13:
              Print (L"\tEnd of MPR read delay center optimization\n");
              break;
            case 14:
              Print (L"\tEnd of Write leveling coarse delay\n");
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
        SocketId = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 7;
        BoardChannelId = GnSp3Channel2BoardChannelXlat[SocChannelId];
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
        SocketId = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 7;
        BoardChannelId = GnSp3Channel2BoardChannelXlat[SocChannelId];
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
        SocketId = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 7;
        BoardChannelId = GnSp3Channel2BoardChannelXlat[SocChannelId];
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
        SocketId = ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 3;  // Isolate socket #
        SocChannelId = (ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 7;
        BoardChannelId = GnSp3Channel2BoardChannelXlat[SocChannelId];
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
 * Name: AmdErrorLogDisplayGnDxeInit
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
AmdErrorLogDisplayGnDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;

  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Entry*********************\n"));

  DisplayAmdErrorLogCallBackGn(NULL, NULL);

  DEBUG ((EFI_D_ERROR, "*****************************DXE Error Log Display Driver Exit*********************\n"));
  return (Status);
}



