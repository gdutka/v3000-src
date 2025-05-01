/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _SMU_SMUIO_H_
#define _SMU_SMUIO_H_


/***********************************************************
* Register Name : AVFS0_CNTL
* Register Description :
* Control register for AVFS modules connected to MM controller0, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : 4'hF - Broadcast address
#define AVFS0_CNTL_mmstop_addr_OFFSET      0
#define AVFS0_CNTL_mmstop_addr_MASK        0xf

// Bitfield Description : 1-Read
// 0-Write
#define AVFS0_CNTL_Read_OFFSET      4
#define AVFS0_CNTL_Read_MASK        0x10

// Bitfield Description : 2'b00- 32-bits
// 2'b01- 64-bits
// 2'b10, 2'b11 - RESERVED
#define AVFS0_CNTL_size_OFFSET      5
#define AVFS0_CNTL_size_MASK        0x60

// Bitfield Description : Register address of the AVFS module
#define AVFS0_CNTL_avfs_reg_addr_OFFSET      7
#define AVFS0_CNTL_avfs_reg_addr_MASK        0x780

// Bitfield Description : set to 1, indicates read operation is done or RdData is valid. Gets cleared on write to this register.
#define AVFS0_CNTL_xfer_done_OFFSET      11
#define AVFS0_CNTL_xfer_done_MASK        0x800

// Bitfield Description : Trigger PSM sequence for AC scan testing
#define AVFS0_CNTL_psmen_OFFSET      12
#define AVFS0_CNTL_psmen_MASK        0x1000

// Bitfield Description : Psm Clock Gater signal input
#define AVFS0_CNTL_psmavfs_gater_OFFSET      13
#define AVFS0_CNTL_psmavfs_gater_MASK        0x2000

// Bitfield Description : AVFS MGCG enable
#define AVFS0_CNTL_clock_gating_enable_OFFSET      14
#define AVFS0_CNTL_clock_gating_enable_MASK        0x4000

// Bitfield Description : RESERVED
#define AVFS0_CNTL_Reserved_31_15_OFFSET      15
#define AVFS0_CNTL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            mmstop_addr:4;
    UINT32                            Read:1;
    UINT32                            size:2;
    UINT32                            avfs_reg_addr:4;
    UINT32                            xfer_done:1;
    UINT32                            psmen:1;
    UINT32                            psmavfs_gater:1;
    UINT32                            clock_gating_enable:1;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} AVFS0_CNTL_STRUCT;

#define SMN_AVFS0_CNTL_ADDRESS    0x5a974UL


/***********************************************************
* Register Name : AVFS0_DBG_CNTL
* Register Description :
* Debug control register for AVFS0 ring controller
* Visibility : 0x2
************************************************************/

// Bitfield Description : Debug bus select to AVFS0 module
#define AVFS0_DBG_CNTL_dbg_sel_OFFSET      0
#define AVFS0_DBG_CNTL_dbg_sel_MASK        0x7

// Bitfield Description : 
#define AVFS0_DBG_CNTL_Reserved_31_3_OFFSET      3
#define AVFS0_DBG_CNTL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            dbg_sel:3;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} AVFS0_DBG_CNTL_STRUCT;

#define SMN_AVFS0_DBG_CNTL_ADDRESS    0x5a9dcUL


/***********************************************************
* Register Name : AVFS0_RDATA0
* Register Description :
* AVFS0 Read data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS0 Read data
#define AVFS0_RDATA0_rdata_OFFSET      0
#define AVFS0_RDATA0_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS0_RDATA0_STRUCT;

#define SMN_AVFS0_RDATA0_ADDRESS    0x5a994UL


/***********************************************************
* Register Name : AVFS0_RDATA1
* Register Description :
* AVFS0 Read data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS0 Read data
#define AVFS0_RDATA1_rdata_OFFSET      0
#define AVFS0_RDATA1_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS0_RDATA1_STRUCT;

#define SMN_AVFS0_RDATA1_ADDRESS    0x5a998UL


/***********************************************************
* Register Name : AVFS0_RD_LATENCY
* Register Description :
* Number of cycles after which the first byte of data is valid after sending a Read command. It's per project and per chain, please refer to SMU10 PWR MAS for details.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of cycles after which the first byte of data is valid after sending a Read command. It's per project and per chain, please refer to SMU10 PWR MAS for details. When it's set to 0x0, the latency is auto detected.
#define AVFS0_RD_LATENCY_rd_latency_OFFSET      0
#define AVFS0_RD_LATENCY_rd_latency_MASK        0x3ff

// Bitfield Description : RESERVED
#define AVFS0_RD_LATENCY_Reserved_31_10_OFFSET      10
#define AVFS0_RD_LATENCY_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            rd_latency:10;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} AVFS0_RD_LATENCY_STRUCT;

#define SMN_AVFS0_RD_LATENCY_ADDRESS    0x5a978UL


/***********************************************************
* Register Name : AVFS0_WDATA0
* Register Description :
* AVFS0 Write data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS0 Write data
#define AVFS0_WDATA0_wdata_OFFSET      0
#define AVFS0_WDATA0_wdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            wdata:32;
  } Field;
  UINT32 Value;
} AVFS0_WDATA0_STRUCT;

#define SMN_AVFS0_WDATA0_ADDRESS    0x5a984UL


/***********************************************************
* Register Name : AVFS0_WDATA1
* Register Description :
* AVFS0 Write data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS0 Write data
#define AVFS0_WDATA1_wdata_OFFSET      0
#define AVFS0_WDATA1_wdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            wdata:32;
  } Field;
  UINT32 Value;
} AVFS0_WDATA1_STRUCT;

#define SMN_AVFS0_WDATA1_ADDRESS    0x5a988UL


/***********************************************************
* Register Name : AVFS1_CNTL
* Register Description :
* Control register for AVFS modules connected to MM controller1, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : 4'hF - Broadcast address
#define AVFS1_CNTL_mmstop_addr_OFFSET      0
#define AVFS1_CNTL_mmstop_addr_MASK        0xf

// Bitfield Description : 1-Read
// 0-Write
#define AVFS1_CNTL_Read_OFFSET      4
#define AVFS1_CNTL_Read_MASK        0x10

// Bitfield Description : 2'b00- 32-bits
// 2'b01- 64-bits
// 2'b10, 2'b11 - RESERVED
#define AVFS1_CNTL_size_OFFSET      5
#define AVFS1_CNTL_size_MASK        0x60

// Bitfield Description : Register address of the AVFS module
#define AVFS1_CNTL_avfs_reg_addr_OFFSET      7
#define AVFS1_CNTL_avfs_reg_addr_MASK        0x780

// Bitfield Description : set to 1, indicates read operation is done or RdData is valid. Gets cleared on write to this register.
#define AVFS1_CNTL_xfer_done_OFFSET      11
#define AVFS1_CNTL_xfer_done_MASK        0x800

// Bitfield Description : Trigger PSM sequence for AC scan testing
#define AVFS1_CNTL_psmen_OFFSET      12
#define AVFS1_CNTL_psmen_MASK        0x1000

// Bitfield Description : Psm Clock Gater signal input
#define AVFS1_CNTL_psmavfs_gater_OFFSET      13
#define AVFS1_CNTL_psmavfs_gater_MASK        0x2000

// Bitfield Description : RESERVED
#define AVFS1_CNTL_Reserved_31_14_OFFSET      14
#define AVFS1_CNTL_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            mmstop_addr:4;
    UINT32                            Read:1;
    UINT32                            size:2;
    UINT32                            avfs_reg_addr:4;
    UINT32                            xfer_done:1;
    UINT32                            psmen:1;
    UINT32                            psmavfs_gater:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} AVFS1_CNTL_STRUCT;

#define SMN_AVFS1_CNTL_ADDRESS    0x5a97cUL


/***********************************************************
* Register Name : AVFS1_DBG_CNTL
* Register Description :
* Debug control register for AVFS1 ring controller
* Visibility : 0x2
************************************************************/

// Bitfield Description : Debug bus select to AVFS1 module
#define AVFS1_DBG_CNTL_dbg_sel_OFFSET      0
#define AVFS1_DBG_CNTL_dbg_sel_MASK        0x7

// Bitfield Description : 
#define AVFS1_DBG_CNTL_Reserved_31_3_OFFSET      3
#define AVFS1_DBG_CNTL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            dbg_sel:3;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} AVFS1_DBG_CNTL_STRUCT;

#define SMN_AVFS1_DBG_CNTL_ADDRESS    0x5a9e0UL


/***********************************************************
* Register Name : AVFS1_RDATA0
* Register Description :
* AVFS1 Read data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS1 Read data
#define AVFS1_RDATA0_rdata_OFFSET      0
#define AVFS1_RDATA0_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS1_RDATA0_STRUCT;

#define SMN_AVFS1_RDATA0_ADDRESS    0x5a99cUL


/***********************************************************
* Register Name : AVFS1_RDATA1
* Register Description :
* AVFS1 Read data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS1 Read data
#define AVFS1_RDATA1_rdata_OFFSET      0
#define AVFS1_RDATA1_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS1_RDATA1_STRUCT;

#define SMN_AVFS1_RDATA1_ADDRESS    0x5a9a0UL


/***********************************************************
* Register Name : AVFS1_RD_LATENCY
* Register Description :
* Number of cycles after which the first byte of data is valid after sending a Read command. It's per project and per chain, please refer to SMU10 PWR MAS for details.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of cycles after which the first byte of data is valid after sending a Read command. It's per project and per chain, please refer to SMU10 PWR MAS for details. When it's set to 0x0, the latency is auto detected.
#define AVFS1_RD_LATENCY_rd_latency_OFFSET      0
#define AVFS1_RD_LATENCY_rd_latency_MASK        0x3ff

// Bitfield Description : RESERVED
#define AVFS1_RD_LATENCY_Reserved_31_10_OFFSET      10
#define AVFS1_RD_LATENCY_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            rd_latency:10;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} AVFS1_RD_LATENCY_STRUCT;

#define SMN_AVFS1_RD_LATENCY_ADDRESS    0x5a980UL


/***********************************************************
* Register Name : AVFS1_WDATA0
* Register Description :
* AVFS1 Write data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS1 Write data
#define AVFS1_WDATA0_wdata_OFFSET      0
#define AVFS1_WDATA0_wdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            wdata:32;
  } Field;
  UINT32 Value;
} AVFS1_WDATA0_STRUCT;

#define SMN_AVFS1_WDATA0_ADDRESS    0x5a98cUL


/***********************************************************
* Register Name : AVFS1_WDATA1
* Register Description :
* AVFS1 Write data register
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS1 Write data
#define AVFS1_WDATA1_wdata_OFFSET      0
#define AVFS1_WDATA1_wdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            wdata:32;
  } Field;
  UINT32 Value;
} AVFS1_WDATA1_STRUCT;

#define SMN_AVFS1_WDATA1_ADDRESS    0x5a990UL


/***********************************************************
* Register Name : AVFS_0_RDATA0
* Register Description :
* AVFS0 Read data register0. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA0_rdata_OFFSET      0
#define AVFS_0_RDATA0_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA0_STRUCT;

#define SMN_AVFS_0_RDATA0_ADDRESS    0x5a9a4UL


/***********************************************************
* Register Name : AVFS_0_RDATA1
* Register Description :
* AVFS0 Read data register1. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA1_rdata_OFFSET      0
#define AVFS_0_RDATA1_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA1_STRUCT;

#define SMN_AVFS_0_RDATA1_ADDRESS    0x5a9a8UL


/***********************************************************
* Register Name : AVFS_0_RDATA10
* Register Description :
* AVFS0 Read data register10. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA10_rdata_OFFSET      0
#define AVFS_0_RDATA10_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA10_STRUCT;

#define SMN_AVFS_0_RDATA10_ADDRESS    0x5a9ccUL


/***********************************************************
* Register Name : AVFS_0_RDATA11
* Register Description :
* AVFS0 Read data register11. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA11_rdata_OFFSET      0
#define AVFS_0_RDATA11_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA11_STRUCT;

#define SMN_AVFS_0_RDATA11_ADDRESS    0x5a9d0UL


/***********************************************************
* Register Name : AVFS_0_RDATA2
* Register Description :
* AVFS0 Read data register2. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA2_rdata_OFFSET      0
#define AVFS_0_RDATA2_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA2_STRUCT;

#define SMN_AVFS_0_RDATA2_ADDRESS    0x5a9acUL


/***********************************************************
* Register Name : AVFS_0_RDATA3
* Register Description :
* AVFS0 Read data register3. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA3_rdata_OFFSET      0
#define AVFS_0_RDATA3_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA3_STRUCT;

#define SMN_AVFS_0_RDATA3_ADDRESS    0x5a9b0UL


/***********************************************************
* Register Name : AVFS_0_RDATA4
* Register Description :
* AVFS0 Read data register4. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA4_rdata_OFFSET      0
#define AVFS_0_RDATA4_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA4_STRUCT;

#define SMN_AVFS_0_RDATA4_ADDRESS    0x5a9b4UL


/***********************************************************
* Register Name : AVFS_0_RDATA5
* Register Description :
* AVFS0 Read data register5. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA5_rdata_OFFSET      0
#define AVFS_0_RDATA5_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA5_STRUCT;

#define SMN_AVFS_0_RDATA5_ADDRESS    0x5a9b8UL


/***********************************************************
* Register Name : AVFS_0_RDATA6
* Register Description :
* AVFS0 Read data register6. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA6_rdata_OFFSET      0
#define AVFS_0_RDATA6_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA6_STRUCT;

#define SMN_AVFS_0_RDATA6_ADDRESS    0x5a9bcUL


/***********************************************************
* Register Name : AVFS_0_RDATA7
* Register Description :
* AVFS0 Read data register7. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA7_rdata_OFFSET      0
#define AVFS_0_RDATA7_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA7_STRUCT;

#define SMN_AVFS_0_RDATA7_ADDRESS    0x5a9c0UL


/***********************************************************
* Register Name : AVFS_0_RDATA8
* Register Description :
* AVFS0 Read data register8. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA8_rdata_OFFSET      0
#define AVFS_0_RDATA8_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA8_STRUCT;

#define SMN_AVFS_0_RDATA8_ADDRESS    0x5a9c4UL


/***********************************************************
* Register Name : AVFS_0_RDATA9
* Register Description :
* AVFS0 Read data register9. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS0_RDATA* registers in order for AVFS0 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_0_RDATA9_rdata_OFFSET      0
#define AVFS_0_RDATA9_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_0_RDATA9_STRUCT;

#define SMN_AVFS_0_RDATA9_ADDRESS    0x5a9c8UL


/***********************************************************
* Register Name : AVFS_1_RDATA0
* Register Description :
* AVFS1 Read data register0. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS1_RDATA* registers in order for AVFS1 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_1_RDATA0_rdata_OFFSET      0
#define AVFS_1_RDATA0_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_1_RDATA0_STRUCT;

#define SMN_AVFS_1_RDATA0_ADDRESS    0x5a9d4UL


/***********************************************************
* Register Name : AVFS_1_RDATA1
* Register Description :
* AVFS1 Read data register1. Please note that in broadcast Read mode, Firmware should program the command first and then Read all AVFS1_RDATA* registers in order for AVFS1 chain and each one should be Read only once.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read data
#define AVFS_1_RDATA1_rdata_OFFSET      0
#define AVFS_1_RDATA1_rdata_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            rdata:32;
  } Field;
  UINT32 Value;
} AVFS_1_RDATA1_STRUCT;

#define SMN_AVFS_1_RDATA1_ADDRESS    0x5a9d8UL


/***********************************************************
* Register Name : AVFS_CNTL
* Register Description :
* Control register for AVFS controller in SMUIO, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : AVFS MGCG enable
#define AVFS_CNTL_clock_gating_enable_OFFSET      0
#define AVFS_CNTL_clock_gating_enable_MASK        0x1

// Bitfield Description : RESERVED
#define AVFS_CNTL_Reserved_31_1_OFFSET      1
#define AVFS_CNTL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            clock_gating_enable:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} AVFS_CNTL_STRUCT;

#define SMN_AVFS_CNTL_ADDRESS    0x5a970UL


/***********************************************************
* Register Name : CCX0_GOLDEN_TSC_SHADOW_LOWER
* Register Description :
* This register gets updated with the value of CCX0 Golden TSC(Lower 32-bits) whenever CPU0 PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of CCX0 Golden TSC(Lower 32-bits) whenever CPU0 PWROK asserts
#define CCX0_GOLDEN_TSC_SHADOW_LOWER_CCX0GoldenTscShadowLower_OFFSET      0
#define CCX0_GOLDEN_TSC_SHADOW_LOWER_CCX0GoldenTscShadowLower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CCX0GoldenTscShadowLower:32;
  } Field;
  UINT32 Value;
} CCX0_GOLDEN_TSC_SHADOW_LOWER_STRUCT;

#define SMN_CCX0_GOLDEN_TSC_SHADOW_LOWER_ADDRESS    0x5a80cUL


/***********************************************************
* Register Name : CCX0_GOLDEN_TSC_SHADOW_UPPER
* Register Description :
* This register gets updated with the value of CCX0 Golden TSC(Upper 24-bits) whenever CPU0 PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of CCX0 Golden TSC(Upper 24-bits) whenever CPU0 PWROK asserts
#define CCX0_GOLDEN_TSC_SHADOW_UPPER_CCX0GoldenTscShadowUpper_OFFSET      0
#define CCX0_GOLDEN_TSC_SHADOW_UPPER_CCX0GoldenTscShadowUpper_MASK        0xffffff

// Bitfield Description : 
#define CCX0_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_OFFSET      24
#define CCX0_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CCX0GoldenTscShadowUpper:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} CCX0_GOLDEN_TSC_SHADOW_UPPER_STRUCT;

#define SMN_CCX0_GOLDEN_TSC_SHADOW_UPPER_ADDRESS    0x5a808UL


/***********************************************************
* Register Name : CCXCTRL_CONFIG
* Register Description :
* CCX Control block configuration register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable CCX Control block clock gating (0 = disabled, 1 = enabled)
#define CCXCTRL_CONFIG_EnableClockGating_OFFSET      0
#define CCXCTRL_CONFIG_EnableClockGating_MASK        0x1

// Bitfield Description : 
#define CCXCTRL_CONFIG_Reserved_31_1_OFFSET      1
#define CCXCTRL_CONFIG_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            EnableClockGating:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} CCXCTRL_CONFIG_STRUCT;

#define SMN_CCXCTRL_CONFIG_ADDRESS    0x5a8b4UL


/***********************************************************
* Register Name : CLDO_EXT_CHAIN0_REGISTER_ACCESS
* Register Description :
* This register contains different access fields of Smuio external LDO's for read/write operation, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address of the Register inside LDO
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Addr_OFFSET      0
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Addr_MASK        0x7

// Bitfield Description : 1-Read
// 0-Write
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Read_OFFSET      3
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Read_MASK        0x8

// Bitfield Description : Data to be written to LDO
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_WrData_OFFSET      4
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_WrData_MASK        0xff0

// Bitfield Description : Selected LDO number for read and write, each bit corresponds to a respective LDO, bit 0 = LDO_0;
// support broadcast write, when ldosel is 5'h1F, then all the LDOs in this chain can be written at the same time.
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_ldosel_OFFSET      12
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_ldosel_MASK        0x1f000

// Bitfield Description : RESERVED
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Reserved_22_17_OFFSET      17
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_Reserved_22_17_MASK        0x7e0000

// Bitfield Description : set to 1, indicates read operation is done or RdData is valid. Gets cleared on write to this register.
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_RdDone_OFFSET      23
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_RdDone_MASK        0x800000

// Bitfield Description : Data Read from selected LDO
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_RdData_OFFSET      24
#define CLDO_EXT_CHAIN0_REGISTER_ACCESS_RdData_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Addr:3;
    UINT32                            Read:1;
    UINT32                            WrData:8;
    UINT32                            ldosel:5;
    UINT32                            Reserved_22_17:6;
    UINT32                            RdDone:1;
    UINT32                            RdData:8;
  } Field;
  UINT32 Value;
} CLDO_EXT_CHAIN0_REGISTER_ACCESS_STRUCT;

#define SMN_CLDO_EXT_CHAIN0_REGISTER_ACCESS_ADDRESS    0x5a8e8UL


/***********************************************************
* Register Name : CLDO_EXT_CHAIN1_REGISTER_ACCESS
* Register Description :
* This register contains different access fields of Smuio external LDO's for read/write operation, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address of the Register inside LDO
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Addr_OFFSET      0
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Addr_MASK        0x7

// Bitfield Description : 1-Read
// 0-Write
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Read_OFFSET      3
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Read_MASK        0x8

// Bitfield Description : Data to be written to LDO
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_WrData_OFFSET      4
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_WrData_MASK        0xff0

// Bitfield Description : Selected LDO number for read and write, each bit corresponds to a respective LDO, bit 0 = LDO_0;
// support broadcast write, when ldosel is 5'h1F, then all the LDOs in this chain can be written at the same time.
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_ldosel_OFFSET      12
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_ldosel_MASK        0x1f000

// Bitfield Description : RESERVED
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Reserved_22_17_OFFSET      17
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_Reserved_22_17_MASK        0x7e0000

// Bitfield Description : set to 1, indicates read operation is done or RdData is valid. Gets cleared on write to this register.
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_RdDone_OFFSET      23
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_RdDone_MASK        0x800000

// Bitfield Description : Data Read from selected LDO
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_RdData_OFFSET      24
#define CLDO_EXT_CHAIN1_REGISTER_ACCESS_RdData_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Addr:3;
    UINT32                            Read:1;
    UINT32                            WrData:8;
    UINT32                            ldosel:5;
    UINT32                            Reserved_22_17:6;
    UINT32                            RdDone:1;
    UINT32                            RdData:8;
  } Field;
  UINT32 Value;
} CLDO_EXT_CHAIN1_REGISTER_ACCESS_STRUCT;

#define SMN_CLDO_EXT_CHAIN1_REGISTER_ACCESS_ADDRESS    0x5a8ecUL


/***********************************************************
* Register Name : CORE_ENABLE
* Register Description :
* Core Enable Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=Enable physical core.
// 0=Disable physical core.
#define CORE_ENABLE_CoreEn_OFFSET      0
#define CORE_ENABLE_CoreEn_MASK        0xff

// Bitfield Description : RESERVED.
#define CORE_ENABLE_Reserved_31_8_OFFSET      8
#define CORE_ENABLE_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            CoreEn:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} CORE_ENABLE_STRUCT;

#define SMN_CORE_ENABLE_ADDRESS    0x5a880UL


/***********************************************************
* Register Name : CPUID_FAMILY_MODEL_STEPPING
* Register Description :
* CPUID Family Model and Stepping Identifiers. This value is updated during the boot-sequence
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stepping
#define CPUID_FAMILY_MODEL_STEPPING_Stepping_OFFSET      0
#define CPUID_FAMILY_MODEL_STEPPING_Stepping_MASK        0xf

// Bitfield Description : BaseModel
#define CPUID_FAMILY_MODEL_STEPPING_BaseModel_OFFSET      4
#define CPUID_FAMILY_MODEL_STEPPING_BaseModel_MASK        0xf0

// Bitfield Description : Base Family
#define CPUID_FAMILY_MODEL_STEPPING_BaseFamily_OFFSET      8
#define CPUID_FAMILY_MODEL_STEPPING_BaseFamily_MASK        0xf00

// Bitfield Description : RESERVED
#define CPUID_FAMILY_MODEL_STEPPING_Reserved_15_12_OFFSET      12
#define CPUID_FAMILY_MODEL_STEPPING_Reserved_15_12_MASK        0xf000

// Bitfield Description : Extended Model
#define CPUID_FAMILY_MODEL_STEPPING_ExtModel_OFFSET      16
#define CPUID_FAMILY_MODEL_STEPPING_ExtModel_MASK        0xf0000

// Bitfield Description : Extended Family.
#define CPUID_FAMILY_MODEL_STEPPING_ExtFamily_OFFSET      20
#define CPUID_FAMILY_MODEL_STEPPING_ExtFamily_MASK        0xff00000

// Bitfield Description : 
#define CPUID_FAMILY_MODEL_STEPPING_Reserved_31_28_OFFSET      28
#define CPUID_FAMILY_MODEL_STEPPING_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Stepping:4;
    UINT32                            BaseModel:4;
    UINT32                            BaseFamily:4;
    UINT32                            Reserved_15_12:4;
    UINT32                            ExtModel:4;
    UINT32                            ExtFamily:8;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} CPUID_FAMILY_MODEL_STEPPING_STRUCT;

#define SMN_CPUID_FAMILY_MODEL_STEPPING_ADDRESS    0x5a86cUL


/***********************************************************
* Register Name : CPU_PWROK
* Register Description :
* CPU Pwr ok
* Visibility : 0x2
************************************************************/

// Bitfield Description : VDDCR_CPU pwr ok
#define CPU_PWROK_cpu0_pwrok_OFFSET      0
#define CPU_PWROK_cpu0_pwrok_MASK        0x1

// Bitfield Description : VDDCR_CPU pwr ok raw for CCX0
#define CPU_PWROK_cpu0_pwrokraw_OFFSET      1
#define CPU_PWROK_cpu0_pwrokraw_MASK        0x2

// Bitfield Description : VDDCR_CPU resetn for CCX0
#define CPU_PWROK_cpu0_resetn_OFFSET      2
#define CPU_PWROK_cpu0_resetn_MASK        0x4

// Bitfield Description : CCX clock stop for CCX0
#define CPU_PWROK_cpu0_clock_stop_OFFSET      3
#define CPU_PWROK_cpu0_clock_stop_MASK        0x8

