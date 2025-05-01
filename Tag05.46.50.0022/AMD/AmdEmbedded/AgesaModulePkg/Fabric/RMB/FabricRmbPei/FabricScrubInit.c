/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric DRAM Scrub initialization.
 *
 * This function initializes the scrub features of PIE.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
#include "AMD.h"
#include <Library/IdsLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/ApobCommonServiceLib.h>
#include <Library/BaseMemoryLib.h>
#include "Filecode.h"
#include "FabricScrubInit.h"

#define FILECODE FABRIC_RMB_FABRICRMBPEI_FABRICSCRUBINIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define NUM_SCRUB_RATE_SETTINGS             16
#define mGetIntLvNumSockets(DramLimitAddr)  (DramLimitAddr.Field.IntLvNumSockets + 1)
#define mGetIntLvNumDies(DramLimitAddr)     (1 << DramLimitAddr.Field.IntLvNumDies)

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
UINT64 GetDramScrubSize (UINTN Socket, UINTN Die);
UINTN  MatchScrubRate (UINT64 ScrubRateInNSec);
UINT32 GetRedirScrubReqLmtEncoding (UINT8 ScrubRedirectionLimit);

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
// In nano seconds
UINT64 DramScrubRateEncodings[NUM_SCRUB_RATE_SETTINGS] = {
  160,      // 0, 160ns
  300,      // 1, 0.3us
  600,      // 2, 0.6us
  1300,     // 3, 1.3us
  2600,     // 4, 2.6us
  5100,     // 5, 5.1us
  10200,    // 6, 10.2us
  20500,    // 7, 20.5us
  41000,    // 8, 41.0us
  81900,    // 9, 81.9us
  163800,   // A, 163.8us
  330000,  // B, 0.33ms
  660000,  // C, 0.66ms
  1310000, // D, 1.31ms
  2620000, // E, 2.62ms
  5240000, // F, 5.24ms
 };

#define REDIR_SCRUB_REQ_LMT_2           1
#define REDIR_SCRUB_REQ_LMT_4           2
#define REDIR_SCRUB_REQ_LMT_8           0
#define REDIR_SCRUB_REQ_LMT_INFINITE    3

UINT32 RedirScrubReqLmtEncodings[] = {
  0, REDIR_SCRUB_REQ_LMT_INFINITE,    // 0 - RedirScrubCtrl[RedirScrubReqLmt]=0x3
  1, REDIR_SCRUB_REQ_LMT_2,           // 1 - RedirScrubCtrl[RedirScrubReqLmt]=0x1
  2, REDIR_SCRUB_REQ_LMT_4,           // 2 - RedirScrubCtrl[RedirScrubReqLmt]=0x2
  3, REDIR_SCRUB_REQ_LMT_8,           // 3 - RedirScrubCtrl[RedirScrubReqLmt]=0x0
 };

/* -----------------------------------------------------------------------------*/
/**
 *
 *  FabricScrubInit
 *
 *  Description:
 *    This function initializes the DRAM scrub features of PIE.
 *
 */
