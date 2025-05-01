/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _PCIECORE_H_
#define _PCIECORE_H_


/***********************************************************
* Register Name : PCIE_RESERVED
* Register Description :
* Reserved register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define PCIE_RESERVED_Reserved_31_0_OFFSET      0
#define PCIE_RESERVED_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} PCIE_RESERVED_STRUCT;

#define SMN_PCIE0_PCIE_RESERVED_ADDRESS    0x11180000UL

#define SMN_PCIE1_PCIE_RESERVED_ADDRESS    0x11280000UL

#define SMN_PCIE2_PCIE_RESERVED_ADDRESS    0x11380000UL

#define SMN_PCIE3_PCIE_RESERVED_ADDRESS    0x11480000UL


/***********************************************************
* Register Name : SWRST_COMMAND_STATUS
* Register Description :
* Software Reset Command and Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When RECONFIGURE_EN bit is 1, forces a reconfiguration event, when a 1 is written. When the reconfiguration is complete, hardware automatically clears this bit.
#define SWRST_COMMAND_STATUS_RECONFIGURE_OFFSET      0
#define SWRST_COMMAND_STATUS_RECONFIGURE_MASK        0x1

// Bitfield Description : When ATOMIC_RESET_EN bit is 1, forces an atomic reset event, when a 1 is written. When the atomic reset is complete, hardware automatically clears this bit.
#define SWRST_COMMAND_STATUS_ATOMIC_RESET_OFFSET      1
#define SWRST_COMMAND_STATUS_ATOMIC_RESET_MASK        0x2

// Bitfield Description : 
#define SWRST_COMMAND_STATUS_Reserved_15_2_OFFSET      2
#define SWRST_COMMAND_STATUS_Reserved_15_2_MASK        0xfffc

// Bitfield Description : Indicates if the reset cycle is complete.
#define SWRST_COMMAND_STATUS_RESET_COMPLETE_OFFSET      16
#define SWRST_COMMAND_STATUS_RESET_COMPLETE_MASK        0x10000

// Bitfield Description : Indicates if the reset cycle is in the wait state (able to download efuses).
#define SWRST_COMMAND_STATUS_WAIT_STATE_OFFSET      17
#define SWRST_COMMAND_STATUS_WAIT_STATE_MASK        0x20000

// Bitfield Description : If supported, forces the reset signal to be asserted.
#define SWRST_COMMAND_STATUS_PERST_ASRT_OFFSET      18
#define SWRST_COMMAND_STATUS_PERST_ASRT_MASK        0x40000

// Bitfield Description : 
#define SWRST_COMMAND_STATUS_Reserved_23_19_OFFSET      19
#define SWRST_COMMAND_STATUS_Reserved_23_19_MASK        0xf80000

// Bitfield Description : (Switch Upstream only) Indicates if there is a request to perform a link reset (hot reset, link disable, or link down). Writing a 1 clears this bit. This bit is sticky.
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_OFFSET      24
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_MASK        0x1000000

// Bitfield Description : (Switch Upstream only) Indicates if there is a request to perform a link reset (hot reset, link disable, or link down) that only resets the Port's configuration space. Writing a 1 clears this bit. This bit is sticky.
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_CFG_ONLY_OFFSET      25
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_CFG_ONLY_MASK        0x2000000

// Bitfield Description : (Switch Upstream only) Indicates if there is a request to perform a link reset (hot reset, link disable, or link down) that also resets the Port's PCS and PHY. Writing a 1 clears this bit. This bit is sticky.
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_PHY_CALIB_OFFSET      26
#define SWRST_COMMAND_STATUS_SWUS_LINK_RESET_PHY_CALIB_MASK        0x4000000

// Bitfield Description : (Switch Downstream only) Indicates if there is a request to perform a link reset (hot reset, link disable, or link down) that resets the secondary bus. Writing a 1 clears this bit. This bit is sticky.
#define SWRST_COMMAND_STATUS_SWDS_LINK_RESET_OFFSET      27
#define SWRST_COMMAND_STATUS_SWDS_LINK_RESET_MASK        0x8000000

// Bitfield Description : (Switch Downstream only) Indicates if there is a request to perform a link reset (hot reset, link disable, or link down) that only resets the secondary bus's configuration space. Writing a 1 clears this bit. This bit is sticky.
#define SWRST_COMMAND_STATUS_SWDS_LINK_RESET_CFG_ONLY_OFFSET      28
#define SWRST_COMMAND_STATUS_SWDS_LINK_RESET_CFG_ONLY_MASK        0x10000000

// Bitfield Description : Indicates if the link reset event was caused by a hot reset. Clearing the link reset request status bit (SWUS_LINK_RESET, SWUS_LINK_RESET_CFG_ONLY, or SWUS_LINK_RESET_PHY_CALIB) clears this bit.
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_HOT_RESET_OFFSET      29
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_HOT_RESET_MASK        0x20000000

// Bitfield Description : Indicates if the link reset event was caused by a link disable. Clearing the link reset request status bit (SWUS_LINK_RESET, SWUS_LINK_RESET_CFG_ONLY, or SWUS_LINK_RESET_PHY_CALIB) clears this bit.
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_LINK_DISABLE_OFFSET      30
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_LINK_DISABLE_MASK        0x40000000

// Bitfield Description : Indicates if the link reset event was caused by a link down. Clearing the link reset request status bit (SWUS_LINK_RESET, SWUS_LINK_RESET_CFG_ONLY, or SWUS_LINK_RESET_PHY_CALIB) clears this bit.
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_LINK_DOWN_OFFSET      31
#define SWRST_COMMAND_STATUS_LINK_RESET_TYPE_LINK_DOWN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RECONFIGURE:1;
    UINT32                            ATOMIC_RESET:1;
    UINT32                            Reserved_15_2:14;
    UINT32                            RESET_COMPLETE:1;
    UINT32                            WAIT_STATE:1;
    UINT32                            PERST_ASRT:1;
    UINT32                            Reserved_23_19:5;
    UINT32                            SWUS_LINK_RESET:1;
    UINT32                            SWUS_LINK_RESET_CFG_ONLY:1;
    UINT32                            SWUS_LINK_RESET_PHY_CALIB:1;
    UINT32                            SWDS_LINK_RESET:1;
    UINT32                            SWDS_LINK_RESET_CFG_ONLY:1;
    UINT32                            LINK_RESET_TYPE_HOT_RESET:1;
    UINT32                            LINK_RESET_TYPE_LINK_DISABLE:1;
    UINT32                            LINK_RESET_TYPE_LINK_DOWN:1;
  } Field;
  UINT32 Value;
} SWRST_COMMAND_STATUS_STRUCT;

#define SMN_PCIE0_SWRST_COMMAND_STATUS_ADDRESS    0x11180400UL

#define SMN_PCIE1_SWRST_COMMAND_STATUS_ADDRESS    0x11280400UL

#define SMN_PCIE2_SWRST_COMMAND_STATUS_ADDRESS    0x11380400UL

#define SMN_PCIE3_SWRST_COMMAND_STATUS_ADDRESS    0x11480400UL


/***********************************************************
* Register Name : SWRST_CONTROL_6
* Register Description :
* Software Reset Control 6 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if link training is blocked for Port A.
#define SWRST_CONTROL_6_HOLD_TRAINING_A_OFFSET      0
#define SWRST_CONTROL_6_HOLD_TRAINING_A_MASK        0x1

// Bitfield Description : Controls if link training is blocked for Port B.
#define SWRST_CONTROL_6_HOLD_TRAINING_B_OFFSET      1
#define SWRST_CONTROL_6_HOLD_TRAINING_B_MASK        0x2

// Bitfield Description : Controls if link training is blocked for Port C.
#define SWRST_CONTROL_6_HOLD_TRAINING_C_OFFSET      2
#define SWRST_CONTROL_6_HOLD_TRAINING_C_MASK        0x4

// Bitfield Description : Controls if link training is blocked for Port D.
#define SWRST_CONTROL_6_HOLD_TRAINING_D_OFFSET      3
#define SWRST_CONTROL_6_HOLD_TRAINING_D_MASK        0x8

// Bitfield Description : Controls if link training is blocked for Port E.
#define SWRST_CONTROL_6_HOLD_TRAINING_E_OFFSET      4
#define SWRST_CONTROL_6_HOLD_TRAINING_E_MASK        0x10

// Bitfield Description : Controls if link training is blocked for Port F.
#define SWRST_CONTROL_6_HOLD_TRAINING_F_OFFSET      5
#define SWRST_CONTROL_6_HOLD_TRAINING_F_MASK        0x20

// Bitfield Description : Controls if link training is blocked for Port G.
#define SWRST_CONTROL_6_HOLD_TRAINING_G_OFFSET      6
#define SWRST_CONTROL_6_HOLD_TRAINING_G_MASK        0x40

// Bitfield Description : Controls if link training is blocked for Port H.
#define SWRST_CONTROL_6_HOLD_TRAINING_H_OFFSET      7
#define SWRST_CONTROL_6_HOLD_TRAINING_H_MASK        0x80

// Bitfield Description : Controls if link training is blocked for Port I (if supported).
#define SWRST_CONTROL_6_HOLD_TRAINING_I_OFFSET      8
#define SWRST_CONTROL_6_HOLD_TRAINING_I_MASK        0x100

// Bitfield Description : Controls if link training is blocked for Port J (if supported).
#define SWRST_CONTROL_6_HOLD_TRAINING_J_OFFSET      9
#define SWRST_CONTROL_6_HOLD_TRAINING_J_MASK        0x200

// Bitfield Description : Controls if link training is blocked for Port K (if supported).
#define SWRST_CONTROL_6_HOLD_TRAINING_K_OFFSET      10
#define SWRST_CONTROL_6_HOLD_TRAINING_K_MASK        0x400

// Bitfield Description : 
#define SWRST_CONTROL_6_Reserved_31_11_OFFSET      11
#define SWRST_CONTROL_6_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            HOLD_TRAINING_A:1;
    UINT32                            HOLD_TRAINING_B:1;
    UINT32                            HOLD_TRAINING_C:1;
    UINT32                            HOLD_TRAINING_D:1;
    UINT32                            HOLD_TRAINING_E:1;
    UINT32                            HOLD_TRAINING_F:1;
    UINT32                            HOLD_TRAINING_G:1;
    UINT32                            HOLD_TRAINING_H:1;
    UINT32                            HOLD_TRAINING_I:1;
    UINT32                            HOLD_TRAINING_J:1;
    UINT32                            HOLD_TRAINING_K:1;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} SWRST_CONTROL_6_STRUCT;

#define SMN_PCIE0_SWRST_CONTROL_6_ADDRESS    0x11180428UL

#define SMN_PCIE1_SWRST_CONTROL_6_ADDRESS    0x11280428UL

#define SMN_PCIE2_SWRST_CONTROL_6_ADDRESS    0x11380428UL

#define SMN_PCIE3_SWRST_CONTROL_6_ADDRESS    0x11480428UL

#endif /* _PCIECORE_H_ */
