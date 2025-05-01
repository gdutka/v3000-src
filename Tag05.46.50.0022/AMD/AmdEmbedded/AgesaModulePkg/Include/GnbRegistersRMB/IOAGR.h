/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IOAGR_H_
#define _IOAGR_H_


/***********************************************************
* Register Name : IOAGR_EARLY_WAKE_UP_EN
* Register Description :
* sdp port control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit enables early OrigClkReq for a upstream client.
#define IOAGR_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_OFFSET      0
#define IOAGR_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_MASK        0x1
#define IOAGR_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_DEFAULT     0x1

// Bitfield Description : 
#define IOAGR_EARLY_WAKE_UP_EN_Reserved_15_1_OFFSET      1
#define IOAGR_EARLY_WAKE_UP_EN_Reserved_15_1_MASK        0xfffe

// Bitfield Description : These bits enables early OrigClkReq for a downstream client, and multiple bits can be set.
#define IOAGR_EARLY_WAKE_UP_EN_HostEnableEarlyClkReq_OFFSET      16
#define IOAGR_EARLY_WAKE_UP_EN_HostEnableEarlyClkReq_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            DMAEnableEarlyClkReq:1;
    UINT32                            Reserved_15_1:15;
    UINT32                            HostEnableEarlyClkReq:16;
  } Field;
  UINT32 Value;
} IOAGR_EARLY_WAKE_UP_EN_STRUCT;

#define SMN_IOAGR_EARLY_WAKE_UP_EN_ADDRESS    0x15b00090UL


/***********************************************************
* Register Name : IOAGR_GLUE_CG_LCLK_CTRL_0
* Register Description :
* IOAGR LCLK Clock Gating Control 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOAGR_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_OFFSET      0
#define IOAGR_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_MASK        0xf

// Bitfield Description : Hysteresis for local clock gating to filter out small turn off changes.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_OFFSET      4
#define IOAGR_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_MASK        0xff0

// Bitfield Description : 
#define IOAGR_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_OFFSET      12
#define IOAGR_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_MASK        0x3ff000

// Bitfield Description : Disable dynamic clock gating for SION_NBIF1_CompRsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for SION_NBIF1_OrigReq.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for SION_PCIE1_CompRsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for SION_PCIE1_OrigReq.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for SION_IOAPIC_CompRsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for SION_IOAPIC_OrigReq.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for SION_intSB_CompRsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for SION_intSB_OrigReq.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for SION_L1IMUIOAGR_CompRsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for SION_L1IMUIOAGR_OrigReq.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_MASK        0x80000000

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
} IOAGR_GLUE_CG_LCLK_CTRL_0_STRUCT;

#define SMN_IOAGR_GLUE_CG_LCLK_CTRL_0_ADDRESS    0x15b00000UL


/***********************************************************
* Register Name : IOAGR_GLUE_CG_LCLK_CTRL_1
* Register Description :
* IOAGR LCLK Clock Gating Control 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOAGR_GLUE_CG_LCLK_CTRL_1_Reserved_21_0_OFFSET      0
#define IOAGR_GLUE_CG_LCLK_CTRL_1_Reserved_21_0_MASK        0x3fffff

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for IOAGR_SION_Perf.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for IOAGR_Perf.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for IOAGR_CfgSlv.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for SION_Credit.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for SION_SeqArb_Rsp.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for SION_SeqArb_Req.
// 0 - Enable Clock Gating
// 1 - Disable Clock Gating.
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_21_0:22;
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
} IOAGR_GLUE_CG_LCLK_CTRL_1_STRUCT;

#define SMN_IOAGR_GLUE_CG_LCLK_CTRL_1_ADDRESS    0x15b00004UL


/***********************************************************
* Register Name : IOAGR_PERF_CNTL
* Register Description :
* IOAGR Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define IOAGR_PERF_CNTL_EVENT0_SEL_OFFSET      0
#define IOAGR_PERF_CNTL_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define IOAGR_PERF_CNTL_EVENT1_SEL_OFFSET      8
#define IOAGR_PERF_CNTL_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define IOAGR_PERF_CNTL_EVENT2_SEL_OFFSET      16
#define IOAGR_PERF_CNTL_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define IOAGR_PERF_CNTL_EVENT3_SEL_OFFSET      24
#define IOAGR_PERF_CNTL_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            EVENT0_SEL:8;
    UINT32                            EVENT1_SEL:8;
    UINT32                            EVENT2_SEL:8;
    UINT32                            EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} IOAGR_PERF_CNTL_STRUCT;

#define SMN_IOAGR_PERF_CNTL_ADDRESS    0x15b00044UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT0
* Register Description :
* IOC Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define IOAGR_PERF_COUNT0_COUNTER0_OFFSET      0
#define IOAGR_PERF_COUNT0_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER0:32;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT0_STRUCT;

#define SMN_IOAGR_PERF_COUNT0_ADDRESS    0x15b00048UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT0_UPPER
* Register Description :
* IOC Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define IOAGR_PERF_COUNT0_UPPER_COUNTER0_UPPER_OFFSET      0
#define IOAGR_PERF_COUNT0_UPPER_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOAGR_PERF_COUNT0_UPPER_ADDRESS    0x15b0004cUL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT1
* Register Description :
* IOC Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define IOAGR_PERF_COUNT1_COUNTER1_OFFSET      0
#define IOAGR_PERF_COUNT1_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER1:32;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT1_STRUCT;

#define SMN_IOAGR_PERF_COUNT1_ADDRESS    0x15b00050UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT1_UPPER
* Register Description :
* IOC Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define IOAGR_PERF_COUNT1_UPPER_COUNTER1_UPPER_OFFSET      0
#define IOAGR_PERF_COUNT1_UPPER_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOAGR_PERF_COUNT1_UPPER_ADDRESS    0x15b00054UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT2
* Register Description :
* IOC Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define IOAGR_PERF_COUNT2_COUNTER2_OFFSET      0
#define IOAGR_PERF_COUNT2_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER2:32;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT2_STRUCT;

#define SMN_IOAGR_PERF_COUNT2_ADDRESS    0x15b00058UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT2_UPPER
* Register Description :
* IOC Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define IOAGR_PERF_COUNT2_UPPER_COUNTER2_UPPER_OFFSET      0
#define IOAGR_PERF_COUNT2_UPPER_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOAGR_PERF_COUNT2_UPPER_ADDRESS    0x15b0005cUL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT3
* Register Description :
* IOC Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define IOAGR_PERF_COUNT3_COUNTER3_OFFSET      0
#define IOAGR_PERF_COUNT3_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER3:32;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT3_STRUCT;

#define SMN_IOAGR_PERF_COUNT3_ADDRESS    0x15b00060UL


/***********************************************************
* Register Name : IOAGR_PERF_COUNT3_UPPER
* Register Description :
* IOC Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define IOAGR_PERF_COUNT3_UPPER_COUNTER3_UPPER_OFFSET      0
#define IOAGR_PERF_COUNT3_UPPER_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOAGR_PERF_COUNT3_UPPER_ADDRESS    0x15b00064UL


/***********************************************************
* Register Name : IOAGR_PGMST_CNTL
* Register Description :
* power gate control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : controls the number of consecutive periods when all PG IPs assert PGMST_PGSLV_pwrGate_ready before initiating power gating. Each period is 1600ns duration
#define IOAGR_PGMST_CNTL_CFG_PG_HYSTERESIS_OFFSET      0
#define IOAGR_PGMST_CNTL_CFG_PG_HYSTERESIS_MASK        0xff

// Bitfield Description : enables dynamic power gating
#define IOAGR_PGMST_CNTL_CFG_PG_EN_OFFSET      8
#define IOAGR_PGMST_CNTL_CFG_PG_EN_MASK        0x100

// Bitfield Description : 
#define IOAGR_PGMST_CNTL_Reserved_9_9_OFFSET      9
#define IOAGR_PGMST_CNTL_Reserved_9_9_MASK        0x200

// Bitfield Description : 0: idleness_counter is disabled, PGMST uses only hysteresis timeout value to enter power gating. Cfg_idleness_count_en > 0: If the internal idleness counter >= Cfg_idleness_count_en, enter power gated state as soon PGSLV becomes idle, without waiting for the hysteresis timeout.
#define IOAGR_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_OFFSET      10
#define IOAGR_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_MASK        0x3c00

// Bitfield Description : 2'x1: enable sending the pwrmgr_int for FW interlock feature, 2'b1x: Enable wakup tile from power gated state using the pwrmgt_int_clear signal
#define IOAGR_PGMST_CNTL_CFG_FW_PG_EXIT_EN_OFFSET      14
#define IOAGR_PGMST_CNTL_CFG_FW_PG_EXIT_EN_MASK        0xc000

// Bitfield Description : 
#define IOAGR_PGMST_CNTL_Reserved_31_16_OFFSET      16
#define IOAGR_PGMST_CNTL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            CFG_PG_HYSTERESIS:8;
    UINT32                            CFG_PG_EN:1;
    UINT32                            Reserved_9_9:1;
    UINT32                            CFG_IDLENESS_COUNT_EN:4;
    UINT32                            CFG_FW_PG_EXIT_EN:2;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} IOAGR_PGMST_CNTL_STRUCT;

#define SMN_IOAGR_PGMST_CNTL_ADDRESS    0x15b00080UL


/***********************************************************
* Register Name : IOAGR_PGSLV_CNTL
* Register Description :
* power gate control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to enter power gating. The counter decrements every 200ns
#define IOAGR_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_OFFSET      0
#define IOAGR_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_MASK        0x1f

// Bitfield Description : 
#define IOAGR_PGSLV_CNTL_Reserved_31_5_OFFSET      5
#define IOAGR_PGSLV_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            CFG_IDLE_HYSTERESIS:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} IOAGR_PGSLV_CNTL_STRUCT;

#define SMN_IOAGR_PGSLV_CNTL_ADDRESS    0x15b00084UL


/***********************************************************
* Register Name : IOAGR_RSMU_HCID
* Register Description :
* rsmu hardware revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu harware revision.
#define IOAGR_RSMU_HCID_RSMU_HCID_HwRev_OFFSET      0
#define IOAGR_RSMU_HCID_RSMU_HCID_HwRev_MASK        0x3f

// Bitfield Description : 
#define IOAGR_RSMU_HCID_Reserved_7_6_OFFSET      6
#define IOAGR_RSMU_HCID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu harware minor revision.
#define IOAGR_RSMU_HCID_RSMU_HCID_HwMinVer_OFFSET      8
#define IOAGR_RSMU_HCID_RSMU_HCID_HwMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOAGR_RSMU_HCID_Reserved_15_15_OFFSET      15
#define IOAGR_RSMU_HCID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu harware major revision.
#define IOAGR_RSMU_HCID_RSMU_HCID_HwMajVer_OFFSET      16
#define IOAGR_RSMU_HCID_RSMU_HCID_HwMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOAGR_RSMU_HCID_Reserved_31_23_OFFSET      23
#define IOAGR_RSMU_HCID_Reserved_31_23_MASK        0xff800000

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
} IOAGR_RSMU_HCID_STRUCT;

#define SMN_IOAGR_RSMU_HCID_ADDRESS    0x15b00088UL


/***********************************************************
* Register Name : IOAGR_RSMU_SIID
* Register Description :
* rsmu software revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu software revision.
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfRev_OFFSET      0
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfRev_MASK        0x3f

// Bitfield Description : 
#define IOAGR_RSMU_SIID_Reserved_7_6_OFFSET      6
#define IOAGR_RSMU_SIID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu software minor revision.
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfMinVer_OFFSET      8
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOAGR_RSMU_SIID_Reserved_15_15_OFFSET      15
#define IOAGR_RSMU_SIID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu software major revision.
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfMajVer_OFFSET      16
#define IOAGR_RSMU_SIID_RSMU_SIID_SwIfMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOAGR_RSMU_SIID_Reserved_31_23_OFFSET      23
#define IOAGR_RSMU_SIID_Reserved_31_23_MASK        0xff800000

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
} IOAGR_RSMU_SIID_STRUCT;

#define SMN_IOAGR_RSMU_SIID_ADDRESS    0x15b0008cUL


/***********************************************************
* Register Name : IOAGR_SDP_PORT_CONTROL
* Register Description :
* SDP port control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to deassert SDP ClkReq when block is idle.
#define IOAGR_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_OFFSET      0
#define IOAGR_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_MASK        0xff

// Bitfield Description : 
#define IOAGR_SDP_PORT_CONTROL_Reserved_31_8_OFFSET      8
#define IOAGR_SDP_PORT_CONTROL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Port_Disconnect_Hysteresis:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOAGR_SDP_PORT_CONTROL_STRUCT;

#define SMN_IOAGR_SDP_PORT_CONTROL_ADDRESS    0x15b00040UL


/***********************************************************
* Register Name : IOAGR_SION_Client_DataPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of DataPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_OFFSET      0
#define IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_DataPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_ADDRESS    0x15b00468UL

#define SMN_N1_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_ADDRESS    0x15b00868UL

#define SMN_N2_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_ADDRESS    0x15b00c68UL

#define SMN_N3_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_ADDRESS    0x15b01068UL

#define SMN_N4_IOAGR_SION_Client_DataPoolCredit_Alloc_Lower_ADDRESS    0x15b01468UL


/***********************************************************
* Register Name : IOAGR_SION_Client_DataPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of DataPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_OFFSET      0
#define IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_DataPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_ADDRESS    0x15b0046cUL

#define SMN_N1_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_ADDRESS    0x15b0086cUL

#define SMN_N2_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_ADDRESS    0x15b00c6cUL

#define SMN_N3_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_ADDRESS    0x15b0106cUL

#define SMN_N4_IOAGR_SION_Client_DataPoolCredit_Alloc_Upper_ADDRESS    0x15b0146cUL


/***********************************************************
* Register Name : IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of RdRspPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_OFFSET      0
#define IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00470UL

#define SMN_N1_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00870UL

#define SMN_N2_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00c70UL

#define SMN_N3_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_ADDRESS    0x15b01070UL

#define SMN_N4_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Lower_ADDRESS    0x15b01470UL


/***********************************************************
* Register Name : IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of RdRspPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_OFFSET      0
#define IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_ADDRESS    0x15b00474UL

#define SMN_N1_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_ADDRESS    0x15b00874UL

#define SMN_N2_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_ADDRESS    0x15b00c74UL

#define SMN_N3_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_ADDRESS    0x15b01074UL

#define SMN_N4_IOAGR_SION_Client_RdRspPoolCredit_Alloc_Upper_ADDRESS    0x15b01474UL


/***********************************************************
* Register Name : IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of ReqPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_OFFSET      0
#define IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_ADDRESS    0x15b00460UL

#define SMN_N1_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_ADDRESS    0x15b00860UL

#define SMN_N2_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_ADDRESS    0x15b00c60UL

#define SMN_N3_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_ADDRESS    0x15b01060UL

#define SMN_N4_IOAGR_SION_Client_ReqPoolCredit_Alloc_Lower_ADDRESS    0x15b01460UL


/***********************************************************
* Register Name : IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of ReqPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_OFFSET      0
#define IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_ADDRESS    0x15b00464UL

#define SMN_N1_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_ADDRESS    0x15b00864UL

#define SMN_N2_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_ADDRESS    0x15b00c64UL

#define SMN_N3_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_ADDRESS    0x15b01064UL

#define SMN_N4_IOAGR_SION_Client_ReqPoolCredit_Alloc_Upper_ADDRESS    0x15b01464UL


/***********************************************************
* Register Name : IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of WrRspPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_OFFSET      0
#define IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00478UL

#define SMN_N1_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00878UL

#define SMN_N2_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_ADDRESS    0x15b00c78UL

#define SMN_N3_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_ADDRESS    0x15b01078UL

#define SMN_N4_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Lower_ADDRESS    0x15b01478UL


/***********************************************************
* Register Name : IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of WrRspPoolCredit_Alloc
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_OFFSET      0
#define IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_ADDRESS    0x15b0047cUL

#define SMN_N1_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_ADDRESS    0x15b0087cUL

#define SMN_N2_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_ADDRESS    0x15b00c7cUL

#define SMN_N3_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_ADDRESS    0x15b0107cUL

#define SMN_N4_IOAGR_SION_Client_WrRspPoolCredit_Alloc_Upper_ADDRESS    0x15b0147cUL


/***********************************************************
* Register Name : IOAGR_SION_LiveLock_WatchDog_Threshold
* Register Description :
* arbitration Threshold
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold for number of arbitration losses a request/response has before it's priority is increased above other requestors. Used to prevent deadlocks. A value greater than 0x10 should be given.
#define IOAGR_SION_LiveLock_WatchDog_Threshold_IOAGR_SION_LiveLock_WatchDog_Threshold_OFFSET      0
#define IOAGR_SION_LiveLock_WatchDog_Threshold_IOAGR_SION_LiveLock_WatchDog_Threshold_MASK        0xff

// Bitfield Description : 
#define IOAGR_SION_LiveLock_WatchDog_Threshold_Reserved_31_8_OFFSET      8
#define IOAGR_SION_LiveLock_WatchDog_Threshold_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            IOAGR_SION_LiveLock_WatchDog_Threshold:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOAGR_SION_LiveLock_WatchDog_Threshold_STRUCT;

#define SMN_IOAGR_SION_LiveLock_WatchDog_Threshold_ADDRESS    0x15b01480UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_CNT_CNTL0
* Register Description :
* IOAGR SION Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Performance Counters.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_CNT_EN_OFFSET      0
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_CNT_EN_MASK        0x1

// Bitfield Description : Write 1 to load the counter shadow registers. Read back value is always 0.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_WR_OFFSET      1
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_WR_MASK        0x2

// Bitfield Description : Write 1 for Global RESET of ALL counters. Read back value is always 0.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_OFFSET      2
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_MASK        0x4

// Bitfield Description : 
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_7_3_OFFSET      3
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_7_3_MASK        0xf8

// Bitfield Description : Programmable Pulse width for Global Shadow Write Toggle.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_DELAY_OFFSET      8
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_DELAY_MASK        0xf00

// Bitfield Description : 
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_14_12_OFFSET      12
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_14_12_MASK        0x7000

// Bitfield Description : Enables the Programmable Pulse with for Global Shadow Write Toggle.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_DELAY_EN_OFFSET      15
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_SHADOW_DELAY_EN_MASK        0x8000

// Bitfield Description : Programmable Pulse width for Global Perf Reset Toggle.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_DELAY_OFFSET      16
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_DELAY_MASK        0xf0000

// Bitfield Description : 
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_22_20_OFFSET      20
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_22_20_MASK        0x700000

// Bitfield Description : Enables the Programmable Pulse width for Global Perf Reset Toggle.
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_DELAY_EN_OFFSET      23
#define IOAGR_SION_PERF_CNT_CNTL0_IOAGR_SION_PERF_RESET_DELAY_EN_MASK        0x800000

// Bitfield Description : 
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_31_24_OFFSET      24
#define IOAGR_SION_PERF_CNT_CNTL0_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_CNT_EN:1;
    UINT32                            IOAGR_SION_SHADOW_WR:1;
    UINT32                            IOAGR_SION_PERF_RESET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IOAGR_SION_SHADOW_DELAY:4;
    UINT32                            Reserved_14_12:3;
    UINT32                            IOAGR_SION_SHADOW_DELAY_EN:1;
    UINT32                            IOAGR_SION_PERF_RESET_DELAY:4;
    UINT32                            Reserved_22_20:3;
    UINT32                            IOAGR_SION_PERF_RESET_DELAY_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_CNT_CNTL0_STRUCT;

#define SMN_IOAGR_SION_PERF_CNT_CNTL0_ADDRESS    0x15b01484UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_CNT_CNTL1
* Register Description :
* IOAGR SION Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT0_SEL_OFFSET      0
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT1_SEL_OFFSET      8
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT2_SEL_OFFSET      16
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT3_SEL_OFFSET      24
#define IOAGR_SION_PERF_CNT_CNTL1_IOAGR_SION_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_EVENT0_SEL:8;
    UINT32                            IOAGR_SION_EVENT1_SEL:8;
    UINT32                            IOAGR_SION_EVENT2_SEL:8;
    UINT32                            IOAGR_SION_EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_CNT_CNTL1_STRUCT;

#define SMN_IOAGR_SION_PERF_CNT_CNTL1_ADDRESS    0x15b01488UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT0
* Register Description :
* IOAGR SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define IOAGR_SION_PERF_COUNT0_IOAGR_SION_COUNTER0_OFFSET      0
#define IOAGR_SION_PERF_COUNT0_IOAGR_SION_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER0:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT0_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT0_ADDRESS    0x15b0148cUL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT0_UPPER
* Register Description :
* IOAGR SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define IOAGR_SION_PERF_COUNT0_UPPER_IOAGR_SION_COUNTER0_UPPER_OFFSET      0
#define IOAGR_SION_PERF_COUNT0_UPPER_IOAGR_SION_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_SION_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_SION_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT0_UPPER_ADDRESS    0x15b01490UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT1
* Register Description :
* IOAGR SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define IOAGR_SION_PERF_COUNT1_IOAGR_SION_COUNTER1_OFFSET      0
#define IOAGR_SION_PERF_COUNT1_IOAGR_SION_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER1:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT1_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT1_ADDRESS    0x15b01494UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT1_UPPER
* Register Description :
* IOAGR SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define IOAGR_SION_PERF_COUNT1_UPPER_IOAGR_SION_COUNTER1_UPPER_OFFSET      0
#define IOAGR_SION_PERF_COUNT1_UPPER_IOAGR_SION_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_SION_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_SION_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT1_UPPER_ADDRESS    0x15b01498UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT2
* Register Description :
* IOAGR SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define IOAGR_SION_PERF_COUNT2_IOAGR_SION_COUNTER2_OFFSET      0
#define IOAGR_SION_PERF_COUNT2_IOAGR_SION_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER2:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT2_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT2_ADDRESS    0x15b0149cUL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT2_UPPER
* Register Description :
* IOAGR SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define IOAGR_SION_PERF_COUNT2_UPPER_IOAGR_SION_COUNTER2_UPPER_OFFSET      0
#define IOAGR_SION_PERF_COUNT2_UPPER_IOAGR_SION_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_SION_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_SION_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT2_UPPER_ADDRESS    0x15b014a0UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT3
* Register Description :
* IOAGR SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define IOAGR_SION_PERF_COUNT3_IOAGR_SION_COUNTER3_OFFSET      0
#define IOAGR_SION_PERF_COUNT3_IOAGR_SION_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER3:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT3_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT3_ADDRESS    0x15b014a4UL


/***********************************************************
* Register Name : IOAGR_SION_PERF_COUNT3_UPPER
* Register Description :
* IOAGR SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define IOAGR_SION_PERF_COUNT3_UPPER_IOAGR_SION_COUNTER3_UPPER_OFFSET      0
#define IOAGR_SION_PERF_COUNT3_UPPER_IOAGR_SION_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOAGR_SION_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define IOAGR_SION_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOAGR_SION_COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOAGR_SION_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOAGR_SION_PERF_COUNT3_UPPER_ADDRESS    0x15b014a8UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client0_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client0_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client0_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client0_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client0_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00420UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client1_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client1_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client1_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client1_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N1_IOAGR_SION_S0_Client1_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00820UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client2_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client2_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client2_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client2_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N2_IOAGR_SION_S0_Client2_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00c20UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client3_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client3_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client3_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client3_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N3_IOAGR_SION_S0_Client3_WrRsp_BurstTarget_Lower_ADDRESS    0x15b01020UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client4_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client4_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client4_WrRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client4_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N4_IOAGR_SION_S0_Client4_WrRsp_BurstTarget_Lower_ADDRESS    0x15b01420UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00410UL

#define SMN_N1_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00810UL

#define SMN_N2_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00c10UL

#define SMN_N3_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b01010UL

#define SMN_N4_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b01410UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00414UL

#define SMN_N1_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00814UL

#define SMN_N2_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00c14UL

#define SMN_N3_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b01014UL

#define SMN_N4_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b01414UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00418UL

#define SMN_N1_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00818UL

#define SMN_N2_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00c18UL

#define SMN_N3_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b01018UL

#define SMN_N4_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b01418UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0041cUL

#define SMN_N1_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0081cUL

#define SMN_N2_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b00c1cUL

#define SMN_N3_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0101cUL

#define SMN_N4_IOAGR_SION_S0_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0141cUL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_Req_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per virtual channel(VC), per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client_Req_BurstTarget_Lower_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S0_Client_Req_BurstTarget_Lower_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_Req_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_Req_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00400UL

#define SMN_N1_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00800UL

#define SMN_N2_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00c00UL

#define SMN_N3_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_ADDRESS    0x15b01000UL

#define SMN_N4_IOAGR_SION_S0_Client_Req_BurstTarget_Lower_ADDRESS    0x15b01400UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_Req_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client_Req_BurstTarget_Upper_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_Req_BurstTarget_Upper_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_Req_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_Req_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00404UL

#define SMN_N1_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00804UL

#define SMN_N2_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00c04UL

#define SMN_N3_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS    0x15b01004UL

#define SMN_N4_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS    0x15b01404UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_Req_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_Req_TimeSlot_Lower_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S0_Client_Req_TimeSlot_Lower_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_Req_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_Req_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00408UL

#define SMN_N1_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00808UL

#define SMN_N2_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00c08UL

#define SMN_N3_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS    0x15b01008UL

#define SMN_N4_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS    0x15b01408UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_Req_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_Req_TimeSlot_Upper_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_Req_TimeSlot_Upper_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_Req_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_Req_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0040cUL

#define SMN_N1_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0080cUL

#define SMN_N2_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS    0x15b00c0cUL

#define SMN_N3_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0100cUL

#define SMN_N4_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0140cUL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00424UL

#define SMN_N1_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00824UL

#define SMN_N2_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00c24UL

#define SMN_N3_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b01024UL

#define SMN_N4_IOAGR_SION_S0_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b01424UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00428UL

#define SMN_N1_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00828UL

#define SMN_N2_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00c28UL

#define SMN_N3_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b01028UL

#define SMN_N4_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b01428UL


/***********************************************************
* Register Name : IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0042cUL

#define SMN_N1_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0082cUL

#define SMN_N2_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b00c2cUL

#define SMN_N3_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0102cUL

#define SMN_N4_IOAGR_SION_S0_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0142cUL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00440UL

#define SMN_N1_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00840UL

#define SMN_N2_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b00c40UL

#define SMN_N3_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b01040UL

#define SMN_N4_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS    0x15b01440UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00444UL

#define SMN_N1_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00844UL

#define SMN_N2_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b00c44UL

#define SMN_N3_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b01044UL

#define SMN_N4_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS    0x15b01444UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00448UL

#define SMN_N1_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00848UL

#define SMN_N2_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b00c48UL

#define SMN_N3_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b01048UL

#define SMN_N4_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Lower_ADDRESS    0x15b01448UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0044cUL

#define SMN_N1_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0084cUL

#define SMN_N2_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b00c4cUL

#define SMN_N3_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0104cUL

#define SMN_N4_IOAGR_SION_S1_Client_RdRsp_TimeSlot_Upper_ADDRESS    0x15b0144cUL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_Req_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_Req_BurstTarget_Lower_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_Req_BurstTarget_Lower_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_Req_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_Req_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00430UL

#define SMN_N1_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00830UL

#define SMN_N2_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS    0x15b00c30UL

#define SMN_N3_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS    0x15b01030UL

#define SMN_N4_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS    0x15b01430UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_Req_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_Req_BurstTarget_Upper_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_Req_BurstTarget_Upper_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_Req_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_Req_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00434UL

#define SMN_N1_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00834UL

#define SMN_N2_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS    0x15b00c34UL

#define SMN_N3_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS    0x15b01034UL

#define SMN_N4_IOAGR_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS    0x15b01434UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_Req_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S1_Client_Req_TimeSlot_Lower_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_Req_TimeSlot_Lower_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_Req_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_Req_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00438UL

#define SMN_N1_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00838UL

#define SMN_N2_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS    0x15b00c38UL

#define SMN_N3_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS    0x15b01038UL

#define SMN_N4_IOAGR_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS    0x15b01438UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_Req_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_Req_TimeSlot_Upper_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_Req_TimeSlot_Upper_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_Req_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_Req_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0043cUL

#define SMN_N1_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0083cUL

#define SMN_N2_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS    0x15b00c3cUL

#define SMN_N3_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0103cUL

#define SMN_N4_IOAGR_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS    0x15b0143cUL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower
* Register Description :
* Lower 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00450UL

#define SMN_N1_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00850UL

#define SMN_N2_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_ADDRESS    0x15b00c50UL

#define SMN_N3_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_ADDRESS    0x15b01050UL

#define SMN_N4_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Lower_ADDRESS    0x15b01450UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper
* Register Description :
* Upper 32-bit field of BurstTarget
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00454UL

#define SMN_N1_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00854UL

#define SMN_N2_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b00c54UL

#define SMN_N3_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b01054UL

#define SMN_N4_IOAGR_SION_S1_Client_WrRsp_BurstTarget_Upper_ADDRESS    0x15b01454UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower
* Register Description :
* Lower 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_OFFSET      0
#define IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00458UL

#define SMN_N1_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00858UL

#define SMN_N2_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b00c58UL

#define SMN_N3_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b01058UL

#define SMN_N4_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Lower_ADDRESS    0x15b01458UL


/***********************************************************
* Register Name : IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper
* Register Description :
* Upper 32-bit field of TimeSlot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_OFFSET      0
#define IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_STRUCT;

#define SMN_N0_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0045cUL

#define SMN_N1_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0085cUL

#define SMN_N2_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b00c5cUL

#define SMN_N3_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0105cUL

#define SMN_N4_IOAGR_SION_S1_Client_WrRsp_TimeSlot_Upper_ADDRESS    0x15b0145cUL

#endif /* _IOAGR_H_ */
