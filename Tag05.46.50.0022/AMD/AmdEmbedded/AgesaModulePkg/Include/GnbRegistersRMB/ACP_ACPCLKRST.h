/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ACP_ACPCLKRST_H_
#define _ACP_ACPCLKRST_H_


/***********************************************************
* Register Name : ACP_CLKMUX_SEL
* Register Description :
* ACP Clock Mux selection Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This is to select the clock bewteen the 96Mhz/48Mhz/24Mhz/DFS_ACLK and I2S_MCLK
// 3'b000 = 96Mhz Clock
// 3'b001 = 48Mhz
// 3'b010 = 24Mhz
// 3b'011 = ACLK
// 3'b100 = I2S_MCLK
#define ACP_CLKMUX_SEL_ACP_CLKMUX_SEL_OFFSET      0
#define ACP_CLKMUX_SEL_ACP_CLKMUX_SEL_MASK        0x7

// Bitfield Description : 
#define ACP_CLKMUX_SEL_Reserved_15_3_OFFSET      3
#define ACP_CLKMUX_SEL_Reserved_15_3_MASK        0xfff8

// Bitfield Description : The clock division value after the MUX
#define ACP_CLKMUX_SEL_ACP_CLKMUX_DIV_VALUE_OFFSET      16
#define ACP_CLKMUX_SEL_ACP_CLKMUX_DIV_VALUE_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ACP_CLKMUX_SEL:3;
    UINT32                            Reserved_15_3:13;
    UINT32                            ACP_CLKMUX_DIV_VALUE:16;
  } Field;
  UINT32 Value;
} ACP_CLKMUX_SEL_STRUCT;

#define SMN_ACP_CLKMUX_SEL_ADDRESS    0x124102cUL


/***********************************************************
* Register Name : ACP_CONTROL
* Register Description :
* ACP control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Clock Enable.
// Set this bit to enable the clock for the Audio Processor engine . Clear this bit to disable the clock. This bit does not affect the clock to the Audio clock and reset control registers.  Note that when disabling the clock, internal logic will ensure that it is turned off at a safe time with respect to ongoing bus cycles;  the actual clock enable state is reflected in ACP_STATUS[ClkOn]
// 0 = Disable
// 1 = Enable
#define ACP_CONTROL_ClkEn_OFFSET      0
#define ACP_CONTROL_ClkEn_MASK        0x1

// Bitfield Description : 
#define ACP_CONTROL_Reserved_31_1_OFFSET      1
#define ACP_CONTROL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ClkEn:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_CONTROL_STRUCT;

#define SMN_ACP_CONTROL_ADDRESS    0x1241004UL


/***********************************************************
* Register Name : ACP_DYNAMIC_CG_MASTER_CONTROL
* Register Description :
* ACP Dynamic Clock Gating Master control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to one block in ACP.  The particular block Dynamic Clock Gating will be disabled by setting this bit.
// [0] - DMA
// [1] - SystemHub Interface
// [2] - SYSHUB Address Translation
// [3] - SHA
// [4] - Reg access root
// [5] - Shared RAM
// [6] - SRC
// [7] - DSP
// [8] - RSMU Reg Slave Interface/Reg To AXI
// [9] - nic400
// [10] - Direct SRAM Interface
// [11] - Fusion DSP
// [12] - P1 axi2apb
// [13] - DSP0 IRAM/DRAM. For ACP 6.0 following bits are not used - 6,9,10
#define ACP_DYNAMIC_CG_MASTER_CONTROL_ACP_DYNAMIC_CG_Disable_OFFSET      0
#define ACP_DYNAMIC_CG_MASTER_CONTROL_ACP_DYNAMIC_CG_Disable_MASK        0xffff

// Bitfield Description : Each bit corresponds to one block in ACP.  The particular block will be clock gated forcefully. This field has more priority than ACP_DYNAMIC_CG_Disable field.
// [0] - DMA
// [1] - SystemHub Interface
// [2] - SYSHUB Address Translation
// [3] - SHA
// [4] - Reg access root
// [5] - Shared RAM
// [6] - SRC
// [7] - DSP
// [9] - nic400[10] - Direct SRAM Interface
// [11] - Fusion DSP
// [12] - P1 axi2apb
// [13] - DSP0 IRAM/DRAM. For ACP 6.0 following bits are not used - 4,6,8,9,10,12
#define ACP_DYNAMIC_CG_MASTER_CONTROL_ACP_Course_CG_OFFSET      16
#define ACP_DYNAMIC_CG_MASTER_CONTROL_ACP_Course_CG_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ACP_DYNAMIC_CG_Disable:16;
    UINT32                            ACP_Course_CG:16;
  } Field;
  UINT32 Value;
} ACP_DYNAMIC_CG_MASTER_CONTROL_STRUCT;

#define SMN_ACP_DYNAMIC_CG_MASTER_CONTROL_ADDRESS    0x1241010UL


/***********************************************************
* Register Name : ACP_INTR_URGENCY_TIMER
* Register Description :
* ACP INTR URGENCY TIMER Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register indicates timer value  for ACP interrupt urgency generation. This register is w.r.t 24Mhz
#define ACP_INTR_URGENCY_TIMER_ACP_INTR_URGENCY_TIMER_OFFSET      0
#define ACP_INTR_URGENCY_TIMER_ACP_INTR_URGENCY_TIMER_MASK        0xffffff

// Bitfield Description : 
#define ACP_INTR_URGENCY_TIMER_Reserved_30_24_OFFSET      24
#define ACP_INTR_URGENCY_TIMER_Reserved_30_24_MASK        0x7f000000

// Bitfield Description : This register is to enable the urgency of the ACP intr .
// 0 = disable , 1 = enable
#define ACP_INTR_URGENCY_TIMER_ACP_INTR_URGENCY_ENABLE_OFFSET      31
#define ACP_INTR_URGENCY_TIMER_ACP_INTR_URGENCY_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ACP_INTR_URGENCY_TIMER:24;
    UINT32                            Reserved_30_24:7;
    UINT32                            ACP_INTR_URGENCY_ENABLE:1;
  } Field;
  UINT32 Value;
} ACP_INTR_URGENCY_TIMER_STRUCT;

#define SMN_ACP_INTR_URGENCY_TIMER_ADDRESS    0x124101cUL


/***********************************************************
* Register Name : ACP_OCD_HALT_ON_RST
* Register Description :
* ACP OCD Halt on Reset Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : OCD_HALT_ON_RST: Set this bit to 1 before releasing DSP0 (HiFi5) SOFT Reset to enter DSP0 (HiFi5) into debug mode
#define ACP_OCD_HALT_ON_RST_OCD_HALT_ON_RST_OFFSET      0
#define ACP_OCD_HALT_ON_RST_OCD_HALT_ON_RST_MASK        0x1

// Bitfield Description : OCD_HALT_ON_RST1: Set this bit to 1 before releasing Fusion DSP SOFT Reset to enter Fusion DSP into debug mode
#define ACP_OCD_HALT_ON_RST_OCD_HALT_ON_RST1_OFFSET      1
#define ACP_OCD_HALT_ON_RST_OCD_HALT_ON_RST1_MASK        0x2

// Bitfield Description : 
#define ACP_OCD_HALT_ON_RST_Reserved_31_2_OFFSET      2
#define ACP_OCD_HALT_ON_RST_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            OCD_HALT_ON_RST:1;
    UINT32                            OCD_HALT_ON_RST1:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} ACP_OCD_HALT_ON_RST_STRUCT;

#define SMN_ACP_OCD_HALT_ON_RST_ADDRESS    0x124100cUL


/***********************************************************
* Register Name : ACP_PGFSM_CONTROL
* Register Description :
* ACP PGFSM Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP ON/OFF power domain Control.
// 0b - Power OFF ACP TILE
// 1b - Power ON ACP TILE
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_OFFSET      0
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_MASK        0x1

// Bitfield Description : ACP TILE0 Selection.
// 0b - Not selected.
// 1b - Selected.
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_T0_OFFSET      1
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_T0_MASK        0x2

// Bitfield Description : ACP TILE1 Selection.
// 0b - Not selected.
// 1b - Selected.
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_T1_OFFSET      2
#define ACP_PGFSM_CONTROL_ACP_PGFSM_CTRL_T1_MASK        0x4

// Bitfield Description : 
#define ACP_PGFSM_CONTROL_Reserved_31_3_OFFSET      3
#define ACP_PGFSM_CONTROL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            ACP_PGFSM_CTRL:1;
    UINT32                            ACP_PGFSM_CTRL_T0:1;
    UINT32                            ACP_PGFSM_CTRL_T1:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} ACP_PGFSM_CONTROL_STRUCT;

#define SMN_ACP_PGFSM_CONTROL_ADDRESS    0x1241024UL


/***********************************************************
* Register Name : ACP_PGFSM_STATUS
* Register Description :
* ACP PGFSM Status Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP P1 TILE ON/OFF power domain Status.
// 00b - Power is ON
// 01b - In power ON Process
// 10b - Power is OFF
// 11b - In power OFF Process
#define ACP_PGFSM_STATUS_ACP_PGFSM_STATUS_P0_OFFSET      0
#define ACP_PGFSM_STATUS_ACP_PGFSM_STATUS_P0_MASK        0x3

// Bitfield Description : ACP P2 TILE ON/OFF power domain Status.
// 00b - Power is ON
// 01b - In power ON Process
// 10b - Power is OFF
// 11b - In power OFF Process
#define ACP_PGFSM_STATUS_ACP_PGFSM_STATUS_P1_OFFSET      2
#define ACP_PGFSM_STATUS_ACP_PGFSM_STATUS_P1_MASK        0xc

// Bitfield Description : 
#define ACP_PGFSM_STATUS_Reserved_31_4_OFFSET      4
#define ACP_PGFSM_STATUS_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            ACP_PGFSM_STATUS_P0:2;
    UINT32                            ACP_PGFSM_STATUS_P1:2;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} ACP_PGFSM_STATUS_STRUCT;

#define SMN_ACP_PGFSM_STATUS_ADDRESS    0x1241028UL


/***********************************************************
* Register Name : ACP_SOFT_RESET
* Register Description :
* ACP Soft Reset Register.
Supports reset and initialization of the ACP
* Visibility : 0x2
************************************************************/

