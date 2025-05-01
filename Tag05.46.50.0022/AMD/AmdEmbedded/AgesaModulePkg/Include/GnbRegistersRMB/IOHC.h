/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IOHC_H_
#define _IOHC_H_


/***********************************************************
* Register Name : CCP_BASE_ADDR_HI
* Register Description :
* CCP private MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : CCP private MMIO base address bits 47:32.
#define CCP_BASE_ADDR_HI_CCP_BASE_ADDR_HI_OFFSET      0
#define CCP_BASE_ADDR_HI_CCP_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define CCP_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define CCP_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            CCP_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} CCP_BASE_ADDR_HI_STRUCT;

#define SMN_CCP_BASE_ADDR_HI_ADDRESS    0x13b102dcUL


/***********************************************************
* Register Name : CCP_BASE_ADDR_LO
* Register Description :
* CCP MMIO base address
* Visibility : 0x2
************************************************************/

// Bitfield Description : CCP private MMIO enable.
#define CCP_BASE_ADDR_LO_CCP_MMIO_EN_OFFSET      0
#define CCP_BASE_ADDR_LO_CCP_MMIO_EN_MASK        0x1

// Bitfield Description : 
#define CCP_BASE_ADDR_LO_Reserved_7_1_OFFSET      1
#define CCP_BASE_ADDR_LO_Reserved_7_1_MASK        0xfe

// Bitfield Description : Locks the CCP private MMIO address range and enable until the next warm reset.
#define CCP_BASE_ADDR_LO_CCP_MMIO_LOCK_OFFSET      8
#define CCP_BASE_ADDR_LO_CCP_MMIO_LOCK_MASK        0x100

// Bitfield Description : 
#define CCP_BASE_ADDR_LO_Reserved_19_9_OFFSET      9
#define CCP_BASE_ADDR_LO_Reserved_19_9_MASK        0xffe00

// Bitfield Description : CCP private MMIO base address bits 31:20.
#define CCP_BASE_ADDR_LO_CCP_BASE_ADDR_LO_OFFSET      20
#define CCP_BASE_ADDR_LO_CCP_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CCP_MMIO_EN:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            CCP_MMIO_LOCK:1;
    UINT32                            Reserved_19_9:11;
    UINT32                            CCP_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} CCP_BASE_ADDR_LO_STRUCT;

#define SMN_CCP_BASE_ADDR_LO_ADDRESS    0x13b102d8UL


/***********************************************************
* Register Name : CFG_IOHC_PCI
* Register Description :
* IOHC PCI Configuration.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enables configuration accesses to Dev0 Fun2 (Used by IOMMU).
#define CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_OFFSET      0
#define CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_MASK        0x1

// Bitfield Description : 
#define CFG_IOHC_PCI_Reserved_30_1_OFFSET      1
#define CFG_IOHC_PCI_Reserved_30_1_MASK        0x7ffffffe

// Bitfield Description : FUSE controlled IOMMU Disable readback.
#define CFG_IOHC_PCI_IOMMU_DIS_OFFSET      31
#define CFG_IOHC_PCI_IOMMU_DIS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CFG_IOHC_PCI_Dev0Fn2RegEn:1;
    UINT32                            Reserved_30_1:30;
    UINT32                            IOMMU_DIS:1;
  } Field;
  UINT32 Value;
} CFG_IOHC_PCI_STRUCT;

#define SMN_CFG_IOHC_PCI_ADDRESS    0x13b10034UL


/***********************************************************
* Register Name : CFG_ZSC_ACK_MASK
* Register Description :
* Chicken bit mask to override fence_ack bits to iohub_zsc_aggregator.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable override of a particular fence_ack signal from a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_ACK_MASK_EN_OFFSET      0
#define CFG_ZSC_ACK_MASK_EN_MASK        0xffff

// Bitfield Description : Set override value of a particular fence_ack signal from a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_ACK_MASK_VAL_OFFSET      16
#define CFG_ZSC_ACK_MASK_VAL_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            EN:16;
    UINT32                            VAL:16;
  } Field;
  UINT32 Value;
} CFG_ZSC_ACK_MASK_STRUCT;

#define SMN_CFG_ZSC_ACK_MASK_ADDRESS    0x13b10040UL


/***********************************************************
* Register Name : CFG_ZSC_IDLE_MASK
* Register Description :
* Chicken bit mask to override idle bits to iohub_zsc_aggregator.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable override of a particular idle signal from a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_IDLE_MASK_EN_OFFSET      0
#define CFG_ZSC_IDLE_MASK_EN_MASK        0xffff

// Bitfield Description : Set override value of a particular idle signal from a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_IDLE_MASK_VAL_OFFSET      16
#define CFG_ZSC_IDLE_MASK_VAL_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            EN:16;
    UINT32                            VAL:16;
  } Field;
  UINT32 Value;
} CFG_ZSC_IDLE_MASK_STRUCT;

#define SMN_CFG_ZSC_IDLE_MASK_ADDRESS    0x13b10038UL


/***********************************************************
* Register Name : CFG_ZSC_REQ_MASK
* Register Description :
* Chicken bit mask to override fence_req bits from iohub_zsc_aggregator.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable override of a particular fence_req signal to a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_REQ_MASK_EN_OFFSET      0
#define CFG_ZSC_REQ_MASK_EN_MASK        0xffff

// Bitfield Description : Set override value of a particular fence_req signal to a given IP index (see iohub_zsc_aggregator RTL for values of indices).
#define CFG_ZSC_REQ_MASK_VAL_OFFSET      16
#define CFG_ZSC_REQ_MASK_VAL_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            EN:16;
    UINT32                            VAL:16;
  } Field;
  UINT32 Value;
} CFG_ZSC_REQ_MASK_STRUCT;

#define SMN_CFG_ZSC_REQ_MASK_ADDRESS    0x13b1003cUL


/***********************************************************
* Register Name : CLASS_CODE_REVID
* Register Description :
* Class Code Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Minor Revision ID.
#define CLASS_CODE_REVID_MINOR_REV_ID_OFFSET      0
#define CLASS_CODE_REVID_MINOR_REV_ID_MASK        0xf

// Bitfield Description : Major Revision ID.
#define CLASS_CODE_REVID_MAJOR_REV_ID_OFFSET      4
#define CLASS_CODE_REVID_MAJOR_REV_ID_MASK        0xf0

// Bitfield Description : Identify the function of the device.
// This field is always 0x06 indicating a base class of bridge device.
#define CLASS_CODE_REVID_CLASS_CODE_OFFSET      8
#define CLASS_CODE_REVID_CLASS_CODE_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            MINOR_REV_ID:4;
    UINT32                            MAJOR_REV_ID:4;
    UINT32                            CLASS_CODE:24;
  } Field;
  UINT32 Value;
} CLASS_CODE_REVID_STRUCT;

#define PCICFG_NBIO_IOHUB_CLASS_CODE_REVID_DEVICE       8
#define PCICFG_NBIO_IOHUB_CLASS_CODE_REVID_FUNC         0
#define PCICFG_NBIO_IOHUB_CLASS_CODE_REVID_OFFSET       0x8
#define PCICFG_NBIO_IOHUB_CLASS_CODE_REVID_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0x8)

#define PCICFG_NBIO_HB0_IOHUB_CLASS_CODE_REVID_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_CLASS_CODE_REVID_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_CLASS_CODE_REVID_OFFSET       0x8
#define PCICFG_NBIO_HB0_IOHUB_CLASS_CODE_REVID_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0x8)

#define SMN_PCIEHB0_CLASS_CODE_REVID_ADDRESS    0x13b41008UL

#define PCICFG_NBIO_HB1_IOHUB_CLASS_CODE_REVID_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_CLASS_CODE_REVID_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_CLASS_CODE_REVID_OFFSET       0x8
#define PCICFG_NBIO_HB1_IOHUB_CLASS_CODE_REVID_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0x8)

#define SMN_PCIEHB1_CLASS_CODE_REVID_ADDRESS    0x13b42008UL

#define SMN_PCIEHB2_CLASS_CODE_REVID_ADDRESS    0x13b43008UL

#define SMN_PCIEHB3_CLASS_CODE_REVID_ADDRESS    0x13b44008UL

#define SMN_PCIEHB4_CLASS_CODE_REVID_ADDRESS    0x13b48008UL


/***********************************************************
* Register Name : DBG_BASE_ADDR_HI
* Register Description :
* DBG MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : DBG MMIO base address bits 47:32.
#define DBG_BASE_ADDR_HI_DBG_BASE_ADDR_HI_OFFSET      0
#define DBG_BASE_ADDR_HI_DBG_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define DBG_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define DBG_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            DBG_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} DBG_BASE_ADDR_HI_STRUCT;

#define SMN_DBG_BASE_ADDR_HI_ADDRESS    0x13b102fcUL


/***********************************************************
* Register Name : DBG_BASE_ADDR_LO
* Register Description :
* DBG MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : DBG MMIO enable.
#define DBG_BASE_ADDR_LO_DBG_MMIO_EN_OFFSET      0
#define DBG_BASE_ADDR_LO_DBG_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the DBG MMIO address range and enable until the next warm reset.
#define DBG_BASE_ADDR_LO_DBG_MMIO_LOCK_OFFSET      1
#define DBG_BASE_ADDR_LO_DBG_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define DBG_BASE_ADDR_LO_Reserved_19_2_OFFSET      2
#define DBG_BASE_ADDR_LO_Reserved_19_2_MASK        0xffffc

// Bitfield Description : DBG MMIO base address bits 31:20.
#define DBG_BASE_ADDR_LO_DBG_BASE_ADDR_LO_OFFSET      20
#define DBG_BASE_ADDR_LO_DBG_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            DBG_MMIO_EN:1;
    UINT32                            DBG_MMIO_LOCK:1;
    UINT32                            Reserved_19_2:18;
    UINT32                            DBG_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} DBG_BASE_ADDR_LO_STRUCT;

#define SMN_DBG_BASE_ADDR_LO_ADDRESS    0x13b102f8UL


/***********************************************************
* Register Name : DEVICE_VENDOR_ID
* Register Description :
* Vendor/Device ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor Identifier
#define DEVICE_VENDOR_ID_VENDOR_ID_OFFSET      0
#define DEVICE_VENDOR_ID_VENDOR_ID_MASK        0xffff

// Bitfield Description : Device Identifier
#define DEVICE_VENDOR_ID_DEVICE_ID_OFFSET      16
#define DEVICE_VENDOR_ID_DEVICE_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VENDOR_ID:16;
    UINT32                            DEVICE_ID:16;
  } Field;
  UINT32 Value;
} DEVICE_VENDOR_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_DEVICE       8
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0x0)

#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0x0)

#define SMN_PCIEHB0_DEVICE_VENDOR_ID_ADDRESS    0x13b41000UL

#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0x0)

#define SMN_PCIEHB1_DEVICE_VENDOR_ID_ADDRESS    0x13b42000UL

#define SMN_PCIEHB2_DEVICE_VENDOR_ID_ADDRESS    0x13b43000UL

#define SMN_PCIEHB3_DEVICE_VENDOR_ID_ADDRESS    0x13b44000UL

#define SMN_PCIEHB4_DEVICE_VENDOR_ID_ADDRESS    0x13b48000UL


/***********************************************************
* Register Name : DEVICE_VENDOR_ID_W
* Register Description :
* Vendor/Device ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor Identifier
#define DEVICE_VENDOR_ID_W_VENDOR_ID_OFFSET      0
#define DEVICE_VENDOR_ID_W_VENDOR_ID_MASK        0xffff

// Bitfield Description : Device Identifier
#define DEVICE_VENDOR_ID_W_DEVICE_ID_OFFSET      16
#define DEVICE_VENDOR_ID_W_DEVICE_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VENDOR_ID:16;
    UINT32                            DEVICE_ID:16;
  } Field;
  UINT32 Value;
} DEVICE_VENDOR_ID_W_STRUCT;

#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_W_DEVICE       8
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_W_FUNC         0
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_W_OFFSET       0x50
#define PCICFG_NBIO_IOHUB_DEVICE_VENDOR_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0x50)

#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_W_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_W_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_W_OFFSET       0x50
#define PCICFG_NBIO_HB0_IOHUB_DEVICE_VENDOR_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0x50)

#define SMN_PCIEHB0_DEVICE_VENDOR_ID_W_ADDRESS    0x13b41050UL

#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_W_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_W_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_W_OFFSET       0x50
#define PCICFG_NBIO_HB1_IOHUB_DEVICE_VENDOR_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0x50)

#define SMN_PCIEHB1_DEVICE_VENDOR_ID_W_ADDRESS    0x13b42050UL

#define SMN_PCIEHB2_DEVICE_VENDOR_ID_W_ADDRESS    0x13b43050UL

#define SMN_PCIEHB3_DEVICE_VENDOR_ID_W_ADDRESS    0x13b44050UL

#define SMN_PCIEHB4_DEVICE_VENDOR_ID_W_ADDRESS    0x13b48050UL


/***********************************************************
* Register Name : EGRESS_POISON_MASK_HI
* Register Description :
* Enable masking of EGRESS_POISON STATUS.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable masking of errors logged in EGRESS_POISON_STATUS_HI. For each bit, 1=Errors are masked 0=Errors trigger response actions.
#define EGRESS_POISON_MASK_HI_EgressPoisonMaskHi_OFFSET      0
#define EGRESS_POISON_MASK_HI_EgressPoisonMaskHi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            EgressPoisonMaskHi:32;
  } Field;
  UINT32 Value;
} EGRESS_POISON_MASK_HI_STRUCT;

#define SMN_EGRESS_POISON_MASK_HI_ADDRESS    0x13b2082cUL


/***********************************************************
* Register Name : EGRESS_POISON_MASK_LO
* Register Description :
* Enable masking of EGRESS_POISON STATUS.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable masking of errors logged in EGRESS_POISON_STATUS_LO. For each bit, 1=Errors are masked 0=Errors trigger response actions.
#define EGRESS_POISON_MASK_LO_EgressPoisonMaskLo_OFFSET      0
#define EGRESS_POISON_MASK_LO_EgressPoisonMaskLo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            EgressPoisonMaskLo:32;
  } Field;
  UINT32 Value;
} EGRESS_POISON_MASK_LO_STRUCT;

#define SMN_EGRESS_POISON_MASK_LO_ADDRESS    0x13b20828UL


/***********************************************************
* Register Name : EGRESS_POISON_SEVERITY_HI
* Register Description :
* IOHC Egress POISON SEVERITY control bit.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC. 1=High Severity 0=Low Severity.
#define EGRESS_POISON_SEVERITY_HI_EgressPoisonSeverityHi_OFFSET      0
#define EGRESS_POISON_SEVERITY_HI_EgressPoisonSeverityHi_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            EgressPoisonSeverityHi:32;
  } Field;
  UINT32 Value;
} EGRESS_POISON_SEVERITY_HI_STRUCT;

#define SMN_EGRESS_POISON_SEVERITY_HI_ADDRESS    0x13b20834UL


/***********************************************************
* Register Name : EGRESS_POISON_SEVERITY_LO
* Register Description :
* IOHC Egress POISON SEVERITY control bit.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC. 1=High Severity 0=Low Severity.
#define EGRESS_POISON_SEVERITY_LO_EgressPoisonSeverityLo_OFFSET      0
#define EGRESS_POISON_SEVERITY_LO_EgressPoisonSeverityLo_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            EgressPoisonSeverityLo:32;
  } Field;
  UINT32 Value;
} EGRESS_POISON_SEVERITY_LO_STRUCT;

#define SMN_EGRESS_POISON_SEVERITY_LO_ADDRESS    0x13b20830UL


/***********************************************************
* Register Name : EGRESS_POISON_STATUS_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_0_OFFSET      0
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_0_MASK        0x1

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_1_OFFSET      1
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_1_MASK        0x2

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_2_OFFSET      2
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_2_MASK        0x4

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_3_OFFSET      3
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_3_MASK        0x8

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_4_OFFSET      4
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_4_MASK        0x10

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_5_OFFSET      5
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_5_MASK        0x20

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_6_OFFSET      6
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_6_MASK        0x40

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_7_OFFSET      7
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_7_MASK        0x80

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_8_OFFSET      8
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_8_MASK        0x100

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_9_OFFSET      9
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_9_MASK        0x200

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_10_OFFSET      10
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_10_MASK        0x400

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_11_OFFSET      11
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_11_MASK        0x800

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_12_OFFSET      12
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_12_MASK        0x1000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_13_OFFSET      13
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_13_MASK        0x2000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_14_OFFSET      14
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_14_MASK        0x4000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_15_OFFSET      15
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_15_MASK        0x8000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_16_OFFSET      16
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_16_MASK        0x10000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_17_OFFSET      17
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_17_MASK        0x20000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_18_OFFSET      18
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_18_MASK        0x40000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_19_OFFSET      19
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_19_MASK        0x80000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_20_OFFSET      20
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_20_MASK        0x100000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_21_OFFSET      21
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_21_MASK        0x200000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_22_OFFSET      22
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_22_MASK        0x400000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_23_OFFSET      23
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_23_MASK        0x800000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_24_OFFSET      24
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_24_MASK        0x1000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_25_OFFSET      25
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_25_MASK        0x2000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_26_OFFSET      26
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_26_MASK        0x4000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_27_OFFSET      27
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_27_MASK        0x8000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_28_OFFSET      28
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_28_MASK        0x10000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_29_OFFSET      29
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_29_MASK        0x20000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_30_OFFSET      30
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_30_MASK        0x40000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_31_OFFSET      31
#define EGRESS_POISON_STATUS_HI_EgressPoisonStatusHi_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            EgressPoisonStatusHi_0:1;
    UINT32                            EgressPoisonStatusHi_1:1;
    UINT32                            EgressPoisonStatusHi_2:1;
    UINT32                            EgressPoisonStatusHi_3:1;
    UINT32                            EgressPoisonStatusHi_4:1;
    UINT32                            EgressPoisonStatusHi_5:1;
    UINT32                            EgressPoisonStatusHi_6:1;
    UINT32                            EgressPoisonStatusHi_7:1;
    UINT32                            EgressPoisonStatusHi_8:1;
    UINT32                            EgressPoisonStatusHi_9:1;
    UINT32                            EgressPoisonStatusHi_10:1;
    UINT32                            EgressPoisonStatusHi_11:1;
    UINT32                            EgressPoisonStatusHi_12:1;
    UINT32                            EgressPoisonStatusHi_13:1;
    UINT32                            EgressPoisonStatusHi_14:1;
    UINT32                            EgressPoisonStatusHi_15:1;
    UINT32                            EgressPoisonStatusHi_16:1;
    UINT32                            EgressPoisonStatusHi_17:1;
    UINT32                            EgressPoisonStatusHi_18:1;
    UINT32                            EgressPoisonStatusHi_19:1;
    UINT32                            EgressPoisonStatusHi_20:1;
    UINT32                            EgressPoisonStatusHi_21:1;
    UINT32                            EgressPoisonStatusHi_22:1;
    UINT32                            EgressPoisonStatusHi_23:1;
    UINT32                            EgressPoisonStatusHi_24:1;
    UINT32                            EgressPoisonStatusHi_25:1;
    UINT32                            EgressPoisonStatusHi_26:1;
    UINT32                            EgressPoisonStatusHi_27:1;
    UINT32                            EgressPoisonStatusHi_28:1;
    UINT32                            EgressPoisonStatusHi_29:1;
    UINT32                            EgressPoisonStatusHi_30:1;
    UINT32                            EgressPoisonStatusHi_31:1;
  } Field;
  UINT32 Value;
} EGRESS_POISON_STATUS_HI_STRUCT;

#define SMN_EGRESS_POISON_STATUS_HI_ADDRESS    0x13b20824UL


/***********************************************************
* Register Name : EGRESS_POISON_STATUS_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_0_OFFSET      0
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_0_MASK        0x1

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_1_OFFSET      1
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_1_MASK        0x2

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_2_OFFSET      2
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_2_MASK        0x4

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_3_OFFSET      3
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_3_MASK        0x8

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_4_OFFSET      4
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_4_MASK        0x10

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_5_OFFSET      5
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_5_MASK        0x20

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_6_OFFSET      6
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_6_MASK        0x40

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_7_OFFSET      7
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_7_MASK        0x80

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_8_OFFSET      8
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_8_MASK        0x100

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_9_OFFSET      9
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_9_MASK        0x200

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_10_OFFSET      10
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_10_MASK        0x400

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_11_OFFSET      11
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_11_MASK        0x800

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_12_OFFSET      12
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_12_MASK        0x1000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_13_OFFSET      13
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_13_MASK        0x2000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_14_OFFSET      14
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_14_MASK        0x4000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_15_OFFSET      15
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_15_MASK        0x8000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_16_OFFSET      16
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_16_MASK        0x10000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_17_OFFSET      17
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_17_MASK        0x20000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_18_OFFSET      18
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_18_MASK        0x40000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_19_OFFSET      19
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_19_MASK        0x80000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_20_OFFSET      20
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_20_MASK        0x100000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_21_OFFSET      21
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_21_MASK        0x200000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_22_OFFSET      22
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_22_MASK        0x400000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_23_OFFSET      23
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_23_MASK        0x800000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_24_OFFSET      24
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_24_MASK        0x1000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_25_OFFSET      25
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_25_MASK        0x2000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_26_OFFSET      26
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_26_MASK        0x4000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_27_OFFSET      27
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_27_MASK        0x8000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_28_OFFSET      28
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_28_MASK        0x10000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_29_OFFSET      29
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_29_MASK        0x20000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_30_OFFSET      30
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_30_MASK        0x40000000

// Bitfield Description : Each bit is associated with a logical IOHC Egress port such as a PCIe RC and indicates whether the destination port transmitted poisoned write or response data.
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_31_OFFSET      31
#define EGRESS_POISON_STATUS_LO_EgressPoisonStatusLo_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            EgressPoisonStatusLo_0:1;
    UINT32                            EgressPoisonStatusLo_1:1;
    UINT32                            EgressPoisonStatusLo_2:1;
    UINT32                            EgressPoisonStatusLo_3:1;
    UINT32                            EgressPoisonStatusLo_4:1;
    UINT32                            EgressPoisonStatusLo_5:1;
    UINT32                            EgressPoisonStatusLo_6:1;
    UINT32                            EgressPoisonStatusLo_7:1;
    UINT32                            EgressPoisonStatusLo_8:1;
    UINT32                            EgressPoisonStatusLo_9:1;
    UINT32                            EgressPoisonStatusLo_10:1;
    UINT32                            EgressPoisonStatusLo_11:1;
    UINT32                            EgressPoisonStatusLo_12:1;
    UINT32                            EgressPoisonStatusLo_13:1;
    UINT32                            EgressPoisonStatusLo_14:1;
    UINT32                            EgressPoisonStatusLo_15:1;
    UINT32                            EgressPoisonStatusLo_16:1;
    UINT32                            EgressPoisonStatusLo_17:1;
    UINT32                            EgressPoisonStatusLo_18:1;
    UINT32                            EgressPoisonStatusLo_19:1;
    UINT32                            EgressPoisonStatusLo_20:1;
    UINT32                            EgressPoisonStatusLo_21:1;
    UINT32                            EgressPoisonStatusLo_22:1;
    UINT32                            EgressPoisonStatusLo_23:1;
    UINT32                            EgressPoisonStatusLo_24:1;
    UINT32                            EgressPoisonStatusLo_25:1;
    UINT32                            EgressPoisonStatusLo_26:1;
    UINT32                            EgressPoisonStatusLo_27:1;
    UINT32                            EgressPoisonStatusLo_28:1;
    UINT32                            EgressPoisonStatusLo_29:1;
    UINT32                            EgressPoisonStatusLo_30:1;
    UINT32                            EgressPoisonStatusLo_31:1;
  } Field;
  UINT32 Value;
} EGRESS_POISON_STATUS_LO_STRUCT;

#define SMN_EGRESS_POISON_STATUS_LO_ADDRESS    0x13b20820UL


/***********************************************************
* Register Name : ErrEvent_ACTION_CONTROL
* Register Description :
* Action controls when ErrEvent occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define ErrEvent_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define ErrEvent_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define ErrEvent_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define ErrEvent_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define ErrEvent_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define ErrEvent_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define ErrEvent_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define ErrEvent_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define ErrEvent_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define ErrEvent_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ErrEvent_ACTION_CONTROL_STRUCT;

#define SMN_ErrEvent_ACTION_CONTROL_ADDRESS    0x13b20100UL


/***********************************************************
* Register Name : FASTREGCNTL_BASE_ADDR_HI
* Register Description :
* FastRegsCntl MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : FastRegsCntl MMIO base address bits 47:32.
#define FASTREGCNTL_BASE_ADDR_HI_FASTREGCNTL_BASE_ADDR_HI_OFFSET      0
#define FASTREGCNTL_BASE_ADDR_HI_FASTREGCNTL_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define FASTREGCNTL_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define FASTREGCNTL_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            FASTREGCNTL_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} FASTREGCNTL_BASE_ADDR_HI_STRUCT;

#define SMN_FASTREGCNTL_BASE_ADDR_HI_ADDRESS    0x13b1030cUL


/***********************************************************
* Register Name : FASTREGCNTL_BASE_ADDR_LO
* Register Description :
* FastRegsCntl MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : FastRegsCntl MMIO enable.
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_MMIO_EN_OFFSET      0
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the FastRegsCntl MMIO address range and enable until the next warm reset.
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_MMIO_LOCK_OFFSET      1
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define FASTREGCNTL_BASE_ADDR_LO_Reserved_11_2_OFFSET      2
#define FASTREGCNTL_BASE_ADDR_LO_Reserved_11_2_MASK        0xffc

// Bitfield Description : FastRegsCntl MMIO base address bits 31:12.
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_BASE_ADDR_LO_OFFSET      12
#define FASTREGCNTL_BASE_ADDR_LO_FASTREGCNTL_BASE_ADDR_LO_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            FASTREGCNTL_MMIO_EN:1;
    UINT32                            FASTREGCNTL_MMIO_LOCK:1;
    UINT32                            Reserved_11_2:10;
    UINT32                            FASTREGCNTL_BASE_ADDR_LO:20;
  } Field;
  UINT32 Value;
} FASTREGCNTL_BASE_ADDR_LO_STRUCT;

#define SMN_FASTREGCNTL_BASE_ADDR_LO_ADDRESS    0x13b10308UL


/***********************************************************
* Register Name : FASTREG_APERTURE
* Register Description :
* FastRegs MMIO limit address
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMN ApertureID used when accessing the lower 1M of the FastRegs MMIO range.
#define FASTREG_APERTURE_FASTREG_APERTURE_ID_OFFSET      0
#define FASTREG_APERTURE_FASTREG_APERTURE_ID_MASK        0xfff

// Bitfield Description : 
#define FASTREG_APERTURE_Reserved_15_12_OFFSET      12
#define FASTREG_APERTURE_Reserved_15_12_MASK        0xf000

// Bitfield Description : SMN ApertureID extension used when accessing the lower 1M of the FastRegs MMIO range.
#define FASTREG_APERTURE_FASTREG_NODE_ID_OFFSET      16
#define FASTREG_APERTURE_FASTREG_NODE_ID_MASK        0xf0000

// Bitfield Description : 
#define FASTREG_APERTURE_Reserved_30_20_OFFSET      20
#define FASTREG_APERTURE_Reserved_30_20_MASK        0x7ff00000

// Bitfield Description : Set SMN transaction to be posted type when accessing the lower 1M of the FastRegs MMIO range.
#define FASTREG_APERTURE_FASTREG_TRAN_POSTED_OFFSET      31
#define FASTREG_APERTURE_FASTREG_TRAN_POSTED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            FASTREG_APERTURE_ID:12;
    UINT32                            Reserved_15_12:4;
    UINT32                            FASTREG_NODE_ID:4;
    UINT32                            Reserved_30_20:11;
    UINT32                            FASTREG_TRAN_POSTED:1;
  } Field;
  UINT32 Value;
} FASTREG_APERTURE_STRUCT;

#define SMN_FASTREG_APERTURE_ADDRESS    0x13b07000UL


/***********************************************************
* Register Name : FASTREG_BASE_ADDR_HI
* Register Description :
* FastRegs MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : FastRegs MMIO base address bits 47:32.
#define FASTREG_BASE_ADDR_HI_FASTREG_BASE_ADDR_HI_OFFSET      0
#define FASTREG_BASE_ADDR_HI_FASTREG_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define FASTREG_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define FASTREG_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            FASTREG_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} FASTREG_BASE_ADDR_HI_STRUCT;

#define SMN_FASTREG_BASE_ADDR_HI_ADDRESS    0x13b10304UL


/***********************************************************
* Register Name : FASTREG_BASE_ADDR_LO
* Register Description :
* FastRegs MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : FastRegs MMIO enable.
#define FASTREG_BASE_ADDR_LO_FASTREG_MMIO_EN_OFFSET      0
#define FASTREG_BASE_ADDR_LO_FASTREG_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the FastRegs MMIO address and enable until the next warm reset.
#define FASTREG_BASE_ADDR_LO_FASTREG_MMIO_LOCK_OFFSET      1
#define FASTREG_BASE_ADDR_LO_FASTREG_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define FASTREG_BASE_ADDR_LO_Reserved_19_2_OFFSET      2
#define FASTREG_BASE_ADDR_LO_Reserved_19_2_MASK        0xffffc

// Bitfield Description : FastRegs MMIO base address bits 31:20.
#define FASTREG_BASE_ADDR_LO_FASTREG_BASE_ADDR_LO_OFFSET      20
#define FASTREG_BASE_ADDR_LO_FASTREG_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            FASTREG_MMIO_EN:1;
    UINT32                            FASTREG_MMIO_LOCK:1;
    UINT32                            Reserved_19_2:18;
    UINT32                            FASTREG_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} FASTREG_BASE_ADDR_LO_STRUCT;

#define SMN_FASTREG_BASE_ADDR_LO_ADDRESS    0x13b10300UL


/***********************************************************
* Register Name : HEADER_TYPE
* Register Description :
* Header Type Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define HEADER_TYPE_Reserved_15_0_OFFSET      0
#define HEADER_TYPE_Reserved_15_0_MASK        0xffff

// Bitfield Description : Indicating that Type 00 Configuration Space Header format is supported.
#define HEADER_TYPE_HEADER_TYPE_OFFSET      16
#define HEADER_TYPE_HEADER_TYPE_MASK        0x7f0000

// Bitfield Description : Indicating that the north bridge block is a multi-function device.
#define HEADER_TYPE_DEVICE_TYPE_OFFSET      23
#define HEADER_TYPE_DEVICE_TYPE_MASK        0x800000

// Bitfield Description : 
#define HEADER_TYPE_Reserved_31_24_OFFSET      24
#define HEADER_TYPE_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            HEADER_TYPE:7;
    UINT32                            DEVICE_TYPE:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} HEADER_TYPE_IOHC_STRUCT;

#define PCICFG_NBIO_IOHUB_HEADER_TYPE_DEVICE       8
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_FUNC         0
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_OFFSET       0xc
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0xc)

#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_OFFSET       0xc
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0xc)

#define SMN_PCIEHB0_HEADER_TYPE_ADDRESS    0x13b4100cUL

#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_OFFSET       0xc
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0xc)

#define SMN_PCIEHB1_HEADER_TYPE_ADDRESS    0x13b4200cUL

#define SMN_PCIEHB2_HEADER_TYPE_ADDRESS    0x13b4300cUL

#define SMN_PCIEHB3_HEADER_TYPE_ADDRESS    0x13b4400cUL

#define SMN_PCIEHB4_HEADER_TYPE_ADDRESS    0x13b4800cUL


/***********************************************************
* Register Name : HEADER_TYPE_W
* Register Description :
* Header Type Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define HEADER_TYPE_W_Reserved_6_0_OFFSET      0
#define HEADER_TYPE_W_Reserved_6_0_MASK        0x7f

// Bitfield Description : This field sets the value in the corresponding field in IOHC::HEADER_TYPE bit 23.
#define HEADER_TYPE_W_DEVICE_TYPE_OFFSET      7
#define HEADER_TYPE_W_DEVICE_TYPE_MASK        0x80

// Bitfield Description : 
#define HEADER_TYPE_W_Reserved_31_8_OFFSET      8
#define HEADER_TYPE_W_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            DEVICE_TYPE:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} HEADER_TYPE_W_STRUCT;

#define PCICFG_NBIO_IOHUB_HEADER_TYPE_W_DEVICE       8
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_W_FUNC         0
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_W_OFFSET       0x40
#define PCICFG_NBIO_IOHUB_HEADER_TYPE_W_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0x40)

#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_W_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_W_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_W_OFFSET       0x40
#define PCICFG_NBIO_HB0_IOHUB_HEADER_TYPE_W_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0x40)

#define SMN_PCIEHB0_HEADER_TYPE_W_ADDRESS    0x13b41040UL

#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_W_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_W_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_W_OFFSET       0x40
#define PCICFG_NBIO_HB1_IOHUB_HEADER_TYPE_W_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0x40)

#define SMN_PCIEHB1_HEADER_TYPE_W_ADDRESS    0x13b42040UL

#define SMN_PCIEHB2_HEADER_TYPE_W_ADDRESS    0x13b43040UL

#define SMN_PCIEHB3_HEADER_TYPE_W_ADDRESS    0x13b44040UL

#define SMN_PCIEHB4_HEADER_TYPE_W_ADDRESS    0x13b48040UL


/***********************************************************
* Register Name : HSP_BASE_ADDR_HI
* Register Description :
* HSP private MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : HSP private MMIO base address bits 47:32.
#define HSP_BASE_ADDR_HI_HSP_BASE_ADDR_HI_OFFSET      0
#define HSP_BASE_ADDR_HI_HSP_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define HSP_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define HSP_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HSP_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} HSP_BASE_ADDR_HI_STRUCT;

#define SMN_HSP_BASE_ADDR_HI_ADDRESS    0x13b102d4UL


/***********************************************************
* Register Name : HSP_BASE_ADDR_LO
* Register Description :
* HSP MMIO base address
* Visibility : 0x2
************************************************************/

// Bitfield Description : HSP private MMIO enable.
#define HSP_BASE_ADDR_LO_HSP_MMIO_EN_OFFSET      0
#define HSP_BASE_ADDR_LO_HSP_MMIO_EN_MASK        0x1

// Bitfield Description : 
#define HSP_BASE_ADDR_LO_Reserved_7_1_OFFSET      1
#define HSP_BASE_ADDR_LO_Reserved_7_1_MASK        0xfe

// Bitfield Description : Locks the HSP private MMIO address range and enable until the next warm reset.
#define HSP_BASE_ADDR_LO_HSP_MMIO_LOCK_OFFSET      8
#define HSP_BASE_ADDR_LO_HSP_MMIO_LOCK_MASK        0x100

// Bitfield Description : 
#define HSP_BASE_ADDR_LO_Reserved_19_9_OFFSET      9
#define HSP_BASE_ADDR_LO_Reserved_19_9_MASK        0xffe00

// Bitfield Description : HSP private MMIO base address bits 31:20.
#define HSP_BASE_ADDR_LO_HSP_BASE_ADDR_LO_OFFSET      20
#define HSP_BASE_ADDR_LO_HSP_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            HSP_MMIO_EN:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            HSP_MMIO_LOCK:1;
    UINT32                            Reserved_19_9:11;
    UINT32                            HSP_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} HSP_BASE_ADDR_LO_STRUCT;

#define SMN_HSP_BASE_ADDR_LO_ADDRESS    0x13b102d0UL


