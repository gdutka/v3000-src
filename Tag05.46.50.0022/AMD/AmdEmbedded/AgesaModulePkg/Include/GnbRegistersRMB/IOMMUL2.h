/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IOMMUL2_H_
#define _IOMMUL2_H_


/***********************************************************
* Register Name : IOMMU_ADAPTER_ID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Subsystem vendor ID.
#define IOMMU_ADAPTER_ID_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define IOMMU_ADAPTER_ID_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Subsystem ID
#define IOMMU_ADAPTER_ID_SUBSYSTEM_ID_OFFSET      16
#define IOMMU_ADAPTER_ID_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} IOMMU_ADAPTER_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2c)

#define SMN_IOMMU_ADAPTER_ID_ADDRESS    0x13f0002cUL


/***********************************************************
* Register Name : IOMMU_ADAPTER_ID_W
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the subsystem vendor ID register in the configuration header.
#define IOMMU_ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_W_OFFSET      0
#define IOMMU_ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_W_MASK        0xffff

// Bitfield Description : Sets the subsystem ID register in the configuration header.
#define IOMMU_ADAPTER_ID_W_SUBSYSTEM_ID_W_OFFSET      16
#define IOMMU_ADAPTER_ID_W_SUBSYSTEM_ID_W_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID_W:16;
    UINT32                            SUBSYSTEM_ID_W:16;
  } Field;
  UINT32 Value;
} IOMMU_ADAPTER_ID_W_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_W_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_W_OFFSET       0x78
#define PCICFG_NBIO_IOHUB_IOMMU_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x78)

#define SMN_IOMMU_ADAPTER_ID_W_ADDRESS    0x13f00078UL


/***********************************************************
* Register Name : IOMMU_BASE_CODE
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sub-Class Code.
// System Base Peripheral.
#define IOMMU_BASE_CODE_BASE_CLASS_CODE_OFFSET      0
#define IOMMU_BASE_CODE_BASE_CLASS_CODE_MASK        0xff

typedef union {
  struct {
    UINT8                            BASE_CLASS_CODE:8;
  } Field;
  UINT8 Value;
} IOMMU_BASE_CODE_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_BASE_CODE_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_BASE_CODE_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_BASE_CODE_OFFSET       0xb
#define PCICFG_NBIO_IOHUB_IOMMU_BASE_CODE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xb)

#define SMN_IOMMU_BASE_CODE_ADDRESS    0x13f0000bUL


/***********************************************************
* Register Name : IOMMU_CAPABILITIES_PTR
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Capabilities Pointer.
// Points to the start of the capabilities list.
#define IOMMU_CAPABILITIES_PTR_CAP_PTR_OFFSET      0
#define IOMMU_CAPABILITIES_PTR_CAP_PTR_MASK        0xff

// Bitfield Description : 
#define IOMMU_CAPABILITIES_PTR_Reserved_31_8_OFFSET      8
#define IOMMU_CAPABILITIES_PTR_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            CAP_PTR:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} IOMMU_CAPABILITIES_PTR_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAPABILITIES_PTR_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAPABILITIES_PTR_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAPABILITIES_PTR_OFFSET       0x34
#define PCICFG_NBIO_IOHUB_IOMMU_CAPABILITIES_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x34)

#define SMN_IOMMU_CAPABILITIES_PTR_ADDRESS    0x13f00034UL


/***********************************************************
* Register Name : IOMMU_CAP_BASE_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies address bits 63:32 of the 512K-byte-aligned base address of the IOMMU memory-mapped control registers.
#define IOMMU_CAP_BASE_HI_IOMMU_BASE_ADDR_HI_OFFSET      0
#define IOMMU_CAP_BASE_HI_IOMMU_BASE_ADDR_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IOMMU_BASE_ADDR_HI:32;
  } Field;
  UINT32 Value;
} IOMMU_CAP_BASE_HI_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_HI_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_HI_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_HI_OFFSET       0x48
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x48)

#define SMN_IOMMU_CAP_BASE_HI_ADDRESS    0x13f00048UL


/***********************************************************
* Register Name : IOMMU_CAP_BASE_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=IOMMU accepts memory accesses to the address specified in the Base Address Register. When Enable is written with a 1, all IOMMU RW capability registers in PCI configuration space are locked until the next system reset.
#define IOMMU_CAP_BASE_LO_IOMMU_ENABLE_OFFSET      0
#define IOMMU_CAP_BASE_LO_IOMMU_ENABLE_MASK        0x1

// Bitfield Description : 
#define IOMMU_CAP_BASE_LO_Reserved_18_1_OFFSET      1
#define IOMMU_CAP_BASE_LO_Reserved_18_1_MASK        0x7fffe

// Bitfield Description : Specifies address bits 31:19 of the 512K-byte-aligned base address of the IOMMU memory-mapped control registers.
#define IOMMU_CAP_BASE_LO_IOMMU_BASE_ADDR_LO_OFFSET      19
#define IOMMU_CAP_BASE_LO_IOMMU_BASE_ADDR_LO_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            IOMMU_ENABLE:1;
    UINT32                            Reserved_18_1:18;
    UINT32                            IOMMU_BASE_ADDR_LO:13;
  } Field;
  UINT32 Value;
} IOMMU_CAP_BASE_LO_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_LO_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_LO_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_LO_OFFSET       0x44
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_BASE_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x44)

#define SMN_IOMMU_CAP_BASE_LO_ADDRESS    0x13f00044UL


/***********************************************************
* Register Name : IOMMU_CAP_HEADER
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates a Secure Device capability block.
#define IOMMU_CAP_HEADER_IOMMU_CAP_ID_OFFSET      0
#define IOMMU_CAP_HEADER_IOMMU_CAP_ID_MASK        0xff

// Bitfield Description : Indicates the location of the next capability block if one is present.
#define IOMMU_CAP_HEADER_IOMMU_CAP_PTR_OFFSET      8
#define IOMMU_CAP_HEADER_IOMMU_CAP_PTR_MASK        0xff00

// Bitfield Description : Specifies the layout of the Capability Block as an IOMMU capability block.
#define IOMMU_CAP_HEADER_IOMMU_CAP_TYPE_OFFSET      16
#define IOMMU_CAP_HEADER_IOMMU_CAP_TYPE_MASK        0x70000

// Bitfield Description : Specifies the IOMMU specification revision.
#define IOMMU_CAP_HEADER_IOMMU_CAP_REV_OFFSET      19
#define IOMMU_CAP_HEADER_IOMMU_CAP_REV_MASK        0xf80000

// Bitfield Description : Indicates support for remote IOTLBs.
#define IOMMU_CAP_HEADER_IOMMU_IO_TLBSUP_OFFSET      24
#define IOMMU_CAP_HEADER_IOMMU_IO_TLBSUP_MASK        0x1000000

// Bitfield Description : Hypertransport tunnel translation support. This register is always set to 0 to indicate that the device does not contain a hypertransport tunnel supporting address translation.
#define IOMMU_CAP_HEADER_IOMMU_HT_TUNNEL_SUP_OFFSET      25
#define IOMMU_CAP_HEADER_IOMMU_HT_TUNNEL_SUP_MASK        0x2000000

// Bitfield Description : 1=Indicates that the IOMMU caches page table entries that are marked as not present. When this bit is set, software must issue an invalidate after any change to a PDE or PTE.
// 0=Indicates that the IOMMU caches only page table entries that are marked as present. When NpCache is clear, software must issue an invalidate after any change to a PDE or PTE marked present before the change.
// Implementation note: For hardware implementations of the IOMMU, this bit must be 0b.
#define IOMMU_CAP_HEADER_IOMMU_NP_CACHE_OFFSET      26
#define IOMMU_CAP_HEADER_IOMMU_NP_CACHE_MASK        0x4000000

// Bitfield Description : 1=Indicates IOMMU Extended Feature Register [MMIO Offset 0030h] is supported.
// 0=[MMIO Offset 0030h] is reserved.
#define IOMMU_CAP_HEADER_IOMMU_EFR_SUP_OFFSET      27
#define IOMMU_CAP_HEADER_IOMMU_EFR_SUP_MASK        0x8000000

// Bitfield Description : 1=Indicates support for IOMMU Miscellaneous Information Register 1 [Capability Offset 14h]. 0=Capability Offset14h not supported.
#define IOMMU_CAP_HEADER_IOMMU_CAP_EXT_OFFSET      28
#define IOMMU_CAP_HEADER_IOMMU_CAP_EXT_MASK        0x10000000

// Bitfield Description : 
#define IOMMU_CAP_HEADER_Reserved_31_29_OFFSET      29
#define IOMMU_CAP_HEADER_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            IOMMU_CAP_ID_BITS:8;
    UINT32                            IOMMU_CAP_PTR:8;
    UINT32                            IOMMU_CAP_TYPE:3;
    UINT32                            IOMMU_CAP_REV:5;
    UINT32                            IOMMU_IO_TLBSUP:1;
    UINT32                            IOMMU_HT_TUNNEL_SUP:1;
    UINT32                            IOMMU_NP_CACHE:1;
    UINT32                            IOMMU_EFR_SUP:1;
    UINT32                            IOMMU_CAP_EXT:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} IOMMU_CAP_HEADER_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_HEADER_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_HEADER_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_HEADER_OFFSET       0x40
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x40)

#define SMN_IOMMU_CAP_HEADER_ADDRESS    0x13f00040UL


/***********************************************************
* Register Name : IOMMU_CAP_MISC
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the MSI vector used for interrupt messages generated by the IOMMU.
#define IOMMU_CAP_MISC_IOMMU_MSI_NUM_OFFSET      0
#define IOMMU_CAP_MISC_IOMMU_MSI_NUM_MASK        0x1f

// Bitfield Description : This field indicates the width of the maximum guest virtual address processed by iommu. 010=48 Bits.
#define IOMMU_CAP_MISC_IOMMU_GVA_SIZE_OFFSET      5
#define IOMMU_CAP_MISC_IOMMU_GVA_SIZE_MASK        0xe0

// Bitfield Description : This field must indicate the size of the maximum physical address generated by the IOMMU. The value is the (unsigned) binary log of the maximum address size. Allowed values are 40, 48 and 52; all other values are reserved.
// 010_1000b = 40 bits
// 011_0000b = 48 bits
// 011_0100b = 52 bits
#define IOMMU_CAP_MISC_IOMMU_PA_SIZE_OFFSET      8
#define IOMMU_CAP_MISC_IOMMU_PA_SIZE_MASK        0x7f00

// Bitfield Description : This field must indicate the size of the maximum virtual address processed by the IOMMU. The value is the (unsigned) binary log of the maximum address size. Allowed values are 32, 40, 48, and 64; all other values are reserved.
// 010_0000b = 32 bits
// 010_1000b = 40 bits
// 011_0000b = 48 bits
#define IOMMU_CAP_MISC_IOMMU_VA_SIZE_OFFSET      15
#define IOMMU_CAP_MISC_IOMMU_VA_SIZE_MASK        0x3f8000

// Bitfield Description : 1=The HyperTransport? Address Translation address range for ATS responses is reserved and cannot be translated by the IOMMU.
// 0=The Address Translation address range can be translated by the IOMMU.
#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_OFFSET      22
#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_MASK        0x400000

// Bitfield Description : 
#define IOMMU_CAP_MISC_Reserved_26_23_OFFSET      23
#define IOMMU_CAP_MISC_Reserved_26_23_MASK        0x7800000

// Bitfield Description : This field must indicate which MSI vector is used for the interrupt message generated by the IOMMU for the peripheral page service request log when [MMIO Offset 0030h[PPR_SUP]] = 1. IOMMU_MSI_NUM_PPR must be 0 when PPR_SUP = 0. For MSI there can be only one IOMMU so this field must be 0. This interrupt is not remapped by the IOMMU.
#define IOMMU_CAP_MISC_IOMMU_MSI_NUM_PPR_OFFSET      27
#define IOMMU_CAP_MISC_IOMMU_MSI_NUM_PPR_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            IOMMU_MSI_NUM:5;
    UINT32                            IOMMU_GVA_SIZE:3;
    UINT32                            IOMMU_PA_SIZE:7;
    UINT32                            IOMMU_VA_SIZE:7;
    UINT32                            IOMMU_HT_ATS_RESV:1;
    UINT32                            Reserved_26_23:4;
    UINT32                            IOMMU_MSI_NUM_PPR:5;
  } Field;
  UINT32 Value;
} IOMMU_CAP_MISC_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_OFFSET       0x50
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x50)

#define SMN_IOMMU_CAP_MISC_ADDRESS    0x13f00050UL


/***********************************************************
* Register Name : IOMMU_CAP_MISC_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : MSI message number. Message number for MSI or MSI-X interrupt associated with the guest vAPIC virtual interrupt request log.
#define IOMMU_CAP_MISC_1_IOMMU_MSI_NUM_GA_OFFSET      0
#define IOMMU_CAP_MISC_1_IOMMU_MSI_NUM_GA_MASK        0x1f

// Bitfield Description : Indicated the working architecture mode of IOMMU. 0=x86 Mode, 1=ARM v1 Mode.
#define IOMMU_CAP_MISC_1_IOMMU_ARCH_MODE_OFFSET      5
#define IOMMU_CAP_MISC_1_IOMMU_ARCH_MODE_MASK        0x20

// Bitfield Description : 0=Enable DVM- do not block DVM based on PNEs.
// 1= Block DVM based on PNEs (refer to IOMMU IOMMU Spec for details).
// 2= Disable DVM - block all DVM.
#define IOMMU_CAP_MISC_1_DVM_MODE_OFFSET      6
#define IOMMU_CAP_MISC_1_DVM_MODE_MASK        0xc0

// Bitfield Description : Indicates the smallest translation unit supported by IOMMU. 0 = 4KB (expected to be set only during bring-up mode). 1 = 16KB (4 contiguous 4KB PTEs, expected to be production mode). Applies to both guest and host translations
#define IOMMU_CAP_MISC_1_STU_SIZE_SEL_OFFSET      8
#define IOMMU_CAP_MISC_1_STU_SIZE_SEL_MASK        0x100

// Bitfield Description : Indicates the page table format used by IOMMU. 0= IOMMU uses existing AMD64 long mode format. 1= IOMMU uses unified guest page table format.
#define IOMMU_CAP_MISC_1_GST_PAGE_TABLE_FMT_SEL_OFFSET      9
#define IOMMU_CAP_MISC_1_GST_PAGE_TABLE_FMT_SEL_MASK        0x200

// Bitfield Description : 
#define IOMMU_CAP_MISC_1_Reserved_14_10_OFFSET      10
#define IOMMU_CAP_MISC_1_Reserved_14_10_MASK        0x7c00

// Bitfield Description : 1 = Allow memory access to SMMU MMIO space. 0= Disable memory access to SMMU MMIO space. When this register is set to 1'b0, access will behave as to non-implemented registers. Successful write completion and read completion with all 0 return data.
#define IOMMU_CAP_MISC_1_SMMUMMIO_EN_OFFSET      15
#define IOMMU_CAP_MISC_1_SMMUMMIO_EN_MASK        0x8000

// Bitfield Description : 
#define IOMMU_CAP_MISC_1_Reserved_30_16_OFFSET      16
#define IOMMU_CAP_MISC_1_Reserved_30_16_MASK        0x7fff0000

// Bitfield Description : RW1S Register. 1 = SMMUMMIO_EN bit is lock, 0=SMMUMMIO_EN bit is unlocked. Once this bit is set to 1'b1, the SMMUMMIO_EN bit is locked until the next system (cold) reset)
#define IOMMU_CAP_MISC_1_SMMUMMIO_LOCK_OFFSET      31
#define IOMMU_CAP_MISC_1_SMMUMMIO_LOCK_MASK        0x80000000

typedef union {
  struct {
    UINT32                            IOMMU_MSI_NUM_GA:5;
    UINT32                            IOMMU_ARCH_MODE:1;
    UINT32                            DVM_MODE:2;
    UINT32                            STU_SIZE_SEL:1;
    UINT32                            GST_PAGE_TABLE_FMT_SEL:1;
    UINT32                            Reserved_14_10:5;
    UINT32                            SMMUMMIO_EN:1;
    UINT32                            Reserved_30_16:15;
    UINT32                            SMMUMMIO_LOCK:1;
  } Field;
  UINT32 Value;
} IOMMU_CAP_MISC_1_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_1_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_1_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_1_OFFSET       0x54
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_MISC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x54)

#define SMN_IOMMU_CAP_MISC_1_ADDRESS    0x13f00054UL


/***********************************************************
* Register Name : IOMMU_CAP_RANGE
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field returns the HyperTransport UnitID used by the IOMMU.
#define IOMMU_CAP_RANGE_IOMMU_UNIT_ID_OFFSET      0
#define IOMMU_CAP_RANGE_IOMMU_UNIT_ID_MASK        0x1f

// Bitfield Description : 
#define IOMMU_CAP_RANGE_Reserved_6_5_OFFSET      5
#define IOMMU_CAP_RANGE_Reserved_6_5_MASK        0x60

// Bitfield Description : 1=The IOMMU_BUS_NUMBER, IOMMU_FIRST_DEVICE and IOMMU_LAST_DEVICE fields are valid. Although the register contents are valid, software is encouraged to use I/O topology information.
// 0=Software must use I/O topology information.
#define IOMMU_CAP_RANGE_IOMMU_RNG_VALID_OFFSET      7
#define IOMMU_CAP_RANGE_IOMMU_RNG_VALID_MASK        0x80

// Bitfield Description : Indicates the bus number that IOMMU_FIRST_DEVICE and IOMMU_LAST_DEVICE reside on.
#define IOMMU_CAP_RANGE_IOMMU_BUS_NUMBER_OFFSET      8
#define IOMMU_CAP_RANGE_IOMMU_BUS_NUMBER_MASK        0xff00

// Bitfield Description : Indicates device and function number of the first integrated device associated with the IOMMU.
#define IOMMU_CAP_RANGE_IOMMU_FIRST_DEVICE_OFFSET      16
#define IOMMU_CAP_RANGE_IOMMU_FIRST_DEVICE_MASK        0xff0000

// Bitfield Description : Indicates device and function number of the last integrated device associated with the IOMMU.
#define IOMMU_CAP_RANGE_IOMMU_LAST_DEVICE_OFFSET      24
#define IOMMU_CAP_RANGE_IOMMU_LAST_DEVICE_MASK        0xff000000

typedef union {
  struct {
    UINT32                            IOMMU_UNIT_ID:5;
    UINT32                            Reserved_6_5:2;
    UINT32                            IOMMU_RNG_VALID:1;
    UINT32                            IOMMU_BUS_NUMBER:8;
    UINT32                            IOMMU_FIRST_DEVICE:8;
    UINT32                            IOMMU_LAST_DEVICE:8;
  } Field;
  UINT32 Value;
} IOMMU_CAP_RANGE_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CAP_RANGE_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_RANGE_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_RANGE_OFFSET       0x4c
#define PCICFG_NBIO_IOHUB_IOMMU_CAP_RANGE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4c)

#define SMN_IOMMU_CAP_RANGE_ADDRESS    0x13f0004cUL


/***********************************************************
* Register Name : IOMMU_COMMAND
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : I/O Access Enable
// This register controls no hardware. This function does not declare any standard I/O BARs.
#define IOMMU_COMMAND_IO_ACCESS_EN_OFFSET      0
#define IOMMU_COMMAND_IO_ACCESS_EN_MASK        0x1

// Bitfield Description : Memory Access Enable
// This register controls no hardware. This function does not declare any standard memory BARs.
#define IOMMU_COMMAND_MEM_ACCESS_EN_OFFSET      1
#define IOMMU_COMMAND_MEM_ACCESS_EN_MASK        0x2

// Bitfield Description : Bus Master Enable
// 1=This function is allowed to generate DMA requests.
// 0=This function is not allowed to generate DMA reqests.
#define IOMMU_COMMAND_BUS_MASTER_EN_OFFSET      2
#define IOMMU_COMMAND_BUS_MASTER_EN_MASK        0x4

// Bitfield Description : 
#define IOMMU_COMMAND_Reserved_5_3_OFFSET      3
#define IOMMU_COMMAND_Reserved_5_3_MASK        0x38

// Bitfield Description : Parity Error Enable.
// 1=This function is allowed to set the PARITY_ERROR_DETECTED status bit.
// 0=This function is not allowed to set the PARITY_ERROR_DETECTED status bit.
#define IOMMU_COMMAND_PARITY_ERROR_EN_OFFSET      6
#define IOMMU_COMMAND_PARITY_ERROR_EN_MASK        0x40

// Bitfield Description : 
#define IOMMU_COMMAND_Reserved_7_7_OFFSET      7
#define IOMMU_COMMAND_Reserved_7_7_MASK        0x80

// Bitfield Description : System Error Enable.
// This function does not set the signaled system error status bit.
#define IOMMU_COMMAND_SERR_EN_OFFSET      8
#define IOMMU_COMMAND_SERR_EN_MASK        0x100

// Bitfield Description : 
#define IOMMU_COMMAND_Reserved_9_9_OFFSET      9
#define IOMMU_COMMAND_Reserved_9_9_MASK        0x200

// Bitfield Description : Interrupt Disable.
// 1=This function is not allowed to generate legacy INTx interrupts.
// 0=This function is allowed to generate legacy INTx interrupts.
#define IOMMU_COMMAND_INTERRUPT_DIS_OFFSET      10
#define IOMMU_COMMAND_INTERRUPT_DIS_MASK        0x400

// Bitfield Description : 
#define IOMMU_COMMAND_Reserved_15_11_OFFSET      11
#define IOMMU_COMMAND_Reserved_15_11_MASK        0xf800

typedef union {
  struct {
    UINT16                            IO_ACCESS_EN:1;
    UINT16                            MEM_ACCESS_EN:1;
    UINT16                            BUS_MASTER_EN:1;
    UINT16                            Reserved_5_3:3;
    UINT16                            PARITY_ERROR_EN:1;
    UINT16                            Reserved_7_7:1;
    UINT16                            SERR_EN:1;
    UINT16                            Reserved_9_9:1;
    UINT16                            INTERRUPT_DIS:1;
    UINT16                            Reserved_15_11:5;
  } Field;
  UINT16 Value;
} IOMMU_COMMAND_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_COMMAND_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_COMMAND_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_IOHUB_IOMMU_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4)

#define SMN_IOMMU_COMMAND_ADDRESS    0x13f00004UL


/***********************************************************
* Register Name : IOMMU_CONTROL_W
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the INTERRUPT_PIN register in the configuration header.
// 0 = No legacy interrupts supported.
// 1 = INTA
// 2 = INTB
// 3 = INTC
// 4 = INTD
#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_OFFSET      0
#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_MASK        0x7

// Bitfield Description : 
#define IOMMU_CONTROL_W_Reserved_3_3_OFFSET      3
#define IOMMU_CONTROL_W_Reserved_3_3_MASK        0x8

// Bitfield Description : Sets the minor revision ID register in the configuration header.
#define IOMMU_CONTROL_W_MINOR_REV_ID_W_OFFSET      4
#define IOMMU_CONTROL_W_MINOR_REV_ID_W_MASK        0xf0

// Bitfield Description : Sets the value of IO_TLBSUP in the IOMMU capability.
#define IOMMU_CONTROL_W_IO_TLBSUP_W_OFFSET      8
#define IOMMU_CONTROL_W_IO_TLBSUP_W_MASK        0x100

// Bitfield Description : Sets the value of EFR_SUP in the IOMMU capability, program the L1 copy of this register in IOMMUL1::L1_FEATURE_SUP_CNTRL[L1_EFR_SUP].
#define IOMMU_CONTROL_W_EFR_SUP_W_OFFSET      9
#define IOMMU_CONTROL_W_EFR_SUP_W_MASK        0x200

// Bitfield Description : Sets the number of MSI messages requested by this function.
#define IOMMU_CONTROL_W_MSI_MULT_MESS_CAP_W_OFFSET      10
#define IOMMU_CONTROL_W_MSI_MULT_MESS_CAP_W_MASK        0x1c00

// Bitfield Description : 1=Indicates support for IOMMU Miscellaneous Information Register 1 [Capability Offset 14h]. 0=Capability Offset14h not supported.
#define IOMMU_CONTROL_W_IOMMU_CAP_EXT_W_OFFSET      13
#define IOMMU_CONTROL_W_IOMMU_CAP_EXT_W_MASK        0x2000

// Bitfield Description : 
#define IOMMU_CONTROL_W_Reserved_31_14_OFFSET      14
#define IOMMU_CONTROL_W_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            INTERRUPT_PIN_W:3;
    UINT32                            Reserved_3_3:1;
    UINT32                            MINOR_REV_ID_W:4;
    UINT32                            IO_TLBSUP_W:1;
    UINT32                            EFR_SUP_W:1;
    UINT32                            MSI_MULT_MESS_CAP_W:3;
    UINT32                            IOMMU_CAP_EXT_W:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} IOMMU_CONTROL_W_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_CONTROL_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_CONTROL_W_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_CONTROL_W_OFFSET       0x7c
#define PCICFG_NBIO_IOHUB_IOMMU_CONTROL_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x7c)

#define SMN_IOMMU_CONTROL_W_ADDRESS    0x13f0007cUL


/***********************************************************
* Register Name : IOMMU_DEVICE_ID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Device Identifier. This 16-bit field is assigned by the device manufacturer and identifies the type of device.
#define IOMMU_DEVICE_ID_DEVICE_ID_OFFSET      0
#define IOMMU_DEVICE_ID_DEVICE_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            DEVICE_ID:16;
  } Field;
  UINT16 Value;
} IOMMU_DEVICE_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_DEVICE_ID_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_IOHUB_IOMMU_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2)

#define SMN_IOMMU_DEVICE_ID_ADDRESS    0x13f00002UL


/***********************************************************
* Register Name : IOMMU_DSFX_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Tells software which feature(bit n) is supported, goes to MMIO.
#define IOMMU_DSFX_CONTROL_DSFXSup_OFFSET      0
#define IOMMU_DSFX_CONTROL_DSFXSup_MASK        0xffffff

// Bitfield Description : States Minor revision number.
#define IOMMU_DSFX_CONTROL_REVISION_MINOR_OFFSET      24
#define IOMMU_DSFX_CONTROL_REVISION_MINOR_MASK        0xf000000

// Bitfield Description : States Major revision number.
#define IOMMU_DSFX_CONTROL_REVISION_MAJOR_OFFSET      28
#define IOMMU_DSFX_CONTROL_REVISION_MAJOR_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            DSFXSup:24;
    UINT32                            REVISION_MINOR:4;
    UINT32                            REVISION_MAJOR:4;
  } Field;
  UINT32 Value;
} IOMMU_DSFX_CONTROL_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_DSFX_CONTROL_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_DSFX_CONTROL_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_DSFX_CONTROL_OFFSET       0x8c
#define PCICFG_NBIO_IOHUB_IOMMU_DSFX_CONTROL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8c)

#define SMN_IOMMU_DSFX_CONTROL_ADDRESS    0x13f0008cUL


/***********************************************************
* Register Name : IOMMU_GST_PREFETCH_CNTRL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if Access bit is required to be preset for prefetching guest PTEs.0=guest PTEs are not required to have A bit preset in order for HW to prefetch them.1=guest PTEs are required to have A preset for HW to prefetch them.This register only applies to prefetched guest PTEs (not to original PTE)
#define IOMMU_GST_PREFETCH_CNTRL_0_GST_PREFETCH_ACCESS_PRESET_OFFSET      0
#define IOMMU_GST_PREFETCH_CNTRL_0_GST_PREFETCH_ACCESS_PRESET_MASK        0x1

// Bitfield Description : 
#define IOMMU_GST_PREFETCH_CNTRL_0_Reserved_31_1_OFFSET      1
#define IOMMU_GST_PREFETCH_CNTRL_0_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            GST_PREFETCH_ACCESS_PRESET:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} IOMMU_GST_PREFETCH_CNTRL_0_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_GST_PREFETCH_CNTRL_0_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_GST_PREFETCH_CNTRL_0_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_GST_PREFETCH_CNTRL_0_OFFSET       0xe0
#define PCICFG_NBIO_IOHUB_IOMMU_GST_PREFETCH_CNTRL_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xe0)

#define SMN_IOMMU_GST_PREFETCH_CNTRL_0_ADDRESS    0x13f000e0UL


/***********************************************************
* Register Name : IOMMU_HEADER
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Header Type.
// Indicates a multi-function device with a type 0 configuration space header.
#define IOMMU_HEADER_HEADER_TYPE_OFFSET      0
#define IOMMU_HEADER_HEADER_TYPE_MASK        0xff

typedef union {
  struct {
    UINT8                            HEADER_TYPE:8;
  } Field;
  UINT8 Value;
} IOMMU_HEADER_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_HEADER_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_HEADER_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_HEADER_OFFSET       0xe
#define PCICFG_NBIO_IOHUB_IOMMU_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xe)

#define SMN_IOMMU_HEADER_ADDRESS    0x13f0000eUL


/***********************************************************
* Register Name : IOMMU_INTERRUPT_LINE
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is read/write for software compatibility. It controls no hardware.
#define IOMMU_INTERRUPT_LINE_INTERRUPT_LINE_OFFSET      0
#define IOMMU_INTERRUPT_LINE_INTERRUPT_LINE_MASK        0xff

typedef union {
  struct {
    UINT8                            INTERRUPT_LINE:8;
  } Field;
  UINT8 Value;
} IOMMU_INTERRUPT_LINE_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_LINE_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3c)

#define SMN_IOMMU_INTERRUPT_LINE_ADDRESS    0x13f0003cUL


/***********************************************************
* Register Name : IOMMU_INTERRUPT_PIN
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field indicates the INTx line used to generate legacy interrupts.
// 1 - INTA
// 2 - INTB
// 3 - INTC
// 4 - INTD
// All other encodings are not supported.
#define IOMMU_INTERRUPT_PIN_INTERRUPT_PIN_OFFSET      0
#define IOMMU_INTERRUPT_PIN_INTERRUPT_PIN_MASK        0xff

typedef union {
  struct {
    UINT8                            INTERRUPT_PIN:8;
  } Field;
  UINT8 Value;
} IOMMU_INTERRUPT_PIN_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_PIN_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_IOHUB_IOMMU_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3d)

#define SMN_IOMMU_INTERRUPT_PIN_ADDRESS    0x13f0003dUL


/***********************************************************
* Register Name : IOMMU_MMIO_CONTROL0_W
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the PREF_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_PREF_SUP_W_OFFSET      0
#define IOMMU_MMIO_CONTROL0_W_PREF_SUP_W_MASK        0x1

// Bitfield Description : Sets the PPR_SUP value in IOMMU Extended feature register, program the L1 copy of this register in IOMMUL1::L1_FEATURE_SUP_CNTRL[L1_PPR_SUP].
#define IOMMU_MMIO_CONTROL0_W_PPR_SUP_W_OFFSET      1
#define IOMMU_MMIO_CONTROL0_W_PPR_SUP_W_MASK        0x2

// Bitfield Description : Sets the XT_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_XT_SUP_W_OFFSET      2
#define IOMMU_MMIO_CONTROL0_W_XT_SUP_W_MASK        0x4

// Bitfield Description : Sets the NX_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_NX_SUP_W_OFFSET      3
#define IOMMU_MMIO_CONTROL0_W_NX_SUP_W_MASK        0x8

// Bitfield Description : Sets the GT_SUP value in IOMMU Extended feature register, program the L1 copy of this register in IOMMUL1::L1_FEATURE_SUP_CNTRL[L1_GT_SUP_W].
#define IOMMU_MMIO_CONTROL0_W_GT_SUP_W_OFFSET      4
#define IOMMU_MMIO_CONTROL0_W_GT_SUP_W_MASK        0x10

// Bitfield Description : 1 = Hardware supports sATS.
#define IOMMU_MMIO_CONTROL0_W_sATS_SUP_W_OFFSET      5
#define IOMMU_MMIO_CONTROL0_W_sATS_SUP_W_MASK        0x20

// Bitfield Description : Sets the IA_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_IA_SUP_W_OFFSET      6
#define IOMMU_MMIO_CONTROL0_W_IA_SUP_W_MASK        0x40

// Bitfield Description : Sets the GA_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_GA_SUP_W_OFFSET      7
#define IOMMU_MMIO_CONTROL0_W_GA_SUP_W_MASK        0x80

// Bitfield Description : Sets the HE_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_HE_SUP_W_OFFSET      8
#define IOMMU_MMIO_CONTROL0_W_HE_SUP_W_MASK        0x100

// Bitfield Description : Sets the PC_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_PC_SUP_W_OFFSET      9
#define IOMMU_MMIO_CONTROL0_W_PC_SUP_W_MASK        0x200

// Bitfield Description : Sets the HATS value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_HATS_W_OFFSET      10
#define IOMMU_MMIO_CONTROL0_W_HATS_W_MASK        0xc00

// Bitfield Description : Sets the US_SUP value in IOMMU Extended featuer registers.
#define IOMMU_MMIO_CONTROL0_W_US_SUP_W_OFFSET      12
#define IOMMU_MMIO_CONTROL0_W_US_SUP_W_MASK        0x1000

// Bitfield Description : Sets the GATS value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_GATS_W_OFFSET      13
#define IOMMU_MMIO_CONTROL0_W_GATS_W_MASK        0x6000

// Bitfield Description : 
#define IOMMU_MMIO_CONTROL0_W_Reserved_20_15_OFFSET      15
#define IOMMU_MMIO_CONTROL0_W_Reserved_20_15_MASK        0x1f8000

// Bitfield Description : Sets the GAM_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET      21
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK        0xe00000

// Bitfield Description : 0x0=PPR log dual buffer not supported.
// 0x1=PPR log dual buffer supported without autoswap.
// 0x2=PPR log dual buffer autoswap supported.
// All other encodings reserved.
#define IOMMU_MMIO_CONTROL0_W_PPRF_W_OFFSET      24
#define IOMMU_MMIO_CONTROL0_W_PPRF_W_MASK        0x3000000

// Bitfield Description : 
#define IOMMU_MMIO_CONTROL0_W_Reserved_27_26_OFFSET      26
#define IOMMU_MMIO_CONTROL0_W_Reserved_27_26_MASK        0xc000000

// Bitfield Description : 0x0=Event log dual buffer autoswap not supported.
// 0x1=Event log dual buffer supported without autoswap.
// 0x2=Event log dual buffer autoswap supported.
// All other encodings reserved.
#define IOMMU_MMIO_CONTROL0_W_EVENTF_W_OFFSET      28
#define IOMMU_MMIO_CONTROL0_W_EVENTF_W_MASK        0x30000000

// Bitfield Description : 00=single-level guest CR3 base table address translation is support.When GLXSup=00b, the value of GLX in the DTE is ignored and the IOMMU performs only single-level Guest CR3 lookups.
// 01=Two-level GCR3 base address table is supported in hardware.
// 10= Three-level GCR3 base address table is supported in hardware for 20-bit PASID values in the future.
// 11= Reserved.single-level GCR3 base table address translation is supported. When GLX_SUP=0, GLX in the DTE is ignored. The value of GLXSup is not meaningful when GTSup=0.
#define IOMMU_MMIO_CONTROL0_W_GLX_SUP_W_OFFSET      30
#define IOMMU_MMIO_CONTROL0_W_GLX_SUP_W_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PREF_SUP_W:1;
    UINT32                            PPR_SUP_W:1;
    UINT32                            XT_SUP_W:1;
    UINT32                            NX_SUP_W:1;
    UINT32                            GT_SUP_W:1;
    UINT32                            sATS_SUP_W:1;
    UINT32                            IA_SUP_W:1;
    UINT32                            GA_SUP_W:1;
    UINT32                            HE_SUP_W:1;
    UINT32                            PC_SUP_W:1;
    UINT32                            HATS_W:2;
    UINT32                            US_SUP_W:1;
    UINT32                            GATS_W:2;
    UINT32                            Reserved_20_15:6;
    UINT32                            GAM_SUP_W:3;
    UINT32                            PPRF_W:2;
    UINT32                            Reserved_27_26:2;
    UINT32                            EVENTF_W:2;
    UINT32                            GLX_SUP_W:2;
  } Field;
  UINT32 Value;
} IOMMU_MMIO_CONTROL0_W_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL0_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL0_W_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL0_W_OFFSET       0x80
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL0_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x80)

#define SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS    0x13f00080UL


/***********************************************************
* Register Name : IOMMU_MMIO_CONTROL1_W
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the PAS_MAX value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL1_W_PAS_MAX_W_OFFSET      0
#define IOMMU_MMIO_CONTROL1_W_PAS_MAX_W_MASK        0xf

// Bitfield Description : 
#define IOMMU_MMIO_CONTROL1_W_Reserved_5_4_OFFSET      4
#define IOMMU_MMIO_CONTROL1_W_Reserved_5_4_MASK        0x30

// Bitfield Description : 0x0 - Hardware supports no DTE segmentation.
// 0x1 - Hardware supports 2-way DTE segmentation.
// 0x2 - Hardware supports 4-way DTE segmentation.
// 0x3 - Hardware supports 8-way DTE segmentation.
// Program the L1 copy of this register in IOMMUL1::L1_FEATURE_SUP_CNTRL[L1_DTE_seg_W].
#define IOMMU_MMIO_CONTROL1_W_DTE_seg_W_OFFSET      6
#define IOMMU_MMIO_CONTROL1_W_DTE_seg_W_MASK        0xc0

// Bitfield Description : 0x1 - Hardware supports PPR_OVERFLOW_EARLY notification on threshold support.
#define IOMMU_MMIO_CONTROL1_W_PPR_OVERFLOW_EARLY_SUP_W_OFFSET      8
#define IOMMU_MMIO_CONTROL1_W_PPR_OVERFLOW_EARLY_SUP_W_MASK        0x100

// Bitfield Description : 0x1 - Hardware supports ppr auto response generation on log about to be full or generate autoresp on reaching threshold if enabled with IOMMUMMIO::IOMMU_MMIO_PPR_OVERFLOW_EARLY_0[PPR_Overflow_early_en] or IOMMUMMIO::IOMMU_MMIO_PPR_B_OVERFLOW_EARLY_0[PPR_B_Overflow_early_en].
#define IOMMU_MMIO_CONTROL1_W_PPR_AUTORESP_SUP_W_OFFSET      9
#define IOMMU_MMIO_CONTROL1_W_PPR_AUTORESP_SUP_W_MASK        0x200

// Bitfield Description : 0=No blocking Stop Mark Messages supported.
// 1=Block Stop Mark Messages supported.
#define IOMMU_MMIO_CONTROL1_W_BLOCK_STOPMARK_SUP_W_OFFSET      10
#define IOMMU_MMIO_CONTROL1_W_BLOCK_STOPMARK_SUP_W_MASK        0x400

// Bitfield Description : Writes number of MARC apertures and thus the number of MARC register 3-tuples supported. 00b = MARC feature not supported, 01b = 4 MARC registers 3-tuples supported, 10b = 8 MARC register 4-tuples are supported, 11b = Reserved.
#define IOMMU_MMIO_CONTROL1_W_MARCnum_SUP_W_OFFSET      11
#define IOMMU_MMIO_CONTROL1_W_MARCnum_SUP_W_MASK        0x1800

// Bitfield Description : 1 = Hardware supports returning memory attributes of the guest page to ATS requester along with ATS completion.
#define IOMMU_MMIO_CONTROL1_W_SNOOP_ATTRS_SUP_W_OFFSET      13
#define IOMMU_MMIO_CONTROL1_W_SNOOP_ATTRS_SUP_W_MASK        0x2000

// Bitfield Description : 1 = Hardware supports nested I/O protection.
#define IOMMU_MMIO_CONTROL1_W_GIo_SUP_W_OFFSET      14
#define IOMMU_MMIO_CONTROL1_W_GIo_SUP_W_MASK        0x4000

// Bitfield Description : 1 = Hardware supports Access bit update support for v1 Page tables.
#define IOMMU_MMIO_CONTROL1_W_HA_SUP_W_OFFSET      15
#define IOMMU_MMIO_CONTROL1_W_HA_SUP_W_MASK        0x8000

// Bitfield Description : 1 = Hardware supports Enhanced PPR Handling.
#define IOMMU_MMIO_CONTROL1_W_EPH_SUP_W_OFFSET      16
#define IOMMU_MMIO_CONTROL1_W_EPH_SUP_W_MASK        0x10000

// Bitfield Description : 1 = Hardware supports forward page table memory attribute to ATC client.
#define IOMMU_MMIO_CONTROL1_W_ATTRFW_SUP_W_OFFSET      17
#define IOMMU_MMIO_CONTROL1_W_ATTRFW_SUP_W_MASK        0x20000

// Bitfield Description : 1 = Hardware supports Disabling v2 Dirty bit updates by hardware.
#define IOMMU_MMIO_CONTROL1_W_V2_HD_DIS_SUP_W_OFFSET      18
#define IOMMU_MMIO_CONTROL1_W_V2_HD_DIS_SUP_W_MASK        0x40000

// Bitfield Description : 1 = Hardware supports invalidate IOTLB type.
#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_OFFSET      19
#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_MASK        0x80000

// Bitfield Description : 1 = Hardware supports Dirty bit update support for v1 Page tables.
#define IOMMU_MMIO_CONTROL1_W_HD_SUP_W_OFFSET      20
#define IOMMU_MMIO_CONTROL1_W_HD_SUP_W_MASK        0x100000

// Bitfield Description : 1 = HW supports virtualized IOMMU.
#define IOMMU_MMIO_CONTROL1_W_VIOMMU_SUP_W_OFFSET      21
#define IOMMU_MMIO_CONTROL1_W_VIOMMU_SUP_W_MASK        0x200000

// Bitfield Description : 1 =  VMGuard IO supported by the HW.
#define IOMMU_MMIO_CONTROL1_W_VMGUARDIO_SUP_W_OFFSET      22
#define IOMMU_MMIO_CONTROL1_W_VMGUARDIO_SUP_W_MASK        0x400000

// Bitfield Description : indicate the size of the VMTable supported.
// 00h 	256 entries	04h 	4K entries	08h 	64K entries
// 01h 	512 entries	05h 	8K entries	09h 	128K entries
// 02h 	1K entries	06h 	16K entries	0Ah 	256K entries
// 03h 	2K entries	07h 	32K entries	0Bh 	512K entries
// 0Ch-0Fh: Reserved
#define IOMMU_MMIO_CONTROL1_W_VMTABLESIZE_W_OFFSET      23
#define IOMMU_MMIO_CONTROL1_W_VMTABLESIZE_W_MASK        0x7800000

// Bitfield Description : 1 = Hardware supports Disabling v2 Access bit updates by HW.
#define IOMMU_MMIO_CONTROL1_W_V2_HA_DIS_SUP_W_OFFSET      27
#define IOMMU_MMIO_CONTROL1_W_V2_HA_DIS_SUP_W_MASK        0x8000000

// Bitfield Description : 1 = HW support Guest APIC Physical Processor Interrupt when Interrupt Virtualization is enabled
#define IOMMU_MMIO_CONTROL1_W_GAPPI_SUP_W_OFFSET      28
#define IOMMU_MMIO_CONTROL1_W_GAPPI_SUP_W_MASK        0x10000000

// Bitfield Description : 
#define IOMMU_MMIO_CONTROL1_W_Reserved_29_29_OFFSET      29
#define IOMMU_MMIO_CONTROL1_W_Reserved_29_29_MASK        0x20000000

// Bitfield Description : 1 = HW supports forcing physical destination mode for remapped interrupts.
#define IOMMU_MMIO_CONTROL1_W_ForcePhyDestSup_W_OFFSET      30
#define IOMMU_MMIO_CONTROL1_W_ForcePhyDestSup_W_MASK        0x40000000

// Bitfield Description : 1 = Hardware supports SNP.
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_OFFSET      31
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_MASK        0x80000000

typedef union {
  struct {
    UINT32                            PAS_MAX_W:4;
    UINT32                            Reserved_5_4:2;
    UINT32                            DTE_seg_W:2;
    UINT32                            PPR_OVERFLOW_EARLY_SUP_W:1;
    UINT32                            PPR_AUTORESP_SUP_W:1;
    UINT32                            BLOCK_STOPMARK_SUP_W:1;
    UINT32                            MARCnum_SUP_W:2;
    UINT32                            SNOOP_ATTRS_SUP_W:1;
    UINT32                            GIo_SUP_W:1;
    UINT32                            HA_SUP_W:1;
    UINT32                            EPH_SUP_W:1;
    UINT32                            ATTRFW_SUP_W:1;
    UINT32                            V2_HD_DIS_SUP_W:1;
    UINT32                            InvIotlbTypeSup_W:1;
    UINT32                            HD_SUP_W:1;
    UINT32                            VIOMMU_SUP_W:1;
    UINT32                            VMGUARDIO_SUP_W:1;
    UINT32                            VMTABLESIZE_W:4;
    UINT32                            V2_HA_DIS_SUP_W:1;
    UINT32                            GAPPI_SUP_W:1;
    UINT32                            Reserved_29_29:1;
    UINT32                            ForcePhyDestSup_W:1;
    UINT32                            SNP_SUP_W:1;
  } Field;
  UINT32 Value;
} IOMMU_MMIO_CONTROL1_W_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL1_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL1_W_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL1_W_OFFSET       0x84
#define PCICFG_NBIO_IOHUB_IOMMU_MMIO_CONTROL1_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x84)

#define SMN_IOMMU_MMIO_CONTROL1_W_ADDRESS    0x13f00084UL


/***********************************************************
* Register Name : IOMMU_MSI_ADDR_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register sets address bits 63:32 used to issue MSI messages.
#define IOMMU_MSI_ADDR_HI_MSI_ADDR_HI_OFFSET      0
#define IOMMU_MSI_ADDR_HI_MSI_ADDR_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_ADDR_HI:32;
  } Field;
  UINT32 Value;
} IOMMU_MSI_ADDR_HI_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_HI_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_HI_OFFSET       0x6c
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6c)

#define SMN_IOMMU_MSI_ADDR_HI_ADDRESS    0x13f0006cUL


/***********************************************************
* Register Name : IOMMU_MSI_ADDR_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOMMU_MSI_ADDR_LO_Reserved_1_0_OFFSET      0
#define IOMMU_MSI_ADDR_LO_Reserved_1_0_MASK        0x3

// Bitfield Description : This register sets address bits 31:2 used to issue MSI messages.
#define IOMMU_MSI_ADDR_LO_MSI_ADDR_LO_OFFSET      2
#define IOMMU_MSI_ADDR_LO_MSI_ADDR_LO_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            MSI_ADDR_LO:30;
  } Field;
  UINT32 Value;
} IOMMU_MSI_ADDR_LO_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_LO_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_LO_OFFSET       0x68
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x68)

#define SMN_IOMMU_MSI_ADDR_LO_ADDRESS    0x13f00068UL


/***********************************************************
* Register Name : IOMMU_MSI_CAP
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates that this is the MSI capability.
#define IOMMU_MSI_CAP_MSI_CAP_ID_OFFSET      0
#define IOMMU_MSI_CAP_MSI_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next configuration space capability.
#define IOMMU_MSI_CAP_MSI_CAP_PTR_OFFSET      8
#define IOMMU_MSI_CAP_MSI_CAP_PTR_MASK        0xff00

// Bitfield Description : Enables MSI for this function and causes legacy interrupts to be disabled.
#define IOMMU_MSI_CAP_MSI_EN_OFFSET      16
#define IOMMU_MSI_CAP_MSI_EN_MASK        0x10000

// Bitfield Description : Indicates the number of MSI messages requested by this function.
#define IOMMU_MSI_CAP_MSI_MULT_MESS_CAP_OFFSET      17
#define IOMMU_MSI_CAP_MSI_MULT_MESS_CAP_MASK        0xe0000

// Bitfield Description : Sets the number of MSI messages assigned to this function.
#define IOMMU_MSI_CAP_MSI_MULT_MESS_EN_OFFSET      20
#define IOMMU_MSI_CAP_MSI_MULT_MESS_EN_MASK        0x700000

// Bitfield Description : Set to indicate that a 64-bit MSI address is supported.
#define IOMMU_MSI_CAP_MSI_64_EN_OFFSET      23
#define IOMMU_MSI_CAP_MSI_64_EN_MASK        0x800000

// Bitfield Description : 
#define IOMMU_MSI_CAP_Reserved_31_24_OFFSET      24
#define IOMMU_MSI_CAP_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            MSI_CAP_ID:8;
    UINT32                            MSI_CAP_PTR:8;
    UINT32                            MSI_EN:1;
    UINT32                            MSI_MULT_MESS_CAP:3;
    UINT32                            MSI_MULT_MESS_EN:3;
    UINT32                            MSI_64_EN:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} IOMMU_MSI_CAP_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MSI_CAP_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_CAP_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_CAP_OFFSET       0x64
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x64)

#define SMN_IOMMU_MSI_CAP_ADDRESS    0x13f00064UL


/***********************************************************
* Register Name : IOMMU_MSI_DATA
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register sets the data issued with MSI messages.
#define IOMMU_MSI_DATA_MSI_DATA_OFFSET      0
#define IOMMU_MSI_DATA_MSI_DATA_MASK        0xffff

// Bitfield Description : 
#define IOMMU_MSI_DATA_Reserved_31_16_OFFSET      16
#define IOMMU_MSI_DATA_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            MSI_DATA:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} IOMMU_MSI_DATA_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MSI_DATA_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_DATA_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_DATA_OFFSET       0x70
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x70)

#define SMN_IOMMU_MSI_DATA_ADDRESS    0x13f00070UL


/***********************************************************
* Register Name : IOMMU_MSI_MAPPING_CAP
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates a Hypertransport capability list item.
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_ID_OFFSET      0
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_ID_MASK        0xff

// Bitfield Description : Points to the next capability list item.
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_PTR_OFFSET      8
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_PTR_MASK        0xff00

// Bitfield Description : Always set to 1 to indicate that the MSI Mapping Capability is always enabled.
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_EN_OFFSET      16
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_EN_MASK        0x10000

// Bitfield Description : Always set to 1 to indicate that this device only maps MSI interrupts with address 0xFEEx_xxxx onto Hypertransport interrupts and that the mapping range is not programmable.
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_FIXD_OFFSET      17
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_FIXD_MASK        0x20000

// Bitfield Description : Reserved for future use. This register controls no hardware
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_RSV_OFFSET      18
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_RSV_MASK        0x7fc0000

// Bitfield Description : Indicates the MSI Mapping Capability.
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_TYPE_OFFSET      27
#define IOMMU_MSI_MAPPING_CAP_MSI_MAP_CAP_TYPE_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            MSI_MAP_CAP_ID:8;
    UINT32                            MSI_MAP_CAP_PTR:8;
    UINT32                            MSI_MAP_EN:1;
    UINT32                            MSI_MAP_FIXD:1;
    UINT32                            MSI_MAP_RSV:9;
    UINT32                            MSI_MAP_CAP_TYPE:5;
  } Field;
  UINT32 Value;
} IOMMU_MSI_MAPPING_CAP_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_MSI_MAPPING_CAP_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_MAPPING_CAP_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_MAPPING_CAP_OFFSET       0x74
#define PCICFG_NBIO_IOHUB_IOMMU_MSI_MAPPING_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x74)

#define SMN_IOMMU_MSI_MAPPING_CAP_ADDRESS    0x13f00074UL


/***********************************************************
* Register Name : IOMMU_RANGE_W
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOMMU_RANGE_W_Reserved_6_0_OFFSET      0
#define IOMMU_RANGE_W_Reserved_6_0_MASK        0x7f

// Bitfield Description : Sets the value of IOMMU_RNG_VALID in IOMMU capability.
#define IOMMU_RANGE_W_RNG_VALID_W_OFFSET      7
#define IOMMU_RANGE_W_RNG_VALID_W_MASK        0x80

// Bitfield Description : Sets the value of IOMMU_BUS_NUMBER in IOMMU capability.
#define IOMMU_RANGE_W_BUS_NUMBER_W_OFFSET      8
#define IOMMU_RANGE_W_BUS_NUMBER_W_MASK        0xff00

// Bitfield Description : Sets the value of IOMMU_FIRST_DEVICE in IOMMU capability.
#define IOMMU_RANGE_W_FIRST_DEVICE_W_OFFSET      16
#define IOMMU_RANGE_W_FIRST_DEVICE_W_MASK        0xff0000

// Bitfield Description : Sets the value of IOMMU_LAST_DEVICE in IOMMU capability.
#define IOMMU_RANGE_W_LAST_DEVICE_W_OFFSET      24
#define IOMMU_RANGE_W_LAST_DEVICE_W_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            RNG_VALID_W:1;
    UINT32                            BUS_NUMBER_W:8;
    UINT32                            FIRST_DEVICE_W:8;
    UINT32                            LAST_DEVICE_W:8;
  } Field;
  UINT32 Value;
} IOMMU_RANGE_W_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_RANGE_W_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_RANGE_W_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_RANGE_W_OFFSET       0x88
#define PCICFG_NBIO_IOHUB_IOMMU_RANGE_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x88)

#define SMN_IOMMU_RANGE_W_ADDRESS    0x13f00088UL


/***********************************************************
* Register Name : IOMMU_REGPROG_INF
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Register Programming Interface.
// IOMMU
#define IOMMU_REGPROG_INF_REG_LEVEL_PROG_INF_OFFSET      0
#define IOMMU_REGPROG_INF_REG_LEVEL_PROG_INF_MASK        0xff

typedef union {
  struct {
    UINT8                            REG_LEVEL_PROG_INF:8;
  } Field;
  UINT8 Value;
} IOMMU_REGPROG_INF_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_REGPROG_INF_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_REGPROG_INF_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_REGPROG_INF_OFFSET       0x9
#define PCICFG_NBIO_IOHUB_IOMMU_REGPROG_INF_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x9)

#define SMN_IOMMU_REGPROG_INF_ADDRESS    0x13f00009UL


/***********************************************************
* Register Name : IOMMU_REVISION_ID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the stepping number of the function.
#define IOMMU_REVISION_ID_MINOR_REV_ID_OFFSET      0
#define IOMMU_REVISION_ID_MINOR_REV_ID_MASK        0xf

// Bitfield Description : Identifies the revision number of the function.
#define IOMMU_REVISION_ID_MAJOR_REV_ID_OFFSET      4
#define IOMMU_REVISION_ID_MAJOR_REV_ID_MASK        0xf0

typedef union {
  struct {
    UINT8                            MINOR_REV_ID:4;
    UINT8                            MAJOR_REV_ID:4;
  } Field;
  UINT8 Value;
} IOMMU_REVISION_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_REVISION_ID_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_IOHUB_IOMMU_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8)

#define SMN_IOMMU_REVISION_ID_ADDRESS    0x13f00008UL


/***********************************************************
* Register Name : IOMMU_STATUS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define IOMMU_STATUS_Reserved_2_0_OFFSET      0
#define IOMMU_STATUS_Reserved_2_0_MASK        0x7

// Bitfield Description : Interrupt Status
// 1=This function has an outstanding interrupt
// 0=This function does not have any outstanding interrupts.
#define IOMMU_STATUS_INT_Status_OFFSET      3
#define IOMMU_STATUS_INT_Status_MASK        0x8

// Bitfield Description : Capabilities List
// This bit is set to indicate that this function`s configuration space supports a capabilities list.
#define IOMMU_STATUS_CAP_LIST_OFFSET      4
#define IOMMU_STATUS_CAP_LIST_MASK        0x10

// Bitfield Description : 
#define IOMMU_STATUS_Reserved_7_5_OFFSET      5
#define IOMMU_STATUS_Reserved_7_5_MASK        0xe0

// Bitfield Description : Master Data Error
// This bit is set by hardware whenever PARITY_ERROR_EN is set and this function receives a read response with the data error attribute set.
#define IOMMU_STATUS_MASTER_DATA_ERROR_OFFSET      8
#define IOMMU_STATUS_MASTER_DATA_ERROR_MASK        0x100

// Bitfield Description : 
#define IOMMU_STATUS_Reserved_10_9_OFFSET      9
#define IOMMU_STATUS_Reserved_10_9_MASK        0x600

// Bitfield Description : Signal Target Abort
// This bit is always set to 0 because this function does not terminate host requests with target abort.
#define IOMMU_STATUS_SIGNAL_TARGET_ABORT_OFFSET      11
#define IOMMU_STATUS_SIGNAL_TARGET_ABORT_MASK        0x800

// Bitfield Description : Received Target Abort.
// This bit is set by hardware whenever this function receives a read response with the target abort status.
#define IOMMU_STATUS_RECEIVED_TARGET_ABORT_OFFSET      12
#define IOMMU_STATUS_RECEIVED_TARGET_ABORT_MASK        0x1000

// Bitfield Description : Received Target Abort.
// This bit is set by hardware whenever this function receives a read response with the master abort status.
#define IOMMU_STATUS_RECEIVED_MASTER_ABORT_OFFSET      13
#define IOMMU_STATUS_RECEIVED_MASTER_ABORT_MASK        0x2000

// Bitfield Description : Signaled System Error
// This bit is always set to 0 because this function does not generate signaled system error status.
#define IOMMU_STATUS_SIGNALED_SYSTEM_ERROR_OFFSET      14
#define IOMMU_STATUS_SIGNALED_SYSTEM_ERROR_MASK        0x4000

// Bitfield Description : Parity Error Detected.
// This bit is set by hardware whenever this function receives a read response with the data error attribute set.
#define IOMMU_STATUS_PARITY_ERROR_DETECTED_OFFSET      15
#define IOMMU_STATUS_PARITY_ERROR_DETECTED_MASK        0x8000

typedef union {
  struct {
    UINT16                            Reserved_2_0:3;
    UINT16                            INT_Status:1;
    UINT16                            CAP_LIST:1;
    UINT16                            Reserved_7_5:3;
    UINT16                            MASTER_DATA_ERROR:1;
    UINT16                            Reserved_10_9:2;
    UINT16                            SIGNAL_TARGET_ABORT:1;
    UINT16                            RECEIVED_TARGET_ABORT:1;
    UINT16                            RECEIVED_MASTER_ABORT:1;
    UINT16                            SIGNALED_SYSTEM_ERROR:1;
    UINT16                            PARITY_ERROR_DETECTED:1;
  } Field;
  UINT16 Value;
} IOMMU_STATUS_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_STATUS_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_STATUS_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_STATUS_OFFSET       0x6
#define PCICFG_NBIO_IOHUB_IOMMU_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6)

#define SMN_IOMMU_STATUS_ADDRESS    0x13f00006UL


/***********************************************************
* Register Name : IOMMU_SUB_CLASS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sub-Class Code.
// IOMMU
#define IOMMU_SUB_CLASS_SUB_CLASS_INF_OFFSET      0
#define IOMMU_SUB_CLASS_SUB_CLASS_INF_MASK        0xff

typedef union {
  struct {
    UINT8                            SUB_CLASS_INF:8;
  } Field;
  UINT8 Value;
} IOMMU_SUB_CLASS_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_SUB_CLASS_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_IOHUB_IOMMU_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa)

#define SMN_IOMMU_SUB_CLASS_ADDRESS    0x13f0000aUL


/***********************************************************
* Register Name : IOMMU_VENDOR_ID
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor Identifier. This 16-bit field identifies the manufacturer of the device: Advanced Micro Devices Inc.
#define IOMMU_VENDOR_ID_VENDOR_ID_OFFSET      0
#define IOMMU_VENDOR_ID_VENDOR_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            VENDOR_ID:16;
  } Field;
  UINT16 Value;
} IOMMU_VENDOR_ID_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_VENDOR_ID_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_IOHUB_IOMMU_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_IOMMU_VENDOR_ID_ADDRESS    0x13f00000UL


/***********************************************************
* Register Name : IOMMU_VFCNTL_BASE_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies address bits 63:32 of the 4M-byte-aligned base address of the VFCNTL memory-mapped control registers.
#define IOMMU_VFCNTL_BASE_HI_VFCNTL_BASE_ADDR_HI_OFFSET      0
#define IOMMU_VFCNTL_BASE_HI_VFCNTL_BASE_ADDR_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            VFCNTL_BASE_ADDR_HI:32;
  } Field;
  UINT32 Value;
} IOMMU_VFCNTL_BASE_HI_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_HI_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_HI_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_HI_OFFSET       0xdc
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xdc)

#define SMN_IOMMU_VFCNTL_BASE_HI_ADDRESS    0x13f000dcUL


/***********************************************************
* Register Name : IOMMU_VFCNTL_BASE_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=IOMMU accepts memory accesses to the address specified in the VFCNTL Base Address Register.
#define IOMMU_VFCNTL_BASE_LO_VFCNTL_ENABLE_OFFSET      0
#define IOMMU_VFCNTL_BASE_LO_VFCNTL_ENABLE_MASK        0x1

// Bitfield Description : 
#define IOMMU_VFCNTL_BASE_LO_Reserved_21_1_OFFSET      1
#define IOMMU_VFCNTL_BASE_LO_Reserved_21_1_MASK        0x3ffffe

// Bitfield Description : Specifies address bits 31:22 of the 4M-byte-aligned base address of the VFCNTL memory-mapped control registers.
#define IOMMU_VFCNTL_BASE_LO_VFCNTL_BASE_ADDR_LO_OFFSET      22
#define IOMMU_VFCNTL_BASE_LO_VFCNTL_BASE_ADDR_LO_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            VFCNTL_ENABLE:1;
    UINT32                            Reserved_21_1:21;
    UINT32                            VFCNTL_BASE_ADDR_LO:10;
  } Field;
  UINT32 Value;
} IOMMU_VFCNTL_BASE_LO_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_LO_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_LO_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_LO_OFFSET       0xd8
#define PCICFG_NBIO_IOHUB_IOMMU_VFCNTL_BASE_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd8)

#define SMN_IOMMU_VFCNTL_BASE_LO_ADDRESS    0x13f000d8UL


/***********************************************************
* Register Name : IOMMU_VF_BASE_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Specifies address bits 63:32 of the 256M-byte-aligned base address of the VF memory-mapped control registers.
#define IOMMU_VF_BASE_HI_VF_BASE_ADDR_HI_OFFSET      0
#define IOMMU_VF_BASE_HI_VF_BASE_ADDR_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            VF_BASE_ADDR_HI:32;
  } Field;
  UINT32 Value;
} IOMMU_VF_BASE_HI_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_HI_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_HI_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_HI_OFFSET       0xd4
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd4)

#define SMN_IOMMU_VF_BASE_HI_ADDRESS    0x13f000d4UL


/***********************************************************
* Register Name : IOMMU_VF_BASE_LO
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=IOMMU accepts memory accesses to the address specified in the VF Base Address Register.
#define IOMMU_VF_BASE_LO_VF_ENABLE_OFFSET      0
#define IOMMU_VF_BASE_LO_VF_ENABLE_MASK        0x1

// Bitfield Description : 
#define IOMMU_VF_BASE_LO_Reserved_27_1_OFFSET      1
#define IOMMU_VF_BASE_LO_Reserved_27_1_MASK        0xffffffe

// Bitfield Description : Specifies address bits 31:28 of the 256M-byte-aligned base address of the VF memory-mapped control registers.
#define IOMMU_VF_BASE_LO_VF_BASE_ADDR_LO_OFFSET      28
#define IOMMU_VF_BASE_LO_VF_BASE_ADDR_LO_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            VF_ENABLE:1;
    UINT32                            Reserved_27_1:27;
    UINT32                            VF_BASE_ADDR_LO:4;
  } Field;
  UINT32 Value;
} IOMMU_VF_BASE_LO_STRUCT;

#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_LO_DEVICE       0
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_LO_FUNC         2
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_LO_OFFSET       0xd0
#define PCICFG_NBIO_IOHUB_IOMMU_VF_BASE_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd0)

#define SMN_IOMMU_VF_BASE_LO_ADDRESS    0x13f000d0UL


/***********************************************************
* Register Name : L2A_UPDATE_FILTER_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=Disable duplicate update filtering;
// 0=Enable the dropping of updates that are already in the l2a_update_filter or in the destination l2a cache.
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_OFFSET      0
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_MASK        0x1

// Bitfield Description : When L2a_Update_Filter_Bypass is 0, assume the invalidation read has completed in the number of clock cycles specified by this field.
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_RdLatency_OFFSET      1
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_RdLatency_MASK        0x1e

// Bitfield Description : 
#define L2A_UPDATE_FILTER_CNTL_Reserved_31_5_OFFSET      5
#define L2A_UPDATE_FILTER_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            L2a_Update_Filter_Bypass:1;
    UINT32                            L2a_Update_Filter_RdLatency:4;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} L2A_UPDATE_FILTER_CNTL_STRUCT;

#define SMN_L2A_UPDATE_FILTER_CNTL_ADDRESS    0x15700088UL


/***********************************************************
* Register Name : L2A_Z10_PERF_COUNT_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Live values of perf counter 0 bits 31:0.
#define L2A_Z10_PERF_COUNT_0_L2A_Z10_PERF_COUNT_0_OFFSET      0
#define L2A_Z10_PERF_COUNT_0_L2A_Z10_PERF_COUNT_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2A_Z10_PERF_COUNT_0:32;
  } Field;
  UINT32 Value;
} L2A_Z10_PERF_COUNT_0_STRUCT;

#define SMN_L2A_Z10_PERF_COUNT_0_ADDRESS    0x15700118UL


/***********************************************************
* Register Name : L2A_Z10_PERF_COUNT_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Live values of perf counter 1 bits 31:0.
#define L2A_Z10_PERF_COUNT_1_L2A_Z10_PERF_COUNT_1_OFFSET      0
#define L2A_Z10_PERF_COUNT_1_L2A_Z10_PERF_COUNT_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2A_Z10_PERF_COUNT_1:32;
  } Field;
  UINT32 Value;
} L2A_Z10_PERF_COUNT_1_STRUCT;

#define SMN_L2A_Z10_PERF_COUNT_1_ADDRESS    0x1570011cUL


/***********************************************************
* Register Name : L2A_Z10_PERF_COUNT_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Live values of perf counter 2 bits 31:0.
#define L2A_Z10_PERF_COUNT_2_L2A_Z10_PERF_COUNT_2_OFFSET      0
#define L2A_Z10_PERF_COUNT_2_L2A_Z10_PERF_COUNT_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2A_Z10_PERF_COUNT_2:32;
  } Field;
  UINT32 Value;
} L2A_Z10_PERF_COUNT_2_STRUCT;

#define SMN_L2A_Z10_PERF_COUNT_2_ADDRESS    0x15700120UL


/***********************************************************
* Register Name : L2A_Z10_PERF_COUNT_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Live values of perf counter 3 bits 31:0.
#define L2A_Z10_PERF_COUNT_3_L2A_Z10_PERF_COUNT_3_OFFSET      0
#define L2A_Z10_PERF_COUNT_3_L2A_Z10_PERF_COUNT_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2A_Z10_PERF_COUNT_3:32;
  } Field;
  UINT32 Value;
} L2A_Z10_PERF_COUNT_3_STRUCT;

#define SMN_L2A_Z10_PERF_COUNT_3_ADDRESS    0x15700124UL


/***********************************************************
* Register Name : L2A_Z10_PERF_COUNT_HI
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Live values of perf counter 0 bits 40:32.
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_0_OFFSET      0
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_0_MASK        0xff

// Bitfield Description : Live values of perf counter 1 bits 40:32.
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_1_OFFSET      8
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_1_MASK        0xff00

// Bitfield Description : Live values of perf counter 2 bits 40:32.
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_2_OFFSET      16
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_2_MASK        0xff0000

// Bitfield Description : Live values of perf counter 3 bits 40:32.
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_3_OFFSET      24
#define L2A_Z10_PERF_COUNT_HI_L2A_Z10_PERF_COUNT_HI_3_MASK        0xff000000

typedef union {
  struct {
    UINT32                            L2A_Z10_PERF_COUNT_HI_0:8;
    UINT32                            L2A_Z10_PERF_COUNT_HI_1:8;
    UINT32                            L2A_Z10_PERF_COUNT_HI_2:8;
    UINT32                            L2A_Z10_PERF_COUNT_HI_3:8;
  } Field;
  UINT32 Value;
} L2A_Z10_PERF_COUNT_HI_STRUCT;

#define SMN_L2A_Z10_PERF_COUNT_HI_ADDRESS    0x15700128UL


/***********************************************************
* Register Name : L2B_SDP_MAXCRED
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Max credits to be released for L2IMU_IOHC_RdRsp interface. L2B_RDRSP[3:0] represents max credits on VC0. L2B_RDRSP[7:4] represents max credits on VC1. L2B_RDRSP[11:8] represents max credits on VC4. L2B_RDRSP[15:12] represents max credits on VC5/7
#define L2B_SDP_MAXCRED_L2B_RDRSP_MAXCRED_OFFSET      0
#define L2B_SDP_MAXCRED_L2B_RDRSP_MAXCRED_MASK        0xffff

// Bitfield Description : Max credits to be released for L2IMU_IOHC_WrRsp interface. L2B_WRRSP[3:0] represents max credits on VC0. L2B_WRRSP[7:4] represents max credits on VC1
#define L2B_SDP_MAXCRED_L2B_WRRSP_MAXCRED_OFFSET      16
#define L2B_SDP_MAXCRED_L2B_WRRSP_MAXCRED_MASK        0xff0000

// Bitfield Description : Max credits to be released for iohc_l2_req interface
#define L2B_SDP_MAXCRED_L2B_REQ_MAXCRED_OFFSET      24
#define L2B_SDP_MAXCRED_L2B_REQ_MAXCRED_MASK        0xf000000

// Bitfield Description : Max credits to be released for iohc_l2_data interface
#define L2B_SDP_MAXCRED_L2B_DATA_MAXCRED_OFFSET      28
#define L2B_SDP_MAXCRED_L2B_DATA_MAXCRED_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            L2B_RDRSP_MAXCRED:16;
    UINT32                            L2B_WRRSP_MAXCRED:8;
    UINT32                            L2B_REQ_MAXCRED:4;
    UINT32                            L2B_DATA_MAXCRED:4;
  } Field;
  UINT32 Value;
} L2B_SDP_MAXCRED_STRUCT;

#define SMN_L2B_SDP_MAXCRED_ADDRESS    0x13f01284UL


/***********************************************************
* Register Name : L2B_SDP_PARITY_ERROR_EN
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enables the parity check on incoming data to the DVM.
#define L2B_SDP_PARITY_ERROR_EN_DVM_PARITY_ERROR_EN_OFFSET      0
#define L2B_SDP_PARITY_ERROR_EN_DVM_PARITY_ERROR_EN_MASK        0x1

// Bitfield Description : Enables the parity check on incoming data to the CP.
#define L2B_SDP_PARITY_ERROR_EN_CP_PARITY_ERROR_EN_OFFSET      1
#define L2B_SDP_PARITY_ERROR_EN_CP_PARITY_ERROR_EN_MASK        0x2

// Bitfield Description : Enables the parity check on incoming data to the TW.
#define L2B_SDP_PARITY_ERROR_EN_TWW_PARITY_ERROR_EN_OFFSET      2
#define L2B_SDP_PARITY_ERROR_EN_TWW_PARITY_ERROR_EN_MASK        0x4

// Bitfield Description : Enables the parity check on incoming data to the VFMMIO Processor.
#define L2B_SDP_PARITY_ERROR_EN_VFMMIO_PARITY_ERROR_EN_OFFSET      3
#define L2B_SDP_PARITY_ERROR_EN_VFMMIO_PARITY_ERROR_EN_MASK        0x8

// Bitfield Description : 
#define L2B_SDP_PARITY_ERROR_EN_Reserved_31_4_OFFSET      4
#define L2B_SDP_PARITY_ERROR_EN_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            DVM_PARITY_ERROR_EN:1;
    UINT32                            CP_PARITY_ERROR_EN:1;
    UINT32                            TWW_PARITY_ERROR_EN:1;
    UINT32                            VFMMIO_PARITY_ERROR_EN:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} L2B_SDP_PARITY_ERROR_EN_STRUCT;

#define SMN_L2B_SDP_PARITY_ERROR_EN_ADDRESS    0x13f01288UL


/***********************************************************
* Register Name : L2B_UPDATE_FILTER_CNTL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1 = Disable duplicate update filtering;
// 0 = Enable the dropping of updates that are already in the l2b_update_filter or in the PDC.
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_OFFSET      0
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_MASK        0x1

// Bitfield Description : When L2b_Update_Filter_Bypass is 0, assume the invalidation read has completed in the number of clock cycles specified by this field.
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_RdLatency_OFFSET      1
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_RdLatency_MASK        0x1e

// Bitfield Description : 
#define L2B_UPDATE_FILTER_CNTL_Reserved_31_5_OFFSET      5
#define L2B_UPDATE_FILTER_CNTL_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            L2b_Update_Filter_Bypass:1;
    UINT32                            L2b_Update_Filter_RdLatency:4;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} L2B_UPDATE_FILTER_CNTL_STRUCT;

#define SMN_L2B_UPDATE_FILTER_CNTL_ADDRESS    0x13f0114cUL


/***********************************************************
* Register Name : L2_CONTROL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_CONTROL_0_Reserved_0_0_OFFSET      0
#define L2_CONTROL_0_Reserved_0_0_MASK        0x1

// Bitfield Description : 1=L2 does not allow L1 to cache DTEs where V=0.
// 0=L2 allows L1 to cache DTEs where V=0 L1 stores IR and IW as if they are both set to 1.
#define L2_CONTROL_0_AllowL1CacheVZero_OFFSET      1
#define L2_CONTROL_0_AllowL1CacheVZero_MASK        0x2

// Bitfield Description : 0=L2 does not allow L1 to cache responses to ATS address translation requests.
// 1=L2 allows L1 to cache responses to ATS address translation requests.
#define L2_CONTROL_0_AllowL1CacheATSRsp_OFFSET      2
#define L2_CONTROL_0_AllowL1CacheATSRsp_MASK        0x4

// Bitfield Description : 0=A DTE is refetched if a DTE with V=0 for a memory request or IV=1 for an interrupt request is hit in the DTC.
// 1=A DTE is not refetched if a DTE with V=0 for a memory request or IV=1 for an interrupt request is hit in the DTC. This DTE is used.
#define L2_CONTROL_0_DTCHitVZeroOrIVZero_OFFSET      3
#define L2_CONTROL_0_DTCHitVZeroOrIVZero_MASK        0x8

// Bitfield Description : 
#define L2_CONTROL_0_Reserved_9_4_OFFSET      4
#define L2_CONTROL_0_Reserved_9_4_MASK        0x3f0

// Bitfield Description : 0=Caches return DTE to L1 on an untranslated exclusion range access
// 1=Caches return PTE to L1 on an untranslated exclusion range access.
#define L2_CONTROL_0_SIDEPTEOnUntransExcl_OFFSET      10
#define L2_CONTROL_0_SIDEPTEOnUntransExcl_MASK        0x400

// Bitfield Description : 0=Caches return DTE to L1 on an address translation exclusion range access
// 1=Caches return PTE to L1 on an address translation exclusion range access.
#define L2_CONTROL_0_SIDEPTEOnAddrTransExcl_OFFSET      11
#define L2_CONTROL_0_SIDEPTEOnAddrTransExcl_MASK        0x800

// Bitfield Description : 1=Responses to L1 with U bit set will not be gated by AT field. 0=Responses to L1 with U bit set will be gated unless AT=1. Note: This register must be programmed to the same value as its copy in L2_MISC_CNTRL_3 located in iommu_l2indx.
#define L2_CONTROL_0_Allow_nonats_u_bit_OFFSET      12
#define L2_CONTROL_0_Allow_nonats_u_bit_MASK        0x1000

// Bitfield Description : 
#define L2_CONTROL_0_Reserved_17_13_OFFSET      13
#define L2_CONTROL_0_Reserved_17_13_MASK        0x3e000

// Bitfield Description : 0=Round-robin arbitration between cache responses and table-walker responses at the fault combiner.
// 1=Table-walker responses always win arbitration at the fault combiner.
#define L2_CONTROL_0_FLTCMBPriority_OFFSET      18
#define L2_CONTROL_0_FLTCMBPriority_MASK        0x40000

// Bitfield Description : 1=Requests with DTE.MODE=0 will be stored in the L1 TLB as large pages. 0=Requests with DTE.MODE=0 will be stored in the L1 TLB as 4k pages. Note: This register must be programmed to the same value as its copy in L1_MISC_CNTRL_2 located in iommu_l1 and in L2_MISC_CNTRL_3 in iommu_l2indx.
#define L2_CONTROL_0_AllowL1CacheLargePagemode0_OFFSET      19
#define L2_CONTROL_0_AllowL1CacheLargePagemode0_MASK        0x80000

// Bitfield Description : Sets the burst length when arbitrating between clients coming into the L2.
#define L2_CONTROL_0_IFifoBurstLength_OFFSET      20
#define L2_CONTROL_0_IFifoBurstLength_MASK        0xf00000

// Bitfield Description : Each bit of this register controls whether the corresponding L1 client is arbitrated as high priority or not. Not all implementations use all of the priority bits due to a lower number of clients versus the register width.
#define L2_CONTROL_0_IFifoClientPriority_OFFSET      24
#define L2_CONTROL_0_IFifoClientPriority_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            AllowL1CacheVZero:1;
    UINT32                            AllowL1CacheATSRsp:1;
    UINT32                            DTCHitVZeroOrIVZero:1;
    UINT32                            Reserved_9_4:6;
    UINT32                            SIDEPTEOnUntransExcl:1;
    UINT32                            SIDEPTEOnAddrTransExcl:1;
    UINT32                            Allow_nonats_u_bit:1;
    UINT32                            Reserved_17_13:5;
    UINT32                            FLTCMBPriority:1;
    UINT32                            AllowL1CacheLargePagemode0:1;
    UINT32                            IFifoBurstLength:4;
    UINT32                            IFifoClientPriority:8;
  } Field;
  UINT32 Value;
} L2_CONTROL_0_STRUCT;

#define SMN_L2_CONTROL_0_ADDRESS    0x15700030UL


/***********************************************************
* Register Name : L2_CONTROL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the number of consecutive invalidation requests to perform when doing sequential invalidation. Regular L2 and invalidation requests alternate access to the main L2 caches based upon SeqInvBurstLimitInv and SeqInvBurstLimitL2Req.
#define L2_CONTROL_1_SeqInvBurstLimitInv_OFFSET      0
#define L2_CONTROL_1_SeqInvBurstLimitInv_MASK        0xff

// Bitfield Description : Sets the number of consecutive iommu L2 requests to perform when doing sequential invalidation. Regular L2 and invalidation requests alternate access to the main L2 caches based upon SeqInvBurstLimitInv and SeqInvBurstLimitL2Req.
#define L2_CONTROL_1_SeqInvBurstLimitL2Req_OFFSET      8
#define L2_CONTROL_1_SeqInvBurstLimitL2Req_MASK        0xff00

// Bitfield Description : Enable stalling L2 requests to allow invalidation cycles to make forward progress based upon SeqInvBurstLimitInv and SeqInvBurstLimitL2Req.
#define L2_CONTROL_1_SeqInvBurstLimitEn_OFFSET      16
#define L2_CONTROL_1_SeqInvBurstLimitEn_MASK        0x10000

// Bitfield Description : 
#define L2_CONTROL_1_Reserved_23_17_OFFSET      17
#define L2_CONTROL_1_Reserved_23_17_MASK        0xfe0000

// Bitfield Description : Fifo threshold level used to calculate certain performance counter values.
#define L2_CONTROL_1_PerfThreshold_OFFSET      24
#define L2_CONTROL_1_PerfThreshold_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SeqInvBurstLimitInv:8;
    UINT32                            SeqInvBurstLimitL2Req:8;
    UINT32                            SeqInvBurstLimitEn:1;
    UINT32                            Reserved_23_17:7;
    UINT32                            PerfThreshold:8;
  } Field;
  UINT32 Value;
} L2_CONTROL_1_STRUCT;

#define SMN_L2_CONTROL_1_ADDRESS    0x15700034UL


/***********************************************************
* Register Name : L2_CONTROL_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 0=Requests in the miss queue and the feedback queue are arbitrated in a round-robin manner.
// 1=Requests in the feedback queue are given priority over requests in the miss queue.
#define L2_CONTROL_5_QueueArbFBPri_OFFSET      0
#define L2_CONTROL_5_QueueArbFBPri_MASK        0x1

// Bitfield Description : 
#define L2_CONTROL_5_Reserved_1_1_OFFSET      1
#define L2_CONTROL_5_Reserved_1_1_MASK        0x2

// Bitfield Description : 0=FC1 flow control loop enabled.
// 1=FC1 flow control loop disabled.
#define L2_CONTROL_5_FC1Dis_OFFSET      2
#define L2_CONTROL_5_FC1Dis_MASK        0x4

// Bitfield Description : 0=DTEs with V=1 and IV=0 are not cached in the DTC.
// 1=DTEs with V=1 and IV=0 are cached in the DTC.
#define L2_CONTROL_5_DTCUpdateVOneIVZero_OFFSET      3
#define L2_CONTROL_5_DTCUpdateVOneIVZero_MASK        0x8

// Bitfield Description : 0=DTEs with V=0 and IV=1 are not cached in the DTC.
// 1=DTEs with V=0 and IV=1 are cached in the DTC.
#define L2_CONTROL_5_DTCUpdateVZeroIVOne_OFFSET      4
#define L2_CONTROL_5_DTCUpdateVZeroIVOne_MASK        0x10

// Bitfield Description : 
#define L2_CONTROL_5_Reserved_5_5_OFFSET      5
#define L2_CONTROL_5_Reserved_5_5_MASK        0x20

// Bitfield Description : 0=FC3 flow-control loop is enabled.
// 1=FC3 flow-control loop is disabled.
#define L2_CONTROL_5_FC3Dis_OFFSET      6
#define L2_CONTROL_5_FC3Dis_MASK        0x40

// Bitfield Description : 
#define L2_CONTROL_5_Reserved_10_7_OFFSET      7
#define L2_CONTROL_5_Reserved_10_7_MASK        0x780

// Bitfield Description : Force table walk fetches to use VC5 for all VC0/1 and VC5 DMA requests
#define L2_CONTROL_5_ForceTWonVCQoS_OFFSET      11
#define L2_CONTROL_5_ForceTWonVCQoS_MASK        0x800

// Bitfield Description : Each bit controls which level of Guest Page Directory SPA pointer is allowed to be cached in PTC. Bit 3: SPA pointer to PTE. Bit 6: SPA pointer to PDE. All other bits are reserved
#define L2_CONTROL_5_GST_partial_ptc_cntrl_OFFSET      12
#define L2_CONTROL_5_GST_partial_ptc_cntrl_MASK        0x7f000

// Bitfield Description : 
#define L2_CONTROL_5_Reserved_19_19_OFFSET      19
#define L2_CONTROL_5_Reserved_19_19_MASK        0x80000

// Bitfield Description : Allow caching Guest PDPE SPA pointer to be cached in QOS PTC cache.
#define L2_CONTROL_5_STORE_PDPE_QOS_PTC_OFFSET      20
#define L2_CONTROL_5_STORE_PDPE_QOS_PTC_MASK        0x100000

// Bitfield Description : 
#define L2_CONTROL_5_Reserved_24_21_OFFSET      21
#define L2_CONTROL_5_Reserved_24_21_MASK        0x1e00000

// Bitfield Description : 0=Do not cache DTEs fetch by PRI requests. 1=Cache DTEs fetched by PRI requests. In order to enable this bit, software should ensure that it marks the DTE invalid in the memory prior to issuing Device Invalidation.
#define L2_CONTROL_5_DTCUpdatePri_OFFSET      25
#define L2_CONTROL_5_DTCUpdatePri_MASK        0x2000000

// Bitfield Description : controls credits for L2B to L2A v1 trans interface
#define L2_CONTROL_5_L2B_L2A_v1_trans_credits_OFFSET      26
#define L2_CONTROL_5_L2B_L2A_v1_trans_credits_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            QueueArbFBPri:1;
    UINT32                            Reserved_1_1:1;
    UINT32                            FC1Dis:1;
    UINT32                            DTCUpdateVOneIVZero:1;
    UINT32                            DTCUpdateVZeroIVOne:1;
    UINT32                            Reserved_5_5:1;
    UINT32                            FC3Dis:1;
    UINT32                            Reserved_10_7:4;
    UINT32                            ForceTWonVCQoS:1;
    UINT32                            GST_partial_ptc_cntrl:7;
    UINT32                            Reserved_19_19:1;
    UINT32                            STORE_PDPE_QOS_PTC:1;
    UINT32                            Reserved_24_21:4;
    UINT32                            DTCUpdatePri:1;
    UINT32                            L2B_L2A_v1_trans_credits:6;
  } Field;
  UINT32 Value;
} L2_CONTROL_5_STRUCT;

#define SMN_L2_CONTROL_5_ADDRESS    0x13f01130UL


/***********************************************************
* Register Name : L2_CONTROL_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Sets the number of consecutive invalidation requests to perform when doing sequential invalidation. PDC and invalidation requests alternate access to the PDC based upon SeqInvBurstLimitInv and SeqInvBurstLimitPDCReq.
#define L2_CONTROL_6_SeqInvBurstLimitInv_OFFSET      0
#define L2_CONTROL_6_SeqInvBurstLimitInv_MASK        0xff

// Bitfield Description : Sets the number of consecutive PDC requests to perform when doing sequential invalidation. PDC and invalidation requests alternate access to the PDC based upon SeqInvBurstLimitInv and SeqInvBurstLimitPDCReq.
#define L2_CONTROL_6_SeqInvBurstLimitPDCReq_OFFSET      8
#define L2_CONTROL_6_SeqInvBurstLimitPDCReq_MASK        0xff00

// Bitfield Description : Enable stalling PDC requests to allow invalidation cycles to make forward progress based upon SeqInvBurstLimitInv and SeqInvBurstLimitPDCReq.
#define L2_CONTROL_6_SeqInvBurstLimitEn_OFFSET      16
#define L2_CONTROL_6_SeqInvBurstLimitEn_MASK        0x10000

// Bitfield Description : 
#define L2_CONTROL_6_Reserved_23_17_OFFSET      17
#define L2_CONTROL_6_Reserved_23_17_MASK        0xfe0000

// Bitfield Description : Fifo threshold level used to calculate certain performance counter values.
#define L2_CONTROL_6_Perf2Threshold_OFFSET      24
#define L2_CONTROL_6_Perf2Threshold_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SeqInvBurstLimitInv:8;
    UINT32                            SeqInvBurstLimitPDCReq:8;
    UINT32                            SeqInvBurstLimitEn:1;
    UINT32                            Reserved_23_17:7;
    UINT32                            Perf2Threshold:8;
  } Field;
  UINT32 Value;
} L2_CONTROL_6_STRUCT;

#define SMN_L2_CONTROL_6_ADDRESS    0x13f0113cUL


/***********************************************************
* Register Name : L2_CONTROL_7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Hysteresis for full disconnect initiation of sdp ports.
#define L2_CONTROL_7_PCTRL_hysteresis_OFFSET      0
#define L2_CONTROL_7_PCTRL_hysteresis_MASK        0xff

// Bitfield Description : 
#define L2_CONTROL_7_Reserved_31_8_OFFSET      8
#define L2_CONTROL_7_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            PCTRL_hysteresis:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} L2_CONTROL_7_STRUCT;

#define SMN_L2_CONTROL_7_ADDRESS    0x13f011a8UL


/***********************************************************
* Register Name : L2_CP_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=Command processor fetches and executes only one command at a time
// 0=Command processor prefetches available commands into its internal storage.
#define L2_CP_CONTROL_CPPrefetchDis_OFFSET      0
#define L2_CP_CONTROL_CPPrefetchDis_MASK        0x1

// Bitfield Description : 1=Command processor flushes out old requests on completion wait.
// 0=No flush is performed on completion wait.
#define L2_CP_CONTROL_CPFlushOnWait_OFFSET      1
#define L2_CP_CONTROL_CPFlushOnWait_MASK        0x2

// Bitfield Description : 1=Command processor flushes out old requests on every invalidation command.
// 0=No flush is performed during invalidations.
#define L2_CP_CONTROL_CPFlushOnInv_OFFSET      2
#define L2_CP_CONTROL_CPFlushOnInv_MASK        0x4

// Bitfield Description : Wait for IOTLB to completed prior to sending Event Log for Invalid Command.
#define L2_CP_CONTROL_CPStallCmdErrForIOTLBCmpl_OFFSET      3
#define L2_CP_CONTROL_CPStallCmdErrForIOTLBCmpl_MASK        0x8

// Bitfield Description : Set the value for CP ReqPassPW interface signal for Command Buffer reads.
#define L2_CP_CONTROL_CPForceReqPassPW_OFFSET      4
#define L2_CP_CONTROL_CPForceReqPassPW_MASK        0x10

// Bitfield Description : 
#define L2_CP_CONTROL_Reserved_15_5_OFFSET      5
#define L2_CP_CONTROL_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Command processor read delay.
#define L2_CP_CONTROL_CPRdDelay_OFFSET      16
#define L2_CP_CONTROL_CPRdDelay_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            CPPrefetchDis:1;
    UINT32                            CPFlushOnWait:1;
    UINT32                            CPFlushOnInv:1;
    UINT32                            CPStallCmdErrForIOTLBCmpl:1;
    UINT32                            CPForceReqPassPW:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            CPRdDelay:16;
  } Field;
  UINT32 Value;
} L2_CP_CONTROL_STRUCT;

#define SMN_L2_CP_CONTROL_ADDRESS    0x13f01158UL


/***********************************************************
* Register Name : L2_CP_CONTROL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register indicates to the IOMMU command processor that a corresponding L1 TLB is inaccessible due to static clock or power gating. System software is responsible for programming this register.
#define L2_CP_CONTROL_1_CPL1Off_OFFSET      0
#define L2_CP_CONTROL_1_CPL1Off_MASK        0xffff

// Bitfield Description : Reserved for future use. This register controls no hardware.
#define L2_CP_CONTROL_1_Reserved_31_16_OFFSET      16
#define L2_CP_CONTROL_1_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            CPL1Off:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} L2_CP_CONTROL_1_STRUCT;

#define SMN_L2_CP_CONTROL_1_ADDRESS    0x13f0115cUL


/***********************************************************
* Register Name : L2_CREDIT_CONTROL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : FC1 credit override value.
#define L2_CREDIT_CONTROL_0_FC1Credits_OFFSET      0
#define L2_CREDIT_CONTROL_0_FC1Credits_MASK        0x7f

// Bitfield Description : Changing this register from 0 to 1 overrides the FC1 credit counter with FC1Credits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_0_FC1Override_OFFSET      7
#define L2_CREDIT_CONTROL_0_FC1Override_MASK        0x80

// Bitfield Description : FC2 credit override value.
#define L2_CREDIT_CONTROL_0_FC2Credits_OFFSET      8
#define L2_CREDIT_CONTROL_0_FC2Credits_MASK        0x3f00

// Bitfield Description : Changing this register from 0 to 1 overrides the FC2 credit counter with FC2Credits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_0_FC2Override_OFFSET      14
#define L2_CREDIT_CONTROL_0_FC2Override_MASK        0x4000

// Bitfield Description : FC3 credit override value.
#define L2_CREDIT_CONTROL_0_FC3Credits_OFFSET      15
#define L2_CREDIT_CONTROL_0_FC3Credits_MASK        0x1f8000

// Bitfield Description : Changing this register from 0 to 1 overrides the FC3 credit counter with FC3Credits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_0_FC3Override_OFFSET      21
#define L2_CREDIT_CONTROL_0_FC3Override_MASK        0x200000

// Bitfield Description : Multi ATS split buffer override credits.
#define L2_CREDIT_CONTROL_0_MultATSCredits_OFFSET      22
#define L2_CREDIT_CONTROL_0_MultATSCredits_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            FC1Credits:7;
    UINT32                            FC1Override:1;
    UINT32                            FC2Credits:6;
    UINT32                            FC2Override:1;
    UINT32                            FC3Credits:6;
    UINT32                            FC3Override:1;
    UINT32                            MultATSCredits:10;
  } Field;
  UINT32 Value;
} L2_CREDIT_CONTROL_0_STRUCT;

#define SMN_L2_CREDIT_CONTROL_0_ADDRESS    0x13f011c0UL


/***********************************************************
* Register Name : L2_CREDIT_CONTROL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : PDTIE credit override value.
#define L2_CREDIT_CONTROL_1_PDTIECredits_OFFSET      0
#define L2_CREDIT_CONTROL_1_PDTIECredits_MASK        0x3f

// Bitfield Description : 
#define L2_CREDIT_CONTROL_1_Reserved_7_6_OFFSET      6
#define L2_CREDIT_CONTROL_1_Reserved_7_6_MASK        0xc0

// Bitfield Description : TWEL credit override value.
#define L2_CREDIT_CONTROL_1_TWELCredits_OFFSET      8
#define L2_CREDIT_CONTROL_1_TWELCredits_MASK        0x3f00

// Bitfield Description : 
#define L2_CREDIT_CONTROL_1_Reserved_15_14_OFFSET      14
#define L2_CREDIT_CONTROL_1_Reserved_15_14_MASK        0xc000

// Bitfield Description : Command processor prefetch credit override value.
#define L2_CREDIT_CONTROL_1_CP_PREFETCH_credits_OFFSET      16
#define L2_CREDIT_CONTROL_1_CP_PREFETCH_credits_MASK        0xf0000

// Bitfield Description : PPR logger credit override value.
#define L2_CREDIT_CONTROL_1_PPR_MCIF_credits_OFFSET      20
#define L2_CREDIT_CONTROL_1_PPR_MCIF_credits_MASK        0xf00000

// Bitfield Description : 
#define L2_CREDIT_CONTROL_1_Reserved_31_24_OFFSET      24
#define L2_CREDIT_CONTROL_1_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PDTIECredits:6;
    UINT32                            Reserved_7_6:2;
    UINT32                            TWELCredits:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            CP_PREFETCH_credits:4;
    UINT32                            PPR_MCIF_credits:4;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L2_CREDIT_CONTROL_1_STRUCT;

#define SMN_L2_CREDIT_CONTROL_1_ADDRESS    0x13f011c4UL


/***********************************************************
* Register Name : L2_CREDIT_CONTROL_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : QUEUE credit override value.
#define L2_CREDIT_CONTROL_2_QUEUECredits_OFFSET      0
#define L2_CREDIT_CONTROL_2_QUEUECredits_MASK        0x3f

// Bitfield Description : 
#define L2_CREDIT_CONTROL_2_Reserved_6_6_OFFSET      6
#define L2_CREDIT_CONTROL_2_Reserved_6_6_MASK        0x40

// Bitfield Description : Changing this register from 0 to 1 overrides the QUEUE credit counter with QUEUECredits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_2_QUEUEOverride_OFFSET      7
#define L2_CREDIT_CONTROL_2_QUEUEOverride_MASK        0x80

// Bitfield Description : FLTCMB credit override value.
#define L2_CREDIT_CONTROL_2_FLTCMBCredits_OFFSET      8
#define L2_CREDIT_CONTROL_2_FLTCMBCredits_MASK        0x3f00

// Bitfield Description : 
#define L2_CREDIT_CONTROL_2_Reserved_14_14_OFFSET      14
#define L2_CREDIT_CONTROL_2_Reserved_14_14_MASK        0x4000

// Bitfield Description : Changing this register from 0 to 1 overrides the FLTCMB credit counter with FLTCMBCredits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_2_FLTCMBOverride_OFFSET      15
#define L2_CREDIT_CONTROL_2_FLTCMBOverride_MASK        0x8000

// Bitfield Description : FCEL credit override value.
#define L2_CREDIT_CONTROL_2_FCELCredits_OFFSET      16
#define L2_CREDIT_CONTROL_2_FCELCredits_MASK        0x3f0000

// Bitfield Description : 
#define L2_CREDIT_CONTROL_2_Reserved_22_22_OFFSET      22
#define L2_CREDIT_CONTROL_2_Reserved_22_22_MASK        0x400000

// Bitfield Description : Changing this register from 0 to 1 overrides the FCEL credit counter with FCELCredits. This should only be performed when the IOMMU is idle.
#define L2_CREDIT_CONTROL_2_FCELOverride_OFFSET      23
#define L2_CREDIT_CONTROL_2_FCELOverride_MASK        0x800000

// Bitfield Description : PPR log buffer credit override value.
#define L2_CREDIT_CONTROL_2_PPR_logger_credits_OFFSET      24
#define L2_CREDIT_CONTROL_2_PPR_logger_credits_MASK        0xf000000

// Bitfield Description : 
#define L2_CREDIT_CONTROL_2_Reserved_31_28_OFFSET      28
#define L2_CREDIT_CONTROL_2_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            QUEUECredits:6;
    UINT32                            Reserved_6_6:1;
    UINT32                            QUEUEOverride:1;
    UINT32                            FLTCMBCredits:6;
    UINT32                            Reserved_14_14:1;
    UINT32                            FLTCMBOverride:1;
    UINT32                            FCELCredits:6;
    UINT32                            Reserved_22_22:1;
    UINT32                            FCELOverride:1;
    UINT32                            PPR_logger_credits:4;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} L2_CREDIT_CONTROL_2_STRUCT;

#define SMN_L2_CREDIT_CONTROL_2_ADDRESS    0x15700080UL


/***********************************************************
* Register Name : L2_DTC_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_DTC_CONTROL_Reserved_2_0_OFFSET      0
#define L2_DTC_CONTROL_Reserved_2_0_MASK        0x7

// Bitfield Description : 0=Reads update replacement state bits when there is a simultaneous read and write to the same DTC index.
// 1=Writes update replacement state bits when there is a simultaneous read and write to the same DTC index.
#define L2_DTC_CONTROL_DTCLRUUpdatePri_OFFSET      3
#define L2_DTC_CONTROL_DTCLRUUpdatePri_MASK        0x8

// Bitfield Description : Enable parity protection of the DTC.
#define L2_DTC_CONTROL_DTCParityEn_OFFSET      4
#define L2_DTC_CONTROL_DTCParityEn_MASK        0x10

// Bitfield Description : 
#define L2_DTC_CONTROL_Reserved_7_5_OFFSET      5
#define L2_DTC_CONTROL_Reserved_7_5_MASK        0xe0

// Bitfield Description : Selects the DTC invalidation algorithm.
// 00=Invalidate the entire DTC
// 01=Fast imprecise invalidation
// 10=Sequential precise invalidation
// 11=Partial sequential precise invalidation
#define L2_DTC_CONTROL_DTCInvalidationSel_OFFSET      8
#define L2_DTC_CONTROL_DTCInvalidationSel_MASK        0x300

// Bitfield Description : Software may write this register to 1 to invalidate all entries in the DTC.
#define L2_DTC_CONTROL_DTCSoftInvalidate_OFFSET      10
#define L2_DTC_CONTROL_DTCSoftInvalidate_MASK        0x400

// Bitfield Description : 
#define L2_DTC_CONTROL_Reserved_12_11_OFFSET      11
#define L2_DTC_CONTROL_Reserved_12_11_MASK        0x1800

// Bitfield Description : When set, all requests bypass the DTC.
#define L2_DTC_CONTROL_DTCBypass_OFFSET      13
#define L2_DTC_CONTROL_DTCBypass_MASK        0x2000

// Bitfield Description : 
#define L2_DTC_CONTROL_Reserved_14_14_OFFSET      14
#define L2_DTC_CONTROL_Reserved_14_14_MASK        0x4000

// Bitfield Description : 0=The DTC does not support parity protection.
// 1=The DTC supports parity protection.
#define L2_DTC_CONTROL_DTCParitySupport_OFFSET      15
#define L2_DTC_CONTROL_DTCParitySupport_MASK        0x8000

// Bitfield Description : Indicates the number of ways in the DTC.
#define L2_DTC_CONTROL_DTCWays_OFFSET      16
#define L2_DTC_CONTROL_DTCWays_MASK        0xff0000

// Bitfield Description : 
#define L2_DTC_CONTROL_Reserved_27_24_OFFSET      24
#define L2_DTC_CONTROL_Reserved_27_24_MASK        0xf000000

// Bitfield Description : The number of entries in the DTC is indicated as 2^DTCEntries.
#define L2_DTC_CONTROL_DTCEntries_OFFSET      28
#define L2_DTC_CONTROL_DTCEntries_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            DTCLRUUpdatePri:1;
    UINT32                            DTCParityEn:1;
    UINT32                            Reserved_7_5:3;
    UINT32                            DTCInvalidationSel:2;
    UINT32                            DTCSoftInvalidate:1;
    UINT32                            Reserved_12_11:2;
    UINT32                            DTCBypass:1;
    UINT32                            Reserved_14_14:1;
    UINT32                            DTCParitySupport:1;
    UINT32                            DTCWays:8;
    UINT32                            Reserved_27_24:4;
    UINT32                            DTCEntries:4;
  } Field;
  UINT32 Value;
} L2_DTC_CONTROL_STRUCT;

#define SMN_L2_DTC_CONTROL_ADDRESS    0x15700040UL


/***********************************************************
* Register Name : L2_DTC_HASH_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_DTC_HASH_CONTROL_Reserved_15_0_OFFSET      0
#define L2_DTC_HASH_CONTROL_Reserved_15_0_MASK        0xffff

// Bitfield Description : This register is a bit-wise AND mask that selects which bits from the untranslated interrupt {MT[2:0],Vector} are used to index into the DTC.
#define L2_DTC_HASH_CONTROL_DTCAddressMask_OFFSET      16
#define L2_DTC_HASH_CONTROL_DTCAddressMask_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            DTCAddressMask:16;
  } Field;
  UINT32 Value;
} L2_DTC_HASH_CONTROL_STRUCT;

#define SMN_L2_DTC_HASH_CONTROL_ADDRESS    0x15700044UL


/***********************************************************
* Register Name : L2_DTC_WAY_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables a way in the DTC when set to 1. An implementation may have less than 32 ways. The entire cache may be disabled by setting the DTCWays lower bits of this register.
#define L2_DTC_WAY_CONTROL_DTCWayDisable_OFFSET      0
#define L2_DTC_WAY_CONTROL_DTCWayDisable_MASK        0xffff

// Bitfield Description : Used with DTCWayDisable to disable a way of set associativity.
#define L2_DTC_WAY_CONTROL_DTCWayAccessDisable_OFFSET      16
#define L2_DTC_WAY_CONTROL_DTCWayAccessDisable_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            DTCWayDisable:16;
    UINT32                            DTCWayAccessDisable:16;
  } Field;
  UINT32 Value;
} L2_DTC_WAY_CONTROL_STRUCT;

#define SMN_L2_DTC_WAY_CONTROL_ADDRESS    0x15700048UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is write-once. Setting this register bit locks the error detection rule set in ERRRuleDisable0/1/2.
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET      0
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK        0x1

// Bitfield Description : 
#define L2_ERR_RULE_CONTROL_0_Reserved_2_1_OFFSET      1
#define L2_ERR_RULE_CONTROL_0_Reserved_2_1_MASK        0x6

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_0_ERRRuleDisable0_OFFSET      3
#define L2_ERR_RULE_CONTROL_0_ERRRuleDisable0_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            ERRRuleLock0:1;
    UINT32                            Reserved_2_1:2;
    UINT32                            ERRRuleDisable0:29;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_0_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_0_ADDRESS    0x13f01200UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_1_ERRRuleDisable1_OFFSET      0
#define L2_ERR_RULE_CONTROL_1_ERRRuleDisable1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable1:32;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_1_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_1_ADDRESS    0x13f01204UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_2_ERRRuleDisable2_OFFSET      0
#define L2_ERR_RULE_CONTROL_2_ERRRuleDisable2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable2:32;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_2_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_2_ADDRESS    0x13f01208UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register is write-once. Setting this register bit locks the error detection rule set in ERRRuleDisable3/4/5.
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET      0
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK        0x1

// Bitfield Description : 
#define L2_ERR_RULE_CONTROL_3_Reserved_3_1_OFFSET      1
#define L2_ERR_RULE_CONTROL_3_Reserved_3_1_MASK        0xe

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_3_ERRRuleDisable3_OFFSET      4
#define L2_ERR_RULE_CONTROL_3_ERRRuleDisable3_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            ERRRuleLock1:1;
    UINT32                            Reserved_3_1:3;
    UINT32                            ERRRuleDisable3:28;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_3_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_3_ADDRESS    0x157000c0UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_4_ERRRuleDisable4_OFFSET      0
#define L2_ERR_RULE_CONTROL_4_ERRRuleDisable4_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable4:32;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_4_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_4_ADDRESS    0x157000c4UL


/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables an error detection rule in the IOMMU.
#define L2_ERR_RULE_CONTROL_5_ERRRuleDisable5_OFFSET      0
#define L2_ERR_RULE_CONTROL_5_ERRRuleDisable5_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ERRRuleDisable5:32;
  } Field;
  UINT32 Value;
} L2_ERR_RULE_CONTROL_5_STRUCT;

#define SMN_L2_ERR_RULE_CONTROL_5_ADDRESS    0x157000c8UL


/***********************************************************
* Register Name : L2_ITC_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_ITC_CONTROL_Reserved_2_0_OFFSET      0
#define L2_ITC_CONTROL_Reserved_2_0_MASK        0x7

// Bitfield Description : 0=Reads update replacement state bits when there is a simultaneous read and write to the same ITC index.
// 1=Writes update replacement state bits when there is a simultaneous read and write to the same ITC index.
#define L2_ITC_CONTROL_ITCLRUUpdatePri_OFFSET      3
#define L2_ITC_CONTROL_ITCLRUUpdatePri_MASK        0x8

// Bitfield Description : Enable parity protection of the ITC.
#define L2_ITC_CONTROL_ITCParityEn_OFFSET      4
#define L2_ITC_CONTROL_ITCParityEn_MASK        0x10

// Bitfield Description : 
#define L2_ITC_CONTROL_Reserved_7_5_OFFSET      5
#define L2_ITC_CONTROL_Reserved_7_5_MASK        0xe0

// Bitfield Description : Selects the ITC invalidation algorithm.
// 00=Invalidate the entire ITC
// 01=Fast imprecise invalidation
// 10=Sequential precise invalidation
// 11=Partial sequential precise invalidation
#define L2_ITC_CONTROL_ITCInvalidationSel_OFFSET      8
#define L2_ITC_CONTROL_ITCInvalidationSel_MASK        0x300

// Bitfield Description : Software may write this register to 1 to invalidate all entries in the ITC.
#define L2_ITC_CONTROL_ITCSoftInvalidate_OFFSET      10
#define L2_ITC_CONTROL_ITCSoftInvalidate_MASK        0x400

// Bitfield Description : 
#define L2_ITC_CONTROL_Reserved_12_11_OFFSET      11
#define L2_ITC_CONTROL_Reserved_12_11_MASK        0x1800

// Bitfield Description : When set, all requests bypass the ITC.
#define L2_ITC_CONTROL_ITCBypass_OFFSET      13
#define L2_ITC_CONTROL_ITCBypass_MASK        0x2000

// Bitfield Description : 
#define L2_ITC_CONTROL_Reserved_14_14_OFFSET      14
#define L2_ITC_CONTROL_Reserved_14_14_MASK        0x4000

// Bitfield Description : 0=The ITC does not support parity protection.
// 1=The ITC supports parity protection.
#define L2_ITC_CONTROL_ITCParitySupport_OFFSET      15
#define L2_ITC_CONTROL_ITCParitySupport_MASK        0x8000

// Bitfield Description : Indicates the number of ways in the ITC.
#define L2_ITC_CONTROL_ITCWays_OFFSET      16
#define L2_ITC_CONTROL_ITCWays_MASK        0xff0000

// Bitfield Description : 
#define L2_ITC_CONTROL_Reserved_27_24_OFFSET      24
#define L2_ITC_CONTROL_Reserved_27_24_MASK        0xf000000

// Bitfield Description : The number of entries in the ITC is indicated as 2^ITCEntries.
#define L2_ITC_CONTROL_ITCEntries_OFFSET      28
#define L2_ITC_CONTROL_ITCEntries_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            ITCLRUUpdatePri:1;
    UINT32                            ITCParityEn:1;
    UINT32                            Reserved_7_5:3;
    UINT32                            ITCInvalidationSel:2;
    UINT32                            ITCSoftInvalidate:1;
    UINT32                            Reserved_12_11:2;
    UINT32                            ITCBypass:1;
    UINT32                            Reserved_14_14:1;
    UINT32                            ITCParitySupport:1;
    UINT32                            ITCWays:8;
    UINT32                            Reserved_27_24:4;
    UINT32                            ITCEntries:4;
  } Field;
  UINT32 Value;
} L2_ITC_CONTROL_STRUCT;

#define SMN_L2_ITC_CONTROL_ADDRESS    0x15700050UL


/***********************************************************
* Register Name : L2_ITC_HASH_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_ITC_HASH_CONTROL_Reserved_15_0_OFFSET      0
#define L2_ITC_HASH_CONTROL_Reserved_15_0_MASK        0xffff

// Bitfield Description : This register is a bit-wise AND mask that selects which bits from the untranslated interrupt {MT[2:0],Vector} are used to index into the ITC.
#define L2_ITC_HASH_CONTROL_ITCAddressMask_OFFSET      16
#define L2_ITC_HASH_CONTROL_ITCAddressMask_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            ITCAddressMask:16;
  } Field;
  UINT32 Value;
} L2_ITC_HASH_CONTROL_STRUCT;

#define SMN_L2_ITC_HASH_CONTROL_ADDRESS    0x15700054UL


/***********************************************************
* Register Name : L2_ITC_WAY_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables a way in the ITC when set to 1. An implementation may have less than 32 ways. The entire cache may be disabled by setting the ITCWays lower bits of this register.
#define L2_ITC_WAY_CONTROL_ITCWayDisable_OFFSET      0
#define L2_ITC_WAY_CONTROL_ITCWayDisable_MASK        0xffff

// Bitfield Description : Used with ITCWayDisable to disable a way of set associativity.
#define L2_ITC_WAY_CONTROL_ITCWayAccessDisable_OFFSET      16
#define L2_ITC_WAY_CONTROL_ITCWayAccessDisable_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ITCWayDisable:16;
    UINT32                            ITCWayAccessDisable:16;
  } Field;
  UINT32 Value;
} L2_ITC_WAY_CONTROL_STRUCT;

#define SMN_L2_ITC_WAY_CONTROL_ADDRESS    0x15700058UL


/***********************************************************
* Register Name : L2_L2A_CK_GATE_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable the gating of the l2b register clock branch.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET      0
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK        0x1

// Bitfield Description : Disable the gating of the l2b dynamic clock branch.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET      1
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK        0x2

// Bitfield Description : Disable the gating of the l2b upper cache ways.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET      2
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK        0x4

// Bitfield Description : Spare bit
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ASpare_OFFSET      3
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ASpare_MASK        0x8

// Bitfield Description : 0=allow 128 clock cycles delay before stopping the clocks when idle asserts
// 1=allow 256 clock cycles delay before stopping the clocks when idle asserts
// 2=allow 512 clock cycles delay before stopping the clocks when idle asserts
// 3=allow 1024 clock cycles delay before stopping the clocks when idle asserts
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ALength_OFFSET      4
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ALength_MASK        0x30

// Bitfield Description : 0=allow 2 clock cycles delay before stopping the clocks when clkready deasserts
// 1=allow 4 clock cycles delay before stopping the clocks when clkready deasserts
// 2=allow 8 clock cycles delay before stopping the clocks when clkready deasserts
// 3=allow 16 clock cycles delay before stopping the clocks when clkready deasserts
#define L2_L2A_CK_GATE_CONTROL_CKGateL2AStop_OFFSET      6
#define L2_L2A_CK_GATE_CONTROL_CKGateL2AStop_MASK        0xc0

// Bitfield Description : 
#define L2_L2A_CK_GATE_CONTROL_Reserved_31_8_OFFSET      8
#define L2_L2A_CK_GATE_CONTROL_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            CKGateL2ARegsDisable:1;
    UINT32                            CKGateL2ADynamicDisable:1;
    UINT32                            CKGateL2ACacheDisable:1;
    UINT32                            CKGateL2ASpare:1;
    UINT32                            CKGateL2ALength:2;
    UINT32                            CKGateL2AStop:2;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} L2_L2A_CK_GATE_CONTROL_STRUCT;

#define SMN_L2_L2A_CK_GATE_CONTROL_ADDRESS    0x157000ccUL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable light sleep feature for cache.
#define L2_L2A_MEMPWR_GATE_1_L2AREG_LS_EN_OFFSET      0
#define L2_L2A_MEMPWR_GATE_1_L2AREG_LS_EN_MASK        0x1

// Bitfield Description : Enable deep  sleep feature for cache.
#define L2_L2A_MEMPWR_GATE_1_L2AREG_DS_EN_OFFSET      1
#define L2_L2A_MEMPWR_GATE_1_L2AREG_DS_EN_MASK        0x2

// Bitfield Description : Enable shutdown feature for cache.
#define L2_L2A_MEMPWR_GATE_1_L2AREG_SD_EN_OFFSET      2
#define L2_L2A_MEMPWR_GATE_1_L2AREG_SD_EN_MASK        0x4

// Bitfield Description : 
#define L2_L2A_MEMPWR_GATE_1_Reserved_3_3_OFFSET      3
#define L2_L2A_MEMPWR_GATE_1_Reserved_3_3_MASK        0x8

// Bitfield Description : Selects wheather the L2A cache memory macros are power gated using the P1_mem_power_ctrl bus from the RSMU.PGFSM (0x0), or by the IP_PGMEM_CTRL module (0x1).
#define L2_L2A_MEMPWR_GATE_1_L2AREG_CACHE_PGMEM_SEL_OFFSET      4
#define L2_L2A_MEMPWR_GATE_1_L2AREG_CACHE_PGMEM_SEL_MASK        0x10

// Bitfield Description : 
#define L2_L2A_MEMPWR_GATE_1_Reserved_31_5_OFFSET      5
#define L2_L2A_MEMPWR_GATE_1_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            L2AREG_LS_EN:1;
    UINT32                            L2AREG_DS_EN:1;
    UINT32                            L2AREG_SD_EN:1;
    UINT32                            Reserved_3_3:1;
    UINT32                            L2AREG_CACHE_PGMEM_SEL:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_1_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_1_ADDRESS    0x157000d4UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_10
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request exit maintain time
#define L2_L2A_MEMPWR_GATE_10_L2AREG_SD_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_10_L2AREG_SD_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_SD_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_10_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_10_ADDRESS    0x157000fcUL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in light sleep.
#define L2_L2A_MEMPWR_GATE_2_L2AREG_LS_thres_OFFSET      0
#define L2_L2A_MEMPWR_GATE_2_L2AREG_LS_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_LS_thres:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_2_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_2_ADDRESS    0x157000d8UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in deep sleep.
#define L2_L2A_MEMPWR_GATE_3_L2AREG_DS_thres_OFFSET      0
#define L2_L2A_MEMPWR_GATE_3_L2AREG_DS_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_DS_thres:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_3_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_3_ADDRESS    0x157000dcUL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in shutdown.
#define L2_L2A_MEMPWR_GATE_4_L2AREG_SD_thres_OFFSET      0
#define L2_L2A_MEMPWR_GATE_4_L2AREG_SD_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_SD_thres:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_4_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_4_ADDRESS    0x157000e0UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request maintain time
#define L2_L2A_MEMPWR_GATE_5_L2AREG_LS_Req_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_5_L2AREG_LS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_LS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_5_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_5_ADDRESS    0x157000e4UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request exit maintain time
#define L2_L2A_MEMPWR_GATE_6_L2AREG_LS_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_6_L2AREG_LS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_LS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_6_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_6_ADDRESS    0x157000e8UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request maintain time
#define L2_L2A_MEMPWR_GATE_7_L2AREG_DS_Req_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_7_L2AREG_DS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_DS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_7_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_7_ADDRESS    0x157000ecUL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_8
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request exit maintain time
#define L2_L2A_MEMPWR_GATE_8_L2AREG_DS_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_8_L2AREG_DS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_DS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_8_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_8_ADDRESS    0x157000f0UL


/***********************************************************
* Register Name : L2_L2A_MEMPWR_GATE_9
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request maintain time
#define L2_L2A_MEMPWR_GATE_9_L2AREG_SD_Req_Maintain_Cnt_OFFSET      0
#define L2_L2A_MEMPWR_GATE_9_L2AREG_SD_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2AREG_SD_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2A_MEMPWR_GATE_9_STRUCT;

#define SMN_L2_L2A_MEMPWR_GATE_9_ADDRESS    0x157000f4UL


/***********************************************************
* Register Name : L2_L2A_PGSIZE_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the TLB search for different (final and partial) guest page sizes.
// Bit 0 = 4k final TLB look up enabled(cannot be disabled)
// Bit 3 = 2MB partial and final TLB look up enabled
// Bit 6  = 1GB partial and final TLB look up enabled. All other bits are reserved. The register is used when L2AREG_PTCSCAN_MODE=000b
#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_OFFSET      0
#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_MASK        0x7f

// Bitfield Description : 
#define L2_L2A_PGSIZE_CONTROL_Reserved_7_7_OFFSET      7
#define L2_L2A_PGSIZE_CONTROL_Reserved_7_7_MASK        0x80

// Bitfield Description : Controls the TLB search for different (final and partial) host page sizes.
// Bit 0 = 4k final TLB look up enabled(cannot be disabled)
// Bit 3 = 2MB partial and final TLB look up enabled
// Bit 6 = 1GB partial and final TLB look up enabled
// All other bits are reserved.The register is used when L2AREG_PTCSCAN_MODE=000b
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_OFFSET      8
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_MASK        0x7f00

// Bitfield Description : 
#define L2_L2A_PGSIZE_CONTROL_Reserved_16_15_OFFSET      15
#define L2_L2A_PGSIZE_CONTROL_Reserved_16_15_MASK        0x18000

// Bitfield Description : Defines the order in which the different page size for host and guest are to be looked up. Current enabled modes are
// 000b=Legacy PTC lookup mode. Eeach translation request triggers back-to-back PTC lookups for different sizes based on L2AREG_HOST_PGSIZE and L2AREG_GUEST_PGSIZE registers
// Remaing encodings enable PTC feedback mode where each translation request triggers page size lookup through PTC based on the order specified by the encoding.For a miss, it is fed back into PTC with next page size in the order.
// 001b=PTC feedback scan mode order 4KB,2MB,1G.
// 010b=PTC feedback scan mode order 2MB,4KB,1G.
// All other encodings are reserved.
#define L2_L2A_PGSIZE_CONTROL_L2AREG_PTCSCAN_MODE_OFFSET      17
#define L2_L2A_PGSIZE_CONTROL_L2AREG_PTCSCAN_MODE_MASK        0xe0000

// Bitfield Description : 
#define L2_L2A_PGSIZE_CONTROL_Reserved_31_20_OFFSET      20
#define L2_L2A_PGSIZE_CONTROL_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            L2AREG_GST_PGSIZE:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            L2AREG_HOST_PGSIZE:7;
    UINT32                            Reserved_16_15:2;
    UINT32                            L2AREG_PTCSCAN_MODE:3;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} L2_L2A_PGSIZE_CONTROL_STRUCT;

#define SMN_L2_L2A_PGSIZE_CONTROL_ADDRESS    0x157000d0UL


/***********************************************************
* Register Name : L2_L2B_CK_GATE_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable the gating of the l2b register clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET      0
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK        0x1

// Bitfield Description : Disable the gating of the l2b dynamic clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET      1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK        0x2

// Bitfield Description : Disable the gating of the l2b miscellaneous clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET      2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK        0x4

// Bitfield Description : Disable the gating of the l2b upper cache ways.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET      3
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK        0x8

// Bitfield Description : Disable the gating of the l2b performance counter clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BPerfDisable_OFFSET      4
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BPerfDisable_MASK        0x10

// Bitfield Description : Disable the gating of the l2b architected performance counter clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BApcDisable_OFFSET      5
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BApcDisable_MASK        0x20

// Bitfield Description : 0=allow 128 clock cycles delay before stopping the clocks when idle asserts
// 1=allow 256 clock cycles delay before stopping the clocks when idle asserts
// 2=allow 512 clock cycles delay before stopping the clocks when idle asserts
// 3=allow 1024 clock cycles delay before stopping the clocks when idle asserts
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BLength_OFFSET      6
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BLength_MASK        0xc0

// Bitfield Description : 0=allow 2 clock cycles delay before stopping the clocks when clkready deasserts
// 1=allow 4 clock cycles delay before stopping the clocks when clkready deasserts
// 2=allow 8 clock cycles delay before stopping the clocks when clkready deasserts
// 3=allow 16 clock cycles delay before stopping the clocks when clkready deasserts
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BStop_OFFSET      8
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BStop_MASK        0x300

// Bitfield Description : 
#define L2_L2B_CK_GATE_CONTROL_Reserved_31_10_OFFSET      10
#define L2_L2B_CK_GATE_CONTROL_Reserved_31_10_MASK        0xfffffc00

typedef union {
  struct {
    UINT32                            CKGateL2BRegsDisable:1;
    UINT32                            CKGateL2BDynamicDisable:1;
    UINT32                            CKGateL2BMiscDisable:1;
    UINT32                            CKGateL2BCacheDisable:1;
    UINT32                            CKGateL2BPerfDisable:1;
    UINT32                            CKGateL2BApcDisable:1;
    UINT32                            CKGateL2BLength:2;
    UINT32                            CKGateL2BStop:2;
    UINT32                            Reserved_31_10:22;
  } Field;
  UINT32 Value;
} L2_L2B_CK_GATE_CONTROL_STRUCT;

#define SMN_L2_L2B_CK_GATE_CONTROL_ADDRESS    0x13f01240UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable light sleep feature for cache.
#define L2_L2B_MEMPWR_GATE_1_L2BREG_LS_EN_OFFSET      0
#define L2_L2B_MEMPWR_GATE_1_L2BREG_LS_EN_MASK        0x1

// Bitfield Description : Enable deep  sleep feature for cache.
#define L2_L2B_MEMPWR_GATE_1_L2BREG_DS_EN_OFFSET      1
#define L2_L2B_MEMPWR_GATE_1_L2BREG_DS_EN_MASK        0x2

// Bitfield Description : Enable shutdown feature for cache.
#define L2_L2B_MEMPWR_GATE_1_L2BREG_SD_EN_OFFSET      2
#define L2_L2B_MEMPWR_GATE_1_L2BREG_SD_EN_MASK        0x4

// Bitfield Description : If dynamic PG is present this feild determines wheather datapath memory is controlled via dynamic PG or internal PGMEM logic, 0 for dynbamic PG
#define L2_L2B_MEMPWR_GATE_1_L2B_IP_PGMEM_SEL_OFFSET      3
#define L2_L2B_MEMPWR_GATE_1_L2B_IP_PGMEM_SEL_MASK        0x8

// Bitfield Description : Selects wheather the L2B cache memory macros are power gated using the P1_mem_power_ctrl bus from the RSMU.PGFSM (0x0), or by the IP_PGMEM_CTRL module (0x1).
#define L2_L2B_MEMPWR_GATE_1_L2BREG_CACHE_PGMEM_SEL_OFFSET      4
#define L2_L2B_MEMPWR_GATE_1_L2BREG_CACHE_PGMEM_SEL_MASK        0x10

// Bitfield Description : 
#define L2_L2B_MEMPWR_GATE_1_Reserved_31_5_OFFSET      5
#define L2_L2B_MEMPWR_GATE_1_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            L2BREG_LS_EN:1;
    UINT32                            L2BREG_DS_EN:1;
    UINT32                            L2BREG_SD_EN:1;
    UINT32                            L2B_IP_PGMEM_SEL:1;
    UINT32                            L2BREG_CACHE_PGMEM_SEL:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_1_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_1_ADDRESS    0x13f01254UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_10
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request exit maintain time
#define L2_L2B_MEMPWR_GATE_10_L2BREG_SD_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_10_L2BREG_SD_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_SD_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_10_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_10_ADDRESS    0x13f012a4UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in light sleep.
#define L2_L2B_MEMPWR_GATE_2_L2BREG_LS_thres_OFFSET      0
#define L2_L2B_MEMPWR_GATE_2_L2BREG_LS_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_LS_thres:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_2_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_2_ADDRESS    0x13f01258UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in deep sleep.
#define L2_L2B_MEMPWR_GATE_3_L2BREG_DS_thres_OFFSET      0
#define L2_L2B_MEMPWR_GATE_3_L2BREG_DS_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_DS_thres:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_3_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_3_ADDRESS    0x13f0125cUL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Threshold value to go in shutdown.
#define L2_L2B_MEMPWR_GATE_4_L2BREG_SD_thres_OFFSET      0
#define L2_L2B_MEMPWR_GATE_4_L2BREG_SD_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_SD_thres:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_4_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_4_ADDRESS    0x13f01260UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request maintain time
#define L2_L2B_MEMPWR_GATE_5_L2BREG_LS_Req_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_5_L2BREG_LS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_LS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_5_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_5_ADDRESS    0x13f01290UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : LS request exit maintain time
#define L2_L2B_MEMPWR_GATE_6_L2BREG_LS_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_6_L2BREG_LS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_LS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_6_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_6_ADDRESS    0x13f01294UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request maintain time
#define L2_L2B_MEMPWR_GATE_7_L2BREG_DS_Req_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_7_L2BREG_DS_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_DS_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_7_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_7_ADDRESS    0x13f01298UL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_8
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : DS request exit maintain time
#define L2_L2B_MEMPWR_GATE_8_L2BREG_DS_Exit_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_8_L2BREG_DS_Exit_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_DS_Exit_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_8_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_8_ADDRESS    0x13f0129cUL


/***********************************************************
* Register Name : L2_L2B_MEMPWR_GATE_9
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : SD request maintain time
#define L2_L2B_MEMPWR_GATE_9_L2BREG_SD_Req_Maintain_Cnt_OFFSET      0
#define L2_L2B_MEMPWR_GATE_9_L2BREG_SD_Req_Maintain_Cnt_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            L2BREG_SD_Req_Maintain_Cnt:32;
  } Field;
  UINT32 Value;
} L2_L2B_MEMPWR_GATE_9_STRUCT;

#define SMN_L2_L2B_MEMPWR_GATE_9_ADDRESS    0x13f012a0UL


/***********************************************************
* Register Name : L2_L2B_PGSIZE_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the allowed Guest PTE sizes stored in PTC cache. Following are the valid programming bit values for this register
// bit 0 = Enable 4k (4k is always on..cannot be disabled)
// bit 3 = Enable 2M final guest page size caching
// bit 6 = Enabled 1G final guest page size caching
// If a given final page size is disabled, next enabled lowest page size will be cached. All other bits are reserved
#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_OFFSET      0
#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_MASK        0x7f

// Bitfield Description : 
#define L2_L2B_PGSIZE_CONTROL_Reserved_7_7_OFFSET      7
#define L2_L2B_PGSIZE_CONTROL_Reserved_7_7_MASK        0x80

// Bitfield Description : Controls the allowed Host PTE sizes stored in PTC cache. Following are the valid programming bit values for this register
// bit 0 = Enable 4k (4k is always on..cannot be disabled)
// bit 3 = Enable 2M final host page size caching
// bit 6 = Enabled 1G final host page size caching.
// All other bits are reserved.
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_OFFSET      8
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_MASK        0x7f00

// Bitfield Description : 
#define L2_L2B_PGSIZE_CONTROL_Reserved_31_15_OFFSET      15
#define L2_L2B_PGSIZE_CONTROL_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            L2BREG_GST_PGSIZE:7;
    UINT32                            Reserved_7_7:1;
    UINT32                            L2BREG_HOST_PGSIZE:7;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} L2_L2B_PGSIZE_CONTROL_STRUCT;

#define SMN_L2_L2B_PGSIZE_CONTROL_ADDRESS    0x13f01250UL


/***********************************************************
* Register Name : L2_MISC_CNTRL_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Ignore Write permission checks for ATS requests.
#define L2_MISC_CNTRL_3_REG_ats_iw_OFFSET      0
#define L2_MISC_CNTRL_3_REG_ats_iw_MASK        0x1

// Bitfield Description : Value = 1 Mask il2_nw with L2PDE_L2TW_vaoffset[0].
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_OFFSET      1
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_MASK        0x2

// Bitfield Description : TW atomic filter enable.
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_OFFSET      2
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_MASK        0x4

// Bitfield Description : Enables putting the L1 Work Queue Identifier in the event log entry.
#define L2_MISC_CNTRL_3_REG_l1wq_id_el_en_OFFSET      3
#define L2_MISC_CNTRL_3_REG_l1wq_id_el_en_MASK        0x8

// Bitfield Description : Enables strict ordering of legacy PPR and vIOMMU virtual PPR requests
#define L2_MISC_CNTRL_3_REG_PPR_StrictOrder_En_OFFSET      4
#define L2_MISC_CNTRL_3_REG_PPR_StrictOrder_En_MASK        0x10

// Bitfield Description : Enables filtering of vIOMMU Interrupts before they are sent out to the L2A
#define L2_MISC_CNTRL_3_REG_vIOMMU_IntFilter_En_OFFSET      5
#define L2_MISC_CNTRL_3_REG_vIOMMU_IntFilter_En_MASK        0x20

// Bitfield Description : TW pagemode0 fix disable.
#define L2_MISC_CNTRL_3_REG_pagemode0_fix_dis_OFFSET      6
#define L2_MISC_CNTRL_3_REG_pagemode0_fix_dis_MASK        0x40

// Bitfield Description : ErrLog ATS on ir/iw 0.
#define L2_MISC_CNTRL_3_REG_LogATS_iriw_zero_OFFSET      7
#define L2_MISC_CNTRL_3_REG_LogATS_iriw_zero_MASK        0x80

// Bitfield Description : Update PTE on ATS.
#define L2_MISC_CNTRL_3_REG_PTC_Update_AddrTransReq_OFFSET      8
#define L2_MISC_CNTRL_3_REG_PTC_Update_AddrTransReq_MASK        0x100

// Bitfield Description : 1=Disable reset of PPR Logger index offset when BASE LO register is updated.
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_BaseHiAcc_Dis_OFFSET      9
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_BaseHiAcc_Dis_MASK        0x200

// Bitfield Description : 1=Disable reset of PPR Logger index offset when BASE HI register is updated.
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_BaseLoAcc_Dis_OFFSET      10
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_BaseLoAcc_Dis_MASK        0x400

// Bitfield Description : 1=Disable reset of PPR Logger index offset when BASE LENGTH register is updated.
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_LenAccs_En_OFFSET      11
#define L2_MISC_CNTRL_3_REG_RstPtrs_on_LenAccs_En_MASK        0x800

// Bitfield Description : Disable error on DTE access with reserve bits set ilegal value.
#define L2_MISC_CNTRL_3_REG_DTEResvBitChkDis_OFFSET      12
#define L2_MISC_CNTRL_3_REG_DTEResvBitChkDis_MASK        0x1000

// Bitfield Description : 1=Disable reset of GVA Logger index offset when BASE LO register is updated.
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_BaseHiAcc_Dis_OFFSET      13
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_BaseHiAcc_Dis_MASK        0x2000

// Bitfield Description : 1=Disable reset of GVA Logger index offset when BASE HI register is updated.
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_BaseLoAcc_Dis_OFFSET      14
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_BaseLoAcc_Dis_MASK        0x4000

// Bitfield Description : 1=Disable reset of GVA Logger index offset when BASE LENGTH register is updated.
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_LenAccs_En_OFFSET      15
#define L2_MISC_CNTRL_3_REG_RstGVAPtrs_on_LenAccs_En_MASK        0x8000

// Bitfield Description : Enables strict ordering of vfmmio MCIF/SDP requests
#define L2_MISC_CNTRL_3_REG_VFMMIO_StrictOrder_En_OFFSET      16
#define L2_MISC_CNTRL_3_REG_VFMMIO_StrictOrder_En_MASK        0x10000

// Bitfield Description : 1=Enables strict ordering of vfmmio MCIF/SDP requests
#define L2_MISC_CNTRL_3_REG_WaitPtr_WrRsp_OFFSET      17
#define L2_MISC_CNTRL_3_REG_WaitPtr_WrRsp_MASK        0x20000

// Bitfield Description : 1=Requests with DTE.MODE=0 will be stored in the L1 TLB as large pages. 0=Requests with DTE.MODE=0 will be stored in the L1 TLB as 4k pages. Note: This register must be programmed to the same value as its copy in L1_MISC_CNTRL_2 located in iommu_l1 and in L2_CONTROL_0 in iommu_l2a.
#define L2_MISC_CNTRL_3_REG_AllowL1CacheLargePagemode0_OFFSET      18
#define L2_MISC_CNTRL_3_REG_AllowL1CacheLargePagemode0_MASK        0x40000

// Bitfield Description : 1=Responses to L1 with U bit set will not be gated by AT field. 0=Responses to L1 with U bit set will be gated unless AT=1. Note: This register must be programmed to the same value as its copy in L2_CONTROL_0 located in iommu_l2a.
#define L2_MISC_CNTRL_3_REG_Allow_nonats_u_bit_OFFSET      19
#define L2_MISC_CNTRL_3_REG_Allow_nonats_u_bit_MASK        0x80000

// Bitfield Description : Drives the Message Type value for GAPPI. Default to 000=fixed interrupt
#define L2_MISC_CNTRL_3_REG_GAPPI_MT_OFFSET      20
#define L2_MISC_CNTRL_3_REG_GAPPI_MT_MASK        0xf00000

// Bitfield Description : Drives the Destination Mode value for GAPPI. Default to 0=Physical Mode
#define L2_MISC_CNTRL_3_REG_GAPPI_DM_OFFSET      24
#define L2_MISC_CNTRL_3_REG_GAPPI_DM_MASK        0x1000000

// Bitfield Description : Drive the Trigger Mode for GAPPI, Default to 0=Edge
#define L2_MISC_CNTRL_3_REG_GAPPI_TM_OFFSET      25
#define L2_MISC_CNTRL_3_REG_GAPPI_TM_MASK        0x2000000

// Bitfield Description : 
#define L2_MISC_CNTRL_3_Reserved_30_26_OFFSET      26
#define L2_MISC_CNTRL_3_Reserved_30_26_MASK        0x7c000000

// Bitfield Description : Controls no hardware.
#define L2_MISC_CNTRL_3_REG_gmc_iommu_dis_OFFSET      31
#define L2_MISC_CNTRL_3_REG_gmc_iommu_dis_MASK        0x80000000

typedef union {
  struct {
    UINT32                            REG_ats_iw:1;
    UINT32                            REG_mask_l2_nw:1;
    UINT32                            REG_atomic_filter_en:1;
    UINT32                            REG_l1wq_id_el_en:1;
    UINT32                            REG_PPR_StrictOrder_En:1;
    UINT32                            REG_vIOMMU_IntFilter_En:1;
    UINT32                            REG_pagemode0_fix_dis:1;
    UINT32                            REG_LogATS_iriw_zero:1;
    UINT32                            REG_PTC_Update_AddrTransReq:1;
    UINT32                            REG_RstPtrs_on_BaseHiAcc_Dis:1;
    UINT32                            REG_RstPtrs_on_BaseLoAcc_Dis:1;
    UINT32                            REG_RstPtrs_on_LenAccs_En:1;
    UINT32                            REG_DTEResvBitChkDis:1;
    UINT32                            REG_RstGVAPtrs_on_BaseHiAcc_Dis:1;
    UINT32                            REG_RstGVAPtrs_on_BaseLoAcc_Dis:1;
    UINT32                            REG_RstGVAPtrs_on_LenAccs_En:1;
    UINT32                            REG_VFMMIO_StrictOrder_En:1;
    UINT32                            REG_WaitPtr_WrRsp:1;
    UINT32                            REG_AllowL1CacheLargePagemode0:1;
    UINT32                            REG_Allow_nonats_u_bit:1;
    UINT32                            REG_GAPPI_MT:4;
    UINT32                            REG_GAPPI_DM:1;
    UINT32                            REG_GAPPI_TM:1;
    UINT32                            Reserved_30_26:5;
    UINT32                            REG_gmc_iommu_dis:1;
  } Field;
  UINT32 Value;
} L2_MISC_CNTRL_3_STRUCT;

#define SMN_L2_MISC_CNTRL_3_ADDRESS    0x13f0111cUL


/***********************************************************
* Register Name : L2_MULTATS_PTE_GROUP_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=Enables grouping 8 contiguous 4KB host or guest (v2 only) PTEs into 32KB ATS response to client for INTGFX.
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_INTGFX_32K_PTE_GROUP_EN_OFFSET      0
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_INTGFX_32K_PTE_GROUP_EN_MASK        0x1

// Bitfield Description : 1=Enables grouping 8 contiguous 4KB host or guest (v2 only) PTEs into 32KB ATS response to client for non-INTGFX clients.
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_OTHER_32K_PTE_GROUP_EN_OFFSET      1
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_OTHER_32K_PTE_GROUP_EN_MASK        0x2

// Bitfield Description : 1=Enables grouping 8 contiguous 4KB host or guest (v2 only) PTEs into 32KB ATS response to client for non-INTGFX clients.
#define L2_MULTATS_PTE_GROUP_CNTRL_PTE_GROUP_NONATS_EN_OFFSET      2
#define L2_MULTATS_PTE_GROUP_CNTRL_PTE_GROUP_NONATS_EN_MASK        0x4

// Bitfield Description : 1=Duplicate the same 32KB ATS response for as many PTEs were requested. 0=32KB ATS response to client will contain 1 PTE.
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_RSP_SPLIT_REQ_PTE_GROUP_OFFSET      3
#define L2_MULTATS_PTE_GROUP_CNTRL_L1_RSP_SPLIT_REQ_PTE_GROUP_MASK        0x8

// Bitfield Description : 1=Follow legacy TW cpl_decode behaviour to handle split req for multi-ats. 0=Truncate the TW cpl_decode split for multi-ats at the first PTE.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_TW_SPLIT_REQ_PTE_GROUP_OFFSET      4
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_TW_SPLIT_REQ_PTE_GROUP_MASK        0x10

// Bitfield Description : 1=Force 32K pte grouping logic to match for PTE valid bit. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_v_OFFSET      5
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_v_MASK        0x20

// Bitfield Description : 1=Force 32K pte grouping logic to decode a host PTE. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_all_host_OFFSET      6
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_all_host_MASK        0x40

// Bitfield Description : 1=Force 32K pte grouping logic to decode a default size 4K page. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_4k_OFFSET      7
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_4k_MASK        0x80

// Bitfield Description : 1=Force 32K pte grouping logic to match all PTE U bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_u_OFFSET      8
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_u_MASK        0x100

// Bitfield Description : 1=Force 32K pte grouping logic to match all PTE FC bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_fc_OFFSET      9
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_fc_MASK        0x200

// Bitfield Description : 1=Force 32K pte grouping logic to match all PTE IR bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_ir_OFFSET      10
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_ir_MASK        0x400

// Bitfield Description : 1=Force 32K pte grouping logic to match all PTE IW bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_iw_OFFSET      11
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_iw_MASK        0x800

// Bitfield Description : 1=Force 32K pte grouping logic to match all PTE reserved bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_res_OFFSET      12
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_res_MASK        0x1000

// Bitfield Description : 1=Force 32K pte grouping logic to decode a guest PTE. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_all_guest_OFFSET      13
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_all_guest_MASK        0x2000

// Bitfield Description : 1=Force 32K pte grouping logic to match all gPTE US bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_us_OFFSET      14
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_us_MASK        0x4000

// Bitfield Description : 1=Force 32K pte grouping logic to match all gPTE NX bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_nx_OFFSET      15
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_nx_MASK        0x8000

// Bitfield Description : 1=Force 32K pte grouping logic to match all gPTE IW bits. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_giw_OFFSET      16
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_giw_MASK        0x10000

// Bitfield Description : 1=Force 32K pte grouping logic to decode proper order for 8 4KB PTEs. 0=Match only when relevant condition is met.
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_order32k_OFFSET      17
#define L2_MULTATS_PTE_GROUP_CNTRL_L2_32K_FORCE_MATCH_order32k_MASK        0x20000

// Bitfield Description : 
#define L2_MULTATS_PTE_GROUP_CNTRL_Reserved_31_18_OFFSET      18
#define L2_MULTATS_PTE_GROUP_CNTRL_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            L1_INTGFX_32K_PTE_GROUP_EN:1;
    UINT32                            L1_OTHER_32K_PTE_GROUP_EN:1;
    UINT32                            PTE_GROUP_NONATS_EN:1;
    UINT32                            L1_RSP_SPLIT_REQ_PTE_GROUP:1;
    UINT32                            L2_TW_SPLIT_REQ_PTE_GROUP:1;
    UINT32                            L2_32K_FORCE_MATCH_v:1;
    UINT32                            L2_32K_FORCE_MATCH_all_host:1;
    UINT32                            L2_32K_FORCE_MATCH_4k:1;
    UINT32                            L2_32K_FORCE_MATCH_u:1;
    UINT32                            L2_32K_FORCE_MATCH_fc:1;
    UINT32                            L2_32K_FORCE_MATCH_ir:1;
    UINT32                            L2_32K_FORCE_MATCH_iw:1;
    UINT32                            L2_32K_FORCE_MATCH_res:1;
    UINT32                            L2_32K_FORCE_MATCH_all_guest:1;
    UINT32                            L2_32K_FORCE_MATCH_us:1;
    UINT32                            L2_32K_FORCE_MATCH_nx:1;
    UINT32                            L2_32K_FORCE_MATCH_giw:1;
    UINT32                            L2_32K_FORCE_MATCH_order32k:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} L2_MULTATS_PTE_GROUP_CNTRL_STRUCT;

#define SMN_L2_MULTATS_PTE_GROUP_CNTRL_ADDRESS    0x13f01310UL


/***********************************************************
* Register Name : L2_PDC_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_PDC_CONTROL_Reserved_2_0_OFFSET      0
#define L2_PDC_CONTROL_Reserved_2_0_MASK        0x7

// Bitfield Description : 0=Reads update replacement state bits when there is a simultaneous read and write to the same PDC index.
// 1=Writes update replacement state bits when there is a simultaneous read and write to the same PDC index.
#define L2_PDC_CONTROL_PDCLRUUpdatePri_OFFSET      3
#define L2_PDC_CONTROL_PDCLRUUpdatePri_MASK        0x8

// Bitfield Description : Enable parity protection of the PDC if the device supports parity.
#define L2_PDC_CONTROL_PDCParityEn_OFFSET      4
#define L2_PDC_CONTROL_PDCParityEn_MASK        0x10

// Bitfield Description : 
#define L2_PDC_CONTROL_Reserved_7_5_OFFSET      5
#define L2_PDC_CONTROL_Reserved_7_5_MASK        0xe0

// Bitfield Description : Selects the PDC invalidation algorithm.
// 00 = Invalidate the entire PDC
// 01 = Fast imprecise invalidation
// 10 = Sequential precise invalidation
// 11 = Partial sequential precise invalidation
#define L2_PDC_CONTROL_PDCInvalidationSel_OFFSET      8
#define L2_PDC_CONTROL_PDCInvalidationSel_MASK        0x300

// Bitfield Description : Software may write this register to 1 to invalidate all entries in the PDC.
#define L2_PDC_CONTROL_PDCSoftInvalidate_OFFSET      10
#define L2_PDC_CONTROL_PDCSoftInvalidate_MASK        0x400

// Bitfield Description : 
#define L2_PDC_CONTROL_Reserved_11_11_OFFSET      11
#define L2_PDC_CONTROL_Reserved_11_11_MASK        0x800

// Bitfield Description : 0=Search PDC from higher levels down.
// 1=Search PDC from lower levels up.
#define L2_PDC_CONTROL_PDCSearchDirection_OFFSET      12
#define L2_PDC_CONTROL_PDCSearchDirection_MASK        0x1000

// Bitfield Description : When set, all requests bypass the PDC. This prevents the multiple issue of requests and increases maximum rate of requests to the table-walker.
#define L2_PDC_CONTROL_PDCBypass_OFFSET      13
#define L2_PDC_CONTROL_PDCBypass_MASK        0x2000

// Bitfield Description : 
#define L2_PDC_CONTROL_Reserved_14_14_OFFSET      14
#define L2_PDC_CONTROL_Reserved_14_14_MASK        0x4000

// Bitfield Description : 0=The PDC does not support parity protection.
// 1=The PDC supports parity protection.
#define L2_PDC_CONTROL_PDCParitySupport_OFFSET      15
#define L2_PDC_CONTROL_PDCParitySupport_MASK        0x8000

// Bitfield Description : Indicates the number of ways in the PDC.
#define L2_PDC_CONTROL_PDCWays_OFFSET      16
#define L2_PDC_CONTROL_PDCWays_MASK        0xff0000

// Bitfield Description : 
#define L2_PDC_CONTROL_Reserved_27_24_OFFSET      24
#define L2_PDC_CONTROL_Reserved_27_24_MASK        0xf000000

// Bitfield Description : Indicates the number of entries in the PDC is indicated as 2^PDCEntries.
#define L2_PDC_CONTROL_PDCEntries_OFFSET      28
#define L2_PDC_CONTROL_PDCEntries_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            PDCLRUUpdatePri:1;
    UINT32                            PDCParityEn:1;
    UINT32                            Reserved_7_5:3;
    UINT32                            PDCInvalidationSel:2;
    UINT32                            PDCSoftInvalidate:1;
    UINT32                            Reserved_11_11:1;
    UINT32                            PDCSearchDirection:1;
    UINT32                            PDCBypass:1;
    UINT32                            Reserved_14_14:1;
    UINT32                            PDCParitySupport:1;
    UINT32                            PDCWays:8;
    UINT32                            Reserved_27_24:4;
    UINT32                            PDCEntries:4;
  } Field;
  UINT32 Value;
} L2_PDC_CONTROL_STRUCT;

#define SMN_L2_PDC_CONTROL_ADDRESS    0x13f01140UL


/***********************************************************
* Register Name : L2_PDC_HASH_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_PDC_HASH_CONTROL_Reserved_15_0_OFFSET      0
#define L2_PDC_HASH_CONTROL_Reserved_15_0_MASK        0xffff

// Bitfield Description : This register is a bit-wise AND mask that selects which virtual address bits are used to index into the PDC.
#define L2_PDC_HASH_CONTROL_PDCAddressMask_OFFSET      16
#define L2_PDC_HASH_CONTROL_PDCAddressMask_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            PDCAddressMask:16;
  } Field;
  UINT32 Value;
} L2_PDC_HASH_CONTROL_STRUCT;

#define SMN_L2_PDC_HASH_CONTROL_ADDRESS    0x13f01144UL


/***********************************************************
* Register Name : L2_PDC_WAY_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables a way in the PDC when set to 1. An implementation may have less than 32 ways. The entire cache may be disabled by setting the PDCWays lower bits of this register.
#define L2_PDC_WAY_CONTROL_PDCWayDisable_OFFSET      0
#define L2_PDC_WAY_CONTROL_PDCWayDisable_MASK        0xffff

// Bitfield Description : Used with PDCWayDisable to disable a way of set associativity.
#define L2_PDC_WAY_CONTROL_PDCWayAccessDisable_OFFSET      16
#define L2_PDC_WAY_CONTROL_PDCWayAccessDisable_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PDCWayDisable:16;
    UINT32                            PDCWayAccessDisable:16;
  } Field;
  UINT32 Value;
} L2_PDC_WAY_CONTROL_STRUCT;

#define SMN_L2_PDC_WAY_CONTROL_ADDRESS    0x13f01148UL


/***********************************************************
* Register Name : L2_PTC_A_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_PTC_A_CONTROL_Reserved_2_0_OFFSET      0
#define L2_PTC_A_CONTROL_Reserved_2_0_MASK        0x7

// Bitfield Description : 0=Reads update replacement state bits when there is a simultaneous read and write to the same PTCA index.
// 1=Writes update replacement state bits when there is a simultaneous read and write to the same PTCA index.
#define L2_PTC_A_CONTROL_PTCALRUUpdatePri_OFFSET      3
#define L2_PTC_A_CONTROL_PTCALRUUpdatePri_MASK        0x8

// Bitfield Description : Enable parity protection of the PTC A sub-cache.
#define L2_PTC_A_CONTROL_PTCAParityEn_OFFSET      4
#define L2_PTC_A_CONTROL_PTCAParityEn_MASK        0x10

// Bitfield Description : 
#define L2_PTC_A_CONTROL_Reserved_7_5_OFFSET      5
#define L2_PTC_A_CONTROL_Reserved_7_5_MASK        0xe0

// Bitfield Description : Selects the PTC A sub-cache invalidation algorithm.
// 00=Invalidate the entire PTC A sub-cache
// 01=Fast imprecise invalidation
// 10=Sequential precise invalidation
// 11=Partial sequential precise invalidation.
#define L2_PTC_A_CONTROL_PTCAInvalidationSel_OFFSET      8
#define L2_PTC_A_CONTROL_PTCAInvalidationSel_MASK        0x300

// Bitfield Description : Software may write this register to 1 to invalidate all entries in the PTC A sub-cache.
#define L2_PTC_A_CONTROL_PTCASoftInvalidate_OFFSET      10
#define L2_PTC_A_CONTROL_PTCASoftInvalidate_MASK        0x400

// Bitfield Description : When set, the PTC A sub-cache stores 2M pages instead of 4K pages.
#define L2_PTC_A_CONTROL_PTCA2MMode_OFFSET      11
#define L2_PTC_A_CONTROL_PTCA2MMode_MASK        0x800

// Bitfield Description : 
#define L2_PTC_A_CONTROL_Reserved_12_12_OFFSET      12
#define L2_PTC_A_CONTROL_Reserved_12_12_MASK        0x1000

// Bitfield Description : When set, all requests bypass the PTC A sub-cache.
#define L2_PTC_A_CONTROL_PTCABypass_OFFSET      13
#define L2_PTC_A_CONTROL_PTCABypass_MASK        0x2000

// Bitfield Description : 
#define L2_PTC_A_CONTROL_Reserved_14_14_OFFSET      14
#define L2_PTC_A_CONTROL_Reserved_14_14_MASK        0x4000

// Bitfield Description : 0=The PTC A sub-cache does not support parity protection.
// 1=The PTC A sub-cache supports parity protection.
#define L2_PTC_A_CONTROL_PTCAParitySupport_OFFSET      15
#define L2_PTC_A_CONTROL_PTCAParitySupport_MASK        0x8000

// Bitfield Description : Indicates the number of ways in the PTC A sub-cache.
#define L2_PTC_A_CONTROL_PTCAWays_OFFSET      16
#define L2_PTC_A_CONTROL_PTCAWays_MASK        0xff0000

// Bitfield Description : 
#define L2_PTC_A_CONTROL_Reserved_27_24_OFFSET      24
#define L2_PTC_A_CONTROL_Reserved_27_24_MASK        0xf000000

// Bitfield Description : The number of entries in the PTC A sub-cache is indicated as 2^PTCAEntries.
#define L2_PTC_A_CONTROL_PTCAEntries_OFFSET      28
#define L2_PTC_A_CONTROL_PTCAEntries_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Reserved_2_0:3;
    UINT32                            PTCALRUUpdatePri:1;
    UINT32                            PTCAParityEn:1;
    UINT32                            Reserved_7_5:3;
    UINT32                            PTCAInvalidationSel:2;
    UINT32                            PTCASoftInvalidate:1;
    UINT32                            PTCA2MMode:1;
    UINT32                            Reserved_12_12:1;
    UINT32                            PTCABypass:1;
    UINT32                            Reserved_14_14:1;
    UINT32                            PTCAParitySupport:1;
    UINT32                            PTCAWays:8;
    UINT32                            Reserved_27_24:4;
    UINT32                            PTCAEntries:4;
  } Field;
  UINT32 Value;
} L2_PTC_A_CONTROL_STRUCT;

#define SMN_L2_PTC_A_CONTROL_ADDRESS    0x15700060UL


/***********************************************************
* Register Name : L2_PTC_A_HASH_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_PTC_A_HASH_CONTROL_Reserved_15_0_OFFSET      0
#define L2_PTC_A_HASH_CONTROL_Reserved_15_0_MASK        0xffff

// Bitfield Description : This register is a bit-wise AND mask that selects which virtual address bits are used to index into the PTC A sub-cache.
#define L2_PTC_A_HASH_CONTROL_PTCAAddressMask_OFFSET      16
#define L2_PTC_A_HASH_CONTROL_PTCAAddressMask_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_15_0:16;
    UINT32                            PTCAAddressMask:16;
  } Field;
  UINT32 Value;
} L2_PTC_A_HASH_CONTROL_STRUCT;

#define SMN_L2_PTC_A_HASH_CONTROL_ADDRESS    0x15700064UL


/***********************************************************
* Register Name : L2_PTC_A_WAY_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Each bit in this register disables a way in the PTC A sub-cache when set to 1. An implementation may have less than 32 ways. The entire cache may be disabled by setting the PTCAWays lower bits of this register.
#define L2_PTC_A_WAY_CONTROL_PTCAWayDisable_OFFSET      0
#define L2_PTC_A_WAY_CONTROL_PTCAWayDisable_MASK        0xffff

// Bitfield Description : Used with PTCWayDisable to disable a way of set associativity.
#define L2_PTC_A_WAY_CONTROL_PTCAWayAccessDisable_OFFSET      16
#define L2_PTC_A_WAY_CONTROL_PTCAWayAccessDisable_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PTCAWayDisable:16;
    UINT32                            PTCAWayAccessDisable:16;
  } Field;
  UINT32 Value;
} L2_PTC_A_WAY_CONTROL_STRUCT;

#define SMN_L2_PTC_A_WAY_CONTROL_ADDRESS    0x15700068UL


/***********************************************************
* Register Name : L2_PWRGATE_CNTRL_REG_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Idle threshold before entering power gating.
#define L2_PWRGATE_CNTRL_REG_0_IP_PG_thres_OFFSET      0
#define L2_PWRGATE_CNTRL_REG_0_IP_PG_thres_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IP_PG_thres:32;
  } Field;
  UINT32 Value;
} L2_PWRGATE_CNTRL_REG_0_STRUCT;

#define SMN_L2_PWRGATE_CNTRL_REG_0_ADDRESS    0x157000f8UL


/***********************************************************
* Register Name : L2_PWRGATE_CNTRL_REG_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1 = Enabled L2 power gating.
#define L2_PWRGATE_CNTRL_REG_3_IP_PG_en_OFFSET      0
#define L2_PWRGATE_CNTRL_REG_3_IP_PG_en_MASK        0x1

// Bitfield Description : High entering or leaving PG.
#define L2_PWRGATE_CNTRL_REG_3_IP_PG_busy_OFFSET      1
#define L2_PWRGATE_CNTRL_REG_3_IP_PG_busy_MASK        0x2

// Bitfield Description : SW readable register for finding the PG status of the L2, 0 - not PG, 1 - power-gated
#define L2_PWRGATE_CNTRL_REG_3_L2_PG_STATUS_OFFSET      2
#define L2_PWRGATE_CNTRL_REG_3_L2_PG_STATUS_MASK        0x4

// Bitfield Description : <RW> 2'x1: enable sending the pwrmgr_int for FW interlock feature, 2'b1x: Enable wakup tile from power gated state using the pwrmgt_int_clear signal
#define L2_PWRGATE_CNTRL_REG_3_CFG_FW_PG_EXIT_EN_OFFSET      3
#define L2_PWRGATE_CNTRL_REG_3_CFG_FW_PG_EXIT_EN_MASK        0x18

// Bitfield Description : 1=Allow an active APC in L2B to gate DPG for all of L2. 0=L2 can enter DPG while APC is active.
#define L2_PWRGATE_CNTRL_REG_3_IP_APC_GATE_DPG_OFFSET      5
#define L2_PWRGATE_CNTRL_REG_3_IP_APC_GATE_DPG_MASK        0x20

// Bitfield Description : 
#define L2_PWRGATE_CNTRL_REG_3_Reserved_31_6_OFFSET      6
#define L2_PWRGATE_CNTRL_REG_3_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            IP_PG_en:1;
    UINT32                            IP_PG_busy:1;
    UINT32                            L2_PG_STATUS:1;
    UINT32                            CFG_FW_PG_EXIT_EN:2;
    UINT32                            IP_APC_GATE_DPG:1;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} L2_PWRGATE_CNTRL_REG_3_STRUCT;

#define SMN_L2_PWRGATE_CNTRL_REG_3_ADDRESS    0x15700104UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_DEBUG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1=iommu will filter all invalidations to riommu dcn clients.
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_OFFSET      0
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_MASK        0x1

// Bitfield Description : 1=iommu will filter INVALIDATE_DEVTAB_ENTRY cmds to riommu dcn clients.
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVDEV_OFFSET      1
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVDEV_MASK        0x2

// Bitfield Description : 1=iommu will filter all INVALIDATE_IOMMU_PAGES to riommu dcn clients.
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVPAGE_OFFSET      2
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVPAGE_MASK        0x4

// Bitfield Description : 1=iommu will filter all INVALIDATE_IOMMU_ALL to riommu dcn clients.
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVALL_OFFSET      3
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_INVALL_MASK        0x8

// Bitfield Description : 1=iommu will filter all COMPLETION_WAIT to riommu dcn clients.
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_WAIT_OFFSET      4
#define L2_RIOMMUDCN_DEBUG_DISABLE_RIOMMUDCN_CMDS_WAIT_MASK        0x10

// Bitfield Description : 1=iommu will always forward invalidate_iommu_pages commands to riommu dcn clients. 0=iommu will filter invalidate_iommu_pages commands when GN=1. This value is meaningful when DISABLE_RIOMMUDCN_CMDS=0.
#define L2_RIOMMUDCN_DEBUG_ALLOW_RIOMMUDCN_INVPAGE_GN_OFFSET      5
#define L2_RIOMMUDCN_DEBUG_ALLOW_RIOMMUDCN_INVPAGE_GN_MASK        0x20

// Bitfield Description : 1=iommu will always forward completion wait commands to riommu dcn clients. 0=iommu will filter back to back completion waits. This value is meaningful when DISABLE_RIOMMUDCN_CMDS=0.
#define L2_RIOMMUDCN_DEBUG_ALLOW_RIOMMUDCN_CMPLWAIT_B2B_OFFSET      6
#define L2_RIOMMUDCN_DEBUG_ALLOW_RIOMMUDCN_CMPLWAIT_B2B_MASK        0x40

// Bitfield Description : 
#define L2_RIOMMUDCN_DEBUG_Reserved_31_7_OFFSET      7
#define L2_RIOMMUDCN_DEBUG_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            DISABLE_RIOMMUDCN_CMDS:1;
    UINT32                            DISABLE_RIOMMUDCN_CMDS_INVDEV:1;
    UINT32                            DISABLE_RIOMMUDCN_CMDS_INVPAGE:1;
    UINT32                            DISABLE_RIOMMUDCN_CMDS_INVALL:1;
    UINT32                            DISABLE_RIOMMUDCN_CMDS_WAIT:1;
    UINT32                            ALLOW_RIOMMUDCN_INVPAGE_GN:1;
    UINT32                            ALLOW_RIOMMUDCN_CMPLWAIT_B2B:1;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_DEBUG_STRUCT;

#define SMN_L2_RIOMMUDCN_DEBUG_ADDRESS    0x13f01308UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_FAULTNOTIF_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 0 forwarding.
#define L2_RIOMMUDCN_FAULTNOTIF_0_RIOMMUDCN_EVENTLOG_DATAWORD_0_OFFSET      0
#define L2_RIOMMUDCN_FAULTNOTIF_0_RIOMMUDCN_EVENTLOG_DATAWORD_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUDCN_EVENTLOG_DATAWORD_0:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_FAULTNOTIF_0_STRUCT;

#define SMN_L2_RIOMMUDCN_FAULTNOTIF_0_ADDRESS    0x13f012d8UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_FAULTNOTIF_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 1 forwarding.
#define L2_RIOMMUDCN_FAULTNOTIF_1_RIOMMUDCN_EVENTLOG_DATAWORD_1_OFFSET      0
#define L2_RIOMMUDCN_FAULTNOTIF_1_RIOMMUDCN_EVENTLOG_DATAWORD_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUDCN_EVENTLOG_DATAWORD_1:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_FAULTNOTIF_1_STRUCT;

#define SMN_L2_RIOMMUDCN_FAULTNOTIF_1_ADDRESS    0x13f012dcUL


/***********************************************************
* Register Name : L2_RIOMMUDCN_FAULTNOTIF_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 2 forwarding.
#define L2_RIOMMUDCN_FAULTNOTIF_2_RIOMMUDCN_EVENTLOG_DATAWORD_2_OFFSET      0
#define L2_RIOMMUDCN_FAULTNOTIF_2_RIOMMUDCN_EVENTLOG_DATAWORD_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUDCN_EVENTLOG_DATAWORD_2:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_FAULTNOTIF_2_STRUCT;

#define SMN_L2_RIOMMUDCN_FAULTNOTIF_2_ADDRESS    0x13f012e0UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_FAULTNOTIF_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 3 forwarding.
#define L2_RIOMMUDCN_FAULTNOTIF_3_RIOMMUDCN_EVENTLOG_DATAWORD_3_OFFSET      0
#define L2_RIOMMUDCN_FAULTNOTIF_3_RIOMMUDCN_EVENTLOG_DATAWORD_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUDCN_EVENTLOG_DATAWORD_3:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_FAULTNOTIF_3_STRUCT;

#define SMN_L2_RIOMMUDCN_FAULTNOTIF_3_ADDRESS    0x13f012e4UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_INVACK_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 2'b00=Invalidation was successful. 2'b01=Invalidation was unsuccessful. 2'b1x=Reserved.
#define L2_RIOMMUDCN_INVACK_0_RIOMMUDCN_INVALIDATION_STATUS_OFFSET      0
#define L2_RIOMMUDCN_INVACK_0_RIOMMUDCN_INVALIDATION_STATUS_MASK        0x3

// Bitfield Description : Bits 29 to 0 of invalidation ack itag mask. Indicates which outstanding invalidations are being acknowledged.
#define L2_RIOMMUDCN_INVACK_0_RIOMMUDCN_INVALIDATION_ITAG_29_0_OFFSET      2
#define L2_RIOMMUDCN_INVACK_0_RIOMMUDCN_INVALIDATION_ITAG_29_0_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            RIOMMUDCN_INVALIDATION_STATUS:2;
    UINT32                            RIOMMUDCN_INVALIDATION_ITAG_29_0:30;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_INVACK_0_STRUCT;

#define SMN_L2_RIOMMUDCN_INVACK_0_ADDRESS    0x13f012d0UL


/***********************************************************
* Register Name : L2_RIOMMUDCN_INVACK_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_RIOMMUDCN_INVACK_1_Reserved_1_0_OFFSET      0
#define L2_RIOMMUDCN_INVACK_1_Reserved_1_0_MASK        0x3

// Bitfield Description : Bits 31 to 30 of invalidation ack itag mask. Indicates which outstanding invalidations are being acknowledged.
#define L2_RIOMMUDCN_INVACK_1_RIOMMUDCN_INVALIDATION_ITAG_31_30_OFFSET      2
#define L2_RIOMMUDCN_INVACK_1_RIOMMUDCN_INVALIDATION_ITAG_31_30_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            RIOMMUDCN_INVALIDATION_ITAG_31_30:30;
  } Field;
  UINT32 Value;
} L2_RIOMMUDCN_INVACK_1_STRUCT;

#define SMN_L2_RIOMMUDCN_INVACK_1_ADDRESS    0x13f012d4UL


/***********************************************************
* Register Name : L2_RIOMMUISP_FAULTNOTIF_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 0 forwarding.
#define L2_RIOMMUISP_FAULTNOTIF_0_RIOMMUISP_EVENTLOG_DATAWORD_0_OFFSET      0
#define L2_RIOMMUISP_FAULTNOTIF_0_RIOMMUISP_EVENTLOG_DATAWORD_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUISP_EVENTLOG_DATAWORD_0:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_FAULTNOTIF_0_STRUCT;

#define SMN_L2_RIOMMUISP_FAULTNOTIF_0_ADDRESS    0x13f012c0UL


/***********************************************************
* Register Name : L2_RIOMMUISP_FAULTNOTIF_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 1 forwarding.
#define L2_RIOMMUISP_FAULTNOTIF_1_RIOMMUISP_EVENTLOG_DATAWORD_1_OFFSET      0
#define L2_RIOMMUISP_FAULTNOTIF_1_RIOMMUISP_EVENTLOG_DATAWORD_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUISP_EVENTLOG_DATAWORD_1:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_FAULTNOTIF_1_STRUCT;

#define SMN_L2_RIOMMUISP_FAULTNOTIF_1_ADDRESS    0x13f012c4UL


/***********************************************************
* Register Name : L2_RIOMMUISP_FAULTNOTIF_2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 2 forwarding.
#define L2_RIOMMUISP_FAULTNOTIF_2_RIOMMUISP_EVENTLOG_DATAWORD_2_OFFSET      0
#define L2_RIOMMUISP_FAULTNOTIF_2_RIOMMUISP_EVENTLOG_DATAWORD_2_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUISP_EVENTLOG_DATAWORD_2:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_FAULTNOTIF_2_STRUCT;

#define SMN_L2_RIOMMUISP_FAULTNOTIF_2_ADDRESS    0x13f012c8UL


/***********************************************************
* Register Name : L2_RIOMMUISP_FAULTNOTIF_3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Write will trigger riommu to main IOMMU eventlog dataword 3 forwarding.
#define L2_RIOMMUISP_FAULTNOTIF_3_RIOMMUISP_EVENTLOG_DATAWORD_3_OFFSET      0
#define L2_RIOMMUISP_FAULTNOTIF_3_RIOMMUISP_EVENTLOG_DATAWORD_3_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIOMMUISP_EVENTLOG_DATAWORD_3:32;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_FAULTNOTIF_3_STRUCT;

#define SMN_L2_RIOMMUISP_FAULTNOTIF_3_ADDRESS    0x13f012ccUL


/***********************************************************
* Register Name : L2_RIOMMUISP_INVACK_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 2'b00=Invalidation was successful. 2'b01=Invalidation was unsuccessful. 2'b1x=Reserved.
#define L2_RIOMMUISP_INVACK_0_RIOMMUISP_INVALIDATION_STATUS_OFFSET      0
#define L2_RIOMMUISP_INVACK_0_RIOMMUISP_INVALIDATION_STATUS_MASK        0x3

// Bitfield Description : Bits 29 to 0 of invalidation ack itag mask. Indicates which outstanding invalidations are being acknowledged.
#define L2_RIOMMUISP_INVACK_0_RIOMMUISP_INVALIDATION_ITAG_29_0_OFFSET      2
#define L2_RIOMMUISP_INVACK_0_RIOMMUISP_INVALIDATION_ITAG_29_0_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            RIOMMUISP_INVALIDATION_STATUS:2;
    UINT32                            RIOMMUISP_INVALIDATION_ITAG_29_0:30;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_INVACK_0_STRUCT;

#define SMN_L2_RIOMMUISP_INVACK_0_ADDRESS    0x13f012b8UL


/***********************************************************
* Register Name : L2_RIOMMUISP_INVACK_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_RIOMMUISP_INVACK_1_Reserved_1_0_OFFSET      0
#define L2_RIOMMUISP_INVACK_1_Reserved_1_0_MASK        0x3

// Bitfield Description : Bits 31 to 30 of invalidation ack itag mask. Indicates which outstanding invalidations are being acknowledged.
#define L2_RIOMMUISP_INVACK_1_RIOMMUISP_INVALIDATION_ITAG_31_30_OFFSET      2
#define L2_RIOMMUISP_INVACK_1_RIOMMUISP_INVALIDATION_ITAG_31_30_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            RIOMMUISP_INVALIDATION_ITAG_31_30:30;
  } Field;
  UINT32 Value;
} L2_RIOMMUISP_INVACK_1_STRUCT;

#define SMN_L2_RIOMMUISP_INVACK_1_ADDRESS    0x13f012bcUL


/***********************************************************
* Register Name : L2_RIOMMU_INV_CNTRL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_RIOMMU_INV_CNTRL_Reserved_0_0_OFFSET      0
#define L2_RIOMMU_INV_CNTRL_Reserved_0_0_MASK        0x1

// Bitfield Description : This 5-bit value sets the limit for number of outstanding invalidations to each riommu client that iommu will manage. Actual limit is value + 1.
#define L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INV_LIM_OFFSET      1
#define L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INV_LIM_MASK        0x3e

// Bitfield Description : This 5-bit value sets the limit for number invalidation acks that each riommu client will transmit to main iommu in a single AXI transaction. Actual limit is value + 1.
#define L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INVACK_LIM_OFFSET      6
#define L2_RIOMMU_INV_CNTRL_RIOMMU_MULTI_INVACK_LIM_MASK        0x7c0

// Bitfield Description : 
#define L2_RIOMMU_INV_CNTRL_Reserved_31_11_OFFSET      11
#define L2_RIOMMU_INV_CNTRL_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            RIOMMU_MULTI_INV_LIM:5;
    UINT32                            RIOMMU_MULTI_INVACK_LIM:5;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} L2_RIOMMU_INV_CNTRL_STRUCT;

#define SMN_L2_RIOMMU_INV_CNTRL_ADDRESS    0x13f0130cUL


/***********************************************************
* Register Name : L2_RT_CONTROL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of slots available for RT traffic inside shared RT/nonRT fifo inside IFIFO. Max slots=fifo_depth=10.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios
#define L2_RT_CONTROL_0_IFIFO_RT_credits_OFFSET      0
#define L2_RT_CONTROL_0_IFIFO_RT_credits_MASK        0xf

// Bitfield Description : Chang the RT/nonRT arbitration to round robin by setting this register to 1. Default is RT always gets higher priority.
#define L2_RT_CONTROL_0_RT_Pri_RR_OFFSET      4
#define L2_RT_CONTROL_0_RT_Pri_RR_MASK        0x10

// Bitfield Description : QUEUE RT credit override value. Should not be more that QUEUEOverride register value.
#define L2_RT_CONTROL_0_QUEUE_RT_Credits_OFFSET      5
#define L2_RT_CONTROL_0_QUEUE_RT_Credits_MASK        0x7e0

// Bitfield Description : Changing this register from 0 to 1 overrides the QUEUE RT credit counter with QUEUE_RT_Credits. This should only be performed when the IOMMU is idle
#define L2_RT_CONTROL_0_QUEUE_RT_Override_OFFSET      11
#define L2_RT_CONTROL_0_QUEUE_RT_Override_MASK        0x800

// Bitfield Description : FLTCMB RT credit override value. Should not be more than FLTCMBOverride register value
#define L2_RT_CONTROL_0_FLTCMB_RT_Credits_OFFSET      12
#define L2_RT_CONTROL_0_FLTCMB_RT_Credits_MASK        0x3f000

// Bitfield Description : Changing this register from 0 to 1 overrides the FLTCMB RT credit counter with FLTCMB_RT_Credits. This should only be performed when the IOMMU is idle
#define L2_RT_CONTROL_0_FLTCMB_RT_Override_OFFSET      18
#define L2_RT_CONTROL_0_FLTCMB_RT_Override_MASK        0x40000

// Bitfield Description : 
#define L2_RT_CONTROL_0_Reserved_23_19_OFFSET      19
#define L2_RT_CONTROL_0_Reserved_23_19_MASK        0xf80000

// Bitfield Description : Number of slots available for RT traffic coming from TW inside Fault combineer.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios.Should be programmed to same value as PDTIE_RT_Credits in L2B index space
#define L2_RT_CONTROL_0_PDTIE_RT_Credits_OFFSET      24
#define L2_RT_CONTROL_0_PDTIE_RT_Credits_MASK        0xf000000

// Bitfield Description : 
#define L2_RT_CONTROL_0_Reserved_31_28_OFFSET      28
#define L2_RT_CONTROL_0_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            IFIFO_RT_credits:4;
    UINT32                            RT_Pri_RR:1;
    UINT32                            QUEUE_RT_Credits:6;
    UINT32                            QUEUE_RT_Override:1;
    UINT32                            FLTCMB_RT_Credits:6;
    UINT32                            FLTCMB_RT_Override:1;
    UINT32                            Reserved_23_19:5;
    UINT32                            PDTIE_RT_Credits:4;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} L2_RT_CONTROL_0_STRUCT;

#define SMN_L2_RT_CONTROL_0_ADDRESS    0x15700070UL


/***********************************************************
* Register Name : L2_RT_CONTROL_CTRL_0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Chang the RT/nonRT arbitration to round robin by setting this register to 1. Default is RT always gets higher priority.
#define L2_RT_CONTROL_CTRL_0_RT_Pri_RR_OFFSET      0
#define L2_RT_CONTROL_CTRL_0_RT_Pri_RR_MASK        0x1

// Bitfield Description : Number of slots available for RT traffic inside PDC latency fifo in QUEUE.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios
#define L2_RT_CONTROL_CTRL_0_FC3RTCredits_OFFSET      1
#define L2_RT_CONTROL_CTRL_0_FC3RTCredits_MASK        0x3e

// Bitfield Description : Number of slots available for RT traffic coming from L2 SIDE fifo in QUEUE.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios.Should be programmed to same value as QUEUE_RT_Credits register in l2a index space
#define L2_RT_CONTROL_CTRL_0_QUEUE_RT_Credits_OFFSET      6
#define L2_RT_CONTROL_CTRL_0_QUEUE_RT_Credits_MASK        0x7c0

// Bitfield Description : Number of credits reserved for RT traffic for PDTIE credits.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios. Shuld be programmed to same value as PDTIE_RT_Credits register in L2A indx space
#define L2_RT_CONTROL_CTRL_0_PDTIE_RT_Credits_OFFSET      11
#define L2_RT_CONTROL_CTRL_0_PDTIE_RT_Credits_MASK        0x7800

// Bitfield Description : Changing this register from 0 to 1 overrides the RT FC1 credit counter with FC1_RT_Credits. This should only be performed when the IOMMU is idle.
#define L2_RT_CONTROL_CTRL_0_FC1_RT_Override_OFFSET      15
#define L2_RT_CONTROL_CTRL_0_FC1_RT_Override_MASK        0x8000

// Bitfield Description : Number of credits reserved for RT traffic for FC1 credits.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios.
#define L2_RT_CONTROL_CTRL_0_FC1_RT_Credits_OFFSET      16
#define L2_RT_CONTROL_CTRL_0_FC1_RT_Credits_MASK        0x7f0000

// Bitfield Description : 
#define L2_RT_CONTROL_CTRL_0_Reserved_23_23_OFFSET      23
#define L2_RT_CONTROL_CTRL_0_Reserved_23_23_MASK        0x800000

// Bitfield Description : Number of credits reserved for RT traffic for TW_L2QUEUE Feedback fifo.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios.
#define L2_RT_CONTROL_CTRL_0_FEEDBACK_RT_Credits_OFFSET      24
#define L2_RT_CONTROL_CTRL_0_FEEDBACK_RT_Credits_MASK        0xff000000

typedef union {
  struct {
    UINT32                            RT_Pri_RR:1;
    UINT32                            FC3RTCredits:5;
    UINT32                            QUEUE_RT_Credits:5;
    UINT32                            PDTIE_RT_Credits:4;
    UINT32                            FC1_RT_Override:1;
    UINT32                            FC1_RT_Credits:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            FEEDBACK_RT_Credits:8;
  } Field;
  UINT32 Value;
} L2_RT_CONTROL_CTRL_0_STRUCT;

#define SMN_L2_RT_CONTROL_CTRL_0_ADDRESS    0x13f012a8UL


/***********************************************************
* Register Name : L2_RT_CONTROL_CTRL_1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of credits reserved for RT traffic inside table walker.This register should only be set when IOMMU Idle. IOMMU behaviour is undefined for non-Idle scenarios.
#define L2_RT_CONTROL_CTRL_1_TW_RT_Credits_OFFSET      0
#define L2_RT_CONTROL_CTRL_1_TW_RT_Credits_MASK        0x7f

// Bitfield Description : 
#define L2_RT_CONTROL_CTRL_1_Reserved_31_7_OFFSET      7
#define L2_RT_CONTROL_CTRL_1_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            TW_RT_Credits:7;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} L2_RT_CONTROL_CTRL_1_STRUCT;

#define SMN_L2_RT_CONTROL_CTRL_1_ADDRESS    0x13f012acUL


/***********************************************************
* Register Name : L2_SB_LOCATION
* Register Description :
* Specifies which PCIE bridge is connected to the SB.  Register set to 32'b0 indicates the GNB is secondary and has no SB connected.
* Visibility : 0x2
************************************************************/

