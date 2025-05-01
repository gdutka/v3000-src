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
#include  <GnbDxio.h>
#include  <BxbComplexData.h>
#include  <Library/PcieConfigLib.h>
#include  <Filecode.h>

#define FILECODE LIBRARY_BXBNBIO_BXBINITLIBV1_BXBCOMPLEXDATA_FILECODE
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

BXB_COMPLEX_CONFIG BixbyComplexData = {
  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof (BXB_COMPLEX_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    0,    // Address
    GPP0_START_PHY_LANE,                           // StartLane
    GPP1_END_PHY_LANE,                             // EndLane
    0    // Bus Number Limit
  },
  //Gpp0 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof (BXB_COMPLEX_CONFIG, Silicon),
      offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR7) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper)
    },
    GPP0_WRAP_ID,
    0,
    GPP0_START_PHY_LANE,
    GPP0_END_PHY_LANE,
    GPP0_START_DXIO_LANE,
    GPP0_END_DXIO_LANE,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //GPP1 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper) - offsetof (BXB_COMPLEX_CONFIG, Silicon),
      0,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR15) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper)
    },
    GPP1_WRAP_ID,
    0,
    GPP1_START_PHY_LANE,
    GPP1_END_PHY_LANE,
    GPP1_START_DXIO_LANE,
    GPP1_END_DXIO_LANE,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP0 WRAPPER START-------------------------------------
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR7) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR6) - offsetof (BXB_COMPLEX_CONFIG, PortPBR7),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR7,                 // GfxWrkRetryCount         [4:7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR6) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR5) - offsetof (BXB_COMPLEX_CONFIG, PortPBR6),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR6,                 // GfxWrkRetryCount         [4:7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR5) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR4) - offsetof (BXB_COMPLEX_CONFIG, PortPBR5),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR5,                 // GfxWrkRetryCount         [4:7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR4) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR3) - offsetof (BXB_COMPLEX_CONFIG, PortPBR4),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR4,                 // GfxWrkRetryCount         [4:7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR3) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR2) - offsetof (BXB_COMPLEX_CONFIG, PortPBR3),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR3,                 // GfxWrkRetryCount         [4:7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR2) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR1) - offsetof (BXB_COMPLEX_CONFIG, PortPBR2),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR2,                 // GfxWrkRetryCount         [4:7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR1) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR0) - offsetof (BXB_COMPLEX_CONFIG, PortPBR1),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR1,                 // GfxWrkRetryCount         [4:7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR0) - offsetof (BXB_COMPLEX_CONFIG, Gpp0Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR15) - offsetof (BXB_COMPLEX_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},                       // EngineData
    0,                                              // Initialization Status
    0xFF,                                           // Scratch
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR0,                 // GfxWrkRetryCount         [4:7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP0 WRAPPER END -------------------------------------
//------------------------------ GPP1 WRAPPER START----------------------------------
  //Port PBR15
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR15) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR14) - offsetof (BXB_COMPLEX_CONFIG, PortPBR15),
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
        PBR15_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR15_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR15_CORE_ID,        // CoreID                   [0:3]
        PBR15_PORT_ID,        // PortID                   [4:7]
        PBR15_PCI_ADDRESS,    // Address PCI_ADDRESS
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR15,                // GfxWrkRetryCount         [4:7]
        PBR15_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR14
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR14) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR13) - offsetof (BXB_COMPLEX_CONFIG, PortPBR14),
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
        PBR14_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR14_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR14_CORE_ID,        // CoreID                   [0:3]
        PBR14_PORT_ID,        // PortID                   [4:7]
        PBR14_PCI_ADDRESS,    // Address PCI_ADDRESS
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR14,                // GfxWrkRetryCount         [4:7]
        PBR14_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR13
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR13) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR12) - offsetof (BXB_COMPLEX_CONFIG, PortPBR13),
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
        PBR13_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR13_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR13_CORE_ID,        // CoreID                   [0:3]
        PBR13_PORT_ID,        // PortID                   [4:7]
        PBR13_PCI_ADDRESS,    // Address PCI_ADDRESS
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR13,                // GfxWrkRetryCount         [4:7]
        PBR13_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR12
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR12) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR11) - offsetof (BXB_COMPLEX_CONFIG, PortPBR12),
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
        PBR12_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR12_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR12_CORE_ID,        // CoreID                   [0:3]
        PBR12_PORT_ID,        // PortID                   [4:7]
        PBR12_PCI_ADDRESS,    // Address PCI_ADDRESS
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR12,                // GfxWrkRetryCount         [4:7]
        PBR12_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR11) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR10) - offsetof (BXB_COMPLEX_CONFIG, PortPBR11),
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
        PBR11_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR11_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR11_CORE_ID,        // CoreID                   [0:3]
        PBR11_PORT_ID,        // PortID                   [4:7]
        PBR11_PCI_ADDRESS,    // Address PCI_ADDRESS
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR11,                // GfxWrkRetryCount         [4:7]
        PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR10) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR9) - offsetof (BXB_COMPLEX_CONFIG, PortPBR10),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR10,                // GfxWrkRetryCount         [4:7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (BXB_COMPLEX_CONFIG, PortPBR9) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      offsetof (BXB_COMPLEX_CONFIG, PortPBR8) - offsetof (BXB_COMPLEX_CONFIG, PortPBR9),
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR9,                 // GfxWrkRetryCount         [4:7]
        PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
     offsetof (BXB_COMPLEX_CONFIG, PortPBR8) - offsetof (BXB_COMPLEX_CONFIG, Gpp1Wrapper),
      0,
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID },                         // EngineData
    0,                                                // Initialization Status
    0xFF,                                             // Scratch
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
        LinkStateResetExit,   // PciBridgeId              [0:3]
        PBR8,                 // GfxWrkRetryCount         [4:7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
};

