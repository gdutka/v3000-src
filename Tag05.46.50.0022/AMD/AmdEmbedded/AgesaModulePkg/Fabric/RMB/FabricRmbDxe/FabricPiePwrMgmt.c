/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric PIE power management initialization.
 *
 * This funtion initializes fabric power management features.
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
#include <Library/AmdIdsHookLib.h>
#include "Filecode.h"
#include "FabricPiePwrMgmt.h"

#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICPIEPWRMGMT_FILECODE

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

/* -----------------------------------------------------------------------------*/
/**
 *
 *  FabricPiePwrMgmtInit
 *
 *  Description:
 *    This funtion initializes fabric power management features.
 *
 */
VOID
FabricPiePwrMgmtInit (
  VOID
  )
{
  UINT8                     DFCStateMode;

  IDS_HDT_CONSOLE (CPU_TRACE, "FabricPiePwrMgmtInit\n");

  DFCStateMode = PcdGet8(PcdAmdFabricCstate);
  if ((DFCStateMode != 0) && (DFCStateMode != 1)) {
    //
    // DF CState is auto.
    //
    PcdSet8S (PcdAmdFabricCstate, PcdGet8(PcdAmdCStateMode));
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "PcdAmdCStateMode=%d\n", PcdGet8(PcdAmdCStateMode));
  IDS_HDT_CONSOLE (CPU_TRACE, "PcdAmdFabricCstate=%d\n", PcdGet8(PcdAmdFabricCstate));

}