VOID
FabricScrubInit (
  IN       CONST EFI_PEI_SERVICES       **PeiServices,
  IN       AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi
  )
{
  UINT8                                            MaxChannelsPerDie;
  UINTN                                            Socket;
  UINTN                                            Die;
  UINTN                                            NumOfProcessorPresent;
  UINTN                                            NumOfDiesOnSocket;
  UINTN                                            ScrubRateIndex;
  UINT64                                           DramScrubSizeOnDie;
  UINTN                                            ScrubTimeInHour;
  UINTN                                            i;
  UINT64                                           ScrubTimeInNSec;
  UINT64                                           ScrubRateInNSec;
  UINT32                                           ApobInstanceId;
  UINT32                                           RedirScrubReqLmtEncoded;
  BOOLEAN                                          ScrubRedirection;
  BOOLEAN                                          ScrubPoisoned;
  BOOLEAN                                          IsEccEnabled;
  REDIRECT_SCRUB_CTRL_REGISTER                     RedirectScrubCtrl;
  DRAM_SCRUB_BASE_ADDR_REGISTER                    DramScrubBaseAddr;
  DRAM_SCRUB_LIMIT_ADDR_REGISTER                   DramScrubLimitAddr;
  EFI_STATUS                                       Status;

  // Time = (MemSizeInBlk) * (DramScrubRate).
  //
  // Calculation methods:
  // 1. TimeHr * 1.1
  // 2. round to closest and faster time with a scrub rate

  ScrubRedirection = PcdGetBool (PcdAmdFabricEccScrubRedirection);
  ScrubPoisoned    = PcdGetBool (PcdAmdFabricEccScrubPoisoned);
  ScrubTimeInHour  = PcdGet8 (PcdAmdFabricDramScrubTime);
  if (ScrubTimeInHour == 0xFF) {
    ScrubTimeInHour = 24; // The default of Auto maps to 24 hours
  }
  RedirScrubReqLmtEncoded = PcdGet8 (PcdAmdFabricEccScrubRedirectionLimit);

  NumOfProcessorPresent = FabricTopologyGetNumberOfProcessorsPresent ();
  for (Socket = 0; Socket < NumOfProcessorPresent; Socket++) {
    NumOfDiesOnSocket = FabricTopologyGetNumberOfDiesOnSocket (Socket);
    for (Die = 0; Die < NumOfDiesOnSocket; Die++) {
      DramScrubSizeOnDie = GetDramScrubSize (Socket, Die);
      if (DramScrubSizeOnDie == 0) {
        continue;
      }

      ApobInstanceId = ((UINT32) Socket << 8) | (UINT32) Die;
      ApobGetMaxChannelsPerDie (ApobInstanceId, &MaxChannelsPerDie);
      IsEccEnabled = FALSE;
      for (i = 0; i < MaxChannelsPerDie; i++) {
        Status = ApobGetEccEnable (ApobInstanceId, i, &IsEccEnabled);
        if (IsEccEnabled) {
          break;
        }
      }

      if (!IsEccEnabled) {
        continue;
      }

      if (ScrubRedirection || ScrubPoisoned) {
        for (i = 0; i < MaxChannelsPerDie; i++) {
          ApobGetEccEnable (ApobInstanceId, i, &IsEccEnabled);
          RedirectScrubCtrl.Value = FabricRegisterAccRead (Socket, Die, REDIRECTSCRUBCTRL_FUNC, REDIRECTSCRUBCTRL_REG, (RMB_CS0_INSTANCE_ID + i));
          RedirectScrubCtrl.Fields.RedirScrubMode = (ScrubRedirection && IsEccEnabled) ? 1 : 0;
          RedirectScrubCtrl.Fields.RedirScrubMode |= (ScrubPoisoned && IsEccEnabled) ? 2 : 0;
          if (IsEccEnabled) {
            RedirectScrubCtrl.Fields.RedirScrubReqLmt = RedirScrubReqLmtEncoded;
          }
          FabricRegisterAccWrite (Socket, Die, REDIRECTSCRUBCTRL_FUNC, REDIRECTSCRUBCTRL_REG, (RMB_CS0_INSTANCE_ID + i), RedirectScrubCtrl.Value, FALSE);
        }
      }

      if (ScrubTimeInHour == 0) {
        // Disable periodic scrubber
        continue;
      }

      ScrubTimeInNSec = MultU64x32 (3960000000000ull, (UINT32) ScrubTimeInHour); // 60 * 60 * 1000 * 1000 * 1000 * 11 Add 10% (* 11 / 10)
      ScrubRateInNSec = DivU64x64Remainder (ScrubTimeInNSec, RShiftU64 (DramScrubSizeOnDie, 6), NULL); // 64 bytes per block
      ScrubRateIndex = MatchScrubRate (ScrubRateInNSec);

      DramScrubLimitAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMSCRUBLIMITADDR_FUNC, DRAMSCRUBLIMITADDR_REG, FABRIC_REG_ACC_BC);
      DramScrubLimitAddr.Fields.DramScrubRate = ScrubRateIndex;
      FabricRegisterAccWrite (Socket, Die, DRAMSCRUBLIMITADDR_FUNC, DRAMSCRUBLIMITADDR_REG, FABRIC_REG_ACC_BC, DramScrubLimitAddr.Value, FALSE);
      IDS_HDT_CONSOLE (CPU_TRACE, "    DramScrubRate is set to 0x%x on Socket %d Die %d\n", ScrubRateIndex, Socket, Die);

      // Enable scrubber
      DramScrubBaseAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMSCRUBBASEADDR_FUNC, DRAMSCRUBBASEADDR_REG, FABRIC_REG_ACC_BC);
      DramScrubBaseAddr.Fields.DramScrubValid = 1;
      DramScrubBaseAddr.Fields.DramScrubEn = 1;
      FabricRegisterAccWrite (Socket, Die, DRAMSCRUBBASEADDR_FUNC, DRAMSCRUBBASEADDR_REG, FABRIC_REG_ACC_BC, DramScrubBaseAddr.Value, FALSE);
    }
  }
}