// Bitfield Description : CCX clock gate for CCX0. warm/cold reset default 0.
// 1: gate CCX0 REFCLK
// 0: not gate CCX0 REFCLK
#define CPU_PWROK_cpu0_sw_clock_gate_OFFSET      4
#define CPU_PWROK_cpu0_sw_clock_gate_MASK        0x10

// Bitfield Description : RESERVED
#define CPU_PWROK_Reserved_31_5_OFFSET      5
#define CPU_PWROK_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            cpu0_pwrok:1;
    UINT32                            cpu0_pwrokraw:1;
    UINT32                            cpu0_resetn:1;
    UINT32                            cpu0_clock_stop:1;
    UINT32                            cpu0_sw_clock_gate:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} CPU_PWROK_STRUCT;

#define SMN_CPU_PWROK_ADDRESS    0x5a884UL


/***********************************************************
* Register Name : GFX_GAP_PWROK
* Register Description :
* GFX GAP PWROK
* Visibility : 0x2
************************************************************/

// Bitfield Description : GFX GAP PWROK
#define GFX_GAP_PWROK_gfx_gap_pwrok_OFFSET      0
#define GFX_GAP_PWROK_gfx_gap_pwrok_MASK        0x1

// Bitfield Description : 
#define GFX_GAP_PWROK_Reserved_31_1_OFFSET      1
#define GFX_GAP_PWROK_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            gfx_gap_pwrok:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} GFX_GAP_PWROK_STRUCT;

#define SMN_GFX_GAP_PWROK_ADDRESS    0x5a8b0UL


/***********************************************************
* Register Name : GFX_GOLDEN_TSC_SHADOW_LOWER
* Register Description :
* This register gets updated with the value of Golden TSC(Lower 32-bits) whenever GFX PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of Golden TSC(Lower 32-bits) whenever GFX PWROK asserts
#define GFX_GOLDEN_TSC_SHADOW_LOWER_GfxGoldenTscShadowLower_OFFSET      0
#define GFX_GOLDEN_TSC_SHADOW_LOWER_GfxGoldenTscShadowLower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            GfxGoldenTscShadowLower:32;
  } Field;
  UINT32 Value;
} GFX_GOLDEN_TSC_SHADOW_LOWER_STRUCT;

#define SMN_GFX_GOLDEN_TSC_SHADOW_LOWER_ADDRESS    0x5a8a0UL


/***********************************************************
* Register Name : GFX_GOLDEN_TSC_SHADOW_UPPER
* Register Description :
* This register gets updated with the value of Golden TSC(Upper 24-bits) whenever GFX PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of Golden TSC(Upper 24-bits) whenever GFX PWROK asserts
#define GFX_GOLDEN_TSC_SHADOW_UPPER_GfxGoldenTscShadowUpper_OFFSET      0
#define GFX_GOLDEN_TSC_SHADOW_UPPER_GfxGoldenTscShadowUpper_MASK        0xffffff

// Bitfield Description : 
#define GFX_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_OFFSET      24
#define GFX_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GfxGoldenTscShadowUpper:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} GFX_GOLDEN_TSC_SHADOW_UPPER_STRUCT;

#define SMN_GFX_GOLDEN_TSC_SHADOW_UPPER_ADDRESS    0x5a89cUL


/***********************************************************
* Register Name : GOLDEN_TSC_COUNT_LOWER
* Register Description :
* This register gets updated with the value of free running Golden TSC(Lower 32-bits)
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of free running Golden TSC(Lower 32-bits)
#define GOLDEN_TSC_COUNT_LOWER_GoldenTscCountLower_OFFSET      0
#define GOLDEN_TSC_COUNT_LOWER_GoldenTscCountLower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            GoldenTscCountLower:32;
  } Field;
  UINT32 Value;
} GOLDEN_TSC_COUNT_LOWER_STRUCT;

#define SMN_GOLDEN_TSC_COUNT_LOWER_ADDRESS    0x5a898UL


/***********************************************************
* Register Name : GOLDEN_TSC_COUNT_UPPER
* Register Description :
* This register gets updated with the value of free running Golden TSC(Upper 24-bits)
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of free running Golden TSC(Upper 24-bits)
#define GOLDEN_TSC_COUNT_UPPER_GoldenTscCountUpper_OFFSET      0
#define GOLDEN_TSC_COUNT_UPPER_GoldenTscCountUpper_MASK        0xffffff

// Bitfield Description : 
#define GOLDEN_TSC_COUNT_UPPER_Reserved_31_24_OFFSET      24
#define GOLDEN_TSC_COUNT_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GoldenTscCountUpper:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} GOLDEN_TSC_COUNT_UPPER_STRUCT;

#define SMN_GOLDEN_TSC_COUNT_UPPER_ADDRESS    0x5a894UL


/***********************************************************
* Register Name : GOLDEN_TSC_INCREMENT_LOWER
* Register Description :
* This register contains the Lower 32-bit value to be incremented for Golden TSC, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register contains the Lower 32-bit value to be incremented for Golden TSC
#define GOLDEN_TSC_INCREMENT_LOWER_GoldenTscIncrementLower_OFFSET      0
#define GOLDEN_TSC_INCREMENT_LOWER_GoldenTscIncrementLower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            GoldenTscIncrementLower:32;
  } Field;
  UINT32 Value;
} GOLDEN_TSC_INCREMENT_LOWER_STRUCT;

#define SMN_GOLDEN_TSC_INCREMENT_LOWER_ADDRESS    0x5a814UL


/***********************************************************
* Register Name : GOLDEN_TSC_INCREMENT_UPPER
* Register Description :
* This register contains the Upper 24-bit value to be incremented for Golden TSC, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register contains the Upper 24-bit value to be incremented for Golden TSC
#define GOLDEN_TSC_INCREMENT_UPPER_GoldenTscIncrementUpper_OFFSET      0
#define GOLDEN_TSC_INCREMENT_UPPER_GoldenTscIncrementUpper_MASK        0xffffff

// Bitfield Description : 
#define GOLDEN_TSC_INCREMENT_UPPER_Reserved_31_24_OFFSET      24
#define GOLDEN_TSC_INCREMENT_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GoldenTscIncrementUpper:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} GOLDEN_TSC_INCREMENT_UPPER_STRUCT;

#define SMN_GOLDEN_TSC_INCREMENT_UPPER_ADDRESS    0x5a810UL


/***********************************************************
* Register Name : IO_SMUIO_PINSTRAP
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : AUD_PORT_CONN pinstrap
#define IO_SMUIO_PINSTRAP_AUD_PORT_CONN_OFFSET      0
#define IO_SMUIO_PINSTRAP_AUD_PORT_CONN_MASK        0x7

// Bitfield Description : AUD pinstrap
#define IO_SMUIO_PINSTRAP_AUD_OFFSET      3
#define IO_SMUIO_PINSTRAP_AUD_MASK        0x18

// Bitfield Description : 
#define IO_SMUIO_PINSTRAP_Reserved_31_5_OFFSET      5
#define IO_SMUIO_PINSTRAP_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            AUD_PORT_CONN:3;
    UINT32                            AUD:2;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} IO_SMUIO_PINSTRAP_STRUCT;

#define SMN_IO_SMUIO_PINSTRAP_ADDRESS    0x5aef4UL


/***********************************************************
* Register Name : IP_DISCOVERY_VERSION
* Register Description :
* IP discovery version
* Visibility : 0x2
************************************************************/

// Bitfield Description : The value in this register will be incremented only if the IP discovery process definition changes.
#define IP_DISCOVERY_VERSION_IP_DISCOVERY_VERSION_OFFSET      0
#define IP_DISCOVERY_VERSION_IP_DISCOVERY_VERSION_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IP_DISCOVERY_VERSION:32;
  } Field;
  UINT32 Value;
} IP_DISCOVERY_VERSION_STRUCT;

#define SMN_IP_DISCOVERY_VERSION_ADDRESS    0x5a800UL


/***********************************************************
* Register Name : LDO_CTRL
* Register Description :
* cldo controll signals
* Visibility : 0x2
************************************************************/

// Bitfield Description : clock gating enable
#define LDO_CTRL_clock_gating_enable_OFFSET      0
#define LDO_CTRL_clock_gating_enable_MASK        0x1

// Bitfield Description : 
#define LDO_CTRL_Reserved_31_1_OFFSET      1
#define LDO_CTRL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            clock_gating_enable:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} LDO_CTRL_STRUCT;

#define SMN_LDO_CTRL_ADDRESS    0x5a8e0UL


/***********************************************************
* Register Name : LDO_TIMING
* Register Description :
* This register contains different access fields of LDO for read/write operation, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : [LOCAL]LDO_TIMING.  Die=RMB IP-Fuse-Name=LDO_TIMING Consumer-IP=SMUIO IP-Instance=SMUIO Category=LOCAL__LDO Size=26 Owner=Fei, Fei Write-Dis=N Secure=NS JTAG=21388:21363 Start-Row=667 Start-Bit=19 End-Row=668 End-Bit=668
#define LDO_TIMING_SMUIO_LDO_TIMING_OFFSET      0
#define LDO_TIMING_SMUIO_LDO_TIMING_MASK        0x3ffffff

// Bitfield Description : RESERVED
#define LDO_TIMING_Reserved_31_26_OFFSET      26
#define LDO_TIMING_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            SMUIO_LDO_TIMING:26;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} LDO_TIMING_STRUCT;

#define SMN_LDO_TIMING_ADDRESS    0x5a8e4UL


/***********************************************************
* Register Name : MSRC001_0064_HI
* Register Description :
* P-state 0 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0064_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0064_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0064_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_HI_PstateEn_OFFSET      31
#define MSRC001_0064_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0064_HI_STRUCT;

#define SMN_MSRC001_0064_HI_ADDRESS    0x5a824UL


/***********************************************************
* Register Name : MSRC001_0064_LO
* Register Description :
* P-state 0 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_LO_CpuFid_OFFSET      0
#define MSRC001_0064_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_LO_CpuDid_OFFSET      8
#define MSRC001_0064_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_LO_CpuVid_OFFSET      14
#define MSRC001_0064_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_LO_IddValue_OFFSET      22
#define MSRC001_0064_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0064_LO_IddDiv_OFFSET      30
#define MSRC001_0064_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0064_LO_STRUCT;

#define SMN_MSRC001_0064_LO_ADDRESS    0x5a820UL


/***********************************************************
* Register Name : MSRC001_0065_HI
* Register Description :
* P-state 1 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0065_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0065_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0065_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_HI_PstateEn_OFFSET      31
#define MSRC001_0065_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0065_HI_STRUCT;

#define SMN_MSRC001_0065_HI_ADDRESS    0x5a82cUL


/***********************************************************
* Register Name : MSRC001_0065_LO
* Register Description :
* P-state 1 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_LO_CpuFid_OFFSET      0
#define MSRC001_0065_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_LO_CpuDid_OFFSET      8
#define MSRC001_0065_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_LO_CpuVid_OFFSET      14
#define MSRC001_0065_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_LO_IddValue_OFFSET      22
#define MSRC001_0065_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0065_LO_IddDiv_OFFSET      30
#define MSRC001_0065_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0065_LO_STRUCT;

#define SMN_MSRC001_0065_LO_ADDRESS    0x5a828UL


/***********************************************************
* Register Name : MSRC001_0066_HI
* Register Description :
* P-state 2 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0066_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0066_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0066_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_HI_PstateEn_OFFSET      31
#define MSRC001_0066_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0066_HI_STRUCT;

#define SMN_MSRC001_0066_HI_ADDRESS    0x5a834UL


/***********************************************************
* Register Name : MSRC001_0066_LO
* Register Description :
* P-state 2 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_LO_CpuFid_OFFSET      0
#define MSRC001_0066_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_LO_CpuDid_OFFSET      8
#define MSRC001_0066_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_LO_CpuVid_OFFSET      14
#define MSRC001_0066_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_LO_IddValue_OFFSET      22
#define MSRC001_0066_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0066_LO_IddDiv_OFFSET      30
#define MSRC001_0066_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0066_LO_STRUCT;

#define SMN_MSRC001_0066_LO_ADDRESS    0x5a830UL


/***********************************************************
* Register Name : MSRC001_0067_HI
* Register Description :
* P-state 3 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0067_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0067_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0067_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_HI_PstateEn_OFFSET      31
#define MSRC001_0067_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0067_HI_STRUCT;

#define SMN_MSRC001_0067_HI_ADDRESS    0x5a83cUL


/***********************************************************
* Register Name : MSRC001_0067_LO
* Register Description :
* P-state 3 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_LO_CpuFid_OFFSET      0
#define MSRC001_0067_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_LO_CpuDid_OFFSET      8
#define MSRC001_0067_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_LO_CpuVid_OFFSET      14
#define MSRC001_0067_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_LO_IddValue_OFFSET      22
#define MSRC001_0067_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0067_LO_IddDiv_OFFSET      30
#define MSRC001_0067_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0067_LO_STRUCT;

#define SMN_MSRC001_0067_LO_ADDRESS    0x5a838UL


/***********************************************************
* Register Name : MSRC001_0068_HI
* Register Description :
* P-state 4 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0068_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0068_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0068_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_HI_PstateEn_OFFSET      31
#define MSRC001_0068_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0068_HI_STRUCT;

#define SMN_MSRC001_0068_HI_ADDRESS    0x5a844UL


/***********************************************************
* Register Name : MSRC001_0068_LO
* Register Description :
* P-state 4 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_LO_CpuFid_OFFSET      0
#define MSRC001_0068_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_LO_CpuDid_OFFSET      8
#define MSRC001_0068_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_LO_CpuVid_OFFSET      14
#define MSRC001_0068_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_LO_IddValue_OFFSET      22
#define MSRC001_0068_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0068_LO_IddDiv_OFFSET      30
#define MSRC001_0068_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0068_LO_STRUCT;

#define SMN_MSRC001_0068_LO_ADDRESS    0x5a840UL


/***********************************************************
* Register Name : MSRC001_0069_HI
* Register Description :
* P-state 5 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_0069_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0069_HI_Reserved_30_1_OFFSET      1
#define MSRC001_0069_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_HI_PstateEn_OFFSET      31
#define MSRC001_0069_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_0069_HI_STRUCT;

#define SMN_MSRC001_0069_HI_ADDRESS    0x5a84cUL


/***********************************************************
* Register Name : MSRC001_0069_LO
* Register Description :
* P-state 5 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_LO_CpuFid_OFFSET      0
#define MSRC001_0069_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_LO_CpuDid_OFFSET      8
#define MSRC001_0069_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_LO_CpuVid_OFFSET      14
#define MSRC001_0069_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_LO_IddValue_OFFSET      22
#define MSRC001_0069_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_0069_LO_IddDiv_OFFSET      30
#define MSRC001_0069_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_0069_LO_STRUCT;

#define SMN_MSRC001_0069_LO_ADDRESS    0x5a848UL


/***********************************************************
* Register Name : MSRC001_006A_HI
* Register Description :
* P-state 6 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_006A_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_006A_HI_Reserved_30_1_OFFSET      1
#define MSRC001_006A_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_HI_PstateEn_OFFSET      31
#define MSRC001_006A_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_006A_HI_STRUCT;

#define SMN_MSRC001_006A_HI_ADDRESS    0x5a854UL


/***********************************************************
* Register Name : MSRC001_006A_LO
* Register Description :
* P-state 6 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_LO_CpuFid_OFFSET      0
#define MSRC001_006A_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_LO_CpuDid_OFFSET      8
#define MSRC001_006A_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_LO_CpuVid_OFFSET      14
#define MSRC001_006A_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_LO_IddValue_OFFSET      22
#define MSRC001_006A_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006A_LO_IddDiv_OFFSET      30
#define MSRC001_006A_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_006A_LO_STRUCT;

#define SMN_MSRC001_006A_LO_ADDRESS    0x5a850UL


/***********************************************************
* Register Name : MSRC001_006B_HI
* Register Description :
* P-state 7 - 63:32
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_HI_CpuVid_MSB_OFFSET      0
#define MSRC001_006B_HI_CpuVid_MSB_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_006B_HI_Reserved_30_1_OFFSET      1
#define MSRC001_006B_HI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : PstateEn
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_HI_PstateEn_OFFSET      31
#define MSRC001_006B_HI_PstateEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CpuVid_MSB:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            PstateEn:1;
  } Field;
  UINT32 Value;
} MSRC001_006B_HI_STRUCT;

#define SMN_MSRC001_006B_HI_ADDRESS    0x5a85cUL


/***********************************************************
* Register Name : MSRC001_006B_LO
* Register Description :
* P-state 7 - 31:0
* Visibility : 0x2
************************************************************/

// Bitfield Description : CpuFid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_LO_CpuFid_OFFSET      0
#define MSRC001_006B_LO_CpuFid_MASK        0xff

// Bitfield Description : CpuDid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_LO_CpuDid_OFFSET      8
#define MSRC001_006B_LO_CpuDid_MASK        0x3f00

// Bitfield Description : CpuVid
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_LO_CpuVid_OFFSET      14
#define MSRC001_006B_LO_CpuVid_MASK        0x3fc000

// Bitfield Description : IddValue
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_LO_IddValue_OFFSET      22
#define MSRC001_006B_LO_IddValue_MASK        0x3fc00000

// Bitfield Description : IddDiv
// Read by SMU/Software
// Written by BIOS.
#define MSRC001_006B_LO_IddDiv_OFFSET      30
#define MSRC001_006B_LO_IddDiv_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CpuFid:8;
    UINT32                            CpuDid:6;
    UINT32                            CpuVid:8;
    UINT32                            IddValue:8;
    UINT32                            IddDiv:2;
  } Field;
  UINT32 Value;
} MSRC001_006B_LO_STRUCT;

#define SMN_MSRC001_006B_LO_ADDRESS    0x5a858UL


/***********************************************************
* Register Name : MSRC001_0292_HI
* Register Description :
* MSRC001_0292 63-32 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit is written by bios to enable / disable PC6.
#define MSRC001_0292_HI_PC6En_OFFSET      0
#define MSRC001_0292_HI_PC6En_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_0292_HI_Reserved_31_1_OFFSET      1
#define MSRC001_0292_HI_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            PC6En:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} MSRC001_0292_HI_STRUCT;

#define SMN_MSRC001_0292_HI_ADDRESS    0x5a868UL


/***********************************************************
* Register Name : MSRC001_0292_LO
* Register Description :
* MSRC001_0292 31-0 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read by Software
// Written by SMU.
#define MSRC001_0292_LO_CurHwPstateLimit_OFFSET      0
#define MSRC001_0292_LO_CurHwPstateLimit_MASK        0x7

// Bitfield Description : Read by Software
// Written by Fuse.
#define MSRC001_0292_LO_StartupPstate_OFFSET      3
#define MSRC001_0292_LO_StartupPstate_MASK        0x38

// Bitfield Description : Read by Software
// Written by Fuse.
#define MSRC001_0292_LO_DFPstateDis_OFFSET      6
#define MSRC001_0292_LO_DFPstateDis_MASK        0x40

// Bitfield Description : Read by Software
// Written by SMU.
#define MSRC001_0292_LO_CurDFVid_OFFSET      7
#define MSRC001_0292_LO_CurDFVid_MASK        0x7f80

// Bitfield Description : Read by Software
// Written by Fuse.
#define MSRC001_0292_LO_MaxCpuCof_OFFSET      15
#define MSRC001_0292_LO_MaxCpuCof_MASK        0x1f8000

// Bitfield Description : Read by Software
// Written by Fuse.
#define MSRC001_0292_LO_MaxDFCof_OFFSET      21
#define MSRC001_0292_LO_MaxDFCof_MASK        0x3e00000

// Bitfield Description : Read by Software
// Written by Fuse.
#define MSRC001_0292_LO_CpbCap_OFFSET      26
#define MSRC001_0292_LO_CpbCap_MASK        0x1c000000

// Bitfield Description : Read by Software
// Written by SMU.
#define MSRC001_0292_LO_CurDFVid_MSB_OFFSET      29
#define MSRC001_0292_LO_CurDFVid_MSB_MASK        0x20000000

// Bitfield Description : RESERVED.
#define MSRC001_0292_LO_Reserved_31_30_OFFSET      30
#define MSRC001_0292_LO_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CurHwPstateLimit:3;
    UINT32                            StartupPstate:3;
    UINT32                            DFPstateDis:1;
    UINT32                            CurDFVid:8;
    UINT32                            MaxCpuCof:6;
    UINT32                            MaxDFCof:5;
    UINT32                            CpbCap:3;
    UINT32                            CurDFVid_MSB:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} MSRC001_0292_LO_STRUCT;

#define SMN_MSRC001_0292_LO_ADDRESS    0x5a864UL


/***********************************************************
* Register Name : MSRC001_02B0_HI
* Register Description :
* CPPC Capability 1 - 63:32 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : RESERVED.
#define MSRC001_02B0_HI_Reserved_31_0_OFFSET      0
#define MSRC001_02B0_HI_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} MSRC001_02B0_HI_STRUCT;

#define SMN_MSRC001_02B0_HI_ADDRESS    0x5a874UL


/***********************************************************
* Register Name : MSRC001_02B0_LO
* Register Description :
* CPPC Capability 1 - 31:0 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lowest Performance, This bit is written by SMU and read by Software.
#define MSRC001_02B0_LO_LowestPerf_OFFSET      0
#define MSRC001_02B0_LO_LowestPerf_MASK        0xff

// Bitfield Description : Lowest Nonlinear Performance, This bit is written by SMU and read by Software.
#define MSRC001_02B0_LO_LowNonLinPerf_OFFSET      8
#define MSRC001_02B0_LO_LowNonLinPerf_MASK        0xff00

// Bitfield Description : Nominal Performance, This bit is written by SMU and read by Software.
#define MSRC001_02B0_LO_NominalPerf_OFFSET      16
#define MSRC001_02B0_LO_NominalPerf_MASK        0xff0000

// Bitfield Description : Highest Performance, This bit is written by SMU and read by Software.
#define MSRC001_02B0_LO_HighestPerf_OFFSET      24
#define MSRC001_02B0_LO_HighestPerf_MASK        0xff000000

typedef union {
  struct {
    UINT32                            LowestPerf:8;
    UINT32                            LowNonLinPerf:8;
    UINT32                            NominalPerf:8;
    UINT32                            HighestPerf:8;
  } Field;
  UINT32 Value;
} MSRC001_02B0_LO_STRUCT;

#define SMN_MSRC001_02B0_LO_ADDRESS    0x5a870UL


/***********************************************************
* Register Name : MSRC001_02B1
* Register Description :
* CPPC Enable - 31:0 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPPC Enable, This bit is written by Software and read by SMU.
#define MSRC001_02B1_CppcEnable_OFFSET      0
#define MSRC001_02B1_CppcEnable_MASK        0x1

// Bitfield Description : RESERVED.
#define MSRC001_02B1_Reserved_31_1_OFFSET      1
#define MSRC001_02B1_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            CppcEnable:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} MSRC001_02B1_STRUCT;

#define SMN_MSRC001_02B1_ADDRESS    0x5a878UL


/***********************************************************
* Register Name : MSRC001_02B3
* Register Description :
* CPPC Request - 31:0 bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : Maximum Performance
#define MSRC001_02B3_MaximumPerformance_OFFSET      0
#define MSRC001_02B3_MaximumPerformance_MASK        0xff

// Bitfield Description : Minimum Performance
#define MSRC001_02B3_MinimumPerformance_OFFSET      8
#define MSRC001_02B3_MinimumPerformance_MASK        0xff00

// Bitfield Description : Desired Performance
#define MSRC001_02B3_DesiredPerformance_OFFSET      16
#define MSRC001_02B3_DesiredPerformance_MASK        0xff0000

// Bitfield Description : Energy Performance Preference
#define MSRC001_02B3_EnergyPerformancePreference_OFFSET      24
#define MSRC001_02B3_EnergyPerformancePreference_MASK        0xff000000

typedef union {
  struct {
    UINT32                            MaximumPerformance:8;
    UINT32                            MinimumPerformance:8;
    UINT32                            DesiredPerformance:8;
    UINT32                            EnergyPerformancePreference:8;
  } Field;
  UINT32 Value;
} MSRC001_02B3_STRUCT;

#define SMN_MSRC001_02B3_ADDRESS    0x5a87cUL


/***********************************************************
* Register Name : ODPR_COMPOUT
* Register Description :
* This register stores odprcompout of maodprcomp
* Visibility : 0x2
************************************************************/

// Bitfield Description : FW can read this register to implement ODPR tuning/correction
#define ODPR_COMPOUT_odpr_compout_OFFSET      0
#define ODPR_COMPOUT_odpr_compout_MASK        0x1

// Bitfield Description : 
#define ODPR_COMPOUT_Reserved_31_1_OFFSET      1
#define ODPR_COMPOUT_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            odpr_compout:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ODPR_COMPOUT_STRUCT;

#define SMN_ODPR_COMPOUT_ADDRESS    0x5a928UL


/***********************************************************
* Register Name : ODPR_COMP_CTRL
* Register Description :
* This register include control signals for maodprcomp
* Visibility : 0x2
************************************************************/

// Bitfield Description : Power down all circuits. 0= all circuits powered off, 1= all circuits powered on
#define ODPR_COMP_CTRL_cal_pd_x_OFFSET      0
#define ODPR_COMP_CTRL_cal_pd_x_MASK        0x1

