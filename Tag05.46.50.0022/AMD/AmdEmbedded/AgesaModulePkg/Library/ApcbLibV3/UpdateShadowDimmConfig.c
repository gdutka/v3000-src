/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains interface to the AMD AGESA library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/ApcbLibV3.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ApobCommonServiceLib.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbDataGroups.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbMemGroup.h>
// #include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBLIBV3_UPDATESHADOWDIMMCONFIG_FILECODE

#define DDR_SPD_BYTES_COUNT   512

// @TODO: Fix APOB.h inclusion
#ifndef ABL_APOB_MAX_SOCKETS_SUPPORTED
  #define ABL_APOB_MAX_SOCKETS_SUPPORTED   2   ///< Max number of sockets in system
#endif

#ifndef ABL_APOB_MAX_DIES_PER_SOCKET
  #define ABL_APOB_MAX_DIES_PER_SOCKET     1   ///< Max dies per socket
#endif

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
// extern BOOLEAN                   mApcbRecoveryFlag;

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */
extern UINT8                     mApcbInstance;
extern BOOLEAN                   mAtRuntime;

/**
 *
 *  Update Shadow DIMM configuration
 *
 *  @retval EFI_SUCCESS             Shadow DIMM configuration updated successfully
 *  @retval EFI_OUT_OF_RESOURCES    Can't allocate space for new APCB
 *  @retval EFI_NOT_FOUND           Can't find shadow DIMM configuration
 *
 **/
