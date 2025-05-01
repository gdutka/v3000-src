/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ATHUB_ATS_H_
#define _ATHUB_ATS_H_


/***********************************************************
* Register Name : ATC_ATS_CNTL
* Register Description :
* This register controls the ATS block in the ATC
* Visibility : 0x2
************************************************************/

// Bitfield Description : When set, disables ATC requests. This has identical functionality, but opposite polarity, to BIF_MC_atcconfig.atc_enable (derived from ATS_CONTROL.ENABLE from the PCIE ATS Capability register). This field must be clear (LOW) and BIF_MC_atcconfig.atc_enable (derived from ATS_CONTROL.ENABLE from the PCIE ATS Capability register) must be HIGH to enable ATC. When this field is set, ATC follows the behavior rules described in ATC_L1_CNTL for transactions bypassing the ATC. While ATC is disabled, the ATC continues to send invalidation returns. For a full discussion on the ATC behavior when it is disabled, please see the ATC Interface Spec.
#define ATC_ATS_CNTL_DISABLE_ATC_OFFSET      0
#define ATC_ATS_CNTL_DISABLE_ATC_MASK        0x1

// Bitfield Description : When set, disables PRI state machine in ATC. Works in conjunction with BIF_MC_atcconfig.pri_enable (derived from PAGE_REQUEST_CONTROL.ENABLE from the PCIE PRI Capability register).
#define ATC_ATS_CNTL_DISABLE_PRI_OFFSET      1
#define ATC_ATS_CNTL_DISABLE_PRI_MASK        0x2

// Bitfield Description : When set, disables PASID usage within ATC. Works in conjunction with BIF_MC_atcconfig.pasid_enable (derived from PASID_CONTROL.PASID_ENABLE from the PCIE PASID Capability register).
#define ATC_ATS_CNTL_DISABLE_PASID_OFFSET      2
#define ATC_ATS_CNTL_DISABLE_PASID_MASK        0x4

// Bitfield Description : 
#define ATC_ATS_CNTL_Reserved_7_3_OFFSET      3
#define ATC_ATS_CNTL_Reserved_7_3_MASK        0xf8

// Bitfield Description : Number of Credits between ATS and RPB on ATS request interface
#define ATC_ATS_CNTL_CREDITS_ATS_RPB_OFFSET      8
#define ATC_ATS_CNTL_CREDITS_ATS_RPB_MASK        0x3f00

// Bitfield Description : 
#define ATC_ATS_CNTL_Reserved_15_14_OFFSET      14
#define ATC_ATS_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Placeholding eco control bits
#define ATC_ATS_CNTL_DEBUG_ECO_OFFSET      16
#define ATC_ATS_CNTL_DEBUG_ECO_MASK        0xf0000

// Bitfield Description : Keep the order that make sure invalidation is logged into IH, and then return inv ACK back to IOMMU. If not set won't keep the order of log and ack to IOMMU, also when LOG is busying, the invalidation will be dropped to log
#define ATC_ATS_CNTL_INVALIDATION_LOG_KEEP_ORDER_OFFSET      20
#define ATC_ATS_CNTL_INVALIDATION_LOG_KEEP_ORDER_MASK        0x100000

// Bitfield Description : Keep the order that make sure PTE is logged into IH, and then return PTE back to ATCL2. If not set won't keep the order of log and return, also when LOG is busying, the returned PTE will be dropped to log
#define ATC_ATS_CNTL_TRANS_LOG_KEEP_ORDER_OFFSET      21
#define ATC_ATS_CNTL_TRANS_LOG_KEEP_ORDER_MASK        0x200000

// Bitfield Description : For requests with exe permission required, to control the granted PTE's exe permission, 00: _ret_exe_perm = 0, 01: _ret_exe_perm = 1, 10: _ret_exe_perm = PTE's exe(default)
#define ATC_ATS_CNTL_TRANS_EXE_RETURN_OFFSET      22
#define ATC_ATS_CNTL_TRANS_EXE_RETURN_MASK        0xc00000

// Bitfield Description : For Fake ATC mode: When guest translation missed, what will be returned, value = 0: fault with default page address without pri; value = 1: retry with default page address with pri
#define ATC_ATS_CNTL_GUEST_TRANS_MISS_MODE_OFFSET      24
#define ATC_ATS_CNTL_GUEST_TRANS_MISS_MODE_MASK        0x1000000

// Bitfield Description : in case of fault, keep the VMID busy if that VMID has outstanding interrupt, not implemented yet
#define ATC_ATS_CNTL_KEEP_VMID_BUSY_BY_INTR_OFFSET      25
#define ATC_ATS_CNTL_KEEP_VMID_BUSY_BY_INTR_MASK        0x2000000

// Bitfield Description : 
#define ATC_ATS_CNTL_Reserved_31_26_OFFSET      26
#define ATC_ATS_CNTL_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            DISABLE_ATC:1;
    UINT32                            DISABLE_PRI:1;
    UINT32                            DISABLE_PASID:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            CREDITS_ATS_RPB:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            DEBUG_ECO:4;
    UINT32                            INVALIDATION_LOG_KEEP_ORDER:1;
    UINT32                            TRANS_LOG_KEEP_ORDER:1;
    UINT32                            TRANS_EXE_RETURN:2;
    UINT32                            GUEST_TRANS_MISS_MODE:1;
    UINT32                            KEEP_VMID_BUSY_BY_INTR:1;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} ATC_ATS_CNTL_STRUCT;

#define SMN_ATC_ATS_CNTL_ADDRESS    0x300cUL


