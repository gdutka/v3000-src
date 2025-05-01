/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe port remapping functions.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 276746 $   @e \$Date: 2014-10-28 12:22:52 -0600 (Mon, 28 Oct 2013) $
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <AMD.h>
#include <Gnb.h>
#include <GnbDxio.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbRegistersRMB.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/GnbPciAccLib.h>

#include <AmdPcieComplex.h>

#define FILECODE NBIO_PCIE_RMB_PEI_PCIEHOTPLUG_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define GPP0_SLOT_CTL_STS_ADDRESS 0x11100068
#define GPP1_SLOT_CTL_STS_ADDRESS 0x11200068

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

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init hotplug features on all hotplug ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieHotplugInitCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  UINT32                Value;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  if (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled ) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Found Hotplug Engine at:\n");
    IDS_HDT_CONSOLE (GNB_TRACE, "  Port.PortId = %d\n", Engine->Type.Port.PortId);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Port.PcieBridgeId = %d\n", Engine->Type.Port.PcieBridgeId);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Port.Address = %x\n", Engine->Type.Port.Address);
    IDS_HDT_CONSOLE (GNB_TRACE, "  Type = ");

    Wrapper = PcieConfigGetParentWrapper (Engine);
    GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);

    // For Hot-plug slots only: IOHC::IOHC_PCIE_CRS_Count[CrsLimitCount] = 0x262
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_IOHC_PCIE_CRS_Count_ADDRESS,
                    (UINT32) ~(IOHC_PCIE_CRS_Count_CrsLimitCount_MASK),
                    0x262 << IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET,
                    0
                    );

    //devCfg:NB_PCIE_SLOT_CAP.HOTPLUG_CAPABLE = 1h;
    GnbLibPciRmw (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CAP_ADDRESS),
                     AccessWidth32,
                     (UINT32) ~(SLOT_CAP_HOTPLUG_CAPABLE_MASK),
                     1 << SLOT_CAP_HOTPLUG_CAPABLE_OFFSET,
                     NULL
                     );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    WRAP_SPACE (GnbHandle, Wrapper, SMN_PCIE0_L1_MISC_CNTRL_1_ADDRESS),
                    (UINT32) ~(L1_MISC_CNTRL_1_REG_force_OrderStreamID_func_MASK),
                    1 << L1_MISC_CNTRL_1_REG_force_OrderStreamID_func_OFFSET,
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS),
                    (UINT32) ~(PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_MASK),
                    0 << PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_OFFSET,
                    0
                    );

    //
    // Type specific hotplug configuration
    //
    switch (Engine->Type.Port.PortData.LinkHotplug) {
    //
    // Basic Hotplug Configuration
    //
    case DxioHotplugBasic:
      IDS_HDT_CONSOLE (GNB_TRACE, "DxioHotplugBasic\n");
      GnbLibPciRmw (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CNTL_ADDRESS),
                       AccessWidth32,
                       (UINT32) ~(SLOT_CNTL_HOTPLUG_INTR_EN_MASK),
                       1 << SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET,
                       NULL
                       );

      // devCfg:PCIEIND_P: PCIEP PCIEP_PORT_CNTL.NATIVE_PME_EN = !legacyHotPlug;
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_PORT_CNTL_ADDRESS),
                       (UINT32) ~(PCIEP_PORT_CNTL_NATIVE_PME_EN_MASK),
                       0 << PCIEP_PORT_CNTL_NATIVE_PME_EN_OFFSET,
                       0
                       );
      break;

    //
    // Enhanced Hotplug Configuration
    //
    case DxioHotplugEnhanced:
      IDS_HDT_CONSOLE (GNB_TRACE, "DxioHotplugEnhanced\n");

      GnbLibPciRmw (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CNTL_ADDRESS),
                       AccessWidth32,
                       (UINT32) ~(SLOT_CNTL_HOTPLUG_INTR_EN_MASK),
                       1 << SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET,
                       NULL
                       );

      // devCfg:PCIEIND_P: PCIEP PCIEP_PORT_CNTL.NATIVE_PME_EN = !legacyHotPlug;
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_PORT_CNTL_ADDRESS),
                      (UINT32) ~(PCIEP_PORT_CNTL_NATIVE_PME_EN_MASK),
                      1 << PCIEP_PORT_CNTL_NATIVE_PME_EN_OFFSET,
                      0
                      );


      GnbLibPciRead (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CAP_ADDRESS),
                      AccessWidth32,
                      &Value,                      NULL
                      );
      IDS_HDT_CONSOLE (GNB_TRACE, "SLOT_CAP Value = 0x%x\n", Value);
      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                       PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL3_ADDRESS),
                        &Value);
      IDS_HDT_CONSOLE (GNB_TRACE, "PCIE_LC_CNTL3 Value = 0x%x\n", Value);


      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                       PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), GPP0_SLOT_CTL_STS_ADDRESS),
                       &Value
                       );
      if ((Value & (UINT32)(1 << 29)) == 0) {
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        IOHC_BRIDGE_SPACE (GnbHandle, Engine, SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS),
                        (UINT32)~(SMN_0x13B31004_BridgeDis_MASK),
                        (0 << SMN_0x13B31004_BridgeDis_OFFSET),
                        0
                        );
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        IOHC_BRIDGE_SPACE (GnbHandle, Engine, SMN_PCIE0DEVINDCFG1_IOHC_Bridge_CNTL_ADDRESS),
                        (UINT32)~(SMN_0x13B31004_BridgeDis_MASK),
                        (0 << SMN_0x13B31004_BridgeDis_OFFSET),
                        0
                        );
        IDS_HDT_CONSOLE (GNB_TRACE, "No ep - BridgeDis: %08x\n", IOHC_BRIDGE_SPACE (GnbHandle, Engine, SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS));
      }

      break;
    //
    // Inboard Hotplug Configuration
    // Inboard Hotplug is similar to Enhanced Hotplug, except that the device must be present at boot
    //
    case DxioHotplugInboard:
      IDS_HDT_CONSOLE (GNB_TRACE, "DxioHotplugInboard\n");
      if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
        GnbLibPciRmw (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CNTL_ADDRESS),
                         AccessWidth32,
                         (UINT32) ~(SLOT_CNTL_HOTPLUG_INTR_EN_MASK),
                         1 << SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET,
                         NULL
                         );

        // devCfg:PCIEIND_P: PCIEP PCIEP_PORT_CNTL.NATIVE_PME_EN = !legacyHotPlug;
        SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIEP_PORT_CNTL_ADDRESS),
                        (UINT32) ~(PCIEP_PORT_CNTL_NATIVE_PME_EN_MASK),
                        1 << PCIEP_PORT_CNTL_NATIVE_PME_EN_OFFSET,
                        0
                        );

        GnbLibPciRead (GnbHandle->Address.AddressValue | MAKE_SBDFO (0, 0, Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber, SLOT_CAP_ADDRESS),
                        AccessWidth32,
                        &Value,
                        NULL
                        );
        IDS_HDT_CONSOLE (GNB_TRACE, "SLOT_CAP Value = 0x%x\n", Value);
        SmnRegisterRead (GnbHandle->Address.Address.Bus,
                         PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SMN_FUNC0_PCIE0_PCIE_LC_CNTL3_ADDRESS),
                         &Value
                         );
        IDS_HDT_CONSOLE (GNB_TRACE, "PCIE_LC_CNTL3 Value = 0x%x\n", Value);

      }
      break;

    default:
      IDS_HDT_CONSOLE (GNB_TRACE, "Invalid Hotplug Type\n");
      ASSERT (FALSE);
      break;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Configure Hotplug Ports
 *
 *
 * @param[in]  Pcie      Pointer to PCIe_PLATFORM_CONFIG
 */

