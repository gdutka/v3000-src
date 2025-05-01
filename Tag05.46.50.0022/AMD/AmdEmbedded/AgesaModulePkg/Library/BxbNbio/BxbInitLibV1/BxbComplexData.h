/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe definitions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 00:39:01 -0600 (Wed, 04 Feb 2015) $
 *
 */
#ifndef _BXBCOMPLEXDATA_H_
#define _BXBCOMPLEXDATA_H_

#include <GnbDxio.h>

#define NUMBER_OF_GPP0_PORTS       8
#define NUMBER_OF_GPP1_PORTS       8

#define NON_INITIALIZED_PCI_ADDRESS  0

#define GPP0_WRAP_ID               0
#define GPP0_START_PHY_LANE        16
#define GPP0_END_PHY_LANE          31
#define GPP0_START_DXIO_LANE       0
#define GPP0_END_DXIO_LANE         15
#define GPP0_CORE_ID               0

#define GPP1_WRAP_ID               1
#define GPP1_START_PHY_LANE        0
#define GPP1_END_PHY_LANE          15
#define GPP1_START_DXIO_LANE       16
#define GPP1_END_DXIO_LANE         31
#define GPP1_CORE_ID               1

// PBR0
#define PBR0                      0
#define PBR0_NATIVE_PCI_DEV       1
#define PBR0_NATIVE_PCI_FUN       1
#define PBR0_CORE_ID              GPP0_CORE_ID
#define PBR0_PORT_ID              0
#define PBR0_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR0_UNIT_ID              0
#define PBR0_NUM_UNIT_IDs         0x1

// PBR1
#define PBR1                      1
#define PBR1_NATIVE_PCI_DEV       1
#define PBR1_NATIVE_PCI_FUN       2
#define PBR1_CORE_ID              GPP0_CORE_ID
#define PBR1_PORT_ID              1
#define PBR1_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR1_UNIT_ID              1
#define PBR1_NUM_UNIT_IDs         0x1

// PBR2
#define PBR2                      2
#define PBR2_NATIVE_PCI_DEV       1
#define PBR2_NATIVE_PCI_FUN       3
#define PBR2_CORE_ID              GPP0_CORE_ID
#define PBR2_PORT_ID              2
#define PBR2_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR2_UNIT_ID              2
#define PBR2_NUM_UNIT_IDs         0x1

// PBR3
#define PBR3                      3
#define PBR3_NATIVE_PCI_DEV       1
#define PBR3_NATIVE_PCI_FUN       4
#define PBR3_CORE_ID              GPP0_CORE_ID
#define PBR3_PORT_ID              3
#define PBR3_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR3_UNIT_ID              3
#define PBR3_NUM_UNIT_IDs         0x1

// PBR4
#define PBR4                      4
#define PBR4_NATIVE_PCI_DEV       1
#define PBR4_NATIVE_PCI_FUN       5
#define PBR4_CORE_ID              GPP0_CORE_ID
#define PBR4_PORT_ID              4
#define PBR4_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR4_UNIT_ID              4
#define PBR4_NUM_UNIT_IDs         0x1

// PBR5
#define PBR5                      5
#define PBR5_NATIVE_PCI_DEV       1
#define PBR5_NATIVE_PCI_FUN       6
#define PBR5_CORE_ID              GPP0_CORE_ID
#define PBR5_PORT_ID              13
#define PBR5_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR5_UNIT_ID              5
#define PBR5_NUM_UNIT_IDs         0x1

// PBR6
#define PBR6                      6
#define PBR6_NATIVE_PCI_DEV       1
#define PBR6_NATIVE_PCI_FUN       7
#define PBR6_CORE_ID              GPP0_CORE_ID
#define PBR6_PORT_ID              6
#define PBR6_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR6_UNIT_ID              6
#define PBR6_NUM_UNIT_IDs         0x1

