/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric Coherent Slave initialization.
 *
 * This function initializes the CS devices in the fabric.
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
#include <Library/BaseFabricTopologyLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/AmdIdsHookLib.h>
#include "Filecode.h"
#include "FabricCsInit.h"

#define FILECODE FABRIC_RMB_FABRICRMBPEI_FABRICCSINIT_FILECODE

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
 *  FabricCsInit
 *
 *  Description:
 *    This function initializes the CS devices in the fabric.
 *
 */
VOID
FabricCsInit (
  IN       CONST EFI_PEI_SERVICES       **PeiServices,
  IN       AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi
  )
{
  UINTN                      CsInstance;
  CS_CFG_A1_REGISTER         CsCfgA1;
  CS_CFG_A2_REGISTER         CsCfgA2;
  REDIRECT_SCRUB_CTRL_REGISTER    RedirectScrubCtrl;

  for (CsInstance = RMB_CS0_INSTANCE_ID; CsInstance < (RMB_CS0_INSTANCE_ID + RMB_NUM_CS_BLOCKS); CsInstance++) {

    RedirectScrubCtrl.Value = FabricRegisterAccRead (0, 0, REDIRECTSCRUBCTRL_FUNC, REDIRECTSCRUBCTRL_REG, CsInstance);
    CsCfgA1.Value = FabricRegisterAccRead (0, 0, CSCFGA1_FUNC, CSCFGA1_REG, CsInstance);
    if (RedirectScrubCtrl.Fields.RedirScrubMode == 0) {
      CsCfgA1.Field.DisRdToRdDatFwd = 1;
      CsCfgA1.Field.DisRdRspCsdUpdate = 1;
    }
    IDS_HOOK (IDS_HOOK_DF_CS_INIT1, NULL, (VOID *) &CsCfgA1);
    FabricRegisterAccWrite (0, 0, CSCFGA1_FUNC, CSCFGA1_REG, CsInstance, CsCfgA1.Value, FALSE);

    CsCfgA2.Value = FabricRegisterAccRead (0, 0, CSCFGA2_FUNC, CSCFGA2_REG, CsInstance);
    IDS_HOOK (IDS_HOOK_DF_CS_INIT2, NULL, (VOID *) &CsCfgA2);
    FabricRegisterAccWrite (0, 0, CSCFGA2_FUNC, CSCFGA2_REG, CsInstance, CsCfgA2.Value, FALSE);
  }
}

