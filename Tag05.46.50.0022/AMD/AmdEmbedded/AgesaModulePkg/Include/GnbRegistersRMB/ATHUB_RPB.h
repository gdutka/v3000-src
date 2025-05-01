/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ATHUB_RPB_H_
#define _ATHUB_RPB_H_


/***********************************************************
* Register Name : RPB_ARB_CNTL
* Register Description :
* this register provides controls for the arb arbiter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of wr requests - 1 to process before switching in arb
#define RPB_ARB_CNTL_RD_SWITCH_NUM_OFFSET      0
#define RPB_ARB_CNTL_RD_SWITCH_NUM_MASK        0xff

// Bitfield Description : number of rd requests - 1 to process before switching in arb
#define RPB_ARB_CNTL_WR_SWITCH_NUM_OFFSET      8
#define RPB_ARB_CNTL_WR_SWITCH_NUM_MASK        0xff00

// Bitfield Description : number of ats requests - 1 to process before switching in atc
#define RPB_ARB_CNTL_ATC_TR_SWITCH_NUM_OFFSET      16
#define RPB_ARB_CNTL_ATC_TR_SWITCH_NUM_MASK        0xff0000

// Bitfield Description : arbitration mode, 1 credit mode, 0 weighted rr mode, OBSOLETE, use fixed swith mode
#define RPB_ARB_CNTL_ARB_MODE_OFFSET      24
#define RPB_ARB_CNTL_ARB_MODE_MASK        0x1000000

// Bitfield Description : switch_num_mode, 1 bandwidth mode, 0 trans num mode, OBSOLETE
#define RPB_ARB_CNTL_SWITCH_NUM_MODE_OFFSET      25
#define RPB_ARB_CNTL_SWITCH_NUM_MODE_MASK        0x2000000

// Bitfield Description : VC0 outbuff depth, for debug.
#define RPB_ARB_CNTL_RPB_VC0_CRD_OFFSET      26
#define RPB_ARB_CNTL_RPB_VC0_CRD_MASK        0x7c000000

// Bitfield Description : disable fed judge. not used for now
#define RPB_ARB_CNTL_DISABLE_FED_OFFSET      31
#define RPB_ARB_CNTL_DISABLE_FED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RD_SWITCH_NUM:8;
    UINT32                            WR_SWITCH_NUM:8;
    UINT32                            ATC_TR_SWITCH_NUM:8;
    UINT32                            ARB_MODE:1;
    UINT32                            SWITCH_NUM_MODE:1;
    UINT32                            RPB_VC0_CRD:5;
    UINT32                            DISABLE_FED:1;
  } Field;
  UINT32 Value;
} RPB_ARB_CNTL_STRUCT;

#define SMN_RPB_ARB_CNTL_ADDRESS    0x3364UL


/***********************************************************
* Register Name : RPB_ARB_CNTL2
* Register Description :
* this register provides controls for the arb arbiter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of p2p wr requests - 1 to process before switching in arb
#define RPB_ARB_CNTL2_P2P_SWITCH_NUM_OFFSET      0
#define RPB_ARB_CNTL2_P2P_SWITCH_NUM_MASK        0xff

// Bitfield Description : number of atomic requests - 1 to process before switching in arb
#define RPB_ARB_CNTL2_ATOMIC_SWITCH_NUM_OFFSET      8
#define RPB_ARB_CNTL2_ATOMIC_SWITCH_NUM_MASK        0xff00

// Bitfield Description : number of ats requests - 1 to process before switching in atc for vc1
#define RPB_ARB_CNTL2_ATC_PAGE_SWITCH_NUM_OFFSET      16
#define RPB_ARB_CNTL2_ATC_PAGE_SWITCH_NUM_MASK        0xff0000

// Bitfield Description : VC1 outbuff depth, for debug.
#define RPB_ARB_CNTL2_RPB_VC1_CRD_OFFSET      24
#define RPB_ARB_CNTL2_RPB_VC1_CRD_MASK        0x1f000000

// Bitfield Description : 
#define RPB_ARB_CNTL2_Reserved_31_29_OFFSET      29
#define RPB_ARB_CNTL2_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            P2P_SWITCH_NUM:8;
    UINT32                            ATOMIC_SWITCH_NUM:8;
    UINT32                            ATC_PAGE_SWITCH_NUM:8;
    UINT32                            RPB_VC1_CRD:5;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} RPB_ARB_CNTL2_STRUCT;

#define SMN_RPB_ARB_CNTL2_ADDRESS    0x3368UL


/***********************************************************
* Register Name : RPB_ATS_CNTL
* Register Description :
* this register tells the control and arbitration configurations for ats related request.
* Visibility : 0x2
************************************************************/

// Bitfield Description : if it is enabled, then it means that ats request need minimal latency. 0 means disabled, ats request shall be one normal weight rr client.
#define RPB_ATS_CNTL_PAGE_MIN_LATENCY_ENABLE_OFFSET      0
#define RPB_ATS_CNTL_PAGE_MIN_LATENCY_ENABLE_MASK        0x1

// Bitfield Description : if it is enabled, then it means that ats request need minimal latency. 0 means disabled, ats request shall be one normal weight rr client.
#define RPB_ATS_CNTL_TR_MIN_LATENCY_ENABLE_OFFSET      1
#define RPB_ATS_CNTL_TR_MIN_LATENCY_ENABLE_MASK        0x2

// Bitfield Description : it tells if we didnt process up to switch_threshold ats request, then we couldnt change the ats priority into normal weighted rr
#define RPB_ATS_CNTL_SWITCH_THRESHOLD_OFFSET      2
#define RPB_ATS_CNTL_SWITCH_THRESHOLD_MASK        0x7c

// Bitfield Description : it tells during this time_slice time, we consider the ats request as burst, these requests could be used to accumulate the counter to compare with the switch_threshold. after the time_slice*4 cycles, we consider that the ats request are totally new. which means that we shall treat it as high priroity again.
#define RPB_ATS_CNTL_TIME_SLICE_OFFSET      7
#define RPB_ATS_CNTL_TIME_SLICE_MASK        0x7f80

// Bitfield Description : number of ats requests - 1 to process before switching in rpb_atc for tr request
#define RPB_ATS_CNTL_ATCTR_GFX_SWITCH_NUM_OFFSET      15
#define RPB_ATS_CNTL_ATCTR_GFX_SWITCH_NUM_MASK        0x78000

// Bitfield Description : number of ats requests - 1 to process before switching in rpb_atc for page invack request
#define RPB_ATS_CNTL_ATCPAGE_SWITCH_NUM_OFFSET      19
#define RPB_ATS_CNTL_ATCPAGE_SWITCH_NUM_MASK        0x780000

// Bitfield Description : at field for the non-translation request comes from atc, 00 shall be the reserverd value for other msg body
#define RPB_ATS_CNTL_WR_AT_OFFSET      23
#define RPB_ATS_CNTL_WR_AT_MASK        0x1800000

// Bitfield Description : 0:go to vc0; 1:go to vc5
#define RPB_ATS_CNTL_MM_TRANS_VC5_ENABLE_OFFSET      25
#define RPB_ATS_CNTL_MM_TRANS_VC5_ENABLE_MASK        0x2000000

// Bitfield Description : 0:go to vc0; 1:go to vc5
#define RPB_ATS_CNTL_GC_TRANS_VC5_ENABLE_OFFSET      26
#define RPB_ATS_CNTL_GC_TRANS_VC5_ENABLE_MASK        0x4000000

// Bitfield Description : 
#define RPB_ATS_CNTL_Reserved_31_27_OFFSET      27
#define RPB_ATS_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            PAGE_MIN_LATENCY_ENABLE:1;
    UINT32                            TR_MIN_LATENCY_ENABLE:1;
    UINT32                            SWITCH_THRESHOLD:5;
    UINT32                            TIME_SLICE:8;
    UINT32                            ATCTR_GFX_SWITCH_NUM:4;
    UINT32                            ATCPAGE_SWITCH_NUM:4;
    UINT32                            WR_AT:2;
    UINT32                            MM_TRANS_VC5_ENABLE:1;
    UINT32                            GC_TRANS_VC5_ENABLE:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RPB_ATS_CNTL_STRUCT;

#define SMN_RPB_ATS_CNTL_ADDRESS    0x33d0UL


/***********************************************************
* Register Name : RPB_ATS_CNTL2
* Register Description :
* this register tells the control and arbitration configurations for ats related request.
* Visibility : 0x2
************************************************************/

// Bitfield Description : invalidation completion command field
#define RPB_ATS_CNTL2_INVAL_COM_CMD_OFFSET      0
#define RPB_ATS_CNTL2_INVAL_COM_CMD_MASK        0x3f

// Bitfield Description : translation request command field
#define RPB_ATS_CNTL2_TRANS_CMD_OFFSET      6
#define RPB_ATS_CNTL2_TRANS_CMD_MASK        0xfc0

// Bitfield Description : page request command field
#define RPB_ATS_CNTL2_PAGE_REQ_CMD_OFFSET      12
#define RPB_ATS_CNTL2_PAGE_REQ_CMD_MASK        0x3f000

// Bitfield Description : page request pcie message routing code
#define RPB_ATS_CNTL2_PAGE_ROUTING_CODE_OFFSET      18
#define RPB_ATS_CNTL2_PAGE_ROUTING_CODE_MASK        0x1c0000

// Bitfield Description : invalidation completion pcie message routing code
#define RPB_ATS_CNTL2_INVAL_COM_ROUTING_CODE_OFFSET      21
#define RPB_ATS_CNTL2_INVAL_COM_ROUTING_CODE_MASK        0xe00000

// Bitfield Description : 0 non-amd, non-dmtf vendor id 1 amd vendor id 2 dmtf mctp vendor id 3 reserved
#define RPB_ATS_CNTL2_VENDOR_ID_OFFSET      24
#define RPB_ATS_CNTL2_VENDOR_ID_MASK        0x3000000

// Bitfield Description : VC5 outbuff depth, for debug.
#define RPB_ATS_CNTL2_RPB_VC5_CRD_OFFSET      26
#define RPB_ATS_CNTL2_RPB_VC5_CRD_MASK        0x7c000000

// Bitfield Description : 
#define RPB_ATS_CNTL2_Reserved_31_31_OFFSET      31
#define RPB_ATS_CNTL2_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            INVAL_COM_CMD:6;
    UINT32                            TRANS_CMD:6;
    UINT32                            PAGE_REQ_CMD:6;
    UINT32                            PAGE_ROUTING_CODE:3;
    UINT32                            INVAL_COM_ROUTING_CODE:3;
    UINT32                            VENDOR_ID:2;
    UINT32                            RPB_VC5_CRD:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} RPB_ATS_CNTL2_STRUCT;

#define SMN_RPB_ATS_CNTL2_ADDRESS    0x33d4UL


/***********************************************************
* Register Name : RPB_ATS_CNTL3
* Register Description :
* more ATS config control
* Visibility : 0x2
************************************************************/

// Bitfield Description : how many outstanding mmhub translation requests can be issued to nbif.
#define RPB_ATS_CNTL3_RPB_ATS_MM_TR_OFFSET      0
#define RPB_ATS_CNTL3_RPB_ATS_MM_TR_MASK        0xff

// Bitfield Description : how many outstanding gfx translation requests can be issued to nbif.
#define RPB_ATS_CNTL3_RPB_ATS_GFX_TR_OFFSET      8
#define RPB_ATS_CNTL3_RPB_ATS_GFX_TR_MASK        0xff00

// Bitfield Description : how many outstanding page request and inv response can be issued to nbif.
#define RPB_ATS_CNTL3_RPB_ATS_PR_OFFSET      16
#define RPB_ATS_CNTL3_RPB_ATS_PR_MASK        0xff0000

// Bitfield Description : 
#define RPB_ATS_CNTL3_Reserved_31_24_OFFSET      24
#define RPB_ATS_CNTL3_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            RPB_ATS_MM_TR:8;
    UINT32                            RPB_ATS_GFX_TR:8;
    UINT32                            RPB_ATS_PR:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} RPB_ATS_CNTL3_STRUCT;

#define SMN_RPB_ATS_CNTL3_ADDRESS    0x33d8UL


