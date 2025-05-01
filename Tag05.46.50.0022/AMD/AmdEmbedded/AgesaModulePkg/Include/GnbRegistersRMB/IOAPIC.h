/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IOAPIC_H_
#define _IOAPIC_H_


/***********************************************************
* Register Name : EOI_REGISTER
* Register Description :
* EOI Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register clears the remote IRR bit in the redirection table entry for the corresponding interrupt vector.
// This provides an alternate mechanism other than the EOI special cycle to reach IOAPIC.
#define EOI_REGISTER_Vector_OFFSET      0
#define EOI_REGISTER_Vector_MASK        0xff

// Bitfield Description : 
#define EOI_REGISTER_Reserved_31_8_OFFSET      8
#define EOI_REGISTER_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Vector:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} EOI_REGISTER_IOAPIC_STRUCT;

#define SMN_EOI_REGISTER_ADDRESS    0x2800040UL


/***********************************************************
* Register Name : FEATURES_ENABLE
* Register Description :
* IOAPIC feature control bits.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define FEATURES_ENABLE_Reserved_1_0_OFFSET      0
#define FEATURES_ENABLE_Reserved_1_0_MASK        0x3

// Bitfield Description : Extend IOAPIC ID from 4-bit to 8-bit
// 0=4-bit ID
// 1=8-bit ID.
#define FEATURES_ENABLE_Ioapic_id_ext_en_OFFSET      2
#define FEATURES_ENABLE_Ioapic_id_ext_en_MASK        0x4

// Bitfield Description : 
#define FEATURES_ENABLE_Reserved_3_3_OFFSET      3
#define FEATURES_ENABLE_Reserved_3_3_MASK        0x8

// Bitfield Description : Enabling masked interrupts to be routed back to FCH PIC/IOAPIC.
#define FEATURES_ENABLE_Ioapic_sb_feature_en_OFFSET      4
#define FEATURES_ENABLE_Ioapic_sb_feature_en_MASK        0x10

// Bitfield Description : Enables secondary mode for IOAPIC within an NBIO instance not directly attached to the FCH 0 - primary IOAPIC 1 - secondary IOAPIC.
#define FEATURES_ENABLE_Ioapic_secondary_en_OFFSET      5
#define FEATURES_ENABLE_Ioapic_secondary_en_MASK        0x20

// Bitfield Description : 
#define FEATURES_ENABLE_Reserved_7_6_OFFSET      6
#define FEATURES_ENABLE_Reserved_7_6_MASK        0xc0

// Bitfield Description : Current system architecture. 0=X86. 1=ARM.
#define FEATURES_ENABLE_Ioapic_processor_mode_OFFSET      8
#define FEATURES_ENABLE_Ioapic_processor_mode_MASK        0x100

// Bitfield Description : Selects Level Triggered Mode for INTx inside IOAPIC. Negative pulses and positive pulses on the interrupt wire are cancelled out when this bit is 1.  When this bit is 0, IOAPIC is in Edge Triggered Mode and only positive pulses are cancelled out inside IOAPIC.
#define FEATURES_ENABLE_INTx_LevelOnlyMode_OFFSET      9
#define FEATURES_ENABLE_INTx_LevelOnlyMode_MASK        0x200

// Bitfield Description : 
#define FEATURES_ENABLE_Reserved_31_10_OFFSET      10
#define FEATURES_ENABLE_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            Ioapic_id_ext_en:1;
    UINT32                            Reserved_3_3:1;
    UINT32                            Ioapic_sb_feature_en:1;
    UINT32                            Ioapic_secondary_en:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            Ioapic_processor_mode:1;
    UINT32                            INTx_LevelOnlyMode:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} FEATURES_ENABLE_STRUCT;

#define SMN_FEATURES_ENABLE_ADDRESS    0x14300000UL


/***********************************************************
* Register Name : IOAPICMIO_DATA
* Register Description :
* Data registers for IOAPIC MIO Indirect space
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data registers for IOAPIC MIO Indirect space.
#define IOAPICMIO_DATA_IOAPICMIO_DATA_OFFSET      0
#define IOAPICMIO_DATA_IOAPICMIO_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAPICMIO_DATA:32;
  } Field;
  UINT32 Value;
} IOAPICMIO_DATA_STRUCT;

#define SMN_IOAPICMIO_DATA_ADDRESS    0x2800010UL


/***********************************************************
* Register Name : IOAPICMIO_INDEX
* Register Description :
* Index registers for IOAPIC MIO Indirect space
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index registers for IOAPIC MIO Indirect space.
#define IOAPICMIO_INDEX_IOAPICMIO_INDEX_data_OFFSET      0
#define IOAPICMIO_INDEX_IOAPICMIO_INDEX_data_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAPICMIO_INDEX_data:32;
  } Field;
  UINT32 Value;
} IOAPICMIO_INDEX_STRUCT;

#define SMN_IOAPICMIO_INDEX_ADDRESS    0x2800000UL


/***********************************************************
* Register Name : IOAPIC_ARBITRATION_REGISTER
* Register Description :
* Arbitration Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOAPIC_ARBITRATION_REGISTER_Reserved_23_0_OFFSET      0
#define IOAPIC_ARBITRATION_REGISTER_Reserved_23_0_MASK        0xffffff

// Bitfield Description : Arbitration ID.
#define IOAPIC_ARBITRATION_REGISTER_Arbitration_ID_OFFSET      24
#define IOAPIC_ARBITRATION_REGISTER_Arbitration_ID_MASK        0xf000000

// Bitfield Description : 
#define IOAPIC_ARBITRATION_REGISTER_Reserved_31_28_OFFSET      28
#define IOAPIC_ARBITRATION_REGISTER_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_23_0:24;
    UINT32                            Arbitration_ID:4;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} IOAPIC_ARBITRATION_REGISTER_IOAPIC_STRUCT;

#define SMN_IOAPIC_ARBITRATION_REGISTER_ADDRESS    0x2801008UL


/***********************************************************
* Register Name : IOAPIC_BR_INTERRUPT_ROUTING
* Register Description :
* Bridge 0 interrupt routing.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Map bridge 0 external INTA/B/C/D to IOAPIC pins.
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET      0
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK        0x7

// Bitfield Description : 
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_3_3_OFFSET      3
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_3_3_MASK        0x8

// Bitfield Description : Swizzle bridge 0 external INTA/B/C/D before map to IOAPIC pins.
// 00=ABCD
// 01=BCDA
// 10=CDAB
// 11=DABC.
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET      4
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK        0x30

// Bitfield Description : 
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_15_6_OFFSET      6
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_15_6_MASK        0xffc0

// Bitfield Description : Map bridge 0 interrupts to IOAPIC redirection table entry.
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET      16
#define IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK        0x1f0000

// Bitfield Description : 
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_31_21_OFFSET      21
#define IOAPIC_BR_INTERRUPT_ROUTING_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            Br_ext_Intr_grp:3;
    UINT32                            Reserved_3_3:1;
    UINT32                            Br_ext_Intr_swz:2;
    UINT32                            Reserved_15_6:10;
    UINT32                            Br_ext_Intr_map:5;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} IOAPIC_BR_INTERRUPT_ROUTING_STRUCT;

#define SMN_N0_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300040UL

#define SMN_N10_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300068UL

#define SMN_N11_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x1430006cUL

#define SMN_N12_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300070UL

#define SMN_N13_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300074UL

#define SMN_N14_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300078UL

#define SMN_N15_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x1430007cUL

#define SMN_N1_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300044UL

#define SMN_N2_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300048UL

#define SMN_N3_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x1430004cUL

#define SMN_N4_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300050UL

#define SMN_N5_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300054UL

#define SMN_N6_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300058UL

#define SMN_N7_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x1430005cUL

#define SMN_N8_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300060UL

#define SMN_N9_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS    0x14300064UL


/***********************************************************
* Register Name : IOAPIC_GLUE_CG_LCLK_CTRL_0
* Register Description :
* IOAPIC LCLK Clock Gating Control 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_OFFSET      0
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_MASK        0xf

// Bitfield Description : Hysteresis for local clock gating to filter out small turn off changes.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_OFFSET      4
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_MASK        0xff0

// Bitfield Description : 
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_OFFSET      12
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_MASK        0x3ff000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for IOAPIC_Host_Dma.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for IOAPIC_Perf.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for ioapic_cfg.
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            CG_OFF_HYSTERESIS:8;
    UINT32                            Reserved_21_12:10;
    UINT32                            SOFT_OVERRIDE_CLK9:1;
    UINT32                            SOFT_OVERRIDE_CLK8:1;
    UINT32                            SOFT_OVERRIDE_CLK7:1;
    UINT32                            SOFT_OVERRIDE_CLK6:1;
    UINT32                            SOFT_OVERRIDE_CLK5:1;
    UINT32                            SOFT_OVERRIDE_CLK4:1;
    UINT32                            SOFT_OVERRIDE_CLK3:1;
    UINT32                            SOFT_OVERRIDE_CLK2:1;
    UINT32                            SOFT_OVERRIDE_CLK1:1;
    UINT32                            SOFT_OVERRIDE_CLK0:1;
  } Field;
  UINT32 Value;
} IOAPIC_GLUE_CG_LCLK_CTRL_0_STRUCT;

#define SMN_IOAPIC_GLUE_CG_LCLK_CTRL_0_ADDRESS    0x14300100UL


/***********************************************************
* Register Name : IOAPIC_ID_REGISTER
* Register Description :
* ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOAPIC_ID_REGISTER_Reserved_23_0_OFFSET      0
#define IOAPIC_ID_REGISTER_Reserved_23_0_MASK        0xffffff

// Bitfield Description : IOAPIC device ID.
#define IOAPIC_ID_REGISTER_DEV_ID_OFFSET      24
#define IOAPIC_ID_REGISTER_DEV_ID_MASK        0xf000000

// Bitfield Description : Extended device id, writable only if Ioapic_id_ext_en is set.
#define IOAPIC_ID_REGISTER_EXTEND_ID_OFFSET      28
#define IOAPIC_ID_REGISTER_EXTEND_ID_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_23_0:24;
    UINT32                            DEV_ID:4;
    UINT32                            EXTEND_ID:4;
  } Field;
  UINT32 Value;
} IOAPIC_ID_REGISTER_IOAPIC_STRUCT;

#define SMN_IOAPIC_ID_REGISTER_ADDRESS    0x2801000UL


/***********************************************************
* Register Name : IOAPIC_PERF_CNTL
* Register Description :
* IOAPIC Performance Counter Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define IOAPIC_PERF_CNTL_EVENT0_SEL_OFFSET      0
#define IOAPIC_PERF_CNTL_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define IOAPIC_PERF_CNTL_EVENT1_SEL_OFFSET      8
#define IOAPIC_PERF_CNTL_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define IOAPIC_PERF_CNTL_EVENT2_SEL_OFFSET      16
#define IOAPIC_PERF_CNTL_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define IOAPIC_PERF_CNTL_EVENT3_SEL_OFFSET      24
#define IOAPIC_PERF_CNTL_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            EVENT0_SEL:8;
    UINT32                            EVENT1_SEL:8;
    UINT32                            EVENT2_SEL:8;
    UINT32                            EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_CNTL_STRUCT;

#define SMN_IOAPIC_PERF_CNTL_ADDRESS    0x14300114UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT0
* Register Description :
* IOAPIC Performance Counter 0
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define IOAPIC_PERF_COUNT0_COUNTER0_OFFSET      0
#define IOAPIC_PERF_COUNT0_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER0:32;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT0_STRUCT;

#define SMN_IOAPIC_PERF_COUNT0_ADDRESS    0x14300118UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT0_UPPER
* Register Description :
* IOAPIC Performance Counter 0
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define IOAPIC_PERF_COUNT0_UPPER_COUNTER0_UPPER_OFFSET      0
#define IOAPIC_PERF_COUNT0_UPPER_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAPIC_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define IOAPIC_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOAPIC_PERF_COUNT0_UPPER_ADDRESS    0x1430011cUL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT1
* Register Description :
* IOAPIC Performance Counter 1
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define IOAPIC_PERF_COUNT1_COUNTER1_OFFSET      0
#define IOAPIC_PERF_COUNT1_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER1:32;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT1_STRUCT;

#define SMN_IOAPIC_PERF_COUNT1_ADDRESS    0x14300120UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT1_UPPER
* Register Description :
* IOAPIC Performance Counter 1
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define IOAPIC_PERF_COUNT1_UPPER_COUNTER1_UPPER_OFFSET      0
#define IOAPIC_PERF_COUNT1_UPPER_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAPIC_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define IOAPIC_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOAPIC_PERF_COUNT1_UPPER_ADDRESS    0x14300124UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT2
* Register Description :
* IOAPIC Performance Counter 2
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define IOAPIC_PERF_COUNT2_COUNTER2_OFFSET      0
#define IOAPIC_PERF_COUNT2_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER2:32;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT2_STRUCT;

#define SMN_IOAPIC_PERF_COUNT2_ADDRESS    0x14300128UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT2_UPPER
* Register Description :
* IOAPIC Performance Counter 2
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define IOAPIC_PERF_COUNT2_UPPER_COUNTER2_UPPER_OFFSET      0
#define IOAPIC_PERF_COUNT2_UPPER_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAPIC_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define IOAPIC_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOAPIC_PERF_COUNT2_UPPER_ADDRESS    0x1430012cUL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT3
* Register Description :
* IOAPIC Performance Counter 3
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define IOAPIC_PERF_COUNT3_COUNTER3_OFFSET      0
#define IOAPIC_PERF_COUNT3_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER3:32;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT3_STRUCT;

#define SMN_IOAPIC_PERF_COUNT3_ADDRESS    0x14300130UL


/***********************************************************
* Register Name : IOAPIC_PERF_COUNT3_UPPER
* Register Description :
* IOAPIC Performance Counter 3
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define IOAPIC_PERF_COUNT3_UPPER_COUNTER3_UPPER_OFFSET      0
#define IOAPIC_PERF_COUNT3_UPPER_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAPIC_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define IOAPIC_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAPIC_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOAPIC_PERF_COUNT3_UPPER_ADDRESS    0x14300134UL


/***********************************************************
* Register Name : IOAPIC_PGSLV_CONTROL
* Register Description :
* power gate control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to enter power gating. The counter decrements every 200ns
#define IOAPIC_PGSLV_CONTROL_PGSLV_Hysteresis_OFFSET      0
#define IOAPIC_PGSLV_CONTROL_PGSLV_Hysteresis_MASK        0x1f

// Bitfield Description : 
#define IOAPIC_PGSLV_CONTROL_Reserved_31_5_OFFSET      5
#define IOAPIC_PGSLV_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            PGSLV_Hysteresis:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} IOAPIC_PGSLV_CONTROL_STRUCT;

#define SMN_IOAPIC_PGSLV_CONTROL_ADDRESS    0x14300140UL


/***********************************************************
* Register Name : IOAPIC_RSMU_HCID
* Register Description :
* rsmu hardware revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu harware revision.
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwRev_OFFSET      0
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwRev_MASK        0x3f

// Bitfield Description : 
#define IOAPIC_RSMU_HCID_Reserved_7_6_OFFSET      6
#define IOAPIC_RSMU_HCID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu harware minor revision.
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwMinVer_OFFSET      8
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOAPIC_RSMU_HCID_Reserved_15_15_OFFSET      15
#define IOAPIC_RSMU_HCID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu harware major revision.
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwMajVer_OFFSET      16
#define IOAPIC_RSMU_HCID_RSMU_HCID_HwMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOAPIC_RSMU_HCID_Reserved_31_23_OFFSET      23
#define IOAPIC_RSMU_HCID_Reserved_31_23_MASK        0xff800000

typedef union {
  struct {
    UINT32                            RSMU_HCID_HwRev:6;
    UINT32                            Reserved_7_6:2;
    UINT32                            RSMU_HCID_HwMinVer:7;
    UINT32                            Reserved_15_15:1;
    UINT32                            RSMU_HCID_HwMajVer:7;
    UINT32                            Reserved_31_23:9;
  } Field;
  UINT32 Value;
} IOAPIC_RSMU_HCID_STRUCT;

#define SMN_IOAPIC_RSMU_HCID_ADDRESS    0x14300144UL


/***********************************************************
* Register Name : IOAPIC_RSMU_SIID
* Register Description :
* rsmu software revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu software revision.
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfRev_OFFSET      0
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfRev_MASK        0x3f

// Bitfield Description : 
#define IOAPIC_RSMU_SIID_Reserved_7_6_OFFSET      6
#define IOAPIC_RSMU_SIID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu software minor revision.
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfMinVer_OFFSET      8
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOAPIC_RSMU_SIID_Reserved_15_15_OFFSET      15
#define IOAPIC_RSMU_SIID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu software major revision.
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfMajVer_OFFSET      16
#define IOAPIC_RSMU_SIID_RSMU_SIID_SwIfMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOAPIC_RSMU_SIID_Reserved_31_23_OFFSET      23
#define IOAPIC_RSMU_SIID_Reserved_31_23_MASK        0xff800000

typedef union {
  struct {
    UINT32                            RSMU_SIID_SwIfRev:6;
    UINT32                            Reserved_7_6:2;
    UINT32                            RSMU_SIID_SwIfMinVer:7;
    UINT32                            Reserved_15_15:1;
    UINT32                            RSMU_SIID_SwIfMajVer:7;
    UINT32                            Reserved_31_23:9;
  } Field;
  UINT32 Value;
} IOAPIC_RSMU_SIID_STRUCT;

#define SMN_IOAPIC_RSMU_SIID_ADDRESS    0x14300148UL


/***********************************************************
* Register Name : IOAPIC_SCRATCH_0
* Register Description :
* Scratch Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 32 scratch bits.
#define IOAPIC_SCRATCH_0_Scratch_0_OFFSET      0
#define IOAPIC_SCRATCH_0_Scratch_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Scratch_0:32;
  } Field;
  UINT32 Value;
} IOAPIC_SCRATCH_0_STRUCT;

#define SMN_IOAPIC_SCRATCH_0_ADDRESS    0x143000f8UL


/***********************************************************
* Register Name : IOAPIC_SCRATCH_1
* Register Description :
* Scratch Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 32 scratch bits.
#define IOAPIC_SCRATCH_1_Scratch_1_OFFSET      0
#define IOAPIC_SCRATCH_1_Scratch_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Scratch_1:32;
  } Field;
  UINT32 Value;
} IOAPIC_SCRATCH_1_STRUCT;

#define SMN_IOAPIC_SCRATCH_1_ADDRESS    0x143000fcUL


/***********************************************************
* Register Name : IOAPIC_SDP_PORT_CONTROL
* Register Description :
* sdp port control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to deassert SDP ClkReq when block is idle.
#define IOAPIC_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_OFFSET      0
#define IOAPIC_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_MASK        0xff

// Bitfield Description : 
#define IOAPIC_SDP_PORT_CONTROL_Reserved_31_8_OFFSET      8
#define IOAPIC_SDP_PORT_CONTROL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Port_Disconnect_Hysteresis:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOAPIC_SDP_PORT_CONTROL_STRUCT;

#define SMN_IOAPIC_SDP_PORT_CONTROL_ADDRESS    0x14300110UL


/***********************************************************
* Register Name : IOAPIC_SERIAL_IRQ_STATUS
* Register Description :
* IRQ Status.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Shows the status of the 32 IOAPIC interrupt table/pins.
#define IOAPIC_SERIAL_IRQ_STATUS_Internal_irq_sts_OFFSET      0
#define IOAPIC_SERIAL_IRQ_STATUS_Internal_irq_sts_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Internal_irq_sts:32;
  } Field;
  UINT32 Value;
} IOAPIC_SERIAL_IRQ_STATUS_STRUCT;

#define SMN_IOAPIC_SERIAL_IRQ_STATUS_ADDRESS    0x143000c4UL


/***********************************************************
* Register Name : IOAPIC_VERSION_REGISTER
* Register Description :
* Version Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : PCI 2.2 compliant.
#define IOAPIC_VERSION_REGISTER_Version_OFFSET      0
#define IOAPIC_VERSION_REGISTER_Version_MASK        0xff

// Bitfield Description : 
#define IOAPIC_VERSION_REGISTER_Reserved_14_8_OFFSET      8
#define IOAPIC_VERSION_REGISTER_Reserved_14_8_MASK        0x7f00

// Bitfield Description : IRQ pin assertion supported.
#define IOAPIC_VERSION_REGISTER_PRQ_OFFSET      15
#define IOAPIC_VERSION_REGISTER_PRQ_MASK        0x8000

// Bitfield Description : The maximum number of redirection entries minus one.
#define IOAPIC_VERSION_REGISTER_Max_Redirection_Entries_OFFSET      16
#define IOAPIC_VERSION_REGISTER_Max_Redirection_Entries_MASK        0xff0000

// Bitfield Description : 
#define IOAPIC_VERSION_REGISTER_Reserved_31_24_OFFSET      24
#define IOAPIC_VERSION_REGISTER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Version:8;
    UINT32                            Reserved_14_8:7;
    UINT32                            PRQ:1;
    UINT32                            Max_Redirection_Entries:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAPIC_VERSION_REGISTER_STRUCT;

#define SMN_IOAPIC_VERSION_REGISTER_ADDRESS    0x2801004UL


/***********************************************************
* Register Name : IRQ_PIN_ASSERTION_REGISTER
* Register Description :
* Interrupt Request Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register triggers an interrupt associated with the specific redirection entry table.
#define IRQ_PIN_ASSERTION_REGISTER_Input_IRQ_OFFSET      0
#define IRQ_PIN_ASSERTION_REGISTER_Input_IRQ_MASK        0xff

// Bitfield Description : 
#define IRQ_PIN_ASSERTION_REGISTER_Reserved_31_8_OFFSET      8
#define IRQ_PIN_ASSERTION_REGISTER_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Input_IRQ:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IRQ_PIN_ASSERTION_REGISTER_IOAPIC_STRUCT;

#define SMN_IRQ_PIN_ASSERTION_REGISTER_ADDRESS    0x2800020UL


/***********************************************************
* Register Name : REDIRECTION_TABLE_ENTRY_HIGH
* Register Description :
* Redirection Table Entry 0 controls
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define REDIRECTION_TABLE_ENTRY_HIGH_Reserved_23_0_OFFSET      0
#define REDIRECTION_TABLE_ENTRY_HIGH_Reserved_23_0_MASK        0xffffff

// Bitfield Description : Bits 19-12 of the address of the interrupt message.
#define REDIRECTION_TABLE_ENTRY_HIGH_Destination_id_OFFSET      24
#define REDIRECTION_TABLE_ENTRY_HIGH_Destination_id_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_23_0:24;
    UINT32                            Destination_id:8;
  } Field;
  UINT32 Value;
} REDIRECTION_TABLE_ENTRY_HIGH_STRUCT;

#define SMN_N0_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801044UL

#define SMN_N10_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801094UL

#define SMN_N11_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280109cUL

#define SMN_N12_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010a4UL

#define SMN_N13_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010acUL

#define SMN_N14_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010b4UL

#define SMN_N15_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010bcUL

#define SMN_N16_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010c4UL

#define SMN_N17_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010ccUL

#define SMN_N18_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010d4UL

#define SMN_N19_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010dcUL

#define SMN_N1_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280104cUL

#define SMN_N20_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010e4UL

#define SMN_N21_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010ecUL

#define SMN_N22_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010f4UL

#define SMN_N23_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x28010fcUL

#define SMN_N24_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801104UL

#define SMN_N25_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280110cUL

#define SMN_N26_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801114UL

#define SMN_N27_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280111cUL

#define SMN_N28_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801124UL

#define SMN_N29_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280112cUL

#define SMN_N2_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801054UL

#define SMN_N30_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801134UL

#define SMN_N31_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280113cUL

#define SMN_N3_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280105cUL

#define SMN_N4_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801064UL

#define SMN_N5_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280106cUL

#define SMN_N6_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801074UL

#define SMN_N7_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280107cUL

#define SMN_N8_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x2801084UL

#define SMN_N9_REDIRECTION_TABLE_ENTRY_HIGH_ADDRESS    0x280108cUL


/***********************************************************
* Register Name : REDIRECTION_TABLE_ENTRY_LOW
* Register Description :
* Redirection Table Entry 0 controls
* Visibility : 0x2
************************************************************/