/***********************************************************
* Register Name : ATC_ATS_CNTL2
* Register Description :
* This register controls the arbitration between TR and PR for ATS->RPB requests, INV still with the highest prioity
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Credits between ATS and RPB on ATS request interface for MM address translation
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_MMTR_OFFSET      0
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_MMTR_MASK        0xff

// Bitfield Description : Number of Credits between ATS and RPB on ATS request interface for GFX address translation
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_GFXTR_OFFSET      8
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_GFXTR_MASK        0xff00

// Bitfield Description : Number of Credits between ATS and RPB on ATS request interface for page request and invalidation ack
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_PRINV_OFFSET      16
#define ATC_ATS_CNTL2_CREDITS_ATS_RPB_PRINV_MASK        0xff0000

// Bitfield Description : Set to 1 to stop the translation requests issuing to RPB, set to 0 to continue the translaiton requests
#define ATC_ATS_CNTL2_TRANSLATION_STALL_OFFSET      24
#define ATC_ATS_CNTL2_TRANSLATION_STALL_MASK        0x1000000

// Bitfield Description : How to process invalidation with PASID when PSEUDO_IOV mode, 0 to issue host invalidation to UTCL2, 1 to just return back inv ack to IOMMU withoutsending it to UTCL2
#define ATC_ATS_CNTL2_INV_PASID_PSEUDO_IOV_OFFSET      25
#define ATC_ATS_CNTL2_INV_PASID_PSEUDO_IOV_MASK        0x2000000

// Bitfield Description : Default mm_trans will go through vc5. when set to 1,gc_trans will go through vc5
#define ATC_ATS_CNTL2_GC_MM_TRANS_SWITCH_OFFSET      26
#define ATC_ATS_CNTL2_GC_MM_TRANS_SWITCH_MASK        0x4000000

// Bitfield Description : gc translation map onto vc5 enable. 0:gc trans map onto vc0; 1:gc trans map onto vc5.
#define ATC_ATS_CNTL2_GC_TRANS_VC5_ENABLE_OFFSET      27
#define ATC_ATS_CNTL2_GC_TRANS_VC5_ENABLE_MASK        0x8000000

// Bitfield Description : mm translation map onto vc5 enable. 0:mm trans map onto vc0, 1:mm trans map onto vc5.
#define ATC_ATS_CNTL2_MM_TRANS_VC5_ENABLE_OFFSET      28
#define ATC_ATS_CNTL2_MM_TRANS_VC5_ENABLE_MASK        0x10000000

// Bitfield Description : when set, enable vf/vfid written check on VMIDx_PASID_MAPPING registers.
#define ATC_ATS_CNTL2_VFID_CHECK_ENABLE_OFFSET      29
#define ATC_ATS_CNTL2_VFID_CHECK_ENABLE_MASK        0x20000000

// Bitfield Description : reserved fields
#define ATC_ATS_CNTL2_Reserved_31_30_OFFSET      30
#define ATC_ATS_CNTL2_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CREDITS_ATS_RPB_MMTR:8;
    UINT32                            CREDITS_ATS_RPB_GFXTR:8;
    UINT32                            CREDITS_ATS_RPB_PRINV:8;
    UINT32                            TRANSLATION_STALL:1;
    UINT32                            INV_PASID_PSEUDO_IOV:1;
    UINT32                            GC_MM_TRANS_SWITCH:1;
    UINT32                            GC_TRANS_VC5_ENABLE:1;
    UINT32                            MM_TRANS_VC5_ENABLE:1;
    UINT32                            VFID_CHECK_ENABLE:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} ATC_ATS_CNTL2_STRUCT;

#define SMN_ATC_ATS_CNTL2_ADDRESS    0x3034UL


/***********************************************************
* Register Name : ATC_ATS_DEBUG
* Register Description :
* Debug Control for ATS Engine. To defeature the wq with fewer entries, program ATC_L2_DEBUG.CREDITS_L2_ATS.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When written to 1, invalidates all ATS L2 and L1 cache entries for all VMID domains one-shot
#define ATC_ATS_DEBUG_INVALIDATE_ALL_OFFSET      0
#define ATC_ATS_DEBUG_INVALIDATE_ALL_MASK        0x1

// Bitfield Description : When set, ATS is in loopback mode where it immediately sends the translation request as a translation response. This is to facilitate performance testing of the L1 and L2 without having to set up a TA. Note that the identity occurs on the ATC RPB interface so the NW bit on the request only transforms the Write bit of the return. The Read bit is always set.
#define ATC_ATS_DEBUG_IDENT_RETURN_OFFSET      1
#define ATC_ATS_DEBUG_IDENT_RETURN_MASK        0x2

// Bitfield Description : When set, ATS will always request NW=0 for Translation Requests (request read & write access).
#define ATC_ATS_DEBUG_ADDRESS_TRANSLATION_REQUEST_WRITE_PERMS_OFFSET      2
#define ATC_ATS_DEBUG_ADDRESS_TRANSLATION_REQUEST_WRITE_PERMS_MASK        0x4

// Bitfield Description : 
#define ATC_ATS_DEBUG_Reserved_3_3_OFFSET      3
#define ATC_ATS_DEBUG_Reserved_3_3_MASK        0x8

// Bitfield Description : Specifies how to set the relaxed ordering attribute bit for Trans Requests. As each tr is the last one in the group, we need clear this bit
#define ATC_ATS_DEBUG_TRANS_REQUESTS_USE_RELAXED_ORDERING_OFFSET      4
#define ATC_ATS_DEBUG_TRANS_REQUESTS_USE_RELAXED_ORDERING_MASK        0x10

// Bitfield Description : Specifies how to set the relaxed ordering attribute bit for Page Requests. As each pr is the last one in the group, we need clear this bit
#define ATC_ATS_DEBUG_PAGE_REQUESTS_USE_RELAXED_ORDERING_OFFSET      5
#define ATC_ATS_DEBUG_PAGE_REQUESTS_USE_RELAXED_ORDERING_MASK        0x20

// Bitfield Description : Specifies how to set the user bit in the TLP prefix
#define ATC_ATS_DEBUG_PRIV_BIT_OFFSET      6
#define ATC_ATS_DEBUG_PRIV_BIT_MASK        0x40

// Bitfield Description : Specifies how to set the exe bit in the TLP prefix
#define ATC_ATS_DEBUG_EXE_BIT_OFFSET      7
#define ATC_ATS_DEBUG_EXE_BIT_MASK        0x80

// Bitfield Description : When set, will always ask for both read and write access for a page, rather than just read or write as needed.
#define ATC_ATS_DEBUG_PAGE_REQUEST_PERMS_OFFSET      8
#define ATC_ATS_DEBUG_PAGE_REQUEST_PERMS_MASK        0x100

// Bitfield Description : When set, untranslated only requests will use the size bit from the translation return. When this bit is cleared, the size bit will be ignored for untranslated only returns.
#define ATC_ATS_DEBUG_UNTRANSLATED_ONLY_REQUESTS_CARRY_SIZE_OFFSET      9
#define ATC_ATS_DEBUG_UNTRANSLATED_ONLY_REQUESTS_CARRY_SIZE_MASK        0x200

// Bitfield Description : By default ATC switch between enable and disable, ATS will initiate a non-flushtype invalidation all request. If set, ATS will not initiate a non-flushtype invalidation request then.
#define ATC_ATS_DEBUG_DISABLE_INVALIDATION_ON_ED_SWITCH_OFFSET      10
#define ATC_ATS_DEBUG_DISABLE_INVALIDATION_ON_ED_SWITCH_MASK        0x400

// Bitfield Description : Overwrite the translation request's exe bit: 0 -> 0, 1 -> 1, 2 -> request's exe, 3 -> ~request's exe
#define ATC_ATS_DEBUG_TRANS_REQ_EXE_OVERWRITE_OFFSET      11
#define ATC_ATS_DEBUG_TRANS_REQ_EXE_OVERWRITE_MASK        0x3800

// Bitfield Description : when set disallows the wq entry to be marked as done after an error. If set, when a state machine entry encounters an error, the entry will stay in that state until reset, which is useful for scandump debug.
#define ATC_ATS_DEBUG_DISALLOW_ERR_TO_DONE_OFFSET      14
#define ATC_ATS_DEBUG_DISALLOW_ERR_TO_DONE_MASK        0x4000

// Bitfield Description : when set ignore the fed bit
#define ATC_ATS_DEBUG_IGNORE_FED_OFFSET      15
#define ATC_ATS_DEBUG_IGNORE_FED_MASK        0x8000

// Bitfield Description : When set, invalidation requests cause faults when ATC is disabled
#define ATC_ATS_DEBUG_INVALIDATION_REQUESTS_DISALLOWED_WHEN_ATC_IS_DISABLED_OFFSET      16
#define ATC_ATS_DEBUG_INVALIDATION_REQUESTS_DISALLOWED_WHEN_ATC_IS_DISABLED_MASK        0x10000

// Bitfield Description : When set, will force flushtype invalidation's address to full range
#define ATC_ATS_DEBUG_DISABLE_INVALIDATE_PER_RANGE_OFFSET      17
#define ATC_ATS_DEBUG_DISABLE_INVALIDATE_PER_RANGE_MASK        0x20000

// Bitfield Description : disable per domain invalidate in the L1 and L2 caches. Setting this bit turns all per domain invalidated into invalidate all. we didn't validate this register field in CI, and it's debug for CI
#define ATC_ATS_DEBUG_DISABLE_INVALIDATE_PER_DOMAIN_OFFSET      18
#define ATC_ATS_DEBUG_DISABLE_INVALIDATE_PER_DOMAIN_MASK        0x40000

// Bitfield Description : When set, VMID0 to PASID mapping specified in the ATC_VMID0_PASID_MAPPING register is disabled.
#define ATC_ATS_DEBUG_DISABLE_VMID0_PASID_MAPPING_OFFSET      19
#define ATC_ATS_DEBUG_DISABLE_VMID0_PASID_MAPPING_MASK        0x80000

// Bitfield Description : By default during world switch (i.e. when MC_SHARED_ACTIVE_FCN_ID_reg is changed), ATS will initiate a non-flushtype invalidation request. If set, ATS will not initiate a non-flushtype invalidation request during world switch.
#define ATC_ATS_DEBUG_DISABLE_INVALIDATION_ON_WORLD_SWITCH_OFFSET      20
#define ATC_ATS_DEBUG_DISABLE_INVALIDATION_ON_WORLD_SWITCH_MASK        0x100000

// Bitfield Description : If set, ATS will initiate non-flushtype invalidation request when MC_SHARED_VF_ENABLE_reg is changed.
#define ATC_ATS_DEBUG_ENABLE_INVALIDATION_ON_VIRTUALIZATION_ENTRY_AND_EXIT_OFFSET      21
#define ATC_ATS_DEBUG_ENABLE_INVALIDATION_ON_VIRTUALIZATION_ENTRY_AND_EXIT_MASK        0x200000

// Bitfield Description : If set, ATS won't issue outstanding flushtype/non-flushtype invalidations at the same time but will make sure only one outstanding invalidation.
#define ATC_ATS_DEBUG_DISABLE_MULTIPLE_INVALIDATIONS_OFFSET      22
#define ATC_ATS_DEBUG_DISABLE_MULTIPLE_INVALIDATIONS_MASK        0x400000

// Bitfield Description : If set, when page requests work queue is full ATS won't issue page request for translation completion without enough permissions. Otherwise, ATS will wait for pr work queue available.
#define ATC_ATS_DEBUG_DROP_PAGE_REQUEST_WHEN_FULL_OFFSET      23
#define ATC_ATS_DEBUG_DROP_PAGE_REQUEST_WHEN_FULL_MASK        0x800000

// Bitfield Description : How much translation work queue entries can be used, (0...6) 2^(-field) number of trans work queue entries, which means 0=>ATHUB__ATS__ATS_WORKQUEUE_DEPTH, 1=>`ATHUB__ATS__ATS_WORKQUEUE_DEPTH/2, 2=>`ATHUB__ATS__ATS_WORKQUEUE_DEPTH/4, ......
#define ATC_ATS_DEBUG_EFFECTIVE_TRANS_WORK_QUEUE_SIZE_OFFSET      24
#define ATC_ATS_DEBUG_EFFECTIVE_TRANS_WORK_QUEUE_SIZE_MASK        0x7000000

// Bitfield Description : How much page request work queue entries can be used, (0...8) 2^(-field) number of pr work queue entries, which means 0=>ATHUB__ATS__PR_WORKQUEUE_DEPTH, 1=>ATHUB__ATS__PR_WORKQUEUE_DEPTH/2, 2=>ATHUB__ATS__PR_WORKQUEUE_DEPTH/4, ......
#define ATC_ATS_DEBUG_EFFECTIVE_PR_WORK_QUEUE_SIZE_OFFSET      27
#define ATC_ATS_DEBUG_EFFECTIVE_PR_WORK_QUEUE_SIZE_MASK        0x78000000

// Bitfield Description : When set, VMID16 to PASID mapping specified in the ATC_VMID16_PASID_MAPPING register is disabled. Its function is same as DISABLE_VMID0_PASID_MAPPING, but it's for MMHUB's vmid0
#define ATC_ATS_DEBUG_DISABLE_VMID16_PASID_MAPPING_OFFSET      31
#define ATC_ATS_DEBUG_DISABLE_VMID16_PASID_MAPPING_MASK        0x80000000

typedef union {
  struct {
    UINT32                            INVALIDATE_ALL:1;
    UINT32                            IDENT_RETURN:1;
    UINT32                            ADDRESS_TRANSLATION_REQUEST_WRITE_PERMS:1;
    UINT32                            Reserved_3_3:1;
    UINT32                            TRANS_REQUESTS_USE_RELAXED_ORDERING:1;
    UINT32                            PAGE_REQUESTS_USE_RELAXED_ORDERING:1;
    UINT32                            PRIV_BIT:1;
    UINT32                            EXE_BIT:1;
    UINT32                            PAGE_REQUEST_PERMS:1;
    UINT32                            UNTRANSLATED_ONLY_REQUESTS_CARRY_SIZE:1;
    UINT32                            DISABLE_INVALIDATION_ON_ED_SWITCH:1;
    UINT32                            TRANS_REQ_EXE_OVERWRITE:3;
    UINT32                            DISALLOW_ERR_TO_DONE:1;
    UINT32                            IGNORE_FED:1;
    UINT32                            INVALIDATION_REQUESTS_DISALLOWED_WHEN_ATC_IS_DISABLED:1;
    UINT32                            DISABLE_INVALIDATE_PER_RANGE:1;
    UINT32                            DISABLE_INVALIDATE_PER_DOMAIN:1;
    UINT32                            DISABLE_VMID0_PASID_MAPPING:1;
    UINT32                            DISABLE_INVALIDATION_ON_WORLD_SWITCH:1;
    UINT32                            ENABLE_INVALIDATION_ON_VIRTUALIZATION_ENTRY_AND_EXIT:1;
    UINT32                            DISABLE_MULTIPLE_INVALIDATIONS:1;
    UINT32                            DROP_PAGE_REQUEST_WHEN_FULL:1;
    UINT32                            EFFECTIVE_TRANS_WORK_QUEUE_SIZE:3;
    UINT32                            EFFECTIVE_PR_WORK_QUEUE_SIZE:4;
    UINT32                            DISABLE_VMID16_PASID_MAPPING:1;
  } Field;
  UINT32 Value;
} ATC_ATS_DEBUG_STRUCT;

#define SMN_ATC_ATS_DEBUG_ADDRESS    0x3010UL


/***********************************************************
* Register Name : ATC_ATS_DEBUG2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : After pri disable, the time to wait before set pri_stop, which can make sure page requests are issued from rpb to nbif, cycles = (1 << value), so the default wait cycles will be 8
#define ATC_ATS_DEBUG2_PRI_STOP_TIME_OFFSET      0
#define ATC_ATS_DEBUG2_PRI_STOP_TIME_MASK        0xf

// Bitfield Description : When mmhub or gfx have multi vmids mapped to the pasid(the other utcl2 has no valid vmid), whether to invalidate the other one also, not used now, IMPR?
#define ATC_ATS_DEBUG2_MAPPING_MULTIPLE_INVALIDATE_ALL_UTCL2_OFFSET      4
#define ATC_ATS_DEBUG2_MAPPING_MULTIPLE_INVALIDATE_ALL_UTCL2_MASK        0x10

// Bitfield Description : When mmhub or gfx both has single valid vmid mapped to the pasid and not equal to each other, whether to do full address range invalidations, not used now, IMPR?
#define ATC_ATS_DEBUG2_MAPPING_DIFF_SINGLE_INVALIDATE_ALL_UTCL2_OFFSET      5
#define ATC_ATS_DEBUG2_MAPPING_DIFF_SINGLE_INVALIDATE_ALL_UTCL2_MASK        0x20

// Bitfield Description : When snapshot happen, whether do snapshot for wq which then won't issue page request for that round translation
#define ATC_ATS_DEBUG2_SNAPSHOT_FOR_WQ_OFFSET      6
#define ATC_ATS_DEBUG2_SNAPSHOT_FOR_WQ_MASK        0x40

// Bitfield Description : When total mmhub/gfx has multiple vmid mapped to single pasid, whether do invalidation all for gfx & mmhub, not used now, IMPR?
#define ATC_ATS_DEBUG2_MAPPING_SAVE_MODE_OFFSET      7
#define ATC_ATS_DEBUG2_MAPPING_SAVE_MODE_MASK        0x80

// Bitfield Description : When host translation missed, what will be returned, value = 0: fault with default page address without pri; value = 1: retry with default page address with pri
#define ATC_ATS_DEBUG2_HOST_TRANS_MISS_MODE_OFFSET      8
#define ATC_ATS_DEBUG2_HOST_TRANS_MISS_MODE_MASK        0x100

// Bitfield Description : Whether log the non flush type invalidation
#define ATC_ATS_DEBUG2_LOG_NONFLUSH_TYPE_INVALIDATION_OFFSET      9
#define ATC_ATS_DEBUG2_LOG_NONFLUSH_TYPE_INVALIDATION_MASK        0x200

// Bitfield Description : reserved
#define ATC_ATS_DEBUG2_Reserved_10_10_OFFSET      10
#define ATC_ATS_DEBUG2_Reserved_10_10_MASK        0x400

// Bitfield Description : Set this bit will disable log function for page migration, turn off by default
#define ATC_ATS_DEBUG2_DISABLE_LOG_FUNCTION_OFFSET      11
#define ATC_ATS_DEBUG2_DISABLE_LOG_FUNCTION_MASK        0x800

// Bitfield Description : If set, ats won't invoke pri reset when page response failure happen
#define ATC_ATS_DEBUG2_DISABLE_PRI_RESET_WHEN_FAILURE_OFFSET      12
#define ATC_ATS_DEBUG2_DISABLE_PRI_RESET_WHEN_FAILURE_MASK        0x1000

// Bitfield Description : decide how to return snoop bit to ATCL2 according to non-snoop bit from iommu, 0->0, 1->1, 2-> PTE's nonsnoop, 3-> ~PTE's nonsnoop
#define ATC_ATS_DEBUG2_TRANS_RET_SNOOP_CNTL_OFFSET      13
#define ATC_ATS_DEBUG2_TRANS_RET_SNOOP_CNTL_MASK        0x6000

// Bitfield Description : 
#define ATC_ATS_DEBUG2_Reserved_23_15_OFFSET      15
#define ATC_ATS_DEBUG2_Reserved_23_15_MASK        0xff8000

// Bitfield Description : How much Log FIFO entries can be used, (0...8) 2^(-field) number of Log FIFO entries, which means 0=>`ATC_LOG_FIFO_DEPTH, 1=>`ATC_LOG_FIFO_DEPTH/2, 2=>`ATC_LOG_FIFO_DEPTH/4, ......
#define ATC_ATS_DEBUG2_EFFECTIVE_LOG_FIFO_DEPTH_OFFSET      24
#define ATC_ATS_DEBUG2_EFFECTIVE_LOG_FIFO_DEPTH_MASK        0xf000000

// Bitfield Description : Whether to fault when translation completion is without enough permissions and ur_stu_ne_size/at_pa_exceeded_44bits/at_ut_set4_request_with_pasid
#define ATC_ATS_DEBUG2_FAULT_ON_NOENOUGH_PERMISSIONS_OFFSET      28
#define ATC_ATS_DEBUG2_FAULT_ON_NOENOUGH_PERMISSIONS_MASK        0x10000000

// Bitfield Description : Whether to fault when GVA is received without valid vmid->pasid mapping
#define ATC_ATS_DEBUG2_GVA_INVALID_PASID_FAULT_OFFSET      29
#define ATC_ATS_DEBUG2_GVA_INVALID_PASID_FAULT_MASK        0x20000000

// Bitfield Description : To stall the whole invalidation flow, when use it take care the inv ack time out issue in IOMMU
#define ATC_ATS_DEBUG2_INVALIDATION_STALL_OFFSET      30
#define ATC_ATS_DEBUG2_INVALIDATION_STALL_MASK        0x40000000

// Bitfield Description : When return inv ack, can don't wait for WQ to improve performance
#define ATC_ATS_DEBUG2_INVALIDATION_NOT_WAIT_WQ_OFFSET      31
#define ATC_ATS_DEBUG2_INVALIDATION_NOT_WAIT_WQ_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PRI_STOP_TIME:4;
    UINT32                            MAPPING_MULTIPLE_INVALIDATE_ALL_UTCL2:1;
    UINT32                            MAPPING_DIFF_SINGLE_INVALIDATE_ALL_UTCL2:1;
    UINT32                            SNAPSHOT_FOR_WQ:1;
    UINT32                            MAPPING_SAVE_MODE:1;
    UINT32                            HOST_TRANS_MISS_MODE:1;
    UINT32                            LOG_NONFLUSH_TYPE_INVALIDATION:1;
    UINT32                            Reserved_10_10:1;
    UINT32                            DISABLE_LOG_FUNCTION:1;
    UINT32                            DISABLE_PRI_RESET_WHEN_FAILURE:1;
    UINT32                            TRANS_RET_SNOOP_CNTL:2;
    UINT32                            Reserved_23_15:9;
    UINT32                            EFFECTIVE_LOG_FIFO_DEPTH:4;
    UINT32                            FAULT_ON_NOENOUGH_PERMISSIONS:1;
    UINT32                            GVA_INVALID_PASID_FAULT:1;
    UINT32                            INVALIDATION_STALL:1;
    UINT32                            INVALIDATION_NOT_WAIT_WQ:1;
  } Field;
  UINT32 Value;
} ATC_ATS_DEBUG2_STRUCT;

#define SMN_ATC_ATS_DEBUG2_ADDRESS    0x3030UL


/***********************************************************
* Register Name : ATC_ATS_DEFAULT_PAGE_LOW
* Register Description :
* Default Page Address 43:12 when fault occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_ATS_DEFAULT_PAGE_LOW_DEFAULT_PAGE_OFFSET      0
#define ATC_ATS_DEFAULT_PAGE_LOW_DEFAULT_PAGE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DEFAULT_PAGE:32;
  } Field;
  UINT32 Value;
} ATC_ATS_DEFAULT_PAGE_LOW_STRUCT;

#define SMN_ATC_ATS_DEFAULT_PAGE_LOW_ADDRESS    0x301cUL


/***********************************************************
* Register Name : ATC_ATS_FAULT_CNTL
* Register Description :
* This register describes which types of faults will trigger error handling of some type within the ATC
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enables logging in the Fault Register Log for various fault cases. Priority if seen on the same cycle is from top to bottom See ATC_ATS_FAULT_STATUS_INFO and ATC_ATS_FAULT_ADDR for more details. Bit 0 = SIZE_SMALLER_THAN_STU:  Bit 1 = ATS_WHILE_DIS: ATC disabled but ATC type request received, see ATC_ATS_FAULT_STATUS registers for details, Bit 2 = DUPLICATED ITAG: Multiple Invalidations Outstanding with same ITAG. See ATC_ATS_FAULT_STATAS registers for details Bit 3 = NON SUCCESS TYPE: Non Success Type Translation Completion or Page Request Group Response, UPRGI response (Unexpected Page Request Group Interface). See ATC_ATS_FAULT_STATUS registers for details Bit 4 = FED_BIT: Fed Bit set Bit 5 = ATC_ENABLED_BME_DEASSERT Bit 6 = PHY_ADDR_TOO_LARGE : Bit47:44 of the translated (or physical) address are non-zero Bit 7 = U bit set on translation for request that was sent with PASID. Bit 8 = Host translation miss or GUEST translation with invalid PASID
#define ATC_ATS_FAULT_CNTL_FAULT_REGISTER_LOG_OFFSET      0
#define ATC_ATS_FAULT_CNTL_FAULT_REGISTER_LOG_MASK        0x1ff

// Bitfield Description : 
#define ATC_ATS_FAULT_CNTL_Reserved_9_9_OFFSET      9
#define ATC_ATS_FAULT_CNTL_Reserved_9_9_MASK        0x200

// Bitfield Description : Enables interrupt generation for various fault cases See ATC_ATS_FAULT_CNTL.FAULT_REGISTER_LOG for description of bit values In addition to the priority described in ATC_ATS_FAULT_CNTL.FAULT_REGISTER_LOG, an interrupt can only be sent once every 8 cycles. If an interrupt occurs in back to back cycles, only the first interrupt will be logged. If two interrupts are seen in the same cycle the interrupt with the highest priority will be logged.
#define ATC_ATS_FAULT_CNTL_FAULT_INTERRUPT_TABLE_OFFSET      10
#define ATC_ATS_FAULT_CNTL_FAULT_INTERRUPT_TABLE_MASK        0x7fc00

// Bitfield Description : 
#define ATC_ATS_FAULT_CNTL_Reserved_19_19_OFFSET      19
#define ATC_ATS_FAULT_CNTL_Reserved_19_19_MASK        0x80000

// Bitfield Description : Enables crashing for various fault cases See ATC_ATS_FAULT_CNTL.FAULT_REGISTER_LOG for description of bit values
#define ATC_ATS_FAULT_CNTL_FAULT_CRASH_TABLE_OFFSET      20
#define ATC_ATS_FAULT_CNTL_FAULT_CRASH_TABLE_MASK        0x1ff00000

// Bitfield Description : 
#define ATC_ATS_FAULT_CNTL_Reserved_31_29_OFFSET      29
#define ATC_ATS_FAULT_CNTL_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            FAULT_REGISTER_LOG:9;
    UINT32                            Reserved_9_9:1;
    UINT32                            FAULT_INTERRUPT_TABLE:9;
    UINT32                            Reserved_19_19:1;
    UINT32                            FAULT_CRASH_TABLE:9;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} ATC_ATS_FAULT_CNTL_STRUCT;

#define SMN_ATC_ATS_FAULT_CNTL_ADDRESS    0x3018UL


/***********************************************************
* Register Name : ATC_ATS_FAULT_DEBUG
* Register Description :
* This register controls ATC debug
* Visibility : 0x2
************************************************************/