/***********************************************************
* Register Name : RPB_BIF_CNTL
* Register Description :
* this register provides controls for the bif arbiter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of arb rd requests to process before switching in bif
#define RPB_BIF_CNTL_VC0_SWITCH_NUM_OFFSET      0
#define RPB_BIF_CNTL_VC0_SWITCH_NUM_MASK        0xff

// Bitfield Description : number of arb wr requests to process before switching in bif
#define RPB_BIF_CNTL_VC1_SWITCH_NUM_OFFSET      8
#define RPB_BIF_CNTL_VC1_SWITCH_NUM_MASK        0xff00

// Bitfield Description : number of arb mmhub tr requests to process before switching in bif
#define RPB_BIF_CNTL_VC2_SWITCH_NUM_OFFSET      16
#define RPB_BIF_CNTL_VC2_SWITCH_NUM_MASK        0xff0000

// Bitfield Description : it will decide if NbioDataInit_OrigClkCtl clears NbioDataInit_OrigReqCredit/NbioDataInit_OrigDataCredit Counter .
#define RPB_BIF_CNTL_NBIF_DMA_ORIGCLKCTL_EN_OFFSET      24
#define RPB_BIF_CNTL_NBIF_DMA_ORIGCLKCTL_EN_MASK        0x1000000

// Bitfield Description : VC# for mm trans requests, value 0/1 is not supported.
#define RPB_BIF_CNTL_TR_QOS_VC_OFFSET      25
#define RPB_BIF_CNTL_TR_QOS_VC_MASK        0xe000000

// Bitfield Description : reserved fields
#define RPB_BIF_CNTL_Reserved_31_28_OFFSET      28
#define RPB_BIF_CNTL_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_SWITCH_NUM:8;
    UINT32                            VC1_SWITCH_NUM:8;
    UINT32                            VC2_SWITCH_NUM:8;
    UINT32                            NBIF_DMA_ORIGCLKCTL_EN:1;
    UINT32                            TR_QOS_VC:3;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} RPB_BIF_CNTL_STRUCT;

#define SMN_RPB_BIF_CNTL_ADDRESS    0x336cUL


/***********************************************************
* Register Name : RPB_BIF_CNTL2
* Register Description :
* this register provides controls for the bif arbiter
* Visibility : 0x2
************************************************************/

// Bitfield Description : arbitration mode, 1 credit mode, 0 weighted rr mode
#define RPB_BIF_CNTL2_ARB_MODE_OFFSET      0
#define RPB_BIF_CNTL2_ARB_MODE_MASK        0x1

// Bitfield Description : tells which channel shall be drained up, 0 means vc0, 1 means vc1,2 means vc5
#define RPB_BIF_CNTL2_DRAIN_VC_NUM_OFFSET      1
#define RPB_BIF_CNTL2_DRAIN_VC_NUM_MASK        0x6

// Bitfield Description : tells if we keeping have credits for drain_vc_num,then we could switch to other chnnal if we keeping processed for a long time.If it is disable ,we could switch other channel only when no request in drain_vc_num or no credits for drain_vc_num ,if drain_vc_num always has requests and enough credits ,will never switch other channel .
#define RPB_BIF_CNTL2_SWITCH_ENABLE_OFFSET      3
#define RPB_BIF_CNTL2_SWITCH_ENABLE_MASK        0x8

// Bitfield Description : tells if we keeping processing vc0 rd request up to switch_threshold while we didnt process any other channel request, we could switch other channel and process for other channel for switch_num request.
#define RPB_BIF_CNTL2_SWITCH_THRESHOLD_OFFSET      4
#define RPB_BIF_CNTL2_SWITCH_THRESHOLD_MASK        0xff0

// Bitfield Description : tell if the page request could have highest priority
#define RPB_BIF_CNTL2_PAGE_PRI_EN_OFFSET      12
#define RPB_BIF_CNTL2_PAGE_PRI_EN_MASK        0x1000

// Bitfield Description : tell if the mm tr request could have highest priority,if both tr and page are en, then mm tr is higher than gfx tr and page.
#define RPB_BIF_CNTL2_MM_TR_PRI_EN_OFFSET      13
#define RPB_BIF_CNTL2_MM_TR_PRI_EN_MASK        0x2000

// Bitfield Description : tell if the gfx tr request could have highest priority,if both tr and page are en, then gfx tr is higher than page.
#define RPB_BIF_CNTL2_GFX_TR_PRI_EN_OFFSET      14
#define RPB_BIF_CNTL2_GFX_TR_PRI_EN_MASK        0x4000

// Bitfield Description : the vc0/vc5's chaied signal shall and with this bits,the default value is zero,which means chained signal ahall always be zero for vc0/vc5.
#define RPB_BIF_CNTL2_VC0_CHAINED_OVERRIDE_OFFSET      15
#define RPB_BIF_CNTL2_VC0_CHAINED_OVERRIDE_MASK        0x8000

// Bitfield Description : whether we do parity check in rpb, if enable it, we will set dataerror to one. if disabled, we just bypass dataerror. for rdrspdata, if we found that error, we will set rdrspdatastatus to DataERR.
#define RPB_BIF_CNTL2_PARITY_CHECK_EN_OFFSET      16
#define RPB_BIF_CNTL2_PARITY_CHECK_EN_MASK        0x10000

// Bitfield Description : it will decide if NbioDataTarg_CompClkCtl clears NbioDataTarg_WrRspCredit Counter .
#define RPB_BIF_CNTL2_NBIF_HST_COMPCLKCTL_EN_OFFSET      17
#define RPB_BIF_CNTL2_NBIF_HST_COMPCLKCTL_EN_MASK        0x20000

// Bitfield Description : it will decide if NbioDataTarg_CompClkCtl clears NbioDataTarg_WrRspCredit Counter .
#define RPB_BIF_CNTL2_ATHUB_NBIF_UNITID_OFFSET      18
#define RPB_BIF_CNTL2_ATHUB_NBIF_UNITID_MASK        0x1fc0000

// Bitfield Description : reserved fields
#define RPB_BIF_CNTL2_Reserved_31_25_OFFSET      25
#define RPB_BIF_CNTL2_Reserved_31_25_MASK        0xfe000000

typedef union {
  struct {
    UINT32                            ARB_MODE:1;
    UINT32                            DRAIN_VC_NUM:2;
    UINT32                            SWITCH_ENABLE:1;
    UINT32                            SWITCH_THRESHOLD:8;
    UINT32                            PAGE_PRI_EN:1;
    UINT32                            MM_TR_PRI_EN:1;
    UINT32                            GFX_TR_PRI_EN:1;
    UINT32                            VC0_CHAINED_OVERRIDE:1;
    UINT32                            PARITY_CHECK_EN:1;
    UINT32                            NBIF_HST_COMPCLKCTL_EN:1;
    UINT32                            ATHUB_NBIF_UNITID:7;
    UINT32                            Reserved_31_25:7;
  } Field;
  UINT32 Value;
} RPB_BIF_CNTL2_STRUCT;

#define SMN_RPB_BIF_CNTL2_ADDRESS    0x3370UL


/***********************************************************
* Register Name : RPB_BLOCKLEVEL_CONF
* Register Description :
* RPB blocklevel Configuration Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : blocklevel field for txns sourced from xpb
#define RPB_BLOCKLEVEL_CONF_XPB_BLOCKLEVEL_OVERRIDE_OFFSET      0
#define RPB_BLOCKLEVEL_CONF_XPB_BLOCKLEVEL_OVERRIDE_MASK        0x3

// Bitfield Description : xpb blocklevel override enable field.
#define RPB_BLOCKLEVEL_CONF_XPB_BLOCKLEVEL_OVERRIDE_EN_OFFSET      2
#define RPB_BLOCKLEVEL_CONF_XPB_BLOCKLEVEL_OVERRIDE_EN_MASK        0x4

// Bitfield Description : blocklevel field for tr txns sourced from vc5
#define RPB_BLOCKLEVEL_CONF_ATC_VC5_TR_BLOCKLEVEL_OFFSET      3
#define RPB_BLOCKLEVEL_CONF_ATC_VC5_TR_BLOCKLEVEL_MASK        0x18

// Bitfield Description : blocklevel field for tr txns sourced from vc0
#define RPB_BLOCKLEVEL_CONF_ATC_VC0_TR_BLOCKLEVEL_OFFSET      5
#define RPB_BLOCKLEVEL_CONF_ATC_VC0_TR_BLOCKLEVEL_MASK        0x60

// Bitfield Description : blocklevel field for page txns sourced from atc
#define RPB_BLOCKLEVEL_CONF_ATC_PAGE_BLOCKLEVEL_OFFSET      7
#define RPB_BLOCKLEVEL_CONF_ATC_PAGE_BLOCKLEVEL_MASK        0x180

// Bitfield Description : blocklevel field for invalidation completeion txns sourced from atc
#define RPB_BLOCKLEVEL_CONF_ATC_INV_BLOCKLEVEL_OFFSET      9
#define RPB_BLOCKLEVEL_CONF_ATC_INV_BLOCKLEVEL_MASK        0x600

// Bitfield Description : blocklevel field if needed for io wr
#define RPB_BLOCKLEVEL_CONF_IO_WR_BLOCKLEVEL_OVERRIDE_OFFSET      11
#define RPB_BLOCKLEVEL_CONF_IO_WR_BLOCKLEVEL_OVERRIDE_MASK        0x1800

// Bitfield Description : io wr blocklevel override enable field.
#define RPB_BLOCKLEVEL_CONF_IO_WR_BLOCKLEVEL_OVERRIDE_EN_OFFSET      13
#define RPB_BLOCKLEVEL_CONF_IO_WR_BLOCKLEVEL_OVERRIDE_EN_MASK        0x2000

// Bitfield Description : blocklevel field if needed for io rd
#define RPB_BLOCKLEVEL_CONF_IO_RD_BLOCKLEVEL_OVERRIDE_OFFSET      14
#define RPB_BLOCKLEVEL_CONF_IO_RD_BLOCKLEVEL_OVERRIDE_MASK        0xc000

// Bitfield Description : io rd blocklevel override enable field.
#define RPB_BLOCKLEVEL_CONF_IO_RD_BLOCKLEVEL_OVERRIDE_EN_OFFSET      16
#define RPB_BLOCKLEVEL_CONF_IO_RD_BLOCKLEVEL_OVERRIDE_EN_MASK        0x10000

// Bitfield Description : blocklevel field if needed for atomic
#define RPB_BLOCKLEVEL_CONF_ATOMIC_BLOCKLEVEL_OVERRIDE_OFFSET      17
#define RPB_BLOCKLEVEL_CONF_ATOMIC_BLOCKLEVEL_OVERRIDE_MASK        0x60000

// Bitfield Description : atomic blocklevel override enable field.
#define RPB_BLOCKLEVEL_CONF_ATOMIC_BLOCKLEVEL_OVERRIDE_EN_OFFSET      19
#define RPB_BLOCKLEVEL_CONF_ATOMIC_BLOCKLEVEL_OVERRIDE_EN_MASK        0x80000

// Bitfield Description : 
#define RPB_BLOCKLEVEL_CONF_Reserved_31_20_OFFSET      20
#define RPB_BLOCKLEVEL_CONF_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            XPB_BLOCKLEVEL_OVERRIDE:2;
    UINT32                            XPB_BLOCKLEVEL_OVERRIDE_EN:1;
    UINT32                            ATC_VC5_TR_BLOCKLEVEL:2;
    UINT32                            ATC_VC0_TR_BLOCKLEVEL:2;
    UINT32                            ATC_PAGE_BLOCKLEVEL:2;
    UINT32                            ATC_INV_BLOCKLEVEL:2;
    UINT32                            IO_WR_BLOCKLEVEL_OVERRIDE:2;
    UINT32                            IO_WR_BLOCKLEVEL_OVERRIDE_EN:1;
    UINT32                            IO_RD_BLOCKLEVEL_OVERRIDE:2;
    UINT32                            IO_RD_BLOCKLEVEL_OVERRIDE_EN:1;
    UINT32                            ATOMIC_BLOCKLEVEL_OVERRIDE:2;
    UINT32                            ATOMIC_BLOCKLEVEL_OVERRIDE_EN:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} RPB_BLOCKLEVEL_CONF_STRUCT;

#define SMN_RPB_BLOCKLEVEL_CONF_ADDRESS    0x3354UL


/***********************************************************
* Register Name : RPB_CHAIN_BREAK_TIMEOUT_LOG
* Register Description :
* The register used to monitor when chain break flush timeout,normally those value should be 0.So if below bits values change to 1, it means something bad is happenning.
* Visibility : 0x2
************************************************************/

// Bitfield Description : every bit indicate one ea:[0]-Gcea0,[1]-Gcea1,[2]-Gcea2,[3]-Gcea3,[4]-Mmea0,[5]-Gus,[6]-Dce
#define RPB_CHAIN_BREAK_TIMEOUT_LOG_EA_FLUSH_TIMROUT_FLAG_OFFSET      0
#define RPB_CHAIN_BREAK_TIMEOUT_LOG_EA_FLUSH_TIMROUT_FLAG_MASK        0x7f

// Bitfield Description : 
#define RPB_CHAIN_BREAK_TIMEOUT_LOG_Reserved_31_7_OFFSET      7
#define RPB_CHAIN_BREAK_TIMEOUT_LOG_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            EA_FLUSH_TIMROUT_FLAG:7;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} RPB_CHAIN_BREAK_TIMEOUT_LOG_STRUCT;

#define SMN_RPB_CHAIN_BREAK_TIMEOUT_LOG_ADDRESS    0x33f0UL


