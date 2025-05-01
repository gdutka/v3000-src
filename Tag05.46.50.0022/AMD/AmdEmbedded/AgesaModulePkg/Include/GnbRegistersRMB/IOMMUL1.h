/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IOMMUL1_H_
#define _IOMMUL1_H_


/***********************************************************
* Register Name : IOMMU_PGSLV_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Control the number of consecutive clocks when IP is idle before asserting PGMS_PGSLV_pwrGate_ready. Large enough to account for L2->L1 ClkReq/ClkAck path with repeaters.  Cannot be smaller than 0xB.
#define IOMMU_PGSLV_CONTROL_CFG_IDLE_HYSTERESIS_OFFSET      0
#define IOMMU_PGSLV_CONTROL_CFG_IDLE_HYSTERESIS_MASK        0x1f

// Bitfield Description : SW readable register for finding the isolation status of the L1, 0 - not in isolation, 1 - in isolation (power-gated).
#define IOMMU_PGSLV_CONTROL_L1_PG_STATUS_OFFSET      5
#define IOMMU_PGSLV_CONTROL_L1_PG_STATUS_MASK        0x20

// Bitfield Description : 
#define IOMMU_PGSLV_CONTROL_Reserved_31_6_OFFSET      6
#define IOMMU_PGSLV_CONTROL_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            CFG_IDLE_HYSTERESIS:5;
    UINT32                            L1_PG_STATUS:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} IOMMU_PGSLV_CONTROL_STRUCT;

#define SMN_IOAGR_IOMMU_PGSLV_CONTROL_ADDRESS    0x148000f4UL

#define SMN_PCIE0_IOMMU_PGSLV_CONTROL_ADDRESS    0x147000f4UL

#define SMN_PCIE2_IOMMU_PGSLV_CONTROL_ADDRESS    0x149000f4UL

#define SMN_PCIE3_IOMMU_PGSLV_CONTROL_ADDRESS    0x14a000f4UL


/***********************************************************
* Register Name : L1_ATS_RESP_CTRL_0
* Register Description :
* Register used to control unsuccessful ATS response delays when PPR Auto response kicks in.
* Visibility : 0x2
************************************************************/

// Bitfield Description : ATS allow time = value x 50 ns.
#define L1_ATS_RESP_CTRL_0_L1_ATS_Resp_allow_timer_OFFSET      0
#define L1_ATS_RESP_CTRL_0_L1_ATS_Resp_allow_timer_MASK        0xff

// Bitfield Description : ATS delay time = value x 50 ns.
#define L1_ATS_RESP_CTRL_0_L1_ATS_Resp_delay_timer_OFFSET      8
#define L1_ATS_RESP_CTRL_0_L1_ATS_Resp_delay_timer_MASK        0xff00

// Bitfield Description : 
#define L1_ATS_RESP_CTRL_0_Reserved_30_16_OFFSET      16
#define L1_ATS_RESP_CTRL_0_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : Enable delaying Unsuccessful ATS responses when a PPR Auto Reponse has been returned prior to this ATS.
#define L1_ATS_RESP_CTRL_0_L1_ATSdely_on_PPRAutoResp_en_OFFSET      31
#define L1_ATS_RESP_CTRL_0_L1_ATSdely_on_PPRAutoResp_en_MASK        0x80000000

typedef union {
  struct {
    UINT32                            L1_ATS_Resp_allow_timer:8;
    UINT32                            L1_ATS_Resp_delay_timer:8;
    UINT32                            Reserved_30_16:15;
    UINT32                            L1_ATSdely_on_PPRAutoResp_en:1;
  } Field;
  UINT32 Value;
} L1_ATS_RESP_CTRL_0_STRUCT;

#define SMN_IOAGR_L1_ATS_RESP_CTRL_0_ADDRESS    0x148000f8UL

#define SMN_PCIE0_L1_ATS_RESP_CTRL_0_ADDRESS    0x147000f8UL

#define SMN_PCIE2_L1_ATS_RESP_CTRL_0_ADDRESS    0x149000f8UL

#define SMN_PCIE3_L1_ATS_RESP_CTRL_0_ADDRESS    0x14a000f8UL


