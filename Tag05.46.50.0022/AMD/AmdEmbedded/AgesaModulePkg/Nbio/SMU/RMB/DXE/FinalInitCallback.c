/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Final Init Callback
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  NBIO
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include "SmuV13Dxe.h"
#include <GnbRegistersRMB.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IdsLib.h>
#include <Protocol/AmdNbioServicesProtocol.h>
#include <Protocol/MpService.h>

#define FILECODE        NBIO_SMU_RMB_DXE_FINALINITCALLBACK_FILECODE

DXE_AMD_NBIO_SMU_INIT_COMPLETE_PROTOCOL mDxeAmdNbioSmuInitCompleteProtocol = {
  AMD_NBIO_SERVICES_PROTOCOL_REVISION
};


/**
 *---------------------------------------------------------------------------------------
 *  SmuFinalInit
 *
 *  Description:
 *     notification event handler after MpService Protocol ready
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
SmuFinalInit (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
  )
{
  EFI_STATUS                           Status;
  EFI_HANDLE                           Handle;
  EFI_MP_SERVICES_PROTOCOL             *MpServices;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  if (!EFI_ERROR (Status)) {
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
      &Handle,
      &gAmdNbioSmuInitCompleteProtocolGuid,
      EFI_NATIVE_INTERFACE,
      &mDxeAmdNbioSmuInitCompleteProtocol
      );
    IDS_HDT_CONSOLE (MAIN_FLOW, "Publish DXE_AMD_NBIO_SMU_INIT_COMPLETE_PROTOCOL(0x%x)\n", Status);

    // Close event, so it will not be invoked again.
    //
    gBS->CloseEvent (Event);
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);

}