VOID
EFIAPI
AmdPspApcbUpdateShadowDimmConfig (
  IN       EFI_EVENT Event,
  IN       VOID      *Context
  )
{
  EFI_STATUS                    Status;
  BOOLEAN                       ApcbRecoveryFlag;
  UINT32                        OriginalDimmConfigSize;
  UINT32                        NewDimmConfigSize;
  PSP_SPD_STRUCT                *OriginalDimmConfigData;
  PSP_SPD_STRUCT                *NewDimmConfigData;
  UINT8                         Socket;
  UINT8                         Die;
  UINT8                         Channel;
  UINT8                         MaxDimms;
  UINT16                        Instance;
  UINT8                         Dimm;
  UINT8                         DimmIndex;
  UINT8                         i;
  UINT16                        j;
  SPD_DEF_STRUCT                *ShadowDimm;
  BOOLEAN                       DimmCfgUpdated;
  AMD_CONFIG_PARAMS             StdHeader;
  UINT8                         MaxChannelsPerSocket;
  UINT8                         MaxChannelsPerDie;
  UINT8                         MaxDimmsPerChannel;
  APOB_SPD_STRUCT               *DimmSmbusInfo;
  UINT8                         CurrentPriorityLevel;
  UINT8                         StartingPriorityLevel;
  UINT8                         EndingPriorityLevel;

  IDS_HDT_CONSOLE_PSP_TRACE ("Enter AmdPspApcbUpdateShadowDimmConfig\n");

  if (!PcdGetBool (PcdAmdApcbDimmSpdShadow)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("APCB.DimmSpdShadow disabled. Exit service\n");
    return;
  }

  ApcbRecoveryFlag = FALSE;
  Status = ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // Exit service, if recovery flag set
  //
  if (mAtRuntime == FALSE) {
    if (ApcbRecoveryFlag) {
      IDS_HDT_CONSOLE_PSP_TRACE ("APCB.RecoveryFlag Set. Exit service\n");
      return;
    }
  }
  //
  // Exit service, if recovery flag set
  //
  if (CheckPspRecoveryFlagV2 ()) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Recovery flag set. Exit service\n");
    // ASSERT (FALSE); // Assertion in the debug build
    return;
  }

  Status = ApobGetDimmCfgUpdatedFlag (&DimmCfgUpdated);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (!DimmCfgUpdated) {
    IDS_HDT_CONSOLE_PSP_TRACE ("No DIMM configuration update. Exit service\n");
    return;
  }

  //
  // Allocate space for new DIMM config
  //
  Status = ApobGetMaxChannelsPerSocket (0, &MaxChannelsPerSocket);
  ASSERT (EFI_SUCCESS == Status);
  Status = ApobGetMaxDimmsPerChannel (0, &MaxDimmsPerChannel);
  ASSERT (EFI_SUCCESS == Status);
  MaxDimms = MaxChannelsPerSocket * MaxDimmsPerChannel;
  NewDimmConfigSize = sizeof (NewDimmConfigData->TotalDimms) + sizeof (SPD_DEF_STRUCT) * MaxDimms;

  ApcbAcquireMutex ();
  //
  // Find the original SPDs
  //
  OriginalDimmConfigData = NULL;
  OriginalDimmConfigSize = 0;
  StartingPriorityLevel = APCB_PRIORITY_LEVEL_HARD_FORCE;
  EndingPriorityLevel = APCB_PRIORITY_LEVEL_LIMIT;
  for (CurrentPriorityLevel = StartingPriorityLevel; CurrentPriorityLevel < EndingPriorityLevel; CurrentPriorityLevel ++) {
    ApcbSetPriorityLevel (CurrentPriorityLevel);
    if (ApcbGetType (APCB_GROUP_MEMORY, APCB_MEM_TYPE_SPD_INFO, 0, (UINT8 **)&OriginalDimmConfigData, &OriginalDimmConfigSize)) {
      ASSERT (OriginalDimmConfigData != NULL);
      ASSERT (OriginalDimmConfigSize != 0);
      break;
    }
  }

  if (OriginalDimmConfigSize > NewDimmConfigSize) {
    NewDimmConfigSize = OriginalDimmConfigSize;
    MaxDimms = (UINT8) (OriginalDimmConfigData->TotalDimms & 0xFF);
  }
  NewDimmConfigData = AllocateZeroPool (NewDimmConfigSize);
  if (NULL == NewDimmConfigData) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Cannot allocate space for DIMM config storage\n");
    return;
  }
  NewDimmConfigData->TotalDimms = MaxDimms;
  IDS_HDT_CONSOLE_PSP_TRACE ("Maximum DIMM counts of the current system: %d\n", MaxDimms);

  if (EndingPriorityLevel != CurrentPriorityLevel) {
    //
    // Copy the original SPD to a temporary buffer
    //
    LibAmdMemCopy (NewDimmConfigData, OriginalDimmConfigData, OriginalDimmConfigSize, &StdHeader);
    //
    // Clear ShadowSpdValid for all shadow DIMM entries
    //
    for (i = 0, ShadowDimm = NewDimmConfigData->SpdData; i < NewDimmConfigData->TotalDimms; i ++, ShadowDimm ++) {
      if (!ShadowDimm->DramDownSpdValid) {
        ShadowDimm->ShadowSpdValid = FALSE;
      }
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("Native dramdown/shadow entries found\n");
  }

  //
  // Replace the shadow SPDs with current SPDs
  //
  for (Socket = 0; Socket < ABL_APOB_MAX_SOCKETS_SUPPORTED; Socket ++) {
    for (Die = 0; Die < ABL_APOB_MAX_DIES_PER_SOCKET; Die ++) {
      Instance = MAKE_APOB_INSTANCE_ID (Socket, Die);
      DimmIndex = 0;
      Status = ApobGetMaxChannelsPerDie (Instance, &MaxChannelsPerDie);

      ASSERT (EFI_SUCCESS == Status);
      for (Channel = MaxChannelsPerDie * Die; Channel < MaxChannelsPerDie * (Die + 1); Channel ++) {
        for (Dimm = 0; Dimm < MaxDimmsPerChannel; Dimm ++, DimmIndex ++) {
          //
          // Retrieve the current DIMM configs
          //
          Status = ApobGetDimmSmbusInfo (Instance, DimmIndex, &DimmSmbusInfo);
          if (EFI_SUCCESS == Status) {
            if (DimmSmbusInfo->DimmPresent) {
              IDS_HDT_CONSOLE_PSP_TRACE ("Dimm Present on Socket: %d, Channel: %d, Slot: %d\n",
                                          DimmSmbusInfo->SocketNumber,
                                          DimmSmbusInfo->ChannelNumber,
                                          DimmSmbusInfo->DimmNumber);
              for (i = 0, ShadowDimm = NewDimmConfigData->SpdData; i < NewDimmConfigData->TotalDimms; i ++, ShadowDimm ++) {
                if (!ShadowDimm->DramDownSpdValid &&
                    !ShadowDimm->ShadowSpdValid &&
                    (ShadowDimm->SocketNumber == DimmSmbusInfo->SocketNumber) &&
                    (ShadowDimm->ChannelNumber == DimmSmbusInfo->ChannelNumber) &&
                    (ShadowDimm->DimmNumber == DimmSmbusInfo->DimmNumber)) {
                  //
                  // Initialize shadow DIMM entries
                  //
                  ShadowDimm->SocketNumber    = DimmSmbusInfo->SocketNumber;
                  ShadowDimm->ChannelNumber   = DimmSmbusInfo->ChannelNumber;
                  ShadowDimm->DimmNumber      = DimmSmbusInfo->DimmNumber;
                  //
                  // Update shadow DIMMs
                  //
                  IDS_HDT_CONSOLE_PSP_TRACE ("Udpate Shadow DIMM config at entry %d for Socket: %d, Channel: %d, Dimm: %d\n",
                                              i, ShadowDimm->SocketNumber, ShadowDimm->ChannelNumber, ShadowDimm->DimmNumber);
                  ShadowDimm->ShadowSpdValid = TRUE;
                  LibAmdMemCopy (ShadowDimm->Data, DimmSmbusInfo->Data, DDR_SPD_BYTES_COUNT, &StdHeader);
                  //
                  // Print the new SPD
                  //
                  IDS_HDT_CONSOLE_PSP_TRACE ("\n\t");
                  for (j = 0; j < DDR_SPD_BYTES_COUNT; j ++) {
                    if (0 == j % 16) {
                      IDS_HDT_CONSOLE_PSP_TRACE ("\n\t");
                    }
                    IDS_HDT_CONSOLE_PSP_TRACE ("%02x ", ShadowDimm->Data[j]);
                  }
                  IDS_HDT_CONSOLE_PSP_TRACE ("\n\n");

                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  //
  // Merge the shadow DIMM entries into shadow APCB
  //
  ApcbSetPriorityLevel (APCB_PRIORITY_LEVEL_LOW);
  if (!ApcbSetType (APCB_GROUP_MEMORY, APCB_MEM_TYPE_SPD_INFO, 0, (UINT8 *)NewDimmConfigData, NewDimmConfigSize)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set Shadow DIMM config to APCB\n");
    ApcbReleaseMutex ();
    FreePool (NewDimmConfigData);
    return;
  }

  AmdPspWriteBackApcbShadowCopy (FALSE);
  // AmdPspWriteBackApcbShadowCopy (TRUE);
  ApcbReleaseMutex ();
  FreePool (NewDimmConfigData);

  return;
}