/***********************************************************
* Register Name : INTERNAL_POISON_MASK
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable masking of errors logged in INTERNAL_POISON_STATUS. For each bit, 1=Errors are masked 0=Errors trigger response actions.
#define INTERNAL_POISON_MASK_IntPoisonMask_OFFSET      0
#define INTERNAL_POISON_MASK_IntPoisonMask_MASK        0xff

// Bitfield Description : 
#define INTERNAL_POISON_MASK_Reserved_31_8_OFFSET      8
#define INTERNAL_POISON_MASK_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            IntPoisonMask:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} INTERNAL_POISON_MASK_STRUCT;

#define SMN_INTERNAL_POISON_MASK_ADDRESS    0x13b2081cUL


/***********************************************************
* Register Name : INTERNAL_POISON_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit indicates the status of IOHC consuming poisoned data in the SMN Master.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_0_OFFSET      0
#define INTERNAL_POISON_STATUS_IntPoisonStatus_0_MASK        0x1

// Bitfield Description : This bit indicates the status of IOHC consuming poisoned data in the Trap block.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_1_OFFSET      1
#define INTERNAL_POISON_STATUS_IntPoisonStatus_1_MASK        0x2

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_2_OFFSET      2
#define INTERNAL_POISON_STATUS_IntPoisonStatus_2_MASK        0x4

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_3_OFFSET      3
#define INTERNAL_POISON_STATUS_IntPoisonStatus_3_MASK        0x8

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_4_OFFSET      4
#define INTERNAL_POISON_STATUS_IntPoisonStatus_4_MASK        0x10

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_5_OFFSET      5
#define INTERNAL_POISON_STATUS_IntPoisonStatus_5_MASK        0x20

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_6_OFFSET      6
#define INTERNAL_POISON_STATUS_IntPoisonStatus_6_MASK        0x40

// Bitfield Description : Reserved.
#define INTERNAL_POISON_STATUS_IntPoisonStatus_7_OFFSET      7
#define INTERNAL_POISON_STATUS_IntPoisonStatus_7_MASK        0x80

// Bitfield Description : 
#define INTERNAL_POISON_STATUS_Reserved_31_8_OFFSET      8
#define INTERNAL_POISON_STATUS_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            IntPoisonStatus_0:1;
    UINT32                            IntPoisonStatus_1:1;
    UINT32                            IntPoisonStatus_2:1;
    UINT32                            IntPoisonStatus_3:1;
    UINT32                            IntPoisonStatus_4:1;
    UINT32                            IntPoisonStatus_5:1;
    UINT32                            IntPoisonStatus_6:1;
    UINT32                            IntPoisonStatus_7:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} INTERNAL_POISON_STATUS_STRUCT;

#define SMN_INTERNAL_POISON_STATUS_ADDRESS    0x13b20818UL


/***********************************************************
* Register Name : IOAPIC_BASE_ADDR_HI
* Register Description :
* IOAPIC MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : IOAPIC private MMIO base address bits 47:32.
#define IOAPIC_BASE_ADDR_HI_IOAPIC_BASE_ADDR_HI_OFFSET      0
#define IOAPIC_BASE_ADDR_HI_IOAPIC_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define IOAPIC_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define IOAPIC_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            IOAPIC_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} IOAPIC_BASE_ADDR_HI_STRUCT;

#define SMN_IOAPIC_BASE_ADDR_HI_ADDRESS    0x13b102f4UL


/***********************************************************
* Register Name : IOAPIC_BASE_ADDR_LO
* Register Description :
* IOAPIC MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : IOAPIC private MMIO enable.
#define IOAPIC_BASE_ADDR_LO_IOAPIC_MMIO_EN_OFFSET      0
#define IOAPIC_BASE_ADDR_LO_IOAPIC_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the IOAPIC private MMIO address and enable until the next warm reset.
#define IOAPIC_BASE_ADDR_LO_IOAPIC_MMIO_LOCK_OFFSET      1
#define IOAPIC_BASE_ADDR_LO_IOAPIC_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define IOAPIC_BASE_ADDR_LO_Reserved_7_2_OFFSET      2
#define IOAPIC_BASE_ADDR_LO_Reserved_7_2_MASK        0xfc

// Bitfield Description : IOAPIC private MMIO base address bits 31:8.
#define IOAPIC_BASE_ADDR_LO_IOAPIC_BASE_ADDR_LO_OFFSET      8
#define IOAPIC_BASE_ADDR_LO_IOAPIC_BASE_ADDR_LO_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            IOAPIC_MMIO_EN:1;
    UINT32                            IOAPIC_MMIO_LOCK:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            IOAPIC_BASE_ADDR_LO:24;
  } Field;
  UINT32 Value;
} IOAPIC_BASE_ADDR_LO_STRUCT;

#define SMN_IOAPIC_BASE_ADDR_LO_ADDRESS    0x13b102f0UL


/***********************************************************
* Register Name : IOHC_AER_CNTL
* Register Description :
* IOHC Advanced Error Reporting Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_AER_CNTL_Reserved_0_0_OFFSET      0
#define IOHC_AER_CNTL_Reserved_0_0_MASK        0x1

// Bitfield Description : When AER is enabled, log all errors in AER. This may result in double-logging of some error types. When set, responses from SDF to PCIE/NBIF have the AERlog bit set.
#define IOHC_AER_CNTL_CFG_IOHC_AER_COMPLIANCE_EN_OFFSET      1
#define IOHC_AER_CNTL_CFG_IOHC_AER_COMPLIANCE_EN_MASK        0x2

// Bitfield Description : 
#define IOHC_AER_CNTL_Reserved_31_2_OFFSET      2
#define IOHC_AER_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            CFG_IOHC_AER_COMPLIANCE_EN:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} IOHC_AER_CNTL_STRUCT;

#define SMN_IOHC_AER_CNTL_ADDRESS    0x13b10058UL


/***********************************************************
* Register Name : IOHC_Bridge_CNTL
* Register Description :
* IOHC Bridge control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field sets to hide the bridge.
// No accesses are allowed to go to the bridge when this bit is set.
#define IOHC_Bridge_CNTL_BridgeDis_OFFSET      0
#define IOHC_Bridge_CNTL_BridgeDis_MASK        0x1

// Bitfield Description : (Debug bit) This field sets to disable the bridge's ability to operate as a master.
// It overrides the Bus Master Enable bit in the bridge when this bit is set.
#define IOHC_Bridge_CNTL_BusMasterDis_OFFSET      1
#define IOHC_Bridge_CNTL_BusMasterDis_MASK        0x2

// Bitfield Description : This field sets to disable onboard/internal configuration access to the bridge.
// Non-SB bridges are not expected to set this bit.
#define IOHC_Bridge_CNTL_CfgDis_OFFSET      2
#define IOHC_Bridge_CNTL_CfgDis_MASK        0x4

// Bitfield Description : (Debug bit) This field sets to disable local peer-to-peer transactions forward to the bridge.
#define IOHC_Bridge_CNTL_P2pDis_OFFSET      3
#define IOHC_Bridge_CNTL_P2pDis_MASK        0x8

// Bitfield Description : 
#define IOHC_Bridge_CNTL_Reserved_4_4_OFFSET      4
#define IOHC_Bridge_CNTL_Reserved_4_4_MASK        0x10

// Bitfield Description : (Debug bit) This field sets to disable VDM accesses to the bridge.
#define IOHC_Bridge_CNTL_VDMDis_OFFSET      5
#define IOHC_Bridge_CNTL_VDMDis_MASK        0x20

// Bitfield Description : The field sets to enable masking a host completion UR status to Success status when return to CPU.
#define IOHC_Bridge_CNTL_MaskUR_Enable_OFFSET      6
#define IOHC_Bridge_CNTL_MaskUR_Enable_MASK        0x40

// Bitfield Description : Disable PassPW/RelaxedOrdering on DMA requests.
#define IOHC_Bridge_CNTL_PassPWDis_OFFSET      7
#define IOHC_Bridge_CNTL_PassPWDis_MASK        0x80

// Bitfield Description : Disable non-coherent DMA requests. These are converted into their equivalent coherent form/command.
#define IOHC_Bridge_CNTL_NoSnoopDis_OFFSET      8
#define IOHC_Bridge_CNTL_NoSnoopDis_MASK        0x100

// Bitfield Description : Force RspPassPW=1 on DMA non-posted requests.
#define IOHC_Bridge_CNTL_ForceRspPassPw_OFFSET      9
#define IOHC_Bridge_CNTL_ForceRspPassPw_MASK        0x200

// Bitfield Description : Override IDO ordering:
// 00 - IDO attributes are passed from the client without modification
// 01 - IDO is disabled. ReqBlockLevel is downgraded from Stream to Unit
// 10 - IDO forced on. ReqBlockLevel is upgraded from Unit to Stream
// 11 - Reserved.
#define IOHC_Bridge_CNTL_IDOMode_OFFSET      10
#define IOHC_Bridge_CNTL_IDOMode_MASK        0xc00

// Bitfield Description : 
#define IOHC_Bridge_CNTL_Reserved_15_12_OFFSET      12
#define IOHC_Bridge_CNTL_Reserved_15_12_MASK        0xf000

// Bitfield Description : (Debug bit) This field sets to indicate to iohc that an external device are being plugged on the bridge.
#define IOHC_Bridge_CNTL_ExtDevPlug_OFFSET      16
#define IOHC_Bridge_CNTL_ExtDevPlug_MASK        0x10000

// Bitfield Description : (Debug bit) This field sets to reset the bridge CRS counter upon external device plug-in or link down.
#define IOHC_Bridge_CNTL_ExtDevCrsEn_OFFSET      17
#define IOHC_Bridge_CNTL_ExtDevCrsEn_MASK        0x20000

// Bitfield Description : This field sets to enable the hardware retry on receiving configuration request retry status.
#define IOHC_Bridge_CNTL_CrsEnable_OFFSET      18
#define IOHC_Bridge_CNTL_CrsEnable_MASK        0x40000

// Bitfield Description : 
#define IOHC_Bridge_CNTL_Reserved_22_19_OFFSET      19
#define IOHC_Bridge_CNTL_Reserved_22_19_MASK        0x780000

// Bitfield Description : This field sets to enable the bridge APIC range decoding. Requests fall in the bridge APIC range if addr[39:12]={20'h00_FEC,APIC_Range[7:0]}.
#define IOHC_Bridge_CNTL_APIC_Enable_OFFSET      23
#define IOHC_Bridge_CNTL_APIC_Enable_MASK        0x800000

// Bitfield Description : This field sets the bits 19:12 of the bridge APIC range.
#define IOHC_Bridge_CNTL_APIC_Range_OFFSET      24
#define IOHC_Bridge_CNTL_APIC_Range_MASK        0xff000000

typedef union {
  struct {
    UINT32                            BridgeDis:1;
    UINT32                            BusMasterDis:1;
    UINT32                            CfgDis:1;
    UINT32                            P2pDis:1;
    UINT32                            Reserved_4_4:1;
    UINT32                            VDMDis:1;
    UINT32                            MaskUR_Enable:1;
    UINT32                            PassPWDis:1;
    UINT32                            NoSnoopDis:1;
    UINT32                            ForceRspPassPw:1;
    UINT32                            IDOMode:2;
    UINT32                            Reserved_15_12:4;
    UINT32                            ExtDevPlug:1;
    UINT32                            ExtDevCrsEn:1;
    UINT32                            CrsEnable:1;
    UINT32                            Reserved_22_19:4;
    UINT32                            APIC_Enable:1;
    UINT32                            APIC_Range:8;
  } Field;
  UINT32 Value;
} IOHC_Bridge_CNTL_STRUCT;

#define SMN_INTSBDEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b3c004UL

#define SMN_NBIF1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b38004UL

#define SMN_NBIF1DEVINDCFG1_IOHC_Bridge_CNTL_ADDRESS    0x13b38404UL

#define SMN_NBIF1DEVINDCFG2_IOHC_Bridge_CNTL_ADDRESS    0x13b38804UL

#define SMN_PCIE0DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b31004UL

#define SMN_PCIE0DEVINDCFG1_IOHC_Bridge_CNTL_ADDRESS    0x13b31404UL

#define SMN_PCIE0DEVINDCFG2_IOHC_Bridge_CNTL_ADDRESS    0x13b31804UL

#define SMN_PCIE0DEVINDCFG3_IOHC_Bridge_CNTL_ADDRESS    0x13b31c04UL

#define SMN_PCIE0DEVINDCFG4_IOHC_Bridge_CNTL_ADDRESS    0x13b32004UL

#define SMN_PCIE1DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b33004UL

#define SMN_PCIE1DEVINDCFG1_IOHC_Bridge_CNTL_ADDRESS    0x13b33404UL

#define SMN_PCIE1DEVINDCFG2_IOHC_Bridge_CNTL_ADDRESS    0x13b33804UL

#define SMN_PCIE1DEVINDCFG3_IOHC_Bridge_CNTL_ADDRESS    0x13b33c04UL

#define SMN_PCIE1DEVINDCFG4_IOHC_Bridge_CNTL_ADDRESS    0x13b34004UL

#define SMN_PCIE1DEVINDCFG5_IOHC_Bridge_CNTL_ADDRESS    0x13b34404UL

#define SMN_PCIE2DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b35004UL

#define SMN_PCIE3DEVINDCFG0_IOHC_Bridge_CNTL_ADDRESS    0x13b35404UL


/***********************************************************
* Register Name : IOHC_Bridge_SCRATCH_0
* Register Description :
* Scratch register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
#define IOHC_Bridge_SCRATCH_0_SCRATCH_0_OFFSET      0
#define IOHC_Bridge_SCRATCH_0_SCRATCH_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_0:32;
  } Field;
  UINT32 Value;
} IOHC_Bridge_SCRATCH_0_STRUCT;

#define SMN_INTSBDEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b3c038UL

#define SMN_NBIF1DEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b38038UL

#define SMN_NBIF1DEVINDCFG1_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b38438UL

#define SMN_NBIF1DEVINDCFG2_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b38838UL

#define SMN_PCIE0DEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b31038UL

#define SMN_PCIE0DEVINDCFG1_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b31438UL

#define SMN_PCIE0DEVINDCFG2_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b31838UL

#define SMN_PCIE0DEVINDCFG3_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b31c38UL

#define SMN_PCIE0DEVINDCFG4_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b32038UL

#define SMN_PCIE1DEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b33038UL

#define SMN_PCIE1DEVINDCFG1_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b33438UL

#define SMN_PCIE1DEVINDCFG2_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b33838UL

#define SMN_PCIE1DEVINDCFG3_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b33c38UL

#define SMN_PCIE1DEVINDCFG4_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b34038UL

#define SMN_PCIE1DEVINDCFG5_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b34438UL

#define SMN_PCIE2DEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b35038UL

#define SMN_PCIE3DEVINDCFG0_IOHC_Bridge_SCRATCH_0_ADDRESS    0x13b35438UL


/***********************************************************
* Register Name : IOHC_Bridge_SCRATCH_1
* Register Description :
* Scratch register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
#define IOHC_Bridge_SCRATCH_1_SCRATCH_1_OFFSET      0
#define IOHC_Bridge_SCRATCH_1_SCRATCH_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_1:32;
  } Field;
  UINT32 Value;
} IOHC_Bridge_SCRATCH_1_STRUCT;

#define SMN_INTSBDEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3c03cUL

#define SMN_NBIF1DEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3803cUL

#define SMN_NBIF1DEVINDCFG1_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3843cUL

#define SMN_NBIF1DEVINDCFG2_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3883cUL

#define SMN_PCIE0DEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3103cUL

#define SMN_PCIE0DEVINDCFG1_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3143cUL

#define SMN_PCIE0DEVINDCFG2_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3183cUL

#define SMN_PCIE0DEVINDCFG3_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b31c3cUL

#define SMN_PCIE0DEVINDCFG4_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3203cUL

#define SMN_PCIE1DEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3303cUL

#define SMN_PCIE1DEVINDCFG1_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3343cUL

#define SMN_PCIE1DEVINDCFG2_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3383cUL

#define SMN_PCIE1DEVINDCFG3_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b33c3cUL

#define SMN_PCIE1DEVINDCFG4_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3403cUL

#define SMN_PCIE1DEVINDCFG5_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3443cUL

#define SMN_PCIE2DEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3503cUL

#define SMN_PCIE3DEVINDCFG0_IOHC_Bridge_SCRATCH_1_ADDRESS    0x13b3543cUL


/***********************************************************
* Register Name : IOHC_Bridge_STATUS
* Register Description :
* Status register
* Visibility : 0x2
************************************************************/

// Bitfield Description : <RW1C> Indicates a host completion with UR status has been masked.
#define IOHC_Bridge_STATUS_MaskUR_Status_OFFSET      0
#define IOHC_Bridge_STATUS_MaskUR_Status_MASK        0x1

// Bitfield Description : 
#define IOHC_Bridge_STATUS_Reserved_31_1_OFFSET      1
#define IOHC_Bridge_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            MaskUR_Status:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOHC_Bridge_STATUS_STRUCT;

#define SMN_INTSBDEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b3c008UL

#define SMN_NBIF1DEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b38008UL

#define SMN_NBIF1DEVINDCFG1_IOHC_Bridge_STATUS_ADDRESS    0x13b38408UL

#define SMN_NBIF1DEVINDCFG2_IOHC_Bridge_STATUS_ADDRESS    0x13b38808UL

#define SMN_PCIE0DEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b31008UL

#define SMN_PCIE0DEVINDCFG1_IOHC_Bridge_STATUS_ADDRESS    0x13b31408UL

#define SMN_PCIE0DEVINDCFG2_IOHC_Bridge_STATUS_ADDRESS    0x13b31808UL

#define SMN_PCIE0DEVINDCFG3_IOHC_Bridge_STATUS_ADDRESS    0x13b31c08UL

#define SMN_PCIE0DEVINDCFG4_IOHC_Bridge_STATUS_ADDRESS    0x13b32008UL

#define SMN_PCIE1DEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b33008UL

#define SMN_PCIE1DEVINDCFG1_IOHC_Bridge_STATUS_ADDRESS    0x13b33408UL

#define SMN_PCIE1DEVINDCFG2_IOHC_Bridge_STATUS_ADDRESS    0x13b33808UL

#define SMN_PCIE1DEVINDCFG3_IOHC_Bridge_STATUS_ADDRESS    0x13b33c08UL

#define SMN_PCIE1DEVINDCFG4_IOHC_Bridge_STATUS_ADDRESS    0x13b34008UL

#define SMN_PCIE1DEVINDCFG5_IOHC_Bridge_STATUS_ADDRESS    0x13b34408UL

#define SMN_PCIE2DEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b35008UL

#define SMN_PCIE3DEVINDCFG0_IOHC_Bridge_STATUS_ADDRESS    0x13b35408UL


/***********************************************************
* Register Name : IOHC_EARLY_WAKE_UP_EN
* Register Description :
* sdp port control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit enables early OrigClkReq for a upstream client, bit 0: SDF, bit 1: NCM
#define IOHC_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_OFFSET      0
#define IOHC_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_MASK        0x3
#define IOHC_EARLY_WAKE_UP_EN_DMAEnableEarlyClkReq_DEFAULT     0x1

// Bitfield Description : 
#define IOHC_EARLY_WAKE_UP_EN_Reserved_15_2_OFFSET      2
#define IOHC_EARLY_WAKE_UP_EN_Reserved_15_2_MASK        0xfffc

// Bitfield Description : These bits enables early OrigClkReq for a downstream client, and multiple bits can be set.
#define IOHC_EARLY_WAKE_UP_EN_HostEnableEarlyClkReq_OFFSET      16
#define IOHC_EARLY_WAKE_UP_EN_HostEnableEarlyClkReq_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            DMAEnableEarlyClkReq:2;
    UINT32                            Reserved_15_2:14;
    UINT32                            HostEnableEarlyClkReq:16;
  } Field;
  UINT32 Value;
} IOHC_EARLY_WAKE_UP_EN_STRUCT;

#define SMN_IOHC_EARLY_WAKE_UP_EN_ADDRESS    0x13b10348UL


/***********************************************************
* Register Name : IOHC_FEATURE_CNTL
* Register Description :
* Control bits for misc IOHC features.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicate message-generating bridge VDM ID in the ReqID bit[7:4] of the HP or PMPME message to the SB.
#define IOHC_FEATURE_CNTL_HpPmpme_DevID_En_OFFSET      0
#define IOHC_FEATURE_CNTL_HpPmpme_DevID_En_MASK        0x1

// Bitfield Description : Peer to peer mode:
// 00 -  Mode 0 : DMA writes not matching a valid P2P range are dropped (Legacy operation)
// 01 - Mode 1 : DMA writes not matching a valid P2P range are forwarded to the DF
// 10 - Mode 2 : all DMA writes are forwarded to the DF
// 11 - Mode 3: Disable P2P
#define IOHC_FEATURE_CNTL_P2P_mode_OFFSET      1
#define IOHC_FEATURE_CNTL_P2P_mode_MASK        0x6

// Bitfield Description : Indicated the working architecture mode of IOHC. 0=x86 Mode 1=ARM Mode.
// Used for SMI/SCI/NMI message generation.
#define IOHC_FEATURE_CNTL_IOHC_ARCH_MODE_OFFSET      3
#define IOHC_FEATURE_CNTL_IOHC_ARCH_MODE_MASK        0x8

// Bitfield Description : 
#define IOHC_FEATURE_CNTL_Reserved_21_4_OFFSET      4
#define IOHC_FEATURE_CNTL_Reserved_21_4_MASK        0x3ffff0

// Bitfield Description : Enables the per-port ARI_EN signals from the IOHC shadow registers. This is used for decoding external configuration requests to the bridges.
#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET      22
#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK        0x400000

// Bitfield Description : 
#define IOHC_FEATURE_CNTL_Reserved_27_23_OFFSET      23
#define IOHC_FEATURE_CNTL_Reserved_27_23_MASK        0xf800000

// Bitfield Description : Indicates the routing mode of the IOHC: 0=APU/CPU Mode 1=dGPU/PCIE-Switch Mode.
#define IOHC_FEATURE_CNTL_IOHC_dGPU_MODE_OFFSET      28
#define IOHC_FEATURE_CNTL_IOHC_dGPU_MODE_MASK        0x10000000

// Bitfield Description : Spare bits - Reserved for future use.
#define IOHC_FEATURE_CNTL_MISC_FEATURE_CNTL_OFFSET      29
#define IOHC_FEATURE_CNTL_MISC_FEATURE_CNTL_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            HpPmpme_DevID_En:1;
    UINT32                            P2P_mode:2;
    UINT32                            IOHC_ARCH_MODE:1;
    UINT32                            Reserved_21_4:18;
    UINT32                            IOHC_ARI_SUPPORTED:1;
    UINT32                            Reserved_27_23:5;
    UINT32                            IOHC_dGPU_MODE:1;
    UINT32                            MISC_FEATURE_CNTL:3;
  } Field;
  UINT32 Value;
} IOHC_FEATURE_CNTL_STRUCT;

#define SMN_IOHC_FEATURE_CNTL_ADDRESS    0x13b10118UL


/***********************************************************
* Register Name : IOHC_FEATURE_CNTL2
* Register Description :
* Control bits for misc IOHC features.
* Visibility : 0x2
************************************************************/

// Bitfield Description : <RW1C> Indicates an NMI interrupt has been sent to SDF.
#define IOHC_FEATURE_CNTL2_NMI_status_OFFSET      0
#define IOHC_FEATURE_CNTL2_NMI_status_MASK        0x1

// Bitfield Description : Indicates IOHUB generated a system error.
#define IOHC_FEATURE_CNTL2_SErr_status_OFFSET      1
#define IOHC_FEATURE_CNTL2_SErr_status_MASK        0x2

// Bitfield Description : 
#define IOHC_FEATURE_CNTL2_Reserved_15_2_OFFSET      2
#define IOHC_FEATURE_CNTL2_Reserved_15_2_MASK        0xfffc

// Bitfield Description : <RW1C> Status indicating a Configuration Retry Status is detected.
#define IOHC_FEATURE_CNTL2_CrsStatus_OFFSET      16
#define IOHC_FEATURE_CNTL2_CrsStatus_MASK        0x10000

// Bitfield Description : <RW1C> DMA posted request dropped status.
// Set when a posted DMA is dropped inside IOHC.
#define IOHC_FEATURE_CNTL2_P_DMA_DROPPED_OFFSET      17
#define IOHC_FEATURE_CNTL2_P_DMA_DROPPED_MASK        0x20000

// Bitfield Description : <RW1C> DMA non-posted request dropped status.
// Set when a non-posted DMA is dropped in IOHC.
#define IOHC_FEATURE_CNTL2_NP_DMA_DROPPED_OFFSET      18
#define IOHC_FEATURE_CNTL2_NP_DMA_DROPPED_MASK        0x40000

// Bitfield Description : 
#define IOHC_FEATURE_CNTL2_Reserved_31_19_OFFSET      19
#define IOHC_FEATURE_CNTL2_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            NMI_status:1;
    UINT32                            SErr_status:1;
    UINT32                            Reserved_15_2:14;
    UINT32                            CrsStatus:1;
    UINT32                            P_DMA_DROPPED:1;
    UINT32                            NP_DMA_DROPPED:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} IOHC_FEATURE_CNTL2_STRUCT;

#define SMN_IOHC_FEATURE_CNTL2_ADDRESS    0x13b10130UL


/***********************************************************
* Register Name : IOHC_GLUE_CG_LCLK_CTRL_0
* Register Description :
* IHOC LCLK Clock Gating Control 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_OFFSET      0
#define IOHC_GLUE_CG_LCLK_CTRL_0_Reserved_3_0_MASK        0xf

// Bitfield Description : Hysteresis for local clock gating to filter out small turn off changes.
#define IOHC_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_OFFSET      4
#define IOHC_GLUE_CG_LCLK_CTRL_0_CG_OFF_HYSTERESIS_MASK        0xff0

// Bitfield Description : 
#define IOHC_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_OFFSET      12
#define IOHC_GLUE_CG_LCLK_CTRL_0_Reserved_21_12_MASK        0x3ff000

// Bitfield Description : Disable dynamic clock gating for IOHCi2p_INT.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for SION_s2p_int_CompRsp.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for SION_s2p_int_OrigReq.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for SION_L2_OrigReq.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for SION_IOAGR_CompRsp.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for SION_IOAGR_OrigReq.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for SION_PCIE0_CompRsp.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for SION_PCIE0_OrigReq.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for SION_SDF_CompRsp.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for SION_SDF_OrigReq.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK0_MASK        0x80000000

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
} IOHC_GLUE_CG_LCLK_CTRL_0_STRUCT;

#define SMN_IOHC_GLUE_CG_LCLK_CTRL_0_ADDRESS    0x13b10088UL


/***********************************************************
* Register Name : IOHC_GLUE_CG_LCLK_CTRL_1
* Register Description :
* IHOC LCLK Clock Gating Control 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_GLUE_CG_LCLK_CTRL_1_Reserved_21_0_OFFSET      0
#define IOHC_GLUE_CG_LCLK_CTRL_1_Reserved_21_0_MASK        0x3fffff

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for IOHC_SION_Perf.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for IOHC_Perf.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for IOHC_RAS.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for IOHC_CfgSlv.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for SION_Credit.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for SION_SeqArb_Rsp.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for SION_SeqArb_Req.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK        0x80000000

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
} IOHC_GLUE_CG_LCLK_CTRL_1_STRUCT;

#define SMN_IOHC_GLUE_CG_LCLK_CTRL_1_ADDRESS    0x13b1008cUL


/***********************************************************
* Register Name : IOHC_GLUE_CG_LCLK_CTRL_2
* Register Description :
* IHOC LCLK Clock Gating Control 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_GLUE_CG_LCLK_CTRL_2_Reserved_21_0_OFFSET      0
#define IOHC_GLUE_CG_LCLK_CTRL_2_Reserved_21_0_MASK        0x3fffff

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK9_OFFSET      22
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK9_MASK        0x400000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK8_OFFSET      23
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK8_MASK        0x800000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK7_OFFSET      24
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK7_MASK        0x1000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK6_OFFSET      25
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK6_MASK        0x2000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK5_OFFSET      26
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK5_MASK        0x4000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK4_OFFSET      27
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK4_MASK        0x8000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK3_OFFSET      28
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK3_MASK        0x10000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK2_OFFSET      29
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK2_MASK        0x20000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK1_OFFSET      30
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK1_MASK        0x40000000

// Bitfield Description : Disable dynamic clock gating for unused branch.
// 0 - Enable Clock Gating.
// 1 - Disable Clock Gating.
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK0_OFFSET      31
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK0_MASK        0x80000000

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
} IOHC_GLUE_CG_LCLK_CTRL_2_STRUCT;

#define SMN_IOHC_GLUE_CG_LCLK_CTRL_2_ADDRESS    0x13b10090UL


/***********************************************************
* Register Name : IOHC_INTERRUPT_EOI
* Register Description :
* End of Interrupt Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicate interrupt handler exited the SMI interrupt routine.
#define IOHC_INTERRUPT_EOI_SMI_EOI_OFFSET      0
#define IOHC_INTERRUPT_EOI_SMI_EOI_MASK        0x1

// Bitfield Description : Indicate interrupt handler exited the SCI interrupt routine.
#define IOHC_INTERRUPT_EOI_SCI_EOI_OFFSET      1
#define IOHC_INTERRUPT_EOI_SCI_EOI_MASK        0x2

// Bitfield Description : Indicate interrupt handler exited the NMI interrupt routine.
#define IOHC_INTERRUPT_EOI_NMI_EOI_OFFSET      2
#define IOHC_INTERRUPT_EOI_NMI_EOI_MASK        0x4

// Bitfield Description : 
#define IOHC_INTERRUPT_EOI_Reserved_31_3_OFFSET      3
#define IOHC_INTERRUPT_EOI_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            SMI_EOI:1;
    UINT32                            SCI_EOI:1;
    UINT32                            NMI_EOI:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} IOHC_INTERRUPT_EOI_STRUCT;

#define SMN_IOHC_INTERRUPT_EOI_ADDRESS    0x13b10120UL


/***********************************************************
* Register Name : IOHC_INTR_CNTL
* Register Description :
* IOHC Interrupt Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_INTR_CNTL_Reserved_7_0_OFFSET      0
#define IOHC_INTR_CNTL_Reserved_7_0_MASK        0xff

// Bitfield Description : Program the destination field for all NMI interrupts generated by IOHC.
#define IOHC_INTR_CNTL_NMI_DEST_ctrl_OFFSET      8
#define IOHC_INTR_CNTL_NMI_DEST_ctrl_MASK        0xff00

// Bitfield Description : 
#define IOHC_INTR_CNTL_Reserved_31_16_OFFSET      16
#define IOHC_INTR_CNTL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_7_0:8;
    UINT32                            NMI_DEST_ctrl:8;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} IOHC_INTR_CNTL_STRUCT;

#define SMN_IOHC_INTR_CNTL_ADDRESS    0x13b1012cUL


/***********************************************************
* Register Name : IOHC_NCM_QOS_CONTROL
* Register Description :
* set QoSPriority per VC for DMA requests sent to the NCM
* Visibility : 0x2
************************************************************/

// Bitfield Description : VC0NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC0NCMQoSPriority_OFFSET      0
#define IOHC_NCM_QOS_CONTROL_VC0NCMQoSPriority_MASK        0xf

// Bitfield Description : VC1NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC1NCMQoSPriority_OFFSET      4
#define IOHC_NCM_QOS_CONTROL_VC1NCMQoSPriority_MASK        0xf0

// Bitfield Description : VC2NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC2NCMQoSPriority_OFFSET      8
#define IOHC_NCM_QOS_CONTROL_VC2NCMQoSPriority_MASK        0xf00

// Bitfield Description : VC3NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC3NCMQoSPriority_OFFSET      12
#define IOHC_NCM_QOS_CONTROL_VC3NCMQoSPriority_MASK        0xf000

// Bitfield Description : VC4NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC4NCMQoSPriority_OFFSET      16
#define IOHC_NCM_QOS_CONTROL_VC4NCMQoSPriority_MASK        0xf0000

// Bitfield Description : VC5NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC5NCMQoSPriority_OFFSET      20
#define IOHC_NCM_QOS_CONTROL_VC5NCMQoSPriority_MASK        0xf00000

// Bitfield Description : VC6NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC6NCMQoSPriority_OFFSET      24
#define IOHC_NCM_QOS_CONTROL_VC6NCMQoSPriority_MASK        0xf000000

// Bitfield Description : VC7NCMQoSPriority.
#define IOHC_NCM_QOS_CONTROL_VC7NCMQoSPriority_OFFSET      28
#define IOHC_NCM_QOS_CONTROL_VC7NCMQoSPriority_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0NCMQoSPriority:4;
    UINT32                            VC1NCMQoSPriority:4;
    UINT32                            VC2NCMQoSPriority:4;
    UINT32                            VC3NCMQoSPriority:4;
    UINT32                            VC4NCMQoSPriority:4;
    UINT32                            VC5NCMQoSPriority:4;
    UINT32                            VC6NCMQoSPriority:4;
    UINT32                            VC7NCMQoSPriority:4;
  } Field;
  UINT32 Value;
} IOHC_NCM_QOS_CONTROL_STRUCT;

#define SMN_IOHC_NCM_QOS_CONTROL_ADDRESS    0x13b14048UL


/***********************************************************
* Register Name : IOHC_P2P_CNTL
* Register Description :
* IOHC Bridge Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOHC_P2P_CNTL_Reserved_10_0_OFFSET      0
#define IOHC_P2P_CNTL_Reserved_10_0_MASK        0x7ff

// Bitfield Description : Reset on DLDown.
// 1=DLDown reset all IOHC PCIE/NBIF shadowed configuration registers.
#define IOHC_P2P_CNTL_DLDownResetEn_OFFSET      11
#define IOHC_P2P_CNTL_DLDownResetEn_MASK        0x800

// Bitfield Description : 
#define IOHC_P2P_CNTL_Reserved_31_12_OFFSET      12
#define IOHC_P2P_CNTL_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            Reserved_10_0:11;
    UINT32                            DLDownResetEn:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} IOHC_P2P_CNTL_STRUCT;

#define SMN_IOHC_P2P_CNTL_ADDRESS    0x13b10030UL


/***********************************************************
* Register Name : IOHC_PCIE_CRS_Count
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the length of time which IOHC waits between resending the configuration cycle.
// Values are specified in 1.6us (1600ns) granularity.
#define IOHC_PCIE_CRS_Count_CrsDelayCount_OFFSET      0
#define IOHC_PCIE_CRS_Count_CrsDelayCount_MASK        0xffff
#define IOHC_PCIE_CRS_Count_CrsDelayCount_DEFAULT     0x6

// Bitfield Description : Sets the length of time which IOHC can issue retries for the same configuration cycle.
// Values are specified in ~1.6ms(1638400ns) granularity.
#define IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET      16
#define IOHC_PCIE_CRS_Count_CrsLimitCount_MASK        0xfff0000

// Bitfield Description : 
#define IOHC_PCIE_CRS_Count_Reserved_31_28_OFFSET      28
#define IOHC_PCIE_CRS_Count_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            CrsDelayCount:16;
    UINT32                            CrsLimitCount:12;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} IOHC_PCIE_CRS_Count_STRUCT;

#define SMN_IOHC_PCIE_CRS_Count_ADDRESS    0x13b10028UL


/***********************************************************
* Register Name : IOHC_PERF_CNTL
* Register Description :
* IOHC Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define IOHC_PERF_CNTL_EVENT0_SEL_OFFSET      0
#define IOHC_PERF_CNTL_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define IOHC_PERF_CNTL_EVENT1_SEL_OFFSET      8
#define IOHC_PERF_CNTL_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define IOHC_PERF_CNTL_EVENT2_SEL_OFFSET      16
#define IOHC_PERF_CNTL_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define IOHC_PERF_CNTL_EVENT3_SEL_OFFSET      24
#define IOHC_PERF_CNTL_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            EVENT0_SEL:8;
    UINT32                            EVENT1_SEL:8;
    UINT32                            EVENT2_SEL:8;
    UINT32                            EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} IOHC_PERF_CNTL_STRUCT;

#define SMN_IOHC_PERF_CNTL_ADDRESS    0x13b10094UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT0
* Register Description :
* IOHC Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define IOHC_PERF_COUNT0_COUNTER0_OFFSET      0
#define IOHC_PERF_COUNT0_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER0:32;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT0_STRUCT;

#define SMN_IOHC_PERF_COUNT0_ADDRESS    0x13b10098UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT0_UPPER
* Register Description :
* IOHC Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define IOHC_PERF_COUNT0_UPPER_COUNTER0_UPPER_OFFSET      0
#define IOHC_PERF_COUNT0_UPPER_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOHC_PERF_COUNT0_UPPER_ADDRESS    0x13b1009cUL


/***********************************************************
* Register Name : IOHC_PERF_COUNT1
* Register Description :
* IOHC Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define IOHC_PERF_COUNT1_COUNTER1_OFFSET      0
#define IOHC_PERF_COUNT1_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER1:32;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT1_STRUCT;

#define SMN_IOHC_PERF_COUNT1_ADDRESS    0x13b100a0UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT1_UPPER
* Register Description :
* IOHC Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define IOHC_PERF_COUNT1_UPPER_COUNTER1_UPPER_OFFSET      0
#define IOHC_PERF_COUNT1_UPPER_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOHC_PERF_COUNT1_UPPER_ADDRESS    0x13b100a4UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT2
* Register Description :
* IOHC Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define IOHC_PERF_COUNT2_COUNTER2_OFFSET      0
#define IOHC_PERF_COUNT2_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER2:32;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT2_STRUCT;

#define SMN_IOHC_PERF_COUNT2_ADDRESS    0x13b100a8UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT2_UPPER
* Register Description :
* IOHC Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define IOHC_PERF_COUNT2_UPPER_COUNTER2_UPPER_OFFSET      0
#define IOHC_PERF_COUNT2_UPPER_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOHC_PERF_COUNT2_UPPER_ADDRESS    0x13b100acUL


/***********************************************************
* Register Name : IOHC_PERF_COUNT3
* Register Description :
* IOHC Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define IOHC_PERF_COUNT3_COUNTER3_OFFSET      0
#define IOHC_PERF_COUNT3_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            COUNTER3:32;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT3_STRUCT;

#define SMN_IOHC_PERF_COUNT3_ADDRESS    0x13b100b0UL


/***********************************************************
* Register Name : IOHC_PERF_COUNT3_UPPER
* Register Description :
* IOHC Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define IOHC_PERF_COUNT3_UPPER_COUNTER3_UPPER_OFFSET      0
#define IOHC_PERF_COUNT3_UPPER_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOHC_PERF_COUNT3_UPPER_ADDRESS    0x13b100b4UL


/***********************************************************
* Register Name : IOHC_PGMST_CNTL
* Register Description :
* power gate control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : controls the number of consecutive periods when all PG IPs assert PGMST_PGSLV_pwrGate_ready before initiating power gating. Each period is 1600ns duration
#define IOHC_PGMST_CNTL_CFG_PG_HYSTERESIS_OFFSET      0
#define IOHC_PGMST_CNTL_CFG_PG_HYSTERESIS_MASK        0xff

// Bitfield Description : enables dynamic power gating
#define IOHC_PGMST_CNTL_CFG_PG_EN_OFFSET      8
#define IOHC_PGMST_CNTL_CFG_PG_EN_MASK        0x100

// Bitfield Description : 
#define IOHC_PGMST_CNTL_Reserved_9_9_OFFSET      9
#define IOHC_PGMST_CNTL_Reserved_9_9_MASK        0x200

// Bitfield Description : 0: idleness_counter is disabled, PGMST uses only hysteresis timeout value to enter power gating. Cfg_idleness_count_en > 0: If the internal idleness counter >= Cfg_idleness_count_en, enter power gated state as soon PGSLV becomes idle, without waiting for the hysteresis timeout.
#define IOHC_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_OFFSET      10
#define IOHC_PGMST_CNTL_CFG_IDLENESS_COUNT_EN_MASK        0x3c00

// Bitfield Description : 2'x1: enable sending the pwrmgr_int for FW interlock feature, 2'b1x: Enable wakup tile from power gated state using the pwrmgt_int_clear signal
#define IOHC_PGMST_CNTL_CFG_FW_PG_EXIT_EN_OFFSET      14
#define IOHC_PGMST_CNTL_CFG_FW_PG_EXIT_EN_MASK        0xc000

// Bitfield Description : 
#define IOHC_PGMST_CNTL_Reserved_31_16_OFFSET      16
#define IOHC_PGMST_CNTL_Reserved_31_16_MASK        0xffff0000

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
} IOHC_PGMST_CNTL_STRUCT;

#define SMN_IOHC_PGMST_CNTL_ADDRESS    0x13b10340UL


/***********************************************************
* Register Name : IOHC_PGSLV_CNTL
* Register Description :
* power gate control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis timer to enter power gating. The counter decrements every 200ns
#define IOHC_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_OFFSET      0
#define IOHC_PGSLV_CNTL_CFG_IDLE_HYSTERESIS_MASK        0x1f

// Bitfield Description : 
#define IOHC_PGSLV_CNTL_Reserved_31_5_OFFSET      5
#define IOHC_PGSLV_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            CFG_IDLE_HYSTERESIS:5;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} IOHC_PGSLV_CNTL_STRUCT;

#define SMN_IOHC_PGSLV_CNTL_ADDRESS    0x13b10350UL


/***********************************************************
* Register Name : IOHC_PIN_CNTL
* Register Description :
* IOHC Pin control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Select the mode of the NMI_Syncflood Combo Pin(LPC_SMI_LNMI_SYNC_FLOOD_L/AGPIO86).
//  0 - Pin as SYNCFLOOD_L
//  1 - Pin as NMI#.
#define IOHC_PIN_CNTL_NMI_SYNCFLOOD_PIN_MODE_OFFSET      0
#define IOHC_PIN_CNTL_NMI_SYNCFLOOD_PIN_MODE_MASK        0x1

// Bitfield Description : 
#define IOHC_PIN_CNTL_Reserved_31_1_OFFSET      1
#define IOHC_PIN_CNTL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            NMI_SYNCFLOOD_PIN_MODE:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOHC_PIN_CNTL_STRUCT;

#define SMN_IOHC_PIN_CNTL_ADDRESS    0x13b10128UL


/***********************************************************
* Register Name : IOHC_QOS_CONTROL
* Register Description :
* Set QoSPriority per VC for DMA requests sent to the SDF.
* Visibility : 0x2
************************************************************/

