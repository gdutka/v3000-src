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
#include "Library/UefiBootServicesTableLib.h"

#include "AGESA.h"
#include "Library/IdsLib.h"
#include "Library/MemRestore.h"
#include "MemRestoreDxe.h"
#include "Filecode.h"

#define FILECODE        MEM_AMDMEMRESTOREDXE_MEMRESTOREDXE_FILECODE

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

/*---------------------------------------------------------------------------------------*/
/**
 * Mark the boot completion by memory context restore
 *
 * This function marks the boot completion by memory context restore
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 *
 */
VOID
EFIAPI
MemRestoreBootCompleteCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  AmdMemRestoreSignOff ();
}

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
  EFI_STATUS                     Status;
  EFI_EVENT                      ReadyToBootEvent;

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tDxeAmdMemRestoreEntry Entry\n");

  IDS_HDT_CONSOLE (MAIN_FLOW, "\t[AMD MemRestore] Install callback to mark boot completion by memory context restore\n");

  Status = gBS->CreateEventEx (
               EVT_NOTIFY_SIGNAL,
               TPL_NOTIFY,
               MemRestoreBootCompleteCallback,
               NULL,
               &gEfiEventReadyToBootGuid,
               &ReadyToBootEvent
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tError Installing callback\n");
    return Status;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tDxeAmdMemRestoreEntry Exit\n");
  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


