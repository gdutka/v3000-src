/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric Rembrandt SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   AMD Fabric Rembrandt SMM  Driver
 *
 */
#include "AGESA.h"
#include "PiSmm.h"
#include <Library/BaseLib.h>
#include <Library/AmdIdsHookLib.h>
#include "Library/IdsLib.h"
#include <Library/UefiLib.h>
#include <Library/SmmFabricTopologyServices2Lib.h>
#include "Filecode.h"

#define FILECODE FABRIC_RMB_FABRICRMBSMM_AMDFABRICRMBSMM_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *               G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *               L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * AMD Fabric SMM driver main entry point
 *
 *
 * @param[in]       ImageHandle           Image Handle
 * @param[in]       SystemTable           Pointer to system globals
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
AmdFabricRmbSmmDriverEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;

  AGESA_TESTPOINT (TpFabricSmmEntry, NULL);

  Status = FabricTopologyService2SmmProtocolInstall (ImageHandle, SystemTable);

  AGESA_TESTPOINT (TpFabricSmmExit, NULL);

  return Status;
}