// Bitfield Description : When set, continuously logs faults in ATC_ATS_FAULT_STATUS registers. When cleared, only the first fault is captured.
#define ATC_ATS_FAULT_DEBUG_ALLOW_SUBSEQUENT_FAULT_STATUS_ADDR_UPDATES_OFFSET      0
#define ATC_ATS_FAULT_DEBUG_ALLOW_SUBSEQUENT_FAULT_STATUS_ADDR_UPDATES_MASK        0x1

// Bitfield Description : 
#define ATC_ATS_FAULT_DEBUG_CLEAR_FAULT_STATUS_ADDR_OFFSET      1
#define ATC_ATS_FAULT_DEBUG_CLEAR_FAULT_STATUS_ADDR_MASK        0x2

// Bitfield Description : 
#define ATC_ATS_FAULT_DEBUG_Reserved_31_2_OFFSET      2
#define ATC_ATS_FAULT_DEBUG_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            ALLOW_SUBSEQUENT_FAULT_STATUS_ADDR_UPDATES:1;
    UINT32                            CLEAR_FAULT_STATUS_ADDR:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} ATC_ATS_FAULT_DEBUG_STRUCT;

#define SMN_ATC_ATS_FAULT_DEBUG_ADDRESS    0x3014UL


/***********************************************************
* Register Name : ATC_ATS_FAULT_STATUS_ADDR
* Register Description :
* This register reports the addr of the first fault after power-on or the last explicit clear of the register. Each page represents 4K (4,096) bytes and address bits 43:12. The values in the status and addr registers remain until explicitly cleared.
* Visibility : 0x2
************************************************************/

// Bitfield Description : NUM_REQUESTS_EXCEEDED, DUPLICATED_ITAG or FED_BIT w/InvReq logical address bits 47:44 otherwise physical address bits 43:12
#define ATC_ATS_FAULT_STATUS_ADDR_PAGE_ADDR_OFFSET      0
#define ATC_ATS_FAULT_STATUS_ADDR_PAGE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PAGE_ADDR:32;
  } Field;
  UINT32 Value;
} ATC_ATS_FAULT_STATUS_ADDR_STRUCT;

#define SMN_ATC_ATS_FAULT_STATUS_ADDR_ADDRESS    0x3070UL


/***********************************************************
* Register Name : ATC_ATS_FAULT_STATUS_INFO
* Register Description :
* This register reports the status of the first fault after power-on or the last explicit clear of the register. ATC_ATS_FAULT_CNTL and ATC_ATS_FAULT_DEBUG control the types of data that get collected here.
* Visibility : 0x2
************************************************************/

// Bitfield Description : See ATC_ATS_FAULT_CNTL.FAULT_REGISTER_LOG for description
#define ATC_ATS_FAULT_STATUS_INFO_FAULT_TYPE_OFFSET      0
#define ATC_ATS_FAULT_STATUS_INFO_FAULT_TYPE_MASK        0x1ff

// Bitfield Description : 
#define ATC_ATS_FAULT_STATUS_INFO_Reserved_9_9_OFFSET      9
#define ATC_ATS_FAULT_STATUS_INFO_Reserved_9_9_MASK        0x200

// Bitfield Description : VMID of fault for DUPLICATED ITAG, NUM_REQUESTS_EXCEEDED or FED_BIT on page requests or invalidation requests. Else Page Request Group where appropriate
#define ATC_ATS_FAULT_STATUS_INFO_VMID_OFFSET      10
#define ATC_ATS_FAULT_STATUS_INFO_VMID_MASK        0x7c00

// Bitfield Description : for DUPLICATED ITAG means Pasid Present ATS_WHILE_DIS means PASID Table Change for NON_SUCCESS_TYPE indicates Unexpected PRG index
#define ATC_ATS_FAULT_STATUS_INFO_EXTRA_INFO_OFFSET      15
#define ATC_ATS_FAULT_STATUS_INFO_EXTRA_INFO_MASK        0x8000

// Bitfield Description : for NUM_REQUESTS_EXCEEDED, write requested NON_SUCCESS_TYPE indicates Translation Completion Size is smaller than STU
#define ATC_ATS_FAULT_STATUS_INFO_EXTRA_INFO2_OFFSET      16
#define ATC_ATS_FAULT_STATUS_INFO_EXTRA_INFO2_MASK        0x10000

// Bitfield Description : indicates whether the error logging is for an invalidation
#define ATC_ATS_FAULT_STATUS_INFO_INVALIDATION_OFFSET      17
#define ATC_ATS_FAULT_STATUS_INFO_INVALIDATION_MASK        0x20000

// Bitfield Description : indicates whether the error logging is for a page request
#define ATC_ATS_FAULT_STATUS_INFO_PAGE_REQUEST_OFFSET      18
#define ATC_ATS_FAULT_STATUS_INFO_PAGE_REQUEST_MASK        0x40000

// Bitfield Description : Status on unexpected or non successful Page Request or Address Translation Duplicated Itag if Multiple Invalidations PA44:40 if PHY_ADDR_TOO_LARGE ATC L2 ID if L2 Parity Error ATC L2 ID of the request if U bit set on translation for request that was sent with PASID
#define ATC_ATS_FAULT_STATUS_INFO_STATUS_OFFSET      19
#define ATC_ATS_FAULT_STATUS_INFO_STATUS_MASK        0xf80000

// Bitfield Description : NUM_REQUESTS_EXCEEDED, DUPLICATED_ITAG or FED_BIT w/InvReq logical address bits 47:44 otherwise physical address bits 47:44
#define ATC_ATS_FAULT_STATUS_INFO_PAGE_ADDR_HIGH_OFFSET      24
#define ATC_ATS_FAULT_STATUS_INFO_PAGE_ADDR_HIGH_MASK        0xf000000

// Bitfield Description : 
#define ATC_ATS_FAULT_STATUS_INFO_Reserved_31_28_OFFSET      28
#define ATC_ATS_FAULT_STATUS_INFO_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            FAULT_TYPE:9;
    UINT32                            Reserved_9_9:1;
    UINT32                            VMID:5;
    UINT32                            EXTRA_INFO:1;
    UINT32                            EXTRA_INFO2:1;
    UINT32                            INVALIDATION:1;
    UINT32                            PAGE_REQUEST:1;
    UINT32                            STATUS:5;
    UINT32                            PAGE_ADDR_HIGH:4;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} ATC_ATS_FAULT_STATUS_INFO_STRUCT;

#define SMN_ATC_ATS_FAULT_STATUS_INFO_ADDRESS    0x306cUL


/***********************************************************
* Register Name : ATC_ATS_FAULT_STATUS_INFO2
* Register Description :
* This register reports the status of the first fault after power-on or the last explicit clear of the register. ATC_ATS_FAULT_CNTL and ATC_ATS_FAULT_DEBUG control the types of data that get collected here.
* Visibility : 0x2
************************************************************/

// Bitfield Description : VF of fault for DUPLICATED ITAG, NUM_REQUESTS_EXCEEDED or FED_BIT on page requests or invalidation requests. Else Page Request Group where appropriate. This register is valid only if GPU virtualization is enabled.
#define ATC_ATS_FAULT_STATUS_INFO2_VF_OFFSET      0
#define ATC_ATS_FAULT_STATUS_INFO2_VF_MASK        0x1

// Bitfield Description : VFID of fault for DUPLICATED ITAG, NUM_REQUESTS_EXCEEDED or FED_BIT on page requests or invalidation requests. Else Page Request Group where appropriate. This register is valid only if GPU virtualization is enabled.
#define ATC_ATS_FAULT_STATUS_INFO2_VFID_OFFSET      1
#define ATC_ATS_FAULT_STATUS_INFO2_VFID_MASK        0x3e

// Bitfield Description : 
#define ATC_ATS_FAULT_STATUS_INFO2_Reserved_8_6_OFFSET      6
#define ATC_ATS_FAULT_STATUS_INFO2_Reserved_8_6_MASK        0x1c0

// Bitfield Description : {whether_invalidate_mmhub[0], invalidate_mmhub_vmid[3:0]}
#define ATC_ATS_FAULT_STATUS_INFO2_MMHUB_INV_VMID_OFFSET      9
#define ATC_ATS_FAULT_STATUS_INFO2_MMHUB_INV_VMID_MASK        0x3e00

// Bitfield Description : 
#define ATC_ATS_FAULT_STATUS_INFO2_Reserved_31_14_OFFSET      14
#define ATC_ATS_FAULT_STATUS_INFO2_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            VF:1;
    UINT32                            VFID:5;
    UINT32                            Reserved_8_6:3;
    UINT32                            MMHUB_INV_VMID:5;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} ATC_ATS_FAULT_STATUS_INFO2_STRUCT;

#define SMN_ATC_ATS_FAULT_STATUS_INFO2_ADDRESS    0x3074UL