// Bitfield Description : VC0QoSPriority.
#define IOHC_QOS_CONTROL_VC0QoSPriority_OFFSET      0
#define IOHC_QOS_CONTROL_VC0QoSPriority_MASK        0xf

// Bitfield Description : VC1QoSPriority.
#define IOHC_QOS_CONTROL_VC1QoSPriority_OFFSET      4
#define IOHC_QOS_CONTROL_VC1QoSPriority_MASK        0xf0

// Bitfield Description : VC2QoSPriority.
#define IOHC_QOS_CONTROL_VC2QoSPriority_OFFSET      8
#define IOHC_QOS_CONTROL_VC2QoSPriority_MASK        0xf00

// Bitfield Description : VC3QoSPriority.
#define IOHC_QOS_CONTROL_VC3QoSPriority_OFFSET      12
#define IOHC_QOS_CONTROL_VC3QoSPriority_MASK        0xf000

// Bitfield Description : VC4QoSPriority.
#define IOHC_QOS_CONTROL_VC4QoSPriority_OFFSET      16
#define IOHC_QOS_CONTROL_VC4QoSPriority_MASK        0xf0000

// Bitfield Description : VC5QoSPriority.
#define IOHC_QOS_CONTROL_VC5QoSPriority_OFFSET      20
#define IOHC_QOS_CONTROL_VC5QoSPriority_MASK        0xf00000

// Bitfield Description : VC6QoSPriority.
#define IOHC_QOS_CONTROL_VC6QoSPriority_OFFSET      24
#define IOHC_QOS_CONTROL_VC6QoSPriority_MASK        0xf000000

// Bitfield Description : VC7QoSPriority.
#define IOHC_QOS_CONTROL_VC7QoSPriority_OFFSET      28
#define IOHC_QOS_CONTROL_VC7QoSPriority_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VC0QoSPriority:4;
    UINT32                            VC1QoSPriority:4;
    UINT32                            VC2QoSPriority:4;
    UINT32                            VC3QoSPriority:4;
    UINT32                            VC4QoSPriority:4;
    UINT32                            VC5QoSPriority:4;
    UINT32                            VC6QoSPriority:4;
    UINT32                            VC7QoSPriority:4;
  } Field;
  UINT32 Value;
} IOHC_QOS_CONTROL_STRUCT;

#define SMN_IOHC_QOS_CONTROL_ADDRESS    0x13b14040UL


/***********************************************************
* Register Name : IOHC_QOS_CONTROL2
* Register Description :
* programmed UnitID is seen being sent to DF, when we set to the QoSPriority to the programmed value
* Visibility : 0x2
************************************************************/

// Bitfield Description : UnitID2 to set QoSPriority
#define IOHC_QOS_CONTROL2_UnitID2_OFFSET      0
#define IOHC_QOS_CONTROL2_UnitID2_MASK        0x7f

// Bitfield Description : 
#define IOHC_QOS_CONTROL2_Reserved_7_7_OFFSET      7
#define IOHC_QOS_CONTROL2_Reserved_7_7_MASK        0x80

// Bitfield Description : QoSPriority to set for UnitID2
#define IOHC_QOS_CONTROL2_UnitID2QoSPriority_OFFSET      8
#define IOHC_QOS_CONTROL2_UnitID2QoSPriority_MASK        0xf00

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID2
#define IOHC_QOS_CONTROL2_UnitID2Enable_OFFSET      12
#define IOHC_QOS_CONTROL2_UnitID2Enable_MASK        0x1000

// Bitfield Description : 
#define IOHC_QOS_CONTROL2_Reserved_15_13_OFFSET      13
#define IOHC_QOS_CONTROL2_Reserved_15_13_MASK        0xe000

// Bitfield Description : UnitID3 to set QoSPriority
#define IOHC_QOS_CONTROL2_UnitID3_OFFSET      16
#define IOHC_QOS_CONTROL2_UnitID3_MASK        0x7f0000

// Bitfield Description : 
#define IOHC_QOS_CONTROL2_Reserved_23_23_OFFSET      23
#define IOHC_QOS_CONTROL2_Reserved_23_23_MASK        0x800000

// Bitfield Description : QoSPriority to set for UnitID3
#define IOHC_QOS_CONTROL2_UnitID3QoSPriority_OFFSET      24
#define IOHC_QOS_CONTROL2_UnitID3QoSPriority_MASK        0xf000000

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID3
#define IOHC_QOS_CONTROL2_UnitID3Enable_OFFSET      28
#define IOHC_QOS_CONTROL2_UnitID3Enable_MASK        0x10000000

// Bitfield Description : 
#define IOHC_QOS_CONTROL2_Reserved_31_29_OFFSET      29
#define IOHC_QOS_CONTROL2_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            UnitID2:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            UnitID2QoSPriority:4;
    UINT32                            UnitID2Enable:1;
    UINT32                            Reserved_15_13:3;
    UINT32                            UnitID3:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            UnitID3QoSPriority:4;
    UINT32                            UnitID3Enable:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} IOHC_QOS_CONTROL2_STRUCT;

#define SMN_IOHC_QOS_CONTROL2_ADDRESS    0x13b1404cUL


/***********************************************************
* Register Name : IOHC_QOS_CONTROL3
* Register Description :
* programmed UnitID is seen being sent to DF, when we set to the QoSPriority to the programmed value
* Visibility : 0x2
************************************************************/

// Bitfield Description : UnitID4 to set QoSPriority
#define IOHC_QOS_CONTROL3_UnitID4_OFFSET      0
#define IOHC_QOS_CONTROL3_UnitID4_MASK        0x7f

// Bitfield Description : 
#define IOHC_QOS_CONTROL3_Reserved_7_7_OFFSET      7
#define IOHC_QOS_CONTROL3_Reserved_7_7_MASK        0x80

// Bitfield Description : QoSPriority to set for UnitID4
#define IOHC_QOS_CONTROL3_UnitID4QoSPriority_OFFSET      8
#define IOHC_QOS_CONTROL3_UnitID4QoSPriority_MASK        0xf00

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID4
#define IOHC_QOS_CONTROL3_UnitID4Enable_OFFSET      12
#define IOHC_QOS_CONTROL3_UnitID4Enable_MASK        0x1000

// Bitfield Description : 
#define IOHC_QOS_CONTROL3_Reserved_15_13_OFFSET      13
#define IOHC_QOS_CONTROL3_Reserved_15_13_MASK        0xe000

// Bitfield Description : UnitID5 to set QoSPriority
#define IOHC_QOS_CONTROL3_UnitID5_OFFSET      16
#define IOHC_QOS_CONTROL3_UnitID5_MASK        0x7f0000

// Bitfield Description : 
#define IOHC_QOS_CONTROL3_Reserved_23_23_OFFSET      23
#define IOHC_QOS_CONTROL3_Reserved_23_23_MASK        0x800000

// Bitfield Description : QoSPriority to set for UnitID5
#define IOHC_QOS_CONTROL3_UnitID5QoSPriority_OFFSET      24
#define IOHC_QOS_CONTROL3_UnitID5QoSPriority_MASK        0xf000000

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID5
#define IOHC_QOS_CONTROL3_UnitID5Enable_OFFSET      28
#define IOHC_QOS_CONTROL3_UnitID5Enable_MASK        0x10000000

// Bitfield Description : 
#define IOHC_QOS_CONTROL3_Reserved_31_29_OFFSET      29
#define IOHC_QOS_CONTROL3_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            UnitID4:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            UnitID4QoSPriority:4;
    UINT32                            UnitID4Enable:1;
    UINT32                            Reserved_15_13:3;
    UINT32                            UnitID5:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            UnitID5QoSPriority:4;
    UINT32                            UnitID5Enable:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} IOHC_QOS_CONTROL3_STRUCT;

#define SMN_IOHC_QOS_CONTROL3_ADDRESS    0x13b14050UL


/***********************************************************
* Register Name : IOHC_REFCLK_MODE
* Register Description :
* REFCLK frequency mode
* Visibility : 0x2
************************************************************/

// Bitfield Description : REFCLK is running at 100 MHZ
#define IOHC_REFCLK_MODE_MODE_100MHZ_OFFSET      0
#define IOHC_REFCLK_MODE_MODE_100MHZ_MASK        0x1

// Bitfield Description : REFCLK is running at 25 MHZ
#define IOHC_REFCLK_MODE_MODE_25MHZ_OFFSET      1
#define IOHC_REFCLK_MODE_MODE_25MHZ_MASK        0x2

// Bitfield Description : REFCLK is running at 27 MHZ
#define IOHC_REFCLK_MODE_MODE_27MHZ_OFFSET      2
#define IOHC_REFCLK_MODE_MODE_27MHZ_MASK        0x4

// Bitfield Description : 
#define IOHC_REFCLK_MODE_Reserved_31_3_OFFSET      3
#define IOHC_REFCLK_MODE_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            MODE_100MHZ:1;
    UINT32                            MODE_25MHZ:1;
    UINT32                            MODE_27MHZ:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} IOHC_REFCLK_MODE_STRUCT;

#define SMN_IOHC_REFCLK_MODE_ADDRESS    0x13b10020UL


/***********************************************************
* Register Name : IOHC_RSMU_HCID
* Register Description :
* rsmu hardware revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu harware revision.
#define IOHC_RSMU_HCID_RSMU_HCID_HwRev_OFFSET      0
#define IOHC_RSMU_HCID_RSMU_HCID_HwRev_MASK        0x3f

// Bitfield Description : 
#define IOHC_RSMU_HCID_Reserved_7_6_OFFSET      6
#define IOHC_RSMU_HCID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu harware minor revision.
#define IOHC_RSMU_HCID_RSMU_HCID_HwMinVer_OFFSET      8
#define IOHC_RSMU_HCID_RSMU_HCID_HwMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOHC_RSMU_HCID_Reserved_15_15_OFFSET      15
#define IOHC_RSMU_HCID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu harware major revision.
#define IOHC_RSMU_HCID_RSMU_HCID_HwMajVer_OFFSET      16
#define IOHC_RSMU_HCID_RSMU_HCID_HwMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOHC_RSMU_HCID_Reserved_31_23_OFFSET      23
#define IOHC_RSMU_HCID_Reserved_31_23_MASK        0xff800000

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
} IOHC_RSMU_HCID_STRUCT;

#define SMN_IOHC_RSMU_HCID_ADDRESS    0x13b10354UL


/***********************************************************
* Register Name : IOHC_RSMU_SIID
* Register Description :
* rsmu software revision register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : rsmu software revision.
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfRev_OFFSET      0
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfRev_MASK        0x3f

// Bitfield Description : 
#define IOHC_RSMU_SIID_Reserved_7_6_OFFSET      6
#define IOHC_RSMU_SIID_Reserved_7_6_MASK        0xc0

// Bitfield Description : rsmu software minor revision.
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfMinVer_OFFSET      8
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfMinVer_MASK        0x7f00

// Bitfield Description : 
#define IOHC_RSMU_SIID_Reserved_15_15_OFFSET      15
#define IOHC_RSMU_SIID_Reserved_15_15_MASK        0x8000

// Bitfield Description : rsmu software major revision.
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfMajVer_OFFSET      16
#define IOHC_RSMU_SIID_RSMU_SIID_SwIfMajVer_MASK        0x7f0000

// Bitfield Description : 
#define IOHC_RSMU_SIID_Reserved_31_23_OFFSET      23
#define IOHC_RSMU_SIID_Reserved_31_23_MASK        0xff800000

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
} IOHC_RSMU_SIID_STRUCT;

#define SMN_IOHC_RSMU_SIID_ADDRESS    0x13b10358UL


/***********************************************************
* Register Name : IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of DataPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_STRUCT;

#define SMN_IOHC_Reorder_SION_DataPoolCredit_Alloc_Lower_ADDRESS    0x13b140d0UL


/***********************************************************
* Register Name : IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of DataPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_STRUCT;

#define SMN_IOHC_Reorder_SION_DataPoolCredit_Alloc_Upper_ADDRESS    0x13b140d4UL


/***********************************************************
* Register Name : IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower
* Register Description :
* Lower 32-bit field of ReqPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_STRUCT;

#define SMN_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Lower_ADDRESS    0x13b140c8UL


/***********************************************************
* Register Name : IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper
* Register Description :
* Upper 32-bit field of ReqPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_STRUCT;

#define SMN_IOHC_Reorder_SION_ReqPoolCredit_Alloc_Upper_ADDRESS    0x13b140ccUL


/***********************************************************
* Register Name : IOHC_SDP_PARITY_CONTROL
* Register Description :
* sdp port control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable parity error check for IOHC internally consumed data.
#define IOHC_SDP_PARITY_CONTROL_SDP_ParityDis_OFFSET      0
#define IOHC_SDP_PARITY_CONTROL_SDP_ParityDis_MASK        0x1

// Bitfield Description : 
#define IOHC_SDP_PARITY_CONTROL_Reserved_31_1_OFFSET      1
#define IOHC_SDP_PARITY_CONTROL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            SDP_ParityDis:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOHC_SDP_PARITY_CONTROL_STRUCT;

#define SMN_IOHC_SDP_PARITY_CONTROL_ADDRESS    0x13b1034cUL


/***********************************************************
* Register Name : IOHC_SDP_PORT_CONTROL
* Register Description :
* sdp port control register
* Visibility : 0x2
************************************************************/

// Bitfield Description : <RW> Hysteresis timer to deassert SDP ClkReq when block is idle.
#define IOHC_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_OFFSET      0
#define IOHC_SDP_PORT_CONTROL_Port_Disconnect_Hysteresis_MASK        0xff

// Bitfield Description : Hysteresis timer to deassert SDP ClkReq to IOM when block is idle. Count in real time(every 200ns).
#define IOHC_SDP_PORT_CONTROL_IOM_Port_Disconnect_Real_Time_Hysteresis_OFFSET      8
#define IOHC_SDP_PORT_CONTROL_IOM_Port_Disconnect_Real_Time_Hysteresis_MASK        0xff00

// Bitfield Description : Hysteresis timer to deassert SDP ClkReq to IOS when block is idle.
#define IOHC_SDP_PORT_CONTROL_IOS_Port_Disconnect_Real_Time_Hysteresis_OFFSET      16
#define IOHC_SDP_PORT_CONTROL_IOS_Port_Disconnect_Real_Time_Hysteresis_MASK        0xff0000

// Bitfield Description : 
#define IOHC_SDP_PORT_CONTROL_Reserved_31_24_OFFSET      24
#define IOHC_SDP_PORT_CONTROL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Port_Disconnect_Hysteresis:8;
    UINT32                            IOM_Port_Disconnect_Real_Time_Hysteresis:8;
    UINT32                            IOS_Port_Disconnect_Real_Time_Hysteresis:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SDP_PORT_CONTROL_STRUCT;

#define SMN_IOHC_SDP_PORT_CONTROL_ADDRESS    0x13b10344UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER
* Register Description :
* Lower 32-bit field of DataPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_DataPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_DataPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DataPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14468UL

#define SMN_N1_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1486cUL

#define SMN_N2_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14c70UL

#define SMN_N3_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15074UL

#define SMN_N4_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15478UL

#define SMN_N5_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1587cUL

#define SMN_N6_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15c80UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER
* Register Description :
* Upper 32-bit field of DataPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_DataPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_DataPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            DataPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b1446cUL

#define SMN_N1_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14870UL

#define SMN_N2_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14c74UL

#define SMN_N3_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15078UL

#define SMN_N4_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b1547cUL

#define SMN_N5_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15880UL

#define SMN_N6_IOHC_SION_CLIENT_DATAPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15c84UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER
* Register Description :
* Lower 32-bit field of RdRspPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_RdRspPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_RdRspPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRspPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14470UL

#define SMN_N1_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14874UL

#define SMN_N2_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14c78UL

#define SMN_N3_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1507cUL

#define SMN_N4_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15480UL

#define SMN_N5_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15884UL

#define SMN_N6_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15c88UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER
* Register Description :
* Upper 32-bit field of RdRspPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_RdRspPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_RdRspPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRspPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14474UL

#define SMN_N1_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14878UL

#define SMN_N2_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14c7cUL

#define SMN_N3_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15080UL

#define SMN_N4_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15484UL

#define SMN_N5_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15888UL

#define SMN_N6_IOHC_SION_CLIENT_RDRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15c8cUL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER
* Register Description :
* Lower 32-bit field of ReqPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ReqPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ReqPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ReqPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14460UL

#define SMN_N1_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14864UL

#define SMN_N2_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14c68UL

#define SMN_N3_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1506cUL

#define SMN_N4_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15470UL

#define SMN_N5_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15874UL

#define SMN_N6_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15c78UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER
* Register Description :
* Upper 32-bit field of ReqPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ReqPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ReqPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ReqPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14464UL

#define SMN_N1_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14868UL

#define SMN_N2_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14c6cUL

#define SMN_N3_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15070UL

#define SMN_N4_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15474UL

#define SMN_N5_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15878UL

#define SMN_N6_IOHC_SION_CLIENT_REQPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15c7cUL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER
* Register Description :
* Lower 32-bit field of WrRspPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC0, Bits 15:8 are for VC1, etc.
#define IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_WrRspPoolCredit_Alloc_Lower_OFFSET      0
#define IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_WrRspPoolCredit_Alloc_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRspPoolCredit_Alloc_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14478UL

#define SMN_N1_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1487cUL

#define SMN_N2_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b14c80UL

#define SMN_N3_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15084UL

#define SMN_N4_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15488UL

#define SMN_N5_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b1588cUL

#define SMN_N6_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_LOWER_ADDRESS    0x13b15c90UL


/***********************************************************
* Register Name : IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER
* Register Description :
* Upper 32-bit field of WrRspPoolCredit_Alloc.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of the Each 8-bits sets the number of pool credits to reserve for each VC, per given destination client. Appropriate values are required to prevent deadlock on interfaces that use pool credits.
// Bits 7:0 are for VC4, Bits 15:8 are for VC5, etc.
#define IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_WrRspPoolCredit_Alloc_Upper_OFFSET      0
#define IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_WrRspPoolCredit_Alloc_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRspPoolCredit_Alloc_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b1447cUL

#define SMN_N1_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14880UL

#define SMN_N2_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b14c84UL

#define SMN_N3_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15088UL

#define SMN_N4_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b1548cUL

#define SMN_N5_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15890UL

#define SMN_N6_IOHC_SION_CLIENT_WRRSPPOOLCREDIT_ALLOC_UPPER_ADDRESS    0x13b15c94UL


/***********************************************************
* Register Name : IOHC_SION_LiveLock_WatchDog_Threshold
* Register Description :
* arbitration Threshold.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold for number of arbitration losses a request/response has before its priority is increased above other requestors.  Used to prevent deadlocks.  A value greater than 0x10 should be given.
#define IOHC_SION_LiveLock_WatchDog_Threshold_IOHC_SION_LiveLock_WatchDog_Threshold_OFFSET      0
#define IOHC_SION_LiveLock_WatchDog_Threshold_IOHC_SION_LiveLock_WatchDog_Threshold_MASK        0xff

// Bitfield Description : 
#define IOHC_SION_LiveLock_WatchDog_Threshold_Reserved_31_8_OFFSET      8
#define IOHC_SION_LiveLock_WatchDog_Threshold_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            IOHC_SION_LiveLock_WatchDog_Threshold:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOHC_SION_LiveLock_WatchDog_Threshold_STRUCT;

#define SMN_IOHC_SION_LiveLock_WatchDog_Threshold_ADDRESS    0x13b15494UL


/***********************************************************
* Register Name : IOHC_SION_PERF_CNT_CNTL0
* Register Description :
* IOHC SION Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Performance Counters.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_CNT_EN_OFFSET      0
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_CNT_EN_MASK        0x1

// Bitfield Description : Write 1 to load the counter shadow registers. Read back value is always 0.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_WR_OFFSET      1
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_WR_MASK        0x2

// Bitfield Description : Write 1 for Global RESET of ALL counters. Read back value is always 0.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_OFFSET      2
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_MASK        0x4

// Bitfield Description : 
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_7_3_OFFSET      3
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_7_3_MASK        0xf8

// Bitfield Description : Programmable Pulse width for Global Shadow Write Toggle.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_DELAY_OFFSET      8
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_DELAY_MASK        0xf00

// Bitfield Description : 
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_14_12_OFFSET      12
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_14_12_MASK        0x7000

// Bitfield Description : Enables the Programmable Pulse with for Global Shadow Write Toggle.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_DELAY_EN_OFFSET      15
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_SHADOW_DELAY_EN_MASK        0x8000

// Bitfield Description : Programmable Pulse width for Global Perf Reset Toggle.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_DELAY_OFFSET      16
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_DELAY_MASK        0xf0000

// Bitfield Description : 
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_22_20_OFFSET      20
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_22_20_MASK        0x700000

// Bitfield Description : Enables the Programmable Pulse width for Global Perf Reset Toggle.
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_DELAY_EN_OFFSET      23
#define IOHC_SION_PERF_CNT_CNTL0_IOHC_SION_PERF_RESET_DELAY_EN_MASK        0x800000

// Bitfield Description : 
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_31_24_OFFSET      24
#define IOHC_SION_PERF_CNT_CNTL0_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_CNT_EN:1;
    UINT32                            IOHC_SION_SHADOW_WR:1;
    UINT32                            IOHC_SION_PERF_RESET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IOHC_SION_SHADOW_DELAY:4;
    UINT32                            Reserved_14_12:3;
    UINT32                            IOHC_SION_SHADOW_DELAY_EN:1;
    UINT32                            IOHC_SION_PERF_RESET_DELAY:4;
    UINT32                            Reserved_22_20:3;
    UINT32                            IOHC_SION_PERF_RESET_DELAY_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_CNT_CNTL0_STRUCT;

#define SMN_IOHC_SION_PERF_CNT_CNTL0_ADDRESS    0x13b15ca0UL


/***********************************************************
* Register Name : IOHC_SION_PERF_CNT_CNTL1
* Register Description :
* IOHC SION Performance Counter Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Counter 0 event select.
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT0_SEL_OFFSET      0
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT0_SEL_MASK        0xff

// Bitfield Description : Counter 1 event select.
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT1_SEL_OFFSET      8
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT1_SEL_MASK        0xff00

// Bitfield Description : Counter 2 event select.
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT2_SEL_OFFSET      16
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT2_SEL_MASK        0xff0000

// Bitfield Description : Counter 3 event select.
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT3_SEL_OFFSET      24
#define IOHC_SION_PERF_CNT_CNTL1_IOHC_SION_EVENT3_SEL_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_EVENT0_SEL:8;
    UINT32                            IOHC_SION_EVENT1_SEL:8;
    UINT32                            IOHC_SION_EVENT2_SEL:8;
    UINT32                            IOHC_SION_EVENT3_SEL:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_CNT_CNTL1_STRUCT;

#define SMN_IOHC_SION_PERF_CNT_CNTL1_ADDRESS    0x13b15ca4UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT0
* Register Description :
* IOHC SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 31:0.
#define IOHC_SION_PERF_COUNT0_IOHC_SION_COUNTER0_OFFSET      0
#define IOHC_SION_PERF_COUNT0_IOHC_SION_COUNTER0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER0:32;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT0_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT0_ADDRESS    0x13b15ca8UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT0_UPPER
* Register Description :
* IOHC SION Performance Counter 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 0 bit 55:32.
#define IOHC_SION_PERF_COUNT0_UPPER_IOHC_SION_COUNTER0_UPPER_OFFSET      0
#define IOHC_SION_PERF_COUNT0_UPPER_IOHC_SION_COUNTER0_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_SION_PERF_COUNT0_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_SION_PERF_COUNT0_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER0_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT0_UPPER_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT0_UPPER_ADDRESS    0x13b15cacUL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT1
* Register Description :
* IOHC SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 31:0.
#define IOHC_SION_PERF_COUNT1_IOHC_SION_COUNTER1_OFFSET      0
#define IOHC_SION_PERF_COUNT1_IOHC_SION_COUNTER1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER1:32;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT1_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT1_ADDRESS    0x13b15cb0UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT1_UPPER
* Register Description :
* IOHC SION Performance Counter 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 1 bit 55:32.
#define IOHC_SION_PERF_COUNT1_UPPER_IOHC_SION_COUNTER1_UPPER_OFFSET      0
#define IOHC_SION_PERF_COUNT1_UPPER_IOHC_SION_COUNTER1_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_SION_PERF_COUNT1_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_SION_PERF_COUNT1_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER1_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT1_UPPER_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT1_UPPER_ADDRESS    0x13b15cb4UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT2
* Register Description :
* IOHC SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 31:0.
#define IOHC_SION_PERF_COUNT2_IOHC_SION_COUNTER2_OFFSET      0
#define IOHC_SION_PERF_COUNT2_IOHC_SION_COUNTER2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER2:32;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT2_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT2_ADDRESS    0x13b15cb8UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT2_UPPER
* Register Description :
* IOHC SION Performance Counter 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 2 bit 55:32.
#define IOHC_SION_PERF_COUNT2_UPPER_IOHC_SION_COUNTER2_UPPER_OFFSET      0
#define IOHC_SION_PERF_COUNT2_UPPER_IOHC_SION_COUNTER2_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_SION_PERF_COUNT2_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_SION_PERF_COUNT2_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER2_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT2_UPPER_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT2_UPPER_ADDRESS    0x13b15cbcUL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT3
* Register Description :
* IOHC SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 33:0.
#define IOHC_SION_PERF_COUNT3_IOHC_SION_COUNTER3_OFFSET      0
#define IOHC_SION_PERF_COUNT3_IOHC_SION_COUNTER3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER3:32;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT3_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT3_ADDRESS    0x13b15cc0UL


/***********************************************************
* Register Name : IOHC_SION_PERF_COUNT3_UPPER
* Register Description :
* IOHC SION Performance Counter 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Read back value of Counter 3 bit 55:32.
#define IOHC_SION_PERF_COUNT3_UPPER_IOHC_SION_COUNTER3_UPPER_OFFSET      0
#define IOHC_SION_PERF_COUNT3_UPPER_IOHC_SION_COUNTER3_UPPER_MASK        0xffffff

// Bitfield Description : 
#define IOHC_SION_PERF_COUNT3_UPPER_Reserved_31_24_OFFSET      24
#define IOHC_SION_PERF_COUNT3_UPPER_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_COUNTER3_UPPER:24;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_PERF_COUNT3_UPPER_STRUCT;

#define SMN_IOHC_SION_PERF_COUNT3_UPPER_ADDRESS    0x13b15cc4UL


/***********************************************************
* Register Name : IOHC_SION_REQTHROTTLETHRESHOLD
* Register Description :
* SION ReqThrottleThreshold
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold for NP(VC0) read response throttle before stalling new requests from being issued. Once deficit has exceeded the threshold, new read request will be stalled until counter is below the threshold.
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_0_OFFSET      0
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_0_MASK        0xff

// Bitfield Description : Threshold for Deadlock-free(VC4) read response throttle before stalling new requests from being issued. Once deficit has exceeded the threshold, new read request will be stalled until counter is below the threshold.
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_1_OFFSET      8
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_1_MASK        0xff00

// Bitfield Description : Threshold for Real-time(VC5,6,7) read response throttle before stalling new requests from being issued. Once deficit has exceeded the threshold, new read request will be stalled until counter is below the threshold.
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_2_OFFSET      16
#define IOHC_SION_REQTHROTTLETHRESHOLD_IOHC_SION_RdReqThrottleThreshold_2_MASK        0xff0000

// Bitfield Description : 
#define IOHC_SION_REQTHROTTLETHRESHOLD_Reserved_31_24_OFFSET      24
#define IOHC_SION_REQTHROTTLETHRESHOLD_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOHC_SION_RdReqThrottleThreshold_0:8;
    UINT32                            IOHC_SION_RdReqThrottleThreshold_1:8;
    UINT32                            IOHC_SION_RdReqThrottleThreshold_2:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOHC_SION_REQTHROTTLETHRESHOLD_STRUCT;

#define SMN_IOHC_SION_REQTHROTTLETHRESHOLD_ADDRESS    0x13b140c4UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_RdRsp_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_RdRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14410UL

#define SMN_N1_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14814UL

#define SMN_N2_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c18UL

#define SMN_N3_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b1501cUL

#define SMN_N4_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15420UL

#define SMN_N5_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15824UL

#define SMN_N6_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15c28UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_RdRsp_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_RdRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14414UL

#define SMN_N1_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14818UL

#define SMN_N2_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14c1cUL

#define SMN_N3_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15020UL

#define SMN_N4_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15424UL

#define SMN_N5_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15828UL

#define SMN_N6_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15c2cUL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_RdRsp_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_RdRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b14418UL

#define SMN_N1_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b1481cUL

#define SMN_N2_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b14c20UL

#define SMN_N3_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15024UL

#define SMN_N4_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15428UL

#define SMN_N5_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b1582cUL

#define SMN_N6_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15c30UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_RdRsp_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_RdRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b1441cUL

#define SMN_N1_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b14820UL

#define SMN_N2_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b14c24UL

#define SMN_N3_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15028UL

#define SMN_N4_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b1542cUL

#define SMN_N5_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15830UL

#define SMN_N6_IOHC_SION_S0_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15c34UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_Req_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_Req_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14400UL

#define SMN_N1_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14804UL

#define SMN_N2_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14c08UL

#define SMN_N3_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b1500cUL

#define SMN_N4_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15410UL

#define SMN_N5_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15814UL

#define SMN_N6_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15c18UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_Req_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_Req_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14404UL

#define SMN_N1_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14808UL

#define SMN_N2_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14c0cUL

#define SMN_N3_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15010UL

#define SMN_N4_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15414UL

#define SMN_N5_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15818UL

#define SMN_N6_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15c1cUL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_Req_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_Req_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b14408UL

#define SMN_N1_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b1480cUL

#define SMN_N2_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b14c10UL

#define SMN_N3_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15014UL

#define SMN_N4_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15418UL

#define SMN_N5_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b1581cUL

#define SMN_N6_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15c20UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_Req_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_Req_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b1440cUL

#define SMN_N1_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b14810UL

#define SMN_N2_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b14c14UL

#define SMN_N3_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15018UL

#define SMN_N4_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b1541cUL

#define SMN_N5_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15820UL

#define SMN_N6_IOHC_SION_S0_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15c24UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_ReqThrottleVCEn
* Register Description :
* SION ReqThrottleVCEn
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable request throttling for each VC at STAGE0 arbiter for Client0. Once read response deficit has exceeded the threshold, new request will be stalled until counter is below the threshold.
// Bit 0 is for VC0, Bit 1 is for VC1, etc.
#define IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ReqThrottleVCEn_OFFSET      0
#define IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ReqThrottleVCEn_MASK        0xff

// Bitfield Description : 
#define IOHC_SION_S0_CLIENT_ReqThrottleVCEn_Reserved_31_8_OFFSET      8
#define IOHC_SION_S0_CLIENT_ReqThrottleVCEn_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            ReqThrottleVCEn:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_ReqThrottleVCEn_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b14484UL

#define SMN_N1_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b14884UL

#define SMN_N2_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b14c88UL

#define SMN_N3_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b1508cUL

#define SMN_N4_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b15490UL

#define SMN_N5_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b15894UL

#define SMN_N6_IOHC_SION_S0_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b15c98UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14420UL

#define SMN_N1_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14824UL

#define SMN_N2_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c28UL

#define SMN_N3_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b1502cUL

#define SMN_N4_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15430UL

#define SMN_N5_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15834UL

#define SMN_N6_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15c38UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per VC, per given source client.
// Bits 7:0 are for VC0, bits 15:8 are for VC1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_WrRsp_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_WrRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14424UL

#define SMN_N1_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14828UL

#define SMN_N2_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14c2cUL

#define SMN_N3_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15030UL

#define SMN_N4_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15434UL

#define SMN_N5_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15838UL

#define SMN_N6_IOHC_SION_S0_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15c3cUL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_WrRsp_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_WrRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b14428UL

#define SMN_N1_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b1482cUL

#define SMN_N2_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b14c30UL

#define SMN_N3_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15034UL

#define SMN_N4_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15438UL

#define SMN_N5_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b1583cUL

#define SMN_N6_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15c40UL


/***********************************************************
* Register Name : IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given source client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose VC0, value 2 means choose VC1, value 3 means choose VC2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_WrRsp_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_WrRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b1442cUL

#define SMN_N1_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b14830UL

#define SMN_N2_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b14c34UL

#define SMN_N3_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15038UL

#define SMN_N4_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b1543cUL

#define SMN_N5_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15840UL

#define SMN_N6_IOHC_SION_S0_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15c44UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_RdRsp_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_RdRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14440UL

#define SMN_N1_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14844UL

#define SMN_N2_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c48UL

#define SMN_N3_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b1504cUL

#define SMN_N4_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15450UL

#define SMN_N5_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15854UL

#define SMN_N6_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15c58UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_RdRsp_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_RdRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14444UL

#define SMN_N1_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14848UL

#define SMN_N2_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14c4cUL

#define SMN_N3_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15050UL

#define SMN_N4_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15454UL

#define SMN_N5_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15858UL

#define SMN_N6_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15c5cUL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_RdRsp_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_RdRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b14448UL

#define SMN_N1_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b1484cUL

#define SMN_N2_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b14c50UL

#define SMN_N3_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15054UL

#define SMN_N4_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15458UL

#define SMN_N5_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b1585cUL

#define SMN_N6_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_LOWER_ADDRESS    0x13b15c60UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_RdRsp_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_RdRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RdRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b1444cUL

#define SMN_N1_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b14850UL

#define SMN_N2_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b14c54UL

#define SMN_N3_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15058UL

#define SMN_N4_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b1545cUL

#define SMN_N5_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15860UL

#define SMN_N6_IOHC_SION_S1_CLIENT_RDRSP_TIMESLOT_UPPER_ADDRESS    0x13b15c64UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_Req_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_Req_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14430UL

#define SMN_N1_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14834UL

#define SMN_N2_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b14c38UL

#define SMN_N3_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b1503cUL

#define SMN_N4_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15440UL

#define SMN_N5_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15844UL

#define SMN_N6_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_LOWER_ADDRESS    0x13b15c48UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_Req_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_Req_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14434UL

#define SMN_N1_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14838UL

#define SMN_N2_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14c3cUL

#define SMN_N3_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15040UL

#define SMN_N4_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15444UL

#define SMN_N5_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15848UL

#define SMN_N6_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15c4cUL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_Req_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_Req_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b14438UL

#define SMN_N1_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b1483cUL

#define SMN_N2_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b14c40UL

#define SMN_N3_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15044UL

#define SMN_N4_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15448UL

#define SMN_N5_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b1584cUL

#define SMN_N6_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_LOWER_ADDRESS    0x13b15c50UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_Req_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_Req_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Req_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b1443cUL

#define SMN_N1_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b14840UL

#define SMN_N2_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b14c44UL

#define SMN_N3_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15048UL

#define SMN_N4_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b1544cUL

#define SMN_N5_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15850UL

#define SMN_N6_IOHC_SION_S1_CLIENT_REQ_TIMESLOT_UPPER_ADDRESS    0x13b15c54UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_ReqThrottleVCEn
* Register Description :
* SION ReqThrottleVCEn
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable request throttling for each VC at STAGE1 arbiter for Client0. Once read response deficit has exceeded the threshold, new request will be stalled until counter is below the threshold.
// Bit 0 is for VC0, Bit 1 is for VC1, etc.
#define IOHC_SION_S1_CLIENT_ReqThrottleVCEn_ReqThrottleVCEn_OFFSET      0
#define IOHC_SION_S1_CLIENT_ReqThrottleVCEn_ReqThrottleVCEn_MASK        0xff

// Bitfield Description : 
#define IOHC_SION_S1_CLIENT_ReqThrottleVCEn_Reserved_31_8_OFFSET      8
#define IOHC_SION_S1_CLIENT_ReqThrottleVCEn_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            ReqThrottleVCEn:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_ReqThrottleVCEn_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_ReqThrottleVCEn_ADDRESS    0x13b14480UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER
* Register Description :
* Lower 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_WrRsp_BurstTarget_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_WrRsp_BurstTarget_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_BurstTarget_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14450UL

#define SMN_N1_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14854UL

#define SMN_N2_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c58UL

#define SMN_N3_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b1505cUL

#define SMN_N4_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15460UL

#define SMN_N5_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15864UL

#define SMN_N6_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15c68UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER
* Register Description :
* Upper 32-bit field of BurstTarget.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of BurstTarget[63:0]. Every 8-bits sets number of beats to group together, per source, per given destination client.
// Bits 7:0 are for Source0, bits 15:8 are for Source1, etc.
// SION only groups beats if they are available. Value of 0 means no grouping (each cycle is a new arbitration decision).
#define IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_WrRsp_BurstTarget_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_WrRsp_BurstTarget_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_BurstTarget_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14454UL

#define SMN_N1_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14858UL

#define SMN_N2_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14c5cUL

#define SMN_N3_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15060UL

#define SMN_N4_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15464UL

#define SMN_N5_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15868UL

#define SMN_N6_IOHC_SION_S1_CLIENT_WRRSP_BURSTTARGET_UPPER_ADDRESS    0x13b15c6cUL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER
* Register Description :
* Lower 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_WrRsp_TimeSlot_Lower_OFFSET      0
#define IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_WrRsp_TimeSlot_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_TimeSlot_Lower:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b14458UL

#define SMN_N1_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b1485cUL

#define SMN_N2_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b14c60UL

#define SMN_N3_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15064UL

#define SMN_N4_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15468UL

#define SMN_N5_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b1586cUL

#define SMN_N6_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_LOWER_ADDRESS    0x13b15c70UL


/***********************************************************
* Register Name : IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER
* Register Description :
* Upper 32-bit field of TimeSlot.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bit field of TimeSlot[63:0]. Every 4-bits sets the arbitration decision for a new timeslot, for a total of 16 timeslots, per given destination client.
// Bits 3:0 are for timeslot 0, Bits 7:4 are for timeslot 1, Bits 11:8 are for timeslot 2, etc.
// A value of 0 means round-robin, value 1 means choose Source0, value 2 means choose Source1, value 3 means choose Source2, etc. If the winner is not available, round-robin is used to pick a new winner.
#define IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_WrRsp_TimeSlot_Upper_OFFSET      0
#define IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_WrRsp_TimeSlot_Upper_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            WrRsp_TimeSlot_Upper:32;
  } Field;
  UINT32 Value;
} IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_STRUCT;

#define SMN_N0_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b1445cUL

#define SMN_N1_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b14860UL

#define SMN_N2_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b14c64UL

#define SMN_N3_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15068UL

#define SMN_N4_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b1546cUL

#define SMN_N5_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15870UL

#define SMN_N6_IOHC_SION_S1_CLIENT_WRRSP_TIMESLOT_UPPER_ADDRESS    0x13b15c74UL


/***********************************************************
* Register Name : IOHC_SMN_MASTER_CNTL
* Register Description :
* read and write response errors from SMN control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls how read and write response errors from SMN are signaled back to the CPU.
// 1=Target Abort. 0=Master Abort.
#define IOHC_SMN_MASTER_CNTL_SmnErrRspMap_OFFSET      0
#define IOHC_SMN_MASTER_CNTL_SmnErrRspMap_MASK        0x1

// Bitfield Description : 
#define IOHC_SMN_MASTER_CNTL_Reserved_31_1_OFFSET      1
#define IOHC_SMN_MASTER_CNTL_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            SmnErrRspMap:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOHC_SMN_MASTER_CNTL_STRUCT;

#define SMN_IOHC_SMN_MASTER_CNTL_ADDRESS    0x13b11038UL


/***********************************************************
* Register Name : IOHC_SMN_MASTER_STATUS
* Register Description :
* IOHC_SMN_MASTER Error Status.
* Visibility : 0x2
************************************************************/

