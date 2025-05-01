/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _GLOBALREGS_H_

#define _GLOBALREGS_H_

//ACP_AZALIAIP.h
//ATHUB_ATS.h
//ATHUB_RPB.h
//ATHUB_XPB.h
//BiosDefaults.h
//CLK_CLK1.h
//DCIO_DCIO.h
//GC_GCVMSHAREDPF.h
/***********************************************************
* Register Name : GCMC_VM_FB_OFFSET
* Register Description :
* VIRT_TYPE=pf_only This register defines the physical location of FB memory in UMA physical memory. The system physical and internal address spaces have 48 address bits. Minimum FB size is 16 MB, and the start location is required to be on a 16 MB boundry. Therefore BASE(23:0) will be treated as 0x000000. Only the 24 MSBs of each are loaded in the register. If inside the FBTOP/FBBOT aperture, the address is adjusted by subtracting FBBOT and adding FB_OFFSET. The register field defines bits (47:24) for FB_OFFSET. CHANGED
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_FB_OFFSET_FB_OFFSET_OFFSET      0
#define GCMC_VM_FB_OFFSET_FB_OFFSET_MASK        0xffffff

// Bitfield Description :
#define GCMC_VM_FB_OFFSET_Reserved_31_24_OFFSET      24
#define GCMC_VM_FB_OFFSET_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            FB_OFFSET:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} GCMC_VM_FB_OFFSET_STRUCT;

#define SMN_GCMC_VM_FB_OFFSET_ADDRESS    0xa51cUL


/***********************************************************
* Register Name : GCMC_VM_NB_LOWER_TOP_OF_DRAM2
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_ENABLE_OFFSET      0
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_ENABLE_MASK        0x1

// Bitfield Description :
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_OFFSET      1
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_MASK        0x7ffffe

// Bitfield Description :
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_OFFSET      23
#define GCMC_VM_NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_MASK        0xff800000

typedef union {
  struct {
    UINT32                            ENABLE:1;
    UINT32                            Reserved_22_1:22;
    UINT32                            LOWER_TOM2:9;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_LOWER_TOP_OF_DRAM2_STRUCT;

#define SMN_GCMC_VM_NB_LOWER_TOP_OF_DRAM2_ADDRESS    0xa514UL


/***********************************************************
* Register Name : GCMC_VM_NB_MMIOBASE
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_MMIOBASE_MMIOBASE_OFFSET      0
#define GCMC_VM_NB_MMIOBASE_MMIOBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOBASE:32;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_MMIOBASE_STRUCT;

#define SMN_GCMC_VM_NB_MMIOBASE_ADDRESS    0xa500UL


/***********************************************************
* Register Name : GCMC_VM_NB_MMIOLIMIT
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_MMIOLIMIT_MMIOLIMIT_OFFSET      0
#define GCMC_VM_NB_MMIOLIMIT_MMIOLIMIT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOLIMIT:32;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_MMIOLIMIT_STRUCT;

#define SMN_GCMC_VM_NB_MMIOLIMIT_ADDRESS    0xa504UL


/***********************************************************
* Register Name : GCMC_VM_NB_PCI_ARB
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_PCI_ARB_Reserved_2_0_OFFSET      0
#define GCMC_VM_NB_PCI_ARB_Reserved_2_0_MASK        0x7

// Bitfield Description :
#define GCMC_VM_NB_PCI_ARB_VGA_HOLE_OFFSET      3
#define GCMC_VM_NB_PCI_ARB_VGA_HOLE_MASK        0x8

// Bitfield Description :
#define GCMC_VM_NB_PCI_ARB_Reserved_31_4_OFFSET      4
#define GCMC_VM_NB_PCI_ARB_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            VGA_HOLE:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_PCI_ARB_STRUCT;

#define SMN_GCMC_VM_NB_PCI_ARB_ADDRESS    0xa50cUL


/***********************************************************
* Register Name : GCMC_VM_NB_PCI_CTRL
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_PCI_CTRL_Reserved_22_0_OFFSET      0
#define GCMC_VM_NB_PCI_CTRL_Reserved_22_0_MASK        0x7fffff

// Bitfield Description :
#define GCMC_VM_NB_PCI_CTRL_MMIOENABLE_OFFSET      23
#define GCMC_VM_NB_PCI_CTRL_MMIOENABLE_MASK        0x800000

// Bitfield Description :
#define GCMC_VM_NB_PCI_CTRL_Reserved_31_24_OFFSET      24
#define GCMC_VM_NB_PCI_CTRL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            MMIOENABLE:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_PCI_CTRL_STRUCT;

#define SMN_GCMC_VM_NB_PCI_CTRL_ADDRESS    0xa508UL


/***********************************************************
* Register Name : GCMC_VM_NB_TOP_OF_DRAM_SLOT1
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_TOP_OF_DRAM_SLOT1_Reserved_22_0_OFFSET      0
#define GCMC_VM_NB_TOP_OF_DRAM_SLOT1_Reserved_22_0_MASK        0x7fffff

// Bitfield Description :
#define GCMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_OFFSET      23
#define GCMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_MASK        0xff800000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            TOP_OF_DRAM:9;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_TOP_OF_DRAM_SLOT1_STRUCT;

#define SMN_GCMC_VM_NB_TOP_OF_DRAM_SLOT1_ADDRESS    0xa510UL


/***********************************************************
* Register Name : GCMC_VM_NB_UPPER_TOP_OF_DRAM2
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define GCMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_OFFSET      0
#define GCMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_MASK        0xfff

// Bitfield Description :
#define GCMC_VM_NB_UPPER_TOP_OF_DRAM2_Reserved_31_12_OFFSET      12
#define GCMC_VM_NB_UPPER_TOP_OF_DRAM2_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            UPPER_TOM2:12;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} GCMC_VM_NB_UPPER_TOP_OF_DRAM2_STRUCT;

#define SMN_GCMC_VM_NB_UPPER_TOP_OF_DRAM2_ADDRESS    0xa518UL


//HDA_AZF0ROOT.h
/***********************************************************
* Register Name : REG_DC_AUDIO_PORT_CONNECTIVITY
* Register Description :
* Register programming for port connectivity.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Number of connected audio pins starting at endpoint 0. REG_PORT_CONNECTIVITY reports 1 for unconnected endpoints.
#define REG_DC_AUDIO_PORT_CONNECTIVITY_REG_PORT_CONNECTIVITY_OFFSET      0
#define REG_DC_AUDIO_PORT_CONNECTIVITY_REG_PORT_CONNECTIVITY_MASK        0x7

// Bitfield Description :
#define REG_DC_AUDIO_PORT_CONNECTIVITY_Reserved_3_3_OFFSET      3
#define REG_DC_AUDIO_PORT_CONNECTIVITY_Reserved_3_3_MASK        0x8

// Bitfield Description : REG_PORT_CONNECTIVITY override enable.
#define REG_DC_AUDIO_PORT_CONNECTIVITY_REG_PORT_CONNECTIVITY_OVERRIDE_ENABLE_OFFSET      4
#define REG_DC_AUDIO_PORT_CONNECTIVITY_REG_PORT_CONNECTIVITY_OVERRIDE_ENABLE_MASK        0x10

// Bitfield Description :
#define REG_DC_AUDIO_PORT_CONNECTIVITY_Reserved_31_5_OFFSET      5
#define REG_DC_AUDIO_PORT_CONNECTIVITY_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            REG_PORT_CONNECTIVITY:3;
    UINT32                            Reserved_3_3:1;
    UINT32                            REG_PORT_CONNECTIVITY_OVERRIDE_ENABLE:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} REG_DC_AUDIO_PORT_CONNECTIVITY_STRUCT;

#define SMN_REG_DC_AUDIO_PORT_CONNECTIVITY_ADDRESS    0xe370UL


//IO.h
//IOAGR.h
//IOAPIC.h
//IOHC.h
//IOMMUL1.h
/***********************************************************
* Register Name : L1_MISC_CNTRL_1
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Value = 0 Force the Function bit in StreamID/Reqid to '0' for Ordering in L1, Value = 1 use the full StreamID/Reqid bus.
#define L1_MISC_CNTRL_1_REG_force_OrderStreamID_func_OFFSET      0
#define L1_MISC_CNTRL_1_REG_force_OrderStreamID_func_MASK        0x1

// Bitfield Description : Value = 0 disable force bypass translation, Value = 1 enable force bypass translation.
#define L1_MISC_CNTRL_1_REG_ForceBypass_OFFSET      1
#define L1_MISC_CNTRL_1_REG_ForceBypass_MASK        0x2

// Bitfield Description : Value = 1 Force not to wait for WQ to empty on dec_wait to generate Flush.
#define L1_MISC_CNTRL_1_REG_cmdwait_wait_emptwq_OFFSET      2
#define L1_MISC_CNTRL_1_REG_cmdwait_wait_emptwq_MASK        0x4

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_3_3_OFFSET      3
#define L1_MISC_CNTRL_1_Reserved_3_3_MASK        0x8

// Bitfield Description : Value = 1 Force not to wait for WQ to empty on dec_inv to generate Flush.
#define L1_MISC_CNTRL_1_REG_inv_wait_emptwq_OFFSET      4
#define L1_MISC_CNTRL_1_REG_inv_wait_emptwq_MASK        0x10

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_5_5_OFFSET      5
#define L1_MISC_CNTRL_1_Reserved_5_5_MASK        0x20

// Bitfield Description : Value =  1 Force invalidate_filter_on in L1.
#define L1_MISC_CNTRL_1_REG_force_invalid_filter_OFFSET      6
#define L1_MISC_CNTRL_1_REG_force_invalid_filter_MASK        0x40

// Bitfield Description : Value = 0 Force IW/IR set on (rsp_dt & rsp_at == 0).
#define L1_MISC_CNTRL_1_REG_L2toL1_rsp_ir_iw_set_OFFSET      7
#define L1_MISC_CNTRL_1_REG_L2toL1_rsp_ir_iw_set_MASK        0x80

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_8_8_OFFSET      8
#define L1_MISC_CNTRL_1_Reserved_8_8_MASK        0x100

// Bitfield Description : Value = 0 Allow credit rel & rspVld regardless of IOMMU_EN, Value = 1 Allow when IOMMU_EN == 1 only.
#define L1_MISC_CNTRL_1_REG_L2toL1_credit_ctrl_OFFSET      9
#define L1_MISC_CNTRL_1_REG_L2toL1_credit_ctrl_MASK        0x200

// Bitfield Description : Value = 1 disable lock bit meaning no updates can happen until this request has been sent out to L2.
#define L1_MISC_CNTRL_1_REG_allow_update_dis_OFFSET      10
#define L1_MISC_CNTRL_1_REG_allow_update_dis_MASK        0x400

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_11_11_OFFSET      11
#define L1_MISC_CNTRL_1_Reserved_11_11_MASK        0x800

// Bitfield Description : 0x1 = Return just one ATS response if first page is greater than 4K, 0x0 = Return all ATS responses of the same length (even if greater than 4K) until a page size changes.
#define L1_MISC_CNTRL_1_REG_Truncate_ATS_Rsp_Over4K_En_OFFSET      12
#define L1_MISC_CNTRL_1_REG_Truncate_ATS_Rsp_Over4K_En_MASK        0x1000

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_13_13_OFFSET      13
#define L1_MISC_CNTRL_1_Reserved_13_13_MASK        0x2000

// Bitfield Description : 0x0 = Increment ATS translations by 4K, 0x1 = Increment ATS translations depending on page size.
#define L1_MISC_CNTRL_1_REG_ATS_no_phys_overlap_En_OFFSET      14
#define L1_MISC_CNTRL_1_REG_ATS_no_phys_overlap_En_MASK        0x4000

// Bitfield Description : Value = 0 Doesn't mask Guest addr from L2, Value = 1 mask Guest addr.
#define L1_MISC_CNTRL_1_REG_guest_addr_mask_en_OFFSET      15
#define L1_MISC_CNTRL_1_REG_guest_addr_mask_en_MASK        0x8000

// Bitfield Description : Set Aerlog bit on TA or MA.
#define L1_MISC_CNTRL_1_REG_aerlog_on_abrt_OFFSET      16
#define L1_MISC_CNTRL_1_REG_aerlog_on_abrt_MASK        0x10000

// Bitfield Description : Force NW low for ATS.
#define L1_MISC_CNTRL_1_REG_enable_nw_OFFSET      17
#define L1_MISC_CNTRL_1_REG_enable_nw_MASK        0x20000

// Bitfield Description : Force PASID valid per L1.
#define L1_MISC_CNTRL_1_REG_force_pasid_vld_OFFSET      18
#define L1_MISC_CNTRL_1_REG_force_pasid_vld_MASK        0x40000

// Bitfield Description :
#define L1_MISC_CNTRL_1_Reserved_19_19_OFFSET      19
#define L1_MISC_CNTRL_1_Reserved_19_19_MASK        0x80000

// Bitfield Description : Force TLP prefix EX based on L1 sticky bit.
#define L1_MISC_CNTRL_1_REG_force_tlpprefix_EX_OFFSET      20
#define L1_MISC_CNTRL_1_REG_force_tlpprefix_EX_MASK        0x100000

// Bitfield Description : Force TLP prefix PMR based on L1 sticky bit.
#define L1_MISC_CNTRL_1_REG_force_tlpprefix_PMR_OFFSET      21
#define L1_MISC_CNTRL_1_REG_force_tlpprefix_PMR_MASK        0x200000

// Bitfield Description : Force ATS S bit on Fragmented.
#define L1_MISC_CNTRL_1_REG_ignore_iwir_for_s_bit_OFFSET      22
#define L1_MISC_CNTRL_1_REG_ignore_iwir_for_s_bit_MASK        0x400000

// Bitfield Description : Force the maximum number of allowable ATS phases within one Multi-ATS request to this value. 0x0 = force is off.
#define L1_MISC_CNTRL_1_REG_force_ats_len_OFFSET      23
#define L1_MISC_CNTRL_1_REG_force_ats_len_MASK        0x7800000

// Bitfield Description : 1=HW ignores REG_HWPrefetchRange register value and assumes it to be 7. 0=HW issues REG_HWPrefetchRange value.
#define L1_MISC_CNTRL_1_REG_HWPrefetchRangeOverride_OFFSET      27
#define L1_MISC_CNTRL_1_REG_HWPrefetchRangeOverride_MASK        0x8000000

// Bitfield Description : Selects the number of PTEs that will be pre-fetched by hardware. Value of 0 means only original request will be translated, and disables Prefetching. The register controls both Guest and Host Hardware Pre-fetch. For Host Prefetch, this register is only valid when TWPrefetchEn inside L2B index space is set to 1. If TWPrefetchEn=0, Host Pre-fetch is disabled.
#define L1_MISC_CNTRL_1_REG_HWPrefetchRange_OFFSET      28
#define L1_MISC_CNTRL_1_REG_HWPrefetchRange_MASK        0x70000000

// Bitfield Description : When HWPrefetchRange!=0(Prefetch enabled) and CAM_Prefetch_Write_Dis=1, disable writing prefetched phases to L1 CAM.
#define L1_MISC_CNTRL_1_REG_CAM_Prefetch_Write_Dis_OFFSET      31
#define L1_MISC_CNTRL_1_REG_CAM_Prefetch_Write_Dis_MASK        0x80000000

typedef union {
  struct {
    UINT32                            REG_force_OrderStreamID_func:1;
    UINT32                            REG_ForceBypass:1;
    UINT32                            REG_cmdwait_wait_emptwq:1;
    UINT32                            Reserved_3_3:1;
    UINT32                            REG_inv_wait_emptwq:1;
    UINT32                            Reserved_5_5:1;
    UINT32                            REG_force_invalid_filter:1;
    UINT32                            REG_L2toL1_rsp_ir_iw_set:1;
    UINT32                            Reserved_8_8:1;
    UINT32                            REG_L2toL1_credit_ctrl:1;
    UINT32                            REG_allow_update_dis:1;
    UINT32                            Reserved_11_11:1;
    UINT32                            REG_Truncate_ATS_Rsp_Over4K_En:1;
    UINT32                            Reserved_13_13:1;
    UINT32                            REG_ATS_no_phys_overlap_En:1;
    UINT32                            REG_guest_addr_mask_en:1;
    UINT32                            REG_aerlog_on_abrt:1;
    UINT32                            REG_enable_nw:1;
    UINT32                            REG_force_pasid_vld:1;
    UINT32                            Reserved_19_19:1;
    UINT32                            REG_force_tlpprefix_EX:1;
    UINT32                            REG_force_tlpprefix_PMR:1;
    UINT32                            REG_ignore_iwir_for_s_bit:1;
    UINT32                            REG_force_ats_len:4;
    UINT32                            REG_HWPrefetchRangeOverride:1;
    UINT32                            REG_HWPrefetchRange:3;
    UINT32                            REG_CAM_Prefetch_Write_Dis:1;
  } Field;
  UINT32 Value;
} L1_MISC_CNTRL_1_STRUCT;

#define SMN_IOAGR_L1_MISC_CNTRL_1_ADDRESS    0x1480001cUL

#define SMN_PCIE0_L1_MISC_CNTRL_1_ADDRESS    0x1470001cUL

#define SMN_PCIE2_L1_MISC_CNTRL_1_ADDRESS    0x1490001cUL

#define SMN_PCIE3_L1_MISC_CNTRL_1_ADDRESS    0x14a0001cUL


/***********************************************************
* Register Name : L1_PGMEM_CTRL_1
* Register Description :
*
* Visibility : 0x8
************************************************************/

// Bitfield Description : Reserved for future memory powergating support.
#define L1_PGMEM_CTRL_1_L1_LS_EN_OFFSET      0
#define L1_PGMEM_CTRL_1_L1_LS_EN_MASK        0x1

// Bitfield Description : Reserved for future memory powergating support.
#define L1_PGMEM_CTRL_1_L1_DS_EN_OFFSET      1
#define L1_PGMEM_CTRL_1_L1_DS_EN_MASK        0x2

// Bitfield Description : Reserved for future memory powergating support.
#define L1_PGMEM_CTRL_1_L1_SD_EN_OFFSET      2
#define L1_PGMEM_CTRL_1_L1_SD_EN_MASK        0x4

// Bitfield Description : Select for which mem_power_ctrl bus is connected to the L1 memories.  0-PGFSM is control power for memories, 1-Use the SMU/IP_PGMEM mem_power_ctrl for memories.  This register is only used if L1 dynamic power gated is supported.  If dyanmic power gated is not supported, the bus from PGFSM does not exist, and we select the RSMU/IP_PGMEM mem_power_ctrl bus by default.
#define L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_OFFSET      3
#define L1_PGMEM_CTRL_1_L1_IP_PGMEM_SEL_MASK        0x8

// Bitfield Description :
#define L1_PGMEM_CTRL_1_Reserved_31_4_OFFSET      4
#define L1_PGMEM_CTRL_1_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            L1_LS_EN:1;
    UINT32                            L1_DS_EN:1;
    UINT32                            L1_SD_EN:1;
    UINT32                            L1_IP_PGMEM_SEL:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} L1_PGMEM_CTRL_1_STRUCT;

#define SMN_IOAGR_L1_PGMEM_CTRL_1_ADDRESS    0x148000e4UL

#define SMN_PCIE0_L1_PGMEM_CTRL_1_ADDRESS    0x147000e4UL

#define SMN_PCIE2_L1_PGMEM_CTRL_1_ADDRESS    0x149000e4UL

#define SMN_PCIE3_L1_PGMEM_CTRL_1_ADDRESS    0x14a000e4UL


//IOMMUL2.h
//IOMMUMMIO.h
//MMHUB_PCTL.h
/***********************************************************
* Register Name : PCTL_MMHUB_DEEPSLEEP_IB
* Register Description :
* IPs (without hardwired DS port) report their deepsleep status to MMHUB and MMHUB aggregates all sideband status signals (hardwired + deepsleep register bits) to be used in aggregate idle conditions for PG and DS mode and passing out clk_ds_allow signal. Please refer to latest connection map (..._pctl_connection.xlsx) at GMHUBs shared point
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS0_OFFSET      0
#define PCTL_MMHUB_DEEPSLEEP_IB_DS0_MASK        0x1

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS1_OFFSET      1
#define PCTL_MMHUB_DEEPSLEEP_IB_DS1_MASK        0x2

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS2_OFFSET      2
#define PCTL_MMHUB_DEEPSLEEP_IB_DS2_MASK        0x4

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS3_OFFSET      3
#define PCTL_MMHUB_DEEPSLEEP_IB_DS3_MASK        0x8

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS4_OFFSET      4
#define PCTL_MMHUB_DEEPSLEEP_IB_DS4_MASK        0x10

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS5_OFFSET      5
#define PCTL_MMHUB_DEEPSLEEP_IB_DS5_MASK        0x20

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS6_OFFSET      6
#define PCTL_MMHUB_DEEPSLEEP_IB_DS6_MASK        0x40

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS7_OFFSET      7
#define PCTL_MMHUB_DEEPSLEEP_IB_DS7_MASK        0x80

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS8_OFFSET      8
#define PCTL_MMHUB_DEEPSLEEP_IB_DS8_MASK        0x100

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS9_OFFSET      9
#define PCTL_MMHUB_DEEPSLEEP_IB_DS9_MASK        0x200

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS10_OFFSET      10
#define PCTL_MMHUB_DEEPSLEEP_IB_DS10_MASK        0x400

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS11_OFFSET      11
#define PCTL_MMHUB_DEEPSLEEP_IB_DS11_MASK        0x800

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS12_OFFSET      12
#define PCTL_MMHUB_DEEPSLEEP_IB_DS12_MASK        0x1000

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS13_OFFSET      13
#define PCTL_MMHUB_DEEPSLEEP_IB_DS13_MASK        0x2000

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS14_OFFSET      14
#define PCTL_MMHUB_DEEPSLEEP_IB_DS14_MASK        0x4000

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS15_OFFSET      15
#define PCTL_MMHUB_DEEPSLEEP_IB_DS15_MASK        0x8000

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_DS16_OFFSET      16
#define PCTL_MMHUB_DEEPSLEEP_IB_DS16_MASK        0x10000

// Bitfield Description :
#define PCTL_MMHUB_DEEPSLEEP_IB_Reserved_30_17_OFFSET      17
#define PCTL_MMHUB_DEEPSLEEP_IB_Reserved_30_17_MASK        0x7ffe0000

// Bitfield Description : This field is used during write to this register. To set DS(N) field just write 1 to that field with SETCLEAR being 0. DS(N) = DS(N) | DATA(N); To clear DS(N) field just write 1 to that field with SETCLEAR being 1. DS(N) = DS(N) & ~DATA(N);
#define PCTL_MMHUB_DEEPSLEEP_IB_SETCLEAR_OFFSET      31
#define PCTL_MMHUB_DEEPSLEEP_IB_SETCLEAR_MASK        0x80000000

typedef union {
  struct {
    UINT32                            DS0:1;
    UINT32                            DS1:1;
    UINT32                            DS2:1;
    UINT32                            DS3:1;
    UINT32                            DS4:1;
    UINT32                            DS5:1;
    UINT32                            DS6:1;
    UINT32                            DS7:1;
    UINT32                            DS8:1;
    UINT32                            DS9:1;
    UINT32                            DS10:1;
    UINT32                            DS11:1;
    UINT32                            DS12:1;
    UINT32                            DS13:1;
    UINT32                            DS14:1;
    UINT32                            DS15:1;
    UINT32                            DS16:1;
    UINT32                            Reserved_30_17:14;
    UINT32                            SETCLEAR:1;
  } Field;
  UINT32 Value;
} PCTL_MMHUB_DEEPSLEEP_IB_STRUCT;

#define SMN_PCTL_MMHUB_DEEPSLEEP_IB_ADDRESS    0x68e04UL


/***********************************************************
* Register Name : PCTL_SLICE0_MISC
* Register Description :
* Miscellaneous controls RW1C for script use only
* Visibility : 0x4
************************************************************/

// Bitfield Description : This pointer splits the RAM of Register ENGine into secure and not secure regions. The RAM cannot be updated below this pointer. Furthermore, any opcodes that result in register writes are either tagged as secure (below this pointer) or non-secure. The non-secure registers writes cannot overwrite any registers that have been locked by CRITICAL_REGS_LOCK or 'garlic request disable'. This field cannot be overwritten once CRITICAL_REGS_LOCK is set.
#define PCTL_SLICE0_MISC_RENG_EXECUTE_NONSECURE_START_PTR_OFFSET      0
#define PCTL_SLICE0_MISC_RENG_EXECUTE_NONSECURE_START_PTR_MASK        0x3ff

// Bitfield Description : Locks critical registers in UTCL2 as well as RENG_EXECUTE_NONSECURE_START_PTR field.
#define PCTL_SLICE0_MISC_CRITICAL_REGS_LOCK_OFFSET      10
#define PCTL_SLICE0_MISC_CRITICAL_REGS_LOCK_MASK        0x400

// Bitfield Description : Sets the threshold on how long should the STCTRL wait after it halted all input interfaces to allow any requests that just missed 'halt' condition to make its way to the initiator and become visible to STCTRL.
#define PCTL_SLICE0_MISC_TILE_IDLE_THRESHOLD_OFFSET      11
#define PCTL_SLICE0_MISC_TILE_IDLE_THRESHOLD_MASK        0x3800

// Bitfield Description : Light Sleep must be disabled when RENG memories are being accessed via RENG_RAM_INDEX/RENG_RAM_DATA pair.
#define PCTL_SLICE0_MISC_RENG_MEM_LS_ENABLE_OFFSET      14
#define PCTL_SLICE0_MISC_RENG_MEM_LS_ENABLE_MASK        0x4000

// Bitfield Description : If set, PGFSM_GMCON_cmd_done is forced to 1 for the corresponding instance of the PGFSM.
#define PCTL_SLICE0_MISC_STCTRL_FORCE_PGFSM_CMD_DONE_OFFSET      15
#define PCTL_SLICE0_MISC_STCTRL_FORCE_PGFSM_CMD_DONE_MASK        0x8000

// Bitfield Description : If set, issue SDP disconnect request to sdp through mm_ea when mmhub in DS-ONLY mode.
#define PCTL_SLICE0_MISC_DEEPSLEEP_DISCSDP_OFFSET      16
#define PCTL_SLICE0_MISC_DEEPSLEEP_DISCSDP_MASK        0x10000

// Bitfield Description : If set, STCTRL triggers SLICE0 RENG to execute opcodes in save mode every time registers get updated. STCTRL allows some time (see STCTRL_RSMU_IDLE_THRESHOLD) before it really triggers RENG to save registers.
#define PCTL_SLICE0_MISC_RENG_EXECUTE_ON_REG_UPDATE_OFFSET      17
#define PCTL_SLICE0_MISC_RENG_EXECUTE_ON_REG_UPDATE_MASK        0x20000

// Bitfield Description : If set, RDVLD timer will be enabled to detect illegal access
#define PCTL_SLICE0_MISC_RD_TIMER_ENABLE_OFFSET      18
#define PCTL_SLICE0_MISC_RD_TIMER_ENABLE_MASK        0x40000

// Bitfield Description : If set, STCTRL will override the SDP0 partack from EA0 when deciding if it can disconnect or power-gate
#define PCTL_SLICE0_MISC_OVR_EA_SDP0_PARTACK_OFFSET      19
#define PCTL_SLICE0_MISC_OVR_EA_SDP0_PARTACK_MASK        0x80000

// Bitfield Description : If set, STCTRL will override the SDP0 fullack from EA0 when deciding if it can disconnect or power-gate
#define PCTL_SLICE0_MISC_OVR_EA_SDP0_FULLACK_OFFSET      20
#define PCTL_SLICE0_MISC_OVR_EA_SDP0_FULLACK_MASK        0x100000

// Bitfield Description :
#define PCTL_SLICE0_MISC_Reserved_31_21_OFFSET      21
#define PCTL_SLICE0_MISC_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            RENG_EXECUTE_NONSECURE_START_PTR:10;
    UINT32                            CRITICAL_REGS_LOCK:1;
    UINT32                            TILE_IDLE_THRESHOLD:3;
    UINT32                            RENG_MEM_LS_ENABLE:1;
    UINT32                            STCTRL_FORCE_PGFSM_CMD_DONE:1;
    UINT32                            DEEPSLEEP_DISCSDP:1;
    UINT32                            RENG_EXECUTE_ON_REG_UPDATE:1;
    UINT32                            RD_TIMER_ENABLE:1;
    UINT32                            OVR_EA_SDP0_PARTACK:1;
    UINT32                            OVR_EA_SDP0_FULLACK:1;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} PCTL_SLICE0_MISC_STRUCT;

#define SMN_PCTL_SLICE0_MISC_ADDRESS    0x68e3cUL


//MMUTCL2_MMVMSHAREDPF.h
/***********************************************************
* Register Name : MMMC_VM_NB_LOWER_TOP_OF_DRAM2
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_ENABLE_OFFSET      0
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_ENABLE_MASK        0x1

// Bitfield Description :
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_OFFSET      1
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_MASK        0x7ffffe

// Bitfield Description :
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_OFFSET      23
#define MMMC_VM_NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_MASK        0xff800000

typedef union {
  struct {
    UINT32                            ENABLE:1;
    UINT32                            Reserved_22_1:22;
    UINT32                            LOWER_TOM2:9;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_LOWER_TOP_OF_DRAM2_STRUCT;

#define SMN_MMMC_VM_NB_LOWER_TOP_OF_DRAM2_ADDRESS    0x6a354UL


/***********************************************************
* Register Name : MMMC_VM_NB_MMIOBASE
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_MMIOBASE_MMIOBASE_OFFSET      0
#define MMMC_VM_NB_MMIOBASE_MMIOBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOBASE:32;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_MMIOBASE_STRUCT;

#define SMN_MMMC_VM_NB_MMIOBASE_ADDRESS    0x6a340UL


/***********************************************************
* Register Name : MMMC_VM_NB_MMIOLIMIT
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_MMIOLIMIT_MMIOLIMIT_OFFSET      0
#define MMMC_VM_NB_MMIOLIMIT_MMIOLIMIT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOLIMIT:32;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_MMIOLIMIT_STRUCT;

#define SMN_MMMC_VM_NB_MMIOLIMIT_ADDRESS    0x6a344UL


/***********************************************************
* Register Name : MMMC_VM_NB_PCI_ARB
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_PCI_ARB_Reserved_2_0_OFFSET      0
#define MMMC_VM_NB_PCI_ARB_Reserved_2_0_MASK        0x7

// Bitfield Description :
#define MMMC_VM_NB_PCI_ARB_VGA_HOLE_OFFSET      3
#define MMMC_VM_NB_PCI_ARB_VGA_HOLE_MASK        0x8

// Bitfield Description :
#define MMMC_VM_NB_PCI_ARB_Reserved_31_4_OFFSET      4
#define MMMC_VM_NB_PCI_ARB_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            VGA_HOLE:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_PCI_ARB_STRUCT;

#define SMN_MMMC_VM_NB_PCI_ARB_ADDRESS    0x6a34cUL


/***********************************************************
* Register Name : MMMC_VM_NB_PCI_CTRL
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_PCI_CTRL_Reserved_22_0_OFFSET      0
#define MMMC_VM_NB_PCI_CTRL_Reserved_22_0_MASK        0x7fffff

// Bitfield Description :
#define MMMC_VM_NB_PCI_CTRL_MMIOENABLE_OFFSET      23
#define MMMC_VM_NB_PCI_CTRL_MMIOENABLE_MASK        0x800000

// Bitfield Description :
#define MMMC_VM_NB_PCI_CTRL_Reserved_31_24_OFFSET      24
#define MMMC_VM_NB_PCI_CTRL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            MMIOENABLE:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_PCI_CTRL_STRUCT;

#define SMN_MMMC_VM_NB_PCI_CTRL_ADDRESS    0x6a348UL


/***********************************************************
* Register Name : MMMC_VM_NB_TOP_OF_DRAM_SLOT1
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_TOP_OF_DRAM_SLOT1_Reserved_22_0_OFFSET      0
#define MMMC_VM_NB_TOP_OF_DRAM_SLOT1_Reserved_22_0_MASK        0x7fffff

// Bitfield Description :
#define MMMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_OFFSET      23
#define MMMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_MASK        0xff800000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            TOP_OF_DRAM:9;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_TOP_OF_DRAM_SLOT1_STRUCT;

#define SMN_MMMC_VM_NB_TOP_OF_DRAM_SLOT1_ADDRESS    0x6a350UL


/***********************************************************
* Register Name : MMMC_VM_NB_UPPER_TOP_OF_DRAM2
* Register Description :
* VIRT_TYPE=pf_only DRAM MMIO mapping register
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define MMMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_OFFSET      0
#define MMMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_MASK        0xfff

// Bitfield Description :
#define MMMC_VM_NB_UPPER_TOP_OF_DRAM2_Reserved_31_12_OFFSET      12
#define MMMC_VM_NB_UPPER_TOP_OF_DRAM2_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            UPPER_TOM2:12;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} MMMC_VM_NB_UPPER_TOP_OF_DRAM2_STRUCT;

#define SMN_MMMC_VM_NB_UPPER_TOP_OF_DRAM2_ADDRESS    0x6a358UL


//MP_MP0CRU.h
//MP_MP1CRU.h
//NBIFEPF0CFG.h
//NBIFEPFCFG.h
//NBIFEPFNCFG.h
//NBIFMM.h


/***********************************************************
* Register Name : RCC_CONSOLE_IOV_FIRST_VF_OFFSET
* Register Description :
* Console IOV first VF Offset register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Indicates the Console IOV Routing ID offset of the first VF associated with the PF.
#define RCC_CONSOLE_IOV_FIRST_VF_OFFSET_CONSOLE_IOV_FIRST_VF_OFFSET_OFFSET      0
#define RCC_CONSOLE_IOV_FIRST_VF_OFFSET_CONSOLE_IOV_FIRST_VF_OFFSET_MASK        0xffff

typedef union {
  struct {
    UINT16                            CONSOLE_IOV_FIRST_VF_OFFSET:16;
  } Field;
  UINT16 Value;
} RCC_CONSOLE_IOV_FIRST_VF_OFFSET_STRUCT;

#define SMN_DEV0_NBIF0_RCC_CONSOLE_IOV_FIRST_VF_OFFSET_ADDRESS    0x36bcUL

#define SMN_DEV0_NBIF0BIFDEC_RCC_CONSOLE_IOV_FIRST_VF_OFFSET_ADDRESS    0x101236bcUL

/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP5
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : PWR_BUDGET_DATA when PWR_BUDGET_SEL=4.
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_4_DEV0_OFFSET      0
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_4_DEV0_MASK        0xff

// Bitfield Description : PWR_BUDGET_DATA when PWR_BUDGET_SEL=5.
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_5_DEV0_OFFSET      8
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_5_DEV0_MASK        0xff00

// Bitfield Description : Set PWR_BUDGET_CAP.SYSTEM_ALLOCATED.
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV0_OFFSET      16
#define RCC_DEV0_PORT_STRAP5_STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV0_MASK        0x10000

// Bitfield Description : Enable 64bit AtomicOP Completer in downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ATOMIC_64BIT_EN_DN_DEV0_OFFSET      17
#define RCC_DEV0_PORT_STRAP5_STRAP_ATOMIC_64BIT_EN_DN_DEV0_MASK        0x20000

// Bitfield Description : Enable AtomicOp Routing support in Downstream Port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ATOMIC_ROUTING_EN_DEV0_OFFSET      18
#define RCC_DEV0_PORT_STRAP5_STRAP_ATOMIC_ROUTING_EN_DEV0_MASK        0x40000

// Bitfield Description : Enable VC capability in Downstream Port.
#define RCC_DEV0_PORT_STRAP5_STRAP_VC_EN_DN_DEV0_OFFSET      19
#define RCC_DEV0_PORT_STRAP5_STRAP_VC_EN_DN_DEV0_MASK        0x80000

// Bitfield Description : Enable Second VC in EP function.
#define RCC_DEV0_PORT_STRAP5_STRAP_TwoVC_EN_DEV0_OFFSET      20
#define RCC_DEV0_PORT_STRAP5_STRAP_TwoVC_EN_DEV0_MASK        0x100000

// Bitfield Description : Enable Second VC in Downstream Port.
#define RCC_DEV0_PORT_STRAP5_STRAP_TwoVC_EN_DN_DEV0_OFFSET      21
#define RCC_DEV0_PORT_STRAP5_STRAP_TwoVC_EN_DN_DEV0_MASK        0x200000

// Bitfield Description : Indicate that this Port supports the Scaled Flow Control Featrue.
#define RCC_DEV0_PORT_STRAP5_STRAP_LOCAL_DLF_SUPPORTED_DEV0_OFFSET      22
#define RCC_DEV0_PORT_STRAP5_STRAP_LOCAL_DLF_SUPPORTED_DEV0_MASK        0x400000

// Bitfield Description : Enable Source Validatoin in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_SOURCE_VALIDATION_DN_DEV0_OFFSET      23
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_SOURCE_VALIDATION_DN_DEV0_MASK        0x800000

// Bitfield Description : Enable Transalation Blocking in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV0_OFFSET      24
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV0_MASK        0x1000000

// Bitfield Description : Enable P2P Request Redirect in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV0_OFFSET      25
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV0_MASK        0x2000000

// Bitfield Description : Enable P2P Completion Redirect in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV0_OFFSET      26
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV0_MASK        0x4000000

// Bitfield Description : Enable Upstream Forwarding in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV0_OFFSET      27
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV0_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV0_OFFSET      28
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV0_MASK        0x10000000

// Bitfield Description : Enable Direct Translated P2P in ACS capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV0_OFFSET      29
#define RCC_DEV0_PORT_STRAP5_STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV0_MASK        0x20000000

// Bitfield Description : Enable MSI MAP Capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_MSI_MAP_EN_DEV0_OFFSET      30
#define RCC_DEV0_PORT_STRAP5_STRAP_MSI_MAP_EN_DEV0_MASK        0x40000000

// Bitfield Description : Enable SSID Capability for downstream port.
#define RCC_DEV0_PORT_STRAP5_STRAP_SSID_EN_DEV0_OFFSET      31
#define RCC_DEV0_PORT_STRAP5_STRAP_SSID_EN_DEV0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_PWR_BUDGET_DATA_8T0_4_DEV0:8;
    UINT32                            STRAP_PWR_BUDGET_DATA_8T0_5_DEV0:8;
    UINT32                            STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV0:1;
    UINT32                            STRAP_ATOMIC_64BIT_EN_DN_DEV0:1;
    UINT32                            STRAP_ATOMIC_ROUTING_EN_DEV0:1;
    UINT32                            STRAP_VC_EN_DN_DEV0:1;
    UINT32                            STRAP_TwoVC_EN_DEV0:1;
    UINT32                            STRAP_TwoVC_EN_DN_DEV0:1;
    UINT32                            STRAP_LOCAL_DLF_SUPPORTED_DEV0:1;
    UINT32                            STRAP_ACS_SOURCE_VALIDATION_DN_DEV0:1;
    UINT32                            STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV0:1;
    UINT32                            STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV0:1;
    UINT32                            STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV0:1;
    UINT32                            STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV0:1;
    UINT32                            STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV0:1;
    UINT32                            STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV0:1;
    UINT32                            STRAP_MSI_MAP_EN_DEV0:1;
    UINT32                            STRAP_SSID_EN_DEV0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP5_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP5_ADDRESS    0x10131014UL

#define SMN_NBIF0_OFFSETH234C0_RCC_DEV0_PORT_STRAP5_ADDRESS    0x101234c0UL

#define SMN_NBIF0_OFFSETH40_RCC_DEV0_PORT_STRAP5_ADDRESS    0x34c0UL

#define SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP5_ADDRESS    0x10131214UL

/***********************************************************
* Register Name : BIFC_MISC_CTRL0
* Register Description :
* BIFC miscellaneous control registers.
* Visibility : 0x4
************************************************************/

// Bitfield Description : 1: check unitID field in virtual wire trans' address. 0: ignore target unitID check. swus to nbif is always p2p smn vwire, so should always be 0 with no unitid check
#define BIFC_MISC_CTRL0_VWIRE_TARG_UNITID_CHECK_EN_OFFSET      0
#define BIFC_MISC_CTRL0_VWIRE_TARG_UNITID_CHECK_EN_MASK        0x1

// Bitfield Description : 0: ignore target unitID check. 1: check source unitID be PCIE ports and ntb-alias. 2: only PCIE port-0 and ntb-alias unitID. 3: reserved.
#define BIFC_MISC_CTRL0_VWIRE_SRC_UNITID_CHECK_EN_OFFSET      1
#define BIFC_MISC_CTRL0_VWIRE_SRC_UNITID_CHECK_EN_MASK        0x6

// Bitfield Description :
#define BIFC_MISC_CTRL0_Reserved_3_3_OFFSET      3
#define BIFC_MISC_CTRL0_Reserved_3_3_MASK        0x8

// Bitfield Description : Select RspStatus returned when detecting transaction other DVMOpReq/DVMSyncReq received on VC-4.
#define BIFC_MISC_CTRL0_DMA_VC4_NON_DVM_STS_OFFSET      4
#define BIFC_MISC_CTRL0_DMA_VC4_NON_DVM_STS_MASK        0xf0

// Bitfield Description : 1: break chain based on PCIe MPS in RC mode; 0: don't affect original chain.
#define BIFC_MISC_CTRL0_DMA_CHAIN_BREAK_IN_RCMODE_OFFSET      8
#define BIFC_MISC_CTRL0_DMA_CHAIN_BREAK_IN_RCMODE_MASK        0x100

// Bitfield Description : 1: lock host arbiter if trans' Reqchain==1.
#define BIFC_MISC_CTRL0_HST_ARB_CHAIN_LOCK_OFFSET      9
#define BIFC_MISC_CTRL0_HST_ARB_CHAIN_LOCK_MASK        0x200

// Bitfield Description : 1: lock gsi's sst arbiter if trans' Reqchain==1.
#define BIFC_MISC_CTRL0_GSI_SST_ARB_CHAIN_LOCK_OFFSET      10
#define BIFC_MISC_CTRL0_GSI_SST_ARB_CHAIN_LOCK_MASK        0x400

// Bitfield Description : When read is in the process of splitting, 0 -- incoming flush will not be stalled, 1 -- incoming flush will be stalled.
#define BIFC_MISC_CTRL0_GSI_RD_SPLIT_STALL_FLUSH_EN_OFFSET      11
#define BIFC_MISC_CTRL0_GSI_RD_SPLIT_STALL_FLUSH_EN_MASK        0x800

// Bitfield Description : When read is in the process of splitting, 0 -- np-write will be stalled, 1 -- np-write will not be stalled.
#define BIFC_MISC_CTRL0_GSI_RD_SPLIT_STALL_NPWR_DIS_OFFSET      12
#define BIFC_MISC_CTRL0_GSI_RD_SPLIT_STALL_NPWR_DIS_MASK        0x1000

// Bitfield Description : Disable setting rd_req_state_preceedingWrPresent to 1.
#define BIFC_MISC_CTRL0_GSI_SET_PRECEEDINGWR_DIS_OFFSET      13
#define BIFC_MISC_CTRL0_GSI_SET_PRECEEDINGWR_DIS_MASK        0x2000

// Bitfield Description :
#define BIFC_MISC_CTRL0_Reserved_15_14_OFFSET      14
#define BIFC_MISC_CTRL0_Reserved_15_14_MASK        0xc000

// Bitfield Description : 1: disable atomic trans vector length/addr check in DMA direction.
#define BIFC_MISC_CTRL0_DMA_ATOMIC_LENGTH_CHK_DIS_OFFSET      16
#define BIFC_MISC_CTRL0_DMA_ATOMIC_LENGTH_CHK_DIS_MASK        0x10000

// Bitfield Description : 0: slvErr ; 1: DecErr.
#define BIFC_MISC_CTRL0_DMA_ATOMIC_FAILED_STS_SEL_OFFSET      17
#define BIFC_MISC_CTRL0_DMA_ATOMIC_FAILED_STS_SEL_MASK        0x20000

// Bitfield Description : 1: force-waive VF DMA request as PF when SRIOV_EN=0;
#define BIFC_MISC_CTRL0_DMA_FORCE_VF_AS_PF_SRIOIVEN_LOW_OFFSET      18
#define BIFC_MISC_CTRL0_DMA_FORCE_VF_AS_PF_SRIOIVEN_LOW_MASK        0x40000

// Bitfield Description : 1: force-waive to keep lowest addr[1:0] (PH) for DMA requests. 0: only maintain for Translation Request (NW bit).
#define BIFC_MISC_CTRL0_DMA_ADDR_KEEP_PH_OFFSET      19
#define BIFC_MISC_CTRL0_DMA_ADDR_KEEP_PH_MASK        0x80000

// Bitfield Description : 1: force-waive to use zero for TD (ECRC GEN) for all RCC generated requests.
#define BIFC_MISC_CTRL0_RCC_GMI_TD_FORCE_ZERO_OFFSET      20
#define BIFC_MISC_CTRL0_RCC_GMI_TD_FORCE_ZERO_MASK        0x100000

// Bitfield Description : 1: defer flush behavior when there is pending trans in pipeline, 0: not defer
#define BIFC_MISC_CTRL0_HST_FLUSH_DEFER_EN_OFFSET      21
#define BIFC_MISC_CTRL0_HST_FLUSH_DEFER_EN_MASK        0x200000

// Bitfield Description : 1: flush can clear the lock status of read-rsp-arbiter, 0:can't clear
#define BIFC_MISC_CTRL0_HST_FLUSH_CLR_LOCK_EN_OFFSET      22
#define BIFC_MISC_CTRL0_HST_FLUSH_CLR_LOCK_EN_MASK        0x400000

// Bitfield Description :
#define BIFC_MISC_CTRL0_Reserved_23_23_OFFSET      23
#define BIFC_MISC_CTRL0_Reserved_23_23_MASK        0x800000

// Bitfield Description : 1: disable PCIe cfg space access protection for those capability registers not enabled.
#define BIFC_MISC_CTRL0_PCIE_CAPABILITY_PROT_DIS_OFFSET      24
#define BIFC_MISC_CTRL0_PCIE_CAPABILITY_PROT_DIS_MASK        0x1000000

// Bitfield Description :
#define BIFC_MISC_CTRL0_Reserved_25_25_OFFSET      25
#define BIFC_MISC_CTRL0_Reserved_25_25_MASK        0x2000000

// Bitfield Description : 1: disable 2nd ReqCmd send in the 1st ReqCmd's successive data sending cycle nBIF's DMA path.
#define BIFC_MISC_CTRL0_DMA_2ND_REQ_DIS_OFFSET      26
#define BIFC_MISC_CTRL0_DMA_2ND_REQ_DIS_MASK        0x4000000

// Bitfield Description : 1: bypass FW for port's dstate change. 0: send RSMU interrupt for port's dstate change.
#define BIFC_MISC_CTRL0_PORT_DSTATE_BYPASS_MODE_OFFSET      27
#define BIFC_MISC_CTRL0_PORT_DSTATE_BYPASS_MODE_MASK        0x8000000

// Bitfield Description : PME_TURNOFF handling depend on FW. 0: bypass; 1: rely on FW.
#define BIFC_MISC_CTRL0_PME_TURNOFF_MODE_OFFSET      28
#define BIFC_MISC_CTRL0_PME_TURNOFF_MODE_MASK        0x10000000

// Bitfield Description : 0: slvErr ; 1: DecErr.
#define BIFC_MISC_CTRL0_DMA_ALL_RST_PROTECT_STS_SEL_OFFSET      29
#define BIFC_MISC_CTRL0_DMA_ALL_RST_PROTECT_STS_SEL_MASK        0x20000000

// Bitfield Description : 1: change the address decode for hdp p2p wirte; 0: keep original design
#define BIFC_MISC_CTRL0_HDP_P2P_DIRECT_ADD_ADJUST_OFFSET      30
#define BIFC_MISC_CTRL0_HDP_P2P_DIRECT_ADD_ADJUST_MASK        0x40000000

// Bitfield Description : Only used for SWDS in certain varaints. 0: select PCIE0.SWUS; 1: select PCIE1.SWUS.
#define BIFC_MISC_CTRL0_PCIESWUS_SELECTION_OFFSET      31
#define BIFC_MISC_CTRL0_PCIESWUS_SELECTION_MASK        0x80000000

typedef union {
  struct {
    UINT32                            VWIRE_TARG_UNITID_CHECK_EN:1;
    UINT32                            VWIRE_SRC_UNITID_CHECK_EN:2;
    UINT32                            Reserved_3_3:1;
    UINT32                            DMA_VC4_NON_DVM_STS:4;
    UINT32                            DMA_CHAIN_BREAK_IN_RCMODE:1;
    UINT32                            HST_ARB_CHAIN_LOCK:1;
    UINT32                            GSI_SST_ARB_CHAIN_LOCK:1;
    UINT32                            GSI_RD_SPLIT_STALL_FLUSH_EN:1;
    UINT32                            GSI_RD_SPLIT_STALL_NPWR_DIS:1;
    UINT32                            GSI_SET_PRECEEDINGWR_DIS:1;
    UINT32                            Reserved_15_14:2;
    UINT32                            DMA_ATOMIC_LENGTH_CHK_DIS:1;
    UINT32                            DMA_ATOMIC_FAILED_STS_SEL:1;
    UINT32                            DMA_FORCE_VF_AS_PF_SRIOIVEN_LOW:1;
    UINT32                            DMA_ADDR_KEEP_PH:1;
    UINT32                            RCC_GMI_TD_FORCE_ZERO:1;
    UINT32                            HST_FLUSH_DEFER_EN:1;
    UINT32                            HST_FLUSH_CLR_LOCK_EN:1;
    UINT32                            Reserved_23_23:1;
    UINT32                            PCIE_CAPABILITY_PROT_DIS:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            DMA_2ND_REQ_DIS:1;
    UINT32                            PORT_DSTATE_BYPASS_MODE:1;
    UINT32                            PME_TURNOFF_MODE:1;
    UINT32                            DMA_ALL_RST_PROTECT_STS_SEL:1;
    UINT32                            HDP_P2P_DIRECT_ADD_ADJUST:1;
    UINT32                            PCIESWUS_SELECTION:1;
  } Field;
  UINT32 Value;
} BIFC_MISC_CTRL0_STRUCT;

#define SMN_NBIF0_BIFC_MISC_CTRL0_ADDRESS    0x1013a010UL


/***********************************************************
* Register Name : DN_PCIE_CNTL
* Register Description :
* PCIExpress control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET      0
#define DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK        0x1

// Bitfield Description :
#define DN_PCIE_CNTL_Reserved_6_1_OFFSET      1
#define DN_PCIE_CNTL_Reserved_6_1_MASK        0x7e

// Bitfield Description :
#define DN_PCIE_CNTL_UR_ERR_REPORT_DIS_DN_OFFSET      7
#define DN_PCIE_CNTL_UR_ERR_REPORT_DIS_DN_MASK        0x80

// Bitfield Description :
#define DN_PCIE_CNTL_Reserved_29_8_OFFSET      8
#define DN_PCIE_CNTL_Reserved_29_8_MASK        0x3fffff00

// Bitfield Description : Disable RX from asserting UR bit to slv when LTR MSG is received and LTR is disabled.
#define DN_PCIE_CNTL_RX_IGNORE_LTR_MSG_UR_OFFSET      30
#define DN_PCIE_CNTL_RX_IGNORE_LTR_MSG_UR_MASK        0x40000000

// Bitfield Description :
#define DN_PCIE_CNTL_Reserved_31_31_OFFSET      31
#define DN_PCIE_CNTL_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HWINIT_WR_LOCK:1;
    UINT32                            Reserved_6_1:6;
    UINT32                            UR_ERR_REPORT_DIS_DN:1;
    UINT32                            Reserved_29_8:22;
    UINT32                            RX_IGNORE_LTR_MSG_UR:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} DN_PCIE_CNTL_STRUCT;

#define SMN_DEV0_NBIF0_DN_PCIE_CNTL_ADDRESS    0x35c8UL

#define SMN_DEV0_NBIF0BIFDEC_DN_PCIE_CNTL_ADDRESS    0x101235c8UL

#define SMN_DEV0_NBIF0PORT0DEC_DN_PCIE_CNTL_ADDRESS    0x101311acUL

#define SMN_DEV1_NBIF0PORT1DEC_DN_PCIE_CNTL_ADDRESS    0x101313acUL

#define SMN_DEV2_NBIF0PORT2DEC_DN_PCIE_CNTL_ADDRESS    0x101315acUL


/***********************************************************
* Register Name : INTR_LINE_ENABLE
* Register Description :
* BIF interrupt line edge detect enable/disable.
* Visibility : 0x4
************************************************************/

// Bitfield Description : each bit per func under port-0. 0: disable interrupt line toggle detector for this function; 1: enable interrupt line detector.
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV0_OFFSET      0
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV0_MASK        0xff

// Bitfield Description : each bit per func under port-1. 0: disable interrupt line toggle detector for this function; 1: enable interrupt line detector.
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV1_OFFSET      8
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV1_MASK        0xff00

// Bitfield Description : each bit per func under port-2. 0: disable interrupt line toggle detector for this function; 1: enable interrupt line detector.
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_OFFSET      16
#define INTR_LINE_ENABLE_INTR_LINE_ENABLE_DEV2_MASK        0xff0000

// Bitfield Description :
#define INTR_LINE_ENABLE_Reserved_31_24_OFFSET      24
#define INTR_LINE_ENABLE_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            INTR_LINE_ENABLE_DEV0:8;
    UINT32                            INTR_LINE_ENABLE_DEV1:8;
    UINT32                            INTR_LINE_ENABLE_DEV2:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} INTR_LINE_ENABLE_STRUCT;

#define SMN_NBIF0_INTR_LINE_ENABLE_ADDRESS    0x1013a008UL


/***********************************************************
* Register Name : NBIF_DS_CTRL_LCLK
* Register Description :
* NBIF LCLK DEEPSLEEP CTRL REGISTER.
* Visibility : 0x4
************************************************************/

// Bitfield Description : NBIF LCLK DS ENABLE.
#define NBIF_DS_CTRL_LCLK_NBIF_LCLK_DS_EN_OFFSET      0
#define NBIF_DS_CTRL_LCLK_NBIF_LCLK_DS_EN_MASK        0x1

// Bitfield Description :
#define NBIF_DS_CTRL_LCLK_Reserved_15_1_OFFSET      1
#define NBIF_DS_CTRL_LCLK_Reserved_15_1_MASK        0xfffe

// Bitfield Description : Idle timer to assert NBIF_NBIO_lclk_idle.
#define NBIF_DS_CTRL_LCLK_NBIF_LCLK_DS_TIMER_OFFSET      16
#define NBIF_DS_CTRL_LCLK_NBIF_LCLK_DS_TIMER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            NBIF_LCLK_DS_EN:1;
    UINT32                            Reserved_15_1:15;
    UINT32                            NBIF_LCLK_DS_TIMER:16;
  } Field;
  UINT32 Value;
} NBIF_DS_CTRL_LCLK_STRUCT;

#define SMN_NBIF0_NBIF_DS_CTRL_LCLK_ADDRESS    0x1013a220UL


/***********************************************************
* Register Name : NBIF_MGCG_CTRL_LCLK
* Register Description :
* NBIF_MGCG CTNTROL REGISTER.
* Visibility : 0x4
************************************************************/

// Bitfield Description : MGCG ENABLE.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_EN_LCLK_OFFSET      0
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_EN_LCLK_MASK        0x1

// Bitfield Description : MGCG working mode.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_MODE_LCLK_OFFSET      1
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_MODE_LCLK_MASK        0x2

// Bitfield Description : MGCG HYSTERESIS for gating count.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HYSTERESIS_LCLK_OFFSET      2
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HYSTERESIS_LCLK_MASK        0x3fc

// Bitfield Description : LCLK MGCG disable for hst branch.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HST_DIS_LCLK_OFFSET      10
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_HST_DIS_LCLK_MASK        0x400

// Bitfield Description : LCLK MGCG disable for dma branch.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_DMA_DIS_LCLK_OFFSET      11
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_DMA_DIS_LCLK_MASK        0x800

// Bitfield Description : LCLK MGCG disable for regs branch.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_REG_DIS_LCLK_OFFSET      12
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_REG_DIS_LCLK_MASK        0x1000

// Bitfield Description : LCLK MGCG disable for aer branch.
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_AER_DIS_LCLK_OFFSET      13
#define NBIF_MGCG_CTRL_LCLK_NBIF_MGCG_AER_DIS_LCLK_MASK        0x2000

// Bitfield Description :
#define NBIF_MGCG_CTRL_LCLK_Reserved_31_14_OFFSET      14
#define NBIF_MGCG_CTRL_LCLK_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            NBIF_MGCG_EN_LCLK:1;
    UINT32                            NBIF_MGCG_MODE_LCLK:1;
    UINT32                            NBIF_MGCG_HYSTERESIS_LCLK:8;
    UINT32                            NBIF_MGCG_HST_DIS_LCLK:1;
    UINT32                            NBIF_MGCG_DMA_DIS_LCLK:1;
    UINT32                            NBIF_MGCG_REG_DIS_LCLK:1;
    UINT32                            NBIF_MGCG_AER_DIS_LCLK:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} NBIF_MGCG_CTRL_LCLK_STRUCT;

#define SMN_NBIF0_NBIF_MGCG_CTRL_LCLK_ADDRESS    0x1013a21cUL

/***********************************************************
* Register Name : BIFC_GMI_SDP_REQ_POOLCRED_ALLOC
* Register Description :
* Register of Pool Credit Allocation for GMI Req.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Pool Credit Allocation for VC0.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC0_ALLOC_OFFSET      0
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC0_ALLOC_MASK        0xf

// Bitfield Description : Pool Credit Allocation for VC1.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC1_ALLOC_OFFSET      4
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC1_ALLOC_MASK        0xf0

// Bitfield Description : Pool Credit Allocation for VC2.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC2_ALLOC_OFFSET      8
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC2_ALLOC_MASK        0xf00

// Bitfield Description : Pool Credit Allocation for VC3.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC3_ALLOC_OFFSET      12
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC3_ALLOC_MASK        0xf000

// Bitfield Description : Pool Credit Allocation for VC4.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC4_ALLOC_OFFSET      16
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC4_ALLOC_MASK        0xf0000

// Bitfield Description : Pool Credit Allocation for VC5.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC5_ALLOC_OFFSET      20
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC5_ALLOC_MASK        0xf00000

// Bitfield Description : Pool Credit Allocation for VC6.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC6_ALLOC_OFFSET      24
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC6_ALLOC_MASK        0xf000000

// Bitfield Description : Pool Credit Allocation for VC7.
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC7_ALLOC_OFFSET      28
#define BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_VC7_ALLOC_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_ALLOC:4;
    UINT32                            VC1_ALLOC:4;
    UINT32                            VC2_ALLOC:4;
    UINT32                            VC3_ALLOC:4;
    UINT32                            VC4_ALLOC:4;
    UINT32                            VC5_ALLOC:4;
    UINT32                            VC6_ALLOC:4;
    UINT32                            VC7_ALLOC:4;
  } Field;
  UINT32 Value;
} BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_STRUCT;

#define SMN_NBIF0_BIFC_GMI_SDP_REQ_POOLCRED_ALLOC_ADDRESS    0x1013a308UL

/***********************************************************
* Register Name : BIFC_GMI_SDP_DAT_POOLCRED_ALLOC
* Register Description :
* Register of Pool Credit Allocation for GMI OrigData.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Pool Credit Allocation for VC0.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC0_ALLOC_OFFSET      0
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC0_ALLOC_MASK        0xf

// Bitfield Description : Pool Credit Allocation for VC1.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC1_ALLOC_OFFSET      4
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC1_ALLOC_MASK        0xf0

// Bitfield Description : Pool Credit Allocation for VC2.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC2_ALLOC_OFFSET      8
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC2_ALLOC_MASK        0xf00

// Bitfield Description : Pool Credit Allocation for VC3.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC3_ALLOC_OFFSET      12
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC3_ALLOC_MASK        0xf000

// Bitfield Description : Pool Credit Allocation for VC4.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC4_ALLOC_OFFSET      16
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC4_ALLOC_MASK        0xf0000

// Bitfield Description : Pool Credit Allocation for VC5.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC5_ALLOC_OFFSET      20
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC5_ALLOC_MASK        0xf00000

// Bitfield Description : Pool Credit Allocation for VC6.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC6_ALLOC_OFFSET      24
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC6_ALLOC_MASK        0xf000000

// Bitfield Description : Pool Credit Allocation for VC7.
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC7_ALLOC_OFFSET      28
#define BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_VC7_ALLOC_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0_ALLOC:4;
    UINT32                            VC1_ALLOC:4;
    UINT32                            VC2_ALLOC:4;
    UINT32                            VC3_ALLOC:4;
    UINT32                            VC4_ALLOC:4;
    UINT32                            VC5_ALLOC:4;
    UINT32                            VC6_ALLOC:4;
    UINT32                            VC7_ALLOC:4;
  } Field;
  UINT32 Value;
} BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_STRUCT;

#define SMN_NBIF0_BIFC_GMI_SDP_DAT_POOLCRED_ALLOC_ADDRESS    0x1013a30cUL

/***********************************************************
* Register Name : SION_CL0_Req_TimeSlot_REG0
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : .
#define SION_CL0_Req_TimeSlot_REG0_Req_TimeSlot_31_0_OFFSET      0
#define SION_CL0_Req_TimeSlot_REG0_Req_TimeSlot_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_31_0:32;
  } Field;
  UINT32 Value;
} SION_CL0_Req_TimeSlot_REG0_STRUCT;

#define SMN_NBIF0_SION_CL0_Req_TimeSlot_REG0_ADDRESS    0x1013a428UL


/***********************************************************
* Register Name : SION_CL0_Req_TimeSlot_REG1
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : .
#define SION_CL0_Req_TimeSlot_REG1_Req_TimeSlot_63_32_OFFSET      0
#define SION_CL0_Req_TimeSlot_REG1_Req_TimeSlot_63_32_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_63_32:32;
  } Field;
  UINT32 Value;
} SION_CL0_Req_TimeSlot_REG1_STRUCT;

#define SMN_NBIF0_SION_CL0_Req_TimeSlot_REG1_ADDRESS    0x1013a42cUL

/***********************************************************
* Register Name : SION_CL2_Req_TimeSlot_REG0
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : .
#define SION_CL2_Req_TimeSlot_REG0_Req_TimeSlot_31_0_OFFSET      0
#define SION_CL2_Req_TimeSlot_REG0_Req_TimeSlot_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_31_0:32;
  } Field;
  UINT32 Value;
} SION_CL2_Req_TimeSlot_REG0_STRUCT;

#define SMN_NBIF0_SION_CL2_Req_TimeSlot_REG0_ADDRESS    0x1013a4c8UL


/***********************************************************
* Register Name : SION_CL2_Req_TimeSlot_REG1
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : .
#define SION_CL2_Req_TimeSlot_REG1_Req_TimeSlot_63_32_OFFSET      0
#define SION_CL2_Req_TimeSlot_REG1_Req_TimeSlot_63_32_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_63_32:32;
  } Field;
  UINT32 Value;
} SION_CL2_Req_TimeSlot_REG1_STRUCT;

#define SMN_NBIF0_SION_CL2_Req_TimeSlot_REG1_ADDRESS    0x1013a4ccUL

/***********************************************************
* Register Name : SION_CNTL_REG1
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : .
#define SION_CNTL_REG1_LIVELOCK_WATCHDOG_THRESHOLD_OFFSET      0
#define SION_CNTL_REG1_LIVELOCK_WATCHDOG_THRESHOLD_MASK        0xff

// Bitfield Description : Hystersis counter for all the MGCG in SION.
#define SION_CNTL_REG1_CG_OFF_HYSTERESIS_OFFSET      8
#define SION_CNTL_REG1_CG_OFF_HYSTERESIS_MASK        0xff00

// Bitfield Description :
#define SION_CNTL_REG1_Reserved_31_16_OFFSET      16
#define SION_CNTL_REG1_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LIVELOCK_WATCHDOG_THRESHOLD:8;
    UINT32                            CG_OFF_HYSTERESIS:8;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} SION_CNTL_REG1_STRUCT;

#define SMN_NBIF0_SION_CNTL_REG1_ADDRESS    0x1013a4f4UL

/***********************************************************
* Register Name : NBIF_PGMST_CTRL
* Register Description :
* nbif pgmst regiser interface set error control registers.
* Visibility : 0x4
************************************************************/

// Bitfield Description : control number of consecutive periods when all PG IPs assert.
#define NBIF_PGMST_CTRL_NBIF_CFG_PG_HYSTERESIS_OFFSET      0
#define NBIF_PGMST_CTRL_NBIF_CFG_PG_HYSTERESIS_MASK        0xff

// Bitfield Description : enable nbif's power gating.
#define NBIF_PGMST_CTRL_NBIF_CFG_PG_EN_OFFSET      8
#define NBIF_PGMST_CTRL_NBIF_CFG_PG_EN_MASK        0x100

// Bitfield Description :
#define NBIF_PGMST_CTRL_Reserved_9_9_OFFSET      9
#define NBIF_PGMST_CTRL_Reserved_9_9_MASK        0x200

// Bitfield Description : 0: idleness_counter is disabled, PGMST uses only hysteresis timeout value to enter power gating.
#define NBIF_PGMST_CTRL_NBIF_CFG_IDLENESS_COUNT_EN_OFFSET      10
#define NBIF_PGMST_CTRL_NBIF_CFG_IDLENESS_COUNT_EN_MASK        0x3c00

// Bitfield Description : 2'bx1: enable sending the pwrmgr_int for FW interlock feature, 2'b1x: Enable wakup tile from power gated state using the pwrmgt_int_clear signal
#define NBIF_PGMST_CTRL_NBIF_CFG_FW_PG_EXIT_EN_OFFSET      14
#define NBIF_PGMST_CTRL_NBIF_CFG_FW_PG_EXIT_EN_MASK        0xc000

// Bitfield Description :
#define NBIF_PGMST_CTRL_Reserved_31_16_OFFSET      16
#define NBIF_PGMST_CTRL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            NBIF_CFG_PG_HYSTERESIS:8;
    UINT32                            NBIF_CFG_PG_EN:1;
    UINT32                            Reserved_9_9:1;
    UINT32                            NBIF_CFG_IDLENESS_COUNT_EN:4;
    UINT32                            NBIF_CFG_FW_PG_EXIT_EN:2;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} NBIF_PGMST_CTRL_STRUCT;

#define SMN_NBIF0_NBIF_PGMST_CTRL_ADDRESS    0x1013a0e0UL

/***********************************************************
* Register Name : BIF_GMI_WRR_WEIGHT2
* Register Description :
* nbif DMA VC arbitrator weight.
* Visibility : 0x4
************************************************************/

// Bitfield Description : GMI Weighted Round Robin arbitrator weight. Per-arbiter entry. Mode-0, 64B number; Mode-1, req number.
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY0_WEIGHT_OFFSET      0
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY0_WEIGHT_MASK        0xff

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY1_WEIGHT_OFFSET      8
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY1_WEIGHT_MASK        0xff00

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY2_WEIGHT_OFFSET      16
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY2_WEIGHT_MASK        0xff0000

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY3_WEIGHT_OFFSET      24
#define BIF_GMI_WRR_WEIGHT2_GMI_REQ_ENTRY3_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GMI_REQ_ENTRY0_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY1_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY2_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY3_WEIGHT:8;
  } Field;
  UINT32 Value;
} BIF_GMI_WRR_WEIGHT2_STRUCT;

#define SMN_NBIF0_BIF_GMI_WRR_WEIGHT2_ADDRESS    0x1013a124UL

/***********************************************************
* Register Name : BIF_GMI_WRR_WEIGHT3
* Register Description :
* nbif DMA VC arbitrator weight.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY4_WEIGHT_OFFSET      0
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY4_WEIGHT_MASK        0xff

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY5_WEIGHT_OFFSET      8
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY5_WEIGHT_MASK        0xff00

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY6_WEIGHT_OFFSET      16
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY6_WEIGHT_MASK        0xff0000

// Bitfield Description :
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY7_WEIGHT_OFFSET      24
#define BIF_GMI_WRR_WEIGHT3_GMI_REQ_ENTRY7_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GMI_REQ_ENTRY4_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY5_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY6_WEIGHT:8;
    UINT32                            GMI_REQ_ENTRY7_WEIGHT:8;
  } Field;
  UINT32 Value;
} BIF_GMI_WRR_WEIGHT3_STRUCT;

#define SMN_NBIF0_BIF_GMI_WRR_WEIGHT3_ADDRESS    0x1013a128UL

/***********************************************************
* Register Name : NBIF_SMN_VWR_VCHG_DIS_CTRL
* Register Description :
* NBIF-SMN VIRTURAL WIRE DISABLE REGISTER PER SET.
* Visibility : 0x4
************************************************************/

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set0 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET0_DIS_OFFSET      0
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET0_DIS_MASK        0x1

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set1 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET1_DIS_OFFSET      1
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET1_DIS_MASK        0x2

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set2 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET2_DIS_OFFSET      2
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET2_DIS_MASK        0x4

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set3 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET3_DIS_OFFSET      3
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET3_DIS_MASK        0x8

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set4 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET4_DIS_OFFSET      4
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET4_DIS_MASK        0x10

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set5 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET5_DIS_OFFSET      5
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET5_DIS_MASK        0x20

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set6 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET6_DIS_OFFSET      6
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET6_DIS_MASK        0x40

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set7 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET7_DIS_OFFSET      7
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET7_DIS_MASK        0x80

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set8 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET8_DIS_OFFSET      8
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET8_DIS_MASK        0x100

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set9 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET9_DIS_OFFSET      9
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET9_DIS_MASK        0x200

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set10 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET10_DIS_OFFSET      10
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET10_DIS_MASK        0x400

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set11 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET11_DIS_OFFSET      11
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET11_DIS_MASK        0x800

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set12 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET12_DIS_OFFSET      12
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET12_DIS_MASK        0x1000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set13 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET13_DIS_OFFSET      13
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET13_DIS_MASK        0x2000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set14 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET14_DIS_OFFSET      14
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET14_DIS_MASK        0x4000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set15 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET15_DIS_OFFSET      15
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET15_DIS_MASK        0x8000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set16 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET16_DIS_OFFSET      16
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET16_DIS_MASK        0x10000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set17 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET17_DIS_OFFSET      17
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET17_DIS_MASK        0x20000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set18 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET18_DIS_OFFSET      18
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET18_DIS_MASK        0x40000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set19 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET19_DIS_OFFSET      19
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET19_DIS_MASK        0x80000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set20 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET20_DIS_OFFSET      20
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET20_DIS_MASK        0x100000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set21 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET21_DIS_OFFSET      21
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET21_DIS_MASK        0x200000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set22 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET22_DIS_OFFSET      22
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET22_DIS_MASK        0x400000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set23 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET23_DIS_OFFSET      23
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET23_DIS_MASK        0x800000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set24 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET24_DIS_OFFSET      24
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET24_DIS_MASK        0x1000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set25 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET25_DIS_OFFSET      25
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET25_DIS_MASK        0x2000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set26 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET26_DIS_OFFSET      26
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET26_DIS_MASK        0x4000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set27 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET27_DIS_OFFSET      27
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET27_DIS_MASK        0x8000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set28 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET28_DIS_OFFSET      28
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET28_DIS_MASK        0x10000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set29 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET29_DIS_OFFSET      29
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET29_DIS_MASK        0x20000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set30 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET30_DIS_OFFSET      30
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET30_DIS_MASK        0x40000000

// Bitfield Description : 1: SMN Virtual wire feature local disable for all events of set31 . sticky.
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET31_DIS_OFFSET      31
#define NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET31_DIS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SMN_VWR_VCHG_SET0_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET1_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET2_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET3_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET4_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET5_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET6_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET7_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET8_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET9_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET10_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET11_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET12_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET13_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET14_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET15_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET16_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET17_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET18_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET19_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET20_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET21_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET22_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET23_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET24_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET25_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET26_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET27_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET28_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET29_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET30_DIS:1;
    UINT32                            SMN_VWR_VCHG_SET31_DIS:1;
  } Field;
  UINT32 Value;
} NBIF_SMN_VWR_VCHG_DIS_CTRL_STRUCT;

#define SMN_NBIF0_NBIF_SMN_VWR_VCHG_DIS_CTRL_ADDRESS    0x1013a204UL


/***********************************************************
* Register Name : RCC_CONFIG_MEMSIZE
* Register Description :
* Scratch regsiter for BIOS to inform driver memory size.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define RCC_CONFIG_MEMSIZE_CONFIG_MEMSIZE_OFFSET      0
#define RCC_CONFIG_MEMSIZE_CONFIG_MEMSIZE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONFIG_MEMSIZE:32;
  } Field;
  UINT32 Value;
} RCC_CONFIG_MEMSIZE_STRUCT;

#define SMN_NBIF0_RCC_CONFIG_MEMSIZE_ADDRESS    0x378cUL


/***********************************************************
* Register Name : RCC_DEV0_EPF0_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV0_EPF0_STRAP0_STRAP_DEVICE_ID_DEV0_F0_OFFSET      0
#define RCC_DEV0_EPF0_STRAP0_STRAP_DEVICE_ID_DEV0_F0_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV0_EPF0_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F0_OFFSET      16
#define RCC_DEV0_EPF0_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F0_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV0_EPF0_STRAP0_STRAP_MINOR_REV_ID_DEV0_F0_OFFSET      20
#define RCC_DEV0_EPF0_STRAP0_STRAP_MINOR_REV_ID_DEV0_F0_MASK        0xf00000

// Bitfield Description : GFX F0 only                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                , ATI_REV_ID.
#define RCC_DEV0_EPF0_STRAP0_STRAP_ATI_REV_ID_DEV0_F0_OFFSET      24
#define RCC_DEV0_EPF0_STRAP0_STRAP_ATI_REV_ID_DEV0_F0_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV0_EPF0_STRAP0_STRAP_FUNC_EN_DEV0_F0_OFFSET      28
#define RCC_DEV0_EPF0_STRAP0_STRAP_FUNC_EN_DEV0_F0_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV0_EPF0_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F0_OFFSET      29
#define RCC_DEV0_EPF0_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F0_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV0_EPF0_STRAP0_STRAP_D1_SUPPORT_DEV0_F0_OFFSET      30
#define RCC_DEV0_EPF0_STRAP0_STRAP_D1_SUPPORT_DEV0_F0_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV0_EPF0_STRAP0_STRAP_D2_SUPPORT_DEV0_F0_OFFSET      31
#define RCC_DEV0_EPF0_STRAP0_STRAP_D2_SUPPORT_DEV0_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV0_F0:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV0_F0:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV0_F0:4;
    UINT32                            STRAP_ATI_REV_ID_DEV0_F0:4;
    UINT32                            STRAP_FUNC_EN_DEV0_F0:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F0:1;
    UINT32                            STRAP_D1_SUPPORT_DEV0_F0:1;
    UINT32                            STRAP_D2_SUPPORT_DEV0_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF0_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP0_ADDRESS    0x10134000UL

#define SMN_NBIF0_OFFSETH234D4_RCC_DEV0_EPF0_STRAP0_ADDRESS    0x101234d4UL

#define SMN_NBIF0_OFFSETH54_RCC_DEV0_EPF0_STRAP0_ADDRESS    0x34d4UL


/***********************************************************
* Register Name : RCC_DEV0_EPF0_STRAP13
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Programming Interface of Class Code for EP function.
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F0_OFFSET      0
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F0_MASK        0xff

// Bitfield Description : Sub-Class-Code of Class Code for EP funciton.
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F0_OFFSET      8
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F0_MASK        0xff00

// Bitfield Description : Base-Class-Code of Class Code for EP function.
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F0_OFFSET      16
#define RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F0_MASK        0xff0000

// Bitfield Description : Set SRIOV_CAP.TOTAL_VF.
#define RCC_DEV0_EPF0_STRAP13_STRAP_SRIOV_TOTAL_VFS_DEV0_F0_OFFSET      24
#define RCC_DEV0_EPF0_STRAP13_STRAP_SRIOV_TOTAL_VFS_DEV0_F0_MASK        0x7f000000

// Bitfield Description :
#define RCC_DEV0_EPF0_STRAP13_Reserved_31_31_OFFSET      31
#define RCC_DEV0_EPF0_STRAP13_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_CLASS_CODE_PIF_DEV0_F0:8;
    UINT32                            STRAP_CLASS_CODE_SUB_DEV0_F0:8;
    UINT32                            STRAP_CLASS_CODE_BASE_DEV0_F0:8;
    UINT32                            STRAP_SRIOV_TOTAL_VFS_DEV0_F0:7;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF0_STRAP13_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP13_ADDRESS    0x10134034UL

#define SMN_NBIF0_OFFSETH234DC_RCC_DEV0_EPF0_STRAP13_ADDRESS    0x101234dcUL

#define SMN_NBIF0_OFFSETH5C_RCC_DEV0_EPF0_STRAP13_ADDRESS    0x34dcUL


/***********************************************************
* Register Name : RCC_DEV0_EPF0_STRAP2
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable SRIOV cap.
#define RCC_DEV0_EPF0_STRAP2_STRAP_SRIOV_EN_DEV0_F0_OFFSET      0
#define RCC_DEV0_EPF0_STRAP2_STRAP_SRIOV_EN_DEV0_F0_MASK        0x1

// Bitfield Description :
#define RCC_DEV0_EPF0_STRAP2_Reserved_5_1_OFFSET      1
#define RCC_DEV0_EPF0_STRAP2_Reserved_5_1_MASK        0x3e

// Bitfield Description : GFX 64-bit BAR enable.
#define RCC_DEV0_EPF0_STRAP2_STRAP_64BAR_DIS_DEV0_F0_OFFSET      6
#define RCC_DEV0_EPF0_STRAP2_STRAP_64BAR_DIS_DEV0_F0_MASK        0x40

// Bitfield Description : Set PMCSR.NO_SOFT_RESET for EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_NO_SOFT_RESET_DEV0_F0_OFFSET      7
#define RCC_DEV0_EPF0_STRAP2_STRAP_NO_SOFT_RESET_DEV0_F0_MASK        0x80

// Bitfield Description : Enable RESIZABLE BAR cap.
#define RCC_DEV0_EPF0_STRAP2_STRAP_RESIZE_BAR_EN_DEV0_F0_OFFSET      8
#define RCC_DEV0_EPF0_STRAP2_STRAP_RESIZE_BAR_EN_DEV0_F0_MASK        0x100

// Bitfield Description : Indicates the width of PASID field supported by the ATC.
#define RCC_DEV0_EPF0_STRAP2_STRAP_MAX_PASID_WIDTH_DEV0_F0_OFFSET      9
#define RCC_DEV0_EPF0_STRAP2_STRAP_MAX_PASID_WIDTH_DEV0_F0_MASK        0x3e00

// Bitfield Description : Enable Per-Vector Masking for MSI.
#define RCC_DEV0_EPF0_STRAP2_STRAP_MSI_PERVECTOR_MASK_CAP_DEV0_F0_OFFSET      14
#define RCC_DEV0_EPF0_STRAP2_STRAP_MSI_PERVECTOR_MASK_CAP_DEV0_F0_MASK        0x4000

// Bitfield Description : Enable ARI cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_ARI_EN_DEV0_F0_OFFSET      15
#define RCC_DEV0_EPF0_STRAP2_STRAP_ARI_EN_DEV0_F0_MASK        0x8000

// Bitfield Description : Enable AER cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_AER_EN_DEV0_F0_OFFSET      16
#define RCC_DEV0_EPF0_STRAP2_STRAP_AER_EN_DEV0_F0_MASK        0x10000

// Bitfield Description : Enable ACS cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_ACS_EN_DEV0_F0_OFFSET      17
#define RCC_DEV0_EPF0_STRAP2_STRAP_ACS_EN_DEV0_F0_MASK        0x20000

// Bitfield Description : Enable ATS cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_ATS_EN_DEV0_F0_OFFSET      18
#define RCC_DEV0_EPF0_STRAP2_STRAP_ATS_EN_DEV0_F0_MASK        0x40000

// Bitfield Description :
#define RCC_DEV0_EPF0_STRAP2_Reserved_19_19_OFFSET      19
#define RCC_DEV0_EPF0_STRAP2_Reserved_19_19_MASK        0x80000

// Bitfield Description : Enable CA error in AER capability in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_CPL_ABORT_ERR_EN_DEV0_F0_OFFSET      20
#define RCC_DEV0_EPF0_STRAP2_STRAP_CPL_ABORT_ERR_EN_DEV0_F0_MASK        0x100000

// Bitfield Description : Enable DPA cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_DPA_EN_DEV0_F0_OFFSET      21
#define RCC_DEV0_EPF0_STRAP2_STRAP_DPA_EN_DEV0_F0_MASK        0x200000

// Bitfield Description : Enable DSN cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_DSN_EN_DEV0_F0_OFFSET      22
#define RCC_DEV0_EPF0_STRAP2_STRAP_DSN_EN_DEV0_F0_MASK        0x400000

// Bitfield Description : Enable VC cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_VC_EN_DEV0_F0_OFFSET      23
#define RCC_DEV0_EPF0_STRAP2_STRAP_VC_EN_DEV0_F0_MASK        0x800000

// Bitfield Description : Set MSI_CAP.MSI_MULTI_CAP in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_MSI_MULTI_CAP_DEV0_F0_OFFSET      24
#define RCC_DEV0_EPF0_STRAP2_STRAP_MSI_MULTI_CAP_DEV0_F0_MASK        0x7000000

// Bitfield Description : Enable PRI cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_PAGE_REQ_EN_DEV0_F0_OFFSET      27
#define RCC_DEV0_EPF0_STRAP2_STRAP_PAGE_REQ_EN_DEV0_F0_MASK        0x8000000

// Bitfield Description : Enable PASID cap in EP function.
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_EN_DEV0_F0_OFFSET      28
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_EN_DEV0_F0_MASK        0x10000000

// Bitfield Description : Enable PASID_CAP.EXE_PERMISSION_SUPPORTED.
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_EXE_PERMISSION_SUPPORTED_DEV0_F0_OFFSET      29
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_EXE_PERMISSION_SUPPORTED_DEV0_F0_MASK        0x20000000

// Bitfield Description : Enable PASID_CAP.GLOBAL_INVALIDATE_SUPPORTED.
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_GLOBAL_INVALIDATE_SUPPORTED_DEV0_F0_OFFSET      30
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_GLOBAL_INVALIDATE_SUPPORTED_DEV0_F0_MASK        0x40000000

// Bitfield Description : Enable PASID_CAP.PRIV_MODE_SUPPORTED.
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_PRIV_MODE_SUPPORTED_DEV0_F0_OFFSET      31
#define RCC_DEV0_EPF0_STRAP2_STRAP_PASID_PRIV_MODE_SUPPORTED_DEV0_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_SRIOV_EN_DEV0_F0:1;
    UINT32                            Reserved_5_1:5;
    UINT32                            STRAP_64BAR_DIS_DEV0_F0:1;
    UINT32                            STRAP_NO_SOFT_RESET_DEV0_F0:1;
    UINT32                            STRAP_RESIZE_BAR_EN_DEV0_F0:1;
    UINT32                            STRAP_MAX_PASID_WIDTH_DEV0_F0:5;
    UINT32                            STRAP_MSI_PERVECTOR_MASK_CAP_DEV0_F0:1;
    UINT32                            STRAP_ARI_EN_DEV0_F0:1;
    UINT32                            STRAP_AER_EN_DEV0_F0:1;
    UINT32                            STRAP_ACS_EN_DEV0_F0:1;
    UINT32                            STRAP_ATS_EN_DEV0_F0:1;
    UINT32                            Reserved_19_19:1;
    UINT32                            STRAP_CPL_ABORT_ERR_EN_DEV0_F0:1;
    UINT32                            STRAP_DPA_EN_DEV0_F0:1;
    UINT32                            STRAP_DSN_EN_DEV0_F0:1;
    UINT32                            STRAP_VC_EN_DEV0_F0:1;
    UINT32                            STRAP_MSI_MULTI_CAP_DEV0_F0:3;
    UINT32                            STRAP_PAGE_REQ_EN_DEV0_F0:1;
    UINT32                            STRAP_PASID_EN_DEV0_F0:1;
    UINT32                            STRAP_PASID_EXE_PERMISSION_SUPPORTED_DEV0_F0:1;
    UINT32                            STRAP_PASID_GLOBAL_INVALIDATE_SUPPORTED_DEV0_F0:1;
    UINT32                            STRAP_PASID_PRIV_MODE_SUPPORTED_DEV0_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF0_STRAP2_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP2_ADDRESS    0x10134008UL

#define SMN_NBIF0_OFFSETH234F4_RCC_DEV0_EPF0_STRAP2_ADDRESS    0x101234f4UL

#define SMN_NBIF0_OFFSETH74_RCC_DEV0_EPF0_STRAP2_ADDRESS    0x34f4UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF1_STRAP2_ADDRESS    0x10134208UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF2_STRAP2_ADDRESS    0x10134408UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF3_STRAP2_ADDRESS    0x10134608UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF4_STRAP2_ADDRESS    0x10134808UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF5_STRAP2_ADDRESS    0x10134a08UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF6_STRAP2_ADDRESS    0x10134c08UL

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF7_STRAP2_ADDRESS    0x10134e08UL

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF0_STRAP2_ADDRESS    0x10135008UL

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF2_STRAP2_ADDRESS    0x10135408UL

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF3_STRAP2_ADDRESS    0x10135608UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF0_STRAP2_ADDRESS    0x10136008UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF2_STRAP2_ADDRESS    0x10136408UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF3_STRAP2_ADDRESS    0x10136608UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF4_STRAP2_ADDRESS    0x10136808UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF5_STRAP2_ADDRESS    0x10136A08UL

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF6_STRAP2_ADDRESS    0x10136C08UL

/***********************************************************
* Register Name : RCC_DEV0_EPF0_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F0_OFFSET      0
#define RCC_DEV0_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F0_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_PWR_EN_DEV0_F0_OFFSET      1
#define RCC_DEV0_EPF0_STRAP3_STRAP_PWR_EN_DEV0_F0_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV0_F0_OFFSET      2
#define RCC_DEV0_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV0_F0_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSI_EN_DEV0_F0_OFFSET      18
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSI_EN_DEV0_F0_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F0_OFFSET      19
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F0_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_EN_DEV0_F0_OFFSET      20
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_EN_DEV0_F0_MASK        0x100000

// Bitfield Description : Set MSIX_CAP.MSIX_TABLE_BIR in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_TABLE_BIR_DEV0_F0_OFFSET      21
#define RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_TABLE_BIR_DEV0_F0_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV0_EPF0_STRAP3_STRAP_PMC_DSI_DEV0_F0_OFFSET      24
#define RCC_DEV0_EPF0_STRAP3_STRAP_PMC_DSI_DEV0_F0_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV0_EPF0_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV0_EPF0_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV0_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F0_OFFSET      26
#define RCC_DEV0_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F0_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV0_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F0_OFFSET      27
#define RCC_DEV0_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F0_MASK        0x8000000

// Bitfield Description : Enable VF ResizableBAR cap.
#define RCC_DEV0_EPF0_STRAP3_STRAP_VF_RESIZE_BAR_EN_DEV0_F0_OFFSET      28
#define RCC_DEV0_EPF0_STRAP3_STRAP_VF_RESIZE_BAR_EN_DEV0_F0_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV0_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV0_F0_OFFSET      29
#define RCC_DEV0_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV0_F0_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV0_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F0_OFFSET      30
#define RCC_DEV0_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F0_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV0_EPF0_STRAP3_STRAP_RTR_EN_DEV0_F0_OFFSET      31
#define RCC_DEV0_EPF0_STRAP3_STRAP_RTR_EN_DEV0_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F0:1;
    UINT32                            STRAP_PWR_EN_DEV0_F0:1;
    UINT32                            STRAP_SUBSYS_ID_DEV0_F0:16;
    UINT32                            STRAP_MSI_EN_DEV0_F0:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV0_F0:1;
    UINT32                            STRAP_MSIX_EN_DEV0_F0:1;
    UINT32                            STRAP_MSIX_TABLE_BIR_DEV0_F0:3;
    UINT32                            STRAP_PMC_DSI_DEV0_F0:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F0:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F0:1;
    UINT32                            STRAP_VF_RESIZE_BAR_EN_DEV0_F0:1;
    UINT32                            STRAP_CLK_PM_EN_DEV0_F0:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV0_F0:1;
    UINT32                            STRAP_RTR_EN_DEV0_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF0_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP3_ADDRESS    0x1013400cUL

/***********************************************************
* Register Name : RCC_DEV0_EPF3_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F3_OFFSET      0
#define RCC_DEV0_EPF3_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F3_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_PWR_EN_DEV0_F3_OFFSET      1
#define RCC_DEV0_EPF3_STRAP3_STRAP_PWR_EN_DEV0_F3_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_SUBSYS_ID_DEV0_F3_OFFSET      2
#define RCC_DEV0_EPF3_STRAP3_STRAP_SUBSYS_ID_DEV0_F3_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSI_EN_DEV0_F3_OFFSET      18
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSI_EN_DEV0_F3_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F3_OFFSET      19
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F3_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSIX_EN_DEV0_F3_OFFSET      20
#define RCC_DEV0_EPF3_STRAP3_STRAP_MSIX_EN_DEV0_F3_MASK        0x100000

// Bitfield Description :
#define RCC_DEV0_EPF3_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV0_EPF3_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV0_EPF3_STRAP3_STRAP_PMC_DSI_DEV0_F3_OFFSET      24
#define RCC_DEV0_EPF3_STRAP3_STRAP_PMC_DSI_DEV0_F3_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV0_EPF3_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV0_EPF3_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV0_EPF3_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F3_OFFSET      26
#define RCC_DEV0_EPF3_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F3_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV0_EPF3_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F3_OFFSET      27
#define RCC_DEV0_EPF3_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F3_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV0_EPF3_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV0_EPF3_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV0_EPF3_STRAP3_STRAP_CLK_PM_EN_DEV0_F3_OFFSET      29
#define RCC_DEV0_EPF3_STRAP3_STRAP_CLK_PM_EN_DEV0_F3_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV0_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F3_OFFSET      30
#define RCC_DEV0_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F3_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV0_EPF3_STRAP3_STRAP_RTR_EN_DEV0_F3_OFFSET      31
#define RCC_DEV0_EPF3_STRAP3_STRAP_RTR_EN_DEV0_F3_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F3:1;
    UINT32                            STRAP_PWR_EN_DEV0_F3:1;
    UINT32                            STRAP_SUBSYS_ID_DEV0_F3:16;
    UINT32                            STRAP_MSI_EN_DEV0_F3:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV0_F3:1;
    UINT32                            STRAP_MSIX_EN_DEV0_F3:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV0_F3:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F3:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F3:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV0_F3:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV0_F3:1;
    UINT32                            STRAP_RTR_EN_DEV0_F3:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF3_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF3_STRAP3_ADDRESS    0x1013460cUL

/***********************************************************
* Register Name : RCC_DEV0_EPF4_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F4_OFFSET      0
#define RCC_DEV0_EPF4_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F4_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_PWR_EN_DEV0_F4_OFFSET      1
#define RCC_DEV0_EPF4_STRAP3_STRAP_PWR_EN_DEV0_F4_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_SUBSYS_ID_DEV0_F4_OFFSET      2
#define RCC_DEV0_EPF4_STRAP3_STRAP_SUBSYS_ID_DEV0_F4_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSI_EN_DEV0_F4_OFFSET      18
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSI_EN_DEV0_F4_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F4_OFFSET      19
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV0_F4_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSIX_EN_DEV0_F4_OFFSET      20
#define RCC_DEV0_EPF4_STRAP3_STRAP_MSIX_EN_DEV0_F4_MASK        0x100000

// Bitfield Description :
#define RCC_DEV0_EPF4_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV0_EPF4_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV0_EPF4_STRAP3_STRAP_PMC_DSI_DEV0_F4_OFFSET      24
#define RCC_DEV0_EPF4_STRAP3_STRAP_PMC_DSI_DEV0_F4_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV0_EPF4_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV0_EPF4_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV0_EPF4_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F4_OFFSET      26
#define RCC_DEV0_EPF4_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F4_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV0_EPF4_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F4_OFFSET      27
#define RCC_DEV0_EPF4_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F4_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV0_EPF4_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV0_EPF4_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV0_EPF4_STRAP3_STRAP_CLK_PM_EN_DEV0_F4_OFFSET      29
#define RCC_DEV0_EPF4_STRAP3_STRAP_CLK_PM_EN_DEV0_F4_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV0_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F4_OFFSET      30
#define RCC_DEV0_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV0_F4_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV0_EPF4_STRAP3_STRAP_RTR_EN_DEV0_F4_OFFSET      31
#define RCC_DEV0_EPF4_STRAP3_STRAP_RTR_EN_DEV0_F4_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV0_F4:1;
    UINT32                            STRAP_PWR_EN_DEV0_F4:1;
    UINT32                            STRAP_SUBSYS_ID_DEV0_F4:16;
    UINT32                            STRAP_MSI_EN_DEV0_F4:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV0_F4:1;
    UINT32                            STRAP_MSIX_EN_DEV0_F4:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV0_F4:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV0_F4:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV0_F4:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV0_F4:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV0_F4:1;
    UINT32                            STRAP_RTR_EN_DEV0_F4:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF4_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF4_STRAP3_ADDRESS    0x1013480cUL

#define SMN_NBIF0_OFFSETH234F8_RCC_DEV0_EPF0_STRAP3_ADDRESS    0x101234f8UL

#define SMN_NBIF0_OFFSETH78_RCC_DEV0_EPF0_STRAP3_ADDRESS    0x34f8UL


/***********************************************************
* Register Name : RCC_DEV0_EPF0_STRAP4
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Reserved straps. bit0 used as GFX_PF_MSIX_VEC3_DIS. bit1 used as S5_REQID_UPDATE_ON_CAPTURE. bit2 used as S5_PAD_CNTL_REGS_ACCESS_ALWAYS_ALLOWED. bit3 used as VFCFG_ACCESS_PROTECTION_EN.
#define RCC_DEV0_EPF0_STRAP4_STRAP_RESERVED_STRAP4_DEV0_F0_OFFSET      0
#define RCC_DEV0_EPF0_STRAP4_STRAP_RESERVED_STRAP4_DEV0_F0_MASK        0x3ff

// Bitfield Description :
#define RCC_DEV0_EPF0_STRAP4_Reserved_19_10_OFFSET      10
#define RCC_DEV0_EPF0_STRAP4_Reserved_19_10_MASK        0xffc00

// Bitfield Description : Enable 64bit AtomicOP Completer in EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_ATOMIC_64BIT_EN_DEV0_F0_OFFSET      20
#define RCC_DEV0_EPF0_STRAP4_STRAP_ATOMIC_64BIT_EN_DEV0_F0_MASK        0x100000

// Bitfield Description : Enable 32bit AtomicOP Completer in EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_ATOMIC_EN_DEV0_F0_OFFSET      21
#define RCC_DEV0_EPF0_STRAP4_STRAP_ATOMIC_EN_DEV0_F0_MASK        0x200000

// Bitfield Description : Enable FLR support in EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_FLR_EN_DEV0_F0_OFFSET      22
#define RCC_DEV0_EPF0_STRAP4_STRAP_FLR_EN_DEV0_F0_MASK        0x400000

// Bitfield Description : Set PMC.PME_support for EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_PME_SUPPORT_DEV0_F0_OFFSET      23
#define RCC_DEV0_EPF0_STRAP4_STRAP_PME_SUPPORT_DEV0_F0_MASK        0xf800000

// Bitfield Description : Set INTERRUPT_PIN for EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_INTERRUPT_PIN_DEV0_F0_OFFSET      28
#define RCC_DEV0_EPF0_STRAP4_STRAP_INTERRUPT_PIN_DEV0_F0_MASK        0x70000000

// Bitfield Description : Indicate AuxPower support  for EP function.
#define RCC_DEV0_EPF0_STRAP4_STRAP_AUXPWR_SUPPORT_DEV0_F0_OFFSET      31
#define RCC_DEV0_EPF0_STRAP4_STRAP_AUXPWR_SUPPORT_DEV0_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_RESERVED_STRAP4_DEV0_F0:10;
    UINT32                            Reserved_19_10:10;
    UINT32                            STRAP_ATOMIC_64BIT_EN_DEV0_F0:1;
    UINT32                            STRAP_ATOMIC_EN_DEV0_F0:1;
    UINT32                            STRAP_FLR_EN_DEV0_F0:1;
    UINT32                            STRAP_PME_SUPPORT_DEV0_F0:5;
    UINT32                            STRAP_INTERRUPT_PIN_DEV0_F0:3;
    UINT32                            STRAP_AUXPWR_SUPPORT_DEV0_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF0_STRAP4_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP4_ADDRESS    0x10134010UL

#define SMN_NBIF0_OFFSETH234FC_RCC_DEV0_EPF0_STRAP4_ADDRESS    0x101234fcUL

#define SMN_NBIF0_OFFSETH7C_RCC_DEV0_EPF0_STRAP4_ADDRESS    0x34fcUL


/***********************************************************
* Register Name : RCC_DEV0_EPF1_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV0_EPF1_STRAP0_STRAP_DEVICE_ID_DEV0_F1_OFFSET      0
#define RCC_DEV0_EPF1_STRAP0_STRAP_DEVICE_ID_DEV0_F1_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV0_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F1_OFFSET      16
#define RCC_DEV0_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F1_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV0_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV0_F1_OFFSET      20
#define RCC_DEV0_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV0_F1_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV0_EPF1_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV0_EPF1_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_OFFSET      28
#define RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV0_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F1_OFFSET      29
#define RCC_DEV0_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F1_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV0_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV0_F1_OFFSET      30
#define RCC_DEV0_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV0_F1_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV0_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV0_F1_OFFSET      31
#define RCC_DEV0_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV0_F1_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV0_F1:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV0_F1:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV0_F1:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV0_F1:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F1:1;
    UINT32                            STRAP_D1_SUPPORT_DEV0_F1:1;
    UINT32                            STRAP_D2_SUPPORT_DEV0_F1:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF1_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF1_STRAP0_ADDRESS    0x10134200UL

#define SMN_NBIF0_OFFSETH2350C_RCC_DEV0_EPF1_STRAP0_ADDRESS    0x1012350cUL

#define SMN_NBIF0_OFFSETH8C_RCC_DEV0_EPF1_STRAP0_ADDRESS    0x350cUL


/***********************************************************
* Register Name : RCC_DEV0_EPF5_STRAP13
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Programming Interface of Class Code for EP function.
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F5_OFFSET      0
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F5_MASK        0xff

// Bitfield Description : Sub-Class-Code of Class Code for EP funciton.
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F5_OFFSET      8
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F5_MASK        0xff00

// Bitfield Description : Base-Class-Code of Class Code for EP function.
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F5_OFFSET      16
#define RCC_DEV0_EPF5_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F5_MASK        0xff0000

// Bitfield Description :
#define RCC_DEV0_EPF5_STRAP13_Reserved_31_24_OFFSET      24
#define RCC_DEV0_EPF5_STRAP13_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            STRAP_CLASS_CODE_PIF_DEV0_F5:8;
    UINT32                            STRAP_CLASS_CODE_SUB_DEV0_F5:8;
    UINT32                            STRAP_CLASS_CODE_BASE_DEV0_F5:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF5_STRAP13_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF5_STRAP13_ADDRESS    0x10134a34UL


/***********************************************************
* Register Name : RCC_DEV0_EPF6_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV0_EPF6_STRAP0_STRAP_DEVICE_ID_DEV0_F6_OFFSET      0
#define RCC_DEV0_EPF6_STRAP0_STRAP_DEVICE_ID_DEV0_F6_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV0_EPF6_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F6_OFFSET      16
#define RCC_DEV0_EPF6_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F6_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV0_EPF6_STRAP0_STRAP_MINOR_REV_ID_DEV0_F6_OFFSET      20
#define RCC_DEV0_EPF6_STRAP0_STRAP_MINOR_REV_ID_DEV0_F6_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV0_EPF6_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV0_EPF6_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV0_EPF6_STRAP0_STRAP_FUNC_EN_DEV0_F6_OFFSET      28
#define RCC_DEV0_EPF6_STRAP0_STRAP_FUNC_EN_DEV0_F6_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV0_EPF6_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F6_OFFSET      29
#define RCC_DEV0_EPF6_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F6_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV0_EPF6_STRAP0_STRAP_D1_SUPPORT_DEV0_F6_OFFSET      30
#define RCC_DEV0_EPF6_STRAP0_STRAP_D1_SUPPORT_DEV0_F6_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV0_EPF6_STRAP0_STRAP_D2_SUPPORT_DEV0_F6_OFFSET      31
#define RCC_DEV0_EPF6_STRAP0_STRAP_D2_SUPPORT_DEV0_F6_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV0_F6:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV0_F6:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV0_F6:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV0_F6:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F6:1;
    UINT32                            STRAP_D1_SUPPORT_DEV0_F6:1;
    UINT32                            STRAP_D2_SUPPORT_DEV0_F6:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF6_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF6_STRAP0_ADDRESS    0x10134c00UL


/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define RCC_DEV0_PORT_STRAP0_Reserved_0_0_OFFSET      0
#define RCC_DEV0_PORT_STRAP0_Reserved_0_0_MASK        0x1

// Bitfield Description : Enable ARI forwarding in Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_ARI_EN_DN_DEV0_OFFSET      1
#define RCC_DEV0_PORT_STRAP0_STRAP_ARI_EN_DN_DEV0_MASK        0x2

// Bitfield Description : Enable ACS capability in Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_ACS_EN_DN_DEV0_OFFSET      2
#define RCC_DEV0_PORT_STRAP0_STRAP_ACS_EN_DN_DEV0_MASK        0x4

// Bitfield Description : Enable AER capability in Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_AER_EN_DN_DEV0_OFFSET      3
#define RCC_DEV0_PORT_STRAP0_STRAP_AER_EN_DN_DEV0_MASK        0x8

// Bitfield Description : Enable CA error in AER capability in Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_CPL_ABORT_ERR_EN_DN_DEV0_OFFSET      4
#define RCC_DEV0_PORT_STRAP0_STRAP_CPL_ABORT_ERR_EN_DN_DEV0_MASK        0x10

// Bitfield Description : Set DEVICE_ID of Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_DEVICE_ID_DN_DEV0_OFFSET      5
#define RCC_DEV0_PORT_STRAP0_STRAP_DEVICE_ID_DN_DEV0_MASK        0x1fffe0

// Bitfield Description : Set InterruptPin of Switch Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_INTERRUPT_PIN_DN_DEV0_OFFSET      21
#define RCC_DEV0_PORT_STRAP0_STRAP_INTERRUPT_PIN_DN_DEV0_MASK        0xe00000

// Bitfield Description : Ignore UR caused by E2E Prefix for Downstream Port.
#define RCC_DEV0_PORT_STRAP0_STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV0_OFFSET      24
#define RCC_DEV0_PORT_STRAP0_STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV0_MASK        0x1000000

// Bitfield Description : Set MAX_PAYLOAD_SIZE in Downstream Port function.
#define RCC_DEV0_PORT_STRAP0_STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV0_OFFSET      25
#define RCC_DEV0_PORT_STRAP0_STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV0_MASK        0xe000000

// Bitfield Description : Set MAX_LINK_WIDTH for nBIF Virtual Link 0 -- x1 1-- x2 2 -- x4 3 -- x8 4 -- x12 5 -- x16 6 -- x32 7 -- reserved.
#define RCC_DEV0_PORT_STRAP0_STRAP_MAX_LINK_WIDTH_SUPPORT_DEV0_OFFSET      28
#define RCC_DEV0_PORT_STRAP0_STRAP_MAX_LINK_WIDTH_SUPPORT_DEV0_MASK        0x70000000

// Bitfield Description : Configure EPF0 as Dummy F0.
#define RCC_DEV0_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV0_OFFSET      31
#define RCC_DEV0_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            STRAP_ARI_EN_DN_DEV0:1;
    UINT32                            STRAP_ACS_EN_DN_DEV0:1;
    UINT32                            STRAP_AER_EN_DN_DEV0:1;
    UINT32                            STRAP_CPL_ABORT_ERR_EN_DN_DEV0:1;
    UINT32                            STRAP_DEVICE_ID_DN_DEV0:16;
    UINT32                            STRAP_INTERRUPT_PIN_DN_DEV0:3;
    UINT32                            STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV0:1;
    UINT32                            STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV0:3;
    UINT32                            STRAP_MAX_LINK_WIDTH_SUPPORT_DEV0:3;
    UINT32                            STRAP_EPF0_DUMMY_EN_DEV0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP0_ADDRESS    0x10131000UL

#define SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP0_ADDRESS    0x10131200UL

#define SMN_NBIF0_OFFSETH1C_RCC_DEV0_PORT_STRAP0_ADDRESS    0x349cUL

#define SMN_NBIF0_OFFSETH2349C_RCC_DEV0_PORT_STRAP0_ADDRESS    0x1012349cUL


/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP1
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set SUBSYSTEM_ID of Downstream Port.
#define RCC_DEV0_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV0_OFFSET      0
#define RCC_DEV0_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV0_MASK        0xffff

// Bitfield Description : Set SUBSYSTEM_VENDOR_ID of Downstream Port.
#define RCC_DEV0_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV0_OFFSET      16
#define RCC_DEV0_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV0_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            STRAP_SUBSYS_ID_DN_DEV0:16;
    UINT32                            STRAP_SUBSYS_VEN_ID_DN_DEV0:16;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP1_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP1_ADDRESS    0x10131004UL

#define SMN_NBIF0_OFFSETH20_RCC_DEV0_PORT_STRAP1_ADDRESS    0x34a0UL

#define SMN_NBIF0_OFFSETH234A0_RCC_DEV0_PORT_STRAP1_ADDRESS    0x101234a0UL


/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP2
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set LINK_CNTL2.SELECTABLE_DE_EMPHASIS.
#define RCC_DEV0_PORT_STRAP2_STRAP_DE_EMPHASIS_SEL_DN_DEV0_OFFSET      0
#define RCC_DEV0_PORT_STRAP2_STRAP_DE_EMPHASIS_SEL_DN_DEV0_MASK        0x1

// Bitfield Description : Enable DSN capability in Downstream Port.
#define RCC_DEV0_PORT_STRAP2_STRAP_DSN_EN_DN_DEV0_OFFSET      1
#define RCC_DEV0_PORT_STRAP2_STRAP_DSN_EN_DN_DEV0_MASK        0x2

// Bitfield Description : Enable E2E Prefix support.
#define RCC_DEV0_PORT_STRAP2_STRAP_E2E_PREFIX_EN_DEV0_OFFSET      2
#define RCC_DEV0_PORT_STRAP2_STRAP_E2E_PREFIX_EN_DEV0_MASK        0x4

// Bitfield Description : Enable PCIE ECN1.1 version support.
#define RCC_DEV0_PORT_STRAP2_STRAP_ECN1P1_EN_DEV0_OFFSET      3
#define RCC_DEV0_PORT_STRAP2_STRAP_ECN1P1_EN_DEV0_MASK        0x8

// Bitfield Description : Enable ECRC check.
#define RCC_DEV0_PORT_STRAP2_STRAP_ECRC_CHECK_EN_DEV0_OFFSET      4
#define RCC_DEV0_PORT_STRAP2_STRAP_ECRC_CHECK_EN_DEV0_MASK        0x10

// Bitfield Description : Enable ECRC generation.
#define RCC_DEV0_PORT_STRAP2_STRAP_ECRC_GEN_EN_DEV0_OFFSET      5
#define RCC_DEV0_PORT_STRAP2_STRAP_ECRC_GEN_EN_DEV0_MASK        0x20

// Bitfield Description : Disable AER error reporting.
#define RCC_DEV0_PORT_STRAP2_STRAP_ERR_REPORTING_DIS_DEV0_OFFSET      6
#define RCC_DEV0_PORT_STRAP2_STRAP_ERR_REPORTING_DIS_DEV0_MASK        0x40

// Bitfield Description : Enable fmt[2] support.
#define RCC_DEV0_PORT_STRAP2_STRAP_EXTENDED_FMT_SUPPORTED_DEV0_OFFSET      7
#define RCC_DEV0_PORT_STRAP2_STRAP_EXTENDED_FMT_SUPPORTED_DEV0_MASK        0x80

// Bitfield Description : Enable extended tag.
#define RCC_DEV0_PORT_STRAP2_STRAP_EXTENDED_TAG_ECN_EN_DEV0_OFFSET      8
#define RCC_DEV0_PORT_STRAP2_STRAP_EXTENDED_TAG_ECN_EN_DEV0_MASK        0x100

// Bitfield Description : obsolete                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   , to be deleted.
#define RCC_DEV0_PORT_STRAP2_STRAP_EXT_VC_COUNT_DN_DEV0_OFFSET      9
#define RCC_DEV0_PORT_STRAP2_STRAP_EXT_VC_COUNT_DN_DEV0_MASK        0xe00

// Bitfield Description : Define Header Logging Mode for first error. 0 -- First detected error logging; 1-- first received error logging.
#define RCC_DEV0_PORT_STRAP2_STRAP_FIRST_RCVD_ERR_LOG_DN_DEV0_OFFSET      12
#define RCC_DEV0_PORT_STRAP2_STRAP_FIRST_RCVD_ERR_LOG_DN_DEV0_MASK        0x1000

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in Downstream Port.
#define RCC_DEV0_PORT_STRAP2_STRAP_POISONED_ADVISORY_NONFATAL_DN_DEV0_OFFSET      13
#define RCC_DEV0_PORT_STRAP2_STRAP_POISONED_ADVISORY_NONFATAL_DN_DEV0_MASK        0x2000

// Bitfield Description : Enable PCIE Gen2 config feature.
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN2_COMPLIANCE_DEV0_OFFSET      14
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN2_COMPLIANCE_DEV0_MASK        0x4000

// Bitfield Description : Enable GEN2 support.
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN2_EN_DEV0_OFFSET      15
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN2_EN_DEV0_MASK        0x8000

// Bitfield Description : Enable PCIE Gen3 config feature.
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN3_COMPLIANCE_DEV0_OFFSET      16
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN3_COMPLIANCE_DEV0_MASK        0x10000

// Bitfield Description : Enable PCIE Gen4 config feature.
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN4_COMPLIANCE_DEV0_OFFSET      17
#define RCC_DEV0_PORT_STRAP2_STRAP_GEN4_COMPLIANCE_DEV0_MASK        0x20000

// Bitfield Description :
#define RCC_DEV0_PORT_STRAP2_Reserved_19_18_OFFSET      18
#define RCC_DEV0_PORT_STRAP2_Reserved_19_18_MASK        0xc0000

// Bitfield Description : Set DEVICE_CAP.L0S_ACCEPTABLE_LATENCY.
#define RCC_DEV0_PORT_STRAP2_STRAP_L0S_ACCEPTABLE_LATENCY_DEV0_OFFSET      20
#define RCC_DEV0_PORT_STRAP2_STRAP_L0S_ACCEPTABLE_LATENCY_DEV0_MASK        0x700000

// Bitfield Description : Set DEVICE_CAP.L0S_EXIT_LATENCY.
#define RCC_DEV0_PORT_STRAP2_STRAP_L0S_EXIT_LATENCY_DEV0_OFFSET      23
#define RCC_DEV0_PORT_STRAP2_STRAP_L0S_EXIT_LATENCY_DEV0_MASK        0x3800000

// Bitfield Description : Set DEVICE_CAP.L1_ACCEPTABLE_LATENCY.
#define RCC_DEV0_PORT_STRAP2_STRAP_L1_ACCEPTABLE_LATENCY_DEV0_OFFSET      26
#define RCC_DEV0_PORT_STRAP2_STRAP_L1_ACCEPTABLE_LATENCY_DEV0_MASK        0x1c000000

// Bitfield Description : Set DEVICE_CAP.L1_EXIT_LATENCY.
#define RCC_DEV0_PORT_STRAP2_STRAP_L1_EXIT_LATENCY_DEV0_OFFSET      29
#define RCC_DEV0_PORT_STRAP2_STRAP_L1_EXIT_LATENCY_DEV0_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            STRAP_DE_EMPHASIS_SEL_DN_DEV0:1;
    UINT32                            STRAP_DSN_EN_DN_DEV0:1;
    UINT32                            STRAP_E2E_PREFIX_EN_DEV0:1;
    UINT32                            STRAP_ECN1P1_EN_DEV0:1;
    UINT32                            STRAP_ECRC_CHECK_EN_DEV0:1;
    UINT32                            STRAP_ECRC_GEN_EN_DEV0:1;
    UINT32                            STRAP_ERR_REPORTING_DIS_DEV0:1;
    UINT32                            STRAP_EXTENDED_FMT_SUPPORTED_DEV0:1;
    UINT32                            STRAP_EXTENDED_TAG_ECN_EN_DEV0:1;
    UINT32                            STRAP_EXT_VC_COUNT_DN_DEV0:3;
    UINT32                            STRAP_FIRST_RCVD_ERR_LOG_DN_DEV0:1;
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DN_DEV0:1;
    UINT32                            STRAP_GEN2_COMPLIANCE_DEV0:1;
    UINT32                            STRAP_GEN2_EN_DEV0:1;
    UINT32                            STRAP_GEN3_COMPLIANCE_DEV0:1;
    UINT32                            STRAP_GEN4_COMPLIANCE_DEV0:1;
    UINT32                            Reserved_19_18:2;
    UINT32                            STRAP_L0S_ACCEPTABLE_LATENCY_DEV0:3;
    UINT32                            STRAP_L0S_EXIT_LATENCY_DEV0:3;
    UINT32                            STRAP_L1_ACCEPTABLE_LATENCY_DEV0:3;
    UINT32                            STRAP_L1_EXIT_LATENCY_DEV0:3;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP2_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP2_ADDRESS    0x10131008UL

#define SMN_NBIF0_OFFSETH234B4_RCC_DEV0_PORT_STRAP2_ADDRESS    0x101234b4UL

#define SMN_NBIF0_OFFSETH34_RCC_DEV0_PORT_STRAP2_ADDRESS    0x34b4UL


/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable link bandwidth notification capability in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV0_OFFSET      0
#define RCC_DEV0_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV0_MASK        0x1

// Bitfield Description : Enable LTR support in EP function.
#define RCC_DEV0_PORT_STRAP3_STRAP_LTR_EN_DEV0_OFFSET      1
#define RCC_DEV0_PORT_STRAP3_STRAP_LTR_EN_DEV0_MASK        0x2

// Bitfield Description : Enable LTR support in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_LTR_EN_DN_DEV0_OFFSET      2
#define RCC_DEV0_PORT_STRAP3_STRAP_LTR_EN_DN_DEV0_MASK        0x4

// Bitfield Description : Set MAX_PAYLOAD_SIZE in EP function.
#define RCC_DEV0_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV0_OFFSET      3
#define RCC_DEV0_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV0_MASK        0x38

// Bitfield Description : Enable MSI capability in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_MSI_EN_DN_DEV0_OFFSET      6
#define RCC_DEV0_PORT_STRAP3_STRAP_MSI_EN_DN_DEV0_MASK        0x40

// Bitfield Description : Enable master completion timeout.
#define RCC_DEV0_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV0_OFFSET      7
#define RCC_DEV0_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV0_MASK        0x80

// Bitfield Description : Set PMCSR.NO_SOFT_RESET for downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV0_OFFSET      8
#define RCC_DEV0_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV0_MASK        0x100

// Bitfield Description : Set DEVICE_CAP2.OBFF_SUPPORTED.
#define RCC_DEV0_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV0_OFFSET      9
#define RCC_DEV0_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV0_MASK        0x600

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV0_OFFSET      11
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV0_MASK        0x3800

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_TX_PRESET in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV0_OFFSET      14
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV0_MASK        0x3c000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV0_OFFSET      18
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV0_MASK        0x1c0000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_TX_PRESET in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV0_OFFSET      21
#define RCC_DEV0_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV0_MASK        0x1e00000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in EP function.
#define RCC_DEV0_PORT_STRAP3_STRAP_PM_SUPPORT_DEV0_OFFSET      25
#define RCC_DEV0_PORT_STRAP3_STRAP_PM_SUPPORT_DEV0_MASK        0x6000000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV0_OFFSET      27
#define RCC_DEV0_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV0_MASK        0x18000000

// Bitfield Description : Enable 32bit AtomicOP Completer in downstream port.
#define RCC_DEV0_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV0_OFFSET      29
#define RCC_DEV0_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV0_MASK        0x20000000

// Bitfield Description :
#define RCC_DEV0_PORT_STRAP3_Reserved_30_30_OFFSET      30
#define RCC_DEV0_PORT_STRAP3_Reserved_30_30_MASK        0x40000000

// Bitfield Description : Set PMC.DSI for downstream port function.
#define RCC_DEV0_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV0_OFFSET      31
#define RCC_DEV0_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV0:1;
    UINT32                            STRAP_LTR_EN_DEV0:1;
    UINT32                            STRAP_LTR_EN_DN_DEV0:1;
    UINT32                            STRAP_MAX_PAYLOAD_SUPPORT_DEV0:3;
    UINT32                            STRAP_MSI_EN_DN_DEV0:1;
    UINT32                            STRAP_MSTCPL_TIMEOUT_EN_DEV0:1;
    UINT32                            STRAP_NO_SOFT_RESET_DN_DEV0:1;
    UINT32                            STRAP_OBFF_SUPPORTED_DEV0:2;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV0:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV0:4;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV0:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV0:4;
    UINT32                            STRAP_PM_SUPPORT_DEV0:2;
    UINT32                            STRAP_PM_SUPPORT_DN_DEV0:2;
    UINT32                            STRAP_ATOMIC_EN_DN_DEV0:1;
    UINT32                            Reserved_30_30:1;
    UINT32                            STRAP_PMC_DSI_DN_DEV0:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP3_ADDRESS    0x1013100cUL

#define SMN_NBIF0_OFFSETH234B8_RCC_DEV0_PORT_STRAP3_ADDRESS    0x101234b8UL

#define SMN_NBIF0_OFFSETH38_RCC_DEV0_PORT_STRAP3_ADDRESS    0x34b8UL


/***********************************************************
* Register Name : RCC_DEV0_PORT_STRAP7
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set LINK_CAP.PortNumber for both Downstream Port and EP function.
#define RCC_DEV0_PORT_STRAP7_STRAP_PORT_NUMBER_DEV0_OFFSET      0
#define RCC_DEV0_PORT_STRAP7_STRAP_PORT_NUMBER_DEV0_MASK        0xff

// Bitfield Description : Set MAJOR_REV_ID for both Downstream Port.
#define RCC_DEV0_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV0_OFFSET      8
#define RCC_DEV0_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV0_MASK        0xf00

// Bitfield Description : Set MINOR_REV_ID for both Downstream Port.
#define RCC_DEV0_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV0_OFFSET      12
#define RCC_DEV0_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV0_MASK        0xf000

// Bitfield Description : Set BusNumber for RootPort.
#define RCC_DEV0_PORT_STRAP7_STRAP_RP_BUSNUM_DEV0_OFFSET      16
#define RCC_DEV0_PORT_STRAP7_STRAP_RP_BUSNUM_DEV0_MASK        0xff0000

// Bitfield Description : Set Device Number for Downstream Port.
#define RCC_DEV0_PORT_STRAP7_STRAP_DN_DEVNUM_DEV0_OFFSET      24
#define RCC_DEV0_PORT_STRAP7_STRAP_DN_DEVNUM_DEV0_MASK        0x1f000000

// Bitfield Description : Set Function ID for Downstream Port.
#define RCC_DEV0_PORT_STRAP7_STRAP_DN_FUNCID_DEV0_OFFSET      29
#define RCC_DEV0_PORT_STRAP7_STRAP_DN_FUNCID_DEV0_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            STRAP_PORT_NUMBER_DEV0:8;
    UINT32                            STRAP_MAJOR_REV_ID_DN_DEV0:4;
    UINT32                            STRAP_MINOR_REV_ID_DN_DEV0:4;
    UINT32                            STRAP_RP_BUSNUM_DEV0:8;
    UINT32                            STRAP_DN_DEVNUM_DEV0:5;
    UINT32                            STRAP_DN_FUNCID_DEV0:3;
  } Field;
  UINT32 Value;
} RCC_DEV0_PORT_STRAP7_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP7_ADDRESS    0x1013101cUL

#define SMN_NBIF0_OFFSETH234C8_RCC_DEV0_PORT_STRAP7_ADDRESS    0x101234c8UL

#define SMN_NBIF0_OFFSETH48_RCC_DEV0_PORT_STRAP7_ADDRESS    0x34c8UL


/***********************************************************
* Register Name : RCC_DEV1_EPF0_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV1_EPF0_STRAP0_STRAP_DEVICE_ID_DEV1_F0_OFFSET      0
#define RCC_DEV1_EPF0_STRAP0_STRAP_DEVICE_ID_DEV1_F0_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV1_EPF0_STRAP0_STRAP_MAJOR_REV_ID_DEV1_F0_OFFSET      16
#define RCC_DEV1_EPF0_STRAP0_STRAP_MAJOR_REV_ID_DEV1_F0_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV1_EPF0_STRAP0_STRAP_MINOR_REV_ID_DEV1_F0_OFFSET      20
#define RCC_DEV1_EPF0_STRAP0_STRAP_MINOR_REV_ID_DEV1_F0_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV1_EPF0_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV1_EPF0_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV1_EPF0_STRAP0_STRAP_FUNC_EN_DEV1_F0_OFFSET      28
#define RCC_DEV1_EPF0_STRAP0_STRAP_FUNC_EN_DEV1_F0_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV1_EPF0_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F0_OFFSET      29
#define RCC_DEV1_EPF0_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F0_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV1_EPF0_STRAP0_STRAP_D1_SUPPORT_DEV1_F0_OFFSET      30
#define RCC_DEV1_EPF0_STRAP0_STRAP_D1_SUPPORT_DEV1_F0_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV1_EPF0_STRAP0_STRAP_D2_SUPPORT_DEV1_F0_OFFSET      31
#define RCC_DEV1_EPF0_STRAP0_STRAP_D2_SUPPORT_DEV1_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV1_F0:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV1_F0:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV1_F0:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV1_F0:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F0:1;
    UINT32                            STRAP_D1_SUPPORT_DEV1_F0:1;
    UINT32                            STRAP_D2_SUPPORT_DEV1_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV1_EPF0_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF0_STRAP0_ADDRESS    0x10135000UL

/***********************************************************
* Register Name : RCC_DEV1_EPF0_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV1_F0_OFFSET      0
#define RCC_DEV1_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV1_F0_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_PWR_EN_DEV1_F0_OFFSET      1
#define RCC_DEV1_EPF0_STRAP3_STRAP_PWR_EN_DEV1_F0_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV1_F0_OFFSET      2
#define RCC_DEV1_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV1_F0_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSI_EN_DEV1_F0_OFFSET      18
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSI_EN_DEV1_F0_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV1_F0_OFFSET      19
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV1_F0_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSIX_EN_DEV1_F0_OFFSET      20
#define RCC_DEV1_EPF0_STRAP3_STRAP_MSIX_EN_DEV1_F0_MASK        0x100000

// Bitfield Description :
#define RCC_DEV1_EPF0_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV1_EPF0_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV1_EPF0_STRAP3_STRAP_PMC_DSI_DEV1_F0_OFFSET      24
#define RCC_DEV1_EPF0_STRAP3_STRAP_PMC_DSI_DEV1_F0_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV1_EPF0_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV1_EPF0_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV1_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV1_F0_OFFSET      26
#define RCC_DEV1_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV1_F0_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV1_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV1_F0_OFFSET      27
#define RCC_DEV1_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV1_F0_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV1_EPF0_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV1_EPF0_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV1_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV1_F0_OFFSET      29
#define RCC_DEV1_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV1_F0_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV1_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV1_F0_OFFSET      30
#define RCC_DEV1_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV1_F0_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV1_EPF0_STRAP3_STRAP_RTR_EN_DEV1_F0_OFFSET      31
#define RCC_DEV1_EPF0_STRAP3_STRAP_RTR_EN_DEV1_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV1_F0:1;
    UINT32                            STRAP_PWR_EN_DEV1_F0:1;
    UINT32                            STRAP_SUBSYS_ID_DEV1_F0:16;
    UINT32                            STRAP_MSI_EN_DEV1_F0:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV1_F0:1;
    UINT32                            STRAP_MSIX_EN_DEV1_F0:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV1_F0:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV1_F0:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV1_F0:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV1_F0:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV1_F0:1;
    UINT32                            STRAP_RTR_EN_DEV1_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV1_EPF0_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF0_STRAP3_ADDRESS    0x1013500cUL

/***********************************************************
* Register Name : RCC_DEV1_EPF1_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV1_EPF1_STRAP0_STRAP_DEVICE_ID_DEV1_F1_OFFSET      0
#define RCC_DEV1_EPF1_STRAP0_STRAP_DEVICE_ID_DEV1_F1_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV1_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV1_F1_OFFSET      16
#define RCC_DEV1_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV1_F1_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV1_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV1_F1_OFFSET      20
#define RCC_DEV1_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV1_F1_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV1_EPF1_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV1_EPF1_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV1_EPF1_STRAP0_STRAP_FUNC_EN_DEV1_F1_OFFSET      28
#define RCC_DEV1_EPF1_STRAP0_STRAP_FUNC_EN_DEV1_F1_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV1_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F1_OFFSET      29
#define RCC_DEV1_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F1_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV1_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV1_F1_OFFSET      30
#define RCC_DEV1_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV1_F1_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV1_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV1_F1_OFFSET      31
#define RCC_DEV1_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV1_F1_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV1_F1:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV1_F1:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV1_F1:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV1_F1:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV1_F1:1;
    UINT32                            STRAP_D1_SUPPORT_DEV1_F1:1;
    UINT32                            STRAP_D2_SUPPORT_DEV1_F1:1;
  } Field;
  UINT32 Value;
} RCC_DEV1_EPF1_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_EPF1_STRAP0_ADDRESS    0x10135200UL


/***********************************************************
* Register Name : RCC_DEV1_PORT_STRAP1
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set SUBSYSTEM_ID of Downstream Port.
#define RCC_DEV1_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV1_OFFSET      0
#define RCC_DEV1_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV1_MASK        0xffff

// Bitfield Description : Set SUBSYSTEM_VENDOR_ID of Downstream Port.
#define RCC_DEV1_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV1_OFFSET      16
#define RCC_DEV1_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV1_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            STRAP_SUBSYS_ID_DN_DEV1:16;
    UINT32                            STRAP_SUBSYS_VEN_ID_DN_DEV1:16;
  } Field;
  UINT32 Value;
} RCC_DEV1_PORT_STRAP1_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP1_ADDRESS    0x10131204UL


/***********************************************************
* Register Name : RCC_DEV1_PORT_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable link bandwidth notification capability in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV1_OFFSET      0
#define RCC_DEV1_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV1_MASK        0x1

// Bitfield Description : Enable LTR support in EP function.
#define RCC_DEV1_PORT_STRAP3_STRAP_LTR_EN_DEV1_OFFSET      1
#define RCC_DEV1_PORT_STRAP3_STRAP_LTR_EN_DEV1_MASK        0x2

// Bitfield Description : Enable LTR support in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_LTR_EN_DN_DEV1_OFFSET      2
#define RCC_DEV1_PORT_STRAP3_STRAP_LTR_EN_DN_DEV1_MASK        0x4

// Bitfield Description : Set MAX_PAYLOAD_SIZE in EP function.
#define RCC_DEV1_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV1_OFFSET      3
#define RCC_DEV1_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV1_MASK        0x38

// Bitfield Description : Enable MSI capability in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_MSI_EN_DN_DEV1_OFFSET      6
#define RCC_DEV1_PORT_STRAP3_STRAP_MSI_EN_DN_DEV1_MASK        0x40

// Bitfield Description : Enable master completion timeout.
#define RCC_DEV1_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV1_OFFSET      7
#define RCC_DEV1_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV1_MASK        0x80

// Bitfield Description : Set PMCSR.NO_SOFT_RESET for downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV1_OFFSET      8
#define RCC_DEV1_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV1_MASK        0x100

// Bitfield Description : Set DEVICE_CAP2.OBFF_SUPPORTED.
#define RCC_DEV1_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV1_OFFSET      9
#define RCC_DEV1_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV1_MASK        0x600

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV1_OFFSET      11
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV1_MASK        0x3800

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_TX_PRESET in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV1_OFFSET      14
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV1_MASK        0x3c000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV1_OFFSET      18
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV1_MASK        0x1c0000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_TX_PRESET in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV1_OFFSET      21
#define RCC_DEV1_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV1_MASK        0x1e00000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in EP function.
#define RCC_DEV1_PORT_STRAP3_STRAP_PM_SUPPORT_DEV1_OFFSET      25
#define RCC_DEV1_PORT_STRAP3_STRAP_PM_SUPPORT_DEV1_MASK        0x6000000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV1_OFFSET      27
#define RCC_DEV1_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV1_MASK        0x18000000

// Bitfield Description : Enable 32bit AtomicOP Completer in downstream port.
#define RCC_DEV1_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV1_OFFSET      29
#define RCC_DEV1_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV1_MASK        0x20000000

// Bitfield Description :
#define RCC_DEV1_PORT_STRAP3_Reserved_30_30_OFFSET      30
#define RCC_DEV1_PORT_STRAP3_Reserved_30_30_MASK        0x40000000

// Bitfield Description : Set PMC.DSI for downstream port function.
#define RCC_DEV1_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV1_OFFSET      31
#define RCC_DEV1_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV1_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV1:1;
    UINT32                            STRAP_LTR_EN_DEV1:1;
    UINT32                            STRAP_LTR_EN_DN_DEV1:1;
    UINT32                            STRAP_MAX_PAYLOAD_SUPPORT_DEV1:3;
    UINT32                            STRAP_MSI_EN_DN_DEV1:1;
    UINT32                            STRAP_MSTCPL_TIMEOUT_EN_DEV1:1;
    UINT32                            STRAP_NO_SOFT_RESET_DN_DEV1:1;
    UINT32                            STRAP_OBFF_SUPPORTED_DEV1:2;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV1:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV1:4;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV1:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV1:4;
    UINT32                            STRAP_PM_SUPPORT_DEV1:2;
    UINT32                            STRAP_PM_SUPPORT_DN_DEV1:2;
    UINT32                            STRAP_ATOMIC_EN_DN_DEV1:1;
    UINT32                            Reserved_30_30:1;
    UINT32                            STRAP_PMC_DSI_DN_DEV1:1;
  } Field;
  UINT32 Value;
} RCC_DEV1_PORT_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP3_ADDRESS    0x1013120cUL


/***********************************************************
* Register Name : RCC_DEV1_PORT_STRAP7
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set LINK_CAP.PortNumber for both Downstream Port and EP function.
#define RCC_DEV1_PORT_STRAP7_STRAP_PORT_NUMBER_DEV1_OFFSET      0
#define RCC_DEV1_PORT_STRAP7_STRAP_PORT_NUMBER_DEV1_MASK        0xff

// Bitfield Description : Set MAJOR_REV_ID for both Downstream Port.
#define RCC_DEV1_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV1_OFFSET      8
#define RCC_DEV1_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV1_MASK        0xf00

// Bitfield Description : Set MINOR_REV_ID for both Downstream Port.
#define RCC_DEV1_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV1_OFFSET      12
#define RCC_DEV1_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV1_MASK        0xf000

// Bitfield Description : Set BusNumber for RootPort.
#define RCC_DEV1_PORT_STRAP7_STRAP_RP_BUSNUM_DEV1_OFFSET      16
#define RCC_DEV1_PORT_STRAP7_STRAP_RP_BUSNUM_DEV1_MASK        0xff0000

// Bitfield Description : Set Device Number for Downstream Port.
#define RCC_DEV1_PORT_STRAP7_STRAP_DN_DEVNUM_DEV1_OFFSET      24
#define RCC_DEV1_PORT_STRAP7_STRAP_DN_DEVNUM_DEV1_MASK        0x1f000000

// Bitfield Description : Set Function ID for Downstream Port.
#define RCC_DEV1_PORT_STRAP7_STRAP_DN_FUNCID_DEV1_OFFSET      29
#define RCC_DEV1_PORT_STRAP7_STRAP_DN_FUNCID_DEV1_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            STRAP_PORT_NUMBER_DEV1:8;
    UINT32                            STRAP_MAJOR_REV_ID_DN_DEV1:4;
    UINT32                            STRAP_MINOR_REV_ID_DN_DEV1:4;
    UINT32                            STRAP_RP_BUSNUM_DEV1:8;
    UINT32                            STRAP_DN_DEVNUM_DEV1:5;
    UINT32                            STRAP_DN_FUNCID_DEV1:3;
  } Field;
  UINT32 Value;
} RCC_DEV1_PORT_STRAP7_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV1_PORT_STRAP7_ADDRESS    0x1013121cUL

/***********************************************************
* Register Name : RCC_DEV2_EPF0_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F0_OFFSET      0
#define RCC_DEV2_EPF0_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F0_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_PWR_EN_DEV2_F0_OFFSET      1
#define RCC_DEV2_EPF0_STRAP3_STRAP_PWR_EN_DEV2_F0_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV2_F0_OFFSET      2
#define RCC_DEV2_EPF0_STRAP3_STRAP_SUBSYS_ID_DEV2_F0_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSI_EN_DEV2_F0_OFFSET      18
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSI_EN_DEV2_F0_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F0_OFFSET      19
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F0_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSIX_EN_DEV2_F0_OFFSET      20
#define RCC_DEV2_EPF0_STRAP3_STRAP_MSIX_EN_DEV2_F0_MASK        0x100000

// Bitfield Description :
#define RCC_DEV2_EPF0_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV2_EPF0_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV2_EPF0_STRAP3_STRAP_PMC_DSI_DEV2_F0_OFFSET      24
#define RCC_DEV2_EPF0_STRAP3_STRAP_PMC_DSI_DEV2_F0_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV2_EPF0_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV2_EPF0_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV2_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F0_OFFSET      26
#define RCC_DEV2_EPF0_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F0_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV2_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F0_OFFSET      27
#define RCC_DEV2_EPF0_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F0_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_EPF0_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV2_EPF0_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV2_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV2_F0_OFFSET      29
#define RCC_DEV2_EPF0_STRAP3_STRAP_CLK_PM_EN_DEV2_F0_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV2_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F0_OFFSET      30
#define RCC_DEV2_EPF0_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F0_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV2_EPF0_STRAP3_STRAP_RTR_EN_DEV2_F0_OFFSET      31
#define RCC_DEV2_EPF0_STRAP3_STRAP_RTR_EN_DEV2_F0_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F0:1;
    UINT32                            STRAP_PWR_EN_DEV2_F0:1;
    UINT32                            STRAP_SUBSYS_ID_DEV2_F0:16;
    UINT32                            STRAP_MSI_EN_DEV2_F0:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV2_F0:1;
    UINT32                            STRAP_MSIX_EN_DEV2_F0:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV2_F0:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F0:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F0:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV2_F0:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV2_F0:1;
    UINT32                            STRAP_RTR_EN_DEV2_F0:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF0_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF0_STRAP3_ADDRESS    0x1013600cUL

/***********************************************************
* Register Name : RCC_DEV2_EPF3_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F3_OFFSET      0
#define RCC_DEV2_EPF3_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F3_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_PWR_EN_DEV2_F3_OFFSET      1
#define RCC_DEV2_EPF3_STRAP3_STRAP_PWR_EN_DEV2_F3_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_SUBSYS_ID_DEV2_F3_OFFSET      2
#define RCC_DEV2_EPF3_STRAP3_STRAP_SUBSYS_ID_DEV2_F3_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSI_EN_DEV2_F3_OFFSET      18
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSI_EN_DEV2_F3_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F3_OFFSET      19
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F3_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSIX_EN_DEV2_F3_OFFSET      20
#define RCC_DEV2_EPF3_STRAP3_STRAP_MSIX_EN_DEV2_F3_MASK        0x100000

// Bitfield Description :
#define RCC_DEV2_EPF3_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV2_EPF3_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV2_EPF3_STRAP3_STRAP_PMC_DSI_DEV2_F3_OFFSET      24
#define RCC_DEV2_EPF3_STRAP3_STRAP_PMC_DSI_DEV2_F3_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV2_EPF3_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV2_EPF3_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV2_EPF3_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F3_OFFSET      26
#define RCC_DEV2_EPF3_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F3_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV2_EPF3_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F3_OFFSET      27
#define RCC_DEV2_EPF3_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F3_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_EPF3_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV2_EPF3_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV2_EPF3_STRAP3_STRAP_CLK_PM_EN_DEV2_F3_OFFSET      29
#define RCC_DEV2_EPF3_STRAP3_STRAP_CLK_PM_EN_DEV2_F3_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV2_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F3_OFFSET      30
#define RCC_DEV2_EPF3_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F3_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV2_EPF3_STRAP3_STRAP_RTR_EN_DEV2_F3_OFFSET      31
#define RCC_DEV2_EPF3_STRAP3_STRAP_RTR_EN_DEV2_F3_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F3:1;
    UINT32                            STRAP_PWR_EN_DEV2_F3:1;
    UINT32                            STRAP_SUBSYS_ID_DEV2_F3:16;
    UINT32                            STRAP_MSI_EN_DEV2_F3:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV2_F3:1;
    UINT32                            STRAP_MSIX_EN_DEV2_F3:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV2_F3:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F3:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F3:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV2_F3:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV2_F3:1;
    UINT32                            STRAP_RTR_EN_DEV2_F3:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF3_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF3_STRAP3_ADDRESS    0x1013660cUL

/***********************************************************
* Register Name : RCC_DEV2_EPF4_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F4_OFFSET      0
#define RCC_DEV2_EPF4_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F4_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_PWR_EN_DEV2_F4_OFFSET      1
#define RCC_DEV2_EPF4_STRAP3_STRAP_PWR_EN_DEV2_F4_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_SUBSYS_ID_DEV2_F4_OFFSET      2
#define RCC_DEV2_EPF4_STRAP3_STRAP_SUBSYS_ID_DEV2_F4_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSI_EN_DEV2_F4_OFFSET      18
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSI_EN_DEV2_F4_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F4_OFFSET      19
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F4_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSIX_EN_DEV2_F4_OFFSET      20
#define RCC_DEV2_EPF4_STRAP3_STRAP_MSIX_EN_DEV2_F4_MASK        0x100000

// Bitfield Description :
#define RCC_DEV2_EPF4_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV2_EPF4_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV2_EPF4_STRAP3_STRAP_PMC_DSI_DEV2_F4_OFFSET      24
#define RCC_DEV2_EPF4_STRAP3_STRAP_PMC_DSI_DEV2_F4_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV2_EPF4_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV2_EPF4_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV2_EPF4_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F4_OFFSET      26
#define RCC_DEV2_EPF4_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F4_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV2_EPF4_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F4_OFFSET      27
#define RCC_DEV2_EPF4_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F4_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_EPF4_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV2_EPF4_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV2_EPF4_STRAP3_STRAP_CLK_PM_EN_DEV2_F4_OFFSET      29
#define RCC_DEV2_EPF4_STRAP3_STRAP_CLK_PM_EN_DEV2_F4_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV2_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F4_OFFSET      30
#define RCC_DEV2_EPF4_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F4_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV2_EPF4_STRAP3_STRAP_RTR_EN_DEV2_F4_OFFSET      31
#define RCC_DEV2_EPF4_STRAP3_STRAP_RTR_EN_DEV2_F4_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F4:1;
    UINT32                            STRAP_PWR_EN_DEV2_F4:1;
    UINT32                            STRAP_SUBSYS_ID_DEV2_F4:16;
    UINT32                            STRAP_MSI_EN_DEV2_F4:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV2_F4:1;
    UINT32                            STRAP_MSIX_EN_DEV2_F4:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV2_F4:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F4:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F4:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV2_F4:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV2_F4:1;
    UINT32                            STRAP_RTR_EN_DEV2_F4:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF4_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF4_STRAP3_ADDRESS    0x1013680cUL

/***********************************************************
* Register Name : RCC_DEV2_EPF1_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV2_EPF1_STRAP0_STRAP_DEVICE_ID_DEV2_F1_OFFSET      0
#define RCC_DEV2_EPF1_STRAP0_STRAP_DEVICE_ID_DEV2_F1_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV2_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F1_OFFSET      16
#define RCC_DEV2_EPF1_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F1_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV2_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV2_F1_OFFSET      20
#define RCC_DEV2_EPF1_STRAP0_STRAP_MINOR_REV_ID_DEV2_F1_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV2_EPF1_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV2_EPF1_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV2_EPF1_STRAP0_STRAP_FUNC_EN_DEV2_F1_OFFSET      28
#define RCC_DEV2_EPF1_STRAP0_STRAP_FUNC_EN_DEV2_F1_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV2_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F1_OFFSET      29
#define RCC_DEV2_EPF1_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F1_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV2_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV2_F1_OFFSET      30
#define RCC_DEV2_EPF1_STRAP0_STRAP_D1_SUPPORT_DEV2_F1_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV2_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV2_F1_OFFSET      31
#define RCC_DEV2_EPF1_STRAP0_STRAP_D2_SUPPORT_DEV2_F1_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV2_F1:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV2_F1:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV2_F1:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV2_F1:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F1:1;
    UINT32                            STRAP_D1_SUPPORT_DEV2_F1:1;
    UINT32                            STRAP_D2_SUPPORT_DEV2_F1:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF1_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF1_STRAP0_ADDRESS    0x10136200UL


/***********************************************************
* Register Name : RCC_DEV2_EPF2_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV2_EPF2_STRAP0_STRAP_DEVICE_ID_DEV2_F2_OFFSET      0
#define RCC_DEV2_EPF2_STRAP0_STRAP_DEVICE_ID_DEV2_F2_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV2_EPF2_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F2_OFFSET      16
#define RCC_DEV2_EPF2_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F2_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV2_EPF2_STRAP0_STRAP_MINOR_REV_ID_DEV2_F2_OFFSET      20
#define RCC_DEV2_EPF2_STRAP0_STRAP_MINOR_REV_ID_DEV2_F2_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV2_EPF2_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV2_EPF2_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV2_EPF2_STRAP0_STRAP_FUNC_EN_DEV2_F2_OFFSET      28
#define RCC_DEV2_EPF2_STRAP0_STRAP_FUNC_EN_DEV2_F2_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV2_EPF2_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F2_OFFSET      29
#define RCC_DEV2_EPF2_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F2_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV2_EPF2_STRAP0_STRAP_D1_SUPPORT_DEV2_F2_OFFSET      30
#define RCC_DEV2_EPF2_STRAP0_STRAP_D1_SUPPORT_DEV2_F2_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV2_EPF2_STRAP0_STRAP_D2_SUPPORT_DEV2_F2_OFFSET      31
#define RCC_DEV2_EPF2_STRAP0_STRAP_D2_SUPPORT_DEV2_F2_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV2_F2:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV2_F2:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV2_F2:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV2_F2:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F2:1;
    UINT32                            STRAP_D1_SUPPORT_DEV2_F2:1;
    UINT32                            STRAP_D2_SUPPORT_DEV2_F2:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF2_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF2_STRAP0_ADDRESS    0x10136400UL


/***********************************************************
* Register Name : RCC_DEV2_PORT_STRAP1
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set SUBSYSTEM_ID of Downstream Port.
#define RCC_DEV2_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV2_OFFSET      0
#define RCC_DEV2_PORT_STRAP1_STRAP_SUBSYS_ID_DN_DEV2_MASK        0xffff

// Bitfield Description : Set SUBSYSTEM_VENDOR_ID of Downstream Port.
#define RCC_DEV2_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV2_OFFSET      16
#define RCC_DEV2_PORT_STRAP1_STRAP_SUBSYS_VEN_ID_DN_DEV2_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            STRAP_SUBSYS_ID_DN_DEV2:16;
    UINT32                            STRAP_SUBSYS_VEN_ID_DN_DEV2:16;
  } Field;
  UINT32 Value;
} RCC_DEV2_PORT_STRAP1_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP1_ADDRESS    0x10131404UL


/***********************************************************
* Register Name : RCC_DEV2_PORT_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable link bandwidth notification capability in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV2_OFFSET      0
#define RCC_DEV2_PORT_STRAP3_STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV2_MASK        0x1

// Bitfield Description : Enable LTR support in EP function.
#define RCC_DEV2_PORT_STRAP3_STRAP_LTR_EN_DEV2_OFFSET      1
#define RCC_DEV2_PORT_STRAP3_STRAP_LTR_EN_DEV2_MASK        0x2

// Bitfield Description : Enable LTR support in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_LTR_EN_DN_DEV2_OFFSET      2
#define RCC_DEV2_PORT_STRAP3_STRAP_LTR_EN_DN_DEV2_MASK        0x4

// Bitfield Description : Set MAX_PAYLOAD_SIZE in EP function.
#define RCC_DEV2_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV2_OFFSET      3
#define RCC_DEV2_PORT_STRAP3_STRAP_MAX_PAYLOAD_SUPPORT_DEV2_MASK        0x38

// Bitfield Description : Enable MSI capability in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_MSI_EN_DN_DEV2_OFFSET      6
#define RCC_DEV2_PORT_STRAP3_STRAP_MSI_EN_DN_DEV2_MASK        0x40

// Bitfield Description : Enable master completion timeout.
#define RCC_DEV2_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV2_OFFSET      7
#define RCC_DEV2_PORT_STRAP3_STRAP_MSTCPL_TIMEOUT_EN_DEV2_MASK        0x80

// Bitfield Description : Set PMCSR.NO_SOFT_RESET for downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV2_OFFSET      8
#define RCC_DEV2_PORT_STRAP3_STRAP_NO_SOFT_RESET_DN_DEV2_MASK        0x100

// Bitfield Description : Set DEVICE_CAP2.OBFF_SUPPORTED.
#define RCC_DEV2_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV2_OFFSET      9
#define RCC_DEV2_PORT_STRAP3_STRAP_OBFF_SUPPORTED_DEV2_MASK        0x600

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV2_OFFSET      11
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV2_MASK        0x3800

// Bitfield Description : Set LANE_EQ_CNTL.DOWNSTREAM_TX_PRESET in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV2_OFFSET      14
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV2_MASK        0x3c000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_RX_PRESET_HINT in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV2_OFFSET      18
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV2_MASK        0x1c0000

// Bitfield Description : Set LANE_EQ_CNTL.UPSTREAM_TX_PRESET in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV2_OFFSET      21
#define RCC_DEV2_PORT_STRAP3_STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV2_MASK        0x1e00000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in EP function.
#define RCC_DEV2_PORT_STRAP3_STRAP_PM_SUPPORT_DEV2_OFFSET      25
#define RCC_DEV2_PORT_STRAP3_STRAP_PM_SUPPORT_DEV2_MASK        0x6000000

// Bitfield Description : Set LINK_CAP.ASPM_SUPPORT in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV2_OFFSET      27
#define RCC_DEV2_PORT_STRAP3_STRAP_PM_SUPPORT_DN_DEV2_MASK        0x18000000

// Bitfield Description : Enable 32bit AtomicOP Completer in downstream port.
#define RCC_DEV2_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV2_OFFSET      29
#define RCC_DEV2_PORT_STRAP3_STRAP_ATOMIC_EN_DN_DEV2_MASK        0x20000000

// Bitfield Description :
#define RCC_DEV2_PORT_STRAP3_Reserved_30_30_OFFSET      30
#define RCC_DEV2_PORT_STRAP3_Reserved_30_30_MASK        0x40000000

// Bitfield Description : Set PMC.DSI for downstream port function.
#define RCC_DEV2_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV2_OFFSET      31
#define RCC_DEV2_PORT_STRAP3_STRAP_PMC_DSI_DN_DEV2_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_LINK_BW_NOTIFICATION_CAP_DN_EN_DEV2:1;
    UINT32                            STRAP_LTR_EN_DEV2:1;
    UINT32                            STRAP_LTR_EN_DN_DEV2:1;
    UINT32                            STRAP_MAX_PAYLOAD_SUPPORT_DEV2:3;
    UINT32                            STRAP_MSI_EN_DN_DEV2:1;
    UINT32                            STRAP_MSTCPL_TIMEOUT_EN_DEV2:1;
    UINT32                            STRAP_NO_SOFT_RESET_DN_DEV2:1;
    UINT32                            STRAP_OBFF_SUPPORTED_DEV2:2;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_RX_PRESET_HINT_DEV2:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_TX_PRESET_DEV2:4;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_RX_PRESET_HINT_DEV2:3;
    UINT32                            STRAP_PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_TX_PRESET_DEV2:4;
    UINT32                            STRAP_PM_SUPPORT_DEV2:2;
    UINT32                            STRAP_PM_SUPPORT_DN_DEV2:2;
    UINT32                            STRAP_ATOMIC_EN_DN_DEV2:1;
    UINT32                            Reserved_30_30:1;
    UINT32                            STRAP_PMC_DSI_DN_DEV2:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_PORT_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP3_ADDRESS    0x1013140cUL


/***********************************************************
* Register Name : RCC_DEV2_PORT_STRAP7
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set LINK_CAP.PortNumber for both Downstream Port and EP function.
#define RCC_DEV2_PORT_STRAP7_STRAP_PORT_NUMBER_DEV2_OFFSET      0
#define RCC_DEV2_PORT_STRAP7_STRAP_PORT_NUMBER_DEV2_MASK        0xff

// Bitfield Description : Set MAJOR_REV_ID for both Downstream Port.
#define RCC_DEV2_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV2_OFFSET      8
#define RCC_DEV2_PORT_STRAP7_STRAP_MAJOR_REV_ID_DN_DEV2_MASK        0xf00

// Bitfield Description : Set MINOR_REV_ID for both Downstream Port.
#define RCC_DEV2_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV2_OFFSET      12
#define RCC_DEV2_PORT_STRAP7_STRAP_MINOR_REV_ID_DN_DEV2_MASK        0xf000

// Bitfield Description : Set BusNumber for RootPort.
#define RCC_DEV2_PORT_STRAP7_STRAP_RP_BUSNUM_DEV2_OFFSET      16
#define RCC_DEV2_PORT_STRAP7_STRAP_RP_BUSNUM_DEV2_MASK        0xff0000

// Bitfield Description : Set Device Number for Downstream Port.
#define RCC_DEV2_PORT_STRAP7_STRAP_DN_DEVNUM_DEV2_OFFSET      24
#define RCC_DEV2_PORT_STRAP7_STRAP_DN_DEVNUM_DEV2_MASK        0x1f000000

// Bitfield Description : Set Function ID for Downstream Port.
#define RCC_DEV2_PORT_STRAP7_STRAP_DN_FUNCID_DEV2_OFFSET      29
#define RCC_DEV2_PORT_STRAP7_STRAP_DN_FUNCID_DEV2_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            STRAP_PORT_NUMBER_DEV2:8;
    UINT32                            STRAP_MAJOR_REV_ID_DN_DEV2:4;
    UINT32                            STRAP_MINOR_REV_ID_DN_DEV2:4;
    UINT32                            STRAP_RP_BUSNUM_DEV2:8;
    UINT32                            STRAP_DN_DEVNUM_DEV2:5;
    UINT32                            STRAP_DN_FUNCID_DEV2:3;
  } Field;
  UINT32 Value;
} RCC_DEV2_PORT_STRAP7_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP7_ADDRESS    0x1013141cUL


//NBIFRCCFG.h
//PCIECORE.h

/***********************************************************
* Register Name : PCIE_TX_POWER_CTRL_1
* Register Description :
* TX Power Control 1 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls Light Sleep Power Saving mode for the Master Request and Completion static memories.
#define PCIE_TX_POWER_CTRL_1_MST_MEM_LS_EN_OFFSET      0
#define PCIE_TX_POWER_CTRL_1_MST_MEM_LS_EN_MASK        0x1

// Bitfield Description : Controls Deep Sleep Power Saving mode for the Master Request and Completion static memories.
#define PCIE_TX_POWER_CTRL_1_MST_MEM_DS_EN_OFFSET      1
#define PCIE_TX_POWER_CTRL_1_MST_MEM_DS_EN_MASK        0x2

// Bitfield Description : Controls Shut Down Power Saving mode for the Master Request and Completion static memories.
#define PCIE_TX_POWER_CTRL_1_MST_MEM_SD_EN_OFFSET      2
#define PCIE_TX_POWER_CTRL_1_MST_MEM_SD_EN_MASK        0x4

// Bitfield Description : Controls Light Sleep Power Saving mode for the Replay static memories.
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_LS_EN_OFFSET      3
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_LS_EN_MASK        0x8

// Bitfield Description : Controls Deep Sleep Power Saving mode for the Replay static memories.
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_DS_EN_OFFSET      4
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_DS_EN_MASK        0x10

// Bitfield Description : Controls Shut Down Power Saving mode for the Replay static memories.
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_SD_EN_OFFSET      5
#define PCIE_TX_POWER_CTRL_1_REPLAY_MEM_SD_EN_MASK        0x20

// Bitfield Description :
#define PCIE_TX_POWER_CTRL_1_Reserved_31_6_OFFSET      6
#define PCIE_TX_POWER_CTRL_1_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            MST_MEM_LS_EN:1;
    UINT32                            MST_MEM_DS_EN:1;
    UINT32                            MST_MEM_SD_EN:1;
    UINT32                            REPLAY_MEM_LS_EN:1;
    UINT32                            REPLAY_MEM_DS_EN:1;
    UINT32                            REPLAY_MEM_SD_EN:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} PCIE_TX_POWER_CTRL_1_STRUCT;

#define SMN_PCIE0_PCIE_TX_POWER_CTRL_1_ADDRESS    0x1118061cUL

#define SMN_PCIE1_PCIE_TX_POWER_CTRL_1_ADDRESS    0x1128061cUL

#define SMN_PCIE2_PCIE_TX_POWER_CTRL_1_ADDRESS    0x1138061cUL

#define SMN_PCIE3_PCIE_TX_POWER_CTRL_1_ADDRESS    0x1148061cUL

/***********************************************************
* Register Name : CPM_CONTROL
* Register Description :
* Clock Power Management Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls LCLK_DYN gating.
#define CPM_CONTROL_LCLK_DYN_GATE_ENABLE_OFFSET      0
#define CPM_CONTROL_LCLK_DYN_GATE_ENABLE_MASK        0x1

// Bitfield Description : Controls TXCLK_DYN gating.
#define CPM_CONTROL_TXCLK_DYN_GATE_ENABLE_OFFSET      1
#define CPM_CONTROL_TXCLK_DYN_GATE_ENABLE_MASK        0x2

// Bitfield Description : Controls L1 gating.
#define CPM_CONTROL_L1_PWR_GATE_ENABLE_OFFSET      2
#define CPM_CONTROL_L1_PWR_GATE_ENABLE_MASK        0x4

// Bitfield Description : Controls L1.1 gating.
#define CPM_CONTROL_L1_1_PWR_GATE_ENABLE_OFFSET      3
#define CPM_CONTROL_L1_1_PWR_GATE_ENABLE_MASK        0x8

// Bitfield Description : Controls L1.2 gating.
#define CPM_CONTROL_L1_2_PWR_GATE_ENABLE_OFFSET      4
#define CPM_CONTROL_L1_2_PWR_GATE_ENABLE_MASK        0x10

// Bitfield Description : Controls TXCLK_LCNT gating.
#define CPM_CONTROL_TXCLK_LCNT_GATE_ENABLE_OFFSET      5
#define CPM_CONTROL_TXCLK_LCNT_GATE_ENABLE_MASK        0x20

// Bitfield Description : Controls TXCLK_REGS gating.
#define CPM_CONTROL_TXCLK_REGS_GATE_ENABLE_OFFSET      6
#define CPM_CONTROL_TXCLK_REGS_GATE_ENABLE_MASK        0x40

// Bitfield Description : Controls TXCLK_PRBS gating.
#define CPM_CONTROL_TXCLK_PRBS_GATE_ENABLE_OFFSET      7
#define CPM_CONTROL_TXCLK_PRBS_GATE_ENABLE_MASK        0x80

// Bitfield Description : Controls REFCLK_REGS gating.
#define CPM_CONTROL_REFCLK_REGS_GATE_ENABLE_OFFSET      8
#define CPM_CONTROL_REFCLK_REGS_GATE_ENABLE_MASK        0x100

// Bitfield Description : Controls the number of clocks to wait before turning off LCLK_DYN.
#define CPM_CONTROL_LCLK_DYN_GATE_LATENCY_OFFSET      9
#define CPM_CONTROL_LCLK_DYN_GATE_LATENCY_MASK        0x600

// Bitfield Description : Controls the number of clocks to wait before turning off TXCLK_DYN.
#define CPM_CONTROL_TXCLK_DYN_GATE_LATENCY_OFFSET      11
#define CPM_CONTROL_TXCLK_DYN_GATE_LATENCY_MASK        0x1800

// Bitfield Description : Controls refClkReq to refClkAck loopback.
#define CPM_CONTROL_REFCLKREQ_REFCLKACK_LOOPBACK_ENABLE_OFFSET      13
#define CPM_CONTROL_REFCLKREQ_REFCLKACK_LOOPBACK_ENABLE_MASK        0x2000

// Bitfield Description : Controls the number of clocks to wait before turning off TXCLK_REGS.
#define CPM_CONTROL_TXCLK_REGS_GATE_LATENCY_OFFSET      14
#define CPM_CONTROL_TXCLK_REGS_GATE_LATENCY_MASK        0x4000

// Bitfield Description : Controls the number of clocks to wait before turning off REFCLK_REGS.
#define CPM_CONTROL_REFCLK_REGS_GATE_LATENCY_OFFSET      15
#define CPM_CONTROL_REFCLK_REGS_GATE_LATENCY_MASK        0x8000

// Bitfield Description : Controls if LCLK gating is independent of TXCLK gating conditions.
#define CPM_CONTROL_LCLK_GATE_TXCLK_FREE_OFFSET      16
#define CPM_CONTROL_LCLK_GATE_TXCLK_FREE_MASK        0x10000

// Bitfield Description : Controls TXCLK_PERM un-gating for Enhanced Hot-Plug request, for receiver detection clock.
#define CPM_CONTROL_RCVR_DET_CLK_ENABLE_OFFSET      17
#define CPM_CONTROL_RCVR_DET_CLK_ENABLE_MASK        0x20000

// Bitfield Description : Controls the number of clocks to wait for clock switch, latency compensation for repeater stages delaying data.
#define CPM_CONTROL_FAST_TXCLK_LATENCY_OFFSET      18
#define CPM_CONTROL_FAST_TXCLK_LATENCY_MASK        0x1c0000

// Bitfield Description : Controls if register Idle condition is used for Power Gating.
#define CPM_CONTROL_IGNORE_REGS_IDLE_IN_PG_OFFSET      21
#define CPM_CONTROL_IGNORE_REGS_IDLE_IN_PG_MASK        0x200000

// Bitfield Description : Controls if stopping REFCLK and switching to XSTCLK is allowed.
#define CPM_CONTROL_REFCLK_XSTCLK_ENABLE_OFFSET      22
#define CPM_CONTROL_REFCLK_XSTCLK_ENABLE_MASK        0x400000

// Bitfield Description : Controls the number of clocks to wait before switching from REFCLK to XSTCLK.
#define CPM_CONTROL_REFCLK_XSTCLK_LATENCY_OFFSET      23
#define CPM_CONTROL_REFCLK_XSTCLK_LATENCY_MASK        0x800000

// Bitfield Description : Controls if TXCLK un-gating by CLKREQb assertion is allowed.
#define CPM_CONTROL_CLKREQb_UNGATE_TXCLK_ENABLE_OFFSET      24
#define CPM_CONTROL_CLKREQb_UNGATE_TXCLK_ENABLE_MASK        0x1000000

// Bitfield Description : Controls if LCLK gating is only allowed in L1.
#define CPM_CONTROL_LCLK_GATE_ALLOW_IN_L1_OFFSET      25
#define CPM_CONTROL_LCLK_GATE_ALLOW_IN_L1_MASK        0x2000000

// Bitfield Description : Controls if Power Gating early wake up from slave by means of either sampling the de-assertion of Electrical Idle or the assertion of CLKREQb.
#define CPM_CONTROL_PG_EARLY_WAKE_ENABLE_OFFSET      26
#define CPM_CONTROL_PG_EARLY_WAKE_ENABLE_MASK        0x4000000

// Bitfield Description : Indicates the PCIe core Idle status for LIVmin mode.
#define CPM_CONTROL_PCIE_CORE_IDLE_OFFSET      27
#define CPM_CONTROL_PCIE_CORE_IDLE_MASK        0x8000000

// Bitfield Description : Indicates if all the Ports are in L1, L1.1, L1.2, or hold_training mode.
#define CPM_CONTROL_PCIE_LINK_IDLE_OFFSET      28
#define CPM_CONTROL_PCIE_LINK_IDLE_MASK        0x10000000

// Bitfield Description : Indicates if all the PCIE memory buffers are empty.
#define CPM_CONTROL_PCIE_BUFFER_EMPTY_OFFSET      29
#define CPM_CONTROL_PCIE_BUFFER_EMPTY_MASK        0x20000000

// Bitfield Description : Controls the number of clocks to wait after register idle before entering Power Gating.
#define CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_OFFSET      30
#define CPM_CONTROL_REGS_IDLE_TO_PG_LATENCY_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LCLK_DYN_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_GATE_ENABLE:1;
    UINT32                            L1_PWR_GATE_ENABLE:1;
    UINT32                            L1_1_PWR_GATE_ENABLE:1;
    UINT32                            L1_2_PWR_GATE_ENABLE:1;
    UINT32                            TXCLK_LCNT_GATE_ENABLE:1;
    UINT32                            TXCLK_REGS_GATE_ENABLE:1;
    UINT32                            TXCLK_PRBS_GATE_ENABLE:1;
    UINT32                            REFCLK_REGS_GATE_ENABLE:1;
    UINT32                            LCLK_DYN_GATE_LATENCY:2;
    UINT32                            TXCLK_DYN_GATE_LATENCY:2;
    UINT32                            REFCLKREQ_REFCLKACK_LOOPBACK_ENABLE:1;
    UINT32                            TXCLK_REGS_GATE_LATENCY:1;
    UINT32                            REFCLK_REGS_GATE_LATENCY:1;
    UINT32                            LCLK_GATE_TXCLK_FREE:1;
    UINT32                            RCVR_DET_CLK_ENABLE:1;
    UINT32                            FAST_TXCLK_LATENCY:3;
    UINT32                            IGNORE_REGS_IDLE_IN_PG:1;
    UINT32                            REFCLK_XSTCLK_ENABLE:1;
    UINT32                            REFCLK_XSTCLK_LATENCY:1;
    UINT32                            CLKREQb_UNGATE_TXCLK_ENABLE:1;
    UINT32                            LCLK_GATE_ALLOW_IN_L1:1;
    UINT32                            PG_EARLY_WAKE_ENABLE:1;
    UINT32                            PCIE_CORE_IDLE:1;
    UINT32                            PCIE_LINK_IDLE:1;
    UINT32                            PCIE_BUFFER_EMPTY:1;
    UINT32                            REGS_IDLE_TO_PG_LATENCY:2;
  } Field;
  UINT32 Value;
} CPM_CONTROL_STRUCT;

#define SMN_PCIE0_CPM_CONTROL_ADDRESS    0x11180460UL

#define SMN_PCIE1_CPM_CONTROL_ADDRESS    0x11280460UL

#define SMN_PCIE2_CPM_CONTROL_ADDRESS    0x11380460UL

#define SMN_PCIE3_CPM_CONTROL_ADDRESS    0x11480460UL

/***********************************************************
* Register Name : CPM_CONTROL_EXT
* Register Description :
* Clock Power Management Control extended register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls POWERDOWN and ElectIdle masking by lane disable.
#define CPM_CONTROL_EXT_PWRDOWN_EI_MASK_DISABLE_OFFSET      0
#define CPM_CONTROL_EXT_PWRDOWN_EI_MASK_DISABLE_MASK        0x1

// Bitfield Description : Controls the delay of HoldTraining per port by binding it to refClkReq and refClkAck.
#define CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_OFFSET      1
#define CPM_CONTROL_EXT_DELAY_HOLD_TRAINING_ENABLE_MASK        0x2

// Bitfield Description : Controls LCLK DeepSleep mode.
#define CPM_CONTROL_EXT_LCLK_DS_MODE_OFFSET      2
#define CPM_CONTROL_EXT_LCLK_DS_MODE_MASK        0x4

// Bitfield Description : Controls LCLK DeepSleep.
#define CPM_CONTROL_EXT_LCLK_DS_ENABLE_OFFSET      3
#define CPM_CONTROL_EXT_LCLK_DS_ENABLE_MASK        0x8

// Bitfield Description : Indicates the PG State of the Power Gating Controller.
#define CPM_CONTROL_EXT_PG_STATE_OFFSET      4
#define CPM_CONTROL_EXT_PG_STATE_MASK        0x70

// Bitfield Description : Allows LCLK gating when hotplug is enabled
#define CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_OFFSET      7
#define CPM_CONTROL_EXT_HOTPLUG_ALLOW_LCLK_GATING_EN_MASK        0x80

// Bitfield Description : Control CIO_PCIE_TPI_SLV_SSCLK gating
#define CPM_CONTROL_EXT_CIO_PCIE_TPI_SLV_SSCLK_GATING_ENABLE_OFFSET      8
#define CPM_CONTROL_EXT_CIO_PCIE_TPI_SLV_SSCLK_GATING_ENABLE_MASK        0x100

// Bitfield Description : Control PCIE_CIO_TPI_MST_SSCLK gating
#define CPM_CONTROL_EXT_PCIE_CIO_TPI_MST_SSCLK_GATING_ENABLE_OFFSET      9
#define CPM_CONTROL_EXT_PCIE_CIO_TPI_MST_SSCLK_GATING_ENABLE_MASK        0x200

// Bitfield Description :
#define CPM_CONTROL_EXT_Reserved_31_10_OFFSET      10
#define CPM_CONTROL_EXT_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            PWRDOWN_EI_MASK_DISABLE:1;
    UINT32                            DELAY_HOLD_TRAINING_ENABLE:1;
    UINT32                            LCLK_DS_MODE:1;
    UINT32                            LCLK_DS_ENABLE:1;
    UINT32                            PG_STATE:3;
    UINT32                            HOTPLUG_ALLOW_LCLK_GATING_EN:1;
    UINT32                            CIO_PCIE_TPI_SLV_SSCLK_GATING_ENABLE:1;
    UINT32                            PCIE_CIO_TPI_MST_SSCLK_GATING_ENABLE:1;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} CPM_CONTROL_EXT_STRUCT;

#define SMN_PCIE0_CPM_CONTROL_EXT_ADDRESS    0x11180468UL

#define SMN_PCIE1_CPM_CONTROL_EXT_ADDRESS    0x11280468UL

#define SMN_PCIE2_CPM_CONTROL_EXT_ADDRESS    0x11380468UL

#define SMN_PCIE3_CPM_CONTROL_EXT_ADDRESS    0x11480468UL

/***********************************************************
* Register Name : LC_CPM_CONTROL_0
* Register Description :
* LC Per-Port Clock Gating Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls per-Port clock gating for LC, Port A.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_A_GATE_ENABLE_OFFSET      0
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_A_GATE_ENABLE_MASK        0x1

// Bitfield Description : Controls per-Port clock gating for LC, Port B.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_B_GATE_ENABLE_OFFSET      1
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_B_GATE_ENABLE_MASK        0x2

// Bitfield Description : Controls per-Port clock gating for LC, Port C.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_C_GATE_ENABLE_OFFSET      2
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_C_GATE_ENABLE_MASK        0x4

// Bitfield Description : Controls per-Port clock gating for LC, Port D.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_D_GATE_ENABLE_OFFSET      3
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_D_GATE_ENABLE_MASK        0x8

// Bitfield Description : Controls per-Port clock gating for LC, Port E.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_E_GATE_ENABLE_OFFSET      4
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_E_GATE_ENABLE_MASK        0x10

// Bitfield Description : Controls per-Port clock gating for LC, Port F.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_F_GATE_ENABLE_OFFSET      5
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_F_GATE_ENABLE_MASK        0x20

// Bitfield Description : Controls per-Port clock gating for LC, Port G.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_G_GATE_ENABLE_OFFSET      6
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_G_GATE_ENABLE_MASK        0x40

// Bitfield Description : Controls per-Port clock gating for LC, Port H.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_H_GATE_ENABLE_OFFSET      7
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_H_GATE_ENABLE_MASK        0x80

// Bitfield Description : Controls per-Port clock gating for LC, Port I.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_I_GATE_ENABLE_OFFSET      8
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_I_GATE_ENABLE_MASK        0x100

// Bitfield Description : Controls per-Port clock gating for LC, Port J.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_J_GATE_ENABLE_OFFSET      9
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_J_GATE_ENABLE_MASK        0x200

// Bitfield Description : Controls per-Port clock gating for LC, Port K.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_K_GATE_ENABLE_OFFSET      10
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_K_GATE_ENABLE_MASK        0x400

// Bitfield Description : Controls per-Port clock gating for LC, Port L.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_L_GATE_ENABLE_OFFSET      11
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_L_GATE_ENABLE_MASK        0x800

// Bitfield Description : Controls per-Port clock gating for LC, Port M.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_M_GATE_ENABLE_OFFSET      12
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_M_GATE_ENABLE_MASK        0x1000

// Bitfield Description : Controls per-Port clock gating for LC, Port N.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_N_GATE_ENABLE_OFFSET      13
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_N_GATE_ENABLE_MASK        0x2000

// Bitfield Description : Controls per-Port clock gating for LC, Port O.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_O_GATE_ENABLE_OFFSET      14
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_O_GATE_ENABLE_MASK        0x4000

// Bitfield Description : Controls per-Port clock gating for LC, Port P.
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_P_GATE_ENABLE_OFFSET      15
#define LC_CPM_CONTROL_0_TXCLK_DYN_PORT_P_GATE_ENABLE_MASK        0x8000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port A.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_A_GATE_ENABLE_OFFSET      16
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_A_GATE_ENABLE_MASK        0x10000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port B.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_B_GATE_ENABLE_OFFSET      17
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_B_GATE_ENABLE_MASK        0x20000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port C.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_C_GATE_ENABLE_OFFSET      18
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_C_GATE_ENABLE_MASK        0x40000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port D.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_D_GATE_ENABLE_OFFSET      19
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_D_GATE_ENABLE_MASK        0x80000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port E.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_E_GATE_ENABLE_OFFSET      20
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_E_GATE_ENABLE_MASK        0x100000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port F.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_F_GATE_ENABLE_OFFSET      21
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_F_GATE_ENABLE_MASK        0x200000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port G.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_G_GATE_ENABLE_OFFSET      22
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_G_GATE_ENABLE_MASK        0x400000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port H.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_H_GATE_ENABLE_OFFSET      23
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_H_GATE_ENABLE_MASK        0x800000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port I.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_I_GATE_ENABLE_OFFSET      24
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_I_GATE_ENABLE_MASK        0x1000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port J.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_J_GATE_ENABLE_OFFSET      25
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_J_GATE_ENABLE_MASK        0x2000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port K.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_K_GATE_ENABLE_OFFSET      26
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_K_GATE_ENABLE_MASK        0x4000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port L.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_L_GATE_ENABLE_OFFSET      27
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_L_GATE_ENABLE_MASK        0x8000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port M.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_M_GATE_ENABLE_OFFSET      28
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_M_GATE_ENABLE_MASK        0x10000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port N.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_N_GATE_ENABLE_OFFSET      29
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_N_GATE_ENABLE_MASK        0x20000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port O.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_O_GATE_ENABLE_OFFSET      30
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_O_GATE_ENABLE_MASK        0x40000000

// Bitfield Description : Controls per-Port equalization logic clock gating for LC, Port P.
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_P_GATE_ENABLE_OFFSET      31
#define LC_CPM_CONTROL_0_TXCLK_DYN_TR_PORT_P_GATE_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            TXCLK_DYN_PORT_A_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_B_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_C_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_D_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_E_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_F_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_G_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_H_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_I_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_J_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_K_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_L_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_M_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_N_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_O_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_PORT_P_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_A_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_B_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_C_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_D_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_E_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_F_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_G_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_H_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_I_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_J_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_K_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_L_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_M_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_N_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_O_GATE_ENABLE:1;
    UINT32                            TXCLK_DYN_TR_PORT_P_GATE_ENABLE:1;
  } Field;
  UINT32 Value;
} LC_CPM_CONTROL_0_STRUCT;

#define SMN_PCIE0_LC_CPM_CONTROL_0_ADDRESS    0x111804ccUL

#define SMN_PCIE1_LC_CPM_CONTROL_0_ADDRESS    0x112804ccUL

#define SMN_PCIE2_LC_CPM_CONTROL_0_ADDRESS    0x113804ccUL

#define SMN_PCIE3_LC_CPM_CONTROL_0_ADDRESS    0x114804ccUL


/***********************************************************
* Register Name : LC_CPM_CONTROL_1
* Register Description :
* LC Per-Port Clock Gating Control 1 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the number of clocks to wait before turning off per-Port LC clocks.
#define LC_CPM_CONTROL_1_TXCLK_DYN_PORT_GATE_LATENCY_OFFSET      0
#define LC_CPM_CONTROL_1_TXCLK_DYN_PORT_GATE_LATENCY_MASK        0x7

// Bitfield Description :
#define LC_CPM_CONTROL_1_Reserved_14_3_OFFSET      3
#define LC_CPM_CONTROL_1_Reserved_14_3_MASK        0x7ff8

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_A would be gated.
#define LC_CPM_CONTROL_1_TXCLK_PI_CLK_EN_ALL_LANES_GATE_ENABLE_OFFSET      15
#define LC_CPM_CONTROL_1_TXCLK_PI_CLK_EN_ALL_LANES_GATE_ENABLE_MASK        0x8000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_A would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_A_GATE_ENABLE_OFFSET      16
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_A_GATE_ENABLE_MASK        0x10000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_B would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_B_GATE_ENABLE_OFFSET      17
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_B_GATE_ENABLE_MASK        0x20000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_C would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_C_GATE_ENABLE_OFFSET      18
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_C_GATE_ENABLE_MASK        0x40000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_D would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_D_GATE_ENABLE_OFFSET      19
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_D_GATE_ENABLE_MASK        0x80000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_E would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_E_GATE_ENABLE_OFFSET      20
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_E_GATE_ENABLE_MASK        0x100000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_F would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_F_GATE_ENABLE_OFFSET      21
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_F_GATE_ENABLE_MASK        0x200000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_G would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_G_GATE_ENABLE_OFFSET      22
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_G_GATE_ENABLE_MASK        0x400000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_H would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_H_GATE_ENABLE_OFFSET      23
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_H_GATE_ENABLE_MASK        0x800000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_I would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_I_GATE_ENABLE_OFFSET      24
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_I_GATE_ENABLE_MASK        0x1000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_J would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_J_GATE_ENABLE_OFFSET      25
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_J_GATE_ENABLE_MASK        0x2000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_K would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_K_GATE_ENABLE_OFFSET      26
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_K_GATE_ENABLE_MASK        0x4000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_L would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_L_GATE_ENABLE_OFFSET      27
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_L_GATE_ENABLE_MASK        0x8000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_M would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_M_GATE_ENABLE_OFFSET      28
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_M_GATE_ENABLE_MASK        0x10000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_N would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_N_GATE_ENABLE_OFFSET      29
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_N_GATE_ENABLE_MASK        0x20000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_O would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_O_GATE_ENABLE_OFFSET      30
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_O_GATE_ENABLE_MASK        0x40000000

// Bitfield Description : Controls gating off the CLK_EN to RXP whenever TXCLK_DYN_PORT_P would be gated.
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_P_GATE_ENABLE_OFFSET      31
#define LC_CPM_CONTROL_1_TXCLK_RXP_CLK_EN_PORT_P_GATE_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            TXCLK_DYN_PORT_GATE_LATENCY:3;
    UINT32                            Reserved_14_3:12;
    UINT32                            TXCLK_PI_CLK_EN_ALL_LANES_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_A_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_B_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_C_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_D_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_E_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_F_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_G_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_H_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_I_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_J_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_K_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_L_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_M_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_N_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_O_GATE_ENABLE:1;
    UINT32                            TXCLK_RXP_CLK_EN_PORT_P_GATE_ENABLE:1;
  } Field;
  UINT32 Value;
} LC_CPM_CONTROL_1_STRUCT;

#define SMN_PCIE0_LC_CPM_CONTROL_1_ADDRESS    0x111804d0UL

#define SMN_PCIE1_LC_CPM_CONTROL_1_ADDRESS    0x112804d0UL

#define SMN_PCIE2_LC_CPM_CONTROL_1_ADDRESS    0x113804d0UL

#define SMN_PCIE3_LC_CPM_CONTROL_1_ADDRESS    0x114804d0UL


/***********************************************************
* Register Name : NBIO_CLKREQb_MAP_CNTL
* Register Description :
* NBIO CLKREQb Map Control register. Control for CLKREQb pin mapping between multi PCIe cores.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When PCIE_CLKREQB_0_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 0.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_0_MAP_OFFSET      0
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_0_MAP_MASK        0xf

// Bitfield Description : When PCIE_CLKREQB_1_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 1.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_1_MAP_OFFSET      4
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_1_MAP_MASK        0xf0

// Bitfield Description : When PCIE_CLKREQB_2_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 2.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_2_MAP_OFFSET      8
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_2_MAP_MASK        0xf00

// Bitfield Description : When PCIE_CLKREQB_3_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 3.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_3_MAP_OFFSET      12
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_3_MAP_MASK        0xf000

// Bitfield Description : When PCIE_CLKREQB_4_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 4.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_4_MAP_OFFSET      16
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_4_MAP_MASK        0xf0000

// Bitfield Description : When PCIE_CLKREQB_5_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 5.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_5_MAP_OFFSET      20
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_5_MAP_MASK        0xf00000

// Bitfield Description : When PCIE_CLKREQB_6_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 6.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_6_MAP_OFFSET      24
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_6_MAP_MASK        0xf000000

// Bitfield Description : When PCIE_CLKREQB_7_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 7.
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_7_MAP_OFFSET      28
#define NBIO_CLKREQb_MAP_CNTL_PCIE_CLKREQB_7_MAP_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            PCIE_CLKREQB_0_MAP:4;
    UINT32                            PCIE_CLKREQB_1_MAP:4;
    UINT32                            PCIE_CLKREQB_2_MAP:4;
    UINT32                            PCIE_CLKREQB_3_MAP:4;
    UINT32                            PCIE_CLKREQB_4_MAP:4;
    UINT32                            PCIE_CLKREQB_5_MAP:4;
    UINT32                            PCIE_CLKREQB_6_MAP:4;
    UINT32                            PCIE_CLKREQB_7_MAP:4;
  } Field;
  UINT32 Value;
} NBIO_CLKREQb_MAP_CNTL_STRUCT;

#define SMN_PCIE0_NBIO_CLKREQb_MAP_CNTL_ADDRESS    0x11180190UL

#define SMN_PCIE1_NBIO_CLKREQb_MAP_CNTL_ADDRESS    0x11280190UL

#define SMN_PCIE2_NBIO_CLKREQb_MAP_CNTL_ADDRESS    0x11380190UL

#define SMN_PCIE3_NBIO_CLKREQb_MAP_CNTL_ADDRESS    0x11480190UL


/***********************************************************
* Register Name : NBIO_CLKREQb_MAP_CNTL2
* Register Description :
* NBIO CLKREQb Map Control 2 register. Control for CLKREQb pin mapping between multi PCIe cores.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When PCIE_CLKREQB_8_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 8.
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_8_MAP_OFFSET      0
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_8_MAP_MASK        0xf

// Bitfield Description : When PCIE_CLKREQB_9_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 9.
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_9_MAP_OFFSET      4
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_9_MAP_MASK        0xf0

// Bitfield Description : When PCIE_CLKREQB_10_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 10.
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_10_MAP_OFFSET      8
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_10_MAP_MASK        0xf00

// Bitfield Description : When PCIE_CLKREQB_11_CNTL_MASK bit is 0, controls NBIO CLKREQb mapping for CLKREQb pin 11.
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_11_MAP_OFFSET      12
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_11_MAP_MASK        0xf000

// Bitfield Description : Controls if PCIE_CLKREQB_0_MAP fields are used to map CLKREQb pin 0
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_0_CNTL_MASK_OFFSET      16
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_0_CNTL_MASK_MASK        0x10000

// Bitfield Description : Controls if PCIE_CLKREQB_1_MAP fields are used to map CLKREQb pin 1
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_1_CNTL_MASK_OFFSET      17
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_1_CNTL_MASK_MASK        0x20000

// Bitfield Description : Controls if PCIE_CLKREQB_2_MAP fields are used to map CLKREQb pin 2
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_2_CNTL_MASK_OFFSET      18
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_2_CNTL_MASK_MASK        0x40000

// Bitfield Description : Controls if PCIE_CLKREQB_3_MAP fields are used to map CLKREQb pin 3
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_3_CNTL_MASK_OFFSET      19
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_3_CNTL_MASK_MASK        0x80000

// Bitfield Description : Controls if PCIE_CLKREQB_4_MAP fields are used to map CLKREQb pin 4
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_4_CNTL_MASK_OFFSET      20
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_4_CNTL_MASK_MASK        0x100000

// Bitfield Description : Controls if PCIE_CLKREQB_5_MAP fields are used to map CLKREQb pin 5
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_5_CNTL_MASK_OFFSET      21
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_5_CNTL_MASK_MASK        0x200000

// Bitfield Description : Controls if PCIE_CLKREQB_6_MAP fields are used to map CLKREQb pin 6
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_6_CNTL_MASK_OFFSET      22
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_6_CNTL_MASK_MASK        0x400000

// Bitfield Description : Controls if PCIE_CLKREQB_7_MAP fields are used to map CLKREQb pin 7
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_7_CNTL_MASK_OFFSET      23
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_7_CNTL_MASK_MASK        0x800000

// Bitfield Description : Controls if PCIE_CLKREQB_8_MAP fields are used to map CLKREQb pin 8
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_8_CNTL_MASK_OFFSET      24
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_8_CNTL_MASK_MASK        0x1000000

// Bitfield Description : Controls if PCIE_CLKREQB_9_MAP fields are used to map CLKREQb pin 9
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_9_CNTL_MASK_OFFSET      25
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_9_CNTL_MASK_MASK        0x2000000

// Bitfield Description : Controls if PCIE_CLKREQB_10_MAP fields are used to map CLKREQb pin 10
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_10_CNTL_MASK_OFFSET      26
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_10_CNTL_MASK_MASK        0x4000000

// Bitfield Description : Controls if PCIE_CLKREQB_11_MAP fields are used to map CLKREQb pin 11
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_11_CNTL_MASK_OFFSET      27
#define NBIO_CLKREQb_MAP_CNTL2_PCIE_CLKREQB_11_CNTL_MASK_MASK        0x8000000

// Bitfield Description :
#define NBIO_CLKREQb_MAP_CNTL2_Reserved_31_28_OFFSET      28
#define NBIO_CLKREQb_MAP_CNTL2_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            PCIE_CLKREQB_8_MAP:4;
    UINT32                            PCIE_CLKREQB_9_MAP:4;
    UINT32                            PCIE_CLKREQB_10_MAP:4;
    UINT32                            PCIE_CLKREQB_11_MAP:4;
    UINT32                            PCIE_CLKREQB_0_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_1_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_2_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_3_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_4_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_5_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_6_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_7_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_8_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_9_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_10_CNTL_MASK:1;
    UINT32                            PCIE_CLKREQB_11_CNTL_MASK:1;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} NBIO_CLKREQb_MAP_CNTL2_STRUCT;

#define SMN_PCIE0_NBIO_CLKREQb_MAP_CNTL2_ADDRESS    0x1118019cUL

#define SMN_PCIE1_NBIO_CLKREQb_MAP_CNTL2_ADDRESS    0x1128019cUL

#define SMN_PCIE2_NBIO_CLKREQb_MAP_CNTL2_ADDRESS    0x1138019cUL

#define SMN_PCIE3_NBIO_CLKREQb_MAP_CNTL2_ADDRESS    0x1148019cUL


/***********************************************************
* Register Name : PCIE_CI_CNTL
* Register Description :
* Chip Interface Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls chaining on slave writes.
#define PCIE_CI_CNTL_CI_SLV_SDP_CHAIN_DIS_OFFSET      0
#define PCIE_CI_CNTL_CI_SLV_SDP_CHAIN_DIS_MASK        0x1

// Bitfield Description :
#define PCIE_CI_CNTL_Reserved_2_1_OFFSET      1
#define PCIE_CI_CNTL_Reserved_2_1_MASK        0x6

// Bitfield Description : Controls allocating extra slots for slave completions.
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_OVERSUBSCRIBE_MODE_OFFSET      3
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_OVERSUBSCRIBE_MODE_MASK        0x38

// Bitfield Description : Reserved.
#define PCIE_CI_CNTL_CI_SLV_RC_RD_REQ_SIZE_OFFSET      6
#define PCIE_CI_CNTL_CI_SLV_RC_RD_REQ_SIZE_MASK        0xc0

// Bitfield Description : Controls if slave ordering logic is used.
#define PCIE_CI_CNTL_CI_SLV_ORDERING_DIS_OFFSET      8
#define PCIE_CI_CNTL_CI_SLV_ORDERING_DIS_MASK        0x100

// Bitfield Description : Controls if the slave uses SDP mem write full command requests.
#define PCIE_CI_CNTL_CI_SLV_SDP_MEM_WR_FULL_DIS_OFFSET      9
#define PCIE_CI_CNTL_CI_SLV_SDP_MEM_WR_FULL_DIS_MASK        0x200

// Bitfield Description : Controls if the slave completion buffer is sub-divided between Ports based on the number of active lanes.
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_DIS_OFFSET      10
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_DIS_MASK        0x400

// Bitfield Description : When CI_SLV_CPL_ALLOC_DIS is 0, controls the slave completion buffer sub-division method.
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_MODE_OFFSET      11
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_MODE_MASK        0x800

// Bitfield Description : Controls if the slave completion buffer is limited to only allowing a single outstanding read.
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_SOR_OFFSET      12
#define PCIE_CI_CNTL_CI_SLV_CPL_ALLOC_SOR_MASK        0x1000

// Bitfield Description :
#define PCIE_CI_CNTL_Reserved_15_13_OFFSET      13
#define PCIE_CI_CNTL_Reserved_15_13_MASK        0xe000

// Bitfield Description : Controls if TLPs with EP=1 sets PCIE_SLV_OrigDataError.
#define PCIE_CI_CNTL_CI_SLV_SDP_ERR_DATA_ON_POISONED_DIS_OFFSET      16
#define PCIE_CI_CNTL_CI_SLV_SDP_ERR_DATA_ON_POISONED_DIS_MASK        0x10000

// Bitfield Description :
#define PCIE_CI_CNTL_Reserved_20_17_OFFSET      17
#define PCIE_CI_CNTL_Reserved_20_17_MASK        0x1e0000

// Bitfield Description : Controls if PGMEM can block PG entry.
#define PCIE_CI_CNTL_TX_PGMEM_CTRL_PGATE_DIS_OFFSET      21
#define PCIE_CI_CNTL_TX_PGMEM_CTRL_PGATE_DIS_MASK        0x200000

// Bitfield Description : (Downstream Ports only) Controls if Root Ports output MST_PCIE_Rd/WrRspStatus=Unsupported Request when Completion Timeout occurs.
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_UR_EN_OFFSET      22
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_UR_EN_MASK        0x400000

// Bitfield Description : (Downstream Ports only) Controls if RPIO Exceptions are processed by changing completion status to Completer Abort.
#define PCIE_CI_CNTL_RX_RCB_RC_DPC_EXCEPTION_EN_OFFSET      23
#define PCIE_CI_CNTL_RX_RCB_RC_DPC_EXCEPTION_EN_MASK        0x800000

// Bitfield Description : (Downstream Ports only) Controls if all synthesized completions during DPC event are generated as Unsupported Request/Completer Abort rather than Completion Timeout.
#define PCIE_CI_CNTL_RX_RCB_RC_DPC_CPL_CTL_EN_OFFSET      24
#define PCIE_CI_CNTL_RX_RCB_RC_DPC_CPL_CTL_EN_MASK        0x1000000

// Bitfield Description :
#define PCIE_CI_CNTL_Reserved_28_25_OFFSET      25
#define PCIE_CI_CNTL_Reserved_28_25_MASK        0x1e000000

// Bitfield Description : (Downstream Ports only) Controls if Port will output MST_PCIE_Rd/WrRspStatus=OK with dummy data for Completion Timeout while in link-down state.
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_SC_IN_LINK_DOWN_EN_OFFSET      29
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_TO_SC_IN_LINK_DOWN_EN_MASK        0x20000000

// Bitfield Description : (Multi-Port only) Controls if slave Port arbitration to be weighted round robin, based on post-degrade link width.
#define PCIE_CI_CNTL_SLV_ARB_LINKWIDTH_WEIGHTED_RROBIN_EN_OFFSET      30
#define PCIE_CI_CNTL_SLV_ARB_LINKWIDTH_WEIGHTED_RROBIN_EN_MASK        0x40000000

// Bitfield Description : (Downstream Ports only) Controls if Port will log Completion Timeout errors in AER or DPC in link-down state.
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_OFFSET      31
#define PCIE_CI_CNTL_RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CI_SLV_SDP_CHAIN_DIS:1;
    UINT32                            Reserved_2_1:2;
    UINT32                            CI_SLV_CPL_ALLOC_OVERSUBSCRIBE_MODE:3;
    UINT32                            CI_SLV_RC_RD_REQ_SIZE:2;
    UINT32                            CI_SLV_ORDERING_DIS:1;
    UINT32                            CI_SLV_SDP_MEM_WR_FULL_DIS:1;
    UINT32                            CI_SLV_CPL_ALLOC_DIS:1;
    UINT32                            CI_SLV_CPL_ALLOC_MODE:1;
    UINT32                            CI_SLV_CPL_ALLOC_SOR:1;
    UINT32                            Reserved_15_13:3;
    UINT32                            CI_SLV_SDP_ERR_DATA_ON_POISONED_DIS:1;
    UINT32                            Reserved_20_17:4;
    UINT32                            TX_PGMEM_CTRL_PGATE_DIS:1;
    UINT32                            RX_RCB_RC_CTO_TO_UR_EN:1;
    UINT32                            RX_RCB_RC_DPC_EXCEPTION_EN:1;
    UINT32                            RX_RCB_RC_DPC_CPL_CTL_EN:1;
    UINT32                            Reserved_28_25:4;
    UINT32                            RX_RCB_RC_CTO_TO_SC_IN_LINK_DOWN_EN:1;
    UINT32                            SLV_ARB_LINKWIDTH_WEIGHTED_RROBIN_EN:1;
    UINT32                            RX_RCB_RC_CTO_IGNORE_ERR_IN_LINK_DOWN_EN:1;
  } Field;
  UINT32 Value;
} PCIE_CI_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_CI_CNTL_ADDRESS    0x11180080UL

#define SMN_PCIE1_PCIE_CI_CNTL_ADDRESS    0x11280080UL

#define SMN_PCIE2_PCIE_CI_CNTL_ADDRESS    0x11380080UL

#define SMN_PCIE3_PCIE_CI_CNTL_ADDRESS    0x11480080UL


/***********************************************************
* Register Name : PCIE_CNTL
* Register Description :
* PCIe Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if HwInit fields are read-only.
#define PCIE_CNTL_HWINIT_WR_LOCK_OFFSET      0
#define PCIE_CNTL_HWINIT_WR_LOCK_MASK        0x1

// Bitfield Description : Controls the Enhanced Hot-Plug counter threshold value.
#define PCIE_CNTL_LC_HOT_PLUG_DELAY_SEL_OFFSET      1
#define PCIE_CNTL_LC_HOT_PLUG_DELAY_SEL_MASK        0xe

// Bitfield Description :
#define PCIE_CNTL_Reserved_6_4_OFFSET      4
#define PCIE_CNTL_Reserved_6_4_MASK        0x70

// Bitfield Description : Controls Unsupported Request error reporting for TX.
#define PCIE_CNTL_UR_ERR_REPORT_DIS_OFFSET      7
#define PCIE_CNTL_UR_ERR_REPORT_DIS_MASK        0x80

// Bitfield Description : Controls if received AtomicOps are treated as Malformed TLPs.
#define PCIE_CNTL_PCIE_MALFORM_ATOMIC_OPS_OFFSET      8
#define PCIE_CNTL_PCIE_MALFORM_ATOMIC_OPS_MASK        0x100

// Bitfield Description : Controls memory write TLP mapping.
#define PCIE_CNTL_PCIE_HT_NP_MEM_WRITE_OFFSET      9
#define PCIE_CNTL_PCIE_HT_NP_MEM_WRITE_MASK        0x200

// Bitfield Description : Reserved.
#define PCIE_CNTL_RX_SB_ADJ_PAYLOAD_SIZE_OFFSET      10
#define PCIE_CNTL_RX_SB_ADJ_PAYLOAD_SIZE_MASK        0x1c00

// Bitfield Description :
#define PCIE_CNTL_Reserved_14_13_OFFSET      13
#define PCIE_CNTL_Reserved_14_13_MASK        0x6000

// Bitfield Description : Reserved.
#define PCIE_CNTL_RX_RCB_ATS_UC_DIS_OFFSET      15
#define PCIE_CNTL_RX_RCB_ATS_UC_DIS_MASK        0x8000

// Bitfield Description : Controls if the Receiver Completion Buffer is allowed to re-order received completions.
#define PCIE_CNTL_RX_RCB_REORDER_EN_OFFSET      16
#define PCIE_CNTL_RX_RCB_REORDER_EN_MASK        0x10000

// Bitfield Description : Controls if the Receiver Completion Buffer checks received completions for incorrect size.
#define PCIE_CNTL_RX_RCB_INVALID_SIZE_DIS_OFFSET      17
#define PCIE_CNTL_RX_RCB_INVALID_SIZE_DIS_MASK        0x20000

// Bitfield Description : Controls if the Receiver Completion Buffer checks if received completions are expected.
#define PCIE_CNTL_RX_RCB_UNEXP_CPL_DIS_OFFSET      18
#define PCIE_CNTL_RX_RCB_UNEXP_CPL_DIS_MASK        0x40000

// Bitfield Description : Reserved.
#define PCIE_CNTL_RX_RCB_CPL_TIMEOUT_TEST_MODE_OFFSET      19
#define PCIE_CNTL_RX_RCB_CPL_TIMEOUT_TEST_MODE_MASK        0x80000

// Bitfield Description : Controls if the Receiver Completion Buffer checks received completions for incorrect TLP prefixes.
#define PCIE_CNTL_RX_RCB_WRONG_PREFIX_DIS_OFFSET      20
#define PCIE_CNTL_RX_RCB_WRONG_PREFIX_DIS_MASK        0x100000

// Bitfield Description : Controls if the Receiver Completion Buffer checks received completions for incorrect attributes.
#define PCIE_CNTL_RX_RCB_WRONG_ATTR_DIS_OFFSET      21
#define PCIE_CNTL_RX_RCB_WRONG_ATTR_DIS_MASK        0x200000

// Bitfield Description : Controls if the Receiver Completion Buffer checks received completions for incorrect function number.
#define PCIE_CNTL_RX_RCB_WRONG_FUNCNUM_DIS_OFFSET      22
#define PCIE_CNTL_RX_RCB_WRONG_FUNCNUM_DIS_MASK        0x400000

// Bitfield Description : Reserved.
#define PCIE_CNTL_RX_ATS_TRAN_CPL_SPLIT_DIS_OFFSET      23
#define PCIE_CNTL_RX_ATS_TRAN_CPL_SPLIT_DIS_MASK        0x800000

// Bitfield Description : Reserved.
#define PCIE_CNTL_TX_CPL_DEBUG_OFFSET      24
#define PCIE_CNTL_TX_CPL_DEBUG_MASK        0x3f000000

// Bitfield Description : When LTR mechanism is disabled, controls if RX treats received LTR messages as Unsupported Request.
#define PCIE_CNTL_RX_IGNORE_LTR_MSG_UR_OFFSET      30
#define PCIE_CNTL_RX_IGNORE_LTR_MSG_UR_MASK        0x40000000

// Bitfield Description : Controls if RX is allowed to re-order completing received posted requests.
#define PCIE_CNTL_RX_CPL_POSTED_REQ_ORD_EN_OFFSET      31
#define PCIE_CNTL_RX_CPL_POSTED_REQ_ORD_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HWINIT_WR_LOCK:1;
    UINT32                            LC_HOT_PLUG_DELAY_SEL:3;
    UINT32                            Reserved_6_4:3;
    UINT32                            UR_ERR_REPORT_DIS:1;
    UINT32                            PCIE_MALFORM_ATOMIC_OPS:1;
    UINT32                            PCIE_HT_NP_MEM_WRITE:1;
    UINT32                            RX_SB_ADJ_PAYLOAD_SIZE:3;
    UINT32                            Reserved_14_13:2;
    UINT32                            RX_RCB_ATS_UC_DIS:1;
    UINT32                            RX_RCB_REORDER_EN:1;
    UINT32                            RX_RCB_INVALID_SIZE_DIS:1;
    UINT32                            RX_RCB_UNEXP_CPL_DIS:1;
    UINT32                            RX_RCB_CPL_TIMEOUT_TEST_MODE:1;
    UINT32                            RX_RCB_WRONG_PREFIX_DIS:1;
    UINT32                            RX_RCB_WRONG_ATTR_DIS:1;
    UINT32                            RX_RCB_WRONG_FUNCNUM_DIS:1;
    UINT32                            RX_ATS_TRAN_CPL_SPLIT_DIS:1;
    UINT32                            TX_CPL_DEBUG:6;
    UINT32                            RX_IGNORE_LTR_MSG_UR:1;
    UINT32                            RX_CPL_POSTED_REQ_ORD_EN:1;
  } Field;
  UINT32 Value;
} PCIE_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_CNTL_ADDRESS    0x11180040UL

#define SMN_PCIE1_PCIE_CNTL_ADDRESS    0x11280040UL

#define SMN_PCIE2_PCIE_CNTL_ADDRESS    0x11380040UL

#define SMN_PCIE3_PCIE_CNTL_ADDRESS    0x11480040UL


/***********************************************************
* Register Name : PCIE_CNTL2
* Register Description :
* PCIe Control 2 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define PCIE_CNTL2_Reserved_15_0_OFFSET      0
#define PCIE_CNTL2_Reserved_15_0_MASK        0xffff

// Bitfield Description : Controls Light Sleep Power Saving mode for the Slave Request and Completion static memories.
#define PCIE_CNTL2_SLV_MEM_LS_EN_OFFSET      16
#define PCIE_CNTL2_SLV_MEM_LS_EN_MASK        0x10000

// Bitfield Description : Controls Light Sleep Aggressive Power Saving mode for the Slave Request and Completion static memories (if supported).
#define PCIE_CNTL2_SLV_MEM_AGGRESSIVE_LS_EN_OFFSET      17
#define PCIE_CNTL2_SLV_MEM_AGGRESSIVE_LS_EN_MASK        0x20000

// Bitfield Description :
#define PCIE_CNTL2_Reserved_19_18_OFFSET      18
#define PCIE_CNTL2_Reserved_19_18_MASK        0xc0000

// Bitfield Description : Controls Shut Down Power Saving mode for the Slave Request and Completion static memories.
#define PCIE_CNTL2_SLV_MEM_SD_EN_OFFSET      20
#define PCIE_CNTL2_SLV_MEM_SD_EN_MASK        0x100000

// Bitfield Description : Controls Shut Down Aggressive Power Saving mode for the Slave Request and Completion static memories (if supported).
#define PCIE_CNTL2_SLV_MEM_AGGRESSIVE_SD_EN_OFFSET      21
#define PCIE_CNTL2_SLV_MEM_AGGRESSIVE_SD_EN_MASK        0x200000

// Bitfield Description :
#define PCIE_CNTL2_Reserved_23_22_OFFSET      22
#define PCIE_CNTL2_Reserved_23_22_MASK        0xc00000

// Bitfield Description : When TX_NP_MEM_WRITE_SWP_ENCODING bit is 1, controls value that encoding 0xE is changed to.
#define PCIE_CNTL2_RX_NP_MEM_WRITE_ENCODING_OFFSET      24
#define PCIE_CNTL2_RX_NP_MEM_WRITE_ENCODING_MASK        0x1f000000

// Bitfield Description : Controls Deep Sleep Power Saving mode for the Slave Request and Completion static memories.
#define PCIE_CNTL2_SLV_MEM_DS_EN_OFFSET      29
#define PCIE_CNTL2_SLV_MEM_DS_EN_MASK        0x20000000

// Bitfield Description :
#define PCIE_CNTL2_Reserved_31_30_OFFSET      30
#define PCIE_CNTL2_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            SLV_MEM_LS_EN:1;
    UINT32                            SLV_MEM_AGGRESSIVE_LS_EN:1;
    UINT32                            Reserved_19_18:2;
    UINT32                            SLV_MEM_SD_EN:1;
    UINT32                            SLV_MEM_AGGRESSIVE_SD_EN:1;
    UINT32                            Reserved_23_22:2;
    UINT32                            RX_NP_MEM_WRITE_ENCODING:5;
    UINT32                            SLV_MEM_DS_EN:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_CNTL2_STRUCT;

#define SMN_PCIE0_PCIE_CNTL2_ADDRESS    0x11180070UL

#define SMN_PCIE1_PCIE_CNTL2_ADDRESS    0x11280070UL

#define SMN_PCIE2_PCIE_CNTL2_ADDRESS    0x11380070UL

#define SMN_PCIE3_PCIE_CNTL2_ADDRESS    0x11480070UL


/***********************************************************
* Register Name : PCIE_CONFIG_CNTL
* Register Description :
* PCIe Configuration Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When bit 3 of this field is 1, bits 2-0 controls the TXCLK dynamic gating delay.
#define PCIE_CONFIG_CNTL_DYN_CLK_LATENCY_OFFSET      0
#define PCIE_CONFIG_CNTL_DYN_CLK_LATENCY_MASK        0xf

// Bitfield Description :
#define PCIE_CONFIG_CNTL_Reserved_31_4_OFFSET      4
#define PCIE_CONFIG_CNTL_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            DYN_CLK_LATENCY:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} PCIE_CONFIG_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_CONFIG_CNTL_ADDRESS    0x11180044UL

#define SMN_PCIE1_PCIE_CONFIG_CNTL_ADDRESS    0x11280044UL

#define SMN_PCIE2_PCIE_CONFIG_CNTL_ADDRESS    0x11380044UL

#define SMN_PCIE3_PCIE_CONFIG_CNTL_ADDRESS    0x11480044UL

/***********************************************************
* Register Name : PCIE_RXMARGIN_CONTROL_CAPABILITIES
* Register Description :
* RX Margining Capability Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if voltage margining is supported.
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_VOLTAGESUPPORTED_OFFSET      0
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_VOLTAGESUPPORTED_MASK        0x1

// Bitfield Description : Controls if independent up and down voltage margining is supported.
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDUPDOWNVOLTAGE_OFFSET      1
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDUPDOWNVOLTAGE_MASK        0x2

// Bitfield Description : Controls if independent left/right timing margining is supported.
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDLEFTRIGHTTIMING_OFFSET      2
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDLEFTRIGHTTIMING_MASK        0x4

// Bitfield Description : Controls the sample reporting method supported during margining.
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_SAMPLEREPORTINGMETHOD_OFFSET      3
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_SAMPLEREPORTINGMETHOD_MASK        0x8

// Bitfield Description : Controls if margining may produce errors in the data stream.
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDERRORSAMPLER_OFFSET      4
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_M_INDERRORSAMPLER_MASK        0x10

// Bitfield Description :
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_Reserved_31_5_OFFSET      5
#define PCIE_RXMARGIN_CONTROL_CAPABILITIES_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            M_VOLTAGESUPPORTED:1;
    UINT32                            M_INDUPDOWNVOLTAGE:1;
    UINT32                            M_INDLEFTRIGHTTIMING:1;
    UINT32                            M_SAMPLEREPORTINGMETHOD:1;
    UINT32                            M_INDERRORSAMPLER:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE_RXMARGIN_CONTROL_CAPABILITIES_STRUCT;

#define SMN_PCIE0_PCIE_RXMARGIN_CONTROL_CAPABILITIES_ADDRESS    0x111804d4UL

#define SMN_PCIE1_PCIE_RXMARGIN_CONTROL_CAPABILITIES_ADDRESS    0x112804d4UL

#define SMN_PCIE2_PCIE_RXMARGIN_CONTROL_CAPABILITIES_ADDRESS    0x113804d4UL

#define SMN_PCIE3_PCIE_RXMARGIN_CONTROL_CAPABILITIES_ADDRESS    0x114804d4UL

/***********************************************************
* Register Name : PCIE_RXMARGIN_1_SETTINGS
* Register Description :
* RX Margin Settings 1 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When M_VOLTAGESUPPORTED bit is set, controls the number of voltage steps from the default setting (either up or down), with the minimum range of +/-50 mV as measured by the 16.0 GT/s reference equalizer.
#define PCIE_RXMARGIN_1_SETTINGS_M_NUMVOLTAGESTEPS_OFFSET      0
#define PCIE_RXMARGIN_1_SETTINGS_M_NUMVOLTAGESTEPS_MASK        0x7f

// Bitfield Description : Controls the number of time steps from the default setting (to either left or right), where the range must be at least +/-0.2 UI.
#define PCIE_RXMARGIN_1_SETTINGS_M_NUMTIMINGSTEPS_OFFSET      7
#define PCIE_RXMARGIN_1_SETTINGS_M_NUMTIMINGSTEPS_MASK        0x1f80

// Bitfield Description : Controls the offset from the default setting at the maximum step value as a percentage of a nominal UI at 16.0 GT/s.
#define PCIE_RXMARGIN_1_SETTINGS_M_MAXTIMINGOFFSET_OFFSET      13
#define PCIE_RXMARGIN_1_SETTINGS_M_MAXTIMINGOFFSET_MASK        0xfe000

// Bitfield Description : When M_VOLTAGESUPPORTED bit is set, controls the offset from the default setting at the maximum step value as a percentage of 1 V.
#define PCIE_RXMARGIN_1_SETTINGS_M_MAXVOLTAGEOFFSET_OFFSET      20
#define PCIE_RXMARGIN_1_SETTINGS_M_MAXVOLTAGEOFFSET_MASK        0x7f00000

// Bitfield Description :
#define PCIE_RXMARGIN_1_SETTINGS_Reserved_31_27_OFFSET      27
#define PCIE_RXMARGIN_1_SETTINGS_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            M_NUMVOLTAGESTEPS:7;
    UINT32                            M_NUMTIMINGSTEPS:6;
    UINT32                            M_MAXTIMINGOFFSET:7;
    UINT32                            M_MAXVOLTAGEOFFSET:7;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} PCIE_RXMARGIN_1_SETTINGS_STRUCT;

#define SMN_PCIE0_PCIE_RXMARGIN_1_SETTINGS_ADDRESS    0x111804d8UL

#define SMN_PCIE1_PCIE_RXMARGIN_1_SETTINGS_ADDRESS    0x112804d8UL

#define SMN_PCIE2_PCIE_RXMARGIN_1_SETTINGS_ADDRESS    0x113804d8UL

#define SMN_PCIE3_PCIE_RXMARGIN_1_SETTINGS_ADDRESS    0x114804d8UL

/***********************************************************
* Register Name : PCIE_RXMARGIN_2_SETTINGS
* Register Description :
* RX Margin Settings 2 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the ratio of bits tested to bits received during voltage margining. A value of 0 is a ratio of 1:64 (1 bit of every 64 bits received), and a value of 63 is a ratio of 64:64 (all bits received).
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLINGRATEVOLTAGE_OFFSET      0
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLINGRATEVOLTAGE_MASK        0x3f

// Bitfield Description : Controls the ratio of bits tested to bits received during timing margining. A value of 0 is a ratio of 1:64 (1 bit of every 64 bits received), and a value of 63 is a ratio of 64:64 (all bits received).
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLINGRATETIMING_OFFSET      6
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLINGRATETIMING_MASK        0xfc0

// Bitfield Description : Controls the sample count value, where value = 3*log(2) (number of bits margined) and number of bits margined is the count of the actual number of bits tested during margining. This count stops when the sample count saturates, or when the error count limit (specified in M_ERROR_COUNT_LIMIT field) is reached. The count resets to 0 on each new margin command.
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLECOUNT_OFFSET      12
#define PCIE_RXMARGIN_2_SETTINGS_M_SAMPLECOUNT_MASK        0x7f000

// Bitfield Description : Controls the maximum number of lanes minus 1 that can be margined at the same time.
#define PCIE_RXMARGIN_2_SETTINGS_M_MAXLANES_OFFSET      19
#define PCIE_RXMARGIN_2_SETTINGS_M_MAXLANES_MASK        0xf80000

// Bitfield Description : Controls the error count limit.
#define PCIE_RXMARGIN_2_SETTINGS_M_ERROR_COUNT_LIMIT_OFFSET      24
#define PCIE_RXMARGIN_2_SETTINGS_M_ERROR_COUNT_LIMIT_MASK        0x3f000000

// Bitfield Description : Controls if precoding is used while margining.
#define PCIE_RXMARGIN_2_SETTINGS_ENABLE_PRECODING_OFFSET      30
#define PCIE_RXMARGIN_2_SETTINGS_ENABLE_PRECODING_MASK        0x40000000

// Bitfield Description :
#define PCIE_RXMARGIN_2_SETTINGS_Reserved_31_31_OFFSET      31
#define PCIE_RXMARGIN_2_SETTINGS_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            M_SAMPLINGRATEVOLTAGE:6;
    UINT32                            M_SAMPLINGRATETIMING:6;
    UINT32                            M_SAMPLECOUNT:7;
    UINT32                            M_MAXLANES:5;
    UINT32                            M_ERROR_COUNT_LIMIT:6;
    UINT32                            ENABLE_PRECODING:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PCIE_RXMARGIN_2_SETTINGS_STRUCT;

#define SMN_PCIE0_PCIE_RXMARGIN_2_SETTINGS_ADDRESS    0x111804dcUL

#define SMN_PCIE1_PCIE_RXMARGIN_2_SETTINGS_ADDRESS    0x112804dcUL

#define SMN_PCIE2_PCIE_RXMARGIN_2_SETTINGS_ADDRESS    0x113804dcUL

#define SMN_PCIE3_PCIE_RXMARGIN_2_SETTINGS_ADDRESS    0x114804dcUL



/***********************************************************
* Register Name : PCIE_MST_CTRL_2
* Register Description :
* PCIE Master (CI) control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if master memory writes will use the master_split256 module. When the master_split256 module is disabled, splitting is done by the TX splitter.
#define PCIE_MST_CTRL_2_CI_MSTSPLIT_DIS_OFFSET      0
#define PCIE_MST_CTRL_2_CI_MSTSPLIT_DIS_MASK        0x1

// Bitfield Description : Controls if master memory writes will be treated as if MST_PCIE_ReqChain is 1.
#define PCIE_MST_CTRL_2_CI_MSTSPLIT_REQ_CHAIN_DIS_OFFSET      1
#define PCIE_MST_CTRL_2_CI_MSTSPLIT_REQ_CHAIN_DIS_MASK        0x2

// Bitfield Description : Controls if the Tag Buffer is allowed to borrow tags from lower queues in 8-bit or 10-bit tag modes.
#define PCIE_MST_CTRL_2_CI_MST_TAG_BORROWING_DIS_OFFSET      2
#define PCIE_MST_CTRL_2_CI_MST_TAG_BORROWING_DIS_MASK        0x4

// Bitfield Description : (Root Complex only) Controls if split completions are allowed to use both channels.
#define PCIE_MST_CTRL_2_CI_SLAVE_SPLIT_MODE_OFFSET      3
#define PCIE_MST_CTRL_2_CI_SLAVE_SPLIT_MODE_MASK        0x8

// Bitfield Description : Controls if invalid slave addresses are not treated as Unsupported Request.
#define PCIE_MST_CTRL_2_CI_SLAVE_GEN_USR_DIS_OFFSET      4
#define PCIE_MST_CTRL_2_CI_SLAVE_GEN_USR_DIS_MASK        0x10

// Bitfield Description : Controls the dummy data used when a master's request does not complete successfully.
#define PCIE_MST_CTRL_2_CI_MST_CMPL_DUMMY_DATA_OFFSET      5
#define PCIE_MST_CTRL_2_CI_MST_CMPL_DUMMY_DATA_MASK        0x20

// Bitfield Description : Controls if RC ordering logic is used.
#define PCIE_MST_CTRL_2_CI_RC_ORDERING_DIS_OFFSET      6
#define PCIE_MST_CTRL_2_CI_RC_ORDERING_DIS_MASK        0x40

// Bitfield Description : Controls if the VC0 credits override register is used.
#define PCIE_MST_CTRL_2_MST_NPDAT_CREDITS_OVERRIDE_EN_OFFSET      7
#define PCIE_MST_CTRL_2_MST_NPDAT_CREDITS_OVERRIDE_EN_MASK        0x80

// Bitfield Description : Sets the number of VC0 MST_PCIE_OrigDataCredits to release/advertise.
#define PCIE_MST_CTRL_2_MST_NPDAT_CREDITS_ADVT_OFFSET      8
#define PCIE_MST_CTRL_2_MST_NPDAT_CREDITS_ADVT_MASK        0xff00

// Bitfield Description : Controls if byte-enables are forced to all asserted when client sends a MEM_RD with non-contiguous (illegal) byte-enables. This bit is sticky.
#define PCIE_MST_CTRL_2_CI_MST_MEMR_RD_NONCONT_BE_EN_OFFSET      16
#define PCIE_MST_CTRL_2_CI_MST_MEMR_RD_NONCONT_BE_EN_MASK        0x10000

// Bitfield Description : Controls if master SDP port does one-side completion disconnect, after one-side request disconnect. This bit is sticky.
#define PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_OFFSET      17
#define PCIE_MST_CTRL_2_CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL_MASK        0x20000

// Bitfield Description : Controls if clock gating is allowed on one-side master disconnect. This bit is sticky.
#define PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_OFFSET      18
#define PCIE_MST_CTRL_2_CI_MSTSDP_CLKGATE_ONESIDED_ENABLE_MASK        0x40000

// Bitfield Description : When 10-bit Tags are enabled, controls the range of tags used when issuing non-posted requests. This bit is sticky.
#define PCIE_MST_CTRL_2_CI_MST_TAG_1024_REQ_EN_OFFSET      19
#define PCIE_MST_CTRL_2_CI_MST_TAG_1024_REQ_EN_MASK        0x80000

// Bitfield Description : Controls if the Relaxed Ordering bit is allowed to be set when generating ATS requests.
#define PCIE_MST_CTRL_2_MST_ATS_RO_DIS_OFFSET      20
#define PCIE_MST_CTRL_2_MST_ATS_RO_DIS_MASK        0x100000

// Bitfield Description : Controls if the dropping of the Test and Modify AtomicOp is supported.
#define PCIE_MST_CTRL_2_MST_DUAL_OP_DROP_DIS_OFFSET      21
#define PCIE_MST_CTRL_2_MST_DUAL_OP_DROP_DIS_MASK        0x200000

// Bitfield Description :
#define PCIE_MST_CTRL_2_Reserved_22_22_OFFSET      22
#define PCIE_MST_CTRL_2_Reserved_22_22_MASK        0x400000

// Bitfield Description : Controls if the VC0 credits override register is used.
#define PCIE_MST_CTRL_2_MST_NPHDR_CREDITS_OVERRIDE_EN_OFFSET      23
#define PCIE_MST_CTRL_2_MST_NPHDR_CREDITS_OVERRIDE_EN_MASK        0x800000

// Bitfield Description : Sets the number of VC0 MST_PCIE_ReqCredits to release/advertise.
#define PCIE_MST_CTRL_2_MST_NPHDR_CREDITS_ADVT_OFFSET      24
#define PCIE_MST_CTRL_2_MST_NPHDR_CREDITS_ADVT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CI_MSTSPLIT_DIS:1;
    UINT32                            CI_MSTSPLIT_REQ_CHAIN_DIS:1;
    UINT32                            CI_MST_TAG_BORROWING_DIS:1;
    UINT32                            CI_SLAVE_SPLIT_MODE:1;
    UINT32                            CI_SLAVE_GEN_USR_DIS:1;
    UINT32                            CI_MST_CMPL_DUMMY_DATA:1;
    UINT32                            CI_RC_ORDERING_DIS:1;
    UINT32                            MST_NPDAT_CREDITS_OVERRIDE_EN:1;
    UINT32                            MST_NPDAT_CREDITS_ADVT:8;
    UINT32                            CI_MST_MEMR_RD_NONCONT_BE_EN:1;
    UINT32                            CI_MSTSDP_DISCONNECT_RSP_ON_PARTIAL:1;
    UINT32                            CI_MSTSDP_CLKGATE_ONESIDED_ENABLE:1;
    UINT32                            CI_MST_TAG_1024_REQ_EN:1;
    UINT32                            MST_ATS_RO_DIS:1;
    UINT32                            MST_DUAL_OP_DROP_DIS:1;
    UINT32                            Reserved_22_22:1;
    UINT32                            MST_NPHDR_CREDITS_OVERRIDE_EN:1;
    UINT32                            MST_NPHDR_CREDITS_ADVT:8;
  } Field;
  UINT32 Value;
} PCIE_MST_CTRL_2_STRUCT;

#define SMN_PCIE0_PCIE_MST_CTRL_2_ADDRESS    0x11180714UL

#define SMN_PCIE1_PCIE_MST_CTRL_2_ADDRESS    0x11280714UL

#define SMN_PCIE2_PCIE_MST_CTRL_2_ADDRESS    0x11380714UL

#define SMN_PCIE3_PCIE_MST_CTRL_2_ADDRESS    0x11480714UL

/***********************************************************
* Register Name : PCIE_MST_CTRL_3
* Register Description :
* Master Control 3 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define PCIE_MST_CTRL_3_Reserved_7_0_OFFSET      0
#define PCIE_MST_CTRL_3_Reserved_7_0_MASK        0xff

// Bitfield Description : (Switch Upstream only) Controls which maximum payload size value is used for Endpoint's maximum payload size received value.
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_PAYLOAD_SIZE_MODE_OFFSET      8
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_PAYLOAD_SIZE_MODE_MASK        0x100

// Bitfield Description : (Switch Upstream only) When CI_SWUS_MAX_PAYLOAD_SIZE_MODE bit is 1, contains the private value for Endpoint's maximum payload size received value.
#define PCIE_MST_CTRL_3_CI_SWUS_PRIV_MAX_PAYLOAD_SIZE_OFFSET      9
#define PCIE_MST_CTRL_3_CI_SWUS_PRIV_MAX_PAYLOAD_SIZE_MASK        0x600

// Bitfield Description : Controls which 10 bit tag enable value is used.
#define PCIE_MST_CTRL_3_CI_10BIT_TAG_EN_OVERRIDE_OFFSET      11
#define PCIE_MST_CTRL_3_CI_10BIT_TAG_EN_OVERRIDE_MASK        0x1800

// Bitfield Description : (Switch Upstream only) Controls which 10 bit tag enable value is used for Endpoint's 10-bit tag enable received value.
#define PCIE_MST_CTRL_3_CI_SWUS_10BIT_TAG_EN_OVERRIDE_OFFSET      13
#define PCIE_MST_CTRL_3_CI_SWUS_10BIT_TAG_EN_OVERRIDE_MASK        0x6000

// Bitfield Description :
#define PCIE_MST_CTRL_3_Reserved_15_15_OFFSET      15
#define PCIE_MST_CTRL_3_Reserved_15_15_MASK        0x8000

// Bitfield Description : Controls which maximum payload size value is used.
#define PCIE_MST_CTRL_3_CI_MAX_PAYLOAD_SIZE_MODE_OFFSET      16
#define PCIE_MST_CTRL_3_CI_MAX_PAYLOAD_SIZE_MODE_MASK        0x10000

// Bitfield Description : When CI_MAX_PAYLOAD_SIZE_MODE bit is 1, contains the private setting used for maximum payload size value.
#define PCIE_MST_CTRL_3_CI_PRIV_MAX_PAYLOAD_SIZE_OFFSET      17
#define PCIE_MST_CTRL_3_CI_PRIV_MAX_PAYLOAD_SIZE_MASK        0xe0000

// Bitfield Description : Controls which maximum read request size value is used.
#define PCIE_MST_CTRL_3_CI_MAX_READ_REQUEST_SIZE_MODE_OFFSET      20
#define PCIE_MST_CTRL_3_CI_MAX_READ_REQUEST_SIZE_MODE_MASK        0x100000

// Bitfield Description : When CI_MAX_READ_REQUEST_SIZE_MODE bit is 1, contains the private setting used for maximum read request size value.
#define PCIE_MST_CTRL_3_CI_PRIV_MAX_READ_REQUEST_SIZE_OFFSET      21
#define PCIE_MST_CTRL_3_CI_PRIV_MAX_READ_REQUEST_SIZE_MASK        0xe00000

// Bitfield Description : Reserved.
#define PCIE_MST_CTRL_3_CI_MAX_READ_SAFE_MODE_OFFSET      24
#define PCIE_MST_CTRL_3_CI_MAX_READ_SAFE_MODE_MASK        0x1000000

// Bitfield Description : Controls which extended tag enable value is used.
#define PCIE_MST_CTRL_3_CI_EXTENDED_TAG_EN_OVERRIDE_OFFSET      25
#define PCIE_MST_CTRL_3_CI_EXTENDED_TAG_EN_OVERRIDE_MASK        0x6000000

// Bitfield Description : (Switch Upstream only) Controls which maximum read request size value is used for Endpoint's maximum read request size received value.
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_READ_REQUEST_SIZE_MODE_OFFSET      27
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_READ_REQUEST_SIZE_MODE_MASK        0x8000000

// Bitfield Description : (Switch Upstream only) When CI_SWUS_MAX_READ_REQUEST_SIZE_MODE bit is 1, contains the private value for Endpoint's maximum read request size received value.
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_READ_REQUEST_SIZE_PRIV_OFFSET      28
#define PCIE_MST_CTRL_3_CI_SWUS_MAX_READ_REQUEST_SIZE_PRIV_MASK        0x30000000

// Bitfield Description : (Switch Upstream only) Contains the private value used for Endpoint's extended tag enable received value.
#define PCIE_MST_CTRL_3_CI_SWUS_EXTENDED_TAG_EN_OVERRIDE_OFFSET      30
#define PCIE_MST_CTRL_3_CI_SWUS_EXTENDED_TAG_EN_OVERRIDE_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            Reserved_7_0:8;
    UINT32                            CI_SWUS_MAX_PAYLOAD_SIZE_MODE:1;
    UINT32                            CI_SWUS_PRIV_MAX_PAYLOAD_SIZE:2;
    UINT32                            CI_10BIT_TAG_EN_OVERRIDE:2;
    UINT32                            CI_SWUS_10BIT_TAG_EN_OVERRIDE:2;
    UINT32                            Reserved_15_15:1;
    UINT32                            CI_MAX_PAYLOAD_SIZE_MODE:1;
    UINT32                            CI_PRIV_MAX_PAYLOAD_SIZE:3;
    UINT32                            CI_MAX_READ_REQUEST_SIZE_MODE:1;
    UINT32                            CI_PRIV_MAX_READ_REQUEST_SIZE:3;
    UINT32                            CI_MAX_READ_SAFE_MODE:1;
    UINT32                            CI_EXTENDED_TAG_EN_OVERRIDE:2;
    UINT32                            CI_SWUS_MAX_READ_REQUEST_SIZE_MODE:1;
    UINT32                            CI_SWUS_MAX_READ_REQUEST_SIZE_PRIV:2;
    UINT32                            CI_SWUS_EXTENDED_TAG_EN_OVERRIDE:2;
  } Field;
  UINT32 Value;
} PCIE_MST_CTRL_3_STRUCT;

#define SMN_PCIE0_PCIE_MST_CTRL_3_ADDRESS    0x11180718UL

#define SMN_PCIE1_PCIE_MST_CTRL_3_ADDRESS    0x11280718UL

#define SMN_PCIE2_PCIE_MST_CTRL_3_ADDRESS    0x11380718UL

#define SMN_PCIE3_PCIE_MST_CTRL_3_ADDRESS    0x11480718UL


/***********************************************************
* Register Name : PCIE_PGMST_CNTL
* Register Description :
* Power Gating Master Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the number of consecutive periods when all power gateable IPs assert PGMST_PGSLV_PWRGATE_READY before initiating power gating. Each period is 200 ns.
#define PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_OFFSET      0
#define PCIE_PGMST_CNTL_CFG_PG_HYSTERESIS_MASK        0xff

// Bitfield Description : Controls if dynamic power gating is enabled. Clearing this bit while power gated results in power un-gating.
#define PCIE_PGMST_CNTL_CFG_PG_EN_OFFSET      8
#define PCIE_PGMST_CNTL_CFG_PG_EN_MASK        0x100

// Bitfield Description :
#define PCIE_PGMST_CNTL_Reserved_9_9_OFFSET      9
#define PCIE_PGMST_CNTL_Reserved_9_9_MASK        0x200

// Bitfield Description : Controls the delay from idle to entering power gating. A zero value disables the idleness_counter (PGMST only waits for the hysteresis timeout to enter power gating). A non-zero value enables the idleness_counter using that value (enter power gated state as soon as PGSLV becomes idle, without waiting for the hysteresis timeout).
#define PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_OFFSET      10
#define PCIE_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_MASK        0x3c00

// Bitfield Description : Controls the power gating exit mode. When bit 0 is set it enables sending the pwrmgr_int for FW interlock feature. When bit 1 is set it enables wake up from power gated state using the pwrmgt_int_clear signal.
#define PCIE_PGMST_CNTL_CFG_FW_PG_EXIT_CNTL_OFFSET      14
#define PCIE_PGMST_CNTL_CFG_FW_PG_EXIT_CNTL_MASK        0xc000

// Bitfield Description : Controls the time (in clocks) the resets are to remain asserted after isolation removal, also controls how long to wait after everything is normal before allowing traffic to resume.
#define PCIE_PGMST_CNTL_PG_EXIT_TIMER_OFFSET      16
#define PCIE_PGMST_CNTL_PG_EXIT_TIMER_MASK        0xff0000

// Bitfield Description :
#define PCIE_PGMST_CNTL_Reserved_31_24_OFFSET      24
#define PCIE_PGMST_CNTL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CFG_PG_HYSTERESIS:8;
    UINT32                            CFG_PG_EN:1;
    UINT32                            Reserved_9_9:1;
    UINT32                            CFG_IDLENESS_COUNT_EN:4;
    UINT32                            CFG_FW_PG_EXIT_CNTL:2;
    UINT32                            PG_EXIT_TIMER:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} PCIE_PGMST_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_PGMST_CNTL_ADDRESS    0x111804c0UL

#define SMN_PCIE1_PCIE_PGMST_CNTL_ADDRESS    0x112804c0UL

#define SMN_PCIE2_PCIE_PGMST_CNTL_ADDRESS    0x113804c0UL

#define SMN_PCIE3_PCIE_PGMST_CNTL_ADDRESS    0x114804c0UL


/***********************************************************
* Register Name : PCIE_PGSLV_CNTL
* Register Description :
* Power Gating Slave Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the number of consecutive clocks when IP is idle before asserting PGMST_PGSLV_PWRGATE_READY.
#define PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_OFFSET      0
#define PCIE_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_MASK        0x1f

// Bitfield Description :
#define PCIE_PGSLV_CNTL_Reserved_31_5_OFFSET      5
#define PCIE_PGSLV_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            CFG_IDLE_HYSTERESIS:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE_PGSLV_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_PGSLV_CNTL_ADDRESS    0x111804c4UL

#define SMN_PCIE1_PCIE_PGSLV_CNTL_ADDRESS    0x112804c4UL

#define SMN_PCIE2_PCIE_PGSLV_CNTL_ADDRESS    0x113804c4UL

#define SMN_PCIE3_PCIE_PGSLV_CNTL_ADDRESS    0x114804c4UL


/***********************************************************
* Register Name : PCIE_P_CNTL
* Register Description :
* PHY Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Reserved.
#define PCIE_P_CNTL_P_PWRDN_EN_OFFSET      0
#define PCIE_P_CNTL_P_PWRDN_EN_MASK        0x1

// Bitfield Description : Controls Data Valid generation mode.
#define PCIE_P_CNTL_P_SYMALIGN_MODE_OFFSET      1
#define PCIE_P_CNTL_P_SYMALIGN_MODE_MASK        0x2

// Bitfield Description : Reserved.
#define PCIE_P_CNTL_P_SYMALIGN_HW_DEBUG_OFFSET      2
#define PCIE_P_CNTL_P_SYMALIGN_HW_DEBUG_MASK        0x4

// Bitfield Description : Reserved.
#define PCIE_P_CNTL_P_ELASTDESKEW_HW_DEBUG_OFFSET      3
#define PCIE_P_CNTL_P_ELASTDESKEW_HW_DEBUG_MASK        0x8

// Bitfield Description : Controls ignoring bad FCRC and bad FP for STP token.
#define PCIE_P_CNTL_P_IGNORE_CRC_ERR_OFFSET      4
#define PCIE_P_CNTL_P_IGNORE_CRC_ERR_MASK        0x10

// Bitfield Description : Controls ignoring invalid length = 0 for STP token.
#define PCIE_P_CNTL_P_IGNORE_LEN_ERR_OFFSET      5
#define PCIE_P_CNTL_P_IGNORE_LEN_ERR_MASK        0x20

// Bitfield Description : Controls ignoring bad EDB token.
#define PCIE_P_CNTL_P_IGNORE_EDB_ERR_OFFSET      6
#define PCIE_P_CNTL_P_IGNORE_EDB_ERR_MASK        0x40

// Bitfield Description : Controls ignoring bad IDL token.
#define PCIE_P_CNTL_P_IGNORE_IDL_ERR_OFFSET      7
#define PCIE_P_CNTL_P_IGNORE_IDL_ERR_MASK        0x80

// Bitfield Description : Controls ignoring invalid token.
#define PCIE_P_CNTL_P_IGNORE_TOK_ERR_OFFSET      8
#define PCIE_P_CNTL_P_IGNORE_TOK_ERR_MASK        0x100

// Bitfield Description :
#define PCIE_P_CNTL_Reserved_11_9_OFFSET      9
#define PCIE_P_CNTL_Reserved_11_9_MASK        0xe00

// Bitfield Description : Reserved.
#define PCIE_P_CNTL_P_BLK_LOCK_MODE_OFFSET      12
#define PCIE_P_CNTL_P_BLK_LOCK_MODE_MASK        0x1000

// Bitfield Description : Controls the selection method for the TX clock rate used by all
// Ports.
#define PCIE_P_CNTL_P_ALWAYS_USE_FAST_TXCLK_OFFSET      13
#define PCIE_P_CNTL_P_ALWAYS_USE_FAST_TXCLK_MASK        0x2000

// Bitfield Description : Controls the Electrical Idle detection mode used for PI (Physical Layer), for each data rate. Terms used are: EI indicates Electrical Idle entry; EIE indicates Electrical Idle Exit; PHY indicates PHY's Electrical Idle Detector is used; INF indicates that PI's Electrical Idle inference logic is used.
#define PCIE_P_CNTL_P_ELEC_IDLE_MODE_OFFSET      14
#define PCIE_P_CNTL_P_ELEC_IDLE_MODE_MASK        0xc000

// Bitfield Description :
#define PCIE_P_CNTL_Reserved_16_16_OFFSET      16
#define PCIE_P_CNTL_Reserved_16_16_MASK        0x10000

// Bitfield Description : Controls the PIPE requirement that MAC must always have TxDataValid asserted when TxElecIdle transitions to either asserted or de-asserted.
#define PCIE_P_CNTL_ASSERT_DVALID_ON_EI_TRANS_OFFSET      17
#define PCIE_P_CNTL_ASSERT_DVALID_ON_EI_TRANS_MASK        0x20000

// Bitfield Description : Controls which internal circuit is used to request PCLK changes.
#define PCIE_P_CNTL_LC_PCLK_USE_OLD_CLOCK_CIRCUIT_OFFSET      18
#define PCIE_P_CNTL_LC_PCLK_USE_OLD_CLOCK_CIRCUIT_MASK        0x40000

// Bitfield Description : Controls which physical lane is attached to the master PLL. When MASTER_PLL_LANE_REFCLKREQ_EN bit is 1, the Port which contains this lane in the current Port config must assert its refClkReq before any other Port can exit L1 substates.
#define PCIE_P_CNTL_MASTER_PLL_LANE_NUM_OFFSET      19
#define PCIE_P_CNTL_MASTER_PLL_LANE_NUM_MASK        0x780000

// Bitfield Description : Controls if the Port containing the master PLL lane needs to assert refClkReq before any other Port can wake up from L1 substates.
#define PCIE_P_CNTL_MASTER_PLL_LANE_REFCLKREQ_EN_OFFSET      23
#define PCIE_P_CNTL_MASTER_PLL_LANE_REFCLKREQ_EN_MASK        0x800000

// Bitfield Description : Controls if a Port waking up from L1 substates must wait for the master PLL lane to finish waking up.
#define PCIE_P_CNTL_REFCLKREQ_WAIT_FOR_MASTER_PLL_OFFSET      24
#define PCIE_P_CNTL_REFCLKREQ_WAIT_FOR_MASTER_PLL_MASK        0x1000000

// Bitfield Description : Controls if the port tsx_rcv module filters out the COM from a SKIP at Gen1/2 to prevent it from interrupting the count for 8 logical idle.
#define PCIE_P_CNTL_LC_FILTER_SKP_FROM_L_IDLE_OFFSET      25
#define PCIE_P_CNTL_LC_FILTER_SKP_FROM_L_IDLE_MASK        0x2000000

// Bitfield Description : Controls if the per-lane refClkReq signals can toggle freely if the lane is tied-off, assuming that refClkAck is tied-off and ignoring its current value.
#define PCIE_P_CNTL_LC_TIEOFF_LANES_IGNORE_REFCLKACK_OFFSET      26
#define PCIE_P_CNTL_LC_TIEOFF_LANES_IGNORE_REFCLKACK_MASK        0x4000000

// Bitfield Description :
#define PCIE_P_CNTL_Reserved_31_27_OFFSET      27
#define PCIE_P_CNTL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            P_PWRDN_EN:1;
    UINT32                            P_SYMALIGN_MODE:1;
    UINT32                            P_SYMALIGN_HW_DEBUG:1;
    UINT32                            P_ELASTDESKEW_HW_DEBUG:1;
    UINT32                            P_IGNORE_CRC_ERR:1;
    UINT32                            P_IGNORE_LEN_ERR:1;
    UINT32                            P_IGNORE_EDB_ERR:1;
    UINT32                            P_IGNORE_IDL_ERR:1;
    UINT32                            P_IGNORE_TOK_ERR:1;
    UINT32                            Reserved_11_9:3;
    UINT32                            P_BLK_LOCK_MODE:1;
    UINT32                            P_ALWAYS_USE_FAST_TXCLK:1;
    UINT32                            P_ELEC_IDLE_MODE:2;
    UINT32                            Reserved_16_16:1;
    UINT32                            ASSERT_DVALID_ON_EI_TRANS:1;
    UINT32                            LC_PCLK_USE_OLD_CLOCK_CIRCUIT:1;
    UINT32                            MASTER_PLL_LANE_NUM:4;
    UINT32                            MASTER_PLL_LANE_REFCLKREQ_EN:1;
    UINT32                            REFCLKREQ_WAIT_FOR_MASTER_PLL:1;
    UINT32                            LC_FILTER_SKP_FROM_L_IDLE:1;
    UINT32                            LC_TIEOFF_LANES_IGNORE_REFCLKACK:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} PCIE_P_CNTL_STRUCT;

#define SMN_PCIE0_PCIE_P_CNTL_ADDRESS    0x11180100UL

#define SMN_PCIE1_PCIE_P_CNTL_ADDRESS    0x11280100UL

#define SMN_PCIE2_PCIE_P_CNTL_ADDRESS    0x11380100UL

#define SMN_PCIE3_PCIE_P_CNTL_ADDRESS    0x11480100UL


/***********************************************************
* Register Name : PCIE_RX_CNTL2
* Register Description :
* RX Control 2 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls ignoring TLP with Invalid PASID. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_INVALIDPASID_UR_OFFSET      0
#define PCIE_RX_CNTL2_RX_IGNORE_EP_INVALIDPASID_UR_MASK        0x1

// Bitfield Description : Controls ignoring Translated Memory Read TLP. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_TRANSMRD_UR_OFFSET      1
#define PCIE_RX_CNTL2_RX_IGNORE_EP_TRANSMRD_UR_MASK        0x2

// Bitfield Description : Controls ignoring Translated Memory Write TLP. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_TRANSMWR_UR_OFFSET      2
#define PCIE_RX_CNTL2_RX_IGNORE_EP_TRANSMWR_UR_MASK        0x4

// Bitfield Description : Controls ignoring ATS Translation Request TLP. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_ATSTRANSREQ_UR_OFFSET      3
#define PCIE_RX_CNTL2_RX_IGNORE_EP_ATSTRANSREQ_UR_MASK        0x8

// Bitfield Description : Controls ignoring Page Request Message TLP. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_PAGEREQMSG_UR_OFFSET      4
#define PCIE_RX_CNTL2_RX_IGNORE_EP_PAGEREQMSG_UR_MASK        0x10

// Bitfield Description : Controls ignoring Invalidate Completion TLP. When this bit is 1, Unsupported Request is not reported. This bit applies to Endpoint only and it is hardwired to 1 in BIF core for Root Complex.
#define PCIE_RX_CNTL2_RX_IGNORE_EP_INVCPL_UR_OFFSET      5
#define PCIE_RX_CNTL2_RX_IGNORE_EP_INVCPL_UR_MASK        0x20

// Bitfield Description :
#define PCIE_RX_CNTL2_Reserved_7_6_OFFSET      6
#define PCIE_RX_CNTL2_Reserved_7_6_MASK        0xc0

// Bitfield Description : Controls the Master Completion Latency timer feature.
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_EN_OFFSET      8
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_EN_MASK        0x100

// Bitfield Description : When RX_RCB_LATENCY_EN bit is 1, controls the scale for the Master Completion latency measurement. 1 tick = 8ns * 2^SCALE.
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_SCALE_OFFSET      9
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_SCALE_MASK        0xe00

// Bitfield Description : Controls Light Sleep Power Saving mode for the Completion static memories.
#define PCIE_RX_CNTL2_SLVCPL_MEM_LS_EN_OFFSET      12
#define PCIE_RX_CNTL2_SLVCPL_MEM_LS_EN_MASK        0x1000

// Bitfield Description : Controls Shut Down Power Saving mode for the Completion static memories.
#define PCIE_RX_CNTL2_SLVCPL_MEM_SD_EN_OFFSET      13
#define PCIE_RX_CNTL2_SLVCPL_MEM_SD_EN_MASK        0x2000

// Bitfield Description : Controls Deep Sleep Power Saving mode for the Completion static memories.
#define PCIE_RX_CNTL2_SLVCPL_MEM_DS_EN_OFFSET      14
#define PCIE_RX_CNTL2_SLVCPL_MEM_DS_EN_MASK        0x4000

// Bitfield Description :
#define PCIE_RX_CNTL2_Reserved_15_15_OFFSET      15
#define PCIE_RX_CNTL2_Reserved_15_15_MASK        0x8000

// Bitfield Description : When RX_RCB_LATENCY_EN bit is 1, captures the Master Completion latency measurement for the highest latency. Time = (0x3ff - MAX_COUNT) * 8ns * 2^SCALE.
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_MAX_COUNT_OFFSET      16
#define PCIE_RX_CNTL2_RX_RCB_LATENCY_MAX_COUNT_MASK        0x3ff0000

// Bitfield Description :
#define PCIE_RX_CNTL2_Reserved_27_26_OFFSET      26
#define PCIE_RX_CNTL2_Reserved_27_26_MASK        0xc000000

// Bitfield Description : Controls extending the time FLR is active in the Receiver Completion Buffer, in order to support silently ignoring unexpected completions targeting the function during and after FLR.
#define PCIE_RX_CNTL2_FLR_EXTEND_MODE_OFFSET      28
#define PCIE_RX_CNTL2_FLR_EXTEND_MODE_MASK        0x70000000

// Bitfield Description :
#define PCIE_RX_CNTL2_Reserved_31_31_OFFSET      31
#define PCIE_RX_CNTL2_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RX_IGNORE_EP_INVALIDPASID_UR:1;
    UINT32                            RX_IGNORE_EP_TRANSMRD_UR:1;
    UINT32                            RX_IGNORE_EP_TRANSMWR_UR:1;
    UINT32                            RX_IGNORE_EP_ATSTRANSREQ_UR:1;
    UINT32                            RX_IGNORE_EP_PAGEREQMSG_UR:1;
    UINT32                            RX_IGNORE_EP_INVCPL_UR:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            RX_RCB_LATENCY_EN:1;
    UINT32                            RX_RCB_LATENCY_SCALE:3;
    UINT32                            SLVCPL_MEM_LS_EN:1;
    UINT32                            SLVCPL_MEM_SD_EN:1;
    UINT32                            SLVCPL_MEM_DS_EN:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            RX_RCB_LATENCY_MAX_COUNT:10;
    UINT32                            Reserved_27_26:2;
    UINT32                            FLR_EXTEND_MODE:3;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PCIE_RX_CNTL2_STRUCT;

#define SMN_PCIE0_PCIE_RX_CNTL2_ADDRESS    0x11180074UL

#define SMN_PCIE1_PCIE_RX_CNTL2_ADDRESS    0x11280074UL

#define SMN_PCIE2_PCIE_RX_CNTL2_ADDRESS    0x11380074UL

#define SMN_PCIE3_PCIE_RX_CNTL2_ADDRESS    0x11480074UL


/***********************************************************
* Register Name : PCIE_SDP_CTRL
* Register Description :
* SDP Interface Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls UnitID[6:3] for SDP interface.
#define PCIE_SDP_CTRL_SDP_UNIT_ID_OFFSET      0
#define PCIE_SDP_CTRL_SDP_UNIT_ID_MASK        0xf

// Bitfield Description : Controls if slave SDP interface requests full disconnect when idle and all Ports are in L1 or lower power state. This bit is sticky.
#define PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_OFFSET      4
#define PCIE_SDP_CTRL_CI_SLV_REQR_FULL_DISCONNECT_EN_MASK        0x10

// Bitfield Description : When CI_SLV_REQR_FULL_DISCONNECT_EN bit is 0, controls if slave SDP interface requests partial disconnect when idle and all Ports are in L1 or lower power state. This bit is sticky.
#define PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_OFFSET      5
#define PCIE_SDP_CTRL_CI_SLV_REQR_PART_DISCONNECT_EN_MASK        0x20

// Bitfield Description :
#define PCIE_SDP_CTRL_Reserved_8_6_OFFSET      6
#define PCIE_SDP_CTRL_Reserved_8_6_MASK        0x1c0

// Bitfield Description : Controls if slave posted requests are allowed to use ReqTag = 0x00-0x7F. This bit is sticky.
#define PCIE_SDP_CTRL_CI_SLAVE_TAG_STEALING_DIS_OFFSET      9
#define PCIE_SDP_CTRL_CI_SLAVE_TAG_STEALING_DIS_MASK        0x200

// Bitfield Description : (Upstream Ports only) Controls if slave requests are allowed to preload a valid TLP Prefix. This bit is sticky.
#define PCIE_SDP_CTRL_SLAVE_PREFIX_PRELOAD_DIS_OFFSET      10
#define PCIE_SDP_CTRL_SLAVE_PREFIX_PRELOAD_DIS_MASK        0x400

// Bitfield Description : When Switch Upstream Port has LTR Mechanism Enable bit 0, controls if master LTR messages are dropped. This bit is sticky.
#define PCIE_SDP_CTRL_CI_DISABLE_LTR_DROPPING_OFFSET      11
#define PCIE_SDP_CTRL_CI_DISABLE_LTR_DROPPING_MASK        0x800

// Bitfield Description : (Switch Upstream only) Controls if RCB error fifo is updated. This bit is sticky.
#define PCIE_SDP_CTRL_RX_SWUS_SIDEBAND_CPLHDR_DIS_OFFSET      12
#define PCIE_SDP_CTRL_RX_SWUS_SIDEBAND_CPLHDR_DIS_MASK        0x1000

// Bitfield Description :
#define PCIE_SDP_CTRL_Reserved_14_13_OFFSET      13
#define PCIE_SDP_CTRL_Reserved_14_13_MASK        0x6000

// Bitfield Description : (Switch Upstream only) Controls if Error messages received on the secondary side are forwarded to the primary side. This bit is sticky.
#define PCIE_SDP_CTRL_CI_SWUS_RCVD_ERR_HANDLING_DIS_OFFSET      15
#define PCIE_SDP_CTRL_CI_SWUS_RCVD_ERR_HANDLING_DIS_MASK        0x8000

// Bitfield Description : Controls if Hardware Early Wake Up feature is used. This bit is sticky.
#define PCIE_SDP_CTRL_EARLY_HW_WAKE_UP_EN_OFFSET      16
#define PCIE_SDP_CTRL_EARLY_HW_WAKE_UP_EN_MASK        0x10000

// Bitfield Description : Controls if slave SDP disconnect requires that all Ports are in L1 or L2/L3 Ready. This bit is sticky.
#define PCIE_SDP_CTRL_SLV_SDP_DISCONNECT_WHEN_IN_L1_EN_OFFSET      17
#define PCIE_SDP_CTRL_SLV_SDP_DISCONNECT_WHEN_IN_L1_EN_MASK        0x20000

// Bitfield Description : Controls if slave SDP disconnect is prevented for a Hardware Early Wake Up initiated SDP connection. This bit is sticky.
#define PCIE_SDP_CTRL_BLOCK_SLV_SDP_DISCONNECT_WHEN_EARLY_HW_WAKE_UP_EN_OFFSET      18
#define PCIE_SDP_CTRL_BLOCK_SLV_SDP_DISCONNECT_WHEN_EARLY_HW_WAKE_UP_EN_MASK        0x40000

// Bitfield Description : Controls if PCIE_SLV_RdRspDataParity is checked.
#define PCIE_SDP_CTRL_CI_SLV_SDP_PARITY_CHECK_EN_OFFSET      19
#define PCIE_SDP_CTRL_CI_SLV_SDP_PARITY_CHECK_EN_MASK        0x80000

// Bitfield Description :
#define PCIE_SDP_CTRL_Reserved_24_20_OFFSET      20
#define PCIE_SDP_CTRL_Reserved_24_20_MASK        0x1f00000

// Bitfield Description : In Switch mode, controls when to send a virtual message from Switch Upstream to nBIF. This bit is sticky.
#define PCIE_SDP_CTRL_CI_VIRTUAL_WIRE_MODE_OFFSET      25
#define PCIE_SDP_CTRL_CI_VIRTUAL_WIRE_MODE_MASK        0x2000000

// Bitfield Description : Controls UnitID[2:0] for SDP interface. Effective UnitID = {SDP_UNIT_ID[3:0], SDP_UNIT_ID_LOWER[28:26]} + port number.
#define PCIE_SDP_CTRL_SDP_UNIT_ID_LOWER_OFFSET      26
#define PCIE_SDP_CTRL_SDP_UNIT_ID_LOWER_MASK        0x1c000000

// Bitfield Description : Controls if MNTR-CI handshake allows reconfigure.
#define PCIE_SDP_CTRL_CI_SDP_RECONFIG_EN_OFFSET      29
#define PCIE_SDP_CTRL_CI_SDP_RECONFIG_EN_MASK        0x20000000

// Bitfield Description : When set, will send Link-Down as virtual wire bit46 to nBIF in SW.US mode. Bit46 is redundant. Side-band is used
#define PCIE_SDP_CTRL_CI_VIRTUAL_WIRE_BIT46_EN_OFFSET      30
#define PCIE_SDP_CTRL_CI_VIRTUAL_WIRE_BIT46_EN_MASK        0x40000000

// Bitfield Description :
#define PCIE_SDP_CTRL_Reserved_31_31_OFFSET      31
#define PCIE_SDP_CTRL_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SDP_UNIT_ID:4;
    UINT32                            CI_SLV_REQR_FULL_DISCONNECT_EN:1;
    UINT32                            CI_SLV_REQR_PART_DISCONNECT_EN:1;
    UINT32                            Reserved_8_6:3;
    UINT32                            CI_SLAVE_TAG_STEALING_DIS:1;
    UINT32                            SLAVE_PREFIX_PRELOAD_DIS:1;
    UINT32                            CI_DISABLE_LTR_DROPPING:1;
    UINT32                            RX_SWUS_SIDEBAND_CPLHDR_DIS:1;
    UINT32                            Reserved_14_13:2;
    UINT32                            CI_SWUS_RCVD_ERR_HANDLING_DIS:1;
    UINT32                            EARLY_HW_WAKE_UP_EN:1;
    UINT32                            SLV_SDP_DISCONNECT_WHEN_IN_L1_EN:1;
    UINT32                            BLOCK_SLV_SDP_DISCONNECT_WHEN_EARLY_HW_WAKE_UP_EN:1;
    UINT32                            CI_SLV_SDP_PARITY_CHECK_EN:1;
    UINT32                            Reserved_24_20:5;
    UINT32                            CI_VIRTUAL_WIRE_MODE:1;
    UINT32                            SDP_UNIT_ID_LOWER:3;
    UINT32                            CI_SDP_RECONFIG_EN:1;
    UINT32                            CI_VIRTUAL_WIRE_BIT46_EN:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PCIE_SDP_CTRL_STRUCT;

#define SMN_PCIE0_PCIE_SDP_CTRL_ADDRESS    0x1118018cUL

#define SMN_PCIE1_PCIE_SDP_CTRL_ADDRESS    0x1128018cUL

#define SMN_PCIE2_PCIE_SDP_CTRL_ADDRESS    0x1138018cUL

#define SMN_PCIE3_PCIE_SDP_CTRL_ADDRESS    0x1148018cUL


/***********************************************************
* Register Name : PCIE_STRAP_F0
* Register Description :
* Strap Function 0 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if Function 0 is enabled. Provides an override for STRAP_F0_EN.
#define PCIE_STRAP_F0_STRAP_F0_EN_OFFSET      0
#define PCIE_STRAP_F0_STRAP_F0_EN_MASK        0x1

// Bitfield Description : Controls if Function 0 is a Legacy Endpoint. Provides an override for STRAP_F0_LEGACY_DEVICE_TYPE_EN.
#define PCIE_STRAP_F0_STRAP_F0_LEGACY_DEVICE_TYPE_EN_OFFSET      1
#define PCIE_STRAP_F0_STRAP_F0_LEGACY_DEVICE_TYPE_EN_MASK        0x2

// Bitfield Description : Controls if Function 0 supports MSI. Provides an override for STRAP_F0_MSI_EN.
#define PCIE_STRAP_F0_STRAP_F0_MSI_EN_OFFSET      2
#define PCIE_STRAP_F0_STRAP_F0_MSI_EN_MASK        0x4

// Bitfield Description : Controls if Function 0 supports Virtual Channel capability. Provides an override for STRAP_F0_VC_EN.
#define PCIE_STRAP_F0_STRAP_F0_VC_EN_OFFSET      3
#define PCIE_STRAP_F0_STRAP_F0_VC_EN_MASK        0x8

// Bitfield Description : Controls if Function 0 supports Device Serial Number capability. Provides an override for STRAP_F0_DSN_EN.
#define PCIE_STRAP_F0_STRAP_F0_DSN_EN_OFFSET      4
#define PCIE_STRAP_F0_STRAP_F0_DSN_EN_MASK        0x10

// Bitfield Description : Controls if Function 0 supports Advanced Error Reporting capability. Provides an override for STRAP_F0_AER_EN.
#define PCIE_STRAP_F0_STRAP_F0_AER_EN_OFFSET      5
#define PCIE_STRAP_F0_STRAP_F0_AER_EN_MASK        0x20

// Bitfield Description : Controls if Function 0 supports Access Control Services capability. Provides an override for STRAP_F0_ACS_EN.
#define PCIE_STRAP_F0_STRAP_F0_ACS_EN_OFFSET      6
#define PCIE_STRAP_F0_STRAP_F0_ACS_EN_MASK        0x40

// Bitfield Description : Controls if Function 0 supports Resizable BAR capability. Provides an override for STRAP_F0_BAR_EN.
#define PCIE_STRAP_F0_STRAP_F0_BAR_EN_OFFSET      7
#define PCIE_STRAP_F0_STRAP_F0_BAR_EN_MASK        0x80

// Bitfield Description : Controls if Function 0 supports Power Budgeting capability. Provides an override for STRAP_F0_PWR_EN.
#define PCIE_STRAP_F0_STRAP_F0_PWR_EN_OFFSET      8
#define PCIE_STRAP_F0_STRAP_F0_PWR_EN_MASK        0x100

// Bitfield Description : Controls if Function 0 supports Dynamic Power Allocation capability. Provides an override for STRAP_F0_DPA_EN.
#define PCIE_STRAP_F0_STRAP_F0_DPA_EN_OFFSET      9
#define PCIE_STRAP_F0_STRAP_F0_DPA_EN_MASK        0x200

// Bitfield Description : Controls if Function 0 supports Address Translation Services capability. Provides an override for STRAP_F0_ATS_EN.
#define PCIE_STRAP_F0_STRAP_F0_ATS_EN_OFFSET      10
#define PCIE_STRAP_F0_STRAP_F0_ATS_EN_MASK        0x400

// Bitfield Description : Controls if Function 0 supports Page Request Interface capability. Provides an override for STRAP_F0_PAGE_REQ_EN.
#define PCIE_STRAP_F0_STRAP_F0_PAGE_REQ_EN_OFFSET      11
#define PCIE_STRAP_F0_STRAP_F0_PAGE_REQ_EN_MASK        0x800

// Bitfield Description : Controls if Function 0 supports PASID capability. Provides an override for STRAP_F0_PASID_EN.
#define PCIE_STRAP_F0_STRAP_F0_PASID_EN_OFFSET      12
#define PCIE_STRAP_F0_STRAP_F0_PASID_EN_MASK        0x1000

// Bitfield Description : Controls if Function 0 supports ECRC Checking capability. Provides an override for STRAP_F0_ECRC_CHECK_EN.
#define PCIE_STRAP_F0_STRAP_F0_ECRC_CHECK_EN_OFFSET      13
#define PCIE_STRAP_F0_STRAP_F0_ECRC_CHECK_EN_MASK        0x2000

// Bitfield Description : Controls if Function 0 supports ECRC Generation capability. Provides an override for STRAP_F0_ECRC_GEN_EN.
#define PCIE_STRAP_F0_STRAP_F0_ECRC_GEN_EN_OFFSET      14
#define PCIE_STRAP_F0_STRAP_F0_ECRC_GEN_EN_MASK        0x4000

// Bitfield Description : Controls if Function 0 supports Completer Abort Error Reporting capability. Provides an override for STRAP_F0_CPL_ABORT_ERR_EN.
#define PCIE_STRAP_F0_STRAP_F0_CPL_ABORT_ERR_EN_OFFSET      15
#define PCIE_STRAP_F0_STRAP_F0_CPL_ABORT_ERR_EN_MASK        0x8000

// Bitfield Description : Controls if Function 0 supports treating Poisoned TLP Received errors as Advisory Non-Fatal. Provides an override for STRAP_F0_POISONED_ADVISORY_NONFATAL.
#define PCIE_STRAP_F0_STRAP_F0_POISONED_ADVISORY_NONFATAL_OFFSET      16
#define PCIE_STRAP_F0_STRAP_F0_POISONED_ADVISORY_NONFATAL_MASK        0x10000

// Bitfield Description : Controls if Function 0 supports Multicast capability. Provides an override for STRAP_F0_MC_EN.
#define PCIE_STRAP_F0_STRAP_F0_MC_EN_OFFSET      17
#define PCIE_STRAP_F0_STRAP_F0_MC_EN_MASK        0x20000

// Bitfield Description : Controls if Function 0 supports 32-bit AtomicOp Completer capability. Provides an override for STRAP_F0_ATOMIC_EN.
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_EN_OFFSET      18
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_EN_MASK        0x40000

// Bitfield Description : Controls if Function 0 supports 64-bit AtomicOp Completer capability. Provides an override for STRAP_F0_ATOMIC_64BIT_EN.
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_64BIT_EN_OFFSET      19
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_64BIT_EN_MASK        0x80000

// Bitfield Description : Controls if Function 0 supports AtomicOp Routing capability. Provides an override for STRAP_F0_ATOMIC_ROUTING_EN.
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_ROUTING_EN_OFFSET      20
#define PCIE_STRAP_F0_STRAP_F0_ATOMIC_ROUTING_EN_MASK        0x100000

// Bitfield Description : Controls the number of MSI vectors that Function 0 supports. Provides an override for STRAP_F0_MSI_MULTI_MSG_CAP.
#define PCIE_STRAP_F0_STRAP_F0_MSI_MULTI_CAP_OFFSET      21
#define PCIE_STRAP_F0_STRAP_F0_MSI_MULTI_CAP_MASK        0xe00000

// Bitfield Description : Controls the number of MSI vectors that VFs associated with Function 0 support. Provides an override for STRAP_F0_MSI_MULTI_MSG_CAP for PF0's VFs.
#define PCIE_STRAP_F0_STRAP_F0_VFn_MSI_MULTI_CAP_OFFSET      24
#define PCIE_STRAP_F0_STRAP_F0_VFn_MSI_MULTI_CAP_MASK        0x7000000

// Bitfield Description : Controls if Function 0 supports MSI Per-vector Masking capability. Provides an override for STRAP_F0_MSI_PERVECTOR_MASK_CAP.
#define PCIE_STRAP_F0_STRAP_F0_MSI_PERVECTOR_MASK_CAP_OFFSET      27
#define PCIE_STRAP_F0_STRAP_F0_MSI_PERVECTOR_MASK_CAP_MASK        0x8000000

// Bitfield Description : Controls if Function 0 supports No RO-enabled PR-PR Passing capability. Provides an override for STRAP_F0_RO_ENABLED_PR_PR_PASSING.
#define PCIE_STRAP_F0_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_OFFSET      28
#define PCIE_STRAP_F0_STRAP_F0_NO_RO_ENABLED_P2P_PASSING_MASK        0x10000000

// Bitfield Description : Controls if Switch Upstream Port supports Alternate Routing-ID Interpretation capability. Provides an override for STRAP_BIF_ARI_EN.
#define PCIE_STRAP_F0_STRAP_SWUS_ARI_EN_OFFSET      29
#define PCIE_STRAP_F0_STRAP_SWUS_ARI_EN_MASK        0x20000000

// Bitfield Description : Controls if Function 0 supports SR-IOV capability. Provides an override for STRAP_BIF_SRIOV_EN.
#define PCIE_STRAP_F0_STRAP_F0_SRIOV_EN_OFFSET      30
#define PCIE_STRAP_F0_STRAP_F0_SRIOV_EN_MASK        0x40000000

// Bitfield Description : Controls if Function 0 supports MSI Mapping (HyperTransport) capability. Provides an override for STRAP_F0_MSI_MAP_EN.
#define PCIE_STRAP_F0_STRAP_F0_MSI_MAP_EN_OFFSET      31
#define PCIE_STRAP_F0_STRAP_F0_MSI_MAP_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_F0_EN:1;
    UINT32                            STRAP_F0_LEGACY_DEVICE_TYPE_EN:1;
    UINT32                            STRAP_F0_MSI_EN:1;
    UINT32                            STRAP_F0_VC_EN:1;
    UINT32                            STRAP_F0_DSN_EN:1;
    UINT32                            STRAP_F0_AER_EN:1;
    UINT32                            STRAP_F0_ACS_EN:1;
    UINT32                            STRAP_F0_BAR_EN:1;
    UINT32                            STRAP_F0_PWR_EN:1;
    UINT32                            STRAP_F0_DPA_EN:1;
    UINT32                            STRAP_F0_ATS_EN:1;
    UINT32                            STRAP_F0_PAGE_REQ_EN:1;
    UINT32                            STRAP_F0_PASID_EN:1;
    UINT32                            STRAP_F0_ECRC_CHECK_EN:1;
    UINT32                            STRAP_F0_ECRC_GEN_EN:1;
    UINT32                            STRAP_F0_CPL_ABORT_ERR_EN:1;
    UINT32                            STRAP_F0_POISONED_ADVISORY_NONFATAL:1;
    UINT32                            STRAP_F0_MC_EN:1;
    UINT32                            STRAP_F0_ATOMIC_EN:1;
    UINT32                            STRAP_F0_ATOMIC_64BIT_EN:1;
    UINT32                            STRAP_F0_ATOMIC_ROUTING_EN:1;
    UINT32                            STRAP_F0_MSI_MULTI_CAP:3;
    UINT32                            STRAP_F0_VFn_MSI_MULTI_CAP:3;
    UINT32                            STRAP_F0_MSI_PERVECTOR_MASK_CAP:1;
    UINT32                            STRAP_F0_NO_RO_ENABLED_P2P_PASSING:1;
    UINT32                            STRAP_SWUS_ARI_EN:1;
    UINT32                            STRAP_F0_SRIOV_EN:1;
    UINT32                            STRAP_F0_MSI_MAP_EN:1;
  } Field;
  UINT32 Value;
} PCIE_STRAP_F0_STRUCT;

#define SMN_PCIE0_PCIE_STRAP_F0_ADDRESS    0x111802c0UL

#define SMN_PCIE1_PCIE_STRAP_F0_ADDRESS    0x112802c0UL

#define SMN_PCIE2_PCIE_STRAP_F0_ADDRESS    0x113802c0UL

#define SMN_PCIE3_PCIE_STRAP_F0_ADDRESS    0x114802c0UL


//PcieCoreInit.h
//PCIEPORT.h
/***********************************************************
* Register Name : PCIEP_HW_DEBUG
* Register Description :
* Hardware Debug register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Bit 0.
#define PCIEP_HW_DEBUG_HW_00_DEBUG_OFFSET      0
#define PCIEP_HW_DEBUG_HW_00_DEBUG_MASK        0x1

// Bitfield Description : Bit 1.
#define PCIEP_HW_DEBUG_HW_01_DEBUG_OFFSET      1
#define PCIEP_HW_DEBUG_HW_01_DEBUG_MASK        0x2

// Bitfield Description : Bit 2.
#define PCIEP_HW_DEBUG_HW_02_DEBUG_OFFSET      2
#define PCIEP_HW_DEBUG_HW_02_DEBUG_MASK        0x4

// Bitfield Description : Bit 3.
#define PCIEP_HW_DEBUG_HW_03_DEBUG_OFFSET      3
#define PCIEP_HW_DEBUG_HW_03_DEBUG_MASK        0x8

// Bitfield Description : Bit 4.
#define PCIEP_HW_DEBUG_HW_04_DEBUG_OFFSET      4
#define PCIEP_HW_DEBUG_HW_04_DEBUG_MASK        0x10

// Bitfield Description : Bit 5.
#define PCIEP_HW_DEBUG_HW_05_DEBUG_OFFSET      5
#define PCIEP_HW_DEBUG_HW_05_DEBUG_MASK        0x20

// Bitfield Description : Bit 6.
#define PCIEP_HW_DEBUG_HW_06_DEBUG_OFFSET      6
#define PCIEP_HW_DEBUG_HW_06_DEBUG_MASK        0x40

// Bitfield Description : Bit 7.
#define PCIEP_HW_DEBUG_HW_07_DEBUG_OFFSET      7
#define PCIEP_HW_DEBUG_HW_07_DEBUG_MASK        0x80

// Bitfield Description : Bit 8.
#define PCIEP_HW_DEBUG_HW_08_DEBUG_OFFSET      8
#define PCIEP_HW_DEBUG_HW_08_DEBUG_MASK        0x100

// Bitfield Description : Bit 9.
#define PCIEP_HW_DEBUG_HW_09_DEBUG_OFFSET      9
#define PCIEP_HW_DEBUG_HW_09_DEBUG_MASK        0x200

// Bitfield Description : Bit 10.
#define PCIEP_HW_DEBUG_HW_10_DEBUG_OFFSET      10
#define PCIEP_HW_DEBUG_HW_10_DEBUG_MASK        0x400

// Bitfield Description : Bit 11.
#define PCIEP_HW_DEBUG_HW_11_DEBUG_OFFSET      11
#define PCIEP_HW_DEBUG_HW_11_DEBUG_MASK        0x800

// Bitfield Description : Bit 12.
#define PCIEP_HW_DEBUG_HW_12_DEBUG_OFFSET      12
#define PCIEP_HW_DEBUG_HW_12_DEBUG_MASK        0x1000

// Bitfield Description : Bit 13.
#define PCIEP_HW_DEBUG_HW_13_DEBUG_OFFSET      13
#define PCIEP_HW_DEBUG_HW_13_DEBUG_MASK        0x2000

// Bitfield Description : Bit 14.
#define PCIEP_HW_DEBUG_HW_14_DEBUG_OFFSET      14
#define PCIEP_HW_DEBUG_HW_14_DEBUG_MASK        0x4000

// Bitfield Description : Bit 15.
#define PCIEP_HW_DEBUG_HW_15_DEBUG_OFFSET      15
#define PCIEP_HW_DEBUG_HW_15_DEBUG_MASK        0x8000

// Bitfield Description : Bit 16.
#define PCIEP_HW_DEBUG_HW_16_DEBUG_OFFSET      16
#define PCIEP_HW_DEBUG_HW_16_DEBUG_MASK        0x10000

// Bitfield Description : Bit 17.
#define PCIEP_HW_DEBUG_HW_17_DEBUG_OFFSET      17
#define PCIEP_HW_DEBUG_HW_17_DEBUG_MASK        0x20000

// Bitfield Description : Bit 18.
#define PCIEP_HW_DEBUG_HW_18_DEBUG_OFFSET      18
#define PCIEP_HW_DEBUG_HW_18_DEBUG_MASK        0x40000

// Bitfield Description : Bit 19.
#define PCIEP_HW_DEBUG_HW_19_DEBUG_OFFSET      19
#define PCIEP_HW_DEBUG_HW_19_DEBUG_MASK        0x80000

// Bitfield Description : Bit 20.
#define PCIEP_HW_DEBUG_HW_20_DEBUG_OFFSET      20
#define PCIEP_HW_DEBUG_HW_20_DEBUG_MASK        0x100000

// Bitfield Description : Bit 21.
#define PCIEP_HW_DEBUG_HW_21_DEBUG_OFFSET      21
#define PCIEP_HW_DEBUG_HW_21_DEBUG_MASK        0x200000

// Bitfield Description : Bit 22.
#define PCIEP_HW_DEBUG_HW_22_DEBUG_OFFSET      22
#define PCIEP_HW_DEBUG_HW_22_DEBUG_MASK        0x400000

// Bitfield Description : Bit 23.
#define PCIEP_HW_DEBUG_HW_23_DEBUG_OFFSET      23
#define PCIEP_HW_DEBUG_HW_23_DEBUG_MASK        0x800000

// Bitfield Description : Bit 24.
#define PCIEP_HW_DEBUG_HW_24_DEBUG_OFFSET      24
#define PCIEP_HW_DEBUG_HW_24_DEBUG_MASK        0x1000000

// Bitfield Description : Bit 25.
#define PCIEP_HW_DEBUG_HW_25_DEBUG_OFFSET      25
#define PCIEP_HW_DEBUG_HW_25_DEBUG_MASK        0x2000000

// Bitfield Description : Bit 26.
#define PCIEP_HW_DEBUG_HW_26_DEBUG_OFFSET      26
#define PCIEP_HW_DEBUG_HW_26_DEBUG_MASK        0x4000000

// Bitfield Description : Bit 27.
#define PCIEP_HW_DEBUG_HW_27_DEBUG_OFFSET      27
#define PCIEP_HW_DEBUG_HW_27_DEBUG_MASK        0x8000000

// Bitfield Description : Bit 28.
#define PCIEP_HW_DEBUG_HW_28_DEBUG_OFFSET      28
#define PCIEP_HW_DEBUG_HW_28_DEBUG_MASK        0x10000000

// Bitfield Description : Bit 29.
#define PCIEP_HW_DEBUG_HW_29_DEBUG_OFFSET      29
#define PCIEP_HW_DEBUG_HW_29_DEBUG_MASK        0x20000000

// Bitfield Description : Bit 30.
#define PCIEP_HW_DEBUG_HW_30_DEBUG_OFFSET      30
#define PCIEP_HW_DEBUG_HW_30_DEBUG_MASK        0x40000000

// Bitfield Description : Bit 31.
#define PCIEP_HW_DEBUG_HW_31_DEBUG_OFFSET      31
#define PCIEP_HW_DEBUG_HW_31_DEBUG_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HW_00_DEBUG:1;
    UINT32                            HW_01_DEBUG:1;
    UINT32                            HW_02_DEBUG:1;
    UINT32                            HW_03_DEBUG:1;
    UINT32                            HW_04_DEBUG:1;
    UINT32                            HW_05_DEBUG:1;
    UINT32                            HW_06_DEBUG:1;
    UINT32                            HW_07_DEBUG:1;
    UINT32                            HW_08_DEBUG:1;
    UINT32                            HW_09_DEBUG:1;
    UINT32                            HW_10_DEBUG:1;
    UINT32                            HW_11_DEBUG:1;
    UINT32                            HW_12_DEBUG:1;
    UINT32                            HW_13_DEBUG:1;
    UINT32                            HW_14_DEBUG:1;
    UINT32                            HW_15_DEBUG:1;
    UINT32                            HW_16_DEBUG:1;
    UINT32                            HW_17_DEBUG:1;
    UINT32                            HW_18_DEBUG:1;
    UINT32                            HW_19_DEBUG:1;
    UINT32                            HW_20_DEBUG:1;
    UINT32                            HW_21_DEBUG:1;
    UINT32                            HW_22_DEBUG:1;
    UINT32                            HW_23_DEBUG:1;
    UINT32                            HW_24_DEBUG:1;
    UINT32                            HW_25_DEBUG:1;
    UINT32                            HW_26_DEBUG:1;
    UINT32                            HW_27_DEBUG:1;
    UINT32                            HW_28_DEBUG:1;
    UINT32                            HW_29_DEBUG:1;
    UINT32                            HW_30_DEBUG:1;
    UINT32                            HW_31_DEBUG:1;
  } Field;
  UINT32 Value;
} PCIEP_HW_DEBUG_PCIEPORT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_OFFSET       0x8
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x8)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_OFFSET       0x8
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x8)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_OFFSET       0x8
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x8)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_OFFSET       0x8
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x8)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_OFFSET       0x8
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x8)

#define SMN_FUNC0_PCIE0_PCIEP_HW_DEBUG_ADDRESS    0x11140008UL

#define SMN_FUNC0_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11240008UL

#define SMN_FUNC0_PCIE2_PCIEP_HW_DEBUG_ADDRESS    0x11340008UL

#define SMN_FUNC0_PCIE3_PCIEP_HW_DEBUG_ADDRESS    0x11440008UL

#define SMN_FUNC1_PCIE0_PCIEP_HW_DEBUG_ADDRESS    0x11141008UL

#define SMN_FUNC1_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11241008UL

#define SMN_FUNC2_PCIE0_PCIEP_HW_DEBUG_ADDRESS    0x11142008UL

#define SMN_FUNC2_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11242008UL

#define SMN_FUNC3_PCIE0_PCIEP_HW_DEBUG_ADDRESS    0x11143008UL

#define SMN_FUNC3_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11243008UL

#define SMN_FUNC4_PCIE0_PCIEP_HW_DEBUG_ADDRESS    0x11144008UL

#define SMN_FUNC4_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11244008UL

#define SMN_FUNC5_PCIE1_PCIEP_HW_DEBUG_ADDRESS    0x11245008UL


/***********************************************************
* Register Name : PCIEP_HW_DEBUG_LC
* Register Description :
* Hardware Debug LC register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : REGS_LC_CCIX_ESM0_16GT_SKP_SEL: Transmit control SKP ordered sets according to PCIe 16.0 GT/s requirements when CCIX ESM Rate0 is operating at 16.0 GT/s rate.
#define PCIEP_HW_DEBUG_LC_HW_00_DEBUG_LC_OFFSET      0
#define PCIEP_HW_DEBUG_LC_HW_00_DEBUG_LC_MASK        0x1

// Bitfield Description : Bit 1.
#define PCIEP_HW_DEBUG_LC_HW_01_DEBUG_LC_OFFSET      1
#define PCIEP_HW_DEBUG_LC_HW_01_DEBUG_LC_MASK        0x2

// Bitfield Description : Bit 2.
#define PCIEP_HW_DEBUG_LC_HW_02_DEBUG_LC_OFFSET      2
#define PCIEP_HW_DEBUG_LC_HW_02_DEBUG_LC_MASK        0x4

// Bitfield Description : Bit 3.
#define PCIEP_HW_DEBUG_LC_HW_03_DEBUG_LC_OFFSET      3
#define PCIEP_HW_DEBUG_LC_HW_03_DEBUG_LC_MASK        0x8

// Bitfield Description : Bit 4.
#define PCIEP_HW_DEBUG_LC_HW_04_DEBUG_LC_OFFSET      4
#define PCIEP_HW_DEBUG_LC_HW_04_DEBUG_LC_MASK        0x10

// Bitfield Description : Bit 5.
#define PCIEP_HW_DEBUG_LC_HW_05_DEBUG_LC_OFFSET      5
#define PCIEP_HW_DEBUG_LC_HW_05_DEBUG_LC_MASK        0x20

// Bitfield Description : Bit 6.
#define PCIEP_HW_DEBUG_LC_HW_06_DEBUG_LC_OFFSET      6
#define PCIEP_HW_DEBUG_LC_HW_06_DEBUG_LC_MASK        0x40

// Bitfield Description : Bit 7.
#define PCIEP_HW_DEBUG_LC_HW_07_DEBUG_LC_OFFSET      7
#define PCIEP_HW_DEBUG_LC_HW_07_DEBUG_LC_MASK        0x80

// Bitfield Description : Bit 8.
#define PCIEP_HW_DEBUG_LC_HW_08_DEBUG_LC_OFFSET      8
#define PCIEP_HW_DEBUG_LC_HW_08_DEBUG_LC_MASK        0x100

// Bitfield Description : Bit 9.
#define PCIEP_HW_DEBUG_LC_HW_09_DEBUG_LC_OFFSET      9
#define PCIEP_HW_DEBUG_LC_HW_09_DEBUG_LC_MASK        0x200

// Bitfield Description : Bit 10.
#define PCIEP_HW_DEBUG_LC_HW_10_DEBUG_LC_OFFSET      10
#define PCIEP_HW_DEBUG_LC_HW_10_DEBUG_LC_MASK        0x400

// Bitfield Description : Bit 11.
#define PCIEP_HW_DEBUG_LC_HW_11_DEBUG_LC_OFFSET      11
#define PCIEP_HW_DEBUG_LC_HW_11_DEBUG_LC_MASK        0x800

// Bitfield Description : Bit 12.
#define PCIEP_HW_DEBUG_LC_HW_12_DEBUG_LC_OFFSET      12
#define PCIEP_HW_DEBUG_LC_HW_12_DEBUG_LC_MASK        0x1000

// Bitfield Description : Bit 13.
#define PCIEP_HW_DEBUG_LC_HW_13_DEBUG_LC_OFFSET      13
#define PCIEP_HW_DEBUG_LC_HW_13_DEBUG_LC_MASK        0x2000

// Bitfield Description : Bit 14.
#define PCIEP_HW_DEBUG_LC_HW_14_DEBUG_LC_OFFSET      14
#define PCIEP_HW_DEBUG_LC_HW_14_DEBUG_LC_MASK        0x4000

// Bitfield Description : Bit 15.
#define PCIEP_HW_DEBUG_LC_HW_15_DEBUG_LC_OFFSET      15
#define PCIEP_HW_DEBUG_LC_HW_15_DEBUG_LC_MASK        0x8000

// Bitfield Description : Bit 16.
#define PCIEP_HW_DEBUG_LC_HW_16_DEBUG_LC_OFFSET      16
#define PCIEP_HW_DEBUG_LC_HW_16_DEBUG_LC_MASK        0x10000

// Bitfield Description : Bit 17.
#define PCIEP_HW_DEBUG_LC_HW_17_DEBUG_LC_OFFSET      17
#define PCIEP_HW_DEBUG_LC_HW_17_DEBUG_LC_MASK        0x20000

// Bitfield Description : Bit 18.
#define PCIEP_HW_DEBUG_LC_HW_18_DEBUG_LC_OFFSET      18
#define PCIEP_HW_DEBUG_LC_HW_18_DEBUG_LC_MASK        0x40000

// Bitfield Description : Bit 19.
#define PCIEP_HW_DEBUG_LC_HW_19_DEBUG_LC_OFFSET      19
#define PCIEP_HW_DEBUG_LC_HW_19_DEBUG_LC_MASK        0x80000

// Bitfield Description : Bit 20.
#define PCIEP_HW_DEBUG_LC_HW_20_DEBUG_LC_OFFSET      20
#define PCIEP_HW_DEBUG_LC_HW_20_DEBUG_LC_MASK        0x100000

// Bitfield Description : Bit 21.
#define PCIEP_HW_DEBUG_LC_HW_21_DEBUG_LC_OFFSET      21
#define PCIEP_HW_DEBUG_LC_HW_21_DEBUG_LC_MASK        0x200000

// Bitfield Description : Bit 22.
#define PCIEP_HW_DEBUG_LC_HW_22_DEBUG_LC_OFFSET      22
#define PCIEP_HW_DEBUG_LC_HW_22_DEBUG_LC_MASK        0x400000

// Bitfield Description : Bit 23.
#define PCIEP_HW_DEBUG_LC_HW_23_DEBUG_LC_OFFSET      23
#define PCIEP_HW_DEBUG_LC_HW_23_DEBUG_LC_MASK        0x800000

// Bitfield Description : Bit 24.
#define PCIEP_HW_DEBUG_LC_HW_24_DEBUG_LC_OFFSET      24
#define PCIEP_HW_DEBUG_LC_HW_24_DEBUG_LC_MASK        0x1000000

// Bitfield Description : Bit 25.
#define PCIEP_HW_DEBUG_LC_HW_25_DEBUG_LC_OFFSET      25
#define PCIEP_HW_DEBUG_LC_HW_25_DEBUG_LC_MASK        0x2000000

// Bitfield Description : Bit 26.
#define PCIEP_HW_DEBUG_LC_HW_26_DEBUG_LC_OFFSET      26
#define PCIEP_HW_DEBUG_LC_HW_26_DEBUG_LC_MASK        0x4000000

// Bitfield Description : Bit 27.
#define PCIEP_HW_DEBUG_LC_HW_27_DEBUG_LC_OFFSET      27
#define PCIEP_HW_DEBUG_LC_HW_27_DEBUG_LC_MASK        0x8000000

// Bitfield Description : Bit 28.
#define PCIEP_HW_DEBUG_LC_HW_28_DEBUG_LC_OFFSET      28
#define PCIEP_HW_DEBUG_LC_HW_28_DEBUG_LC_MASK        0x10000000

// Bitfield Description : Bit 29.
#define PCIEP_HW_DEBUG_LC_HW_29_DEBUG_LC_OFFSET      29
#define PCIEP_HW_DEBUG_LC_HW_29_DEBUG_LC_MASK        0x20000000

// Bitfield Description : Bit 30.
#define PCIEP_HW_DEBUG_LC_HW_30_DEBUG_LC_OFFSET      30
#define PCIEP_HW_DEBUG_LC_HW_30_DEBUG_LC_MASK        0x40000000

// Bitfield Description : Bit 31.
#define PCIEP_HW_DEBUG_LC_HW_31_DEBUG_LC_OFFSET      31
#define PCIEP_HW_DEBUG_LC_HW_31_DEBUG_LC_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HW_00_DEBUG_LC:1;
    UINT32                            HW_01_DEBUG_LC:1;
    UINT32                            HW_02_DEBUG_LC:1;
    UINT32                            HW_03_DEBUG_LC:1;
    UINT32                            HW_04_DEBUG_LC:1;
    UINT32                            HW_05_DEBUG_LC:1;
    UINT32                            HW_06_DEBUG_LC:1;
    UINT32                            HW_07_DEBUG_LC:1;
    UINT32                            HW_08_DEBUG_LC:1;
    UINT32                            HW_09_DEBUG_LC:1;
    UINT32                            HW_10_DEBUG_LC:1;
    UINT32                            HW_11_DEBUG_LC:1;
    UINT32                            HW_12_DEBUG_LC:1;
    UINT32                            HW_13_DEBUG_LC:1;
    UINT32                            HW_14_DEBUG_LC:1;
    UINT32                            HW_15_DEBUG_LC:1;
    UINT32                            HW_16_DEBUG_LC:1;
    UINT32                            HW_17_DEBUG_LC:1;
    UINT32                            HW_18_DEBUG_LC:1;
    UINT32                            HW_19_DEBUG_LC:1;
    UINT32                            HW_20_DEBUG_LC:1;
    UINT32                            HW_21_DEBUG_LC:1;
    UINT32                            HW_22_DEBUG_LC:1;
    UINT32                            HW_23_DEBUG_LC:1;
    UINT32                            HW_24_DEBUG_LC:1;
    UINT32                            HW_25_DEBUG_LC:1;
    UINT32                            HW_26_DEBUG_LC:1;
    UINT32                            HW_27_DEBUG_LC:1;
    UINT32                            HW_28_DEBUG_LC:1;
    UINT32                            HW_29_DEBUG_LC:1;
    UINT32                            HW_30_DEBUG_LC:1;
    UINT32                            HW_31_DEBUG_LC:1;
  } Field;
  UINT32 Value;
} PCIEP_HW_DEBUG_LC_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_LC_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_LC_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_LC_OFFSET       0xc
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0xc)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_LC_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_LC_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_LC_OFFSET       0xc
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0xc)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_LC_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_LC_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_LC_OFFSET       0xc
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0xc)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_LC_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_LC_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_LC_OFFSET       0xc
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0xc)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_LC_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_LC_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_LC_OFFSET       0xc
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0xc)

#define SMN_FUNC0_PCIE0_PCIEP_HW_DEBUG_LC_ADDRESS    0x1114000cUL

#define SMN_FUNC0_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124000cUL

#define SMN_FUNC0_PCIE2_PCIEP_HW_DEBUG_LC_ADDRESS    0x1134000cUL

#define SMN_FUNC0_PCIE3_PCIEP_HW_DEBUG_LC_ADDRESS    0x1144000cUL

#define SMN_FUNC1_PCIE0_PCIEP_HW_DEBUG_LC_ADDRESS    0x1114100cUL

#define SMN_FUNC1_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124100cUL

#define SMN_FUNC2_PCIE0_PCIEP_HW_DEBUG_LC_ADDRESS    0x1114200cUL

#define SMN_FUNC2_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124200cUL

#define SMN_FUNC3_PCIE0_PCIEP_HW_DEBUG_LC_ADDRESS    0x1114300cUL

#define SMN_FUNC3_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124300cUL

#define SMN_FUNC4_PCIE0_PCIEP_HW_DEBUG_LC_ADDRESS    0x1114400cUL

#define SMN_FUNC4_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124400cUL

#define SMN_FUNC5_PCIE1_PCIEP_HW_DEBUG_LC_ADDRESS    0x1124500cUL

/***********************************************************
* Register Name : PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES
* Register Description :
* Fine Grain Clock Gating Override register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if the enable term for the output of transmit_mux is set to prevent clock gating.
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_TRANSMIT_MUX_OUTPUT_GATING_OFFSET      0
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_TRANSMIT_MUX_OUTPUT_GATING_MASK        0x1

// Bitfield Description : Controls if the enable term for the output of symbol_mux is set to prevent clock gating.
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_SYMBOL_MUX_OUTPUT_GATING_OFFSET      1
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_SYMBOL_MUX_OUTPUT_GATING_MASK        0x2

// Bitfield Description : Controls if the enable term for the output of lc_pkt_gen is set to prevent dynamic clock gating.
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_LC_PKT_GEN_DYN_CLK_GATING_OFFSET      2
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_LC_PKT_GEN_DYN_CLK_GATING_MASK        0x4

// Bitfield Description : Controls if the enable term for the output of transmit_mux is set to prevent dynamic clock gating.
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_TRANSMIT_MUX_DYN_CLK_GATING_OFFSET      3
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_TRANSMIT_MUX_DYN_CLK_GATING_MASK        0x8

// Bitfield Description : Controls if the enable term output for the LTSSM is set to prevent dynamic clock gating.
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_LTSSM_DYN_CLK_GATING_OFFSET      4
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_LC_DISABLE_LTSSM_DYN_CLK_GATING_MASK        0x10

// Bitfield Description :
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_Reserved_31_5_OFFSET      5
#define PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            LC_DISABLE_TRANSMIT_MUX_OUTPUT_GATING:1;
    UINT32                            LC_DISABLE_SYMBOL_MUX_OUTPUT_GATING:1;
    UINT32                            LC_DISABLE_LC_PKT_GEN_DYN_CLK_GATING:1;
    UINT32                            LC_DISABLE_TRANSMIT_MUX_DYN_CLK_GATING:1;
    UINT32                            LC_DISABLE_LTSSM_DYN_CLK_GATING:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_OFFSET       0x388
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x388)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_OFFSET       0x388
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x388)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_OFFSET       0x388
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x388)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_OFFSET       0x388
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x388)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_OFFSET       0x388
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x388)

#define SMN_FUNC0_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11140388UL

#define SMN_FUNC0_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11240388UL

#define SMN_FUNC0_PCIE2_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11340388UL

#define SMN_FUNC0_PCIE3_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11440388UL

#define SMN_FUNC1_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11141388UL

#define SMN_FUNC1_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11241388UL

#define SMN_FUNC2_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11142388UL

#define SMN_FUNC2_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11242388UL

#define SMN_FUNC3_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11143388UL

#define SMN_FUNC3_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11243388UL

#define SMN_FUNC4_PCIE0_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11144388UL

#define SMN_FUNC4_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11244388UL

#define SMN_FUNC5_PCIE1_PCIE_LC_FINE_GRAIN_CLK_GATE_OVERRIDES_ADDRESS    0x11245388UL

/***********************************************************
* Register Name : PCIEP_PORT_CNTL
* Register Description :
* Port Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if Port is allowed to suspend all slave requests to client.
#define PCIEP_PORT_CNTL_SLV_PORT_REQ_EN_OFFSET      0
#define PCIEP_PORT_CNTL_SLV_PORT_REQ_EN_MASK        0x1

// Bitfield Description : Controls forcing all slave requests to be snoop requests. When set it clears No Snoop bit in attributes on received Request TLPs. TLP is passed to Slave Client with Attr[0]=0. When error occurs, AER Header Log, may report Attr[0]=0. Completion sent back on PCIE client has the original Attr[0] (not affected by override).
#define PCIEP_PORT_CNTL_CI_SNOOP_OVERRIDE_OFFSET      1
#define PCIEP_PORT_CNTL_CI_SNOOP_OVERRIDE_MASK        0x2

// Bitfield Description : Controls legacy PME (hot-plug) messages (BIF_CHIP_pme) if PMI_STATUS_CNTL:PME_EN is also set.
#define PCIEP_PORT_CNTL_HOTPLUG_MSG_EN_OFFSET      2
#define PCIEP_PORT_CNTL_HOTPLUG_MSG_EN_MASK        0x4

// Bitfield Description : Controls native PME.
#define PCIEP_PORT_CNTL_NATIVE_PME_EN_OFFSET      3
#define PCIEP_PORT_CNTL_NATIVE_PME_EN_MASK        0x8

// Bitfield Description : Controls power fault detection.
#define PCIEP_PORT_CNTL_PWR_FAULT_EN_OFFSET      4
#define PCIEP_PORT_CNTL_PWR_FAULT_EN_MASK        0x10

// Bitfield Description : Controls bus master for power saving state.
#define PCIEP_PORT_CNTL_PMI_BM_DIS_OFFSET      5
#define PCIEP_PORT_CNTL_PMI_BM_DIS_MASK        0x20

// Bitfield Description : REGS_PME_EN_HW_DEBUG: XORed with PMI_STATUS_CNTL:PME_EN to override asserting legacy PME messages (BIF_CHIP_pme).
#define PCIEP_PORT_CNTL_PME_EN_HW_DEBUG_OFFSET      6
#define PCIEP_PORT_CNTL_PME_EN_HW_DEBUG_MASK        0x40

// Bitfield Description : REGS_PME_MODE_HW_DEBUG: Disable asserting legacy hot-plug messages (BIF_CHIP_hpme).
#define PCIEP_PORT_CNTL_PME_MODE_HW_DEBUG_OFFSET      7
#define PCIEP_PORT_CNTL_PME_MODE_HW_DEBUG_MASK        0x80

// Bitfield Description : When PCIE_CI_CNTL.CI_SLV_CPL_ALLOC_MODE bit is 1, controls the limit for outstanding slave snooped non-posted requests to slave. A value of 0 indicates 128 requests.
#define PCIEP_PORT_CNTL_CI_SLV_CPL_STATIC_ALLOC_LIMIT_S_OFFSET      8
#define PCIEP_PORT_CNTL_CI_SLV_CPL_STATIC_ALLOC_LIMIT_S_MASK        0x3ff00

// Bitfield Description : When CI_MAX_CPL_PAYLOAD_SIZE_MODE field is 1 or 2, contains the private setting used for maximum payload size value for slave completions.
#define PCIEP_PORT_CNTL_CI_PRIV_MAX_CPL_PAYLOAD_SIZE_OFFSET      18
#define PCIEP_PORT_CNTL_CI_PRIV_MAX_CPL_PAYLOAD_SIZE_MASK        0x1c0000

// Bitfield Description :
#define PCIEP_PORT_CNTL_Reserved_23_21_OFFSET      21
#define PCIEP_PORT_CNTL_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Controls how the Port handles a Poisoned TLP Received Error.
#define PCIEP_PORT_CNTL_CI_SLV_RSP_POISONED_UR_MODE_OFFSET      24
#define PCIEP_PORT_CNTL_CI_SLV_RSP_POISONED_UR_MODE_MASK        0x3000000

// Bitfield Description : Controls which maximum payload size value is used for slave completions.
#define PCIEP_PORT_CNTL_CI_MAX_CPL_PAYLOAD_SIZE_MODE_OFFSET      26
#define PCIEP_PORT_CNTL_CI_MAX_CPL_PAYLOAD_SIZE_MODE_MASK        0xc000000

// Bitfield Description :
#define PCIEP_PORT_CNTL_Reserved_31_28_OFFSET      28
#define PCIEP_PORT_CNTL_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            SLV_PORT_REQ_EN:1;
    UINT32                            CI_SNOOP_OVERRIDE:1;
    UINT32                            HOTPLUG_MSG_EN:1;
    UINT32                            NATIVE_PME_EN:1;
    UINT32                            PWR_FAULT_EN:1;
    UINT32                            PMI_BM_DIS:1;
    UINT32                            PME_EN_HW_DEBUG:1;
    UINT32                            PME_MODE_HW_DEBUG:1;
    UINT32                            CI_SLV_CPL_STATIC_ALLOC_LIMIT_S:10;
    UINT32                            CI_PRIV_MAX_CPL_PAYLOAD_SIZE:3;
    UINT32                            Reserved_23_21:3;
    UINT32                            CI_SLV_RSP_POISONED_UR_MODE:2;
    UINT32                            CI_MAX_CPL_PAYLOAD_SIZE_MODE:2;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} PCIEP_PORT_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_PORT_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_PORT_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_PORT_CNTL_OFFSET       0x40
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_PORT_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x40)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_PORT_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_PORT_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_PORT_CNTL_OFFSET       0x40
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_PORT_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x40)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_PORT_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_PORT_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_PORT_CNTL_OFFSET       0x40
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_PORT_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x40)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_PORT_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_PORT_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_PORT_CNTL_OFFSET       0x40
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_PORT_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x40)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_PORT_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_PORT_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_PORT_CNTL_OFFSET       0x40
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_PORT_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x40)

#define SMN_FUNC0_PCIE0_PCIEP_PORT_CNTL_ADDRESS    0x11140040UL

#define SMN_FUNC0_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11240040UL

#define SMN_FUNC0_PCIE2_PCIEP_PORT_CNTL_ADDRESS    0x11340040UL

#define SMN_FUNC0_PCIE3_PCIEP_PORT_CNTL_ADDRESS    0x11440040UL

#define SMN_FUNC1_PCIE0_PCIEP_PORT_CNTL_ADDRESS    0x11141040UL

#define SMN_FUNC1_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11241040UL

#define SMN_FUNC2_PCIE0_PCIEP_PORT_CNTL_ADDRESS    0x11142040UL

#define SMN_FUNC2_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11242040UL

#define SMN_FUNC3_PCIE0_PCIEP_PORT_CNTL_ADDRESS    0x11143040UL

#define SMN_FUNC3_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11243040UL

#define SMN_FUNC4_PCIE0_PCIEP_PORT_CNTL_ADDRESS    0x11144040UL

#define SMN_FUNC4_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11244040UL

#define SMN_FUNC5_PCIE1_PCIEP_PORT_CNTL_ADDRESS    0x11245040UL

/***********************************************************
* Register Name : PCIEP_STRAP_LC
* Register Description :
* Link Controller Strap register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls FTS TSx count. Provides an override value for STRAP_FTS_yTSx_COUNT.
#define PCIEP_STRAP_LC_STRAP_FTS_yTSx_COUNT_OFFSET      0
#define PCIEP_STRAP_LC_STRAP_FTS_yTSx_COUNT_MASK        0x3

// Bitfield Description : Controls long TSx count. Provides an override value for STRAP_LONG_yTSx_COUNT.
#define PCIEP_STRAP_LC_STRAP_LONG_yTSx_COUNT_OFFSET      2
#define PCIEP_STRAP_LC_STRAP_LONG_yTSx_COUNT_MASK        0xc

// Bitfield Description : Controls medium TSx count. Provides an override value for STRAP_MED_yTSx_COUNT.
#define PCIEP_STRAP_LC_STRAP_MED_yTSx_COUNT_OFFSET      4
#define PCIEP_STRAP_LC_STRAP_MED_yTSx_COUNT_MASK        0x30

// Bitfield Description : Controls short TSx count. Provides an override value for STRAP_SHORT_yTSx_COUNT.
#define PCIEP_STRAP_LC_STRAP_SHORT_yTSx_COUNT_OFFSET      6
#define PCIEP_STRAP_LC_STRAP_SHORT_yTSx_COUNT_MASK        0xc0

// Bitfield Description : Controls SKP generation interval. Provides an override value for STRAP_SKIP_INTERVAL. The values are defined for: SRNS (2.5 GT/s or 5.0 GT/s); SRIS (2.5 GT/s or 5.0 GT/s); SRNS (8.0 GT/s or higher); SRIS (8.0 GT/s or higher).
#define PCIEP_STRAP_LC_STRAP_SKIP_INTERVAL_OFFSET      8
#define PCIEP_STRAP_LC_STRAP_SKIP_INTERVAL_MASK        0x700

// Bitfield Description : Controls if receiver detection is bypassed. Provides an override value for STRAP_BYPASS_RCVR_DET.
#define PCIEP_STRAP_LC_STRAP_BYPASS_RCVR_DET_OFFSET      11
#define PCIEP_STRAP_LC_STRAP_BYPASS_RCVR_DET_MASK        0x800

// Bitfield Description : Controls if Compliance mode is disabled. Provides an override value for STRAP_COMPLIANCE_DIS.
#define PCIEP_STRAP_LC_STRAP_COMPLIANCE_DIS_OFFSET      12
#define PCIEP_STRAP_LC_STRAP_COMPLIANCE_DIS_MASK        0x1000

// Bitfield Description : Controls if Compliance mode is forced. Provides an override value for STRAP_FORCE_COMPLIANCE.
#define PCIEP_STRAP_LC_STRAP_FORCE_COMPLIANCE_OFFSET      13
#define PCIEP_STRAP_LC_STRAP_FORCE_COMPLIANCE_MASK        0x2000

// Bitfield Description : Controls lane ordering. Provides an override value for STRAP_REVERSE_LC_LANES.
#define PCIEP_STRAP_LC_STRAP_REVERSE_LC_LANES_OFFSET      14
#define PCIEP_STRAP_LC_STRAP_REVERSE_LC_LANES_MASK        0x4000

// Bitfield Description : Controls if automatic link speed negotiation is supported. Provides an override value for STRAP_AUTO_RC_SPEED_NEGOTIATION_DIS.
#define PCIEP_STRAP_LC_STRAP_AUTO_RC_SPEED_NEGOTIATION_DIS_OFFSET      15
#define PCIEP_STRAP_LC_STRAP_AUTO_RC_SPEED_NEGOTIATION_DIS_MASK        0x8000

// Bitfield Description : Controls link negotiation mode. Provides an override value for STRAP_LANE_NEGOTIATION.
#define PCIEP_STRAP_LC_STRAP_LANE_NEGOTIATION_OFFSET      16
#define PCIEP_STRAP_LC_STRAP_LANE_NEGOTIATION_MASK        0x70000

// Bitfield Description : Controls if RX Margining requires software. Provides an override value for STRAP_MARGINING_USES_SOFTWARE.
#define PCIEP_STRAP_LC_STRAP_MARGINING_USES_SOFTWARE_OFFSET      19
#define PCIEP_STRAP_LC_STRAP_MARGINING_USES_SOFTWARE_MASK        0x80000

// Bitfield Description : Controls if first Retimer presence detect is supported. Provides an override value for STRAP_RTM1_PRESENCE_DET_SUPP.
#define PCIEP_STRAP_LC_STRAP_RTM1_PRESENCE_DET_SUPP_OFFSET      20
#define PCIEP_STRAP_LC_STRAP_RTM1_PRESENCE_DET_SUPP_MASK        0x100000

// Bitfield Description : Controls if second Retimer presence detect is supported. Provides an override value for STRAP_RTM2_PRESENCE_DET_SUPP.
#define PCIEP_STRAP_LC_STRAP_RTM2_PRESENCE_DET_SUPP_OFFSET      21
#define PCIEP_STRAP_LC_STRAP_RTM2_PRESENCE_DET_SUPP_MASK        0x200000

// Bitfield Description :
#define PCIEP_STRAP_LC_Reserved_31_22_OFFSET      22
#define PCIEP_STRAP_LC_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            STRAP_FTS_yTSx_COUNT:2;
    UINT32                            STRAP_LONG_yTSx_COUNT:2;
    UINT32                            STRAP_MED_yTSx_COUNT:2;
    UINT32                            STRAP_SHORT_yTSx_COUNT:2;
    UINT32                            STRAP_SKIP_INTERVAL:3;
    UINT32                            STRAP_BYPASS_RCVR_DET:1;
    UINT32                            STRAP_COMPLIANCE_DIS:1;
    UINT32                            STRAP_FORCE_COMPLIANCE:1;
    UINT32                            STRAP_REVERSE_LC_LANES:1;
    UINT32                            STRAP_AUTO_RC_SPEED_NEGOTIATION_DIS:1;
    UINT32                            STRAP_LANE_NEGOTIATION:3;
    UINT32                            STRAP_MARGINING_USES_SOFTWARE:1;
    UINT32                            STRAP_RTM1_PRESENCE_DET_SUPP:1;
    UINT32                            STRAP_RTM2_PRESENCE_DET_SUPP:1;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} PCIEP_STRAP_LC_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_LC_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_LC_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_LC_OFFSET       0x300
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x300)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_LC_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_LC_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_LC_OFFSET       0x300
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x300)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_LC_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_LC_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_LC_OFFSET       0x300
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x300)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_LC_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_LC_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_LC_OFFSET       0x300
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x300)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_LC_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_LC_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_LC_OFFSET       0x300
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_LC_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x300)

#define SMN_FUNC0_PCIE0_PCIEP_STRAP_LC_ADDRESS    0x11140300UL

#define SMN_FUNC0_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11240300UL

#define SMN_FUNC0_PCIE2_PCIEP_STRAP_LC_ADDRESS    0x11340300UL

#define SMN_FUNC0_PCIE3_PCIEP_STRAP_LC_ADDRESS    0x11440300UL

#define SMN_FUNC1_PCIE0_PCIEP_STRAP_LC_ADDRESS    0x11141300UL

#define SMN_FUNC1_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11241300UL

#define SMN_FUNC2_PCIE0_PCIEP_STRAP_LC_ADDRESS    0x11142300UL

#define SMN_FUNC2_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11242300UL

#define SMN_FUNC3_PCIE0_PCIEP_STRAP_LC_ADDRESS    0x11143300UL

#define SMN_FUNC3_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11243300UL

#define SMN_FUNC4_PCIE0_PCIEP_STRAP_LC_ADDRESS    0x11144300UL

#define SMN_FUNC4_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11244300UL

#define SMN_FUNC5_PCIE1_PCIEP_STRAP_LC_ADDRESS    0x11245300UL


/***********************************************************
* Register Name : PCIEP_STRAP_MISC
* Register Description :
* Miscellaneous Strap register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls lane ordering. Provides an override value for STRAP_REVERSE_LANES.
#define PCIEP_STRAP_MISC_STRAP_REVERSE_LANES_OFFSET      0
#define PCIEP_STRAP_MISC_STRAP_REVERSE_LANES_MASK        0x1

// Bitfield Description : Controls if End-End TLP Prefix is supported. Provides an override value for STRAP_E2E_PREFIX_EN.
#define PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_OFFSET      1
#define PCIEP_STRAP_MISC_STRAP_E2E_PREFIX_EN_MASK        0x2

// Bitfield Description : Controls if Extended FMT is supported. Provides an override value for STRAP_EXTENDED_FMT_SUPPORTED.
#define PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_OFFSET      2
#define PCIEP_STRAP_MISC_STRAP_EXTENDED_FMT_SUPPORTED_MASK        0x4

// Bitfield Description : Controls if OBFF is supported. Provides an override value for STRAP_OBFF_SUPPORTED.
#define PCIEP_STRAP_MISC_STRAP_OBFF_SUPPORTED_OFFSET      3
#define PCIEP_STRAP_MISC_STRAP_OBFF_SUPPORTED_MASK        0x18

// Bitfield Description : Controls if LTR is supported. Provides an override value for STRAP_LTR_SUPPORTED.
#define PCIEP_STRAP_MISC_STRAP_LTR_SUPPORTED_OFFSET      5
#define PCIEP_STRAP_MISC_STRAP_LTR_SUPPORTED_MASK        0x20

// Bitfield Description : Controls if CCIX Transport DVSEC is supported. Provides an override value for STRAP_BIF_CCIX_EN.
#define PCIEP_STRAP_MISC_STRAP_CCIX_EN_OFFSET      6
#define PCIEP_STRAP_MISC_STRAP_CCIX_EN_MASK        0x40

// Bitfield Description : Controls if the Port is permitted to use Optimized TLP format for CCIX. Provides an override for STRAP_BIF_CCIX_OPT_TLP_FMT_SUPPORT.
#define PCIEP_STRAP_MISC_STRAP_CCIX_OPT_TLP_FMT_SUPPORT_OFFSET      7
#define PCIEP_STRAP_MISC_STRAP_CCIX_OPT_TLP_FMT_SUPPORT_MASK        0x80

// Bitfield Description :
#define PCIEP_STRAP_MISC_Reserved_31_8_OFFSET      8
#define PCIEP_STRAP_MISC_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            STRAP_REVERSE_LANES:1;
    UINT32                            STRAP_E2E_PREFIX_EN:1;
    UINT32                            STRAP_EXTENDED_FMT_SUPPORTED:1;
    UINT32                            STRAP_OBFF_SUPPORTED:2;
    UINT32                            STRAP_LTR_SUPPORTED:1;
    UINT32                            STRAP_CCIX_EN:1;
    UINT32                            STRAP_CCIX_OPT_TLP_FMT_SUPPORT:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} PCIEP_STRAP_MISC_PCIEPORT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_MISC_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_MISC_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_MISC_OFFSET       0x304
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_STRAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x304)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_MISC_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_MISC_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_MISC_OFFSET       0x304
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_STRAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x304)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_MISC_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_MISC_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_MISC_OFFSET       0x304
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_STRAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x304)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_MISC_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_MISC_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_MISC_OFFSET       0x304
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_STRAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x304)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_MISC_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_MISC_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_MISC_OFFSET       0x304
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_STRAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x304)

#define SMN_FUNC0_PCIE0_PCIEP_STRAP_MISC_ADDRESS    0x11140304UL

#define SMN_FUNC0_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11240304UL

#define SMN_FUNC0_PCIE2_PCIEP_STRAP_MISC_ADDRESS    0x11340304UL

#define SMN_FUNC0_PCIE3_PCIEP_STRAP_MISC_ADDRESS    0x11440304UL

#define SMN_FUNC1_PCIE0_PCIEP_STRAP_MISC_ADDRESS    0x11141304UL

#define SMN_FUNC1_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11241304UL

#define SMN_FUNC2_PCIE0_PCIEP_STRAP_MISC_ADDRESS    0x11142304UL

#define SMN_FUNC2_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11242304UL

#define SMN_FUNC3_PCIE0_PCIEP_STRAP_MISC_ADDRESS    0x11143304UL

#define SMN_FUNC3_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11243304UL

#define SMN_FUNC4_PCIE0_PCIEP_STRAP_MISC_ADDRESS    0x11144304UL

#define SMN_FUNC4_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11244304UL

#define SMN_FUNC5_PCIE1_PCIEP_STRAP_MISC_ADDRESS    0x11245304UL


/***********************************************************
* Register Name : PCIE_LC_CNTL
* Register Description :
* Link Controller Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define PCIE_LC_CNTL_Reserved_0_0_OFFSET      0
#define PCIE_LC_CNTL_Reserved_0_0_MASK        0x1

// Bitfield Description : Controls if the Link is permitted to enter L2/L3 Ready when the Port is in D0.
#define PCIE_LC_CNTL_LC_DONT_ENTER_L23_IN_D0_OFFSET      1
#define PCIE_LC_CNTL_LC_DONT_ENTER_L23_IN_D0_MASK        0x2

// Bitfield Description : Controls resetting of the Electrical Idle counter.
#define PCIE_LC_CNTL_LC_RESET_L_IDLE_COUNT_EN_OFFSET      2
#define PCIE_LC_CNTL_LC_RESET_L_IDLE_COUNT_EN_MASK        0x4

// Bitfield Description : Triggers a reset of an individual Link without resetting the other Ports.
#define PCIE_LC_CNTL_LC_RESET_LINK_OFFSET      3
#define PCIE_LC_CNTL_LC_RESET_LINK_MASK        0x8

// Bitfield Description : Controls the time that the LC waits for the transmission pipe to drain before entering L1 or L2/L3 Ready. Starting with 1, each incremental value increases the delay, while 0 results in the highest delay. When LC_STATIC_TX_PIPE_COUNT_EN=0, the delay value is scaled based on the link width (i.e. smaller link width results in larger actual delay value).
#define PCIE_LC_CNTL_LC_16X_CLEAR_TX_PIPE_OFFSET      4
#define PCIE_LC_CNTL_LC_16X_CLEAR_TX_PIPE_MASK        0xf0

// Bitfield Description : Controls the threshold setting of the Tx_L0s inactivity timer. This timer determines when the Link may attempt to enter TX_L0s.
#define PCIE_LC_CNTL_LC_L0S_INACTIVITY_OFFSET      8
#define PCIE_LC_CNTL_LC_L0S_INACTIVITY_MASK        0xf00

// Bitfield Description : (Upstream Ports only) Controls the threshold setting of the L1 inactivity timer. This timer determines when the Link may attempt to enter ASPM L1.
#define PCIE_LC_CNTL_LC_L1_INACTIVITY_OFFSET      12
#define PCIE_LC_CNTL_LC_L1_INACTIVITY_MASK        0xf000

// Bitfield Description : (Upstream Ports only) Controls if the Link is permitted to enter PCI-PM L1 when the Port is not in D0.
#define PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_OFFSET      16
#define PCIE_LC_CNTL_LC_PMI_TO_L1_DIS_MASK        0x10000

// Bitfield Description : Controls incrementing N_FTS for each transition to Recovery.
#define PCIE_LC_CNTL_LC_INC_N_FTS_EN_OFFSET      17
#define PCIE_LC_CNTL_LC_INC_N_FTS_EN_MASK        0x20000

// Bitfield Description : Controls the number of clocks to wait to receive Electrical Idle ordered set in L1 or L2/L3 Ready.
#define PCIE_LC_CNTL_LC_LOOK_FOR_IDLE_IN_L1L23_OFFSET      18
#define PCIE_LC_CNTL_LC_LOOK_FOR_IDLE_IN_L1L23_MASK        0xc0000

// Bitfield Description : Controls factoring in the Extended Sync bit value in the calculation for the replay timer adjustment.
#define PCIE_LC_CNTL_LC_FACTOR_IN_EXT_SYNC_OFFSET      20
#define PCIE_LC_CNTL_LC_FACTOR_IN_EXT_SYNC_MASK        0x100000

// Bitfield Description : Controls waiting for PM_Request_Ack DLLP in L2/L3 Ready entry handshake.
#define PCIE_LC_CNTL_LC_WAIT_FOR_PM_ACK_DIS_OFFSET      21
#define PCIE_LC_CNTL_LC_WAIT_FOR_PM_ACK_DIS_MASK        0x200000

// Bitfield Description : (Downstream Ports only) Triggers waking the Link from L2/L3 Ready.
#define PCIE_LC_CNTL_LC_WAKE_FROM_L23_OFFSET      22
#define PCIE_LC_CNTL_LC_WAKE_FROM_L23_MASK        0x400000

// Bitfield Description : (Downstream Ports only) Controls if Port always returns PM_Request_Ack DLLP to an ASPM L1 entry request (i.e. never returns PM_Active_State_Nak TLP).
#define PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_OFFSET      23
#define PCIE_LC_CNTL_LC_L1_IMMEDIATE_ACK_MASK        0x800000

// Bitfield Description : Controls if a Link is permitted to enter ASPM L1.
#define PCIE_LC_CNTL_LC_ASPM_TO_L1_DIS_OFFSET      24
#define PCIE_LC_CNTL_LC_ASPM_TO_L1_DIS_MASK        0x1000000

// Bitfield Description : When LC_DELAY_L0s_EXIT bit is 1, controls the minimum amount of time the Link is required to stay in Tx_L0s. When LC_DELAY_L1_EXIT bit is 1, controls the minimum amount of time the Link is required to stay in L1.
#define PCIE_LC_CNTL_LC_DELAY_COUNT_OFFSET      25
#define PCIE_LC_CNTL_LC_DELAY_COUNT_MASK        0x6000000

// Bitfield Description : Controls staying in Tx_L0s for the minimum time specified in LC_DELAY_COUNT.
#define PCIE_LC_CNTL_LC_DELAY_L0S_EXIT_OFFSET      27
#define PCIE_LC_CNTL_LC_DELAY_L0S_EXIT_MASK        0x8000000

// Bitfield Description : Controls staying in L1 for the minimum time specified in LC_DELAY_COUNT.
#define PCIE_LC_CNTL_LC_DELAY_L1_EXIT_OFFSET      28
#define PCIE_LC_CNTL_LC_DELAY_L1_EXIT_MASK        0x10000000

// Bitfield Description : Controls waiting for Electrical Idle in L1 or L2/L3 Ready.
#define PCIE_LC_CNTL_LC_EXTEND_WAIT_FOR_EL_IDLE_OFFSET      29
#define PCIE_LC_CNTL_LC_EXTEND_WAIT_FOR_EL_IDLE_MASK        0x20000000

// Bitfield Description : Controls L1 or L2/L3 Ready entry escape arcs.
#define PCIE_LC_CNTL_LC_ESCAPE_L1L23_EN_OFFSET      30
#define PCIE_LC_CNTL_LC_ESCAPE_L1L23_EN_MASK        0x40000000

// Bitfield Description : Controls if ignoring PHY's Electrical Idle detector.
#define PCIE_LC_CNTL_LC_GATE_RCVR_IDLE_OFFSET      31
#define PCIE_LC_CNTL_LC_GATE_RCVR_IDLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            LC_DONT_ENTER_L23_IN_D0:1;
    UINT32                            LC_RESET_L_IDLE_COUNT_EN:1;
    UINT32                            LC_RESET_LINK:1;
    UINT32                            LC_16X_CLEAR_TX_PIPE:4;
    UINT32                            LC_L0S_INACTIVITY:4;
    UINT32                            LC_L1_INACTIVITY:4;
    UINT32                            LC_PMI_TO_L1_DIS:1;
    UINT32                            LC_INC_N_FTS_EN:1;
    UINT32                            LC_LOOK_FOR_IDLE_IN_L1L23:2;
    UINT32                            LC_FACTOR_IN_EXT_SYNC:1;
    UINT32                            LC_WAIT_FOR_PM_ACK_DIS:1;
    UINT32                            LC_WAKE_FROM_L23:1;
    UINT32                            LC_L1_IMMEDIATE_ACK:1;
    UINT32                            LC_ASPM_TO_L1_DIS:1;
    UINT32                            LC_DELAY_COUNT:2;
    UINT32                            LC_DELAY_L0S_EXIT:1;
    UINT32                            LC_DELAY_L1_EXIT:1;
    UINT32                            LC_EXTEND_WAIT_FOR_EL_IDLE:1;
    UINT32                            LC_ESCAPE_L1L23_EN:1;
    UINT32                            LC_GATE_RCVR_IDLE:1;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x280)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x280)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x280)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x280)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x280)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL_ADDRESS    0x11140280UL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11240280UL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL_ADDRESS    0x11340280UL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL_ADDRESS    0x11440280UL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL_ADDRESS    0x11141280UL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11241280UL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL_ADDRESS    0x11142280UL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11242280UL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL_ADDRESS    0x11143280UL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11243280UL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL_ADDRESS    0x11144280UL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11244280UL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL_ADDRESS    0x11245280UL


/***********************************************************
* Register Name : PCIE_LC_CNTL2
* Register Description :
* Link Controller Control 2 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When LC_STATE_TIMED_OUT bit is 1, indicates state that the LC was in when the dead man timer expired.
#define PCIE_LC_CNTL2_LC_TIMED_OUT_STATE_OFFSET      0
#define PCIE_LC_CNTL2_LC_TIMED_OUT_STATE_MASK        0x3f

// Bitfield Description : Indicates if the dead man timer expired.
#define PCIE_LC_CNTL2_LC_STATE_TIMED_OUT_OFFSET      6
#define PCIE_LC_CNTL2_LC_STATE_TIMED_OUT_MASK        0x40

// Bitfield Description : Controls checking for bandwidth change when reporting Link Bandwidth Notification Status.
#define PCIE_LC_CNTL2_LC_LOOK_FOR_BW_REDUCTION_OFFSET      7
#define PCIE_LC_CNTL2_LC_LOOK_FOR_BW_REDUCTION_MASK        0x80

// Bitfield Description : Controls the number of TS2s require sent before changing the Link's state.
#define PCIE_LC_CNTL2_LC_MORE_TS2_EN_OFFSET      8
#define PCIE_LC_CNTL2_LC_MORE_TS2_EN_MASK        0x100

// Bitfield Description : Controls if Link negotiation to x12 is supported.
#define PCIE_LC_CNTL2_LC_X12_NEGOTIATION_DIS_OFFSET      9
#define PCIE_LC_CNTL2_LC_X12_NEGOTIATION_DIS_MASK        0x200

// Bitfield Description : Controls if Link negotiation using lane reversal is permitted when it results in a wider width in Link Up.
#define PCIE_LC_CNTL2_LC_LINK_UP_REVERSAL_EN_OFFSET      10
#define PCIE_LC_CNTL2_LC_LINK_UP_REVERSAL_EN_MASK        0x400

// Bitfield Description : Indicates if the LC is in an illegal state.
#define PCIE_LC_CNTL2_LC_ILLEGAL_STATE_OFFSET      11
#define PCIE_LC_CNTL2_LC_ILLEGAL_STATE_MASK        0x800

// Bitfield Description : Controls if the LC is restarted when it is in an illegal state.
#define PCIE_LC_CNTL2_LC_ILLEGAL_STATE_RESTART_EN_OFFSET      12
#define PCIE_LC_CNTL2_LC_ILLEGAL_STATE_RESTART_EN_MASK        0x1000

// Bitfield Description : Controls limiting the delay introduced by waiting for TSx on other lanes.
#define PCIE_LC_CNTL2_LC_WAIT_FOR_OTHER_LANES_MODE_OFFSET      13
#define PCIE_LC_CNTL2_LC_WAIT_FOR_OTHER_LANES_MODE_MASK        0x2000

// Bitfield Description : Controls the Electrical Idle detection mode used by the LC, for each data rate. Terms used are: EI indicates Electrical Idle entry; EIE indicates Electrical Idle Exit; PHY indicates PHY's Electrical Idle Detector is used; INF indicates that LC's Electrical Idle inference logic is used.
#define PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_OFFSET      14
#define PCIE_LC_CNTL2_LC_ELEC_IDLE_MODE_MASK        0xc000

// Bitfield Description : Controls disabling the LC's inference of Electrical Idle.
#define PCIE_LC_CNTL2_LC_DISABLE_INFERRED_ELEC_IDLE_DET_OFFSET      16
#define PCIE_LC_CNTL2_LC_DISABLE_INFERRED_ELEC_IDLE_DET_MASK        0x10000

// Bitfield Description : Controls if the BIF_CHIP_CLK_PDWN output is set to 1 when the LC is in L1.
#define PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L1_OFFSET      17
#define PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L1_MASK        0x20000

// Bitfield Description : Controls if the BIF_CHIP_CLK_PDWN output is set to 1 when the LC is in L2/L3 Ready.
#define PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L23_OFFSET      18
#define PCIE_LC_CNTL2_LC_ALLOW_PDWN_IN_L23_MASK        0x40000

// Bitfield Description : Controls resetting EIOS counter when a non-consecutive EIOS is received.
#define PCIE_LC_CNTL2_LC_CONSECUTIVE_EIOS_RESET_EN_OFFSET      19
#define PCIE_LC_CNTL2_LC_CONSECUTIVE_EIOS_RESET_EN_MASK        0x80000

// Bitfield Description : Controls if Electrical Idle causes a transition from L0 to Rx_L0s.
#define PCIE_LC_CNTL2_LC_BLOCK_EL_IDLE_IN_L0_OFFSET      20
#define PCIE_LC_CNTL2_LC_BLOCK_EL_IDLE_IN_L0_MASK        0x100000

// Bitfield Description : Controls if transition from receiver L0 to Rx_L0s is permitted.
#define PCIE_LC_CNTL2_LC_RCV_L0_TO_RCV_L0S_DIS_OFFSET      21
#define PCIE_LC_CNTL2_LC_RCV_L0_TO_RCV_L0S_DIS_MASK        0x200000

// Bitfield Description : Controls if the INACTIVE_LANES signals are asserted when CHIP_BIF_hold_training is high.
#define PCIE_LC_CNTL2_LC_ASSERT_INACTIVE_DURING_HOLD_OFFSET      22
#define PCIE_LC_CNTL2_LC_ASSERT_INACTIVE_DURING_HOLD_MASK        0x400000

// Bitfield Description : When LC_WAIT_FOR_OTHER_LANES_MODE bit is 0, controls the delay time used to wait for TSx on all lanes in link width negotiation. The units for the values are TX clocks.
#define PCIE_LC_CNTL2_LC_WAIT_FOR_LANES_IN_LW_NEG_OFFSET      23
#define PCIE_LC_CNTL2_LC_WAIT_FOR_LANES_IN_LW_NEG_MASK        0x1800000

// Bitfield Description : Controls if unused lanes are permitted to be powered down.
#define PCIE_LC_CNTL2_LC_PWR_DOWN_NEG_OFF_LANES_OFFSET      25
#define PCIE_LC_CNTL2_LC_PWR_DOWN_NEG_OFF_LANES_MASK        0x2000000

// Bitfield Description : Controls transition to Recovery.RcvrLock from Configuration.Idle or Recovery.Idle when a TSx is received. Behavior is similar to that of the 'idle_to_rlock_transitioned' variable (i.e. when variable is 00h or FFh).
#define PCIE_LC_CNTL2_LC_DISABLE_LOST_SYM_LOCK_ARCS_OFFSET      26
#define PCIE_LC_CNTL2_LC_DISABLE_LOST_SYM_LOCK_ARCS_MASK        0x4000000

// Bitfield Description : Controls if the Link Bandwidth Notification feature is used.
#define PCIE_LC_CNTL2_LC_LINK_BW_NOTIFICATION_DIS_OFFSET      27
#define PCIE_LC_CNTL2_LC_LINK_BW_NOTIFICATION_DIS_MASK        0x8000000

// Bitfield Description : Controls if the LC waits for the slave to be empty before advancing to PCI-PM L1.
#define PCIE_LC_CNTL2_LC_PMI_L1_WAIT_FOR_SLV_IDLE_OFFSET      28
#define PCIE_LC_CNTL2_LC_PMI_L1_WAIT_FOR_SLV_IDLE_MASK        0x10000000

// Bitfield Description : Controls the LTTSM state timeout threshold values.
#define PCIE_LC_CNTL2_LC_TEST_TIMER_SEL_OFFSET      29
#define PCIE_LC_CNTL2_LC_TEST_TIMER_SEL_MASK        0x60000000

// Bitfield Description : Controls if inferred Electrical Idle detection for PI (Physical Layer blocks) is used.
#define PCIE_LC_CNTL2_LC_ENABLE_INFERRED_ELEC_IDLE_FOR_PI_OFFSET      31
#define PCIE_LC_CNTL2_LC_ENABLE_INFERRED_ELEC_IDLE_FOR_PI_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LC_TIMED_OUT_STATE:6;
    UINT32                            LC_STATE_TIMED_OUT:1;
    UINT32                            LC_LOOK_FOR_BW_REDUCTION:1;
    UINT32                            LC_MORE_TS2_EN:1;
    UINT32                            LC_X12_NEGOTIATION_DIS:1;
    UINT32                            LC_LINK_UP_REVERSAL_EN:1;
    UINT32                            LC_ILLEGAL_STATE:1;
    UINT32                            LC_ILLEGAL_STATE_RESTART_EN:1;
    UINT32                            LC_WAIT_FOR_OTHER_LANES_MODE:1;
    UINT32                            LC_ELEC_IDLE_MODE:2;
    UINT32                            LC_DISABLE_INFERRED_ELEC_IDLE_DET:1;
    UINT32                            LC_ALLOW_PDWN_IN_L1:1;
    UINT32                            LC_ALLOW_PDWN_IN_L23:1;
    UINT32                            LC_CONSECUTIVE_EIOS_RESET_EN:1;
    UINT32                            LC_BLOCK_EL_IDLE_IN_L0:1;
    UINT32                            LC_RCV_L0_TO_RCV_L0S_DIS:1;
    UINT32                            LC_ASSERT_INACTIVE_DURING_HOLD:1;
    UINT32                            LC_WAIT_FOR_LANES_IN_LW_NEG:2;
    UINT32                            LC_PWR_DOWN_NEG_OFF_LANES:1;
    UINT32                            LC_DISABLE_LOST_SYM_LOCK_ARCS:1;
    UINT32                            LC_LINK_BW_NOTIFICATION_DIS:1;
    UINT32                            LC_PMI_L1_WAIT_FOR_SLV_IDLE:1;
    UINT32                            LC_TEST_TIMER_SEL:2;
    UINT32                            LC_ENABLE_INFERRED_ELEC_IDLE_FOR_PI:1;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL2_PCIEPORT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL2_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL2_OFFSET       0x2c4
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x2c4)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL2_OFFSET       0x2c4
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x2c4)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL2_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL2_OFFSET       0x2c4
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x2c4)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL2_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL2_OFFSET       0x2c4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x2c4)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL2_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL2_OFFSET       0x2c4
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x2c4)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL2_ADDRESS    0x111402c4UL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112402c4UL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL2_ADDRESS    0x113402c4UL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL2_ADDRESS    0x114402c4UL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL2_ADDRESS    0x111412c4UL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112412c4UL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL2_ADDRESS    0x111422c4UL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112422c4UL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL2_ADDRESS    0x111432c4UL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112432c4UL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL2_ADDRESS    0x111442c4UL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112442c4UL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL2_ADDRESS    0x112452c4UL


/***********************************************************
* Register Name : PCIE_LC_CNTL3
* Register Description :
* Link Controller Control 3 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : (Upstream Ports only) Controls the value of the Selectable De-emphasis bit (symbol 4, bit 6) in any subsequently transmitted TS1s. Setting this bit requests the Link partner to use this de-emphasis setting at 5.0 GT/s.
#define PCIE_LC_CNTL3_LC_SELECT_DEEMPHASIS_OFFSET      0
#define PCIE_LC_CNTL3_LC_SELECT_DEEMPHASIS_MASK        0x1

// Bitfield Description : (Downstream Ports only) Controls how the transmitter de-emphasis setting for 5.0 GT/s is determined on entry to Recovery.RcvrCfg. The resulting setting will be applied to the transmitters of both Link partners when operating at 5.0 GT/s.
#define PCIE_LC_CNTL3_LC_SELECT_DEEMPHASIS_CNTL_OFFSET      1
#define PCIE_LC_CNTL3_LC_SELECT_DEEMPHASIS_CNTL_MASK        0x6

// Bitfield Description : Indicates the 5.0 GT/s de-emphasis setting requested by other end.
#define PCIE_LC_CNTL3_LC_RCVD_DEEMPHASIS_OFFSET      3
#define PCIE_LC_CNTL3_LC_RCVD_DEEMPHASIS_MASK        0x8

// Bitfield Description : When sending Modified Compliance Pattern, controls if LTSSM exits Polling.Compliance upon receiving TS1s.
#define PCIE_LC_CNTL3_LC_COMP_TO_DETECT_OFFSET      4
#define PCIE_LC_CNTL3_LC_COMP_TO_DETECT_MASK        0x10

// Bitfield Description : Controls if Training Sequence counter is reset when DIRECTED_SPEED_CHANGE is asserted in Recovery.RcvrLock.
#define PCIE_LC_CNTL3_LC_RESET_TSX_CNT_IN_RLOCK_EN_OFFSET      5
#define PCIE_LC_CNTL3_LC_RESET_TSX_CNT_IN_RLOCK_EN_MASK        0x20

// Bitfield Description : (Downstream Ports only) Controls the number of unsuccessful autonomous link speed changes that are allowed, before LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED bit is set.
#define PCIE_LC_CNTL3_LC_AUTO_SPEED_CHANGE_ATTEMPTS_ALLOWED_OFFSET      6
#define PCIE_LC_CNTL3_LC_AUTO_SPEED_CHANGE_ATTEMPTS_ALLOWED_MASK        0xc0

// Bitfield Description : (Downstream Ports only) Indicates if the number of unsuccessful autonomous link speed change attempts has reached the threshold value set in LC_AUTO_SPEED_CHANGE_ATTEMPTS_ALLOWED field. While this bit is 1, the Port is prevented from initiating any more autonomous link speed changes. For Upstream Ports, this bit is always 0.
#define PCIE_LC_CNTL3_LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED_OFFSET      8
#define PCIE_LC_CNTL3_LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED_MASK        0x100

// Bitfield Description : (Downstream Ports only) Clears LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED bit and resets the counter of unsuccessful Autonomous Speed Changes.
#define PCIE_LC_CNTL3_LC_CLR_FAILED_AUTO_SPD_CHANGE_CNT_OFFSET      9
#define PCIE_LC_CNTL3_LC_CLR_FAILED_AUTO_SPD_CHANGE_CNT_MASK        0x200

// Bitfield Description : (Downstream Ports only) Controls if Enhanced Hot Plug feature is enabled on the associated Port.
#define PCIE_LC_CNTL3_LC_ENHANCED_HOT_PLUG_EN_OFFSET      10
#define PCIE_LC_CNTL3_LC_ENHANCED_HOT_PLUG_EN_MASK        0x400

// Bitfield Description : (Downstream Ports only) Controls if LTSSM checks for CHIP_BIF_RcvrDetEn assertion before being allowed to exit Detect.
#define PCIE_LC_CNTL3_LC_RCVR_DET_EN_OVERRIDE_OFFSET      11
#define PCIE_LC_CNTL3_LC_RCVR_DET_EN_OVERRIDE_MASK        0x800

// Bitfield Description : Controls allowing an autonomous speed change attempt after a Link Down event (as if the device was coming out of fundamental reset).
#define PCIE_LC_CNTL3_LC_LINK_DOWN_SPD_CHG_EN_OFFSET      12
#define PCIE_LC_CNTL3_LC_LINK_DOWN_SPD_CHG_EN_MASK        0x1000

// Bitfield Description : Controls blocking DLLP transmission when autonomous speed changes are not being performed.
#define PCIE_LC_CNTL3_LC_CLR_DELAY_DLLP_WHEN_NO_AUTO_EQ_OFFSET      13
#define PCIE_LC_CNTL3_LC_CLR_DELAY_DLLP_WHEN_NO_AUTO_EQ_MASK        0x2000

// Bitfield Description : When LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN bit is 1, controls autonomous speed changes based on the data rate advertised by Link partner.
#define PCIE_LC_CNTL3_LC_MULT_AUTO_SPD_CHG_ON_LAST_RATE_OFFSET      14
#define PCIE_LC_CNTL3_LC_MULT_AUTO_SPD_CHG_ON_LAST_RATE_MASK        0x4000

// Bitfield Description : Controls if a successful speed change will automatically reset the counter of failed speed changes, used to trigger PCIE_LC_SPEED_CNTL.LC_SPEED_CHANGE_ATTEMPT_FAILED.
#define PCIE_LC_CNTL3_LC_RST_FAILING_SPD_CHANGE_CNT_ON_SUCCESS_EN_OFFSET      15
#define PCIE_LC_CNTL3_LC_RST_FAILING_SPD_CHANGE_CNT_ON_SUCCESS_EN_MASK        0x8000

// Bitfield Description : For SB architectures, controls if CHIP_BIF_USB_IDLE_EN is used, which when set low delays L1 entry due to inactivity.
#define PCIE_LC_CNTL3_LC_CHIP_BIF_USB_IDLE_EN_OFFSET      16
#define PCIE_LC_CNTL3_LC_CHIP_BIF_USB_IDLE_EN_MASK        0x10000

// Bitfield Description : Controls if LTSSM forces L1 request to be satisfied before Link reconfiguration.
#define PCIE_LC_CNTL3_LC_L1_BLOCK_RECONFIG_EN_OFFSET      17
#define PCIE_LC_CNTL3_LC_L1_BLOCK_RECONFIG_EN_MASK        0x20000

// Bitfield Description : Controls if LTSSM is allowed to autonomously disable speed support after detecting training failures.
#define PCIE_LC_CNTL3_LC_AUTO_DISABLE_SPEED_SUPPORT_EN_OFFSET      18
#define PCIE_LC_CNTL3_LC_AUTO_DISABLE_SPEED_SUPPORT_EN_MASK        0x40000

// Bitfield Description : When LC_AUTO_DISABLE_SPEED_SUPPORT_EN bit is 1, controls the number of training failures allowed to occur before disabling speed capability.
#define PCIE_LC_CNTL3_LC_AUTO_DISABLE_SPEED_SUPPORT_MAX_FAIL_SEL_OFFSET      19
#define PCIE_LC_CNTL3_LC_AUTO_DISABLE_SPEED_SUPPORT_MAX_FAIL_SEL_MASK        0x180000

// Bitfield Description : Controls if LTSSM allows transition from L1.Entry to L1.Idle after AUX_COUNT has first expired.
#define PCIE_LC_CNTL3_LC_FAST_L1_ENTRY_EXIT_EN_OFFSET      21
#define PCIE_LC_CNTL3_LC_FAST_L1_ENTRY_EXIT_EN_MASK        0x200000

// Bitfield Description : Controls if upon waking from L1 due to having a TLP to transmit, the LTSSM waits before going to P0.
#define PCIE_LC_CNTL3_LC_POWERDOWN_P0_WAIT_FOR_REFCLKACK_ON_L1_EXIT_OFFSET      22
#define PCIE_LC_CNTL3_LC_POWERDOWN_P0_WAIT_FOR_REFCLKACK_ON_L1_EXIT_MASK        0x400000

// Bitfield Description : (Upstream Ports only) Controls if LTSSM requests entering L2/L3 Ready, after receiving PME TO Ack to a PME Turn Off request.
#define PCIE_LC_CNTL3_LC_DSC_DONT_ENTER_L23_AFTER_PME_ACK_OFFSET      23
#define PCIE_LC_CNTL3_LC_DSC_DONT_ENTER_L23_AFTER_PME_ACK_MASK        0x800000

// Bitfield Description : Controls the chip/bif_core speed control interface.
#define PCIE_LC_CNTL3_LC_HW_VOLTAGE_IF_CONTROL_OFFSET      24
#define PCIE_LC_CNTL3_LC_HW_VOLTAGE_IF_CONTROL_MASK        0x3000000

// Bitfield Description : Controls the delay in the circuit that filters noise out of the chip/bif_core voltage interface.
#define PCIE_LC_CNTL3_LC_VOLTAGE_TIMER_SEL_OFFSET      26
#define PCIE_LC_CNTL3_LC_VOLTAGE_TIMER_SEL_MASK        0x3c000000

// Bitfield Description : When the Link is in L0, force the Link to Recovery.
#define PCIE_LC_CNTL3_LC_GO_TO_RECOVERY_OFFSET      30
#define PCIE_LC_CNTL3_LC_GO_TO_RECOVERY_MASK        0x40000000

// Bitfield Description : Controls automatically going to Recovery, when a voltage change causes a change in the supported data rates.
#define PCIE_LC_CNTL3_LC_AUTO_RECOVERY_DIS_OFFSET      31
#define PCIE_LC_CNTL3_LC_AUTO_RECOVERY_DIS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LC_SELECT_DEEMPHASIS:1;
    UINT32                            LC_SELECT_DEEMPHASIS_CNTL:2;
    UINT32                            LC_RCVD_DEEMPHASIS:1;
    UINT32                            LC_COMP_TO_DETECT:1;
    UINT32                            LC_RESET_TSX_CNT_IN_RLOCK_EN:1;
    UINT32                            LC_AUTO_SPEED_CHANGE_ATTEMPTS_ALLOWED:2;
    UINT32                            LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED:1;
    UINT32                            LC_CLR_FAILED_AUTO_SPD_CHANGE_CNT:1;
    UINT32                            LC_ENHANCED_HOT_PLUG_EN:1;
    UINT32                            LC_RCVR_DET_EN_OVERRIDE:1;
    UINT32                            LC_LINK_DOWN_SPD_CHG_EN:1;
    UINT32                            LC_CLR_DELAY_DLLP_WHEN_NO_AUTO_EQ:1;
    UINT32                            LC_MULT_AUTO_SPD_CHG_ON_LAST_RATE:1;
    UINT32                            LC_RST_FAILING_SPD_CHANGE_CNT_ON_SUCCESS_EN:1;
    UINT32                            LC_CHIP_BIF_USB_IDLE_EN:1;
    UINT32                            LC_L1_BLOCK_RECONFIG_EN:1;
    UINT32                            LC_AUTO_DISABLE_SPEED_SUPPORT_EN:1;
    UINT32                            LC_AUTO_DISABLE_SPEED_SUPPORT_MAX_FAIL_SEL:2;
    UINT32                            LC_FAST_L1_ENTRY_EXIT_EN:1;
    UINT32                            LC_POWERDOWN_P0_WAIT_FOR_REFCLKACK_ON_L1_EXIT:1;
    UINT32                            LC_DSC_DONT_ENTER_L23_AFTER_PME_ACK:1;
    UINT32                            LC_HW_VOLTAGE_IF_CONTROL:2;
    UINT32                            LC_VOLTAGE_TIMER_SEL:4;
    UINT32                            LC_GO_TO_RECOVERY:1;
    UINT32                            LC_AUTO_RECOVERY_DIS:1;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL3_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL3_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL3_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL3_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x2d4)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL3_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL3_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL3_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x2d4)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL3_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL3_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL3_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x2d4)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL3_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL3_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL3_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x2d4)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL3_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL3_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL3_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x2d4)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL3_ADDRESS    0x111402d4UL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112402d4UL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL3_ADDRESS    0x113402d4UL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL3_ADDRESS    0x114402d4UL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL3_ADDRESS    0x111412d4UL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112412d4UL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL3_ADDRESS    0x111422d4UL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112422d4UL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL3_ADDRESS    0x111432d4UL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112432d4UL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL3_ADDRESS    0x111442d4UL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112442d4UL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL3_ADDRESS    0x112452d4UL


/***********************************************************
* Register Name : PCIE_LC_CNTL6
* Register Description :
* Link Controller Control 6 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the data path width when the Port is running at 2.5 GT/s.
#define PCIE_LC_CNTL6_LC_SPC_MODE_2P5GT_OFFSET      0
#define PCIE_LC_CNTL6_LC_SPC_MODE_2P5GT_MASK        0x3

// Bitfield Description : Controls the data path width when the Port is running at 5.0 GT/s.
#define PCIE_LC_CNTL6_LC_SPC_MODE_5GT_OFFSET      2
#define PCIE_LC_CNTL6_LC_SPC_MODE_5GT_MASK        0xc

// Bitfield Description : Controls the data path width when the Port is running at 8.0 GT/s.
#define PCIE_LC_CNTL6_LC_SPC_MODE_8GT_OFFSET      4
#define PCIE_LC_CNTL6_LC_SPC_MODE_8GT_MASK        0x30

// Bitfield Description : Controls the data path width when the Port is running at 16.0 GT/s.
#define PCIE_LC_CNTL6_LC_SPC_MODE_16GT_OFFSET      6
#define PCIE_LC_CNTL6_LC_SPC_MODE_16GT_MASK        0xc0

// Bitfield Description : Controls the data path width when the Port is running at 32.0 GT/s.
#define PCIE_LC_CNTL6_LC_SPC_MODE_32GT_OFFSET      8
#define PCIE_LC_CNTL6_LC_SPC_MODE_32GT_MASK        0x300

// Bitfield Description :
#define PCIE_LC_CNTL6_Reserved_11_10_OFFSET      10
#define PCIE_LC_CNTL6_Reserved_11_10_MASK        0xc00

// Bitfield Description : Controls if SRIS mode is enabled. When the bit is 1, Port generates SKP ordered sets with SRIS SKP ordered set interval, unless overridden by the LC_SRNS_SKIP_IN_SRIS field.
#define PCIE_LC_CNTL6_LC_SRIS_EN_OFFSET      12
#define PCIE_LC_CNTL6_LC_SRIS_EN_MASK        0x1000

// Bitfield Description : When LC_SRIS_EN bit is 1, controls if the Port generates SKP ordered sets with the SRNS SKP ordered set interval for the corresponding Link speed.
#define PCIE_LC_CNTL6_LC_SRNS_SKIP_IN_SRIS_OFFSET      13
#define PCIE_LC_CNTL6_LC_SRNS_SKIP_IN_SRIS_MASK        0x3e000

// Bitfield Description :
#define PCIE_LC_CNTL6_Reserved_19_18_OFFSET      18
#define PCIE_LC_CNTL6_Reserved_19_18_MASK        0xc0000

// Bitfield Description : Controls if the modification of the SKP ordered set interval based on elasticity buffer pointer slip feedback from the PCS is allowed.
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_EN_OFFSET      20
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_EN_MASK        0x100000

// Bitfield Description : Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS.
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_FACTOR_OFFSET      21
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_FACTOR_MASK        0x600000

// Bitfield Description : Controls the SKP ordered set interval selection method when LC_SRIS_AUTODETECT_EN bit is 1.
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_MODE_OFFSET      23
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_MODE_MASK        0x1800000

// Bitfield Description : Indicates that the SKP ordered set auto detection circuit has generated an out of range SKP ordered set interval value.
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_OUT_OF_RANGE_OFFSET      25
#define PCIE_LC_CNTL6_LC_SRIS_AUTODETECT_OUT_OF_RANGE_MASK        0x2000000

// Bitfield Description : Controls overriding the Retimer Present bit (symbol 5, bit 4) and the Two Retimers Present bit (symbol 5, bit 5) status in received TS2s.
#define PCIE_LC_CNTL6_LC_OVERRIDE_RETIMER_PRESENCE_EN_OFFSET      26
#define PCIE_LC_CNTL6_LC_OVERRIDE_RETIMER_PRESENCE_EN_MASK        0x4000000

// Bitfield Description : When LC_OVERRIDE_RETIMER_PRESENCE_EN bit is 1, controls the Retimer Present and the Two Retimers Present status used by the receiver.
#define PCIE_LC_CNTL6_LC_OVERRIDE_RETIMER_PRESENCE_OFFSET      27
#define PCIE_LC_CNTL6_LC_OVERRIDE_RETIMER_PRESENCE_MASK        0x18000000

// Bitfield Description : Controls ignoring the Retimer Present bit (symbol 5, bit 4) and the Two Retimers Present bit (symbol 5, bit 5) in received TS2s.
#define PCIE_LC_CNTL6_LC_IGNORE_RETIMER_PRESENCE_OFFSET      29
#define PCIE_LC_CNTL6_LC_IGNORE_RETIMER_PRESENCE_MASK        0x20000000

// Bitfield Description : This is the final Retimer Presence Detected and Two Retimers Presence Detect values being used (including override control if enabled).
#define PCIE_LC_CNTL6_LC_RETIMER_PRESENCE_OFFSET      30
#define PCIE_LC_CNTL6_LC_RETIMER_PRESENCE_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LC_SPC_MODE_2P5GT:2;
    UINT32                            LC_SPC_MODE_5GT:2;
    UINT32                            LC_SPC_MODE_8GT:2;
    UINT32                            LC_SPC_MODE_16GT:2;
    UINT32                            LC_SPC_MODE_32GT:2;
    UINT32                            Reserved_11_10:2;
    UINT32                            LC_SRIS_EN:1;
    UINT32                            LC_SRNS_SKIP_IN_SRIS:5;
    UINT32                            Reserved_19_18:2;
    UINT32                            LC_SRIS_AUTODETECT_EN:1;
    UINT32                            LC_SRIS_AUTODETECT_FACTOR:2;
    UINT32                            LC_SRIS_AUTODETECT_MODE:2;
    UINT32                            LC_SRIS_AUTODETECT_OUT_OF_RANGE:1;
    UINT32                            LC_OVERRIDE_RETIMER_PRESENCE_EN:1;
    UINT32                            LC_OVERRIDE_RETIMER_PRESENCE:2;
    UINT32                            LC_IGNORE_RETIMER_PRESENCE:1;
    UINT32                            LC_RETIMER_PRESENCE:2;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL6_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL6_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL6_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL6_OFFSET       0x2ec
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL6_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x2ec)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL6_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL6_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL6_OFFSET       0x2ec
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL6_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x2ec)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL6_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL6_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL6_OFFSET       0x2ec
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL6_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x2ec)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL6_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL6_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL6_OFFSET       0x2ec
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL6_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x2ec)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL6_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL6_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL6_OFFSET       0x2ec
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL6_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x2ec)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL6_ADDRESS    0x111402ecUL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112402ecUL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL6_ADDRESS    0x113402ecUL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL6_ADDRESS    0x114402ecUL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL6_ADDRESS    0x111412ecUL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112412ecUL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL6_ADDRESS    0x111422ecUL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112422ecUL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL6_ADDRESS    0x111432ecUL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112432ecUL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL6_ADDRESS    0x111442ecUL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112442ecUL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL6_ADDRESS    0x112452ecUL


/***********************************************************
* Register Name : PCIE_LC_CNTL9
* Register Description :
* Link Controller Control 9 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if the receiver detection is reset on Polling.Active to Detect_Idle, or Rcvd_Disable to Detect.Quiet, or Recovery.Speed to Detect.Quiet transitions.
#define PCIE_LC_CNTL9_LC_RESET_RCVR_DETECTED_ALL_ARCS_OFFSET      0
#define PCIE_LC_CNTL9_LC_RESET_RCVR_DETECTED_ALL_ARCS_MASK        0x1

// Bitfield Description : Controls if the assertion of MAC_PCS_TxDetectRx_Loopback waits for symbol lock on all lanes that received data in Polling.Active.
#define PCIE_LC_CNTL9_LC_LOOPBACK_WAIT_FOR_ALL_ACTIVE_LANES_OFFSET      1
#define PCIE_LC_CNTL9_LC_LOOPBACK_WAIT_FOR_ALL_ACTIVE_LANES_MASK        0x2

// Bitfield Description : Controls if the first TS1s received with any EC value will block any further coefficient changes upon entering Loopback at 8.0 GT/s or higher.
#define PCIE_LC_CNTL9_LC_CHECK_EC_GEN3_LOOPBACK_ACTIVE_OFFSET      2
#define PCIE_LC_CNTL9_LC_CHECK_EC_GEN3_LOOPBACK_ACTIVE_MASK        0x4

// Bitfield Description : Controls if entering Loopback with Equalization is allowed.
#define PCIE_LC_CNTL9_LC_LOOPBACK_EQ_ARC_EN_OFFSET      3
#define PCIE_LC_CNTL9_LC_LOOPBACK_EQ_ARC_EN_MASK        0x8

// Bitfield Description : When Loopback with Equalization is entered, controls if the Modified Compliance Pattern is sent.
#define PCIE_LC_CNTL9_LC_LOOPBACK_EQ_TRANSMIT_MOD_COMP_PATTERN_EN_OFFSET      4
#define PCIE_LC_CNTL9_LC_LOOPBACK_EQ_TRANSMIT_MOD_COMP_PATTERN_EN_MASK        0x10

// Bitfield Description : Controls if L1_SUBSTATE_CLK_PDWN is prevented from re-asserting during L1 wake-up.
#define PCIE_LC_CNTL9_LC_ENFORCE_SINGLE_L1_SUBSTATE_CLK_PDWN_ASSERTION_EN_OFFSET      5
#define PCIE_LC_CNTL9_LC_ENFORCE_SINGLE_L1_SUBSTATE_CLK_PDWN_ASSERTION_EN_MASK        0x20

// Bitfield Description : (Downstream Ports only) Controls if Port waits for TS1 to be received, before incrementing common mode counters, upon ASPM L1.2 to Recovery.RcvrLock transition.
#define PCIE_LC_CNTL9_LC_EXT_ASPM_L12_COMMONMODE_COUNT_METHOD_OFFSET      6
#define PCIE_LC_CNTL9_LC_EXT_ASPM_L12_COMMONMODE_COUNT_METHOD_MASK        0x40

// Bitfield Description : Controls when RxEqInProgress is used to indicate that a Link partner's Tx settings may be changing.
#define PCIE_LC_CNTL9_LC_ALT_RX_EQ_IN_PROGRESS_EN_OFFSET      7
#define PCIE_LC_CNTL9_LC_ALT_RX_EQ_IN_PROGRESS_EN_MASK        0x80

// Bitfield Description : Controls if longer timeout values are used for Recovery.RcvrLock and Config_Step{1-4}. Not to be used for normal operation, but should only be used for serial simulations.
#define PCIE_LC_CNTL9_LC_USE_LONG_SERIAL_QUICKSIM_TIMEOUTS_OFFSET      8
#define PCIE_LC_CNTL9_LC_USE_LONG_SERIAL_QUICKSIM_TIMEOUTS_MASK        0x100

// Bitfield Description : Controls if DLLPs are allowed to be sent if autonomous Link Equalization is not yet complete, due to the Link partner having initially advertised 8.0 GT/s or higher, then having removed that advertisement before Link Equalization at the highest initially advertised data rate is complete.
#define PCIE_LC_CNTL9_LC_ALLOW_DLLPS_OTHER_SIDE_REMOVE_SPEED_OFFSET      9
#define PCIE_LC_CNTL9_LC_ALLOW_DLLPS_OTHER_SIDE_REMOVE_SPEED_MASK        0x200

// Bitfield Description : When LC_REQ_COEFFS_FOR_TXMARGIN_EN bit is 1 and the TxMargin setting is changed, controls if a speed change to 8.0 GT/s or higher in Polling.Compliance is delayed until all new preset coefficients are fetched.
#define PCIE_LC_CNTL9_LC_DELAY_POLL_COMP_SPD_CHG_AFTER_TXMARGIN_OFFSET      10
#define PCIE_LC_CNTL9_LC_DELAY_POLL_COMP_SPD_CHG_AFTER_TXMARGIN_MASK        0x400

// Bitfield Description : Controls whether the selection between regular SKPs and CTL-SKPs is reset on entry to s_Training_Bit.
#define PCIE_LC_CNTL9_LC_RESET_SKP_SELECT_16GT_ON_TRAINING_BIT_OFFSET      11
#define PCIE_LC_CNTL9_LC_RESET_SKP_SELECT_16GT_ON_TRAINING_BIT_MASK        0x800

// Bitfield Description : Controls the number of consecutive sets with matching Training Control bits that must be received in Configuration and Recovery.Idle prior to transition to training state.
#define PCIE_LC_CNTL9_LC_TRAINING_BITS_REQUIRED_OFFSET      12
#define PCIE_LC_CNTL9_LC_TRAINING_BITS_REQUIRED_MASK        0x3000

// Bitfield Description : When LC_RXEQEVAL_AFTER_TIMEOUT_EN bit is 1 and LC_AUTO_REJECT_AFTER_TIMEOUT bit is 0, controls if a second adaption is performed if symbol lock is recovered after the initial adaption was issued and the Link partner is accepting the request.
#define PCIE_LC_CNTL9_LC_REPEAT_RXEQEVAL_AFTER_TIMEOUT_OFFSET      14
#define PCIE_LC_CNTL9_LC_REPEAT_RXEQEVAL_AFTER_TIMEOUT_MASK        0x4000

// Bitfield Description : Controls if this Port's CHIP_PCS_refClkReq/refClkAck handshake is checked before informing the CPM that the ltssm is idle.
#define PCIE_LC_CNTL9_LC_CPM_IDLE_REFCLKREQ_CHECK_OFFSET      15
#define PCIE_LC_CNTL9_LC_CPM_IDLE_REFCLKREQ_CHECK_MASK        0x8000

// Bitfield Description : Controls if refClkReq is de-asserted on lanes if they are turned off due to not detecting a receiver.
#define PCIE_LC_CNTL9_LC_REFCLK_OFF_NO_RCVR_LANES_OFFSET      16
#define PCIE_LC_CNTL9_LC_REFCLK_OFF_NO_RCVR_LANES_MASK        0x10000

// Bitfield Description : Controls if this Port's CHIP_PCS_refClkReq is generated only using per-Port signals and can be de-asserted independently of other Ports. All Ports with this bit disabled will only de-assert CHIP_PCS_refClkReq if all Ports are able to.
#define PCIE_LC_CNTL9_LC_INDEPENDENT_CHIP_PCS_REFCLKREQ_EN_OFFSET      17
#define PCIE_LC_CNTL9_LC_INDEPENDENT_CHIP_PCS_REFCLKREQ_EN_MASK        0x20000

// Bitfield Description : Controls if the LC can de-assert refClkReq in Detect once all PowerDown changes are completed in HoldTraining. This setting should only be used for ports that will be held statically in Detect. Once the refClkReq is de-asserted, internal clocks may be gated until HoldTraining is deasserted.
#define PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_OFFSET      18
#define PCIE_LC_CNTL9_LC_REFCLKREQ_IN_HOLD_TRAINING_MASK        0x40000

// Bitfield Description : Controls if refClkReq can be de-asserted in L1 if substates will not be entered.
#define PCIE_LC_CNTL9_LC_DEASSERT_REFCLKREQ_IN_NON_SS_L1_OFFSET      19
#define PCIE_LC_CNTL9_LC_DEASSERT_REFCLKREQ_IN_NON_SS_L1_MASK        0x80000

// Bitfield Description : Controls if the de-assertion of RefClkReq is delayed until after the PowerDown mode is set to its lowest power state in L1 substates.
#define PCIE_LC_CNTL9_LC_HOLD_REFCLKREQ_UNTIL_L1SS_POWERDOWN_OFFSET      20
#define PCIE_LC_CNTL9_LC_HOLD_REFCLKREQ_UNTIL_L1SS_POWERDOWN_MASK        0x100000

// Bitfield Description : Controls if the LTSSM does not allow TXCLK_DYN to be gated until the refClkReq and refClkAck handshake with the PHY has finished.
#define PCIE_LC_CNTL9_LC_CLKGATE_WAIT_FOR_REFCLKACK_OFFSET      21
#define PCIE_LC_CNTL9_LC_CLKGATE_WAIT_FOR_REFCLKACK_MASK        0x200000

// Bitfield Description : Controls if dynamic inactive lanes follow the PowerDown setting of the active lanes in L1 substates.
#define PCIE_LC_CNTL9_LC_DYN_LANES_L1_SS_POWERDOWN_OFFSET      22
#define PCIE_LC_CNTL9_LC_DYN_LANES_L1_SS_POWERDOWN_MASK        0x400000

// Bitfield Description : Controls mode for changing PowerDown on dynamic inactive lanes.
#define PCIE_LC_CNTL9_LC_USE_OLD_PHYSTATUS_FOR_POWERDOWN_INACTIVE_OFFSET      23
#define PCIE_LC_CNTL9_LC_USE_OLD_PHYSTATUS_FOR_POWERDOWN_INACTIVE_MASK        0x800000

// Bitfield Description : Controls if L0s entry is delayed until all unused lanes are powered down and no L1 entry is pending.
#define PCIE_LC_CNTL9_LC_BLOCK_L0s_FOR_POWERDOWN_CHANGE_OFFSET      24
#define PCIE_LC_CNTL9_LC_BLOCK_L0s_FOR_POWERDOWN_CHANGE_MASK        0x1000000

// Bitfield Description : Controls if the LTSSM waits for the ASPM NAK to be transmitted before going to Recovery, when an ASPM NAK is generated shortly before the Link is directed to Recovery.
#define PCIE_LC_CNTL9_LC_RECOVERY_WAIT_FOR_ASPM_NAK_OFFSET      25
#define PCIE_LC_CNTL9_LC_RECOVERY_WAIT_FOR_ASPM_NAK_MASK        0x2000000

// Bitfield Description : (Upstream Ports only) Controls if the LTSSM waits for a non-PAD Link number on logical Lane 0 before transitioning from Config_Step2 to Config_Step2b, ensuring that the received Link number is not lost due to lane to lane skew.
#define PCIE_LC_CNTL9_LC_WAIT_FOR_NONPAD_LINK_NUM_LANE0_OFFSET      26
#define PCIE_LC_CNTL9_LC_WAIT_FOR_NONPAD_LINK_NUM_LANE0_MASK        0x4000000

// Bitfield Description : Controls whether any previously received Link/Lane numbers are cleared when entering Configuration on lanes that did not receive training sets in the prior state.
#define PCIE_LC_CNTL9_LC_CLR_LINK_LANE_NUM_ON_NO_TSX_LANE_OFFSET      27
#define PCIE_LC_CNTL9_LC_CLR_LINK_LANE_NUM_ON_NO_TSX_LANE_MASK        0x8000000

// Bitfield Description : Controls if the selection for sending EQ TS1s uses the new mode.
#define PCIE_LC_CNTL9_LC_USE_NEW_EQ_SYMBOL_6_EN_OFFSET      28
#define PCIE_LC_CNTL9_LC_USE_NEW_EQ_SYMBOL_6_EN_MASK        0x10000000

// Bitfield Description : Controls if the counters used by LC_SPEED_CHANGE_ATTEMPT_FAILED and LC_AUTO_SPEED_CHANGE_ATTEMPT_FAILED fields are decremented by one when the Bypass Equalization to Highest Rate is initially negotiated, but then is subsequently aborted by a failed speed change to the highest data rate. This excludes the aborted bypass attempt from counting against the failed speed change limit.
#define PCIE_LC_CNTL9_LC_DEC_FAILED_SPEED_CHANGE_COUNT_ABORT_BYPASS_TO_HIGH_RATE_OFFSET      29
#define PCIE_LC_CNTL9_LC_DEC_FAILED_SPEED_CHANGE_COUNT_ABORT_BYPASS_TO_HIGH_RATE_MASK        0x20000000

// Bitfield Description : When doing a link width reconfiguration at 8.0 GT/s or higher, controls if the TS1 counters wait until an EIEOS is seen on a lane before beginning to count on that lane.
#define PCIE_LC_CNTL9_LC_CONFIG_WAIT_FOR_EIEOS_OFFSET      30
#define PCIE_LC_CNTL9_LC_CONFIG_WAIT_FOR_EIEOS_MASK        0x40000000

// Bitfield Description : Controls if a TLP to be transmitted, that is dropped while in L1, causes a full wake up from L1 substates.
#define PCIE_LC_CNTL9_LC_HOLD_TLP_TO_XMIT_PULSE_IN_L1_OFFSET      31
#define PCIE_LC_CNTL9_LC_HOLD_TLP_TO_XMIT_PULSE_IN_L1_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LC_RESET_RCVR_DETECTED_ALL_ARCS:1;
    UINT32                            LC_LOOPBACK_WAIT_FOR_ALL_ACTIVE_LANES:1;
    UINT32                            LC_CHECK_EC_GEN3_LOOPBACK_ACTIVE:1;
    UINT32                            LC_LOOPBACK_EQ_ARC_EN:1;
    UINT32                            LC_LOOPBACK_EQ_TRANSMIT_MOD_COMP_PATTERN_EN:1;
    UINT32                            LC_ENFORCE_SINGLE_L1_SUBSTATE_CLK_PDWN_ASSERTION_EN:1;
    UINT32                            LC_EXT_ASPM_L12_COMMONMODE_COUNT_METHOD:1;
    UINT32                            LC_ALT_RX_EQ_IN_PROGRESS_EN:1;
    UINT32                            LC_USE_LONG_SERIAL_QUICKSIM_TIMEOUTS:1;
    UINT32                            LC_ALLOW_DLLPS_OTHER_SIDE_REMOVE_SPEED:1;
    UINT32                            LC_DELAY_POLL_COMP_SPD_CHG_AFTER_TXMARGIN:1;
    UINT32                            LC_RESET_SKP_SELECT_16GT_ON_TRAINING_BIT:1;
    UINT32                            LC_TRAINING_BITS_REQUIRED:2;
    UINT32                            LC_REPEAT_RXEQEVAL_AFTER_TIMEOUT:1;
    UINT32                            LC_CPM_IDLE_REFCLKREQ_CHECK:1;
    UINT32                            LC_REFCLK_OFF_NO_RCVR_LANES:1;
    UINT32                            LC_INDEPENDENT_CHIP_PCS_REFCLKREQ_EN:1;
    UINT32                            LC_REFCLKREQ_IN_HOLD_TRAINING:1;
    UINT32                            LC_DEASSERT_REFCLKREQ_IN_NON_SS_L1:1;
    UINT32                            LC_HOLD_REFCLKREQ_UNTIL_L1SS_POWERDOWN:1;
    UINT32                            LC_CLKGATE_WAIT_FOR_REFCLKACK:1;
    UINT32                            LC_DYN_LANES_L1_SS_POWERDOWN:1;
    UINT32                            LC_USE_OLD_PHYSTATUS_FOR_POWERDOWN_INACTIVE:1;
    UINT32                            LC_BLOCK_L0s_FOR_POWERDOWN_CHANGE:1;
    UINT32                            LC_RECOVERY_WAIT_FOR_ASPM_NAK:1;
    UINT32                            LC_WAIT_FOR_NONPAD_LINK_NUM_LANE0:1;
    UINT32                            LC_CLR_LINK_LANE_NUM_ON_NO_TSX_LANE:1;
    UINT32                            LC_USE_NEW_EQ_SYMBOL_6_EN:1;
    UINT32                            LC_DEC_FAILED_SPEED_CHANGE_COUNT_ABORT_BYPASS_TO_HIGH_RATE:1;
    UINT32                            LC_CONFIG_WAIT_FOR_EIEOS:1;
    UINT32                            LC_HOLD_TLP_TO_XMIT_PULSE_IN_L1:1;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL9_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL9_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL9_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL9_OFFSET       0x378
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL9_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x378)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL9_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL9_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL9_OFFSET       0x378
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL9_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x378)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL9_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL9_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL9_OFFSET       0x378
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL9_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x378)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL9_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL9_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL9_OFFSET       0x378
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL9_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x378)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL9_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL9_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL9_OFFSET       0x378
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL9_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x378)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL9_ADDRESS    0x11140378UL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11240378UL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL9_ADDRESS    0x11340378UL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL9_ADDRESS    0x11440378UL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL9_ADDRESS    0x11141378UL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11241378UL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL9_ADDRESS    0x11142378UL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11242378UL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL9_ADDRESS    0x11143378UL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11243378UL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL9_ADDRESS    0x11144378UL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11244378UL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL9_ADDRESS    0x11245378UL


/***********************************************************
* Register Name : PCIE_LC_EQ_CNTL_8GT
* Register Description :
* Link Controller Equalization Control 8.0 GT/s register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if the LTSSM enters Recovery.Equalization for 8.0 GT/s.
#define PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_8GT_OFFSET      0
#define PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_8GT_MASK        0x1

// Bitfield Description : At 8.0 GT/s, controls the value of Request Equalization bit (symbol 6, bit 7) in any subsequently transmitted TS2s. Setting this bit requests that the Link Equalization procedure be repeated. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_8GT_LC_REDO_EQ_8GT_OFFSET      1
#define PCIE_LC_EQ_CNTL_8GT_LC_REDO_EQ_8GT_MASK        0x2

// Bitfield Description : Controls the 8.0 GT/s Link Equalization search algorithm.
#define PCIE_LC_EQ_CNTL_8GT_LC_EQ_SEARCH_MODE_8GT_OFFSET      2
#define PCIE_LC_EQ_CNTL_8GT_LC_EQ_SEARCH_MODE_8GT_MASK        0xc

// Bitfield Description : When LC_EQ_SEARCH_MODE_8GT field is 3, controls the 8.0 GT/s enhanced Preset Search algorithm.
#define PCIE_LC_EQ_CNTL_8GT_LC_ENH_PRESET_SEARCH_SEL_8GT_OFFSET      4
#define PCIE_LC_EQ_CNTL_8GT_LC_ENH_PRESET_SEARCH_SEL_8GT_MASK        0x30

// Bitfield Description : (Downstream Ports only) Controls if 8.0 GT/s Link Equalization Phase 2 and Phase 3 is attempted.
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_EQ_NOT_REQD_8GT_OFFSET      6
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_EQ_NOT_REQD_8GT_MASK        0x40

// Bitfield Description : (Downstream Ports only) In 8.0 GT/s, force the Link to Recovery.Equalization. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_GO_TO_EQ_8GT_OFFSET      7
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_GO_TO_EQ_8GT_MASK        0x80

// Bitfield Description : Indicates if unsupported presets or coefficients were received in Recovery.RcvrLock when entered after Recovery.Equalization, at 8.0 GT/s. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_8GT_LC_UNEXPECTED_COEFFS_RCVD_8GT_OFFSET      8
#define PCIE_LC_EQ_CNTL_8GT_LC_UNEXPECTED_COEFFS_RCVD_8GT_MASK        0x100

// Bitfield Description : Controls if LTSSM enters the requesting Phase of 8.0 GT/s Link Equalization (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_REQ_PHASE_8GT_OFFSET      9
#define PCIE_LC_EQ_CNTL_8GT_LC_BYPASS_EQ_REQ_PHASE_8GT_MASK        0x200

// Bitfield Description : Controls if only the preset value specified in LC_FORCE_PRESET_VALUE_8GT field is requested, when in 8.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_8GT_LC_FORCE_PRESET_IN_EQ_REQ_PHASE_8GT_OFFSET      10
#define PCIE_LC_EQ_CNTL_8GT_LC_FORCE_PRESET_IN_EQ_REQ_PHASE_8GT_MASK        0x400

// Bitfield Description : When LC_FORCE_PRESET_IN_EQ_REQ_PHASE bit is 1, controls the preset value to be requested, when in 8.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_8GT_LC_FORCE_PRESET_VALUE_8GT_OFFSET      11
#define PCIE_LC_EQ_CNTL_8GT_LC_FORCE_PRESET_VALUE_8GT_MASK        0x7800

// Bitfield Description : When coefficient exhaustive search is selected, controls the exhaustive search algorithm, when in 8.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_8GT_LC_SAFE_EQ_SEARCH_8GT_OFFSET      15
#define PCIE_LC_EQ_CNTL_8GT_LC_SAFE_EQ_SEARCH_8GT_MASK        0x8000

// Bitfield Description : (Downstream Ports only) Controls if 8.0 GT/s Link Equalization is re-done autonomously before 16.0 GT/s Link Equalization is done, if either end detected issues with the 8.0 GT/s Link Equalization.
#define PCIE_LC_EQ_CNTL_8GT_LC_8GT_EQ_REDO_EN_OFFSET      16
#define PCIE_LC_EQ_CNTL_8GT_LC_8GT_EQ_REDO_EN_MASK        0x10000

// Bitfield Description : (Upstream Ports only) Controls if the Port replies to an 8.0 GT/s Link Equalization redo request by advertising support for 8.0 GT/s (if possible).
#define PCIE_LC_EQ_CNTL_8GT_LC_DSC_ACCEPT_8GT_EQ_REDO_OFFSET      17
#define PCIE_LC_EQ_CNTL_8GT_LC_DSC_ACCEPT_8GT_EQ_REDO_MASK        0x20000

// Bitfield Description : (Downstream Ports only) Controls if the Port will redo 8.0 GT/s Link Equalization autonomously (once) if the other side requests it. This bit is cleared when the 8.0 GT/s redo is performed.
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_HW_8GT_EQ_REDO_EN_OFFSET      18
#define PCIE_LC_EQ_CNTL_8GT_LC_USC_HW_8GT_EQ_REDO_EN_MASK        0x40000

// Bitfield Description : Controls if the local preset conversion to coefficient values for Gen3 are performed, even if Gen3 is disabled by strap.
#define PCIE_LC_EQ_CNTL_8GT_LC_ALWAYS_PERFORM_GEN3_PRESET_CONVERSION_OFFSET      19
#define PCIE_LC_EQ_CNTL_8GT_LC_ALWAYS_PERFORM_GEN3_PRESET_CONVERSION_MASK        0x80000

// Bitfield Description :
#define PCIE_LC_EQ_CNTL_8GT_Reserved_31_20_OFFSET      20
#define PCIE_LC_EQ_CNTL_8GT_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            LC_BYPASS_EQ_8GT:1;
    UINT32                            LC_REDO_EQ_8GT:1;
    UINT32                            LC_EQ_SEARCH_MODE_8GT:2;
    UINT32                            LC_ENH_PRESET_SEARCH_SEL_8GT:2;
    UINT32                            LC_USC_EQ_NOT_REQD_8GT:1;
    UINT32                            LC_USC_GO_TO_EQ_8GT:1;
    UINT32                            LC_UNEXPECTED_COEFFS_RCVD_8GT:1;
    UINT32                            LC_BYPASS_EQ_REQ_PHASE_8GT:1;
    UINT32                            LC_FORCE_PRESET_IN_EQ_REQ_PHASE_8GT:1;
    UINT32                            LC_FORCE_PRESET_VALUE_8GT:4;
    UINT32                            LC_SAFE_EQ_SEARCH_8GT:1;
    UINT32                            LC_8GT_EQ_REDO_EN:1;
    UINT32                            LC_DSC_ACCEPT_8GT_EQ_REDO:1;
    UINT32                            LC_USC_HW_8GT_EQ_REDO_EN:1;
    UINT32                            LC_ALWAYS_PERFORM_GEN3_PRESET_CONVERSION:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} PCIE_LC_EQ_CNTL_8GT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_8GT_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_8GT_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_8GT_OFFSET       0x390
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x390)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_8GT_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_8GT_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_8GT_OFFSET       0x390
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x390)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_8GT_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_8GT_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_8GT_OFFSET       0x390
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x390)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_8GT_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_8GT_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_8GT_OFFSET       0x390
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x390)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_8GT_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_8GT_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_8GT_OFFSET       0x390
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x390)

#define SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11140390UL

#define SMN_FUNC0_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11240390UL

#define SMN_FUNC0_PCIE2_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11340390UL

#define SMN_FUNC0_PCIE3_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11440390UL

#define SMN_FUNC1_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11141390UL

#define SMN_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11241390UL

#define SMN_FUNC2_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11142390UL

#define SMN_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11242390UL

#define SMN_FUNC3_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11143390UL

#define SMN_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11243390UL

#define SMN_FUNC4_PCIE0_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11144390UL

#define SMN_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11244390UL

#define SMN_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_8GT_ADDRESS    0x11245390UL

/***********************************************************
* Register Name : PCIE_LC_EQ_CNTL_16GT
* Register Description :
* Link Controller Equalization Control 16.0 GT/s register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if the LTSSM enters Recovery.Equalization for 16.0 GT/s.
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_16GT_OFFSET      0
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_16GT_MASK        0x1

// Bitfield Description : At 16.0 GT/s, controls the value of Request Equalization bit (symbol 6, bit 7) in any subsequently transmitted TS2s. Setting this bit requests that the Link Equalization procedure be repeated. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_16GT_LC_REDO_EQ_16GT_OFFSET      1
#define PCIE_LC_EQ_CNTL_16GT_LC_REDO_EQ_16GT_MASK        0x2

// Bitfield Description : Controls the 16.0 GT/s Link Equalization search algorithm.
#define PCIE_LC_EQ_CNTL_16GT_LC_EQ_SEARCH_MODE_16GT_OFFSET      2
#define PCIE_LC_EQ_CNTL_16GT_LC_EQ_SEARCH_MODE_16GT_MASK        0xc

// Bitfield Description : When LC_EQ_SEARCH_MODE_16GT field is 3, controls the 16.0 GT/s enhanced Preset Search algorithm.
#define PCIE_LC_EQ_CNTL_16GT_LC_ENH_PRESET_SEARCH_SEL_16GT_OFFSET      4
#define PCIE_LC_EQ_CNTL_16GT_LC_ENH_PRESET_SEARCH_SEL_16GT_MASK        0x30

// Bitfield Description : (Downstream Ports only) Controls if 16.0 GT/s Link Equalization Phase 2 and Phase 3 is attempted.
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_EQ_NOT_REQD_16GT_OFFSET      6
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_EQ_NOT_REQD_16GT_MASK        0x40

// Bitfield Description : (Downstream Ports only) In 16.0 GT/s, force the Link to Recovery.Equalization. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_GO_TO_EQ_16GT_OFFSET      7
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_GO_TO_EQ_16GT_MASK        0x80

// Bitfield Description : Indicates if unsupported presets or coefficients were received in Recovery.RcvrLock when entered after Recovery.Equalization, at 16.0 GT/s. This bit is automatically cleared upon entry to Recovery.Equalization.
#define PCIE_LC_EQ_CNTL_16GT_LC_UNEXPECTED_COEFFS_RCVD_16GT_OFFSET      8
#define PCIE_LC_EQ_CNTL_16GT_LC_UNEXPECTED_COEFFS_RCVD_16GT_MASK        0x100

// Bitfield Description : Controls if LTSSM enters the requesting Phase of 16.0 GT/s Link Equalization (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_REQ_PHASE_16GT_OFFSET      9
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_REQ_PHASE_16GT_MASK        0x200

// Bitfield Description : Controls if only the preset value specified in LC_FORCE_PRESET_VALUE_16GT field is requested, when in 16.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_16GT_LC_FORCE_PRESET_IN_EQ_REQ_PHASE_16GT_OFFSET      10
#define PCIE_LC_EQ_CNTL_16GT_LC_FORCE_PRESET_IN_EQ_REQ_PHASE_16GT_MASK        0x400

// Bitfield Description : When LC_FORCE_PRESET_IN_EQ_REQ_PHASE_16GT bit is 1, controls the preset value to be requested, when in 8.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_16GT_LC_FORCE_PRESET_VALUE_16GT_OFFSET      11
#define PCIE_LC_EQ_CNTL_16GT_LC_FORCE_PRESET_VALUE_16GT_MASK        0x7800

// Bitfield Description : When coefficient exhaustive search is selected, controls the exhaustive search algorithm, when in 16.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_EQ_CNTL_16GT_LC_SAFE_EQ_SEARCH_16GT_OFFSET      15
#define PCIE_LC_EQ_CNTL_16GT_LC_SAFE_EQ_SEARCH_16GT_MASK        0x8000

// Bitfield Description : (Downstream Ports only) Controls if 16.0 GT/s Link Equalization is re-done autonomously before 32.0 GT/s Link Equalization is done, if either end detected issues with the 16.0 GT/s Link Equalization.
#define PCIE_LC_EQ_CNTL_16GT_LC_16GT_EQ_REDO_EN_OFFSET      16
#define PCIE_LC_EQ_CNTL_16GT_LC_16GT_EQ_REDO_EN_MASK        0x10000

// Bitfield Description : When LC_BYPASS_EQ_16GT bit is 1, controls the preset value to be used at 16.0 GT/s data rate.
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_PRESET_16GT_OFFSET      17
#define PCIE_LC_EQ_CNTL_16GT_LC_BYPASS_EQ_PRESET_16GT_MASK        0x1e0000

// Bitfield Description : (Upstream Ports only) Controls if the Port replies to an 16.0 GT/s Link Equalization redo request by advertising support for 16.0 GT/s (if possible).
#define PCIE_LC_EQ_CNTL_16GT_LC_DSC_ACCEPT_16GT_EQ_REDO_OFFSET      21
#define PCIE_LC_EQ_CNTL_16GT_LC_DSC_ACCEPT_16GT_EQ_REDO_MASK        0x200000

// Bitfield Description : (Downstream Ports only) Controls if the Port will redo 16.0 GT/s Link Equalization autonomously (once) if the other side requests it. This bit is cleared when the 16.0 GT/s redo is performed.
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_HW_16GT_EQ_REDO_EN_OFFSET      22
#define PCIE_LC_EQ_CNTL_16GT_LC_USC_HW_16GT_EQ_REDO_EN_MASK        0x400000

// Bitfield Description : (Upstream Ports only) Enables transmission in Recovery.RcvrCfg at the appropriate data rate of the preset value specified in LC_EQTS2_PRESET_16GT field.
#define PCIE_LC_EQ_CNTL_16GT_LC_EQTS2_PRESET_EN_16GT_OFFSET      23
#define PCIE_LC_EQ_CNTL_16GT_LC_EQTS2_PRESET_EN_16GT_MASK        0x800000

// Bitfield Description : (Upstream Ports only) When LC_EQTS2_PRESET_EN_16GT bit is 1, controls the preset value requested in either EQ TS2s or 128b/130b EQ TS2s (as appropriate) that are transmitted in Recovery.RcvrCfg at the data rate from which the speed change to perform Link Equalization at 16.0 GT/s will be performed.
#define PCIE_LC_EQ_CNTL_16GT_LC_EQTS2_PRESET_16GT_OFFSET      24
#define PCIE_LC_EQ_CNTL_16GT_LC_EQTS2_PRESET_16GT_MASK        0xf000000

// Bitfield Description : (Downstream Ports only) Controls (along with LC_USC_ACCEPTABLE_PRESETS field) if the preset values received in either EQ TS2 or 128b/130b EQ TS2s (as appropriate) are used for the initial speed change to 16.0 GT/s data rate.
#define PCIE_LC_EQ_CNTL_16GT_LC_USE_EQTS2_PRESET_16GT_OFFSET      28
#define PCIE_LC_EQ_CNTL_16GT_LC_USE_EQTS2_PRESET_16GT_MASK        0x10000000

// Bitfield Description : Controls if the local preset conversion to coefficient values for Gen4 are performed, even if Gen4 is disabled by strap.
#define PCIE_LC_EQ_CNTL_16GT_LC_ALWAYS_PERFORM_GEN4_PRESET_CONVERSION_OFFSET      29
#define PCIE_LC_EQ_CNTL_16GT_LC_ALWAYS_PERFORM_GEN4_PRESET_CONVERSION_MASK        0x20000000

// Bitfield Description :
#define PCIE_LC_EQ_CNTL_16GT_Reserved_31_30_OFFSET      30
#define PCIE_LC_EQ_CNTL_16GT_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LC_BYPASS_EQ_16GT:1;
    UINT32                            LC_REDO_EQ_16GT:1;
    UINT32                            LC_EQ_SEARCH_MODE_16GT:2;
    UINT32                            LC_ENH_PRESET_SEARCH_SEL_16GT:2;
    UINT32                            LC_USC_EQ_NOT_REQD_16GT:1;
    UINT32                            LC_USC_GO_TO_EQ_16GT:1;
    UINT32                            LC_UNEXPECTED_COEFFS_RCVD_16GT:1;
    UINT32                            LC_BYPASS_EQ_REQ_PHASE_16GT:1;
    UINT32                            LC_FORCE_PRESET_IN_EQ_REQ_PHASE_16GT:1;
    UINT32                            LC_FORCE_PRESET_VALUE_16GT:4;
    UINT32                            LC_SAFE_EQ_SEARCH_16GT:1;
    UINT32                            LC_16GT_EQ_REDO_EN:1;
    UINT32                            LC_BYPASS_EQ_PRESET_16GT:4;
    UINT32                            LC_DSC_ACCEPT_16GT_EQ_REDO:1;
    UINT32                            LC_USC_HW_16GT_EQ_REDO_EN:1;
    UINT32                            LC_EQTS2_PRESET_EN_16GT:1;
    UINT32                            LC_EQTS2_PRESET_16GT:4;
    UINT32                            LC_USE_EQTS2_PRESET_16GT:1;
    UINT32                            LC_ALWAYS_PERFORM_GEN4_PRESET_CONVERSION:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_LC_EQ_CNTL_16GT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_16GT_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_16GT_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_16GT_OFFSET       0x394
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x394)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_16GT_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_16GT_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_16GT_OFFSET       0x394
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x394)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_16GT_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_16GT_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_16GT_OFFSET       0x394
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x394)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_16GT_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_16GT_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_16GT_OFFSET       0x394
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x394)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_16GT_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_16GT_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_16GT_OFFSET       0x394
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x394)

#define SMN_FUNC0_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11140394UL

#define SMN_FUNC0_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11240394UL

#define SMN_FUNC0_PCIE2_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11340394UL

#define SMN_FUNC0_PCIE3_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11440394UL

#define SMN_FUNC1_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11141394UL

#define SMN_FUNC1_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11241394UL

#define SMN_FUNC2_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11142394UL

#define SMN_FUNC2_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11242394UL

#define SMN_FUNC3_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11143394UL

#define SMN_FUNC3_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11243394UL

#define SMN_FUNC4_PCIE0_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11144394UL

#define SMN_FUNC4_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11244394UL

#define SMN_FUNC5_PCIE1_PCIE_LC_EQ_CNTL_16GT_ADDRESS    0x11245394UL

/***********************************************************
* Register Name : PCIE_LC_CNTL4
* Register Description :
* Link Controller Control 4 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls extending TX_ENABLE for an extra symbol time before and/or after the default behavior, to account for any PHY corruption.
#define PCIE_LC_CNTL4_LC_TX_ENABLE_BEHAVIOUR_OFFSET      0
#define PCIE_LC_CNTL4_LC_TX_ENABLE_BEHAVIOUR_MASK        0x3

// Bitfield Description : Controls checking for contiguous sets received in Recovery.RcvrCfg or Configuration.Complete, when these states precede logical idle states such as Recovery.Idle or Configuration.Idle.
#define PCIE_LC_CNTL4_LC_DIS_CONTIG_END_SET_CHECK_OFFSET      2
#define PCIE_LC_CNTL4_LC_DIS_CONTIG_END_SET_CHECK_MASK        0x4

// Bitfield Description : Controls if ASPM L1 entry is allowed during a link speed change.
#define PCIE_LC_CNTL4_LC_DIS_ASPM_L1_IN_SPEED_CHANGE_OFFSET      3
#define PCIE_LC_CNTL4_LC_DIS_ASPM_L1_IN_SPEED_CHANGE_MASK        0x8

// Bitfield Description : Controls the PowerDown value in L1.0.
#define PCIE_LC_CNTL4_LC_L1_POWERDOWN_OFFSET      4
#define PCIE_LC_CNTL4_LC_L1_POWERDOWN_MASK        0x10

// Bitfield Description : Controls the PowerDown traversal path into and out of P2.
#define PCIE_LC_CNTL4_LC_P2_ENTRY_OFFSET      5
#define PCIE_LC_CNTL4_LC_P2_ENTRY_MASK        0x20

// Bitfield Description : Controls the value of Reset EIEOS Interval Count bit (symbol 6, bit 2) in any subsequently transmitted TS1s, during the requesting Phase of Link Equalization (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port). Setting this bit causes the Link partner to extend its EIEOS transmission interval to 65536 TS1s during that phase.
#define PCIE_LC_CNTL4_LC_EXTEND_EIEOS_OFFSET      6
#define PCIE_LC_CNTL4_LC_EXTEND_EIEOS_MASK        0x40

// Bitfield Description : When LC_EXTEND_EIEOS bit is 1, controls when the Reset EIEOS Interval Count bit (symbol 6, bit 2) is asserted in any subsequently transmitted TS1s, during the requesting Phase of Link Equalization (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port).
#define PCIE_LC_CNTL4_LC_EXTEND_EIEOS_MODE_OFFSET      7
#define PCIE_LC_CNTL4_LC_EXTEND_EIEOS_MODE_MASK        0x80

// Bitfield Description : At 8.0 GT/s, controls if checking Parity (symbol 9, bit 7) of received TS1s.
#define PCIE_LC_CNTL4_LC_IGNORE_PARITY_OFFSET      8
#define PCIE_LC_CNTL4_LC_IGNORE_PARITY_MASK        0x100

// Bitfield Description :
#define PCIE_LC_CNTL4_Reserved_9_9_OFFSET      9
#define PCIE_LC_CNTL4_Reserved_9_9_MASK        0x200

// Bitfield Description : (Upstream Ports only) Controls checking final coefficients requested by Link partner, in Recovery.RcvrLock after Link Equalization is performed.
#define PCIE_LC_CNTL4_LC_DSC_CHECK_COEFFS_IN_RLOCK_OFFSET      10
#define PCIE_LC_CNTL4_LC_DSC_CHECK_COEFFS_IN_RLOCK_MASK        0x400

// Bitfield Description : If a SKP ordered set is scheduled, and LTSSM is transitioning to a state that requires an EIEOS (e.g. Reset_Idle, Rcvd_Disable_Entry), controls deferring SKP ordered set until after EIEOS is sent.
#define PCIE_LC_CNTL4_LC_DEFER_SKIP_FOR_EIEOS_EN_OFFSET      11
#define PCIE_LC_CNTL4_LC_DEFER_SKIP_FOR_EIEOS_EN_MASK        0x800

// Bitfield Description : At greater than 2.5 GT/s, controls if LC waits for at least one EIEOS to be sent in Recovery.RcvrCfg before transitioning to Recovery.Idle.
#define PCIE_LC_CNTL4_LC_SEND_EIEOS_IN_RCFG_OFFSET      12
#define PCIE_LC_CNTL4_LC_SEND_EIEOS_IN_RCFG_MASK        0x1000

// Bitfield Description : At 8.0 GT/s or higher, controls the value of Quiesce bit (symbol 6, bit 6) in any subsequently transmitted TS2s. Setting this bit indicates to the other end that the Port is ready to redo Link Equalization (if required).
#define PCIE_LC_CNTL4_LC_SET_QUIESCE_OFFSET      13
#define PCIE_LC_CNTL4_LC_SET_QUIESCE_MASK        0x2000

// Bitfield Description : At 8.0 GT/s or higher, indicates if TS2s were received with the Quiesce bit (symbol 6, bit 6) set to 1. These TS2s are either received on any lane after transitioning from Recovery.RcvrCfg to Recovery.Speed, or received on all lanes after transitioning from Recovery.RcvrCfg to Recovery.Idle. (Upstream Ports only) This bit is cleared if LC_SET_QUIESCE bit is 1.
#define PCIE_LC_CNTL4_LC_QUIESCE_RCVD_OFFSET      14
#define PCIE_LC_CNTL4_LC_QUIESCE_RCVD_MASK        0x4000

// Bitfield Description : Controls if the symbol trackers wait to see the full sequence of two EIEOS before informing the rest of the LC, when the data rate is 32.0 GT/s.
#define PCIE_LC_CNTL4_LC_WAIT_FOR_TWO_EIEOS_SEQUENCE_OFFSET      15
#define PCIE_LC_CNTL4_LC_WAIT_FOR_TWO_EIEOS_SEQUENCE_MASK        0x8000

// Bitfield Description : Controls delaying the detections of TSxs to ensure all symbols are compared when identical training sets are required.
#define PCIE_LC_CNTL4_LC_DELAY_DETECTED_TSX_RCV_EN_OFFSET      16
#define PCIE_LC_CNTL4_LC_DELAY_DETECTED_TSX_RCV_EN_MASK        0x10000

// Bitfield Description : Controls requiring receiving EQ TS2s in Recovery.RcvrCfg, before allowing transitions to Recovery.Speed (for 8.0 GT/s).
#define PCIE_LC_CNTL4_LC_DONT_CHECK_EQTS_IN_RCFG_OFFSET      17
#define PCIE_LC_CNTL4_LC_DONT_CHECK_EQTS_IN_RCFG_MASK        0x20000

// Bitfield Description : Controls forcing a coefficients (TS1 symbols 6-9) update delay on specific Link Equalization Phase transitions during Preset requests (Phase 1 to Phase 2 transition in an Upstream Port or Phase 2 to Phase 3 in a Downstream Port).
#define PCIE_LC_CNTL4_LC_DELAY_COEFF_UPDATE_DIS_OFFSET      18
#define PCIE_LC_CNTL4_LC_DELAY_COEFF_UPDATE_DIS_MASK        0x40000

// Bitfield Description : Controls increasing the number of TS1s expected at 5.0 GT/s or higher when Link width degraded. Actual number of TS1s is selected by LC_WAIT_FOR_MORE_TS_IN_RLOCK.
#define PCIE_LC_CNTL4_LC_DYNAMIC_INACTIVE_TS_SELECT_OFFSET      19
#define PCIE_LC_CNTL4_LC_DYNAMIC_INACTIVE_TS_SELECT_MASK        0x180000

// Bitfield Description : Controls if LC waits for at least one EIEOS to be sent before allowing exit from Recovery.RcvrLock, when link width is reduced.
#define PCIE_LC_CNTL4_LC_WAIT_FOR_EIEOS_IN_RLOCK_OFFSET      21
#define PCIE_LC_CNTL4_LC_WAIT_FOR_EIEOS_IN_RLOCK_MASK        0x200000

// Bitfield Description : (Downstream Ports only) Controls delaying transmission of DLLPs until after speed change to 8.0 GT/s or higher (if supported) is attempted.
#define PCIE_LC_CNTL4_LC_USC_DELAY_DLLPS_OFFSET      22
#define PCIE_LC_CNTL4_LC_USC_DELAY_DLLPS_MASK        0x400000

// Bitfield Description : Controls the transmitter voltage swing level used by the PHY.
#define PCIE_LC_CNTL4_LC_TX_SWING_OFFSET      23
#define PCIE_LC_CNTL4_LC_TX_SWING_MASK        0x800000

// Bitfield Description : Controls if the coefficient block is prevented from advancing before PHY evaluation is done. Used to prevent errors during Link Equalization when the Link partner does not send same preset or coefficient requests for at least 1 us.
#define PCIE_LC_CNTL4_LC_EQ_WAIT_FOR_EVAL_DONE_OFFSET      24
#define PCIE_LC_CNTL4_LC_EQ_WAIT_FOR_EVAL_DONE_MASK        0x1000000

// Bitfield Description : At 8.0 GT/s, controls ensuring SKP ordered set is not inserted between EDS and EIEOS on transition from L0 to Recovery.
#define PCIE_LC_CNTL4_LC_8GT_SKIP_ORDER_EN_OFFSET      25
#define PCIE_LC_CNTL4_LC_8GT_SKIP_ORDER_EN_MASK        0x2000000

// Bitfield Description : Controls enabling the LC to look for more TSx (the number of TSx is specified by the value in the field) in Recovery.RcvrLock after entering from states where lanes had been powered off. A value of 0 means the feature is disabled. LC looks for the following additional TSx: 4x field value (2.5 GT/s); 8x field value (5.0 GT/s); 16x field value (8.0 GT/s or higher). Valid non-zero values begin at 2.
#define PCIE_LC_CNTL4_LC_WAIT_FOR_MORE_TS_IN_RLOCK_OFFSET      26
#define PCIE_LC_CNTL4_LC_WAIT_FOR_MORE_TS_IN_RLOCK_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            LC_TX_ENABLE_BEHAVIOUR:2;
    UINT32                            LC_DIS_CONTIG_END_SET_CHECK:1;
    UINT32                            LC_DIS_ASPM_L1_IN_SPEED_CHANGE:1;
    UINT32                            LC_L1_POWERDOWN:1;
    UINT32                            LC_P2_ENTRY:1;
    UINT32                            LC_EXTEND_EIEOS:1;
    UINT32                            LC_EXTEND_EIEOS_MODE:1;
    UINT32                            LC_IGNORE_PARITY:1;
    UINT32                            Reserved_9_9:1;
    UINT32                            LC_DSC_CHECK_COEFFS_IN_RLOCK:1;
    UINT32                            LC_DEFER_SKIP_FOR_EIEOS_EN:1;
    UINT32                            LC_SEND_EIEOS_IN_RCFG:1;
    UINT32                            LC_SET_QUIESCE:1;
    UINT32                            LC_QUIESCE_RCVD:1;
    UINT32                            LC_WAIT_FOR_TWO_EIEOS_SEQUENCE:1;
    UINT32                            LC_DELAY_DETECTED_TSX_RCV_EN:1;
    UINT32                            LC_DONT_CHECK_EQTS_IN_RCFG:1;
    UINT32                            LC_DELAY_COEFF_UPDATE_DIS:1;
    UINT32                            LC_DYNAMIC_INACTIVE_TS_SELECT:2;
    UINT32                            LC_WAIT_FOR_EIEOS_IN_RLOCK:1;
    UINT32                            LC_USC_DELAY_DLLPS:1;
    UINT32                            LC_TX_SWING:1;
    UINT32                            LC_EQ_WAIT_FOR_EVAL_DONE:1;
    UINT32                            LC_8GT_SKIP_ORDER_EN:1;
    UINT32                            LC_WAIT_FOR_MORE_TS_IN_RLOCK:6;
  } Field;
  UINT32 Value;
} PCIE_LC_CNTL4_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL4_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL4_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL4_OFFSET       0x2d8
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_CNTL4_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x2d8)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL4_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL4_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL4_OFFSET       0x2d8
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_CNTL4_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x2d8)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL4_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL4_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL4_OFFSET       0x2d8
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_CNTL4_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x2d8)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL4_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL4_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL4_OFFSET       0x2d8
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_CNTL4_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x2d8)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL4_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL4_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL4_OFFSET       0x2d8
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_CNTL4_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x2d8)

#define SMN_FUNC0_PCIE0_PCIE_LC_CNTL4_ADDRESS    0x111402d8UL

#define SMN_FUNC0_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112402d8UL

#define SMN_FUNC0_PCIE2_PCIE_LC_CNTL4_ADDRESS    0x113402d8UL

#define SMN_FUNC0_PCIE3_PCIE_LC_CNTL4_ADDRESS    0x114402d8UL

#define SMN_FUNC1_PCIE0_PCIE_LC_CNTL4_ADDRESS    0x111412d8UL

#define SMN_FUNC1_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112412d8UL

#define SMN_FUNC2_PCIE0_PCIE_LC_CNTL4_ADDRESS    0x111422d8UL

#define SMN_FUNC2_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112422d8UL

#define SMN_FUNC3_PCIE0_PCIE_LC_CNTL4_ADDRESS    0x111432d8UL

#define SMN_FUNC3_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112432d8UL

#define SMN_FUNC4_PCIE0_PCIE_LC_CNTL4_ADDRESS    0x111442d8UL

#define SMN_FUNC4_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112442d8UL

#define SMN_FUNC5_PCIE1_PCIE_LC_CNTL4_ADDRESS    0x112452d8UL

/***********************************************************
* Register Name : PCIE_LC_L1_PM_SUBSTATE
* Register Description :
* L1 PM Substates Private 1 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if overriding L1 PM Substates enable settings. When this bit is 1, values specified in LC_PCI_PM_L1_2_OVERRIDE, LC_PCI_PM_L1_1_OVERRIDE, LC_ASPM_L1_2_OVERRIDE, and LC_ASPM_L1_1_OVERRIDE bits are used instead of the corresponding bits in L1 PM Substates Control 1 register to configure L1 PM Substates.
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_SUBSTATES_OVERRIDE_EN_OFFSET      0
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_SUBSTATES_OVERRIDE_EN_MASK        0x1

// Bitfield Description : When LC_L1_SUBSTATES_OVERRIDE_EN bit is 1, controls if PCI-PM L1.2 is enabled.
#define PCIE_LC_L1_PM_SUBSTATE_LC_PCI_PM_L1_2_OVERRIDE_OFFSET      1
#define PCIE_LC_L1_PM_SUBSTATE_LC_PCI_PM_L1_2_OVERRIDE_MASK        0x2

// Bitfield Description : When LC_L1_SUBSTATES_OVERRIDE_EN bit is 1, controls if PCI-PM L1.1 is enabled.
#define PCIE_LC_L1_PM_SUBSTATE_LC_PCI_PM_L1_1_OVERRIDE_OFFSET      2
#define PCIE_LC_L1_PM_SUBSTATE_LC_PCI_PM_L1_1_OVERRIDE_MASK        0x4

// Bitfield Description : When LC_L1_SUBSTATES_OVERRIDE_EN bit is 1, controls if ASPM L1.2 is enabled.
#define PCIE_LC_L1_PM_SUBSTATE_LC_ASPM_L1_2_OVERRIDE_OFFSET      3
#define PCIE_LC_L1_PM_SUBSTATE_LC_ASPM_L1_2_OVERRIDE_MASK        0x8

// Bitfield Description : When LC_L1_SUBSTATES_OVERRIDE_EN bit is 1, controls if ASPM L1.1 is enabled.
#define PCIE_LC_L1_PM_SUBSTATE_LC_ASPM_L1_1_OVERRIDE_OFFSET      4
#define PCIE_LC_L1_PM_SUBSTATE_LC_ASPM_L1_1_OVERRIDE_MASK        0x10

// Bitfield Description : Controls filtering of CLKREQb signal in LC in order to avoid false L1 substate entries/exits.
#define PCIE_LC_L1_PM_SUBSTATE_LC_CLKREQ_FILTER_EN_OFFSET      5
#define PCIE_LC_L1_PM_SUBSTATE_LC_CLKREQ_FILTER_EN_MASK        0x20

// Bitfield Description : When this field is a non-zero value, overrides the value in T_POWER_ON Scale field in the L1 PM Substates Control 2 register.
#define PCIE_LC_L1_PM_SUBSTATE_LC_T_POWER_ON_SCALE_OFFSET      6
#define PCIE_LC_L1_PM_SUBSTATE_LC_T_POWER_ON_SCALE_MASK        0xc0

// Bitfield Description : When this field is a non-zero value, overrides the value in T_POWER_ON Value field in the L1 PM Substates Control 2 register.
#define PCIE_LC_L1_PM_SUBSTATE_LC_T_POWER_ON_VALUE_OFFSET      8
#define PCIE_LC_L1_PM_SUBSTATE_LC_T_POWER_ON_VALUE_MASK        0x1f00

// Bitfield Description : Enables sending a Master AXI register write to the FCH register shadow copy whenever T_POWER_ON is updated in the configuration space. The target address is PCIE_LC_L1_PM_SUBSTATE3/4.T_POWER_ON_FCH_TARGET_ADDRESS.
#define PCIE_LC_L1_PM_SUBSTATE_T_POWER_ON_FCH_COPY_EN_OFFSET      13
#define PCIE_LC_L1_PM_SUBSTATE_T_POWER_ON_FCH_COPY_EN_MASK        0x2000

// Bitfield Description : When a 1 is written to this bit, force a MST_AXI transaction to be sent to FCH shadow copy with the current T_POWER_ON value. Reads to this bit always read 0.
#define PCIE_LC_L1_PM_SUBSTATE_T_POWER_ON_FCH_COPY_TRIGGER_OFFSET      14
#define PCIE_LC_L1_PM_SUBSTATE_T_POWER_ON_FCH_COPY_TRIGGER_MASK        0x4000

// Bitfield Description : Controls if the transition from L1_2_Idle to L1_2_Exit waits for PG_Commit to de-assert.
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_BLOCK_EXIT_PG_COMMIT_OFFSET      15
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_BLOCK_EXIT_PG_COMMIT_MASK        0x8000

// Bitfield Description : Controls the PowerDown value to be propagated to the PIPE interface when the Link is in L1.1.
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_1_POWERDOWN_OFFSET      16
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_1_POWERDOWN_MASK        0x70000

// Bitfield Description :
#define PCIE_LC_L1_PM_SUBSTATE_Reserved_19_19_OFFSET      19
#define PCIE_LC_L1_PM_SUBSTATE_Reserved_19_19_MASK        0x80000

// Bitfield Description : Controls the PowerDown value to be propagated to the PIPE interface when the Link is in L1.2.
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_POWERDOWN_OFFSET      20
#define PCIE_LC_L1_PM_SUBSTATE_LC_L1_2_POWERDOWN_MASK        0x700000

// Bitfield Description : Controls the delay between assertion of REFCLK and L1.2 exit.
#define PCIE_LC_L1_PM_SUBSTATE_LC_DEFER_L1_2_EXIT_OFFSET      23
#define PCIE_LC_L1_PM_SUBSTATE_LC_DEFER_L1_2_EXIT_MASK        0x3800000

// Bitfield Description : Controls if disabling L1 in LINK_CNTL.PM_CONTROL, when the link is in ASPM L1, will force an L1 exit.
#define PCIE_LC_L1_PM_SUBSTATE_LC_WAKE_FROM_ASPM_L1_ON_PM_CONTROL_CLEAR_OFFSET      26
#define PCIE_LC_L1_PM_SUBSTATE_LC_WAKE_FROM_ASPM_L1_ON_PM_CONTROL_CLEAR_MASK        0x4000000

// Bitfield Description : Controls if a register write that triggers an L1 exit forces an exit from dynamic PG.
#define PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_OFFSET      27
#define PCIE_LC_L1_PM_SUBSTATE_LC_FORCE_L1_PG_EXIT_ON_REG_WRITE_MASK        0x8000000

// Bitfield Description : Controls if entry to the L1.1 substate is blocked if the exit condition occurs while still in L1.
#define PCIE_LC_L1_PM_SUBSTATE_LC_QUICK_L1_1_ABORT_IN_L1_OFFSET      28
#define PCIE_LC_L1_PM_SUBSTATE_LC_QUICK_L1_1_ABORT_IN_L1_MASK        0x10000000

// Bitfield Description : Controls if entry to the L1.2.Entry state is blocked if the exit condition occurs while still in L1.
#define PCIE_LC_L1_PM_SUBSTATE_LC_QUICK_L1_2_ABORT_IN_L1_OFFSET      29
#define PCIE_LC_L1_PM_SUBSTATE_LC_QUICK_L1_2_ABORT_IN_L1_MASK        0x20000000

// Bitfield Description : Controls if the AUX_COUNTER will count in larger increments during L1.2.Entry and L1.2.Idle, due to a slower Refclk replacing PCLK.
#define PCIE_LC_L1_PM_SUBSTATE_LC_AUX_COUNT_REFCLK_INCREMENT_EN_OFFSET      30
#define PCIE_LC_L1_PM_SUBSTATE_LC_AUX_COUNT_REFCLK_INCREMENT_EN_MASK        0x40000000

// Bitfield Description : When LC_AUX_COUNT_REFCLK_INCREMENT_EN bit is 1, controls if the determination of when the slower Refclk is running is based on PCS sideband signals, rather than on internal states.
#define PCIE_LC_L1_PM_SUBSTATE_LC_AUX_COUNT_REFCLK_INCREMENT_USE_PCS_SIDEBAND_OFFSET      31
#define PCIE_LC_L1_PM_SUBSTATE_LC_AUX_COUNT_REFCLK_INCREMENT_USE_PCS_SIDEBAND_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LC_L1_SUBSTATES_OVERRIDE_EN:1;
    UINT32                            LC_PCI_PM_L1_2_OVERRIDE:1;
    UINT32                            LC_PCI_PM_L1_1_OVERRIDE:1;
    UINT32                            LC_ASPM_L1_2_OVERRIDE:1;
    UINT32                            LC_ASPM_L1_1_OVERRIDE:1;
    UINT32                            LC_CLKREQ_FILTER_EN:1;
    UINT32                            LC_T_POWER_ON_SCALE:2;
    UINT32                            LC_T_POWER_ON_VALUE:5;
    UINT32                            T_POWER_ON_FCH_COPY_EN:1;
    UINT32                            T_POWER_ON_FCH_COPY_TRIGGER:1;
    UINT32                            LC_L1_2_BLOCK_EXIT_PG_COMMIT:1;
    UINT32                            LC_L1_1_POWERDOWN:3;
    UINT32                            Reserved_19_19:1;
    UINT32                            LC_L1_2_POWERDOWN:3;
    UINT32                            LC_DEFER_L1_2_EXIT:3;
    UINT32                            LC_WAKE_FROM_ASPM_L1_ON_PM_CONTROL_CLEAR:1;
    UINT32                            LC_FORCE_L1_PG_EXIT_ON_REG_WRITE:1;
    UINT32                            LC_QUICK_L1_1_ABORT_IN_L1:1;
    UINT32                            LC_QUICK_L1_2_ABORT_IN_L1:1;
    UINT32                            LC_AUX_COUNT_REFCLK_INCREMENT_EN:1;
    UINT32                            LC_AUX_COUNT_REFCLK_INCREMENT_USE_PCS_SIDEBAND:1;
  } Field;
  UINT32 Value;
} PCIE_LC_L1_PM_SUBSTATE_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE_OFFSET       0x318
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x318)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE_OFFSET       0x318
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x318)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE_OFFSET       0x318
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x318)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE_OFFSET       0x318
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x318)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE_OFFSET       0x318
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x318)

#define SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11140318UL

#define SMN_FUNC0_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11240318UL

#define SMN_FUNC0_PCIE2_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11340318UL

#define SMN_FUNC0_PCIE3_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11440318UL

#define SMN_FUNC1_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11141318UL

#define SMN_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11241318UL

#define SMN_FUNC2_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11142318UL

#define SMN_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11242318UL

#define SMN_FUNC3_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11143318UL

#define SMN_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11243318UL

#define SMN_FUNC4_PCIE0_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11144318UL

#define SMN_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11244318UL

#define SMN_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE_ADDRESS    0x11245318UL


/***********************************************************
* Register Name : PCIE_LC_L1_PM_SUBSTATE3
* Register Description :
* L1 PM Substates Private 3 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Sets the address of the FCH shadow register for T_POWER_ON. Bits 31:0. The FCH-side register is an 8 bit register, so bits 1:0 will be used as byte enables for the register transaction.
#define PCIE_LC_L1_PM_SUBSTATE3_T_POWER_ON_FCH_TARGET_ADDRESS_LO_OFFSET      0
#define PCIE_LC_L1_PM_SUBSTATE3_T_POWER_ON_FCH_TARGET_ADDRESS_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            T_POWER_ON_FCH_TARGET_ADDRESS_LO:32;
  } Field;
  UINT32 Value;
} PCIE_LC_L1_PM_SUBSTATE3_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_OFFSET       0x320
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x320)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_OFFSET       0x320
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x320)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_OFFSET       0x320
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x320)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_OFFSET       0x320
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x320)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_OFFSET       0x320
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x320)

#define SMN_FUNC0_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11140320UL

#define SMN_FUNC0_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11240320UL

#define SMN_FUNC0_PCIE2_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11340320UL

#define SMN_FUNC0_PCIE3_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11440320UL

#define SMN_FUNC1_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11141320UL

#define SMN_FUNC1_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11241320UL

#define SMN_FUNC2_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11142320UL

#define SMN_FUNC2_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11242320UL

#define SMN_FUNC3_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11143320UL

#define SMN_FUNC3_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11243320UL

#define SMN_FUNC4_PCIE0_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11144320UL

#define SMN_FUNC4_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11244320UL

#define SMN_FUNC5_PCIE1_PCIE_LC_L1_PM_SUBSTATE3_ADDRESS    0x11245320UL


/***********************************************************
* Register Name : PCIE_LC_LINK_WIDTH_CNTL
* Register Description :
* Link Width Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the target link width for the next link negotiation.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_LINK_WIDTH_OFFSET      0
#define PCIE_LC_LINK_WIDTH_CNTL_LC_LINK_WIDTH_MASK        0x7

// Bitfield Description :
#define PCIE_LC_LINK_WIDTH_CNTL_Reserved_3_3_OFFSET      3
#define PCIE_LC_LINK_WIDTH_CNTL_Reserved_3_3_MASK        0x8

// Bitfield Description : Indicates the current link width.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_LINK_WIDTH_RD_OFFSET      4
#define PCIE_LC_LINK_WIDTH_CNTL_LC_LINK_WIDTH_RD_MASK        0x70

// Bitfield Description : Controls expediting transition from Recovery.Idle to Detect during a long reconfiguration.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RECONFIG_ARC_MISSING_ESCAPE_OFFSET      7
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RECONFIG_ARC_MISSING_ESCAPE_MASK        0x80

// Bitfield Description : Triggers a link negotiation.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RECONFIG_NOW_OFFSET      8
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RECONFIG_NOW_MASK        0x100

// Bitfield Description : Indicates Link partner's advertised link width renegotiation support.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RENEGOTIATION_SUPPORT_OFFSET      9
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RENEGOTIATION_SUPPORT_MASK        0x200

// Bitfield Description : Controls if Link re-negotiation is enabled.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RENEGOTIATE_EN_OFFSET      10
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RENEGOTIATE_EN_MASK        0x400

// Bitfield Description : Controls if the vendor-specific Short Reconfiguration feature is enabled.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_SHORT_RECONFIG_EN_OFFSET      11
#define PCIE_LC_LINK_WIDTH_CNTL_LC_SHORT_RECONFIG_EN_MASK        0x800

// Bitfield Description : Controls if the PCIe Upconfigure feature is enabled.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_SUPPORT_OFFSET      12
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_SUPPORT_MASK        0x1000

// Bitfield Description : Controls allowing LTSSM to use all other control signals of the PCIe Upconfigure feature.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_DIS_OFFSET      13
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_DIS_MASK        0x2000

// Bitfield Description : Controls waiting for all receivers during a link width upconfigure.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCFG_WAIT_FOR_RCVR_DIS_OFFSET      14
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCFG_WAIT_FOR_RCVR_DIS_MASK        0x4000

// Bitfield Description : Controls time that state machine waits to receive TSx on all receivers during a link width upconfigure.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCFG_TIMER_SEL_OFFSET      15
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCFG_TIMER_SEL_MASK        0x8000

// Bitfield Description : Controls powering down (TX_PDNB asserted) unused lanes.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DEASSERT_TX_PDNB_OFFSET      16
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DEASSERT_TX_PDNB_MASK        0x10000

// Bitfield Description : Controls allowing link width reconfiguration initiation in L1.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_L1_RECONFIG_EN_OFFSET      17
#define PCIE_LC_LINK_WIDTH_CNTL_LC_L1_RECONFIG_EN_MASK        0x20000

// Bitfield Description : Controls HW initiated link width change feature.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DYNLINK_MST_EN_OFFSET      18
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DYNLINK_MST_EN_MASK        0x40000

// Bitfield Description : Controls allowing link width reconfiguration to be initiated, when the Link partner has already initiated a link width change.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DUAL_END_RECONFIG_EN_OFFSET      19
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DUAL_END_RECONFIG_EN_MASK        0x80000

// Bitfield Description : Indicates if PCIe Upconfigure feature is supported (upconfigure_capable variable is 1) by both ends of the Link.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_CAPABLE_OFFSET      20
#define PCIE_LC_LINK_WIDTH_CNTL_LC_UPCONFIGURE_CAPABLE_MASK        0x100000

// Bitfield Description : Controls power state of lanes that are unused after a link width change is performed. The link width change can use the vendor-specific Short Reconfiguration feature or the PCIe Upconfigure feature.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DYN_LANES_PWR_STATE_OFFSET      21
#define PCIE_LC_LINK_WIDTH_CNTL_LC_DYN_LANES_PWR_STATE_MASK        0x600000

// Bitfield Description :
#define PCIE_LC_LINK_WIDTH_CNTL_Reserved_23_23_OFFSET      23
#define PCIE_LC_LINK_WIDTH_CNTL_Reserved_23_23_MASK        0x800000

// Bitfield Description : Controls allowing multiple reversal attempts when searching for widest possible link.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_MULT_REVERSE_ATTEMP_EN_OFFSET      24
#define PCIE_LC_LINK_WIDTH_CNTL_LC_MULT_REVERSE_ATTEMP_EN_MASK        0x1000000

// Bitfield Description : Controls resetting TSx counter (TSx_COUNT) if there is a change in the received TSx type in Recovery.RcvrCfg.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RESET_TSX_CNT_IN_RCONFIG_EN_OFFSET      25
#define PCIE_LC_LINK_WIDTH_CNTL_LC_RESET_TSX_CNT_IN_RCONFIG_EN_MASK        0x2000000

// Bitfield Description : Controls if LTSSM waits to receive the required number of Logical Idles before sending Logical Idles in Recovery.Idle.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_WAIT_FOR_L_IDLE_IN_R_IDLE_OFFSET      26
#define PCIE_LC_LINK_WIDTH_CNTL_LC_WAIT_FOR_L_IDLE_IN_R_IDLE_MASK        0x4000000

// Bitfield Description : Controls if LTSSM waits for receivers to detect an exit from Electrical Idle before exiting from Rx_L0s to Recovery.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_WAIT_FOR_NON_EI_ON_RXL0S_EXIT_OFFSET      27
#define PCIE_LC_LINK_WIDTH_CNTL_LC_WAIT_FOR_NON_EI_ON_RXL0S_EXIT_MASK        0x8000000

// Bitfield Description : Controls extending received Electrical Idle until PHY command change is done for the transmitter.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_HOLD_EI_FOR_RSPEED_CMD_CHANGE_OFFSET      28
#define PCIE_LC_LINK_WIDTH_CNTL_LC_HOLD_EI_FOR_RSPEED_CMD_CHANGE_MASK        0x10000000

// Bitfield Description : Controls avoiding unnecessary entry into Rx_L0s when receivers have detected a quick entry and exit into and out of Electrical Idle.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_BYPASS_RXL0S_ON_SHORT_EI_OFFSET      29
#define PCIE_LC_LINK_WIDTH_CNTL_LC_BYPASS_RXL0S_ON_SHORT_EI_MASK        0x20000000

// Bitfield Description : Controls turning off lanes for which receivers were not detected or that were not included during the initial Link configuration.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_TURN_OFF_UNUSED_LANES_OFFSET      30
#define PCIE_LC_LINK_WIDTH_CNTL_LC_TURN_OFF_UNUSED_LANES_MASK        0x40000000

// Bitfield Description : Controls ability to bypass the check for RxStandbyStatus when changing RxStandby.
#define PCIE_LC_LINK_WIDTH_CNTL_LC_BYPASS_RXSTANDBY_STATUS_OFFSET      31
#define PCIE_LC_LINK_WIDTH_CNTL_LC_BYPASS_RXSTANDBY_STATUS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LC_LINK_WIDTH:3;
    UINT32                            Reserved_3_3:1;
    UINT32                            LC_LINK_WIDTH_RD:3;
    UINT32                            LC_RECONFIG_ARC_MISSING_ESCAPE:1;
    UINT32                            LC_RECONFIG_NOW:1;
    UINT32                            LC_RENEGOTIATION_SUPPORT:1;
    UINT32                            LC_RENEGOTIATE_EN:1;
    UINT32                            LC_SHORT_RECONFIG_EN:1;
    UINT32                            LC_UPCONFIGURE_SUPPORT:1;
    UINT32                            LC_UPCONFIGURE_DIS:1;
    UINT32                            LC_UPCFG_WAIT_FOR_RCVR_DIS:1;
    UINT32                            LC_UPCFG_TIMER_SEL:1;
    UINT32                            LC_DEASSERT_TX_PDNB:1;
    UINT32                            LC_L1_RECONFIG_EN:1;
    UINT32                            LC_DYNLINK_MST_EN:1;
    UINT32                            LC_DUAL_END_RECONFIG_EN:1;
    UINT32                            LC_UPCONFIGURE_CAPABLE:1;
    UINT32                            LC_DYN_LANES_PWR_STATE:2;
    UINT32                            Reserved_23_23:1;
    UINT32                            LC_MULT_REVERSE_ATTEMP_EN:1;
    UINT32                            LC_RESET_TSX_CNT_IN_RCONFIG_EN:1;
    UINT32                            LC_WAIT_FOR_L_IDLE_IN_R_IDLE:1;
    UINT32                            LC_WAIT_FOR_NON_EI_ON_RXL0S_EXIT:1;
    UINT32                            LC_HOLD_EI_FOR_RSPEED_CMD_CHANGE:1;
    UINT32                            LC_BYPASS_RXL0S_ON_SHORT_EI:1;
    UINT32                            LC_TURN_OFF_UNUSED_LANES:1;
    UINT32                            LC_BYPASS_RXSTANDBY_STATUS:1;
  } Field;
  UINT32 Value;
} PCIE_LC_LINK_WIDTH_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x288)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x288)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x288)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x288)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x288)

#define SMN_FUNC0_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11140288UL

#define SMN_FUNC0_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11240288UL

#define SMN_FUNC0_PCIE2_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11340288UL

#define SMN_FUNC0_PCIE3_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11440288UL

#define SMN_FUNC1_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11141288UL

#define SMN_FUNC1_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11241288UL

#define SMN_FUNC2_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11142288UL

#define SMN_FUNC2_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11242288UL

#define SMN_FUNC3_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11143288UL

#define SMN_FUNC3_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11243288UL

#define SMN_FUNC4_PCIE0_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11144288UL

#define SMN_FUNC4_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11244288UL

#define SMN_FUNC5_PCIE1_PCIE_LC_LINK_WIDTH_CNTL_ADDRESS    0x11245288UL


/***********************************************************
* Register Name : PCIE_LC_SPEED_CNTL
* Register Description :
* Link Speed Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls if 5.0 GT/s data rate is supported (strap loadable).
#define PCIE_LC_SPEED_CNTL_LC_GEN2_EN_STRAP_OFFSET      0
#define PCIE_LC_SPEED_CNTL_LC_GEN2_EN_STRAP_MASK        0x1

// Bitfield Description : Controls if 8.0 GT/s data rate is supported (strap loadable).
#define PCIE_LC_SPEED_CNTL_LC_GEN3_EN_STRAP_OFFSET      1
#define PCIE_LC_SPEED_CNTL_LC_GEN3_EN_STRAP_MASK        0x2

// Bitfield Description : Controls if 16.0 GT/s data rate is supported (strap loadable).
#define PCIE_LC_SPEED_CNTL_LC_GEN4_EN_STRAP_OFFSET      2
#define PCIE_LC_SPEED_CNTL_LC_GEN4_EN_STRAP_MASK        0x4

// Bitfield Description : Controls if 32.0 GT/s data rate is supported (strap loadable).
#define PCIE_LC_SPEED_CNTL_LC_GEN5_EN_STRAP_OFFSET      3
#define PCIE_LC_SPEED_CNTL_LC_GEN5_EN_STRAP_MASK        0x8

// Bitfield Description :
#define PCIE_LC_SPEED_CNTL_Reserved_4_4_OFFSET      4
#define PCIE_LC_SPEED_CNTL_Reserved_4_4_MASK        0x10

// Bitfield Description : Indicates the current data rate of the Link.
#define PCIE_LC_SPEED_CNTL_LC_CURRENT_DATA_RATE_OFFSET      5
#define PCIE_LC_SPEED_CNTL_LC_CURRENT_DATA_RATE_MASK        0xe0

// Bitfield Description : Indicates the highest data rate advertised by the Port.
#define PCIE_LC_SPEED_CNTL_LC_DATA_RATE_ADVERTISED_OFFSET      8
#define PCIE_LC_SPEED_CNTL_LC_DATA_RATE_ADVERTISED_MASK        0x700

// Bitfield Description : Controls overriding of Target Link Speed field value in Link Control 2 register.
#define PCIE_LC_SPEED_CNTL_LC_TARGET_LINK_SPEED_OVERRIDE_EN_OFFSET      11
#define PCIE_LC_SPEED_CNTL_LC_TARGET_LINK_SPEED_OVERRIDE_EN_MASK        0x800

// Bitfield Description : When LC_TARGET_LINK_SPEED_OVERRIDE_EN bit is 1, contains the value used instead of Target Link Speed field.
#define PCIE_LC_SPEED_CNTL_LC_TARGET_LINK_SPEED_OVERRIDE_OFFSET      12
#define PCIE_LC_SPEED_CNTL_LC_TARGET_LINK_SPEED_OVERRIDE_MASK        0x7000

// Bitfield Description :
#define PCIE_LC_SPEED_CNTL_Reserved_20_15_OFFSET      15
#define PCIE_LC_SPEED_CNTL_Reserved_20_15_MASK        0x1f8000

// Bitfield Description : When LC_GEN2_EN_STRAP bit is 0, controls if the LC checks the Data Rate Identifier (symbol 4) of received TSxs.
#define PCIE_LC_SPEED_CNTL_LC_CHECK_DATA_RATE_OFFSET      21
#define PCIE_LC_SPEED_CNTL_LC_CHECK_DATA_RATE_MASK        0x200000

// Bitfield Description : Indicates a cumulative 5.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN2_OFFSET      22
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN2_MASK        0x400000

// Bitfield Description : Indicates the current 5.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN2_OFFSET      23
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN2_MASK        0x800000

// Bitfield Description : Indicates the cumulative 8.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN3_OFFSET      24
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN3_MASK        0x1000000

// Bitfield Description : Indicates the current 8.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN3_OFFSET      25
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN3_MASK        0x2000000

// Bitfield Description : Indicates the cumulative 16.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN4_OFFSET      26
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN4_MASK        0x4000000

// Bitfield Description : Indicates the current 16.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN4_OFFSET      27
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN4_MASK        0x8000000

// Bitfield Description : Indicates the cumulative 32.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN5_OFFSET      28
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_EVER_SENT_GEN5_MASK        0x10000000

// Bitfield Description : Indicates the current 32.0 GT/s capability of the Link partner.
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN5_OFFSET      29
#define PCIE_LC_SPEED_CNTL_LC_OTHER_SIDE_SUPPORTS_GEN5_MASK        0x20000000

// Bitfield Description :
#define PCIE_LC_SPEED_CNTL_Reserved_31_30_OFFSET      30
#define PCIE_LC_SPEED_CNTL_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LC_GEN2_EN_STRAP:1;
    UINT32                            LC_GEN3_EN_STRAP:1;
    UINT32                            LC_GEN4_EN_STRAP:1;
    UINT32                            LC_GEN5_EN_STRAP:1;
    UINT32                            Reserved_4_4:1;
    UINT32                            LC_CURRENT_DATA_RATE:3;
    UINT32                            LC_DATA_RATE_ADVERTISED:3;
    UINT32                            LC_TARGET_LINK_SPEED_OVERRIDE_EN:1;
    UINT32                            LC_TARGET_LINK_SPEED_OVERRIDE:3;
    UINT32                            Reserved_20_15:6;
    UINT32                            LC_CHECK_DATA_RATE:1;
    UINT32                            LC_OTHER_SIDE_EVER_SENT_GEN2:1;
    UINT32                            LC_OTHER_SIDE_SUPPORTS_GEN2:1;
    UINT32                            LC_OTHER_SIDE_EVER_SENT_GEN3:1;
    UINT32                            LC_OTHER_SIDE_SUPPORTS_GEN3:1;
    UINT32                            LC_OTHER_SIDE_EVER_SENT_GEN4:1;
    UINT32                            LC_OTHER_SIDE_SUPPORTS_GEN4:1;
    UINT32                            LC_OTHER_SIDE_EVER_SENT_GEN5:1;
    UINT32                            LC_OTHER_SIDE_SUPPORTS_GEN5:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_LC_SPEED_CNTL_PCIEPORT_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x290)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x290)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x290)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x290)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x290)

#define SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS    0x11140290UL

#define SMN_FUNC0_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11240290UL

#define SMN_FUNC0_PCIE2_PCIE_LC_SPEED_CNTL_ADDRESS    0x11340290UL

#define SMN_FUNC0_PCIE3_PCIE_LC_SPEED_CNTL_ADDRESS    0x11440290UL

#define SMN_FUNC1_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS    0x11141290UL

#define SMN_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11241290UL

#define SMN_FUNC2_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS    0x11142290UL

#define SMN_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11242290UL

#define SMN_FUNC3_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS    0x11143290UL

#define SMN_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11243290UL

#define SMN_FUNC4_PCIE0_PCIE_LC_SPEED_CNTL_ADDRESS    0x11144290UL

#define SMN_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11244290UL

#define SMN_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL_ADDRESS    0x11245290UL


/***********************************************************
* Register Name : PCIE_LC_TRAINING_CNTL
* Register Description :
* Training Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the value of bits 3:1 in Training Control field (symbol 5) in any subsequently transmitted TSxs. Bit 0 = Reserved; Bit 1 = Disable Link; Bit 2 = Loopback; Bit 3 = Disable Scrambling. When a bit in this field is 1, the corresponding bit in Training Control field is set.
#define PCIE_LC_TRAINING_CNTL_LC_TRAINING_CNTL_OFFSET      0
#define PCIE_LC_TRAINING_CNTL_LC_TRAINING_CNTL_MASK        0xf

// Bitfield Description : Controls the value of Compliance Receive bit in Training Control field (symbol 5, bit 4) in any subsequently transmitted TS1s. This bit is sticky.
#define PCIE_LC_TRAINING_CNTL_LC_COMPLIANCE_RECEIVE_OFFSET      4
#define PCIE_LC_TRAINING_CNTL_LC_COMPLIANCE_RECEIVE_MASK        0x10

// Bitfield Description : Controls looking for more non-matching TS1s.
#define PCIE_LC_TRAINING_CNTL_LC_LOOK_FOR_MORE_NON_MATCHING_TS1_OFFSET      5
#define PCIE_LC_TRAINING_CNTL_LC_LOOK_FOR_MORE_NON_MATCHING_TS1_MASK        0x20

// Bitfield Description : (Downstream Ports only) Controls transitioning from Tx_L0s or L1 to Recovery, if a Hot Reset or Link Disable is initiated by the Port.
#define PCIE_LC_TRAINING_CNTL_LC_L0S_L1_TRAINING_CNTL_EN_OFFSET      6
#define PCIE_LC_TRAINING_CNTL_LC_L0S_L1_TRAINING_CNTL_EN_MASK        0x40

// Bitfield Description : Controls enabling the fix for FTS going to L1 issue.
#define PCIE_LC_TRAINING_CNTL_LC_L1_LONG_WAKE_FIX_EN_OFFSET      7
#define PCIE_LC_TRAINING_CNTL_LC_L1_LONG_WAKE_FIX_EN_MASK        0x80

// Bitfield Description : Indicates the current LTTSM Power state.
#define PCIE_LC_TRAINING_CNTL_LC_POWER_STATE_OFFSET      8
#define PCIE_LC_TRAINING_CNTL_LC_POWER_STATE_MASK        0x700

// Bitfield Description : Controls blocking the LTSSM from going to Rx_L0s, if it has already acknowledged a request to go to L1 but it hasn't transitioned to L1 yet.
#define PCIE_LC_TRAINING_CNTL_LC_DONT_GO_TO_L0S_IF_L1_ARMED_OFFSET      11
#define PCIE_LC_TRAINING_CNTL_LC_DONT_GO_TO_L0S_IF_L1_ARMED_MASK        0x800

// Bitfield Description : Controls if LTSSM can initiate a link speed change, when Retrain Link bit is 1 and Target Link Speed field is not equal to the current link speed.
#define PCIE_LC_TRAINING_CNTL_LC_INIT_SPD_CHG_WITH_CSR_EN_OFFSET      12
#define PCIE_LC_TRAINING_CNTL_LC_INIT_SPD_CHG_WITH_CSR_EN_MASK        0x1000

// Bitfield Description : Controls if LTSSM looks at Training Control field in received TSxs to determine whether any state transition occurs. When this bit is 1, LTSSM does not respond to Hot Reset, Disable Link, Loopback, Disable Scrambling, or Compliance Receive requests.
#define PCIE_LC_TRAINING_CNTL_LC_DISABLE_TRAINING_BIT_ARCH_OFFSET      13
#define PCIE_LC_TRAINING_CNTL_LC_DISABLE_TRAINING_BIT_ARCH_MASK        0x2000

// Bitfield Description : Controls when LTSSM begins counting the minimum number of TS2 required to be sent out in Recovery.RcvrCfg.
#define PCIE_LC_TRAINING_CNTL_LC_WAIT_FOR_SETS_IN_RCFG_OFFSET      14
#define PCIE_LC_TRAINING_CNTL_LC_WAIT_FOR_SETS_IN_RCFG_MASK        0x4000

// Bitfield Description : (Downstream Ports only) Controls if LTSSM uses the Hot Reset bit in received TS1s, in deciding when to exit Hot Reset.
#define PCIE_LC_TRAINING_CNTL_LC_HOT_RESET_QUICK_EXIT_EN_OFFSET      15
#define PCIE_LC_TRAINING_CNTL_LC_HOT_RESET_QUICK_EXIT_EN_MASK        0x8000

// Bitfield Description : Controls extending the timer when in Rcv_L0s_Skp state.
#define PCIE_LC_TRAINING_CNTL_LC_EXTEND_WAIT_FOR_SKP_OFFSET      16
#define PCIE_LC_TRAINING_CNTL_LC_EXTEND_WAIT_FOR_SKP_MASK        0x10000

// Bitfield Description : Controls over-riding the value of Autonomous Change (symbol 4, bit 6) in any subsequently transmitted TSxs.
#define PCIE_LC_TRAINING_CNTL_LC_AUTONOMOUS_CHANGE_OFF_OFFSET      17
#define PCIE_LC_TRAINING_CNTL_LC_AUTONOMOUS_CHANGE_OFF_MASK        0x20000

// Bitfield Description : Controls over-riding the value of Link Upconfigure Capability (symbol 4, bit 6) in any subsequently transmitted TS2s.
#define PCIE_LC_TRAINING_CNTL_LC_UPCONFIGURE_CAP_OFF_OFFSET      18
#define PCIE_LC_TRAINING_CNTL_LC_UPCONFIGURE_CAP_OFF_MASK        0x40000

// Bitfield Description : (Root Complexes only) Controls if the Hardware or Chip-induced Link Disable feature is used.
#define PCIE_LC_TRAINING_CNTL_LC_HW_LINK_DIS_EN_OFFSET      19
#define PCIE_LC_TRAINING_CNTL_LC_HW_LINK_DIS_EN_MASK        0x80000

// Bitfield Description : Indicates if the Link was disabled due to the Hardware or Chip-induced Link Disable mechanism. This bit is sticky.
#define PCIE_LC_TRAINING_CNTL_LC_LINK_DIS_BY_HW_OFFSET      20
#define PCIE_LC_TRAINING_CNTL_LC_LINK_DIS_BY_HW_MASK        0x100000

// Bitfield Description : Controls using the same WAIT_FOR_EMPTY_PIPE values for all link widths when going to L1 or L2/L3 Ready.
#define PCIE_LC_TRAINING_CNTL_LC_STATIC_TX_PIPE_COUNT_EN_OFFSET      21
#define PCIE_LC_TRAINING_CNTL_LC_STATIC_TX_PIPE_COUNT_EN_MASK        0x200000

// Bitfield Description : (Downstream Ports only) Controls timer threshold to be used when a request to go to L1 is declined (i.e. PM Active State Nak is sent).
#define PCIE_LC_TRAINING_CNTL_LC_ASPM_L1_NAK_TIMER_SEL_OFFSET      22
#define PCIE_LC_TRAINING_CNTL_LC_ASPM_L1_NAK_TIMER_SEL_MASK        0xc00000

// Bitfield Description : Controls preventing de-assertion of RX_EN in Recovery.Speed.
#define PCIE_LC_TRAINING_CNTL_LC_DONT_DEASSERT_RX_EN_IN_R_SPEED_OFFSET      24
#define PCIE_LC_TRAINING_CNTL_LC_DONT_DEASSERT_RX_EN_IN_R_SPEED_MASK        0x1000000

// Bitfield Description : Controls preventing de-assertion of RX_EN in Polling.Compliance and Loopback.
#define PCIE_LC_TRAINING_CNTL_LC_DONT_DEASSERT_RX_EN_IN_TEST_OFFSET      25
#define PCIE_LC_TRAINING_CNTL_LC_DONT_DEASSERT_RX_EN_IN_TEST_MASK        0x2000000

// Bitfield Description : (Downstream Ports only) Controls preventing L1 Nak Counter from being reset before it has expired (i.e. reached LC_ASPM_L1_NAK_TIMER_SEL value) if additional ASPM L1 requests are received.
#define PCIE_LC_TRAINING_CNTL_LC_RESET_ASPM_L1_NAK_TIMER_OFFSET      26
#define PCIE_LC_TRAINING_CNTL_LC_RESET_ASPM_L1_NAK_TIMER_MASK        0x4000000

// Bitfield Description : Controls the timeout threshold in Recovery.RcvrCfg.
#define PCIE_LC_TRAINING_CNTL_LC_SHORT_RCFG_TIMEOUT_OFFSET      27
#define PCIE_LC_TRAINING_CNTL_LC_SHORT_RCFG_TIMEOUT_MASK        0x8000000

// Bitfield Description : Controls allowing TX to prevent LC from going to L1 when there are outstanding completions.
#define PCIE_LC_TRAINING_CNTL_LC_ALLOW_TX_L1_CONTROL_OFFSET      28
#define PCIE_LC_TRAINING_CNTL_LC_ALLOW_TX_L1_CONTROL_MASK        0x10000000

// Bitfield Description : Controls time to wait for Link partner to reply to preset or coefficient requests during Link Equalization.
#define PCIE_LC_TRAINING_CNTL_LC_EXTEND_EQ_REQ_TIME_OFFSET      29
#define PCIE_LC_TRAINING_CNTL_LC_EXTEND_EQ_REQ_TIME_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            LC_TRAINING_CNTL:4;
    UINT32                            LC_COMPLIANCE_RECEIVE:1;
    UINT32                            LC_LOOK_FOR_MORE_NON_MATCHING_TS1:1;
    UINT32                            LC_L0S_L1_TRAINING_CNTL_EN:1;
    UINT32                            LC_L1_LONG_WAKE_FIX_EN:1;
    UINT32                            LC_POWER_STATE:3;
    UINT32                            LC_DONT_GO_TO_L0S_IF_L1_ARMED:1;
    UINT32                            LC_INIT_SPD_CHG_WITH_CSR_EN:1;
    UINT32                            LC_DISABLE_TRAINING_BIT_ARCH:1;
    UINT32                            LC_WAIT_FOR_SETS_IN_RCFG:1;
    UINT32                            LC_HOT_RESET_QUICK_EXIT_EN:1;
    UINT32                            LC_EXTEND_WAIT_FOR_SKP:1;
    UINT32                            LC_AUTONOMOUS_CHANGE_OFF:1;
    UINT32                            LC_UPCONFIGURE_CAP_OFF:1;
    UINT32                            LC_HW_LINK_DIS_EN:1;
    UINT32                            LC_LINK_DIS_BY_HW:1;
    UINT32                            LC_STATIC_TX_PIPE_COUNT_EN:1;
    UINT32                            LC_ASPM_L1_NAK_TIMER_SEL:2;
    UINT32                            LC_DONT_DEASSERT_RX_EN_IN_R_SPEED:1;
    UINT32                            LC_DONT_DEASSERT_RX_EN_IN_TEST:1;
    UINT32                            LC_RESET_ASPM_L1_NAK_TIMER:1;
    UINT32                            LC_SHORT_RCFG_TIMEOUT:1;
    UINT32                            LC_ALLOW_TX_L1_CONTROL:1;
    UINT32                            LC_EXTEND_EQ_REQ_TIME:3;
  } Field;
  UINT32 Value;
} PCIE_LC_TRAINING_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_TRAINING_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_TRAINING_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_TRAINING_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x284)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_TRAINING_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_TRAINING_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_TRAINING_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x284)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_TRAINING_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_TRAINING_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_TRAINING_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x284)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_TRAINING_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_TRAINING_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_TRAINING_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x284)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_TRAINING_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_TRAINING_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_TRAINING_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x284)

#define SMN_FUNC0_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11140284UL

#define SMN_FUNC0_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11240284UL

#define SMN_FUNC0_PCIE2_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11340284UL

#define SMN_FUNC0_PCIE3_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11440284UL

#define SMN_FUNC1_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11141284UL

#define SMN_FUNC1_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11241284UL

#define SMN_FUNC2_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11142284UL

#define SMN_FUNC2_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11242284UL

#define SMN_FUNC3_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11143284UL

#define SMN_FUNC3_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11243284UL

#define SMN_FUNC4_PCIE0_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11144284UL

#define SMN_FUNC4_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11244284UL

#define SMN_FUNC5_PCIE1_PCIE_LC_TRAINING_CNTL_ADDRESS    0x11245284UL


/***********************************************************
* Register Name : PCIE_LC_SPEED_CNTL2
* Register Description :
* Link Speed Control 2 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When LC_FORCE_DIS_SW_SPEED_CHANGE bit is 0, controls forcing the bif_core to allow speed changes initiated by private registers.
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_EN_SW_SPEED_CHANGE_OFFSET      0
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_EN_SW_SPEED_CHANGE_MASK        0x1

// Bitfield Description : Controls forcing the bif_core to prevent speed changes initiated by private registers.
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_DIS_SW_SPEED_CHANGE_OFFSET      1
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_DIS_SW_SPEED_CHANGE_MASK        0x2

// Bitfield Description : When LC_FORCE_DIS_HW_SPEED_CHANGE bit is 0, controls forcing the bif_core to allow speed changes initiated by the chip interface (based on voltage levels).
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_EN_HW_SPEED_CHANGE_OFFSET      2
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_EN_HW_SPEED_CHANGE_MASK        0x4

// Bitfield Description : Controls forcing the bif_core to prevent speed changes initiated by the chip interface (based on voltage levels).
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_DIS_HW_SPEED_CHANGE_OFFSET      3
#define PCIE_LC_SPEED_CNTL2_LC_FORCE_DIS_HW_SPEED_CHANGE_MASK        0x8

// Bitfield Description : Controls allowing link speed negotiation in TX_L0s.
#define PCIE_LC_SPEED_CNTL2_LC_INIT_SPEED_NEG_IN_L0s_EN_OFFSET      4
#define PCIE_LC_SPEED_CNTL2_LC_INIT_SPEED_NEG_IN_L0s_EN_MASK        0x10

// Bitfield Description : Controls allowing link speed negotiation in L1.
#define PCIE_LC_SPEED_CNTL2_LC_INIT_SPEED_NEG_IN_L1_EN_OFFSET      5
#define PCIE_LC_SPEED_CNTL2_LC_INIT_SPEED_NEG_IN_L1_EN_MASK        0x20

// Bitfield Description : Triggers initiating link speed negotiation, when allowed by the register settings.
#define PCIE_LC_SPEED_CNTL2_LC_INITIATE_LINK_SPEED_CHANGE_OFFSET      6
#define PCIE_LC_SPEED_CNTL2_LC_INITIATE_LINK_SPEED_CHANGE_MASK        0x40

// Bitfield Description : Indicates if LTSSM tried to change to a different data rate, but the Link partner refused. When this bit is 1, it blocks hardware (i.e. voltage) initiated link speed changes.
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_STATUS_OFFSET      7
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_STATUS_MASK        0x80

// Bitfield Description : Controls the number of unsuccessful link speed change attempts that are allowed, before LC_SPEED_CHANGE_ATTEMPT_FAILED bit is set.
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_ATTEMPTS_ALLOWED_OFFSET      8
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_ATTEMPTS_ALLOWED_MASK        0x300

// Bitfield Description : Indicates if the number of unsuccessful link speed change attempts has reached the threshold value set in LC_SPEED_CHANGE_ATTEMPTS_ALLOWED field. While this bit is 1, the Port is prevented from initiating any more link speed changes. This bit can be cleared by setting the LC_CLR_FAILED_SPD_CHANGE_CNT bit to 1.
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_ATTEMPT_FAILED_OFFSET      10
#define PCIE_LC_SPEED_CNTL2_LC_SPEED_CHANGE_ATTEMPT_FAILED_MASK        0x400

// Bitfield Description : Controls clearing the LC_SPEED_CHANGE_ATTEMPT_FAILED bit and its associated counter, when this bit is written with a 1. Writes of 0 have no effect.
#define PCIE_LC_SPEED_CNTL2_LC_CLR_FAILED_SPD_CHANGE_CNT_OFFSET      11
#define PCIE_LC_SPEED_CNTL2_LC_CLR_FAILED_SPD_CHANGE_CNT_MASK        0x800

// Bitfield Description : (Downstream Ports only) When STRAP_BIF_AUTO_RC_SPEED_NEGOTIATION_DIS bit is 0, controls allowing the upstream component to initiate multiple link speed changes to the highest commonly supported link speed. Multiple speed changes are only allowed if there were no failures in previous link speed change attempts.
#define PCIE_LC_SPEED_CNTL2_LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN_OFFSET      12
#define PCIE_LC_SPEED_CNTL2_LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN_MASK        0x1000

// Bitfield Description : Controls if Target Link Speed initiated link speed negotiation failure counter is reset.
#define PCIE_LC_SPEED_CNTL2_LC_DONT_CLR_TARGET_SPD_CHANGE_STATUS_OFFSET      13
#define PCIE_LC_SPEED_CNTL2_LC_DONT_CLR_TARGET_SPD_CHANGE_STATUS_MASK        0x2000

// Bitfield Description : Controls allowing transition from Recovery.RcvrCfg to Recovery.Speed when one or more but not all eight TS2s (with required parameters for a speed change) are received.
#define PCIE_LC_SPEED_CNTL2_LC_1_OR_MORE_TS2_SPEED_ARC_EN_OFFSET      14
#define PCIE_LC_SPEED_CNTL2_LC_1_OR_MORE_TS2_SPEED_ARC_EN_MASK        0x4000

// Bitfield Description : (Downstream Ports only) If the initial autonomous Link Equalization was not successful for a given data rate, abort autonomous Link Equalization for that data rate and any higher data rates.
#define PCIE_LC_SPEED_CNTL2_LC_ABORT_AUTO_EQ_AFTER_FAILED_EQ_OFFSET      15
#define PCIE_LC_SPEED_CNTL2_LC_ABORT_AUTO_EQ_AFTER_FAILED_EQ_MASK        0x8000

// Bitfield Description : (Downstream Ports only) If Link Equalization has not been done for a given data rate, do not advertise that data rate unless the Link Equalization can be initiated from the current link speed.
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_CORRECT_SPEED_FOR_EQ_OFFSET      16
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_CORRECT_SPEED_FOR_EQ_MASK        0x10000

// Bitfield Description : (Downstream Ports only) Once the Link goes DL_Active, if there are any data rates that have not performed autonomous Link Equalization, do not advertise those data rates until software requests Link Equalization for one of those data rates.
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_SOFTWARE_PERFORM_EQ_OFFSET      17
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_SOFTWARE_PERFORM_EQ_MASK        0x20000

// Bitfield Description : Controls whether EQ TS2s or 128b/130b EQ TS2s (as appropriate) can be sent, for Link Equalization at a given data rate, based on when the Link partner advertised support for that data rate, since the last time it exited Detect.
#define PCIE_LC_SPEED_CNTL2_LC_SEND_EQ_TS2_IF_OTHER_SIDE_EVER_ADVERTISED_SPEED_OFFSET      18
#define PCIE_LC_SPEED_CNTL2_LC_SEND_EQ_TS2_IF_OTHER_SIDE_EVER_ADVERTISED_SPEED_MASK        0x40000

// Bitfield Description : (Downstream Ports only) Controls if only a single Link Equalization can be performed during the current Recovery.
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_SINGLE_EQ_PER_RECOVERY_OFFSET      19
#define PCIE_LC_SPEED_CNTL2_LC_ENFORCE_SINGLE_EQ_PER_RECOVERY_MASK        0x80000

// Bitfield Description : (Downstream Ports only) Controls if the legacy (pre 6.0) conditions are used to clear DELAY_DLLPs.
#define PCIE_LC_SPEED_CNTL2_LC_USE_LEGACY_CLEAR_DELAY_DLLPs_OFFSET      20
#define PCIE_LC_SPEED_CNTL2_LC_USE_LEGACY_CLEAR_DELAY_DLLPs_MASK        0x100000

// Bitfield Description : (Downstream Ports only) Controls if a software write of 1 to RETRAIN_LINK bit, while the LTSSM is currently in Recovery, always results in the LTSSM completing the current Recovery and then initiating a new Recovery.
#define PCIE_LC_SPEED_CNTL2_LC_DEFER_RETRAIN_LINK_UNTIL_EXIT_RECOVERY_OFFSET      21
#define PCIE_LC_SPEED_CNTL2_LC_DEFER_RETRAIN_LINK_UNTIL_EXIT_RECOVERY_MASK        0x200000

// Bitfield Description :
#define PCIE_LC_SPEED_CNTL2_Reserved_31_22_OFFSET      22
#define PCIE_LC_SPEED_CNTL2_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            LC_FORCE_EN_SW_SPEED_CHANGE:1;
    UINT32                            LC_FORCE_DIS_SW_SPEED_CHANGE:1;
    UINT32                            LC_FORCE_EN_HW_SPEED_CHANGE:1;
    UINT32                            LC_FORCE_DIS_HW_SPEED_CHANGE:1;
    UINT32                            LC_INIT_SPEED_NEG_IN_L0s_EN:1;
    UINT32                            LC_INIT_SPEED_NEG_IN_L1_EN:1;
    UINT32                            LC_INITIATE_LINK_SPEED_CHANGE:1;
    UINT32                            LC_SPEED_CHANGE_STATUS:1;
    UINT32                            LC_SPEED_CHANGE_ATTEMPTS_ALLOWED:2;
    UINT32                            LC_SPEED_CHANGE_ATTEMPT_FAILED:1;
    UINT32                            LC_CLR_FAILED_SPD_CHANGE_CNT:1;
    UINT32                            LC_MULT_UPSTREAM_AUTO_SPD_CHNG_EN:1;
    UINT32                            LC_DONT_CLR_TARGET_SPD_CHANGE_STATUS:1;
    UINT32                            LC_1_OR_MORE_TS2_SPEED_ARC_EN:1;
    UINT32                            LC_ABORT_AUTO_EQ_AFTER_FAILED_EQ:1;
    UINT32                            LC_ENFORCE_CORRECT_SPEED_FOR_EQ:1;
    UINT32                            LC_ENFORCE_SOFTWARE_PERFORM_EQ:1;
    UINT32                            LC_SEND_EQ_TS2_IF_OTHER_SIDE_EVER_ADVERTISED_SPEED:1;
    UINT32                            LC_ENFORCE_SINGLE_EQ_PER_RECOVERY:1;
    UINT32                            LC_USE_LEGACY_CLEAR_DELAY_DLLPs:1;
    UINT32                            LC_DEFER_RETRAIN_LINK_UNTIL_EXIT_RECOVERY:1;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} PCIE_LC_SPEED_CNTL2_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL2_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL2_OFFSET       0x414
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x414)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL2_OFFSET       0x414
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x414)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL2_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL2_OFFSET       0x414
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x414)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL2_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL2_OFFSET       0x414
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x414)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL2_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL2_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL2_OFFSET       0x414
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x414)

#define SMN_FUNC0_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11140414UL

#define SMN_FUNC0_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11240414UL

#define SMN_FUNC0_PCIE2_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11340414UL

#define SMN_FUNC0_PCIE3_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11440414UL

#define SMN_FUNC1_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11141414UL

#define SMN_FUNC1_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11241414UL

#define SMN_FUNC2_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11142414UL

#define SMN_FUNC2_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11242414UL

#define SMN_FUNC3_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11143414UL

#define SMN_FUNC3_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11243414UL

#define SMN_FUNC4_PCIE0_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11144414UL

#define SMN_FUNC4_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11244414UL

#define SMN_FUNC5_PCIE1_PCIE_LC_SPEED_CNTL2_ADDRESS    0x11245414UL

/***********************************************************
* Register Name : PCIE_TX_PORT_CTRL_1
* Register Description :
* TX Port Control 1 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define PCIE_TX_PORT_CTRL_1_Reserved_9_0_OFFSET      0
#define PCIE_TX_PORT_CTRL_1_Reserved_9_0_MASK        0x3ff

// Bitfield Description : Controls the value for the No Snoop bit for master requests.
#define PCIE_TX_PORT_CTRL_1_TX_SNR_OVERRIDE_OFFSET      10
#define PCIE_TX_PORT_CTRL_1_TX_SNR_OVERRIDE_MASK        0xc00

// Bitfield Description : Controls the value for the Relaxed Ordering bit for master requests.
#define PCIE_TX_PORT_CTRL_1_TX_RO_OVERRIDE_OFFSET      12
#define PCIE_TX_PORT_CTRL_1_TX_RO_OVERRIDE_MASK        0x3000

// Bitfield Description : Controls back-to-back packing of TLPs and DLLPs.
#define PCIE_TX_PORT_CTRL_1_TX_PACK_PACKET_DIS_OFFSET      14
#define PCIE_TX_PORT_CTRL_1_TX_PACK_PACKET_DIS_MASK        0x4000

// Bitfield Description : Controls if TLPs are flushed when Data Link is down.
#define PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_OFFSET      15
#define PCIE_TX_PORT_CTRL_1_TX_FLUSH_TLP_DIS_MASK        0x8000

// Bitfield Description :
#define PCIE_TX_PORT_CTRL_1_Reserved_19_16_OFFSET      16
#define PCIE_TX_PORT_CTRL_1_Reserved_19_16_MASK        0xf0000

// Bitfield Description : Controls the ordering rule, let completion pass posted request.
#define PCIE_TX_PORT_CTRL_1_TX_CPL_PASS_P_OFFSET      20
#define PCIE_TX_PORT_CTRL_1_TX_CPL_PASS_P_MASK        0x100000

// Bitfield Description : Controls the ordering rule, let non-posted request pass posted request.
#define PCIE_TX_PORT_CTRL_1_TX_NP_PASS_P_OFFSET      21
#define PCIE_TX_PORT_CTRL_1_TX_NP_PASS_P_MASK        0x200000

// Bitfield Description : Controls the clearing of excess PM DLLPs from pipe.
#define PCIE_TX_PORT_CTRL_1_TX_CLEAR_EXTRA_PM_REQS_OFFSET      22
#define PCIE_TX_PORT_CTRL_1_TX_CLEAR_EXTRA_PM_REQS_MASK        0x400000

// Bitfield Description : Controls automatic flow control updates based on timer.
#define PCIE_TX_PORT_CTRL_1_TX_FC_UPDATE_TIMEOUT_DIS_OFFSET      23
#define PCIE_TX_PORT_CTRL_1_TX_FC_UPDATE_TIMEOUT_DIS_MASK        0x800000

// Bitfield Description :
#define PCIE_TX_PORT_CTRL_1_Reserved_26_24_OFFSET      24
#define PCIE_TX_PORT_CTRL_1_Reserved_26_24_MASK        0x7000000

// Bitfield Description : Controls Encapsulated Message override from Routed to RC, to Broadcast from RC.
#define PCIE_TX_PORT_CTRL_1_TX_SWAP_RTRC_WITH_BFRC_ENABLE_OFFSET      27
#define PCIE_TX_PORT_CTRL_1_TX_SWAP_RTRC_WITH_BFRC_ENABLE_MASK        0x8000000

// Bitfield Description :
#define PCIE_TX_PORT_CTRL_1_Reserved_31_28_OFFSET      28
#define PCIE_TX_PORT_CTRL_1_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_9_0:10;
    UINT32                            TX_SNR_OVERRIDE:2;
    UINT32                            TX_RO_OVERRIDE:2;
    UINT32                            TX_PACK_PACKET_DIS:1;
    UINT32                            TX_FLUSH_TLP_DIS:1;
    UINT32                            Reserved_19_16:4;
    UINT32                            TX_CPL_PASS_P:1;
    UINT32                            TX_NP_PASS_P:1;
    UINT32                            TX_CLEAR_EXTRA_PM_REQS:1;
    UINT32                            TX_FC_UPDATE_TIMEOUT_DIS:1;
    UINT32                            Reserved_26_24:3;
    UINT32                            TX_SWAP_RTRC_WITH_BFRC_ENABLE:1;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} PCIE_TX_PORT_CTRL_1_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_PORT_CTRL_1_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_PORT_CTRL_1_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_PORT_CTRL_1_OFFSET       0x600
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x600)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_PORT_CTRL_1_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_PORT_CTRL_1_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_PORT_CTRL_1_OFFSET       0x600
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x600)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_PORT_CTRL_1_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_PORT_CTRL_1_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_PORT_CTRL_1_OFFSET       0x600
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x600)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_PORT_CTRL_1_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_PORT_CTRL_1_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_PORT_CTRL_1_OFFSET       0x600
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x600)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_PORT_CTRL_1_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_PORT_CTRL_1_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_PORT_CTRL_1_OFFSET       0x600
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x600)

#define SMN_FUNC0_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11140600UL

#define SMN_FUNC0_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11240600UL

#define SMN_FUNC0_PCIE2_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11340600UL

#define SMN_FUNC0_PCIE3_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11440600UL

#define SMN_FUNC1_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11141600UL

#define SMN_FUNC1_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11241600UL

#define SMN_FUNC2_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11142600UL

#define SMN_FUNC2_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11242600UL

#define SMN_FUNC3_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11143600UL

#define SMN_FUNC3_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11243600UL

#define SMN_FUNC4_PCIE0_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11144600UL

#define SMN_FUNC4_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11244600UL

#define SMN_FUNC5_PCIE1_PCIE_TX_PORT_CTRL_1_ADDRESS    0x11245600UL


/***********************************************************
* Register Name : PCIE_TX_REQUESTER_ID
* Register Description :
* TX Requester ID register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Function ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_FUNCTION_OFFSET      0
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_FUNCTION_MASK        0x7

// Bitfield Description : Device ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_DEVICE_OFFSET      3
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_DEVICE_MASK        0xf8

// Bitfield Description : Bus ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_BUS_OFFSET      8
#define PCIE_TX_REQUESTER_ID_TX_REQUESTER_ID_BUS_MASK        0xff00

// Bitfield Description : Function ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_FUNCTION_OFFSET      16
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_FUNCTION_MASK        0x70000

// Bitfield Description : Device ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_DEVICE_OFFSET      19
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_DEVICE_MASK        0xf80000

// Bitfield Description : Bus ID of Requester for Master transactions or Completer for Slave Completions.
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_BUS_OFFSET      24
#define PCIE_TX_REQUESTER_ID_TX_SWUS_REQUESTER_ID_BUS_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TX_REQUESTER_ID_FUNCTION:3;
    UINT32                            TX_REQUESTER_ID_DEVICE:5;
    UINT32                            TX_REQUESTER_ID_BUS:8;
    UINT32                            TX_SWUS_REQUESTER_ID_FUNCTION:3;
    UINT32                            TX_SWUS_REQUESTER_ID_DEVICE:5;
    UINT32                            TX_SWUS_REQUESTER_ID_BUS:8;
  } Field;
  UINT32 Value;
} PCIE_TX_REQUESTER_ID_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_REQUESTER_ID_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_REQUESTER_ID_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_REQUESTER_ID_OFFSET       0x84
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x84)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_REQUESTER_ID_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_REQUESTER_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_REQUESTER_ID_OFFSET       0x84
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x84)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_REQUESTER_ID_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_REQUESTER_ID_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_REQUESTER_ID_OFFSET       0x84
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x84)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_REQUESTER_ID_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_REQUESTER_ID_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_REQUESTER_ID_OFFSET       0x84
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x84)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_REQUESTER_ID_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_REQUESTER_ID_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_REQUESTER_ID_OFFSET       0x84
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x84)

#define SMN_FUNC0_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS    0x11140084UL

#define SMN_FUNC0_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11240084UL

#define SMN_FUNC0_PCIE2_PCIE_TX_REQUESTER_ID_ADDRESS    0x11340084UL

#define SMN_FUNC0_PCIE3_PCIE_TX_REQUESTER_ID_ADDRESS    0x11440084UL

#define SMN_FUNC1_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS    0x11141084UL

#define SMN_FUNC1_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11241084UL

#define SMN_FUNC2_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS    0x11142084UL

#define SMN_FUNC2_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11242084UL

#define SMN_FUNC3_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS    0x11143084UL

#define SMN_FUNC3_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11243084UL

#define SMN_FUNC4_PCIE0_PCIE_TX_REQUESTER_ID_ADDRESS    0x11144084UL

#define SMN_FUNC4_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11244084UL

#define SMN_FUNC5_PCIE1_PCIE_TX_REQUESTER_ID_ADDRESS    0x11245084UL


//PciePortInit.h
//PCIERCCFG.h
//RIOMMU.h
/***********************************************************
* Register Name : RIOMMU_REFCLK_MODE
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : REFCLK is running at 100 MHZ
#define RIOMMU_REFCLK_MODE_MODE_100MHZ_OFFSET      0
#define RIOMMU_REFCLK_MODE_MODE_100MHZ_MASK        0x1

// Bitfield Description : REFCLK is running at 25 MHZ
#define RIOMMU_REFCLK_MODE_MODE_25MHZ_OFFSET      1
#define RIOMMU_REFCLK_MODE_MODE_25MHZ_MASK        0x2

// Bitfield Description : REFCLK is running at 27 MHZ
#define RIOMMU_REFCLK_MODE_MODE_27MHZ_OFFSET      2
#define RIOMMU_REFCLK_MODE_MODE_27MHZ_MASK        0x4

// Bitfield Description :
#define RIOMMU_REFCLK_MODE_Reserved_31_3_OFFSET      3
#define RIOMMU_REFCLK_MODE_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            MODE_100MHZ:1;
    UINT32                            MODE_25MHZ:1;
    UINT32                            MODE_27MHZ:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} RIOMMU_REFCLK_MODE_STRUCT;

#define SMN_RIOMMU_REFCLK_MODE_ADDRESS    0x16b0a120UL


/***********************************************************
* Register Name : SHDW_RIOMMU_INV_CNTRL
* Register Description :
*
* Visibility : 0x8
************************************************************/

// Bitfield Description :
#define SHDW_RIOMMU_INV_CNTRL_Reserved_0_0_OFFSET      0
#define SHDW_RIOMMU_INV_CNTRL_Reserved_0_0_MASK        0x1

// Bitfield Description : This 5-bit value sets the limit for number of outstanding invalidations to each riommu client that iommu will manage. Actual limit is value + 1.
#define SHDW_RIOMMU_INV_CNTRL_MULTI_INV_LIM_OFFSET      1
#define SHDW_RIOMMU_INV_CNTRL_MULTI_INV_LIM_MASK        0x3e

// Bitfield Description : This 5-bit value sets the limit for number invalidation acks that each riommu client will transmit to main iommu in a single AXI transaction. Actual limit is value + 1.
#define SHDW_RIOMMU_INV_CNTRL_MULTI_INVACK_LIM_OFFSET      6
#define SHDW_RIOMMU_INV_CNTRL_MULTI_INVACK_LIM_MASK        0x7c0

// Bitfield Description :
#define SHDW_RIOMMU_INV_CNTRL_Reserved_31_11_OFFSET      11
#define SHDW_RIOMMU_INV_CNTRL_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            MULTI_INV_LIM:5;
    UINT32                            MULTI_INVACK_LIM:5;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} SHDW_RIOMMU_INV_CNTRL_STRUCT;

#define SMN_SHDW_RIOMMU_INV_CNTRL_ADDRESS    0x16b0e200UL


//SMU_FUSE.h
/***********************************************************
* Register Name : FUSE_DATA_311
* Register Description :
* Fuse Data 311
* Visibility : 0x8
************************************************************/

// Bitfield Description : [LOCAL]"DIO/DMU is disabled".  Die=RMB IP-Fuse-Name=MP0_DCN_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9952 Start-Row=311 Start-Bit=0 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_DCN_DISABLE_OFFSET      0
#define FUSE_DATA_311_MP0_MP0_DCN_DISABLE_MASK        0x1

// Bitfield Description : [LOCAL]"GC is disabled".  Die=RMB IP-Fuse-Name=MP0_GFX_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9953 Start-Row=311 Start-Bit=1 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_GFX_DISABLE_OFFSET      1
#define FUSE_DATA_311_MP0_MP0_GFX_DISABLE_MASK        0x2

// Bitfield Description : [LOCAL]"MMHUB is disabled".  Die=RMB IP-Fuse-Name=MP0_MMHUB_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9954 Start-Row=311 Start-Bit=2 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_MMHUB_DISABLE_OFFSET      2
#define FUSE_DATA_311_MP0_MP0_MMHUB_DISABLE_MASK        0x4

// Bitfield Description : [LOCAL]"L1IMUPCIE1/L1IMUPCIE2/PCIE2/PCIE3/USB3/DPA/USB4/USB4RT0/USB4RT1/USB4PHY0/USB4PHY1 is disabled".  Die=RMB IP-Fuse-Name=MP0_USB4_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9955 Start-Row=311 Start-Bit=3 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_USB4_DISABLE_OFFSET      3
#define FUSE_DATA_311_MP0_MP0_USB4_DISABLE_MASK        0x8

// Bitfield Description : [LOCAL]"UVD0 is disabled".  Die=RMB IP-Fuse-Name=MP0_VCN_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9956 Start-Row=311 Start-Bit=4 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_VCN_DISABLE_OFFSET      4
#define FUSE_DATA_311_MP0_MP0_VCN_DISABLE_MASK        0x10

// Bitfield Description : [LOCAL]"XGBE0/PCS3/PCS4 is disabled".  Die=RMB IP-Fuse-Name=MP0_XGBE_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Ho, David Write-Dis=Y Secure=NS JTAG=9957 Start-Row=311 Start-Bit=5 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_MP0_XGBE_DISABLE_OFFSET      5
#define FUSE_DATA_311_MP0_MP0_XGBE_DISABLE_MASK        0x20

// Bitfield Description : [LOCAL]"1 - multi thread, 0 - single thread".  Die=RMB IP-Fuse-Name=SMT_ENABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=1 Owner=Dittmann, Justin Write-Dis=N Secure=NS JTAG=9958 Start-Row=311 Start-Bit=6 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_SMT_ENABLE_OFFSET      6
#define FUSE_DATA_311_MP0_SMT_ENABLE_MASK        0x40

// Bitfield Description : [LOCAL]"CPU core disable".  Die=RMB IP-Fuse-Name=CORE_DISABLE Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=8 Owner=Dittmann, Justin Write-Dis=N Secure=NS JTAG=9966:9959 Start-Row=311 Start-Bit=7 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_CORE_DISABLE_OFFSET      7
#define FUSE_DATA_311_MP0_CORE_DISABLE_MASK        0x7f80

// Bitfield Description : [LOCAL]"Spare IP disable fuses for harvesting ".  Die=RMB IP-Fuse-Name=HARVESTING_FUSES Consumer-IP=MP0 IP-Instance=MP0 Category=MP0_NONSECURE_HARVEST Size=17 Owner=Ho, David Write-Dis=N Secure=NS JTAG=9983:9967 Start-Row=311 Start-Bit=15 End-Row=311 End-Bit=311
#define FUSE_DATA_311_MP0_HARVESTING_FUSES_OFFSET      15
#define FUSE_DATA_311_MP0_HARVESTING_FUSES_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            MP0_MP0_DCN_DISABLE:1;
    UINT32                            MP0_MP0_GFX_DISABLE:1;
    UINT32                            MP0_MP0_MMHUB_DISABLE:1;
    UINT32                            MP0_MP0_USB4_DISABLE:1;
    UINT32                            MP0_MP0_VCN_DISABLE:1;
    UINT32                            MP0_MP0_XGBE_DISABLE:1;
    UINT32                            MP0_SMT_ENABLE:1;
    UINT32                            MP0_CORE_DISABLE:8;
    UINT32                            MP0_HARVESTING_FUSES:17;
  } Field;
  UINT32 Value;
} FUSE_DATA_311_STRUCT;

#define SMN_FUSE_DATA_311_ADDRESS    0x5d4dcUL


//SMU_SMUIO.h
//SST.h
//SYSHUBMM.h

/***********************************************************
* Register Name : A2S_CNTL_SW0
* Register Description :
* A2S control for A2S convertor 0 for DMACK0V3.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Static VC Override For Per Switch.
#define A2S_CNTL_SW0_STATIC_VC_ENABLE_OFFSET      0
#define A2S_CNTL_SW0_STATIC_VC_ENABLE_MASK        0x1

// Bitfield Description : Static VC Value For Per Switch.
#define A2S_CNTL_SW0_STATIC_VC_VALUE_OFFSET      1
#define A2S_CNTL_SW0_STATIC_VC_VALUE_MASK        0xe

// Bitfield Description :
#define A2S_CNTL_SW0_Reserved_5_4_OFFSET      4
#define A2S_CNTL_SW0_Reserved_5_4_MASK        0x30

// Bitfield Description : Force-waive all response to be returned in the same order as request.
#define A2S_CNTL_SW0_FORCE_RSP_REORDER_EN_OFFSET      6
#define A2S_CNTL_SW0_FORCE_RSP_REORDER_EN_MASK        0x40

// Bitfield Description :
#define A2S_CNTL_SW0_Reserved_8_7_OFFSET      7
#define A2S_CNTL_SW0_Reserved_8_7_MASK        0x180

// Bitfield Description : Disable chaining SDP write, Msg/Msgd must be chained and it is not controlled by this register.
#define A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_OFFSET      9
#define A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_MASK        0x200

// Bitfield Description :
#define A2S_CNTL_SW0_Reserved_15_10_OFFSET      10
#define A2S_CNTL_SW0_Reserved_15_10_MASK        0xfc00

// Bitfield Description : Weight assigned for AXI read request in WRR arbiter.
#define A2S_CNTL_SW0_WRR_RD_WEIGHT_OFFSET      16
#define A2S_CNTL_SW0_WRR_RD_WEIGHT_MASK        0xff0000

// Bitfield Description : Weight assigned for AXI write request in WRR arbiter.
#define A2S_CNTL_SW0_WRR_WR_WEIGHT_OFFSET      24
#define A2S_CNTL_SW0_WRR_WR_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            STATIC_VC_ENABLE:1;
    UINT32                            STATIC_VC_VALUE:3;
    UINT32                            Reserved_5_4:2;
    UINT32                            FORCE_RSP_REORDER_EN:1;
    UINT32                            Reserved_8_7:2;
    UINT32                            SDP_WR_CHAIN_DIS:1;
    UINT32                            Reserved_15_10:6;
    UINT32                            WRR_RD_WEIGHT:8;
    UINT32                            WRR_WR_WEIGHT:8;
  } Field;
  UINT32 Value;
} A2S_CNTL_SW0_STRUCT;

#define SMN_NBIF0_A2S_CNTL_SW0_ADDRESS    0x1403b40UL

/***********************************************************
* Register Name : A2S_CNTL_SW3
* Register Description :
* A2S control for A2S convertor 3 for DMACK1V5.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Static VC Override For Per Switch.
#define A2S_CNTL_SW3_STATIC_VC_ENABLE_OFFSET      0
#define A2S_CNTL_SW3_STATIC_VC_ENABLE_MASK        0x1

// Bitfield Description : Static VC Value For Per Switch.
#define A2S_CNTL_SW3_STATIC_VC_VALUE_OFFSET      1
#define A2S_CNTL_SW3_STATIC_VC_VALUE_MASK        0xe

// Bitfield Description :
#define A2S_CNTL_SW3_Reserved_5_4_OFFSET      4
#define A2S_CNTL_SW3_Reserved_5_4_MASK        0x30

// Bitfield Description : Force-waive all response to be returned in the same order as request.
#define A2S_CNTL_SW3_FORCE_RSP_REORDER_EN_OFFSET      6
#define A2S_CNTL_SW3_FORCE_RSP_REORDER_EN_MASK        0x40

// Bitfield Description :
#define A2S_CNTL_SW3_Reserved_8_7_OFFSET      7
#define A2S_CNTL_SW3_Reserved_8_7_MASK        0x180

// Bitfield Description : Disable chaining SDP write, Msg/Msgd must be chained and it is not controlled by this register.
#define A2S_CNTL_SW3_SDP_WR_CHAIN_DIS_OFFSET      9
#define A2S_CNTL_SW3_SDP_WR_CHAIN_DIS_MASK        0x200

// Bitfield Description :
#define A2S_CNTL_SW3_Reserved_15_10_OFFSET      10
#define A2S_CNTL_SW3_Reserved_15_10_MASK        0xfc00

// Bitfield Description : Weight assigned for AXI read request in WRR arbiter.
#define A2S_CNTL_SW3_WRR_RD_WEIGHT_OFFSET      16
#define A2S_CNTL_SW3_WRR_RD_WEIGHT_MASK        0xff0000

// Bitfield Description : Weight assigned for AXI write request in WRR arbiter.
#define A2S_CNTL_SW3_WRR_WR_WEIGHT_OFFSET      24
#define A2S_CNTL_SW3_WRR_WR_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            STATIC_VC_ENABLE:1;
    UINT32                            STATIC_VC_VALUE:3;
    UINT32                            Reserved_5_4:2;
    UINT32                            FORCE_RSP_REORDER_EN:1;
    UINT32                            Reserved_8_7:2;
    UINT32                            SDP_WR_CHAIN_DIS:1;
    UINT32                            Reserved_15_10:6;
    UINT32                            WRR_RD_WEIGHT:8;
    UINT32                            WRR_WR_WEIGHT:8;
  } Field;
  UINT32 Value;
} A2S_CNTL_SW3_STRUCT;

#define SMN_NBIF0_A2S_CNTL_SW3_ADDRESS    0x1403b4cUL

/***********************************************************
* Register Name : A2S_CNTL_SW4
* Register Description :
* A2S control for A2S convertor 4 for DMACK3B.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Static VC Override For Per Switch.
#define A2S_CNTL_SW4_STATIC_VC_ENABLE_OFFSET      0
#define A2S_CNTL_SW4_STATIC_VC_ENABLE_MASK        0x1

// Bitfield Description : Static VC Value For Per Switch.
#define A2S_CNTL_SW4_STATIC_VC_VALUE_OFFSET      1
#define A2S_CNTL_SW4_STATIC_VC_VALUE_MASK        0xe

// Bitfield Description :
#define A2S_CNTL_SW4_Reserved_5_4_OFFSET      4
#define A2S_CNTL_SW4_Reserved_5_4_MASK        0x30

// Bitfield Description : Force-waive all response to be returned in the same order as request.
#define A2S_CNTL_SW4_FORCE_RSP_REORDER_EN_OFFSET      6
#define A2S_CNTL_SW4_FORCE_RSP_REORDER_EN_MASK        0x40

// Bitfield Description :
#define A2S_CNTL_SW4_Reserved_8_7_OFFSET      7
#define A2S_CNTL_SW4_Reserved_8_7_MASK        0x180

// Bitfield Description : Disable chaining SDP write, Msg/Msgd must be chained and it is not controlled by this register.
#define A2S_CNTL_SW4_SDP_WR_CHAIN_DIS_OFFSET      9
#define A2S_CNTL_SW4_SDP_WR_CHAIN_DIS_MASK        0x200

// Bitfield Description :
#define A2S_CNTL_SW4_Reserved_15_10_OFFSET      10
#define A2S_CNTL_SW4_Reserved_15_10_MASK        0xfc00

// Bitfield Description : Weight assigned for AXI read request in WRR arbiter.
#define A2S_CNTL_SW4_WRR_RD_WEIGHT_OFFSET      16
#define A2S_CNTL_SW4_WRR_RD_WEIGHT_MASK        0xff0000

// Bitfield Description : Weight assigned for AXI write request in WRR arbiter.
#define A2S_CNTL_SW4_WRR_WR_WEIGHT_OFFSET      24
#define A2S_CNTL_SW4_WRR_WR_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            STATIC_VC_ENABLE:1;
    UINT32                            STATIC_VC_VALUE:3;
    UINT32                            Reserved_5_4:2;
    UINT32                            FORCE_RSP_REORDER_EN:1;
    UINT32                            Reserved_8_7:2;
    UINT32                            SDP_WR_CHAIN_DIS:1;
    UINT32                            Reserved_15_10:6;
    UINT32                            WRR_RD_WEIGHT:8;
    UINT32                            WRR_WR_WEIGHT:8;
  } Field;
  UINT32 Value;
} A2S_CNTL_SW4_STRUCT;

#define SMN_NBIF0_A2S_CNTL_SW4_ADDRESS    0x1403b50UL

/***********************************************************
* Register Name : A2S_REQ_RSP_TAG_CNTL
* Register Description :
* A2S Read/Write Request and Response Control.
* Visibility : 0x4
************************************************************/

// Bitfield Description : WRR or LRG mode for arbiter in A2S Read Tag Control Module. 0 -- WRR mode; 1 -- LRG mode.
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_MODE_OFFSET      0
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_MODE_MASK        0x1

// Bitfield Description : Arbitration mode for A2S Read Tag Control Module WRR LRG arbiter in A2S convertor.0 -- arbitrate based on Request number; 1 -- arbitrate based on Requster size in unit of 64B.
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_SIZE_MODE_OFFSET      1
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_SIZE_MODE_MASK        0x2

// Bitfield Description : counter retain or clear after grant shift for arbiter in A2S Read Tag Control Module. 0 -- counter clear after grant shift; 1 -- counter retain after grant shift.
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_COUNTER_MODE_OFFSET      2
#define A2S_REQ_RSP_TAG_CNTL_RD_WRR_LRG_COUNTER_MODE_MASK        0x4

// Bitfield Description : Arbiter mode for A2S Read Tag Response Module.. 0 -- Round-Robin;  1 -- Strict Priority.
#define A2S_REQ_RSP_TAG_CNTL_RD_A2S_RSP_ARBMODE_OFFSET      3
#define A2S_REQ_RSP_TAG_CNTL_RD_A2S_RSP_ARBMODE_MASK        0x8

// Bitfield Description :
#define A2S_REQ_RSP_TAG_CNTL_Reserved_15_4_OFFSET      4
#define A2S_REQ_RSP_TAG_CNTL_Reserved_15_4_MASK        0xfff0

// Bitfield Description : WRR or LRG mode for arbiter in A2S Write Tag Control Module. 0 -- WRR mode; 1 -- LRG mode.
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_MODE_OFFSET      16
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_MODE_MASK        0x10000

// Bitfield Description : Arbitration mode for A2S Write Tag Control Module WRR LRG arbiter in A2S convertor.0 -- arbitrate based on Request number; 1 -- arbitrate based on Requster size in unit of 64B.
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_SIZE_MODE_OFFSET      17
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_SIZE_MODE_MASK        0x20000

// Bitfield Description : counter retain or clear after grant shift for arbiter in A2S Write Tag Control Module. 0 -- counter clear after grant shift; 1 -- counter retain after grant shift.
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_COUNTER_MODE_OFFSET      18
#define A2S_REQ_RSP_TAG_CNTL_WR_WRR_LRG_COUNTER_MODE_MASK        0x40000

// Bitfield Description : Arbiter mode for A2S Write Tag Response Module. 0 -- Round-Robin;  1 -- Strict Priority.
#define A2S_REQ_RSP_TAG_CNTL_WR_A2S_RSP_ARBMODE_OFFSET      19
#define A2S_REQ_RSP_TAG_CNTL_WR_A2S_RSP_ARBMODE_MASK        0x80000

// Bitfield Description : Check tag when send write chain. If there is no pool tag could be used, there will be no write chain for write request. 0 -- Disable tag check;  1 -- Enable tag check.
#define A2S_REQ_RSP_TAG_CNTL_WR_TAG_FOR_CHAIN_ENABLE_OFFSET      20
#define A2S_REQ_RSP_TAG_CNTL_WR_TAG_FOR_CHAIN_ENABLE_MASK        0x100000

// Bitfield Description :
#define A2S_REQ_RSP_TAG_CNTL_Reserved_31_21_OFFSET      21
#define A2S_REQ_RSP_TAG_CNTL_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            RD_WRR_LRG_MODE:1;
    UINT32                            RD_WRR_LRG_SIZE_MODE:1;
    UINT32                            RD_WRR_LRG_COUNTER_MODE:1;
    UINT32                            RD_A2S_RSP_ARBMODE:1;
    UINT32                            Reserved_15_4:12;
    UINT32                            WR_WRR_LRG_MODE:1;
    UINT32                            WR_WRR_LRG_SIZE_MODE:1;
    UINT32                            WR_WRR_LRG_COUNTER_MODE:1;
    UINT32                            WR_A2S_RSP_ARBMODE:1;
    UINT32                            WR_TAG_FOR_CHAIN_ENABLE:1;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} A2S_REQ_RSP_TAG_CNTL_STRUCT;

#define SMN_NBIF0_A2S_REQ_RSP_TAG_CNTL_ADDRESS    0x1403b6cUL

/***********************************************************
* Register Name : A2S_TAG_ALLOC_0
* Register Description :
* A2S Tag allocation control register 0.
* Visibility : 0x4
************************************************************/

// Bitfield Description : The number of tag reserved for VC0 write.
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_WR_OFFSET      0
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_WR_MASK        0xff

// Bitfield Description : The number of tag reserved for VC0 read.
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_RD_OFFSET      8
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC0_RD_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC1 write.
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC1_WR_OFFSET      16
#define A2S_TAG_ALLOC_0_TAG_ALLOC_FOR_VC1_WR_MASK        0xff0000

// Bitfield Description :
#define A2S_TAG_ALLOC_0_Reserved_31_24_OFFSET      24
#define A2S_TAG_ALLOC_0_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC0_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC0_RD:8;
    UINT32                            TAG_ALLOC_FOR_VC1_WR:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} A2S_TAG_ALLOC_0_STRUCT;

#define SMN_NBIF0_A2S_TAG_ALLOC_0_ADDRESS    0x1403b74UL
#define SMN_NBIF0_A2S_TAG_ALLOC_0_ADDRESS_Bx 0x140F954UL


/***********************************************************
* Register Name : A2S_TAG_ALLOC_1
* Register Description :
* A2S Tag allocation control register 1.
* Visibility : 0x4
************************************************************/

// Bitfield Description : The number of tag reserved for VC3 write.
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC3_WR_OFFSET      0
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC3_WR_MASK        0xff

// Bitfield Description :
#define A2S_TAG_ALLOC_1_Reserved_15_8_OFFSET      8
#define A2S_TAG_ALLOC_1_Reserved_15_8_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC7 write.
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_WR_OFFSET      16
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_WR_MASK        0xff0000

// Bitfield Description : The number of tag reserved for VC7 read.
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_RD_OFFSET      24
#define A2S_TAG_ALLOC_1_TAG_ALLOC_FOR_VC7_RD_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC3_WR:8;
    UINT32                            Reserved_15_8:8;
    UINT32                            TAG_ALLOC_FOR_VC7_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC7_RD:8;
  } Field;
  UINT32 Value;
} A2S_TAG_ALLOC_1_STRUCT;

#define SMN_NBIF0_A2S_TAG_ALLOC_1_ADDRESS    0x1403b78UL
#define SMN_NBIF0_A2S_TAG_ALLOC_1_ADDRESS_Bx 0x140F958UL


/***********************************************************
* Register Name : A2S_TAG_ALLOC_2
* Register Description :
* A2S Tag allocation control register 2.
* Visibility : 0x4
************************************************************/

// Bitfield Description : The number of tag reserved for VC5 write.
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_OFFSET      0
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_WR_MASK        0xff

// Bitfield Description : The number of tag reserved for VC5 read.
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_RD_OFFSET      8
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC5_RD_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC6 write.
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_WR_OFFSET      16
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_WR_MASK        0xff0000

// Bitfield Description : The number of tag reserved for VC6 read.
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_RD_OFFSET      24
#define A2S_TAG_ALLOC_2_TAG_ALLOC_FOR_VC6_RD_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC5_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC5_RD:8;
    UINT32                            TAG_ALLOC_FOR_VC6_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC6_RD:8;
  } Field;
  UINT32 Value;
} A2S_TAG_ALLOC_2_STRUCT;

#define SMN_NBIF0_A2S_TAG_ALLOC_2_ADDRESS    0x1403b7cUL
#define SMN_NBIF0_A2S_TAG_ALLOC_2_ADDRESS_Bx 0x140F95CUL


/***********************************************************
* Register Name : A2S_TAG_ALLOC_3
* Register Description :
* A2S Tag allocation control register 3.
* Visibility : 0x4
************************************************************/

// Bitfield Description : The number of tag reserved for VC4 write.
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_WR_OFFSET      0
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_WR_MASK        0xff

// Bitfield Description : The number of tag reserved for VC4 read.
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_RD_OFFSET      8
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC4_RD_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC2 write.
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_WR_OFFSET      16
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_WR_MASK        0xff0000

// Bitfield Description : The number of tag reserved for VC2 read.
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_RD_OFFSET      24
#define A2S_TAG_ALLOC_3_TAG_ALLOC_FOR_VC2_RD_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC4_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC4_RD:8;
    UINT32                            TAG_ALLOC_FOR_VC2_WR:8;
    UINT32                            TAG_ALLOC_FOR_VC2_RD:8;
  } Field;
  UINT32 Value;
} A2S_TAG_ALLOC_3_STRUCT;

#define SMN_NBIF0_A2S_TAG_ALLOC_3_ADDRESS    0x1403b80UL
#define SMN_NBIF0_A2S_TAG_ALLOC_3_ADDRESS_Bx 0x140F960UL


/***********************************************************
* Register Name : NGDC_PGMST_CTRL
* Register Description :
* ngdc pgmst regiser interface set error control registers.
* Visibility : 0x4
************************************************************/

// Bitfield Description : control number of consecutive periods when all PG IPs assert.
#define NGDC_PGMST_CTRL_NGDC_CFG_PG_HYSTERESIS_OFFSET      0
#define NGDC_PGMST_CTRL_NGDC_CFG_PG_HYSTERESIS_MASK        0xff

// Bitfield Description : enable ngdc's power gating.
#define NGDC_PGMST_CTRL_NGDC_CFG_PG_EN_OFFSET      8
#define NGDC_PGMST_CTRL_NGDC_CFG_PG_EN_MASK        0x100

// Bitfield Description :
#define NGDC_PGMST_CTRL_Reserved_9_9_OFFSET      9
#define NGDC_PGMST_CTRL_Reserved_9_9_MASK        0x200

// Bitfield Description : 0: idleness_counter is disabled, PGMST uses only hysteresis timeout value to enter power gating.
#define NGDC_PGMST_CTRL_NGDC_CFG_IDLENESS_COUNT_EN_OFFSET      10
#define NGDC_PGMST_CTRL_NGDC_CFG_IDLENESS_COUNT_EN_MASK        0x3c00

// Bitfield Description : 2'bx1: enable sending the pwrmgr_int for FW interlock feature, 2'b1x: Enable wakup tile from power gated state using the pwrmgt_int_clear signal .
#define NGDC_PGMST_CTRL_NGDC_CFG_FW_PG_EXIT_EN_OFFSET      14
#define NGDC_PGMST_CTRL_NGDC_CFG_FW_PG_EXIT_EN_MASK        0xc000

// Bitfield Description :
#define NGDC_PGMST_CTRL_Reserved_31_16_OFFSET      16
#define NGDC_PGMST_CTRL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            NGDC_CFG_PG_HYSTERESIS:8;
    UINT32                            NGDC_CFG_PG_EN:1;
    UINT32                            Reserved_9_9:1;
    UINT32                            NGDC_CFG_IDLENESS_COUNT_EN:4;
    UINT32                            NGDC_CFG_FW_PG_EXIT_EN:2;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} NGDC_PGMST_CTRL_STRUCT;

#define SMN_NBIF0_NGDC_PGMST_CTRL_ADDRESS    0x1403c64UL


/***********************************************************
* Register Name : SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : syshub shubclk hst sw0 response bypass enable,it can't be both set with imm_en. imm_en&bypass must be 0
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_HST_SW0_bypass_en_OFFSET      0
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_HST_SW0_bypass_en_MASK        0x1

// Bitfield Description :
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_Reserved_15_1_OFFSET      1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_Reserved_15_1_MASK        0xfffe

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW0 in SHUBCLK domain. It could not be set to 1 if SYSHUB_bgen_shubclk_DMA_SW0_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW0_bypass_en_OFFSET      16
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW0_bypass_en_MASK        0x10000

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW1 in SHUBCLK domain. It could not be set to 1 if SYSHUB_bgen_shubclk_DMA_SW1_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW1_bypass_en_OFFSET      17
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW1_bypass_en_MASK        0x20000

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW2 in SHUBCLK domain. It could not be set to 1 if SYSHUB_bgen_shubclk_DMA_SW2_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW2_bypass_en_OFFSET      18
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW2_bypass_en_MASK        0x40000

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW3 in SHUBCLK domain. It could not be set to 1 if SYSHUB_bgen_shubclk_DMA_SW3_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW3_bypass_en_OFFSET      19
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_bgen_shubclk_DMA_SW3_bypass_en_MASK        0x80000

// Bitfield Description :
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_Reserved_30_20_OFFSET      20
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_Reserved_30_20_MASK        0x7ff00000

// Bitfield Description : To change BGEN BYPASS_EN dynamic. 0: can't change BYPASS_EN dynamic. 1: can change BYPASS_EN dynamic.
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_DYNAMIC_BYPASS_EN_shubclk_OFFSET      31
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_SYSHUB_DYNAMIC_BYPASS_EN_shubclk_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SYSHUB_bgen_shubclk_HST_SW0_bypass_en:1;
    UINT32                            Reserved_15_1:15;
    UINT32                            SYSHUB_bgen_shubclk_DMA_SW0_bypass_en:1;
    UINT32                            SYSHUB_bgen_shubclk_DMA_SW1_bypass_en:1;
    UINT32                            SYSHUB_bgen_shubclk_DMA_SW2_bypass_en:1;
    UINT32                            SYSHUB_bgen_shubclk_DMA_SW3_bypass_en:1;
    UINT32                            Reserved_30_20:11;
    UINT32                            SYSHUB_DYNAMIC_BYPASS_EN_shubclk:1;
  } Field;
  UINT32 Value;
} SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SHUBCLK_ADDRESS    0x1411008UL


/***********************************************************
* Register Name : SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : syshub socclk hst sw0 response bypass enable,it can't be both set with imm_en. imm_en&bypass must be 0
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_HST_SW0_bypass_en_OFFSET      0
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_HST_SW0_bypass_en_MASK        0x1

// Bitfield Description : syshub socclk hst sw1 response bypass enable,it can't be both set with imm_en. imm_en&bypass must be 0
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_HST_SW1_bypass_en_OFFSET      1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_HST_SW1_bypass_en_MASK        0x2

// Bitfield Description :
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_Reserved_15_2_OFFSET      2
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_Reserved_15_2_MASK        0xfffc

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW0 in SOCCLK domain. It could not be set to 1 if SYSHUB_bgen_socclk_DMA_SW0_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_DMA_SW0_bypass_en_OFFSET      16
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_DMA_SW0_bypass_en_MASK        0x10000

// Bitfield Description : Enable BRESP enhancement feature for DMA_SW1 in SOCCLK domain. It could not be set to 1 if SYSHUB_bgen_socclk_DMA_SW1_imm_en has been set to 1
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_DMA_SW1_bypass_en_OFFSET      17
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_bgen_socclk_DMA_SW1_bypass_en_MASK        0x20000

// Bitfield Description :
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_Reserved_30_18_OFFSET      18
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_Reserved_30_18_MASK        0x7ffc0000

// Bitfield Description : To change BGEN BYPASS_EN dynamic. 0: can't change BYPASS_EN dynamic. 1: can change BYPASS_EN dynamic.
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_DYNAMIC_BYPASS_EN_socclk_OFFSET      31
#define SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_SYSHUB_DYNAMIC_BYPASS_EN_socclk_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SYSHUB_bgen_socclk_HST_SW0_bypass_en:1;
    UINT32                            SYSHUB_bgen_socclk_HST_SW1_bypass_en:1;
    UINT32                            Reserved_15_2:14;
    UINT32                            SYSHUB_bgen_socclk_DMA_SW0_bypass_en:1;
    UINT32                            SYSHUB_bgen_socclk_DMA_SW1_bypass_en:1;
    UINT32                            Reserved_30_18:13;
    UINT32                            SYSHUB_DYNAMIC_BYPASS_EN_socclk:1;
  } Field;
  UINT32 Value;
} SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_BGEN_ENHANCEMENT_BYPASS_EN_SOCCLK_ADDRESS    0x1410008UL


/***********************************************************
* Register Name : SYSHUB_DS_CTRL_SHUBCLK
* Register Description :
* SHUBCLK DeepSleep control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Mask shubclk_ds_allow input from HST CL0 acpsram.
#define SYSHUB_DS_CTRL_SHUBCLK_HST_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      0
#define SYSHUB_DS_CTRL_SHUBCLK_HST_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x1

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL0 mp0.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      1
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x2

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL1 pspccp.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL1_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      2
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL1_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x4

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL2 mp1.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL2_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      3
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL2_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x8

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL3 mp2.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL3_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      4
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL3_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL4 sata0.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL4_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      5
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL4_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x20

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL5 usb2.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL5_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      6
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL5_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x40

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL6 acp_doorbell.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL6_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      7
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL6_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x80

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL7 mpm.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL7_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      8
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL7_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x100

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL8 mp1dram.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL8_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      9
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL8_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x200

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL9 usb0.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL9_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      10
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL9_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x400

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL10 usb1.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL10_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      11
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL10_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x800

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL11 usb2.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL11_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      12
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL11_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x1000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL12 acp.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL12_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      13
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL12_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x2000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL13 mpmdram.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL13_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      14
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL13_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x4000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL14 usb3.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL14_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      15
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL14_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x8000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL15 usb4.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL15_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      16
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL15_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL16 usb0.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL16_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      17
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL16_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x20000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL17 usb1.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL17_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      18
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL17_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x40000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL18 usb3.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL18_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      19
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL18_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x80000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL19 usb4.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL19_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      20
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL19_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x100000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL20 usbrt0.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL20_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      21
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL20_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x200000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL21 usbrt1.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL21_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      22
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL21_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x400000

// Bitfield Description : Mask shubclk_ds_allow input from DMA CL22 xgbe.
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      23
#define SYSHUB_DS_CTRL_SHUBCLK_DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x800000

// Bitfield Description :
#define SYSHUB_DS_CTRL_SHUBCLK_Reserved_27_24_OFFSET      24
#define SYSHUB_DS_CTRL_SHUBCLK_Reserved_27_24_MASK        0xf000000

// Bitfield Description : Mask shubclk_ds_allow indication from SystemHub inside.
#define SYSHUB_DS_CTRL_SHUBCLK_SYSHUB_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      28
#define SYSHUB_DS_CTRL_SHUBCLK_SYSHUB_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10000000

// Bitfield Description :
#define SYSHUB_DS_CTRL_SHUBCLK_Reserved_30_29_OFFSET      29
#define SYSHUB_DS_CTRL_SHUBCLK_Reserved_30_29_MASK        0x60000000

// Bitfield Description : SHUBCLK DeepSleep Enable.
#define SYSHUB_DS_CTRL_SHUBCLK_SYSHUB_SHUBCLK_DS_EN_OFFSET      31
#define SYSHUB_DS_CTRL_SHUBCLK_SYSHUB_SHUBCLK_DS_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HST_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL0_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL1_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL2_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL3_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL4_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL5_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL6_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL7_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL8_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL9_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL10_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL11_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL12_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL13_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL14_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL15_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL16_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL17_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL18_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL19_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL20_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL21_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_27_24:4;
    UINT32                            SYSHUB_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_30_29:2;
    UINT32                            SYSHUB_SHUBCLK_DS_EN:1;
  } Field;
  UINT32 Value;
} SYSHUB_DS_CTRL_SHUBCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS    0x1411000UL


/***********************************************************
* Register Name : SYSHUB_DS_CTRL_SOCCLK
* Register Description :
* SOCCLK DeepSleep control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Mask socclk_ds_allow input from HST CL0 ih.
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      0
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x1

// Bitfield Description : Mask socclk_ds_allow input from HST CL1 cpf.
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      1
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x2

// Bitfield Description : Mask socclk_ds_allow input from HST CL2 vcn0.
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      2
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x4

// Bitfield Description : Mask socclk_ds_allow input from HST CL3 hdp.
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      3
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x8

// Bitfield Description : Mask socclk_ds_allow input from HST CL4 vga.
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      4
#define SYSHUB_DS_CTRL_SOCCLK_HST_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10

// Bitfield Description : Mask socclk_ds_allow input from DMA CL0 dbgu0.
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      5
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x20

// Bitfield Description : Mask socclk_ds_allow input from DMA CL1 dbgu1.
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      6
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x40

// Bitfield Description : Mask socclk_ds_allow input from DMA CL2 dbgu2.
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      7
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x80

// Bitfield Description : Mask socclk_ds_allow input from DMA CL3 dbgu3.
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      8
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x100

// Bitfield Description : Mask socclk_ds_allow input from DMA CL4 dce.
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      9
#define SYSHUB_DS_CTRL_SOCCLK_DMA_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x200

// Bitfield Description :
#define SYSHUB_DS_CTRL_SOCCLK_Reserved_27_10_OFFSET      10
#define SYSHUB_DS_CTRL_SOCCLK_Reserved_27_10_MASK        0xffffc00

// Bitfield Description : Mask socclk_ds_allow indication from SystemHub inside.
#define SYSHUB_DS_CTRL_SOCCLK_SYSHUB_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      28
#define SYSHUB_DS_CTRL_SOCCLK_SYSHUB_SOCCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10000000

// Bitfield Description :
#define SYSHUB_DS_CTRL_SOCCLK_Reserved_30_29_OFFSET      29
#define SYSHUB_DS_CTRL_SOCCLK_Reserved_30_29_MASK        0x60000000

// Bitfield Description : SOCCLK DeepSleep Enable.
#define SYSHUB_DS_CTRL_SOCCLK_SYSHUB_SOCCLK_DS_EN_OFFSET      31
#define SYSHUB_DS_CTRL_SOCCLK_SYSHUB_SOCCLK_DS_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HST_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            HST_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            HST_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            HST_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            HST_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL0_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL1_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL2_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL3_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            DMA_CL4_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_27_10:18;
    UINT32                            SYSHUB_SOCCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_30_29:2;
    UINT32                            SYSHUB_SOCCLK_DS_EN:1;
  } Field;
  UINT32 Value;
} SYSHUB_DS_CTRL_SOCCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_DS_CTRL_SOCCLK_ADDRESS    0x1410000UL


/***********************************************************
* Register Name : SYSHUB_MGCG_CTRL_SHUBCLK
* Register Description :
* SYSHUB_MGCG CTNTROL REGISTER
* Visibility : 0x4
************************************************************/

// Bitfield Description : GDC MGCG ENABLE
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_EN_SHUBCLK_OFFSET      0
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_EN_SHUBCLK_MASK        0x1

// Bitfield Description : GDC MGCG working mode
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_MODE_SHUBCLK_OFFSET      1
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_MODE_SHUBCLK_MASK        0x2

// Bitfield Description : GDC MGCG HYSTERESIS for gating count
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_HYSTERESIS_SHUBCLK_OFFSET      2
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_HYSTERESIS_SHUBCLK_MASK        0x3fc

// Bitfield Description : SHUBCLK MGCG disable for hst branch
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_HST_DIS_SHUBCLK_OFFSET      10
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_HST_DIS_SHUBCLK_MASK        0x400

// Bitfield Description : SHUBCLK MGCG disable for dma branch
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_DMA_DIS_SHUBCLK_OFFSET      11
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_DMA_DIS_SHUBCLK_MASK        0x800

// Bitfield Description : SHUBCLK MGCG disable for regs branch
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_REG_DIS_SHUBCLK_OFFSET      12
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_REG_DIS_SHUBCLK_MASK        0x1000

// Bitfield Description : SHUBCLK MGCG disable for aer branch
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_AER_DIS_SHUBCLK_OFFSET      13
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_AER_DIS_SHUBCLK_MASK        0x2000

// Bitfield Description :
#define SYSHUB_MGCG_CTRL_SHUBCLK_Reserved_15_14_OFFSET      14
#define SYSHUB_MGCG_CTRL_SHUBCLK_Reserved_15_14_MASK        0xc000

// Bitfield Description : 1: Enable DMA clock branch of SHUBCLK to be waken-up by USB02NBIF_PM_Exit_Req and USB12NBIF_PM_Exit_Req(2 USB functions)/ USB2NBIF_PM_Exit_Req (only 1 USB function); 0: disable.
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_DMA_USB_EARLY_WAKEUP_EN_OFFSET      16
#define SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_DMA_USB_EARLY_WAKEUP_EN_MASK        0x10000

// Bitfield Description :
#define SYSHUB_MGCG_CTRL_SHUBCLK_Reserved_31_17_OFFSET      17
#define SYSHUB_MGCG_CTRL_SHUBCLK_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            SYSHUB_MGCG_EN_SHUBCLK:1;
    UINT32                            SYSHUB_MGCG_MODE_SHUBCLK:1;
    UINT32                            SYSHUB_MGCG_HYSTERESIS_SHUBCLK:8;
    UINT32                            SYSHUB_MGCG_HST_DIS_SHUBCLK:1;
    UINT32                            SYSHUB_MGCG_DMA_DIS_SHUBCLK:1;
    UINT32                            SYSHUB_MGCG_REG_DIS_SHUBCLK:1;
    UINT32                            SYSHUB_MGCG_AER_DIS_SHUBCLK:1;
    UINT32                            Reserved_15_14:2;
    UINT32                            SYSHUB_MGCG_DMA_USB_EARLY_WAKEUP_EN:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} SYSHUB_MGCG_CTRL_SHUBCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_MGCG_CTRL_SHUBCLK_ADDRESS    0x1411020UL


/***********************************************************
* Register Name : SYSHUB_MGCG_CTRL_SOCCLK
* Register Description :
* SYSHUB_MGCG CTNTROL REGISTER
* Visibility : 0x4
************************************************************/

// Bitfield Description : GDC MGCG ENABLE
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_EN_SOCCLK_OFFSET      0
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_EN_SOCCLK_MASK        0x1

// Bitfield Description : GDC MGCG working mode
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_MODE_SOCCLK_OFFSET      1
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_MODE_SOCCLK_MASK        0x2

// Bitfield Description : GDC MGCG HYSTERESIS for gating count
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HYSTERESIS_SOCCLK_OFFSET      2
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HYSTERESIS_SOCCLK_MASK        0x3fc

// Bitfield Description : SOCCLK MGCG disable for hst branch
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HST_DIS_SOCCLK_OFFSET      10
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_HST_DIS_SOCCLK_MASK        0x400

// Bitfield Description : SOCCLK MGCG disable for dma branch
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_DMA_DIS_SOCCLK_OFFSET      11
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_DMA_DIS_SOCCLK_MASK        0x800

// Bitfield Description : SOCCLK MGCG disable for regs branch
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_REG_DIS_SOCCLK_OFFSET      12
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_REG_DIS_SOCCLK_MASK        0x1000

// Bitfield Description : SOCCLK MGCG disable for aer branch
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_AER_DIS_SOCCLK_OFFSET      13
#define SYSHUB_MGCG_CTRL_SOCCLK_SYSHUB_MGCG_AER_DIS_SOCCLK_MASK        0x2000

// Bitfield Description :
#define SYSHUB_MGCG_CTRL_SOCCLK_Reserved_31_14_OFFSET      14
#define SYSHUB_MGCG_CTRL_SOCCLK_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            SYSHUB_MGCG_EN_SOCCLK:1;
    UINT32                            SYSHUB_MGCG_MODE_SOCCLK:1;
    UINT32                            SYSHUB_MGCG_HYSTERESIS_SOCCLK:8;
    UINT32                            SYSHUB_MGCG_HST_DIS_SOCCLK:1;
    UINT32                            SYSHUB_MGCG_DMA_DIS_SOCCLK:1;
    UINT32                            SYSHUB_MGCG_REG_DIS_SOCCLK:1;
    UINT32                            SYSHUB_MGCG_AER_DIS_SOCCLK:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} SYSHUB_MGCG_CTRL_SOCCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_MGCG_CTRL_SOCCLK_ADDRESS    0x1410020UL

/***********************************************************
* Register Name : DMA_CLK1_SW1_CL4_CNTL
* Register Description :
* client control register for acp, CL4 of DMA SW1 in CLK1 domain.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable FLR to reset corresponding AXI RegisterSlice, by means of RS_RESET.
#define DMA_CLK1_SW1_CL4_CNTL_FLR_ON_RS_RESET_EN_OFFSET      0
#define DMA_CLK1_SW1_CL4_CNTL_FLR_ON_RS_RESET_EN_MASK        0x1

// Bitfield Description : Enable Link Reset to reset AXI RegisterSlice, by means of RS_RESET.
#define DMA_CLK1_SW1_CL4_CNTL_LKRST_ON_RS_RESET_EN_OFFSET      1
#define DMA_CLK1_SW1_CL4_CNTL_LKRST_ON_RS_RESET_EN_MASK        0x2

// Bitfield Description :
#define DMA_CLK1_SW1_CL4_CNTL_Reserved_7_2_OFFSET      2
#define DMA_CLK1_SW1_CL4_CNTL_Reserved_7_2_MASK        0xfc

// Bitfield Description : Enable static AxQOS override.
#define DMA_CLK1_SW1_CL4_CNTL_QOS_STATIC_OVERRIDE_EN_OFFSET      8
#define DMA_CLK1_SW1_CL4_CNTL_QOS_STATIC_OVERRIDE_EN_MASK        0x100

// Bitfield Description : AxQOS value for static override.
#define DMA_CLK1_SW1_CL4_CNTL_QOS_STATIC_OVERRIDE_VALUE_OFFSET      9
#define DMA_CLK1_SW1_CL4_CNTL_QOS_STATIC_OVERRIDE_VALUE_MASK        0x1e00

// Bitfield Description :
#define DMA_CLK1_SW1_CL4_CNTL_Reserved_15_13_OFFSET      13
#define DMA_CLK1_SW1_CL4_CNTL_Reserved_15_13_MASK        0xe000

// Bitfield Description : weight setting of read request
#define DMA_CLK1_SW1_CL4_CNTL_READ_WRR_WEIGHT_OFFSET      16
#define DMA_CLK1_SW1_CL4_CNTL_READ_WRR_WEIGHT_MASK        0xff0000

// Bitfield Description : weight setting of write request
#define DMA_CLK1_SW1_CL4_CNTL_WRITE_WRR_WEIGHT_OFFSET      24
#define DMA_CLK1_SW1_CL4_CNTL_WRITE_WRR_WEIGHT_MASK        0xff000000

typedef union {
  struct {
    UINT32                            FLR_ON_RS_RESET_EN:1;
    UINT32                            LKRST_ON_RS_RESET_EN:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            QOS_STATIC_OVERRIDE_EN:1;
    UINT32                            QOS_STATIC_OVERRIDE_VALUE:4;
    UINT32                            Reserved_15_13:3;
    UINT32                            READ_WRR_WEIGHT:8;
    UINT32                            WRITE_WRR_WEIGHT:8;
  } Field;
  UINT32 Value;
} DMA_CLK1_SW1_CL4_CNTL_STRUCT;

#define SMN_NBIF0_DMA_CLK1_SW1_CL4_CNTL_ADDRESS    0x1411990UL

//UVD_UVDPG.h
/***********************************************************
* Register Name : UVD_DCLK_DS_CNTL
* Register Description :
* DCLK Deep-Sleep side-band signal to CLKIP control.
* Visibility : 0x8
************************************************************/

// Bitfield Description : Enable deep-sleep allow for DCLK.
#define UVD_DCLK_DS_CNTL_DCLK_DS_EN_OFFSET      0
#define UVD_DCLK_DS_CNTL_DCLK_DS_EN_MASK        0x1

// Bitfield Description :
#define UVD_DCLK_DS_CNTL_Reserved_3_1_OFFSET      1
#define UVD_DCLK_DS_CNTL_Reserved_3_1_MASK        0xe

// Bitfield Description : DCLK deep-sleep allow status.
#define UVD_DCLK_DS_CNTL_DCLK_DS_STATUS_OFFSET      4
#define UVD_DCLK_DS_CNTL_DCLK_DS_STATUS_MASK        0x10

// Bitfield Description :
#define UVD_DCLK_DS_CNTL_Reserved_15_5_OFFSET      5
#define UVD_DCLK_DS_CNTL_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Number of dclk cycles x32 before allowing deep-sleep.
#define UVD_DCLK_DS_CNTL_DCLK_DS_HYSTERESIS_CNT_OFFSET      16
#define UVD_DCLK_DS_CNTL_DCLK_DS_HYSTERESIS_CNT_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            DCLK_DS_EN:1;
    UINT32                            Reserved_3_1:3;
    UINT32                            DCLK_DS_STATUS:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            DCLK_DS_HYSTERESIS_CNT:16;
  } Field;
  UINT32 Value;
} UVD_DCLK_DS_CNTL_STRUCT;

#define SMN_UVD_DCLK_DS_CNTL_ADDRESS    0x1f958UL


/***********************************************************
* Register Name : UVD_VCLK_DS_CNTL
* Register Description :
* VCLK Deep-Sleep side-band signal to CLKIP control.
* Visibility : 0x8
************************************************************/

// Bitfield Description : Enable deep-sleep allow for VCLK.
#define UVD_VCLK_DS_CNTL_VCLK_DS_EN_OFFSET      0
#define UVD_VCLK_DS_CNTL_VCLK_DS_EN_MASK        0x1

// Bitfield Description :
#define UVD_VCLK_DS_CNTL_Reserved_3_1_OFFSET      1
#define UVD_VCLK_DS_CNTL_Reserved_3_1_MASK        0xe

// Bitfield Description : VCLK deep-sleep allow status.
#define UVD_VCLK_DS_CNTL_VCLK_DS_STATUS_OFFSET      4
#define UVD_VCLK_DS_CNTL_VCLK_DS_STATUS_MASK        0x10

// Bitfield Description :
#define UVD_VCLK_DS_CNTL_Reserved_15_5_OFFSET      5
#define UVD_VCLK_DS_CNTL_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Number of vclk cycles x32 before allowing deep-sleep.
#define UVD_VCLK_DS_CNTL_VCLK_DS_HYSTERESIS_CNT_OFFSET      16
#define UVD_VCLK_DS_CNTL_VCLK_DS_HYSTERESIS_CNT_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VCLK_DS_EN:1;
    UINT32                            Reserved_3_1:3;
    UINT32                            VCLK_DS_STATUS:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            VCLK_DS_HYSTERESIS_CNT:16;
  } Field;
  UINT32 Value;
} UVD_VCLK_DS_CNTL_STRUCT;

#define SMN_UVD_VCLK_DS_CNTL_ADDRESS    0x1f954UL


/***********************************************************
* Register Name : EP_PCIE_ERR_CNTL
* Register Description :
* Error Control Registers.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Disable PCI Express Advanced Error Reporting.
#define EP_PCIE_ERR_CNTL_ERR_REPORTING_DIS_OFFSET      0
#define EP_PCIE_ERR_CNTL_ERR_REPORTING_DIS_MASK        0x1

// Bitfield Description :
#define EP_PCIE_ERR_CNTL_Reserved_7_1_OFFSET      1
#define EP_PCIE_ERR_CNTL_Reserved_7_1_MASK        0xfe

// Bitfield Description : AER first error log timeout.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_TIMEOUT_OFFSET      8
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_TIMEOUT_MASK        0x700

// Bitfield Description :
#define EP_PCIE_ERR_CNTL_Reserved_16_11_OFFSET      11
#define EP_PCIE_ERR_CNTL_Reserved_16_11_MASK        0x1f800

// Bitfield Description : To enable to send fatal/non-fatal error messages immediately instead of delaying until first error logged.
#define EP_PCIE_ERR_CNTL_SEND_ERR_MSG_IMMEDIATELY_OFFSET      17
#define EP_PCIE_ERR_CNTL_SEND_ERR_MSG_IMMEDIATELY_MASK        0x20000

// Bitfield Description : Provide an override for STRAP_POISONED_ADVISORY_NONFATAL for RC.
#define EP_PCIE_ERR_CNTL_STRAP_POISONED_ADVISORY_NONFATAL_OFFSET      18
#define EP_PCIE_ERR_CNTL_STRAP_POISONED_ADVISORY_NONFATAL_MASK        0x40000

// Bitfield Description :
#define EP_PCIE_ERR_CNTL_Reserved_23_19_OFFSET      19
#define EP_PCIE_ERR_CNTL_Reserved_23_19_MASK        0xf80000

// Bitfield Description : AER function 0 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F0_TIMER_EXPIRED_OFFSET      24
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F0_TIMER_EXPIRED_MASK        0x1000000

// Bitfield Description : AER function 1 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F1_TIMER_EXPIRED_OFFSET      25
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F1_TIMER_EXPIRED_MASK        0x2000000

// Bitfield Description : AER function 2 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F2_TIMER_EXPIRED_OFFSET      26
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F2_TIMER_EXPIRED_MASK        0x4000000

// Bitfield Description : AER function 3 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F3_TIMER_EXPIRED_OFFSET      27
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F3_TIMER_EXPIRED_MASK        0x8000000

// Bitfield Description : AER function 4 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F4_TIMER_EXPIRED_OFFSET      28
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F4_TIMER_EXPIRED_MASK        0x10000000

// Bitfield Description : AER function 5 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F5_TIMER_EXPIRED_OFFSET      29
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F5_TIMER_EXPIRED_MASK        0x20000000

// Bitfield Description : AER function 6 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F6_TIMER_EXPIRED_OFFSET      30
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F6_TIMER_EXPIRED_MASK        0x40000000

// Bitfield Description : AER function 7 first error log timer expired.
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F7_TIMER_EXPIRED_OFFSET      31
#define EP_PCIE_ERR_CNTL_AER_HDR_LOG_F7_TIMER_EXPIRED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ERR_REPORTING_DIS:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            AER_HDR_LOG_TIMEOUT:3;
    UINT32                            Reserved_16_11:6;
    UINT32                            SEND_ERR_MSG_IMMEDIATELY:1;
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL:1;
    UINT32                            Reserved_23_19:5;
    UINT32                            AER_HDR_LOG_F0_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F1_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F2_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F3_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F4_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F5_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F6_TIMER_EXPIRED:1;
    UINT32                            AER_HDR_LOG_F7_TIMER_EXPIRED:1;
  } Field;
  UINT32 Value;
} EP_PCIE_ERR_CNTL_STRUCT;

/***********************************************************
* Register Name : PCIE_ERR_CNTL
* Register Description :
* Error Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls PCIe Advanced Error Reporting.
#define PCIE_ERR_CNTL_ERR_REPORTING_DIS_OFFSET      0
#define PCIE_ERR_CNTL_ERR_REPORTING_DIS_MASK        0x1

// Bitfield Description : Controls the error logging method. Provides an override for STRAP_FIRST_RCVD_ERR_LOG.
#define PCIE_ERR_CNTL_STRAP_FIRST_RCVD_ERR_LOG_OFFSET      1
#define PCIE_ERR_CNTL_STRAP_FIRST_RCVD_ERR_LOG_MASK        0x2

// Bitfield Description : Controls if received ECRC errors are dropped.
#define PCIE_ERR_CNTL_RX_DROP_ECRC_FAILURES_OFFSET      2
#define PCIE_ERR_CNTL_RX_DROP_ECRC_FAILURES_MASK        0x4

// Bitfield Description :
#define PCIE_ERR_CNTL_Reserved_4_3_OFFSET      3
#define PCIE_ERR_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Controls if LCRC error is generated for the next received TLP.
#define PCIE_ERR_CNTL_RX_GENERATE_LCRC_ERR_OFFSET      5
#define PCIE_ERR_CNTL_RX_GENERATE_LCRC_ERR_MASK        0x20

// Bitfield Description : A write of 1 to this register causes the next received TLP with Data to be poisoned.
#define PCIE_ERR_CNTL_RX_GENERATE_POIS_TLP_OFFSET      6
#define PCIE_ERR_CNTL_RX_GENERATE_POIS_TLP_MASK        0x40

// Bitfield Description : Controls if ECRC error is generated for the next received TLP.
#define PCIE_ERR_CNTL_RX_GENERATE_ECRC_ERR_OFFSET      7
#define PCIE_ERR_CNTL_RX_GENERATE_ECRC_ERR_MASK        0x80

// Bitfield Description : Controls the AER first error log timeout.
#define PCIE_ERR_CNTL_AER_HDR_LOG_TIMEOUT_OFFSET      8
#define PCIE_ERR_CNTL_AER_HDR_LOG_TIMEOUT_MASK        0x700

// Bitfield Description : Indicates if AER function 0 first error log timer has expired.
#define PCIE_ERR_CNTL_AER_HDR_LOG_F0_TIMER_EXPIRED_OFFSET      11
#define PCIE_ERR_CNTL_AER_HDR_LOG_F0_TIMER_EXPIRED_MASK        0x800

// Bitfield Description :
#define PCIE_ERR_CNTL_Reserved_13_12_OFFSET      12
#define PCIE_ERR_CNTL_Reserved_13_12_MASK        0x3000

// Bitfield Description : Indicates posted slave buffer read-out halt status.
#define PCIE_ERR_CNTL_CI_P_SLV_BUF_RD_HALT_STATUS_OFFSET      14
#define PCIE_ERR_CNTL_CI_P_SLV_BUF_RD_HALT_STATUS_MASK        0x4000

// Bitfield Description : Indicates non-posted slave buffer read-out halt status.
#define PCIE_ERR_CNTL_CI_NP_SLV_BUF_RD_HALT_STATUS_OFFSET      15
#define PCIE_ERR_CNTL_CI_NP_SLV_BUF_RD_HALT_STATUS_MASK        0x8000

// Bitfield Description : Triggers a reset of the read-out halt state of both posted and non-posted slave buffers.
#define PCIE_ERR_CNTL_CI_SLV_BUF_HALT_RESET_OFFSET      16
#define PCIE_ERR_CNTL_CI_SLV_BUF_HALT_RESET_MASK        0x10000

// Bitfield Description : Controls if allowed to send fatal/non-fatal error messages immediately instead of delaying until first error is logged. This bit is sticky.
#define PCIE_ERR_CNTL_SEND_ERR_MSG_IMMEDIATELY_OFFSET      17
#define PCIE_ERR_CNTL_SEND_ERR_MSG_IMMEDIATELY_MASK        0x20000

// Bitfield Description : Controls the error logging method. Provides an override for STRAP_POISONED_ADVISORY_NONFATAL.
#define PCIE_ERR_CNTL_STRAP_POISONED_ADVISORY_NONFATAL_OFFSET      18
#define PCIE_ERR_CNTL_STRAP_POISONED_ADVISORY_NONFATAL_MASK        0x40000

// Bitfield Description : Controls completely masking BAD_DLLP error (error status bit will not be set). This bit is sticky.
#define PCIE_ERR_CNTL_AER_PRIV_MASK_BAD_DLLP_OFFSET      19
#define PCIE_ERR_CNTL_AER_PRIV_MASK_BAD_DLLP_MASK        0x80000

// Bitfield Description : Controls completely masking BAD_TLP error (error status bit will not be set). This bit is sticky.
#define PCIE_ERR_CNTL_AER_PRIV_MASK_BAD_TLP_OFFSET      20
#define PCIE_ERR_CNTL_AER_PRIV_MASK_BAD_TLP_MASK        0x100000

// Bitfield Description :
#define PCIE_ERR_CNTL_Reserved_31_21_OFFSET      21
#define PCIE_ERR_CNTL_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            ERR_REPORTING_DIS:1;
    UINT32                            STRAP_FIRST_RCVD_ERR_LOG:1;
    UINT32                            RX_DROP_ECRC_FAILURES:1;
    UINT32                            Reserved_4_3:2;
    UINT32                            RX_GENERATE_LCRC_ERR:1;
    UINT32                            RX_GENERATE_POIS_TLP:1;
    UINT32                            RX_GENERATE_ECRC_ERR:1;
    UINT32                            AER_HDR_LOG_TIMEOUT:3;
    UINT32                            AER_HDR_LOG_F0_TIMER_EXPIRED:1;
    UINT32                            Reserved_13_12:2;
    UINT32                            CI_P_SLV_BUF_RD_HALT_STATUS:1;
    UINT32                            CI_NP_SLV_BUF_RD_HALT_STATUS:1;
    UINT32                            CI_SLV_BUF_HALT_RESET:1;
    UINT32                            SEND_ERR_MSG_IMMEDIATELY:1;
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL:1;
    UINT32                            AER_PRIV_MASK_BAD_DLLP:1;
    UINT32                            AER_PRIV_MASK_BAD_TLP:1;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} PCIE_ERR_CNTL_PCIEPORT_STRUCT;


#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_ERR_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_ERR_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x1a8)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_ERR_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_ERR_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x1a8)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_ERR_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_ERR_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x1a8)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_ERR_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_ERR_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x1a8)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_ERR_CNTL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_ERR_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x1a8)

#define SMN_FUNC0_PCIE0_PCIE_ERR_CNTL_ADDRESS    0x111401a8UL

#define SMN_FUNC0_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112401a8UL

#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_ERR_CNTL_DEVICE       1
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_ERR_CNTL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 1, 1, 0x1a8)

#define SMN_FUNC1_PCIE0_PCIE_ERR_CNTL_ADDRESS    0x111411a8UL

#define SMN_FUNC1_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112411a8UL

#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_ERR_CNTL_DEVICE       1
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_ERR_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 1, 2, 0x1a8)

#define SMN_FUNC2_PCIE0_PCIE_ERR_CNTL_ADDRESS    0x111421a8UL

#define SMN_FUNC2_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112421a8UL

#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_ERR_CNTL_DEVICE       1
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_ERR_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 1, 3, 0x1a8)

#define SMN_FUNC3_PCIE0_PCIE_ERR_CNTL_ADDRESS    0x111431a8UL

#define SMN_FUNC3_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112431a8UL

#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_ERR_CNTL_DEVICE       1
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_ERR_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_ERR_CNTL_OFFSET       0x1a8
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_ERR_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 1, 4, 0x1a8)

#define SMN_FUNC4_PCIE0_PCIE_ERR_CNTL_ADDRESS    0x111441a8UL

#define SMN_FUNC4_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112441a8UL

#define SMN_FUNC5_PCIE1_PCIE_ERR_CNTL_ADDRESS    0x112451a8UL


// PCICFG
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)

#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET       0x380
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x380)


#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC5_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC6_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC7_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_CNTL_OFFSET       0x386
#define PCICFG_NBIO_FUNC8_PCIE0_PCIE_DPC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x386)

/***********************************************************
* Register Name : RSMU_MASTER_CONTROL
* Register Description :
* RSMU-AXI Master Interface Control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls allowing the AXI master interface to send messages.
#define RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_OFFSET      0
#define RSMU_MASTER_CONTROL_RSMU_MASTER_MESSAGE_SEND_ENABLE_MASK        0x1

// Bitfield Description : Controls putting the AXI master interface into debug mode, where it sends consecutive messages equal to the number of clients.
#define RSMU_MASTER_CONTROL_RSMU_MASTER_DEBUG_EN_OFFSET      1
#define RSMU_MASTER_CONTROL_RSMU_MASTER_DEBUG_EN_MASK        0x2

// Bitfield Description :
#define RSMU_MASTER_CONTROL_Reserved_31_2_OFFSET      2
#define RSMU_MASTER_CONTROL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            RSMU_MASTER_MESSAGE_SEND_ENABLE:1;
    UINT32                            RSMU_MASTER_DEBUG_EN:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} RSMU_MASTER_CONTROL_STRUCT;

#define SMN_PCIE0_RSMU_MASTER_CONTROL_ADDRESS    0x1118047cUL

#define SMN_PCIE1_RSMU_MASTER_CONTROL_ADDRESS    0x1128047cUL

#define SMN_PCIE2_RSMU_MASTER_CONTROL_ADDRESS    0x1138047cUL

#define SMN_PCIE3_RSMU_MASTER_CONTROL_ADDRESS    0x1148047cUL

/***********************************************************
* Register Name : SYSHUB_DS_CTRL_HSPCLK
* Register Description :
* HSPCLK DeepSleep control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Mask hspclk_ds_allow input from DMA CL0 hsp.
#define SYSHUB_DS_CTRL_HSPCLK_DMA_CL0_HSPCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      0
#define SYSHUB_DS_CTRL_HSPCLK_DMA_CL0_HSPCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x1

// Bitfield Description :
#define SYSHUB_DS_CTRL_HSPCLK_Reserved_27_1_OFFSET      1
#define SYSHUB_DS_CTRL_HSPCLK_Reserved_27_1_MASK        0xffffffe

// Bitfield Description : Mask hspclk_ds_allow indication from SystemHub inside.
#define SYSHUB_DS_CTRL_HSPCLK_SYSHUB_HSPCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      28
#define SYSHUB_DS_CTRL_HSPCLK_SYSHUB_HSPCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10000000

// Bitfield Description :
#define SYSHUB_DS_CTRL_HSPCLK_Reserved_30_29_OFFSET      29
#define SYSHUB_DS_CTRL_HSPCLK_Reserved_30_29_MASK        0x60000000

// Bitfield Description : HSPCLK DeepSleep Enable.
#define SYSHUB_DS_CTRL_HSPCLK_SYSHUB_HSPCLK_DS_EN_OFFSET      31
#define SYSHUB_DS_CTRL_HSPCLK_SYSHUB_HSPCLK_DS_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            DMA_CL0_HSPCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_27_1:27;
    UINT32                            SYSHUB_HSPCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_30_29:2;
    UINT32                            SYSHUB_HSPCLK_DS_EN:1;
  } Field;
  UINT32 Value;
} SYSHUB_DS_CTRL_HSPCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_DS_CTRL_HSPCLK_ADDRESS    0x1413000UL

/***********************************************************
* Register Name : CLK0_CLK11_ALLOW_DS
* Register Description :
* Deep Sleep Allow Control
* Visibility : 0x4
************************************************************/

// Bitfield Description : 0x1 ALLOW CLK slice11 to respond to Deep Sleep request
// 0x0 Ignore Deep Sleep requests
#define CLK0_CLK11_ALLOW_DS_CLK11_ALLOW_DS_OFFSET      0
#define CLK0_CLK11_ALLOW_DS_CLK11_ALLOW_DS_MASK        0x1

// Bitfield Description :
#define CLK0_CLK11_ALLOW_DS_Reserved_7_1_OFFSET      1
#define CLK0_CLK11_ALLOW_DS_Reserved_7_1_MASK        0xfe

// Bitfield Description : Override CLK slice11 DS signal to 1'b1
#define CLK0_CLK11_ALLOW_DS_CLK11_DS_override_OFFSET      8
#define CLK0_CLK11_ALLOW_DS_CLK11_DS_override_MASK        0x700

// Bitfield Description :
#define CLK0_CLK11_ALLOW_DS_Reserved_31_11_OFFSET      11
#define CLK0_CLK11_ALLOW_DS_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            CLK11_ALLOW_DS:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            CLK11_DS_override:3;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} CLK0_CLK11_ALLOW_DS_STRUCT;

#define SMN_N0_CLK0_CLK11_ALLOW_DS_ADDRESS    0x5b388UL

/***********************************************************
* Register Name : SYSHUB_DS_CTRL_NICCLK
* Register Description :
* NICCLK DeepSleep control register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Mask nicclk_ds_allow input from HST CL0 cviphst.
#define SYSHUB_DS_CTRL_NICCLK_HST_CL0_NICCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      0
#define SYSHUB_DS_CTRL_NICCLK_HST_CL0_NICCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x1

// Bitfield Description :
#define SYSHUB_DS_CTRL_NICCLK_Reserved_27_1_OFFSET      1
#define SYSHUB_DS_CTRL_NICCLK_Reserved_27_1_MASK        0xffffffe

// Bitfield Description : Mask nicclk_ds_allow indication from SystemHub inside.
#define SYSHUB_DS_CTRL_NICCLK_SYSHUB_NICCLK_DEEPSLEEP_ALLOW_ENABLE_OFFSET      28
#define SYSHUB_DS_CTRL_NICCLK_SYSHUB_NICCLK_DEEPSLEEP_ALLOW_ENABLE_MASK        0x10000000

// Bitfield Description :
#define SYSHUB_DS_CTRL_NICCLK_Reserved_30_29_OFFSET      29
#define SYSHUB_DS_CTRL_NICCLK_Reserved_30_29_MASK        0x60000000

// Bitfield Description : NICCLK DeepSleep Enable.
#define SYSHUB_DS_CTRL_NICCLK_SYSHUB_NICCLK_DS_EN_OFFSET      31
#define SYSHUB_DS_CTRL_NICCLK_SYSHUB_NICCLK_DS_EN_MASK        0x80000000

typedef union {
  struct {
    UINT32                            HST_CL0_NICCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_27_1:27;
    UINT32                            SYSHUB_NICCLK_DEEPSLEEP_ALLOW_ENABLE:1;
    UINT32                            Reserved_30_29:2;
    UINT32                            SYSHUB_NICCLK_DS_EN:1;
  } Field;
  UINT32 Value;
} SYSHUB_DS_CTRL_NICCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_DS_CTRL_NICCLK_ADDRESS    0x1412000UL

/***********************************************************
* Register Name : OBFF_EMU_CFG
* Register Description :
* .
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable OBFF emulation for interrupt.
#define OBFF_EMU_CFG_OBFF_EMU_INTR_EN_OFFSET      0
#define OBFF_EMU_CFG_OBFF_EMU_INTR_EN_MASK        0x1

// Bitfield Description :
#define OBFF_EMU_CFG_Reserved_31_1_OFFSET      1
#define OBFF_EMU_CFG_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            OBFF_EMU_INTR_EN:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} OBFF_EMU_CFG_STRUCT;

#define SMN_NBIF0_OBFF_EMU_CFG_ADDRESS    0x1013a1d0UL


/***********************************************************
* Register Name : OBFF_EMU_CFG_SOCCLK
* Register Description :
* DMA request OBFF emulation configure register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable OBFF emulaion for DMA trans
#define OBFF_EMU_CFG_SOCCLK_OBFF_EMU_DMA_EN_OFFSET      0
#define OBFF_EMU_CFG_SOCCLK_OBFF_EMU_DMA_EN_MASK        0x1

// Bitfield Description : ATHUB urgent mode
#define OBFF_EMU_CFG_SOCCLK_ATHUB_URGENT_MODE_OFFSET      1
#define OBFF_EMU_CFG_SOCCLK_ATHUB_URGENT_MODE_MASK        0x6

// Bitfield Description :
#define OBFF_EMU_CFG_SOCCLK_Reserved_31_3_OFFSET      3
#define OBFF_EMU_CFG_SOCCLK_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            OBFF_EMU_DMA_EN:1;
    UINT32                            ATHUB_URGENT_MODE:2;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} OBFF_EMU_CFG_SOCCLK_STRUCT;

#define SMN_NBIF0_OBFF_EMU_CFG_SOCCLK_ADDRESS    0x1410040UL

//
// NGDC_MGCG_CTRL Register Bitfields:
//

/// Bitfield Description : MGCG Enable for GDC CLOCK.
#define NGDC_MGCG_CTRL_NGDC_MGCG_EN_OFFSET                     0
#define NGDC_MGCG_CTRL_NGDC_MGCG_EN_WIDTH                      1
#define NGDC_MGCG_CTRL_NGDC_MGCG_EN_MASK                       0x1

/// Bitfield Description : NGDC MGCG working mode.
#define NGDC_MGCG_CTRL_NGDC_MGCG_MODE_OFFSET                   1
#define NGDC_MGCG_CTRL_NGDC_MGCG_MODE_WIDTH                    1
#define NGDC_MGCG_CTRL_NGDC_MGCG_MODE_MASK                     0x2

/// Bitfield Description : NGDC MGCG HYSTERESIS counter.
#define NGDC_MGCG_CTRL_NGDC_MGCG_HYSTERESIS_OFFSET             2
#define NGDC_MGCG_CTRL_NGDC_MGCG_HYSTERESIS_WIDTH              8
#define NGDC_MGCG_CTRL_NGDC_MGCG_HYSTERESIS_MASK               0x3fc

/// Bitfield Description : MGCG disable for HST clock branch of GDC CLOCK.
#define NGDC_MGCG_CTRL_NGDC_MGCG_HST_DIS_OFFSET                10
#define NGDC_MGCG_CTRL_NGDC_MGCG_HST_DIS_WIDTH                 1
#define NGDC_MGCG_CTRL_NGDC_MGCG_HST_DIS_MASK                  0x400

/// Bitfield Description : MGCG disable for DMA clock branch of GDC CLOCK.
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_DIS_OFFSET                11
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_DIS_WIDTH                 1
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_DIS_MASK                  0x800

/// Bitfield Description : MGCG disable for REG clock branch of GDC CLOCK.
#define NGDC_MGCG_CTRL_NGDC_MGCG_REG_DIS_OFFSET                12
#define NGDC_MGCG_CTRL_NGDC_MGCG_REG_DIS_WIDTH                 1
#define NGDC_MGCG_CTRL_NGDC_MGCG_REG_DIS_MASK                  0x1000

/// Bitfield Description : MGCG disable for AER clock branch of GDC CLOCK.
#define NGDC_MGCG_CTRL_NGDC_MGCG_AER_DIS_OFFSET                13
#define NGDC_MGCG_CTRL_NGDC_MGCG_AER_DIS_WIDTH                 1
#define NGDC_MGCG_CTRL_NGDC_MGCG_AER_DIS_MASK                  0x2000

/// Bitfield Description :
#define NGDC_MGCG_CTRL_Reserved_15_14_OFFSET                   14
#define NGDC_MGCG_CTRL_Reserved_15_14_WIDTH                    2
#define NGDC_MGCG_CTRL_Reserved_15_14_MASK                     0xc000

/// Bitfield Description : 1: Enable DMA clock branch of GDC clock to be waken-up by USB02NBIF_PM_Exit_Req and USB12NBIF_PM_Exit_Req(2 USB functions)/ USB2NBIF_PM_Exit_Req (only 1 USB function); 0: disable.
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_USB_EARLY_WAKEUP_EN_OFFSET 16
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_USB_EARLY_WAKEUP_EN_WIDTH  1
#define NGDC_MGCG_CTRL_NGDC_MGCG_DMA_USB_EARLY_WAKEUP_EN_MASK   0x10000

/// Bitfield Description :
#define NGDC_MGCG_CTRL_Reserved_31_17_OFFSET                   17
#define NGDC_MGCG_CTRL_Reserved_31_17_WIDTH                    15
#define NGDC_MGCG_CTRL_Reserved_31_17_MASK                     0xfffe0000L

typedef union {
  struct {
    UINT32                                            NGDC_MGCG_EN:1 ; ///<
    UINT32                                          NGDC_MGCG_MODE:1 ; ///<
    UINT32                                    NGDC_MGCG_HYSTERESIS:8 ; ///<
    UINT32                                       NGDC_MGCG_HST_DIS:1 ; ///<
    UINT32                                       NGDC_MGCG_DMA_DIS:1 ; ///<
    UINT32                                       NGDC_MGCG_REG_DIS:1 ; ///<
    UINT32                                       NGDC_MGCG_AER_DIS:1 ; ///<
    UINT32                                          Reserved_15_14:2 ; ///<
    UINT32                       NGDC_MGCG_DMA_USB_EARLY_WAKEUP_EN:1 ; ///<
    UINT32                                          Reserved_31_17:15; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} NGDC_MGCG_CTRL_STRUCT;


//
// Register Name NGDC_MGCG_CTRL
//
#define SMN_NGDC_MGCG_CTRL_ADDRESS                             0x1403ba8UL
#define SMN_NGDC_MGCG_CTRL_OFFSET                              0x3ba8UL

/***********************************************************
* Register Name : SYSHUB_MGCG_CTRL_HSPCLK
* Register Description :
* SYSHUB_MGCG CTNTROL REGISTER
* Visibility : 0x4
************************************************************/

// Bitfield Description : GDC MGCG ENABLE
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_EN_HSPCLK_OFFSET      0
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_EN_HSPCLK_MASK        0x1

// Bitfield Description : GDC MGCG working mode
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_MODE_HSPCLK_OFFSET      1
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_MODE_HSPCLK_MASK        0x2

// Bitfield Description : GDC MGCG HYSTERESIS for gating count
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HYSTERESIS_HSPCLK_OFFSET      2
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HYSTERESIS_HSPCLK_MASK        0x3fc

// Bitfield Description : HSPCLK MGCG disable for hst branch
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HST_DIS_HSPCLK_OFFSET      10
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_HST_DIS_HSPCLK_MASK        0x400

// Bitfield Description : HSPCLK MGCG disable for dma branch
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_DMA_DIS_HSPCLK_OFFSET      11
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_DMA_DIS_HSPCLK_MASK        0x800

// Bitfield Description : HSPCLK MGCG disable for regs branch
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_REG_DIS_HSPCLK_OFFSET      12
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_REG_DIS_HSPCLK_MASK        0x1000

// Bitfield Description : HSPCLK MGCG disable for aer branch
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_AER_DIS_HSPCLK_OFFSET      13
#define SYSHUB_MGCG_CTRL_HSPCLK_SYSHUB_MGCG_AER_DIS_HSPCLK_MASK        0x2000

// Bitfield Description :
#define SYSHUB_MGCG_CTRL_HSPCLK_Reserved_31_14_OFFSET      14
#define SYSHUB_MGCG_CTRL_HSPCLK_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            SYSHUB_MGCG_EN_HSPCLK:1;
    UINT32                            SYSHUB_MGCG_MODE_HSPCLK:1;
    UINT32                            SYSHUB_MGCG_HYSTERESIS_HSPCLK:8;
    UINT32                            SYSHUB_MGCG_HST_DIS_HSPCLK:1;
    UINT32                            SYSHUB_MGCG_DMA_DIS_HSPCLK:1;
    UINT32                            SYSHUB_MGCG_REG_DIS_HSPCLK:1;
    UINT32                            SYSHUB_MGCG_AER_DIS_HSPCLK:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} SYSHUB_MGCG_CTRL_HSPCLK_STRUCT;

#define SMN_SYSHUB_MGCG_CTRL_HSPCLK_ADDRESS    0x1413020UL

/***********************************************************
* Register Name : SYSHUB_SELECT_SHUBCLK
* Register Description :
* select regiser in SHUBCLK domain.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Select source client of USB2.
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB2_OFFSET      0
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB2_MASK        0x1

// Bitfield Description : Select source client of USB0.
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB0_OFFSET      1
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB0_MASK        0x2

// Bitfield Description : Select source client of USB1.
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB1_OFFSET      2
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB1_MASK        0x4

// Bitfield Description : Select source client of USB3.
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB3_OFFSET      3
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB3_MASK        0x8

// Bitfield Description : Select source client of USB4.
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB4_OFFSET      4
#define SYSHUB_SELECT_SHUBCLK_SELECT_USB4_MASK        0x10

// Bitfield Description :
#define SYSHUB_SELECT_SHUBCLK_Reserved_31_5_OFFSET      5
#define SYSHUB_SELECT_SHUBCLK_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            SELECT_USB2:1;
    UINT32                            SELECT_USB0:1;
    UINT32                            SELECT_USB1:1;
    UINT32                            SELECT_USB3:1;
    UINT32                            SELECT_USB4:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} SYSHUB_SELECT_SHUBCLK_STRUCT;

#define SMN_NBIF0_SYSHUB_SELECT_SHUBCLK_ADDRESS    0x1411034UL

/***********************************************************
* Register Name : PCIE_LC_STATE0
* Register Description :
* Link Controller State 0 register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Indicates current LC State.
#define PCIE_LC_STATE0_LC_CURRENT_STATE_OFFSET      0
#define PCIE_LC_STATE0_LC_CURRENT_STATE_MASK        0x3f

// Bitfield Description :
#define PCIE_LC_STATE0_Reserved_7_6_OFFSET      6
#define PCIE_LC_STATE0_Reserved_7_6_MASK        0xc0

// Bitfield Description : Indicates 1st previous LC State.
#define PCIE_LC_STATE0_LC_PREV_STATE1_OFFSET      8
#define PCIE_LC_STATE0_LC_PREV_STATE1_MASK        0x3f00

// Bitfield Description :
#define PCIE_LC_STATE0_Reserved_15_14_OFFSET      14
#define PCIE_LC_STATE0_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates 2nd previous LC State.
#define PCIE_LC_STATE0_LC_PREV_STATE2_OFFSET      16
#define PCIE_LC_STATE0_LC_PREV_STATE2_MASK        0x3f0000

// Bitfield Description :
#define PCIE_LC_STATE0_Reserved_23_22_OFFSET      22
#define PCIE_LC_STATE0_Reserved_23_22_MASK        0xc00000

// Bitfield Description : Indicates 3rd previous LC State.
#define PCIE_LC_STATE0_LC_PREV_STATE3_OFFSET      24
#define PCIE_LC_STATE0_LC_PREV_STATE3_MASK        0x3f000000

// Bitfield Description :
#define PCIE_LC_STATE0_Reserved_31_30_OFFSET      30
#define PCIE_LC_STATE0_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LC_CURRENT_STATE:6;
    UINT32                            Reserved_7_6:2;
    UINT32                            LC_PREV_STATE1:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            LC_PREV_STATE2:6;
    UINT32                            Reserved_23_22:2;
    UINT32                            LC_PREV_STATE3:6;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_LC_STATE0_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LC_STATE0_ADDRESS    0x11140294UL
#define SMN_FUNC0_PCIE1_PCIE_LC_STATE0_ADDRESS    0x11240294UL

/***********************************************************
* Register Name : RCC_DEV2_PORT_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description :
#define RCC_DEV2_PORT_STRAP0_Reserved_0_0_OFFSET      0
#define RCC_DEV2_PORT_STRAP0_Reserved_0_0_MASK        0x1

// Bitfield Description : Enable ARI forwarding in Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_ARI_EN_DN_DEV2_OFFSET      1
#define RCC_DEV2_PORT_STRAP0_STRAP_ARI_EN_DN_DEV2_MASK        0x2

// Bitfield Description : Enable ACS capability in Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_ACS_EN_DN_DEV2_OFFSET      2
#define RCC_DEV2_PORT_STRAP0_STRAP_ACS_EN_DN_DEV2_MASK        0x4

// Bitfield Description : Enable AER capability in Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_AER_EN_DN_DEV2_OFFSET      3
#define RCC_DEV2_PORT_STRAP0_STRAP_AER_EN_DN_DEV2_MASK        0x8

// Bitfield Description : Enable CA error in AER capability in Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_CPL_ABORT_ERR_EN_DN_DEV2_OFFSET      4
#define RCC_DEV2_PORT_STRAP0_STRAP_CPL_ABORT_ERR_EN_DN_DEV2_MASK        0x10

// Bitfield Description : Set DEVICE_ID of Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_DEVICE_ID_DN_DEV2_OFFSET      5
#define RCC_DEV2_PORT_STRAP0_STRAP_DEVICE_ID_DN_DEV2_MASK        0x1fffe0

// Bitfield Description : Set InterruptPin of Switch Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_INTERRUPT_PIN_DN_DEV2_OFFSET      21
#define RCC_DEV2_PORT_STRAP0_STRAP_INTERRUPT_PIN_DN_DEV2_MASK        0xe00000

// Bitfield Description : Ignore UR caused by E2E Prefix for Downstream Port.
#define RCC_DEV2_PORT_STRAP0_STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV2_OFFSET      24
#define RCC_DEV2_PORT_STRAP0_STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV2_MASK        0x1000000

// Bitfield Description : Set MAX_PAYLOAD_SIZE in Downstream Port function.
#define RCC_DEV2_PORT_STRAP0_STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV2_OFFSET      25
#define RCC_DEV2_PORT_STRAP0_STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV2_MASK        0xe000000

// Bitfield Description : Set MAX_LINK_WIDTH for nBIF Virtual Link 0 -- x1 1-- x2 2 -- x4 3 -- x8 4 -- x12 5 -- x16 6 -- x32 7 -- reserved.
#define RCC_DEV2_PORT_STRAP0_STRAP_MAX_LINK_WIDTH_SUPPORT_DEV2_OFFSET      28
#define RCC_DEV2_PORT_STRAP0_STRAP_MAX_LINK_WIDTH_SUPPORT_DEV2_MASK        0x70000000

// Bitfield Description : Configure EPF0 as Dummy F0.
#define RCC_DEV2_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV2_OFFSET      31
#define RCC_DEV2_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV2_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            STRAP_ARI_EN_DN_DEV2:1;
    UINT32                            STRAP_ACS_EN_DN_DEV2:1;
    UINT32                            STRAP_AER_EN_DN_DEV2:1;
    UINT32                            STRAP_CPL_ABORT_ERR_EN_DN_DEV2:1;
    UINT32                            STRAP_DEVICE_ID_DN_DEV2:16;
    UINT32                            STRAP_INTERRUPT_PIN_DN_DEV2:3;
    UINT32                            STRAP_IGNORE_E2E_PREFIX_UR_DN_DEV2:1;
    UINT32                            STRAP_MAX_PAYLOAD_SUPPORT_DN_DEV2:3;
    UINT32                            STRAP_MAX_LINK_WIDTH_SUPPORT_DEV2:3;
    UINT32                            STRAP_EPF0_DUMMY_EN_DEV2:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_PORT_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP0_ADDRESS    0x10131400UL


/***********************************************************
* Register Name : RCC_DEV2_PORT_STRAP5
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : PWR_BUDGET_DATA when PWR_BUDGET_SEL=4.
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_4_DEV2_OFFSET      0
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_4_DEV2_MASK        0xff

// Bitfield Description : PWR_BUDGET_DATA when PWR_BUDGET_SEL=5.
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_5_DEV2_OFFSET      8
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_DATA_8T0_5_DEV2_MASK        0xff00

// Bitfield Description : Set PWR_BUDGET_CAP.SYSTEM_ALLOCATED.
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV2_OFFSET      16
#define RCC_DEV2_PORT_STRAP5_STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV2_MASK        0x10000

// Bitfield Description : Enable 64bit AtomicOP Completer in downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ATOMIC_64BIT_EN_DN_DEV2_OFFSET      17
#define RCC_DEV2_PORT_STRAP5_STRAP_ATOMIC_64BIT_EN_DN_DEV2_MASK        0x20000

// Bitfield Description : Enable AtomicOp Routing support in Downstream Port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ATOMIC_ROUTING_EN_DEV2_OFFSET      18
#define RCC_DEV2_PORT_STRAP5_STRAP_ATOMIC_ROUTING_EN_DEV2_MASK        0x40000

// Bitfield Description : Enable VC capability in Downstream Port.
#define RCC_DEV2_PORT_STRAP5_STRAP_VC_EN_DN_DEV2_OFFSET      19
#define RCC_DEV2_PORT_STRAP5_STRAP_VC_EN_DN_DEV2_MASK        0x80000

// Bitfield Description : Enable Second VC in EP function.
#define RCC_DEV2_PORT_STRAP5_STRAP_TwoVC_EN_DEV2_OFFSET      20
#define RCC_DEV2_PORT_STRAP5_STRAP_TwoVC_EN_DEV2_MASK        0x100000

// Bitfield Description : Enable Second VC in Downstream Port.
#define RCC_DEV2_PORT_STRAP5_STRAP_TwoVC_EN_DN_DEV2_OFFSET      21
#define RCC_DEV2_PORT_STRAP5_STRAP_TwoVC_EN_DN_DEV2_MASK        0x200000

// Bitfield Description : Indicate that this Port supports the Scaled Flow Control Featrue.
#define RCC_DEV2_PORT_STRAP5_STRAP_LOCAL_DLF_SUPPORTED_DEV2_OFFSET      22
#define RCC_DEV2_PORT_STRAP5_STRAP_LOCAL_DLF_SUPPORTED_DEV2_MASK        0x400000

// Bitfield Description : Enable Source Validatoin in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_SOURCE_VALIDATION_DN_DEV2_OFFSET      23
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_SOURCE_VALIDATION_DN_DEV2_MASK        0x800000

// Bitfield Description : Enable Transalation Blocking in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV2_OFFSET      24
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV2_MASK        0x1000000

// Bitfield Description : Enable P2P Request Redirect in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV2_OFFSET      25
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV2_MASK        0x2000000

// Bitfield Description : Enable P2P Completion Redirect in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV2_OFFSET      26
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV2_MASK        0x4000000

// Bitfield Description : Enable Upstream Forwarding in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV2_OFFSET      27
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV2_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV2_OFFSET      28
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV2_MASK        0x10000000

// Bitfield Description : Enable Direct Translated P2P in ACS capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV2_OFFSET      29
#define RCC_DEV2_PORT_STRAP5_STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV2_MASK        0x20000000

// Bitfield Description : Enable MSI MAP Capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_MSI_MAP_EN_DEV2_OFFSET      30
#define RCC_DEV2_PORT_STRAP5_STRAP_MSI_MAP_EN_DEV2_MASK        0x40000000

// Bitfield Description : Enable SSID Capability for downstream port.
#define RCC_DEV2_PORT_STRAP5_STRAP_SSID_EN_DEV2_OFFSET      31
#define RCC_DEV2_PORT_STRAP5_STRAP_SSID_EN_DEV2_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_PWR_BUDGET_DATA_8T0_4_DEV2:8;
    UINT32                            STRAP_PWR_BUDGET_DATA_8T0_5_DEV2:8;
    UINT32                            STRAP_PWR_BUDGET_SYSTEM_ALLOCATED_DEV2:1;
    UINT32                            STRAP_ATOMIC_64BIT_EN_DN_DEV2:1;
    UINT32                            STRAP_ATOMIC_ROUTING_EN_DEV2:1;
    UINT32                            STRAP_VC_EN_DN_DEV2:1;
    UINT32                            STRAP_TwoVC_EN_DEV2:1;
    UINT32                            STRAP_TwoVC_EN_DN_DEV2:1;
    UINT32                            STRAP_LOCAL_DLF_SUPPORTED_DEV2:1;
    UINT32                            STRAP_ACS_SOURCE_VALIDATION_DN_DEV2:1;
    UINT32                            STRAP_ACS_TRANSLATION_BLOCKING_DN_DEV2:1;
    UINT32                            STRAP_ACS_P2P_REQUEST_REDIRECT_DN_DEV2:1;
    UINT32                            STRAP_ACS_P2P_COMPLETION_REDIRECT_DN_DEV2:1;
    UINT32                            STRAP_ACS_UPSTREAM_FORWARDING_DN_DEV2:1;
    UINT32                            STRAP_ACS_P2P_EGRESS_CONTROL_DN_DEV2:1;
    UINT32                            STRAP_ACS_DIRECT_TRANSLATED_P2P_DN_DEV2:1;
    UINT32                            STRAP_MSI_MAP_EN_DEV2:1;
    UINT32                            STRAP_SSID_EN_DEV2:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_PORT_STRAP5_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_PORT_STRAP5_ADDRESS    0x10131414UL

/***********************************************************
* Register Name : PCIE_LC_PRESET_MASK_CNTL
* Register Description :
* Link Controller Control register for Enhanced Preset Search Masks.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When PCIE_LC_EQ_CNTL_8GT.LC_EQ_SEARCH_MODE_8GT field is 3, controls which presets are unmasked (to be evaluated),
// when in 8.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port). When any individual bit value is 1,
// the corresponding preset is included in the selection for evaluation, while a bit value of 0 excludes the corresponding preset from evaluation,
// unless all bits are 0, in which case this field is ignored. This field is a bit vector that covers from P0 to P9. Bit 0 corresponds to P0.
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_8GT_OFFSET      0
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_8GT_MASK        0x3ff

// Bitfield Description : When PCIE_LC_EQ_CNTL_16GT.LC_EQ_SEARCH_MODE_16GT field is 3, controls which presets are unmasked (to be evaluated),
// when in 16.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port). When any individual bit value is 1,
// the corresponding preset is included in the selection for evaluation, while a bit value of 0 excludes the corresponding preset from evaluation,
// unless all bits are 0, in which case this field is ignored. This field is a bit vector that covers from P0 to P9. Bit 0 corresponds to P0.
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_16GT_OFFSET      10
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_16GT_MASK        0xffc00

// Bitfield Description : When LC_EQ_SEARCH_MODE_32GT.LC_EQ_SEARCH_MODE_32GT field is 3, controls which presets are unmasked (to be evaluated),
// when in 32.0 GT/s Link Equalization requesting Phase (Phase 2 in an Upstream Port or Phase 3 in a Downstream Port). When any individual bit value is 1,
// the corresponding preset is included in the selection for evaluation, while a bit value of 0 excludes the corresponding preset from evaluation,
// unless all bits are 0, in which case this field is ignored. This field is a bit vector that covers from P0 to P9. Bit 0 corresponds to P0.
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_32GT_OFFSET      20
#define PCIE_LC_PRESET_MASK_CNTL_LC_PRESET_MASK_32GT_MASK        0x3ff00000

// Bitfield Description :
#define PCIE_LC_PRESET_MASK_CNTL_Reserved_31_30_OFFSET      30
#define PCIE_LC_PRESET_MASK_CNTL_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            LC_PRESET_MASK_8GT:10;
    UINT32                            LC_PRESET_MASK_16GT:10;
    UINT32                            LC_PRESET_MASK_32GT:10;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_LC_PRESET_MASK_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11140404UL

#define SMN_FUNC0_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11240404UL

#define SMN_FUNC1_PCIE0_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11141404UL

#define SMN_FUNC1_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11241404UL

#define SMN_FUNC2_PCIE0_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11142404UL

#define SMN_FUNC2_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11242404UL

#define SMN_FUNC3_PCIE0_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11143404UL

#define SMN_FUNC3_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11243404UL

#define SMN_FUNC4_PCIE0_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11144404UL

#define SMN_FUNC4_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11244404UL

#define SMN_FUNC5_PCIE1_PCIE_LC_PRESET_MASK_CNTL_ADDRESS    0x11245404UL

/***********************************************************
* Register Name : ATHUB_MISC_CNTL
* Register Description :
* control for atc domain clock gate
* Visibility : 0x4
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
* Register Name : PCIEP_HPGI
* Register Description :
* Hot-Plug Control and Status register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : When REG_HPGI_HOOK bit is 1, controls if the Port generates a SMI to indicate the start of a Hot-Plug event.
#define PCIEP_HPGI_REG_HPGI_ASSERT_TO_SMI_EN_OFFSET      0
#define PCIEP_HPGI_REG_HPGI_ASSERT_TO_SMI_EN_MASK        0x1

// Bitfield Description : When REG_HPGI_HOOK bit is 1, controls if the Port generates a SCI to indicate the start of a Hot-Plug event.
#define PCIEP_HPGI_REG_HPGI_ASSERT_TO_SCI_EN_OFFSET      1
#define PCIEP_HPGI_REG_HPGI_ASSERT_TO_SCI_EN_MASK        0x2

// Bitfield Description : When REG_HPGI_HOOK bit is 1, controls if the Port generates a SMI to indicate the end of a Hot-Plug event.
#define PCIEP_HPGI_REG_HPGI_DEASSERT_TO_SMI_EN_OFFSET      2
#define PCIEP_HPGI_REG_HPGI_DEASSERT_TO_SMI_EN_MASK        0x4

// Bitfield Description : When REG_HPGI_HOOK bit is 1, controls if the Port generates a SCI to indicate the end of a Hot-Plug event.
#define PCIEP_HPGI_REG_HPGI_DEASSERT_TO_SCI_EN_OFFSET      3
#define PCIEP_HPGI_REG_HPGI_DEASSERT_TO_SCI_EN_MASK        0x8

// Bitfield Description :
#define PCIEP_HPGI_Reserved_6_4_OFFSET      4
#define PCIEP_HPGI_Reserved_6_4_MASK        0x70

// Bitfield Description : Controls if Hot-Plug events are signaled by interrupt or by SMI/SCI.
#define PCIEP_HPGI_REG_HPGI_HOOK_OFFSET      7
#define PCIEP_HPGI_REG_HPGI_HOOK_MASK        0x80

// Bitfield Description : When REG_HPGI_HOOK bit is 1, indicates if the Port asserted SMI to indicate the start of a Hot-Plug event.
#define PCIEP_HPGI_HPGI_REG_ASSERT_TO_SMI_STATUS_OFFSET      8
#define PCIEP_HPGI_HPGI_REG_ASSERT_TO_SMI_STATUS_MASK        0x100

// Bitfield Description : When REG_HPGI_HOOK bit is 1, indicates if the Port asserted SCI to indicate the start of a Hot-Plug event.
#define PCIEP_HPGI_HPGI_REG_ASSERT_TO_SCI_STATUS_OFFSET      9
#define PCIEP_HPGI_HPGI_REG_ASSERT_TO_SCI_STATUS_MASK        0x200

// Bitfield Description : When REG_HPGI_HOOK bit is 1, indicates if the Port asserted SMI to indicate the end of a Hot-Plug event.
#define PCIEP_HPGI_HPGI_REG_DEASSERT_TO_SMI_STATUS_OFFSET      10
#define PCIEP_HPGI_HPGI_REG_DEASSERT_TO_SMI_STATUS_MASK        0x400

// Bitfield Description : When REG_HPGI_HOOK bit is 1, indicates if the Port asserted SCI to indicate the end of a Hot-Plug event.
#define PCIEP_HPGI_HPGI_REG_DEASSERT_TO_SCI_STATUS_OFFSET      11
#define PCIEP_HPGI_HPGI_REG_DEASSERT_TO_SCI_STATUS_MASK        0x800

// Bitfield Description :
#define PCIEP_HPGI_Reserved_14_12_OFFSET      12
#define PCIEP_HPGI_Reserved_14_12_MASK        0x7000

// Bitfield Description : When REG_HPGI_HOOK bit is 1 and REG_HPGI_PRESENCE_DETECT_STATE_CHANGE_EN bit is 0, indicates the Port's masked presence detect state change status.
#define PCIEP_HPGI_HPGI_REG_PRESENCE_DETECT_STATE_CHANGE_STATUS_OFFSET      15
#define PCIEP_HPGI_HPGI_REG_PRESENCE_DETECT_STATE_CHANGE_STATUS_MASK        0x8000

// Bitfield Description : When REG_HPGI_HOOK bit is 1, controls if the Port's SLOT_STATUS.PRESENCE_DETECT_STATE bit reports the Port's unmasked presence detect status.
#define PCIEP_HPGI_REG_HPGI_PRESENCE_DETECT_STATE_CHANGE_EN_OFFSET      16
#define PCIEP_HPGI_REG_HPGI_PRESENCE_DETECT_STATE_CHANGE_EN_MASK        0x10000

// Bitfield Description : Controls if SMI and SCI are to be generated on HPGI DL_Active events.
#define PCIEP_HPGI_HPGI_BLOCK_DL_ACTIVE_INT_OFFSET      17
#define PCIEP_HPGI_HPGI_BLOCK_DL_ACTIVE_INT_MASK        0x20000

// Bitfield Description :
#define PCIEP_HPGI_Reserved_31_18_OFFSET      18
#define PCIEP_HPGI_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            REG_HPGI_ASSERT_TO_SMI_EN:1;
    UINT32                            REG_HPGI_ASSERT_TO_SCI_EN:1;
    UINT32                            REG_HPGI_DEASSERT_TO_SMI_EN:1;
    UINT32                            REG_HPGI_DEASSERT_TO_SCI_EN:1;
    UINT32                            Reserved_6_4:3;
    UINT32                            REG_HPGI_HOOK:1;
    UINT32                            HPGI_REG_ASSERT_TO_SMI_STATUS:1;
    UINT32                            HPGI_REG_ASSERT_TO_SCI_STATUS:1;
    UINT32                            HPGI_REG_DEASSERT_TO_SMI_STATUS:1;
    UINT32                            HPGI_REG_DEASSERT_TO_SCI_STATUS:1;
    UINT32                            Reserved_14_12:3;
    UINT32                            HPGI_REG_PRESENCE_DETECT_STATE_CHANGE_STATUS:1;
    UINT32                            REG_HPGI_PRESENCE_DETECT_STATE_CHANGE_EN:1;
    UINT32                            HPGI_BLOCK_DL_ACTIVE_INT:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} PCIEP_HPGI_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HPGI_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HPGI_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HPGI_OFFSET       0x368
#define PCICFG_NBIO_FUNC1_PCIE1_PCIEP_HPGI_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x368)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HPGI_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HPGI_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HPGI_OFFSET       0x368
#define PCICFG_NBIO_FUNC2_PCIE1_PCIEP_HPGI_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x368)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HPGI_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HPGI_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HPGI_OFFSET       0x368
#define PCICFG_NBIO_FUNC3_PCIE1_PCIEP_HPGI_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x368)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HPGI_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HPGI_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HPGI_OFFSET       0x368
#define PCICFG_NBIO_FUNC4_PCIE1_PCIEP_HPGI_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x368)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HPGI_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HPGI_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HPGI_OFFSET       0x368
#define PCICFG_NBIO_FUNC5_PCIE1_PCIEP_HPGI_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x368)

#define SMN_FUNC0_PCIE0_PCIEP_HPGI_ADDRESS    0x11140368UL

#define SMN_FUNC0_PCIE1_PCIEP_HPGI_ADDRESS    0x11240368UL

#define SMN_FUNC0_PCIE2_PCIEP_HPGI_ADDRESS    0x11340368UL

#define SMN_FUNC0_PCIE3_PCIEP_HPGI_ADDRESS    0x11440368UL

#define SMN_FUNC1_PCIE0_PCIEP_HPGI_ADDRESS    0x11141368UL

#define SMN_FUNC1_PCIE1_PCIEP_HPGI_ADDRESS    0x11241368UL

#define SMN_FUNC2_PCIE0_PCIEP_HPGI_ADDRESS    0x11142368UL

#define SMN_FUNC2_PCIE1_PCIEP_HPGI_ADDRESS    0x11242368UL

#define SMN_FUNC3_PCIE0_PCIEP_HPGI_ADDRESS    0x11143368UL

#define SMN_FUNC3_PCIE1_PCIEP_HPGI_ADDRESS    0x11243368UL

#define SMN_FUNC4_PCIE0_PCIEP_HPGI_ADDRESS    0x11144368UL

#define SMN_FUNC4_PCIE1_PCIEP_HPGI_ADDRESS    0x11244368UL

#define SMN_FUNC5_PCIE1_PCIEP_HPGI_ADDRESS    0x11245368UL

/***********************************************************
* Register Name : SMU_PCIE_USB_BDF_ADDRESS
* Register Description :
* RSMU USB BDF Address register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the address of the Requester ID for PCIE over USB4.
#define SMU_PCIE_USB_BDF_ADDRESS_USB_BDF_ADDR_LOWER_OFFSET      0
#define SMU_PCIE_USB_BDF_ADDRESS_USB_BDF_ADDR_LOWER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            USB_BDF_ADDR_LOWER:32;
  } Field;
  UINT32 Value;
} SMU_PCIE_USB_BDF_ADDRESS_STRUCT;

#define SMN_PCIE2_SMU_PCIE_USB_BDF_ADDRESS_ADDRESS    0x113801b0UL

#define SMN_PCIE3_SMU_PCIE_USB_BDF_ADDRESS_ADDRESS    0x114801b0UL

/***********************************************************
* Register Name : SMU_PCIE_USB_S_LTR_ADDRESS
* Register Description :
* RSMU USB Snoop LTR Address register.
* Visibility : 0x4
************************************************************/

// Bitfield Description : Controls the address of the LTR for PCIE over USB4.
#define SMU_PCIE_USB_S_LTR_ADDRESS_USB_S_LTR_ADDR_LOWER_OFFSET      0
#define SMU_PCIE_USB_S_LTR_ADDRESS_USB_S_LTR_ADDR_LOWER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            USB_S_LTR_ADDR_LOWER:32;
  } Field;
  UINT32 Value;
} SMU_PCIE_USB_S_LTR_ADDRESS_STRUCT;

#define SMN_PCIE2_SMU_PCIE_USB_S_LTR_ADDRESS_ADDRESS    0x113801b4UL

#define SMN_PCIE3_SMU_PCIE_USB_S_LTR_ADDRESS_ADDRESS    0x114801b4UL

/***********************************************************
* Register Name : RCC_DEV0_EPF7_STRAP0
* Register Description :
*
* Visibility : 0x2
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV0_EPF7_STRAP0_STRAP_DEVICE_ID_DEV0_F7_OFFSET      0
#define RCC_DEV0_EPF7_STRAP0_STRAP_DEVICE_ID_DEV0_F7_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV0_EPF7_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F7_OFFSET      16
#define RCC_DEV0_EPF7_STRAP0_STRAP_MAJOR_REV_ID_DEV0_F7_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV0_EPF7_STRAP0_STRAP_MINOR_REV_ID_DEV0_F7_OFFSET      20
#define RCC_DEV0_EPF7_STRAP0_STRAP_MINOR_REV_ID_DEV0_F7_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV0_EPF7_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV0_EPF7_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV0_EPF7_STRAP0_STRAP_FUNC_EN_DEV0_F7_OFFSET      28
#define RCC_DEV0_EPF7_STRAP0_STRAP_FUNC_EN_DEV0_F7_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV0_EPF7_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F7_OFFSET      29
#define RCC_DEV0_EPF7_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F7_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV0_EPF7_STRAP0_STRAP_D1_SUPPORT_DEV0_F7_OFFSET      30
#define RCC_DEV0_EPF7_STRAP0_STRAP_D1_SUPPORT_DEV0_F7_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV0_EPF7_STRAP0_STRAP_D2_SUPPORT_DEV0_F7_OFFSET      31
#define RCC_DEV0_EPF7_STRAP0_STRAP_D2_SUPPORT_DEV0_F7_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV0_F7:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV0_F7:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV0_F7:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV0_F7:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV0_F7:1;
    UINT32                            STRAP_D1_SUPPORT_DEV0_F7:1;
    UINT32                            STRAP_D2_SUPPORT_DEV0_F7:1;
  } Field;
  UINT32 Value;
} RCC_DEV0_EPF7_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV0_EPF7_STRAP0_ADDRESS    0x10134e00UL

/***********************************************************
* Register Name : SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : The number of tag reserved for VC0 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC0_CHAIN_OFFSET      0
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC0_CHAIN_MASK        0xff

// Bitfield Description : The number of tag reserved for VC1 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC1_CHAIN_OFFSET      8
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC1_CHAIN_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC2 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC2_CHAIN_OFFSET      16
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC2_CHAIN_MASK        0xff0000

// Bitfield Description : The number of tag reserved for VC3 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC3_CHAIN_OFFSET      24
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_TAG_ALLOC_FOR_VC3_CHAIN_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC0_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC1_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC2_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC3_CHAIN:8;
  } Field;
  UINT32 Value;
} SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_STRUCT;

#define SMN_NBIF0_SHUB_A2S_TAG_ALLOC_FOR_CHAIN_0_ADDRESS    0x140f96cUL


/***********************************************************
* Register Name : SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : The number of tag reserved for VC4 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC4_CHAIN_OFFSET      0
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC4_CHAIN_MASK        0xff

// Bitfield Description : The number of tag reserved for VC5 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC5_CHAIN_OFFSET      8
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC5_CHAIN_MASK        0xff00

// Bitfield Description : The number of tag reserved for VC6 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC6_CHAIN_OFFSET      16
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC6_CHAIN_MASK        0xff0000

// Bitfield Description : The number of tag reserved for VC7 write chain request.
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC7_CHAIN_OFFSET      24
#define SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_TAG_ALLOC_FOR_VC7_CHAIN_MASK        0xff000000

typedef union {
  struct {
    UINT32                            TAG_ALLOC_FOR_VC4_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC5_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC6_CHAIN:8;
    UINT32                            TAG_ALLOC_FOR_VC7_CHAIN:8;
  } Field;
  UINT32 Value;
} SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_STRUCT;

#define SMN_NBIF0_SHUB_A2S_TAG_ALLOC_FOR_CHAIN_1_ADDRESS    0x140f970UL

/***********************************************************
* Register Name : NGDC_PG_MISC_CTRL
* Register Description :
* ngdc pg misc control registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description :
#define NGDC_PG_MISC_CTRL_Reserved_9_0_OFFSET      0
#define NGDC_PG_MISC_CTRL_Reserved_9_0_MASK        0x3ff

// Bitfield Description : 1: PG is only permitted when all endpoint's Dstate are D3.
#define NGDC_PG_MISC_CTRL_NGDC_PG_ENDP_D3_ONLY_OFFSET      10
#define NGDC_PG_MISC_CTRL_NGDC_PG_ENDP_D3_ONLY_MASK        0x400

// Bitfield Description :
#define NGDC_PG_MISC_CTRL_Reserved_12_11_OFFSET      11
#define NGDC_PG_MISC_CTRL_Reserved_12_11_MASK        0x1800

// Bitfield Description : 1: when gdc/shub is on the way to enter PG, force-disable MGCG --make gdc/shub's (only when gdc_pg is both) all clk branches always-running.
#define NGDC_PG_MISC_CTRL_NGDC_PG_CLK_PERM1_OFFSET      13
#define NGDC_PG_MISC_CTRL_NGDC_PG_CLK_PERM1_MASK        0x2000

// Bitfield Description : 1: SHUB_CLK#_DS_ALLOW would have no effect for PG; 0: DS_ALLOW affect PG. .
#define NGDC_PG_MISC_CTRL_NGDC_PG_DS_ALLOW_DIS_OFFSET      14
#define NGDC_PG_MISC_CTRL_NGDC_PG_DS_ALLOW_DIS_MASK        0x4000

// Bitfield Description :
#define NGDC_PG_MISC_CTRL_Reserved_15_15_OFFSET      15
#define NGDC_PG_MISC_CTRL_Reserved_15_15_MASK        0x8000

// Bitfield Description : 1: when gdc/shub is already in PG(iso-enable asserts), force-disable MGCG --make gdc/shub's (only when gdc_pg is both) all clk branches always-running.
#define NGDC_PG_MISC_CTRL_NGDC_PG_CLK_PERM2_OFFSET      16
#define NGDC_PG_MISC_CTRL_NGDC_PG_CLK_PERM2_MASK        0x10000

// Bitfield Description :
#define NGDC_PG_MISC_CTRL_Reserved_23_17_OFFSET      17
#define NGDC_PG_MISC_CTRL_Reserved_23_17_MASK        0xfe0000

// Bitfield Description : proper setting for 200ns time stamp based on refclk.
#define NGDC_PG_MISC_CTRL_NGDC_CFG_REFCLK_CYCLE_FOR_200NS_OFFSET      24
#define NGDC_PG_MISC_CTRL_NGDC_CFG_REFCLK_CYCLE_FOR_200NS_MASK        0x3f000000

// Bitfield Description :
#define NGDC_PG_MISC_CTRL_Reserved_30_30_OFFSET      30
#define NGDC_PG_MISC_CTRL_Reserved_30_30_MASK        0x40000000

// Bitfield Description : Only for debug use.1, gdc is forced to exit from PG mode; if this bit is set,it will avoid entering pg-mode.
#define NGDC_PG_MISC_CTRL_NGDC_CFG_PG_EXIT_OVERRIDE_OFFSET      31
#define NGDC_PG_MISC_CTRL_NGDC_CFG_PG_EXIT_OVERRIDE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_9_0:10;
    UINT32                            NGDC_PG_ENDP_D3_ONLY:1;
    UINT32                            Reserved_12_11:2;
    UINT32                            NGDC_PG_CLK_PERM1:1;
    UINT32                            NGDC_PG_DS_ALLOW_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            NGDC_PG_CLK_PERM2:1;
    UINT32                            Reserved_23_17:7;
    UINT32                            NGDC_CFG_REFCLK_CYCLE_FOR_200NS:6;
    UINT32                            Reserved_30_30:1;
    UINT32                            NGDC_CFG_PG_EXIT_OVERRIDE:1;
  } Field;
  UINT32 Value;
} NGDC_PG_MISC_CTRL_STRUCT;

#define SMN_NBIF0_NGDC_PG_MISC_CTRL_ADDRESS    0x1403c60UL

/***********************************************************
* Register Name : RIOMMU_CP_CNTRL_0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Treat all page and device invalidations as invalidate_all command.
#define RIOMMU_CP_CNTRL_0_CONVERT_TO_INV_ALL_OFFSET      0
#define RIOMMU_CP_CNTRL_0_CONVERT_TO_INV_ALL_MASK        0x1

// Bitfield Description : Do not refetch DTE/Level4/Level3 entries during invalidation.
#define RIOMMU_CP_CNTRL_0_DISABLE_REFETCH_OFFSET      1
#define RIOMMU_CP_CNTRL_0_DISABLE_REFETCH_MASK        0x2

// Bitfield Description :
#define RIOMMU_CP_CNTRL_0_Reserved_31_2_OFFSET      2
#define RIOMMU_CP_CNTRL_0_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            CONVERT_TO_INV_ALL:1;
    UINT32                            DISABLE_REFETCH:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} RIOMMU_CP_CNTRL_0_STRUCT;

#define SMN_RIOMMU_CP_CNTRL_0_ADDRESS        0x16b0a124UL

#endif /* _GLOBALREGS_H_ */
