/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _SST_H_
#define _SST_H_


/***********************************************************
* Register Name : CFG_SST_DataPoolCredit_Alloc_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for Data channel VC 7~4
#define CFG_SST_DataPoolCredit_Alloc_HI_CFG_SST_DataPoolCredit_Alloc_HI_OFFSET      0
#define CFG_SST_DataPoolCredit_Alloc_HI_CFG_SST_DataPoolCredit_Alloc_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_DataPoolCredit_Alloc_HI:32;
  } Field;
  UINT32 Value;
} CFG_SST_DataPoolCredit_Alloc_HI_STRUCT;

#define SMN_SST0_CFG_SST_DataPoolCredit_Alloc_HI_ADDRESS    0x17400414UL

#define SMN_SST1_CFG_SST_DataPoolCredit_Alloc_HI_ADDRESS    0x17500414UL


/***********************************************************
* Register Name : CFG_SST_DataPoolCredit_Alloc_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for Data channel VC 3~0
#define CFG_SST_DataPoolCredit_Alloc_LO_CFG_SST_DataPoolCredit_Alloc_LO_OFFSET      0
#define CFG_SST_DataPoolCredit_Alloc_LO_CFG_SST_DataPoolCredit_Alloc_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_DataPoolCredit_Alloc_LO:32;
  } Field;
  UINT32 Value;
} CFG_SST_DataPoolCredit_Alloc_LO_STRUCT;

#define SMN_SST0_CFG_SST_DataPoolCredit_Alloc_LO_ADDRESS    0x17400410UL

#define SMN_SST1_CFG_SST_DataPoolCredit_Alloc_LO_ADDRESS    0x17500410UL


/***********************************************************
* Register Name : CFG_SST_RdRspPoolCredit_Alloc_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for RdRsp channel VC 7~4
#define CFG_SST_RdRspPoolCredit_Alloc_HI_CFG_SST_RdRspPoolCredit_Alloc_HI_OFFSET      0
#define CFG_SST_RdRspPoolCredit_Alloc_HI_CFG_SST_RdRspPoolCredit_Alloc_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_RdRspPoolCredit_Alloc_HI:32;
  } Field;
  UINT32 Value;
} CFG_SST_RdRspPoolCredit_Alloc_HI_STRUCT;

#define SMN_SST0_CFG_SST_RdRspPoolCredit_Alloc_HI_ADDRESS    0x1740041cUL

#define SMN_SST1_CFG_SST_RdRspPoolCredit_Alloc_HI_ADDRESS    0x1750041cUL


/***********************************************************
* Register Name : CFG_SST_RdRspPoolCredit_Alloc_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for RdRsp channel VC 3~0
#define CFG_SST_RdRspPoolCredit_Alloc_LO_CFG_SST_RdRspPoolCredit_Alloc_LO_OFFSET      0
#define CFG_SST_RdRspPoolCredit_Alloc_LO_CFG_SST_RdRspPoolCredit_Alloc_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_RdRspPoolCredit_Alloc_LO:32;
  } Field;
  UINT32 Value;
} CFG_SST_RdRspPoolCredit_Alloc_LO_STRUCT;

#define SMN_SST0_CFG_SST_RdRspPoolCredit_Alloc_LO_ADDRESS    0x17400418UL

#define SMN_SST1_CFG_SST_RdRspPoolCredit_Alloc_LO_ADDRESS    0x17500418UL


/***********************************************************
* Register Name : CFG_SST_ReqPoolCredit_Alloc_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for Req channel VC 7~4
#define CFG_SST_ReqPoolCredit_Alloc_HI_CFG_SST_ReqPoolCredit_Alloc_HI_OFFSET      0
#define CFG_SST_ReqPoolCredit_Alloc_HI_CFG_SST_ReqPoolCredit_Alloc_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_ReqPoolCredit_Alloc_HI:32;
  } Field;
  UINT32 Value;
} CFG_SST_ReqPoolCredit_Alloc_HI_STRUCT;

#define SMN_SST0_CFG_SST_ReqPoolCredit_Alloc_HI_ADDRESS    0x1740040cUL

#define SMN_SST1_CFG_SST_ReqPoolCredit_Alloc_HI_ADDRESS    0x1750040cUL


/***********************************************************
* Register Name : CFG_SST_ReqPoolCredit_Alloc_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for Req channel VC 3~0
#define CFG_SST_ReqPoolCredit_Alloc_LO_CFG_SST_ReqPoolCredit_Alloc_LO_OFFSET      0
#define CFG_SST_ReqPoolCredit_Alloc_LO_CFG_SST_ReqPoolCredit_Alloc_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_ReqPoolCredit_Alloc_LO:32;
  } Field;
  UINT32 Value;
} CFG_SST_ReqPoolCredit_Alloc_LO_STRUCT;

#define SMN_SST0_CFG_SST_ReqPoolCredit_Alloc_LO_ADDRESS    0x17400408UL

#define SMN_SST1_CFG_SST_ReqPoolCredit_Alloc_LO_ADDRESS    0x17500408UL


/***********************************************************
* Register Name : CFG_SST_WrRspPoolCredit_Alloc_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for WrRsp channel VC 7~4
#define CFG_SST_WrRspPoolCredit_Alloc_HI_CFG_SST_WrRspPoolCredit_Alloc_HI_OFFSET      0
#define CFG_SST_WrRspPoolCredit_Alloc_HI_CFG_SST_WrRspPoolCredit_Alloc_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_WrRspPoolCredit_Alloc_HI:32;
  } Field;
  UINT32 Value;
} CFG_SST_WrRspPoolCredit_Alloc_HI_STRUCT;

#define SMN_SST0_CFG_SST_WrRspPoolCredit_Alloc_HI_ADDRESS    0x17400424UL

#define SMN_SST1_CFG_SST_WrRspPoolCredit_Alloc_HI_ADDRESS    0x17500424UL


/***********************************************************
* Register Name : CFG_SST_WrRspPoolCredit_Alloc_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pool credit reserved for WrRsp channel VC 3~0
#define CFG_SST_WrRspPoolCredit_Alloc_LO_CFG_SST_WrRspPoolCredit_Alloc_LO_OFFSET      0
#define CFG_SST_WrRspPoolCredit_Alloc_LO_CFG_SST_WrRspPoolCredit_Alloc_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CFG_SST_WrRspPoolCredit_Alloc_LO:32;
  } Field;
  UINT32 Value;
} CFG_SST_WrRspPoolCredit_Alloc_LO_STRUCT;

#define SMN_SST0_CFG_SST_WrRspPoolCredit_Alloc_LO_ADDRESS    0x17400420UL

#define SMN_SST1_CFG_SST_WrRspPoolCredit_Alloc_LO_ADDRESS    0x17500420UL


/***********************************************************
* Register Name : SION_CFG_S0_RDRSP_BURSTTARGET_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Read Response Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_RDRSP_BURSTTARGET_HI_cfg_s0_src_RdRsp_BurstTarget_hi_OFFSET      0
#define SION_CFG_S0_RDRSP_BURSTTARGET_HI_cfg_s0_src_RdRsp_BurstTarget_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_RdRsp_BurstTarget_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_RDRSP_BURSTTARGET_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_RDRSP_BURSTTARGET_HI_ADDRESS    0x1740010cUL

#define SMN_SST1_SION_CFG_S0_RDRSP_BURSTTARGET_HI_ADDRESS    0x1750010cUL


/***********************************************************
* Register Name : SION_CFG_S0_RDRSP_BURSTTARGET_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Read Response Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_RDRSP_BURSTTARGET_LO_cfg_s0_src_RdRsp_BurstTarget_lo_OFFSET      0
#define SION_CFG_S0_RDRSP_BURSTTARGET_LO_cfg_s0_src_RdRsp_BurstTarget_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_RdRsp_BurstTarget_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_RDRSP_BURSTTARGET_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_RDRSP_BURSTTARGET_LO_ADDRESS    0x17400108UL

#define SMN_SST1_SION_CFG_S0_RDRSP_BURSTTARGET_LO_ADDRESS    0x17500108UL


/***********************************************************
* Register Name : SION_CFG_S0_RDRSP_TIMESLOT_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Read Response Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SION MAS for more detail.
#define SION_CFG_S0_RDRSP_TIMESLOT_HI_cfg_s0_src_RdRsp_TimeSlot_hi_OFFSET      0
#define SION_CFG_S0_RDRSP_TIMESLOT_HI_cfg_s0_src_RdRsp_TimeSlot_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_RdRsp_TimeSlot_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_RDRSP_TIMESLOT_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_RDRSP_TIMESLOT_HI_ADDRESS    0x1740020cUL

#define SMN_SST1_SION_CFG_S0_RDRSP_TIMESLOT_HI_ADDRESS    0x1750020cUL


/***********************************************************
* Register Name : SION_CFG_S0_RDRSP_TIMESLOT_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Read Response Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SION MAS for more detail.
#define SION_CFG_S0_RDRSP_TIMESLOT_LO_cfg_s0_src_RdRsp_TimeSlot_lo_OFFSET      0
#define SION_CFG_S0_RDRSP_TIMESLOT_LO_cfg_s0_src_RdRsp_TimeSlot_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_RdRsp_TimeSlot_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_RDRSP_TIMESLOT_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_RDRSP_TIMESLOT_LO_ADDRESS    0x17400208UL

#define SMN_SST1_SION_CFG_S0_RDRSP_TIMESLOT_LO_ADDRESS    0x17500208UL


/***********************************************************
* Register Name : SION_CFG_S0_REQ_BURSTTARGET_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Request Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_REQ_BURSTTARGET_HI_cfg_s0_src_Req_BurstTarget_hi_OFFSET      0
#define SION_CFG_S0_REQ_BURSTTARGET_HI_cfg_s0_src_Req_BurstTarget_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_Req_BurstTarget_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_REQ_BURSTTARGET_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_REQ_BURSTTARGET_HI_ADDRESS    0x17400104UL

#define SMN_SST1_SION_CFG_S0_REQ_BURSTTARGET_HI_ADDRESS    0x17500104UL


/***********************************************************
* Register Name : SION_CFG_S0_REQ_BURSTTARGET_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Request Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_REQ_BURSTTARGET_LO_cfg_s0_src_Req_BurstTarget_lo_OFFSET      0
#define SION_CFG_S0_REQ_BURSTTARGET_LO_cfg_s0_src_Req_BurstTarget_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_Req_BurstTarget_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_REQ_BURSTTARGET_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_REQ_BURSTTARGET_LO_ADDRESS    0x17400100UL

#define SMN_SST1_SION_CFG_S0_REQ_BURSTTARGET_LO_ADDRESS    0x17500100UL


/***********************************************************
* Register Name : SION_CFG_S0_REQ_TIMESLOT_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Request Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SION MAS for more detail.
#define SION_CFG_S0_REQ_TIMESLOT_HI_cfg_s0_src_Req_TimeSlot_hi_OFFSET      0
#define SION_CFG_S0_REQ_TIMESLOT_HI_cfg_s0_src_Req_TimeSlot_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_Req_TimeSlot_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_REQ_TIMESLOT_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_REQ_TIMESLOT_HI_ADDRESS    0x17400204UL

#define SMN_SST1_SION_CFG_S0_REQ_TIMESLOT_HI_ADDRESS    0x17500204UL


/***********************************************************
* Register Name : SION_CFG_S0_REQ_TIMESLOT_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Request Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SION MAS for more detail.
#define SION_CFG_S0_REQ_TIMESLOT_LO_cfg_s0_src_Req_TimeSlot_lo_OFFSET      0
#define SION_CFG_S0_REQ_TIMESLOT_LO_cfg_s0_src_Req_TimeSlot_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_Req_TimeSlot_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_REQ_TIMESLOT_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_REQ_TIMESLOT_LO_ADDRESS    0x17400200UL

#define SMN_SST1_SION_CFG_S0_REQ_TIMESLOT_LO_ADDRESS    0x17500200UL


/***********************************************************
* Register Name : SION_CFG_S0_WRRSP_BURSTTARGET_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Write Response Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_WRRSP_BURSTTARGET_HI_cfg_s0_src_WrRsp_BurstTarget_hi_OFFSET      0
#define SION_CFG_S0_WRRSP_BURSTTARGET_HI_cfg_s0_src_WrRsp_BurstTarget_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_WrRsp_BurstTarget_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_WRRSP_BURSTTARGET_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_WRRSP_BURSTTARGET_HI_ADDRESS    0x17400114UL

#define SMN_SST1_SION_CFG_S0_WRRSP_BURSTTARGET_HI_ADDRESS    0x17500114UL


/***********************************************************
* Register Name : SION_CFG_S0_WRRSP_BURSTTARGET_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Write Response Burst target for each arbiter in STAGE0. Refer to SION MAS for more detail.
#define SION_CFG_S0_WRRSP_BURSTTARGET_LO_cfg_s0_src_WrRsp_BurstTarget_lo_OFFSET      0
#define SION_CFG_S0_WRRSP_BURSTTARGET_LO_cfg_s0_src_WrRsp_BurstTarget_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_WrRsp_BurstTarget_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_WRRSP_BURSTTARGET_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_WRRSP_BURSTTARGET_LO_ADDRESS    0x17400110UL

#define SMN_SST1_SION_CFG_S0_WRRSP_BURSTTARGET_LO_ADDRESS    0x17500110UL


/***********************************************************
* Register Name : SION_CFG_S0_WRRSP_TIMESLOT_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. High part of the Write Response Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SIONMAS for more detail.
#define SION_CFG_S0_WRRSP_TIMESLOT_HI_cfg_s0_src_WrRsp_TimeSlot_hi_OFFSET      0
#define SION_CFG_S0_WRRSP_TIMESLOT_HI_cfg_s0_src_WrRsp_TimeSlot_hi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_WrRsp_TimeSlot_hi:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_WRRSP_TIMESLOT_HI_STRUCT;

#define SMN_SST0_SION_CFG_S0_WRRSP_TIMESLOT_HI_ADDRESS    0x17400214UL

#define SMN_SST1_SION_CFG_S0_WRRSP_TIMESLOT_HI_ADDRESS    0x17500214UL


/***********************************************************
* Register Name : SION_CFG_S0_WRRSP_TIMESLOT_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SION Configuration register. Low part of the Write Response Time slot value for each arbiter in STAGE0. A Timeslot value of 0 is round-robin. Refer to SION MAS for more detail.
#define SION_CFG_S0_WRRSP_TIMESLOT_LO_cfg_s0_src_WrRsp_TimeSlot_lo_OFFSET      0
#define SION_CFG_S0_WRRSP_TIMESLOT_LO_cfg_s0_src_WrRsp_TimeSlot_lo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            cfg_s0_src_WrRsp_TimeSlot_lo:32;
  } Field;
  UINT32 Value;
} SION_CFG_S0_WRRSP_TIMESLOT_LO_STRUCT;

#define SMN_SST0_SION_CFG_S0_WRRSP_TIMESLOT_LO_ADDRESS    0x17400210UL

#define SMN_SST1_SION_CFG_S0_WRRSP_TIMESLOT_LO_ADDRESS    0x17500210UL


/***********************************************************
* Register Name : SION_WRAPPER_CFG_CG_OFF_HYSTERESIS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Clock in SION gate off hysteresis. Refer to SION MAS for more detail.
#define SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_CFG_CG_OFF_HYSTERESIS_OFFSET      0
#define SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_CFG_CG_OFF_HYSTERESIS_MASK        0xff

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_Reserved_31_8_OFFSET      8
#define SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            CFG_CG_OFF_HYSTERESIS:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_STRUCT;

#define SMN_SST0_SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_ADDRESS    0x17400400UL

#define SMN_SST1_SION_WRAPPER_CFG_CG_OFF_HYSTERESIS_ADDRESS    0x17500400UL


/***********************************************************
* Register Name : SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable dynamic clock gating for the SION ORIG REQ clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_OFFSET      0
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_MASK        0x1

// Bitfield Description : Disable dynamic clock gating for the SION COMP RSP clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_OFFSET      1
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK        0x2

// Bitfield Description : Disable dynamic clock gating for the SION ARB REQ clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET      2
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK        0x4

// Bitfield Description : Disable dynamic clock gating for the SION ARB RSP clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_OFFSET      3
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK        0x8

// Bitfield Description : Disable dynamic clock gating for the SION credit clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_OFFSET      4
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_MASK        0x10

// Bitfield Description : Disable dynamic clock gating for the SION cfgSlv clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_OFFSET      5
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_MASK        0x20

// Bitfield Description : Disable dynamic clock gating for the SION performance counter clock branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_OFFSET      6
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_MASK        0x40

// Bitfield Description : Reserved for disabling dynamic clock gating for future clock branches.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_OFFSET      7
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK        0x80

// Bitfield Description : Reserved for disabling dynamic clock gating for future clock branches.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET      8
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_MASK        0x100

// Bitfield Description : Reserved for disabling dynamic clock gating for future clock branches.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_OFFSET      9
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_MASK        0x200

// Bitfield Description : Reserved for disabling dynamic clock gating for future clock branches.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_15_10_OFFSET      10
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_15_10_MASK        0xfc00

// Bitfield Description : 
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_25_16_OFFSET      16
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_25_16_MASK        0x3ff0000

// Bitfield Description : Reserved for disabling dynamic clock gating for future clock branches.
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_31_26_OFFSET      26
#define SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8:1;
    UINT32                            CFG_SSTSION_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9:1;
    UINT32                            Reserved_15_10:6;
    UINT32                            Reserved_25_16:10;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_STRUCT;

#define SMN_SST0_SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_ADDRESS    0x17400404UL

#define SMN_SST1_SION_WRAPPER_CFG_SSTSION_GLUE_CG_LCLK_CTRL_SOFT_OVERRIDE_CLK_ADDRESS    0x17500404UL


/***********************************************************
* Register Name : SSTNBIO_PGSLV_CONTROL
* Register Description :
* power gate control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to enter power gating. The counter decrements every 200ns
#define SSTNBIO_PGSLV_CONTROL_PGSLV_Hysteresis_OFFSET      0
#define SSTNBIO_PGSLV_CONTROL_PGSLV_Hysteresis_MASK        0x1f

// Bitfield Description : 
#define SSTNBIO_PGSLV_CONTROL_Reserved_31_5_OFFSET      5
#define SSTNBIO_PGSLV_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            PGSLV_Hysteresis:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} SSTNBIO_PGSLV_CONTROL_STRUCT;

#define SMN_SST0_SSTNBIO_PGSLV_CONTROL_ADDRESS    0x1740000cUL

#define SMN_SST1_SSTNBIO_PGSLV_CONTROL_ADDRESS    0x1750000cUL


/***********************************************************
* Register Name : SST_CLOCK_CTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Gating the clock of TX sub-blocks.
#define SST_CLOCK_CTRL_TXCLKGATEEn_OFFSET      0
#define SST_CLOCK_CTRL_TXCLKGATEEn_MASK        0x1

// Bitfield Description : Enable Gating the clock of TX ssclk sub-blocks.
#define SST_CLOCK_CTRL_TXCLKGATEEn2_OFFSET      1
#define SST_CLOCK_CTRL_TXCLKGATEEn2_MASK        0x2

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SST_CLOCK_CTRL_Reserved_5_2_OFFSET      2
#define SST_CLOCK_CTRL_Reserved_5_2_MASK        0x3c

// Bitfield Description : Delay to turn off TX_SSCLK till opposite side SST can receive port control message delivered on TX_SSB_CREDIT_PKT @TX_SSCLK
#define SST_CLOCK_CTRL_PCTRL_IDLE_TIME_OFFSET      6
#define SST_CLOCK_CTRL_PCTRL_IDLE_TIME_MASK        0xffc0
#define SST_CLOCK_CTRL_PCTRL_IDLE_TIME_DEFAULT     0xf0

// Bitfield Description : Enable Gating the clock of RX sub-blocks.
#define SST_CLOCK_CTRL_RXCLKGATEEn_OFFSET      16
#define SST_CLOCK_CTRL_RXCLKGATEEn_MASK        0x10000

// Bitfield Description : Enable Gating the clock of RX ssclk sub-blocks.
#define SST_CLOCK_CTRL_RXCLKGATEEn2_OFFSET      17
#define SST_CLOCK_CTRL_RXCLKGATEEn2_MASK        0x20000

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SST_CLOCK_CTRL_Reserved_31_18_OFFSET      18
#define SST_CLOCK_CTRL_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            TXCLKGATEEn:1;
    UINT32                            TXCLKGATEEn2:1;
    UINT32                            Reserved_5_2:4;
    UINT32                            PCTRL_IDLE_TIME:10;
    UINT32                            RXCLKGATEEn:1;
    UINT32                            RXCLKGATEEn2:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} SST_CLOCK_CTRL_STRUCT;

#define SMN_SST0_SST_CLOCK_CTRL_ADDRESS    0x17400004UL

#define SMN_SST1_SST_CLOCK_CTRL_ADDRESS    0x17500004UL


/***********************************************************
* Register Name : SST_ENABLE_CTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SST_ENABLE_CTRL_SST_ENABLE_OFFSET      0
#define SST_ENABLE_CTRL_SST_ENABLE_MASK        0x1

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SST_ENABLE_CTRL_Reserved_7_1_OFFSET      1
#define SST_ENABLE_CTRL_Reserved_7_1_MASK        0xfe

// Bitfield Description : 1 means the SST goes out of reset
#define SST_ENABLE_CTRL_SST_RST_DONE_OFFSET      8
#define SST_ENABLE_CTRL_SST_RST_DONE_MASK        0x100

// Bitfield Description : Reserved for future use. This register controls no hardware
#define SST_ENABLE_CTRL_Reserved_31_9_OFFSET      9
#define SST_ENABLE_CTRL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            SST_ENABLE:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            SST_RST_DONE:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} SST_ENABLE_CTRL_STRUCT;

#define SMN_SST0_SST_ENABLE_CTRL_ADDRESS    0x17400008UL

#define SMN_SST1_SST_ENABLE_CTRL_ADDRESS    0x17500008UL


/***********************************************************
* Register Name : SST_SION_PERF_CNT_CNTL0
* Register Description :
* IOAGR SION Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Performance Counters.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_CNT_EN_OFFSET      0
#define SST_SION_PERF_CNT_CNTL0_SST_SION_CNT_EN_MASK        0x1

// Bitfield Description : Write 1 to load the counter shadow registers. Read back value is always 0.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_WR_OFFSET      1
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_WR_MASK        0x2

// Bitfield Description : Write 1 for Global RESET of ALL counters. Read back value is always 0.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_OFFSET      2
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_MASK        0x4

// Bitfield Description : 
#define SST_SION_PERF_CNT_CNTL0_Reserved_7_3_OFFSET      3
#define SST_SION_PERF_CNT_CNTL0_Reserved_7_3_MASK        0xf8

// Bitfield Description : Programmable Pulse width for Global Shadow Write Toggle.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_DELAY_OFFSET      8
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_DELAY_MASK        0xf00

// Bitfield Description : 
#define SST_SION_PERF_CNT_CNTL0_Reserved_14_12_OFFSET      12
#define SST_SION_PERF_CNT_CNTL0_Reserved_14_12_MASK        0x7000

// Bitfield Description : Enables the Programmable Pulse with for Global Shadow Write Toggle.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_DELAY_EN_OFFSET      15
#define SST_SION_PERF_CNT_CNTL0_SST_SION_SHADOW_DELAY_EN_MASK        0x8000

// Bitfield Description : Programmable Pulse width for Global Perf Reset Toggle.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_DELAY_OFFSET      16
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_DELAY_MASK        0xf0000

// Bitfield Description : 
#define SST_SION_PERF_CNT_CNTL0_Reserved_22_20_OFFSET      20
#define SST_SION_PERF_CNT_CNTL0_Reserved_22_20_MASK        0x700000

// Bitfield Description : Enables the Programmable Pulse width for Global Perf Reset Toggle.
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_DELAY_EN_OFFSET      23
#define SST_SION_PERF_CNT_CNTL0_SST_SION_PERF_RESET_DELAY_EN_MASK        0x800000

// Bitfield Description : 
#define SST_SION_PERF_CNT_CNTL0_Reserved_31_24_OFFSET      24
#define SST_SION_PERF_CNT_CNTL0_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_CNT_EN:1;
    UINT32                            SST_SION_SHADOW_WR:1;
    UINT32                            SST_SION_PERF_RESET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            SST_SION_SHADOW_DELAY:4;
    UINT32                            Reserved_14_12:3;
    UINT32                            SST_SION_SHADOW_DELAY_EN:1;
    UINT32                            SST_SION_PERF_RESET_DELAY:4;
    UINT32                            Reserved_22_20:3;
    UINT32                            SST_SION_PERF_RESET_DELAY_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_CNT_CNTL0_STRUCT;

#define SMN_SST0_SST_SION_PERF_CNT_CNTL0_ADDRESS    0x17401094UL

#define SMN_SST1_SST_SION_PERF_CNT_CNTL0_ADDRESS    0x17501094UL


/***********************************************************
* Register Name : SST_SION_PERF_CNT_CNTL1
* Register Description :
* IOAGR SION Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT0_SEL_OFFSET      0
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT1_SEL_OFFSET      8
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT2_SEL_OFFSET      16
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT3_SEL_OFFSET      24
#define SST_SION_PERF_CNT_CNTL1_SST_SION_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_EVENT0_SEL:8;
    UINT32                            SST_SION_EVENT1_SEL:8;
    UINT32                            SST_SION_EVENT2_SEL:8;
    UINT32                            SST_SION_EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_CNT_CNTL1_STRUCT;

#define SMN_SST0_SST_SION_PERF_CNT_CNTL1_ADDRESS    0x17401098UL

#define SMN_SST1_SST_SION_PERF_CNT_CNTL1_ADDRESS    0x17501098UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT0
* Register Description :
* IOAGR SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define SST_SION_PERF_COUNT0_SST_SION_COUNTER0_OFFSET      0
#define SST_SION_PERF_COUNT0_SST_SION_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER0:32;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT0_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT0_ADDRESS    0x1740109cUL

#define SMN_SST1_SST_SION_PERF_COUNT0_ADDRESS    0x1750109cUL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT0_UPPER
* Register Description :
* IOAGR SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define SST_SION_PERF_COUNT0_UPPER_SST_SION_COUNTER0_UPPER_OFFSET      0
#define SST_SION_PERF_COUNT0_UPPER_SST_SION_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define SST_SION_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define SST_SION_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT0_UPPER_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT0_UPPER_ADDRESS    0x174010a0UL

#define SMN_SST1_SST_SION_PERF_COUNT0_UPPER_ADDRESS    0x175010a0UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT1
* Register Description :
* IOAGR SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define SST_SION_PERF_COUNT1_SST_SION_COUNTER1_OFFSET      0
#define SST_SION_PERF_COUNT1_SST_SION_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER1:32;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT1_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT1_ADDRESS    0x174010a4UL

#define SMN_SST1_SST_SION_PERF_COUNT1_ADDRESS    0x175010a4UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT1_UPPER
* Register Description :
* IOAGR SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define SST_SION_PERF_COUNT1_UPPER_SST_SION_COUNTER1_UPPER_OFFSET      0
#define SST_SION_PERF_COUNT1_UPPER_SST_SION_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define SST_SION_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define SST_SION_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT1_UPPER_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT1_UPPER_ADDRESS    0x174010a8UL

#define SMN_SST1_SST_SION_PERF_COUNT1_UPPER_ADDRESS    0x175010a8UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT2
* Register Description :
* IOAGR SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define SST_SION_PERF_COUNT2_SST_SION_COUNTER2_OFFSET      0
#define SST_SION_PERF_COUNT2_SST_SION_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER2:32;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT2_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT2_ADDRESS    0x174010acUL

#define SMN_SST1_SST_SION_PERF_COUNT2_ADDRESS    0x175010acUL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT2_UPPER
* Register Description :
* IOAGR SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define SST_SION_PERF_COUNT2_UPPER_SST_SION_COUNTER2_UPPER_OFFSET      0
#define SST_SION_PERF_COUNT2_UPPER_SST_SION_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define SST_SION_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define SST_SION_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT2_UPPER_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT2_UPPER_ADDRESS    0x174010b0UL

#define SMN_SST1_SST_SION_PERF_COUNT2_UPPER_ADDRESS    0x175010b0UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT3
* Register Description :
* IOAGR SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define SST_SION_PERF_COUNT3_SST_SION_COUNTER3_OFFSET      0
#define SST_SION_PERF_COUNT3_SST_SION_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER3:32;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT3_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT3_ADDRESS    0x174010b4UL

#define SMN_SST1_SST_SION_PERF_COUNT3_ADDRESS    0x175010b4UL


/***********************************************************
* Register Name : SST_SION_PERF_COUNT3_UPPER
* Register Description :
* IOAGR SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define SST_SION_PERF_COUNT3_UPPER_SST_SION_COUNTER3_UPPER_OFFSET      0
#define SST_SION_PERF_COUNT3_UPPER_SST_SION_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define SST_SION_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define SST_SION_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SST_SION_COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SST_SION_PERF_COUNT3_UPPER_STRUCT;

#define SMN_SST0_SST_SION_PERF_COUNT3_UPPER_ADDRESS    0x174010b8UL

#define SMN_SST1_SST_SION_PERF_COUNT3_UPPER_ADDRESS    0x175010b8UL


/***********************************************************
* Register Name : SST_STATISTIC_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Statisic counter for outstanding number of read  request in host direction of sst_nbio or dma direction of sst_fch
#define SST_STATISTIC_0_RdRspCnt_OFFSET      0
#define SST_STATISTIC_0_RdRspCnt_MASK        0xffff

// Bitfield Description : Statisic counter for outstanding number of write request in host direction of sst_nbio or dma direction of sst_fch
#define SST_STATISTIC_0_WrRspCnt_OFFSET      16
#define SST_STATISTIC_0_WrRspCnt_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RdRspCnt:16;
    UINT32                            WrRspCnt:16;
  } Field;
  UINT32 Value;
} SST_STATISTIC_0_STRUCT;

#define SMN_SST0_SST_STATISTIC_0_ADDRESS    0x17400020UL

#define SMN_SST1_SST_STATISTIC_0_ADDRESS    0x17500020UL

#endif /* _SST_H_ */