// Bitfield Description : Interrupt vector assoicated with this interrupt.
#define REDIRECTION_TABLE_ENTRY_LOW_Vector_OFFSET      0
#define REDIRECTION_TABLE_ENTRY_LOW_Vector_MASK        0xff

// Bitfield Description : Delivery Mode.
#define REDIRECTION_TABLE_ENTRY_LOW_Delivery_Mode_OFFSET      8
#define REDIRECTION_TABLE_ENTRY_LOW_Delivery_Mode_MASK        0x700

// Bitfield Description : Destination Mode.
#define REDIRECTION_TABLE_ENTRY_LOW_Destination_Mode_OFFSET      11
#define REDIRECTION_TABLE_ENTRY_LOW_Destination_Mode_MASK        0x800

// Bitfield Description : Delivery Status.
#define REDIRECTION_TABLE_ENTRY_LOW_Delivery_status_OFFSET      12
#define REDIRECTION_TABLE_ENTRY_LOW_Delivery_status_MASK        0x1000

// Bitfield Description : Interrupt Pin Polarity.
#define REDIRECTION_TABLE_ENTRY_LOW_Interrupt_Pin_Polarity_OFFSET      13
#define REDIRECTION_TABLE_ENTRY_LOW_Interrupt_Pin_Polarity_MASK        0x2000