/***********************************************************
* Register Name : ATC_ATS_GFX_ATCL2_STATUS
* Register Description :
* This register provides some status info by SMU.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is set to 1 by RLC/SMU when RLC/SMU powers down VDDGFX (GFX ATCL2 is in VDDGFX power domain). Once RLC/SMU powers up VDDGFX, RLC/SMU should clear this register to 0. If this register is set, ATCAGGR will not send invalidation request to GFX ATCL2 because GFX ATCL2 is powered down. If there is already outstanding invalidation request in GFX ATCL2 when this bit is set, AGGR will complete the invalidation ack on behalf of GFX ATCL2.
#define ATC_ATS_GFX_ATCL2_STATUS_POWERED_DOWN_OFFSET      0
#define ATC_ATS_GFX_ATCL2_STATUS_POWERED_DOWN_MASK        0x1

// Bitfield Description : 
#define ATC_ATS_GFX_ATCL2_STATUS_Reserved_31_1_OFFSET      1
#define ATC_ATS_GFX_ATCL2_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            POWERED_DOWN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ATC_ATS_GFX_ATCL2_STATUS_STRUCT;

#define SMN_ATC_ATS_GFX_ATCL2_STATUS_ADDRESS    0x3064UL


/***********************************************************
* Register Name : ATC_ATS_MISC_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : when enabled, ATS grouping 8*4k PTE to 32k PTE to MMHUB ATCL2 for host requests
#define ATC_ATS_MISC_CNTL_MM_32K_GROUPING_EN_HOST_OFFSET      0
#define ATC_ATS_MISC_CNTL_MM_32K_GROUPING_EN_HOST_MASK        0x1

// Bitfield Description : when enabled, ATS grouping 8*4k PTE to 32k PTE to GFX ATCL2 for host requests
#define ATC_ATS_MISC_CNTL_GFX_32K_GROUPING_EN_HOST_OFFSET      1
#define ATC_ATS_MISC_CNTL_GFX_32K_GROUPING_EN_HOST_MASK        0x2

// Bitfield Description : when enabled, ATS grouping 8*4k PTE to 32k PTE to MMHUB ATCL2 for guest requests
#define ATC_ATS_MISC_CNTL_MM_32K_GROUPING_EN_GUEST_OFFSET      2
#define ATC_ATS_MISC_CNTL_MM_32K_GROUPING_EN_GUEST_MASK        0x4

// Bitfield Description : when enabled, ATS grouping 8*4k PTE to 32k PTE to GFX ATCL2 for guest requests
#define ATC_ATS_MISC_CNTL_GFX_32K_GROUPING_EN_GUEST_OFFSET      3
#define ATC_ATS_MISC_CNTL_GFX_32K_GROUPING_EN_GUEST_MASK        0x8

// Bitfield Description : When set, trans response with stat=RETRY, R=W=EX=0 if invalidation collision for host requests
#define ATC_ATS_MISC_CNTL_TRANS_RESP_IN_INV_COLLISION_HOST_OFFSET      4
#define ATC_ATS_MISC_CNTL_TRANS_RESP_IN_INV_COLLISION_HOST_MASK        0x10

// Bitfield Description : If set, trans response with stat=RETRY, R=W=EX=0 if invalidation collision for guest requests
#define ATC_ATS_MISC_CNTL_TRANS_RESP_IN_INV_COLLISION_GUEST_OFFSET      5
#define ATC_ATS_MISC_CNTL_TRANS_RESP_IN_INV_COLLISION_GUEST_MASK        0x20

// Bitfield Description : reserved fields
#define ATC_ATS_MISC_CNTL_Reserved_31_6_OFFSET      6
#define ATC_ATS_MISC_CNTL_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            MM_32K_GROUPING_EN_HOST:1;
    UINT32                            GFX_32K_GROUPING_EN_HOST:1;
    UINT32                            MM_32K_GROUPING_EN_GUEST:1;
    UINT32                            GFX_32K_GROUPING_EN_GUEST:1;
    UINT32                            TRANS_RESP_IN_INV_COLLISION_HOST:1;
    UINT32                            TRANS_RESP_IN_INV_COLLISION_GUEST:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} ATC_ATS_MISC_CNTL_STRUCT;

#define SMN_ATC_ATS_MISC_CNTL_ADDRESS    0x303cUL


/***********************************************************
* Register Name : ATC_ATS_MMHUB_ATCL2_STATUS
* Register Description :
* This register provides some status info by SMU.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is set to 1 by RLC/SMU when RLC/SMU powers down VDDMMHUB (MMHUB ATCL2 is in VDDMMHUB power domain). Once RLC/SMU powers up VDDMMHUB, RLC/SMU should clear this register to 0. If this register is set, ATCAGGR will not send invalidation request to MMHUB ATCL2 because MMHUB ATCL2 is powered down. If there is already outstanding invalidation request in MMHUB ATCL2 when this bit is set, AGGR will complete the invalidation ack on behalf of MMHUB ATCL2.
#define ATC_ATS_MMHUB_ATCL2_STATUS_POWERED_DOWN_OFFSET      0
#define ATC_ATS_MMHUB_ATCL2_STATUS_POWERED_DOWN_MASK        0x1

// Bitfield Description : 
#define ATC_ATS_MMHUB_ATCL2_STATUS_Reserved_31_1_OFFSET      1
#define ATC_ATS_MMHUB_ATCL2_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            POWERED_DOWN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} ATC_ATS_MMHUB_ATCL2_STATUS_STRUCT;

#define SMN_ATC_ATS_MMHUB_ATCL2_STATUS_ADDRESS    0x3068UL


/***********************************************************
* Register Name : ATC_ATS_SDPPORT_CNTL
* Register Description :
* this register provides controls for the sdp handshake parameters
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_SELF_ACTIVATE_OFFSET      0
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_SELF_ACTIVATE_MASK        0x1

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_CFG_MODE_OFFSET      1
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_CFG_MODE_MASK        0x6

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_HALT_THRESHOLD_OFFSET      3
#define ATC_ATS_SDPPORT_CNTL_ATS_INV_HALT_THRESHOLD_MASK        0x78

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_SELF_ACTIVATE_OFFSET      7
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_SELF_ACTIVATE_MASK        0x80

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_QUICK_COMACK_OFFSET      8
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_QUICK_COMACK_MASK        0x100

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_HALT_THRESHOLD_OFFSET      9
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_HALT_THRESHOLD_MASK        0x1e00

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_PASSIVE_MODE_OFFSET      13
#define ATC_ATS_SDPPORT_CNTL_UTCL2_TRANS_PASSIVE_MODE_MASK        0x2000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_RDY_MODE_OFFSET      14
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_RDY_MODE_MASK        0x4000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_MMHUB_RDY_MODE_OFFSET      15
#define ATC_ATS_SDPPORT_CNTL_UTCL2_MMHUB_RDY_MODE_MASK        0x8000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPCKEN_OFFSET      16
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPCKEN_MASK        0x10000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPCKENRCV_OFFSET      17
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPCKENRCV_MASK        0x20000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPDATACKEN_OFFSET      18
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPDATACKEN_MASK        0x40000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPDATACKENRCV_OFFSET      19
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_RDRSPDATACKENRCV_MASK        0x80000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_WRRSPCKEN_OFFSET      20
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_WRRSPCKEN_MASK        0x100000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_WRRSPCKENRCV_OFFSET      21
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_WRRSPCKENRCV_MASK        0x200000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_REQCKEN_OFFSET      22
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_REQCKEN_MASK        0x400000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_REQCKENRCV_OFFSET      23
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_REQCKENRCV_MASK        0x800000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_ORIGDATACKEN_OFFSET      24
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_ORIGDATACKEN_MASK        0x1000000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_ORIGDATACKENRCV_OFFSET      25
#define ATC_ATS_SDPPORT_CNTL_UTCL2_GFX_SDPVDCI_ORIGDATACKENRCV_MASK        0x2000000

// Bitfield Description : 
#define ATC_ATS_SDPPORT_CNTL_Reserved_31_26_OFFSET      26
#define ATC_ATS_SDPPORT_CNTL_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            ATS_INV_SELF_ACTIVATE:1;
    UINT32                            ATS_INV_CFG_MODE:2;
    UINT32                            ATS_INV_HALT_THRESHOLD:4;
    UINT32                            UTCL2_TRANS_SELF_ACTIVATE:1;
    UINT32                            UTCL2_TRANS_QUICK_COMACK:1;
    UINT32                            UTCL2_TRANS_HALT_THRESHOLD:4;
    UINT32                            UTCL2_TRANS_PASSIVE_MODE:1;
    UINT32                            UTCL2_GFX_RDY_MODE:1;
    UINT32                            UTCL2_MMHUB_RDY_MODE:1;
    UINT32                            UTCL2_GFX_SDPVDCI_RDRSPCKEN:1;
    UINT32                            UTCL2_GFX_SDPVDCI_RDRSPCKENRCV:1;
    UINT32                            UTCL2_GFX_SDPVDCI_RDRSPDATACKEN:1;
    UINT32                            UTCL2_GFX_SDPVDCI_RDRSPDATACKENRCV:1;
    UINT32                            UTCL2_GFX_SDPVDCI_WRRSPCKEN:1;
    UINT32                            UTCL2_GFX_SDPVDCI_WRRSPCKENRCV:1;
    UINT32                            UTCL2_GFX_SDPVDCI_REQCKEN:1;
    UINT32                            UTCL2_GFX_SDPVDCI_REQCKENRCV:1;
    UINT32                            UTCL2_GFX_SDPVDCI_ORIGDATACKEN:1;
    UINT32                            UTCL2_GFX_SDPVDCI_ORIGDATACKENRCV:1;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} ATC_ATS_SDPPORT_CNTL_STRUCT;

#define SMN_ATC_ATS_SDPPORT_CNTL_ADDRESS    0x302cUL


/***********************************************************
* Register Name : ATC_ATS_STATUS
* Register Description :
* Status for ATS Protocol Engine
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reports on whether the ATS is busy (1) or idle(0).
#define ATC_ATS_STATUS_BUSY_OFFSET      0
#define ATC_ATS_STATUS_BUSY_MASK        0x1

// Bitfield Description : Reports whether ATC L1 is crashed due to a fault.
#define ATC_ATS_STATUS_CRASHED_OFFSET      1
#define ATC_ATS_STATUS_CRASHED_MASK        0x2

// Bitfield Description : reports when outstanding transactions, but no returns in over 2^32 cycles
#define ATC_ATS_STATUS_DEADLOCK_DETECTION_OFFSET      2
#define ATC_ATS_STATUS_DEADLOCK_DETECTION_MASK        0x4

// Bitfield Description : The number of flush-type invalidation ats issued to aggr/utcl2.
#define ATC_ATS_STATUS_FLUSH_INVALIDATION_OUTSTANDING_OFFSET      3
#define ATC_ATS_STATUS_FLUSH_INVALIDATION_OUTSTANDING_MASK        0x38

// Bitfield Description : The number of non_flush-type invalidation ats issued to aggr/utcl2.
#define ATC_ATS_STATUS_NONFLUSH_INVALIDATION_OUTSTANDING_OFFSET      6
#define ATC_ATS_STATUS_NONFLUSH_INVALIDATION_OUTSTANDING_MASK        0x1c0

// Bitfield Description : poision data occur indication.
#define ATC_ATS_STATUS_FAULT_FED_IND_OFFSET      9
#define ATC_ATS_STATUS_FAULT_FED_IND_MASK        0x200

// Bitfield Description : STU fault occur indication.
#define ATC_ATS_STATUS_FAULT_STU_IND_OFFSET      10
#define ATC_ATS_STATUS_FAULT_STU_IND_MASK        0x400

// Bitfield Description : host error fault occur indication.
#define ATC_ATS_STATUS_FAULT_HOST_ERR_IND_OFFSET      11
#define ATC_ATS_STATUS_FAULT_HOST_ERR_IND_MASK        0x800

// Bitfield Description : physical address too large error fault occur indication.
#define ATC_ATS_STATUS_FAULT_PA_ERR_IND_OFFSET      12
#define ATC_ATS_STATUS_FAULT_PA_ERR_IND_MASK        0x1000

// Bitfield Description : UT error fault occur indication.
#define ATC_ATS_STATUS_FAULT_UBIT_ERR_IND_OFFSET      13
#define ATC_ATS_STATUS_FAULT_UBIT_ERR_IND_MASK        0x2000

// Bitfield Description : ATC disable fault occur indication.
#define ATC_ATS_STATUS_FAULT_ATC_DISABLE_ERR_IND_OFFSET      14
#define ATC_ATS_STATUS_FAULT_ATC_DISABLE_ERR_IND_MASK        0x4000

// Bitfield Description : ITAG fault occur indication.
#define ATC_ATS_STATUS_FAULT_ITAG_ERR_IND_OFFSET      15
#define ATC_ATS_STATUS_FAULT_ITAG_ERR_IND_MASK        0x8000

// Bitfield Description : non success fault occur indication.
#define ATC_ATS_STATUS_FAULT_NON_SUCCESS_ERR_IND_OFFSET      16
#define ATC_ATS_STATUS_FAULT_NON_SUCCESS_ERR_IND_MASK        0x10000

// Bitfield Description : BME fault occur indication.
#define ATC_ATS_STATUS_FAULT_BME_ERR_IND_OFFSET      17
#define ATC_ATS_STATUS_FAULT_BME_ERR_IND_MASK        0x20000

// Bitfield Description : 
#define ATC_ATS_STATUS_Reserved_31_18_OFFSET      18
#define ATC_ATS_STATUS_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            BUSY:1;
    UINT32                            CRASHED:1;
    UINT32                            DEADLOCK_DETECTION:1;
    UINT32                            FLUSH_INVALIDATION_OUTSTANDING:3;
    UINT32                            NONFLUSH_INVALIDATION_OUTSTANDING:3;
    UINT32                            FAULT_FED_IND:1;
    UINT32                            FAULT_STU_IND:1;
    UINT32                            FAULT_HOST_ERR_IND:1;
    UINT32                            FAULT_PA_ERR_IND:1;
    UINT32                            FAULT_UBIT_ERR_IND:1;
    UINT32                            FAULT_ATC_DISABLE_ERR_IND:1;
    UINT32                            FAULT_ITAG_ERR_IND:1;
    UINT32                            FAULT_NON_SUCCESS_ERR_IND:1;
    UINT32                            FAULT_BME_ERR_IND:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} ATC_ATS_STATUS_STRUCT;

#define SMN_ATC_ATS_STATUS_ADDRESS    0x3110UL


/***********************************************************
* Register Name : ATC_ATS_TR_QOS_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : the WQ credits for MM AT requests, max is 64
#define ATC_ATS_TR_QOS_CNTL_MM_TR_WQ_CREDITS_OFFSET      0
#define ATC_ATS_TR_QOS_CNTL_MM_TR_WQ_CREDITS_MASK        0xff

// Bitfield Description : the WQ credits for GFX AT requests, max is 64
#define ATC_ATS_TR_QOS_CNTL_GFX_TR_WQ_CREDITS_OFFSET      8
#define ATC_ATS_TR_QOS_CNTL_GFX_TR_WQ_CREDITS_MASK        0xff00

// Bitfield Description : the arbitration mode for GFX_AT and MM_AT into WQ, 0--round_robin, 1--GFX high, 2--MM high, 3--RATIO MODE, the default is MM is high
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_ARB_MODE_AGGR_OFFSET      16
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_ARB_MODE_AGGR_MASK        0x30000

// Bitfield Description : the ration when arbitration from the iFIFOs into WQs b/w GFX iFIFO and MM iFIFO,23:21--MM ration, 20:18--GFX ratio
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_RATIO_AGGR_OFFSET      18
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_RATIO_AGGR_MASK        0xfc0000

// Bitfield Description : the arbitration mode for GFX_AT and MM_AT in WQ to RPB, 0--round_robin, 1--GFX high, 2--MM high, 3--RATIO MODE, the default is MM is high
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_ARB_MODE_WQ_OFFSET      24
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_ARB_MODE_WQ_MASK        0x3000000

// Bitfield Description : the ration when arbitration from the WQ into RPB b/w GFX AT and MM AT,31:29--MM ration, 28:26--GFX ratio
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_RATIO_WQ_OFFSET      26
#define ATC_ATS_TR_QOS_CNTL_GFX_MM_TR_RATIO_WQ_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            MM_TR_WQ_CREDITS:8;
    UINT32                            GFX_TR_WQ_CREDITS:8;
    UINT32                            GFX_MM_TR_ARB_MODE_AGGR:2;
    UINT32                            GFX_MM_TR_RATIO_AGGR:6;
    UINT32                            GFX_MM_TR_ARB_MODE_WQ:2;
    UINT32                            GFX_MM_TR_RATIO_WQ:6;
  } Field;
  UINT32 Value;
} ATC_ATS_TR_QOS_CNTL_STRUCT;

#define SMN_ATC_ATS_TR_QOS_CNTL_ADDRESS    0x3038UL


/***********************************************************
* Register Name : ATC_ATS_VMID_SNAPSHOT_GFX_STAT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : The busy status for vmid = 0 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID0_OFFSET      0
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID0_MASK        0x1

// Bitfield Description : The busy status for vmid = 1 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID1_OFFSET      1
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID1_MASK        0x2

// Bitfield Description : The busy status for vmid = 2 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID2_OFFSET      2
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID2_MASK        0x4

// Bitfield Description : The busy status for vmid = 3 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID3_OFFSET      3
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID3_MASK        0x8

// Bitfield Description : The busy status for vmid = 4 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID4_OFFSET      4
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID4_MASK        0x10

// Bitfield Description : The busy status for vmid = 5 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID5_OFFSET      5
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID5_MASK        0x20

// Bitfield Description : The busy status for vmid = 6 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID6_OFFSET      6
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID6_MASK        0x40

// Bitfield Description : The busy status for vmid = 7 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID7_OFFSET      7
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID7_MASK        0x80

// Bitfield Description : The busy status for vmid = 8 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID8_OFFSET      8
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID8_MASK        0x100

// Bitfield Description : The busy status for vmid = 9 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID9_OFFSET      9
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID9_MASK        0x200

// Bitfield Description : The busy status for vmid = 10 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID10_OFFSET      10
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID10_MASK        0x400

// Bitfield Description : The busy status for vmid = 11 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID11_OFFSET      11
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID11_MASK        0x800

// Bitfield Description : The busy status for vmid = 12 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID12_OFFSET      12
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID12_MASK        0x1000

// Bitfield Description : The busy status for vmid = 13 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID13_OFFSET      13
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID13_MASK        0x2000

// Bitfield Description : The busy status for vmid = 14 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID14_OFFSET      14
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID14_MASK        0x4000

// Bitfield Description : The busy status for vmid = 15 for gfx after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID15_OFFSET      15
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_VMID15_MASK        0x8000

// Bitfield Description : 
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_Reserved_31_16_OFFSET      16
#define ATC_ATS_VMID_SNAPSHOT_GFX_STAT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VMID0:1;
    UINT32                            VMID1:1;
    UINT32                            VMID2:1;
    UINT32                            VMID3:1;
    UINT32                            VMID4:1;
    UINT32                            VMID5:1;
    UINT32                            VMID6:1;
    UINT32                            VMID7:1;
    UINT32                            VMID8:1;
    UINT32                            VMID9:1;
    UINT32                            VMID10:1;
    UINT32                            VMID11:1;
    UINT32                            VMID12:1;
    UINT32                            VMID13:1;
    UINT32                            VMID14:1;
    UINT32                            VMID15:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} ATC_ATS_VMID_SNAPSHOT_GFX_STAT_STRUCT;

#define SMN_ATC_ATS_VMID_SNAPSHOT_GFX_STAT_ADDRESS    0x3114UL


/***********************************************************
* Register Name : ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : The busy status for vmid = 0 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID0_OFFSET      0
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID0_MASK        0x1

// Bitfield Description : The busy status for vmid = 1 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID1_OFFSET      1
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID1_MASK        0x2

// Bitfield Description : The busy status for vmid = 2 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID2_OFFSET      2
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID2_MASK        0x4

// Bitfield Description : The busy status for vmid = 3 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID3_OFFSET      3
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID3_MASK        0x8

// Bitfield Description : The busy status for vmid = 4 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID4_OFFSET      4
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID4_MASK        0x10

// Bitfield Description : The busy status for vmid = 5 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID5_OFFSET      5
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID5_MASK        0x20

// Bitfield Description : The busy status for vmid = 6 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID6_OFFSET      6
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID6_MASK        0x40

// Bitfield Description : The busy status for vmid = 7 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID7_OFFSET      7
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID7_MASK        0x80

// Bitfield Description : The busy status for vmid = 8 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID8_OFFSET      8
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID8_MASK        0x100

// Bitfield Description : The busy status for vmid = 9 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID9_OFFSET      9
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID9_MASK        0x200

// Bitfield Description : The busy status for vmid = 10 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID10_OFFSET      10
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID10_MASK        0x400

// Bitfield Description : The busy status for vmid = 11 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID11_OFFSET      11
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID11_MASK        0x800

// Bitfield Description : The busy status for vmid = 12 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID12_OFFSET      12
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID12_MASK        0x1000

// Bitfield Description : The busy status for vmid = 13 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID13_OFFSET      13
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID13_MASK        0x2000

// Bitfield Description : The busy status for vmid = 14 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID14_OFFSET      14
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID14_MASK        0x4000

// Bitfield Description : The busy status for vmid = 15 for mmhub after do snaptshot(writing value 1 to this bit, writing value 0 to this bit won't have effect)
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID15_OFFSET      15
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_VMID15_MASK        0x8000

// Bitfield Description : 
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_Reserved_31_16_OFFSET      16
#define ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VMID0:1;
    UINT32                            VMID1:1;
    UINT32                            VMID2:1;
    UINT32                            VMID3:1;
    UINT32                            VMID4:1;
    UINT32                            VMID5:1;
    UINT32                            VMID6:1;
    UINT32                            VMID7:1;
    UINT32                            VMID8:1;
    UINT32                            VMID9:1;
    UINT32                            VMID10:1;
    UINT32                            VMID11:1;
    UINT32                            VMID12:1;
    UINT32                            VMID13:1;
    UINT32                            VMID14:1;
    UINT32                            VMID15:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_STRUCT;

#define SMN_ATC_ATS_VMID_SNAPSHOT_MMHUB_STAT_ADDRESS    0x3118UL


/***********************************************************
* Register Name : ATC_ATS_VMID_STATUS
* Register Description :
* This register provides VMID busy or idle status.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID0_OUTSTANDING_OFFSET      0
#define ATC_ATS_VMID_STATUS_VMID0_OUTSTANDING_MASK        0x1

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID1_OUTSTANDING_OFFSET      1
#define ATC_ATS_VMID_STATUS_VMID1_OUTSTANDING_MASK        0x2

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID2_OUTSTANDING_OFFSET      2
#define ATC_ATS_VMID_STATUS_VMID2_OUTSTANDING_MASK        0x4

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID3_OUTSTANDING_OFFSET      3
#define ATC_ATS_VMID_STATUS_VMID3_OUTSTANDING_MASK        0x8

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID4_OUTSTANDING_OFFSET      4
#define ATC_ATS_VMID_STATUS_VMID4_OUTSTANDING_MASK        0x10

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID5_OUTSTANDING_OFFSET      5
#define ATC_ATS_VMID_STATUS_VMID5_OUTSTANDING_MASK        0x20

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID6_OUTSTANDING_OFFSET      6
#define ATC_ATS_VMID_STATUS_VMID6_OUTSTANDING_MASK        0x40

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID7_OUTSTANDING_OFFSET      7
#define ATC_ATS_VMID_STATUS_VMID7_OUTSTANDING_MASK        0x80

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID8_OUTSTANDING_OFFSET      8
#define ATC_ATS_VMID_STATUS_VMID8_OUTSTANDING_MASK        0x100

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID9_OUTSTANDING_OFFSET      9
#define ATC_ATS_VMID_STATUS_VMID9_OUTSTANDING_MASK        0x200

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID10_OUTSTANDING_OFFSET      10
#define ATC_ATS_VMID_STATUS_VMID10_OUTSTANDING_MASK        0x400

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID11_OUTSTANDING_OFFSET      11
#define ATC_ATS_VMID_STATUS_VMID11_OUTSTANDING_MASK        0x800

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID12_OUTSTANDING_OFFSET      12
#define ATC_ATS_VMID_STATUS_VMID12_OUTSTANDING_MASK        0x1000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID13_OUTSTANDING_OFFSET      13
#define ATC_ATS_VMID_STATUS_VMID13_OUTSTANDING_MASK        0x2000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID14_OUTSTANDING_OFFSET      14
#define ATC_ATS_VMID_STATUS_VMID14_OUTSTANDING_MASK        0x4000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for GFX 0~15
#define ATC_ATS_VMID_STATUS_VMID15_OUTSTANDING_OFFSET      15
#define ATC_ATS_VMID_STATUS_VMID15_OUTSTANDING_MASK        0x8000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID16_OUTSTANDING_OFFSET      16
#define ATC_ATS_VMID_STATUS_VMID16_OUTSTANDING_MASK        0x10000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID17_OUTSTANDING_OFFSET      17
#define ATC_ATS_VMID_STATUS_VMID17_OUTSTANDING_MASK        0x20000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID18_OUTSTANDING_OFFSET      18
#define ATC_ATS_VMID_STATUS_VMID18_OUTSTANDING_MASK        0x40000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID19_OUTSTANDING_OFFSET      19
#define ATC_ATS_VMID_STATUS_VMID19_OUTSTANDING_MASK        0x80000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID20_OUTSTANDING_OFFSET      20
#define ATC_ATS_VMID_STATUS_VMID20_OUTSTANDING_MASK        0x100000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID21_OUTSTANDING_OFFSET      21
#define ATC_ATS_VMID_STATUS_VMID21_OUTSTANDING_MASK        0x200000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID22_OUTSTANDING_OFFSET      22
#define ATC_ATS_VMID_STATUS_VMID22_OUTSTANDING_MASK        0x400000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID23_OUTSTANDING_OFFSET      23
#define ATC_ATS_VMID_STATUS_VMID23_OUTSTANDING_MASK        0x800000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID24_OUTSTANDING_OFFSET      24
#define ATC_ATS_VMID_STATUS_VMID24_OUTSTANDING_MASK        0x1000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID25_OUTSTANDING_OFFSET      25
#define ATC_ATS_VMID_STATUS_VMID25_OUTSTANDING_MASK        0x2000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID26_OUTSTANDING_OFFSET      26
#define ATC_ATS_VMID_STATUS_VMID26_OUTSTANDING_MASK        0x4000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID27_OUTSTANDING_OFFSET      27
#define ATC_ATS_VMID_STATUS_VMID27_OUTSTANDING_MASK        0x8000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID28_OUTSTANDING_OFFSET      28
#define ATC_ATS_VMID_STATUS_VMID28_OUTSTANDING_MASK        0x10000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID29_OUTSTANDING_OFFSET      29
#define ATC_ATS_VMID_STATUS_VMID29_OUTSTANDING_MASK        0x20000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID30_OUTSTANDING_OFFSET      30
#define ATC_ATS_VMID_STATUS_VMID30_OUTSTANDING_MASK        0x40000000

// Bitfield Description : Reports whether VMID is busy or idle: 1=busy, 0=idle, it's for MMHUB 0~15
#define ATC_ATS_VMID_STATUS_VMID31_OUTSTANDING_OFFSET      31
#define ATC_ATS_VMID_STATUS_VMID31_OUTSTANDING_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VMID0_OUTSTANDING:1;
    UINT32                            VMID1_OUTSTANDING:1;
    UINT32                            VMID2_OUTSTANDING:1;
    UINT32                            VMID3_OUTSTANDING:1;
    UINT32                            VMID4_OUTSTANDING:1;
    UINT32                            VMID5_OUTSTANDING:1;
    UINT32                            VMID6_OUTSTANDING:1;
    UINT32                            VMID7_OUTSTANDING:1;
    UINT32                            VMID8_OUTSTANDING:1;
    UINT32                            VMID9_OUTSTANDING:1;
    UINT32                            VMID10_OUTSTANDING:1;
    UINT32                            VMID11_OUTSTANDING:1;
    UINT32                            VMID12_OUTSTANDING:1;
    UINT32                            VMID13_OUTSTANDING:1;
    UINT32                            VMID14_OUTSTANDING:1;
    UINT32                            VMID15_OUTSTANDING:1;
    UINT32                            VMID16_OUTSTANDING:1;
    UINT32                            VMID17_OUTSTANDING:1;
    UINT32                            VMID18_OUTSTANDING:1;
    UINT32                            VMID19_OUTSTANDING:1;
    UINT32                            VMID20_OUTSTANDING:1;
    UINT32                            VMID21_OUTSTANDING:1;
    UINT32                            VMID22_OUTSTANDING:1;
    UINT32                            VMID23_OUTSTANDING:1;
    UINT32                            VMID24_OUTSTANDING:1;
    UINT32                            VMID25_OUTSTANDING:1;
    UINT32                            VMID26_OUTSTANDING:1;
    UINT32                            VMID27_OUTSTANDING:1;
    UINT32                            VMID28_OUTSTANDING:1;
    UINT32                            VMID29_OUTSTANDING:1;
    UINT32                            VMID30_OUTSTANDING:1;
    UINT32                            VMID31_OUTSTANDING:1;
  } Field;
  UINT32 Value;
} ATC_ATS_VMID_STATUS_STRUCT;

#define SMN_ATC_ATS_VMID_STATUS_ADDRESS    0x310cUL


/***********************************************************
* Register Name : ATC_PERFCOUNTER0_CFG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. ATS has 23 events. These events are event 1 = event start for statistical min sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 2 = event end for statistical min sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 3 = event start for statistical min sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 4 = event end for statistical min sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 5 = event start for statistical max sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 6 = event end for statistical max sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 7 = event start for statistical max sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 8 = event end for statistical max sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 9 = event start for max sclks from ATS_L2 invalidate request to L2_ATS invalidate ack event 10 = event end for max sclks from ATS_L2 invalidate request to L2_ATS invalidate ack event 11 = number of requests from L2 event 12 = number of first time trans successes event 13 = number of pri requests event 14 = number of trans failures event 15 = number of pri requests not satisfied event 16 = number of invalidate requests w/PASID, to get a correct number event 17 = number of invalidate requests w/out PASID event 18 = number of PASID changes event 19 = number of times 1 translations returned event 20 = number of times 2-8 translations returned not blocked by invalidations event 21 = number of times 5-8 translations returned not blocked by invalidations event 22 = number of times size bit was set on non speculative transactions returned to L2 (>4K translation was returned) event 23 = number of invalidate requests that matched multiple vmids event
#define ATC_PERFCOUNTER0_CFG_PERF_SEL_OFFSET      0
#define ATC_PERFCOUNTER0_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define ATC_PERFCOUNTER0_CFG_PERF_SEL_END_OFFSET      8
#define ATC_PERFCOUNTER0_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define ATC_PERFCOUNTER0_CFG_Reserved_23_16_OFFSET      16
#define ATC_PERFCOUNTER0_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define ATC_PERFCOUNTER0_CFG_PERF_MODE_OFFSET      24
#define ATC_PERFCOUNTER0_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : 
#define ATC_PERFCOUNTER0_CFG_ENABLE_OFFSET      28
#define ATC_PERFCOUNTER0_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : 
#define ATC_PERFCOUNTER0_CFG_CLEAR_OFFSET      29
#define ATC_PERFCOUNTER0_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define ATC_PERFCOUNTER0_CFG_Reserved_31_30_OFFSET      30
#define ATC_PERFCOUNTER0_CFG_Reserved_31_30_MASK        0xc0000000

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
} ATC_PERFCOUNTER0_CFG_STRUCT;

#define SMN_ATC_PERFCOUNTER0_CFG_ADDRESS    0x3040UL


/***********************************************************
* Register Name : ATC_PERFCOUNTER1_CFG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : A value of 0 means count cycles, otherwise selects event to count. When PERF_MODE is one of the latency counters this is the event that is used to indicate the start. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated. Latency counters do not begin until the start trigger has been triggered. ATC has 37 events. Among them, events 1-23 belong to ATCATS; events 24-29 belong to ATCL2; events 30-33 belong to ATCL1RD; and events 33-37 belong to ATCL1WR. These events are event 1 = event start for statistical min sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 2 = event end for statistical min sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 3 = event start for statistical min sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 4 = event end for statistical min sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 5 = event start for statistical max sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 6 = event end for statistical max sclks from ATC_RPB_req translation request to RPB_ATC_ret completion event 7 = event start for statistical max sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 8 = event end for statistical max sclks from ATC_RPB_req page request to RPB_ATC_ret page return event 9 = event start for max sclks from ATS_L2 invalidate request to L2_ATS invalidate ack event 10 = event end for max sclks from ATS_L2 invalidate request to L2_ATS invalidate ack event 11 = number of requests from L2 event 12 = number of first time trans successes event 13 = number of pri requests event 14 = number of trans failures event 15 = number of pri requests not satisfied event 16 = number of invalidate requests w/PASID, to get a correct number event 17 = number of invalidate requests w/out PASID event 18 = number of PASID changes event 19 = number of times 1 translations returned event 20 = number of times 2-8 translations returned not blocked by invalidations event 21 = number of times 5-8 translations returned not blocked by invalidations event 22 = number of times size bit was set on non speculative transactions returned to L2 (>4K translation was returned) event 23 = number of invalidate requests that matched multiple vmids event 24 = number of L1 requests event 25 = number of bank0 requests event 26 = number of bank0 hits event 27 = number of bank0 misses event 28 = number of ATS Requests event 29 = number of ATS Responses event 30 = number of L1RD TLB requests, these are the transactions that get translated event 31 = number of L1RD TLB hits event 32 = number of L1RD TLB misses event 33 = number of L1RD TLB bypasses (need_ats==0) event 34 = number of L1WR TLB requests, these are the transactions that get translated event 35 = number of L1WR TLB hits event 36 = number of L1WR TLB misses event 37 = number of L1WR TLB bypasses (need_ats==0)
#define ATC_PERFCOUNTER1_CFG_PERF_SEL_OFFSET      0
#define ATC_PERFCOUNTER1_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : When PERF_MODE is one of the latency counters this is the event that is used to indicate the end. Note that when counting latency it is expected that there is always an end event before another start event is seen, and there is always one and only one start event for every end event. Behavior is undefined if these are violated.
#define ATC_PERFCOUNTER1_CFG_PERF_SEL_END_OFFSET      8
#define ATC_PERFCOUNTER1_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define ATC_PERFCOUNTER1_CFG_Reserved_23_16_OFFSET      16
#define ATC_PERFCOUNTER1_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define ATC_PERFCOUNTER1_CFG_PERF_MODE_OFFSET      24
#define ATC_PERFCOUNTER1_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : 
#define ATC_PERFCOUNTER1_CFG_ENABLE_OFFSET      28
#define ATC_PERFCOUNTER1_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : 
#define ATC_PERFCOUNTER1_CFG_CLEAR_OFFSET      29
#define ATC_PERFCOUNTER1_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define ATC_PERFCOUNTER1_CFG_Reserved_31_30_OFFSET      30
#define ATC_PERFCOUNTER1_CFG_Reserved_31_30_MASK        0xc0000000

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
} ATC_PERFCOUNTER1_CFG_STRUCT;

#define SMN_ATC_PERFCOUNTER1_CFG_ADDRESS    0x3044UL


/***********************************************************
* Register Name : ATC_PERFCOUNTER2_CFG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_PERF_SEL_OFFSET      0
#define ATC_PERFCOUNTER2_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_PERF_SEL_END_OFFSET      8
#define ATC_PERFCOUNTER2_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_Reserved_23_16_OFFSET      16
#define ATC_PERFCOUNTER2_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_PERF_MODE_OFFSET      24
#define ATC_PERFCOUNTER2_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_ENABLE_OFFSET      28
#define ATC_PERFCOUNTER2_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_CLEAR_OFFSET      29
#define ATC_PERFCOUNTER2_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define ATC_PERFCOUNTER2_CFG_Reserved_31_30_OFFSET      30
#define ATC_PERFCOUNTER2_CFG_Reserved_31_30_MASK        0xc0000000

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
} ATC_PERFCOUNTER2_CFG_STRUCT;

#define SMN_ATC_PERFCOUNTER2_CFG_ADDRESS    0x3048UL


/***********************************************************
* Register Name : ATC_PERFCOUNTER3_CFG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_PERF_SEL_OFFSET      0
#define ATC_PERFCOUNTER3_CFG_PERF_SEL_MASK        0xff

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_PERF_SEL_END_OFFSET      8
#define ATC_PERFCOUNTER3_CFG_PERF_SEL_END_MASK        0xff00

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_Reserved_23_16_OFFSET      16
#define ATC_PERFCOUNTER3_CFG_Reserved_23_16_MASK        0xff0000

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_PERF_MODE_OFFSET      24
#define ATC_PERFCOUNTER3_CFG_PERF_MODE_MASK        0xf000000

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_ENABLE_OFFSET      28
#define ATC_PERFCOUNTER3_CFG_ENABLE_MASK        0x10000000

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_CLEAR_OFFSET      29
#define ATC_PERFCOUNTER3_CFG_CLEAR_MASK        0x20000000

// Bitfield Description : 
#define ATC_PERFCOUNTER3_CFG_Reserved_31_30_OFFSET      30
#define ATC_PERFCOUNTER3_CFG_Reserved_31_30_MASK        0xc0000000

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
} ATC_PERFCOUNTER3_CFG_STRUCT;

#define SMN_ATC_PERFCOUNTER3_CFG_ADDRESS    0x304cUL


/***********************************************************
* Register Name : ATC_PERFCOUNTER_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_PERFCOUNTER_HI_COUNTER_HI_OFFSET      0
#define ATC_PERFCOUNTER_HI_COUNTER_HI_MASK        0xffff

// Bitfield Description : 
#define ATC_PERFCOUNTER_HI_COMPARE_VALUE_OFFSET      16
#define ATC_PERFCOUNTER_HI_COMPARE_VALUE_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            COUNTER_HI:16;
    UINT32                            COMPARE_VALUE:16;
  } Field;
  UINT32 Value;
} ATC_PERFCOUNTER_HI_STRUCT;

#define SMN_ATC_PERFCOUNTER_HI_ADDRESS    0x3058UL


/***********************************************************
* Register Name : ATC_PERFCOUNTER_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_PERFCOUNTER_LO_COUNTER_LO_OFFSET      0
#define ATC_PERFCOUNTER_LO_COUNTER_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER_LO:32;
  } Field;
  UINT32 Value;
} ATC_PERFCOUNTER_LO_STRUCT;

#define SMN_ATC_PERFCOUNTER_LO_ADDRESS    0x3054UL


/***********************************************************
* Register Name : ATC_PERFCOUNTER_RSLT_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_PERF_COUNTER_SELECT_OFFSET      0
#define ATC_PERFCOUNTER_RSLT_CNTL_PERF_COUNTER_SELECT_MASK        0xf

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_Reserved_7_4_OFFSET      4
#define ATC_PERFCOUNTER_RSLT_CNTL_Reserved_7_4_MASK        0xf0

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_START_TRIGGER_OFFSET      8
#define ATC_PERFCOUNTER_RSLT_CNTL_START_TRIGGER_MASK        0xff00

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_STOP_TRIGGER_OFFSET      16
#define ATC_PERFCOUNTER_RSLT_CNTL_STOP_TRIGGER_MASK        0xff0000

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_ENABLE_ANY_OFFSET      24
#define ATC_PERFCOUNTER_RSLT_CNTL_ENABLE_ANY_MASK        0x1000000

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_CLEAR_ALL_OFFSET      25
#define ATC_PERFCOUNTER_RSLT_CNTL_CLEAR_ALL_MASK        0x2000000

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_STOP_ALL_ON_SATURATE_OFFSET      26
#define ATC_PERFCOUNTER_RSLT_CNTL_STOP_ALL_ON_SATURATE_MASK        0x4000000

// Bitfield Description : 
#define ATC_PERFCOUNTER_RSLT_CNTL_Reserved_31_27_OFFSET      27
#define ATC_PERFCOUNTER_RSLT_CNTL_Reserved_31_27_MASK        0xf8000000

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
} ATC_PERFCOUNTER_RSLT_CNTL_STRUCT;

#define SMN_ATC_PERFCOUNTER_RSLT_CNTL_ADDRESS    0x3050UL


/***********************************************************
* Register Name : ATC_TRANS_FAULT_RSPCNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 0 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID0_OFFSET      0
#define ATC_TRANS_FAULT_RSPCNTRL_VMID0_MASK        0x1

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 1 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID1_OFFSET      1
#define ATC_TRANS_FAULT_RSPCNTRL_VMID1_MASK        0x2

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 2 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID2_OFFSET      2
#define ATC_TRANS_FAULT_RSPCNTRL_VMID2_MASK        0x4

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 3 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID3_OFFSET      3
#define ATC_TRANS_FAULT_RSPCNTRL_VMID3_MASK        0x8

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 4 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID4_OFFSET      4
#define ATC_TRANS_FAULT_RSPCNTRL_VMID4_MASK        0x10

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 5 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID5_OFFSET      5
#define ATC_TRANS_FAULT_RSPCNTRL_VMID5_MASK        0x20

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 6 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID6_OFFSET      6
#define ATC_TRANS_FAULT_RSPCNTRL_VMID6_MASK        0x40

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 7 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID7_OFFSET      7
#define ATC_TRANS_FAULT_RSPCNTRL_VMID7_MASK        0x80

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 8 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID8_OFFSET      8
#define ATC_TRANS_FAULT_RSPCNTRL_VMID8_MASK        0x100

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 9 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID9_OFFSET      9
#define ATC_TRANS_FAULT_RSPCNTRL_VMID9_MASK        0x200

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 10 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID10_OFFSET      10
#define ATC_TRANS_FAULT_RSPCNTRL_VMID10_MASK        0x400

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 11 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID11_OFFSET      11
#define ATC_TRANS_FAULT_RSPCNTRL_VMID11_MASK        0x800

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 12 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID12_OFFSET      12
#define ATC_TRANS_FAULT_RSPCNTRL_VMID12_MASK        0x1000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 13 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID13_OFFSET      13
#define ATC_TRANS_FAULT_RSPCNTRL_VMID13_MASK        0x2000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 14 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID14_OFFSET      14
#define ATC_TRANS_FAULT_RSPCNTRL_VMID14_MASK        0x4000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 15 for gfx, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID15_OFFSET      15
#define ATC_TRANS_FAULT_RSPCNTRL_VMID15_MASK        0x8000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 0 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID16_OFFSET      16
#define ATC_TRANS_FAULT_RSPCNTRL_VMID16_MASK        0x10000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 1 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID17_OFFSET      17
#define ATC_TRANS_FAULT_RSPCNTRL_VMID17_MASK        0x20000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 2 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID18_OFFSET      18
#define ATC_TRANS_FAULT_RSPCNTRL_VMID18_MASK        0x40000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 3 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID19_OFFSET      19
#define ATC_TRANS_FAULT_RSPCNTRL_VMID19_MASK        0x80000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 4 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID20_OFFSET      20
#define ATC_TRANS_FAULT_RSPCNTRL_VMID20_MASK        0x100000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 5 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID21_OFFSET      21
#define ATC_TRANS_FAULT_RSPCNTRL_VMID21_MASK        0x200000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 6 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID22_OFFSET      22
#define ATC_TRANS_FAULT_RSPCNTRL_VMID22_MASK        0x400000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 7 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID23_OFFSET      23
#define ATC_TRANS_FAULT_RSPCNTRL_VMID23_MASK        0x800000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 8 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID24_OFFSET      24
#define ATC_TRANS_FAULT_RSPCNTRL_VMID24_MASK        0x1000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 9 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID25_OFFSET      25
#define ATC_TRANS_FAULT_RSPCNTRL_VMID25_MASK        0x2000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 10 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID26_OFFSET      26
#define ATC_TRANS_FAULT_RSPCNTRL_VMID26_MASK        0x4000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 11 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID27_OFFSET      27
#define ATC_TRANS_FAULT_RSPCNTRL_VMID27_MASK        0x8000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 12 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID28_OFFSET      28
#define ATC_TRANS_FAULT_RSPCNTRL_VMID28_MASK        0x10000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 13 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID29_OFFSET      29
#define ATC_TRANS_FAULT_RSPCNTRL_VMID29_MASK        0x20000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 14 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID30_OFFSET      30
#define ATC_TRANS_FAULT_RSPCNTRL_VMID30_MASK        0x40000000

// Bitfield Description : What status to return to UTCL2 when fault happen for requests with vmid = 15 for mmhub, 0 means retry(_ret_stat = 1), 1 means fault(_ret_stat = 3)
#define ATC_TRANS_FAULT_RSPCNTRL_VMID31_OFFSET      31
#define ATC_TRANS_FAULT_RSPCNTRL_VMID31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VMID0:1;
    UINT32                            VMID1:1;
    UINT32                            VMID2:1;
    UINT32                            VMID3:1;
    UINT32                            VMID4:1;
    UINT32                            VMID5:1;
    UINT32                            VMID6:1;
    UINT32                            VMID7:1;
    UINT32                            VMID8:1;
    UINT32                            VMID9:1;
    UINT32                            VMID10:1;
    UINT32                            VMID11:1;
    UINT32                            VMID12:1;
    UINT32                            VMID13:1;
    UINT32                            VMID14:1;
    UINT32                            VMID15:1;
    UINT32                            VMID16:1;
    UINT32                            VMID17:1;
    UINT32                            VMID18:1;
    UINT32                            VMID19:1;
    UINT32                            VMID20:1;
    UINT32                            VMID21:1;
    UINT32                            VMID22:1;
    UINT32                            VMID23:1;
    UINT32                            VMID24:1;
    UINT32                            VMID25:1;
    UINT32                            VMID26:1;
    UINT32                            VMID27:1;
    UINT32                            VMID28:1;
    UINT32                            VMID29:1;
    UINT32                            VMID30:1;
    UINT32                            VMID31:1;
  } Field;
  UINT32 Value;
} ATC_TRANS_FAULT_RSPCNTRL_STRUCT;

#define SMN_ATC_TRANS_FAULT_RSPCNTRL_ADDRESS    0x3020UL


/***********************************************************
* Register Name : ATC_VMID0_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID0 maps to. When Console-IOV mode, we will use this gourp registers as following: VFID[4:0], VF[7], NO_INVALIDATION[30]. VF = 0 means it's PF and VFID settings will be ignored, when VF = 1, VFID will be used to do remapping; the NO_INVALIDATION bit will act as same as normal mode(setting it will disable invalidation when writing this register)
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID0_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID0_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID0_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID0_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID0_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID0_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid0 is translated, it will be translated with a NULL PASID.
#define ATC_VMID0_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID0_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID0_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID0_PASID_MAPPING_ADDRESS    0x311cUL


/***********************************************************
* Register Name : ATC_VMID10_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID10 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID10_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID10_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID10_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID10_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID10_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID10_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid10 is translated, it will be translated with a NULL PASID.
#define ATC_VMID10_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID10_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID10_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID10_PASID_MAPPING_ADDRESS    0x3144UL


/***********************************************************
* Register Name : ATC_VMID11_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID11 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID11_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID11_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID11_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID11_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID11_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID11_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid11 is translated, it will be translated with a NULL PASID.
#define ATC_VMID11_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID11_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID11_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID11_PASID_MAPPING_ADDRESS    0x3148UL


/***********************************************************
* Register Name : ATC_VMID12_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID12 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID12_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID12_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID12_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID12_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID12_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID12_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid12 is translated, it will be translated with a NULL PASID.
#define ATC_VMID12_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID12_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID12_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID12_PASID_MAPPING_ADDRESS    0x314cUL


/***********************************************************
* Register Name : ATC_VMID13_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID13 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID13_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID13_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID13_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID13_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID13_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID13_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid13 is translated, it will be translated with a NULL PASID.
#define ATC_VMID13_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID13_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID13_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID13_PASID_MAPPING_ADDRESS    0x3150UL


/***********************************************************
* Register Name : ATC_VMID14_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID14 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID14_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID14_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID14_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID14_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID14_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID14_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid14 is translated, it will be translated with a NULL PASID.
#define ATC_VMID14_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID14_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID14_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID14_PASID_MAPPING_ADDRESS    0x3154UL


/***********************************************************
* Register Name : ATC_VMID15_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID15 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID15_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID15_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID15_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID15_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID15_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID15_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid15 is translated, it will be translated with a NULL PASID.
#define ATC_VMID15_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID15_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID15_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID15_PASID_MAPPING_ADDRESS    0x3158UL


/***********************************************************
* Register Name : ATC_VMID16_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID0 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID16_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID16_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID16_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID16_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID16_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID16_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid0 is translated, it will be translated with a NULL PASID.
#define ATC_VMID16_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID16_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID16_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID16_PASID_MAPPING_ADDRESS    0x315cUL


/***********************************************************
* Register Name : ATC_VMID17_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID1 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID17_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID17_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID17_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID17_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID17_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID17_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid1 is translated, it will be translated with a NULL PASID.
#define ATC_VMID17_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID17_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID17_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID17_PASID_MAPPING_ADDRESS    0x3160UL


/***********************************************************
* Register Name : ATC_VMID18_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID2 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID18_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID18_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID18_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID18_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID18_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID18_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid2 is translated, it will be translated with a NULL PASID.
#define ATC_VMID18_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID18_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID18_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID18_PASID_MAPPING_ADDRESS    0x3164UL


/***********************************************************
* Register Name : ATC_VMID19_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID3 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID19_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID19_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID19_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID19_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID19_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID19_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid3 is translated, it will be translated with a NULL PASID.
#define ATC_VMID19_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID19_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID19_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID19_PASID_MAPPING_ADDRESS    0x3168UL


/***********************************************************
* Register Name : ATC_VMID1_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID1 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID1_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID1_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID1_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID1_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID1_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID1_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid1 is translated, it will be translated with a NULL PASID.
#define ATC_VMID1_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID1_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID1_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID1_PASID_MAPPING_ADDRESS    0x3120UL


/***********************************************************
* Register Name : ATC_VMID20_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID4 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID20_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID20_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID20_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID20_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID20_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID20_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid4 is translated, it will be translated with a NULL PASID.
#define ATC_VMID20_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID20_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID20_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID20_PASID_MAPPING_ADDRESS    0x316cUL


/***********************************************************
* Register Name : ATC_VMID21_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID5 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID21_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID21_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID21_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID21_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID21_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID21_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid5 is translated, it will be translated with a NULL PASID.
#define ATC_VMID21_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID21_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID21_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID21_PASID_MAPPING_ADDRESS    0x3170UL


/***********************************************************
* Register Name : ATC_VMID22_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID6 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID22_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID22_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID22_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID22_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID22_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID22_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid6 is translated, it will be translated with a NULL PASID.
#define ATC_VMID22_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID22_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID22_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID22_PASID_MAPPING_ADDRESS    0x3174UL


/***********************************************************
* Register Name : ATC_VMID23_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID7 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID23_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID23_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID23_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID23_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID23_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID23_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid7 is translated, it will be translated with a NULL PASID.
#define ATC_VMID23_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID23_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID23_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID23_PASID_MAPPING_ADDRESS    0x3178UL


/***********************************************************
* Register Name : ATC_VMID24_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID8 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID24_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID24_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID24_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID24_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID24_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID24_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid8 is translated, it will be translated with a NULL PASID.
#define ATC_VMID24_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID24_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID24_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID24_PASID_MAPPING_ADDRESS    0x317cUL


/***********************************************************
* Register Name : ATC_VMID25_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID9 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID25_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID25_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID25_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID25_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID25_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID25_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid9 is translated, it will be translated with a NULL PASID.
#define ATC_VMID25_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID25_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID25_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID25_PASID_MAPPING_ADDRESS    0x3180UL


/***********************************************************
* Register Name : ATC_VMID26_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID10 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID26_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID26_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID26_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID26_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID26_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID26_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid10 is translated, it will be translated with a NULL PASID.
#define ATC_VMID26_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID26_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID26_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID26_PASID_MAPPING_ADDRESS    0x3184UL


/***********************************************************
* Register Name : ATC_VMID27_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID11 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID27_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID27_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID27_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID27_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID27_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID27_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid11 is translated, it will be translated with a NULL PASID.
#define ATC_VMID27_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID27_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID27_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID27_PASID_MAPPING_ADDRESS    0x3188UL


/***********************************************************
* Register Name : ATC_VMID28_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID12 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID28_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID28_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID28_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID28_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID28_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID28_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid12 is translated, it will be translated with a NULL PASID.
#define ATC_VMID28_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID28_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID28_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID28_PASID_MAPPING_ADDRESS    0x318cUL


/***********************************************************
* Register Name : ATC_VMID29_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID13 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID29_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID29_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID29_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID29_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID29_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID29_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid13 is translated, it will be translated with a NULL PASID.
#define ATC_VMID29_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID29_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID29_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID29_PASID_MAPPING_ADDRESS    0x3190UL


/***********************************************************
* Register Name : ATC_VMID2_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID2 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID2_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID2_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID2_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID2_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID2_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID2_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid2 is translated, it will be translated with a NULL PASID.
#define ATC_VMID2_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID2_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID2_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID2_PASID_MAPPING_ADDRESS    0x3124UL


/***********************************************************
* Register Name : ATC_VMID30_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID14 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID30_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID30_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID30_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID30_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID30_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID30_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid14 is translated, it will be translated with a NULL PASID.
#define ATC_VMID30_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID30_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID30_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID30_PASID_MAPPING_ADDRESS    0x3194UL


/***********************************************************
* Register Name : ATC_VMID31_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID15 maps to for mmhub
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID31_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID31_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID31_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID31_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID31_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID31_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid15 is translated, it will be translated with a NULL PASID.
#define ATC_VMID31_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID31_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID31_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID31_PASID_MAPPING_ADDRESS    0x3198UL


/***********************************************************
* Register Name : ATC_VMID3_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID3 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID3_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID3_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID3_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID3_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID3_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID3_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid3 is translated, it will be translated with a NULL PASID.
#define ATC_VMID3_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID3_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID3_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID3_PASID_MAPPING_ADDRESS    0x3128UL


/***********************************************************
* Register Name : ATC_VMID4_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID4 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID4_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID4_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID4_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID4_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID4_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID4_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid4 is translated, it will be translated with a NULL PASID.
#define ATC_VMID4_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID4_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID4_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID4_PASID_MAPPING_ADDRESS    0x312cUL


/***********************************************************
* Register Name : ATC_VMID5_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID5 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID5_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID5_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID5_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID5_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID5_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID5_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid5 is translated, it will be translated with a NULL PASID.
#define ATC_VMID5_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID5_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID5_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID5_PASID_MAPPING_ADDRESS    0x3130UL


/***********************************************************
* Register Name : ATC_VMID6_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID6 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID6_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID6_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID6_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID6_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID6_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID6_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid6 is translated, it will be translated with a NULL PASID.
#define ATC_VMID6_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID6_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID6_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID6_PASID_MAPPING_ADDRESS    0x3134UL


/***********************************************************
* Register Name : ATC_VMID7_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID7 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID7_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID7_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID7_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID7_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID7_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID7_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid7 is translated, it will be translated with a NULL PASID.
#define ATC_VMID7_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID7_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID7_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID7_PASID_MAPPING_ADDRESS    0x3138UL


/***********************************************************
* Register Name : ATC_VMID8_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID8 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID8_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID8_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID8_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID8_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID8_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID8_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid8 is translated, it will be translated with a NULL PASID.
#define ATC_VMID8_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID8_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID8_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID8_PASID_MAPPING_ADDRESS    0x313cUL


/***********************************************************
* Register Name : ATC_VMID9_PASID_MAPPING
* Register Description :
* This register defines which PASID VMID9 maps to
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID9_PASID_MAPPING_PASID_OFFSET      0
#define ATC_VMID9_PASID_MAPPING_PASID_MASK        0xffff

// Bitfield Description : 
#define ATC_VMID9_PASID_MAPPING_Reserved_29_16_OFFSET      16
#define ATC_VMID9_PASID_MAPPING_Reserved_29_16_MASK        0x3fff0000

// Bitfield Description : When writing a value of one to this register, ATS will not initiate invalidation requests to the ATC caches during VMID/PASID programming. This is only intended to be used by GMCON register restore-engine to tell ATC to provent ATS initiating invalidation while restoring VMID/PASID mapping table during GMC power-up sequence. Software should NEVER write one to the register during regular VMID/PASID mapping/remapping.
#define ATC_VMID9_PASID_MAPPING_NO_INVALIDATION_OFFSET      30
#define ATC_VMID9_PASID_MAPPING_NO_INVALIDATION_MASK        0x40000000

// Bitfield Description : When asserted indicates that the programmed PASID value is valid and may be used. If not valid, and a transaction with need_ats and Vmid9 is translated, it will be translated with a NULL PASID.
#define ATC_VMID9_PASID_MAPPING_VALID_OFFSET      31
#define ATC_VMID9_PASID_MAPPING_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PASID:16;
    UINT32                            Reserved_29_16:14;
    UINT32                            NO_INVALIDATION:1;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} ATC_VMID9_PASID_MAPPING_STRUCT;

#define SMN_ATC_VMID9_PASID_MAPPING_ADDRESS    0x3140UL


/***********************************************************
* Register Name : ATC_VMID_PASID_MAPPING_UPDATE_STATUS
* Register Description :
* This Registers indicates whether the VMID PASID remapping is complete The appropriate bit is cleared by hardware when ATC_VMID_PASID_MAPPING register is programmed or the bits can be cleared by software by writing a 1 to clear. Software may remap more than one VMID at a time, however hardware will issue them one at a time. If software wishes to remap a VMID a second time while the first remap of the same VMID is ongoing, the Remapping Finished register will not have the correct state.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID0_REMAPPING_FINISHED_OFFSET      0
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID0_REMAPPING_FINISHED_MASK        0x1

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID1_REMAPPING_FINISHED_OFFSET      1
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID1_REMAPPING_FINISHED_MASK        0x2

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID2_REMAPPING_FINISHED_OFFSET      2
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID2_REMAPPING_FINISHED_MASK        0x4

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID3_REMAPPING_FINISHED_OFFSET      3
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID3_REMAPPING_FINISHED_MASK        0x8

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID4_REMAPPING_FINISHED_OFFSET      4
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID4_REMAPPING_FINISHED_MASK        0x10

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID5_REMAPPING_FINISHED_OFFSET      5
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID5_REMAPPING_FINISHED_MASK        0x20

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID6_REMAPPING_FINISHED_OFFSET      6
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID6_REMAPPING_FINISHED_MASK        0x40

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID7_REMAPPING_FINISHED_OFFSET      7
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID7_REMAPPING_FINISHED_MASK        0x80

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID8_REMAPPING_FINISHED_OFFSET      8
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID8_REMAPPING_FINISHED_MASK        0x100

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID9_REMAPPING_FINISHED_OFFSET      9
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID9_REMAPPING_FINISHED_MASK        0x200

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID10_REMAPPING_FINISHED_OFFSET      10
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID10_REMAPPING_FINISHED_MASK        0x400

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID11_REMAPPING_FINISHED_OFFSET      11
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID11_REMAPPING_FINISHED_MASK        0x800

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID12_REMAPPING_FINISHED_OFFSET      12
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID12_REMAPPING_FINISHED_MASK        0x1000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID13_REMAPPING_FINISHED_OFFSET      13
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID13_REMAPPING_FINISHED_MASK        0x2000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID14_REMAPPING_FINISHED_OFFSET      14
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID14_REMAPPING_FINISHED_MASK        0x4000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID15_REMAPPING_FINISHED_OFFSET      15
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID15_REMAPPING_FINISHED_MASK        0x8000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID16_REMAPPING_FINISHED_OFFSET      16
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID16_REMAPPING_FINISHED_MASK        0x10000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID17_REMAPPING_FINISHED_OFFSET      17
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID17_REMAPPING_FINISHED_MASK        0x20000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID18_REMAPPING_FINISHED_OFFSET      18
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID18_REMAPPING_FINISHED_MASK        0x40000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID19_REMAPPING_FINISHED_OFFSET      19
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID19_REMAPPING_FINISHED_MASK        0x80000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID20_REMAPPING_FINISHED_OFFSET      20
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID20_REMAPPING_FINISHED_MASK        0x100000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID21_REMAPPING_FINISHED_OFFSET      21
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID21_REMAPPING_FINISHED_MASK        0x200000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID22_REMAPPING_FINISHED_OFFSET      22
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID22_REMAPPING_FINISHED_MASK        0x400000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID23_REMAPPING_FINISHED_OFFSET      23
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID23_REMAPPING_FINISHED_MASK        0x800000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID24_REMAPPING_FINISHED_OFFSET      24
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID24_REMAPPING_FINISHED_MASK        0x1000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID25_REMAPPING_FINISHED_OFFSET      25
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID25_REMAPPING_FINISHED_MASK        0x2000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID26_REMAPPING_FINISHED_OFFSET      26
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID26_REMAPPING_FINISHED_MASK        0x4000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID27_REMAPPING_FINISHED_OFFSET      27
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID27_REMAPPING_FINISHED_MASK        0x8000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID28_REMAPPING_FINISHED_OFFSET      28
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID28_REMAPPING_FINISHED_MASK        0x10000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID29_REMAPPING_FINISHED_OFFSET      29
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID29_REMAPPING_FINISHED_MASK        0x20000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID30_REMAPPING_FINISHED_OFFSET      30
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID30_REMAPPING_FINISHED_MASK        0x40000000

// Bitfield Description : 
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID31_REMAPPING_FINISHED_OFFSET      31
#define ATC_VMID_PASID_MAPPING_UPDATE_STATUS_VMID31_REMAPPING_FINISHED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VMID0_REMAPPING_FINISHED:1;
    UINT32                            VMID1_REMAPPING_FINISHED:1;
    UINT32                            VMID2_REMAPPING_FINISHED:1;
    UINT32                            VMID3_REMAPPING_FINISHED:1;
    UINT32                            VMID4_REMAPPING_FINISHED:1;
    UINT32                            VMID5_REMAPPING_FINISHED:1;
    UINT32                            VMID6_REMAPPING_FINISHED:1;
    UINT32                            VMID7_REMAPPING_FINISHED:1;
    UINT32                            VMID8_REMAPPING_FINISHED:1;
    UINT32                            VMID9_REMAPPING_FINISHED:1;
    UINT32                            VMID10_REMAPPING_FINISHED:1;
    UINT32                            VMID11_REMAPPING_FINISHED:1;
    UINT32                            VMID12_REMAPPING_FINISHED:1;
    UINT32                            VMID13_REMAPPING_FINISHED:1;
    UINT32                            VMID14_REMAPPING_FINISHED:1;
    UINT32                            VMID15_REMAPPING_FINISHED:1;
    UINT32                            VMID16_REMAPPING_FINISHED:1;
    UINT32                            VMID17_REMAPPING_FINISHED:1;
    UINT32                            VMID18_REMAPPING_FINISHED:1;
    UINT32                            VMID19_REMAPPING_FINISHED:1;
    UINT32                            VMID20_REMAPPING_FINISHED:1;
    UINT32                            VMID21_REMAPPING_FINISHED:1;
    UINT32                            VMID22_REMAPPING_FINISHED:1;
    UINT32                            VMID23_REMAPPING_FINISHED:1;
    UINT32                            VMID24_REMAPPING_FINISHED:1;
    UINT32                            VMID25_REMAPPING_FINISHED:1;
    UINT32                            VMID26_REMAPPING_FINISHED:1;
    UINT32                            VMID27_REMAPPING_FINISHED:1;
    UINT32                            VMID28_REMAPPING_FINISHED:1;
    UINT32                            VMID29_REMAPPING_FINISHED:1;
    UINT32                            VMID30_REMAPPING_FINISHED:1;
    UINT32                            VMID31_REMAPPING_FINISHED:1;
  } Field;
  UINT32 Value;
} ATC_VMID_PASID_MAPPING_UPDATE_STATUS_STRUCT;

#define SMN_ATC_VMID_PASID_MAPPING_UPDATE_STATUS_ADDRESS    0x3108UL


/***********************************************************
* Register Name : ATHUB_ATS_MODE_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_ATS_MODE_CNTL_HOST_TRANS_ENABLE_OFFSET      0
#define ATHUB_ATS_MODE_CNTL_HOST_TRANS_ENABLE_MASK        0x1

// Bitfield Description : To indicate whether we are in pseudo IOV mode(fake SR-IVO mode)
#define ATHUB_ATS_MODE_CNTL_PSEUDO_IOV_ENABLE_OFFSET      1
#define ATHUB_ATS_MODE_CNTL_PSEUDO_IOV_ENABLE_MASK        0x2

// Bitfield Description : 
#define ATHUB_ATS_MODE_CNTL_Reserved_31_2_OFFSET      2
#define ATHUB_ATS_MODE_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            HOST_TRANS_ENABLE:1;
    UINT32                            PSEUDO_IOV_ENABLE:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} ATHUB_ATS_MODE_CNTL_STRUCT;

#define SMN_ATHUB_ATS_MODE_CNTL_ADDRESS    0x3000UL


/***********************************************************
* Register Name : ATHUB_COMMAND
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_COMMAND_Reserved_1_0_OFFSET      0
#define ATHUB_COMMAND_Reserved_1_0_MASK        0x3

// Bitfield Description : 
#define ATHUB_COMMAND_BUS_MASTER_EN_OFFSET      2
#define ATHUB_COMMAND_BUS_MASTER_EN_MASK        0x4

// Bitfield Description : 
#define ATHUB_COMMAND_Reserved_31_3_OFFSET      3
#define ATHUB_COMMAND_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            BUS_MASTER_EN:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} ATHUB_COMMAND_STRUCT;

#define SMN_ATHUB_COMMAND_ADDRESS    0x3088UL


/***********************************************************
* Register Name : ATHUB_IH_CREDIT
* Register Description :
* IH cookie credit register for INTERRUPT
* Visibility : 0x2
************************************************************/