// Bitfield Description : <RW1C> IOHC SMN master consumed poisoned write data and terminated the request.
#define IOHC_SMN_MASTER_STATUS_SmnPoisonErrStatus_OFFSET      0
#define IOHC_SMN_MASTER_STATUS_SmnPoisonErrStatus_MASK        0x1

// Bitfield Description : 
#define IOHC_SMN_MASTER_STATUS_Reserved_31_1_OFFSET      1
#define IOHC_SMN_MASTER_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            SmnPoisonErrStatus:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOHC_SMN_MASTER_STATUS_STRUCT;

#define SMN_IOHC_SMN_MASTER_STATUS_ADDRESS    0x13b1103cUL


/***********************************************************
* Register Name : MCA_SMN_INT_APERTUREID
* Register Description :
* SMN write apertureID for MCA Error reporting Interrput.
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMN write apertureID for MCA Error reporting Interrput.
#define MCA_SMN_INT_APERTUREID_SMN_INT_APERTUREID_OFFSET      0
#define MCA_SMN_INT_APERTUREID_SMN_INT_APERTUREID_MASK        0xfff

// Bitfield Description : 
#define MCA_SMN_INT_APERTUREID_Reserved_31_12_OFFSET      12
#define MCA_SMN_INT_APERTUREID_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            SMN_INT_APERTUREID:12;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} MCA_SMN_INT_APERTUREID_STRUCT;

#define SMN_MCA_SMN_INT_APERTUREID_ADDRESS    0x13b14088UL


/***********************************************************
* Register Name : MCA_SMN_INT_CONTROL
* Register Description :
* MCA Error Reporting Interrupt control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register corresponds to a crosstrigger signal. When the bit is set to 1, the corresponding crosstrigger signal is asserted when the McaErrTrig is activated. More than one bit may be set.
#define MCA_SMN_INT_CONTROL_MCACrossTrigger_OFFSET      0
#define MCA_SMN_INT_CONTROL_MCACrossTrigger_MASK        0xf

// Bitfield Description : 
#define MCA_SMN_INT_CONTROL_Reserved_31_4_OFFSET      4
#define MCA_SMN_INT_CONTROL_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            MCACrossTrigger:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} MCA_SMN_INT_CONTROL_STRUCT;

#define SMN_MCA_SMN_INT_CONTROL_ADDRESS    0x13b1408cUL


/***********************************************************
* Register Name : MCA_SMN_INT_MCM_ADDR
* Register Description :
* SMN write mcm address for MCA Error reporting Interrput.
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMN write mcm address for MCA Error reporting Interrput.
#define MCA_SMN_INT_MCM_ADDR_SMN_INT_MCM_ADDR_OFFSET      0
#define MCA_SMN_INT_MCM_ADDR_SMN_INT_MCM_ADDR_MASK        0xf

// Bitfield Description : 
#define MCA_SMN_INT_MCM_ADDR_Reserved_31_4_OFFSET      4
#define MCA_SMN_INT_MCM_ADDR_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            SMN_INT_MCM_ADDR:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} MCA_SMN_INT_MCM_ADDR_STRUCT;

#define SMN_MCA_SMN_INT_MCM_ADDR_ADDRESS    0x13b14084UL


/***********************************************************
* Register Name : MCA_SMN_INT_REQ_ADDR
* Register Description :
* SMN write address for MCA Error reporting Interrput.
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMN write address for MCA Error reporting Interrput.
#define MCA_SMN_INT_REQ_ADDR_SMN_INT_REQ_ADDR_OFFSET      0
#define MCA_SMN_INT_REQ_ADDR_SMN_INT_REQ_ADDR_MASK        0xfffff

// Bitfield Description : 
#define MCA_SMN_INT_REQ_ADDR_Reserved_31_20_OFFSET      20
#define MCA_SMN_INT_REQ_ADDR_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            SMN_INT_REQ_ADDR:20;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} MCA_SMN_INT_REQ_ADDR_STRUCT;

#define SMN_MCA_SMN_INT_REQ_ADDR_ADDRESS    0x13b14080UL


/***********************************************************
* Register Name : MISC_RAS_CONTROL
* Register Description :
* Miscellenous RAS Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define MISC_RAS_CONTROL_Reserved_1_0_OFFSET      0
#define MISC_RAS_CONTROL_Reserved_1_0_MASK        0x3

// Bitfield Description : Enable generation NMI or SDP ErrEvent based on the mode of the NMI_SyncFlood# pin(PC_SMI_LNMI_SYNC_FLOOD_L/AGPIO86), the model is controlled by IOHC::IOHC_PIN_CNTL[NMI_SYNCFLOOD_PIN_MODE].
#define MISC_RAS_CONTROL_PIN_NMI_SyncFlood_En_OFFSET      2
#define MISC_RAS_CONTROL_PIN_NMI_SyncFlood_En_MASK        0x4

// Bitfield Description : Reserved.
#define MISC_RAS_CONTROL_GNB_SB_LinkNeverDis_OFFSET      3
#define MISC_RAS_CONTROL_GNB_SB_LinkNeverDis_MASK        0x8

// Bitfield Description : 
#define MISC_RAS_CONTROL_Reserved_8_4_OFFSET      4
#define MISC_RAS_CONTROL_Reserved_8_4_MASK        0x1f0

// Bitfield Description : Disable I/O RAS controller from generating NMI/SMI/SCI.
#define MISC_RAS_CONTROL_InterruptOutputDis_OFFSET      9
#define MISC_RAS_CONTROL_InterruptOutputDis_MASK        0x200

// Bitfield Description : Disable I/O RAS controller from generating PCIe link disable.
#define MISC_RAS_CONTROL_LinkDisOutputDis_OFFSET      10
#define MISC_RAS_CONTROL_LinkDisOutputDis_MASK        0x400

// Bitfield Description : Disable I/O RAS controller from generating SDP ErrEvent.
#define MISC_RAS_CONTROL_SyncFldOutputDis_OFFSET      11
#define MISC_RAS_CONTROL_SyncFldOutputDis_MASK        0x800

// Bitfield Description : Enable PCIE/nBIF to generate NMI by asserting input PCIE_IOC_NMI.
#define MISC_RAS_CONTROL_PCIe_NMI_En_OFFSET      12
#define MISC_RAS_CONTROL_PCIe_NMI_En_MASK        0x1000

// Bitfield Description : Enable PCIE to generate SCI by asserting input PCIE_IOC_SCI.
#define MISC_RAS_CONTROL_PCIe_SCI_En_OFFSET      13
#define MISC_RAS_CONTROL_PCIe_SCI_En_MASK        0x2000

// Bitfield Description : Enable PCIE to generate SMI by asserting input PCIE_IOC_SMI.
#define MISC_RAS_CONTROL_PCIe_SMI_En_OFFSET      14
#define MISC_RAS_CONTROL_PCIe_SMI_En_MASK        0x4000

// Bitfield Description : Enable software to generate SCI by programming SW_SCI_Status.
#define MISC_RAS_CONTROL_SW_SCI_En_OFFSET      15
#define MISC_RAS_CONTROL_SW_SCI_En_MASK        0x8000

// Bitfield Description : Enable software to generate SMI by programming SW_SMI_Status.
#define MISC_RAS_CONTROL_SW_SMI_En_OFFSET      16
#define MISC_RAS_CONTROL_SW_SMI_En_MASK        0x10000

// Bitfield Description : Enable software to generate NMI by programming SW_NMI_Status.
#define MISC_RAS_CONTROL_SW_NMI_En_OFFSET      17
#define MISC_RAS_CONTROL_SW_NMI_En_MASK        0x20000

// Bitfield Description : 
#define MISC_RAS_CONTROL_Reserved_31_18_OFFSET      18
#define MISC_RAS_CONTROL_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            PIN_NMI_SyncFlood_En:1;
    UINT32                            GNB_SB_LinkNeverDis:1;
    UINT32                            Reserved_8_4:5;
    UINT32                            InterruptOutputDis:1;
    UINT32                            LinkDisOutputDis:1;
    UINT32                            SyncFldOutputDis:1;
    UINT32                            PCIe_NMI_En:1;
    UINT32                            PCIe_SCI_En:1;
    UINT32                            PCIe_SMI_En:1;
    UINT32                            SW_SCI_En:1;
    UINT32                            SW_SMI_En:1;
    UINT32                            SW_NMI_En:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} MISC_RAS_CONTROL_STRUCT;

#define SMN_MISC_RAS_CONTROL_ADDRESS    0x13b200f4UL


/***********************************************************
* Register Name : MISC_SEVERITY_CONTROL
* Register Description :
* Miscellenous Error Severity Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define MISC_SEVERITY_CONTROL_Reserved_3_0_OFFSET      0
#define MISC_SEVERITY_CONTROL_Reserved_3_0_MASK        0xf

// Bitfield Description : Controls the error severity of error event signalled by any iohc port:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable.
#define MISC_SEVERITY_CONTROL_ErrEventErrSev_OFFSET      4
#define MISC_SEVERITY_CONTROL_ErrEventErrSev_MASK        0x30

// Bitfield Description : Controls the error severity of poisoned data error signalled by any pcie controller:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable.
#define MISC_SEVERITY_CONTROL_PcieParityErrSev_OFFSET      6
#define MISC_SEVERITY_CONTROL_PcieParityErrSev_MASK        0xc0

// Bitfield Description : 
#define MISC_SEVERITY_CONTROL_Reserved_31_8_OFFSET      8
#define MISC_SEVERITY_CONTROL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            ErrEventErrSev:2;
    UINT32                            PcieParityErrSev:2;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} MISC_SEVERITY_CONTROL_STRUCT;

#define SMN_MISC_SEVERITY_CONTROL_ADDRESS    0x13b200f0UL


/***********************************************************
* Register Name : MPM_BASE_ADDR_HI
* Register Description :
* MPM MMIO base address
* Visibility : 0x2
************************************************************/

// Bitfield Description : MPM MMIO base address bits 47:32.
#define MPM_BASE_ADDR_HI_MPM_BASE_ADDR_HI_OFFSET      0
#define MPM_BASE_ADDR_HI_MPM_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define MPM_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define MPM_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            MPM_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} MPM_BASE_ADDR_HI_STRUCT;

#define SMN_MPM_BASE_ADDR_HI_ADDRESS    0x13b10324UL


/***********************************************************
* Register Name : MPM_BASE_ADDR_LO
* Register Description :
* MPM MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : MPM MMIO enable.
#define MPM_BASE_ADDR_LO_MPM_MMIO_EN_OFFSET      0
#define MPM_BASE_ADDR_LO_MPM_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the MPM MMIO address range and enable until the next warm reset.
#define MPM_BASE_ADDR_LO_MPM_MMIO_LOCK_OFFSET      1
#define MPM_BASE_ADDR_LO_MPM_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define MPM_BASE_ADDR_LO_Reserved_19_2_OFFSET      2
#define MPM_BASE_ADDR_LO_Reserved_19_2_MASK        0xffffc

// Bitfield Description : MPM MMIO base address bits 31:20.
#define MPM_BASE_ADDR_LO_MPM_BASE_ADDR_LO_OFFSET      20
#define MPM_BASE_ADDR_LO_MPM_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            MPM_MMIO_EN:1;
    UINT32                            MPM_MMIO_LOCK:1;
    UINT32                            Reserved_19_2:18;
    UINT32                            MPM_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} MPM_BASE_ADDR_LO_STRUCT;

#define SMN_MPM_BASE_ADDR_LO_ADDRESS    0x13b10320UL


/***********************************************************
* Register Name : NBCFG_SCRATCH_0
* Register Description :
* Scratch Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
// All bits in this register can be written to and read from, but it does not control any hardware.
#define NBCFG_SCRATCH_0_NBCFG_SCRATCH_0_OFFSET      0
#define NBCFG_SCRATCH_0_NBCFG_SCRATCH_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBCFG_SCRATCH_0:32;
  } Field;
  UINT32 Value;
} NBCFG_SCRATCH_0_STRUCT;

#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_0_DEVICE       0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_0_FUNC         0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_0_OFFSET       0x68
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x68)

#define SMN_NBCFG_SCRATCH_0_ADDRESS    0x13b00068UL


/***********************************************************
* Register Name : NBCFG_SCRATCH_1
* Register Description :
* Scratch Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
// All bits in this register can be written to and read from, but it does not control any hardware.
#define NBCFG_SCRATCH_1_NBCFG_SCRATCH_1_OFFSET      0
#define NBCFG_SCRATCH_1_NBCFG_SCRATCH_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBCFG_SCRATCH_1:32;
  } Field;
  UINT32 Value;
} NBCFG_SCRATCH_1_STRUCT;

#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_1_FUNC         0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_1_OFFSET       0x6c
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6c)

#define SMN_NBCFG_SCRATCH_1_ADDRESS    0x13b0006cUL


/***********************************************************
* Register Name : NBCFG_SCRATCH_2
* Register Description :
* Scratch Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
// All bits in this register can be written to and read from, but it does not control any hardware.
#define NBCFG_SCRATCH_2_NBCFG_SCRATCH_2_OFFSET      0
#define NBCFG_SCRATCH_2_NBCFG_SCRATCH_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBCFG_SCRATCH_2:32;
  } Field;
  UINT32 Value;
} NBCFG_SCRATCH_2_STRUCT;

#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_2_DEVICE       0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_2_FUNC         0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_2_OFFSET       0x70
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x70)

#define SMN_NBCFG_SCRATCH_2_ADDRESS    0x13b00070UL


/***********************************************************
* Register Name : NBCFG_SCRATCH_3
* Register Description :
* Scratch Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
// All bits in this register can be written to and read from, but it does not control any hardware.
#define NBCFG_SCRATCH_3_NBCFG_SCRATCH_3_OFFSET      0
#define NBCFG_SCRATCH_3_NBCFG_SCRATCH_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBCFG_SCRATCH_3:32;
  } Field;
  UINT32 Value;
} NBCFG_SCRATCH_3_STRUCT;

#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_3_DEVICE       0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_3_FUNC         0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_3_OFFSET       0x74
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x74)

#define SMN_NBCFG_SCRATCH_3_ADDRESS    0x13b00074UL


/***********************************************************
* Register Name : NBCFG_SCRATCH_4
* Register Description :
* Scratch Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
// All bits in this register can be written to and read from, but it does not control any hardware.
#define NBCFG_SCRATCH_4_NBCFG_SCRATCH_4_OFFSET      0
#define NBCFG_SCRATCH_4_NBCFG_SCRATCH_4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBCFG_SCRATCH_4:32;
  } Field;
  UINT32 Value;
} NBCFG_SCRATCH_4_STRUCT;

#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_4_DEVICE       0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_4_FUNC         0
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_4_OFFSET       0x78
#define PCICFG_NBIO_IOHUB_NBCFG_SCRATCH_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x78)

#define SMN_NBCFG_SCRATCH_4_ADDRESS    0x13b00078UL


/***********************************************************
* Register Name : NBIF1PortAExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAExtCorr_ACTION_CONTROL_ADDRESS    0x13b202ccUL


/***********************************************************
* Register Name : NBIF1PortAExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAExtFatal_ACTION_CONTROL_ADDRESS    0x13b202c4UL


/***********************************************************
* Register Name : NBIF1PortAExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b202c8UL


/***********************************************************
* Register Name : NBIF1PortAIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAIntCorr_ACTION_CONTROL_ADDRESS    0x13b202c0UL


/***********************************************************
* Register Name : NBIF1PortAIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAIntFatal_ACTION_CONTROL_ADDRESS    0x13b202b8UL


/***********************************************************
* Register Name : NBIF1PortAIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b202bcUL


/***********************************************************
* Register Name : NBIF1PortAParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortAParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortAParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortAParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortAParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortAParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortAParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortAParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortAParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortAParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortAParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortAParityErr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortAParityErr_ACTION_CONTROL_ADDRESS    0x13b202d0UL


/***********************************************************
* Register Name : NBIF1PortASerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortASerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortASerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortASerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortASerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortASerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortASerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortASerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortASerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortASerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortASerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortASerr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortASerr_ACTION_CONTROL_ADDRESS    0x13b202b4UL


/***********************************************************
* Register Name : NBIF1PortBExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBExtCorr_ACTION_CONTROL_ADDRESS    0x13b202ecUL


/***********************************************************
* Register Name : NBIF1PortBExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBExtFatal_ACTION_CONTROL_ADDRESS    0x13b202e4UL


/***********************************************************
* Register Name : NBIF1PortBExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b202e8UL


/***********************************************************
* Register Name : NBIF1PortBIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBIntCorr_ACTION_CONTROL_ADDRESS    0x13b202e0UL


/***********************************************************
* Register Name : NBIF1PortBIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBIntFatal_ACTION_CONTROL_ADDRESS    0x13b202d8UL


/***********************************************************
* Register Name : NBIF1PortBIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b202dcUL


/***********************************************************
* Register Name : NBIF1PortBParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBParityErr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBParityErr_ACTION_CONTROL_ADDRESS    0x13b202f0UL


/***********************************************************
* Register Name : NBIF1PortBSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortBSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortBSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortBSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortBSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortBSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortBSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortBSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortBSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortBSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortBSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortBSerr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortBSerr_ACTION_CONTROL_ADDRESS    0x13b202d4UL


/***********************************************************
* Register Name : NBIF1PortCExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCExtCorr_ACTION_CONTROL_ADDRESS    0x13b2030cUL


/***********************************************************
* Register Name : NBIF1PortCExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCExtFatal_ACTION_CONTROL_ADDRESS    0x13b20304UL


/***********************************************************
* Register Name : NBIF1PortCExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20308UL


/***********************************************************
* Register Name : NBIF1PortCIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCIntCorr_ACTION_CONTROL_ADDRESS    0x13b20300UL


/***********************************************************
* Register Name : NBIF1PortCIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCIntFatal_ACTION_CONTROL_ADDRESS    0x13b202f8UL


/***********************************************************
* Register Name : NBIF1PortCIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b202fcUL


/***********************************************************
* Register Name : NBIF1PortCParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCParityErr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCParityErr_ACTION_CONTROL_ADDRESS    0x13b20310UL


/***********************************************************
* Register Name : NBIF1PortCSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define NBIF1PortCSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define NBIF1PortCSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define NBIF1PortCSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define NBIF1PortCSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define NBIF1PortCSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define NBIF1PortCSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define NBIF1PortCSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define NBIF1PortCSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define NBIF1PortCSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define NBIF1PortCSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} NBIF1PortCSerr_ACTION_CONTROL_STRUCT;

#define SMN_NBIF1PortCSerr_ACTION_CONTROL_ADDRESS    0x13b202f4UL


/***********************************************************
* Register Name : NB_ADAPTER_ID
* Register Description :
* Subsystem and Subvendor ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Subsystem vendor ID.
#define NB_ADAPTER_ID_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define NB_ADAPTER_ID_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Subsystem ID.
#define NB_ADAPTER_ID_SUBSYSTEM_ID_OFFSET      16
#define NB_ADAPTER_ID_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} NB_ADAPTER_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2c)

#define SMN_NB_ADAPTER_ID_ADDRESS    0x13b0002cUL


/***********************************************************
* Register Name : NB_ADAPTER_ID_W
* Register Description :
* Subsystem Vendor ID and Subsystem ID write register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field sets the value in the corresponding field in IOHC::NB_ADAPTER_ID.
#define NB_ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define NB_ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : This field sets the value in the corresponding field in IOHC::NB_ADAPTER_ID.
#define NB_ADAPTER_ID_W_SUBSYSTEM_ID_OFFSET      16
#define NB_ADAPTER_ID_W_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} NB_ADAPTER_ID_W_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_W_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_W_OFFSET       0x50
#define PCICFG_NBIO_IOHUB_NB_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x50)

#define SMN_NB_ADAPTER_ID_W_ADDRESS    0x13b00050UL


/***********************************************************
* Register Name : NB_BASE_CODE
* Register Description :
* Class Code Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Host bridge class code as defined in the PCI specification.
#define NB_BASE_CODE_BASE_CLASS_CODE_OFFSET      0
#define NB_BASE_CODE_BASE_CLASS_CODE_MASK        0xff

typedef union {
  struct {
    UINT8                            BASE_CLASS_CODE:8;
  } Field;
  UINT8 Value;
} NB_BASE_CODE_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_BASE_CODE_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_BASE_CODE_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_BASE_CODE_OFFSET       0xb
#define PCICFG_NBIO_IOHUB_NB_BASE_CODE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb)

#define SMN_NB_BASE_CODE_ADDRESS    0x13b0000bUL


/***********************************************************
* Register Name : NB_BUS_NUM_CNTL
* Register Description :
* GNB Bus Number Control.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies the number of the NBIO local bus when NB_BUS_LAT_mode is set.
#define NB_BUS_NUM_CNTL_NB_BUS_NUM_OFFSET      0
#define NB_BUS_NUM_CNTL_NB_BUS_NUM_MASK        0xff

// Bitfield Description : NBIO bus number is specified by NB_BUS_NUM.
// 0 = Local bus number of NBIO is capture from any type 0 configuration request.
// 1= Use the NB_BUS_NUM to decode for configuration cycles targeting the NBIO bus.
#define NB_BUS_NUM_CNTL_NB_BUS_LAT_Mode_OFFSET      8
#define NB_BUS_NUM_CNTL_NB_BUS_LAT_Mode_MASK        0x100

// Bitfield Description : 
#define NB_BUS_NUM_CNTL_Reserved_31_9_OFFSET      9
#define NB_BUS_NUM_CNTL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            NB_BUS_NUM:8;
    UINT32                            NB_BUS_LAT_Mode:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} NB_BUS_NUM_CNTL_STRUCT;

#define SMN_NB_BUS_NUM_CNTL_ADDRESS    0x13b10044UL


/***********************************************************
* Register Name : NB_CACHE_LINE
* Register Description :
* Cacheline Size Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cacheline size.
#define NB_CACHE_LINE_CACHE_LINE_SIZE_OFFSET      0
#define NB_CACHE_LINE_CACHE_LINE_SIZE_MASK        0xff

typedef union {
  struct {
    UINT8                            CACHE_LINE_SIZE:8;
  } Field;
  UINT8 Value;
} NB_CACHE_LINE_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_CACHE_LINE_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_IOHUB_NB_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc)

#define SMN_NB_CACHE_LINE_ADDRESS    0x13b0000cUL


/***********************************************************
* Register Name : NB_CAPABILITIES_PTR
* Register Description :
* Capabilities Pointer Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Capabilities pointer exists.
#define NB_CAPABILITIES_PTR_CAP_PTR_OFFSET      0
#define NB_CAPABILITIES_PTR_CAP_PTR_MASK        0xff

// Bitfield Description : 
#define NB_CAPABILITIES_PTR_Reserved_31_8_OFFSET      8
#define NB_CAPABILITIES_PTR_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            CAP_PTR:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} NB_CAPABILITIES_PTR_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_CAPABILITIES_PTR_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_CAPABILITIES_PTR_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_CAPABILITIES_PTR_OFFSET       0x34
#define PCICFG_NBIO_IOHUB_NB_CAPABILITIES_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x34)

#define SMN_NB_CAPABILITIES_PTR_ADDRESS    0x13b00034UL


/***********************************************************
* Register Name : NB_CNTL
* Register Description :
* Northbridge Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_CNTL_Reserved_6_0_OFFSET      0
#define NB_CNTL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Lock HWInit registers.
// This bit prevents updates to the IOHC shadow copies of the BIF core configuration registers locked by C_PCIEIND:0x1010010[0].
#define NB_CNTL_HWINIT_WR_LOCK_OFFSET      7
#define NB_CNTL_HWINIT_WR_LOCK_MASK        0x80

// Bitfield Description : 
#define NB_CNTL_Reserved_31_8_OFFSET      8
#define NB_CNTL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            HWINIT_WR_LOCK:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} NB_CNTL_STRUCT;

#define SMN_NB_CNTL_ADDRESS    0x13b10000UL


/***********************************************************
* Register Name : NB_COMMAND
* Register Description :
* Command Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : I/O Access Enable.
// This bit is always 0.
#define NB_COMMAND_IO_ACCESS_EN_OFFSET      0
#define NB_COMMAND_IO_ACCESS_EN_MASK        0x1

// Bitfield Description : Memory Access Enable.
// Controls whether PCI memory accesses to system memory are accepted.
// This bit is always 0.
#define NB_COMMAND_MEM_ACCESS_EN_OFFSET      1
#define NB_COMMAND_MEM_ACCESS_EN_MASK        0x2

// Bitfield Description : Bus Master Enable.
// This bit is always 0.
#define NB_COMMAND_BUS_MASTER_EN_OFFSET      2
#define NB_COMMAND_BUS_MASTER_EN_MASK        0x4

// Bitfield Description : 
#define NB_COMMAND_Reserved_15_3_OFFSET      3
#define NB_COMMAND_Reserved_15_3_MASK        0xfff8

typedef union {
  struct {
    UINT16                            IO_ACCESS_EN:1;
    UINT16                            MEM_ACCESS_EN:1;
    UINT16                            BUS_MASTER_EN:1;
    UINT16                            Reserved_15_3:13;
  } Field;
  UINT16 Value;
} NB_COMMAND_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_COMMAND_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_COMMAND_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_IOHUB_NB_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4)

#define SMN_NB_COMMAND_ADDRESS    0x13b00004UL


/***********************************************************
* Register Name : NB_DEVICE_ID
* Register Description :
* Device ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Device Identifier
// This 16-bit field is assigned by the device manufacturer and identifies the type of device.
#define NB_DEVICE_ID_DEVICE_ID_OFFSET      0
#define NB_DEVICE_ID_DEVICE_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            DEVICE_ID:16;
  } Field;
  UINT16 Value;
} NB_DEVICE_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2)

#define SMN_NB_DEVICE_ID_ADDRESS    0x13b00002UL


/***********************************************************
* Register Name : NB_DEVICE_ID_W
* Register Description :
* Device ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Device Identifier
// This 16-bit field is assigned by the device manufacturer and identifies the type of device.
#define NB_DEVICE_ID_W_DEVICE_ID_OFFSET      0
#define NB_DEVICE_ID_W_DEVICE_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            DEVICE_ID:16;
  } Field;
  UINT16 Value;
} NB_DEVICE_ID_W_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_W_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_W_OFFSET       0x102
#define PCICFG_NBIO_IOHUB_NB_DEVICE_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x102)

#define SMN_NB_DEVICE_ID_W_ADDRESS    0x13b00102UL


/***********************************************************
* Register Name : NB_DRAM3_BASE
* Register Description :
* Memory 3 Base
* Visibility : 0x2
************************************************************/

// Bitfield Description : DRAM3_BASE defines the bottom of physical memory between 40-bit and 52-bit space inclusive. The register contains bit 51:22. Lower address bits are implicitly set to 0.
#define NB_DRAM3_BASE_DRAM3_BASE_OFFSET      0
#define NB_DRAM3_BASE_DRAM3_BASE_MASK        0x3fffffff

// Bitfield Description : 
#define NB_DRAM3_BASE_Reserved_31_30_OFFSET      30
#define NB_DRAM3_BASE_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            DRAM3_BASE:30;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} NB_DRAM3_BASE_STRUCT;

#define SMN_NB_DRAM3_BASE_ADDRESS    0x13b102c4UL