UINT64
GetDramScrubSize (
  IN       UINTN Socket,
  IN       UINTN Die
  )
{

  DRAM_SCRUB_BASE_ADDR_REGISTER   DramScrubBaseAddr;
  DRAM_SCRUB_LIMIT_ADDR_REGISTER  DramScrubLimitAddr;
  DRAM_BASE_ADDRESS_REGISTER      DramBaseAddr;
  DRAM_LIMIT_ADDRESS_REGISTER     DramLimitAddr;
  DRAM_HOLE_CONTROL_REGISTER      DramHoleControl;
  UINT64                          DramScrubSize;
  UINT32                          AddrMapPair;

  DramScrubBaseAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMSCRUBBASEADDR_FUNC, DRAMSCRUBBASEADDR_REG, FABRIC_REG_ACC_BC);
  DramScrubLimitAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMSCRUBLIMITADDR_FUNC, DRAMSCRUBLIMITADDR_REG, FABRIC_REG_ACC_BC);
  if (DramScrubLimitAddr.Value == 0) {
    return (0);
  }
  DramScrubSize = LShiftU64 ((UINT64) ((DramScrubLimitAddr.Fields.DramScrubLimitAddr + 1) - DramScrubBaseAddr.Fields.DramScrubBaseAddr), 20);

  for (AddrMapPair = 0; AddrMapPair < 2; AddrMapPair++) {
    DramBaseAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMBASEADDR0_FUNC, DRAMBASEADDR0_REG + (AddrMapPair << 3), RMB_PIE_INSTANCE_ID);
    if (DramBaseAddr.Field.AddrRngVal == 0) {
      continue;
    }
    DramLimitAddr.Value = FabricRegisterAccRead (Socket, Die, DRAMLIMITADDR0_FUNC, DRAMLIMITADDR0_REG + (AddrMapPair << 3), RMB_PIE_INSTANCE_ID);
    if ((DramLimitAddr.Field.DstFabricID & 0x20) != (Socket << 5)) {
      continue;
    }

    // There should be only one of LgcyMmioHoleEn set in the maps.
    if (DramBaseAddr.Field.LgcyMmioHoleEn == 1) {
      DramHoleControl.Value = FabricRegisterAccRead (Socket, Die, DRAMHOLECTRL_FUNC, DRAMHOLECTRL_REG, RMB_PIE_INSTANCE_ID);
      if (DramHoleControl.Field.DramHoleValid == 1) {
        DramScrubSize -= (0x100 - DramHoleControl.Field.DramHoleBase) << 24;
      }
    }
  }

  return (DramScrubSize);
}


UINTN
MatchScrubRate (
  IN       UINT64 ScrubRateInNSec
  )
{
  UINTN   Index;
  UINTN   ScrubRateIndex;

  ScrubRateIndex = 0;
  for (Index =   0; Index < NUM_SCRUB_RATE_SETTINGS; Index++) {
    if (ScrubRateInNSec >= DramScrubRateEncodings[Index]) {
      ScrubRateIndex = Index;
    } else {
      break;
    }
  }
  return (ScrubRateIndex);
}

UINT32
GetRedirScrubReqLmtEncoding (
  IN       UINT8 ScrubRedirectionLimit
  )
{
  UINT8 i;
  UINT32 Result;

  Result = REDIR_SCRUB_REQ_LMT_4; // default is REDIR_SCRUB_REQ_LMT_4
  for (i = 0; i < (sizeof (RedirScrubReqLmtEncodings) / sizeof (UINT32)); i += 2) {
    if (RedirScrubReqLmtEncodings[i] == ScrubRedirectionLimit) {
      Result = RedirScrubReqLmtEncodings[i + 1];
      break;
    }
  }

  return Result;
}