// Bitfield Description : The credit number that IH released to ATS for COOKIE writing for INTERRUPT.
#define ATHUB_IH_CREDIT_CREDIT_VALUE_OFFSET      0
#define ATHUB_IH_CREDIT_CREDIT_VALUE_MASK        0x3

// Bitfield Description : 
#define ATHUB_IH_CREDIT_Reserved_15_2_OFFSET      2
#define ATHUB_IH_CREDIT_Reserved_15_2_MASK        0xfffc

// Bitfield Description : The credit target ID
#define ATHUB_IH_CREDIT_IH_CLIENT_ID_OFFSET      16
#define ATHUB_IH_CREDIT_IH_CLIENT_ID_MASK        0xff0000

// Bitfield Description : 
#define ATHUB_IH_CREDIT_Reserved_31_24_OFFSET      24
#define ATHUB_IH_CREDIT_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CREDIT_VALUE:2;
    UINT32                            Reserved_15_2:14;
    UINT32                            IH_CLIENT_ID:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} ATHUB_IH_CREDIT_STRUCT;

#define SMN_ATHUB_IH_CREDIT_ADDRESS    0x3060UL


/***********************************************************
* Register Name : ATHUB_MEM_POWER_LS
* Register Description :
* Settings for Virage Memory LS modes in ATHUB.
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS setup time to disable LS mode in SCLK cycles
#define ATHUB_MEM_POWER_LS_LS_SETUP_OFFSET      0
#define ATHUB_MEM_POWER_LS_LS_SETUP_MASK        0x3f

// Bitfield Description : LS hold time to start LS mode in SCLK cycles
#define ATHUB_MEM_POWER_LS_LS_HOLD_OFFSET      6
#define ATHUB_MEM_POWER_LS_LS_HOLD_MASK        0xfc0

// Bitfield Description : 
#define ATHUB_MEM_POWER_LS_Reserved_31_12_OFFSET      12
#define ATHUB_MEM_POWER_LS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            LS_SETUP:6;
    UINT32                            LS_HOLD:6;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} ATHUB_MEM_POWER_LS_STRUCT;

#define SMN_ATHUB_MEM_POWER_LS_ADDRESS    0x3028UL


/***********************************************************
* Register Name : ATHUB_MISC_CNTL
* Register Description :
* control for atc domain clock gate
* Visibility : 0x2
************************************************************/