/***********************************************************
* Register Name : L1_BANK_DISABLE_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the number of cache entries to disable in cache 0.
#define L1_BANK_DISABLE_0_L1CacheLineDis_0_OFFSET      0
#define L1_BANK_DISABLE_0_L1CacheLineDis_0_MASK        0x7f

// Bitfield Description : 
#define L1_BANK_DISABLE_0_Reserved_7_7_OFFSET      7
#define L1_BANK_DISABLE_0_Reserved_7_7_MASK        0x80

// Bitfield Description : Sets the number of cache entries to disable in cache 1.
#define L1_BANK_DISABLE_0_L1CacheLineDis_1_OFFSET      8
#define L1_BANK_DISABLE_0_L1CacheLineDis_1_MASK        0x7f00

// Bitfield Description : Sets the number of cache entries to disable in qos cache.
#define L1_BANK_DISABLE_0_L1CacheLineDis_qos_OFFSET      15
#define L1_BANK_DISABLE_0_L1CacheLineDis_qos_MASK        0x3f8000

// Bitfield Description : 
#define L1_BANK_DISABLE_0_Reserved_31_22_OFFSET      22
#define L1_BANK_DISABLE_0_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            L1CacheLineDis_0:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            L1CacheLineDis_1:7;
    UINT32                            L1CacheLineDis_qos:7;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} L1_BANK_DISABLE_0_STRUCT;

#define SMN_IOAGR_L1_BANK_DISABLE_0_ADDRESS    0x1480004cUL

#define SMN_PCIE0_L1_BANK_DISABLE_0_ADDRESS    0x1470004cUL

#define SMN_PCIE2_L1_BANK_DISABLE_0_ADDRESS    0x1490004cUL

#define SMN_PCIE3_L1_BANK_DISABLE_0_ADDRESS    0x14a0004cUL


/***********************************************************
* Register Name : L1_BANK_SEL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Value is used to determine the virtual address bit that selects between the 2 banks of the L1 cache (if present). The bank is selected  by bitwise ANDing this register against virtual address bits 19:12 and XORing the result.
#define L1_BANK_SEL_0_L1CacheBankSel_0_OFFSET      0
#define L1_BANK_SEL_0_L1CacheBankSel_0_MASK        0xffff

// Bitfield Description : 
#define L1_BANK_SEL_0_Reserved_31_16_OFFSET      16
#define L1_BANK_SEL_0_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            L1CacheBankSel_0:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} L1_BANK_SEL_0_STRUCT;

#define SMN_IOAGR_L1_BANK_SEL_0_ADDRESS    0x14800040UL

#define SMN_PCIE0_L1_BANK_SEL_0_ADDRESS    0x14700040UL

#define SMN_PCIE2_L1_BANK_SEL_0_ADDRESS    0x14900040UL

#define SMN_PCIE3_L1_BANK_SEL_0_ADDRESS    0x14a00040UL


/***********************************************************
* Register Name : L1_CACHE_CTRL_0
* Register Description :
* Allow Cache Bypass.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enables L1 cache bypass. One hot encoding representing 1 bit for each cache for implementations with multiple caches.
#define L1_CACHE_CTRL_0_CacheByPass_OFFSET      0
#define L1_CACHE_CTRL_0_CacheByPass_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CacheByPass:32;
  } Field;
  UINT32 Value;
} L1_CACHE_CTRL_0_STRUCT;

#define SMN_IOAGR_L1_CACHE_CTRL_0_ADDRESS    0x14800170UL

#define SMN_PCIE0_L1_CACHE_CTRL_0_ADDRESS    0x14700170UL

#define SMN_PCIE2_L1_CACHE_CTRL_0_ADDRESS    0x14900170UL

#define SMN_PCIE3_L1_CACHE_CTRL_0_ADDRESS    0x14a00170UL


/***********************************************************
* Register Name : L1_CACHE_CTRL_1
* Register Description :
* Reserved for future use.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved for future use.
#define L1_CACHE_CTRL_1_TLBMode_OFFSET      0
#define L1_CACHE_CTRL_1_TLBMode_MASK        0x7

// Bitfield Description : 
#define L1_CACHE_CTRL_1_Reserved_31_3_OFFSET      3
#define L1_CACHE_CTRL_1_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            TLBMode:3;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} L1_CACHE_CTRL_1_STRUCT;

#define SMN_IOAGR_L1_CACHE_CTRL_1_ADDRESS    0x14800174UL

#define SMN_PCIE0_L1_CACHE_CTRL_1_ADDRESS    0x14700174UL

#define SMN_PCIE2_L1_CACHE_CTRL_1_ADDRESS    0x14900174UL

#define SMN_PCIE3_L1_CACHE_CTRL_1_ADDRESS    0x14a00174UL


/***********************************************************
* Register Name : L1_CLKCNTRL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_CLKCNTRL_0_Reserved_3_0_OFFSET      0
#define L1_CLKCNTRL_0_Reserved_3_0_MASK        0xf

// Bitfield Description : Enables clock gating for internal DMA path blocks.
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET      4
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK        0x10

// Bitfield Description : Enables clock gating for internal Cache blocks.
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET      5
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK        0x20

// Bitfield Description : Enables clock gating for internal Command Process Slave blocks.
#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET      6
#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK        0x40

// Bitfield Description : 
#define L1_CLKCNTRL_0_Reserved_7_7_OFFSET      7
#define L1_CLKCNTRL_0_Reserved_7_7_MASK        0x80

// Bitfield Description : Enables clock gating for internal Performance blocks.
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET      8
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK        0x100

// Bitfield Description : Enables clock gating for internal memory blocks.
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET      9
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK        0x200

// Bitfield Description : Enables clock gating for internal register blocks.
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET      10
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK        0x400

// Bitfield Description : Enables clock gating for internal HOST path blocks.
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET      11
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK        0x800

// Bitfield Description : Enables clock gating for internal DMA Response path blocks.
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET      12
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK        0x1000

// Bitfield Description : Enables clock gating for internal HOST Response path blocks.
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET      13
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK        0x2000

// Bitfield Description : Enables clock gating for sion Performance blocks.
#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET      14
#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK        0x4000

// Bitfield Description : IOMMU L1 Clock branch Hysteresis used in all the branches - Reset to 16 clycles.
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET      15
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK        0x7f8000

// Bitfield Description : 
#define L1_CLKCNTRL_0_Reserved_30_23_OFFSET      23
#define L1_CLKCNTRL_0_Reserved_30_23_MASK        0x7f800000

// Bitfield Description : 0x0 - L1 does not wait for L2toL1ClkGrant before sending request, 0x1-L1 needs to receive L2toL1ClkGrant before sending request.  This register must be set if L2 clock gating or L2 power gating is enabled.
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET      31
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            L1_DMA_CLKGATE_EN:1;
    UINT32                            L1_CACHE_CLKGATE_EN:1;
    UINT32                            L1_CPSLV_CLKGATE_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            L1_PERF_CLKGATE_EN:1;
    UINT32                            L1_MEMORY_CLKGATE_EN:1;
    UINT32                            L1_REG_CLKGATE_EN:1;
    UINT32                            L1_HOSTREQ_CLKGATE_EN:1;
    UINT32                            L1_DMARSP_CLKGATE_EN:1;
    UINT32                            L1_HOSTRSP_CLKGATE_EN:1;
    UINT32                            L1_SION_PERF_CLKGATE_EN:1;
    UINT32                            L1_CLKGATE_HYSTERESIS:8;
    UINT32                            Reserved_30_23:8;
    UINT32                            L1_L2_CLKGATE_EN:1;
  } Field;
  UINT32 Value;
} L1_CLKCNTRL_0_STRUCT;

#define SMN_IOAGR_L1_CLKCNTRL_0_ADDRESS    0x148000ccUL

#define SMN_PCIE0_L1_CLKCNTRL_0_ADDRESS    0x147000ccUL

#define SMN_PCIE2_L1_CLKCNTRL_0_ADDRESS    0x149000ccUL

#define SMN_PCIE3_L1_CLKCNTRL_0_ADDRESS    0x14a000ccUL


/***********************************************************
* Register Name : L1_CNTRL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_CNTRL_0_Reserved_0_0_OFFSET      0
#define L1_CNTRL_0_Reserved_0_0_MASK        0x1

// Bitfield Description : Disable variable page size support in L1 cache - only 4K pages.
#define L1_CNTRL_0_Fragment_dis_OFFSET      1
#define L1_CNTRL_0_Fragment_dis_MASK        0x2

// Bitfield Description : Cache read only pages in L1.
#define L1_CNTRL_0_CacheIR_only_OFFSET      2
#define L1_CNTRL_0_CacheIR_only_MASK        0x4

// Bitfield Description : Cache write only pages in L1.
#define L1_CNTRL_0_CacheIW_only_OFFSET      3
#define L1_CNTRL_0_CacheIW_only_MASK        0x8

// Bitfield Description : Forces responses for aborted DMA requests to return in order with other DMA requests in L1.
#define L1_CNTRL_0_L1ForceOrderedAbort_OFFSET      4
#define L1_CNTRL_0_L1ForceOrderedAbort_MASK        0x10

// Bitfield Description : Controls credits for L1 to L2 interface, cannot exceed 0xa.
#define L1_CNTRL_0_L2Credits_OFFSET      5
#define L1_CNTRL_0_L2Credits_MASK        0x7e0

// Bitfield Description : Controls RT credits for L1 to L2 interface, cannot exceed 0xa.
#define L1_CNTRL_0_L2RTCredits_OFFSET      11
#define L1_CNTRL_0_L2RTCredits_MASK        0x1f800

// Bitfield Description : 
#define L1_CNTRL_0_Reserved_19_17_OFFSET      17
#define L1_CNTRL_0_Reserved_19_17_MASK        0xe0000

// Bitfield Description : Set by hardware - number of caches in L1.
#define L1_CNTRL_0_L1Banks_OFFSET      20
#define L1_CNTRL_0_L1Banks_MASK        0x700000

// Bitfield Description : 
#define L1_CNTRL_0_Reserved_23_23_OFFSET      23
#define L1_CNTRL_0_Reserved_23_23_MASK        0x800000

// Bitfield Description : Set by hardware - number of entries in each L1 cache, 2^ L1entries.
#define L1_CNTRL_0_L1Entries_OFFSET      24
#define L1_CNTRL_0_L1Entries_MASK        0xf000000

// Bitfield Description : Disable ErrEvent Detection in L1.
#define L1_CNTRL_0_L1ErrEventDetectDis_OFFSET      28
#define L1_CNTRL_0_L1ErrEventDetectDis_MASK        0x10000000

// Bitfield Description : Forces the incoming host response to have PassPW=1 behavior. 0x00 = Disabled. 0x01 = Force incoming HostRdRsp PassPW = 1. 0x10 = Force incoming HostWrRsp PassPW = 1. 0x11 = Force both HostRdRsp and HostWrRsp PassPW = 1.
#define L1_CNTRL_0_L1ForceHostRspPassPWHigh_OFFSET      29
#define L1_CNTRL_0_L1ForceHostRspPassPWHigh_MASK        0x60000000

// Bitfield Description : Disable interrupt DW check.
#define L1_CNTRL_0_L1InterruptHalfDwDis_OFFSET      31
#define L1_CNTRL_0_L1InterruptHalfDwDis_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            Fragment_dis:1;
    UINT32                            CacheIR_only:1;
    UINT32                            CacheIW_only:1;
    UINT32                            L1ForceOrderedAbort:1;
    UINT32                            L2Credits:6;
    UINT32                            L2RTCredits:6;
    UINT32                            Reserved_19_17:3;
    UINT32                            L1Banks:3;
    UINT32                            Reserved_23_23:1;
    UINT32                            L1Entries:4;
    UINT32                            L1ErrEventDetectDis:1;
    UINT32                            L1ForceHostRspPassPWHigh:2;
    UINT32                            L1InterruptHalfDwDis:1;
  } Field;
  UINT32 Value;
} L1_CNTRL_0_STRUCT;

#define SMN_IOAGR_L1_CNTRL_0_ADDRESS    0x14800030UL

#define SMN_PCIE0_L1_CNTRL_0_ADDRESS    0x14700030UL

#define SMN_PCIE2_L1_CNTRL_0_ADDRESS    0x14900030UL

#define SMN_PCIE3_L1_CNTRL_0_ADDRESS    0x14a00030UL


/***********************************************************
* Register Name : L1_CNTRL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_CNTRL_1_Reserved_3_0_OFFSET      0
#define L1_CNTRL_1_Reserved_3_0_MASK        0xf

// Bitfield Description : 0- Oldest QoS Request (VC5) always get priority over other ready requests in L1 ordering. 1 - Ready request on any VC get sent upstream in the oldest to yongest order
#define L1_CNTRL_1_QoSAlwaysPasses_dis_OFFSET      4
#define L1_CNTRL_1_QoSAlwaysPasses_dis_MASK        0x10

// Bitfield Description : Allows one non-QOS request to pass a QOS request in L1->L2 WQ arbiter ever X cycles.
#define L1_CNTRL_1_WQMISS_ARB_burst_OFFSET      5
#define L1_CNTRL_1_WQMISS_ARB_burst_MASK        0xe0

// Bitfield Description : Disable predictive TLP Rd Latency optimization.
#define L1_CNTRL_1_TLBRdPred_dis_OFFSET      8
#define L1_CNTRL_1_TLBRdPred_dis_MASK        0x100

// Bitfield Description : Wait to receive all links of a chain from the client before sending it to IOHC.
#define L1_CNTRL_1_WaitAllChainLinks_OFFSET      9
#define L1_CNTRL_1_WaitAllChainLinks_MASK        0x200

// Bitfield Description : Enables forced miss of L1 cache due to failed parity check.
#define L1_CNTRL_1_L1CacheParityEn_OFFSET      10
#define L1_CNTRL_1_L1CacheParityEn_MASK        0x400

// Bitfield Description : Controls no hardware.
#define L1_CNTRL_1_L1ParityEn_OFFSET      11
#define L1_CNTRL_1_L1ParityEn_MASK        0x800

// Bitfield Description : Disables L1 caching of DTE.
#define L1_CNTRL_1_L1DTEDis_OFFSET      12
#define L1_CNTRL_1_L1DTEDis_MASK        0x1000

// Bitfield Description : These bits are appended to bottom of WQ_EntryDis1 Register. Value indicates how many credits should be limited by the L1 to the client. This value is subtracted from the number of WQ entries to determine the number of credits available to be released.
#define L1_CNTRL_1_WQ_EntryDis_OFFSET      13
#define L1_CNTRL_1_WQ_EntryDis_MASK        0xfe000

// Bitfield Description : Disables filtering of requests to L2.
#define L1_CNTRL_1_Snd_filter_dis_OFFSET      20
#define L1_CNTRL_1_Snd_filter_dis_MASK        0x100000

// Bitfield Description : Enables strict ordering of all requests through L1. Must set to 1'b0 in IOMMU4.X.
#define L1_CNTRL_1_L1Order_en_OFFSET      21
#define L1_CNTRL_1_L1Order_en_MASK        0x200000

// Bitfield Description : Enables invalidation of entire cache when invalidation command is sent.
#define L1_CNTRL_1_L1CacheInvAllEn_OFFSET      22
#define L1_CNTRL_1_L1CacheInvAllEn_MASK        0x400000

// Bitfield Description : 00b =  ATS_tlbinv_pulse_width is used for Invalidation Timeout, else a fixed value is used.
#define L1_CNTRL_1_Select_timeout_pulse_OFFSET      23
#define L1_CNTRL_1_Select_timeout_pulse_MASK        0x3800000

// Bitfield Description : When set allows the reqid to be used in hashing between multiple L1 caches.
#define L1_CNTRL_1_L1_cache_sel_reqid_OFFSET      26
#define L1_CNTRL_1_L1_cache_sel_reqid_MASK        0x4000000

// Bitfield Description : When set causes cache updates to toggle between multiple caches.
#define L1_CNTRL_1_L1_cache_sel_interleave_OFFSET      27
#define L1_CNTRL_1_L1_cache_sel_interleave_MASK        0x8000000

// Bitfield Description : When set, VA is not used for filtering pretrans requests
#define L1_CNTRL_1_Pretrans_noVA_filterEn_OFFSET      28
#define L1_CNTRL_1_Pretrans_noVA_filterEn_MASK        0x10000000

// Bitfield Description : Enable filtering of requests on a 2M boundry instead of 4K
#define L1_CNTRL_1_UnTrans_2M_filterEn_OFFSET      29
#define L1_CNTRL_1_UnTrans_2M_filterEn_MASK        0x20000000

// Bitfield Description : Enables strict ordering of all requests within a VC (VC0/1/3/7)
#define L1_CNTRL_1_L1StrictVCOrder_En_OFFSET      30
#define L1_CNTRL_1_L1StrictVCOrder_En_MASK        0x40000000

// Bitfield Description : 0: Use incoming ReqChain bit value for ordering for only DMA Writes and Atomics
// 1: Use ReqChain bit for all DMA requests.  Use this at own risk as this behavior is not verified.
#define L1_CNTRL_1_L1DmaUseChainAll_En_OFFSET      31
#define L1_CNTRL_1_L1DmaUseChainAll_En_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            QoSAlwaysPasses_dis:1;
    UINT32                            WQMISS_ARB_burst:3;
    UINT32                            TLBRdPred_dis:1;
    UINT32                            WaitAllChainLinks:1;
    UINT32                            L1CacheParityEn:1;
    UINT32                            L1ParityEn:1;
    UINT32                            L1DTEDis:1;
    UINT32                            WQ_EntryDis:7;
    UINT32                            Snd_filter_dis:1;
    UINT32                            L1Order_en:1;
    UINT32                            L1CacheInvAllEn:1;
    UINT32                            Select_timeout_pulse:3;
    UINT32                            L1_cache_sel_reqid:1;
    UINT32                            L1_cache_sel_interleave:1;
    UINT32                            Pretrans_noVA_filterEn:1;
    UINT32                            UnTrans_2M_filterEn:1;
    UINT32                            L1StrictVCOrder_En:1;
    UINT32                            L1DmaUseChainAll_En:1;
  } Field;
  UINT32 Value;
} L1_CNTRL_1_STRUCT;

#define SMN_IOAGR_L1_CNTRL_1_ADDRESS    0x14800034UL

#define SMN_PCIE0_L1_CNTRL_1_ADDRESS    0x14700034UL

#define SMN_PCIE2_L1_CNTRL_1_ADDRESS    0x14900034UL

#define SMN_PCIE3_L1_CNTRL_1_ADDRESS    0x14a00034UL


/***********************************************************
* Register Name : L1_CNTRL_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit is not used in hardware
#define L1_CNTRL_2_L1Disable_OFFSET      0
#define L1_CNTRL_2_L1Disable_MASK        0x1

// Bitfield Description : Disable mapping of MSI to HT interrupts. This bit is used in dGPU mode
#define L1_CNTRL_2_MSI_to_HT_remap_dis_OFFSET      1
#define L1_CNTRL_2_MSI_to_HT_remap_dis_MASK        0x2

// Bitfield Description : Disable abort of ats requests when iommu is disabled.
#define L1_CNTRL_2_L1_abrt_ats_dis_OFFSET      2
#define L1_CNTRL_2_L1_abrt_ats_dis_MASK        0x4

// Bitfield Description : Enables setting RdRspDataStatus to 1 when ats macro fails parity check. RdRspDataStatus=3 when this is disabled.
#define L1_CNTRL_2_L1ATSDataErrorSignalEn_OFFSET      3
#define L1_CNTRL_2_L1ATSDataErrorSignalEn_MASK        0x8

// Bitfield Description : 
#define L1_CNTRL_2_Reserved_23_4_OFFSET      4
#define L1_CNTRL_2_Reserved_23_4_MASK        0xfffff0

// Bitfield Description : 000 = No response taken when IOMMU consumes poisoned data.
// 001 = ErrEvent issued when IOMMU consumes poisoned data.
// 010 = Target Abort issued when IOMMU consumes poisoned data.
// 011 = Master Abort issued when IOMMU consumes poisoned data.
#define L1_CNTRL_2_CPD_RESP_MODE_OFFSET      24
#define L1_CNTRL_2_CPD_RESP_MODE_MASK        0x7000000

// Bitfield Description : Enables asserting Data Error on egress sdp request when parity check fails on IOMMU non-consumed data.
#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET      27
#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK        0x8000000

// Bitfield Description : Enables asserting Data Error on egress sdp request when parity check fails on IOMMU consumed data.
#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET      28
#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK        0x10000000

// Bitfield Description : Enables parity checking in L1 for ingress SDP requests. Forces all Datapath parity options (logging and data error signaling) to be disabled if this bit is set to 0.
#define L1_CNTRL_2_L1SDPParityEn_OFFSET      29
#define L1_CNTRL_2_L1SDPParityEn_MASK        0x20000000

// Bitfield Description : These bits are appended to top of WQ_EntryDis Register. Value indicates how many credits should be limited by the L1 to the client. This value is subtracted from the number of WQ entries to determine the number of credits available to be released.
#define L1_CNTRL_2_WQ_EntryDis1_OFFSET      30
#define L1_CNTRL_2_WQ_EntryDis1_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            L1Disable:1;
    UINT32                            MSI_to_HT_remap_dis:1;
    UINT32                            L1_abrt_ats_dis:1;
    UINT32                            L1ATSDataErrorSignalEn:1;
    UINT32                            Reserved_23_4:20;
    UINT32                            CPD_RESP_MODE:3;
    UINT32                            L1NonConsumedDataErrorSignalEn:1;
    UINT32                            L1ConsumedDataErrorSignalEn:1;
    UINT32                            L1SDPParityEn:1;
    UINT32                            WQ_EntryDis1:2;
  } Field;
  UINT32 Value;
} L1_CNTRL_2_STRUCT;

#define SMN_IOAGR_L1_CNTRL_2_ADDRESS    0x14800038UL

#define SMN_PCIE0_L1_CNTRL_2_ADDRESS    0x14700038UL

#define SMN_PCIE2_L1_CNTRL_2_ADDRESS    0x14900038UL

#define SMN_PCIE3_L1_CNTRL_2_ADDRESS    0x14a00038UL


/***********************************************************
* Register Name : L1_CNTRL_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the pulse width of the ats invalidation counters.
#define L1_CNTRL_3_ATS_tlbinv_pulse_width_OFFSET      0
#define L1_CNTRL_3_ATS_tlbinv_pulse_width_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ATS_tlbinv_pulse_width:32;
  } Field;
  UINT32 Value;
} L1_CNTRL_3_STRUCT;

#define SMN_IOAGR_L1_CNTRL_3_ADDRESS    0x1480003cUL

#define SMN_PCIE0_L1_CNTRL_3_ADDRESS    0x1470003cUL

#define SMN_PCIE2_L1_CNTRL_3_ADDRESS    0x1490003cUL

#define SMN_PCIE3_L1_CNTRL_3_ADDRESS    0x14a0003cUL


/***********************************************************
* Register Name : L1_CNTRL_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Allow multiple ATS responses for a large sized ATS request.
#define L1_CNTRL_4_ATS_multiple_resp_en_OFFSET      0
#define L1_CNTRL_4_ATS_multiple_resp_en_MASK        0x1

// Bitfield Description : 
#define L1_CNTRL_4_Reserved_1_1_OFFSET      1
#define L1_CNTRL_4_Reserved_1_1_MASK        0x2

// Bitfield Description : Use external pulse for Invalidation Timeout.
#define L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET      2
#define L1_CNTRL_4_Timeout_pulse_ext_En_MASK        0x4

// Bitfield Description : 
#define L1_CNTRL_4_Reserved_22_3_OFFSET      3
#define L1_CNTRL_4_Reserved_22_3_MASK        0x7ffff8

// Bitfield Description : Enable the passing of the Memory Attributes of the PTE to the ATC in the address translation response.
#define L1_CNTRL_4_AtsRsp_send_mem_type_en_OFFSET      23
#define L1_CNTRL_4_AtsRsp_send_mem_type_en_MASK        0x800000

// Bitfield Description : Value used to comapre against the incoming UnitID to determine requests coming from Internal Graphics. And if AtsRsp_send_mem_type_en is set, L1 returns the memory attributes back to the internal graphics ATC.
#define L1_CNTRL_4_IntGfx_UnitID_Val_OFFSET      24
#define L1_CNTRL_4_IntGfx_UnitID_Val_MASK        0x7f000000

// Bitfield Description : 
#define L1_CNTRL_4_Reserved_31_31_OFFSET      31
#define L1_CNTRL_4_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ATS_multiple_resp_en:1;
    UINT32                            Reserved_1_1:1;
    UINT32                            Timeout_pulse_ext_En:1;
    UINT32                            Reserved_22_3:20;
    UINT32                            AtsRsp_send_mem_type_en:1;
    UINT32                            IntGfx_UnitID_Val:7;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} L1_CNTRL_4_STRUCT;

#define SMN_IOAGR_L1_CNTRL_4_ADDRESS    0x148000c8UL

#define SMN_PCIE0_L1_CNTRL_4_ADDRESS    0x147000c8UL

#define SMN_PCIE2_L1_CNTRL_4_ADDRESS    0x149000c8UL

#define SMN_PCIE3_L1_CNTRL_4_ADDRESS    0x14a000c8UL


/***********************************************************
* Register Name : L1_CNTRL_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_CNTRL_5_Reserved_31_0_OFFSET      0
#define L1_CNTRL_5_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} L1_CNTRL_5_STRUCT;

#define SMN_IOAGR_L1_CNTRL_5_ADDRESS    0x148000e0UL

#define SMN_PCIE0_L1_CNTRL_5_ADDRESS    0x147000e0UL

#define SMN_PCIE2_L1_CNTRL_5_ADDRESS    0x149000e0UL

#define SMN_PCIE3_L1_CNTRL_5_ADDRESS    0x14a000e0UL


/***********************************************************
* Register Name : L1_FEATURE_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bits programmed 1 remain 1 until reset. This field is not used in hardware.
#define L1_FEATURE_CNTRL_Rsv_sticky_bits0_OFFSET      0
#define L1_FEATURE_CNTRL_Rsv_sticky_bits0_MASK        0x1

// Bitfield Description : Bits programmed 1 remain 1 until reset. Sticky lock bits for the PMR permission checks.
#define L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET      1
#define L1_FEATURE_CNTRL_PMR_lock_bit_MASK        0x2

// Bitfield Description : Bits programmed 1 remain 1 until reset. Sticky lock bits for the EXE permission checks.
#define L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET      2
#define L1_FEATURE_CNTRL_EXE_lock_bit_MASK        0x4

// Bitfield Description : Bits programmed 1 remain 1 until reset. This field is not used in hardware.
#define L1_FEATURE_CNTRL_Rsv_sticky_bits7to3_OFFSET      3
#define L1_FEATURE_CNTRL_Rsv_sticky_bits7to3_MASK        0xf8

// Bitfield Description : 
#define L1_FEATURE_CNTRL_Reserved_31_8_OFFSET      8
#define L1_FEATURE_CNTRL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Rsv_sticky_bits0:1;
    UINT32                            PMR_lock_bit:1;
    UINT32                            EXE_lock_bit:1;
    UINT32                            Rsv_sticky_bits7to3:5;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} L1_FEATURE_CNTRL_STRUCT;

#define SMN_IOAGR_L1_FEATURE_CNTRL_ADDRESS    0x1480009cUL

#define SMN_PCIE0_L1_FEATURE_CNTRL_ADDRESS    0x1470009cUL

#define SMN_PCIE2_L1_FEATURE_CNTRL_ADDRESS    0x1490009cUL

#define SMN_PCIE3_L1_FEATURE_CNTRL_ADDRESS    0x14a0009cUL


/***********************************************************
* Register Name : L1_FEATURE_SUP_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_CONTROL_W[EFR_SUP_W].
#define L1_FEATURE_SUP_CNTRL_L1_EFR_SUP_OFFSET      0
#define L1_FEATURE_SUP_CNTRL_L1_EFR_SUP_MASK        0x1

// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_MMIO_CONTROL0_W[PPR_SUP_W].
#define L1_FEATURE_SUP_CNTRL_L1_PPR_SUP_OFFSET      1
#define L1_FEATURE_SUP_CNTRL_L1_PPR_SUP_MASK        0x2

// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_MMIO_CONTROL1_W[DTE_seg_W].
#define L1_FEATURE_SUP_CNTRL_L1_DTE_seg_W_OFFSET      2
#define L1_FEATURE_SUP_CNTRL_L1_DTE_seg_W_MASK        0xc

// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_MMIO_CONTROL0_W[GT_SUP_W].
#define L1_FEATURE_SUP_CNTRL_L1_GT_SUP_W_OFFSET      4
#define L1_FEATURE_SUP_CNTRL_L1_GT_SUP_W_MASK        0x10

// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_MMIO_CONTROL0_W[XT_SUP_W].
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET      5
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK        0x20

// Bitfield Description : 
#define L1_FEATURE_SUP_CNTRL_Reserved_31_6_OFFSET      6
#define L1_FEATURE_SUP_CNTRL_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            L1_EFR_SUP:1;
    UINT32                            L1_PPR_SUP:1;
    UINT32                            L1_DTE_seg_W:2;
    UINT32                            L1_GT_SUP_W:1;
    UINT32                            L1_XT_SUP_W:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} L1_FEATURE_SUP_CNTRL_STRUCT;

#define SMN_IOAGR_L1_FEATURE_SUP_CNTRL_ADDRESS    0x148000dcUL

#define SMN_PCIE0_L1_FEATURE_SUP_CNTRL_ADDRESS    0x147000dcUL

#define SMN_PCIE2_L1_FEATURE_SUP_CNTRL_ADDRESS    0x149000dcUL

#define SMN_PCIE3_L1_FEATURE_SUP_CNTRL_ADDRESS    0x14a000dcUL


/***********************************************************
* Register Name : L1_GUEST_ADDR_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit is not used in hardware.
#define L1_GUEST_ADDR_CNTRL_L1_CANONICAL_ERR_EN_OFFSET      0
#define L1_GUEST_ADDR_CNTRL_L1_CANONICAL_ERR_EN_MASK        0x1

// Bitfield Description : 
#define L1_GUEST_ADDR_CNTRL_Reserved_7_1_OFFSET      1
#define L1_GUEST_ADDR_CNTRL_Reserved_7_1_MASK        0xfe

// Bitfield Description : Mask for guest addr.
#define L1_GUEST_ADDR_CNTRL_L1_GUEST_ADDR_MSK_OFFSET      8
#define L1_GUEST_ADDR_CNTRL_L1_GUEST_ADDR_MSK_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            L1_CANONICAL_ERR_EN:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            L1_GUEST_ADDR_MSK:24;
  } Field;
  UINT32 Value;
} L1_GUEST_ADDR_CNTRL_STRUCT;

#define SMN_IOAGR_L1_GUEST_ADDR_CNTRL_ADDRESS    0x148000d8UL

#define SMN_PCIE0_L1_GUEST_ADDR_CNTRL_ADDRESS    0x147000d8UL

#define SMN_PCIE2_L1_GUEST_ADDR_CNTRL_ADDRESS    0x149000d8UL

#define SMN_PCIE3_L1_GUEST_ADDR_CNTRL_ADDRESS    0x14a000d8UL


/***********************************************************
* Register Name : L1_LATENCY_COUNTER_CNTRL
* Register Description :
* Register used to control the IOMMU translation latency counter.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enables the IOMMU translation latency counter.
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_EN_OFFSET      0
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_EN_MASK        0x1

// Bitfield Description : WO - Writing 1 resets the IOMMU translation latency counter.
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_RST_OFFSET      1
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_RST_MASK        0x2

// Bitfield Description : 1=one of the Latency Counters has maxed out and counter has halted. RO
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_HALT_OFFSET      2
#define L1_LATENCY_COUNTER_CNTRL_LAT_CNT_HALT_MASK        0x4

// Bitfield Description : 
#define L1_LATENCY_COUNTER_CNTRL_Reserved_31_3_OFFSET      3
#define L1_LATENCY_COUNTER_CNTRL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            LAT_CNT_EN:1;
    UINT32                            LAT_CNT_RST:1;
    UINT32                            LAT_CNT_HALT:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} L1_LATENCY_COUNTER_CNTRL_STRUCT;

#define SMN_IOAGR_L1_LATENCY_COUNTER_CNTRL_ADDRESS    0x14800134UL

#define SMN_PCIE0_L1_LATENCY_COUNTER_CNTRL_ADDRESS    0x14700134UL

#define SMN_PCIE2_L1_LATENCY_COUNTER_CNTRL_ADDRESS    0x14900134UL

#define SMN_PCIE3_L1_LATENCY_COUNTER_CNTRL_ADDRESS    0x14a00134UL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_10
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request exit maintain time
#define L1_PGMEM_CTRL_10_L1_SD_Exit_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_10_L1_SD_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SD_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_10_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_10_ADDRESS    0x148000bcUL

#define SMN_PCIE0_L1_PGMEM_CTRL_10_ADDRESS    0x147000bcUL

#define SMN_PCIE2_L1_PGMEM_CTRL_10_ADDRESS    0x149000bcUL

#define SMN_PCIE3_L1_PGMEM_CTRL_10_ADDRESS    0x14a000bcUL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved for future memory powergating support.
#define L1_PGMEM_CTRL_4_L1_SD_thres_OFFSET      0
#define L1_PGMEM_CTRL_4_L1_SD_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SD_thres:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_4_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_4_ADDRESS    0x148000f0UL

#define SMN_PCIE0_L1_PGMEM_CTRL_4_ADDRESS    0x147000f0UL

#define SMN_PCIE2_L1_PGMEM_CTRL_4_ADDRESS    0x149000f0UL

#define SMN_PCIE3_L1_PGMEM_CTRL_4_ADDRESS    0x14a000f0UL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request maintain time
#define L1_PGMEM_CTRL_5_L1_LS_Req_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_5_L1_LS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_LS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_5_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_5_ADDRESS    0x148000a8UL

#define SMN_PCIE0_L1_PGMEM_CTRL_5_ADDRESS    0x147000a8UL

#define SMN_PCIE2_L1_PGMEM_CTRL_5_ADDRESS    0x149000a8UL

#define SMN_PCIE3_L1_PGMEM_CTRL_5_ADDRESS    0x14a000a8UL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request exit maintain time
#define L1_PGMEM_CTRL_6_L1_LS_Exit_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_6_L1_LS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_LS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_6_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_6_ADDRESS    0x148000acUL

#define SMN_PCIE0_L1_PGMEM_CTRL_6_ADDRESS    0x147000acUL

#define SMN_PCIE2_L1_PGMEM_CTRL_6_ADDRESS    0x149000acUL

#define SMN_PCIE3_L1_PGMEM_CTRL_6_ADDRESS    0x14a000acUL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request maintain time
#define L1_PGMEM_CTRL_7_L1_DS_Req_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_7_L1_DS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_DS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_7_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_7_ADDRESS    0x148000b0UL

#define SMN_PCIE0_L1_PGMEM_CTRL_7_ADDRESS    0x147000b0UL

#define SMN_PCIE2_L1_PGMEM_CTRL_7_ADDRESS    0x149000b0UL

#define SMN_PCIE3_L1_PGMEM_CTRL_7_ADDRESS    0x14a000b0UL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_8
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request exit maintain time
#define L1_PGMEM_CTRL_8_L1_DS_Exit_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_8_L1_DS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_DS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_8_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_8_ADDRESS    0x148000b4UL

#define SMN_PCIE0_L1_PGMEM_CTRL_8_ADDRESS    0x147000b4UL

#define SMN_PCIE2_L1_PGMEM_CTRL_8_ADDRESS    0x149000b4UL

#define SMN_PCIE3_L1_PGMEM_CTRL_8_ADDRESS    0x14a000b4UL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_9
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request maintain time
#define L1_PGMEM_CTRL_9_L1_SD_Req_Maintain_Cnt_OFFSET      0
#define L1_PGMEM_CTRL_9_L1_SD_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SD_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_9_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_9_ADDRESS    0x148000b8UL

#define SMN_PCIE0_L1_PGMEM_CTRL_9_ADDRESS    0x147000b8UL

#define SMN_PCIE2_L1_PGMEM_CTRL_9_ADDRESS    0x149000b8UL

#define SMN_PCIE3_L1_PGMEM_CTRL_9_ADDRESS    0x14a000b8UL


/***********************************************************
* Register Name : L1_SB_LOCATION
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Port location of SB.
#define L1_SB_LOCATION_SBlocated_Port_OFFSET      0
#define L1_SB_LOCATION_SBlocated_Port_MASK        0xffff

// Bitfield Description : Core location of SB.
#define L1_SB_LOCATION_SBlocated_Core_OFFSET      16
#define L1_SB_LOCATION_SBlocated_Core_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SBlocated_Port:16;
    UINT32                            SBlocated_Core:16;
  } Field;
  UINT32 Value;
} L1_SB_LOCATION_STRUCT;

#define SMN_IOAGR_L1_SB_LOCATION_ADDRESS    0x14800024UL

#define SMN_PCIE0_L1_SB_LOCATION_ADDRESS    0x14700024UL

#define SMN_PCIE2_L1_SB_LOCATION_ADDRESS    0x14900024UL

#define SMN_PCIE3_L1_SB_LOCATION_ADDRESS    0x14a00024UL


/***********************************************************
* Register Name : L1_SDP_CLKREQ_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Connect the ingress OrigClkReq signal on the client SDP interface to the OrigClkReq on the L1_IOHC interface, and the L1_L2_req_clkreq to the L2.
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET      0
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK        0x1

// Bitfield Description : Connect the ingress OrigClkReq signal on the IOHC SDP interface to the OrigClkReq on the L1_CLIENT interface.
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_HOST_OFFSET      1
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_HOST_MASK        0x2

// Bitfield Description : 1=Ignore OrigClkReq signal for all SDP interfaces for the assertion of NBIO_LCLK_CTRL Deep Sleep. 0=Gate NBIO_LCLK_CTRL Deep Sleep if OrigClkReq signal is set for any SDP interface.
#define L1_SDP_CLKREQ_CNTRL_NBIO_DS_IGNORE_PCTRL_OFFSET      2
#define L1_SDP_CLKREQ_CNTRL_NBIO_DS_IGNORE_PCTRL_MASK        0x4

// Bitfield Description : 
#define L1_SDP_CLKREQ_CNTRL_Reserved_31_3_OFFSET      3
#define L1_SDP_CLKREQ_CNTRL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            HW_PG_WAKEUP_EN_DMA:1;
    UINT32                            HW_PG_WAKEUP_EN_HOST:1;
    UINT32                            NBIO_DS_IGNORE_PCTRL:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} L1_SDP_CLKREQ_CNTRL_STRUCT;

#define SMN_IOAGR_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x148000d4UL

#define SMN_PCIE0_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x147000d4UL

#define SMN_PCIE2_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x149000d4UL

#define SMN_PCIE3_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x14a000d4UL


/***********************************************************
* Register Name : L1_SDP_DMA_DATA_CREDIT_0
* Register Description :
* Register used to control the credit assignment to VC for dma data.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for dma data
#define L1_SDP_DMA_DATA_CREDIT_0_VC0_DMA_DATA_CREDIT_OFFSET      0
#define L1_SDP_DMA_DATA_CREDIT_0_VC0_DMA_DATA_CREDIT_MASK        0xff

// Bitfield Description : Credits on VC1 for dma data
#define L1_SDP_DMA_DATA_CREDIT_0_VC1_DMA_DATA_CREDIT_OFFSET      8
#define L1_SDP_DMA_DATA_CREDIT_0_VC1_DMA_DATA_CREDIT_MASK        0xff00

// Bitfield Description : Credits on VC2 for dma data
#define L1_SDP_DMA_DATA_CREDIT_0_VC2_DMA_DATA_CREDIT_OFFSET      16
#define L1_SDP_DMA_DATA_CREDIT_0_VC2_DMA_DATA_CREDIT_MASK        0xff0000

// Bitfield Description : Credits on VC3 for dma data
#define L1_SDP_DMA_DATA_CREDIT_0_VC3_DMA_DATA_CREDIT_OFFSET      24
#define L1_SDP_DMA_DATA_CREDIT_0_VC3_DMA_DATA_CREDIT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC0_DMA_DATA_CREDIT:8;
    UINT32                            VC1_DMA_DATA_CREDIT:8;
    UINT32                            VC2_DMA_DATA_CREDIT:8;
    UINT32                            VC3_DMA_DATA_CREDIT:8;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_DATA_CREDIT_0_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_DATA_CREDIT_0_ADDRESS    0x1480011cUL

#define SMN_PCIE0_L1_SDP_DMA_DATA_CREDIT_0_ADDRESS    0x1470011cUL

#define SMN_PCIE2_L1_SDP_DMA_DATA_CREDIT_0_ADDRESS    0x1490011cUL

#define SMN_PCIE3_L1_SDP_DMA_DATA_CREDIT_0_ADDRESS    0x14a0011cUL


/***********************************************************
* Register Name : L1_SDP_DMA_DATA_CREDIT_1
* Register Description :
* Register used to control the credit assignment to VC for dma data.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC4 for dma data
#define L1_SDP_DMA_DATA_CREDIT_1_VC4_DMA_DATA_CREDIT_OFFSET      0
#define L1_SDP_DMA_DATA_CREDIT_1_VC4_DMA_DATA_CREDIT_MASK        0xff

// Bitfield Description : Credits on VC5 for dma data
#define L1_SDP_DMA_DATA_CREDIT_1_VC5_DMA_DATA_CREDIT_OFFSET      8
#define L1_SDP_DMA_DATA_CREDIT_1_VC5_DMA_DATA_CREDIT_MASK        0xff00

// Bitfield Description : Credits on VC6 for dma data
#define L1_SDP_DMA_DATA_CREDIT_1_VC6_DMA_DATA_CREDIT_OFFSET      16
#define L1_SDP_DMA_DATA_CREDIT_1_VC6_DMA_DATA_CREDIT_MASK        0xff0000

// Bitfield Description : Credits on VC7 for dma data
#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_OFFSET      24
#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC4_DMA_DATA_CREDIT:8;
    UINT32                            VC5_DMA_DATA_CREDIT:8;
    UINT32                            VC6_DMA_DATA_CREDIT:8;
    UINT32                            VC7_DMA_DATA_CREDIT:8;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_DATA_CREDIT_1_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_DATA_CREDIT_1_ADDRESS    0x14800120UL

#define SMN_PCIE0_L1_SDP_DMA_DATA_CREDIT_1_ADDRESS    0x14700120UL

#define SMN_PCIE2_L1_SDP_DMA_DATA_CREDIT_1_ADDRESS    0x14900120UL

#define SMN_PCIE3_L1_SDP_DMA_DATA_CREDIT_1_ADDRESS    0x14a00120UL


/***********************************************************
* Register Name : L1_SDP_DMA_RDRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for dma read response.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC0_DMA_RDRSP_CREDIT_OFFSET      0
#define L1_SDP_DMA_RDRSP_CREDIT_VC0_DMA_RDRSP_CREDIT_MASK        0xf

// Bitfield Description : Credits on VC1 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC1_DMA_RDRSP_CREDIT_OFFSET      4
#define L1_SDP_DMA_RDRSP_CREDIT_VC1_DMA_RDRSP_CREDIT_MASK        0xf0

// Bitfield Description : Credits on VC2 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC2_DMA_RDRSP_CREDIT_OFFSET      8
#define L1_SDP_DMA_RDRSP_CREDIT_VC2_DMA_RDRSP_CREDIT_MASK        0xf00

// Bitfield Description : Credits on VC3 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC3_DMA_RDRSP_CREDIT_OFFSET      12
#define L1_SDP_DMA_RDRSP_CREDIT_VC3_DMA_RDRSP_CREDIT_MASK        0xf000

// Bitfield Description : Credits on VC4 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_MASK        0xf0000

// Bitfield Description : Credits on VC5 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC5_DMA_RDRSP_CREDIT_OFFSET      20
#define L1_SDP_DMA_RDRSP_CREDIT_VC5_DMA_RDRSP_CREDIT_MASK        0xf00000

// Bitfield Description : Credits on VC6 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC6_DMA_RDRSP_CREDIT_OFFSET      24
#define L1_SDP_DMA_RDRSP_CREDIT_VC6_DMA_RDRSP_CREDIT_MASK        0xf000000

// Bitfield Description : Credits on VC7 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC7_DMA_RDRSP_CREDIT_OFFSET      28
#define L1_SDP_DMA_RDRSP_CREDIT_VC7_DMA_RDRSP_CREDIT_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_DMA_RDRSP_CREDIT:4;
    UINT32                            VC1_DMA_RDRSP_CREDIT:4;
    UINT32                            VC2_DMA_RDRSP_CREDIT:4;
    UINT32                            VC3_DMA_RDRSP_CREDIT:4;
    UINT32                            VC4_DMA_RDRSP_CREDIT:4;
    UINT32                            VC5_DMA_RDRSP_CREDIT:4;
    UINT32                            VC6_DMA_RDRSP_CREDIT:4;
    UINT32                            VC7_DMA_RDRSP_CREDIT:4;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_RDRSP_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_RDRSP_CREDIT_ADDRESS    0x14800124UL

#define SMN_PCIE0_L1_SDP_DMA_RDRSP_CREDIT_ADDRESS    0x14700124UL

#define SMN_PCIE2_L1_SDP_DMA_RDRSP_CREDIT_ADDRESS    0x14900124UL

#define SMN_PCIE3_L1_SDP_DMA_RDRSP_CREDIT_ADDRESS    0x14a00124UL


/***********************************************************
* Register Name : L1_SDP_DMA_REQ_CREDIT_0
* Register Description :
* Register used to control the credit assignment to VC for dma req.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_0_VC0_DMA_REQ_CREDIT_OFFSET      0
#define L1_SDP_DMA_REQ_CREDIT_0_VC0_DMA_REQ_CREDIT_MASK        0xff

// Bitfield Description : Credits on VC1 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_0_VC1_DMA_REQ_CREDIT_OFFSET      8
#define L1_SDP_DMA_REQ_CREDIT_0_VC1_DMA_REQ_CREDIT_MASK        0xff00

// Bitfield Description : Credits on VC2 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_0_VC2_DMA_REQ_CREDIT_OFFSET      16
#define L1_SDP_DMA_REQ_CREDIT_0_VC2_DMA_REQ_CREDIT_MASK        0xff0000

// Bitfield Description : Credits on VC3 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_0_VC3_DMA_REQ_CREDIT_OFFSET      24
#define L1_SDP_DMA_REQ_CREDIT_0_VC3_DMA_REQ_CREDIT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC0_DMA_REQ_CREDIT:8;
    UINT32                            VC1_DMA_REQ_CREDIT:8;
    UINT32                            VC2_DMA_REQ_CREDIT:8;
    UINT32                            VC3_DMA_REQ_CREDIT:8;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_REQ_CREDIT_0_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_REQ_CREDIT_0_ADDRESS    0x14800114UL

#define SMN_PCIE0_L1_SDP_DMA_REQ_CREDIT_0_ADDRESS    0x14700114UL

#define SMN_PCIE2_L1_SDP_DMA_REQ_CREDIT_0_ADDRESS    0x14900114UL

#define SMN_PCIE3_L1_SDP_DMA_REQ_CREDIT_0_ADDRESS    0x14a00114UL


/***********************************************************
* Register Name : L1_SDP_DMA_REQ_CREDIT_1
* Register Description :
* Register used to control the credit assignment to VC for dma req.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC4 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_1_VC4_DMA_REQ_CREDIT_OFFSET      0
#define L1_SDP_DMA_REQ_CREDIT_1_VC4_DMA_REQ_CREDIT_MASK        0xff

// Bitfield Description : Credits on VC5 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_1_VC5_DMA_REQ_CREDIT_OFFSET      8
#define L1_SDP_DMA_REQ_CREDIT_1_VC5_DMA_REQ_CREDIT_MASK        0xff00

// Bitfield Description : Credits on VC6 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_1_VC6_DMA_REQ_CREDIT_OFFSET      16
#define L1_SDP_DMA_REQ_CREDIT_1_VC6_DMA_REQ_CREDIT_MASK        0xff0000

// Bitfield Description : Credits on VC7 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_OFFSET      24
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC4_DMA_REQ_CREDIT:8;
    UINT32                            VC5_DMA_REQ_CREDIT:8;
    UINT32                            VC6_DMA_REQ_CREDIT:8;
    UINT32                            VC7_DMA_REQ_CREDIT:8;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_REQ_CREDIT_1_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_REQ_CREDIT_1_ADDRESS    0x14800118UL

#define SMN_PCIE0_L1_SDP_DMA_REQ_CREDIT_1_ADDRESS    0x14700118UL

#define SMN_PCIE2_L1_SDP_DMA_REQ_CREDIT_1_ADDRESS    0x14900118UL

#define SMN_PCIE3_L1_SDP_DMA_REQ_CREDIT_1_ADDRESS    0x14a00118UL


/***********************************************************
* Register Name : L1_SDP_DMA_WRRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for dma write response.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC0_DMA_WRRSP_CREDIT_OFFSET      0
#define L1_SDP_DMA_WRRSP_CREDIT_VC0_DMA_WRRSP_CREDIT_MASK        0xf

// Bitfield Description : Credits on VC1 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC1_DMA_WRRSP_CREDIT_OFFSET      4
#define L1_SDP_DMA_WRRSP_CREDIT_VC1_DMA_WRRSP_CREDIT_MASK        0xf0

// Bitfield Description : Credits on VC2 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC2_DMA_WRRSP_CREDIT_OFFSET      8
#define L1_SDP_DMA_WRRSP_CREDIT_VC2_DMA_WRRSP_CREDIT_MASK        0xf00

// Bitfield Description : Credits on VC3 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC3_DMA_WRRSP_CREDIT_OFFSET      12
#define L1_SDP_DMA_WRRSP_CREDIT_VC3_DMA_WRRSP_CREDIT_MASK        0xf000

// Bitfield Description : Credits on VC4 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_MASK        0xf0000

// Bitfield Description : Credits on VC5 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC5_DMA_WRRSP_CREDIT_OFFSET      20
#define L1_SDP_DMA_WRRSP_CREDIT_VC5_DMA_WRRSP_CREDIT_MASK        0xf00000

// Bitfield Description : Credits on VC6 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_OFFSET      24
#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_MASK        0xf000000

// Bitfield Description : Credits on VC7 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC7_DMA_WRRSP_CREDIT_OFFSET      28
#define L1_SDP_DMA_WRRSP_CREDIT_VC7_DMA_WRRSP_CREDIT_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_DMA_WRRSP_CREDIT:4;
    UINT32                            VC1_DMA_WRRSP_CREDIT:4;
    UINT32                            VC2_DMA_WRRSP_CREDIT:4;
    UINT32                            VC3_DMA_WRRSP_CREDIT:4;
    UINT32                            VC4_DMA_WRRSP_CREDIT:4;
    UINT32                            VC5_DMA_WRRSP_CREDIT:4;
    UINT32                            VC6_DMA_WRRSP_CREDIT:4;
    UINT32                            VC7_DMA_WRRSP_CREDIT:4;
  } Field;
  UINT32 Value;
} L1_SDP_DMA_WRRSP_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_DMA_WRRSP_CREDIT_ADDRESS    0x14800128UL

#define SMN_PCIE0_L1_SDP_DMA_WRRSP_CREDIT_ADDRESS    0x14700128UL

#define SMN_PCIE2_L1_SDP_DMA_WRRSP_CREDIT_ADDRESS    0x14900128UL

#define SMN_PCIE3_L1_SDP_DMA_WRRSP_CREDIT_ADDRESS    0x14a00128UL


/***********************************************************
* Register Name : L1_SDP_HOST_DATA_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for host data.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC0_HOST_DATA_CREDIT_OFFSET      0
#define L1_SDP_HOST_DATA_CREDIT_VC0_HOST_DATA_CREDIT_MASK        0xf

// Bitfield Description : Credits on VC1 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC1_HOST_DATA_CREDIT_OFFSET      4
#define L1_SDP_HOST_DATA_CREDIT_VC1_HOST_DATA_CREDIT_MASK        0xf0

// Bitfield Description : Credits on VC2 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC2_HOST_DATA_CREDIT_OFFSET      8
#define L1_SDP_HOST_DATA_CREDIT_VC2_HOST_DATA_CREDIT_MASK        0xf00

// Bitfield Description : Credits on VC3 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC3_HOST_DATA_CREDIT_OFFSET      12
#define L1_SDP_HOST_DATA_CREDIT_VC3_HOST_DATA_CREDIT_MASK        0xf000

// Bitfield Description : Credits on VC4 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC4_HOST_DATA_CREDIT_OFFSET      16
#define L1_SDP_HOST_DATA_CREDIT_VC4_HOST_DATA_CREDIT_MASK        0xf0000

// Bitfield Description : Credits on VC5 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC5_HOST_DATA_CREDIT_OFFSET      20
#define L1_SDP_HOST_DATA_CREDIT_VC5_HOST_DATA_CREDIT_MASK        0xf00000

// Bitfield Description : Credits on VC6 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC6_HOST_DATA_CREDIT_OFFSET      24
#define L1_SDP_HOST_DATA_CREDIT_VC6_HOST_DATA_CREDIT_MASK        0xf000000

// Bitfield Description : Credits on VC7 for host data
#define L1_SDP_HOST_DATA_CREDIT_VC7_HOST_DATA_CREDIT_OFFSET      28
#define L1_SDP_HOST_DATA_CREDIT_VC7_HOST_DATA_CREDIT_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_HOST_DATA_CREDIT:4;
    UINT32                            VC1_HOST_DATA_CREDIT:4;
    UINT32                            VC2_HOST_DATA_CREDIT:4;
    UINT32                            VC3_HOST_DATA_CREDIT:4;
    UINT32                            VC4_HOST_DATA_CREDIT:4;
    UINT32                            VC5_HOST_DATA_CREDIT:4;
    UINT32                            VC6_HOST_DATA_CREDIT:4;
    UINT32                            VC7_HOST_DATA_CREDIT:4;
  } Field;
  UINT32 Value;
} L1_SDP_HOST_DATA_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_HOST_DATA_CREDIT_ADDRESS    0x14800130UL

#define SMN_PCIE0_L1_SDP_HOST_DATA_CREDIT_ADDRESS    0x14700130UL

#define SMN_PCIE2_L1_SDP_HOST_DATA_CREDIT_ADDRESS    0x14900130UL

#define SMN_PCIE3_L1_SDP_HOST_DATA_CREDIT_ADDRESS    0x14a00130UL


/***********************************************************
* Register Name : L1_SDP_HOST_RDRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for host read response.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_SDP_HOST_RDRSP_CREDIT_Reserved_31_0_OFFSET      0
#define L1_SDP_HOST_RDRSP_CREDIT_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} L1_SDP_HOST_RDRSP_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_HOST_RDRSP_CREDIT_ADDRESS    0x1480017cUL

#define SMN_PCIE0_L1_SDP_HOST_RDRSP_CREDIT_ADDRESS    0x1470017cUL

#define SMN_PCIE2_L1_SDP_HOST_RDRSP_CREDIT_ADDRESS    0x1490017cUL

#define SMN_PCIE3_L1_SDP_HOST_RDRSP_CREDIT_ADDRESS    0x14a0017cUL


/***********************************************************
* Register Name : L1_SDP_HOST_REQ_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for host request.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC0_HOST_REQ_CREDIT_OFFSET      0
#define L1_SDP_HOST_REQ_CREDIT_VC0_HOST_REQ_CREDIT_MASK        0xf

// Bitfield Description : Credits on VC1 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC1_HOST_REQ_CREDIT_OFFSET      4
#define L1_SDP_HOST_REQ_CREDIT_VC1_HOST_REQ_CREDIT_MASK        0xf0

// Bitfield Description : Credits on VC2 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC2_HOST_REQ_CREDIT_OFFSET      8
#define L1_SDP_HOST_REQ_CREDIT_VC2_HOST_REQ_CREDIT_MASK        0xf00

// Bitfield Description : Credits on VC3 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC3_HOST_REQ_CREDIT_OFFSET      12
#define L1_SDP_HOST_REQ_CREDIT_VC3_HOST_REQ_CREDIT_MASK        0xf000

// Bitfield Description : Credits on VC4 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC4_HOST_REQ_CREDIT_OFFSET      16
#define L1_SDP_HOST_REQ_CREDIT_VC4_HOST_REQ_CREDIT_MASK        0xf0000

// Bitfield Description : Credits on VC5 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC5_HOST_REQ_CREDIT_OFFSET      20
#define L1_SDP_HOST_REQ_CREDIT_VC5_HOST_REQ_CREDIT_MASK        0xf00000

// Bitfield Description : Credits on VC6 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC6_HOST_REQ_CREDIT_OFFSET      24
#define L1_SDP_HOST_REQ_CREDIT_VC6_HOST_REQ_CREDIT_MASK        0xf000000

// Bitfield Description : Credits on VC7 for host request
#define L1_SDP_HOST_REQ_CREDIT_VC7_HOST_REQ_CREDIT_OFFSET      28
#define L1_SDP_HOST_REQ_CREDIT_VC7_HOST_REQ_CREDIT_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_HOST_REQ_CREDIT:4;
    UINT32                            VC1_HOST_REQ_CREDIT:4;
    UINT32                            VC2_HOST_REQ_CREDIT:4;
    UINT32                            VC3_HOST_REQ_CREDIT:4;
    UINT32                            VC4_HOST_REQ_CREDIT:4;
    UINT32                            VC5_HOST_REQ_CREDIT:4;
    UINT32                            VC6_HOST_REQ_CREDIT:4;
    UINT32                            VC7_HOST_REQ_CREDIT:4;
  } Field;
  UINT32 Value;
} L1_SDP_HOST_REQ_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_HOST_REQ_CREDIT_ADDRESS    0x1480012cUL

#define SMN_PCIE0_L1_SDP_HOST_REQ_CREDIT_ADDRESS    0x1470012cUL

#define SMN_PCIE2_L1_SDP_HOST_REQ_CREDIT_ADDRESS    0x1490012cUL

#define SMN_PCIE3_L1_SDP_HOST_REQ_CREDIT_ADDRESS    0x14a0012cUL


/***********************************************************
* Register Name : L1_SDP_HOST_WRRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for host write response.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Credits on VC0 for host write response
#define L1_SDP_HOST_WRRSP_CREDIT_VC0_HOST_WRRSP_CREDIT_OFFSET      0
#define L1_SDP_HOST_WRRSP_CREDIT_VC0_HOST_WRRSP_CREDIT_MASK        0xf

// Bitfield Description : Credits on VC1 for host write response
#define L1_SDP_HOST_WRRSP_CREDIT_VC1_HOST_WRRSP_CREDIT_OFFSET      4
#define L1_SDP_HOST_WRRSP_CREDIT_VC1_HOST_WRRSP_CREDIT_MASK        0xf0

// Bitfield Description : 
#define L1_SDP_HOST_WRRSP_CREDIT_Reserved_31_8_OFFSET      8
#define L1_SDP_HOST_WRRSP_CREDIT_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            VC0_HOST_WRRSP_CREDIT:4;
    UINT32                            VC1_HOST_WRRSP_CREDIT:4;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} L1_SDP_HOST_WRRSP_CREDIT_STRUCT;

#define SMN_IOAGR_L1_SDP_HOST_WRRSP_CREDIT_ADDRESS    0x14800178UL

#define SMN_PCIE0_L1_SDP_HOST_WRRSP_CREDIT_ADDRESS    0x14700178UL

#define SMN_PCIE2_L1_SDP_HOST_WRRSP_CREDIT_ADDRESS    0x14900178UL

#define SMN_PCIE3_L1_SDP_HOST_WRRSP_CREDIT_ADDRESS    0x14a00178UL


/***********************************************************
* Register Name : L1_SDP_MAXCRED_0
* Register Description :
* Register used to set the max credits on the L1 SDP interfaces.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Max credits to be released for l1 flush response. Applies to VC0, VC1, VC7.
#define L1_SDP_MAXCRED_0_L1_FLUSHRSP_MAXCRED_OFFSET      0
#define L1_SDP_MAXCRED_0_L1_FLUSHRSP_MAXCRED_MASK        0xf

// Bitfield Description : Value indicates how many credits should be limited by the L1 to the client. This value is subtracted from the total number of host response entries to determine the number of credits available to be released.
#define L1_SDP_MAXCRED_0_L1_HOSTRDRSP_MAXCRED_OFFSET      4
#define L1_SDP_MAXCRED_0_L1_HOSTRDRSP_MAXCRED_MASK        0x3f0

// Bitfield Description : Value indicates how many credits should be limited by the L1 to the client. This value is subtracted from the total number of host response entries to determine the number of credits available to be released.
#define L1_SDP_MAXCRED_0_L1_HOSTWRRSP_MAXCRED_OFFSET      10
#define L1_SDP_MAXCRED_0_L1_HOSTWRRSP_MAXCRED_MASK        0xfc00

// Bitfield Description : 
#define L1_SDP_MAXCRED_0_Reserved_30_16_OFFSET      16
#define L1_SDP_MAXCRED_0_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : Programing to 1 enables IOMMU credit count blocks to dynamicly change number of reserved credits per VC without presense of traffic.
#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET      31
#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            L1_FLUSHRSP_MAXCRED:4;
    UINT32                            L1_HOSTRDRSP_MAXCRED:6;
    UINT32                            L1_HOSTWRRSP_MAXCRED:6;
    UINT32                            Reserved_30_16:15;
    UINT32                            L1_DYNAMIC_CRED_RELOCATION_EN:1;
  } Field;
  UINT32 Value;
} L1_SDP_MAXCRED_0_STRUCT;

#define SMN_IOAGR_L1_SDP_MAXCRED_0_ADDRESS    0x1480010cUL

#define SMN_PCIE0_L1_SDP_MAXCRED_0_ADDRESS    0x1470010cUL

#define SMN_PCIE2_L1_SDP_MAXCRED_0_ADDRESS    0x1490010cUL

#define SMN_PCIE3_L1_SDP_MAXCRED_0_ADDRESS    0x14a0010cUL


/***********************************************************
* Register Name : L1_SION_PERF_CNT_CNTL0_HI
* Register Description :
* SION Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_HI_Reserved_0_0_OFFSET      0
#define L1_SION_PERF_CNT_CNTL0_HI_Reserved_0_0_MASK        0x1

// Bitfield Description : Write 1 to load the counter shadow registers. Read back value has no meaning.
#define L1_SION_PERF_CNT_CNTL0_HI_L1_SION_SHADOW_WR_OFFSET      1
#define L1_SION_PERF_CNT_CNTL0_HI_L1_SION_SHADOW_WR_MASK        0x2

// Bitfield Description : Write 1 for Global RESET of ALL counters. Read back value has no meaning.
#define L1_SION_PERF_CNT_CNTL0_HI_L1_SION_PERF_RESET_OFFSET      2
#define L1_SION_PERF_CNT_CNTL0_HI_L1_SION_PERF_RESET_MASK        0x4

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_HI_Reserved_7_3_OFFSET      3
#define L1_SION_PERF_CNT_CNTL0_HI_Reserved_7_3_MASK        0xf8

typedef union {
  struct {
    UINT8                            Reserved_0_0:1;
    UINT8                            L1_SION_SHADOW_WR:1;
    UINT8                            L1_SION_PERF_RESET:1;
    UINT8                            Reserved_7_3:5;
  } Field;
  UINT8 Value;
} L1_SION_PERF_CNT_CNTL0_HI_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_CNT_CNTL0_HI_ADDRESS    0x1480014bUL

#define SMN_PCIE0_L1_SION_PERF_CNT_CNTL0_HI_ADDRESS    0x1470014bUL

#define SMN_PCIE2_L1_SION_PERF_CNT_CNTL0_HI_ADDRESS    0x1490014bUL

#define SMN_PCIE3_L1_SION_PERF_CNT_CNTL0_HI_ADDRESS    0x14a0014bUL


/***********************************************************
* Register Name : L1_SION_PERF_CNT_CNTL0_LO
* Register Description :
* SION Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Performance Counters.
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_CNT_EN_OFFSET      0
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_CNT_EN_MASK        0x1

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_7_1_OFFSET      1
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_7_1_MASK        0xfe

// Bitfield Description : Programmable Pulse width for Global Shadow Write Toggle.
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_SHADOW_DELAY_OFFSET      8
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_SHADOW_DELAY_MASK        0xf00

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_14_12_OFFSET      12
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_14_12_MASK        0x7000

// Bitfield Description : Enables the Programmable Pulse with for Global Shadow Write Toggle.
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_SHADOW_DELAY_EN_OFFSET      15
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_SHADOW_DELAY_EN_MASK        0x8000

// Bitfield Description : Programmable Pulse width for Global Perf Reset Toggle.
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_PERF_RESET_DELAY_OFFSET      16
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_PERF_RESET_DELAY_MASK        0xf0000

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_22_20_OFFSET      20
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_22_20_MASK        0x700000

// Bitfield Description : Enables the Programmable Pulse width for Global Perf Reset Toggle.
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_PERF_RESET_DELAY_EN_OFFSET      23
#define L1_SION_PERF_CNT_CNTL0_LO_L1_SION_PERF_RESET_DELAY_EN_MASK        0x800000

// Bitfield Description : 
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_31_24_OFFSET      24
#define L1_SION_PERF_CNT_CNTL0_LO_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_CNT_EN:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            L1_SION_SHADOW_DELAY:4;
    UINT32                            Reserved_14_12:3;
    UINT32                            L1_SION_SHADOW_DELAY_EN:1;
    UINT32                            L1_SION_PERF_RESET_DELAY:4;
    UINT32                            Reserved_22_20:3;
    UINT32                            L1_SION_PERF_RESET_DELAY_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_CNT_CNTL0_LO_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_CNT_CNTL0_LO_ADDRESS    0x14800148UL

#define SMN_PCIE0_L1_SION_PERF_CNT_CNTL0_LO_ADDRESS    0x14700148UL

#define SMN_PCIE2_L1_SION_PERF_CNT_CNTL0_LO_ADDRESS    0x14900148UL

#define SMN_PCIE3_L1_SION_PERF_CNT_CNTL0_LO_ADDRESS    0x14a00148UL


/***********************************************************
* Register Name : L1_SION_PERF_CNT_CNTL1
* Register Description :
* L1 SION Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT0_SEL_OFFSET      0
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT1_SEL_OFFSET      8
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT2_SEL_OFFSET      16
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT3_SEL_OFFSET      24
#define L1_SION_PERF_CNT_CNTL1_L1_SION_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_EVENT0_SEL:8;
    UINT32                            L1_SION_EVENT1_SEL:8;
    UINT32                            L1_SION_EVENT2_SEL:8;
    UINT32                            L1_SION_EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_CNT_CNTL1_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_CNT_CNTL1_ADDRESS    0x1480014cUL

#define SMN_PCIE0_L1_SION_PERF_CNT_CNTL1_ADDRESS    0x1470014cUL

#define SMN_PCIE2_L1_SION_PERF_CNT_CNTL1_ADDRESS    0x1490014cUL

#define SMN_PCIE3_L1_SION_PERF_CNT_CNTL1_ADDRESS    0x14a0014cUL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT0
* Register Description :
* L1 SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define L1_SION_PERF_COUNT0_L1_SION_COUNTER0_OFFSET      0
#define L1_SION_PERF_COUNT0_L1_SION_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER0:32;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT0_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT0_ADDRESS    0x14800150UL

#define SMN_PCIE0_L1_SION_PERF_COUNT0_ADDRESS    0x14700150UL

#define SMN_PCIE2_L1_SION_PERF_COUNT0_ADDRESS    0x14900150UL

#define SMN_PCIE3_L1_SION_PERF_COUNT0_ADDRESS    0x14a00150UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT0_UPPER
* Register Description :
* L1 SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define L1_SION_PERF_COUNT0_UPPER_L1_SION_COUNTER0_UPPER_OFFSET      0
#define L1_SION_PERF_COUNT0_UPPER_L1_SION_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define L1_SION_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define L1_SION_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT0_UPPER_ADDRESS    0x14800154UL

#define SMN_PCIE0_L1_SION_PERF_COUNT0_UPPER_ADDRESS    0x14700154UL

#define SMN_PCIE2_L1_SION_PERF_COUNT0_UPPER_ADDRESS    0x14900154UL

#define SMN_PCIE3_L1_SION_PERF_COUNT0_UPPER_ADDRESS    0x14a00154UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT1
* Register Description :
* L1 SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define L1_SION_PERF_COUNT1_L1_SION_COUNTER1_OFFSET      0
#define L1_SION_PERF_COUNT1_L1_SION_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER1:32;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT1_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT1_ADDRESS    0x14800158UL

#define SMN_PCIE0_L1_SION_PERF_COUNT1_ADDRESS    0x14700158UL

#define SMN_PCIE2_L1_SION_PERF_COUNT1_ADDRESS    0x14900158UL

#define SMN_PCIE3_L1_SION_PERF_COUNT1_ADDRESS    0x14a00158UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT1_UPPER
* Register Description :
* L1 SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define L1_SION_PERF_COUNT1_UPPER_L1_SION_COUNTER1_UPPER_OFFSET      0
#define L1_SION_PERF_COUNT1_UPPER_L1_SION_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define L1_SION_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define L1_SION_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT1_UPPER_ADDRESS    0x1480015cUL

#define SMN_PCIE0_L1_SION_PERF_COUNT1_UPPER_ADDRESS    0x1470015cUL

#define SMN_PCIE2_L1_SION_PERF_COUNT1_UPPER_ADDRESS    0x1490015cUL

#define SMN_PCIE3_L1_SION_PERF_COUNT1_UPPER_ADDRESS    0x14a0015cUL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT2
* Register Description :
* L1 SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define L1_SION_PERF_COUNT2_L1_SION_COUNTER2_OFFSET      0
#define L1_SION_PERF_COUNT2_L1_SION_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER2:32;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT2_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT2_ADDRESS    0x14800160UL

#define SMN_PCIE0_L1_SION_PERF_COUNT2_ADDRESS    0x14700160UL

#define SMN_PCIE2_L1_SION_PERF_COUNT2_ADDRESS    0x14900160UL

#define SMN_PCIE3_L1_SION_PERF_COUNT2_ADDRESS    0x14a00160UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT2_UPPER
* Register Description :
* L1 SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define L1_SION_PERF_COUNT2_UPPER_L1_SION_COUNTER2_UPPER_OFFSET      0
#define L1_SION_PERF_COUNT2_UPPER_L1_SION_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define L1_SION_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define L1_SION_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT2_UPPER_ADDRESS    0x14800164UL

#define SMN_PCIE0_L1_SION_PERF_COUNT2_UPPER_ADDRESS    0x14700164UL

#define SMN_PCIE2_L1_SION_PERF_COUNT2_UPPER_ADDRESS    0x14900164UL

#define SMN_PCIE3_L1_SION_PERF_COUNT2_UPPER_ADDRESS    0x14a00164UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT3
* Register Description :
* L1 SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define L1_SION_PERF_COUNT3_L1_SION_COUNTER3_OFFSET      0
#define L1_SION_PERF_COUNT3_L1_SION_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER3:32;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT3_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT3_ADDRESS    0x14800168UL

#define SMN_PCIE0_L1_SION_PERF_COUNT3_ADDRESS    0x14700168UL

#define SMN_PCIE2_L1_SION_PERF_COUNT3_ADDRESS    0x14900168UL

#define SMN_PCIE3_L1_SION_PERF_COUNT3_ADDRESS    0x14a00168UL


/***********************************************************
* Register Name : L1_SION_PERF_COUNT3_UPPER
* Register Description :
* L1 SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define L1_SION_PERF_COUNT3_UPPER_L1_SION_COUNTER3_UPPER_OFFSET      0
#define L1_SION_PERF_COUNT3_UPPER_L1_SION_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define L1_SION_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define L1_SION_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L1_SION_COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L1_SION_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOAGR_L1_SION_PERF_COUNT3_UPPER_ADDRESS    0x1480016cUL

#define SMN_PCIE0_L1_SION_PERF_COUNT3_UPPER_ADDRESS    0x1470016cUL

#define SMN_PCIE2_L1_SION_PERF_COUNT3_UPPER_ADDRESS    0x1490016cUL

#define SMN_PCIE3_L1_SION_PERF_COUNT3_UPPER_ADDRESS    0x14a0016cUL


/***********************************************************
* Register Name : L1_TOTAL_REQUESTS_LOWER
* Register Description :
* Register used to track the total number of requests for the IOMMU translation latency counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of requests sent through WQs
#define L1_TOTAL_REQUESTS_LOWER_L1_TOTAL_REQUESTS_LOWER_OFFSET      0
#define L1_TOTAL_REQUESTS_LOWER_L1_TOTAL_REQUESTS_LOWER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_TOTAL_REQUESTS_LOWER:32;
  } Field;
  UINT32 Value;
} L1_TOTAL_REQUESTS_LOWER_STRUCT;

#define SMN_IOAGR_L1_TOTAL_REQUESTS_LOWER_ADDRESS    0x14800140UL

#define SMN_PCIE0_L1_TOTAL_REQUESTS_LOWER_ADDRESS    0x14700140UL

#define SMN_PCIE2_L1_TOTAL_REQUESTS_LOWER_ADDRESS    0x14900140UL

#define SMN_PCIE3_L1_TOTAL_REQUESTS_LOWER_ADDRESS    0x14a00140UL


/***********************************************************
* Register Name : L1_TOTAL_REQUESTS_UPPER
* Register Description :
* Register used to track the total wait time for the IOMMU translation latency counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of requests sent through WQs
#define L1_TOTAL_REQUESTS_UPPER_L1_TOTAL_REQUESTS_UPPER_OFFSET      0
#define L1_TOTAL_REQUESTS_UPPER_L1_TOTAL_REQUESTS_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_TOTAL_REQUESTS_UPPER:32;
  } Field;
  UINT32 Value;
} L1_TOTAL_REQUESTS_UPPER_STRUCT;

#define SMN_IOAGR_L1_TOTAL_REQUESTS_UPPER_ADDRESS    0x14800144UL

#define SMN_PCIE0_L1_TOTAL_REQUESTS_UPPER_ADDRESS    0x14700144UL

#define SMN_PCIE2_L1_TOTAL_REQUESTS_UPPER_ADDRESS    0x14900144UL

#define SMN_PCIE3_L1_TOTAL_REQUESTS_UPPER_ADDRESS    0x14a00144UL


/***********************************************************
* Register Name : L1_TOTAL_WAIT_TIME_LOWER
* Register Description :
* Register used to track the total wait time for the IOMMU translation latency counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of clocks WQs are in non-idle state
#define L1_TOTAL_WAIT_TIME_LOWER_L1_TOTAL_WAIT_TIME_LOWER_OFFSET      0
#define L1_TOTAL_WAIT_TIME_LOWER_L1_TOTAL_WAIT_TIME_LOWER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_TOTAL_WAIT_TIME_LOWER:32;
  } Field;
  UINT32 Value;
} L1_TOTAL_WAIT_TIME_LOWER_STRUCT;

#define SMN_IOAGR_L1_TOTAL_WAIT_TIME_LOWER_ADDRESS    0x14800138UL

#define SMN_PCIE0_L1_TOTAL_WAIT_TIME_LOWER_ADDRESS    0x14700138UL

#define SMN_PCIE2_L1_TOTAL_WAIT_TIME_LOWER_ADDRESS    0x14900138UL

#define SMN_PCIE3_L1_TOTAL_WAIT_TIME_LOWER_ADDRESS    0x14a00138UL


/***********************************************************
* Register Name : L1_TOTAL_WAIT_TIME_UPPER
* Register Description :
* Register used to track the total wait time for the IOMMU translation latency counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : number of clocks WQs are in non-idle state
#define L1_TOTAL_WAIT_TIME_UPPER_L1_TOTAL_WAIT_TIME_UPPER_OFFSET      0
#define L1_TOTAL_WAIT_TIME_UPPER_L1_TOTAL_WAIT_TIME_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L1_TOTAL_WAIT_TIME_UPPER:32;
  } Field;
  UINT32 Value;
} L1_TOTAL_WAIT_TIME_UPPER_STRUCT;

#define SMN_IOAGR_L1_TOTAL_WAIT_TIME_UPPER_ADDRESS    0x1480013cUL

#define SMN_PCIE0_L1_TOTAL_WAIT_TIME_UPPER_ADDRESS    0x1470013cUL

#define SMN_PCIE2_L1_TOTAL_WAIT_TIME_UPPER_ADDRESS    0x1490013cUL

#define SMN_PCIE3_L1_TOTAL_WAIT_TIME_UPPER_ADDRESS    0x14a0013cUL

#endif /* _IOMMUL1_H_ */
