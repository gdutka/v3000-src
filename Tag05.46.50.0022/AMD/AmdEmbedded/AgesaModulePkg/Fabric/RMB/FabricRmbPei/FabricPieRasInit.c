/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric PIE RAS initialization.
 *
 * This function initializes the RAS features of PIE.
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
#include <FabricInfoRmb.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/AmdIdsHookLib.h>
#include "Filecode.h"
#include "FabricPieRasInit.h"

#define FILECODE FABRIC_RMB_FABRICRMBPEI_FABRICPIERASINIT_FILECODE

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
CONST UINTN ROMDATA RmbDfGlblCtrlInstanceIds[] = {
  RMB_CS0_INSTANCE_ID,
  RMB_CS1_INSTANCE_ID,
  RMB_CS2_INSTANCE_ID,
  RMB_CS3_INSTANCE_ID,
  RMB_CCM0_INSTANCE_ID,
  RMB_GCM0_INSTANCE_ID,
  RMB_GCM1_INSTANCE_ID,
  RMB_GCM2_INSTANCE_ID,
  RMB_GCM3_INSTANCE_ID,
  RMB_NCM0_INSTANCE_ID,
  RMB_NCM1_INSTANCE_ID,
  RMB_NCS0_INSTANCE_ID,
  RMB_NCS1_INSTANCE_ID,
  RMB_IOMS0_INSTANCE_ID,
  RMB_PIE_INSTANCE_ID
};

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
 *  FabricPieRasInit
 *
 *  Description:
 *    This function initializes the RAS features of PIE.
 *
 */
VOID
FabricPieRasInit (
  IN       CONST EFI_PEI_SERVICES       **PeiServices,
  IN       AMD_PEI_SOC_LOGICAL_ID_PPI   *SocLogicalIdPpi
  )
{
  UINTN                                      i;
  UINTN                                      j;
  UINTN                                      k;
  UINTN                                      InstancesAccountedFor;
  UINTN                                      DeviceMapIndex;
  UINT32                                     WdtCfgRequest;
  UINT32                                     WdtCntSelRequest;
  HWA_STS_HI_REGISTER                        HwaStsHi;
  HWA_STS_LOW_REGISTER                       HwaStsLow;
  HWA_MASK_HI_REGISTER                       HwaMaskHi;
  HWA_MASK_LOW_REGISTER                      HwaMaskLow;
  DF_GLBL_CTRL_REGISTER                      DfGlblCtrl;
  FABRIC_BLK_INST_COUNT_REGISTER             FabricBlkInstCount;
  CONST AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *DeviceMap;

  WdtCfgRequest = (UINT32) PcdGet8 (PcdAmdFabricWdtCfg);
  WdtCntSelRequest = (UINT32) PcdGet8 (PcdAmdFabricWdtCntSel);

  for (i = 0; i < FabricTopologyGetNumberOfProcessorsPresent (); i++) {
    for (j = 0; j < FabricTopologyGetNumberOfDiesOnSocket (i); j++) {
      DeviceMap = FabricTopologyGetDeviceMapOnDie (i, j);
      FabricBlkInstCount.Value = FabricRegisterAccRead (i, j, FABBLKINSTCOUNT_FUNC, FABBLKINSTCOUNT_REG, FABRIC_REG_ACC_BC);
      InstancesAccountedFor = 0;
      DeviceMapIndex = 0;
      while (InstancesAccountedFor < FabricBlkInstCount.Field.BlkInstCount) {
        for (k = 0; k < DeviceMap[DeviceMapIndex].Count; k++) {
          HwaStsLow.Value = FabricRegisterAccRead (i, j, HWA_STS_LOW_FUNC, HWA_STS_LOW_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID);
          if (HwaStsLow.Value != 0) {
            HwaMaskLow.Value = FabricRegisterAccRead (i, j, HWA_MASK_LOW_FUNC, HWA_MASK_LOW_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID);
            HwaStsLow.Value &= ~HwaMaskLow.Value;
            if (HwaStsLow.Value != 0) {
              IDS_HDT_CONSOLE (TOPO_TRACE, "  DF Hardware Assert Low: Socket %d, Die %d, InstanceID %d, Value %x\n", i, j, DeviceMap[DeviceMapIndex].IDs[k].InstanceID, HwaStsLow.Value);
              HwaStsLow.Value = 0;
            }
            FabricRegisterAccWrite (i, j, HWA_STS_LOW_FUNC, HWA_STS_LOW_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID, HwaStsLow.Value, FALSE);
          }
          HwaStsHi.Value = FabricRegisterAccRead (i, j, HWA_STS_HI_FUNC, HWA_STS_HI_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID);
          if (HwaStsHi.Value != 0) {
            HwaMaskHi.Value = FabricRegisterAccRead (i, j, HWA_MASK_HI_FUNC, HWA_MASK_HI_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID);
            HwaStsHi.Value &= ~HwaMaskHi.Value;
            if (HwaStsHi.Value != 0) {
              IDS_HDT_CONSOLE (TOPO_TRACE, "  DF Hardware Assert High: Socket %d, Die %d, InstanceID %d, Value %x\n", i, j, DeviceMap[DeviceMapIndex].IDs[k].InstanceID, HwaStsHi.Value);
              HwaStsHi.Value = 0;
            }
            FabricRegisterAccWrite (i, j, HWA_STS_HI_FUNC, HWA_STS_HI_REG, DeviceMap[DeviceMapIndex].IDs[k].InstanceID, HwaStsHi.Value, FALSE);
          }
          InstancesAccountedFor++;
        }
        DeviceMapIndex++;
      }

      for (k = 0; k < (sizeof (RmbDfGlblCtrlInstanceIds) / sizeof (RmbDfGlblCtrlInstanceIds[0])); k++) {
        DfGlblCtrl.Value = FabricRegisterAccRead (i, j, DF_GLBL_CTRL_FUNC, DF_GLBL_CTRL_REG, RmbDfGlblCtrlInstanceIds[k]);
        if (WdtCfgRequest <= 3) {
          DfGlblCtrl.Field.WDTBaseSel = WdtCfgRequest;
        }
        if (WdtCntSelRequest <= 7) {
          DfGlblCtrl.Field.PIEWDTCntSel = WdtCntSelRequest;
          DfGlblCtrl.Field.IOMWDTCntSel = WdtCntSelRequest;
          DfGlblCtrl.Field.CCMWDTCntSel = WdtCntSelRequest;
        }
        DfGlblCtrl.Field.DisImmSyncFloodOnFatalErr = PcdGetBool (PcdAmdFabricImmSyncFloodOnFatalErrCtrl) ? 0 : 1;
        FabricRegisterAccWrite (i, j, DF_GLBL_CTRL_FUNC, DF_GLBL_CTRL_REG, RmbDfGlblCtrlInstanceIds[k], DfGlblCtrl.Value, FALSE);
      }
    }
  }
}