// Bitfield Description : delay in clock cycles to turn off clock after busy goes low
#define ATHUB_MISC_CNTL_CG_OFFDLY_OFFSET      0
#define ATHUB_MISC_CNTL_CG_OFFDLY_MASK        0x3f

// Bitfield Description : 
#define ATHUB_MISC_CNTL_CG_ENABLE_OFFSET      6
#define ATHUB_MISC_CNTL_CG_ENABLE_MASK        0x40

// Bitfield Description : 
#define ATHUB_MISC_CNTL_CG_MEM_LS_ENABLE_OFFSET      7
#define ATHUB_MISC_CNTL_CG_MEM_LS_ENABLE_MASK        0x80
#define ATHUB_MISC_CNTL_CG_MEM_LS_ENABLE_DEFAULT     0x1

// Bitfield Description : whether to enable power gating function
#define ATHUB_MISC_CNTL_PG_ENABLE_OFFSET      8
#define ATHUB_MISC_CNTL_PG_ENABLE_MASK        0x100

// Bitfield Description : delay in clock cycles to power down after busy goes low, cycle = (1 << (4 + value))
#define ATHUB_MISC_CNTL_PG_OFFDLY_OFFSET      9
#define ATHUB_MISC_CNTL_PG_OFFDLY_MASK        0x7e00