// Bitfield Description : ACP Soft reset. When set, asserts soft reset to the entire ACP block. Check SoftResetAudDone for soft reset assertion status
#define ACP_SOFT_RESET_SoftResetAud_OFFSET      0
#define ACP_SOFT_RESET_SoftResetAud_MASK        0x1

// Bitfield Description : DMA Soft reset. When set, asserts soft reset to the DMA controller. Check SoftResetDMADone for soft reset assertion status
#define ACP_SOFT_RESET_SoftResetDMA_OFFSET      1
#define ACP_SOFT_RESET_SoftResetDMA_MASK        0x2

// Bitfield Description : DSP Soft reset. When set, asserts soft reset to the DSP. Check SoftResetDSPDone for soft reset assertion status
#define ACP_SOFT_RESET_SoftResetDSP_OFFSET      2
#define ACP_SOFT_RESET_SoftResetDSP_MASK        0x4

// Bitfield Description : Fusion DSP Soft reset. When set, asserts soft reset to the Fusion DSP. Check SoftResetFusionDSPDone for soft reset assertion status
#define ACP_SOFT_RESET_SoftResetFusionDSP_OFFSET      3
#define ACP_SOFT_RESET_SoftResetFusionDSP_MASK        0x8

// Bitfield Description : 
#define ACP_SOFT_RESET_Reserved_7_4_OFFSET      4
#define ACP_SOFT_RESET_Reserved_7_4_MASK        0xf0