/***********************************************************
* Register Name : RPB_CID_QUEUE_EX
* Register Description :
* This register provides wide access to the cid lookup tables used for reads and writes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define RPB_CID_QUEUE_EX_START_OFFSET      0
#define RPB_CID_QUEUE_EX_START_MASK        0x1

// Bitfield Description : This field controls the starting offset into the lookup tables in multiples of 4 3-bit entries Separate internal pointers are maintained for reads and writes which are initialized at start and auto-incremented upon register read/write operations.
#define RPB_CID_QUEUE_EX_OFFSET_OFFSET      1
#define RPB_CID_QUEUE_EX_OFFSET_MASK        0x1fe

// Bitfield Description : 
#define RPB_CID_QUEUE_EX_Reserved_31_9_OFFSET      9
#define RPB_CID_QUEUE_EX_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            START:1;
    UINT32                            OFFSET:8;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} RPB_CID_QUEUE_EX_STRUCT;

#define SMN_RPB_CID_QUEUE_EX_ADDRESS    0x338cUL


/***********************************************************
* Register Name : RPB_CID_QUEUE_EX_DATA
* Register Description :
* Read/Write the wide data for the cid lookup tables using one-shot and post-read-trigger
* Visibility : 0x2
************************************************************/

// Bitfield Description : lower 16 bits are 5 3-bit entries for write unitid lookup table at current offset
#define RPB_CID_QUEUE_EX_DATA_WRITE_ENTRIES_OFFSET      0
#define RPB_CID_QUEUE_EX_DATA_WRITE_ENTRIES_MASK        0xffff

// Bitfield Description : upper 16 bits are 5 3-bit entries for read unitid lookup table at current offset
#define RPB_CID_QUEUE_EX_DATA_READ_ENTRIES_OFFSET      16
#define RPB_CID_QUEUE_EX_DATA_READ_ENTRIES_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            WRITE_ENTRIES:16;
    UINT32                            READ_ENTRIES:16;
  } Field;
  UINT32 Value;
} RPB_CID_QUEUE_EX_DATA_STRUCT;

#define SMN_RPB_CID_QUEUE_EX_DATA_ADDRESS    0x3390UL


/***********************************************************
* Register Name : RPB_CID_QUEUE_RD
* Register Description :
* this register reads rw entries in the cid to queue lookup table
* Visibility : 0x2
************************************************************/

// Bitfield Description : client id used for update operation
#define RPB_CID_QUEUE_RD_CLIENT_ID_LOW_OFFSET      0
#define RPB_CID_QUEUE_RD_CLIENT_ID_LOW_MASK        0x1f

// Bitfield Description : client id used for update operation
#define RPB_CID_QUEUE_RD_CLIENT_ID_HIGH_OFFSET      5
#define RPB_CID_QUEUE_RD_CLIENT_ID_HIGH_MASK        0x7e0

// Bitfield Description : 
#define RPB_CID_QUEUE_RD_WRITE_QUEUE_OFFSET      11
#define RPB_CID_QUEUE_RD_WRITE_QUEUE_MASK        0x3800

// Bitfield Description : 
#define RPB_CID_QUEUE_RD_READ_QUEUE_OFFSET      14
#define RPB_CID_QUEUE_RD_READ_QUEUE_MASK        0x1c000

// Bitfield Description : 
#define RPB_CID_QUEUE_RD_Reserved_31_17_OFFSET      17
#define RPB_CID_QUEUE_RD_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            CLIENT_ID_LOW:5;
    UINT32                            CLIENT_ID_HIGH:6;
    UINT32                            WRITE_QUEUE:3;
    UINT32                            READ_QUEUE:3;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} RPB_CID_QUEUE_RD_STRUCT;

#define SMN_RPB_CID_QUEUE_RD_ADDRESS    0x3388UL


/***********************************************************
* Register Name : RPB_CID_QUEUE_WR
* Register Description :
* this register updates rw entries in the unitid to queue lookup table
* Visibility : 0x2
************************************************************/

// Bitfield Description : client id used for update operation
#define RPB_CID_QUEUE_WR_CLIENT_ID_LOW_OFFSET      0
#define RPB_CID_QUEUE_WR_CLIENT_ID_LOW_MASK        0x1f

// Bitfield Description : client id used for update operation
#define RPB_CID_QUEUE_WR_CLIENT_ID_HIGH_OFFSET      5
#define RPB_CID_QUEUE_WR_CLIENT_ID_HIGH_MASK        0x7e0

// Bitfield Description : 0: unitid based updates, 1: all updated at the same time
#define RPB_CID_QUEUE_WR_UPDATE_MODE_OFFSET      11
#define RPB_CID_QUEUE_WR_UPDATE_MODE_MASK        0x800

// Bitfield Description : 
#define RPB_CID_QUEUE_WR_WRITE_QUEUE_OFFSET      12
#define RPB_CID_QUEUE_WR_WRITE_QUEUE_MASK        0x7000

// Bitfield Description : 
#define RPB_CID_QUEUE_WR_READ_QUEUE_OFFSET      15
#define RPB_CID_QUEUE_WR_READ_QUEUE_MASK        0x38000

// Bitfield Description : 0: no-op, 1: send
#define RPB_CID_QUEUE_WR_UPDATE_OFFSET      18
#define RPB_CID_QUEUE_WR_UPDATE_MASK        0x40000

// Bitfield Description : 
#define RPB_CID_QUEUE_WR_Reserved_31_19_OFFSET      19
#define RPB_CID_QUEUE_WR_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CLIENT_ID_LOW:5;
    UINT32                            CLIENT_ID_HIGH:6;
    UINT32                            UPDATE_MODE:1;
    UINT32                            WRITE_QUEUE:3;
    UINT32                            READ_QUEUE:3;
    UINT32                            UPDATE:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} RPB_CID_QUEUE_WR_STRUCT;

#define SMN_RPB_CID_QUEUE_WR_ADDRESS    0x3380UL


/***********************************************************
* Register Name : RPB_DBG1
* Register Description :
* Debug Controllability
* Visibility : 0x2
************************************************************/

// Bitfield Description : how many outstanding databeat io read request equivalents to be issued to nbif. A value of 0 means no limit. (e.g. 128 requests = 128 * 1 databeat = 128 * 512bits = 128 * 64bytes
#define RPB_DBG1_RPB_OUTSTANDING_RD_32B_OFFSET      0
#define RPB_DBG1_RPB_OUTSTANDING_RD_32B_MASK        0x1fff

// Bitfield Description : if enable, athub will pass thru errevent to nbif, if disable, athub will back slverr for errvent.
#define RPB_DBG1_ERREVENT_ENABLE_OFFSET      13
#define RPB_DBG1_ERREVENT_ENABLE_MASK        0x2000

// Bitfield Description : If enbale, mm translation and gfx translation share a tag space, the shared tag num can be configured by rpb_ats_tr_shared_tag .
#define RPB_DBG1_RPB_ATS_TR_SHARED_TAG_ENABLE_OFFSET      14
#define RPB_DBG1_RPB_ATS_TR_SHARED_TAG_ENABLE_MASK        0x4000

// Bitfield Description : how many tag nums can be shared by mm translation and gfx translation.
#define RPB_DBG1_RPB_ATS_TR_SHARED_TAG_OFFSET      15
#define RPB_DBG1_RPB_ATS_TR_SHARED_TAG_MASK        0x3f8000

// Bitfield Description : If enable, reverse the LSB of rdrspdata of SDP_AT_DMA intf.
#define RPB_DBG1_RPB_DMA_INSERT_PARITY_ERR_OFFSET      22
#define RPB_DBG1_RPB_DMA_INSERT_PARITY_ERR_MASK        0x400000

// Bitfield Description : If enable, reverse the LSB of wrdata of SDP_AT_HST intf.
#define RPB_DBG1_RPB_HST_INSERT_PARITY_ERR_OFFSET      23
#define RPB_DBG1_RPB_HST_INSERT_PARITY_ERR_MASK        0x800000

// Bitfield Description : If enable, we need check vf and vfid with the register write requests that only a request wuth vfid equaling to current active vfid.
#define RPB_DBG1_VFID_CHECK_ENABLE_OFFSET      24
#define RPB_DBG1_VFID_CHECK_ENABLE_MASK        0x1000000

// Bitfield Description : reserved fields
#define RPB_DBG1_Reserved_31_25_OFFSET      25
#define RPB_DBG1_Reserved_31_25_MASK        0xfe000000

typedef union {
  struct {
    UINT32                            RPB_OUTSTANDING_RD_32B:13;
    UINT32                            ERREVENT_ENABLE:1;
    UINT32                            RPB_ATS_TR_SHARED_TAG_ENABLE:1;
    UINT32                            RPB_ATS_TR_SHARED_TAG:7;
    UINT32                            RPB_DMA_INSERT_PARITY_ERR:1;
    UINT32                            RPB_HST_INSERT_PARITY_ERR:1;
    UINT32                            VFID_CHECK_ENABLE:1;
    UINT32                            Reserved_31_25:7;
  } Field;
  UINT32 Value;
} RPB_DBG1_STRUCT;

#define SMN_RPB_DBG1_ADDRESS    0x335cUL


/***********************************************************
* Register Name : RPB_DEINTRLV_COMBINE_CNTL
* Register Description :
* This register controls the write combiners
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of idle cycles before flushing write combiner (2^field),we couldnt break the original wr request into smaller one,it's used when the req_chain mode is enabled, or we use the legacy WC_FLUSH_TIMER.
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CHAINED_FLUSH_TIMER_OFFSET      0
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CHAINED_FLUSH_TIMER_MASK        0xf

// Bitfield Description : enabled means we could break the ea write grouping, disabled means we must wait for the total packets comes into de_interleave buffer, no matter how many cycles.
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CHAINED_BREAK_EN_OFFSET      4
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CHAINED_BREAK_EN_MASK        0x10

// Bitfield Description : 1 means we do not takes care the unitid, etc check, we do write grouping only based on reqchain. 0 means we do check the unitid, etc.
#define RPB_DEINTRLV_COMBINE_CNTL_WC_HANDLE_CHECK_DISABLE_OFFSET      5
#define RPB_DEINTRLV_COMBINE_CNTL_WC_HANDLE_CHECK_DISABLE_MASK        0x20

// Bitfield Description : max # of credits available to use on intf for sending wrreq to xpb
#define RPB_DEINTRLV_COMBINE_CNTL_XPB_WRREQ_CRD_OFFSET      6
#define RPB_DEINTRLV_COMBINE_CNTL_XPB_WRREQ_CRD_MASK        0x3fc0

// Bitfield Description : enable the handling of interleaved data beats from different clients through one ea
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CLI_INTLV_EN_OFFSET      14
#define RPB_DEINTRLV_COMBINE_CNTL_WC_CLI_INTLV_EN_MASK        0x4000

// Bitfield Description : reserved fields
#define RPB_DEINTRLV_COMBINE_CNTL_Reserved_31_15_OFFSET      15
#define RPB_DEINTRLV_COMBINE_CNTL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            WC_CHAINED_FLUSH_TIMER:4;
    UINT32                            WC_CHAINED_BREAK_EN:1;
    UINT32                            WC_HANDLE_CHECK_DISABLE:1;
    UINT32                            XPB_WRREQ_CRD:8;
    UINT32                            WC_CLI_INTLV_EN:1;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} RPB_DEINTRLV_COMBINE_CNTL_STRUCT;

#define SMN_RPB_DEINTRLV_COMBINE_CNTL_ADDRESS    0x3394UL


/***********************************************************
* Register Name : RPB_DF_SDPPORT_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define RPB_DF_SDPPORT_CNTL_DF_REQ_CRD_OFFSET      0
#define RPB_DF_SDPPORT_CNTL_DF_REQ_CRD_MASK        0x3f

// Bitfield Description : 
#define RPB_DF_SDPPORT_CNTL_DF_DATA_CRD_OFFSET      6
#define RPB_DF_SDPPORT_CNTL_DF_DATA_CRD_MASK        0xfc0

// Bitfield Description : 
#define RPB_DF_SDPPORT_CNTL_DF_HALT_THRESHOLD_OFFSET      12
#define RPB_DF_SDPPORT_CNTL_DF_HALT_THRESHOLD_MASK        0xf000

// Bitfield Description : 
#define RPB_DF_SDPPORT_CNTL_DF_RELEASE_CREDIT_MODE_OFFSET      16
#define RPB_DF_SDPPORT_CNTL_DF_RELEASE_CREDIT_MODE_MASK        0x10000

// Bitfield Description : If enable, reverse the LSB of wrdata of SDP_NCS intf.
#define RPB_DF_SDPPORT_CNTL_DF_INSERT_PARITY_ERR_OFFSET      17
#define RPB_DF_SDPPORT_CNTL_DF_INSERT_PARITY_ERR_MASK        0x20000

// Bitfield Description : 1 means will invoke clock with the connection status, 0 mean don't. OBSOLETE
#define RPB_DF_SDPPORT_CNTL_DF_BUSY_INCLUDE_CONN_OFFSET      18
#define RPB_DF_SDPPORT_CNTL_DF_BUSY_INCLUDE_CONN_MASK        0x40000

// Bitfield Description : How many time when athub send orig_ack to df.
#define RPB_DF_SDPPORT_CNTL_DF_ORIG_ACK_TIMER_OFFSET      19
#define RPB_DF_SDPPORT_CNTL_DF_ORIG_ACK_TIMER_MASK        0x7f80000

// Bitfield Description : reserved fields
#define RPB_DF_SDPPORT_CNTL_Reserved_31_27_OFFSET      27
#define RPB_DF_SDPPORT_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            DF_REQ_CRD:6;
    UINT32                            DF_DATA_CRD:6;
    UINT32                            DF_HALT_THRESHOLD:4;
    UINT32                            DF_RELEASE_CREDIT_MODE:1;
    UINT32                            DF_INSERT_PARITY_ERR:1;
    UINT32                            DF_BUSY_INCLUDE_CONN:1;
    UINT32                            DF_ORIG_ACK_TIMER:8;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RPB_DF_SDPPORT_CNTL_STRUCT;

#define SMN_RPB_DF_SDPPORT_CNTL_ADDRESS    0x33dcUL


/***********************************************************
* Register Name : RPB_EA_QUEUE_WR
* Register Description :
* this register updates rw entries in the unitid to queue lookup table
* Visibility : 0x2
************************************************************/

