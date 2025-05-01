/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/DebugLib.h>
#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspPsbFusingLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDPSPPSBFUSINGLIB_AMDPSPPSBFUSINGLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                            L O C A L   F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

BOOLEAN
IsPsbFusingRequired (
  )
{
  TYPE_ATTRIB                 TypeAttrib;
  UINT64                      EntryAddress;
  UINT32                      EntrySize;
  UINT64                      EntryDest;
  PSB_HSTI_STATUS_1           PsbHstiStatus1;
  PSB_STATUS_2                PsbStatus2;
  UINT32                      MailboxHSTIState;

  GetPsbStatus2 (&PsbStatus2);
  IDS_HDT_CONSOLE_PSP_TRACE ("PSB_STATUS_2:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformVendorId: %X\n", PsbStatus2.PlatformVendorId);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformModelId: %X\n", PsbStatus2.PlatformModelId);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tBiosKeyRevisionId: %X\n", PsbStatus2.BiosKeyRevisionId);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tRootKeySelect: %X\n", PsbStatus2.RootKeySelect);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformSecureBootEn: %X\n", PsbStatus2.PlatformSecureBootEn);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableBiosKeyAntiRollback: %X\n", PsbStatus2.DisableBiosKeyAntiRollback);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableAmdKeyUsage: %X\n", PsbStatus2.DisableAmdKeyUsage);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableSecureDebugUnlock: %X\n", PsbStatus2.DisableSecureDebugUnlock);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tCustomerKeyLock: %X\n", PsbStatus2.CustomerKeyLock);

  // Check if PSB fuse already burned
  if (PsbStatus2.PlatformSecureBootEn == 1) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: PSB fuse already burned\n");
    return FALSE;
  }

  // Check if PSB has been disabled
  if ((PsbStatus2.CustomerKeyLock == 1)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: PSB has been permanently disabled\n");
    return FALSE;
  }

  GetPsbHstiStatus1 (&PsbHstiStatus1);
  IDS_HDT_CONSOLE_PSP_TRACE ("PSB_HSTI_STATUS:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPsbTestStatus: %X\n", PsbHstiStatus1.PsbTestStatus);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPsbFusingReadiness: %X\n", PsbHstiStatus1.PsbFusingReadiness);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHstiState: %X\n", PsbHstiStatus1.HstiState);
  // Check PSB status
  if (PsbHstiStatus1.PsbTestStatus != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: PSB Test fail\n");
    return FALSE;
  }

  // Check if required PSP entry exist, BIOS Pub Key and BIOS Signature
  if ((BIOSEntryInfo (BIOS_PUBLIC_KEY, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest) == FALSE) ||
      (BIOSEntryInfo (BIOS_RTM_SIGNATURE, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest) == FALSE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: Required FW entries are not existed\n");
    return FALSE;
  }

  //Test to make sure PSP return the same value between C2P mailbox and C2P register
  PspMboxBiosQueryHSTIState (&MailboxHSTIState);
  if ((MailboxHSTIState & HSTI_STATE_MASK) != PsbHstiStatus1.HstiState) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: HSTI value between C2P mailbox and C2P register not equal\n");
    return FALSE;
  }

  //Before send the command Check PsbFusingReadiness
  if ((PsbHstiStatus1.PsbFusingReadiness == 1) && (PsbStatus2.CustomerKeyLock == 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

///Do PSB Fusing
VOID
DoPsbFusing (
  )
{
  EFI_STATUS                  Status;
  // Send AutoPsb command to PSP
  IDS_HDT_CONSOLE_PSP_TRACE ("Send AutoPsb command to PSP\n");
  Status = PspMboxBiosPsbAutoFusing ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Error occurs during AutoPsb\n");
    ASSERT (FALSE);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("AutoPsb succeeded\n");
  }
}