// Bitfield Description : select bits for current bias of the comparator
#define ODPR_COMP_CTRL_csr_bias_sel_OFFSET      1
#define ODPR_COMP_CTRL_csr_bias_sel_MASK        0x6

// Bitfield Description : RC Select. 0: RC filter corner, 25-50MHz, 1: RC filter corner, 50-100MHz
#define ODPR_COMP_CTRL_csr_rc_sel_OFFSET      3
#define ODPR_COMP_CTRL_csr_rc_sel_MASK        0x8

// Bitfield Description : 10-bit res_ctl broadcasted to the odpr macros with default 196ohm
#define ODPR_COMP_CTRL_res_ctl_comp_OFFSET      4
#define ODPR_COMP_CTRL_res_ctl_comp_MASK        0x3ff0

// Bitfield Description : 
#define ODPR_COMP_CTRL_Reserved_31_14_OFFSET      14
#define ODPR_COMP_CTRL_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            cal_pd_x:1;
    UINT32                            csr_bias_sel:2;
    UINT32                            csr_rc_sel:1;
    UINT32                            res_ctl_comp:10;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} ODPR_COMP_CTRL_STRUCT;

#define SMN_ODPR_COMP_CTRL_ADDRESS    0x5a924UL


/***********************************************************
* Register Name : ODPR_CSR_EN_0_31
* Register Description :
* This register tells which ODPR csr_en to update, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit corresponds to csr_en of respective odpr.csr_en. MSB is 31
#define ODPR_CSR_EN_0_31_odpr_csr_en_0_31_OFFSET      0
#define ODPR_CSR_EN_0_31_odpr_csr_en_0_31_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            odpr_csr_en_0_31:32;
  } Field;
  UINT32 Value;
} ODPR_CSR_EN_0_31_STRUCT;

#define SMN_ODPR_CSR_EN_0_31_ADDRESS    0x5a920UL


/***********************************************************
* Register Name : ODPR_READ_ENABLE
* Register Description :
* write/read odpr switch: 0: write mode,it's default value. 1: read mode
* Visibility : 0x2
************************************************************/

// Bitfield Description : write/read odpr 0~31 switch: 0: write mode,it's default value. 1: read mode
#define ODPR_READ_ENABLE_odpr_read_enable_0_31_OFFSET      0
#define ODPR_READ_ENABLE_odpr_read_enable_0_31_MASK        0x1

// Bitfield Description : 
#define ODPR_READ_ENABLE_Reserved_31_1_OFFSET      1
#define ODPR_READ_ENABLE_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            odpr_read_enable_0_31:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ODPR_READ_ENABLE_STRUCT;

#define SMN_ODPR_READ_ENABLE_ADDRESS    0x5a91cUL


/***********************************************************
* Register Name : ODPR_RES_CTL
* Register Description :
* This register contains res_ctl driven to ODPR, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : 10-bit res_ctl broadcasted to the odpr macros with default 200ohm
#define ODPR_RES_CTL_res_ctl_OFFSET      0
#define ODPR_RES_CTL_res_ctl_MASK        0x3ff

// Bitfield Description : CSR Spare bits. csr_spares[0] indicates OPRD enable, 0: enable, 1: disable
#define ODPR_RES_CTL_csr_spares_OFFSET      10
#define ODPR_RES_CTL_csr_spares_MASK        0x1c00

// Bitfield Description : 
#define ODPR_RES_CTL_Reserved_22_13_OFFSET      13
#define ODPR_RES_CTL_Reserved_22_13_MASK        0x7fe000

// Bitfield Description : RESERVED
#define ODPR_RES_CTL_Reserved_31_23_OFFSET      23
#define ODPR_RES_CTL_Reserved_31_23_MASK        0xff800000

typedef union {
  struct {
    UINT32                            res_ctl:10;
    UINT32                            csr_spares:3;
    UINT32                            Reserved_22_13:10;
    UINT32                            Reserved_31_23:9;
  } Field;
  UINT32 Value;
} ODPR_RES_CTL_STRUCT;

#define SMN_ODPR_RES_CTL_ADDRESS    0x5a914UL


/***********************************************************
* Register Name : ODPR_RES_CTL_READ
* Register Description :
* RES_CTL read value from ODPR macro
* Visibility : 0x2
************************************************************/

// Bitfield Description : 10-bit res_ctl read value from odpr macro
#define ODPR_RES_CTL_READ_res_ctl_read_OFFSET      0
#define ODPR_RES_CTL_READ_res_ctl_read_MASK        0x3ff

// Bitfield Description : 
#define ODPR_RES_CTL_READ_Reserved_31_10_OFFSET      10
#define ODPR_RES_CTL_READ_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            res_ctl_read:10;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} ODPR_RES_CTL_READ_STRUCT;

#define SMN_ODPR_RES_CTL_READ_ADDRESS    0x5a930UL


/***********************************************************
* Register Name : ODPR_SEL_0_31
* Register Description :
* This register tells which ODPR of 0~31 to update, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selection of odpr to update. Multiple odprs can be selected. MSB is 31
#define ODPR_SEL_0_31_odpr_sel_0_31_OFFSET      0
#define ODPR_SEL_0_31_odpr_sel_0_31_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            odpr_sel_0_31:32;
  } Field;
  UINT32 Value;
} ODPR_SEL_0_31_STRUCT;

#define SMN_ODPR_SEL_0_31_ADDRESS    0x5a918UL


/***********************************************************
* Register Name : ODPR_TIMING
* Register Description :
* This register contains control fields of ODPR's timing during update operation, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of SMN cycles allowed res_ctl to stabilize before 'update' is asserted to ODPR
#define ODPR_TIMING_PreUpdateDuration_OFFSET      0
#define ODPR_TIMING_PreUpdateDuration_MASK        0x3f

// Bitfield Description : Duration of Update pulse to ODPR(in SMN cycles)
#define ODPR_TIMING_UpdateDuration_OFFSET      6
#define ODPR_TIMING_UpdateDuration_MASK        0xfc0

// Bitfield Description : Number of SMN cycles data,addr and read should be stable after de-assertion of Sel
#define ODPR_TIMING_PostUpdateDuration_OFFSET      12
#define ODPR_TIMING_PostUpdateDuration_MASK        0x3f000

// Bitfield Description : RESERVED
#define ODPR_TIMING_Reserved_31_18_OFFSET      18
#define ODPR_TIMING_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            PreUpdateDuration:6;
    UINT32                            UpdateDuration:6;
    UINT32                            PostUpdateDuration:6;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} ODPR_TIMING_STRUCT;

#define SMN_ODPR_TIMING_ADDRESS    0x5a910UL


/***********************************************************
* Register Name : PWRMGT_STATUS
* Register Description :
* Power Management status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Power Management status register. Written by MP1 FW. This status is fed to TDR for JTAG access.
// [0]:  CPUOFF
// [1]:  GFXOFF
// [2]:  VDDOFF
// [3]:  CPUPWROK
// [4]:  CPUPWROKRAW
// [16]: USB0_PG
// [17]: USB1_PG
// [18]: USB2_PG
// [19]: USB3_PG
// [20]: USB4_PG
#define PWRMGT_STATUS_pwrmgt_status_OFFSET      0
#define PWRMGT_STATUS_pwrmgt_status_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            pwrmgt_status:32;
  } Field;
  UINT32 Value;
} PWRMGT_STATUS_STRUCT;

#define SMN_PWRMGT_STATUS_ADDRESS    0x5a890UL


/***********************************************************
* Register Name : PWROK_REFCLK_GAP_CYCLES
* Register Description :
* This register contains the Pre & Post PWROK assertion REFCLK gap values in REFCLK cycles, This is accessible through SMN
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register contains the Pre PWROK assertion REFCLK gap value in REFCLK cycles
#define PWROK_REFCLK_GAP_CYCLES_Pwrok_PreAssertion_clkgap_cycles_OFFSET      0
#define PWROK_REFCLK_GAP_CYCLES_Pwrok_PreAssertion_clkgap_cycles_MASK        0xff

// Bitfield Description : This register contains the Post PWROK assertion REFCLK gap value in REFCLK cycles
#define PWROK_REFCLK_GAP_CYCLES_Pwrok_PostAssertion_clkgap_cycles_OFFSET      8
#define PWROK_REFCLK_GAP_CYCLES_Pwrok_PostAssertion_clkgap_cycles_MASK        0xff00

// Bitfield Description : 
#define PWROK_REFCLK_GAP_CYCLES_Reserved_31_16_OFFSET      16
#define PWROK_REFCLK_GAP_CYCLES_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Pwrok_PreAssertion_clkgap_cycles:8;
    UINT32                            Pwrok_PostAssertion_clkgap_cycles:8;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PWROK_REFCLK_GAP_CYCLES_STRUCT;

#define SMN_PWROK_REFCLK_GAP_CYCLES_ADDRESS    0x5a804UL


/***********************************************************
* Register Name : PWR_DISP_TIMER2_CONTROL
* Register Description :
* TIMER2 control register.It could have 17 copies in virtualization chip
* Visibility : 0x2
************************************************************/

// Bitfield Description : PWR_DISP_TIMER2_CONTROL
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_COUNT_OFFSET      0
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_COUNT_MASK        0x1ffffff

// Bitfield Description : Start the timer, write-one trigger=1, No action=0
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_ENABLE_OFFSET      25
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_ENABLE_MASK        0x2000000

// Bitfield Description : Stop the timer, write-one trigger=1, No action=0
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_DISABLE_OFFSET      26
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_DISABLE_MASK        0x4000000

// Bitfield Description : Display Countdown Timer can generate HW interrupt when count reached=0, Display Countdown Timer cannot generate HW interrupt = 1
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_MASK_OFFSET      27
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_MASK_MASK        0x8000000

// Bitfield Description : Clear the interrupt line when timer is in one-shot mode, or periodic mode with level-based type=1, No action=0
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_STAT_AK_OFFSET      28
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_STAT_AK_MASK        0x10000000

// Bitfield Description : For periodic mode, send level-based interrupt. N/A for one-shot mode=1, For periodic mode, send pulse-based interrupt=0
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_TYPE_OFFSET      29
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_TYPE_MASK        0x20000000

// Bitfield Description : Periodic mode. Timer restarts itself when it gets expired=1, One-time shot mode. Timer stops when it gets expired
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_MODE_OFFSET      30
#define PWR_DISP_TIMER2_CONTROL_DISP_TIMER_INT_MODE_MASK        0x40000000

// Bitfield Description : 
#define PWR_DISP_TIMER2_CONTROL_Reserved_31_31_OFFSET      31
#define PWR_DISP_TIMER2_CONTROL_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            DISP_TIMER_INT_COUNT:25;
    UINT32                            DISP_TIMER_INT_ENABLE:1;
    UINT32                            DISP_TIMER_INT_DISABLE:1;
    UINT32                            DISP_TIMER_INT_MASK:1;
    UINT32                            DISP_TIMER_INT_STAT_AK:1;
    UINT32                            DISP_TIMER_INT_TYPE:1;
    UINT32                            DISP_TIMER_INT_MODE:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PWR_DISP_TIMER2_CONTROL_STRUCT;

#define SMN_PWR_DISP_TIMER2_CONTROL_ADDRESS    0x5ac7cUL


/***********************************************************
* Register Name : PWR_DISP_TIMER2_DEBUG
* Register Description :
* DISPLAY TIMER2 debug register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Timer interrupt counter running=1, Timer interrupt count not running=0
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_RUNNING_OFFSET      0
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_RUNNING_MASK        0x1

// Bitfield Description : Interrupt line is asserted=1, Interrupt line is not asserted=0
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_STAT_OFFSET      1
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_STAT_MASK        0x2

// Bitfield Description : Interrupt line is asserted and sent to IH=1, No interrupt is sent to=0
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_OFFSET      2
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_INT_MASK        0x4

// Bitfield Description : 
#define PWR_DISP_TIMER2_DEBUG_Reserved_6_3_OFFSET      3
#define PWR_DISP_TIMER2_DEBUG_Reserved_6_3_MASK        0x78

// Bitfield Description : read only
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_RUN_VAL_OFFSET      7
#define PWR_DISP_TIMER2_DEBUG_DISP_TIMER_RUN_VAL_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            DISP_TIMER_INT_RUNNING:1;
    UINT32                            DISP_TIMER_INT_STAT:1;
    UINT32                            DISP_TIMER_INT:1;
    UINT32                            Reserved_6_3:4;
    UINT32                            DISP_TIMER_RUN_VAL:25;
  } Field;
  UINT32 Value;
} PWR_DISP_TIMER2_DEBUG_STRUCT;

#define SMN_PWR_DISP_TIMER2_DEBUG_ADDRESS    0x5ac80UL


/***********************************************************
* Register Name : PWR_DISP_TIMER_CONTROL
* Register Description :
* DISPLAY TIMER1 control register.It could have 17 copies in virtualization chip
* Visibility : 0x2
************************************************************/

// Bitfield Description : The value when counter reaches, will logic send out the interrupt
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_COUNT_OFFSET      0
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_COUNT_MASK        0x1ffffff

// Bitfield Description : Start the timer, write-one trigger=1, No action=0
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_ENABLE_OFFSET      25
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_ENABLE_MASK        0x2000000

// Bitfield Description : Stop the timer, write-one trigger=1, No action=0
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_DISABLE_OFFSET      26
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_DISABLE_MASK        0x4000000

// Bitfield Description : Display Countdown Timer can generate HW interrupt when count reached=0, Display Countdown Timer cannot generate HW interrupt = 1
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_MASK_OFFSET      27
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_MASK_MASK        0x8000000

// Bitfield Description : Clear the interrupt line when timer is in one-shot mode, or periodic mode with level-based type=1, No action=0
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_STAT_AK_OFFSET      28
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_STAT_AK_MASK        0x10000000

// Bitfield Description : For periodic mode, send level-based interrupt. N/A for one-shot mode=1, For periodic mode, send pulse-based interrupt=0
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_TYPE_OFFSET      29
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_TYPE_MASK        0x20000000

// Bitfield Description : Periodic mode. Timer restarts itself when it gets expired=1, One-time shot mode. Timer stops when it gets expired
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_MODE_OFFSET      30
#define PWR_DISP_TIMER_CONTROL_DISP_TIMER_INT_MODE_MASK        0x40000000

// Bitfield Description : 
#define PWR_DISP_TIMER_CONTROL_Reserved_31_31_OFFSET      31
#define PWR_DISP_TIMER_CONTROL_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            DISP_TIMER_INT_COUNT:25;
    UINT32                            DISP_TIMER_INT_ENABLE:1;
    UINT32                            DISP_TIMER_INT_DISABLE:1;
    UINT32                            DISP_TIMER_INT_MASK:1;
    UINT32                            DISP_TIMER_INT_STAT_AK:1;
    UINT32                            DISP_TIMER_INT_TYPE:1;
    UINT32                            DISP_TIMER_INT_MODE:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PWR_DISP_TIMER_CONTROL_STRUCT;

#define SMN_PWR_DISP_TIMER_CONTROL_ADDRESS    0x5ac74UL


/***********************************************************
* Register Name : PWR_DISP_TIMER_DEBUG
* Register Description :
* DISPLAY TIMER1 debug register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Timer interrupt counter running=1, Timer interrupt count not running=0
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_RUNNING_OFFSET      0
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_RUNNING_MASK        0x1

// Bitfield Description : Interrupt line is asserted=1, Interrupt line is not asserted=0
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_STAT_OFFSET      1
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_STAT_MASK        0x2

// Bitfield Description : Interrupt line is asserted and sent to IH=1, No interrupt is sent to=0
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_OFFSET      2
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_INT_MASK        0x4

// Bitfield Description : 
#define PWR_DISP_TIMER_DEBUG_Reserved_6_3_OFFSET      3
#define PWR_DISP_TIMER_DEBUG_Reserved_6_3_MASK        0x78

// Bitfield Description : read only
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_RUN_VAL_OFFSET      7
#define PWR_DISP_TIMER_DEBUG_DISP_TIMER_RUN_VAL_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            DISP_TIMER_INT_RUNNING:1;
    UINT32                            DISP_TIMER_INT_STAT:1;
    UINT32                            DISP_TIMER_INT:1;
    UINT32                            Reserved_6_3:4;
    UINT32                            DISP_TIMER_RUN_VAL:25;
  } Field;
  UINT32 Value;
} PWR_DISP_TIMER_DEBUG_STRUCT;

#define SMN_PWR_DISP_TIMER_DEBUG_ADDRESS    0x5ac78UL


/***********************************************************
* Register Name : PWR_DISP_TIMER_GLOBAL_CONTROL
* Register Description :
* TIMER global control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of xclk cycles per microsecond pulse for display timer
#define PWR_DISP_TIMER_GLOBAL_CONTROL_DISP_TIMER_PULSE_WIDTH_OFFSET      0
#define PWR_DISP_TIMER_GLOBAL_CONTROL_DISP_TIMER_PULSE_WIDTH_MASK        0x3ff

// Bitfield Description : If this bit is set to 0, we should stop the pulse generation logic
#define PWR_DISP_TIMER_GLOBAL_CONTROL_DISP_TIMER_PULSE_EN_OFFSET      10
#define PWR_DISP_TIMER_GLOBAL_CONTROL_DISP_TIMER_PULSE_EN_MASK        0x400

// Bitfield Description : 
#define PWR_DISP_TIMER_GLOBAL_CONTROL_Reserved_31_11_OFFSET      11
#define PWR_DISP_TIMER_GLOBAL_CONTROL_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            DISP_TIMER_PULSE_WIDTH:10;
    UINT32                            DISP_TIMER_PULSE_EN:1;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} PWR_DISP_TIMER_GLOBAL_CONTROL_STRUCT;

#define SMN_PWR_DISP_TIMER_GLOBAL_CONTROL_ADDRESS    0x5ac84UL


/***********************************************************
* Register Name : PWR_IH_CONTROL
* Register Description :
* PWR_IH control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : max credit value that PWR timer sends to IH
#define PWR_IH_CONTROL_MAX_CREDIT_OFFSET      0
#define PWR_IH_CONTROL_MAX_CREDIT_MASK        0x1f

// Bitfield Description : maskt bit for timer1 IH. 0: enable timer1 IH, 1: disable timer1 IH
#define PWR_IH_CONTROL_DISP_TIMER_TRIGGER_MASK_OFFSET      5
#define PWR_IH_CONTROL_DISP_TIMER_TRIGGER_MASK_MASK        0x20

// Bitfield Description : maskt bit for timer2 IH. 0: enable timer2 IH, 1: disable timer1 IH
#define PWR_IH_CONTROL_DISP_TIMER2_TRIGGER_MASK_OFFSET      6
#define PWR_IH_CONTROL_DISP_TIMER2_TRIGGER_MASK_MASK        0x40

// Bitfield Description : 
#define PWR_IH_CONTROL_Reserved_30_7_OFFSET      7
#define PWR_IH_CONTROL_Reserved_30_7_MASK        0x7fffff80

// Bitfield Description : enable dynamic clock gating for pwr ih block
#define PWR_IH_CONTROL_PWR_IH_CLK_GATE_EN_OFFSET      31
#define PWR_IH_CONTROL_PWR_IH_CLK_GATE_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            MAX_CREDIT:5;
    UINT32                            DISP_TIMER_TRIGGER_MASK:1;
    UINT32                            DISP_TIMER2_TRIGGER_MASK:1;
    UINT32                            Reserved_30_7:24;
    UINT32                            PWR_IH_CLK_GATE_EN:1;
  } Field;
  UINT32 Value;
} PWR_IH_CONTROL_STRUCT;

#define SMN_PWR_IH_CONTROL_ADDRESS    0x5ac88UL


/***********************************************************
* Register Name : PWR_PWRMGT
* Register Description :
* This register stores clock gating enable for maodprcomp
* Visibility : 0x2
************************************************************/

// Bitfield Description : Clock gating enable for maodprcomp, 1: maodprcomp clock is gating; 0: maodprcomp clock is not gating
#define PWR_PWRMGT_odpr_clk_gate_en_OFFSET      0
#define PWR_PWRMGT_odpr_clk_gate_en_MASK        0x1

// Bitfield Description : clock gating enable for odpr wrapper/controller in smuio_core
#define PWR_PWRMGT_clock_gating_enable_OFFSET      1
#define PWR_PWRMGT_clock_gating_enable_MASK        0x2

// Bitfield Description : 
#define PWR_PWRMGT_Reserved_31_2_OFFSET      2
#define PWR_PWRMGT_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            odpr_clk_gate_en:1;
    UINT32                            clock_gating_enable:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} PWR_PWRMGT_STRUCT;

#define SMN_PWR_PWRMGT_ADDRESS    0x5a92cUL


/***********************************************************
* Register Name : PWR_SOC_VDD_ISO_CNTL
* Register Description :
* PWRIP domain register controls for ISO cells and VDCIs around PG GFXIP core.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable for Isolation Cell on clk signal from VDDCR VDD to VDDCR SoC. 0=Enabled, isolated. 1=Disabled, not isolated.
#define PWR_SOC_VDD_ISO_CNTL_PWR_VDD_SOC_CLK_ISOn_EN_OFFSET      0
#define PWR_SOC_VDD_ISO_CNTL_PWR_VDD_SOC_CLK_ISOn_EN_MASK        0x1

// Bitfield Description : Enable for Isolation Cell on signal from VDDCR VDD to VDDCR SoC. 0=Enabled, isolated. 1=Disabled, not isolated.
#define PWR_SOC_VDD_ISO_CNTL_PWR_VDD_SOC_ISOn_EN_OFFSET      1
#define PWR_SOC_VDD_ISO_CNTL_PWR_VDD_SOC_ISOn_EN_MASK        0x2

// Bitfield Description : 
#define PWR_SOC_VDD_ISO_CNTL_Reserved_31_2_OFFSET      2
#define PWR_SOC_VDD_ISO_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            PWR_VDD_SOC_CLK_ISOn_EN:1;
    UINT32                            PWR_VDD_SOC_ISOn_EN:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} PWR_SOC_VDD_ISO_CNTL_STRUCT;

#define SMN_PWR_SOC_VDD_ISO_CNTL_ADDRESS    0x5a320UL


/***********************************************************
* Register Name : PWR_VIRT_RESET_REQ
* Register Description :
* PWR virtualization reset request register
* Visibility : 0x2
************************************************************/

// Bitfield Description : VF FLR filed
#define PWR_VIRT_RESET_REQ_VF_FLR_OFFSET      0
#define PWR_VIRT_RESET_REQ_VF_FLR_MASK        0x7fffffff

// Bitfield Description : PF FLR filed
#define PWR_VIRT_RESET_REQ_PF_FLR_OFFSET      31
#define PWR_VIRT_RESET_REQ_PF_FLR_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VF_FLR:31;
    UINT32                            PF_FLR:1;
  } Field;
  UINT32 Value;
} PWR_VIRT_RESET_REQ_STRUCT;

#define SMN_PWR_VIRT_RESET_REQ_ADDRESS    0x5ac70UL


/***********************************************************
* Register Name : SCRATCH_REGISTER0
* Register Description :
* SCRATCH REGISTER0
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER0
#define SCRATCH_REGISTER0_ScratchPad0_OFFSET      0
#define SCRATCH_REGISTER0_ScratchPad0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad0:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER0_STRUCT;

#define SMN_SCRATCH_REGISTER0_ADDRESS    0x5aef8UL


/***********************************************************
* Register Name : SCRATCH_REGISTER1
* Register Description :
* SCRATCH REGISTER1
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER1
#define SCRATCH_REGISTER1_ScratchPad1_OFFSET      0
#define SCRATCH_REGISTER1_ScratchPad1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad1:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER1_STRUCT;

#define SMN_SCRATCH_REGISTER1_ADDRESS    0x5aefcUL


/***********************************************************
* Register Name : SCRATCH_REGISTER2
* Register Description :
* SCRATCH REGISTER2
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER2
#define SCRATCH_REGISTER2_ScratchPad2_OFFSET      0
#define SCRATCH_REGISTER2_ScratchPad2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad2:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER2_STRUCT;

#define SMN_SCRATCH_REGISTER2_ADDRESS    0x5af00UL


/***********************************************************
* Register Name : SCRATCH_REGISTER3
* Register Description :
* SCRATCH REGISTER3
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER3
#define SCRATCH_REGISTER3_ScratchPad3_OFFSET      0
#define SCRATCH_REGISTER3_ScratchPad3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad3:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER3_STRUCT;

#define SMN_SCRATCH_REGISTER3_ADDRESS    0x5af04UL


/***********************************************************
* Register Name : SCRATCH_REGISTER4
* Register Description :
* SCRATCH REGISTER4
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER4
#define SCRATCH_REGISTER4_ScratchPad4_OFFSET      0
#define SCRATCH_REGISTER4_ScratchPad4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad4:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER4_STRUCT;

#define SMN_SCRATCH_REGISTER4_ADDRESS    0x5af08UL


/***********************************************************
* Register Name : SCRATCH_REGISTER5
* Register Description :
* SCRATCH REGISTER5
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER5
#define SCRATCH_REGISTER5_ScratchPad5_OFFSET      0
#define SCRATCH_REGISTER5_ScratchPad5_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad5:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER5_STRUCT;

#define SMN_SCRATCH_REGISTER5_ADDRESS    0x5af0cUL


/***********************************************************
* Register Name : SCRATCH_REGISTER6
* Register Description :
* SCRATCH REGISTER6
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER6
#define SCRATCH_REGISTER6_ScratchPad6_OFFSET      0
#define SCRATCH_REGISTER6_ScratchPad6_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad6:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER6_STRUCT;

#define SMN_SCRATCH_REGISTER6_ADDRESS    0x5af10UL


/***********************************************************
* Register Name : SCRATCH_REGISTER7
* Register Description :
* SCRATCH REGISTER7
* Visibility : 0x2
************************************************************/

