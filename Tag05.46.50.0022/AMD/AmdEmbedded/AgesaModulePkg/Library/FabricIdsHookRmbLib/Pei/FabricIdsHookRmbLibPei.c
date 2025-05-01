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
 * AMD Fabric IDS hook implementation
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */

#include <FabricRegistersDf3.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <IdsHookId.h>
#include <Filecode.h>

#define FILECODE LIBRARY_FABRICIDSHOOKRMBLIB_PEI_FABRICIDSHOOKRMBLIBPEI_FILECODE


/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

// Freeze DF module queues on error --- AGESA
// gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdFabricDisImmSyncFloodOnFatalErr

// DRAM scrub time --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabricDramScrubTime

// Redirect scrubber control --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabricEccScrubRedirection

// Poison scrubber control --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabricEccScrubPoisoned

// Redirect scrubber limit --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabricEccScrubRedirectionLimit

// Disable DF sync flood propagation --- ABL
// APCB_TOKEN_UID_DF_SYNC_FLOOD_PROP

// GMI encryption control --- ABL
// APCB_TOKEN_UID_DF_GMI_ENCRYPT

// xGMI encryption control --- ABL
// APCB_TOKEN_UID_DF_XGMI_ENCRYPT

// CC6 memory region encryption --- ABL
// APCB_TOKEN_UID_DF_SAVE_RESTORE_MEM_ENCRYPT

// Location of private memory regions --- ABL
// APCB_TOKEN_UID_DF_SYS_STORAGE_AT_TOP_OF_MEM

// System probe filter --- ABL
// APCB_TOKEN_UID_DF_PROBE_FILTER

// Memory interleaving --- ABL
// APCB_TOKEN_UID_DF_MEM_INTERLEAVING

// Memory interleaving size --- ABL
// APCB_TOKEN_UID_DF_MEM_INTERLEAVING_SIZE

// Channel interleaving hash --- ABL
// APCB_TOKEN_UID_DF_CHAN_INTLV_HASH

// Memory Clear --- ABL
// APCB_TOKEN_UID_DF_MEM_CLEAR

// CAKE CRC perf bounds --- ABL
// APCB_TOKEN_UID_DF_CAKE_CRC_THRESH_PERF_BOUNDS

// GMI2 replay --- ABL
// APCB_TOKEN_UID_DF_GMI2_REPLAY

// ACPI SLIT Distance Control --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabricSlitDegree

// ACPI SLIT non-self distance --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabric2ndDegreeSlitDistance

// ACPI SLIT same socket distance --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabric3rdDegreeSlitLocalDistance

// ACPI SLIT remote socket distance --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdFabric3rdDegreeSlitRemoteDistance

// PSP error injection support --- AGESA
// gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdPspEinjSupport

#ifndef IDS_HOOK_INTERNAL_SUPPORT
  #define FABRIC_RMB_IDS_HOOKS_INT_PEI
#else
  #include "Internal/FabricIdsHookRmbLibIntPei.h"
#endif

IDS_HOOK_ELEMENT FabricRmbIdsHooksPei[] = {
  FABRIC_RMB_IDS_HOOKS_INT_PEI
  IDS_HOOKS_END
};

IDS_HOOK_TABLE FabricRmbIdsHookTablePei = {
  IDS_HOOK_TABLE_HEADER_REV1_DATA,
  FabricRmbIdsHooksPei
};


AGESA_STATUS
GetIdsHookTable (
  IDS_HOOK_TABLE **IdsHookTable
  )
{
  *IdsHookTable = &FabricRmbIdsHookTablePei;
  return AGESA_SUCCESS;
}
