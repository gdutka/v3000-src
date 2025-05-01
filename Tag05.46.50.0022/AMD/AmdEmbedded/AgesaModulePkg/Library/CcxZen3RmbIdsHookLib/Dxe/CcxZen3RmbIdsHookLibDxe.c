/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX ZEN3 RMB API, and related functions.
 *
 * Contains code that initializes the core complex
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include "CcxZen3RmbIdsSyncMsr.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CcxRolesLib.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmControl2.h>
#include <IdsHookId.h>
#include "CcxZen3RmbIdsCustomPstates.h"
#include <Filecode.h>

#define FILECODE LIBRARY_CCXZEN3RMBIDSHOOKLIB_DXE_CCXZEN3RMBIDSHOOKLIBDXE_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_EVENT                             CcxIdsZen3RmbDxeMpServiceEvent;

VOID                                  *CcxIdsZen3RmbForMpServiceEvent;

#define IDS_MAX_NUM_OF_SYNC_MSR 20
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
EFIAPI
CcxIdsZen3RmbDxeMpServiceCallBack (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

IDS_HOOK_STATUS
CcxZen3RmbDxeAfterApLaunch (
  HOOK_ID   HookId,
  VOID      *Handle,
  VOID      *Data
  )
{
  EFI_STATUS        Status;
  EFI_STATUS        CalledStatus;
  AMD_CONFIG_PARAMS StdHeader;

  Status = EFI_SUCCESS;

  if (CcxIsBsp (&StdHeader)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "CcxZen3RmbDxeAfterApLaunch External Options\n");

    // L1 Stream HW Prefetcher --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdL1StreamPrefetcher

    // L2 Stream HW Prefetcher --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdL2StreamPrefetcher

    // Platform First Error Handling --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCcxCfgPFEHEnable

    // Core Performance Boost --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCpbMode

    // Global C-state Control --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCStateMode

    // Power Supply Idle Control --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPowerSupplyIdleControl

    // SEV ASID Count --- ABL
    // APCB_TOKEN_UID_CCX_SEV_ASID_COUNT

    // SEV-ES ASID Space Limit --- ABL
    // APCB_TOKEN_UID_CCX_MIN_SEV_ASID

    // XTRIG7 Workaround --- ABL
    // APCB_TOKEN_UID_XTRIG7_WORKAROUND

    // ACPI _CST C1 Declaration --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiCstC1

    // Core Watchdog Timer Enable --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCpuWdtEn

    // Core Watchdog Timer Interval --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCpuWdtTimeout

    // Core Watchdog Timer Severity --- AGESA
    // gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdCpuWdtSeverity

    // MCA error thresh enable --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdMcaErrThreshEn

    // MCA error thresh count --- AGESA
    // gEfiAmdAgesaPkgTokenSpaceGuid.PcdMcaErrThreshCount

    // RedirectForReturnDis --- AGESA
    // gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdRedirectForReturnDis

    // Streaming Stores Control --- AGESA
    // gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdStreamingStoresCtrl
  }

  //
  // Set up call back after gEfiMpServiceProtocolGuid.
  //
  CalledStatus = gBS->CreateEventEx (
                           EVT_NOTIFY_SIGNAL,
                           TPL_NOTIFY,
                           CcxIdsZen3RmbDxeMpServiceCallBack,
                           NULL,
                           NULL,
                           &CcxIdsZen3RmbDxeMpServiceEvent
                           );
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = gBS->RegisterProtocolNotify (
                          &gEfiMpServiceProtocolGuid,
                          CcxIdsZen3RmbDxeMpServiceEvent,
                          &(CcxIdsZen3RmbForMpServiceEvent)
                          );
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  if (Status == EFI_SUCCESS) {
    return IDS_HOOK_SUCCESS;
  } else {
    return IDS_HOOK_ERROR;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * CcxIdsZen3RmbDxeMpServiceCallBack
 *
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 */
VOID
EFIAPI
CcxIdsZen3RmbDxeMpServiceCallBack (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT16              i;
  IDS_BSC_AP_MSR_SYNC ApMsrSync[IDS_MAX_NUM_OF_SYNC_MSR];
  AMD_CONFIG_PARAMS   StdHeader;

  ZeroMem (ApMsrSync, sizeof (ApMsrSync));

  i = 0;

  // End of sync-up list

  ASSERT (i < IDS_MAX_NUM_OF_SYNC_MSR);
  if (i >= IDS_MAX_NUM_OF_SYNC_MSR) {
    IDS_HDT_CONSOLE (CPU_TRACE, "ERROR: IDS ApMsrSync overflow\n");
    IDS_DEADLOOP ()
  }
  CcxIdsZen3RmbDxeSyncMsr (ApMsrSync, &StdHeader);
}


#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define CCX_IDS_HOOKS_INT
#else
  #include <Internal/CcxZen3RmbIdsHookLibIntDxe.h>
#endif

IDS_HOOK_ELEMENT CcxZen3RmbIdsHooks[] = {
//  {
//    IDS_HOOK_CCX_AFTER_AP_LAUNCH,
//    &CcxZen3RmbDxeAfterApLaunch
//  },
  {
    IDS_HOOK_CCX_CUSTOM_PSTATES,
    &CcxZen3RmbIdsDxeCustomPstates
  },
  CCX_IDS_HOOKS_INT
  IDS_HOOKS_END
};

IDS_HOOK_TABLE CcxZen3RmbIdsHookTable = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  CcxZen3RmbIdsHooks
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &CcxZen3RmbIdsHookTable;
  return AGESA_SUCCESS;
}


