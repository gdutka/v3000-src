/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIO_IOAPIC_TABLE_H_
#define _NBIO_IOAPIC_TABLE_H_
// --------------------------------------------------
// 14.2.3.1 IOAPIC Clock Gating and Power Management
// --------------------------------------------------

  // IOAPIC::IOAPIC_GLUE_CG_LCLK_CTRL_0[SOFT_OVERRIDE_CLK2~0]=0h
  #define NBIO_IOAPIC_CLOCK_GATING_TBL \
    GNB_ENTRY_RMW_IF_PCD_EQUAL ( \
      ALL_NBIO_TYPE, \
      PcdToken(PcdIOHCClkGatingSupport), \
      TRUE, \
      SMN_IOAPIC_GLUE_CG_LCLK_CTRL_0_ADDRESS, \
      IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK | \
      IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK | \
      IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_MASK, \
      (0x0 << IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET) | \
      (0x0 << IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_OFFSET) | \
      (0x0 << IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_OFFSET) \
      ),
  // End of NBIO_IOAPIC_CLOCK_GATING_TBL


// --------------------------------------------------
// 14.2.3.2 IOAPIC Initialization
//   Table 168: Recommended Interrupt Routing and Swizzling
// --------------------------------------------------

  #define NBIO_IOAPIC_INTR_ROUTING_TBL \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N0_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N1_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N2_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N3_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N4_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N5_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N6_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x6 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N7_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x6 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N8_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N9_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N10_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N11_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N12_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ), \
    GNB_ENTRY_RMW ( \
      ALL_NBIO_TYPE, \
      SMN_N13_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS, \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
      IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
      (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
      (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
      ),
  // End of NBIO_IOAPIC_INTR_ROUTING_TBL

#endif /* _NBIO_IOAPIC_TABLE_H_ */
