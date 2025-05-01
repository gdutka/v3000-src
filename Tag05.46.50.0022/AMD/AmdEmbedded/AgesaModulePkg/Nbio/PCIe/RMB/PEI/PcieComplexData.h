/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
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
#ifndef _PCIECOMPLEXDATA_H_
#define _PCIECOMPLEXDATA_H_
#include <GnbDxio.h>

#define NON_INITIALIZED_PCI_ADDRESS  0

#define NUMBER_OF_GPP0_PORTS       5
#define NUMBER_OF_GFX_PORTS        6

#define GPP0_WRAP_ID               0
#define GPP0_START_PHY_LANE        0
#define GPP0_END_PHY_LANE          11
#define GPP0_CORE_ID               0

#define GFX_WRAP_ID                1
#define GFX_START_PHY_LANE         12
#define GFX_END_PHY_LANE           19
#define GFX_CORE_ID                1


// PBR0
#define PBR0                      0
#define PBR0_NATIVE_PCI_DEV       1
#define PBR0_NATIVE_PCI_FUN       1
#define PBR0_CORE_ID              GPP0_CORE_ID
#define PBR0_PORT_ID              0
#define PBR0_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR0_UNIT_ID              0

// PBR1
#define PBR1                      1
#define PBR1_NATIVE_PCI_DEV       1
#define PBR1_NATIVE_PCI_FUN       2
#define PBR1_CORE_ID              GPP0_CORE_ID
#define PBR1_PORT_ID              1
#define PBR1_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR1_UNIT_ID              1

// PBR2
#define PBR2                      2
#define PBR2_NATIVE_PCI_DEV       1
#define PBR2_NATIVE_PCI_FUN       3
#define PBR2_CORE_ID              GPP0_CORE_ID
#define PBR2_PORT_ID              2
#define PBR2_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR2_UNIT_ID              2

// PBR3
#define PBR3                      3
#define PBR3_NATIVE_PCI_DEV       1
#define PBR3_NATIVE_PCI_FUN       4
#define PBR3_CORE_ID              GPP0_CORE_ID
#define PBR3_PORT_ID              3
#define PBR3_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR3_UNIT_ID              3

// PBR4
#define PBR4                      4
#define PBR4_NATIVE_PCI_DEV       1
#define PBR4_NATIVE_PCI_FUN       5
#define PBR4_CORE_ID              GPP0_CORE_ID
#define PBR4_PORT_ID              4
#define PBR4_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR4_UNIT_ID              4

// PBR5
#define PBR5                      8
#define PBR5_NATIVE_PCI_DEV       2
#define PBR5_NATIVE_PCI_FUN       1
#define PBR5_CORE_ID              GFX_CORE_ID
#define PBR5_PORT_ID              0
#define PBR5_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR5_UNIT_ID              5

// PBR6
#define PBR6                      9
#define PBR6_NATIVE_PCI_DEV       2
#define PBR6_NATIVE_PCI_FUN       2
#define PBR6_CORE_ID              GFX_CORE_ID
#define PBR6_PORT_ID              1
#define PBR6_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR6_UNIT_ID              6

// PBR7
#define PBR7                      10
#define PBR7_NATIVE_PCI_DEV       2
#define PBR7_NATIVE_PCI_FUN       3
#define PBR7_CORE_ID              GFX_CORE_ID
#define PBR7_PORT_ID              2
#define PBR7_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR7_UNIT_ID              7

// PBR8
#define PBR8                      11
#define PBR8_NATIVE_PCI_DEV       2
#define PBR8_NATIVE_PCI_FUN       4
#define PBR8_CORE_ID              GFX_CORE_ID
#define PBR8_PORT_ID              3
#define PBR8_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR8_UNIT_ID              8

// PBR9
#define PBR9                      12
#define PBR9_NATIVE_PCI_DEV       2
#define PBR9_NATIVE_PCI_FUN       5
#define PBR9_CORE_ID              GFX_CORE_ID
#define PBR9_PORT_ID              4
#define PBR9_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR9_UNIT_ID              9

// PBR10
#define PBR10                      13
#define PBR10_NATIVE_PCI_DEV       2
#define PBR10_NATIVE_PCI_FUN       6
#define PBR10_CORE_ID              GFX_CORE_ID
#define PBR10_PORT_ID              5
#define PBR10_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR10_UNIT_ID              10

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     Gpp0Wrapper;            ///< GPP0 Wrapper
  PCIe_WRAPPER_CONFIG     GfxWrapper;             ///< GFX Wrapper
  // GPP0
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // GFX
  PCIe_ENGINE_CONFIG      PortPBR10;              ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5

} RMB_COMPLEX_CONFIG;


#endif