/***********************************************************
* Register Name : NB_DRAM_SLOT1_BASE
* Register Description :
* North Bridge Memory Base Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_DRAM_SLOT1_BASE_Reserved_22_0_OFFSET      0
#define NB_DRAM_SLOT1_BASE_Reserved_22_0_MASK        0x7fffff

// Bitfield Description : Specifies the base address that divides between MMIO and DRAM. For addresses below 4G and not included in NB_DRAM_SLOT1_BASE to NB_DRAM_SLOT1_LIMIT is MMIO, between the range is DRAM: BIOS should write to this field following completion of the memory sizing algorithm, after it has determined the total size of the installed memory.
#define NB_DRAM_SLOT1_BASE_DRAM_BASE_OFFSET      23
#define NB_DRAM_SLOT1_BASE_DRAM_BASE_MASK        0xff800000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            DRAM_BASE:9;
  } Field;
  UINT32 Value;
} NB_DRAM_SLOT1_BASE_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_DRAM_SLOT1_BASE_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_DRAM_SLOT1_BASE_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_DRAM_SLOT1_BASE_OFFSET       0x88
#define PCICFG_NBIO_IOHUB_NB_DRAM_SLOT1_BASE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x88)

#define SMN_NB_DRAM_SLOT1_BASE_ADDRESS    0x13b00088UL


/***********************************************************
* Register Name : NB_HEADER
* Register Description :
* Header Type Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicating that Type 00 Configuration Space Header format is supported.
#define NB_HEADER_HEADER_TYPE_OFFSET      0
#define NB_HEADER_HEADER_TYPE_MASK        0x7f

// Bitfield Description : Indicating that the Northbridge block is a multi-function device.
#define NB_HEADER_DEVICE_TYPE_OFFSET      7
#define NB_HEADER_DEVICE_TYPE_MASK        0x80

typedef union {
  struct {
    UINT8                            HEADER_TYPE:7;
    UINT8                            DEVICE_TYPE:1;
  } Field;
  UINT8 Value;
} NB_HEADER_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_HEADER_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_HEADER_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_HEADER_OFFSET       0xe
#define PCICFG_NBIO_IOHUB_NB_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xe)

#define SMN_NB_HEADER_ADDRESS    0x13b0000eUL


/***********************************************************
* Register Name : NB_HEADER_W
* Register Description :
* Header Type Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_HEADER_W_Reserved_6_0_OFFSET      0
#define NB_HEADER_W_Reserved_6_0_MASK        0x7f

// Bitfield Description : This field sets the value in the corresponding field in IOHC::NB_HEADER bit 7.
#define NB_HEADER_W_DEVICE_TYPE_OFFSET      7
#define NB_HEADER_W_DEVICE_TYPE_MASK        0x80

// Bitfield Description : 
#define NB_HEADER_W_Reserved_31_8_OFFSET      8
#define NB_HEADER_W_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            DEVICE_TYPE:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} NB_HEADER_W_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_HEADER_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_HEADER_W_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_HEADER_W_OFFSET       0x48
#define PCICFG_NBIO_IOHUB_NB_HEADER_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48)

#define SMN_NB_HEADER_W_ADDRESS    0x13b00048UL


/***********************************************************
* Register Name : NB_INDEX_DATA_MUTEX0
* Register Description :
* Link Index/Data Mutex Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hardware successfully claimed the mutex when it reads back its own assigned ID after it writes its ID into register.
// Writing 1 to bit 31 releases the lock and hardware clears this field automatically.
#define NB_INDEX_DATA_MUTEX0_NB_INDEX_DATA_MUTEX0_OFFSET      0
#define NB_INDEX_DATA_MUTEX0_NB_INDEX_DATA_MUTEX0_MASK        0x7fffffff

// Bitfield Description : Writing 1 to release the lock.
#define NB_INDEX_DATA_MUTEX0_NB_INDEX_DATA_MUTEX0_UNLOCK_OFFSET      31
#define NB_INDEX_DATA_MUTEX0_NB_INDEX_DATA_MUTEX0_UNLOCK_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NB_INDEX_DATA_MUTEX0:31;
    UINT32                            NB_INDEX_DATA_MUTEX0_UNLOCK:1;
  } Field;
  UINT32 Value;
} NB_INDEX_DATA_MUTEX0_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX0_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX0_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX0_OFFSET       0xa8
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa8)

#define SMN_NB_INDEX_DATA_MUTEX0_ADDRESS    0x13b000a8UL


/***********************************************************
* Register Name : NB_INDEX_DATA_MUTEX1
* Register Description :
* Link Index/Data Mutex Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hardware successfully claimed the mutex when it reads back its own assigned ID after it writes its ID into register.
// Writing 1 to bit 31 release the lock and hardware clears this field automatically.
#define NB_INDEX_DATA_MUTEX1_NB_INDEX_DATA_MUTEX1_OFFSET      0
#define NB_INDEX_DATA_MUTEX1_NB_INDEX_DATA_MUTEX1_MASK        0x7fffffff

// Bitfield Description : Writing 1 to release the lock
#define NB_INDEX_DATA_MUTEX1_NB_INDEX_DATA_MUTEX1_UNLOCK_OFFSET      31
#define NB_INDEX_DATA_MUTEX1_NB_INDEX_DATA_MUTEX1_UNLOCK_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NB_INDEX_DATA_MUTEX1:31;
    UINT32                            NB_INDEX_DATA_MUTEX1_UNLOCK:1;
  } Field;
  UINT32 Value;
} NB_INDEX_DATA_MUTEX1_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX1_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX1_OFFSET       0xac
#define PCICFG_NBIO_IOHUB_NB_INDEX_DATA_MUTEX1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xac)

#define SMN_NB_INDEX_DATA_MUTEX1_ADDRESS    0x13b000acUL


/***********************************************************
* Register Name : NB_LATENCY
* Register Description :
* Latency Timer Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit field defines the minimum amount of time in PCI clock cycles that the bus master can retain ownership of the bus. This is mandatory for masters that are capable of performing a burst consisting of more than two data phases.
#define NB_LATENCY_LATENCY_TIMER_OFFSET      0
#define NB_LATENCY_LATENCY_TIMER_MASK        0xff

typedef union {
  struct {
    UINT8                            LATENCY_TIMER:8;
  } Field;
  UINT8 Value;
} NB_LATENCY_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_LATENCY_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_LATENCY_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_IOHUB_NB_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd)

#define SMN_NB_LATENCY_ADDRESS    0x13b0000dUL


/***********************************************************
* Register Name : NB_LOWER_DRAM2_BASE
* Register Description :
* Memory 2 Base Low
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_LOWER_DRAM2_BASE_Reserved_22_0_OFFSET      0
#define NB_LOWER_DRAM2_BASE_Reserved_22_0_MASK        0x7fffff

// Bitfield Description : 4GB) for upstream read and write transactions that are forwarded to the host bridge (from the GNB to the CNB). All addresses less than this system address are forwarded to DRAM and are not checked to determine if the transaction is a peer-to-peer transaction.  All upstream reads with addresses greater than this system address are master aborted.
#define NB_LOWER_DRAM2_BASE_LOWER_DRAM2_BASE_OFFSET      23
#define NB_LOWER_DRAM2_BASE_LOWER_DRAM2_BASE_MASK        0xff800000

typedef union {
  struct {
    UINT32                            Reserved_22_0:23;
    UINT32                            LOWER_DRAM2_BASE:9;
  } Field;
  UINT32 Value;
} NB_LOWER_DRAM2_BASE_STRUCT;

#define SMN_NB_LOWER_DRAM2_BASE_ADDRESS    0x13b1006cUL


/***********************************************************
* Register Name : NB_LOWER_TOP_OF_DRAM2
* Register Description :
* Top of Memory 2 Low.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TOM2  enabled.
#define NB_LOWER_TOP_OF_DRAM2_ENABLE_OFFSET      0
#define NB_LOWER_TOP_OF_DRAM2_ENABLE_MASK        0x1

// Bitfield Description : 
#define NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_OFFSET      1
#define NB_LOWER_TOP_OF_DRAM2_Reserved_22_1_MASK        0x7ffffe

// Bitfield Description : TOM2[31:23] - This field specifies the maximum system address for upstream read and write transactions that are forwarded to the host bridge (from the GNB to the CNB). All addresses less than this system address are forwarded to DRAM and are not checked to determine if the transaction is a peer-to-peer transaction.  All upstream reads with addresses greater than this system address are master aborted.
#define NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_OFFSET      23
#define NB_LOWER_TOP_OF_DRAM2_LOWER_TOM2_MASK        0xff800000

typedef union {
  struct {
    UINT32                            ENABLE:1;
    UINT32                            Reserved_22_1:22;
    UINT32                            LOWER_TOM2:9;
  } Field;
  UINT32 Value;
} NB_LOWER_TOP_OF_DRAM2_STRUCT;

#define SMN_NB_LOWER_TOP_OF_DRAM2_ADDRESS    0x13b10064UL


/***********************************************************
* Register Name : NB_MMIOBASE
* Register Description :
* Memory Mapped IO Base Address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Memory mapped IO base address [47:16].
#define NB_MMIOBASE_MMIOBASE_OFFSET      0
#define NB_MMIOBASE_MMIOBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOBASE:32;
  } Field;
  UINT32 Value;
} NB_MMIOBASE_STRUCT;

#define SMN_NB_MMIOBASE_ADDRESS    0x13b1005cUL


/***********************************************************
* Register Name : NB_MMIOLIMIT
* Register Description :
* Memory Mapped IO Limit Address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Memory mapped IO limit address [47:16].
#define NB_MMIOLIMIT_MMIOLIMIT_OFFSET      0
#define NB_MMIOLIMIT_MMIOLIMIT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MMIOLIMIT:32;
  } Field;
  UINT32 Value;
} NB_MMIOLIMIT_STRUCT;

#define SMN_NB_MMIOLIMIT_ADDRESS    0x13b10060UL


/***********************************************************
* Register Name : NB_PCI_ARB
* Register Description :
* Link Arbitration Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_PCI_ARB_Reserved_2_0_OFFSET      0
#define NB_PCI_ARB_Reserved_2_0_MASK        0x7

// Bitfield Description : VGA range decode:
// 0 = VGA range is decoded as system memory.
// 1 = VGA range is decoded as MMIO region.
#define NB_PCI_ARB_VGA_HOLE_OFFSET      3
#define NB_PCI_ARB_VGA_HOLE_MASK        0x8

// Bitfield Description : 
#define NB_PCI_ARB_Reserved_7_4_OFFSET      4
#define NB_PCI_ARB_Reserved_7_4_MASK        0xf0

// Bitfield Description : PME message mode:
// 0 = PME_Turn_Off message is triggered by a PME_Turn_Off message from the FCH.
// 1 = PME_Turn_Off is triggered by writing 1 to PMETurnOff bit(0x84[9]).
#define NB_PCI_ARB_PMEMode_OFFSET      8
#define NB_PCI_ARB_PMEMode_MASK        0x100

// Bitfield Description : PME_Turn_Off message trigger:
// In case PMEMode is set, write 1 to this bit triggers a PME_Turn_Off messages to all downstream devices. This bit is reset only when the system power is turned off.
#define NB_PCI_ARB_PMETurnOff_OFFSET      9
#define NB_PCI_ARB_PMETurnOff_MASK        0x200

// Bitfield Description : Status of detecting all connected downstream PCI-Express ports return PMETOAck after PME_Turn_Off is sent. This bit is reset when the PMETurnOff register is written.
#define NB_PCI_ARB_PMETOAckStatus_OFFSET      10
#define NB_PCI_ARB_PMETOAckStatus_MASK        0x400

// Bitfield Description : 
#define NB_PCI_ARB_Reserved_15_11_OFFSET      11
#define NB_PCI_ARB_Reserved_15_11_MASK        0xf800

// Bitfield Description : Directed PME message target select. Only valid during PMEMode 1. Represents {5-bit DEVICE, 3-bit FUNCTION} of intended target of PME_Turn_Off message. Value of 0 means broadcast to all active PCI-Express ports.
#define NB_PCI_ARB_PMETarget_OFFSET      16
#define NB_PCI_ARB_PMETarget_MASK        0xff0000

// Bitfield Description : 
#define NB_PCI_ARB_Reserved_31_24_OFFSET      24
#define NB_PCI_ARB_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            VGA_HOLE:1;
    UINT32                            Reserved_7_4:4;
    UINT32                            PMEMode:1;
    UINT32                            PMETurnOff:1;
    UINT32                            PMETOAckStatus:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            PMETarget:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} NB_PCI_ARB_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_PCI_ARB_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_PCI_ARB_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_PCI_ARB_OFFSET       0x84
#define PCICFG_NBIO_IOHUB_NB_PCI_ARB_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x84)

#define SMN_NB_PCI_ARB_ADDRESS    0x13b00084UL


/***********************************************************
* Register Name : NB_PCI_CTRL
* Register Description :
* PCI Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_PCI_CTRL_Reserved_3_0_OFFSET      0
#define NB_PCI_CTRL_Reserved_3_0_MASK        0xf

// Bitfield Description : (Debug bit) Disable PME message generation to FCH.
#define NB_PCI_CTRL_PMEDis_OFFSET      4
#define NB_PCI_CTRL_PMEDis_MASK        0x10

// Bitfield Description : (Debug bit) Disable System Error message generation to FCH.
#define NB_PCI_CTRL_SErrDis_OFFSET      5
#define NB_PCI_CTRL_SErrDis_MASK        0x20

// Bitfield Description : 
#define NB_PCI_CTRL_Reserved_22_6_OFFSET      6
#define NB_PCI_CTRL_Reserved_22_6_MASK        0x7fffc0

// Bitfield Description : Enable Memory Mapped I/O decoding.
#define NB_PCI_CTRL_MMIOEnable_OFFSET      23
#define NB_PCI_CTRL_MMIOEnable_MASK        0x800000

// Bitfield Description : 
#define NB_PCI_CTRL_Reserved_25_24_OFFSET      24
#define NB_PCI_CTRL_Reserved_25_24_MASK        0x3000000

// Bitfield Description : (Debug bit) Disable Hot Plug message generation to FCH.
#define NB_PCI_CTRL_HPDis_OFFSET      26
#define NB_PCI_CTRL_HPDis_MASK        0x4000000

// Bitfield Description : 
#define NB_PCI_CTRL_Reserved_31_27_OFFSET      27
#define NB_PCI_CTRL_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            PMEDis:1;
    UINT32                            SErrDis:1;
    UINT32                            Reserved_22_6:17;
    UINT32                            MMIOEnable:1;
    UINT32                            Reserved_25_24:2;
    UINT32                            HPDis:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} NB_PCI_CTRL_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_PCI_CTRL_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_PCI_CTRL_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_PCI_CTRL_OFFSET       0x4c
#define PCICFG_NBIO_IOHUB_NB_PCI_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4c)

#define SMN_NB_PCI_CTRL_ADDRESS    0x13b0004cUL


/***********************************************************
* Register Name : NB_PERF_CNT_CTRL
* Register Description :
* Performance Counters Control Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable Performance Counters.
#define NB_PERF_CNT_CTRL_GLOBE_CNT_EN_OFFSET      0
#define NB_PERF_CNT_CTRL_GLOBE_CNT_EN_MASK        0x1

// Bitfield Description : Write 1 to load the counter shadow registers. Read back value is always 0.
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_WR_OFFSET      1
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_WR_MASK        0x2

// Bitfield Description : Write 1 for Global RESET of ALL counters. Read back value is always 0.
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_OFFSET      2
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_MASK        0x4

// Bitfield Description : 
#define NB_PERF_CNT_CTRL_Reserved_7_3_OFFSET      3
#define NB_PERF_CNT_CTRL_Reserved_7_3_MASK        0xf8

// Bitfield Description : Programmable Pulse width for Global Shadow Write Toggle.
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_DELAY_OFFSET      8
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_DELAY_MASK        0xf00

// Bitfield Description : 
#define NB_PERF_CNT_CTRL_Reserved_14_12_OFFSET      12
#define NB_PERF_CNT_CTRL_Reserved_14_12_MASK        0x7000

// Bitfield Description : Enables the Programmable Pulse with for Global Shadow Write Toggle.
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_DELAY_EN_OFFSET      15
#define NB_PERF_CNT_CTRL_GLOBE_SHADOW_DELAY_EN_MASK        0x8000

// Bitfield Description : Programmable Pulse width for Global Perf Reset Toggle.
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_DELAY_OFFSET      16
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_DELAY_MASK        0xf0000

// Bitfield Description : 
#define NB_PERF_CNT_CTRL_Reserved_22_20_OFFSET      20
#define NB_PERF_CNT_CTRL_Reserved_22_20_MASK        0x700000

// Bitfield Description : Enables the Programmable Pulse width for Global Perf Reset Toggle.
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_DELAY_EN_OFFSET      23
#define NB_PERF_CNT_CTRL_GLOBE_PERF_RESET_DELAY_EN_MASK        0x800000

// Bitfield Description : 
#define NB_PERF_CNT_CTRL_Reserved_31_24_OFFSET      24
#define NB_PERF_CNT_CTRL_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            GLOBE_CNT_EN:1;
    UINT32                            GLOBE_SHADOW_WR:1;
    UINT32                            GLOBE_PERF_RESET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            GLOBE_SHADOW_DELAY:4;
    UINT32                            Reserved_14_12:3;
    UINT32                            GLOBE_SHADOW_DELAY_EN:1;
    UINT32                            GLOBE_PERF_RESET_DELAY:4;
    UINT32                            Reserved_22_20:3;
    UINT32                            GLOBE_PERF_RESET_DELAY_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} NB_PERF_CNT_CTRL_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_PERF_CNT_CTRL_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_PERF_CNT_CTRL_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_PERF_CNT_CTRL_OFFSET       0xf4
#define PCICFG_NBIO_IOHUB_NB_PERF_CNT_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xf4)

#define SMN_NB_PERF_CNT_CTRL_ADDRESS    0x13b000f4UL


/***********************************************************
* Register Name : NB_PROG_DEVICE_REMAP
* Register Description :
* Software Device Remapping Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Programming DevNum, FnNum to map to PCIE0 Port0
// Bit[7:3]-DevNum Bit[2:0]-FnNum.
#define NB_PROG_DEVICE_REMAP_DevFnMap_OFFSET      0
#define NB_PROG_DEVICE_REMAP_DevFnMap_MASK        0xff

// Bitfield Description : 
#define NB_PROG_DEVICE_REMAP_Reserved_31_8_OFFSET      8
#define NB_PROG_DEVICE_REMAP_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            DevFnMap:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} NB_PROG_DEVICE_REMAP_STRUCT;

#define SMN_N0_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100b8UL

#define SMN_N10_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100e0UL

#define SMN_N11_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100e4UL

#define SMN_N12_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100e8UL

#define SMN_N13_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100ecUL

#define SMN_N14_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100f0UL

#define SMN_N15_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100f4UL

#define SMN_N1_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100bcUL

#define SMN_N2_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100c0UL

#define SMN_N3_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100c4UL

#define SMN_N4_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100c8UL

#define SMN_N5_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100ccUL

#define SMN_N6_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100d0UL

#define SMN_N7_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100d4UL

#define SMN_N8_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100d8UL

#define SMN_N9_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100dcUL


/***********************************************************
* Register Name : NB_REGPROG_INF
* Register Description :
* Program Interface Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Programmig Interface.
#define NB_REGPROG_INF_REG_LEVEL_PROG_INF_OFFSET      0
#define NB_REGPROG_INF_REG_LEVEL_PROG_INF_MASK        0xff

typedef union {
  struct {
    UINT8                            REG_LEVEL_PROG_INF:8;
  } Field;
  UINT8 Value;
} NB_REGPROG_INF_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_REGPROG_INF_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_REGPROG_INF_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_REGPROG_INF_OFFSET       0x9
#define PCICFG_NBIO_IOHUB_NB_REGPROG_INF_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x9)

#define SMN_NB_REGPROG_INF_ADDRESS    0x13b00009UL


/***********************************************************
* Register Name : NB_REVISION_ID
* Register Description :
* Revision ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies Stepping number of the device.
#define NB_REVISION_ID_MINOR_REV_ID_OFFSET      0
#define NB_REVISION_ID_MINOR_REV_ID_MASK        0xf

// Bitfield Description : Identifies Revision number of the device.
// This field is always 0.
#define NB_REVISION_ID_MAJOR_REV_ID_OFFSET      4
#define NB_REVISION_ID_MAJOR_REV_ID_MASK        0xf0

typedef union {
  struct {
    UINT8                            MINOR_REV_ID:4;
    UINT8                            MAJOR_REV_ID:4;
  } Field;
  UINT8 Value;
} NB_REVISION_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_REVISION_ID_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_IOHUB_NB_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8)

#define SMN_NB_REVISION_ID_ADDRESS    0x13b00008UL


/***********************************************************
* Register Name : NB_SMN_DATA_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_0_NB_SMN_DATA_0_OFFSET      0
#define NB_SMN_DATA_0_NB_SMN_DATA_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_0:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_0_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_0_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_0_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_0_OFFSET       0x64
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x64)

#define SMN_NB_SMN_DATA_0_ADDRESS    0x13b00064UL


/***********************************************************
* Register Name : NB_SMN_DATA_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_1_NB_SMN_DATA_1_OFFSET      0
#define NB_SMN_DATA_1_NB_SMN_DATA_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_1:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_1_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_1_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_1_OFFSET       0xa4
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa4)

#define SMN_NB_SMN_DATA_1_ADDRESS    0x13b000a4UL


/***********************************************************
* Register Name : NB_SMN_DATA_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_2_NB_SMN_DATA_2_OFFSET      0
#define NB_SMN_DATA_2_NB_SMN_DATA_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_2:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_2_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_2_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_2_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_2_OFFSET       0xbc
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xbc)

#define SMN_NB_SMN_DATA_2_ADDRESS    0x13b000bcUL


/***********************************************************
* Register Name : NB_SMN_DATA_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_3_NB_SMN_DATA_3_OFFSET      0
#define NB_SMN_DATA_3_NB_SMN_DATA_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_3:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_3_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_3_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_3_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_3_OFFSET       0xc8
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc8)

#define SMN_NB_SMN_DATA_3_ADDRESS    0x13b000c8UL


/***********************************************************
* Register Name : NB_SMN_DATA_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_4_NB_SMN_DATA_4_OFFSET      0
#define NB_SMN_DATA_4_NB_SMN_DATA_4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_4:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_4_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_4_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_4_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_4_OFFSET       0xd4
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd4)

#define SMN_NB_SMN_DATA_4_ADDRESS    0x13b000d4UL


/***********************************************************
* Register Name : NB_SMN_DATA_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_5_NB_SMN_DATA_5_OFFSET      0
#define NB_SMN_DATA_5_NB_SMN_DATA_5_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_5:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_5_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_5_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_5_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_5_OFFSET       0xe4
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xe4)

#define SMN_NB_SMN_DATA_5_ADDRESS    0x13b000e4UL


/***********************************************************
* Register Name : NB_SMN_DATA_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Data value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_DATA_6_NB_SMN_DATA_6_OFFSET      0
#define NB_SMN_DATA_6_NB_SMN_DATA_6_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_DATA_6:32;
  } Field;
  UINT32 Value;
} NB_SMN_DATA_6_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_6_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_6_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_6_OFFSET       0xfc
#define PCICFG_NBIO_IOHUB_NB_SMN_DATA_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xfc)

#define SMN_NB_SMN_DATA_6_ADDRESS    0x13b000fcUL


/***********************************************************
* Register Name : NB_SMN_INDEX_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_0_NB_SMN_INDEX_0_OFFSET      0
#define NB_SMN_INDEX_0_NB_SMN_INDEX_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_0:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_0_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_0_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_0_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_0_OFFSET       0x60
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x60)

#define SMN_NB_SMN_INDEX_0_ADDRESS    0x13b00060UL


/***********************************************************
* Register Name : NB_SMN_INDEX_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_1_NB_SMN_INDEX_1_OFFSET      0
#define NB_SMN_INDEX_1_NB_SMN_INDEX_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_1:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_1_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_1_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_1_OFFSET       0xa0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa0)

#define SMN_NB_SMN_INDEX_1_ADDRESS    0x13b000a0UL


/***********************************************************
* Register Name : NB_SMN_INDEX_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_2_NB_SMN_INDEX_2_OFFSET      0
#define NB_SMN_INDEX_2_NB_SMN_INDEX_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_2:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_2_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_2_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_2_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_2_OFFSET       0xb8
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb8)

#define SMN_NB_SMN_INDEX_2_ADDRESS    0x13b000b8UL


/***********************************************************
* Register Name : NB_SMN_INDEX_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_3_NB_SMN_INDEX_3_OFFSET      0
#define NB_SMN_INDEX_3_NB_SMN_INDEX_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_3:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_3_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_3_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_3_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_3_OFFSET       0xc4
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc4)

#define SMN_NB_SMN_INDEX_3_ADDRESS    0x13b000c4UL


/***********************************************************
* Register Name : NB_SMN_INDEX_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_4_NB_SMN_INDEX_4_OFFSET      0
#define NB_SMN_INDEX_4_NB_SMN_INDEX_4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_4:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_4_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_4_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_4_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_4_OFFSET       0xd0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd0)

#define SMN_NB_SMN_INDEX_4_ADDRESS    0x13b000d0UL


/***********************************************************
* Register Name : NB_SMN_INDEX_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_5_NB_SMN_INDEX_5_OFFSET      0
#define NB_SMN_INDEX_5_NB_SMN_INDEX_5_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_5:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_5_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_5_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_5_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_5_OFFSET       0xe0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xe0)

#define SMN_NB_SMN_INDEX_5_ADDRESS    0x13b000e0UL


/***********************************************************
* Register Name : NB_SMN_INDEX_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Index value for SMN Index/Data pair access. Register access using these reg pairs will have security level 7.
#define NB_SMN_INDEX_6_NB_SMN_INDEX_6_OFFSET      0
#define NB_SMN_INDEX_6_NB_SMN_INDEX_6_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_6:32;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_6_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_6_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_6_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_6_OFFSET       0xf8
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xf8)

#define SMN_NB_SMN_INDEX_6_ADDRESS    0x13b000f8UL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of RSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_0_NB_SMN_INDEX_EXTENSION_0_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_0_NB_SMN_INDEX_EXTENSION_0_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_0_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_0_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_0:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_0_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_0_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_0_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_0_OFFSET       0x5c
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x5c)

#define SMN_NB_SMN_INDEX_EXTENSION_0_ADDRESS    0x13b0005cUL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of RSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_1_NB_SMN_INDEX_EXTENSION_1_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_1_NB_SMN_INDEX_EXTENSION_1_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_1_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_1_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_1:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_1_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_1_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_1_OFFSET       0x9c
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x9c)

#define SMN_NB_SMN_INDEX_EXTENSION_1_ADDRESS    0x13b0009cUL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of RSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_2_NB_SMN_INDEX_EXTENSION_2_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_2_NB_SMN_INDEX_EXTENSION_2_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_2_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_2_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_2:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_2_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_2_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_2_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_2_OFFSET       0xb4
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb4)

#define SMN_NB_SMN_INDEX_EXTENSION_2_ADDRESS    0x13b000b4UL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of rSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_3_NB_SMN_INDEX_EXTENSION_3_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_3_NB_SMN_INDEX_EXTENSION_3_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_3_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_3_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_3:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_3_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_3_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_3_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_3_OFFSET       0xc0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc0)

#define SMN_NB_SMN_INDEX_EXTENSION_3_ADDRESS    0x13b000c0UL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of RSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_4_NB_SMN_INDEX_EXTENSION_4_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_4_NB_SMN_INDEX_EXTENSION_4_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_4_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_4_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_4:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_4_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_4_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_4_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_4_OFFSET       0xcc
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xcc)

#define SMN_NB_SMN_INDEX_EXTENSION_4_ADDRESS    0x13b000ccUL


/***********************************************************
* Register Name : NB_SMN_INDEX_EXTENSION_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit [35:32] of RSMU address access providing MCM Address [3:0] for SMN Index/Data pair access.
#define NB_SMN_INDEX_EXTENSION_5_NB_SMN_INDEX_EXTENSION_5_OFFSET      0
#define NB_SMN_INDEX_EXTENSION_5_NB_SMN_INDEX_EXTENSION_5_MASK        0xf

// Bitfield Description : 
#define NB_SMN_INDEX_EXTENSION_5_Reserved_31_4_OFFSET      4
#define NB_SMN_INDEX_EXTENSION_5_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            NB_SMN_INDEX_EXTENSION_5:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} NB_SMN_INDEX_EXTENSION_5_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_5_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_5_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_5_OFFSET       0xdc
#define PCICFG_NBIO_IOHUB_NB_SMN_INDEX_EXTENSION_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xdc)

#define SMN_NB_SMN_INDEX_EXTENSION_5_ADDRESS    0x13b000dcUL


/***********************************************************
* Register Name : NB_STATUS
* Register Description :
* Status Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NB_STATUS_Reserved_3_0_OFFSET      0
#define NB_STATUS_Reserved_3_0_MASK        0xf

// Bitfield Description : Capabilities List.
// This bit is always set to 0 to indicate that this device's configuration space does not support a capabilities list.
#define NB_STATUS_CAP_LIST_OFFSET      4
#define NB_STATUS_CAP_LIST_MASK        0x10

// Bitfield Description : 
#define NB_STATUS_Reserved_11_5_OFFSET      5
#define NB_STATUS_Reserved_11_5_MASK        0xfe0

// Bitfield Description : Status bits to indicate abnormal host completions.
#define NB_STATUS_RECEIVED_TARGET_ABORT_OFFSET      12
#define NB_STATUS_RECEIVED_TARGET_ABORT_MASK        0x1000

// Bitfield Description : Status bits to indicate abnormal host completions.
#define NB_STATUS_RECEIVED_MASTER_ABORT_OFFSET      13
#define NB_STATUS_RECEIVED_MASTER_ABORT_MASK        0x2000

// Bitfield Description : 
#define NB_STATUS_Reserved_15_14_OFFSET      14
#define NB_STATUS_Reserved_15_14_MASK        0xc000

typedef union {
  struct {
    UINT16                            Reserved_3_0:4;
    UINT16                            CAP_LIST:1;
    UINT16                            Reserved_11_5:7;
    UINT16                            RECEIVED_TARGET_ABORT:1;
    UINT16                            RECEIVED_MASTER_ABORT:1;
    UINT16                            Reserved_15_14:2;
  } Field;
  UINT16 Value;
} NB_STATUS_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_STATUS_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_STATUS_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_STATUS_OFFSET       0x6
#define PCICFG_NBIO_IOHUB_NB_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6)

#define SMN_NB_STATUS_ADDRESS    0x13b00006UL


/***********************************************************
* Register Name : NB_SUB_CLASS
* Register Description :
* Sub-Class Code Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sub-class code.
#define NB_SUB_CLASS_SUB_CLASS_INF_OFFSET      0
#define NB_SUB_CLASS_SUB_CLASS_INF_MASK        0xff

typedef union {
  struct {
    UINT8                            SUB_CLASS_INF:8;
  } Field;
  UINT8 Value;
} NB_SUB_CLASS_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_SUB_CLASS_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_IOHUB_NB_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa)

#define SMN_NB_SUB_CLASS_ADDRESS    0x13b0000aUL


/***********************************************************
* Register Name : NB_TOP_OF_DRAM3
* Register Description :
* Top of Memory 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TOM3 defines the top of physical memory between 40-bit and 52-bit space inclusive.
// The register contains bit 51:22.
// Lower address bits are implicitly set to 1.
#define NB_TOP_OF_DRAM3_TOM3_LIMIT_OFFSET      0
#define NB_TOP_OF_DRAM3_TOM3_LIMIT_MASK        0x3fffffff

// Bitfield Description : 
#define NB_TOP_OF_DRAM3_Reserved_30_30_OFFSET      30
#define NB_TOP_OF_DRAM3_Reserved_30_30_MASK        0x40000000

// Bitfield Description : Enable TOM3 for memory space decoding.
#define NB_TOP_OF_DRAM3_TOM3_ENABLE_OFFSET      31
#define NB_TOP_OF_DRAM3_TOM3_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            TOM3_LIMIT:30;
    UINT32                            Reserved_30_30:1;
    UINT32                            TOM3_ENABLE:1;
  } Field;
  UINT32 Value;
} NB_TOP_OF_DRAM3_STRUCT;

#define SMN_NB_TOP_OF_DRAM3_ADDRESS    0x13b10138UL


/***********************************************************
* Register Name : NB_TOP_OF_DRAM_SLOT1
* Register Description :
* North Bridge Memory Base Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies bit 32 for the upper limit address that divides between MMIO and DRAM. For addresses below 4G and not included in NB_DRAM_SLOT1_BASE to NB_DRAM_SLOT1_LIMIT is MMIO, between the range is DRAM:BIOS should write to this field following completion of the memory sizing algorithm, after it has determined the total size of the installed memory.
#define NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_BIT_32_OFFSET      0
#define NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_BIT_32_MASK        0x1

// Bitfield Description : 
#define NB_TOP_OF_DRAM_SLOT1_Reserved_22_1_OFFSET      1
#define NB_TOP_OF_DRAM_SLOT1_Reserved_22_1_MASK        0x7ffffe

// Bitfield Description : Specifies bit 32 for the upper limit address that divides between MMIO and DRAM. For addresses below 4G and not included in NB_DRAM_SLOT1_BASE to NB_DRAM_SLOT1_LIMIT is MMIO, between the range is DRAM:BIOS should write to this field following completion of the memory sizing algorithm, after it has determined the total size of the installed memory.
#define NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_OFFSET      23
#define NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_MASK        0xff800000

typedef union {
  struct {
    UINT32                            TOP_OF_DRAM_BIT_32:1;
    UINT32                            Reserved_22_1:22;
    UINT32                            TOP_OF_DRAM:9;
  } Field;
  UINT32 Value;
} NB_TOP_OF_DRAM_SLOT1_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_OFFSET       0x90
#define PCICFG_NBIO_IOHUB_NB_TOP_OF_DRAM_SLOT1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x90)

#define SMN_NB_TOP_OF_DRAM_SLOT1_ADDRESS    0x13b00090UL


/***********************************************************
* Register Name : NB_UPPER_DRAM2_BASE
* Register Description :
* Memory 2 Base High
* Visibility : 0x2
************************************************************/

// Bitfield Description : 4GB for upstream read and write transactions that are forwarded to the host bridge (from the GNB to the CNB). All addresses less than this system address are forwarded to DRAM and are not checked to determine if the transaction is a peer-to-peer transaction.  All upstream reads with addresses greater than this system address are master aborted.
#define NB_UPPER_DRAM2_BASE_UPPER_DRAM2_BASE_OFFSET      0
#define NB_UPPER_DRAM2_BASE_UPPER_DRAM2_BASE_MASK        0x1ff

// Bitfield Description : 
#define NB_UPPER_DRAM2_BASE_Reserved_31_9_OFFSET      9
#define NB_UPPER_DRAM2_BASE_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UPPER_DRAM2_BASE:9;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} NB_UPPER_DRAM2_BASE_STRUCT;

#define SMN_NB_UPPER_DRAM2_BASE_ADDRESS    0x13b10070UL


/***********************************************************
* Register Name : NB_UPPER_TOP_OF_DRAM2
* Register Description :
* Top of Memory 2 High.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TOM2[40:32]. See IOHC::NB_LOWER_TOP_OF_DRAM2.
#define NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_OFFSET      0
#define NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_MASK        0x1ff

// Bitfield Description : 
#define NB_UPPER_TOP_OF_DRAM2_Reserved_31_9_OFFSET      9
#define NB_UPPER_TOP_OF_DRAM2_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UPPER_TOM2:9;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} NB_UPPER_TOP_OF_DRAM2_STRUCT;

#define SMN_NB_UPPER_TOP_OF_DRAM2_ADDRESS    0x13b10068UL


/***********************************************************
* Register Name : NB_VENDOR_ID
* Register Description :
* Vendor ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor Identifier
// This 16-bit field identifies the manufacturer of the device.
#define NB_VENDOR_ID_VENDOR_ID_OFFSET      0
#define NB_VENDOR_ID_VENDOR_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            VENDOR_ID:16;
  } Field;
  UINT16 Value;
} NB_VENDOR_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_NB_VENDOR_ID_ADDRESS    0x13b00000UL


/***********************************************************
* Register Name : NB_VENDOR_ID_W
* Register Description :
* Vendor ID Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor Identifier
// This 16-bit field identifies the manufacturer of the device.
#define NB_VENDOR_ID_W_VENDOR_ID_OFFSET      0
#define NB_VENDOR_ID_W_VENDOR_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            VENDOR_ID:16;
  } Field;
  UINT16 Value;
} NB_VENDOR_ID_W_STRUCT;

#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_W_FUNC         0
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_W_OFFSET       0x100
#define PCICFG_NBIO_IOHUB_NB_VENDOR_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x100)

#define SMN_NB_VENDOR_ID_W_ADDRESS    0x13b00100UL


/***********************************************************
* Register Name : NMI_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit indicates the status of an NMI triggered by the pin.
#define NMI_STATUS_NMIFromPin_OFFSET      0
#define NMI_STATUS_NMIFromPin_MASK        0x1

// Bitfield Description : 
#define NMI_STATUS_Reserved_31_1_OFFSET      1
#define NMI_STATUS_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            NMIFromPin:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} NMI_STATUS_IOHC_STRUCT;

#define SMN_NMI_STATUS_ADDRESS    0x13b20804UL


/***********************************************************
* Register Name : PARITY_CONTROL_0
* Register Description :
* Parity Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies the number of correctable parity errors logged before an action is triggered. The errors are counted per parity error group which may encompass multiple errors from multiple structures.
#define PARITY_CONTROL_0_ParityCorrThreshold_OFFSET      0
#define PARITY_CONTROL_0_ParityCorrThreshold_MASK        0xffff

// Bitfield Description : Specifies the number of ucp parity errors logged before an action is triggered. The errors are counted per parity error group which may encompass multiple errors from multiple structures.
#define PARITY_CONTROL_0_ParityUCPThreshold_OFFSET      16
#define PARITY_CONTROL_0_ParityUCPThreshold_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ParityCorrThreshold:16;
    UINT32                            ParityUCPThreshold:16;
  } Field;
  UINT32 Value;
} PARITY_CONTROL_0_STRUCT;

#define SMN_PARITY_CONTROL_0_ADDRESS    0x13b20000UL


/***********************************************************
* Register Name : PARITY_CONTROL_1
* Register Description :
* Parity Error Injection Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the parity error group that is the target of error injection or masking.
// Software must wait for the ParityErrGenInjectAllow bit to be asserted before updating this register.
#define PARITY_CONTROL_1_ParityErrGenGroupSel_OFFSET      0
#define PARITY_CONTROL_1_ParityErrGenGroupSel_MASK        0xff

// Bitfield Description : Select Group Type:
// 0 - Uncorrectable Group Type
// 1 - Correctable Group Type.
// Software must wait for the ParityErrGenInjectAllow bit to be asserted before updating this register.
// For ECC error injection, this field is a don't care.
#define PARITY_CONTROL_1_ParityErrGenGroupTypeSel_OFFSET      8
#define PARITY_CONTROL_1_ParityErrGenGroupTypeSel_MASK        0x100

// Bitfield Description : 
#define PARITY_CONTROL_1_Reserved_10_9_OFFSET      9
#define PARITY_CONTROL_1_Reserved_10_9_MASK        0x600

// Bitfield Description : Selects a structure within the target parity group for error injection or masking. Software must wait for the ParityErrGenInjectAllow bit to be asserted before updating this register.
#define PARITY_CONTROL_1_ParityErrGenIdSel_OFFSET      11
#define PARITY_CONTROL_1_ParityErrGenIdSel_MASK        0xf800

// Bitfield Description : Software must wait for the ParityErrGenInjectAllow bit to be asserted before updating this register.
// Ecoding for Uncorrectable and Correctable Parity Wrappers:
// 0000 - Reserved
// 0001 - Error Injection on Write Side
// 0010 - Error Injection on Read Side
// 0011 - Reserved
// 0100 - Disable Error Reporting
// 0101 - Enable Error Reporting
// 0110 - Reserved
// 0111-1111 - Reserved
// Encoding for ECC Parity Wrappers:
// 0000 - Disable ECC Error Detection and Correction
// 0001 - Enable ECC Error Detection and Correction
// 0010 - SEC Error Injection
// 0011 - Disable SEC Error Reporting
// 0100 - Enable SEC Error Reporting
// 0101 - DED Error Injection
// 0110 - Disable DED Error Reporting
// 0111 - Enable DED Error Reporting
// 1000 -  Disable Conversion of DED Errors to Poisoned Data
// 1001 - Enable Conversion of DED Errors to Poisoned Data
// 1010 - UCP Error Injection
// 1011 - Disable UCP Error Reporting
// 1100 - Enable UCP Error Reporting
// 1101-1111 - Reserved
#define PARITY_CONTROL_1_ParityErrGenCmd_OFFSET      16
#define PARITY_CONTROL_1_ParityErrGenCmd_MASK        0xf0000

// Bitfield Description : 
#define PARITY_CONTROL_1_Reserved_29_20_OFFSET      20
#define PARITY_CONTROL_1_Reserved_29_20_MASK        0x3ff00000

// Bitfield Description : Writing this bit to 1 triggers the command defined by the other ParityErrGen* fields in this register, doesn't require clear the bits before next write.
#define PARITY_CONTROL_1_ParityErrGenTrigger_OFFSET      30
#define PARITY_CONTROL_1_ParityErrGenTrigger_MASK        0x40000000

// Bitfield Description : Software must poll this bit and make sure this bit is asserted before updating the rest of the fields in this register. When asserted, it indicates that there are no pending ErrGen commands outstanding.
#define PARITY_CONTROL_1_ParityErrGenInjectAllow_OFFSET      31
#define PARITY_CONTROL_1_ParityErrGenInjectAllow_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ParityErrGenGroupSel:8;
    UINT32                            ParityErrGenGroupTypeSel:1;
    UINT32                            Reserved_10_9:2;
    UINT32                            ParityErrGenIdSel:5;
    UINT32                            ParityErrGenCmd:4;
    UINT32                            Reserved_29_20:10;
    UINT32                            ParityErrGenTrigger:1;
    UINT32                            ParityErrGenInjectAllow:1;
  } Field;
  UINT32 Value;
} PARITY_CONTROL_1_STRUCT;

#define SMN_PARITY_CONTROL_1_ADDRESS    0x13b20004UL


/***********************************************************
* Register Name : PARITY_COUNTER_CORR_GRP
* Register Description :
* Threshold counters for correctable parity groups 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Correctable error counter associated with group 0. This counter is compared against ParityCorrThreshold before triggering an action.
#define PARITY_COUNTER_CORR_GRP_ThresholdCounter_OFFSET      0
#define PARITY_COUNTER_CORR_GRP_ThresholdCounter_MASK        0xffff

// Bitfield Description : 
#define PARITY_COUNTER_CORR_GRP_Reserved_30_16_OFFSET      16
#define PARITY_COUNTER_CORR_GRP_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : Reset ThresholdCounter to 0.
// This bit toggles back to 0 the next cycle after the write.
#define PARITY_COUNTER_CORR_GRP_ResetEn_OFFSET      31
#define PARITY_COUNTER_CORR_GRP_ResetEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ThresholdCounter:16;
    UINT32                            Reserved_30_16:15;
    UINT32                            ResetEn:1;
  } Field;
  UINT32 Value;
} PARITY_COUNTER_CORR_GRP_STRUCT;

#define SMN_N0_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20078UL

#define SMN_N1_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b2007cUL

#define SMN_N2_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20080UL

#define SMN_N3_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20084UL

#define SMN_N4_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20088UL

#define SMN_N5_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b2008cUL

#define SMN_N6_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20090UL

#define SMN_N7_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20094UL

#define SMN_N8_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b20098UL

#define SMN_N9_PARITY_COUNTER_CORR_GRP_ADDRESS    0x13b2009cUL


/***********************************************************
* Register Name : PARITY_COUNTER_UCP_GRP
* Register Description :
* Threshold counters for ucp parity groups 0
* Visibility : 0x2
************************************************************/