// Bitfield Description : ea_number used for update operation, ea number up to 19. for the ea encoding,please see the ATHUB MAS.
#define RPB_EA_QUEUE_WR_EA_NUMBER_OFFSET      0
#define RPB_EA_QUEUE_WR_EA_NUMBER_MASK        0x1f

// Bitfield Description : 
#define RPB_EA_QUEUE_WR_WRITE_QUEUE_OFFSET      5
#define RPB_EA_QUEUE_WR_WRITE_QUEUE_MASK        0xe0

// Bitfield Description : 
#define RPB_EA_QUEUE_WR_READ_QUEUE_OFFSET      8
#define RPB_EA_QUEUE_WR_READ_QUEUE_MASK        0x700

// Bitfield Description : 
#define RPB_EA_QUEUE_WR_UPDATE_OFFSET      11
#define RPB_EA_QUEUE_WR_UPDATE_MASK        0x800

// Bitfield Description : 
#define RPB_EA_QUEUE_WR_Reserved_31_12_OFFSET      12
#define RPB_EA_QUEUE_WR_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            EA_NUMBER:5;
    UINT32                            WRITE_QUEUE:3;
    UINT32                            READ_QUEUE:3;
    UINT32                            UPDATE:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} RPB_EA_QUEUE_WR_STRUCT;

#define SMN_RPB_EA_QUEUE_WR_ADDRESS    0x3384UL


/***********************************************************
* Register Name : RPB_EFF_CNTL
* Register Description :
* this register provides additional general controls for cid-based efficiency queues
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of cycles to wait after wr requests arrive to start processing eff queues
#define RPB_EFF_CNTL_WR_LAZY_TIMER_OFFSET      0
#define RPB_EFF_CNTL_WR_LAZY_TIMER_MASK        0xff

// Bitfield Description : number of cycles to wait after rd requests arrive to start processing eff queues
#define RPB_EFF_CNTL_RD_LAZY_TIMER_OFFSET      8
#define RPB_EFF_CNTL_RD_LAZY_TIMER_MASK        0xff00

// Bitfield Description : 
#define RPB_EFF_CNTL_Reserved_31_16_OFFSET      16
#define RPB_EFF_CNTL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            WR_LAZY_TIMER:8;
    UINT32                            RD_LAZY_TIMER:8;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RPB_EFF_CNTL_STRUCT;

#define SMN_RPB_EFF_CNTL_ADDRESS    0x3360UL


/***********************************************************
* Register Name : RPB_INTERLEAVE_ONEEA_STATUS
* Register Description :
* The register used to monitor when interleave of one ea happen,normally those value should be 0.So if below bits values change to 1, it means something bad is happenning.
* Visibility : 0x2
************************************************************/

// Bitfield Description : every bit indicate one ea:[0]-Gcea0,[1]-Gcea1,[2]-Gcea2,[3]-Gcea3,[4]-Mmea0,[5]-Gus,[6]-Dce
#define RPB_INTERLEAVE_ONEEA_STATUS_EA_ERROR_FLAG_OFFSET      0
#define RPB_INTERLEAVE_ONEEA_STATUS_EA_ERROR_FLAG_MASK        0x7f

// Bitfield Description : 
#define RPB_INTERLEAVE_ONEEA_STATUS_Reserved_31_7_OFFSET      7
#define RPB_INTERLEAVE_ONEEA_STATUS_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            EA_ERROR_FLAG:7;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} RPB_INTERLEAVE_ONEEA_STATUS_STRUCT;

#define SMN_RPB_INTERLEAVE_ONEEA_STATUS_ADDRESS    0x33ecUL


/***********************************************************
* Register Name : RPB_NBIF_SDPPORT_CNTL
* Register Description :
* this register tells the credits available to use on the RPB<->NBIF interface .
* Visibility : 0x2
************************************************************/

// Bitfield Description : max # of credits available to use on intf for receiving wrrsp from nbif dma channel.
#define RPB_NBIF_SDPPORT_CNTL_NBIF_DMA_WRRSP_CRD_OFFSET      0
#define RPB_NBIF_SDPPORT_CNTL_NBIF_DMA_WRRSP_CRD_MASK        0xff

// Bitfield Description : max # of credits available to use on intf for receiving rdrsp from nbif dma channel.
#define RPB_NBIF_SDPPORT_CNTL_NBIF_DMA_RDRSP_CRD_OFFSET      8
#define RPB_NBIF_SDPPORT_CNTL_NBIF_DMA_RDRSP_CRD_MASK        0xff00

// Bitfield Description : max # of credits available to use on intf for receiving req from nbif hst channel.
#define RPB_NBIF_SDPPORT_CNTL_NBIF_HST_REQ_CRD_OFFSET      16
#define RPB_NBIF_SDPPORT_CNTL_NBIF_HST_REQ_CRD_MASK        0xff0000

// Bitfield Description : max # of credits available to use on intf for receiving data from nbif hst channel.
#define RPB_NBIF_SDPPORT_CNTL_NBIF_HST_DATA_CRD_OFFSET      24
#define RPB_NBIF_SDPPORT_CNTL_NBIF_HST_DATA_CRD_MASK        0xff000000

typedef union {
  struct {
    UINT32                            NBIF_DMA_WRRSP_CRD:8;
    UINT32                            NBIF_DMA_RDRSP_CRD:8;
    UINT32                            NBIF_HST_REQ_CRD:8;
    UINT32                            NBIF_HST_DATA_CRD:8;
  } Field;
  UINT32 Value;
} RPB_NBIF_SDPPORT_CNTL_STRUCT;

#define SMN_RPB_NBIF_SDPPORT_CNTL_ADDRESS    0x33e4UL