// Bitfield Description : Set this bit will make ats status always busy, can be used to invoke clock/power mannually
#define ATHUB_MISC_CNTL_ALWAYS_BUSY_OFFSET      15
#define ATHUB_MISC_CNTL_ALWAYS_BUSY_MASK        0x8000

// Bitfield Description : whether clock gated, 0 = clock active, 1 = clock idle/gated
#define ATHUB_MISC_CNTL_CG_STATUS_OFFSET      16
#define ATHUB_MISC_CNTL_CG_STATUS_MASK        0x10000

// Bitfield Description : whether power down, 0 = power up, 1 = power down
#define ATHUB_MISC_CNTL_PG_STATUS_OFFSET      17
#define ATHUB_MISC_CNTL_PG_STATUS_MASK        0x20000

// Bitfield Description : Reports on whether the RPB is busy (1) or idle(0).
#define ATHUB_MISC_CNTL_RPB_BUSY_OFFSET      18
#define ATHUB_MISC_CNTL_RPB_BUSY_MASK        0x40000

// Bitfield Description : Reports on whether the XPB is busy (1) or idle(0).
#define ATHUB_MISC_CNTL_XPB_BUSY_OFFSET      19
#define ATHUB_MISC_CNTL_XPB_BUSY_MASK        0x80000

