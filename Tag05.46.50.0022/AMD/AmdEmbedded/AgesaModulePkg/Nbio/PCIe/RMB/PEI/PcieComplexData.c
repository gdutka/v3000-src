/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * This file was automatically generated.
 *
 * RMB
 *
* @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: NBIO
 * @e \$Revision$   @e \$Date$
 *
 */
#include <Library/IdsLib.h>
#include <GnbDxio.h>
#include <PcieComplexData.h>

#define FILECODE        NBIO_RMB_PCIE_DXE_PCIECOMPLEX_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

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

/*----------------------------------------------------------------------------------------*/
//
// Complex configuration
//

RMB_COMPLEX_CONFIG ComplexData = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof (RMB_COMPLEX_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    0,    // Address
    GPP0_START_PHY_LANE,                           // StartLane
    GFX_END_PHY_LANE - GPP0_START_PHY_LANE,       // EndLane
    0    // Bus Number Limit
  },
  //Gpp0 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof (RMB_COMPLEX_CONFIG, Silicon),
      offsetof (RMB_COMPLEX_CONFIG, GfxWrapper) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR4) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper)
    },
    GPP0_WRAP_ID,
    0,
    GPP0_START_PHY_LANE,
    GPP0_END_PHY_LANE,
    GPP0_START_PHY_LANE,
    GPP0_END_PHY_LANE,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Gfx Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (RMB_COMPLEX_CONFIG, GfxWrapper) - offsetof (RMB_COMPLEX_CONFIG, Silicon),
      0,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR10) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper)
    },
    GFX_WRAP_ID,
    0,
    GFX_START_PHY_LANE,
    GFX_END_PHY_LANE,
    GFX_START_PHY_LANE,
    GFX_END_PHY_LANE,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP0 WRAPPER START-------------------------------------
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR4) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR3) - offsetof (RMB_COMPLEX_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR4_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR4_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR4_CORE_ID,         // CoreID                   [0:3]
        PBR4_PORT_ID,         // PortID                   [4:7]
        PBR4_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR4,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR3) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR2) - offsetof (RMB_COMPLEX_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR3_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR3_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR3_CORE_ID,         // CoreID                   [0:3]
        PBR3_PORT_ID,         // PortID                   [4:7]
        PBR3_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR3,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR2) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR1) - offsetof (RMB_COMPLEX_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR2_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR2_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR2_CORE_ID,         // CoreID                   [0:3]
        PBR2_PORT_ID,         // PortID                   [4:7]
        PBR2_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR2,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR1) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR0) - offsetof (RMB_COMPLEX_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR1_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR1_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR1_CORE_ID,         // CoreID                   [0:3]
        PBR1_PORT_ID,         // PortID                   [4:7]
        PBR1_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR1,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR0) - offsetof (RMB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR10) - offsetof (RMB_COMPLEX_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR0_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR0_CORE_ID,         // CoreID                   [0:3]
        PBR0_PORT_ID,         // PortID                   [4:7]
        PBR0_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR0,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP0 WRAPPER END -------------------------------------
//------------------------------ GFX WRAPPER START-------------------------------------
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR10) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR9) - offsetof (RMB_COMPLEX_CONFIG, PortPBR10),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR10_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR10_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR10_CORE_ID,        // CoreID                   [0:3]
        PBR10_PORT_ID,        // PortID                   [4:7]
        PBR10_PCI_ADDRESS,    // Address PCI_ADDRESS
        PBR10,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR9) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR8) - offsetof (RMB_COMPLEX_CONFIG, PortPBR9),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR9_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR9_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR9_CORE_ID,         // CoreID                   [0:3]
        PBR9_PORT_ID,         // PortID                   [4:7]
        PBR9_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR9,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR8) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR7) - offsetof (RMB_COMPLEX_CONFIG, PortPBR8),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR8_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR8_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR8_CORE_ID,         // CoreID                   [0:3]
        PBR8_PORT_ID,         // PortID                   [4:7]
        PBR8_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR8,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR7) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR6) - offsetof (RMB_COMPLEX_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR7_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR7_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR7_CORE_ID,         // CoreID                   [0:3]
        PBR7_PORT_ID,         // PortID                   [4:7]
        PBR7_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR7,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR6) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      offsetof (RMB_COMPLEX_CONFIG, PortPBR5) - offsetof (RMB_COMPLEX_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR6_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR6_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR6_CORE_ID,         // CoreID                   [0:3]
        PBR6_PORT_ID,         // PortID                   [4:7]
        PBR6_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR6,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (RMB_COMPLEX_CONFIG, PortPBR5) - offsetof (RMB_COMPLEX_CONFIG, GfxWrapper),
      0,
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR5_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR5_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR5_CORE_ID,         // CoreID                   [0:3]
        PBR5_PORT_ID,         // PortID                   [4:7]
        PBR5_PCI_ADDRESS,     // Address PCI_ADDRESS
        PBR5,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GFX WRAPPER END -------------------------------------
};