// Bitfield Description : 0 = When internal soft reset is asserted, do not reset credit/debit counters.
// 1 = When internal soft reset is asserted, also reset credit/debit counters.
#define ACP_SOFT_RESET_InternalSoftResetMode_OFFSET      8
#define ACP_SOFT_RESET_InternalSoftResetMode_MASK        0x100

// Bitfield Description : 
#define ACP_SOFT_RESET_Reserved_15_9_OFFSET      9
#define ACP_SOFT_RESET_Reserved_15_9_MASK        0xfe00

// Bitfield Description : Status of Soft reset assertion to the ACP.
// 0 = SOFT RESET NOT ASSERTED
// 1 = SOFT RESET ASSERTED
#define ACP_SOFT_RESET_SoftResetAudDone_OFFSET      16
#define ACP_SOFT_RESET_SoftResetAudDone_MASK        0x10000

// Bitfield Description : Status of Soft reset assertion to DMA Controller.
// 0 = SOFT RESET NOT ASSERTED
// 1 = SOFT RESET ASSERTED
#define ACP_SOFT_RESET_SoftResetDMADone_OFFSET      17
#define ACP_SOFT_RESET_SoftResetDMADone_MASK        0x20000

// Bitfield Description : Status of Soft reset assertion to DSP.
// 0 = SOFT RESET NOT ASSERTED
// 1 = SOFT RESET ASSERTED
#define ACP_SOFT_RESET_SoftResetDSPDone_OFFSET      18
#define ACP_SOFT_RESET_SoftResetDSPDone_MASK        0x40000

// Bitfield Description : Status of Soft reset assertion to Fusion DSP.
// 0 = SOFT RESET NOT ASSERTED
// 1 = SOFT RESET ASSERTED
#define ACP_SOFT_RESET_SoftResetFusionDSPDone_OFFSET      19
#define ACP_SOFT_RESET_SoftResetFusionDSPDone_MASK        0x80000

// Bitfield Description : 
#define ACP_SOFT_RESET_Reserved_31_20_OFFSET      20
#define ACP_SOFT_RESET_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            SoftResetAud:1;
    UINT32                            SoftResetDMA:1;
    UINT32                            SoftResetDSP:1;
    UINT32                            SoftResetFusionDSP:1;
    UINT32                            Reserved_7_4:4;
    UINT32                            InternalSoftResetMode:1;
    UINT32                            Reserved_15_9:7;
    UINT32                            SoftResetAudDone:1;
    UINT32                            SoftResetDMADone:1;
    UINT32                            SoftResetDSPDone:1;
    UINT32                            SoftResetFusionDSPDone:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} ACP_SOFT_RESET_STRUCT;