// Bitfield Description : Reports on whether the ATS path is busy (1) or idle(0)
#define ATHUB_MISC_CNTL_ATS_BUSY_OFFSET      20
#define ATHUB_MISC_CNTL_ATS_BUSY_MASK        0x100000

// Bitfield Description : Reports on whether the SDP NCS module is busy (1) or idle(0)
#define ATHUB_MISC_CNTL_SDPNCS_BUSY_OFFSET      21
#define ATHUB_MISC_CNTL_SDPNCS_BUSY_MASK        0x200000

// Bitfield Description : Reports on whether the DF sdp port is busy (1) or idle(0)
#define ATHUB_MISC_CNTL_DFPORT_BUSY_OFFSET      22
#define ATHUB_MISC_CNTL_DFPORT_BUSY_MASK        0x400000

// Bitfield Description : Whether can we switch PG_ENABLE/CG_ENABLE at any time. 0=No(we still need always_busy), 1=Yes
#define ATHUB_MISC_CNTL_SWITCH_CNTL_OFFSET      23
#define ATHUB_MISC_CNTL_SWITCH_CNTL_MASK        0x800000

// Bitfield Description : 
#define ATHUB_MISC_CNTL_Reserved_31_24_OFFSET      24
#define ATHUB_MISC_CNTL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CG_OFFDLY:6;
    UINT32                            CG_ENABLE:1;
    UINT32                            CG_MEM_LS_ENABLE:1;
    UINT32                            PG_ENABLE:1;
    UINT32                            PG_OFFDLY:6;
    UINT32                            ALWAYS_BUSY:1;
    UINT32                            CG_STATUS:1;
    UINT32                            PG_STATUS:1;
    UINT32                            RPB_BUSY:1;
    UINT32                            XPB_BUSY:1;
    UINT32                            ATS_BUSY:1;
    UINT32                            SDPNCS_BUSY:1;
    UINT32                            DFPORT_BUSY:1;
    UINT32                            SWITCH_CNTL:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} ATHUB_MISC_CNTL_STRUCT;

#define SMN_ATHUB_MISC_CNTL_ADDRESS    0x3024UL


/***********************************************************
* Register Name : ATHUB_PCIE_ATS_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_PCIE_ATS_CNTL_Reserved_15_0_OFFSET      0
#define ATHUB_PCIE_ATS_CNTL_Reserved_15_0_MASK        0xffff

// Bitfield Description : 
#define ATHUB_PCIE_ATS_CNTL_STU_OFFSET      16
#define ATHUB_PCIE_ATS_CNTL_STU_MASK        0x1f0000

// Bitfield Description : 
#define ATHUB_PCIE_ATS_CNTL_Reserved_30_21_OFFSET      21
#define ATHUB_PCIE_ATS_CNTL_Reserved_30_21_MASK        0x7fe00000

// Bitfield Description : 
#define ATHUB_PCIE_ATS_CNTL_ATC_ENABLE_OFFSET      31
#define ATHUB_PCIE_ATS_CNTL_ATC_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            STU:5;
    UINT32                            Reserved_30_21:10;
    UINT32                            ATC_ENABLE:1;
  } Field;
  UINT32 Value;
} ATHUB_PCIE_ATS_CNTL_STRUCT;

#define SMN_ATHUB_PCIE_ATS_CNTL_ADDRESS    0x3078UL


/***********************************************************
* Register Name : ATHUB_PCIE_OUTSTAND_PAGE_REQ_ALLOC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_PCIE_OUTSTAND_PAGE_REQ_ALLOC_OUTSTAND_PAGE_REQ_ALLOC_OFFSET      0
#define ATHUB_PCIE_OUTSTAND_PAGE_REQ_ALLOC_OUTSTAND_PAGE_REQ_ALLOC_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            OUTSTAND_PAGE_REQ_ALLOC:32;
  } Field;
  UINT32 Value;
} ATHUB_PCIE_OUTSTAND_PAGE_REQ_ALLOC_STRUCT;

#define SMN_ATHUB_PCIE_OUTSTAND_PAGE_REQ_ALLOC_ADDRESS    0x3084UL


/***********************************************************
* Register Name : ATHUB_PCIE_PAGE_REQ_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_PCIE_PAGE_REQ_CNTL_PRI_ENABLE_OFFSET      0
#define ATHUB_PCIE_PAGE_REQ_CNTL_PRI_ENABLE_MASK        0x1

// Bitfield Description : 
#define ATHUB_PCIE_PAGE_REQ_CNTL_PRI_RESET_OFFSET      1
#define ATHUB_PCIE_PAGE_REQ_CNTL_PRI_RESET_MASK        0x2

// Bitfield Description : 
#define ATHUB_PCIE_PAGE_REQ_CNTL_Reserved_31_2_OFFSET      2
#define ATHUB_PCIE_PAGE_REQ_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            PRI_ENABLE:1;
    UINT32                            PRI_RESET:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} ATHUB_PCIE_PAGE_REQ_CNTL_STRUCT;

#define SMN_ATHUB_PCIE_PAGE_REQ_CNTL_ADDRESS    0x3080UL


/***********************************************************
* Register Name : ATHUB_PCIE_PASID_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_PCIE_PASID_CNTL_Reserved_15_0_OFFSET      0
#define ATHUB_PCIE_PASID_CNTL_Reserved_15_0_MASK        0xffff

// Bitfield Description : 
#define ATHUB_PCIE_PASID_CNTL_PASID_EN_OFFSET      16
#define ATHUB_PCIE_PASID_CNTL_PASID_EN_MASK        0x10000

// Bitfield Description : 
#define ATHUB_PCIE_PASID_CNTL_PASID_EXE_PERMISSION_ENABLE_OFFSET      17
#define ATHUB_PCIE_PASID_CNTL_PASID_EXE_PERMISSION_ENABLE_MASK        0x20000

// Bitfield Description : 
#define ATHUB_PCIE_PASID_CNTL_PASID_PRIV_MODE_SUPPORTED_ENABLE_OFFSET      18
#define ATHUB_PCIE_PASID_CNTL_PASID_PRIV_MODE_SUPPORTED_ENABLE_MASK        0x40000

// Bitfield Description : 
#define ATHUB_PCIE_PASID_CNTL_Reserved_31_19_OFFSET      19
#define ATHUB_PCIE_PASID_CNTL_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            PASID_EN:1;
    UINT32                            PASID_EXE_PERMISSION_ENABLE:1;
    UINT32                            PASID_PRIV_MODE_SUPPORTED_ENABLE:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} ATHUB_PCIE_PASID_CNTL_STRUCT;

#define SMN_ATHUB_PCIE_PASID_CNTL_ADDRESS    0x307cUL


/***********************************************************
* Register Name : ATHUB_SHARED_ACTIVE_FCN_ID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_SHARED_ACTIVE_FCN_ID_VFID_OFFSET      0
#define ATHUB_SHARED_ACTIVE_FCN_ID_VFID_MASK        0x1f

// Bitfield Description : 
#define ATHUB_SHARED_ACTIVE_FCN_ID_Reserved_30_5_OFFSET      5
#define ATHUB_SHARED_ACTIVE_FCN_ID_Reserved_30_5_MASK        0x7fffffe0

// Bitfield Description : 
#define ATHUB_SHARED_ACTIVE_FCN_ID_VF_OFFSET      31
#define ATHUB_SHARED_ACTIVE_FCN_ID_VF_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VFID:5;
    UINT32                            Reserved_30_5:26;
    UINT32                            VF:1;
  } Field;
  UINT32 Value;
} ATHUB_SHARED_ACTIVE_FCN_ID_STRUCT;

#define SMN_ATHUB_SHARED_ACTIVE_FCN_ID_ADDRESS    0x3008UL


/***********************************************************
* Register Name : ATHUB_SHARED_VIRT_RESET_REQ
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define ATHUB_SHARED_VIRT_RESET_REQ_VF_OFFSET      0
#define ATHUB_SHARED_VIRT_RESET_REQ_VF_MASK        0x7fffffff

// Bitfield Description : 
#define ATHUB_SHARED_VIRT_RESET_REQ_PF_OFFSET      31
#define ATHUB_SHARED_VIRT_RESET_REQ_PF_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VF:31;
    UINT32                            PF:1;
  } Field;
  UINT32 Value;
} ATHUB_SHARED_VIRT_RESET_REQ_STRUCT;

#define SMN_ATHUB_SHARED_VIRT_RESET_REQ_ADDRESS    0x3004UL


/***********************************************************
* Register Name : ATS_IH_CREDIT
* Register Description :
* IH cookie credit register for LOG
* Visibility : 0x2
************************************************************/

// Bitfield Description : The credit number that IH released to ATS for COOKIE writing for LOG.
#define ATS_IH_CREDIT_CREDIT_VALUE_OFFSET      0
#define ATS_IH_CREDIT_CREDIT_VALUE_MASK        0x3

// Bitfield Description : 
#define ATS_IH_CREDIT_Reserved_15_2_OFFSET      2
#define ATS_IH_CREDIT_Reserved_15_2_MASK        0xfffc

// Bitfield Description : The credit target ID
#define ATS_IH_CREDIT_IH_CLIENT_ID_OFFSET      16
#define ATS_IH_CREDIT_IH_CLIENT_ID_MASK        0xff0000

// Bitfield Description : 
#define ATS_IH_CREDIT_Reserved_31_24_OFFSET      24
#define ATS_IH_CREDIT_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CREDIT_VALUE:2;
    UINT32                            Reserved_15_2:14;
    UINT32                            IH_CLIENT_ID:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} ATS_IH_CREDIT_STRUCT;

#define SMN_ATS_IH_CREDIT_ADDRESS    0x305cUL

#endif /* _ATHUB_ATS_H_ */