// Bitfield Description : Uncorrectable error counter associated with group 0. This counter is compared against ParityUCPThreshold before triggering an action.
#define PARITY_COUNTER_UCP_GRP_ThresholdCounter_OFFSET      0
#define PARITY_COUNTER_UCP_GRP_ThresholdCounter_MASK        0xffff

// Bitfield Description : 
#define PARITY_COUNTER_UCP_GRP_Reserved_30_16_OFFSET      16
#define PARITY_COUNTER_UCP_GRP_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : Reset ThresholdCounter to 0.
// This bit toggles back to 1'b0 the next cycle after the write.
#define PARITY_COUNTER_UCP_GRP_ResetEn_OFFSET      31
#define PARITY_COUNTER_UCP_GRP_ResetEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ThresholdCounter:16;
    UINT32                            Reserved_30_16:15;
    UINT32                            ResetEn:1;
  } Field;
  UINT32 Value;
} PARITY_COUNTER_UCP_GRP_STRUCT;

#define SMN_N0_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200c8UL

#define SMN_N1_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200ccUL

#define SMN_N2_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200d0UL

#define SMN_N3_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200d4UL

#define SMN_N4_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200d8UL

#define SMN_N5_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200dcUL

#define SMN_N6_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200e0UL

#define SMN_N7_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200e4UL

#define SMN_N8_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200e8UL

#define SMN_N9_PARITY_COUNTER_UCP_GRP_ADDRESS    0x13b200ecUL


/***********************************************************
* Register Name : PARITY_ERROR_STATUS_CORR_GRP
* Register Description :
* Parity Error status Register for correctable parity group 0. Not all bits in this register may be in use.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 0. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id0_OFFSET      0
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id0_MASK        0x1

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 1. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id1_OFFSET      1
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id1_MASK        0x2

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 2. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id2_OFFSET      2
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id2_MASK        0x4

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 3. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id3_OFFSET      3
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id3_MASK        0x8

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 4. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id4_OFFSET      4
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id4_MASK        0x10

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 5. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id5_OFFSET      5
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id5_MASK        0x20

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 6. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id6_OFFSET      6
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id6_MASK        0x40

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 7. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id7_OFFSET      7
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id7_MASK        0x80

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 8. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id8_OFFSET      8
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id8_MASK        0x100

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 9. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id9_OFFSET      9
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id9_MASK        0x200

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 10. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id10_OFFSET      10
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id10_MASK        0x400

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 11. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id11_OFFSET      11
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id11_MASK        0x800

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 12. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id12_OFFSET      12
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id12_MASK        0x1000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 13. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id13_OFFSET      13
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id13_MASK        0x2000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 14. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id14_OFFSET      14
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id14_MASK        0x4000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 15. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id15_OFFSET      15
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id15_MASK        0x8000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 16. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id16_OFFSET      16
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id16_MASK        0x10000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 17. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id17_OFFSET      17
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id17_MASK        0x20000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 18. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id18_OFFSET      18
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id18_MASK        0x40000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 19. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id19_OFFSET      19
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id19_MASK        0x80000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 20. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id20_OFFSET      20
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id20_MASK        0x100000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 21. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id21_OFFSET      21
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id21_MASK        0x200000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 22. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id22_OFFSET      22
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id22_MASK        0x400000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 23. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id23_OFFSET      23
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id23_MASK        0x800000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 24. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id24_OFFSET      24
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id24_MASK        0x1000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 25. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id25_OFFSET      25
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id25_MASK        0x2000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 26. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id26_OFFSET      26
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id26_MASK        0x4000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 27. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id27_OFFSET      27
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id27_MASK        0x8000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 28. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id28_OFFSET      28
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id28_MASK        0x10000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 29. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id29_OFFSET      29
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id29_MASK        0x20000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 30. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id30_OFFSET      30
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id30_MASK        0x40000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of correctable ECC/Parity errors from group 0 id 31. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id31_OFFSET      31
#define PARITY_ERROR_STATUS_CORR_GRP_ParityErrDetected_Id31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ParityErrDetected_Id0:1;
    UINT32                            ParityErrDetected_Id1:1;
    UINT32                            ParityErrDetected_Id2:1;
    UINT32                            ParityErrDetected_Id3:1;
    UINT32                            ParityErrDetected_Id4:1;
    UINT32                            ParityErrDetected_Id5:1;
    UINT32                            ParityErrDetected_Id6:1;
    UINT32                            ParityErrDetected_Id7:1;
    UINT32                            ParityErrDetected_Id8:1;
    UINT32                            ParityErrDetected_Id9:1;
    UINT32                            ParityErrDetected_Id10:1;
    UINT32                            ParityErrDetected_Id11:1;
    UINT32                            ParityErrDetected_Id12:1;
    UINT32                            ParityErrDetected_Id13:1;
    UINT32                            ParityErrDetected_Id14:1;
    UINT32                            ParityErrDetected_Id15:1;
    UINT32                            ParityErrDetected_Id16:1;
    UINT32                            ParityErrDetected_Id17:1;
    UINT32                            ParityErrDetected_Id18:1;
    UINT32                            ParityErrDetected_Id19:1;
    UINT32                            ParityErrDetected_Id20:1;
    UINT32                            ParityErrDetected_Id21:1;
    UINT32                            ParityErrDetected_Id22:1;
    UINT32                            ParityErrDetected_Id23:1;
    UINT32                            ParityErrDetected_Id24:1;
    UINT32                            ParityErrDetected_Id25:1;
    UINT32                            ParityErrDetected_Id26:1;
    UINT32                            ParityErrDetected_Id27:1;
    UINT32                            ParityErrDetected_Id28:1;
    UINT32                            ParityErrDetected_Id29:1;
    UINT32                            ParityErrDetected_Id30:1;
    UINT32                            ParityErrDetected_Id31:1;
  } Field;
  UINT32 Value;
} PARITY_ERROR_STATUS_CORR_GRP_STRUCT;

#define SMN_N0_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20050UL

#define SMN_N1_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20054UL

#define SMN_N2_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20058UL

#define SMN_N3_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b2005cUL

#define SMN_N4_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20060UL

#define SMN_N5_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20064UL

#define SMN_N6_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20068UL

#define SMN_N7_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b2006cUL

#define SMN_N8_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20070UL

#define SMN_N9_PARITY_ERROR_STATUS_CORR_GRP_ADDRESS    0x13b20074UL


/***********************************************************
* Register Name : PARITY_ERROR_STATUS_UCP_GRP
* Register Description :
* Parity Error status Register for uncorrectable coverts to poison data parity group 0. Not all bits in this register may be in use.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 0. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id0_OFFSET      0
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id0_MASK        0x1

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 1. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id1_OFFSET      1
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id1_MASK        0x2

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 2. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id2_OFFSET      2
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id2_MASK        0x4

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 3. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id3_OFFSET      3
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id3_MASK        0x8

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 4. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id4_OFFSET      4
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id4_MASK        0x10

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 5. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id5_OFFSET      5
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id5_MASK        0x20

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 6. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id6_OFFSET      6
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id6_MASK        0x40

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 7. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id7_OFFSET      7
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id7_MASK        0x80

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 8. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id8_OFFSET      8
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id8_MASK        0x100

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 9. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id9_OFFSET      9
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id9_MASK        0x200

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 10. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id10_OFFSET      10
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id10_MASK        0x400

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 11. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id11_OFFSET      11
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id11_MASK        0x800

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 12. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id12_OFFSET      12
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id12_MASK        0x1000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 13. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id13_OFFSET      13
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id13_MASK        0x2000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 14. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id14_OFFSET      14
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id14_MASK        0x4000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 15. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id15_OFFSET      15
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id15_MASK        0x8000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 16. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id16_OFFSET      16
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id16_MASK        0x10000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 17. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id17_OFFSET      17
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id17_MASK        0x20000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 18. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id18_OFFSET      18
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id18_MASK        0x40000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 19. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id19_OFFSET      19
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id19_MASK        0x80000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 20. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id20_OFFSET      20
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id20_MASK        0x100000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 21. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id21_OFFSET      21
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id21_MASK        0x200000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 22. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id22_OFFSET      22
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id22_MASK        0x400000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 23. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id23_OFFSET      23
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id23_MASK        0x800000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 24. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id24_OFFSET      24
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id24_MASK        0x1000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 25. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id25_OFFSET      25
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id25_MASK        0x2000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 26. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id26_OFFSET      26
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id26_MASK        0x4000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 27. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id27_OFFSET      27
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id27_MASK        0x8000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 28. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id28_OFFSET      28
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id28_MASK        0x10000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 29. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id29_OFFSET      29
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id29_MASK        0x20000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 30. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id30_OFFSET      30
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id30_MASK        0x40000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable errors converted to poisoned data in group 0 id 31. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id31_OFFSET      31
#define PARITY_ERROR_STATUS_UCP_GRP_ParityErrDetected_Id31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ParityErrDetected_Id0:1;
    UINT32                            ParityErrDetected_Id1:1;
    UINT32                            ParityErrDetected_Id2:1;
    UINT32                            ParityErrDetected_Id3:1;
    UINT32                            ParityErrDetected_Id4:1;
    UINT32                            ParityErrDetected_Id5:1;
    UINT32                            ParityErrDetected_Id6:1;
    UINT32                            ParityErrDetected_Id7:1;
    UINT32                            ParityErrDetected_Id8:1;
    UINT32                            ParityErrDetected_Id9:1;
    UINT32                            ParityErrDetected_Id10:1;
    UINT32                            ParityErrDetected_Id11:1;
    UINT32                            ParityErrDetected_Id12:1;
    UINT32                            ParityErrDetected_Id13:1;
    UINT32                            ParityErrDetected_Id14:1;
    UINT32                            ParityErrDetected_Id15:1;
    UINT32                            ParityErrDetected_Id16:1;
    UINT32                            ParityErrDetected_Id17:1;
    UINT32                            ParityErrDetected_Id18:1;
    UINT32                            ParityErrDetected_Id19:1;
    UINT32                            ParityErrDetected_Id20:1;
    UINT32                            ParityErrDetected_Id21:1;
    UINT32                            ParityErrDetected_Id22:1;
    UINT32                            ParityErrDetected_Id23:1;
    UINT32                            ParityErrDetected_Id24:1;
    UINT32                            ParityErrDetected_Id25:1;
    UINT32                            ParityErrDetected_Id26:1;
    UINT32                            ParityErrDetected_Id27:1;
    UINT32                            ParityErrDetected_Id28:1;
    UINT32                            ParityErrDetected_Id29:1;
    UINT32                            ParityErrDetected_Id30:1;
    UINT32                            ParityErrDetected_Id31:1;
  } Field;
  UINT32 Value;
} PARITY_ERROR_STATUS_UCP_GRP_STRUCT;

#define SMN_N0_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200a0UL

#define SMN_N1_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200a4UL

#define SMN_N2_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200a8UL

#define SMN_N3_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200acUL

#define SMN_N4_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200b0UL

#define SMN_N5_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200b4UL

#define SMN_N6_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200b8UL

#define SMN_N7_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200bcUL

#define SMN_N8_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200c0UL

#define SMN_N9_PARITY_ERROR_STATUS_UCP_GRP_ADDRESS    0x13b200c4UL


/***********************************************************
* Register Name : PARITY_ERROR_STATUS_UNCORR_GRP
* Register Description :
* Parity Error status Register for uncorrectable parity group 0. Not all bits in this register may be in use.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 0. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id0_OFFSET      0
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id0_MASK        0x1

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 1. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id1_OFFSET      1
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id1_MASK        0x2

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 2. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id2_OFFSET      2
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id2_MASK        0x4

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 3. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id3_OFFSET      3
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id3_MASK        0x8

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 4. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id4_OFFSET      4
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id4_MASK        0x10

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 5. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id5_OFFSET      5
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id5_MASK        0x20

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 6. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id6_OFFSET      6
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id6_MASK        0x40

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 7. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id7_OFFSET      7
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id7_MASK        0x80

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 8. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id8_OFFSET      8
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id8_MASK        0x100

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 9. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id9_OFFSET      9
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id9_MASK        0x200

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 10. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id10_OFFSET      10
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id10_MASK        0x400

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 11. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id11_OFFSET      11
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id11_MASK        0x800

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 12. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id12_OFFSET      12
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id12_MASK        0x1000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 13. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id13_OFFSET      13
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id13_MASK        0x2000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 14. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id14_OFFSET      14
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id14_MASK        0x4000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 15. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id15_OFFSET      15
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id15_MASK        0x8000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 16. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id16_OFFSET      16
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id16_MASK        0x10000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 17. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id17_OFFSET      17
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id17_MASK        0x20000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 18. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id18_OFFSET      18
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id18_MASK        0x40000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 19. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id19_OFFSET      19
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id19_MASK        0x80000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 20. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id20_OFFSET      20
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id20_MASK        0x100000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 21. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id21_OFFSET      21
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id21_MASK        0x200000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 22. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id22_OFFSET      22
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id22_MASK        0x400000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 23. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id23_OFFSET      23
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id23_MASK        0x800000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 24. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id24_OFFSET      24
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id24_MASK        0x1000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 25. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id25_OFFSET      25
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id25_MASK        0x2000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 26. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id26_OFFSET      26
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id26_MASK        0x4000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 27. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id27_OFFSET      27
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id27_MASK        0x8000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 28. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id28_OFFSET      28
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id28_MASK        0x10000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 29. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id29_OFFSET      29
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id29_MASK        0x20000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 30. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id30_OFFSET      30
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id30_MASK        0x40000000

// Bitfield Description : Write-1-to-clear This bit indicates the status of uncorrectable ECC/Parity errors from group 0 id 31. The value is preserved across a warm reset.
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id31_OFFSET      31
#define PARITY_ERROR_STATUS_UNCORR_GRP_ParityErrDetected_Id31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ParityErrDetected_Id0:1;
    UINT32                            ParityErrDetected_Id1:1;
    UINT32                            ParityErrDetected_Id2:1;
    UINT32                            ParityErrDetected_Id3:1;
    UINT32                            ParityErrDetected_Id4:1;
    UINT32                            ParityErrDetected_Id5:1;
    UINT32                            ParityErrDetected_Id6:1;
    UINT32                            ParityErrDetected_Id7:1;
    UINT32                            ParityErrDetected_Id8:1;
    UINT32                            ParityErrDetected_Id9:1;
    UINT32                            ParityErrDetected_Id10:1;
    UINT32                            ParityErrDetected_Id11:1;
    UINT32                            ParityErrDetected_Id12:1;
    UINT32                            ParityErrDetected_Id13:1;
    UINT32                            ParityErrDetected_Id14:1;
    UINT32                            ParityErrDetected_Id15:1;
    UINT32                            ParityErrDetected_Id16:1;
    UINT32                            ParityErrDetected_Id17:1;
    UINT32                            ParityErrDetected_Id18:1;
    UINT32                            ParityErrDetected_Id19:1;
    UINT32                            ParityErrDetected_Id20:1;
    UINT32                            ParityErrDetected_Id21:1;
    UINT32                            ParityErrDetected_Id22:1;
    UINT32                            ParityErrDetected_Id23:1;
    UINT32                            ParityErrDetected_Id24:1;
    UINT32                            ParityErrDetected_Id25:1;
    UINT32                            ParityErrDetected_Id26:1;
    UINT32                            ParityErrDetected_Id27:1;
    UINT32                            ParityErrDetected_Id28:1;
    UINT32                            ParityErrDetected_Id29:1;
    UINT32                            ParityErrDetected_Id30:1;
    UINT32                            ParityErrDetected_Id31:1;
  } Field;
  UINT32 Value;
} PARITY_ERROR_STATUS_UNCORR_GRP_STRUCT;

#define SMN_N0_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20028UL

#define SMN_N1_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b2002cUL

#define SMN_N2_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20030UL

#define SMN_N3_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20034UL

#define SMN_N4_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20038UL

#define SMN_N5_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b2003cUL

#define SMN_N6_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20040UL

#define SMN_N7_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20044UL

#define SMN_N8_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b20048UL

#define SMN_N9_PARITY_ERROR_STATUS_UNCORR_GRP_ADDRESS    0x13b2004cUL


/***********************************************************
* Register Name : PARITY_SEVERITY_CONTROL_CORR_0
* Register Description :
* Parity Correctable Groups Error Severity Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the error severity detected in correctable parity error group 0:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp0_OFFSET      0
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp0_MASK        0x3

// Bitfield Description : Controls the error severity detected in correctable parity error group 1:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp1_OFFSET      2
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp1_MASK        0xc

// Bitfield Description : Controls the error severity detected in correctable parity error group 2:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp2_OFFSET      4
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp2_MASK        0x30

// Bitfield Description : Controls the error severity detected in correctable parity error group 3:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp3_OFFSET      6
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp3_MASK        0xc0

// Bitfield Description : Controls the error severity detected in correctable parity error group 4:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp4_OFFSET      8
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp4_MASK        0x300

// Bitfield Description : Controls the error severity detected in correctable parity error group 5:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp5_OFFSET      10
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp5_MASK        0xc00

// Bitfield Description : Controls the error severity detected in correctable parity error group 6:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp6_OFFSET      12
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp6_MASK        0x3000

// Bitfield Description : Controls the error severity detected in correctable parity error group 7:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp7_OFFSET      14
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp7_MASK        0xc000

// Bitfield Description : Controls the error severity detected in correctable parity error group 8:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp8_OFFSET      16
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp8_MASK        0x30000

// Bitfield Description : Controls the error severity detected in correctable parity error group 9:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp9_OFFSET      18
#define PARITY_SEVERITY_CONTROL_CORR_0_ParityErrSevCorrGrp9_MASK        0xc0000

// Bitfield Description : 
#define PARITY_SEVERITY_CONTROL_CORR_0_Reserved_31_20_OFFSET      20
#define PARITY_SEVERITY_CONTROL_CORR_0_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ParityErrSevCorrGrp0:2;
    UINT32                            ParityErrSevCorrGrp1:2;
    UINT32                            ParityErrSevCorrGrp2:2;
    UINT32                            ParityErrSevCorrGrp3:2;
    UINT32                            ParityErrSevCorrGrp4:2;
    UINT32                            ParityErrSevCorrGrp5:2;
    UINT32                            ParityErrSevCorrGrp6:2;
    UINT32                            ParityErrSevCorrGrp7:2;
    UINT32                            ParityErrSevCorrGrp8:2;
    UINT32                            ParityErrSevCorrGrp9:2;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} PARITY_SEVERITY_CONTROL_CORR_0_STRUCT;

#define SMN_PARITY_SEVERITY_CONTROL_CORR_0_ADDRESS    0x13b20010UL


/***********************************************************
* Register Name : PARITY_SEVERITY_CONTROL_UCP_0
* Register Description :
* Parity UCP Groups Error Severity Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the error severity detected in UCP parity error group 0:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp0_OFFSET      0
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp0_MASK        0x3

// Bitfield Description : Controls the error severity detected in UCP parity error group 1:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp1_OFFSET      2
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp1_MASK        0xc

// Bitfield Description : Controls the error severity detected in UCP parity error group 2:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp2_OFFSET      4
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp2_MASK        0x30

// Bitfield Description : Controls the error severity detected in UCP parity error group 3:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp3_OFFSET      6
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp3_MASK        0xc0

// Bitfield Description : Controls the error severity detected in UCP parity error group 4:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp4_OFFSET      8
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp4_MASK        0x300

// Bitfield Description : Controls the error severity detected in UCP parity error group 5:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp5_OFFSET      10
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp5_MASK        0xc00

// Bitfield Description : Controls the error severity detected in UCP parity error group 6:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp6_OFFSET      12
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp6_MASK        0x3000

// Bitfield Description : Controls the error severity detected in UCP parity error group 7:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp7_OFFSET      14
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp7_MASK        0xc000

// Bitfield Description : Controls the error severity detected in UCP parity error group 8:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp8_OFFSET      16
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp8_MASK        0x30000

// Bitfield Description : Controls the error severity detected in UCP parity error group 9:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp9_OFFSET      18
#define PARITY_SEVERITY_CONTROL_UCP_0_ParityErrSevUCPGrp9_MASK        0xc0000

// Bitfield Description : 
#define PARITY_SEVERITY_CONTROL_UCP_0_Reserved_31_20_OFFSET      20
#define PARITY_SEVERITY_CONTROL_UCP_0_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ParityErrSevUCPGrp0:2;
    UINT32                            ParityErrSevUCPGrp1:2;
    UINT32                            ParityErrSevUCPGrp2:2;
    UINT32                            ParityErrSevUCPGrp3:2;
    UINT32                            ParityErrSevUCPGrp4:2;
    UINT32                            ParityErrSevUCPGrp5:2;
    UINT32                            ParityErrSevUCPGrp6:2;
    UINT32                            ParityErrSevUCPGrp7:2;
    UINT32                            ParityErrSevUCPGrp8:2;
    UINT32                            ParityErrSevUCPGrp9:2;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} PARITY_SEVERITY_CONTROL_UCP_0_STRUCT;

#define SMN_PARITY_SEVERITY_CONTROL_UCP_0_ADDRESS    0x13b20018UL


/***********************************************************
* Register Name : PARITY_SEVERITY_CONTROL_UNCORR_0
* Register Description :
* Parity Uncorrectable Groups Error Severity Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 0:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp0_OFFSET      0
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp0_MASK        0x3

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 1:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp1_OFFSET      2
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp1_MASK        0xc

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 2:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp2_OFFSET      4
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp2_MASK        0x30

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 3:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp3_OFFSET      6
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp3_MASK        0xc0

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 4:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp4_OFFSET      8
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp4_MASK        0x300

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 5:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp5_OFFSET      10
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp5_MASK        0xc00

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 6:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp6_OFFSET      12
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp6_MASK        0x3000

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 7:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp7_OFFSET      14
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp7_MASK        0xc000

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 8:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp8_OFFSET      16
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp8_MASK        0x30000

// Bitfield Description : Controls the error severity detected in uncorrectable parity error group 9:
// 11=System Error
// 10=Fatal
// 01=Non-fatal
// 00=Correctable
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp9_OFFSET      18
#define PARITY_SEVERITY_CONTROL_UNCORR_0_ParityErrSevUnCorrGrp9_MASK        0xc0000

// Bitfield Description : 
#define PARITY_SEVERITY_CONTROL_UNCORR_0_Reserved_31_20_OFFSET      20
#define PARITY_SEVERITY_CONTROL_UNCORR_0_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ParityErrSevUnCorrGrp0:2;
    UINT32                            ParityErrSevUnCorrGrp1:2;
    UINT32                            ParityErrSevUnCorrGrp2:2;
    UINT32                            ParityErrSevUnCorrGrp3:2;
    UINT32                            ParityErrSevUnCorrGrp4:2;
    UINT32                            ParityErrSevUnCorrGrp5:2;
    UINT32                            ParityErrSevUnCorrGrp6:2;
    UINT32                            ParityErrSevUnCorrGrp7:2;
    UINT32                            ParityErrSevUnCorrGrp8:2;
    UINT32                            ParityErrSevUnCorrGrp9:2;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} PARITY_SEVERITY_CONTROL_UNCORR_0_STRUCT;

#define SMN_PARITY_SEVERITY_CONTROL_UNCORR_0_ADDRESS    0x13b20008UL


/***********************************************************
* Register Name : PCIE0PortAExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAExtCorr_ACTION_CONTROL_ADDRESS    0x13b2012cUL


/***********************************************************
* Register Name : PCIE0PortAExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAExtFatal_ACTION_CONTROL_ADDRESS    0x13b20124UL


/***********************************************************
* Register Name : PCIE0PortAExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20128UL


/***********************************************************
* Register Name : PCIE0PortAIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAIntCorr_ACTION_CONTROL_ADDRESS    0x13b20120UL


/***********************************************************
* Register Name : PCIE0PortAIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAIntFatal_ACTION_CONTROL_ADDRESS    0x13b20118UL


/***********************************************************
* Register Name : PCIE0PortAIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2011cUL


/***********************************************************
* Register Name : PCIE0PortAParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortAParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortAParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortAParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortAParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortAParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortAParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortAParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortAParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortAParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortAParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortAParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortAParityErr_ACTION_CONTROL_ADDRESS    0x13b20130UL


/***********************************************************
* Register Name : PCIE0PortASerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortASerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortASerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortASerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortASerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortASerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortASerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortASerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortASerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortASerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortASerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortASerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortASerr_ACTION_CONTROL_ADDRESS    0x13b20114UL


/***********************************************************
* Register Name : PCIE0PortBExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBExtCorr_ACTION_CONTROL_ADDRESS    0x13b2014cUL


/***********************************************************
* Register Name : PCIE0PortBExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBExtFatal_ACTION_CONTROL_ADDRESS    0x13b20144UL


/***********************************************************
* Register Name : PCIE0PortBExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20148UL


/***********************************************************
* Register Name : PCIE0PortBIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBIntCorr_ACTION_CONTROL_ADDRESS    0x13b20140UL


/***********************************************************
* Register Name : PCIE0PortBIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBIntFatal_ACTION_CONTROL_ADDRESS    0x13b20138UL


/***********************************************************
* Register Name : PCIE0PortBIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2013cUL


/***********************************************************
* Register Name : PCIE0PortBParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBParityErr_ACTION_CONTROL_ADDRESS    0x13b20150UL


/***********************************************************
* Register Name : PCIE0PortBSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortBSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortBSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortBSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortBSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortBSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortBSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortBSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortBSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortBSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortBSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortBSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortBSerr_ACTION_CONTROL_ADDRESS    0x13b20134UL


/***********************************************************
* Register Name : PCIE0PortCExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCExtCorr_ACTION_CONTROL_ADDRESS    0x13b2016cUL


/***********************************************************
* Register Name : PCIE0PortCExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCExtFatal_ACTION_CONTROL_ADDRESS    0x13b20164UL


/***********************************************************
* Register Name : PCIE0PortCExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20168UL


/***********************************************************
* Register Name : PCIE0PortCIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCIntCorr_ACTION_CONTROL_ADDRESS    0x13b20160UL


/***********************************************************
* Register Name : PCIE0PortCIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCIntFatal_ACTION_CONTROL_ADDRESS    0x13b20158UL


/***********************************************************
* Register Name : PCIE0PortCIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2015cUL


/***********************************************************
* Register Name : PCIE0PortCParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCParityErr_ACTION_CONTROL_ADDRESS    0x13b20170UL


/***********************************************************
* Register Name : PCIE0PortCSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortCSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortCSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortCSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortCSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortCSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortCSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortCSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortCSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortCSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortCSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortCSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortCSerr_ACTION_CONTROL_ADDRESS    0x13b20154UL


/***********************************************************
* Register Name : PCIE0PortDExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDExtCorr_ACTION_CONTROL_ADDRESS    0x13b2018cUL


/***********************************************************
* Register Name : PCIE0PortDExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDExtFatal_ACTION_CONTROL_ADDRESS    0x13b20184UL


/***********************************************************
* Register Name : PCIE0PortDExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20188UL


/***********************************************************
* Register Name : PCIE0PortDIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDIntCorr_ACTION_CONTROL_ADDRESS    0x13b20180UL


/***********************************************************
* Register Name : PCIE0PortDIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDIntFatal_ACTION_CONTROL_ADDRESS    0x13b20178UL


/***********************************************************
* Register Name : PCIE0PortDIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2017cUL


/***********************************************************
* Register Name : PCIE0PortDParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDParityErr_ACTION_CONTROL_ADDRESS    0x13b20190UL


/***********************************************************
* Register Name : PCIE0PortDSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortDSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortDSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortDSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortDSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortDSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortDSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortDSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortDSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortDSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortDSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortDSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortDSerr_ACTION_CONTROL_ADDRESS    0x13b20174UL


/***********************************************************
* Register Name : PCIE0PortEExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEExtCorr_ACTION_CONTROL_ADDRESS    0x13b201acUL


/***********************************************************
* Register Name : PCIE0PortEExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEExtFatal_ACTION_CONTROL_ADDRESS    0x13b201a4UL


/***********************************************************
* Register Name : PCIE0PortEExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b201a8UL


/***********************************************************
* Register Name : PCIE0PortEIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEIntCorr_ACTION_CONTROL_ADDRESS    0x13b201a0UL


/***********************************************************
* Register Name : PCIE0PortEIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEIntFatal_ACTION_CONTROL_ADDRESS    0x13b20198UL


/***********************************************************
* Register Name : PCIE0PortEIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2019cUL


/***********************************************************
* Register Name : PCIE0PortEParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortEParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortEParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortEParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortEParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortEParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortEParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortEParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortEParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortEParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortEParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortEParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortEParityErr_ACTION_CONTROL_ADDRESS    0x13b201b0UL


/***********************************************************
* Register Name : PCIE0PortESerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE0PortESerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE0PortESerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE0PortESerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE0PortESerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE0PortESerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE0PortESerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE0PortESerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE0PortESerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE0PortESerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE0PortESerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE0PortESerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE0PortESerr_ACTION_CONTROL_ADDRESS    0x13b20194UL


/***********************************************************
* Register Name : PCIE1PortAExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAExtCorr_ACTION_CONTROL_ADDRESS    0x13b201ccUL


/***********************************************************
* Register Name : PCIE1PortAExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAExtFatal_ACTION_CONTROL_ADDRESS    0x13b201c4UL


/***********************************************************
* Register Name : PCIE1PortAExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b201c8UL


/***********************************************************
* Register Name : PCIE1PortAIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAIntCorr_ACTION_CONTROL_ADDRESS    0x13b201c0UL


/***********************************************************
* Register Name : PCIE1PortAIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAIntFatal_ACTION_CONTROL_ADDRESS    0x13b201b8UL


/***********************************************************
* Register Name : PCIE1PortAIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b201bcUL


/***********************************************************
* Register Name : PCIE1PortAParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortAParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortAParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortAParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortAParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortAParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortAParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortAParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortAParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortAParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortAParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortAParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortAParityErr_ACTION_CONTROL_ADDRESS    0x13b201d0UL


/***********************************************************
* Register Name : PCIE1PortASerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortASerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortASerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortASerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortASerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortASerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortASerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortASerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortASerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortASerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortASerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortASerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortASerr_ACTION_CONTROL_ADDRESS    0x13b201b4UL


/***********************************************************
* Register Name : PCIE1PortBExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBExtCorr_ACTION_CONTROL_ADDRESS    0x13b201ecUL


/***********************************************************
* Register Name : PCIE1PortBExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBExtFatal_ACTION_CONTROL_ADDRESS    0x13b201e4UL


/***********************************************************
* Register Name : PCIE1PortBExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b201e8UL


/***********************************************************
* Register Name : PCIE1PortBIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBIntCorr_ACTION_CONTROL_ADDRESS    0x13b201e0UL


/***********************************************************
* Register Name : PCIE1PortBIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBIntFatal_ACTION_CONTROL_ADDRESS    0x13b201d8UL


/***********************************************************
* Register Name : PCIE1PortBIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b201dcUL


/***********************************************************
* Register Name : PCIE1PortBParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBParityErr_ACTION_CONTROL_ADDRESS    0x13b201f0UL


/***********************************************************
* Register Name : PCIE1PortBSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortBSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortBSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortBSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortBSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortBSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortBSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortBSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortBSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortBSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortBSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortBSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortBSerr_ACTION_CONTROL_ADDRESS    0x13b201d4UL


/***********************************************************
* Register Name : PCIE1PortCExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCExtCorr_ACTION_CONTROL_ADDRESS    0x13b2020cUL


/***********************************************************
* Register Name : PCIE1PortCExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCExtFatal_ACTION_CONTROL_ADDRESS    0x13b20204UL


/***********************************************************
* Register Name : PCIE1PortCExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20208UL


/***********************************************************
* Register Name : PCIE1PortCIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCIntCorr_ACTION_CONTROL_ADDRESS    0x13b20200UL


/***********************************************************
* Register Name : PCIE1PortCIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCIntFatal_ACTION_CONTROL_ADDRESS    0x13b201f8UL


/***********************************************************
* Register Name : PCIE1PortCIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b201fcUL


/***********************************************************
* Register Name : PCIE1PortCParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCParityErr_ACTION_CONTROL_ADDRESS    0x13b20210UL


/***********************************************************
* Register Name : PCIE1PortCSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortCSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortCSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortCSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortCSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortCSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortCSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortCSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortCSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortCSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortCSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortCSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortCSerr_ACTION_CONTROL_ADDRESS    0x13b201f4UL


/***********************************************************
* Register Name : PCIE1PortDExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDExtCorr_ACTION_CONTROL_ADDRESS    0x13b2022cUL


/***********************************************************
* Register Name : PCIE1PortDExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDExtFatal_ACTION_CONTROL_ADDRESS    0x13b20224UL


/***********************************************************
* Register Name : PCIE1PortDExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20228UL


/***********************************************************
* Register Name : PCIE1PortDIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDIntCorr_ACTION_CONTROL_ADDRESS    0x13b20220UL


/***********************************************************
* Register Name : PCIE1PortDIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDIntFatal_ACTION_CONTROL_ADDRESS    0x13b20218UL


/***********************************************************
* Register Name : PCIE1PortDIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2021cUL


/***********************************************************
* Register Name : PCIE1PortDParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDParityErr_ACTION_CONTROL_ADDRESS    0x13b20230UL


/***********************************************************
* Register Name : PCIE1PortDSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortDSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortDSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortDSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortDSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortDSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortDSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortDSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortDSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortDSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortDSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortDSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortDSerr_ACTION_CONTROL_ADDRESS    0x13b20214UL


/***********************************************************
* Register Name : PCIE1PortEExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEExtCorr_ACTION_CONTROL_ADDRESS    0x13b2024cUL


/***********************************************************
* Register Name : PCIE1PortEExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEExtFatal_ACTION_CONTROL_ADDRESS    0x13b20244UL


/***********************************************************
* Register Name : PCIE1PortEExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20248UL


/***********************************************************
* Register Name : PCIE1PortEIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEIntCorr_ACTION_CONTROL_ADDRESS    0x13b20240UL


/***********************************************************
* Register Name : PCIE1PortEIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEIntFatal_ACTION_CONTROL_ADDRESS    0x13b20238UL


/***********************************************************
* Register Name : PCIE1PortEIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2023cUL


/***********************************************************
* Register Name : PCIE1PortEParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortEParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortEParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortEParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortEParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortEParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortEParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortEParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortEParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortEParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortEParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortEParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortEParityErr_ACTION_CONTROL_ADDRESS    0x13b20250UL


/***********************************************************
* Register Name : PCIE1PortESerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortESerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortESerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortESerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortESerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortESerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortESerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortESerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortESerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortESerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortESerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortESerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortESerr_ACTION_CONTROL_ADDRESS    0x13b20234UL


/***********************************************************
* Register Name : PCIE1PortFExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFExtCorr_ACTION_CONTROL_ADDRESS    0x13b2026cUL


/***********************************************************
* Register Name : PCIE1PortFExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFExtFatal_ACTION_CONTROL_ADDRESS    0x13b20264UL


/***********************************************************
* Register Name : PCIE1PortFExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20268UL


/***********************************************************
* Register Name : PCIE1PortFIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFIntCorr_ACTION_CONTROL_ADDRESS    0x13b20260UL


/***********************************************************
* Register Name : PCIE1PortFIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFIntFatal_ACTION_CONTROL_ADDRESS    0x13b20258UL


/***********************************************************
* Register Name : PCIE1PortFIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2025cUL


/***********************************************************
* Register Name : PCIE1PortFParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFParityErr_ACTION_CONTROL_ADDRESS    0x13b20270UL


/***********************************************************
* Register Name : PCIE1PortFSerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE1PortFSerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE1PortFSerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE1PortFSerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE1PortFSerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE1PortFSerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE1PortFSerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE1PortFSerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE1PortFSerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE1PortFSerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE1PortFSerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE1PortFSerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE1PortFSerr_ACTION_CONTROL_ADDRESS    0x13b20254UL


/***********************************************************
* Register Name : PCIE2PortAExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAExtCorr_ACTION_CONTROL_ADDRESS    0x13b2028cUL


/***********************************************************
* Register Name : PCIE2PortAExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAExtFatal_ACTION_CONTROL_ADDRESS    0x13b20284UL


/***********************************************************
* Register Name : PCIE2PortAExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b20288UL


/***********************************************************
* Register Name : PCIE2PortAIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAIntCorr_ACTION_CONTROL_ADDRESS    0x13b20280UL


/***********************************************************
* Register Name : PCIE2PortAIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAIntFatal_ACTION_CONTROL_ADDRESS    0x13b20278UL


/***********************************************************
* Register Name : PCIE2PortAIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2027cUL


/***********************************************************
* Register Name : PCIE2PortAParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortAParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortAParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortAParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortAParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortAParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortAParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortAParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortAParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortAParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortAParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortAParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortAParityErr_ACTION_CONTROL_ADDRESS    0x13b20290UL


/***********************************************************
* Register Name : PCIE2PortASerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE2PortASerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE2PortASerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE2PortASerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE2PortASerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE2PortASerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE2PortASerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE2PortASerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE2PortASerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE2PortASerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE2PortASerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE2PortASerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE2PortASerr_ACTION_CONTROL_ADDRESS    0x13b20274UL


/***********************************************************
* Register Name : PCIE3PortAExtCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtCorr_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAExtCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAExtCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAExtCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAExtCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAExtCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAExtCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAExtCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAExtCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAExtCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAExtCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAExtCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAExtCorr_ACTION_CONTROL_ADDRESS    0x13b202acUL


/***********************************************************
* Register Name : PCIE3PortAExtFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAExtFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAExtFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAExtFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAExtFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAExtFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAExtFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAExtFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAExtFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAExtFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAExtFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAExtFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAExtFatal_ACTION_CONTROL_ADDRESS    0x13b202a4UL


/***********************************************************
* Register Name : PCIE3PortAExtNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port receives a ExtNonFatal_ERR message over the PCIe link
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAExtNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAExtNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAExtNonFatal_ACTION_CONTROL_ADDRESS    0x13b202a8UL


/***********************************************************
* Register Name : PCIE3PortAIntCorr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntCorr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAIntCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAIntCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAIntCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAIntCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAIntCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAIntCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAIntCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAIntCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAIntCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAIntCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAIntCorr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAIntCorr_ACTION_CONTROL_ADDRESS    0x13b202a0UL


/***********************************************************
* Register Name : PCIE3PortAIntFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAIntFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAIntFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAIntFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAIntFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAIntFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAIntFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAIntFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAIntFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAIntFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAIntFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAIntFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAIntFatal_ACTION_CONTROL_ADDRESS    0x13b20298UL


/***********************************************************
* Register Name : PCIE3PortAIntNonFatal_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a IntNonFatal error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAIntNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAIntNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAIntNonFatal_ACTION_CONTROL_ADDRESS    0x13b2029cUL


/***********************************************************
* Register Name : PCIE3PortAParityErr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port's primary side ParityErr_DETECTED status is set
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortAParityErr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortAParityErr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortAParityErr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortAParityErr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortAParityErr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortAParityErr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortAParityErr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortAParityErr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortAParityErr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortAParityErr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortAParityErr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortAParityErr_ACTION_CONTROL_ADDRESS    0x13b202b0UL


/***********************************************************
* Register Name : PCIE3PortASerr_ACTION_CONTROL
* Register Description :
* The actions controlled in this register are triggered when the corresponding root port internally detects a Serr error
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define PCIE3PortASerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define PCIE3PortASerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define PCIE3PortASerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define PCIE3PortASerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define PCIE3PortASerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define PCIE3PortASerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define PCIE3PortASerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define PCIE3PortASerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define PCIE3PortASerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define PCIE3PortASerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} PCIE3PortASerr_ACTION_CONTROL_STRUCT;

#define SMN_PCIE3PortASerr_ACTION_CONTROL_ADDRESS    0x13b20294UL


/***********************************************************
* Register Name : PCIE_VDM_CNTL2
* Register Description :
* PCIE VDM / MCTP VDM control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register controls the routing of ID-routed vendor defined messages 0 - Local VDM P2P only 1 - Local VDM P2P + Remote VDM P2P 2 - Remote P2P only. VDMs to local targets are reflected off the Data Fabric. 3 - VDM ID-routing is disabled. All ID-routed VDMs are terminated.
#define PCIE_VDM_CNTL2_VdmP2pMode_OFFSET      0
#define PCIE_VDM_CNTL2_VdmP2pMode_MASK        0x3

// Bitfield Description : 
#define PCIE_VDM_CNTL2_Reserved_3_2_OFFSET      2
#define PCIE_VDM_CNTL2_Reserved_3_2_MASK        0xc

// Bitfield Description : Enables MCTP VDM requests to be sent to SMU.
#define PCIE_VDM_CNTL2_MCTPT2SMUEn_OFFSET      4
#define PCIE_VDM_CNTL2_MCTPT2SMUEn_MASK        0x10

// Bitfield Description : Enables AMD VDM requests to be sent to SMU.
#define PCIE_VDM_CNTL2_AMDVDM2SMUEn_OFFSET      5
#define PCIE_VDM_CNTL2_AMDVDM2SMUEn_MASK        0x20

// Bitfield Description : Enables non-MCTP/non-APMTP VDM requests to be sent to SMU.
#define PCIE_VDM_CNTL2_OtherVDM2SMUEn_OFFSET      6
#define PCIE_VDM_CNTL2_OtherVDM2SMUEn_MASK        0x40

// Bitfield Description : 
#define PCIE_VDM_CNTL2_Reserved_14_7_OFFSET      7
#define PCIE_VDM_CNTL2_Reserved_14_7_MASK        0x7f80

// Bitfield Description : Indicates whether the contents of MCTPMasterID are valid.
#define PCIE_VDM_CNTL2_MCTPMasterValid_OFFSET      15
#define PCIE_VDM_CNTL2_MCTPMasterValid_MASK        0x8000

// Bitfield Description : Indicates the Bus, Device, Function number of the MCTP master.
#define PCIE_VDM_CNTL2_MCTPMasterID_OFFSET      16
#define PCIE_VDM_CNTL2_MCTPMasterID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            VdmP2pMode:2;
    UINT32                            Reserved_3_2:2;
    UINT32                            MCTPT2SMUEn:1;
    UINT32                            AMDVDM2SMUEn:1;
    UINT32                            OtherVDM2SMUEn:1;
    UINT32                            Reserved_14_7:8;
    UINT32                            MCTPMasterValid:1;
    UINT32                            MCTPMasterID:16;
  } Field;
  UINT32 Value;
} PCIE_VDM_CNTL2_STRUCT;

#define SMN_PCIE_VDM_CNTL2_ADDRESS    0x13b10230UL


/***********************************************************
* Register Name : PCIE_VDM_CNTL3
* Register Description :
* PCIE APMTP VDM control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define PCIE_VDM_CNTL3_Reserved_14_0_OFFSET      0
#define PCIE_VDM_CNTL3_Reserved_14_0_MASK        0x7fff

// Bitfield Description : Indicates whether the contents of APMTPMasterID are valid.
#define PCIE_VDM_CNTL3_APMTPMasterValid_OFFSET      15
#define PCIE_VDM_CNTL3_APMTPMasterValid_MASK        0x8000

// Bitfield Description : Indicates the Bus, Device, Function number of the APMTP master.
#define PCIE_VDM_CNTL3_APMTPMasterID_OFFSET      16
#define PCIE_VDM_CNTL3_APMTPMasterID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_14_0:15;
    UINT32                            APMTPMasterValid:1;
    UINT32                            APMTPMasterID:16;
  } Field;
  UINT32 Value;
} PCIE_VDM_CNTL3_STRUCT;

#define SMN_PCIE_VDM_CNTL3_ADDRESS    0x13b10234UL


/***********************************************************
* Register Name : PCIE_VDM_NODE_CTRL4
* Register Description :
* PCIE VDM controls for node0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The lowest bus number residing below NBIO node0.
#define PCIE_VDM_NODE_CTRL4_BUS_RANGE_BASE_OFFSET      0
#define PCIE_VDM_NODE_CTRL4_BUS_RANGE_BASE_MASK        0xff

// Bitfield Description : The highest bus number residing below NBIO node0.
#define PCIE_VDM_NODE_CTRL4_BUS_RANGE_LIMIT_OFFSET      8
#define PCIE_VDM_NODE_CTRL4_BUS_RANGE_LIMIT_MASK        0xff00

// Bitfield Description : 
#define PCIE_VDM_NODE_CTRL4_Reserved_30_16_OFFSET      16
#define PCIE_VDM_NODE_CTRL4_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : Indicate Node0 is in the system, and all values programmed in PCIE_VDM_NODE0_CTRL0-4 are valid.
#define PCIE_VDM_NODE_CTRL4_NODE_PRESENT_OFFSET      31
#define PCIE_VDM_NODE_CTRL4_NODE_PRESENT_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BUS_RANGE_BASE:8;
    UINT32                            BUS_RANGE_LIMIT:8;
    UINT32                            Reserved_30_16:15;
    UINT32                            NODE_PRESENT:1;
  } Field;
  UINT32 Value;
} PCIE_VDM_NODE_CTRL4_STRUCT;

#define SMN_N0_PCIE_VDM_NODE_CTRL4_ADDRESS    0x13b10190UL


/***********************************************************
* Register Name : POISON_ACTION_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when IOHC attempts to consume poisoned data.
#define POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_OFFSET      0
#define POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_MASK        0x1
#define POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_DEFAULT     0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when IOHC attempts to consume poisoned data:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define POISON_ACTION_CONTROL_IntPoisonIntrGenSel_OFFSET      1
#define POISON_ACTION_CONTROL_IntPoisonIntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when IOHC attempts to consume poisoned data. This may be used in combination with IntPoisonSyncFloodEn to prevent errors from being transmitted out of the processor over the PCIe interface.
#define POISON_ACTION_CONTROL_IntPoisonLinkDisEn_OFFSET      3
#define POISON_ACTION_CONTROL_IntPoisonLinkDisEn_MASK        0x8
#define POISON_ACTION_CONTROL_IntPoisonLinkDisEn_DEFAULT     0x1

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when IOHC attempts to consume poisoned data. This halts the system.
#define POISON_ACTION_CONTROL_IntPoisonSyncFloodEn_OFFSET      4
#define POISON_ACTION_CONTROL_IntPoisonSyncFloodEn_MASK        0x10
#define POISON_ACTION_CONTROL_IntPoisonSyncFloodEn_DEFAULT     0x1

// Bitfield Description : 
#define POISON_ACTION_CONTROL_Reserved_7_5_OFFSET      5
#define POISON_ACTION_CONTROL_Reserved_7_5_MASK        0xe0

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when IOHC sends low severity poisoned data to one of its clients.
#define POISON_ACTION_CONTROL_EgressPoisonLSAPMLErrEn_OFFSET      8
#define POISON_ACTION_CONTROL_EgressPoisonLSAPMLErrEn_MASK        0x100

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when IOHC sends low severity poisoned data to one of its clients:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define POISON_ACTION_CONTROL_EgressPoisonLSIntrGenSel_OFFSET      9
#define POISON_ACTION_CONTROL_EgressPoisonLSIntrGenSel_MASK        0x600

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when IOHC sends low severity poisoned data to one of its clients. This may be used in combination with EgressPoisonLSSyncFloodEn to prevent errors from being transmitted out of the processor over the PCIe interface.
#define POISON_ACTION_CONTROL_EgressPoisonLSLinkDisEn_OFFSET      11
#define POISON_ACTION_CONTROL_EgressPoisonLSLinkDisEn_MASK        0x800

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when IOHC sends low severity poisoned data to one of its clients. This halts the system.
#define POISON_ACTION_CONTROL_EgressPoisonLSSyncFloodEn_OFFSET      12
#define POISON_ACTION_CONTROL_EgressPoisonLSSyncFloodEn_MASK        0x1000

// Bitfield Description : 
#define POISON_ACTION_CONTROL_Reserved_15_13_OFFSET      13
#define POISON_ACTION_CONTROL_Reserved_15_13_MASK        0xe000

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when IOHC sends high severity poisoned data to one of its clients.
#define POISON_ACTION_CONTROL_EgressPoisonHSAPMLErrEn_OFFSET      16
#define POISON_ACTION_CONTROL_EgressPoisonHSAPMLErrEn_MASK        0x10000

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when IOHC sends high severity poisoned data to one of its clients:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define POISON_ACTION_CONTROL_EgressPoisonHSIntrGenSel_OFFSET      17
#define POISON_ACTION_CONTROL_EgressPoisonHSIntrGenSel_MASK        0x60000

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when IOHC sends high severity poisoned data to one of its clients. This may be used in combination with EgressPoisonHSSyncFloodEn to prevent errors from being transmitted out of the processor over the PCIe interface.
#define POISON_ACTION_CONTROL_EgressPoisonHSLinkDisEn_OFFSET      19
#define POISON_ACTION_CONTROL_EgressPoisonHSLinkDisEn_MASK        0x80000

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when IOHC sends high severity poisoned data to one of its clients. This halts the system.
#define POISON_ACTION_CONTROL_EgressPoisonHSSyncFloodEn_OFFSET      20
#define POISON_ACTION_CONTROL_EgressPoisonHSSyncFloodEn_MASK        0x100000

// Bitfield Description : 
#define POISON_ACTION_CONTROL_Reserved_31_21_OFFSET      21
#define POISON_ACTION_CONTROL_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            IntPoisonAPMLErrEn:1;
    UINT32                            IntPoisonIntrGenSel:2;
    UINT32                            IntPoisonLinkDisEn:1;
    UINT32                            IntPoisonSyncFloodEn:1;
    UINT32                            Reserved_7_5:3;
    UINT32                            EgressPoisonLSAPMLErrEn:1;
    UINT32                            EgressPoisonLSIntrGenSel:2;
    UINT32                            EgressPoisonLSLinkDisEn:1;
    UINT32                            EgressPoisonLSSyncFloodEn:1;
    UINT32                            Reserved_15_13:3;
    UINT32                            EgressPoisonHSAPMLErrEn:1;
    UINT32                            EgressPoisonHSIntrGenSel:2;
    UINT32                            EgressPoisonHSLinkDisEn:1;
    UINT32                            EgressPoisonHSSyncFloodEn:1;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} POISON_ACTION_CONTROL_STRUCT;

#define SMN_POISON_ACTION_CONTROL_ADDRESS    0x13b20814UL


/***********************************************************
* Register Name : PSP_BASE_ADDR_HI
* Register Description :
* PSP private MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : PSP private MMIO base address bits 47:32.
#define PSP_BASE_ADDR_HI_PSP_BASE_ADDR_HI_OFFSET      0
#define PSP_BASE_ADDR_HI_PSP_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define PSP_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define PSP_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PSP_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PSP_BASE_ADDR_HI_STRUCT;

#define SMN_PSP_BASE_ADDR_HI_ADDRESS    0x13b102e4UL


/***********************************************************
* Register Name : PSP_BASE_ADDR_LO
* Register Description :
* PSP MMIO base address
* Visibility : 0x2
************************************************************/

