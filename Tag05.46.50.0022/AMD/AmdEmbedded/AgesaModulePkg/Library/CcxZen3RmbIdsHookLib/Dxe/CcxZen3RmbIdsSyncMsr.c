/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Implement Ccx IDS sync up MSRs
 *
 * Contains code that sync up MSRs
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include "CcxZen3RmbIdsSyncMsr.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MpService.h>
#include <Filecode.h>

#define FILECODE LIBRARY_CCXZEN3RMBIDSHOOKLIB_DXE_CCXZEN3RMBIDSSYNCMSR_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
CcxIdsZen3RmbDxeSyncMsrAps (
  IN       VOID *MsrTable
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * CcxIdsZen3RmbDxeSyncMsr
 *
 * Synchronize modified MSR from BSP to AP
 *
 * @param[in]  ApMsrSync         MSRs table
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
CcxIdsZen3RmbDxeSyncMsr (
  IN OUT   IDS_BSC_AP_MSR_SYNC *ApMsrSync,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT16      i;
  EFI_STATUS  Status;
  EFI_MP_SERVICES_PROTOCOL *MpServices;

  //
  //Sync all MSR settings with BSP
  //

  for (i = 0; ApMsrSync[i].RegisterAddress != 0; i++) {
    if (!ApMsrSync[i].ForceSetting) {
      ApMsrSync[i].RegisterValue = AsmReadMsr64 (ApMsrSync[i].RegisterAddress);
    }
  }

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);

  if (!EFI_ERROR (Status)) {
    MpServices->StartupAllAPs (
      MpServices,
      CcxIdsZen3RmbDxeSyncMsrAps,
      FALSE,
      NULL,
      0,
      (VOID *) ApMsrSync,
      NULL
    );
  }

}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * AP task to sync MSRs with the BSC
 *
 *
 * @param[in]  MsrTable          MSRs table
 *
 */
VOID
CcxIdsZen3RmbDxeSyncMsrAps (
  IN       VOID *MsrTable
  )
{
  UINT8   i;

  for (i = 0; ((IDS_BSC_AP_MSR_SYNC *) MsrTable)[i].RegisterAddress != 0; i++) {
    AsmMsrAndThenOr64 (
        ((IDS_BSC_AP_MSR_SYNC *) MsrTable)[i].RegisterAddress,
        ~(((IDS_BSC_AP_MSR_SYNC *) MsrTable)[i].RegisterMask),
        (((IDS_BSC_AP_MSR_SYNC *) MsrTable)[i].RegisterValue &
        ((IDS_BSC_AP_MSR_SYNC *) MsrTable)[i].RegisterMask)
        );
  }
}