#define SMN_ACP_SOFT_RESET_ADDRESS    0x1241000UL


/***********************************************************
* Register Name : ACP_STATUS
* Register Description :
* ACP status register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates On/Off status of Audio Processor clock.  This bit is not necessarily a mirror of ACP_CONTRL[ClkEn] when disabling the clock due to delays incurred while waiting for bus cycles to complete.
// 0 = Off
// 1 = On
#define ACP_STATUS_ClkOn_OFFSET      0
#define ACP_STATUS_ClkOn_MASK        0x1

// Bitfield Description : 
#define ACP_STATUS_Reserved_31_1_OFFSET      1
#define ACP_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ClkOn:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_STATUS_STRUCT;

#define SMN_ACP_STATUS_ADDRESS    0x1241008UL


/***********************************************************
* Register Name : ACP_ZSC_DSP_CTRL
* Register Description :
* ACP ZSC feature control by DSP
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write 1 for ZSC Idle assertion and 0 for de-assertion
#define ACP_ZSC_DSP_CTRL_ACP_ZSC_DSP_En_OFFSET      0
#define ACP_ZSC_DSP_CTRL_ACP_ZSC_DSP_En_MASK        0x1

// Bitfield Description : 
#define ACP_ZSC_DSP_CTRL_Reserved_15_1_OFFSET      1
#define ACP_ZSC_DSP_CTRL_Reserved_15_1_MASK        0xfffe

// Bitfield Description : Delay count for updating the status after ZSC wakeup. Please see if really required.
#define ACP_ZSC_DSP_CTRL_ACP_ZSC_DSP_DELAY_CNT_OFFSET      16
#define ACP_ZSC_DSP_CTRL_ACP_ZSC_DSP_DELAY_CNT_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ACP_ZSC_DSP_En:1;
    UINT32                            Reserved_15_1:15;
    UINT32                            ACP_ZSC_DSP_DELAY_CNT:16;
  } Field;
  UINT32 Value;
} ACP_ZSC_DSP_CTRL_STRUCT;

#define SMN_ACP_ZSC_DSP_CTRL_ADDRESS    0x1241014UL


/***********************************************************
* Register Name : ACP_ZSC_STS
* Register Description :
* The status of the ACP ZSC state
* Visibility : 0x2
************************************************************/

// Bitfield Description : the ZSC status. '1' indicate ZSC Idle and '0' indicate not in Idle state
#define ACP_ZSC_STS_ACP_ZSC_Stat_OFFSET      0
#define ACP_ZSC_STS_ACP_ZSC_Stat_MASK        0x1

// Bitfield Description : 
#define ACP_ZSC_STS_Reserved_31_1_OFFSET      1
#define ACP_ZSC_STS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            ACP_ZSC_Stat:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ACP_ZSC_STS_STRUCT;

#define SMN_ACP_ZSC_STS_ADDRESS    0x1241018UL


/***********************************************************
* Register Name : AZ_INTR_URGENCY_TIMER
* Register Description :
* AZ INTR URGENCY TIMER Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register indicates timer value  for AZ interrupt urgency generation. This register is w.r.t 24Mhz
#define AZ_INTR_URGENCY_TIMER_AZ_INTR_URGENCY_TIMER_OFFSET      0
#define AZ_INTR_URGENCY_TIMER_AZ_INTR_URGENCY_TIMER_MASK        0xffffff

// Bitfield Description : 
#define AZ_INTR_URGENCY_TIMER_Reserved_30_24_OFFSET      24
#define AZ_INTR_URGENCY_TIMER_Reserved_30_24_MASK        0x7f000000

// Bitfield Description : This register is to enable the urgency of the AZ intr .
// 0 = disable , 1 = enable
#define AZ_INTR_URGENCY_TIMER_AZ_INTR_URGENCY_ENABLE_OFFSET      31
#define AZ_INTR_URGENCY_TIMER_AZ_INTR_URGENCY_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            AZ_INTR_URGENCY_TIMER:24;
    UINT32                            Reserved_30_24:7;
    UINT32                            AZ_INTR_URGENCY_ENABLE:1;
  } Field;
  UINT32 Value;
} AZ_INTR_URGENCY_TIMER_STRUCT;

#define SMN_AZ_INTR_URGENCY_TIMER_ADDRESS    0x1241020UL

#endif /* _ACP_ACPCLKRST_H_ */