// Bitfield Description : Used for level triggered interrupts only, set when message delivered, cleared by EOI special cycle transaction or write to EOI Register.
#define REDIRECTION_TABLE_ENTRY_LOW_Remote_IRP_OFFSET      14
#define REDIRECTION_TABLE_ENTRY_LOW_Remote_IRP_MASK        0x4000

// Bitfield Description : Triggering Mode.
#define REDIRECTION_TABLE_ENTRY_LOW_Trigger_Mode_OFFSET      15
#define REDIRECTION_TABLE_ENTRY_LOW_Trigger_Mode_MASK        0x8000

// Bitfield Description : Masks the interrupt injection at the input of the device.
#define REDIRECTION_TABLE_ENTRY_LOW_Mask_OFFSET      16
#define REDIRECTION_TABLE_ENTRY_LOW_Mask_MASK        0x10000

// Bitfield Description : 
#define REDIRECTION_TABLE_ENTRY_LOW_Reserved_31_17_OFFSET      17
#define REDIRECTION_TABLE_ENTRY_LOW_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            Vector:8;
    UINT32                            Delivery_Mode:3;
    UINT32                            Destination_Mode:1;
    UINT32                            Delivery_status:1;
    UINT32                            Interrupt_Pin_Polarity:1;
    UINT32                            Remote_IRP:1;
    UINT32                            Trigger_Mode:1;
    UINT32                            Mask:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} REDIRECTION_TABLE_ENTRY_LOW_STRUCT;