// PBR7
#define PBR7                      7
#define PBR7_NATIVE_PCI_DEV       2
#define PBR7_NATIVE_PCI_FUN       1
#define PBR7_CORE_ID              GPP0_CORE_ID
#define PBR7_PORT_ID              7
#define PBR7_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR7_UNIT_ID              7
#define PBR7_NUM_UNIT_IDs         0x1

// PBR8
#define PBR8                      8
#define PBR8_NATIVE_PCI_DEV       3
#define PBR8_NATIVE_PCI_FUN       1
#define PBR8_CORE_ID              GPP1_CORE_ID
#define PBR8_PORT_ID              0
#define PBR8_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR8_UNIT_ID              8
#define PBR8_NUM_UNIT_IDs         0x1

// PBR9
#define PBR9                      9
#define PBR9_NATIVE_PCI_DEV       3
#define PBR9_NATIVE_PCI_FUN       2
#define PBR9_CORE_ID              GPP1_CORE_ID
#define PBR9_PORT_ID              1
#define PBR9_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR9_UNIT_ID              9
#define PBR9_NUM_UNIT_IDs         0x1

// PBR10
#define PBR10                      10
#define PBR10_NATIVE_PCI_DEV       3
#define PBR10_NATIVE_PCI_FUN       3
#define PBR10_CORE_ID              GPP1_CORE_ID
#define PBR10_PORT_ID              2
#define PBR10_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR10_UNIT_ID              10
#define PBR10_NUM_UNIT_IDs         0x1

// PBR11
#define PBR11                      11
#define PBR11_NATIVE_PCI_DEV       3
#define PBR11_NATIVE_PCI_FUN       4
#define PBR11_CORE_ID              GPP1_CORE_ID
#define PBR11_PORT_ID              3
#define PBR11_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR11_UNIT_ID              11
#define PBR11_NUM_UNIT_IDs         0x1

// PBR12
#define PBR12                      12
#define PBR12_NATIVE_PCI_DEV       3
#define PBR12_NATIVE_PCI_FUN       5
#define PBR12_CORE_ID              GPP1_CORE_ID
#define PBR12_PORT_ID              4
#define PBR12_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR12_UNIT_ID              12
#define PBR12_NUM_UNIT_IDs         0x1

// PBR13
#define PBR13                      13
#define PBR13_NATIVE_PCI_DEV       3
#define PBR13_NATIVE_PCI_FUN       6
#define PBR13_CORE_ID              GPP1_CORE_ID
#define PBR13_PORT_ID              5
#define PBR13_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR13_UNIT_ID              13
#define PBR13_NUM_UNIT_IDs         0x1

// PBR14
#define PBR14                      14
#define PBR14_NATIVE_PCI_DEV       3
#define PBR14_NATIVE_PCI_FUN       7
#define PBR14_CORE_ID              GPP1_CORE_ID
#define PBR14_PORT_ID              6
#define PBR14_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR14_UNIT_ID              14
#define PBR14_NUM_UNIT_IDs         0x1

// PBR15
#define PBR15                      15
#define PBR15_NATIVE_PCI_DEV       4
#define PBR15_NATIVE_PCI_FUN       1
#define PBR15_CORE_ID              GPP1_CORE_ID
#define PBR15_PORT_ID              7
#define PBR15_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR15_UNIT_ID              15
#define PBR15_NUM_UNIT_IDs         0x1


/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     Gpp0Wrapper;            ///< GPP0 Wrapper
  PCIe_WRAPPER_CONFIG     Gpp1Wrapper;            ///< GPP1 Wrapper
  // GPP0
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // GPP1
  PCIe_ENGINE_CONFIG      PortPBR15;               ///< Port PBR15
  PCIe_ENGINE_CONFIG      PortPBR14;               ///< Port PBR14
  PCIe_ENGINE_CONFIG      PortPBR13;               ///< Port PBR13
  PCIe_ENGINE_CONFIG      PortPBR12;               ///< Port PBR12
  PCIe_ENGINE_CONFIG      PortPBR11;               ///< Port PBR11
  PCIe_ENGINE_CONFIG      PortPBR10;               ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8

} BXB_COMPLEX_CONFIG;

#endif

