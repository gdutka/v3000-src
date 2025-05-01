/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric ready to boot tasks
 *
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
#include <AMD.h>
#include <Filecode.h>
#include <Library/IdsLib.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <Library/FabricResourceReportToGcdLib.h>
#include <FabricRegistersDf3.h>
#include "FabricReadyToBoot.h"

#define FILECODE FABRIC_RMB_FABRICRMBDXE_FABRICREADYTOBOOT_FILECODE

/* -----------------------------------------------------------------------------*/
/**
 *
 *  FabricReadyToBoot
 *
 *  Description:
 *    This function disables PCI extended configuration register access
 *
 */
VOID
EFIAPI
FabricReadyToBoot (
  IN       EFI_EVENT Event,
  IN       VOID      *Context
  )
{
  UINTN                         i;
  UINTN                         j;
  CORE_MASTER_ACC_CTRL_REGISTER CoreMasterAccCtrl;

  AGESA_TESTPOINT (TpDfReadyToBootEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "FabricReadyToBoot: Disable extended configuration access\n");

  for (i = 0; i < FabricTopologyGetNumberOfProcessorsPresent (); i++) {
    for (j = 0; j < FabricTopologyGetNumberOfDiesOnSocket (i); j++) {
      CoreMasterAccCtrl.Value = FabricRegisterAccRead (i, j, CORE_MASTER_ACC_CTRL_FUNC, CORE_MASTER_ACC_CTRL_REG, FABRIC_REG_ACC_BC);
      CoreMasterAccCtrl.Field.EnableCf8ExtCfg = 0;
      FabricRegisterAccWrite (i, j, CORE_MASTER_ACC_CTRL_FUNC, CORE_MASTER_ACC_CTRL_REG, FABRIC_REG_ACC_BC, CoreMasterAccCtrl.Value, TRUE);
    }
  }

  // Report Non-Pci MMIO Region to GCD
  FabricResourceReportToGcd ();

  AGESA_TESTPOINT (TpDfReadyToBootExit, NULL);
}
