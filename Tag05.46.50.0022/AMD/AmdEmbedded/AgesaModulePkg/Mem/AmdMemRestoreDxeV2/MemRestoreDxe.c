/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Memory Context Save/Restore driver
 *
 * Contains code that performs memory context save/restore
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  MEM
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
#include "Uefi.h"
#include "PiDxe.h"
// #include "Library/UefiBootServicesTableLib.h"

#include "AGESA.h"
#include "Library/IdsLib.h"
#include "Library/MemRestore.h"
#include <Library/ApobCommonServiceLib.h>
// #include "MemRestoreDxe.h"
#include "Filecode.h"

#define FILECODE        MEM_AMDMEMRESTOREDXEV2_MEMRESTOREDXE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

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

/**
 *---------------------------------------------------------------------------------------
 *
 *  Entry for DxeAmdMemRestoreEntry
 *
 *    @retval          EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
DxeAmdMemRestoreEntry (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS        Status;
  BOOLEAN           ApcbRecoveryFlag;

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tDxeAmdMemRestoreEntry Entry\n");

  ApcbRecoveryFlag = FALSE;
  Status = ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
  ASSERT (Status == EFI_SUCCESS);

  if (ApcbRecoveryFlag != TRUE) { // Non APCB Recovery Flag
    AmdMemRestoreSignOff ();
  } else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Recovery flag set. do nothing\n");
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tDxeAmdMemRestoreEntry Exit\n");
  return EFI_SUCCESS;
}
