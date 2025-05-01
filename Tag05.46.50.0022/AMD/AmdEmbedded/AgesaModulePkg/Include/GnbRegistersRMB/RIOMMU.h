/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _RIOMMU_H_
#define _RIOMMU_H_


/***********************************************************
* Register Name : RIOMMU_CK_GATE_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable the gating of the DMA clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGateDMADis_OFFSET      0
#define RIOMMU_CK_GATE_CONTROL_CKGateDMADis_MASK        0x1

// Bitfield Description : Disable the gating of the Performance clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGatePerfDis_OFFSET      1
#define RIOMMU_CK_GATE_CONTROL_CKGatePerfDis_MASK        0x2

// Bitfield Description : Disable the gating of the Command Processor clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGateCPDis_OFFSET      2
#define RIOMMU_CK_GATE_CONTROL_CKGateCPDis_MASK        0x4

// Bitfield Description : Disable the gating of the Registers clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGateRegDis_OFFSET      3
#define RIOMMU_CK_GATE_CONTROL_CKGateRegDis_MASK        0x8

// Bitfield Description : Disable the gating of the Main IOMMU RSMUMaster clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGateRSMUMasterDis_OFFSET      4
#define RIOMMU_CK_GATE_CONTROL_CKGateRSMUMasterDis_MASK        0x10

// Bitfield Description : Disable the gating of the PTC Valid clock branch.
#define RIOMMU_CK_GATE_CONTROL_CKGatePTCDis_OFFSET      5
#define RIOMMU_CK_GATE_CONTROL_CKGatePTCDis_MASK        0x20

// Bitfield Description : 
#define RIOMMU_CK_GATE_CONTROL_Reserved_11_6_OFFSET      6
#define RIOMMU_CK_GATE_CONTROL_Reserved_11_6_MASK        0xfc0

// Bitfield Description : 0=allow 128 clock cycles delay before stopping the clocks when idle asserts
// 1=allow 256 clock cycles delay before stopping the clocks when idle asserts
// 2=allow 512 clock cycles delay before stopping the clocks when idle asserts
// 3=allow 1024 clock cycles delay before stopping the clocks when idle asserts
#define RIOMMU_CK_GATE_CONTROL_CKGateLength_OFFSET      12
#define RIOMMU_CK_GATE_CONTROL_CKGateLength_MASK        0x3000

// Bitfield Description : 0=allow 2 clock cycles delay before stopping the clocks when clkready deasserts
// 1=allow 4 clock cycles delay before stopping the clocks when clkready deasserts
// 2=allow 8 clock cycles delay before stopping the clocks when clkready deasserts
// 3=allow 16 clock cycles delay before stopping the clocks when clkready deasserts
#define RIOMMU_CK_GATE_CONTROL_CKGateStop_OFFSET      14
#define RIOMMU_CK_GATE_CONTROL_CKGateStop_MASK        0xc000

// Bitfield Description : 
#define RIOMMU_CK_GATE_CONTROL_Reserved_31_16_OFFSET      16
#define RIOMMU_CK_GATE_CONTROL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            CKGateDMADis:1;
    UINT32                            CKGatePerfDis:1;
    UINT32                            CKGateCPDis:1;
    UINT32                            CKGateRegDis:1;
    UINT32                            CKGateRSMUMasterDis:1;
    UINT32                            CKGatePTCDis:1;
    UINT32                            Reserved_11_6:6;
    UINT32                            CKGateLength:2;
    UINT32                            CKGateStop:2;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_CK_GATE_CONTROL_STRUCT;

#define SMN_RIOMMU_CK_GATE_CONTROL_ADDRESS    0x16b0a0fcUL


/***********************************************************
* Register Name : RIOMMU_CONNECT_ACK
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : RSMU Connection acknowledgement from main IOMMU.
#define RIOMMU_CONNECT_ACK_CONNECT_ACK_OFFSET      0
#define RIOMMU_CONNECT_ACK_CONNECT_ACK_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONNECT_ACK:32;
  } Field;
  UINT32 Value;
} RIOMMU_CONNECT_ACK_STRUCT;

#define SMN_RIOMMU_CONNECT_ACK_ADDRESS    0x16b0a040UL


/***********************************************************
* Register Name : RIOMMU_DEV_FUNC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Device ID and the Function ID of the request.
#define RIOMMU_DEV_FUNC_dev_func_OFFSET      0
#define RIOMMU_DEV_FUNC_dev_func_MASK        0xff

// Bitfield Description : 
#define RIOMMU_DEV_FUNC_Reserved_31_8_OFFSET      8
#define RIOMMU_DEV_FUNC_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            dev_func:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} RIOMMU_DEV_FUNC_STRUCT;

#define SMN_RIOMMU_DEV_FUNC_ADDRESS    0x16b0a0f8UL


/***********************************************************
* Register Name : RIOMMU_ERR_RULE_CONTROL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is write-once. Setting this register bit locks the error detection rule set in ERRRuleDisable0/1/2.
#define RIOMMU_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET      0
#define RIOMMU_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK        0x1

// Bitfield Description : 
#define RIOMMU_ERR_RULE_CONTROL_0_Reserved_2_1_OFFSET      1
#define RIOMMU_ERR_RULE_CONTROL_0_Reserved_2_1_MASK        0x6

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define RIOMMU_ERR_RULE_CONTROL_0_ERRRuleDisable0_OFFSET      3
#define RIOMMU_ERR_RULE_CONTROL_0_ERRRuleDisable0_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            ERRRuleLock0:1;
    UINT32                            Reserved_2_1:2;
    UINT32                            ERRRuleDisable0:29;
  } Field;
  UINT32 Value;
} RIOMMU_ERR_RULE_CONTROL_0_STRUCT;

#define SMN_RIOMMU_ERR_RULE_CONTROL_0_ADDRESS    0x16b0a0e0UL


/***********************************************************
* Register Name : RIOMMU_ERR_RULE_CONTROL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define RIOMMU_ERR_RULE_CONTROL_1_ERRRuleDisable1_OFFSET      0
#define RIOMMU_ERR_RULE_CONTROL_1_ERRRuleDisable1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable1:32;
  } Field;
  UINT32 Value;
} RIOMMU_ERR_RULE_CONTROL_1_STRUCT;

#define SMN_RIOMMU_ERR_RULE_CONTROL_1_ADDRESS    0x16b0a0e4UL


/***********************************************************
* Register Name : RIOMMU_ERR_RULE_CONTROL_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define RIOMMU_ERR_RULE_CONTROL_2_ERRRuleDisable2_OFFSET      0
#define RIOMMU_ERR_RULE_CONTROL_2_ERRRuleDisable2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable2:32;
  } Field;
  UINT32 Value;
} RIOMMU_ERR_RULE_CONTROL_2_STRUCT;

#define SMN_RIOMMU_ERR_RULE_CONTROL_2_ADDRESS    0x16b0a0e8UL


/***********************************************************
* Register Name : RIOMMU_EVLOG_BUFFER_OVERFLOW
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=riommu's internal eventlog buffer has overflown. SW clear by writing 1'b0.
#define RIOMMU_EVLOG_BUFFER_OVERFLOW_EVLOG_BUFFER_OVERFLOW_OFFSET      0
#define RIOMMU_EVLOG_BUFFER_OVERFLOW_EVLOG_BUFFER_OVERFLOW_MASK        0x1

// Bitfield Description : 
#define RIOMMU_EVLOG_BUFFER_OVERFLOW_Reserved_31_1_OFFSET      1
#define RIOMMU_EVLOG_BUFFER_OVERFLOW_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            EVLOG_BUFFER_OVERFLOW:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} RIOMMU_EVLOG_BUFFER_OVERFLOW_STRUCT;

#define SMN_RIOMMU_EVLOG_BUFFER_OVERFLOW_ADDRESS    0x16b0a084UL


/***********************************************************
* Register Name : RIOMMU_EVLOG_CRED_REL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Eventlogger credit release from main IOMMU.
#define RIOMMU_EVLOG_CRED_REL_EVLOG_CRED_REL_OFFSET      0
#define RIOMMU_EVLOG_CRED_REL_EVLOG_CRED_REL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            EVLOG_CRED_REL:32;
  } Field;
  UINT32 Value;
} RIOMMU_EVLOG_CRED_REL_STRUCT;

#define SMN_RIOMMU_EVLOG_CRED_REL_ADDRESS    0x16b0a080UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 0 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE0_INV_PHASE0_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE0_INV_PHASE0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            INV_PHASE0:32;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE0_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE0_ADDRESS    0x16b0a000UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 1 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE1_INV_PHASE1_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE1_INV_PHASE1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            INV_PHASE1:32;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE1_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE1_ADDRESS    0x16b0a004UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 2 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE2_INV_PHASE2_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE2_INV_PHASE2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            INV_PHASE2:32;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE2_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE2_ADDRESS    0x16b0a008UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 3 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE3_INV_PHASE3_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE3_INV_PHASE3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            INV_PHASE3:32;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE3_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE3_ADDRESS    0x16b0a00cUL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 4 of invalidation being forwarded from main IOMMU. This phase contains the ITAG encoding.
#define RIOMMU_INVALIDATION_PHASE4_INV_PHASE4_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE4_INV_PHASE4_MASK        0x1f

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE4_Reserved_31_5_OFFSET      5
#define RIOMMU_INVALIDATION_PHASE4_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            INV_PHASE4:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE4_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE4_ADDRESS    0x16b0a010UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 5 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE5_INV_PHASE5_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE5_INV_PHASE5_MASK        0xffff

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE5_Reserved_31_16_OFFSET      16
#define RIOMMU_INVALIDATION_PHASE5_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            INV_PHASE5:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE5_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE5_ADDRESS    0x16b0a014UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 6 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE6_INV_PHASE6_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE6_INV_PHASE6_MASK        0xffff

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE6_Reserved_31_16_OFFSET      16
#define RIOMMU_INVALIDATION_PHASE6_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            INV_PHASE6:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE6_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE6_ADDRESS    0x16b0a018UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 7 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE7_INV_PHASE7_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE7_INV_PHASE7_MASK        0xffff

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE7_Reserved_31_16_OFFSET      16
#define RIOMMU_INVALIDATION_PHASE7_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            INV_PHASE7:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE7_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE7_ADDRESS    0x16b0a01cUL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE8
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 8 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE8_INV_PHASE8_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE8_INV_PHASE8_MASK        0xffff

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE8_Reserved_31_16_OFFSET      16
#define RIOMMU_INVALIDATION_PHASE8_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            INV_PHASE8:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE8_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE8_ADDRESS    0x16b0a020UL


/***********************************************************
* Register Name : RIOMMU_INVALIDATION_PHASE9
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Phase 9 of invalidation being forwarded from main IOMMU.
#define RIOMMU_INVALIDATION_PHASE9_INV_PHASE9_OFFSET      0
#define RIOMMU_INVALIDATION_PHASE9_INV_PHASE9_MASK        0xffff

// Bitfield Description : 
#define RIOMMU_INVALIDATION_PHASE9_Reserved_31_16_OFFSET      16
#define RIOMMU_INVALIDATION_PHASE9_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            INV_PHASE9:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_INVALIDATION_PHASE9_STRUCT;

#define SMN_RIOMMU_INVALIDATION_PHASE9_ADDRESS    0x16b0a07cUL


/***********************************************************
* Register Name : RIOMMU_MISC_CNTRL_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define RIOMMU_MISC_CNTRL_3_Reserved_2_0_OFFSET      0
#define RIOMMU_MISC_CNTRL_3_Reserved_2_0_MASK        0x7

// Bitfield Description : This field specifies the invalidation time-out for invalidation completion waits. 001b= 50 us, 010b=1 ms, 011b=10 ms, 100b=100 ms, 101b=1 sec, 110b/111b=10 sec. 000b disables the time-out counter
#define RIOMMU_MISC_CNTRL_3_INV_TIMEOUT_OFFSET      3
#define RIOMMU_MISC_CNTRL_3_INV_TIMEOUT_MASK        0x38

// Bitfield Description : 
#define RIOMMU_MISC_CNTRL_3_Reserved_11_6_OFFSET      6
#define RIOMMU_MISC_CNTRL_3_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Disable error on DTE access with reserve bits set ilegal value.
#define RIOMMU_MISC_CNTRL_3_DTEResvBitChkDis_OFFSET      12
#define RIOMMU_MISC_CNTRL_3_DTEResvBitChkDis_MASK        0x1000

// Bitfield Description : 
#define RIOMMU_MISC_CNTRL_3_Reserved_31_13_OFFSET      13
#define RIOMMU_MISC_CNTRL_3_Reserved_31_13_MASK        0xffffe000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            INV_TIMEOUT:3;
    UINT32                            Reserved_11_6:6;
    UINT32                            DTEResvBitChkDis:1;
    UINT32                            Reserved_31_13:19;
  } Field;
  UINT32 Value;
} RIOMMU_MISC_CNTRL_3_STRUCT;

#define SMN_RIOMMU_MISC_CNTRL_3_ADDRESS    0x16b0a0f0UL


/***********************************************************
* Register Name : RIOMMU_PCTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis for full disconnect initiation of sdp ports.
#define RIOMMU_PCTRL_PCTRL_hysteresis_OFFSET      0
#define RIOMMU_PCTRL_PCTRL_hysteresis_MASK        0xff

// Bitfield Description : 
#define RIOMMU_PCTRL_Reserved_31_8_OFFSET      8
#define RIOMMU_PCTRL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            PCTRL_hysteresis:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} RIOMMU_PCTRL_STRUCT;

#define SMN_RIOMMU_PCTRL_ADDRESS    0x16b0a100UL


/***********************************************************
* Register Name : RIOMMU_REQ_ARB
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Req FIFO arbiter priority.
#define RIOMMU_REQ_ARB_reqfifo_arb_pri_OFFSET      0
#define RIOMMU_REQ_ARB_reqfifo_arb_pri_MASK        0x1

// Bitfield Description : Pre-fetch and invalidation refetch request arbiter priority.
#define RIOMMU_REQ_ARB_cpreq_arb_pri_OFFSET      1
#define RIOMMU_REQ_ARB_cpreq_arb_pri_MASK        0x2

// Bitfield Description : Burst size of the request to the arbiter.
#define RIOMMU_REQ_ARB_req_arb_burst_OFFSET      2
#define RIOMMU_REQ_ARB_req_arb_burst_MASK        0xc

// Bitfield Description : 
#define RIOMMU_REQ_ARB_Reserved_31_4_OFFSET      4
#define RIOMMU_REQ_ARB_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            reqfifo_arb_pri:1;
    UINT32                            cpreq_arb_pri:1;
    UINT32                            req_arb_burst:2;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} RIOMMU_REQ_ARB_STRUCT;

#define SMN_RIOMMU_REQ_ARB_ADDRESS    0x16b0a0f4UL


/***********************************************************
* Register Name : RIOMMU_SYS_MODE
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 0=rIOMMU operates in PC mode (default), with prefetch expected for physical function device only. 1=rIOMMU operates in console mode and supports multiple vfids (and the physical function) to be prefetched and stored in the cache.
#define RIOMMU_SYS_MODE_CONSOLE_MODE_OFFSET      0
#define RIOMMU_SYS_MODE_CONSOLE_MODE_MASK        0x1

// Bitfield Description : 
#define RIOMMU_SYS_MODE_Reserved_31_1_OFFSET      1
#define RIOMMU_SYS_MODE_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            CONSOLE_MODE:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} RIOMMU_SYS_MODE_STRUCT;

#define SMN_RIOMMU_SYS_MODE_ADDRESS    0x16b13004UL


/***********************************************************
* Register Name : RIOMMU_TLVL_ASSIGNMENT_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define RIOMMU_TLVL_ASSIGNMENT_0_Reserved_11_0_OFFSET      0
#define RIOMMU_TLVL_ASSIGNMENT_0_Reserved_11_0_MASK        0xfff

// Bitfield Description : Trust level for RIOMMU TW traffic.
#define RIOMMU_TLVL_ASSIGNMENT_0_TLvlMapping3_OFFSET      12
#define RIOMMU_TLVL_ASSIGNMENT_0_TLvlMapping3_MASK        0xf000

// Bitfield Description : 
#define RIOMMU_TLVL_ASSIGNMENT_0_Reserved_31_16_OFFSET      16
#define RIOMMU_TLVL_ASSIGNMENT_0_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_11_0:12;
    UINT32                            TLvlMapping3:4;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RIOMMU_TLVL_ASSIGNMENT_0_STRUCT;

#define SMN_RIOMMU_TLVL_ASSIGNMENT_0_ADDRESS    0x16b13000UL


/***********************************************************
* Register Name : RIOMMU_TW_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define RIOMMU_TW_CONTROL_Reserved_5_0_OFFSET      0
#define RIOMMU_TW_CONTROL_Reserved_5_0_MASK        0x3f

// Bitfield Description : 1=Table-walker always genereates coherent requests. The DTE SD bit is ignored when this bit is set to 1.
#define RIOMMU_TW_CONTROL_TWForceCoherent_OFFSET      6
#define RIOMMU_TW_CONTROL_TWForceCoherent_MASK        0x40

// Bitfield Description : 
#define RIOMMU_TW_CONTROL_Reserved_15_7_OFFSET      7
#define RIOMMU_TW_CONTROL_Reserved_15_7_MASK        0xff80

// Bitfield Description : Disables filtering within Table Walker of memory read requests to same address.
#define RIOMMU_TW_CONTROL_TWFilter_Dis_OFFSET      16
#define RIOMMU_TW_CONTROL_TWFilter_Dis_MASK        0x10000

// Bitfield Description : 
#define RIOMMU_TW_CONTROL_Reserved_24_17_OFFSET      17
#define RIOMMU_TW_CONTROL_Reserved_24_17_MASK        0x1fe0000

// Bitfield Description : 1-Enables storing Nested PTEs (2MB and larger) in PDC. 0-Disable storing Nested PTEs in PDC (default)
#define RIOMMU_TW_CONTROL_TWCacheNestedPTE_OFFSET      25
#define RIOMMU_TW_CONTROL_TWCacheNestedPTE_MASK        0x2000000

// Bitfield Description : 1-Stores all 8 phases of refetch request trigged by invalidation regardless of how many phases are actually needed. 0-Stores exact number of phases needed for the prefetch
#define RIOMMU_TW_CONTROL_ForcePhaseLenEqZeroOnInv_OFFSET      26
#define RIOMMU_TW_CONTROL_ForcePhaseLenEqZeroOnInv_MASK        0x4000000

// Bitfield Description : 
#define RIOMMU_TW_CONTROL_Reserved_31_27_OFFSET      27
#define RIOMMU_TW_CONTROL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            Reserved_5_0:6;
    UINT32                            TWForceCoherent:1;
    UINT32                            Reserved_15_7:9;
    UINT32                            TWFilter_Dis:1;
    UINT32                            Reserved_24_17:8;
    UINT32                            TWCacheNestedPTE:1;
    UINT32                            ForcePhaseLenEqZeroOnInv:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} RIOMMU_TW_CONTROL_STRUCT;

#define SMN_RIOMMU_TW_CONTROL_ADDRESS    0x16b0a0ecUL


/***********************************************************
* Register Name : RIOMMU_Z10_HW_INIT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit maintained by MSMU to assist HW in tracking the start and end of the restore phase of Z10.
#define RIOMMU_Z10_HW_INIT_HW_INIT_OFFSET      0
#define RIOMMU_Z10_HW_INIT_HW_INIT_MASK        0x1

// Bitfield Description : 
#define RIOMMU_Z10_HW_INIT_Reserved_31_1_OFFSET      1
#define RIOMMU_Z10_HW_INIT_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            HW_INIT:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} RIOMMU_Z10_HW_INIT_STRUCT;

#define SMN_RIOMMU_Z10_HW_INIT_ADDRESS    0x16b13008UL

#endif /* _RIOMMU_H_ */