// Bitfield Description : SCRATCH REGISTER7
#define SCRATCH_REGISTER7_ScratchPad7_OFFSET      0
#define SCRATCH_REGISTER7_ScratchPad7_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ScratchPad7:32;
  } Field;
  UINT32 Value;
} SCRATCH_REGISTER7_STRUCT;

#define SMN_SCRATCH_REGISTER7_ADDRESS    0x5af14UL


/***********************************************************
* Register Name : SMN_CCX_VDCI2_RESET
* Register Description :
* PWR TO CCX for SMN VDCI Reset
* Visibility : 0x2
************************************************************/

// Bitfield Description : PWR to CCX0 for SMN VDCI Reset
#define SMN_CCX_VDCI2_RESET_SMN_CCX0_VDCI2_RESET_OFFSET      0
#define SMN_CCX_VDCI2_RESET_SMN_CCX0_VDCI2_RESET_MASK        0x1

// Bitfield Description : RESERVED
#define SMN_CCX_VDCI2_RESET_Reserved_31_1_OFFSET      1
#define SMN_CCX_VDCI2_RESET_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            SMN_CCX0_VDCI2_RESET:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} SMN_CCX_VDCI2_RESET_STRUCT;

#define SMN_SMN_CCX_VDCI2_RESET_ADDRESS    0x5a88cUL


/***********************************************************
* Register Name : SMUIO_CDCI_RESET_CNTL
* Register Description :
* This register controls CDCI reset
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reset CDCI pointer on interface between GC and non-GC, 0:Not Reset 1:Reset
#define SMUIO_CDCI_RESET_CNTL_GFX_CDCI_reset_OFFSET      0
#define SMUIO_CDCI_RESET_CNTL_GFX_CDCI_reset_MASK        0x1

// Bitfield Description : Reset CDCI pointer on interface between EA and DF, 0:Not Reset 1:Reset
#define SMUIO_CDCI_RESET_CNTL_EA_DF_CDCI_reset_OFFSET      1
#define SMUIO_CDCI_RESET_CNTL_EA_DF_CDCI_reset_MASK        0x2

// Bitfield Description : 
#define SMUIO_CDCI_RESET_CNTL_Reserved_31_2_OFFSET      2
#define SMUIO_CDCI_RESET_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            GFX_CDCI_reset:1;
    UINT32                            EA_DF_CDCI_reset:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} SMUIO_CDCI_RESET_CNTL_STRUCT;

#define SMN_SMUIO_CDCI_RESET_CNTL_ADDRESS    0x5a324UL


/***********************************************************
* Register Name : SMUIO_GFX_ISO_CNTL
* Register Description :
* This register controls isolation from VDDCR_GFX to VDDCR_SOC
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable for VDCI2 Isolation Cell on interface between GFXIP rSMU and SMN 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_SOC_G_rSMU_SMN_VDCI_ISO_EN_OFFSET      0
#define SMUIO_GFX_ISO_CNTL_SOC_G_rSMU_SMN_VDCI_ISO_EN_MASK        0x1

// Bitfield Description : Enable for VDCI2 Isolation Cell on interface between GFXIP CPAXI and SYSHUB 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_SOC_CPAXI_SYSHUB_VDCI_ISO_EN_OFFSET      1
#define SMUIO_GFX_ISO_CNTL_SOC_CPAXI_SYSHUB_VDCI_ISO_EN_MASK        0x2

// Bitfield Description : Enable for VDCI2 Isolation Cell on interface between GFXIP up to 15 EA instances 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_SOC_EA_SDF_VDCI_ISO_EN_OFFSET      2
#define SMUIO_GFX_ISO_CNTL_SOC_EA_SDF_VDCI_ISO_EN_MASK        0x4

// Bitfield Description : Enable for VDCI2 Isolation Cell on interface between GFXIP UTCL2 instance and ATHUB 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_SOC_UTCL2_ATHUB_VDCI_ISO_EN_OFFSET      3
#define SMUIO_GFX_ISO_CNTL_SOC_UTCL2_ATHUB_VDCI_ISO_EN_MASK        0x8

// Bitfield Description : Enable for Isolation Cell on signal from GFXIP MON master PGFSM Instance to PWRIP 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_GFX2SOC_ISOn_OFFSET      4
#define SMUIO_GFX_ISO_CNTL_GFX2SOC_ISOn_MASK        0x10

// Bitfield Description : Enable for Isolation Cell on signal from GFXIP MON master PGFSM Instance to PWRIP 1:Disabled, Not Isolated 0:Enabled, Isolated
#define SMUIO_GFX_ISO_CNTL_GFX2SOC_CLK_ISOn_OFFSET      5
#define SMUIO_GFX_ISO_CNTL_GFX2SOC_CLK_ISOn_MASK        0x20

// Bitfield Description : Enable for Isolation Cells on signals from GFXIP RLC instance to VDDCR_SOC domain. 1=Disabled, not isolated. 0=Enabled, isolated.
#define SMUIO_GFX_ISO_CNTL_SOC_RLC_SoC_ISO_EN_OFFSET      6
#define SMUIO_GFX_ISO_CNTL_SOC_RLC_SoC_ISO_EN_MASK        0x40

// Bitfield Description : Enable for Isolation Cells on signals from GFXIP master PGFSM instance to VDDCR_SOC domain. 1=Disabled, not isolated. 0=Enabled, isolated.
#define SMUIO_GFX_ISO_CNTL_SOC_MON_SoC_BPM_P1_iso_clampn_ISO_EN_OFFSET      7
#define SMUIO_GFX_ISO_CNTL_SOC_MON_SoC_BPM_P1_iso_clampn_ISO_EN_MASK        0x80

// Bitfield Description : 
#define SMUIO_GFX_ISO_CNTL_Reserved_31_8_OFFSET      8
#define SMUIO_GFX_ISO_CNTL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            SOC_G_rSMU_SMN_VDCI_ISO_EN:1;
    UINT32                            SOC_CPAXI_SYSHUB_VDCI_ISO_EN:1;
    UINT32                            SOC_EA_SDF_VDCI_ISO_EN:1;
    UINT32                            SOC_UTCL2_ATHUB_VDCI_ISO_EN:1;
    UINT32                            GFX2SOC_ISOn:1;
    UINT32                            GFX2SOC_CLK_ISOn:1;
    UINT32                            SOC_RLC_SoC_ISO_EN:1;
    UINT32                            SOC_MON_SoC_BPM_P1_iso_clampn_ISO_EN:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} SMUIO_GFX_ISO_CNTL_STRUCT;

#define SMN_SMUIO_GFX_ISO_CNTL_ADDRESS    0x5a310UL


/***********************************************************
* Register Name : SMUIO_GFX_MISC_CNTL
* Register Description :
* This register controls misc interface from SMUIO to GFX
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cold Boot vs. GFXOFF exit signalling to RLC. 0=Cold Boot. 1=GFXOFF Exit.
#define SMUIO_GFX_MISC_CNTL_SMU_GFX_cold_vs_gfxoff_OFFSET      0
#define SMUIO_GFX_MISC_CNTL_SMU_GFX_cold_vs_gfxoff_MASK        0x1

// Bitfield Description : Status of GFXOFF. 0=GFXOFF(default). 1=Transition out of GFX State. 2=Not in GFXOFF. 3=Transition into GFXOFF.
#define SMUIO_GFX_MISC_CNTL_PWR_GFXOFF_STATUS_OFFSET      1
#define SMUIO_GFX_MISC_CNTL_PWR_GFXOFF_STATUS_MASK        0x6

// Bitfield Description : 
#define SMUIO_GFX_MISC_CNTL_Reserved_31_3_OFFSET      3
#define SMUIO_GFX_MISC_CNTL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            SMU_GFX_cold_vs_gfxoff:1;
    UINT32                            PWR_GFXOFF_STATUS:2;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} SMUIO_GFX_MISC_CNTL_STRUCT;

#define SMN_SMUIO_GFX_MISC_CNTL_ADDRESS    0x5a314UL


/***********************************************************
* Register Name : SMUIO_GFX_RESET_CNTL
* Register Description :
* This register controls isolation from VDDCR_GFX to VDDCR_SOC
* Visibility : 0x2
************************************************************/

// Bitfield Description : SOC2GFX PWROKRAW override
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_PWROKRAW_OFFSET      0
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_PWROKRAW_MASK        0x1

// Bitfield Description : SOC2GFX PWROK override
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_PWROK_OFFSET      1
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_PWROK_MASK        0x2

// Bitfield Description : SOC2GFX RESETn override
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_RESETn_OFFSET      2
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_RESETn_MASK        0x4

// Bitfield Description : SOC2GFX aeb_reset override
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_aeb_reset_OFFSET      3
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_aeb_reset_MASK        0x8

// Bitfield Description : SOC2GFX aeb_valid override
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_aeb_valid_OFFSET      4
#define SMUIO_GFX_RESET_CNTL_SOC2GFX_aeb_valid_MASK        0x10

// Bitfield Description : 
#define SMUIO_GFX_RESET_CNTL_Reserved_31_5_OFFSET      5
#define SMUIO_GFX_RESET_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            SOC2GFX_PWROKRAW:1;
    UINT32                            SOC2GFX_PWROK:1;
    UINT32                            SOC2GFX_RESETn:1;
    UINT32                            SOC2GFX_aeb_reset:1;
    UINT32                            SOC2GFX_aeb_valid:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} SMUIO_GFX_RESET_CNTL_STRUCT;

#define SMN_SMUIO_GFX_RESET_CNTL_ADDRESS    0x5a308UL


/***********************************************************
* Register Name : SMUIO_GFX_VDCI_RESET_CNTL
* Register Description :
* This register controls isolation from VDDCR_GFX to VDDCR_SOC
* Visibility : 0x2
************************************************************/

// Bitfield Description : SOC2GFX VDCI RESET override
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC2GFX_VDCI_RESETn_OFFSET      0
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC2GFX_VDCI_RESETn_MASK        0x1

// Bitfield Description : Reset VDCI pointer on interface between GFXIP CPAXI and SYSHUB 0:Not Reset 1:Reset
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_CPAXI_SYSHUB_VDCI_reset_OFFSET      1
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_CPAXI_SYSHUB_VDCI_reset_MASK        0x2

// Bitfield Description : Reset VDCI pointer on interface between GFXIP up to 15 EA instances 0:Not Reset 1:Reset
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_EA_SDF_VDCI_reset_OFFSET      2
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_EA_SDF_VDCI_reset_MASK        0x4

// Bitfield Description : Reset VDCI pointer on interface between GFXIP UTCL2 instance and ATHUB 0:Not Reset 1:Reset
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_UTCL2_ATHUB_VDCI_reset_OFFSET      3
#define SMUIO_GFX_VDCI_RESET_CNTL_SOC_UTCL2_ATHUB_VDCI_reset_MASK        0x8

// Bitfield Description : 
#define SMUIO_GFX_VDCI_RESET_CNTL_Reserved_31_4_OFFSET      4
#define SMUIO_GFX_VDCI_RESET_CNTL_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            SOC2GFX_VDCI_RESETn:1;
    UINT32                            SOC_CPAXI_SYSHUB_VDCI_reset:1;
    UINT32                            SOC_EA_SDF_VDCI_reset:1;
    UINT32                            SOC_UTCL2_ATHUB_VDCI_reset:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} SMUIO_GFX_VDCI_RESET_CNTL_STRUCT;

#define SMN_SMUIO_GFX_VDCI_RESET_CNTL_ADDRESS    0x5a30cUL


/***********************************************************
* Register Name : SMUIO_MCM_CONFIG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : die id,which is from FCH
#define SMUIO_MCM_CONFIG_DIE_ID_OFFSET      0
#define SMUIO_MCM_CONFIG_DIE_ID_MASK        0x3

// Bitfield Description : pkg type, which is programmable by MP1/bootcode with fuse value
#define SMUIO_MCM_CONFIG_PKG_TYPE_OFFSET      2
#define SMUIO_MCM_CONFIG_PKG_TYPE_MASK        0x1c

// Bitfield Description : soccket id, which is from FCH
#define SMUIO_MCM_CONFIG_SOCKET_ID_OFFSET      5
#define SMUIO_MCM_CONFIG_SOCKET_ID_MASK        0x20

// Bitfield Description : pkg subtype, which is programmable by MP1/bootcode with fuse value
#define SMUIO_MCM_CONFIG_PKG_SUBTYPE_OFFSET      6
#define SMUIO_MCM_CONFIG_PKG_SUBTYPE_MASK        0xc0

// Bitfield Description : 
#define SMUIO_MCM_CONFIG_Reserved_15_8_OFFSET      8
#define SMUIO_MCM_CONFIG_Reserved_15_8_MASK        0xff00

// Bitfield Description : Console K
#define SMUIO_MCM_CONFIG_CONSOLE_K_OFFSET      16
#define SMUIO_MCM_CONFIG_CONSOLE_K_MASK        0x10000

// Bitfield Description : Console A
#define SMUIO_MCM_CONFIG_CONSOLE_A_OFFSET      17
#define SMUIO_MCM_CONFIG_CONSOLE_A_MASK        0x20000

// Bitfield Description : 
#define SMUIO_MCM_CONFIG_Reserved_31_18_OFFSET      18
#define SMUIO_MCM_CONFIG_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            DIE_ID:2;
    UINT32                            PKG_TYPE:3;
    UINT32                            SOCKET_ID:1;
    UINT32                            PKG_SUBTYPE:2;
    UINT32                            Reserved_15_8:8;
    UINT32                            CONSOLE_K:1;
    UINT32                            CONSOLE_A:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} SMUIO_MCM_CONFIG_STRUCT;

#define SMN_SMUIO_MCM_CONFIG_ADDRESS    0x5a08cUL


/***********************************************************
* Register Name : SMUIO_PCC_CONTROL
* Register Description :
* this reg is used to control the 1.8V PCC GPIO PAD.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable the PCC function, PCC_ENABLE needs to be programmed to 1 after other fields of register
// SMUIO_PCC_CONTROL are programmed, then the PCC function is enabled
#define SMUIO_PCC_CONTROL_PCC_ENABLE_OFFSET      0
#define SMUIO_PCC_CONTROL_PCC_ENABLE_MASK        0x1

// Bitfield Description : BP_OCP_L is low active = 0, high active = 1
#define SMUIO_PCC_CONTROL_PCC_POLARITY_OFFSET      1
#define SMUIO_PCC_CONTROL_PCC_POLARITY_MASK        0x2

// Bitfield Description : GPIO pad's output.  The value to be outputted to the pad's Y if PCC_ENABLE is 1 and PCC_OE is 1.
#define SMUIO_PCC_CONTROL_PCC_A_OFFSET      2
#define SMUIO_PCC_CONTROL_PCC_A_MASK        0x4

// Bitfield Description : GPIO pad's output enable.  If 1, GPIO pad is in output mode.  If 0, GPIO pad accepts inputs from pad.
#define SMUIO_PCC_CONTROL_PCC_OE_OFFSET      3
#define SMUIO_PCC_CONTROL_PCC_OE_MASK        0x8

// Bitfield Description : 0 - GPIO Internal pull up is disabled 1 - GPIO Internal pull up is enabled
#define SMUIO_PCC_CONTROL_PCC_PU_OFFSET      4
#define SMUIO_PCC_CONTROL_PCC_PU_MASK        0x10

// Bitfield Description : 0 - GPIO Internal pull down is disabled 1 - GPIO Internal pull down is enabled
#define SMUIO_PCC_CONTROL_PCC_PD_OFFSET      5
#define SMUIO_PCC_CONTROL_PCC_PD_MASK        0x20

// Bitfield Description : S1 S0 Zmode  VDD18_GPIO operation  Description
// 0  0  Z40    1.2V  only            All Driver Transistors Turned On 100% drive
// 0  1  Z40    1.5V & 1.8V           83.4% drive
// 1  0  Z60    1.2V/1.5V/1.8V        66.7% drive
// 1  1  Z80    1.2V/1.5V/1.8V        Only 80 Ohms driver is ON (50% drive)
// NOTE:  above Table is true only if iOE = 1   & iDPWRGD_18 = 1
#define SMUIO_PCC_CONTROL_PCC_S0_OFFSET      6
#define SMUIO_PCC_CONTROL_PCC_S0_MASK        0x40

// Bitfield Description : S1 S0 Zmode  VDD18_GPIO operation  Description
// 0  0  Z40    1.2V  only            All Driver Transistors Turned On 100% drive
// 0  1  Z40    1.5V & 1.8V           83.4% drive
// 1  0  Z60    1.2V/1.5V/1.8V        66.7% drive
// 1  1  Z80    1.2V/1.5V/1.8V        Only 80 Ohms driver is ON (50% drive)
// NOTE:  above Table is true only if iOE = 1   & iDPWRGD_18 = 1
#define SMUIO_PCC_CONTROL_PCC_S1_OFFSET      7
#define SMUIO_PCC_CONTROL_PCC_S1_MASK        0x80

// Bitfield Description : 0 - 1.8V 200MHz Receivers are disabled 1 - 1.8V 200MHz Receivers are enabled
#define SMUIO_PCC_CONTROL_PCC_SCHMEN_OFFSET      8
#define SMUIO_PCC_CONTROL_PCC_SCHMEN_MASK        0x100

// Bitfield Description : 
#define SMUIO_PCC_CONTROL_Reserved_31_9_OFFSET      9
#define SMUIO_PCC_CONTROL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            PCC_ENABLE:1;
    UINT32                            PCC_POLARITY:1;
    UINT32                            PCC_A:1;
    UINT32                            PCC_OE:1;
    UINT32                            PCC_PU:1;
    UINT32                            PCC_PD:1;
    UINT32                            PCC_S0:1;
    UINT32                            PCC_S1:1;
    UINT32                            PCC_SCHMEN:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} SMUIO_PCC_CONTROL_STRUCT;

#define SMN_SMUIO_PCC_CONTROL_ADDRESS    0x5af18UL


/***********************************************************
* Register Name : SMUIO_PCC_COUNT
* Register Description :
* active PCC level counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : active PCC level counter, FW read clear
#define SMUIO_PCC_COUNT_PCC_COUNT_OFFSET      0
#define SMUIO_PCC_COUNT_PCC_COUNT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PCC_COUNT:32;
  } Field;
  UINT32 Value;
} SMUIO_PCC_COUNT_STRUCT;

#define SMN_SMUIO_PCC_COUNT_ADDRESS    0x5af1cUL


/***********************************************************
* Register Name : SMUIO_RESET_CONFIG
* Register Description :
* RESET block configuration register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable RESET block clock gating (0 = disabled, 1 = enabled)
#define SMUIO_RESET_CONFIG_EnableClockGating_OFFSET      0
#define SMUIO_RESET_CONFIG_EnableClockGating_MASK        0x1

// Bitfield Description : 
#define SMUIO_RESET_CONFIG_Reserved_31_1_OFFSET      1
#define SMUIO_RESET_CONFIG_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            EnableClockGating:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} SMUIO_RESET_CONFIG_STRUCT;

#define SMN_SMUIO_RESET_CONFIG_ADDRESS    0x5a328UL


/***********************************************************
* Register Name : SMUIO_SOC_HALT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SMUIO_SOC_HALT_Reserved_1_0_OFFSET      0
#define SMUIO_SOC_HALT_Reserved_1_0_MASK        0x3

// Bitfield Description : when watch dog timer expired, force Cpl_PWROK to be de-asserted
#define SMUIO_SOC_HALT_WDT_FORCE_PWROK_EN_OFFSET      2
#define SMUIO_SOC_HALT_WDT_FORCE_PWROK_EN_MASK        0x4

// Bitfield Description : when watch dog timer expired, force Cpl_RESETn to be asserted
#define SMUIO_SOC_HALT_WDT_FORCE_RESETn_EN_OFFSET      3
#define SMUIO_SOC_HALT_WDT_FORCE_RESETn_EN_MASK        0x8

// Bitfield Description : 
#define SMUIO_SOC_HALT_Reserved_31_4_OFFSET      4
#define SMUIO_SOC_HALT_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            WDT_FORCE_PWROK_EN:1;
    UINT32                            WDT_FORCE_RESETn_EN:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} SMUIO_SOC_HALT_STRUCT;

#define SMN_SMUIO_SOC_HALT_ADDRESS    0x5a304UL


/***********************************************************
* Register Name : SMUIO_VDD_RESET_CNTL
* Register Description :
* This register controls reset from VDDCR_SOC to VDDCR_VDD
* Visibility : 0x2
************************************************************/

// Bitfield Description : SOC2VDD PWROKRAW override
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_PWROKRAW_OFFSET      0
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_PWROKRAW_MASK        0x1

// Bitfield Description : SOC2VDD PWROK override
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_PWROK_OFFSET      1
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_PWROK_MASK        0x2

// Bitfield Description : SOC2VDD RESETn override
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_RESETn_OFFSET      2
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_RESETn_MASK        0x4

// Bitfield Description : SOC2VDD aeb_reset override
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_aeb_reset_OFFSET      3
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_aeb_reset_MASK        0x8

// Bitfield Description : SOC2VDD aeb_valid override
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_aeb_valid_OFFSET      4
#define SMUIO_VDD_RESET_CNTL_SOC2VDD_aeb_valid_MASK        0x10

// Bitfield Description : 
#define SMUIO_VDD_RESET_CNTL_Reserved_31_5_OFFSET      5
#define SMUIO_VDD_RESET_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            SOC2VDD_PWROKRAW:1;
    UINT32                            SOC2VDD_PWROK:1;
    UINT32                            SOC2VDD_RESETn:1;
    UINT32                            SOC2VDD_aeb_reset:1;
    UINT32                            SOC2VDD_aeb_valid:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} SMUIO_VDD_RESET_CNTL_STRUCT;

#define SMN_SMUIO_VDD_RESET_CNTL_ADDRESS    0x5a318UL


/***********************************************************
* Register Name : SOC2DF_VDCI_RESETn
* Register Description :
* SOC2DF VDCI Reset
* Visibility : 0x2
************************************************************/

// Bitfield Description : SOC2DF VDCI Reset
#define SOC2DF_VDCI_RESETn_SOC2DF_VDCI_RESETn_OFFSET      0
#define SOC2DF_VDCI_RESETn_SOC2DF_VDCI_RESETn_MASK        0x1

// Bitfield Description : RESERVED
#define SOC2DF_VDCI_RESETn_Reserved_31_1_OFFSET      1
#define SOC2DF_VDCI_RESETn_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            SOC2DF_VDCI_RESETn:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} SOC2DF_VDCI_RESETn_STRUCT;

#define SMN_SOC2DF_VDCI_RESETn_ADDRESS    0x5a888UL


