/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <AMD.h>
#include <Gnb.h>
#include <Filecode.h>
#include <GnbRegistersRMB.h>
#include <GnbDxio.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/PcieConfigLib.h>
#include "PcieComplexData.h"
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>

#define FILECODE NBIO_PCIE_RMB_PEI_HIDEPORTS_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                     L O C A L   D A T A   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
STATIC
PcieControlPorts (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      GNB_HANDLE              *GnbHandle
  );

VOID
STATIC
PcieEnablePorts (
  IN      GNB_HANDLE              *GnbHandle
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Control port visibility in PCI config space
 *
 *
 * @param[in]  Control         Control Hide/Unhide ports
 * @param[in]  GnbHandle       Pointer to GNB_HANDLE
 */
VOID
PciePortsVisibilityControl (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      GNB_HANDLE              *GnbHandle
  )
{
  switch (Control) {
  case UnhidePorts:
    IDS_HDT_CONSOLE (GNB_TRACE, "Unhide Ports\n");
    PcieControlPorts (UnhidePorts, GnbHandle);
    break;
  case HidePorts:
    IDS_HDT_CONSOLE (GNB_TRACE, "Hide Ports\n");
    PcieControlPorts (HidePorts, GnbHandle);
    PcieEnablePorts (GnbHandle);
    break;
  default:
    ASSERT (FALSE);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Hide/Unhide all ports
 *
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 */

VOID
STATIC
PcieControlPorts (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      GNB_HANDLE              *GnbHandle
  )
{
  UINT32              Value;
  UINT32              Index;
  PRESIL_CTRL0        PreSilCtrl0;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry \n", __FUNCTION__);
  Value = (1 << IOHC_Bridge_CNTL_CrsEnable_OFFSET);
  PreSilCtrl0.Value = PcdGet32 (PcdAmdPreSilCtrl0);
  if ((PreSilCtrl0.Field.Environment == PRESIL_CTRL_ENV_HW) && (Control == HidePorts)) {
    Value |= ((1 << IOHC_Bridge_CNTL_BridgeDis_OFFSET) | (1 << IOHC_Bridge_CNTL_CfgDis_OFFSET));
  }

  for (Index = 0; Index < NUMBER_OF_GPP0_PORTS; Index++) {
   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE (GnbHandle, (SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS + (Index << 10))),
                   (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK | IOHC_Bridge_CNTL_CfgDis_MASK | IOHC_Bridge_CNTL_CrsEnable_MASK),
                   Value,
                   0   // Change to flags if used in DXE
                   );
  }
  for (Index = 0; Index < NUMBER_OF_GFX_PORTS; Index++) {
   SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE (GnbHandle, (SMN_PCIE1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS + (Index << 10))),
                   (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK | IOHC_Bridge_CNTL_CfgDis_MASK | IOHC_Bridge_CNTL_CrsEnable_MASK),
                   Value,
                   0   // Change to flags if used in DXE
                   );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Hide unused ports
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 */

VOID
STATIC
PcieEnablePorts (
  IN      GNB_HANDLE              *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry \n", __FUNCTION__);
  EngineList = PcieConfigGetChildEngine (GnbHandle);
  while (EngineList != NULL) {
    if ((EngineList->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
         (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard) &&
         (EngineList->Type.Port.PortData.PortPresent == 1))) {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a Enabling %d\n", __FUNCTION__, EngineList->Type.Port.LogicalBridgeId);
      SmnRegisterRMW(GnbHandle->Address.Address.Bus,
                     IOHC_BRIDGE_SPACE (GnbHandle, EngineList, SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS),
                      (UINT32)~(IOHC_Bridge_CNTL_BridgeDis_MASK | IOHC_Bridge_CNTL_CfgDis_MASK | IOHC_Bridge_CNTL_CrsEnable_MASK),
                      (1 << IOHC_Bridge_CNTL_CrsEnable_OFFSET),
                      0   // Change to flags if used in DXE
                      );
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit \n", __FUNCTION__);
  return;
}