// Bitfield Description : PSP private MMIO enable.
#define PSP_BASE_ADDR_LO_PSP_MMIO_EN_OFFSET      0
#define PSP_BASE_ADDR_LO_PSP_MMIO_EN_MASK        0x1

// Bitfield Description : 
#define PSP_BASE_ADDR_LO_Reserved_7_1_OFFSET      1
#define PSP_BASE_ADDR_LO_Reserved_7_1_MASK        0xfe

// Bitfield Description : Locks the PSP private MMIO address range and enable until the next warm reset.
#define PSP_BASE_ADDR_LO_PSP_MMIO_LOCK_OFFSET      8
#define PSP_BASE_ADDR_LO_PSP_MMIO_LOCK_MASK        0x100

// Bitfield Description : 
#define PSP_BASE_ADDR_LO_Reserved_19_9_OFFSET      9
#define PSP_BASE_ADDR_LO_Reserved_19_9_MASK        0xffe00

// Bitfield Description : PSP private MMIO base address bits 31:20.
#define PSP_BASE_ADDR_LO_PSP_BASE_ADDR_LO_OFFSET      20
#define PSP_BASE_ADDR_LO_PSP_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            PSP_MMIO_EN:1;
    UINT32                            Reserved_7_1:7;
    UINT32                            PSP_MMIO_LOCK:1;
    UINT32                            Reserved_19_9:11;
    UINT32                            PSP_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} PSP_BASE_ADDR_LO_STRUCT;

#define SMN_PSP_BASE_ADDR_LO_ADDRESS    0x13b102e0UL


/***********************************************************
* Register Name : ParityCorr_ACTION_CONTROL
* Register Description :
* Action controls when ParityCorr occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define ParityCorr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define ParityCorr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define ParityCorr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define ParityCorr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define ParityCorr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define ParityCorr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define ParityCorr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define ParityCorr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define ParityCorr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define ParityCorr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ParityCorr_ACTION_CONTROL_STRUCT;

#define SMN_ParityCorr_ACTION_CONTROL_ADDRESS    0x13b20110UL


/***********************************************************
* Register Name : ParityFatal_ACTION_CONTROL
* Register Description :
* Action controls when ParityFatal occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define ParityFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define ParityFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define ParityFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define ParityFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define ParityFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define ParityFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define ParityFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define ParityFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define ParityFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define ParityFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ParityFatal_ACTION_CONTROL_STRUCT;

#define SMN_ParityFatal_ACTION_CONTROL_ADDRESS    0x13b20108UL


/***********************************************************
* Register Name : ParityNonFatal_ACTION_CONTROL
* Register Description :
* Action controls when ParityNonFatal occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define ParityNonFatal_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define ParityNonFatal_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define ParityNonFatal_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define ParityNonFatal_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define ParityNonFatal_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define ParityNonFatal_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define ParityNonFatal_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define ParityNonFatal_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define ParityNonFatal_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define ParityNonFatal_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ParityNonFatal_ACTION_CONTROL_STRUCT;

#define SMN_ParityNonFatal_ACTION_CONTROL_ADDRESS    0x13b2010cUL


/***********************************************************
* Register Name : ParitySerr_ACTION_CONTROL
* Register Description :
* Action controls when ParitySerr occurs
* Visibility : 0x2
************************************************************/

// Bitfield Description : Setting this bit to 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged. SMU firmware may then signal the error via APML.
#define ParitySerr_ACTION_CONTROL_APML_ERR_En_OFFSET      0
#define ParitySerr_ACTION_CONTROL_APML_ERR_En_MASK        0x1

// Bitfield Description : Setting this field to a non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged:
// 11=SMI
// 10=SCI
// 01=NMI
// 00=Disable interrupt generation.
#define ParitySerr_ACTION_CONTROL_IntrGenSel_OFFSET      1
#define ParitySerr_ACTION_CONTROL_IntrGenSel_MASK        0x6

// Bitfield Description : Setting this bit to 1 causes PCIe links to be disabled when the associated error is logged. This may be used in combination with SyncFlood_En to prevent errors from being transmitted out of the processor over the PCIe interface.
#define ParitySerr_ACTION_CONTROL_LinkDis_En_OFFSET      3
#define ParitySerr_ACTION_CONTROL_LinkDis_En_MASK        0x8

// Bitfield Description : Setting this bit to 1 causes an SDP ErrEvent to be triggered when the associated error is logged. This halts the system.
#define ParitySerr_ACTION_CONTROL_SyncFlood_En_OFFSET      4
#define ParitySerr_ACTION_CONTROL_SyncFlood_En_MASK        0x10

// Bitfield Description : 
#define ParitySerr_ACTION_CONTROL_Reserved_31_5_OFFSET      5
#define ParitySerr_ACTION_CONTROL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            APML_ERR_En:1;
    UINT32                            IntrGenSel:2;
    UINT32                            LinkDis_En:1;
    UINT32                            SyncFlood_En:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} ParitySerr_ACTION_CONTROL_STRUCT;

#define SMN_ParitySerr_ACTION_CONTROL_ADDRESS    0x13b20104UL


/***********************************************************
* Register Name : RAS_GLOBAL_STATUS_HI
* Register Description :
* RAS Global Error Status Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit indicates the status of errors from PCIE0 PortA.
#define RAS_GLOBAL_STATUS_HI_PCIE0PortAErr_OFFSET      0
#define RAS_GLOBAL_STATUS_HI_PCIE0PortAErr_MASK        0x1

// Bitfield Description : This bit indicates the status of errors from PCIE0 PortB.
#define RAS_GLOBAL_STATUS_HI_PCIE0PortBErr_OFFSET      1
#define RAS_GLOBAL_STATUS_HI_PCIE0PortBErr_MASK        0x2

// Bitfield Description : This bit indicates the status of errors from PCIE0 PortC.
#define RAS_GLOBAL_STATUS_HI_PCIE0PortCErr_OFFSET      2
#define RAS_GLOBAL_STATUS_HI_PCIE0PortCErr_MASK        0x4

// Bitfield Description : This bit indicates the status of errors from PCIE0 PortD.
#define RAS_GLOBAL_STATUS_HI_PCIE0PortDErr_OFFSET      3
#define RAS_GLOBAL_STATUS_HI_PCIE0PortDErr_MASK        0x8

// Bitfield Description : This bit indicates the status of errors from PCIE0 PortE.
#define RAS_GLOBAL_STATUS_HI_PCIE0PortEErr_OFFSET      4
#define RAS_GLOBAL_STATUS_HI_PCIE0PortEErr_MASK        0x10

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortA.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortAErr_OFFSET      5
#define RAS_GLOBAL_STATUS_HI_PCIE1PortAErr_MASK        0x20

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortB.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortBErr_OFFSET      6
#define RAS_GLOBAL_STATUS_HI_PCIE1PortBErr_MASK        0x40

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortC.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortCErr_OFFSET      7
#define RAS_GLOBAL_STATUS_HI_PCIE1PortCErr_MASK        0x80

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortD.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortDErr_OFFSET      8
#define RAS_GLOBAL_STATUS_HI_PCIE1PortDErr_MASK        0x100

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortE.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortEErr_OFFSET      9
#define RAS_GLOBAL_STATUS_HI_PCIE1PortEErr_MASK        0x200

// Bitfield Description : This bit indicates the status of errors from PCIE1 PortF.
#define RAS_GLOBAL_STATUS_HI_PCIE1PortFErr_OFFSET      10
#define RAS_GLOBAL_STATUS_HI_PCIE1PortFErr_MASK        0x400

// Bitfield Description : This bit indicates the status of errors from PCIE2 PortA.
#define RAS_GLOBAL_STATUS_HI_PCIE2PortAErr_OFFSET      11
#define RAS_GLOBAL_STATUS_HI_PCIE2PortAErr_MASK        0x800

// Bitfield Description : This bit indicates the status of errors from PCIE3 PortA.
#define RAS_GLOBAL_STATUS_HI_PCIE3PortAErr_OFFSET      12
#define RAS_GLOBAL_STATUS_HI_PCIE3PortAErr_MASK        0x1000

// Bitfield Description : This bit indicates the status of errors from NBIF1 PortA.
#define RAS_GLOBAL_STATUS_HI_NBIF1PortAErr_OFFSET      13
#define RAS_GLOBAL_STATUS_HI_NBIF1PortAErr_MASK        0x2000

// Bitfield Description : This bit indicates the status of errors from NBIF1 PortB.
#define RAS_GLOBAL_STATUS_HI_NBIF1PortBErr_OFFSET      14
#define RAS_GLOBAL_STATUS_HI_NBIF1PortBErr_MASK        0x4000

// Bitfield Description : This bit indicates the status of errors from NBIF1 PortC.
#define RAS_GLOBAL_STATUS_HI_NBIF1PortCErr_OFFSET      15
#define RAS_GLOBAL_STATUS_HI_NBIF1PortCErr_MASK        0x8000

// Bitfield Description : 
#define RAS_GLOBAL_STATUS_HI_Reserved_31_16_OFFSET      16
#define RAS_GLOBAL_STATUS_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PCIE0PortAErr:1;
    UINT32                            PCIE0PortBErr:1;
    UINT32                            PCIE0PortCErr:1;
    UINT32                            PCIE0PortDErr:1;
    UINT32                            PCIE0PortEErr:1;
    UINT32                            PCIE1PortAErr:1;
    UINT32                            PCIE1PortBErr:1;
    UINT32                            PCIE1PortCErr:1;
    UINT32                            PCIE1PortDErr:1;
    UINT32                            PCIE1PortEErr:1;
    UINT32                            PCIE1PortFErr:1;
    UINT32                            PCIE2PortAErr:1;
    UINT32                            PCIE3PortAErr:1;
    UINT32                            NBIF1PortAErr:1;
    UINT32                            NBIF1PortBErr:1;
    UINT32                            NBIF1PortCErr:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RAS_GLOBAL_STATUS_HI_STRUCT;

#define SMN_RAS_GLOBAL_STATUS_HI_ADDRESS    0x13b20024UL


/***********************************************************
* Register Name : RAS_GLOBAL_STATUS_LO
* Register Description :
* RAS Global Error Status Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit indicates status of a correctable ECC or parity error. Additional status information is logged in the PARITY_ERROR_STATUS_* registers.
#define RAS_GLOBAL_STATUS_LO_ParityErrCorr_OFFSET      0
#define RAS_GLOBAL_STATUS_LO_ParityErrCorr_MASK        0x1

// Bitfield Description : This bit indicates status of a non-fatal ECC or parity error. Additional status information is logged in the PARITY_ERROR_STATUS_* registers.
#define RAS_GLOBAL_STATUS_LO_ParityErrNonFatal_OFFSET      1
#define RAS_GLOBAL_STATUS_LO_ParityErrNonFatal_MASK        0x2

// Bitfield Description : This bit indicates status of a fatal ECC or parity error. Additional status information is logged in the PARITY_ERROR_STATUS_* registers.
#define RAS_GLOBAL_STATUS_LO_ParityErrFatal_OFFSET      2
#define RAS_GLOBAL_STATUS_LO_ParityErrFatal_MASK        0x4

// Bitfield Description : This bit indicates status of a system error severity ECC or parity error. Additional status information is logged in the PARITY_ERROR_STATUS_* registers.
#define RAS_GLOBAL_STATUS_LO_ParityErrSerr_OFFSET      3
#define RAS_GLOBAL_STATUS_LO_ParityErrSerr_MASK        0x8

// Bitfield Description : 
#define RAS_GLOBAL_STATUS_LO_Reserved_5_4_OFFSET      4
#define RAS_GLOBAL_STATUS_LO_Reserved_5_4_MASK        0x30

// Bitfield Description : This bit indicates status of an NMI issued on behalf of PCIe or an nBIF client.
#define RAS_GLOBAL_STATUS_LO_HPLGWA_NMI_OFFSET      6
#define RAS_GLOBAL_STATUS_LO_HPLGWA_NMI_MASK        0x40

// Bitfield Description : This bit indicates status of an SCI issued on behalf of PCIe or an nBIF client.
#define RAS_GLOBAL_STATUS_LO_HPLGWA_SCI_OFFSET      7
#define RAS_GLOBAL_STATUS_LO_HPLGWA_SCI_MASK        0x80

// Bitfield Description : This bit indicates status of an SMI issued on behalf of PCIe or an nBIF client.
#define RAS_GLOBAL_STATUS_LO_HPLGWA_SMI_OFFSET      8
#define RAS_GLOBAL_STATUS_LO_HPLGWA_SMI_MASK        0x100

// Bitfield Description : This bit indicates status of an SMI issued via IOHC::SW_SMI_CNTL
#define RAS_GLOBAL_STATUS_LO_SW_SMI_OFFSET      9
#define RAS_GLOBAL_STATUS_LO_SW_SMI_MASK        0x200

// Bitfield Description : This bit indicates status of an SCI issued via IOHC::SW_SCI_CNTL
#define RAS_GLOBAL_STATUS_LO_SW_SCI_OFFSET      10
#define RAS_GLOBAL_STATUS_LO_SW_SCI_MASK        0x400

// Bitfield Description : This bit indicates status of an NMI issued via IOHC::SW_NMI_CNTL
#define RAS_GLOBAL_STATUS_LO_SW_NMI_OFFSET      11
#define RAS_GLOBAL_STATUS_LO_SW_NMI_MASK        0x800

// Bitfield Description : This bit indicates status of an NMI  issued via IOHC::APML_TRIGGER[APML_NMI_TRIGGER]
#define RAS_GLOBAL_STATUS_LO_APML_NMI_OFFSET      12
#define RAS_GLOBAL_STATUS_LO_APML_NMI_MASK        0x1000

// Bitfield Description : This bit indicates status of an SDP ErrEvent triggered using IOHC::SW_SYNCFLOOD_CNTL[SW_SYNCFLOOD_APML]
#define RAS_GLOBAL_STATUS_LO_APML_SyncFld_OFFSET      13
#define RAS_GLOBAL_STATUS_LO_APML_SyncFld_MASK        0x2000

// Bitfield Description : This bit indicates status of the NMI/Synflood input pin.
#define RAS_GLOBAL_STATUS_LO_PIN_SyncFld_NMI_OFFSET      14
#define RAS_GLOBAL_STATUS_LO_PIN_SyncFld_NMI_MASK        0x4000

// Bitfield Description : This bit indicates status of an SDP ErrEvent triggered using IOHC::SW_SYNCFLOOD_CNTL[SW_SYNCFLOOD_PRIVATE]
#define RAS_GLOBAL_STATUS_LO_APML_SyncFld_Private_OFFSET      15
#define RAS_GLOBAL_STATUS_LO_APML_SyncFld_Private_MASK        0x8000

// Bitfield Description : 
#define RAS_GLOBAL_STATUS_LO_Reserved_31_16_OFFSET      16
#define RAS_GLOBAL_STATUS_LO_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ParityErrCorr:1;
    UINT32                            ParityErrNonFatal:1;
    UINT32                            ParityErrFatal:1;
    UINT32                            ParityErrSerr:1;
    UINT32                            Reserved_5_4:2;
    UINT32                            HPLGWA_NMI:1;
    UINT32                            HPLGWA_SCI:1;
    UINT32                            HPLGWA_SMI:1;
    UINT32                            SW_SMI:1;
    UINT32                            SW_SCI:1;
    UINT32                            SW_NMI:1;
    UINT32                            APML_NMI:1;
    UINT32                            APML_SyncFld:1;
    UINT32                            PIN_SyncFld_NMI:1;
    UINT32                            APML_SyncFld_Private:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RAS_GLOBAL_STATUS_LO_STRUCT;

#define SMN_RAS_GLOBAL_STATUS_LO_ADDRESS    0x13b20020UL


/***********************************************************
* Register Name : RAS_SCRATCH_0
* Register Description :
* RAS index configuration space scratch register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch register.
#define RAS_SCRATCH_0_SCRATCH_0_OFFSET      0
#define RAS_SCRATCH_0_SCRATCH_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_0:32;
  } Field;
  UINT32 Value;
} RAS_SCRATCH_0_STRUCT;

#define SMN_RAS_SCRATCH_0_ADDRESS    0x13b200f8UL


/***********************************************************
* Register Name : RAS_SCRATCH_1
* Register Description :
* RAS index configuration space scratch register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch register.
#define RAS_SCRATCH_1_SCRATCH_1_OFFSET      0
#define RAS_SCRATCH_1_SCRATCH_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_1:32;
  } Field;
  UINT32 Value;
} RAS_SCRATCH_1_STRUCT;

#define SMN_RAS_SCRATCH_1_ADDRESS    0x13b200fcUL


/***********************************************************
* Register Name : SB_COMMAND
* Register Description :
* iFCH Enablement
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable IO access to the iFCH. 0=Disable 1=Enable.
#define SB_COMMAND_IO_ACCESS_EN_OFFSET      0
#define SB_COMMAND_IO_ACCESS_EN_MASK        0x1

// Bitfield Description : Enable memory access to the iFCH.  0=Disable 1=Enable.
#define SB_COMMAND_MEM_ACCESS_EN_OFFSET      1
#define SB_COMMAND_MEM_ACCESS_EN_MASK        0x2

// Bitfield Description : Enable memory access from the iFCH.  0=Disable 1=Enable.
#define SB_COMMAND_BUS_MASTER_EN_OFFSET      2
#define SB_COMMAND_BUS_MASTER_EN_MASK        0x4

// Bitfield Description : 
#define SB_COMMAND_Reserved_15_3_OFFSET      3
#define SB_COMMAND_Reserved_15_3_MASK        0xfff8

typedef union {
  struct {
    UINT16                            IO_ACCESS_EN:1;
    UINT16                            MEM_ACCESS_EN:1;
    UINT16                            BUS_MASTER_EN:1;
    UINT16                            Reserved_15_3:13;
  } Field;
  UINT16 Value;
} SB_COMMAND_STRUCT;

#define SMN_SB_COMMAND_ADDRESS    0x13b14000UL


/***********************************************************
* Register Name : SB_DEVICE_CNTL2
* Register Description :
* The Device Control 2 register controls PCI Express device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_DEVICE_CNTL2_Reserved_4_0_OFFSET      0
#define SB_DEVICE_CNTL2_Reserved_4_0_MASK        0x1f

// Bitfield Description : ARI Forwarding enable. When set, this assumes Dev number=0 and allows 8-bit Fn number. Otherwise, it is required that Dev=0 for external config request decoding.
#define SB_DEVICE_CNTL2_ARI_FORWARDING_EN_OFFSET      5
#define SB_DEVICE_CNTL2_ARI_FORWARDING_EN_MASK        0x20

// Bitfield Description : 
#define SB_DEVICE_CNTL2_Reserved_15_6_OFFSET      6
#define SB_DEVICE_CNTL2_Reserved_15_6_MASK        0xffc0

typedef union {
  struct {
    UINT16                            Reserved_4_0:5;
    UINT16                            ARI_FORWARDING_EN:1;
    UINT16                            Reserved_15_6:10;
  } Field;
  UINT16 Value;
} SB_DEVICE_CNTL2_STRUCT;

#define SMN_SB_DEVICE_CNTL2_ADDRESS    0x13b14034UL


/***********************************************************
* Register Name : SB_EXT_BRIDGE_CNTL
* Register Description :
* External Bridge Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Register to enable IO port 80 decoding. Enables fixed IO decode to address 0x80 or 0x1080 to iFCH.
#define SB_EXT_BRIDGE_CNTL_IO_PORT_80_EN_OFFSET      0
#define SB_EXT_BRIDGE_CNTL_IO_PORT_80_EN_MASK        0x1

// Bitfield Description : 
#define SB_EXT_BRIDGE_CNTL_Reserved_7_1_OFFSET      1
#define SB_EXT_BRIDGE_CNTL_Reserved_7_1_MASK        0xfe

typedef union {
  struct {
    UINT8                            IO_PORT_80_EN:1;
    UINT8                            Reserved_7_1:7;
  } Field;
  UINT8 Value;
} SB_EXT_BRIDGE_CNTL_STRUCT;

#define SMN_SB_EXT_BRIDGE_CNTL_ADDRESS    0x13b14024UL


/***********************************************************
* Register Name : SB_IO_BASE_LIMIT
* Register Description :
* I/O Base Register Limit is used by the iFCH to determine when to forward I/O transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_IO_BASE_LIMIT_Reserved_3_0_OFFSET      0
#define SB_IO_BASE_LIMIT_Reserved_3_0_MASK        0xf

// Bitfield Description : I/O Base.
#define SB_IO_BASE_LIMIT_IO_BASE_OFFSET      4
#define SB_IO_BASE_LIMIT_IO_BASE_MASK        0xf0

// Bitfield Description : 
#define SB_IO_BASE_LIMIT_Reserved_11_8_OFFSET      8
#define SB_IO_BASE_LIMIT_Reserved_11_8_MASK        0xf00

// Bitfield Description : I/O Limit.
#define SB_IO_BASE_LIMIT_IO_LIMIT_OFFSET      12
#define SB_IO_BASE_LIMIT_IO_LIMIT_MASK        0xf000

typedef union {
  struct {
    UINT16                            Reserved_3_0:4;
    UINT16                            IO_BASE:4;
    UINT16                            Reserved_11_8:4;
    UINT16                            IO_LIMIT:4;
  } Field;
  UINT16 Value;
} SB_IO_BASE_LIMIT_STRUCT;

#define SMN_SB_IO_BASE_LIMIT_ADDRESS    0x13b14008UL


/***********************************************************
* Register Name : SB_IO_BASE_LIMIT_HI
* Register Description :
* I/O Base and I/O Limit Upper 16 bits.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 16 bits for 32-bit address.
#define SB_IO_BASE_LIMIT_HI_IO_BASE_31_16_OFFSET      0
#define SB_IO_BASE_LIMIT_HI_IO_BASE_31_16_MASK        0xffff

// Bitfield Description : Upper 16 bits for 32-bit address.
#define SB_IO_BASE_LIMIT_HI_IO_LIMIT_31_16_OFFSET      16
#define SB_IO_BASE_LIMIT_HI_IO_LIMIT_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            IO_BASE_31_16:16;
    UINT32                            IO_LIMIT_31_16:16;
  } Field;
  UINT32 Value;
} SB_IO_BASE_LIMIT_HI_STRUCT;

#define SMN_SB_IO_BASE_LIMIT_HI_ADDRESS    0x13b1401cUL


/***********************************************************
* Register Name : SB_IRQ_BRIDGE_CNTL
* Register Description :
* Bridge Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_IRQ_BRIDGE_CNTL_Reserved_1_0_OFFSET      0
#define SB_IRQ_BRIDGE_CNTL_Reserved_1_0_MASK        0x3

// Bitfield Description : Enable fixed ISA IO regions decode to the iFCH. ISA regions are within the first 256B of each 1K address range (ie. address bits 31:16=0 and 9:8=0).
#define SB_IRQ_BRIDGE_CNTL_ISA_EN_OFFSET      2
#define SB_IRQ_BRIDGE_CNTL_ISA_EN_MASK        0x4

// Bitfield Description : Enable fixed VGA memory and IO regions decode to the iFCH. 16-bit VGA IO ranges are address bits 31:16=0, 9:0=3B0 to 3BB or 9:0=3C0 to 3DF. VGA memory range is A_0000h to B_FFFFh.
#define SB_IRQ_BRIDGE_CNTL_VGA_EN_OFFSET      3
#define SB_IRQ_BRIDGE_CNTL_VGA_EN_MASK        0x8

// Bitfield Description : Enables the iFCH to provide 16-bit decoding of VGA I/O address precluding the decoding of alias addresses every 1 kB (ie. requires address bits 15:10=0).
#define SB_IRQ_BRIDGE_CNTL_VGA_DEC_OFFSET      4
#define SB_IRQ_BRIDGE_CNTL_VGA_DEC_MASK        0x10

// Bitfield Description : 
#define SB_IRQ_BRIDGE_CNTL_Reserved_15_5_OFFSET      5
#define SB_IRQ_BRIDGE_CNTL_Reserved_15_5_MASK        0xffe0

typedef union {
  struct {
    UINT16                            Reserved_1_0:2;
    UINT16                            ISA_EN:1;
    UINT16                            VGA_EN:1;
    UINT16                            VGA_DEC:1;
    UINT16                            Reserved_15_5:11;
  } Field;
  UINT16 Value;
} SB_IRQ_BRIDGE_CNTL_STRUCT;

#define SMN_SB_IRQ_BRIDGE_CNTL_ADDRESS    0x13b14020UL


/***********************************************************
* Register Name : SB_LOCATION
* Register Description :
* South Bridge Location Register. Register set 0 indicates the GNB is secondary and there is no SB/FCH connected.
* Visibility : 0x2
************************************************************/