/***********************************************************
* Register Name : RPB_PASSPW_CONF
* Register Description :
* RPB Configuration Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : set reqpasspw field for txns sourced from xpb
#define RPB_PASSPW_CONF_XPB_PASSPW_OVERRIDE_OFFSET      0
#define RPB_PASSPW_CONF_XPB_PASSPW_OVERRIDE_MASK        0x1

// Bitfield Description : set reqrsppasspw field for txns sourced from xpb
#define RPB_PASSPW_CONF_XPB_RSPPASSPW_OVERRIDE_OFFSET      1
#define RPB_PASSPW_CONF_XPB_RSPPASSPW_OVERRIDE_MASK        0x2

// Bitfield Description : reqpasspw field ON/OFF override for tr txns sourced from atc
#define RPB_PASSPW_CONF_ATC_VC5_TR_PASSPW_OVERRIDE_OFFSET      2
#define RPB_PASSPW_CONF_ATC_VC5_TR_PASSPW_OVERRIDE_MASK        0x4

// Bitfield Description : atc tr txns reqpasspw override enable field.
#define RPB_PASSPW_CONF_ATC_VC5_TR_PASSPW_OVERRIDE_EN_OFFSET      3
#define RPB_PASSPW_CONF_ATC_VC5_TR_PASSPW_OVERRIDE_EN_MASK        0x8

// Bitfield Description : rsppasspw field ON/OFF override for txns sourced from atc
#define RPB_PASSPW_CONF_ATC_VC5_RSPPASSPW_OVERRIDE_OFFSET      4
#define RPB_PASSPW_CONF_ATC_VC5_RSPPASSPW_OVERRIDE_MASK        0x10

// Bitfield Description : enable field.
#define RPB_PASSPW_CONF_ATC_VC5_RSPPASSPW_OVERRIDE_EN_OFFSET      5
#define RPB_PASSPW_CONF_ATC_VC5_RSPPASSPW_OVERRIDE_EN_MASK        0x20

// Bitfield Description : reqpasspw field ON/OFF override for tr txns sourced from atc
#define RPB_PASSPW_CONF_ATC_VC0_TR_PASSPW_OVERRIDE_OFFSET      6
#define RPB_PASSPW_CONF_ATC_VC0_TR_PASSPW_OVERRIDE_MASK        0x40

// Bitfield Description : atc tr txns reqpasspw override enable field.
#define RPB_PASSPW_CONF_ATC_VC0_TR_PASSPW_OVERRIDE_EN_OFFSET      7
#define RPB_PASSPW_CONF_ATC_VC0_TR_PASSPW_OVERRIDE_EN_MASK        0x80

// Bitfield Description : rsppasspw field ON/OFF override for txns sourced from atc
#define RPB_PASSPW_CONF_ATC_VC0_RSPPASSPW_OVERRIDE_OFFSET      8
#define RPB_PASSPW_CONF_ATC_VC0_RSPPASSPW_OVERRIDE_MASK        0x100

// Bitfield Description : enable field.
#define RPB_PASSPW_CONF_ATC_VC0_RSPPASSPW_OVERRIDE_EN_OFFSET      9
#define RPB_PASSPW_CONF_ATC_VC0_RSPPASSPW_OVERRIDE_EN_MASK        0x200

// Bitfield Description : reqpasspw field ON/OFF override for page txns sourced from atc
#define RPB_PASSPW_CONF_ATC_PAGE_PASSPW_OVERRIDE_OFFSET      10
#define RPB_PASSPW_CONF_ATC_PAGE_PASSPW_OVERRIDE_MASK        0x400

// Bitfield Description : atc page txns reqpasspw override enable field.
#define RPB_PASSPW_CONF_ATC_PAGE_PASSPW_OVERRIDE_EN_OFFSET      11
#define RPB_PASSPW_CONF_ATC_PAGE_PASSPW_OVERRIDE_EN_MASK        0x800

// Bitfield Description : reqpasspw field ON/OFF override for page txns sourced from atc
#define RPB_PASSPW_CONF_ATC_PAGE_RSPPASSPW_OVERRIDE_OFFSET      12
#define RPB_PASSPW_CONF_ATC_PAGE_RSPPASSPW_OVERRIDE_MASK        0x1000

// Bitfield Description : atc page txns rsppasspw override enable field.
#define RPB_PASSPW_CONF_ATC_PAGE_RSPPASSPW_OVERRIDE_EN_OFFSET      13
#define RPB_PASSPW_CONF_ATC_PAGE_RSPPASSPW_OVERRIDE_EN_MASK        0x2000

// Bitfield Description : reqpasspw field force to zero if needed for wr txns sourced from IO (not include atomic)
#define RPB_PASSPW_CONF_WR_PASSPW_OVERRIDE_OFFSET      14
#define RPB_PASSPW_CONF_WR_PASSPW_OVERRIDE_MASK        0x4000

// Bitfield Description : reqrsppasspw field force to one if needed for wr txns sourced from IO (not include atomic)
#define RPB_PASSPW_CONF_WR_RSPPASSPW_OVERRIDE_OFFSET      15
#define RPB_PASSPW_CONF_WR_RSPPASSPW_OVERRIDE_MASK        0x8000

// Bitfield Description : reqpasspw field force to zero if needed for rd txns sourced from IO (not include atomic)
#define RPB_PASSPW_CONF_RD_PASSPW_OVERRIDE_OFFSET      16
#define RPB_PASSPW_CONF_RD_PASSPW_OVERRIDE_MASK        0x10000

// Bitfield Description : reqrsppasspw field force to one if needed for rd txns sourced from IO (not include atomic)
#define RPB_PASSPW_CONF_RD_RSPPASSPW_OVERRIDE_OFFSET      17
#define RPB_PASSPW_CONF_RD_RSPPASSPW_OVERRIDE_MASK        0x20000

// Bitfield Description : reqpasspw field ON/OFF override for atomic txns. means that we force the atomic request reqpass pw to zero, which means that it will not pass post write.
#define RPB_PASSPW_CONF_ATOMIC_PASSPW_OVERRIDE_OFFSET      18
#define RPB_PASSPW_CONF_ATOMIC_PASSPW_OVERRIDE_MASK        0x40000

// Bitfield Description : rsppasspw field ON/OFF override for atomic txns. means that we force the atomic request rsppass pw to 1, which means that atomic return could pass host write.
#define RPB_PASSPW_CONF_ATOMIC_RSPPASSPW_OVERRIDE_OFFSET      19
#define RPB_PASSPW_CONF_ATOMIC_RSPPASSPW_OVERRIDE_MASK        0x80000

// Bitfield Description : wrrsppasspw field override for IO/atomicnr(turn from rd to wr)/p2p wr/unsupport command wrrsp txns soourced from nbif
#define RPB_PASSPW_CONF_WRRSP_PASSPW_OVERRIDE_OFFSET      20
#define RPB_PASSPW_CONF_WRRSP_PASSPW_OVERRIDE_MASK        0x100000

// Bitfield Description : wrrsppasspw override enable field.
#define RPB_PASSPW_CONF_WRRSP_PASSPW_OVERRIDE_EN_OFFSET      21
#define RPB_PASSPW_CONF_WRRSP_PASSPW_OVERRIDE_EN_MASK        0x200000

// Bitfield Description : rdrsppasspw field override for IO rdrsp/atomic txns sourced from nbif
#define RPB_PASSPW_CONF_RDRSP_PASSPW_OVERRIDE_OFFSET      22
#define RPB_PASSPW_CONF_RDRSP_PASSPW_OVERRIDE_MASK        0x400000

// Bitfield Description : rdrsppasspw override enable field.
#define RPB_PASSPW_CONF_RDRSP_PASSPW_OVERRIDE_EN_OFFSET      23
#define RPB_PASSPW_CONF_RDRSP_PASSPW_OVERRIDE_EN_MASK        0x800000

// Bitfield Description : 
#define RPB_PASSPW_CONF_Reserved_31_24_OFFSET      24
#define RPB_PASSPW_CONF_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            XPB_PASSPW_OVERRIDE:1;
    UINT32                            XPB_RSPPASSPW_OVERRIDE:1;
    UINT32                            ATC_VC5_TR_PASSPW_OVERRIDE:1;
    UINT32                            ATC_VC5_TR_PASSPW_OVERRIDE_EN:1;
    UINT32                            ATC_VC5_RSPPASSPW_OVERRIDE:1;
    UINT32                            ATC_VC5_RSPPASSPW_OVERRIDE_EN:1;
    UINT32                            ATC_VC0_TR_PASSPW_OVERRIDE:1;
    UINT32                            ATC_VC0_TR_PASSPW_OVERRIDE_EN:1;
    UINT32                            ATC_VC0_RSPPASSPW_OVERRIDE:1;
    UINT32                            ATC_VC0_RSPPASSPW_OVERRIDE_EN:1;
    UINT32                            ATC_PAGE_PASSPW_OVERRIDE:1;
    UINT32                            ATC_PAGE_PASSPW_OVERRIDE_EN:1;
    UINT32                            ATC_PAGE_RSPPASSPW_OVERRIDE:1;
    UINT32                            ATC_PAGE_RSPPASSPW_OVERRIDE_EN:1;
    UINT32                            WR_PASSPW_OVERRIDE:1;
    UINT32                            WR_RSPPASSPW_OVERRIDE:1;
    UINT32                            RD_PASSPW_OVERRIDE:1;
    UINT32                            RD_RSPPASSPW_OVERRIDE:1;
    UINT32                            ATOMIC_PASSPW_OVERRIDE:1;
    UINT32                            ATOMIC_RSPPASSPW_OVERRIDE:1;
    UINT32                            WRRSP_PASSPW_OVERRIDE:1;
    UINT32                            WRRSP_PASSPW_OVERRIDE_EN:1;
    UINT32                            RDRSP_PASSPW_OVERRIDE:1;
    UINT32                            RDRSP_PASSPW_OVERRIDE_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} RPB_PASSPW_CONF_STRUCT;

#define SMN_RPB_PASSPW_CONF_ADDRESS    0x3350UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER0_CFG
* Register Description :
* Configuration for each performance counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. event 0 reserved event 1 number of write requests in event 2 number of read requests in event 3 number of atc requests in event 4 number of xpb requests in event 5 number of atomic requests in event 24:6 number of write q0~18 requests in event 43:25 number of write q0~18 packets out event 49:44 number of read q0~5 requests in event 50 number of write packets out event 51 number of atomic request out event 52 number of xpb request out event 53 number of write request out event 54 number of read request out event 55 number of trans request out event 56 number of dma atomic request out event 57 number of dma trans request out event 58 number of dma read request out event 59 number of dma write request out event 60 number of dma write response in event 61 number of dma read response in event 62 number of dma trans response in event 255:63 reserved
#define RPB_PERFCOUNTER0_CFG_PERF_SEL_OFFSET      0
#define RPB_PERFCOUNTER0_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define RPB_PERFCOUNTER0_CFG_PERF_SEL_END_OFFSET      8
#define RPB_PERFCOUNTER0_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define RPB_PERFCOUNTER0_CFG_Reserved_23_16_OFFSET      16
#define RPB_PERFCOUNTER0_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define RPB_PERFCOUNTER0_CFG_PERF_MODE_OFFSET      24
#define RPB_PERFCOUNTER0_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : Only enable counting for this counter when RPB_PERFCOUNTER_RSLT_CNTL.ENABLE_ALL and this bit are set..
#define RPB_PERFCOUNTER0_CFG_ENABLE_OFFSET      28
#define RPB_PERFCOUNTER0_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : Write Only, when written to a 1 clears the performance counter. Reads of this register have no meaning. Writing 0 to this field has no effect.
#define RPB_PERFCOUNTER0_CFG_CLEAR_OFFSET      29
#define RPB_PERFCOUNTER0_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define RPB_PERFCOUNTER0_CFG_Reserved_31_30_OFFSET      30
#define RPB_PERFCOUNTER0_CFG_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PERF_SEL:8;
    UINT32                            PERF_SEL_END:8;
    UINT32                            Reserved_23_16:8;
    UINT32                            PERF_MODE:4;
    UINT32                            ENABLE:1;
    UINT32                            CLEAR:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER0_CFG_STRUCT;

#define SMN_RPB_PERFCOUNTER0_CFG_ADDRESS    0x33acUL


/***********************************************************
* Register Name : RPB_PERFCOUNTER1_CFG
* Register Description :
* Configuration for each performance counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. event 0 reserved event 1 number of write requests in event 2 number of read requests in event 3 number of atc requests in event 4 number of xpb requests in event 5 number of atomic requests in event 24:6 number of write q0~18 requests in event 43:25 number of write q0~18 packets out event 49:44 number of read q0~5 requests in event 50 number of write packets out event 51 number of atomic request out event 52 number of xpb request out event 53 number of write request out event 54 number of read request out event 55 number of trans request out event 56 number of dma atomic request out event 57 number of dma trans request out event 58 number of dma read request out event 59 number of dma write request out event 60 number of dma write response in event 61 number of dma read response in event 62 number of dma trans response in event 255:63 reserved
#define RPB_PERFCOUNTER1_CFG_PERF_SEL_OFFSET      0
#define RPB_PERFCOUNTER1_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define RPB_PERFCOUNTER1_CFG_PERF_SEL_END_OFFSET      8
#define RPB_PERFCOUNTER1_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define RPB_PERFCOUNTER1_CFG_Reserved_23_16_OFFSET      16
#define RPB_PERFCOUNTER1_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define RPB_PERFCOUNTER1_CFG_PERF_MODE_OFFSET      24
#define RPB_PERFCOUNTER1_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : Only enable counting for this counter when RPB_PERFCOUNTER_RSLT_CNTL.ENABLE_ALL and this bit are set..
#define RPB_PERFCOUNTER1_CFG_ENABLE_OFFSET      28
#define RPB_PERFCOUNTER1_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : Write Only, when written to a 1 clears the performance counter. Reads of this register have no meaning. Writing 0 to this field has no effect.
#define RPB_PERFCOUNTER1_CFG_CLEAR_OFFSET      29
#define RPB_PERFCOUNTER1_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define RPB_PERFCOUNTER1_CFG_Reserved_31_30_OFFSET      30
#define RPB_PERFCOUNTER1_CFG_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PERF_SEL:8;
    UINT32                            PERF_SEL_END:8;
    UINT32                            Reserved_23_16:8;
    UINT32                            PERF_MODE:4;
    UINT32                            ENABLE:1;
    UINT32                            CLEAR:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER1_CFG_STRUCT;

#define SMN_RPB_PERFCOUNTER1_CFG_ADDRESS    0x33b0UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER2_CFG
* Register Description :
* Configuration for each performance counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. event 0 reserved event 1 number of write requests in event 2 number of read requests in event 3 number of atc requests in event 4 number of xpb requests in event 5 number of atomic requests in event 24:6 number of write q0~18 requests in event 43:25 number of write q0~18 packets out event 49:44 number of read q0~5 requests in event 50 number of write packets out event 51 number of atomic request out event 52 number of xpb request out event 53 number of write request out event 54 number of read request out event 55 number of trans request out event 56 number of dma atomic request out event 57 number of dma trans request out event 58 number of dma read request out event 59 number of dma write request out event 60 number of dma write response in event 61 number of dma read response in event 62 number of dma trans response in event 255:63 reserved
#define RPB_PERFCOUNTER2_CFG_PERF_SEL_OFFSET      0
#define RPB_PERFCOUNTER2_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define RPB_PERFCOUNTER2_CFG_PERF_SEL_END_OFFSET      8
#define RPB_PERFCOUNTER2_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define RPB_PERFCOUNTER2_CFG_Reserved_23_16_OFFSET      16
#define RPB_PERFCOUNTER2_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define RPB_PERFCOUNTER2_CFG_PERF_MODE_OFFSET      24
#define RPB_PERFCOUNTER2_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : Only enable counting for this counter when RPB_PERFCOUNTER_RSLT_CNTL.ENABLE_ALL and this bit are set..
#define RPB_PERFCOUNTER2_CFG_ENABLE_OFFSET      28
#define RPB_PERFCOUNTER2_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : Write Only, when written to a 1 clears the performance counter. Reads of this register have no meaning. Writing 0 to this field has no effect.
#define RPB_PERFCOUNTER2_CFG_CLEAR_OFFSET      29
#define RPB_PERFCOUNTER2_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define RPB_PERFCOUNTER2_CFG_Reserved_31_30_OFFSET      30
#define RPB_PERFCOUNTER2_CFG_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PERF_SEL:8;
    UINT32                            PERF_SEL_END:8;
    UINT32                            Reserved_23_16:8;
    UINT32                            PERF_MODE:4;
    UINT32                            ENABLE:1;
    UINT32                            CLEAR:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER2_CFG_STRUCT;

#define SMN_RPB_PERFCOUNTER2_CFG_ADDRESS    0x33b4UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER3_CFG
* Register Description :
* Configuration for each performance counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. event 0 reserved event 1 number of write requests in event 2 number of read requests in event 3 number of atc requests in event 4 number of xpb requests in event 5 number of atomic requests in event 24:6 number of write q0~18 requests in event 43:25 number of write q0~18 packets out event 49:44 number of read q0~5 requests in event 50 number of write packets out event 51 number of atomic request out event 52 number of xpb request out event 53 number of write request out event 54 number of read request out event 55 number of trans request out event 56 number of dma atomic request out event 57 number of dma trans request out event 58 number of dma read request out event 59 number of dma write request out event 60 number of dma write response in event 61 number of dma read response in event 62 number of dma trans response in event 255:63 reserved
#define RPB_PERFCOUNTER3_CFG_PERF_SEL_OFFSET      0
#define RPB_PERFCOUNTER3_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define RPB_PERFCOUNTER3_CFG_PERF_SEL_END_OFFSET      8
#define RPB_PERFCOUNTER3_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define RPB_PERFCOUNTER3_CFG_Reserved_23_16_OFFSET      16
#define RPB_PERFCOUNTER3_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define RPB_PERFCOUNTER3_CFG_PERF_MODE_OFFSET      24
#define RPB_PERFCOUNTER3_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : Only enable counting for this counter when RPB_PERFCOUNTER_RSLT_CNTL.ENABLE_ALL and this bit are set..
#define RPB_PERFCOUNTER3_CFG_ENABLE_OFFSET      28
#define RPB_PERFCOUNTER3_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : Write Only, when written to 1 clears the performance counter. Reads of this register have no meaning. Writing 0 to this field has no effect.
#define RPB_PERFCOUNTER3_CFG_CLEAR_OFFSET      29
#define RPB_PERFCOUNTER3_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define RPB_PERFCOUNTER3_CFG_Reserved_31_30_OFFSET      30
#define RPB_PERFCOUNTER3_CFG_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PERF_SEL:8;
    UINT32                            PERF_SEL_END:8;
    UINT32                            Reserved_23_16:8;
    UINT32                            PERF_MODE:4;
    UINT32                            ENABLE:1;
    UINT32                            CLEAR:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER3_CFG_STRUCT;

#define SMN_RPB_PERFCOUNTER3_CFG_ADDRESS    0x33b8UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER_HI
* Register Description :
* High Bits of the Performance Counter one-shot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Either Bits 47:32 of the counter, or a comparison value used for PERF_MODE 7,8, 13
#define RPB_PERFCOUNTER_HI_COUNTER_HI_OFFSET      0
#define RPB_PERFCOUNTER_HI_COUNTER_HI_MASK        0xffff

// Bitfield Description : Comparison Value used for PERF_MODE PERFMON_COUNTER_MODE_CYCLES_GE_HI, PERFMON_COUNTER_MODE_CYCLES_EQ_HI, and PERFMON_COUNTER_MODE_STATISTICAL_LATENCY_GE_HI
#define RPB_PERFCOUNTER_HI_COMPARE_VALUE_OFFSET      16
#define RPB_PERFCOUNTER_HI_COMPARE_VALUE_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            COUNTER_HI:16;
    UINT32                            COMPARE_VALUE:16;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER_HI_STRUCT;

#define SMN_RPB_PERFCOUNTER_HI_ADDRESS    0x33a8UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER_LO
* Register Description :
* Low Bits of the Performance Counter one-shot
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bits 31:0 of the counter
#define RPB_PERFCOUNTER_LO_COUNTER_LO_OFFSET      0
#define RPB_PERFCOUNTER_LO_COUNTER_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER_LO:32;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER_LO_STRUCT;

#define SMN_RPB_PERFCOUNTER_LO_ADDRESS    0x33a4UL


/***********************************************************
* Register Name : RPB_PERFCOUNTER_RSLT_CNTL
* Register Description :
* Control to Select which Performance Counter to read from, and some more master control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects counter for display in RPB_PERFCOUNTER_LO Selects counter for display in RPB_PERFCOUNTER_HI
#define RPB_PERFCOUNTER_RSLT_CNTL_PERF_COUNTER_SELECT_OFFSET      0
#define RPB_PERFCOUNTER_RSLT_CNTL_PERF_COUNTER_SELECT_MASK        0xf

// Bitfield Description : 
#define RPB_PERFCOUNTER_RSLT_CNTL_Reserved_7_4_OFFSET      4
#define RPB_PERFCOUNTER_RSLT_CNTL_Reserved_7_4_MASK        0xf0

// Bitfield Description : A value of 0 means always start the counters. If another value is selected, dont start counting until the first time we see that event after an RPB_PERFCOUNTER_RSLT_CNTL.CLEAR_ALL or a reset.enable or clear. If a latency PERF_MODE is selected, the first start event after trigger is triggered will start the latency counting.
#define RPB_PERFCOUNTER_RSLT_CNTL_START_TRIGGER_OFFSET      8
#define RPB_PERFCOUNTER_RSLT_CNTL_START_TRIGGER_MASK        0xff00

// Bitfield Description : A value of 0 means dont stop the counters. If another value is selected, stop counting when we see that event. After the STOP_TRIGGER is triggered, the counter will only start counting again after the counter has been cleared either using RPB_PERFCOUNTER_RSLT_CNTL.CLEAR_ALL. or that counters RPB_PERFCOUNTER_CFG.CLEAR is set. If a latency PERF_MODE is selected and the STOP_TRIGGER is triggered before an end, that transaction will not be accounted for in the latency count.
#define RPB_PERFCOUNTER_RSLT_CNTL_STOP_TRIGGER_OFFSET      16
#define RPB_PERFCOUNTER_RSLT_CNTL_STOP_TRIGGER_MASK        0xff0000

// Bitfield Description : Master Control for enabling all performance counters in this block. No counter will count when this bit is clear.
#define RPB_PERFCOUNTER_RSLT_CNTL_ENABLE_ANY_OFFSET      24
#define RPB_PERFCOUNTER_RSLT_CNTL_ENABLE_ANY_MASK        0x1000000

// Bitfield Description : When written to a 1 clears all the performance counters in this block, Reads of this register have no meaning, Writing 0 to this field has no effect
#define RPB_PERFCOUNTER_RSLT_CNTL_CLEAR_ALL_OFFSET      25
#define RPB_PERFCOUNTER_RSLT_CNTL_CLEAR_ALL_MASK        0x2000000

// Bitfield Description : Stop all counters in RPB when any counter saturates. When PERFMON_COUNTER_MODE_ACCUM is selected, saturation occurs when the counter is within 2^48-2^10. When other PERF_MODEs are selected, saturation occurs when all 48 bits are 1.
#define RPB_PERFCOUNTER_RSLT_CNTL_STOP_ALL_ON_SATURATE_OFFSET      26
#define RPB_PERFCOUNTER_RSLT_CNTL_STOP_ALL_ON_SATURATE_MASK        0x4000000

// Bitfield Description : 
#define RPB_PERFCOUNTER_RSLT_CNTL_Reserved_31_27_OFFSET      27
#define RPB_PERFCOUNTER_RSLT_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            PERF_COUNTER_SELECT:4;
    UINT32                            Reserved_7_4:4;
    UINT32                            START_TRIGGER:8;
    UINT32                            STOP_TRIGGER:8;
    UINT32                            ENABLE_ANY:1;
    UINT32                            CLEAR_ALL:1;
    UINT32                            STOP_ALL_ON_SATURATE:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RPB_PERFCOUNTER_RSLT_CNTL_STRUCT;

#define SMN_RPB_PERFCOUNTER_RSLT_CNTL_ADDRESS    0x33bcUL


/***********************************************************
* Register Name : RPB_PERF_COUNTER_CNTL
* Register Description :
* This register selects the value available for reading in the RPB PERF_COUNTER_STATUS register. OBSOLETE There are 4 actual counters, but one can assign any one of 16 events to each counter.
* Visibility : 0x2
************************************************************/