#define SMN_N0_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801040UL

#define SMN_N10_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801090UL

#define SMN_N11_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801098UL

#define SMN_N12_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010a0UL

#define SMN_N13_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010a8UL

#define SMN_N14_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010b0UL

#define SMN_N15_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010b8UL

#define SMN_N16_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010c0UL

#define SMN_N17_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010c8UL

#define SMN_N18_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010d0UL

#define SMN_N19_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010d8UL

#define SMN_N1_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801048UL

#define SMN_N20_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010e0UL

#define SMN_N21_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010e8UL

#define SMN_N22_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010f0UL

#define SMN_N23_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x28010f8UL

#define SMN_N24_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801100UL

#define SMN_N25_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801108UL

#define SMN_N26_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801110UL

#define SMN_N27_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801118UL

#define SMN_N28_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801120UL

#define SMN_N29_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801128UL

#define SMN_N2_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801050UL

#define SMN_N30_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801130UL

#define SMN_N31_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801138UL

#define SMN_N3_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801058UL

#define SMN_N4_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801060UL

#define SMN_N5_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801068UL

#define SMN_N6_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801070UL

#define SMN_N7_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801078UL

#define SMN_N8_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801080UL

#define SMN_N9_REDIRECTION_TABLE_ENTRY_LOW_ADDRESS    0x2801088UL

#endif /* _IOAPIC_H_ */