/***********************************************************
* Register Name : SOC_GAP_PWROK
* Register Description :
* This register controls the SOC domain GAP PWROK
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register bit controls the SOC domain GAP PWROK
#define SOC_GAP_PWROK_soc_gap_pwrok_OFFSET      0
#define SOC_GAP_PWROK_soc_gap_pwrok_MASK        0x1

// Bitfield Description : 
#define SOC_GAP_PWROK_Reserved_31_1_OFFSET      1
#define SOC_GAP_PWROK_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            soc_gap_pwrok:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} SOC_GAP_PWROK_STRUCT;

#define SMN_SOC_GAP_PWROK_ADDRESS    0x5a8acUL


/***********************************************************
* Register Name : SOC_GOLDEN_TSC_SHADOW_LOWER
* Register Description :
* This register gets updated with the value of Golden TSC(Lower 32-bits) whenever SOC GAP PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of Golden TSC(Lower 32-bits) whenever SOC GAP PWROK asserts
#define SOC_GOLDEN_TSC_SHADOW_LOWER_SocGoldenTscShadowLower_OFFSET      0
#define SOC_GOLDEN_TSC_SHADOW_LOWER_SocGoldenTscShadowLower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SocGoldenTscShadowLower:32;
  } Field;
  UINT32 Value;
} SOC_GOLDEN_TSC_SHADOW_LOWER_STRUCT;

#define SMN_SOC_GOLDEN_TSC_SHADOW_LOWER_ADDRESS    0x5a8a8UL


/***********************************************************
* Register Name : SOC_GOLDEN_TSC_SHADOW_UPPER
* Register Description :
* This register gets updated with the value of Golden TSC(Upper 24-bits) whenever SOC GAP PWROK asserts
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register gets updated with the value of Golden TSC(Upper 24-bits) whenever SOC GAP PWROK asserts
#define SOC_GOLDEN_TSC_SHADOW_UPPER_SocGoldenTscShadowUpper_OFFSET      0
#define SOC_GOLDEN_TSC_SHADOW_UPPER_SocGoldenTscShadowUpper_MASK        0xffffff

// Bitfield Description : 
#define SOC_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_OFFSET      24
#define SOC_GOLDEN_TSC_SHADOW_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SocGoldenTscShadowUpper:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SOC_GOLDEN_TSC_SHADOW_UPPER_STRUCT;

#define SMN_SOC_GOLDEN_TSC_SHADOW_UPPER_ADDRESS    0x5a8a4UL


/***********************************************************
* Register Name : SOFT_DOWNCORE
* Register Description :
* Downcore Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=Disable logical core.
// 0=Logical Core enabled.
#define SOFT_DOWNCORE_DisCore_OFFSET      0
#define SOFT_DOWNCORE_DisCore_MASK        0xff

// Bitfield Description : Reserved for DisCore expansion.
// Read by SMU
// Written by Software
#define SOFT_DOWNCORE_Reserved_31_8_OFFSET      8
#define SOFT_DOWNCORE_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            DisCore:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} SOFT_DOWNCORE_STRUCT;

#define SMN_SOFT_DOWNCORE_ADDRESS    0x5a860UL


/***********************************************************
* Register Name : SVI3_ACK_TIMEOUT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : get Slave ACK timeout counter, unit is us
#define SVI3_ACK_TIMEOUT_ACK_TIMEOUT_OFFSET      0
#define SVI3_ACK_TIMEOUT_ACK_TIMEOUT_MASK        0xff

// Bitfield Description : 
#define SVI3_ACK_TIMEOUT_Reserved_31_8_OFFSET      8
#define SVI3_ACK_TIMEOUT_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            ACK_TIMEOUT:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} SVI3_ACK_TIMEOUT_STRUCT;

#define SMN_SVI30_SVI3_ACK_TIMEOUT_ADDRESS    0x6f058UL


/***********************************************************
* Register Name : SVI3_BOOTCODE_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : once thermtrip is triggered, this bit will be 1, it will be cleared only after reset
#define SVI3_BOOTCODE_CNTL_THERMTRIP_TRIGGERED_OFFSET      0
#define SVI3_BOOTCODE_CNTL_THERMTRIP_TRIGGERED_MASK        0x1

// Bitfield Description : skip bootcode startup vid sending for slave 1
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_1_OFFSET      1
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_1_MASK        0x2

// Bitfield Description : skip bootcode startup vid sending for slave 2
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_2_OFFSET      2
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_2_MASK        0x4

// Bitfield Description : skip bootcode startup vid sending for slave 3
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_3_OFFSET      3
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_3_MASK        0x8

// Bitfield Description : skip bootcode startup vid sending for slave 4
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_4_OFFSET      4
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_4_MASK        0x10

// Bitfield Description : skip bootcode startup vid sending for slave 5
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_5_OFFSET      5
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_5_MASK        0x20

// Bitfield Description : skip bootcode startup vid sending for slave 6
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_6_OFFSET      6
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_6_MASK        0x40

// Bitfield Description : skip bootcode startup vid sending for slave 7
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_7_OFFSET      7
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_7_MASK        0x80

// Bitfield Description : skip bootcode startup vid sending for slave 8
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_8_OFFSET      8
#define SVI3_BOOTCODE_CNTL_SKIP_STARTUPVID_8_MASK        0x100

// Bitfield Description : reserved bits
#define SVI3_BOOTCODE_CNTL_Reserved_31_9_OFFSET      9
#define SVI3_BOOTCODE_CNTL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            THERMTRIP_TRIGGERED:1;
    UINT32                            SKIP_STARTUPVID_1:1;
    UINT32                            SKIP_STARTUPVID_2:1;
    UINT32                            SKIP_STARTUPVID_3:1;
    UINT32                            SKIP_STARTUPVID_4:1;
    UINT32                            SKIP_STARTUPVID_5:1;
    UINT32                            SKIP_STARTUPVID_6:1;
    UINT32                            SKIP_STARTUPVID_7:1;
    UINT32                            SKIP_STARTUPVID_8:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} SVI3_BOOTCODE_CNTL_STRUCT;

#define SMN_SVI30_SVI3_BOOTCODE_CNTL_ADDRESS    0x6f0f8UL


/***********************************************************
* Register Name : SVI3_BOOT_VID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to configure its BOOT_VID in current register.
// It ranges from 1~8 (0 is reserved)
#define SVI3_BOOT_VID_SLV_ADDR_OFFSET      0
#define SVI3_BOOT_VID_SLV_ADDR_MASK        0x3f

// Bitfield Description : boot VID. Firmware needs to configure it after each initialization by the following rules:
// 1, read the boot VID from each slave by sending out register read pacekts.
// 2, configures the read back data to this filed on Master side to make it sync between Master and Slave.
// The bit-definition is aligned with SVI3 spec for VID_BOOT_VOLTAGE[3:0] register on Slave side.
#define SVI3_BOOT_VID_BOOT_VID_OFFSET      6
#define SVI3_BOOT_VID_BOOT_VID_MASK        0x3c0

// Bitfield Description : 
#define SVI3_BOOT_VID_Reserved_31_10_OFFSET      10
#define SVI3_BOOT_VID_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            SLV_ADDR:6;
    UINT32                            BOOT_VID:4;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SVI3_BOOT_VID_STRUCT;

#define SMN_SVI30_SVI3_BOOT_VID_ADDRESS    0x6f018UL


/***********************************************************
* Register Name : SVI3_CLK_GATE_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: SVI3CLK gate enable
#define SVI3_CLK_GATE_CNTL_CLK_GATE_EN_OFFSET      0
#define SVI3_CLK_GATE_CNTL_CLK_GATE_EN_MASK        0x1

// Bitfield Description : SVI3CLK gate IDLE counter
#define SVI3_CLK_GATE_CNTL_CLK_GATE_IDLE_CNT_OFFSET      1
#define SVI3_CLK_GATE_CNTL_CLK_GATE_IDLE_CNT_MASK        0x1fe

// Bitfield Description : 
#define SVI3_CLK_GATE_CNTL_Reserved_31_9_OFFSET      9
#define SVI3_CLK_GATE_CNTL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            CLK_GATE_EN:1;
    UINT32                            CLK_GATE_IDLE_CNT:8;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} SVI3_CLK_GATE_CNTL_STRUCT;

#define SMN_SVI30_SVI3_CLK_GATE_CNTL_ADDRESS    0x6f054UL


/***********************************************************
* Register Name : SVI3_CMD_FIFO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: command fifo empty
#define SVI3_CMD_FIFO_FIFO_EMPTY_OFFSET      0
#define SVI3_CMD_FIFO_FIFO_EMPTY_MASK        0x1

// Bitfield Description : 1: command fifo full
#define SVI3_CMD_FIFO_FIFO_FULL_OFFSET      1
#define SVI3_CMD_FIFO_FIFO_FULL_MASK        0x2

// Bitfield Description : 1: command fifo almost full
#define SVI3_CMD_FIFO_FIFO_AFULL_OFFSET      2
#define SVI3_CMD_FIFO_FIFO_AFULL_MASK        0x4

// Bitfield Description : 1: to flush commnd fifo
#define SVI3_CMD_FIFO_FLUSH_FIFO_OFFSET      3
#define SVI3_CMD_FIFO_FLUSH_FIFO_MASK        0x8

// Bitfield Description : 
#define SVI3_CMD_FIFO_Reserved_31_4_OFFSET      4
#define SVI3_CMD_FIFO_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            FIFO_EMPTY:1;
    UINT32                            FIFO_FULL:1;
    UINT32                            FIFO_AFULL:1;
    UINT32                            FLUSH_FIFO:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} SVI3_CMD_FIFO_STRUCT;

#define SMN_SVI30_SVI3_CMD_FIFO_ADDRESS    0x6f048UL


/***********************************************************
* Register Name : SVI3_CMD_TRACE_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current write pointer for CMD trace buffer
#define SVI3_CMD_TRACE_CNTL_WR_PTR_OFFSET      0
#define SVI3_CMD_TRACE_CNTL_WR_PTR_MASK        0xf

// Bitfield Description : read pointer for CMD trace buffer
#define SVI3_CMD_TRACE_CNTL_RD_PTR_OFFSET      4
#define SVI3_CMD_TRACE_CNTL_RD_PTR_MASK        0xf0

// Bitfield Description : 1: halt trace, 0: unhalt trace
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_OFFSET      8
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_MASK        0x100

// Bitfield Description : halt cmd trace when CMD NACK received
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_ON_CMD_NACK_ERROR_EN_OFFSET      9
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_ON_CMD_NACK_ERROR_EN_MASK        0x200

// Bitfield Description : halt cmd trace when TEL PKT ERR received
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_ON_TEL_PKT_ERROR_EN_OFFSET      10
#define SVI3_CMD_TRACE_CNTL_TRACE_HALT_ON_TEL_PKT_ERROR_EN_MASK        0x400

// Bitfield Description : 
#define SVI3_CMD_TRACE_CNTL_Reserved_15_11_OFFSET      11
#define SVI3_CMD_TRACE_CNTL_Reserved_15_11_MASK        0xf800

// Bitfield Description : enable the trace trigger function, trace buffer will only be filled in when trigger condition meet
#define SVI3_CMD_TRACE_CNTL_TRACE_TRIGGER_EN_OFFSET      16
#define SVI3_CMD_TRACE_CNTL_TRACE_TRIGGER_EN_MASK        0x10000

// Bitfield Description : 0: record the first CMD_PKT==TRACE_TRIGGER_PKT and the following 15 commands
// 1: record the first CMD_PKT==TRACE_TRIGGER_PKT and 7 commands before it, 8 commands after it
// 2: record the first CMD_PKT==TRACE_TRIGGER_PKT and 15 commands before it
#define SVI3_CMD_TRACE_CNTL_TRACE_TRIGGER_TYPE_OFFSET      17
#define SVI3_CMD_TRACE_CNTL_TRACE_TRIGGER_TYPE_MASK        0xe0000

// Bitfield Description : cmd trace trigger packet hit
#define SVI3_CMD_TRACE_CNTL_TRIGGER_HIT_OFFSET      20
#define SVI3_CMD_TRACE_CNTL_TRIGGER_HIT_MASK        0x100000

// Bitfield Description : cmd trace trigger done
#define SVI3_CMD_TRACE_CNTL_TRIGGER_DONE_OFFSET      21
#define SVI3_CMD_TRACE_CNTL_TRIGGER_DONE_MASK        0x200000

// Bitfield Description : 
#define SVI3_CMD_TRACE_CNTL_Reserved_31_22_OFFSET      22
#define SVI3_CMD_TRACE_CNTL_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            WR_PTR:4;
    UINT32                            RD_PTR:4;
    UINT32                            TRACE_HALT:1;
    UINT32                            TRACE_HALT_ON_CMD_NACK_ERROR_EN:1;
    UINT32                            TRACE_HALT_ON_TEL_PKT_ERROR_EN:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            TRACE_TRIGGER_EN:1;
    UINT32                            TRACE_TRIGGER_TYPE:3;
    UINT32                            TRIGGER_HIT:1;
    UINT32                            TRIGGER_DONE:1;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} SVI3_CMD_TRACE_CNTL_STRUCT;

#define SMN_SVI30_SVI3_CMD_TRACE_CNTL_ADDRESS    0x6f0a0UL


/***********************************************************
* Register Name : SVI3_CMD_TRACE_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : read data for  CMD trace buffer.
#define SVI3_CMD_TRACE_DATA_RD_DATA_OFFSET      0
#define SVI3_CMD_TRACE_DATA_RD_DATA_MASK        0xffffff

// Bitfield Description : read data length type for CMD trace buffer
#define SVI3_CMD_TRACE_DATA_RD_DATA_LEN_OFFSET      24
#define SVI3_CMD_TRACE_DATA_RD_DATA_LEN_MASK        0x3000000

// Bitfield Description : 
#define SVI3_CMD_TRACE_DATA_Reserved_31_26_OFFSET      26
#define SVI3_CMD_TRACE_DATA_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            RD_DATA:24;
    UINT32                            RD_DATA_LEN:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SVI3_CMD_TRACE_DATA_STRUCT;

#define SMN_SVI30_SVI3_CMD_TRACE_DATA_ADDRESS    0x6f0a8UL


/***********************************************************
* Register Name : SVI3_CMD_TRACE_TRIGGER_PKT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : command trace buffer trigger packet conext
#define SVI3_CMD_TRACE_TRIGGER_PKT_TRACE_TRIGGER_PKT_OFFSET      0
#define SVI3_CMD_TRACE_TRIGGER_PKT_TRACE_TRIGGER_PKT_MASK        0xffffff

// Bitfield Description : command trace buffer trigger packet length type
#define SVI3_CMD_TRACE_TRIGGER_PKT_TRACE_TRIGGER_PKT_LEN_OFFSET      24
#define SVI3_CMD_TRACE_TRIGGER_PKT_TRACE_TRIGGER_PKT_LEN_MASK        0x3000000

// Bitfield Description : 
#define SVI3_CMD_TRACE_TRIGGER_PKT_Reserved_31_26_OFFSET      26
#define SVI3_CMD_TRACE_TRIGGER_PKT_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            TRACE_TRIGGER_PKT:24;
    UINT32                            TRACE_TRIGGER_PKT_LEN:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SVI3_CMD_TRACE_TRIGGER_PKT_STRUCT;

#define SMN_SVI30_SVI3_CMD_TRACE_TRIGGER_PKT_ADDRESS    0x6f0a4UL


/***********************************************************
* Register Name : SVI3_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: Send VID change packet only when target VID is different with previous VID
// 0: always send VID change packet no matter if target VID is same as previous VID.
#define SVI3_CNTL_SEND_ON_VID_CHG_ONLY_OFFSET      0
#define SVI3_CNTL_SEND_ON_VID_CHG_ONLY_MASK        0x1

// Bitfield Description : 1: enable back to back command
// 0: single command
#define SVI3_CNTL_B2B_CMD_EN_OFFSET      1
#define SVI3_CNTL_B2B_CMD_EN_MASK        0x2

// Bitfield Description : stop SVC when bus is idle
#define SVI3_CNTL_STOP_SVC_IN_IDLE_OFFSET      2
#define SVI3_CNTL_STOP_SVC_IN_IDLE_MASK        0x4

// Bitfield Description : 
#define SVI3_CNTL_Reserved_7_3_OFFSET      3
#define SVI3_CNTL_Reserved_7_3_MASK        0xf8

// Bitfield Description : 
#define SVI3_CNTL_BUS_DISABLE_OFFSET      8
#define SVI3_CNTL_BUS_DISABLE_MASK        0x100

// Bitfield Description : 
#define SVI3_CNTL_Reserved_31_9_OFFSET      9
#define SVI3_CNTL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            SEND_ON_VID_CHG_ONLY:1;
    UINT32                            B2B_CMD_EN:1;
    UINT32                            STOP_SVC_IN_IDLE:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            BUS_DISABLE:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} SVI3_CNTL_STRUCT;

#define SMN_SVI30_SVI3_CNTL_ADDRESS    0x6f000UL


/***********************************************************
* Register Name : SVI3_CUR_PSI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to report other info in current register.
// It ranges from 1~8 (0 is reserved)
#define SVI3_CUR_PSI_CUR_ADDR_OFFSET      0
#define SVI3_CUR_PSI_CUR_ADDR_MASK        0x3f

// Bitfield Description : current PSI. It can be updated once receiving Slave ACK for PSI change packet by the following rules:
// 1, set as Target PSI after PSI change packet is triggered by registers.
// 2, set as Target PSI after PSI change packet is triggered by sideband interface.
// 3, set as 3'h0 after register reset packet with VID/PSI type is triggered by registers.
// 4, set as 3'h0 after return register packet is triggered by registers.
#define SVI3_CUR_PSI_CUR_PSI_OFFSET      6
#define SVI3_CUR_PSI_CUR_PSI_MASK        0x1c0

// Bitfield Description : 1: PSI change done by receiving slave ACK. It is only valid for psi change packet triggers by registers.
#define SVI3_CUR_PSI_PSI_CHG_DONE_OFFSET      9
#define SVI3_CUR_PSI_PSI_CHG_DONE_MASK        0x200

// Bitfield Description : 1: all the slaves have PSI change done triggered by registers.
#define SVI3_CUR_PSI_ALL_PSI_CHG_DONE_OFFSET      10
#define SVI3_CUR_PSI_ALL_PSI_CHG_DONE_MASK        0x400

// Bitfield Description : 
#define SVI3_CUR_PSI_Reserved_31_11_OFFSET      11
#define SVI3_CUR_PSI_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            CUR_ADDR:6;
    UINT32                            CUR_PSI:3;
    UINT32                            PSI_CHG_DONE:1;
    UINT32                            ALL_PSI_CHG_DONE:1;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} SVI3_CUR_PSI_STRUCT;

#define SMN_SVI30_SVI3_CUR_PSI_ADDRESS    0x6f020UL


/***********************************************************
* Register Name : SVI3_CUR_VID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to report its other info in current register.
// It ranges from 1~8 (0 is reserved)
#define SVI3_CUR_VID_CUR_ADDR_OFFSET      0
#define SVI3_CUR_VID_CUR_ADDR_MASK        0x3f

// Bitfield Description : current VID. It can be updated once VID is calculated done by the following rules:
// 1, set as boot VID after Firmware configure boot VID at the 1st time
// 2, set as TARGET_VID after vid change packet is triggered by registers and calculation is done.
// 3, set as TARGET_VID after vid change packet is triggered by sideband interface and calculation is done.
// 4, set as boot VID after return_vid/ return_reg packet is triggered by registers and calculation is done.
// 5, set as boot VID after register reset packet with VID/PSI type is triggered by registers and calculation is done.
#define SVI3_CUR_VID_CUR_VID_OFFSET      6
#define SVI3_CUR_VID_CUR_VID_MASK        0x7fc0

// Bitfield Description : 1: VID change done by calculation. It is only valid for vid change packet triggers by registers.
#define SVI3_CUR_VID_VID_CHG_DONE_OFFSET      15
#define SVI3_CUR_VID_VID_CHG_DONE_MASK        0x8000

// Bitfield Description : 1: all the slaves have VID change done by calculation triggered by registers.
#define SVI3_CUR_VID_ALL_VID_CHG_DONE_OFFSET      16
#define SVI3_CUR_VID_ALL_VID_CHG_DONE_MASK        0x10000

// Bitfield Description : 
#define SVI3_CUR_VID_Reserved_31_17_OFFSET      17
#define SVI3_CUR_VID_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            CUR_ADDR:6;
    UINT32                            CUR_VID:9;
    UINT32                            VID_CHG_DONE:1;
    UINT32                            ALL_VID_CHG_DONE:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} SVI3_CUR_VID_STRUCT;

#define SMN_SVI30_SVI3_CUR_VID_ADDRESS    0x6f014UL


/***********************************************************
* Register Name : SVI3_DEBUG0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG0_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG0_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG0_STRUCT;

#define SMN_SVI30_SVI3_DEBUG0_ADDRESS    0x6f0d0UL


/***********************************************************
* Register Name : SVI3_DEBUG1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG1_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG1_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG1_STRUCT;

#define SMN_SVI30_SVI3_DEBUG1_ADDRESS    0x6f0d4UL


/***********************************************************
* Register Name : SVI3_DEBUG2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG2_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG2_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG2_STRUCT;

#define SMN_SVI30_SVI3_DEBUG2_ADDRESS    0x6f0d8UL


/***********************************************************
* Register Name : SVI3_DEBUG3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG3_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG3_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG3_STRUCT;

#define SMN_SVI30_SVI3_DEBUG3_ADDRESS    0x6f0dcUL


/***********************************************************
* Register Name : SVI3_DEBUG4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG4_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG4_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG4_STRUCT;

#define SMN_SVI30_SVI3_DEBUG4_ADDRESS    0x6f0e0UL


/***********************************************************
* Register Name : SVI3_DEBUG5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG5_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG5_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG5_STRUCT;

#define SMN_SVI30_SVI3_DEBUG5_ADDRESS    0x6f0e4UL


/***********************************************************
* Register Name : SVI3_DEBUG6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG6_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG6_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG6_STRUCT;

#define SMN_SVI30_SVI3_DEBUG6_ADDRESS    0x6f0e8UL


/***********************************************************
* Register Name : SVI3_DEBUG7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : internal debug register
#define SVI3_DEBUG7_DEBUG_DATA_OFFSET      0
#define SVI3_DEBUG7_DEBUG_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEBUG_DATA:32;
  } Field;
  UINT32 Value;
} SVI3_DEBUG7_STRUCT;

#define SMN_SVI30_SVI3_DEBUG7_ADDRESS    0x6f0ecUL


/***********************************************************
* Register Name : SVI3_DEBUG_CFG_ADDR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : address field for debug packets
#define SVI3_DEBUG_CFG_ADDR_PKT_ADDR_OFFSET      0
#define SVI3_DEBUG_CFG_ADDR_PKT_ADDR_MASK        0x3f

// Bitfield Description : 
#define SVI3_DEBUG_CFG_ADDR_Reserved_31_6_OFFSET      6
#define SVI3_DEBUG_CFG_ADDR_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            PKT_ADDR:6;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} SVI3_DEBUG_CFG_ADDR_STRUCT;

#define SMN_SVI30_SVI3_DEBUG_CFG_ADDR_ADDRESS    0x6f098UL


/***********************************************************
* Register Name : SVI3_DEBUG_CFG_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SVI3 packet context
#define SVI3_DEBUG_CFG_DATA_PKT_CONTEXT_OFFSET      0
#define SVI3_DEBUG_CFG_DATA_PKT_CONTEXT_MASK        0xffffff

// Bitfield Description : 2'b00: 8-bit packet 2'b01: 16bit packet 2'b10: 24bit packet 2'b11: 32bit packet
#define SVI3_DEBUG_CFG_DATA_PKT_LEN_OFFSET      24
#define SVI3_DEBUG_CFG_DATA_PKT_LEN_MASK        0x3000000

// Bitfield Description : 
#define SVI3_DEBUG_CFG_DATA_PKT_TYPE_OFFSET      26
#define SVI3_DEBUG_CFG_DATA_PKT_TYPE_MASK        0x7c000000

// Bitfield Description : 
#define SVI3_DEBUG_CFG_DATA_Reserved_31_31_OFFSET      31
#define SVI3_DEBUG_CFG_DATA_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PKT_CONTEXT:24;
    UINT32                            PKT_LEN:2;
    UINT32                            PKT_TYPE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} SVI3_DEBUG_CFG_DATA_STRUCT;

#define SMN_SVI30_SVI3_DEBUG_CFG_DATA_ADDRESS    0x6f094UL


/***********************************************************
* Register Name : SVI3_DEBUG_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : write 1 to enable debug mode
#define SVI3_DEBUG_CNTL_DEBUG_EN_OFFSET      0
#define SVI3_DEBUG_CNTL_DEBUG_EN_MASK        0x1

// Bitfield Description : write 1 to trigger command send from SVI3_DEBUG_CMD
#define SVI3_DEBUG_CNTL_DEBUG_CMD_SEND_OFFSET      1
#define SVI3_DEBUG_CNTL_DEBUG_CMD_SEND_MASK        0x2

// Bitfield Description : set to 1 when command is sent out, and after that, Firmware can trigger a new one.
#define SVI3_DEBUG_CNTL_DEBUG_CMD_SEND_GRANT_OFFSET      2
#define SVI3_DEBUG_CNTL_DEBUG_CMD_SEND_GRANT_MASK        0x4

// Bitfield Description : 
#define SVI3_DEBUG_CNTL_Reserved_31_3_OFFSET      3
#define SVI3_DEBUG_CNTL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            DEBUG_EN:1;
    UINT32                            DEBUG_CMD_SEND:1;
    UINT32                            DEBUG_CMD_SEND_GRANT:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} SVI3_DEBUG_CNTL_STRUCT;

#define SMN_SVI30_SVI3_DEBUG_CNTL_ADDRESS    0x6f090UL


/***********************************************************
* Register Name : SVI3_DEBUG_RETURN_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Payload for the whole debug packet return.
#define SVI3_DEBUG_RETURN_DATA_PKT_PAYLOAD_OFFSET      0
#define SVI3_DEBUG_RETURN_DATA_PKT_PAYLOAD_MASK        0xffffff

// Bitfield Description : 00: 4-bit length 01: 8-bit length 10: 16-bit length 11: 24-bit length
#define SVI3_DEBUG_RETURN_DATA_PKT_LEN_TYPE_OFFSET      24
#define SVI3_DEBUG_RETURN_DATA_PKT_LEN_TYPE_MASK        0x3000000

// Bitfield Description : 
#define SVI3_DEBUG_RETURN_DATA_Reserved_31_26_OFFSET      26
#define SVI3_DEBUG_RETURN_DATA_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            PKT_PAYLOAD:24;
    UINT32                            PKT_LEN_TYPE:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SVI3_DEBUG_RETURN_DATA_STRUCT;

#define SMN_SVI30_SVI3_DEBUG_RETURN_DATA_ADDRESS    0x6f09cUL


/***********************************************************
* Register Name : SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_CMD_NACK_TYPE_OFFSET      0
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_CMD_NACK_TYPE_MASK        0x3

// Bitfield Description : log command packet when NACK recevied
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_COMMAND_PKT_OFFSET      2
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_COMMAND_PKT_MASK        0x3fffffc

// Bitfield Description : log command packet lenghth type when NACK recevied
// 2'b00: 8-bit packet 2'b01: 16bit packet 2'b10: 24bit packet 2'b11: 32bit packet
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_COMMAND_LEN_TYPE_OFFSET      26
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_COMMAND_LEN_TYPE_MASK        0xc000000

// Bitfield Description : 
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_Reserved_31_28_OFFSET      28
#define SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            CMD_NACK_TYPE:2;
    UINT32                            COMMAND_PKT:24;
    UINT32                            COMMAND_LEN_TYPE:2;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_STRUCT;

#define SMN_SVI30_SVI3_FATAL_HIGH_INTR_ERR_LOG_FOR_NACK_ERR_ADDRESS    0x6f068UL


/***********************************************************
* Register Name : SVI3_FATAL_INTR_EN
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: Interrupt enable for command NACK communication error
#define SVI3_FATAL_INTR_EN_CMD_NACK_COMMUNICATION_ERR_EN_OFFSET      0
#define SVI3_FATAL_INTR_EN_CMD_NACK_COMMUNICATION_ERR_EN_MASK        0x1

// Bitfield Description : 1: Interrupt enable for command NACK incomplete error
#define SVI3_FATAL_INTR_EN_CMD_NACK_INCOMPLETE_ERR_EN_OFFSET      1
#define SVI3_FATAL_INTR_EN_CMD_NACK_INCOMPLETE_ERR_EN_MASK        0x2

// Bitfield Description : 1: Interrupt enable for get CRC from Telemetry packet
#define SVI3_FATAL_INTR_EN_TEL_CRC_ERR_EN_OFFSET      2
#define SVI3_FATAL_INTR_EN_TEL_CRC_ERR_EN_MASK        0x4

// Bitfield Description : 1: Interrupt enable for get communication error from Telemetry packet
#define SVI3_FATAL_INTR_EN_TEL_COMMUNICATION_ERR_EN_OFFSET      3
#define SVI3_FATAL_INTR_EN_TEL_COMMUNICATION_ERR_EN_MASK        0x8

// Bitfield Description : 1: Interrupt enable for get data error from Telemetry packet
#define SVI3_FATAL_INTR_EN_TEL_INVALID_ERR_EN_OFFSET      4
#define SVI3_FATAL_INTR_EN_TEL_INVALID_ERR_EN_MASK        0x10

// Bitfield Description : 1: Interrupt enable for get unsupported slave address from init process
#define SVI3_FATAL_INTR_EN_UNSUPPORTED_INIT_SLAVE_ADDR_ERR_EN_OFFSET      5
#define SVI3_FATAL_INTR_EN_UNSUPPORTED_INIT_SLAVE_ADDR_ERR_EN_MASK        0x20

// Bitfield Description : 1: Interrupt enable for command fifo underflow
#define SVI3_FATAL_INTR_EN_READ_ON_FIFO_EMPTY_ERR_EN_OFFSET      6
#define SVI3_FATAL_INTR_EN_READ_ON_FIFO_EMPTY_ERR_EN_MASK        0x40

// Bitfield Description : 1: Interrupt enable for command fifo overflow
#define SVI3_FATAL_INTR_EN_WRITE_ON_FIFO_FULL_ERR_EN_OFFSET      7
#define SVI3_FATAL_INTR_EN_WRITE_ON_FIFO_FULL_ERR_EN_MASK        0x80

// Bitfield Description : 1: Interrupt enable for get slave ack timeout error
#define SVI3_FATAL_INTR_EN_GET_SLAVE_ACK_TIMEOUT_ERR_EN_OFFSET      8
#define SVI3_FATAL_INTR_EN_GET_SLAVE_ACK_TIMEOUT_ERR_EN_MASK        0x100

// Bitfield Description : 1: Interrupt enable for sideband  request de-assertion without ACK asserted
#define SVI3_FATAL_INTR_EN_SB_REQ_F_WO_ACK_ERR_EN_OFFSET      9
#define SVI3_FATAL_INTR_EN_SB_REQ_F_WO_ACK_ERR_EN_MASK        0x200

// Bitfield Description : 
#define SVI3_FATAL_INTR_EN_Reserved_31_10_OFFSET      10
#define SVI3_FATAL_INTR_EN_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            CMD_NACK_COMMUNICATION_ERR_EN:1;
    UINT32                            CMD_NACK_INCOMPLETE_ERR_EN:1;
    UINT32                            TEL_CRC_ERR_EN:1;
    UINT32                            TEL_COMMUNICATION_ERR_EN:1;
    UINT32                            TEL_INVALID_ERR_EN:1;
    UINT32                            UNSUPPORTED_INIT_SLAVE_ADDR_ERR_EN:1;
    UINT32                            READ_ON_FIFO_EMPTY_ERR_EN:1;
    UINT32                            WRITE_ON_FIFO_FULL_ERR_EN:1;
    UINT32                            GET_SLAVE_ACK_TIMEOUT_ERR_EN:1;
    UINT32                            SB_REQ_F_WO_ACK_ERR_EN:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SVI3_FATAL_INTR_EN_STRUCT;

#define SMN_SVI30_SVI3_FATAL_INTR_EN_ADDRESS    0x6f064UL


/***********************************************************
* Register Name : SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : captured VID data or PSI data directly from input port.
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_VID_OFFSET      0
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_VID_MASK        0x1ff

// Bitfield Description : captured Address for VID or PSI command directly from input port.
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_ADDR_OFFSET      9
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_ADDR_MASK        0x7e00

// Bitfield Description : captured PSI or VID flag. 1'b1: PSI; 1'b0: VID
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_IS_PSI_OFFSET      15
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_IS_PSI_MASK        0x8000

// Bitfield Description : W1CLR. It is cleared which is aligned to the sideband interface indicated by
// SIDEBAND_ERR_LOG_IDX[2:0].
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_VLD_OFFSET      16
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_VLD_MASK        0x10000

// Bitfield Description : 
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_IDX_OFFSET      17
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ERR_LOG_IDX_MASK        0xe0000

// Bitfield Description : 
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_Reserved_31_20_OFFSET      20
#define SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ERR_LOG_VID:9;
    UINT32                            ERR_LOG_ADDR:6;
    UINT32                            ERR_LOG_IS_PSI:1;
    UINT32                            ERR_LOG_VLD:1;
    UINT32                            ERR_LOG_IDX:3;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_STRUCT;

#define SMN_SVI30_SVI3_FATAL_INTR_ERR_LOG_FOR_SB_ERR_ADDRESS    0x6f070UL


/***********************************************************
* Register Name : SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TEL_ERROR_TYPE_OFFSET      0
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TEL_ERROR_TYPE_MASK        0x3

// Bitfield Description : log telemetry packet context when NACK recevied,  header pkt+3bits crc if crc error occurs.
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TELEMETRY_PKT_OFFSET      2
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TELEMETRY_PKT_MASK        0x3fffffc

// Bitfield Description : log telemetry packet lenghth type when NACK recevied
// 2'b00: 8-bit packet 2'b01: 16bit packet 2'b10: 24bit packet 2'b11: 32bit packet
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TELEMETRY_LEN_TYPE_OFFSET      26
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_TELEMETRY_LEN_TYPE_MASK        0xc000000

// Bitfield Description : 
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_Reserved_31_28_OFFSET      28
#define SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            TEL_ERROR_TYPE:2;
    UINT32                            TELEMETRY_PKT:24;
    UINT32                            TELEMETRY_LEN_TYPE:2;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_STRUCT;

#define SMN_SVI30_SVI3_FATAL_INTR_ERR_LOG_FOR_TEL_ERR_ADDRESS    0x6f06cUL


/***********************************************************
* Register Name : SVI3_FATAL_INTR_SRC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: command NACK communication error, W1CLR
#define SVI3_FATAL_INTR_SRC_CMD_NACK_COMMUNICATION_ERR_OFFSET      0
#define SVI3_FATAL_INTR_SRC_CMD_NACK_COMMUNICATION_ERR_MASK        0x1

// Bitfield Description : 1: command NACK incomplete error, W1CLR
#define SVI3_FATAL_INTR_SRC_CMD_NACK_INCOMPLETE_ERR_OFFSET      1
#define SVI3_FATAL_INTR_SRC_CMD_NACK_INCOMPLETE_ERR_MASK        0x2

// Bitfield Description : 1: get CRC from Telemetry packet, W1CLR
#define SVI3_FATAL_INTR_SRC_TEL_CRC_ERR_OFFSET      2
#define SVI3_FATAL_INTR_SRC_TEL_CRC_ERR_MASK        0x4

// Bitfield Description : 1: get communication error from Telemetry packet( eg. Start, end bit error), W1CLR
#define SVI3_FATAL_INTR_SRC_TEL_COMMUNICATION_ERR_OFFSET      3
#define SVI3_FATAL_INTR_SRC_TEL_COMMUNICATION_ERR_MASK        0x8

// Bitfield Description : 1: get data error from Telemetry packet( eg. format error), W1CLR
#define SVI3_FATAL_INTR_SRC_TEL_INVALID_ERR_OFFSET      4
#define SVI3_FATAL_INTR_SRC_TEL_INVALID_ERR_MASK        0x10

// Bitfield Description : 1: get unsupported slave address from init process, W1CLR
#define SVI3_FATAL_INTR_SRC_UNSUPPORTED_INIT_SLAVE_ADDR_ERR_OFFSET      5
#define SVI3_FATAL_INTR_SRC_UNSUPPORTED_INIT_SLAVE_ADDR_ERR_MASK        0x20

// Bitfield Description : 1: command fifo underflow, W1CLR
#define SVI3_FATAL_INTR_SRC_READ_ON_FIFO_EMPTY_ERR_OFFSET      6
#define SVI3_FATAL_INTR_SRC_READ_ON_FIFO_EMPTY_ERR_MASK        0x40

// Bitfield Description : 1: command fifo overflow, W1CLR
#define SVI3_FATAL_INTR_SRC_WRITE_ON_FIFO_FULL_ERR_OFFSET      7
#define SVI3_FATAL_INTR_SRC_WRITE_ON_FIFO_FULL_ERR_MASK        0x80

// Bitfield Description : 1: get slave ack timeout error, W1CLR.
#define SVI3_FATAL_INTR_SRC_GET_SLAVE_ACK_TIMEOUT_ERR_OFFSET      8
#define SVI3_FATAL_INTR_SRC_GET_SLAVE_ACK_TIMEOUT_ERR_MASK        0x100

// Bitfield Description : 1: Sideband  request de-assertion without ACK asserted, W1CLR
#define SVI3_FATAL_INTR_SRC_SB_REQ_F_WO_ACK_ERR_OFFSET      9
#define SVI3_FATAL_INTR_SRC_SB_REQ_F_WO_ACK_ERR_MASK        0x200

// Bitfield Description : 
#define SVI3_FATAL_INTR_SRC_Reserved_31_10_OFFSET      10
#define SVI3_FATAL_INTR_SRC_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            CMD_NACK_COMMUNICATION_ERR:1;
    UINT32                            CMD_NACK_INCOMPLETE_ERR:1;
    UINT32                            TEL_CRC_ERR:1;
    UINT32                            TEL_COMMUNICATION_ERR:1;
    UINT32                            TEL_INVALID_ERR:1;
    UINT32                            UNSUPPORTED_INIT_SLAVE_ADDR_ERR:1;
    UINT32                            READ_ON_FIFO_EMPTY_ERR:1;
    UINT32                            WRITE_ON_FIFO_FULL_ERR:1;
    UINT32                            GET_SLAVE_ACK_TIMEOUT_ERR:1;
    UINT32                            SB_REQ_F_WO_ACK_ERR:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SVI3_FATAL_INTR_SRC_STRUCT;

#define SMN_SVI30_SVI3_FATAL_INTR_SRC_ADDRESS    0x6f060UL


/***********************************************************
* Register Name : SVI3_GLB_CMD_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : write 1 to trigger bus initialization
#define SVI3_GLB_CMD_CNTL_INIT_TRIGGER_OFFSET      0
#define SVI3_GLB_CMD_CNTL_INIT_TRIGGER_MASK        0x1

// Bitfield Description : write 1 to trigger CRC enable packet sending
#define SVI3_GLB_CMD_CNTL_CRC_EN_OFFSET      1
#define SVI3_GLB_CMD_CNTL_CRC_EN_MASK        0x2

// Bitfield Description : write 1 to trgger CRC disable packet sending
#define SVI3_GLB_CMD_CNTL_CRC_DIS_OFFSET      2
#define SVI3_GLB_CMD_CNTL_CRC_DIS_MASK        0x4

// Bitfield Description : write 1 to enable telemetry report packet sending
#define SVI3_GLB_CMD_CNTL_TEL_REQ_OFFSET      3
#define SVI3_GLB_CMD_CNTL_TEL_REQ_MASK        0x8

// Bitfield Description : wrie 1 to trigger return vid packet sending
#define SVI3_GLB_CMD_CNTL_RETURN_VID_OFFSET      4
#define SVI3_GLB_CMD_CNTL_RETURN_VID_MASK        0x10

// Bitfield Description : write 1 to trigger return register packet sending
#define SVI3_GLB_CMD_CNTL_RETURN_REG_OFFSET      5
#define SVI3_GLB_CMD_CNTL_RETURN_REG_MASK        0x20

// Bitfield Description : 
#define SVI3_GLB_CMD_CNTL_Reserved_7_6_OFFSET      6
#define SVI3_GLB_CMD_CNTL_Reserved_7_6_MASK        0xc0

// Bitfield Description : 1: CRC_EN/ CRC_DIS request get slave ACK
#define SVI3_GLB_CMD_CNTL_CRC_CHG_DONE_OFFSET      8
#define SVI3_GLB_CMD_CNTL_CRC_CHG_DONE_MASK        0x100

// Bitfield Description : 1: TEL reqest get  slave ACK
#define SVI3_GLB_CMD_CNTL_TEL_REQ_DONE_OFFSET      9
#define SVI3_GLB_CMD_CNTL_TEL_REQ_DONE_MASK        0x200

// Bitfield Description : 1: all telemetry data stream has been received done
#define SVI3_GLB_CMD_CNTL_TEL_DATA_DONE_OFFSET      10
#define SVI3_GLB_CMD_CNTL_TEL_DATA_DONE_MASK        0x400

// Bitfield Description : 1: return VID calculation done after SVI3 master receives slave ACK.  Max delay is used for calculation. When the flag is set,  SVI3 Master will update CUR_VID  to boot VID.
#define SVI3_GLB_CMD_CNTL_RETURN_VID_DONE_OFFSET      11
#define SVI3_GLB_CMD_CNTL_RETURN_VID_DONE_MASK        0x800

// Bitfield Description : 
#define SVI3_GLB_CMD_CNTL_RETURN_REG_DONE_OFFSET      12
#define SVI3_GLB_CMD_CNTL_RETURN_REG_DONE_MASK        0x1000

// Bitfield Description : 
#define SVI3_GLB_CMD_CNTL_Reserved_31_13_OFFSET      13
#define SVI3_GLB_CMD_CNTL_Reserved_31_13_MASK        0xffffe000

typedef union {
  struct {
    UINT32                            INIT_TRIGGER:1;
    UINT32                            CRC_EN:1;
    UINT32                            CRC_DIS:1;
    UINT32                            TEL_REQ:1;
    UINT32                            RETURN_VID:1;
    UINT32                            RETURN_REG:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            CRC_CHG_DONE:1;
    UINT32                            TEL_REQ_DONE:1;
    UINT32                            TEL_DATA_DONE:1;
    UINT32                            RETURN_VID_DONE:1;
    UINT32                            RETURN_REG_DONE:1;
    UINT32                            Reserved_31_13:19;
  } Field;
  UINT32 Value;
} SVI3_GLB_CMD_CNTL_STRUCT;

#define SMN_SVI30_SVI3_GLB_CMD_CNTL_ADDRESS    0x6f008UL


/***********************************************************
* Register Name : SVI3_HIGH_INTR_EN
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: Interrupt enable for command NACK invalid error
#define SVI3_HIGH_INTR_EN_CMD_NACK_INVALID_ERR_EN_OFFSET      0
#define SVI3_HIGH_INTR_EN_CMD_NACK_INVALID_ERR_EN_MASK        0x1

// Bitfield Description : 1: Interrupt enable for Firmware send out AXI register access without granted status assertion
#define SVI3_HIGH_INTR_EN_AXI_REQ_WO_GRANT_ERR_EN_OFFSET      1
#define SVI3_HIGH_INTR_EN_AXI_REQ_WO_GRANT_ERR_EN_MASK        0x2

// Bitfield Description : 
#define SVI3_HIGH_INTR_EN_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_EN_OFFSET      2
#define SVI3_HIGH_INTR_EN_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_EN_MASK        0x4

// Bitfield Description : 
#define SVI3_HIGH_INTR_EN_Reserved_31_3_OFFSET      3
#define SVI3_HIGH_INTR_EN_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            CMD_NACK_INVALID_ERR_EN:1;
    UINT32                            AXI_REQ_WO_GRANT_ERR_EN:1;
    UINT32                            CMD_SENT_ADDR_OVER_SLV_NUM_ERR_EN:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} SVI3_HIGH_INTR_EN_STRUCT;

#define SMN_SVI30_SVI3_HIGH_INTR_EN_ADDRESS    0x6f078UL


/***********************************************************
* Register Name : SVI3_HIGH_INTR_ERR_LOG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : request type.
// bit0: vid_chg  bit1: psi bit2: reg_w bit3: reg_r
// bit4: crc_en bit5: crc_dis bit6: return vid bit7: return_reg
// bit8: reg_reset bit9: tel_req
#define SVI3_HIGH_INTR_ERR_LOG_AXI_REQ_WO_GRANT_ERR_LOG_OFFSET      0
#define SVI3_HIGH_INTR_ERR_LOG_AXI_REQ_WO_GRANT_ERR_LOG_MASK        0x3ff

// Bitfield Description : 
#define SVI3_HIGH_INTR_ERR_LOG_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_LOG_OFFSET      10
#define SVI3_HIGH_INTR_ERR_LOG_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_LOG_MASK        0x7c00

// Bitfield Description : 
#define SVI3_HIGH_INTR_ERR_LOG_Reserved_31_15_OFFSET      15
#define SVI3_HIGH_INTR_ERR_LOG_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            AXI_REQ_WO_GRANT_ERR_LOG:10;
    UINT32                            CMD_SENT_ADDR_OVER_SLV_NUM_ERR_LOG:5;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} SVI3_HIGH_INTR_ERR_LOG_STRUCT;

#define SMN_SVI30_SVI3_HIGH_INTR_ERR_LOG_ADDRESS    0x6f07cUL


/***********************************************************
* Register Name : SVI3_HIGH_INTR_SRC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: command NACK invalid error, W1CLR
#define SVI3_HIGH_INTR_SRC_CMD_NACK_INVALID_ERR_OFFSET      0
#define SVI3_HIGH_INTR_SRC_CMD_NACK_INVALID_ERR_MASK        0x1

// Bitfield Description : 1: Firmware send out AXI register access without granted status assertion, W1CLR
#define SVI3_HIGH_INTR_SRC_AXI_REQ_WO_GRANT_ERR_OFFSET      1
#define SVI3_HIGH_INTR_SRC_AXI_REQ_WO_GRANT_ERR_MASK        0x2

// Bitfield Description : 1: address field for all commands sent out to SLAVE is out of range for INIT slave number, W1CLR
#define SVI3_HIGH_INTR_SRC_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_OFFSET      2
#define SVI3_HIGH_INTR_SRC_CMD_SENT_ADDR_OVER_SLV_NUM_ERR_MASK        0x4

// Bitfield Description : 
#define SVI3_HIGH_INTR_SRC_Reserved_31_3_OFFSET      3
#define SVI3_HIGH_INTR_SRC_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            CMD_NACK_INVALID_ERR:1;
    UINT32                            AXI_REQ_WO_GRANT_ERR:1;
    UINT32                            CMD_SENT_ADDR_OVER_SLV_NUM_ERR:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} SVI3_HIGH_INTR_SRC_STRUCT;

#define SMN_SVI30_SVI3_HIGH_INTR_SRC_ADDRESS    0x6f074UL


/***********************************************************
* Register Name : SVI3_LOW_INTR_EN
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: Interrupt enable for new telemetry request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_TEL_REQ_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      0
#define SVI3_LOW_INTR_EN_TEL_REQ_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x1

// Bitfield Description : 1: Interrupt enable for return vid request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_RETURN_VID_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      1
#define SVI3_LOW_INTR_EN_RETURN_VID_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x2

// Bitfield Description : 1: Interrupt enable for return reg request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_RETURN_REG_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      2
#define SVI3_LOW_INTR_EN_RETURN_REG_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x4

// Bitfield Description : 1: Interrupt enable for reg read request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_REG_R_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      3
#define SVI3_LOW_INTR_EN_REG_R_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x8

// Bitfield Description : 1: Interrupt enable for reg write request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_REG_W_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      4
#define SVI3_LOW_INTR_EN_REG_W_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x10

// Bitfield Description : 1: Interrupt enable for reg reset request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_REG_RESET_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      5
#define SVI3_LOW_INTR_EN_REG_RESET_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x20

// Bitfield Description : 1: Interrupt enable for psi change request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_PSI_CHG_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      6
#define SVI3_LOW_INTR_EN_PSI_CHG_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x40

// Bitfield Description : 1: Interrupt enable for vid change request comes without waiting for previous data return done
#define SVI3_LOW_INTR_EN_VID_CHG_TRIG_WO_PREV_DATA_RETURN_EN_OFFSET      7
#define SVI3_LOW_INTR_EN_VID_CHG_TRIG_WO_PREV_DATA_RETURN_EN_MASK        0x80

// Bitfield Description : 1: Interrupt enable for Firmware update Boot vid for multiple times with various value
#define SVI3_LOW_INTR_EN_BOOT_VID_UPDATE_ERR_EN_OFFSET      8
#define SVI3_LOW_INTR_EN_BOOT_VID_UPDATE_ERR_EN_MASK        0x100

// Bitfield Description : 
#define SVI3_LOW_INTR_EN_REG_ACCESS_CNTL_MULTI_TRIG_EN_OFFSET      9
#define SVI3_LOW_INTR_EN_REG_ACCESS_CNTL_MULTI_TRIG_EN_MASK        0x200

// Bitfield Description : 
#define SVI3_LOW_INTR_EN_Reserved_31_10_OFFSET      10
#define SVI3_LOW_INTR_EN_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            TEL_REQ_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            RETURN_VID_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            RETURN_REG_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            REG_R_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            REG_W_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            REG_RESET_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            PSI_CHG_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            VID_CHG_TRIG_WO_PREV_DATA_RETURN_EN:1;
    UINT32                            BOOT_VID_UPDATE_ERR_EN:1;
    UINT32                            REG_ACCESS_CNTL_MULTI_TRIG_EN:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SVI3_LOW_INTR_EN_STRUCT;

#define SMN_SVI30_SVI3_LOW_INTR_EN_ADDRESS    0x6f084UL


/***********************************************************
* Register Name : SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : W1CLR. The corresponding sticky bit which is aligned to ERR_LOG_IDX[5:0] is cleared for
// REG_R interrupt
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_R_ERR_LOG_VLD_OFFSET      0
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_R_ERR_LOG_VLD_MASK        0x1

// Bitfield Description : W1CLR. The corresponding sticky bit which is aligned to ERR_LOG_IDX[5:0] is cleared for
// REG_W interrupt
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_W_ERR_LOG_VLD_OFFSET      1
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_W_ERR_LOG_VLD_MASK        0x2

// Bitfield Description : W1CLR. The corresponding sticky bit which is aligned to ERR_LOG_IDX[5:0] is cleared for
// REG_RESET interrupt
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_RESET_ERR_LOG_VLD_OFFSET      2
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_REG_RESET_ERR_LOG_VLD_MASK        0x4

// Bitfield Description : W1CLR. The corresponding sticky bit which is aligned to ERR_LOG_IDX[5:0] is cleared for
// PSI_CHG interrupt
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_PSI_CHG_ERR_LOG_VLD_OFFSET      3
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_PSI_CHG_ERR_LOG_VLD_MASK        0x8

// Bitfield Description : W1CLR. The corresponding sticky bit which is aligned to ERR_LOG_IDX[5:0] is cleared for
// VID_CHG interrupt
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_VID_CHG_ERR_LOG_VLD_OFFSET      4
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_VID_CHG_ERR_LOG_VLD_MASK        0x10

// Bitfield Description : 
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_ERR_LOG_IDX_OFFSET      5
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_ERR_LOG_IDX_MASK        0x7e0

// Bitfield Description : 
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_Reserved_31_11_OFFSET      11
#define SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            REG_R_ERR_LOG_VLD:1;
    UINT32                            REG_W_ERR_LOG_VLD:1;
    UINT32                            REG_RESET_ERR_LOG_VLD:1;
    UINT32                            PSI_CHG_ERR_LOG_VLD:1;
    UINT32                            VID_CHG_ERR_LOG_VLD:1;
    UINT32                            ERR_LOG_IDX:6;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_STRUCT;

#define SMN_SVI30_SVI3_LOW_INTR_LOG_FOR_DONE_VIOLATE_ERR_ADDRESS    0x6f088UL


/***********************************************************
* Register Name : SVI3_LOW_INTR_SRC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : new telemetry request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_TEL_REQ_TRIG_WO_PREV_DATA_RETURN_OFFSET      0
#define SVI3_LOW_INTR_SRC_TEL_REQ_TRIG_WO_PREV_DATA_RETURN_MASK        0x1

// Bitfield Description : return vid request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_RETURN_VID_TRIG_WO_PREV_DATA_RETURN_OFFSET      1
#define SVI3_LOW_INTR_SRC_RETURN_VID_TRIG_WO_PREV_DATA_RETURN_MASK        0x2

// Bitfield Description : return reg request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_RETURN_REG_TRIG_WO_PREV_DATA_RETURN_OFFSET      2
#define SVI3_LOW_INTR_SRC_RETURN_REG_TRIG_WO_PREV_DATA_RETURN_MASK        0x4

// Bitfield Description : reg read request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_REG_R_TRIG_WO_PREV_DATA_RETURN_OFFSET      3
#define SVI3_LOW_INTR_SRC_REG_R_TRIG_WO_PREV_DATA_RETURN_MASK        0x8

// Bitfield Description : reg write request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_REG_W_TRIG_WO_PREV_DATA_RETURN_OFFSET      4
#define SVI3_LOW_INTR_SRC_REG_W_TRIG_WO_PREV_DATA_RETURN_MASK        0x10

// Bitfield Description : reg reset request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_REG_RESET_TRIG_WO_PREV_DATA_RETURN_OFFSET      5
#define SVI3_LOW_INTR_SRC_REG_RESET_TRIG_WO_PREV_DATA_RETURN_MASK        0x20

// Bitfield Description : psi change request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_PSI_CHG_TRIG_WO_PREV_DATA_RETURN_OFFSET      6
#define SVI3_LOW_INTR_SRC_PSI_CHG_TRIG_WO_PREV_DATA_RETURN_MASK        0x40

// Bitfield Description : vid change request comes without waiting for previous data return done, W1CLR
#define SVI3_LOW_INTR_SRC_VID_CHG_TRIG_WO_PREV_DATA_RETURN_OFFSET      7
#define SVI3_LOW_INTR_SRC_VID_CHG_TRIG_WO_PREV_DATA_RETURN_MASK        0x80

// Bitfield Description : Firmware update Boot vid for multiple times with various value, W1CLR
#define SVI3_LOW_INTR_SRC_BOOT_VID_UPDATE_ERR_OFFSET      8
#define SVI3_LOW_INTR_SRC_BOOT_VID_UPDATE_ERR_MASK        0x100

// Bitfield Description : 
#define SVI3_LOW_INTR_SRC_REG_ACCESS_CNTL_MULTI_TRIG_ERR_OFFSET      9
#define SVI3_LOW_INTR_SRC_REG_ACCESS_CNTL_MULTI_TRIG_ERR_MASK        0x200

// Bitfield Description : 
#define SVI3_LOW_INTR_SRC_Reserved_31_10_OFFSET      10
#define SVI3_LOW_INTR_SRC_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            TEL_REQ_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            RETURN_VID_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            RETURN_REG_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            REG_R_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            REG_W_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            REG_RESET_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            PSI_CHG_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            VID_CHG_TRIG_WO_PREV_DATA_RETURN:1;
    UINT32                            BOOT_VID_UPDATE_ERR:1;
    UINT32                            REG_ACCESS_CNTL_MULTI_TRIG_ERR:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SVI3_LOW_INTR_SRC_STRUCT;

#define SMN_SVI30_SVI3_LOW_INTR_SRC_ADDRESS    0x6f080UL


/***********************************************************
* Register Name : SVI3_PLANE_MAP_CFG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : plane mapping configuration register
#define SVI3_PLANE_MAP_CFG_PLANE_MAP_CFG_OFFSET      0
#define SVI3_PLANE_MAP_CFG_PLANE_MAP_CFG_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PLANE_MAP_CFG:32;
  } Field;
  UINT32 Value;
} SVI3_PLANE_MAP_CFG_STRUCT;

#define SMN_SVI30_SVI3_PLANE_MAP_CFG_ADDRESS    0x6f0f4UL


/***********************************************************
* Register Name : SVI3_PSI_CHG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : ADDRESS field for PSI change packet. Currently it ranges from 1~8. (0 is reserved)
#define SVI3_PSI_CHG_TARGET_ADDR_OFFSET      0
#define SVI3_PSI_CHG_TARGET_ADDR_MASK        0x3f

// Bitfield Description : PSI field for PSI change packet
#define SVI3_PSI_CHG_TARGET_PSI_OFFSET      6
#define SVI3_PSI_CHG_TARGET_PSI_MASK        0x1c0

// Bitfield Description : 
#define SVI3_PSI_CHG_Reserved_14_9_OFFSET      9
#define SVI3_PSI_CHG_Reserved_14_9_MASK        0x7e00

// Bitfield Description : write 1 to trigger a PSI change packet sending
#define SVI3_PSI_CHG_PSI_CHG_TRIGGER_OFFSET      15
#define SVI3_PSI_CHG_PSI_CHG_TRIGGER_MASK        0x8000

// Bitfield Description : 
#define SVI3_PSI_CHG_Reserved_31_16_OFFSET      16
#define SVI3_PSI_CHG_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            TARGET_ADDR:6;
    UINT32                            TARGET_PSI:3;
    UINT32                            Reserved_14_9:6;
    UINT32                            PSI_CHG_TRIGGER:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} SVI3_PSI_CHG_STRUCT;

#define SMN_SVI30_SVI3_PSI_CHG_ADDRESS    0x6f01cUL


/***********************************************************
* Register Name : SVI3_REG_ACCESS_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to:
// 1, configure other info in current registers.
// 2, read data from SVI3_REG_ACCESS_STATUS.
// It ranges from 1~8 (0 is reserved)
#define SVI3_REG_ACCESS_CNTL_ADDR_OFFSET      0
#define SVI3_REG_ACCESS_CNTL_ADDR_MASK        0x3f

// Bitfield Description : OFFSET field for all register related packets (reg_r/w/reset)
#define SVI3_REG_ACCESS_CNTL_OFFSET_OFFSET      6
#define SVI3_REG_ACCESS_CNTL_OFFSET_MASK        0x3fc0

// Bitfield Description : WR_DATA field for register write packets
#define SVI3_REG_ACCESS_CNTL_WR_DATA_OFFSET      14
#define SVI3_REG_ACCESS_CNTL_WR_DATA_MASK        0x3fc000

// Bitfield Description : MGF field for all register related packets  (reg_r/w/reset)
#define SVI3_REG_ACCESS_CNTL_WR_MFG_OFFSET      22
#define SVI3_REG_ACCESS_CNTL_WR_MFG_MASK        0x400000

// Bitfield Description : 
#define SVI3_REG_ACCESS_CNTL_Reserved_23_23_OFFSET      23
#define SVI3_REG_ACCESS_CNTL_Reserved_23_23_MASK        0x800000

// Bitfield Description : write 1 to trigger register write packet sending
#define SVI3_REG_ACCESS_CNTL_WRITE_TRIGGER_OFFSET      24
#define SVI3_REG_ACCESS_CNTL_WRITE_TRIGGER_MASK        0x1000000

// Bitfield Description : write 1 to trigger register read packet sending
#define SVI3_REG_ACCESS_CNTL_READ_TRIGGER_OFFSET      25
#define SVI3_REG_ACCESS_CNTL_READ_TRIGGER_MASK        0x2000000

// Bitfield Description : write 1 to trigger register reset packet sending
#define SVI3_REG_ACCESS_CNTL_RESET_TRIGGER_OFFSET      26
#define SVI3_REG_ACCESS_CNTL_RESET_TRIGGER_MASK        0x4000000

// Bitfield Description : Register Type field for register reset packet sending
#define SVI3_REG_ACCESS_CNTL_RESET_TYPE_OFFSET      27
#define SVI3_REG_ACCESS_CNTL_RESET_TYPE_MASK        0x38000000

// Bitfield Description : 
#define SVI3_REG_ACCESS_CNTL_Reserved_31_30_OFFSET      30
#define SVI3_REG_ACCESS_CNTL_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ADDR:6;
    UINT32                            OFFSET:8;
    UINT32                            WR_DATA:8;
    UINT32                            WR_MFG:1;
    UINT32                            Reserved_23_23:1;
    UINT32                            WRITE_TRIGGER:1;
    UINT32                            READ_TRIGGER:1;
    UINT32                            RESET_TRIGGER:1;
    UINT32                            RESET_TYPE:3;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} SVI3_REG_ACCESS_CNTL_STRUCT;

#define SMN_SVI30_SVI3_REG_ACCESS_CNTL_ADDRESS    0x6f030UL


/***********************************************************
* Register Name : SVI3_REG_ACCESS_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Register Index field for register read return packet
#define SVI3_REG_ACCESS_STATUS_RD_INDEX_OFFSET      0
#define SVI3_REG_ACCESS_STATUS_RD_INDEX_MASK        0xff

// Bitfield Description : Register data field for register read return packet
#define SVI3_REG_ACCESS_STATUS_RD_DATA_OFFSET      8
#define SVI3_REG_ACCESS_STATUS_RD_DATA_MASK        0xff00

// Bitfield Description : Mfg field for register read return packet
#define SVI3_REG_ACCESS_STATUS_RD_MFG_OFFSET      16
#define SVI3_REG_ACCESS_STATUS_RD_MFG_MASK        0x10000

// Bitfield Description : 
#define SVI3_REG_ACCESS_STATUS_Reserved_23_17_OFFSET      17
#define SVI3_REG_ACCESS_STATUS_Reserved_23_17_MASK        0xfe0000

// Bitfield Description : 1: register write is done by receiving slave ACK.
#define SVI3_REG_ACCESS_STATUS_WR_DONE_OFFSET      24
#define SVI3_REG_ACCESS_STATUS_WR_DONE_MASK        0x1000000

// Bitfield Description : 1: register read is done by receiving slave ACK.
#define SVI3_REG_ACCESS_STATUS_RD_DONE_OFFSET      25
#define SVI3_REG_ACCESS_STATUS_RD_DONE_MASK        0x2000000

// Bitfield Description : 1: register reset is done based on the following rules:
// 1), if register_type[0]=1, it is set after VID change calculation done by up/ down delta VID.
// 2), if register_type[1]=1, it is set after slave ack returned.
// 3), if register_type[2]=1, it is set after slave ack returned.
#define SVI3_REG_ACCESS_STATUS_RESET_DONE_OFFSET      26
#define SVI3_REG_ACCESS_STATUS_RESET_DONE_MASK        0x4000000

// Bitfield Description : 1: all the slaves have register write done triggered by registers.
#define SVI3_REG_ACCESS_STATUS_ALL_WR_DONE_OFFSET      27
#define SVI3_REG_ACCESS_STATUS_ALL_WR_DONE_MASK        0x8000000

// Bitfield Description : 1: all the slaves have register read done triggered by registers.
#define SVI3_REG_ACCESS_STATUS_ALL_RD_DONE_OFFSET      28
#define SVI3_REG_ACCESS_STATUS_ALL_RD_DONE_MASK        0x10000000

// Bitfield Description : 1: all the slaves have register reset done triggered by registers.
#define SVI3_REG_ACCESS_STATUS_ALL_RESET_DONE_OFFSET      29
#define SVI3_REG_ACCESS_STATUS_ALL_RESET_DONE_MASK        0x20000000

// Bitfield Description : 1: register read packet returns read data valid, it is cleared when corresponding register read trigger is
// received for the slave.
#define SVI3_REG_ACCESS_STATUS_RD_DATA_VALID_OFFSET      30
#define SVI3_REG_ACCESS_STATUS_RD_DATA_VALID_MASK        0x40000000

// Bitfield Description : 
#define SVI3_REG_ACCESS_STATUS_Reserved_31_31_OFFSET      31
#define SVI3_REG_ACCESS_STATUS_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RD_INDEX:8;
    UINT32                            RD_DATA:8;
    UINT32                            RD_MFG:1;
    UINT32                            Reserved_23_17:7;
    UINT32                            WR_DONE:1;
    UINT32                            RD_DONE:1;
    UINT32                            RESET_DONE:1;
    UINT32                            ALL_WR_DONE:1;
    UINT32                            ALL_RD_DONE:1;
    UINT32                            ALL_RESET_DONE:1;
    UINT32                            RD_DATA_VALID:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} SVI3_REG_ACCESS_STATUS_STRUCT;

#define SMN_SVI30_SVI3_REG_ACCESS_STATUS_ADDRESS    0x6f034UL


/***********************************************************
* Register Name : SVI3_S0IX_SIDEBAND
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: disable FCH_SMUIO_VIDCHG_REQ to trigger S0ix exit 0: enable
#define SVI3_S0IX_SIDEBAND_FCH_SMUIO_VIDCHG_REQ_MASK_OFFSET      0
#define SVI3_S0IX_SIDEBAND_FCH_SMUIO_VIDCHG_REQ_MASK_MASK        0x1

// Bitfield Description : 1: disable SMUIO_MP_RESET_INTR to trigger S0ix exit 0: enable
#define SVI3_S0IX_SIDEBAND_SMUIO_MP_RESET_INTR_REQ_MASK_OFFSET      1
#define SVI3_S0IX_SIDEBAND_SMUIO_MP_RESET_INTR_REQ_MASK_MASK        0x2

// Bitfield Description : 1: disable NBIO_CLK_LCLK_CTRL to trigger S0ix exit 0: enable
#define SVI3_S0IX_SIDEBAND_NBIO_CLK_LCLK_CTRL_REQ_MASK_OFFSET      2
#define SVI3_S0IX_SIDEBAND_NBIO_CLK_LCLK_CTRL_REQ_MASK_MASK        0x4

// Bitfield Description : 1: disable Cpl_DF_SmuWake to trigger S0ix exit 0: enable
#define SVI3_S0IX_SIDEBAND_CPL_DF_SMUWAKE_REQ_MASK_OFFSET      3
#define SVI3_S0IX_SIDEBAND_CPL_DF_SMUWAKE_REQ_MASK_MASK        0x8

// Bitfield Description : write 1 to clear ACK for SMUIO_MP1_VID_CHG_ack
#define SVI3_S0IX_SIDEBAND_S0IX_MP_ACK_CLR_OFFSET      4
#define SVI3_S0IX_SIDEBAND_S0IX_MP_ACK_CLR_MASK        0x10

// Bitfield Description : write 1 to trigger the start of IP to latch the all of the S0IX exit requests at this point.
#define SVI3_S0IX_SIDEBAND_S0IX_ENTRY_ST_OFFSET      5
#define SVI3_S0IX_SIDEBAND_S0IX_ENTRY_ST_MASK        0x20

// Bitfield Description : 
#define SVI3_S0IX_SIDEBAND_Reserved_31_6_OFFSET      6
#define SVI3_S0IX_SIDEBAND_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            FCH_SMUIO_VIDCHG_REQ_MASK:1;
    UINT32                            SMUIO_MP_RESET_INTR_REQ_MASK:1;
    UINT32                            NBIO_CLK_LCLK_CTRL_REQ_MASK:1;
    UINT32                            CPL_DF_SMUWAKE_REQ_MASK:1;
    UINT32                            S0IX_MP_ACK_CLR:1;
    UINT32                            S0IX_ENTRY_ST:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} SVI3_S0IX_SIDEBAND_STRUCT;

#define SMN_SVI30_SVI3_S0IX_SIDEBAND_ADDRESS    0x6f040UL


/***********************************************************
* Register Name : SVI3_SIDEBAND_CNTL_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: Sideband ADDRESS override enable;
// 0: Sideband ADDRESS override disable.
// In current application:
// 1), sideband #0~#5 ADDR is tied off, so ADDR_OVERRIDE_EN should be set as 1'b1.
#define SVI3_SIDEBAND_CNTL_DATA_ADDR_OVERRIDE_EN_OFFSET      0
#define SVI3_SIDEBAND_CNTL_DATA_ADDR_OVERRIDE_EN_MASK        0x1

// Bitfield Description : Sideband ADDRESS override if ADDR_OVERRIDE_EN=1'b1.
// For S0ix rail (sideband #6), always use override info
#define SVI3_SIDEBAND_CNTL_DATA_ADDR_OVERRIDE_OFFSET      1
#define SVI3_SIDEBAND_CNTL_DATA_ADDR_OVERRIDE_MASK        0x7e

// Bitfield Description : 1: Sideband VID override enable; 0: Sideband VID override enable
#define SVI3_SIDEBAND_CNTL_DATA_VID_OVERRIDE_EN_OFFSET      7
#define SVI3_SIDEBAND_CNTL_DATA_VID_OVERRIDE_EN_MASK        0x80

// Bitfield Description : Sideband VID override if VID_OVERRIDE_EN=1'b1.
// For S0ix rail (sideband #6), always use override info
#define SVI3_SIDEBAND_CNTL_DATA_VID_OVERRIDE_OFFSET      8
#define SVI3_SIDEBAND_CNTL_DATA_VID_OVERRIDE_MASK        0x1ff00

// Bitfield Description : 1: send sideband ACK when VID chang calculation done
// 0: send sideband ACK when SVI3 get ACK response
// For S0ix rail, when Firmare triggers SVI3_VID_CHG.S0IX_RAIL together with SVI3_VID_CHG.VID_CHG_TRIGGER, S0ix process will be entered once SVI3 get target VID calculation done. For S0ix exit process, it based on this configuration to exit and assert ACK to MP. But ACK to sideband is always set as 1'b0.
#define SVI3_SIDEBAND_CNTL_DATA_SEND_ACK_ON_VID_CHG_DONE_OFFSET      17
#define SVI3_SIDEBAND_CNTL_DATA_SEND_ACK_ON_VID_CHG_DONE_MASK        0x20000

// Bitfield Description : 1: Sideband interface enable. Ignore the corresponding bit for the last interface (S0IX FCH.)
#define SVI3_SIDEBAND_CNTL_DATA_SIDEBAND_EN_OFFSET      18
#define SVI3_SIDEBAND_CNTL_DATA_SIDEBAND_EN_MASK        0x40000

// Bitfield Description : 1: Sideband PSI override enable; 0: Sideband PSI override enable
#define SVI3_SIDEBAND_CNTL_DATA_PSI_OVERRIDE_EN_OFFSET      19
#define SVI3_SIDEBAND_CNTL_DATA_PSI_OVERRIDE_EN_MASK        0x80000

// Bitfield Description : Sideband PSI override if PSI_OVERRIDE_EN=1'b1. For S0IX rail of sideband, PSI is not supported.
#define SVI3_SIDEBAND_CNTL_DATA_PSI_OVERRIDE_OFFSET      20
#define SVI3_SIDEBAND_CNTL_DATA_PSI_OVERRIDE_MASK        0x700000

// Bitfield Description : 1: sideband VID calculation done. It will be cleared if sideband triggers a new VID change request.
#define SVI3_SIDEBAND_CNTL_DATA_SB_VID_DONE_OFFSET      23
#define SVI3_SIDEBAND_CNTL_DATA_SB_VID_DONE_MASK        0x800000

// Bitfield Description : 
#define SVI3_SIDEBAND_CNTL_DATA_Reserved_31_24_OFFSET      24
#define SVI3_SIDEBAND_CNTL_DATA_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            ADDR_OVERRIDE_EN:1;
    UINT32                            ADDR_OVERRIDE:6;
    UINT32                            VID_OVERRIDE_EN:1;
    UINT32                            VID_OVERRIDE:9;
    UINT32                            SEND_ACK_ON_VID_CHG_DONE:1;
    UINT32                            SIDEBAND_EN:1;
    UINT32                            PSI_OVERRIDE_EN:1;
    UINT32                            PSI_OVERRIDE:3;
    UINT32                            SB_VID_DONE:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SVI3_SIDEBAND_CNTL_DATA_STRUCT;

#define SMN_SVI30_SVI3_SIDEBAND_CNTL_DATA_ADDRESS    0x6f03cUL


/***********************************************************
* Register Name : SVI3_SIDEBAND_CNTL_INDEX
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : sideband number, which targets to select the specific sideband interface to configure its SVI3_SIDEBAND_CNTL_DATA.
// It ranges from 0~6
#define SVI3_SIDEBAND_CNTL_INDEX_SIDEBAND_NUM_OFFSET      0
#define SVI3_SIDEBAND_CNTL_INDEX_SIDEBAND_NUM_MASK        0x7

// Bitfield Description : 
#define SVI3_SIDEBAND_CNTL_INDEX_Reserved_31_3_OFFSET      3
#define SVI3_SIDEBAND_CNTL_INDEX_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            SIDEBAND_NUM:3;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} SVI3_SIDEBAND_CNTL_INDEX_STRUCT;

#define SMN_SVI30_SVI3_SIDEBAND_CNTL_INDEX_ADDRESS    0x6f038UL


/***********************************************************
* Register Name : SVI3_SLEW_CNTL_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: up VID change for calculation enables slew rate method. Then VID done is calculated by:
// SLEW_RATE_UP_CNT*delta Voltage
// 0: up VID change for calculation enables fix counter method. Then VID done is calculated by:
// FIXED_VOL_UP_CNT[7:0]
#define SVI3_SLEW_CNTL_DATA_EN_SLEW_RATE_UP_OFFSET      0
#define SVI3_SLEW_CNTL_DATA_EN_SLEW_RATE_UP_MASK        0x1

// Bitfield Description : Each bit indicates if down slew rate is enabled for different PSI state.
// For each bit, if it is high, that means its decay condition is not met for that particular PSI state, then it will use SLEW_RATE_DOWN_CNT[3:0] or one-forth of SLEW_RATE_DOWN_CNT[3:0] configuration to calculate VID_CHG_DONE, based on DOWN_SLEW_RATE setting.
// bit[2]: decay in PSI0/1/2; bit[1]: PSI3/4/7; bit[0]: PSI6
// 1: down VID change for calculation enables slew rate method. Then VID done is calculated by:
// SLEW_RATE_UP_CNT*delta Voltage
// 0: down VID change for calculation enables fix counter method. Then VID done is calculated by:
// FIXED_VOL_UP_CNT[7:0]
#define SVI3_SLEW_CNTL_DATA_EN_SLEW_RATE_DOWN_OFFSET      1
#define SVI3_SLEW_CNTL_DATA_EN_SLEW_RATE_DOWN_MASK        0xe

// Bitfield Description : up slew_rate = reg[3:0]*2.5 + 2.5mv/us, default is FUSE strap
#define SVI3_SLEW_CNTL_DATA_SLEW_RATE_UP_CNT_OFFSET      4
#define SVI3_SLEW_CNTL_DATA_SLEW_RATE_UP_CNT_MASK        0xf0

// Bitfield Description : down slew_rate = reg[3:0]*2.5 + 2.5mv/us, default is FUSE strap
#define SVI3_SLEW_CNTL_DATA_SLEW_RATE_DOWN_CNT_OFFSET      8
#define SVI3_SLEW_CNTL_DATA_SLEW_RATE_DOWN_CNT_MASK        0xf00

// Bitfield Description : Fixed voltage up done counter, unit is 1 svi3_refclk, in Apu projects, it's 24Mhz; in Server projects, it's 100Mhz.
#define SVI3_SLEW_CNTL_DATA_FIXED_VOL_UP_CNT_OFFSET      12
#define SVI3_SLEW_CNTL_DATA_FIXED_VOL_UP_CNT_MASK        0xff000

// Bitfield Description : fixed voltage down done counter, unit is 1 svi3_refclk, in Apu projects, it's 24Mhz; in Server projects, it's 100Mhz.
#define SVI3_SLEW_CNTL_DATA_FIXED_VOL_DOWN_CNT_OFFSET      20
#define SVI3_SLEW_CNTL_DATA_FIXED_VOL_DOWN_CNT_MASK        0xff00000

// Bitfield Description : slew rate when decay condition not met. 1 to use one-fourth of negative slew rate, 0 to use negative slew rate
#define SVI3_SLEW_CNTL_DATA_DOWN_SLEW_RATE_OFFSET      28
#define SVI3_SLEW_CNTL_DATA_DOWN_SLEW_RATE_MASK        0x10000000

// Bitfield Description : down decay condition in different PSI state enable.
// 0: down slew rate enable is indicated by EN_SLEW_RATE_DOWN[0];
// 1: down slew rate enable is indicated by EN_SLEW_RATE_DOWN[2:0] for different PSI statte.
#define SVI3_SLEW_CNTL_DATA_PSI_DOWN_DECAY_EN_OFFSET      29
#define SVI3_SLEW_CNTL_DATA_PSI_DOWN_DECAY_EN_MASK        0x20000000

// Bitfield Description : 
#define SVI3_SLEW_CNTL_DATA_Reserved_31_30_OFFSET      30
#define SVI3_SLEW_CNTL_DATA_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            EN_SLEW_RATE_UP:1;
    UINT32                            EN_SLEW_RATE_DOWN:3;
    UINT32                            SLEW_RATE_UP_CNT:4;
    UINT32                            SLEW_RATE_DOWN_CNT:4;
    UINT32                            FIXED_VOL_UP_CNT:8;
    UINT32                            FIXED_VOL_DOWN_CNT:8;
    UINT32                            DOWN_SLEW_RATE:1;
    UINT32                            PSI_DOWN_DECAY_EN:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} SVI3_SLEW_CNTL_DATA_STRUCT;

#define SMN_SVI30_SVI3_SLEW_CNTL_DATA_ADDRESS    0x6f028UL


/***********************************************************
* Register Name : SVI3_SLEW_CNTL_INDEX
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to configure its SVI3_SLEW_CNTL_DATA.
// It ranges from 1~8 (0 is reserved)
#define SVI3_SLEW_CNTL_INDEX_TARGET_ADDR_OFFSET      0
#define SVI3_SLEW_CNTL_INDEX_TARGET_ADDR_MASK        0x3f

// Bitfield Description : 
#define SVI3_SLEW_CNTL_INDEX_Reserved_31_6_OFFSET      6
#define SVI3_SLEW_CNTL_INDEX_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            TARGET_ADDR:6;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} SVI3_SLEW_CNTL_INDEX_STRUCT;

#define SMN_SVI30_SVI3_SLEW_CNTL_INDEX_ADDRESS    0x6f024UL


/***********************************************************
* Register Name : SVI3_SLEW_CNTL_MAX_CNT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : fixed counter used for global return register and return vid packets for VID done calculation.
// unit is 1 svi3_refclk, in Apu projects, it's 24Mhz; in Server projects, it's 100Mhz.
#define SVI3_SLEW_CNTL_MAX_CNT_VID_CHG_MAX_CNT_OFFSET      0
#define SVI3_SLEW_CNTL_MAX_CNT_VID_CHG_MAX_CNT_MASK        0xffff

// Bitfield Description : Firmware extra VID Change Done delay counter, unit is 1 svi3_refclk, in Apu projects, it's 24Mhz; in Server projects, it's 100Mhz.
#define SVI3_SLEW_CNTL_MAX_CNT_FW_VID_CHG_DELAY_CNT_OFFSET      16
#define SVI3_SLEW_CNTL_MAX_CNT_FW_VID_CHG_DELAY_CNT_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VID_CHG_MAX_CNT:16;
    UINT32                            FW_VID_CHG_DELAY_CNT:16;
  } Field;
  UINT32 Value;
} SVI3_SLEW_CNTL_MAX_CNT_STRUCT;

#define SMN_SVI30_SVI3_SLEW_CNTL_MAX_CNT_ADDRESS    0x6f02cUL


/***********************************************************
* Register Name : SVI3_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: set when arbiter has scheduled out current command triggered by register access
// 0: cleared when Firmware triggers next SVI3 packet command configure by register.
#define SVI3_STATUS_CMD_GRANTED_OFFSET      0
#define SVI3_STATUS_CMD_GRANTED_MASK        0x1

// Bitfield Description : 1: CRC enabled, 0: CRC disabled
#define SVI3_STATUS_CRC_STATUS_OFFSET      1
#define SVI3_STATUS_CRC_STATUS_MASK        0x2

// Bitfield Description : 
#define SVI3_STATUS_Reserved_7_2_OFFSET      2
#define SVI3_STATUS_Reserved_7_2_MASK        0xfc

// Bitfield Description : 0: bus disabled
// 1: bus enable but idle without toggle on SVC/SVT/SVD
// 2: bus pending
// 3: bus busy
#define SVI3_STATUS_BUS_STATUS_OFFSET      8
#define SVI3_STATUS_BUS_STATUS_MASK        0x300

// Bitfield Description : 0: bus initialization done is not finished
// 1: bus initialization done is finished
#define SVI3_STATUS_BUS_INIT_DONE_OFFSET      10
#define SVI3_STATUS_BUS_INIT_DONE_MASK        0x400

// Bitfield Description : 
#define SVI3_STATUS_SLAVE_NUM_OFFSET      11
#define SVI3_STATUS_SLAVE_NUM_MASK        0x1f800

// Bitfield Description : 
#define SVI3_STATUS_Reserved_31_17_OFFSET      17
#define SVI3_STATUS_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            CMD_GRANTED:1;
    UINT32                            CRC_STATUS:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            BUS_STATUS:2;
    UINT32                            BUS_INIT_DONE:1;
    UINT32                            SLAVE_NUM:6;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} SVI3_STATUS_STRUCT;

#define SMN_SVI30_SVI3_STATUS_ADDRESS    0x6f004UL


/***********************************************************
* Register Name : SVI3_SVC_PAD_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SVC PAD schmit enable
#define SVI3_SVC_PAD_CNTL_SCHMEN_OFFSET      0
#define SVI3_SVC_PAD_CNTL_SCHMEN_MASK        0x1

// Bitfield Description : SVC PAD pull up
#define SVI3_SVC_PAD_CNTL_PU_OFFSET      1
#define SVI3_SVC_PAD_CNTL_PU_MASK        0x2

// Bitfield Description : SVC PAD pull down
#define SVI3_SVC_PAD_CNTL_PD_OFFSET      2
#define SVI3_SVC_PAD_CNTL_PD_MASK        0x4

// Bitfield Description : SVC PAD strengh control 0
#define SVI3_SVC_PAD_CNTL_S0_OFFSET      3
#define SVI3_SVC_PAD_CNTL_S0_MASK        0x8

// Bitfield Description : SVC PAD strengh control 1
#define SVI3_SVC_PAD_CNTL_S1_OFFSET      4
#define SVI3_SVC_PAD_CNTL_S1_MASK        0x10

// Bitfield Description : SVC PAD output enable override enable
#define SVI3_SVC_PAD_CNTL_OE_OVERRIDE_OFFSET      5
#define SVI3_SVC_PAD_CNTL_OE_OVERRIDE_MASK        0x20

// Bitfield Description : SVC PAD output enable
#define SVI3_SVC_PAD_CNTL_OE_OFFSET      6
#define SVI3_SVC_PAD_CNTL_OE_MASK        0x40

// Bitfield Description : SVC PAD output data override enable
#define SVI3_SVC_PAD_CNTL_A_OVERRIDE_OFFSET      7
#define SVI3_SVC_PAD_CNTL_A_OVERRIDE_MASK        0x80

// Bitfield Description : SVC PAD output data
#define SVI3_SVC_PAD_CNTL_A_OFFSET      8
#define SVI3_SVC_PAD_CNTL_A_MASK        0x100

// Bitfield Description : 
#define SVI3_SVC_PAD_CNTL_Reserved_30_9_OFFSET      9
#define SVI3_SVC_PAD_CNTL_Reserved_30_9_MASK        0x7ffffe00

// Bitfield Description : SVC PAD input value
#define SVI3_SVC_PAD_CNTL_Y_OFFSET      31
#define SVI3_SVC_PAD_CNTL_Y_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SCHMEN:1;
    UINT32                            PU:1;
    UINT32                            PD:1;
    UINT32                            S0:1;
    UINT32                            S1:1;
    UINT32                            OE_OVERRIDE:1;
    UINT32                            OE:1;
    UINT32                            A_OVERRIDE:1;
    UINT32                            A:1;
    UINT32                            Reserved_30_9:22;
    UINT32                            Y:1;
  } Field;
  UINT32 Value;
} SVI3_SVC_PAD_CNTL_STRUCT;

#define SMN_SVI30_SVI3_SVC_PAD_CNTL_ADDRESS    0x6f0c0UL


/***********************************************************
* Register Name : SVI3_SVD_PAD_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SVD PAD schmit enable
#define SVI3_SVD_PAD_CNTL_SCHMEN_OFFSET      0
#define SVI3_SVD_PAD_CNTL_SCHMEN_MASK        0x1

// Bitfield Description : SVD PAD pull up
#define SVI3_SVD_PAD_CNTL_PU_OFFSET      1
#define SVI3_SVD_PAD_CNTL_PU_MASK        0x2

// Bitfield Description : SVD PAD pull down
#define SVI3_SVD_PAD_CNTL_PD_OFFSET      2
#define SVI3_SVD_PAD_CNTL_PD_MASK        0x4

// Bitfield Description : SVD PAD strengh control 0
#define SVI3_SVD_PAD_CNTL_S0_OFFSET      3
#define SVI3_SVD_PAD_CNTL_S0_MASK        0x8

// Bitfield Description : SVD PAD strengh control 1
#define SVI3_SVD_PAD_CNTL_S1_OFFSET      4
#define SVI3_SVD_PAD_CNTL_S1_MASK        0x10

// Bitfield Description : SVD PAD output enable override enable
#define SVI3_SVD_PAD_CNTL_OE_OVERRIDE_OFFSET      5
#define SVI3_SVD_PAD_CNTL_OE_OVERRIDE_MASK        0x20

// Bitfield Description : SVD PAD output enable
#define SVI3_SVD_PAD_CNTL_OE_OFFSET      6
#define SVI3_SVD_PAD_CNTL_OE_MASK        0x40

// Bitfield Description : SVD PAD output data override enable
#define SVI3_SVD_PAD_CNTL_A_OVERRIDE_OFFSET      7
#define SVI3_SVD_PAD_CNTL_A_OVERRIDE_MASK        0x80

// Bitfield Description : SVD PAD output data
#define SVI3_SVD_PAD_CNTL_A_OFFSET      8
#define SVI3_SVD_PAD_CNTL_A_MASK        0x100

// Bitfield Description : 
#define SVI3_SVD_PAD_CNTL_Reserved_30_9_OFFSET      9
#define SVI3_SVD_PAD_CNTL_Reserved_30_9_MASK        0x7ffffe00

// Bitfield Description : SVD PAD input value
#define SVI3_SVD_PAD_CNTL_Y_OFFSET      31
#define SVI3_SVD_PAD_CNTL_Y_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SCHMEN:1;
    UINT32                            PU:1;
    UINT32                            PD:1;
    UINT32                            S0:1;
    UINT32                            S1:1;
    UINT32                            OE_OVERRIDE:1;
    UINT32                            OE:1;
    UINT32                            A_OVERRIDE:1;
    UINT32                            A:1;
    UINT32                            Reserved_30_9:22;
    UINT32                            Y:1;
  } Field;
  UINT32 Value;
} SVI3_SVD_PAD_CNTL_STRUCT;

#define SMN_SVI30_SVI3_SVD_PAD_CNTL_ADDRESS    0x6f0c4UL


/***********************************************************
* Register Name : SVI3_SVT_PAD_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SVT PAD schmit enable
#define SVI3_SVT_PAD_CNTL_SCHMEN_OFFSET      0
#define SVI3_SVT_PAD_CNTL_SCHMEN_MASK        0x1

// Bitfield Description : SVT PAD pull up
#define SVI3_SVT_PAD_CNTL_PU_OFFSET      1
#define SVI3_SVT_PAD_CNTL_PU_MASK        0x2

// Bitfield Description : SVT PAD pull down
#define SVI3_SVT_PAD_CNTL_PD_OFFSET      2
#define SVI3_SVT_PAD_CNTL_PD_MASK        0x4

// Bitfield Description : SVT PAD strengh control 0
#define SVI3_SVT_PAD_CNTL_S0_OFFSET      3
#define SVI3_SVT_PAD_CNTL_S0_MASK        0x8

// Bitfield Description : SVT PAD strengh control 1
#define SVI3_SVT_PAD_CNTL_S1_OFFSET      4
#define SVI3_SVT_PAD_CNTL_S1_MASK        0x10

// Bitfield Description : SVT PAD output enable override enable
#define SVI3_SVT_PAD_CNTL_OE_OVERRIDE_OFFSET      5
#define SVI3_SVT_PAD_CNTL_OE_OVERRIDE_MASK        0x20

// Bitfield Description : SVT PAD output enable
#define SVI3_SVT_PAD_CNTL_OE_OFFSET      6
#define SVI3_SVT_PAD_CNTL_OE_MASK        0x40

// Bitfield Description : SVT PAD output data override enable
#define SVI3_SVT_PAD_CNTL_A_OVERRIDE_OFFSET      7
#define SVI3_SVT_PAD_CNTL_A_OVERRIDE_MASK        0x80

// Bitfield Description : SVT PAD output data
#define SVI3_SVT_PAD_CNTL_A_OFFSET      8
#define SVI3_SVT_PAD_CNTL_A_MASK        0x100

// Bitfield Description : 
#define SVI3_SVT_PAD_CNTL_Reserved_30_9_OFFSET      9
#define SVI3_SVT_PAD_CNTL_Reserved_30_9_MASK        0x7ffffe00

// Bitfield Description : SVT PAD input value
#define SVI3_SVT_PAD_CNTL_Y_OFFSET      31
#define SVI3_SVT_PAD_CNTL_Y_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SCHMEN:1;
    UINT32                            PU:1;
    UINT32                            PD:1;
    UINT32                            S0:1;
    UINT32                            S1:1;
    UINT32                            OE_OVERRIDE:1;
    UINT32                            OE:1;
    UINT32                            A_OVERRIDE:1;
    UINT32                            A:1;
    UINT32                            Reserved_30_9:22;
    UINT32                            Y:1;
  } Field;
  UINT32 Value;
} SVI3_SVT_PAD_CNTL_STRUCT;

#define SMN_SVI30_SVI3_SVT_PAD_CNTL_ADDRESS    0x6f0c8UL


/***********************************************************
* Register Name : SVI3_TELEMETRY_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current slave adderss, which targets to select the specific slave to report DATA/TEL_DATA_VALID info  in current register.
// It ranges from 1~8 (0 is reserved)
#define SVI3_TELEMETRY_DATA_ADDR_OFFSET      0
#define SVI3_TELEMETRY_DATA_ADDR_MASK        0x3f

// Bitfield Description : current Telemetry Index, which targets to select the specific index to report DATA/TEL_DATA_VALID info  in current register.
#define SVI3_TELEMETRY_DATA_INDEX_OFFSET      6
#define SVI3_TELEMETRY_DATA_INDEX_MASK        0x1c0

// Bitfield Description : current Telemetry format, which targets to select the specific format to report DATA/TEL_DATA_VALID info  in current register.
#define SVI3_TELEMETRY_DATA_FORMAT_OFFSET      9
#define SVI3_TELEMETRY_DATA_FORMAT_MASK        0x200

// Bitfield Description : telemetry data
#define SVI3_TELEMETRY_DATA_DATA_OFFSET      10
#define SVI3_TELEMETRY_DATA_DATA_MASK        0x3fffc00

// Bitfield Description : 1: indicate the telemetry data for the specific addr/index/format is valid. it is cleared when corresponding telemetery request trigger is received by registers.
#define SVI3_TELEMETRY_DATA_TEL_DATA_VALID_OFFSET      26
#define SVI3_TELEMETRY_DATA_TEL_DATA_VALID_MASK        0x4000000

// Bitfield Description : 
#define SVI3_TELEMETRY_DATA_Reserved_31_27_OFFSET      27
#define SVI3_TELEMETRY_DATA_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            ADDR:6;
    UINT32                            INDEX:3;
    UINT32                            FORMAT:1;
    UINT32                            DATA:16;
    UINT32                            TEL_DATA_VALID:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} SVI3_TELEMETRY_DATA_STRUCT;

#define SMN_SVI30_SVI3_TELEMETRY_DATA_ADDRESS    0x6f044UL


/***********************************************************
* Register Name : SVI3_TEL_TRACE_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : current write pointer for TEL trace buffer
#define SVI3_TEL_TRACE_CNTL_WR_PTR_OFFSET      0
#define SVI3_TEL_TRACE_CNTL_WR_PTR_MASK        0xf

// Bitfield Description : read pointer for TEL trace buffer
#define SVI3_TEL_TRACE_CNTL_RD_PTR_OFFSET      4
#define SVI3_TEL_TRACE_CNTL_RD_PTR_MASK        0xf0

// Bitfield Description : 1: halt trace, 0: unhalt trace
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_OFFSET      8
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_MASK        0x100

// Bitfield Description : halt tel trace when CMD NACK received
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_ON_CMD_NACK_ERROR_EN_OFFSET      9
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_ON_CMD_NACK_ERROR_EN_MASK        0x200

// Bitfield Description : halt tel trace when TEL PKT ERR received
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_ON_TEL_PKT_ERROR_EN_OFFSET      10
#define SVI3_TEL_TRACE_CNTL_TRACE_HALT_ON_TEL_PKT_ERROR_EN_MASK        0x400

// Bitfield Description : 
#define SVI3_TEL_TRACE_CNTL_Reserved_15_11_OFFSET      11
#define SVI3_TEL_TRACE_CNTL_Reserved_15_11_MASK        0xf800

// Bitfield Description : enable the trace trigger function, trace buffer will only be filled in when trigger condition meet
#define SVI3_TEL_TRACE_CNTL_TRACE_TRIGGER_EN_OFFSET      16
#define SVI3_TEL_TRACE_CNTL_TRACE_TRIGGER_EN_MASK        0x10000

// Bitfield Description : 0: record the first TEL_PKT==TRACE_TRIGGER_PKT and the following 15 commands
// 1: record the first TEL_PKT==TRACE_TRIGGER_PKT and 7 commands before it, 8 commands after it
// 2: record the first TEL_PKT==TRACE_TRIGGER_PKT and 15 commands before it
#define SVI3_TEL_TRACE_CNTL_TRACE_TRIGGER_TYPE_OFFSET      17
#define SVI3_TEL_TRACE_CNTL_TRACE_TRIGGER_TYPE_MASK        0xe0000

// Bitfield Description : tel trace trigger packet hit
#define SVI3_TEL_TRACE_CNTL_TRIGGER_HIT_OFFSET      20
#define SVI3_TEL_TRACE_CNTL_TRIGGER_HIT_MASK        0x100000

// Bitfield Description : 
#define SVI3_TEL_TRACE_CNTL_TRIGGER_DONE_OFFSET      21
#define SVI3_TEL_TRACE_CNTL_TRIGGER_DONE_MASK        0x200000

// Bitfield Description : 
#define SVI3_TEL_TRACE_CNTL_Reserved_31_22_OFFSET      22
#define SVI3_TEL_TRACE_CNTL_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            WR_PTR:4;
    UINT32                            RD_PTR:4;
    UINT32                            TRACE_HALT:1;
    UINT32                            TRACE_HALT_ON_CMD_NACK_ERROR_EN:1;
    UINT32                            TRACE_HALT_ON_TEL_PKT_ERROR_EN:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            TRACE_TRIGGER_EN:1;
    UINT32                            TRACE_TRIGGER_TYPE:3;
    UINT32                            TRIGGER_HIT:1;
    UINT32                            TRIGGER_DONE:1;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} SVI3_TEL_TRACE_CNTL_STRUCT;

#define SMN_SVI30_SVI3_TEL_TRACE_CNTL_ADDRESS    0x6f0b0UL


/***********************************************************
* Register Name : SVI3_TEL_TRACE_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : read data for  TEL trace buffer.
#define SVI3_TEL_TRACE_DATA_RD_DATA_OFFSET      0
#define SVI3_TEL_TRACE_DATA_RD_DATA_MASK        0xffffff

// Bitfield Description : read data length type for TEL trace buffer
#define SVI3_TEL_TRACE_DATA_RD_DATA_LEN_OFFSET      24
#define SVI3_TEL_TRACE_DATA_RD_DATA_LEN_MASK        0x3000000

// Bitfield Description : 
#define SVI3_TEL_TRACE_DATA_Reserved_31_26_OFFSET      26
#define SVI3_TEL_TRACE_DATA_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            RD_DATA:24;
    UINT32                            RD_DATA_LEN:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SVI3_TEL_TRACE_DATA_STRUCT;

#define SMN_SVI30_SVI3_TEL_TRACE_DATA_ADDRESS    0x6f0b8UL


/***********************************************************
* Register Name : SVI3_TEL_TRACE_TRIGGER
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : telementry trace buffer trigger packet conext
#define SVI3_TEL_TRACE_TRIGGER_TRACE_TRIGGER_PKT_OFFSET      0
#define SVI3_TEL_TRACE_TRIGGER_TRACE_TRIGGER_PKT_MASK        0xffffff

// Bitfield Description : telementry trace buffer trigger packet length type
#define SVI3_TEL_TRACE_TRIGGER_TRACE_TRIGGER_PKT_LEN_OFFSET      24
#define SVI3_TEL_TRACE_TRIGGER_TRACE_TRIGGER_PKT_LEN_MASK        0x3000000

// Bitfield Description : 
#define SVI3_TEL_TRACE_TRIGGER_Reserved_31_26_OFFSET      26
#define SVI3_TEL_TRACE_TRIGGER_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            TRACE_TRIGGER_PKT:24;
    UINT32                            TRACE_TRIGGER_PKT_LEN:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SVI3_TEL_TRACE_TRIGGER_STRUCT;

#define SMN_SVI30_SVI3_TEL_TRACE_TRIGGER_ADDRESS    0x6f0b4UL


/***********************************************************
* Register Name : SVI3_TIMING
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : specify SVC high duration: tHIGH = HIGH_CNT+1
#define SVI3_TIMING_HIGH_CNT_OFFSET      0
#define SVI3_TIMING_HIGH_CNT_MASK        0xff

// Bitfield Description : specify SVC low duration: tLOW = LOW_CNT+1
#define SVI3_TIMING_LOW_CNT_OFFSET      8
#define SVI3_TIMING_LOW_CNT_MASK        0xff00

// Bitfield Description : specify SVT capture time. 8'h0: using SVC falling edge to capture SVT; 8'h1: using one svi3_clk ahead of SVC falling edge to capture SVT
#define SVI3_TIMING_SETUP_CNT_OFFSET      16
#define SVI3_TIMING_SETUP_CNT_MASK        0xff0000

// Bitfield Description : unused
#define SVI3_TIMING_HOLD_CNT_OFFSET      24
#define SVI3_TIMING_HOLD_CNT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            HIGH_CNT:8;
    UINT32                            LOW_CNT:8;
    UINT32                            SETUP_CNT:8;
    UINT32                            HOLD_CNT:8;
  } Field;
  UINT32 Value;
} SVI3_TIMING_STRUCT;

#define SMN_SVI30_SVI3_TIMING_ADDRESS    0x6f050UL


/***********************************************************
* Register Name : SVI3_VERSION
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : version ID
#define SVI3_VERSION_VERSION_ID_OFFSET      0
#define SVI3_VERSION_VERSION_ID_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            VERSION_ID:32;
  } Field;
  UINT32 Value;
} SVI3_VERSION_STRUCT;

#define SMN_SVI30_SVI3_VERSION_ADDRESS    0x6f0f0UL


/***********************************************************
* Register Name : SVI3_VID_CHG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : ADDRESS field for VID change packet. Currently it ranges from 1~8. (0 is reserved)
#define SVI3_VID_CHG_TARGET_ADDR_OFFSET      0
#define SVI3_VID_CHG_TARGET_ADDR_MASK        0x3f

// Bitfield Description : VID field for VID change packet
#define SVI3_VID_CHG_TARGET_VID_OFFSET      6
#define SVI3_VID_CHG_TARGET_VID_MASK        0x7fc0

// Bitfield Description : write 1 to trigger a VID change packet sending
#define SVI3_VID_CHG_VID_CHG_TRIGGER_OFFSET      15
#define SVI3_VID_CHG_VID_CHG_TRIGGER_MASK        0x8000

// Bitfield Description : S0IX rail indication. If it is valid when VID_CHG_TRIGGER is set, then SIDEBAND#6 will enter into S0IX process
// by change VID to a low target value.
#define SVI3_VID_CHG_S0IX_RAIL_OFFSET      16
#define SVI3_VID_CHG_S0IX_RAIL_MASK        0x10000

// Bitfield Description : VID change request excution delay cycles as the unit of svi3_refclk.
#define SVI3_VID_CHG_VID_CHG_DELAY_OFFSET      17
#define SVI3_VID_CHG_VID_CHG_DELAY_MASK        0x1ffe0000

// Bitfield Description : 
#define SVI3_VID_CHG_Reserved_31_29_OFFSET      29
#define SVI3_VID_CHG_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            TARGET_ADDR:6;
    UINT32                            TARGET_VID:9;
    UINT32                            VID_CHG_TRIGGER:1;
    UINT32                            S0IX_RAIL:1;
    UINT32                            VID_CHG_DELAY:12;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} SVI3_VID_CHG_STRUCT;

#define SMN_SVI30_SVI3_VID_CHG_ADDRESS    0x6f010UL


/***********************************************************
* Register Name : THREAD_CONFIGURATION
* Register Description :
* Thread Configuration Register, Indirect PCI address/data registers accessible to BIOS
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field specifies the number of logical cores for an enabled complex on the die. The number of logical cores per complex = CoreCount + 1.
// Read by BIOS
// Written by SMU.
#define THREAD_CONFIGURATION_CoreCount_OFFSET      0
#define THREAD_CONFIGURATION_CoreCount_MASK        0xf

// Bitfield Description : This field specifies the number of enabled complexes on the die. The number of enabled complexes = ComplexCount + 1.
// Read by BIOS
// Written by SMU.
#define THREAD_CONFIGURATION_ComplexCount_OFFSET      4
#define THREAD_CONFIGURATION_ComplexCount_MASK        0xf0

// Bitfield Description : 1=Single thread
// 0=Multi thread.
// Read by BIOS
// Written by SMU.
#define THREAD_CONFIGURATION_SMTMode_OFFSET      8
#define THREAD_CONFIGURATION_SMTMode_MASK        0x100

// Bitfield Description : 1=Single thread
// 0=Multi thread
// Software can only set SMTDisable, it should never clear it. Writes to this field take effect on the next warm reset.
// Software must program this field to the same value on all dies.
// This field is for internal-use only.
// Read by SMU
// Written by BIOS.
#define THREAD_CONFIGURATION_SMTDisable_OFFSET      9
#define THREAD_CONFIGURATION_SMTDisable_MASK        0x200

// Bitfield Description : 
#define THREAD_CONFIGURATION_Reserved_31_10_OFFSET      10
#define THREAD_CONFIGURATION_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            CoreCount:4;
    UINT32                            ComplexCount:4;
    UINT32                            SMTMode:1;
    UINT32                            SMTDisable:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} THREAD_CONFIGURATION_STRUCT;

#define SMN_THREAD_CONFIGURATION_ADDRESS    0x5a81cUL


/***********************************************************
* Register Name : THREAD_ENABLE
* Register Description :
* Thread Enable Register, Indirect PCI address/data registers accessible to BIOS
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field is used to enable each of the logical threads on the die after a reset.
// 1=Enable the thread to start fetching and executing code from the boot vector.
// [1]: Thread 1 enable;
// ...;
// [N]: Thread N enable. Software can only set ThreadEn, it should never clear it. Bit 0 is reserved on the BSP die.
// Read by SMU
// Written by BIOS.
#define THREAD_ENABLE_ThreadEn_OFFSET      0
#define THREAD_ENABLE_ThreadEn_MASK        0xffff

// Bitfield Description : 
#define THREAD_ENABLE_Reserved_31_16_OFFSET      16
#define THREAD_ENABLE_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ThreadEn:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} THREAD_ENABLE_STRUCT;

#define SMN_THREAD_ENABLE_ADDRESS    0x5a818UL

#endif /* _SMU_SMUIO_H_ */