// Bitfield Description : One hot encoding:
// 0x1 - SB is located on port A of SBlocated_Core
// 0x2 - SB is located on port B of SBlocated_Core
// 0x4 - SB is located on port C of SBlocated_Core
// 0x8 - SB is located on port D of SBlocated_Core
// All other encodings are reserved.
#define L2_SB_LOCATION_SBlocated_Port_OFFSET      0
#define L2_SB_LOCATION_SBlocated_Port_MASK        0xffff

// Bitfield Description : One hot encoding:
// 0x1 - SB is under GPP0
// 0x2 - SB is under GPP1
// 0x4 - SB is under GPP2
// All other encodings are reserved.
#define L2_SB_LOCATION_SBlocated_Core_OFFSET      16
#define L2_SB_LOCATION_SBlocated_Core_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SBlocated_Port:16;
    UINT32                            SBlocated_Core:16;
  } Field;
  UINT32 Value;
} L2_SB_LOCATION_STRUCT;

#define SMN_L2_SB_LOCATION_ADDRESS    0x13f0112cUL


/***********************************************************
* Register Name : L2_TW_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define L2_TW_CONTROL_Reserved_5_0_OFFSET      0
#define L2_TW_CONTROL_Reserved_5_0_MASK        0x3f

// Bitfield Description : 1=Table-walker always genereates coherent requests. The DTE SD bit is ignored when this bit is set to 1.
#define L2_TW_CONTROL_TWForceCoherent_OFFSET      6
#define L2_TW_CONTROL_TWForceCoherent_MASK        0x40

// Bitfield Description : 
#define L2_TW_CONTROL_Reserved_7_7_OFFSET      7
#define L2_TW_CONTROL_Reserved_7_7_MASK        0x80

// Bitfield Description : Enable HW host prefetching in the table-walker.
#define L2_TW_CONTROL_TWPrefetchEn_OFFSET      8
#define L2_TW_CONTROL_TWPrefetchEn_MASK        0x100

// Bitfield Description : 1=Allow non-4K pages to be prefetched.
// 0=Only 4K pages are prefetched.
#define L2_TW_CONTROL_TWPrefetchOnly4KDis_OFFSET      9
#define L2_TW_CONTROL_TWPrefetchOnly4KDis_MASK        0x200

// Bitfield Description : 0=Table walker returns DTE to L1 on an untranslated exclusion range access.
// 1=Table walker returns PTE to L1 on an untranslated exclusion range access.
#define L2_TW_CONTROL_TWPTEOnUntransExcl_OFFSET      10
#define L2_TW_CONTROL_TWPTEOnUntransExcl_MASK        0x400

// Bitfield Description : 0=Table walker returns DTE to L1 on an address translation exclusion range access.
// 1=Table walker returns PTE to L1 on an address translation exclusion range access.
#define L2_TW_CONTROL_TWPTEOnAddrTransExcl_OFFSET      11
#define L2_TW_CONTROL_TWPTEOnAddrTransExcl_MASK        0x800

// Bitfield Description : Selects the number of pages to prefetch.
#define L2_TW_CONTROL_TWPrefetchRange_OFFSET      12
#define L2_TW_CONTROL_TWPrefetchRange_MASK        0x7000

// Bitfield Description : 
#define L2_TW_CONTROL_Reserved_15_15_OFFSET      15
#define L2_TW_CONTROL_Reserved_15_15_MASK        0x8000

// Bitfield Description : Disables filtering within Table Walker of memory read requests to same address.
#define L2_TW_CONTROL_TWFilter_Dis_OFFSET      16
#define L2_TW_CONTROL_TWFilter_Dis_MASK        0x10000

// Bitfield Description : 1=Table Walker memory reads are 16B. 0=Table Walker memory reads are 64B.
#define L2_TW_CONTROL_TWFilter_64B_Dis_OFFSET      17
#define L2_TW_CONTROL_TWFilter_64B_Dis_MASK        0x20000

// Bitfield Description : 0=Continue walking tables on a write permission error.
// 1=Stop walking on a write permission error.
#define L2_TW_CONTROL_TWContWalkOnPErrDis_OFFSET      18
#define L2_TW_CONTROL_TWContWalkOnPErrDis_MASK        0x40000

// Bitfield Description : Bit is for ARM mode. Setting this bit disables updating Access bit in ARM PTE.
#define L2_TW_CONTROL_TWSetAccessBit_Dis_OFFSET      19
#define L2_TW_CONTROL_TWSetAccessBit_Dis_MASK        0x80000

// Bitfield Description : Bit is for ARM mode only. Setting this register to zero will enable clearing AP bit in ARM PTE when DBM bit is found to be 1 and AP bit is not clear and TWSetAccessBit_Dis=0.
#define L2_TW_CONTROL_TWClearAPBit_Dis_OFFSET      20
#define L2_TW_CONTROL_TWClearAPBit_Dis_MASK        0x100000

// Bitfield Description : 
#define L2_TW_CONTROL_Reserved_24_21_OFFSET      21
#define L2_TW_CONTROL_Reserved_24_21_MASK        0x1e00000

// Bitfield Description : 1-Enables storing Nested PTEs (2MB and larger) in PDC. 0-Disable storing Nested PTEs in PDC (default)
#define L2_TW_CONTROL_TWCacheNestedPTE_OFFSET      25
#define L2_TW_CONTROL_TWCacheNestedPTE_MASK        0x2000000

// Bitfield Description : 
#define L2_TW_CONTROL_Reserved_31_26_OFFSET      26
#define L2_TW_CONTROL_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            Reserved_5_0:6;
    UINT32                            TWForceCoherent:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            TWPrefetchEn:1;
    UINT32                            TWPrefetchOnly4KDis:1;
    UINT32                            TWPTEOnUntransExcl:1;
    UINT32                            TWPTEOnAddrTransExcl:1;
    UINT32                            TWPrefetchRange:3;
    UINT32                            Reserved_15_15:1;
    UINT32                            TWFilter_Dis:1;
    UINT32                            TWFilter_64B_Dis:1;
    UINT32                            TWContWalkOnPErrDis:1;
    UINT32                            TWSetAccessBit_Dis:1;
    UINT32                            TWClearAPBit_Dis:1;
    UINT32                            Reserved_24_21:4;
    UINT32                            TWCacheNestedPTE:1;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} L2_TW_CONTROL_STRUCT;

#define SMN_L2_TW_CONTROL_ADDRESS    0x13f01150UL


/***********************************************************
* Register Name : L2_WQ_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : This value + 1 configures the number of WQs enabled in the GST CMD Fetch Processor.
#define L2_WQ_CONTROL_REG_GSTCMD_Num_WQs_m1_OFFSET      0
#define L2_WQ_CONTROL_REG_GSTCMD_Num_WQs_m1_MASK        0x7

// Bitfield Description : 
#define L2_WQ_CONTROL_Reserved_7_3_OFFSET      3
#define L2_WQ_CONTROL_Reserved_7_3_MASK        0xf8

// Bitfield Description : 1=Disables all but one WQ in the PPRLOGGER. 0=All PPRLOGGER WQs are enabled.
#define L2_WQ_CONTROL_REG_PPR_One_WQ_OFFSET      8
#define L2_WQ_CONTROL_REG_PPR_One_WQ_MASK        0x100

// Bitfield Description : 
#define L2_WQ_CONTROL_Reserved_31_9_OFFSET      9
#define L2_WQ_CONTROL_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            REG_GSTCMD_Num_WQs_m1:3;
    UINT32                            Reserved_7_3:5;
    UINT32                            REG_PPR_One_WQ:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} L2_WQ_CONTROL_STRUCT;

#define SMN_L2_WQ_CONTROL_ADDRESS    0x13f012b4UL


/***********************************************************
* Register Name : L2_Z10_DEBUG
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 0 = IP will wait until PG handshake is complete before asserting IP_ZSC_idle. 1 = IP will assert IP_ZSC_idle regardless of PG STATE.
#define L2_Z10_DEBUG_Z10_PG_DELAYS_ENTRY_dis_OFFSET      0
#define L2_Z10_DEBUG_Z10_PG_DELAYS_ENTRY_dis_MASK        0x1

// Bitfield Description : 
#define L2_Z10_DEBUG_Reserved_31_1_OFFSET      1
#define L2_Z10_DEBUG_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            Z10_PG_DELAYS_ENTRY_dis:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} L2_Z10_DEBUG_STRUCT;

#define SMN_L2_Z10_DEBUG_ADDRESS    0x15700114UL


/***********************************************************
* Register Name : L2_Z10_HYSTERESIS
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Debug hysteresis value for Busy to Idle transition of ZSC Handshake FSM.
#define L2_Z10_HYSTERESIS_ZSC_FSM_Busy_Idle_Hysteresis_OFFSET      0
#define L2_Z10_HYSTERESIS_ZSC_FSM_Busy_Idle_Hysteresis_MASK        0xfff

// Bitfield Description : Debug hysteresis value for Preack to Ack transition of ZSC Handshake FSM.
#define L2_Z10_HYSTERESIS_ZSC_FSM_Preack_Ack_Hysteresis_OFFSET      12
#define L2_Z10_HYSTERESIS_ZSC_FSM_Preack_Ack_Hysteresis_MASK        0xfff000

// Bitfield Description : 
#define L2_Z10_HYSTERESIS_Reserved_31_24_OFFSET      24
#define L2_Z10_HYSTERESIS_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            ZSC_FSM_Busy_Idle_Hysteresis:12;
    UINT32                            ZSC_FSM_Preack_Ack_Hysteresis:12;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} L2_Z10_HYSTERESIS_STRUCT;

#define SMN_L2_Z10_HYSTERESIS_ADDRESS    0x15700130UL


/***********************************************************
* Register Name : L2_Z10_MASK
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Debug mask enable for forcing ZSC interface bits.
#define L2_Z10_MASK_ZSC_Mask_En_OFFSET      0
#define L2_Z10_MASK_ZSC_Mask_En_MASK        0xff

// Bitfield Description : Debug mask value for forcing ZSC interface bits.
#define L2_Z10_MASK_ZSC_Mask_Val_OFFSET      8
#define L2_Z10_MASK_ZSC_Mask_Val_MASK        0xff00

// Bitfield Description : 
#define L2_Z10_MASK_Reserved_31_16_OFFSET      16
#define L2_Z10_MASK_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ZSC_Mask_En:8;
    UINT32                            ZSC_Mask_Val:8;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} L2_Z10_MASK_STRUCT;

#define SMN_L2_Z10_MASK_ADDRESS    0x1570012cUL


/***********************************************************
* Register Name : PPR_CONTROL
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : Minimum time before new interrupt generated for consecutive PPR. Intervals of 50ns. Maximum of PPR_IntReqDelay PPR requests before sending new interrupt is ensured.
#define PPR_CONTROL_PPR_IntTimeDelay_OFFSET      0
#define PPR_CONTROL_PPR_IntTimeDelay_MASK        0xff

// Bitfield Description : Minimum number of PPR requests for new interrupt to be generated.
#define PPR_CONTROL_PPR_IntReqDelay_OFFSET      8
#define PPR_CONTROL_PPR_IntReqDelay_MASK        0xff00

// Bitfield Description : Enables interrupt coallescing. Set to 1 if PPR_IntTimeDelay and PPR_IntReqDelay are not 0.
#define PPR_CONTROL_PPR_IntCoallesce_En_OFFSET      16
#define PPR_CONTROL_PPR_IntCoallesce_En_MASK        0x10000

// Bitfield Description : 
#define PPR_CONTROL_Reserved_31_17_OFFSET      17
#define PPR_CONTROL_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            PPR_IntTimeDelay:8;
    UINT32                            PPR_IntReqDelay:8;
    UINT32                            PPR_IntCoallesce_En:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} PPR_CONTROL_STRUCT;

#define SMN_PPR_CONTROL_ADDRESS    0x13f01248UL

#endif /* _IOMMUL2_H_ */