// Bitfield Description : selects counter for display in status register (value 0-3)
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_SELECT_OFFSET      0
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_SELECT_MASK        0x3

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_CLEAR_SELECTED_PERF_COUNTER_OFFSET      2
#define RPB_PERF_COUNTER_CNTL_CLEAR_SELECTED_PERF_COUNTER_MASK        0x4

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_CLEAR_ALL_PERF_COUNTERS_OFFSET      3
#define RPB_PERF_COUNTER_CNTL_CLEAR_ALL_PERF_COUNTERS_MASK        0x8

// Bitfield Description : all counters stop if any enabled counter saturates
#define RPB_PERF_COUNTER_CNTL_STOP_ON_COUNTER_SATURATION_OFFSET      4
#define RPB_PERF_COUNTER_CNTL_STOP_ON_COUNTER_SATURATION_MASK        0x10

// Bitfield Description : set associated bit to 1 for enable, 0 for disable
#define RPB_PERF_COUNTER_CNTL_ENABLE_PERF_COUNTERS_OFFSET      5
#define RPB_PERF_COUNTER_CNTL_ENABLE_PERF_COUNTERS_MASK        0x1e0

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_0_OFFSET      9
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_0_MASK        0x3e00

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_1_OFFSET      14
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_1_MASK        0x7c000

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_2_OFFSET      19
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_2_MASK        0xf80000

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_3_OFFSET      24
#define RPB_PERF_COUNTER_CNTL_PERF_COUNTER_ASSIGN_3_MASK        0x1f000000

// Bitfield Description : 
#define RPB_PERF_COUNTER_CNTL_Reserved_31_29_OFFSET      29
#define RPB_PERF_COUNTER_CNTL_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            PERF_COUNTER_SELECT:2;
    UINT32                            CLEAR_SELECTED_PERF_COUNTER:1;
    UINT32                            CLEAR_ALL_PERF_COUNTERS:1;
    UINT32                            STOP_ON_COUNTER_SATURATION:1;
    UINT32                            ENABLE_PERF_COUNTERS:4;
    UINT32                            PERF_COUNTER_ASSIGN_0:5;
    UINT32                            PERF_COUNTER_ASSIGN_1:5;
    UINT32                            PERF_COUNTER_ASSIGN_2:5;
    UINT32                            PERF_COUNTER_ASSIGN_3:5;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} RPB_PERF_COUNTER_CNTL_STRUCT;

#define SMN_RPB_PERF_COUNTER_CNTL_ADDRESS    0x339cUL


/***********************************************************
* Register Name : RPB_PERF_COUNTER_STATUS
* Register Description :
* This register contains the value of the performance counter selected in the PERF_COUNTER_CNTL register. OBSOLETE
* Visibility : 0x2
************************************************************/

// Bitfield Description : value of selected performance counter
#define RPB_PERF_COUNTER_STATUS_PERFORMANCE_COUNTER_VALUE_OFFSET      0
#define RPB_PERF_COUNTER_STATUS_PERFORMANCE_COUNTER_VALUE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PERFORMANCE_COUNTER_VALUE:32;
  } Field;
  UINT32 Value;
} RPB_PERF_COUNTER_STATUS_STRUCT;

#define SMN_RPB_PERF_COUNTER_STATUS_ADDRESS    0x33a0UL


/***********************************************************
* Register Name : RPB_RD_QUEUE_CNTL
* Register Description :
* this register tells us how to configure the rd_queue
* Visibility : 0x2
************************************************************/

// Bitfield Description : arb mode for rd queue allocation
#define RPB_RD_QUEUE_CNTL_ARB_MODE_OFFSET      0
#define RPB_RD_QUEUE_CNTL_ARB_MODE_MASK        0x1

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL_Q4_SHARED_OFFSET      1
#define RPB_RD_QUEUE_CNTL_Q4_SHARED_MASK        0x2

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL_Q5_SHARED_OFFSET      2
#define RPB_RD_QUEUE_CNTL_Q5_SHARED_MASK        0x4

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL_Q4_UNITID_EA_MODE_OFFSET      3
#define RPB_RD_QUEUE_CNTL_Q4_UNITID_EA_MODE_MASK        0x8

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL_Q5_UNITID_EA_MODE_OFFSET      4
#define RPB_RD_QUEUE_CNTL_Q5_UNITID_EA_MODE_MASK        0x10

// Bitfield Description : its valid only if the q4 are in reserved mode, in ea mode, this means the sdp port that need special treatment.in unitid mode, it means that unitid need special processing.
#define RPB_RD_QUEUE_CNTL_Q4_PATTERN_LOW_OFFSET      5
#define RPB_RD_QUEUE_CNTL_Q4_PATTERN_LOW_MASK        0x3e0

// Bitfield Description : its valid only if the q4 are in reserved mode, in ea mode, this means the sdp port that need special treatment.in unitid mode, it means that unitid need special processing.
#define RPB_RD_QUEUE_CNTL_Q4_PATTERN_HIGH_OFFSET      10
#define RPB_RD_QUEUE_CNTL_Q4_PATTERN_HIGH_MASK        0xfc00

// Bitfield Description : its valid only if the q5 are in reserved mode, in ea mode, this means the sdp port that need special treatment. in unitid mode, it means that unitid need special processing
#define RPB_RD_QUEUE_CNTL_Q5_PATTERN_LOW_OFFSET      16
#define RPB_RD_QUEUE_CNTL_Q5_PATTERN_LOW_MASK        0x1f0000

// Bitfield Description : its valid only if the q5 are in reserved mode, in ea mode, this means the sdp port that need special treatment. in unitid mode, it means that unitid need special processing
#define RPB_RD_QUEUE_CNTL_Q5_PATTERN_HIGH_OFFSET      21
#define RPB_RD_QUEUE_CNTL_Q5_PATTERN_HIGH_MASK        0x7e00000

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL_Reserved_31_27_OFFSET      27
#define RPB_RD_QUEUE_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            ARB_MODE:1;
    UINT32                            Q4_SHARED:1;
    UINT32                            Q5_SHARED:1;
    UINT32                            Q4_UNITID_EA_MODE:1;
    UINT32                            Q5_UNITID_EA_MODE:1;
    UINT32                            Q4_PATTERN_LOW:5;
    UINT32                            Q4_PATTERN_HIGH:6;
    UINT32                            Q5_PATTERN_LOW:5;
    UINT32                            Q5_PATTERN_HIGH:6;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RPB_RD_QUEUE_CNTL_STRUCT;

#define SMN_RPB_RD_QUEUE_CNTL_ADDRESS    0x33c0UL


/***********************************************************
* Register Name : RPB_RD_QUEUE_CNTL2
* Register Description :
* this register tells us how to configure the rd_queues mask pattern
* Visibility : 0x2
************************************************************/

// Bitfield Description : co-work with the q4_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_RD_QUEUE_CNTL2_Q4_PATTERN_MASK_LOW_OFFSET      0
#define RPB_RD_QUEUE_CNTL2_Q4_PATTERN_MASK_LOW_MASK        0x1f