/*----------------------------------------------------------------------------------------*/
/**
  Initialize PCIe engine parameters

  @param PcieEngine   Pcie Engine passed in by callback
  @param Buffer       Nothing here, will be NULL
  @param Pcie         Pcie platform config
**/
VOID
PcieEngineConfigDefaults (
  IN      PCIe_ENGINE_CONFIG                *PcieEngine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  PcieEngine->Type.Port.SlotPowerLimit = PcdGet8 (PcdBxbSlotPowerLimit);  ///< Slot Power Limit
  PcieEngine->Type.Port.MaxPayloadSize = 5;                      ///< Max_Payload_Size
  PcieEngine->Type.Port.TXDeEmphasis = 0;                        ///< TX De-emphasis
  PcieEngine->Type.Port.TXMargin = 0;                            ///< TX Margin
  PcieEngine->Type.Port.EqSearchMode = 1;                        ///< Equalization Search Mode
  PcieEngine->Type.Port.DisGen3EQPhase = 0;                      ///< Disable Gen3 EQ Phase2/3
  PcieEngine->Type.Port.LinkAspmL1_1 = 0;                        ///< Enable PM L1 SS L1.1
  PcieEngine->Type.Port.LinkAspmL1_2 = 0;                        ///< Enable PM L1 SS L1.2
  PcieEngine->Type.Port.SlotPowerLimitScale = 0;                 ///< Slot Power Limit Scale
  PcieEngine->Type.Port.ClkReq = 0;                              ///< ClkReq:[0:3]
  PcieEngine->Type.Port.EqPreset = 0;                            ///< EqPreset:[4:7]
  PcieEngine->Type.Port.SpcMode.SpcGen1 = 0;                     ///< SPC Mode 2P5GT
  PcieEngine->Type.Port.SpcMode.SpcGen2 = 0;                     ///< SPC Mode 5GT
  PcieEngine->Type.Port.SpcMode.SpcGen3 = 1;                     ///< SPC Mode 8GT
  PcieEngine->Type.Port.LaneEqualizationCntl.DsTxPreset = 7;     ///< Gen3 Downstream Tx Preset
  PcieEngine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = 3; ///< Gen3 Downstream Rx Preset Hint
  PcieEngine->Type.Port.LaneEqualizationCntl.UsTxPreset = 7;     ///< Gen3 Upstream Tx Preset
  PcieEngine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = 3; ///< Gen3 Upstream Rx Preset Hint
  PcieEngine->Type.Port.BypassGen3EQ = 0;                        ///< BypassGen3EQ
  PcieEngine->Type.Port.SRIS_SRNS = 0;                           ///< SRIS SRNS
  PcieEngine->Type.Port.SRIS_LowerSKPSupport = 0;                ///< SRIS Lower SKP Support
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset = 7; ///< Gen4 Downstream Tx Preset
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset = 5; ///< Gen4 Upstream Tx Preset
  PcieEngine->Type.Port.DisGen4EQPhase = 0;                      ///< Gen4 Bypass phase2/3 EQ
  PcieEngine->Type.Port.BypassGen4EQ = 0;                        ///< Gen4 Bypass phase3 EQ
  PcieEngine->Type.Port.EqSearchModeGen4 = 0;                    ///< Equalization Search Mode for Gen4
  PcieEngine->Type.Port.EsmControl = 0;                          ///< Esm Control Bit for this port
  PcieEngine->Type.Port.CcixControl = 0;                         ///< CCIX Esm control bit for this port
  PcieEngine->Type.Port.DlfExchangeDisable = 0;                  ///< DLF Exchange 1:Disable 0:Enable
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
  Initialize PCIe wrapper parameters

  @param Wrapper      Pcie wrapper passed in by callback
  @param Buffer       Nothing here, will be NULL
  @param Pcie         Pcie platform config
**/
VOID
PcieWrapperConfigDefaults (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  Wrapper->Features.PowerOffUnusedLanes = 1;
  Wrapper->Features.PowerOffUnusedPlls = 1;
  Wrapper->Features.ClkGating = 1;
  Wrapper->Features.LclkGating = 1;
  Wrapper->Features.TxclkGatingPllPowerDown = 1;
  Wrapper->Features.PllOffInL1 = 1;
  Wrapper->Features.AccessEncoding = 1;
  Wrapper->Features.CoreReversed = 0;

  PcieConfigRunProcForAllEnginesInWrapper (DESCRIPTOR_ALL_ENGINES, PcieEngineConfigDefaults, NULL, Wrapper);

  return;
}