VOID
PcieConfigureHotplugPorts (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieHotplugInitCallback,
    NULL,
    Pcie
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 */

VOID
PcieHotplugConfigureUSB4 (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT32      ControllerDevice;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  for (ControllerDevice = 3; ControllerDevice < 5; ControllerDevice++) {
    // Set SLOT_CAP.HOTPLUG_CAPABLE = 0x1;
    // Set SLOT_CAP.HOTPLUG_SURPRISE = 0x1;
    GnbLibPciRmw (MAKE_SBDFO (0, 0, ControllerDevice, 1, SLOT_CAP_ADDRESS),
                  AccessWidth32,
                  (UINT32) ~(SLOT_CAP_HOTPLUG_SURPRISE_MASK | SLOT_CAP_HOTPLUG_CAPABLE_MASK),
                  ((1 << SLOT_CAP_HOTPLUG_SURPRISE_OFFSET) | (1 << SLOT_CAP_HOTPLUG_CAPABLE_OFFSET)),
                  NULL
                  );
    // Set SLOT_CNTL.HOTPLUG_INTR_EN = 0x1;
    GnbLibPciRmw (MAKE_SBDFO (0, 0, ControllerDevice, 1, SLOT_CNTL_ADDRESS),
                  AccessWidth32,
                  (UINT32) ~(SLOT_CNTL_HOTPLUG_INTR_EN_MASK),
                  1 << SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET,
                  NULL
                  );
    // Set PCIE_CAP.SLOT_IMPLEMENTED = 0x1;
    GnbLibPciRmw (MAKE_SBDFO (0, 0, ControllerDevice, 1, PCIE_CAP_LIST_ADDRESS),
                  AccessWidth16,
                  (UINT16) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK),
                  1 << PCIE_CAP_SLOT_IMPLEMENTED_OFFSET,
                  NULL
                  );
  }
  return;
}



