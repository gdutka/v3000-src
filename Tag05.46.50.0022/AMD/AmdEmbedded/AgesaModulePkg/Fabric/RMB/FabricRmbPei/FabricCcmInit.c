/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric Core Coherent Master initialization.
 *
 * This function initializes the CCM devices in the fabric.
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
#include <Library/BaseFabricTopologyLib.h>
#include <FabricRegistersDf3.h>
#include <Library/FabricRegisterAccLib.h>
#include "Filecode.h"
#include "FabricCcmInit.h"

#define FILECODE FABRIC_RMB_FABRICRMBPEI_FABRICCCMINIT_FILECODE

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
 *  FabricCcmInit
 *
 *  Description:
 *    This function initializes the CCM devices in the fabric.
 *
 */
VOID
FabricCcmInit (
  IN       CONST EFI_PEI_SERVICES       **PeiServices,
  IN       AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi
  )
{
  UINTN                                 i;
  UINTN                                 j;
  MASTER_LL_LP_REQ_PRI_THRESH_REGISTER  MasterLlLpReqPriThresh;

  for (i = 0; i < FabricTopologyGetNumberOfProcessorsPresent (); i++) {
    for (j = 0; j < FabricTopologyGetNumberOfDiesOnSocket (i); j++) {
      MasterLlLpReqPriThresh.Value = 0;
      MasterLlLpReqPriThresh.Field.LPElvPriReqThr = 2;
      MasterLlLpReqPriThresh.Field.LPCumElvPriReqThr = 0x10;
      MasterLlLpReqPriThresh.Field.LPPreEmptiveBypSatThr = 3;
      FabricRegisterAccWrite (i, j, MASTER_LL_LP_REQ_PRI_THRESH_FUNC, MASTER_LL_LP_REQ_PRI_THRESH_REG, FABRIC_REG_ACC_BC, MasterLlLpReqPriThresh.Value, FALSE);
    }
  }
}