// Bitfield Description : co-work with the q4_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_RD_QUEUE_CNTL2_Q4_PATTERN_MASK_HIGH_OFFSET      5
#define RPB_RD_QUEUE_CNTL2_Q4_PATTERN_MASK_HIGH_MASK        0x7e0

// Bitfield Description : co-work with the q5_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_RD_QUEUE_CNTL2_Q5_PATTERN_MASK_LOW_OFFSET      11
#define RPB_RD_QUEUE_CNTL2_Q5_PATTERN_MASK_LOW_MASK        0xf800

// Bitfield Description : co-work with the q5_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_RD_QUEUE_CNTL2_Q5_PATTERN_MASK_HIGH_OFFSET      16
#define RPB_RD_QUEUE_CNTL2_Q5_PATTERN_MASK_HIGH_MASK        0x3f0000

// Bitfield Description : 
#define RPB_RD_QUEUE_CNTL2_Reserved_31_22_OFFSET      22
#define RPB_RD_QUEUE_CNTL2_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            Q4_PATTERN_MASK_LOW:5;
    UINT32                            Q4_PATTERN_MASK_HIGH:6;
    UINT32                            Q5_PATTERN_MASK_LOW:5;
    UINT32                            Q5_PATTERN_MASK_HIGH:6;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} RPB_RD_QUEUE_CNTL2_STRUCT;

#define SMN_RPB_RD_QUEUE_CNTL2_ADDRESS    0x33c4UL


/***********************************************************
* Register Name : RPB_RD_SWITCH_CNTL
* Register Description :
* this register controls the read efficiency queues
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of requests to process before switching queues
#define RPB_RD_SWITCH_CNTL_QUEUE0_SWITCH_NUM_OFFSET      0
#define RPB_RD_SWITCH_CNTL_QUEUE0_SWITCH_NUM_MASK        0x7f

// Bitfield Description : number of requests to process before switching queues
#define RPB_RD_SWITCH_CNTL_QUEUE1_SWITCH_NUM_OFFSET      7
#define RPB_RD_SWITCH_CNTL_QUEUE1_SWITCH_NUM_MASK        0x3f80

// Bitfield Description : number of requests to process before switching queues
#define RPB_RD_SWITCH_CNTL_QUEUE2_SWITCH_NUM_OFFSET      14
#define RPB_RD_SWITCH_CNTL_QUEUE2_SWITCH_NUM_MASK        0x1fc000

// Bitfield Description : number of requests to process before switching queues
#define RPB_RD_SWITCH_CNTL_QUEUE3_SWITCH_NUM_OFFSET      21
#define RPB_RD_SWITCH_CNTL_QUEUE3_SWITCH_NUM_MASK        0xfe00000

// Bitfield Description : switch_num_mode, 1 bandwidth mode, 0 trans num mode.
#define RPB_RD_SWITCH_CNTL_SWITCH_NUM_MODE_OFFSET      28
#define RPB_RD_SWITCH_CNTL_SWITCH_NUM_MODE_MASK        0x10000000

// Bitfield Description : enable workload adjust R-R weight .
#define RPB_RD_SWITCH_CNTL_WORKLOAD_ADJUST_EN_OFFSET      29
#define RPB_RD_SWITCH_CNTL_WORKLOAD_ADJUST_EN_MASK        0x20000000

// Bitfield Description : weight adjusted step under workload adjust weight R-R mode (2^field)
#define RPB_RD_SWITCH_CNTL_WEIGHT_ADJUST_STEP_OFFSET      30
#define RPB_RD_SWITCH_CNTL_WEIGHT_ADJUST_STEP_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            QUEUE0_SWITCH_NUM:7;
    UINT32                            QUEUE1_SWITCH_NUM:7;
    UINT32                            QUEUE2_SWITCH_NUM:7;
    UINT32                            QUEUE3_SWITCH_NUM:7;
    UINT32                            SWITCH_NUM_MODE:1;
    UINT32                            WORKLOAD_ADJUST_EN:1;
    UINT32                            WEIGHT_ADJUST_STEP:2;
  } Field;
  UINT32 Value;
} RPB_RD_SWITCH_CNTL_STRUCT;

#define SMN_RPB_RD_SWITCH_CNTL_ADDRESS    0x3378UL


/***********************************************************
* Register Name : RPB_SDPPORT_CNTL
* Register Description :
* this register provides controls for the sdp handshake parameters
* Visibility : 0x2
************************************************************/

// Bitfield Description : 0 means that after each reset, we wont do one round connection, until theres request in block. 1 means that after each reset, we quickly do one round connection.the reason is that we are afraid that if after reset, nbif couldnt release the credits for the orig channels of athub.
#define RPB_SDPPORT_CNTL_NBIF_DMA_SELF_ACTIVATE_OFFSET      0
#define RPB_SDPPORT_CNTL_NBIF_DMA_SELF_ACTIVATE_MASK        0x1

// Bitfield Description : 0 means that it is dynamically connected and disconnected,based on busy signal 1 means that we based on input from power module, or other ctrl block. 2 means that we connected it all the time after reset, we wont disconnected.
#define RPB_SDPPORT_CNTL_NBIF_DMA_CFG_MODE_OFFSET      1
#define RPB_SDPPORT_CNTL_NBIF_DMA_CFG_MODE_MASK        0x6

// Bitfield Description : it will decide if we need to release credits, 0 means dont release, 1 means release.
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_REISSUE_CREDIT_OFFSET      3
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_REISSUE_CREDIT_MASK        0x8

// Bitfield Description : it will decide if we saturate the counter for the dma channel( dma req/data credits). 0 we dont saturate, the counter will overflow. 1 we saturate, the counter will keep the max value.
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_SATURATE_COUNTER_OFFSET      4
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_SATURATE_COUNTER_MASK        0x10

// Bitfield Description : 1 means we send fulldis based on the sdp enhance protocol. 0 means we send one-side dis based on the sdp enhance protocol.
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_DISRUPT_FULLDIS_OFFSET      5
#define RPB_SDPPORT_CNTL_NBIF_DMA_ENABLE_DISRUPT_FULLDIS_MASK        0x20

// Bitfield Description : means we wait how many cycles before the state changes , one means 16 cycles.
#define RPB_SDPPORT_CNTL_NBIF_DMA_HALT_THRESHOLD_OFFSET      6
#define RPB_SDPPORT_CNTL_NBIF_DMA_HALT_THRESHOLD_MASK        0x3c0

// Bitfield Description : 0 means that after each reset, we wont do one round connection, until theres request in block. 1 means that after each reset, we quickly do one round connection.the reason is that we are afraid that if after reset, nbif couldnt release the credits for the orig channels of athub.
#define RPB_SDPPORT_CNTL_NBIF_HST_SELF_ACTIVATE_OFFSET      10
#define RPB_SDPPORT_CNTL_NBIF_HST_SELF_ACTIVATE_MASK        0x400

// Bitfield Description : OBSOLETE. 0 means that it is dynamically connected and disconnected,based on busy signal 1 means that we based on input from power module, or other ctrl block. 2 means that we connected it all the time after reset, we wont disconnected.
#define RPB_SDPPORT_CNTL_NBIF_HST_CFG_MODE_OFFSET      11
#define RPB_SDPPORT_CNTL_NBIF_HST_CFG_MODE_MASK        0x1800

// Bitfield Description : it will decide if we need to release credits, 0 means dont release, 1 means release.
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_REISSUE_CREDIT_OFFSET      13
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_REISSUE_CREDIT_MASK        0x2000

// Bitfield Description : it will decide if we saturate the counter for the hst channel( hst req/data credits). 0 we dont saturate, the counter will overflow. 1 we saturate, the counter will keep the max value.
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_SATURATE_COUNTER_OFFSET      14
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_SATURATE_COUNTER_MASK        0x4000

// Bitfield Description : 1 means we send fulldis based on the sdp enhance protocol. 0 means we send one-side dis based on the sdp enhance protocol.
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_DISRUPT_FULLDIS_OFFSET      15
#define RPB_SDPPORT_CNTL_NBIF_HST_ENABLE_DISRUPT_FULLDIS_MASK        0x8000

// Bitfield Description : means we wait how many cycles before the state changes , one means 16 cycles.
#define RPB_SDPPORT_CNTL_NBIF_HST_HALT_THRESHOLD_OFFSET      16
#define RPB_SDPPORT_CNTL_NBIF_HST_HALT_THRESHOLD_MASK        0xf0000

// Bitfield Description : 1 means we will not disconnect automatically. we only disconnect when we see that the orig want to disconnected.. 0 means we could disconnect automatically. we could disconnect when we see that the rtn path is idle for some cycles.
#define RPB_SDPPORT_CNTL_NBIF_HST_PASSIVE_MODE_OFFSET      20
#define RPB_SDPPORT_CNTL_NBIF_HST_PASSIVE_MODE_MASK        0x100000

// Bitfield Description : 1 means we give back ack to the originitor quickly without taking cares our internal cg or busy state. 0 means we give back ack to the originitor slowly after we taking cares our internal cg or busy state.
#define RPB_SDPPORT_CNTL_NBIF_HST_QUICK_COMACK_OFFSET      21
#define RPB_SDPPORT_CNTL_NBIF_HST_QUICK_COMACK_MASK        0x200000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPCKEN_OFFSET      22
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPCKEN_MASK        0x400000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPCKENRCV_OFFSET      23
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPCKENRCV_MASK        0x800000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPDATACKEN_OFFSET      24
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPDATACKEN_MASK        0x1000000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPDATACKENRCV_OFFSET      25
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_RDRSPDATACKENRCV_MASK        0x2000000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_WRRSPCKEN_OFFSET      26
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_WRRSPCKEN_MASK        0x4000000

// Bitfield Description : sdpvdci sideband signal bw athub/df.
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_WRRSPCKENRCV_OFFSET      27
#define RPB_SDPPORT_CNTL_DF_SDPVDCI_WRRSPCKENRCV_MASK        0x8000000

// Bitfield Description : the register used to control whether we send busy with all sdp port busy, or just send out the OrigClkReq as ATHUB side. 1: means include all, 0:mean just the OrigClkReq
#define RPB_SDPPORT_CNTL_CG_BUSY_PORT_OFFSET      28
#define RPB_SDPPORT_CNTL_CG_BUSY_PORT_MASK        0x10000000

// Bitfield Description : reserved fields
#define RPB_SDPPORT_CNTL_Reserved_31_29_OFFSET      29
#define RPB_SDPPORT_CNTL_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            NBIF_DMA_SELF_ACTIVATE:1;
    UINT32                            NBIF_DMA_CFG_MODE:2;
    UINT32                            NBIF_DMA_ENABLE_REISSUE_CREDIT:1;
    UINT32                            NBIF_DMA_ENABLE_SATURATE_COUNTER:1;
    UINT32                            NBIF_DMA_ENABLE_DISRUPT_FULLDIS:1;
    UINT32                            NBIF_DMA_HALT_THRESHOLD:4;
    UINT32                            NBIF_HST_SELF_ACTIVATE:1;
    UINT32                            NBIF_HST_CFG_MODE:2;
    UINT32                            NBIF_HST_ENABLE_REISSUE_CREDIT:1;
    UINT32                            NBIF_HST_ENABLE_SATURATE_COUNTER:1;
    UINT32                            NBIF_HST_ENABLE_DISRUPT_FULLDIS:1;
    UINT32                            NBIF_HST_HALT_THRESHOLD:4;
    UINT32                            NBIF_HST_PASSIVE_MODE:1;
    UINT32                            NBIF_HST_QUICK_COMACK:1;
    UINT32                            DF_SDPVDCI_RDRSPCKEN:1;
    UINT32                            DF_SDPVDCI_RDRSPCKENRCV:1;
    UINT32                            DF_SDPVDCI_RDRSPDATACKEN:1;
    UINT32                            DF_SDPVDCI_RDRSPDATACKENRCV:1;
    UINT32                            DF_SDPVDCI_WRRSPCKEN:1;
    UINT32                            DF_SDPVDCI_WRRSPCKENRCV:1;
    UINT32                            CG_BUSY_PORT:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} RPB_SDPPORT_CNTL_STRUCT;

#define SMN_RPB_SDPPORT_CNTL_ADDRESS    0x33e0UL


/***********************************************************
* Register Name : RPB_SECLEVEL_CONF
* Register Description :
* RPB seclevel Configuration Register, psp only
* Visibility : 0x2
************************************************************/

// Bitfield Description : atc translation request/page request/invalidation completion txns seclevel field
#define RPB_SECLEVEL_CONF_ATC_SECLEVEL_OFFSET      0
#define RPB_SECLEVEL_CONF_ATC_SECLEVEL_MASK        0xf

// Bitfield Description : 
#define RPB_SECLEVEL_CONF_Reserved_31_4_OFFSET      4
#define RPB_SECLEVEL_CONF_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            ATC_SECLEVEL:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} RPB_SECLEVEL_CONF_STRUCT;

#define SMN_RPB_SECLEVEL_CONF_ADDRESS    0x33e8UL


