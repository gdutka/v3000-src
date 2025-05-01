/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file
 *
 * AMD Integrated Debug Debug_library Routines
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 */

 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdStbLib.h>
#include <Library/AmdCfgPcdBufLib.h>
#include <Filecode.h>

#define FILECODE  LIBRARY_IDSMISCLIB_IDSMISCLIB_FILECODE

/**
 *  IDS back-end code for AGESA_TESTPOINT with verbosity
 *
 *  @param[in] TestPoint  Progress indicator value, see @ref AGESA_POST_CODE
 *  @param[in] Verbosity of TestPoint, Valid value: LOW_LEVEL_VERBOSITY,PROD_LEVEL_VERBOSITY,HIGH_LEVEL_VERBOSITY
 *
 **/
#define AGESA_POST_CODE_PREFIX 0xB0000000ul
VOID
IdsAgesaTestPointVerbosity (
  IN       AGESA_POST_CODE      TestPoint,
  IN       UINT8  Verbosity
  )
{
  UINT32   SizedTp;
  UINT32   TpPrefix;

  TpPrefix = AGESA_POST_CODE_PREFIX;
  SizedTp = (UINT32) TestPoint;
  SizedTp |= TpPrefix;
  if (AmdCfgPcdBufGetBool (EnumPcdAgesaTestPointEnable)) {
    LibAmdIoWrite (AmdCfgPcdBufGet8 (EnumPcdAgesaTestPointWidth), AmdCfgPcdBufGet16 (EnumPcdIdsDebugPort), &SizedTp, NULL);
    IDS_HDT_CONSOLE (MAIN_FLOW, "AGESA_TP:[%x]\n", SizedTp);
  }

  if (AmdCfgPcdBufGetBool (EnumPcdAgesaTestPointToStb)) {
    AmdStbWriteVerboseProd (SizedTp);
  }
}
/*--------------------------------------------------------------------------------------*/
/**
 *  IDS back-end code for AGESA_TESTPOINT
 *
 *  @param[in] TestPoint  Progress indicator value, see @ref AGESA_POST_CODE
 *  @param[in,out] StdHeader    The Pointer of AGESA Header
 *
 **/
/*--------------------------------------------------------------------------------------*/
VOID
IdsAgesaTestPoint (
  IN       AGESA_POST_CODE      TestPoint,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IdsAgesaTestPointVerbosity (TestPoint, PROD_LEVEL_VERBOSITY);
}

/**
 *  IDS Function to display FileCode and Line number on port 0x80 display
 *
 *  @param[in]    FileCode    FileCode and Line Number to display
 *
 **/
BOOLEAN
IdsErrorStop (
  IN       UINT32 FileCode
  )
{
  UINT64  Port80Message;
  UINT16  SimNowEnterDebuggerStatus;
  UINTN   Index;

  // Write out FileCode one time and fire SimNow breakpoint
  IoWrite16 (0xE0, 0xDEAD);
  IoWrite32 (0x80, FileCode);
  SimNowEnterDebuggerStatus = LibAmdSimNowEnterDebugger ();

  Port80Message = 0xDEAD000000000000 | LShiftU64 (FileCode, 16);
  while (1) {
    if (SimNowEnterDebuggerStatus != 0) {
      break;
    }
    for (Index = 0; Index < 6; Index ++) {
      Port80Message = LRotU64 (Port80Message, 8);
      IoWrite32 (0x80, (UINT32)Port80Message);
      MicroSecondDelay (2500000);
    }
    // Rotate back to original value
    Port80Message = LRotU64 (Port80Message, 16);
  }
  return FALSE;
}

/**
 *  IDS Backend Function for ASSERT
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
 *
 **/
VOID
IdsAssert (
  IN      UINT32 FileCode
  )
{
  if (AmdCfgPcdBufGetBool (EnumPcdAgesaAssertEnable)) {
    IdsErrorStop (FileCode);
  }
}

/**
 * Dump buffer to HDTOUT
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 * @param[in]     DataWidth       DataWidth 1 - Byte; 2 - Word; 3 - DWORD; 4 - QWORD
 * @param[in]     LineWidth       Number of data item per line
 */
VOID
IdsDumpBuffer (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count,
  IN       UINT8            DataWidth,
  IN       UINT8            LineWidth
  )
{
  UINT32  Index;
  UINT32  DataItemCount;
  ASSERT (LineWidth != 0);
  ASSERT (DataWidth >= 1 && DataWidth <= 4);
  DataItemCount = 0;
  IDS_HDT_CONSOLE (HdtOutFilter, "Buffer:0x%x Size:0x%x:\n", Buffer, Count * (1 << (DataWidth - 1)));
  for (Index = 0; Index < Count; ) {
    switch (DataWidth) {
    case 1:
      IDS_HDT_CONSOLE (HdtOutFilter, "%02x ", *((UINT8 *) Buffer + Index));
      Index += 1;
      break;
    case 2:
      IDS_HDT_CONSOLE (HdtOutFilter, "%04x ", *(UINT16 *) ((UINT8 *) Buffer + Index));
      Index += 2;
      break;
    case 3:
      IDS_HDT_CONSOLE (HdtOutFilter, "%08x ", *(UINT32 *) ((UINT8 *) Buffer + Index));
      Index += 4;
      break;
    case 4:
      IDS_HDT_CONSOLE (HdtOutFilter, "%08x%08x", *(UINT32 *) ((UINT8 *) Buffer + Index), *(UINT32 *) ((UINT8 *) Buffer + Index + 4));
      Index += 8;
      break;
    default:
      IDS_HDT_CONSOLE (HdtOutFilter, "ERROR! Incorrect Data Width\n");
      return;
    }
    if (++DataItemCount >= LineWidth) {
      IDS_HDT_CONSOLE (HdtOutFilter, "\n");
      DataItemCount = 0;
    }
  }
}

/**
 * Dump buffer to HDTOUT, with 16 bytes per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferByte (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  )
{
  IdsDumpBuffer (HdtOutFilter, Buffer, Count, 1, 16);
}


/**
 * Dump buffer to HDTOUT, with 16 words per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferWord (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  )
{
  IdsDumpBuffer (HdtOutFilter, Buffer, Count, 2, 16);
}

/**
 * Dump buffer to HDTOUT, with 16 Dword per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferDword (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  )
{
  IdsDumpBuffer (HdtOutFilter, Buffer, Count, 3, 16);
}

/**
 * Dump buffer to HDTOUT, with 16 Qword per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferQword (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  )
{
  IdsDumpBuffer (HdtOutFilter, Buffer, Count, 4, 16);
}