// Bitfield Description : SB/FCH Port Location.
// One hot encoding register:
// 0x1 - FCH located on port0 of SBlocated_Core
// 0x2 - FCH located on port1 of SBlocated_Core
// 0x4 - FCH located on port2 of SBlocated_Core
// 0x8 - FCH located on port3 of SBlocated_Core
// etc...
#define SB_LOCATION_SBlocated_Port_OFFSET      0
#define SB_LOCATION_SBlocated_Port_MASK        0xffff

// Bitfield Description : SB/FCH Core Location.
// One hot encoding register:
// 0x1 - FCH resides under PCIE/NBIF/iFCH core0
// 0x2 - FCH resides under PCIE/NBIF/iFCH core1
// 0x4 - FCH resides under PCIE/NBIF/iFCH core2
// 0x8 - FCH resides under PCIE/NBIF/iFCH core3
// etc...
#define SB_LOCATION_SBlocated_Core_OFFSET      16
#define SB_LOCATION_SBlocated_Core_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SBlocated_Port:16;
    UINT32                            SBlocated_Core:16;
  } Field;
  UINT32 Value;
} SB_LOCATION_STRUCT;

#define SMN_SB_LOCATION_ADDRESS    0x13b1007cUL


/***********************************************************
* Register Name : SB_MEM_BASE_LIMIT
* Register Description :
* Memory Limit Register defines a memory mapped I/O address range which is used by the iFCH to determine when to forward memory transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_MEM_BASE_LIMIT_Reserved_3_0_OFFSET      0
#define SB_MEM_BASE_LIMIT_Reserved_3_0_MASK        0xf

// Bitfield Description : Memory Base Register.
#define SB_MEM_BASE_LIMIT_MEM_BASE_31_20_OFFSET      4
#define SB_MEM_BASE_LIMIT_MEM_BASE_31_20_MASK        0xfff0

// Bitfield Description : 
#define SB_MEM_BASE_LIMIT_Reserved_19_16_OFFSET      16
#define SB_MEM_BASE_LIMIT_Reserved_19_16_MASK        0xf0000

// Bitfield Description : Memory Limit.
#define SB_MEM_BASE_LIMIT_MEM_LIMIT_31_20_OFFSET      20
#define SB_MEM_BASE_LIMIT_MEM_LIMIT_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            MEM_BASE_31_20:12;
    UINT32                            Reserved_19_16:4;
    UINT32                            MEM_LIMIT_31_20:12;
  } Field;
  UINT32 Value;
} SB_MEM_BASE_LIMIT_STRUCT;

#define SMN_SB_MEM_BASE_LIMIT_ADDRESS    0x13b1400cUL


/***********************************************************
* Register Name : SB_PMI_STATUS_CNTL
* Register Description :
* Power Management Control and Status Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Determine the current power state.
// 00b - D0
// 01b - Reserved
// 10b - Reserved
// 11b - D3
#define SB_PMI_STATUS_CNTL_POWER_STATE_OFFSET      0
#define SB_PMI_STATUS_CNTL_POWER_STATE_MASK        0x3

// Bitfield Description : 
#define SB_PMI_STATUS_CNTL_Reserved_7_2_OFFSET      2
#define SB_PMI_STATUS_CNTL_Reserved_7_2_MASK        0xfc

typedef union {
  struct {
    UINT8                            POWER_STATE:2;
    UINT8                            Reserved_7_2:6;
  } Field;
  UINT8 Value;
} SB_PMI_STATUS_CNTL_STRUCT;

#define SMN_SB_PMI_STATUS_CNTL_ADDRESS    0x13b14028UL


/***********************************************************
* Register Name : SB_PREF_BASE_LIMIT
* Register Description :
* Prefetchable Memory Base Limit indicates 64-bit addresses are supported.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_PREF_BASE_LIMIT_Reserved_3_0_OFFSET      0
#define SB_PREF_BASE_LIMIT_Reserved_3_0_MASK        0xf

// Bitfield Description : Prefetchable Memory Base.
#define SB_PREF_BASE_LIMIT_PREF_MEM_BASE_31_20_OFFSET      4
#define SB_PREF_BASE_LIMIT_PREF_MEM_BASE_31_20_MASK        0xfff0

// Bitfield Description : 
#define SB_PREF_BASE_LIMIT_Reserved_19_16_OFFSET      16
#define SB_PREF_BASE_LIMIT_Reserved_19_16_MASK        0xf0000

// Bitfield Description : Prefetchable Memory Limit.
#define SB_PREF_BASE_LIMIT_PREF_MEM_LIMIT_31_20_OFFSET      20
#define SB_PREF_BASE_LIMIT_PREF_MEM_LIMIT_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            PREF_MEM_BASE_31_20:12;
    UINT32                            Reserved_19_16:4;
    UINT32                            PREF_MEM_LIMIT_31_20:12;
  } Field;
  UINT32 Value;
} SB_PREF_BASE_LIMIT_STRUCT;

#define SMN_SB_PREF_BASE_LIMIT_ADDRESS    0x13b14010UL


/***********************************************************
* Register Name : SB_PREF_BASE_UPPER
* Register Description :
* Prefetchable Memory Base Upper 32 bits.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32 bits for 64-bit address.
#define SB_PREF_BASE_UPPER_PREF_BASE_UPPER_OFFSET      0
#define SB_PREF_BASE_UPPER_PREF_BASE_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PREF_BASE_UPPER:32;
  } Field;
  UINT32 Value;
} SB_PREF_BASE_UPPER_STRUCT;

#define SMN_SB_PREF_BASE_UPPER_ADDRESS    0x13b14014UL


/***********************************************************
* Register Name : SB_PREF_LIMIT_UPPER
* Register Description :
* Prefetchable Memory Limit Upper 32 bits.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32 bits for 64-bit address.
#define SB_PREF_LIMIT_UPPER_PREF_LIMIT_UPPER_OFFSET      0
#define SB_PREF_LIMIT_UPPER_PREF_LIMIT_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PREF_LIMIT_UPPER:32;
  } Field;
  UINT32 Value;
} SB_PREF_LIMIT_UPPER_STRUCT;

#define SMN_SB_PREF_LIMIT_UPPER_ADDRESS    0x13b14018UL


/***********************************************************
* Register Name : SB_ROOT_CNTL
* Register Description :
* The Root Control register controls PCI Express Root Complex specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_ROOT_CNTL_Reserved_3_0_OFFSET      0
#define SB_ROOT_CNTL_Reserved_3_0_MASK        0xf

// Bitfield Description : This bit when set enables the Root Port to return Configuration Request Retry Status Completion Status to software.
#define SB_ROOT_CNTL_CRS_SOFTWARE_VISIBILITY_EN_OFFSET      4
#define SB_ROOT_CNTL_CRS_SOFTWARE_VISIBILITY_EN_MASK        0x10

// Bitfield Description : 
#define SB_ROOT_CNTL_Reserved_15_5_OFFSET      5
#define SB_ROOT_CNTL_Reserved_15_5_MASK        0xffe0

typedef union {
  struct {
    UINT16                            Reserved_3_0:4;
    UINT16                            CRS_SOFTWARE_VISIBILITY_EN:1;
    UINT16                            Reserved_15_5:11;
  } Field;
  UINT16 Value;
} SB_ROOT_CNTL_STRUCT;

#define SMN_SB_ROOT_CNTL_ADDRESS    0x13b14030UL


/***********************************************************
* Register Name : SB_SLOT_CAP
* Register Description :
* The Slot Capabilities register identifies PCI Express slot specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_SLOT_CAP_Reserved_6_0_OFFSET      0
#define SB_SLOT_CAP_Reserved_6_0_MASK        0x7f

// Bitfield Description : In combination with the Slot Power Limit Scale value, specifies the upper limit on power supplied by slot.
#define SB_SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET      7
#define SB_SLOT_CAP_SLOT_PWR_LIMIT_VALUE_MASK        0x7f80

// Bitfield Description : Specifies the scale used for the Slot Power Limit Value.
#define SB_SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET      15
#define SB_SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK        0x18000

// Bitfield Description : 
#define SB_SLOT_CAP_Reserved_31_17_OFFSET      17
#define SB_SLOT_CAP_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            SLOT_PWR_LIMIT_VALUE:8;
    UINT32                            SLOT_PWR_LIMIT_SCALE:2;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} SB_SLOT_CAP_STRUCT;

#define SMN_SB_SLOT_CAP_ADDRESS    0x13b1402cUL


/***********************************************************
* Register Name : SB_SUB_BUS_NUMBER_LATENCY
* Register Description :
* Subordinate Bus Number Latency
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SB_SUB_BUS_NUMBER_LATENCY_Reserved_7_0_OFFSET      0
#define SB_SUB_BUS_NUMBER_LATENCY_Reserved_7_0_MASK        0xff

// Bitfield Description : Secondary Bus Number register is used to record the bus number of the PCIE bus segment to which the secondary interface of the iFCH is connected.
#define SB_SUB_BUS_NUMBER_LATENCY_SECONDARY_BUS_OFFSET      8
#define SB_SUB_BUS_NUMBER_LATENCY_SECONDARY_BUS_MASK        0xff00

// Bitfield Description : Subordinate Bus Number Register is used to record the bus number of the highest numbered PCI bus segment which is behind the iFCH.
#define SB_SUB_BUS_NUMBER_LATENCY_SUB_BUS_NUM_OFFSET      16
#define SB_SUB_BUS_NUMBER_LATENCY_SUB_BUS_NUM_MASK        0xff0000

// Bitfield Description : 
#define SB_SUB_BUS_NUMBER_LATENCY_Reserved_31_24_OFFSET      24
#define SB_SUB_BUS_NUMBER_LATENCY_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_7_0:8;
    UINT32                            SECONDARY_BUS:8;
    UINT32                            SUB_BUS_NUM:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} SB_SUB_BUS_NUMBER_LATENCY_STRUCT;

#define SMN_SB_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x13b14004UL


/***********************************************************
* Register Name : SCRATCH_4
* Register Description :
* scratch registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
#define SCRATCH_4_SCRATCH_4_OFFSET      0
#define SCRATCH_4_SCRATCH_4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_4:32;
  } Field;
  UINT32 Value;
} SCRATCH_4_STRUCT;

#define SMN_SCRATCH_4_ADDRESS    0x13b103f0UL


/***********************************************************
* Register Name : SCRATCH_5
* Register Description :
* scratch registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Scratch bits.
#define SCRATCH_5_SCRATCH_5_OFFSET      0
#define SCRATCH_5_SCRATCH_5_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH_5:32;
  } Field;
  UINT32 Value;
} SCRATCH_5_STRUCT;

#define SMN_SCRATCH_5_ADDRESS    0x13b103f4UL


/***********************************************************
* Register Name : SMU_BASE_ADDR_HI
* Register Description :
* SMU MMIO base address
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMU MMIO base address bits 47:32.
#define SMU_BASE_ADDR_HI_SMU_BASE_ADDR_HI_OFFSET      0
#define SMU_BASE_ADDR_HI_SMU_BASE_ADDR_HI_MASK        0xffff

// Bitfield Description : 
#define SMU_BASE_ADDR_HI_Reserved_31_16_OFFSET      16
#define SMU_BASE_ADDR_HI_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SMU_BASE_ADDR_HI:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} SMU_BASE_ADDR_HI_STRUCT;

#define SMN_SMU_BASE_ADDR_HI_ADDRESS    0x13b102ecUL


/***********************************************************
* Register Name : SMU_BASE_ADDR_LO
* Register Description :
* SMU MMIO base address.
* Visibility : 0x2
************************************************************/

// Bitfield Description : SMU MMIO enable.
#define SMU_BASE_ADDR_LO_SMU_MMIO_EN_OFFSET      0
#define SMU_BASE_ADDR_LO_SMU_MMIO_EN_MASK        0x1

// Bitfield Description : Locks the SMU MMIO address range and enable until the next warm reset.
#define SMU_BASE_ADDR_LO_SMU_MMIO_LOCK_OFFSET      1
#define SMU_BASE_ADDR_LO_SMU_MMIO_LOCK_MASK        0x2

// Bitfield Description : 
#define SMU_BASE_ADDR_LO_Reserved_19_2_OFFSET      2
#define SMU_BASE_ADDR_LO_Reserved_19_2_MASK        0xffffc

// Bitfield Description : SMU MMIO base address bits 31:20.
#define SMU_BASE_ADDR_LO_SMU_BASE_ADDR_LO_OFFSET      20
#define SMU_BASE_ADDR_LO_SMU_BASE_ADDR_LO_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            SMU_MMIO_EN:1;
    UINT32                            SMU_MMIO_LOCK:1;
    UINT32                            Reserved_19_2:18;
    UINT32                            SMU_BASE_ADDR_LO:12;
  } Field;
  UINT32 Value;
} SMU_BASE_ADDR_LO_STRUCT;

#define SMN_SMU_BASE_ADDR_LO_ADDRESS    0x13b102e8UL


/***********************************************************
* Register Name : STATUS_COMMAND
* Register Description :
* Command/Status Register
* Visibility : 0x2
************************************************************/

// Bitfield Description : Command.
#define STATUS_COMMAND_COMMAND_OFFSET      0
#define STATUS_COMMAND_COMMAND_MASK        0xffff

// Bitfield Description : Status.
#define STATUS_COMMAND_STATUS_OFFSET      16
#define STATUS_COMMAND_STATUS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            COMMAND:16;
    UINT32                            STATUS:16;
  } Field;
  UINT32 Value;
} STATUS_COMMAND_STRUCT;

#define PCICFG_NBIO_IOHUB_STATUS_COMMAND_DEVICE       8
#define PCICFG_NBIO_IOHUB_STATUS_COMMAND_FUNC         0
#define PCICFG_NBIO_IOHUB_STATUS_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_IOHUB_STATUS_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 8, 0, 0x4)

#define PCICFG_NBIO_HB0_IOHUB_STATUS_COMMAND_DEVICE       1
#define PCICFG_NBIO_HB0_IOHUB_STATUS_COMMAND_FUNC         0
#define PCICFG_NBIO_HB0_IOHUB_STATUS_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_HB0_IOHUB_STATUS_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 1, 0, 0x4)

#define SMN_PCIEHB0_STATUS_COMMAND_ADDRESS    0x13b41004UL

#define PCICFG_NBIO_HB1_IOHUB_STATUS_COMMAND_DEVICE       2
#define PCICFG_NBIO_HB1_IOHUB_STATUS_COMMAND_FUNC         0
#define PCICFG_NBIO_HB1_IOHUB_STATUS_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_HB1_IOHUB_STATUS_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 2, 0, 0x4)

#define SMN_PCIEHB1_STATUS_COMMAND_ADDRESS    0x13b42004UL

#define SMN_PCIEHB2_STATUS_COMMAND_ADDRESS    0x13b43004UL

#define SMN_PCIEHB3_STATUS_COMMAND_ADDRESS    0x13b44004UL

#define SMN_PCIEHB4_STATUS_COMMAND_ADDRESS    0x13b48004UL


/***********************************************************
* Register Name : SW_LATENCY
* Register Description :
* PCIe Switch Latency register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Latency induced by bridge. Used to calculate conglomerated LTR value of PCIe switch. Value has granularity of 32ns.
#define SW_LATENCY_SwitchLatency_OFFSET      0
#define SW_LATENCY_SwitchLatency_MASK        0x3ff

// Bitfield Description : 
#define SW_LATENCY_Reserved_31_10_OFFSET      10
#define SW_LATENCY_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            SwitchLatency:10;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} SW_LATENCY_STRUCT;

#define SMN_INTSBDEVINDCFG0_SW_LATENCY_ADDRESS    0x13b3c010UL

#define SMN_NBIF1DEVINDCFG0_SW_LATENCY_ADDRESS    0x13b38010UL

#define SMN_NBIF1DEVINDCFG1_SW_LATENCY_ADDRESS    0x13b38410UL

#define SMN_NBIF1DEVINDCFG2_SW_LATENCY_ADDRESS    0x13b38810UL

#define SMN_PCIE0DEVINDCFG0_SW_LATENCY_ADDRESS    0x13b31010UL

#define SMN_PCIE0DEVINDCFG1_SW_LATENCY_ADDRESS    0x13b31410UL

#define SMN_PCIE0DEVINDCFG2_SW_LATENCY_ADDRESS    0x13b31810UL

#define SMN_PCIE0DEVINDCFG3_SW_LATENCY_ADDRESS    0x13b31c10UL

#define SMN_PCIE0DEVINDCFG4_SW_LATENCY_ADDRESS    0x13b32010UL

#define SMN_PCIE1DEVINDCFG0_SW_LATENCY_ADDRESS    0x13b33010UL

#define SMN_PCIE1DEVINDCFG1_SW_LATENCY_ADDRESS    0x13b33410UL

#define SMN_PCIE1DEVINDCFG2_SW_LATENCY_ADDRESS    0x13b33810UL

#define SMN_PCIE1DEVINDCFG3_SW_LATENCY_ADDRESS    0x13b33c10UL

#define SMN_PCIE1DEVINDCFG4_SW_LATENCY_ADDRESS    0x13b34010UL

#define SMN_PCIE1DEVINDCFG5_SW_LATENCY_ADDRESS    0x13b34410UL

#define SMN_PCIE2DEVINDCFG0_SW_LATENCY_ADDRESS    0x13b35010UL

#define SMN_PCIE3DEVINDCFG0_SW_LATENCY_ADDRESS    0x13b35410UL


/***********************************************************
* Register Name : SW_NMI_CNTL
* Register Description :
* Software Requesting NMI interrupt register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Software NMI Status.
// Software uses these bits to indicate the reasoning for issuing NMI.
// Writing 1 to bit[x] inverts the value of bit[x].
#define SW_NMI_CNTL_SW_NMI_Status_OFFSET      0
#define SW_NMI_CNTL_SW_NMI_Status_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SW_NMI_Status:32;
  } Field;
  UINT32 Value;
} SW_NMI_CNTL_STRUCT;

#define SMN_SW_NMI_CNTL_ADDRESS    0x13b10108UL


/***********************************************************
* Register Name : SW_SCI_CNTL
* Register Description :
* Software Requesting SCI interrupt register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Software SCI Status.
// Software uses these bits to indicate the reasoning for issuing SCI.
// Writing 1 to bit[x] inverts the value of bit[x].
#define SW_SCI_CNTL_SW_SCI_Status_OFFSET      0
#define SW_SCI_CNTL_SW_SCI_Status_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SW_SCI_Status:32;
  } Field;
  UINT32 Value;
} SW_SCI_CNTL_STRUCT;

#define SMN_SW_SCI_CNTL_ADDRESS    0x13b10110UL


/***********************************************************
* Register Name : SW_SMI_CNTL
* Register Description :
* Software Requesting SMI interrupt register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Software SMI Status.
// Software uses these bits to indicate the reasoning for issuing SMI.
// Writing 1 to bit[x] inverts the value of bit[x].
#define SW_SMI_CNTL_SW_SMI_Status_OFFSET      0
#define SW_SMI_CNTL_SW_SMI_Status_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SW_SMI_Status:32;
  } Field;
  UINT32 Value;
} SW_SMI_CNTL_STRUCT;

#define SMN_SW_SMI_CNTL_ADDRESS    0x13b1010cUL


/***********************************************************
* Register Name : SW_US_LOCATION
* Register Description :
* Switch Upstream Location Register. Register set 0 indicates there is no SWUS connected.
* Visibility : 0x2
************************************************************/

// Bitfield Description : <RWS> SW_US Port Location.
// One hot encoding register:
// 0x1 - SW_US located on port0 of SW_USlocated_Core
// 0x2 - SW_US located on port1 of SW_USlocated_Core
// 0x4 - SW_US located on port2 of SW_USlocated_Core
// 0x8 - SW_US located on port3 of SW_USlocated_Core
// etc...
#define SW_US_LOCATION_SW_USlocated_Port_OFFSET      0
#define SW_US_LOCATION_SW_USlocated_Port_MASK        0xffff

// Bitfield Description : <RWS> SW_US Core Location.
// One hot encoding register:
// 0x1 - SW_US resides under PCIE/NBIF/iFCH core0
// 0x2 - SW_US resides under PCIE/NBIF/iFCH core1
// 0x4 - SW_US resides under PCIE/NBIF/iFCH core2
// 0x8 - SW_US resides under PCIE/NBIF/iFCH core3
// etc...
#define SW_US_LOCATION_SW_USlocated_Core_OFFSET      16
#define SW_US_LOCATION_SW_USlocated_Core_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SW_USlocated_Port:16;
    UINT32                            SW_USlocated_Core:16;
  } Field;
  UINT32 Value;
} SW_US_LOCATION_STRUCT;

#define SMN_SW_US_LOCATION_ADDRESS    0x13b10080UL


/***********************************************************
* Register Name : SYNCFLOOD_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit indicates the status of an SDP ErrEvent triggered by the I/O RAS controller.
#define SYNCFLOOD_STATUS_SyncfloodFromRASCntl_OFFSET      0
#define SYNCFLOOD_STATUS_SyncfloodFromRASCntl_MASK        0x1

// Bitfield Description : This bit indicates the status of an SDP ErrEvent triggered by the APML interface.
#define SYNCFLOOD_STATUS_SyncfloodFromAPML_OFFSET      1
#define SYNCFLOOD_STATUS_SyncfloodFromAPML_MASK        0x2

// Bitfield Description : This bit indicates the status of an SDP ErrEvent triggered by the NMI_SYNCFLOOD# pin(LPC_SMI_LNMI_SYNC_FLOOD_L/AGPIO86).
#define SYNCFLOOD_STATUS_SyncfloodFromPin_OFFSET      2
#define SYNCFLOOD_STATUS_SyncfloodFromPin_MASK        0x4

// Bitfield Description : 
#define SYNCFLOOD_STATUS_Reserved_3_3_OFFSET      3
#define SYNCFLOOD_STATUS_Reserved_3_3_MASK        0x8

// Bitfield Description : This bit indicates the status of an SDP ErrEvent triggered by PSP or SMU via SW_SYNCFLOOD_PRIVATE.
#define SYNCFLOOD_STATUS_SyncfloodFromPrivate_OFFSET      4
#define SYNCFLOOD_STATUS_SyncfloodFromPrivate_MASK        0x10

// Bitfield Description : This bit indicates the status of an SDP ErrEvent triggered by MCA.
#define SYNCFLOOD_STATUS_SyncfloodFromMCA_OFFSET      5
#define SYNCFLOOD_STATUS_SyncfloodFromMCA_MASK        0x20

// Bitfield Description : 
#define SYNCFLOOD_STATUS_Reserved_7_6_OFFSET      6
#define SYNCFLOOD_STATUS_Reserved_7_6_MASK        0xc0

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 0.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_8_OFFSET      8
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_8_MASK        0x100

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 1.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_9_OFFSET      9
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_9_MASK        0x200

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 2.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_10_OFFSET      10
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_10_MASK        0x400

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 3.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_11_OFFSET      11
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_11_MASK        0x800

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 4.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_12_OFFSET      12
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_12_MASK        0x1000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 5.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_13_OFFSET      13
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_13_MASK        0x2000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 6.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_14_OFFSET      14
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_14_MASK        0x4000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 7.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_15_OFFSET      15
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_15_MASK        0x8000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 8.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_16_OFFSET      16
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_16_MASK        0x10000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 9.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_17_OFFSET      17
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_17_MASK        0x20000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 10.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_18_OFFSET      18
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_18_MASK        0x40000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 11.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_19_OFFSET      19
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_19_MASK        0x80000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 12.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_20_OFFSET      20
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_20_MASK        0x100000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 13.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_21_OFFSET      21
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_21_MASK        0x200000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 14.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_22_OFFSET      22
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_22_MASK        0x400000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 15.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_23_OFFSET      23
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_23_MASK        0x800000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 16.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_24_OFFSET      24
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_24_MASK        0x1000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 17.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_25_OFFSET      25
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_25_MASK        0x2000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 18.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_26_OFFSET      26
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_26_MASK        0x4000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 19.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_27_OFFSET      27
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_27_MASK        0x8000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 20.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_28_OFFSET      28
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_28_MASK        0x10000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 21.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_29_OFFSET      29
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_29_MASK        0x20000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 22.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_30_OFFSET      30
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_30_MASK        0x40000000

// Bitfield Description : This bit indicates the status of an SDP ErrEvent received from IOHC Port 23.
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_31_OFFSET      31
#define SYNCFLOOD_STATUS_SyncfloodFromIOHCPortN_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SyncfloodFromRASCntl:1;
    UINT32                            SyncfloodFromAPML:1;
    UINT32                            SyncfloodFromPin:1;
    UINT32                            Reserved_3_3:1;
    UINT32                            SyncfloodFromPrivate:1;
    UINT32                            SyncfloodFromMCA:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            SyncfloodFromIOHCPortN_8:1;
    UINT32                            SyncfloodFromIOHCPortN_9:1;
    UINT32                            SyncfloodFromIOHCPortN_10:1;
    UINT32                            SyncfloodFromIOHCPortN_11:1;
    UINT32                            SyncfloodFromIOHCPortN_12:1;
    UINT32                            SyncfloodFromIOHCPortN_13:1;
    UINT32                            SyncfloodFromIOHCPortN_14:1;
    UINT32                            SyncfloodFromIOHCPortN_15:1;
    UINT32                            SyncfloodFromIOHCPortN_16:1;
    UINT32                            SyncfloodFromIOHCPortN_17:1;
    UINT32                            SyncfloodFromIOHCPortN_18:1;
    UINT32                            SyncfloodFromIOHCPortN_19:1;
    UINT32                            SyncfloodFromIOHCPortN_20:1;
    UINT32                            SyncfloodFromIOHCPortN_21:1;
    UINT32                            SyncfloodFromIOHCPortN_22:1;
    UINT32                            SyncfloodFromIOHCPortN_23:1;
    UINT32                            SyncfloodFromIOHCPortN_24:1;
    UINT32                            SyncfloodFromIOHCPortN_25:1;
    UINT32                            SyncfloodFromIOHCPortN_26:1;
    UINT32                            SyncfloodFromIOHCPortN_27:1;
    UINT32                            SyncfloodFromIOHCPortN_28:1;
    UINT32                            SyncfloodFromIOHCPortN_29:1;
    UINT32                            SyncfloodFromIOHCPortN_30:1;
    UINT32                            SyncfloodFromIOHCPortN_31:1;
  } Field;
  UINT32 Value;
} SYNCFLOOD_STATUS_STRUCT;

#define SMN_SYNCFLOOD_STATUS_ADDRESS    0x13b20800UL


/***********************************************************
* Register Name : USB_QoS_CNTL
* Register Description :
* programmed UnitID is seen being sent to DF, when we set to the QoSPriority to the programmed value
* Visibility : 0x2
************************************************************/

// Bitfield Description : UnitID to set QoSPriority
#define USB_QoS_CNTL_UnitID0_OFFSET      0
#define USB_QoS_CNTL_UnitID0_MASK        0x7f

// Bitfield Description : 
#define USB_QoS_CNTL_Reserved_7_7_OFFSET      7
#define USB_QoS_CNTL_Reserved_7_7_MASK        0x80

// Bitfield Description : QoSPriority to set for UnitID0
#define USB_QoS_CNTL_UnitID0QoSPriority_OFFSET      8
#define USB_QoS_CNTL_UnitID0QoSPriority_MASK        0xf00

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID0
#define USB_QoS_CNTL_UnitID0Enable_OFFSET      12
#define USB_QoS_CNTL_UnitID0Enable_MASK        0x1000

// Bitfield Description : 
#define USB_QoS_CNTL_Reserved_15_13_OFFSET      13
#define USB_QoS_CNTL_Reserved_15_13_MASK        0xe000

// Bitfield Description : UnitID to set QoSPriority
#define USB_QoS_CNTL_UnitID1_OFFSET      16
#define USB_QoS_CNTL_UnitID1_MASK        0x7f0000

// Bitfield Description : 
#define USB_QoS_CNTL_Reserved_23_23_OFFSET      23
#define USB_QoS_CNTL_Reserved_23_23_MASK        0x800000

// Bitfield Description : QoSPriority to set for UnitID1
#define USB_QoS_CNTL_UnitID1QoSPriority_OFFSET      24
#define USB_QoS_CNTL_UnitID1QoSPriority_MASK        0xf000000

// Bitfield Description : Enable UnitIDQoSPrioty setting for UnitID1
#define USB_QoS_CNTL_UnitID1Enable_OFFSET      28
#define USB_QoS_CNTL_UnitID1Enable_MASK        0x10000000

// Bitfield Description : 
#define USB_QoS_CNTL_Reserved_31_29_OFFSET      29
#define USB_QoS_CNTL_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            UnitID0:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            UnitID0QoSPriority:4;
    UINT32                            UnitID0Enable:1;
    UINT32                            Reserved_15_13:3;
    UINT32                            UnitID1:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            UnitID1QoSPriority:4;
    UINT32                            UnitID1Enable:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} USB_QoS_CNTL_STRUCT;

#define SMN_USB_QoS_CNTL_ADDRESS    0x13b14044UL

//
// APML_STATUS Register Bitfields:
//

/// Bitfield Description : This bit indicates the status of a correctable error reported to APML.
#define APML_STATUS_APML_Corr_OFFSET      0
#define APML_STATUS_APML_Corr_WIDTH       1
#define APML_STATUS_APML_Corr_MASK        0x1

/// Bitfield Description : This bit indicates the status of a non-fatal error reported to APML.
#define APML_STATUS_APML_NonFatal_OFFSET      1
#define APML_STATUS_APML_NonFatal_WIDTH       1
#define APML_STATUS_APML_NonFatal_MASK        0x2

/// Bitfield Description : This bit indicates the status of a fatal error reported to APML.
#define APML_STATUS_APML_Fatal_OFFSET      2
#define APML_STATUS_APML_Fatal_WIDTH       1
#define APML_STATUS_APML_Fatal_MASK        0x4

/// Bitfield Description : This bit indicates the status of a system error reported to APML.
#define APML_STATUS_APML_Serr_OFFSET      3
#define APML_STATUS_APML_Serr_WIDTH       1
#define APML_STATUS_APML_Serr_MASK        0x8

/// Bitfield Description : This bit indicates the status of a Internal Poison Data error reported to APML.
#define APML_STATUS_APML_IntPoisonErr_OFFSET      4
#define APML_STATUS_APML_IntPoisonErr_WIDTH       1
#define APML_STATUS_APML_IntPoisonErr_MASK        0x10

/// Bitfield Description : This bit indicates the status of a Egress Poison Data Low Severity error reported to APML.
#define APML_STATUS_APML_EgressPoisonErrLo_OFFSET      5
#define APML_STATUS_APML_EgressPoisonErrLo_WIDTH       1
#define APML_STATUS_APML_EgressPoisonErrLo_MASK        0x20

/// Bitfield Description : This bit indicates the status of a Egress Poison Data High Severity error reported to APML.
#define APML_STATUS_APML_EgressPoisonErrHi_OFFSET      6
#define APML_STATUS_APML_EgressPoisonErrHi_WIDTH       1
#define APML_STATUS_APML_EgressPoisonErrHi_MASK        0x40

/// Bitfield Description :
#define APML_STATUS_Reserved_31_7_OFFSET      7
#define APML_STATUS_Reserved_31_7_WIDTH       25
#define APML_STATUS_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            APML_Corr:1;
    UINT32                            APML_NonFatal:1;
    UINT32                            APML_Fatal:1;
    UINT32                            APML_Serr:1;
    UINT32                            APML_IntPoisonErr:1;
    UINT32                            APML_EgressPoisonErrLo:1;
    UINT32                            APML_EgressPoisonErrHi:1;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} APML_STATUS_STRUCT;

//
// Register Name APML_STATUS
//
#define SMN_APML_STATUS_ADDRESS    0x13b20dc0UL

/***********************************************************
* Register Name : APML_CONTROL
* Register Description :
* APML Error Control Register.
* Visibility : 0x8
************************************************************/

// Bitfield Description : Enable NMI generation from APML.
#define APML_CONTROL_APML_NMI_En_OFFSET      0
#define APML_CONTROL_APML_NMI_En_MASK        0x1

// Bitfield Description : Enable SDP ErrEvent generation from APML.
#define APML_CONTROL_APML_SyncFlood_En_OFFSET      1
#define APML_CONTROL_APML_SyncFlood_En_MASK        0x2

// Bitfield Description :
#define APML_CONTROL_Reserved_7_2_OFFSET      2
#define APML_CONTROL_Reserved_7_2_MASK        0xfc

// Bitfield Description : Disable I/O RAS controller from signalling error status to APML. This prevents interrupt generation to SMU.
#define APML_CONTROL_APML_OutputDis_OFFSET      8
#define APML_CONTROL_APML_OutputDis_MASK        0x100

// Bitfield Description :
#define APML_CONTROL_Reserved_31_9_OFFSET      9
#define APML_CONTROL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            APML_NMI_En:1;
    UINT32                            APML_SyncFlood_En:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            APML_OutputDis:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} APML_CONTROL_STRUCT;

#define SMN_APML_CONTROL_ADDRESS    0x13b20dc4UL

#endif /* _IOHC_H_ */