/***********************************************************
* Register Name : RPB_SWITCH_CNTL2
* Register Description :
* this register controls the read efficiency queues
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of requests to process before switching queues
#define RPB_SWITCH_CNTL2_RD_QUEUE4_SWITCH_NUM_OFFSET      0
#define RPB_SWITCH_CNTL2_RD_QUEUE4_SWITCH_NUM_MASK        0x7f

// Bitfield Description : number of requests to process before switching queues
#define RPB_SWITCH_CNTL2_RD_QUEUE5_SWITCH_NUM_OFFSET      7
#define RPB_SWITCH_CNTL2_RD_QUEUE5_SWITCH_NUM_MASK        0x3f80

// Bitfield Description : number of requests to process before switching queues
#define RPB_SWITCH_CNTL2_WR_QUEUE4_SWITCH_NUM_OFFSET      14
#define RPB_SWITCH_CNTL2_WR_QUEUE4_SWITCH_NUM_MASK        0x1fc000

// Bitfield Description : number of requests to process before switching queues
#define RPB_SWITCH_CNTL2_WR_QUEUE5_SWITCH_NUM_OFFSET      21
#define RPB_SWITCH_CNTL2_WR_QUEUE5_SWITCH_NUM_MASK        0xfe00000

// Bitfield Description : 
#define RPB_SWITCH_CNTL2_Reserved_31_28_OFFSET      28
#define RPB_SWITCH_CNTL2_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            RD_QUEUE4_SWITCH_NUM:7;
    UINT32                            RD_QUEUE5_SWITCH_NUM:7;
    UINT32                            WR_QUEUE4_SWITCH_NUM:7;
    UINT32                            WR_QUEUE5_SWITCH_NUM:7;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} RPB_SWITCH_CNTL2_STRUCT;

#define SMN_RPB_SWITCH_CNTL2_ADDRESS    0x337cUL


/***********************************************************
* Register Name : RPB_TAG_CONF
* Register Description :
* RPB TAG allocation Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : how many outstanding read requests can be issued to nbif.
#define RPB_TAG_CONF_RPB_IO_RD_OFFSET      0
#define RPB_TAG_CONF_RPB_IO_RD_MASK        0xff

// Bitfield Description : 
#define RPB_TAG_CONF_Reserved_9_8_OFFSET      8
#define RPB_TAG_CONF_Reserved_9_8_MASK        0x300

// Bitfield Description : how many outstanding write requests can be issued to nbif.
#define RPB_TAG_CONF_RPB_IO_WR_OFFSET      10
#define RPB_TAG_CONF_RPB_IO_WR_MASK        0x3fc00

// Bitfield Description : 
#define RPB_TAG_CONF_Reserved_31_18_OFFSET      18
#define RPB_TAG_CONF_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            RPB_IO_RD:8;
    UINT32                            Reserved_9_8:2;
    UINT32                            RPB_IO_WR:8;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} RPB_TAG_CONF_STRUCT;

#define SMN_RPB_TAG_CONF_ADDRESS    0x3358UL


/***********************************************************
* Register Name : RPB_VC_SWITCH_RDWR
* Register Description :
* RPB interface Configuration Register to control inputs from sdp_ncs_intf to XPB
* Visibility : 0x2
************************************************************/

// Bitfield Description : zero means rr 1 means bandwidth 2 means totally see qospriority 3 resserved
#define RPB_VC_SWITCH_RDWR_MODE_OFFSET      0
#define RPB_VC_SWITCH_RDWR_MODE_MASK        0x3

// Bitfield Description : number of requests to process before switching to wr
#define RPB_VC_SWITCH_RDWR_NUM_RD_OFFSET      2
#define RPB_VC_SWITCH_RDWR_NUM_RD_MASK        0x3fc

// Bitfield Description : number of requests to process before switching to rd
#define RPB_VC_SWITCH_RDWR_NUM_WR_OFFSET      10
#define RPB_VC_SWITCH_RDWR_NUM_WR_MASK        0x3fc00

// Bitfield Description : max # of credits available to use on intf for sending rdreq to xpb. when rd in bypass_xpb mode, we should config the value as 64, to sync with the fifo depth of rpb_rdreq2
#define RPB_VC_SWITCH_RDWR_XPB_RDREQ_CRD_OFFSET      18
#define RPB_VC_SWITCH_RDWR_XPB_RDREQ_CRD_MASK        0x3fc0000

// Bitfield Description : reserved fields
#define RPB_VC_SWITCH_RDWR_Reserved_31_26_OFFSET      26
#define RPB_VC_SWITCH_RDWR_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            MODE:2;
    UINT32                            NUM_RD:8;
    UINT32                            NUM_WR:8;
    UINT32                            XPB_RDREQ_CRD:8;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} RPB_VC_SWITCH_RDWR_STRUCT;

#define SMN_RPB_VC_SWITCH_RDWR_ADDRESS    0x3398UL


/***********************************************************
* Register Name : RPB_WR_QUEUE_CNTL
* Register Description :
* this register tells us how to configure the wr_queue
* Visibility : 0x2
************************************************************/

// Bitfield Description : arb mode for wr queue allocation
#define RPB_WR_QUEUE_CNTL_ARB_MODE_OFFSET      0
#define RPB_WR_QUEUE_CNTL_ARB_MODE_MASK        0x1

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL_Q4_SHARED_OFFSET      1
#define RPB_WR_QUEUE_CNTL_Q4_SHARED_MASK        0x2

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL_Q5_SHARED_OFFSET      2
#define RPB_WR_QUEUE_CNTL_Q5_SHARED_MASK        0x4

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL_Q4_UNITID_EA_MODE_OFFSET      3
#define RPB_WR_QUEUE_CNTL_Q4_UNITID_EA_MODE_MASK        0x8

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL_Q5_UNITID_EA_MODE_OFFSET      4
#define RPB_WR_QUEUE_CNTL_Q5_UNITID_EA_MODE_MASK        0x10

// Bitfield Description : its valid only if the q4 are in reserved mode, in ea mode, this means the sdp port that need special treatment.in unitid mode, it means that unitid need special processing.
#define RPB_WR_QUEUE_CNTL_Q4_PATTERN_LOW_OFFSET      5
#define RPB_WR_QUEUE_CNTL_Q4_PATTERN_LOW_MASK        0x3e0

// Bitfield Description : its valid only if the q4 are in reserved mode, in ea mode, this means the sdp port that need special treatment.in unitid mode, it means that unitid need special processing.
#define RPB_WR_QUEUE_CNTL_Q4_PATTERN_HIGH_OFFSET      10
#define RPB_WR_QUEUE_CNTL_Q4_PATTERN_HIGH_MASK        0xfc00

// Bitfield Description : its valid only if the q5 are in reserved mode, in ea mode, this means the sdp port that need special treatment. in unitid mode, it means that unitid need special processing
#define RPB_WR_QUEUE_CNTL_Q5_PATTERN_LOW_OFFSET      16
#define RPB_WR_QUEUE_CNTL_Q5_PATTERN_LOW_MASK        0x1f0000

// Bitfield Description : its valid only if the q5 are in reserved mode, in ea mode, this means the sdp port that need special treatment. in unitid mode, it means that unitid need special processing
#define RPB_WR_QUEUE_CNTL_Q5_PATTERN_HIGH_OFFSET      21
#define RPB_WR_QUEUE_CNTL_Q5_PATTERN_HIGH_MASK        0x7e00000

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL_Reserved_31_27_OFFSET      27
#define RPB_WR_QUEUE_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            ARB_MODE:1;
    UINT32                            Q4_SHARED:1;
    UINT32                            Q5_SHARED:1;
    UINT32                            Q4_UNITID_EA_MODE:1;
    UINT32                            Q5_UNITID_EA_MODE:1;
    UINT32                            Q4_PATTERN_LOW:5;
    UINT32                            Q4_PATTERN_HIGH:6;
    UINT32                            Q5_PATTERN_LOW:5;
    UINT32                            Q5_PATTERN_HIGH:6;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RPB_WR_QUEUE_CNTL_STRUCT;

#define SMN_RPB_WR_QUEUE_CNTL_ADDRESS    0x33c8UL


/***********************************************************
* Register Name : RPB_WR_QUEUE_CNTL2
* Register Description :
* this register tells us how to configure the wr_queues mask pattern
* Visibility : 0x2
************************************************************/

// Bitfield Description : co-work with the q4_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_WR_QUEUE_CNTL2_Q4_PATTERN_MASK_LOW_OFFSET      0
#define RPB_WR_QUEUE_CNTL2_Q4_PATTERN_MASK_LOW_MASK        0x1f

// Bitfield Description : co-work with the q4_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_WR_QUEUE_CNTL2_Q4_PATTERN_MASK_HIGH_OFFSET      5
#define RPB_WR_QUEUE_CNTL2_Q4_PATTERN_MASK_HIGH_MASK        0x7e0

// Bitfield Description : co-work with the q5_pattern to decide the final unitid or ea port, when masked bit is 1, its dont care bits.
#define RPB_WR_QUEUE_CNTL2_Q5_PATTERN_MASK_LOW_OFFSET      11
#define RPB_WR_QUEUE_CNTL2_Q5_PATTERN_MASK_LOW_MASK        0xf800

// Bitfield Description : co-work with the q5_pattern to decide the final unitid or ea port, when masked bit is 1, it is dont care bits.
#define RPB_WR_QUEUE_CNTL2_Q5_PATTERN_MASK_HIGH_OFFSET      16
#define RPB_WR_QUEUE_CNTL2_Q5_PATTERN_MASK_HIGH_MASK        0x3f0000

// Bitfield Description : 
#define RPB_WR_QUEUE_CNTL2_Reserved_31_22_OFFSET      22
#define RPB_WR_QUEUE_CNTL2_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            Q4_PATTERN_MASK_LOW:5;
    UINT32                            Q4_PATTERN_MASK_HIGH:6;
    UINT32                            Q5_PATTERN_MASK_LOW:5;
    UINT32                            Q5_PATTERN_MASK_HIGH:6;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} RPB_WR_QUEUE_CNTL2_STRUCT;

#define SMN_RPB_WR_QUEUE_CNTL2_ADDRESS    0x33ccUL


/***********************************************************
* Register Name : RPB_WR_SWITCH_CNTL
* Register Description :
* this register controls the write efficiency queues
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of requests to process before switching queues
#define RPB_WR_SWITCH_CNTL_QUEUE0_SWITCH_NUM_OFFSET      0
#define RPB_WR_SWITCH_CNTL_QUEUE0_SWITCH_NUM_MASK        0x7f

// Bitfield Description : number of requests to process before switching queues
#define RPB_WR_SWITCH_CNTL_QUEUE1_SWITCH_NUM_OFFSET      7
#define RPB_WR_SWITCH_CNTL_QUEUE1_SWITCH_NUM_MASK        0x3f80

// Bitfield Description : number of requests to process before switching queues
#define RPB_WR_SWITCH_CNTL_QUEUE2_SWITCH_NUM_OFFSET      14
#define RPB_WR_SWITCH_CNTL_QUEUE2_SWITCH_NUM_MASK        0x1fc000

// Bitfield Description : number of requests to process before switching queues
#define RPB_WR_SWITCH_CNTL_QUEUE3_SWITCH_NUM_OFFSET      21
#define RPB_WR_SWITCH_CNTL_QUEUE3_SWITCH_NUM_MASK        0xfe00000

// Bitfield Description : switch_num_mode, 1 bandwidth mode, 0 trans num mode.
#define RPB_WR_SWITCH_CNTL_SWITCH_NUM_MODE_OFFSET      28
#define RPB_WR_SWITCH_CNTL_SWITCH_NUM_MODE_MASK        0x10000000

// Bitfield Description : enable workload adjust weighted R-R.
#define RPB_WR_SWITCH_CNTL_WORKLOAD_ADJUST_EN_OFFSET      29
#define RPB_WR_SWITCH_CNTL_WORKLOAD_ADJUST_EN_MASK        0x20000000

// Bitfield Description : weight adjusted step under workload adjust weight R-R mode (2^field)
#define RPB_WR_SWITCH_CNTL_WEIGHT_ADJUST_STEP_OFFSET      30
#define RPB_WR_SWITCH_CNTL_WEIGHT_ADJUST_STEP_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            QUEUE0_SWITCH_NUM:7;
    UINT32                            QUEUE1_SWITCH_NUM:7;
    UINT32                            QUEUE2_SWITCH_NUM:7;
    UINT32                            QUEUE3_SWITCH_NUM:7;
    UINT32                            SWITCH_NUM_MODE:1;
    UINT32                            WORKLOAD_ADJUST_EN:1;
    UINT32                            WEIGHT_ADJUST_STEP:2;
  } Field;
  UINT32 Value;
} RPB_WR_SWITCH_CNTL_STRUCT;

#define SMN_RPB_WR_SWITCH_CNTL_ADDRESS    0x3374UL

#endif /* _ATHUB_RPB_H_ */
